#ifndef __SQLITE_ACCEPTOR_HPP__
#define __SQLITE_ACCEPTOR_HPP__

#include "inc.hpp"

using namespace Poco::Data::Keywords;
using Poco::Data::Session;
using Poco::Data::Statement;

struct MyPerson
{
    std::string name;
    std::string address;
    int         age;
    int         flags;
};


class SQLiteAcceptor
{
public:
    SQLiteAcceptor()
        : _if_services(new io_service_pool(8))
    {
        std::cout << "SQLiteAcceptor" << std::endl;
    }
    ~SQLiteAcceptor()
    {
        std::cout << "SQLiteAcceptor" << std::endl;
    }
    void start(std::string db_name) // sample.db
    {
        Poco::Data::SQLite::Connector::registerConnector();

        std::shared_ptr<Poco::Data::Session> session = std::make_shared<Poco::Data::Session>("SQLite", db_name);

        _if_services->run();
        _accept_threads.emplace_back([=] {
            handle_new_data(session);
        });
    }
    void handle_new_data(std::shared_ptr<Poco::Data::Session> session)
    {

        while(1)
        {
            Poco::Data::Statement select(*session);
            MyPerson person;
            select << "SELECT Name, Address, Age, Flags FROM Person WHERE Flags != 1",
                into(person.name),
                into(person.address),
                into(person.age),
                into(person.flags),
                range(0, 1);
            
            while(!select.done())
            {
                size_t num = select.execute();
                if(num > 0)
                {
                    
                    person.flags = 1;
                    Statement update(*session);
                    update << "UPDATE Person SET Flags = 1 WHERE Name=? and Address=? and age=?",
                        use(person.name),
                        use(person.address),
                        use(person.age);
                    update.execute();
                    
                    // 이 데이터를 worker 줘야하는데??
                    boost::asio::io_service &io = _if_services->get_io_service();
                    io.post([person]{
                        std::cout << "[thread:" << std::this_thread::get_id() << "] .. " << person.name << " " << person.address << " " << person.age << " " << person.flags << std::endl;
                    });
                }
            }
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }

    }
private:
    std::vector<std::thread> _accept_threads;
    std::shared_ptr<io_service_pool> _if_services;
};

#endif