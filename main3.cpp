#include "Poco/Data/Session.h"
#include "Poco/Data/SQLite/Connector.h"
#include <vector>
#include <iostream>
#include <iostream>
#include <chrono>
#include <thread>

using namespace Poco::Data::Keywords;
using Poco::Data::Session;
using Poco::Data::Statement;

struct Person
{
    std::string name;
    std::string address;
    int         age;
    int         flags;
};


int main(int argc, char** argv)
{
    // register SQLite connector
    Poco::Data::SQLite::Connector::registerConnector();

    // create a session
    Session session("SQLite", "sample.db");

    // drop sample table, if it exists
    session << "DROP TABLE IF EXISTS Person", now;

    // (re)create table
    session << "CREATE TABLE Person (Name VARCHAR(30), Address VARCHAR, Age INTEGER(3), Flags INTEGER(1))", now;

    // insert some rows
    Person person = 
    {
        "Bart Simpson",
        "Springfield",
        12,
        0
    };

    Statement insert(session);
    insert << "INSERT INTO Person VALUES(?, ?, ?, ?)",
        use(person.name),
        use(person.address),
        use(person.age),
        use(person.flags);

    insert.execute();

    person.name    = "Lisa Simpson";
    person.address = "Springfield";
    person.age     = 10;
    person.flags   = 0;

    insert.execute();

    // a simple query

    

    Statement select(session);
    select << "SELECT Name, Address, Age, Flags FROM Person WHERE Flags != 1",
        into(person.name),
        into(person.address),
        into(person.age),
        into(person.flags),
        range(0, 1); //  iterate over result set one row at a time
        

    Statement::Result result = select.executeAsync();
    
    std::cout << "select is async?? " << select.isAsync() << std::endl;

    bool try_wait = false;
    while(1)
    {
        try_wait = result.tryWait(100);
        if(try_wait)
        {
            std::cout << "try_wait : " << try_wait << std::endl;
            auto data = result.data();
            std::cout << "data : " << data << std::endl;
            if(data > 0)
            {
                std::cout << person.name << " " << person.address << " " << person.age << " " << person.flags << std::endl;
            }
            break;
        }
        std::cout << "try_wait : " << try_wait << std::endl;
    }
    


    ///////////////////////////////

    // result.wait();

    // auto data = result.data();
    // std::cout << "data : " << data << std::endl;
    // if(data > 0)
    // {
    //     std::cout << person.name << " " << person.address << " " << person.age << " " << person.flags << std::endl;
    // }

    ///////////////////////////////    





    std::cout << "end" << std::endl;
    return 0;
}