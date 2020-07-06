#include "inc.hpp"

// using namespace Poco::Data::Keywords;
// using Poco::Data::Session;
// using Poco::Data::Statement;


struct Person
{
    std::string name;
    std::string address;
    int         age;
    int         flags;
};


int main(int argc, char** argv)
{
    SQLiteAcceptor sqlac;
    sqlac.start("sample.db");

    while(1)
    {
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    return 0;
}