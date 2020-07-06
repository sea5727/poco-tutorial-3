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
    session << "CREATE TABLE Person (Name VARCHAR(30), Address VARCHAR, Age INTEGER(3), Flags INTEGER(0))", now;

    Person person = 
    {
        "Bart Simpson",
        "Springfield",
        15,
        0
    };

    for(int i = 0 ; i < 100 ; i++)
    {
        // insert some rows
      

        Statement insert(session);
        insert << "INSERT INTO Person VALUES(?, ?, ?, ?)",
            use(person.name),
            use(person.address),
            use(i),
            use(person.flags);

        insert.execute();
    }

    // a simple query

    Statement select(session);
    select << "SELECT Name, Address, Age, Flags FROM Person",
        into(person.name),
        into(person.address),
        into(person.age),
        into(person.flags),
        range(0, 1); //  iterate over result set one row at a time
        
    while (!select.done())
    {
        size_t num = select.execute();
        if(num > 0)
        {
            std::cout << person.name << " " << person.address << " " << person.age << " " << person.flags << std::endl;
        }
    }



    std::cout << "end" << std::endl;
    return 0;
}