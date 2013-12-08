/**
 * Bank subsystem
 * Author: Eugenia Oshurko
 *
 * Hash utilities
 */
 

#include <iostream>
using namespace std;

#include "manager.h"
using namespace db;

int main()
{
    Manager m;
    m.openDB("dev.db");
    m.createTables();
    Account a1("1111111111111111",
               "1111",
               "George Washington",
               1000.0,
               200.0);

    Account a2("2222222222222222",
               "2222",
               "Bogdan Kulynych",
               200.0,
               50.0);

    Account a3("3333333333333333",
               "3333",
               "Jon Snow",
               10000.0);

    m.insertAccount(a1);
    m.insertAccount(a2);
    m.insertAccount(a3);
    std::cout << "Exists:" << std::boolalpha << m.existsRequest("3434") << std::endl;
    std::cout << "Exists:" << std::boolalpha << m.existsRequest("1111111111111111") << std::endl;
    std::cout << std::boolalpha << m.authRequest("1111111111111111", "1111") << std::endl;
    std::cout << std::boolalpha << m.authRequest("1111111111111111", "1311") << std::endl;
    std::cout << "Name: " << m.nameRequest("1111111111111111") << std::endl;
    std::cout << "Available: " << m.availableRequest("1111111111111111") << std::endl;
    std::cout << "Hold: " << m.holdRequest("1111111111111111") << std::endl;
    m.changeAvailable("2222222222222222", 340);
    m.changeHold("2222222222222222", 0);
    if (m.supplementaryRequest("1111111111111111") == "") {
        std::cout << "Suppl card: is empty" << std::endl;
    } else {
        std::cout << "Suppl card: " << m.supplementaryRequest("1111111111111111") << std::endl;
    }
    m.changeSupplCard("1111111111111111", "2222222222222222");
    m.changeMaxSum("1111111111111111", 300);
    if (m.supplementaryRequest("1111111111111111") == "") {
        std::cout << "Suppl card: is empty" << std::endl;
    } else {
        std::cout << "Suppl card: " << m.supplementaryRequest("1111111111111111") << std::endl;
        std::cout << "Max sum: " << m.maxSumRequest("1111111111111111") << std::endl;
    }
    m.changeMaxSum("2222222222222222", 10050);
    m.changeSupplCard("2222222222222222", "3333333333333333");
    if (m.supplementaryRequest("1111111111111111") == "") {
        std::cout << "Suppl card: is empty" << std::endl;
    } else {
        std::cout << "Suppl card: " << m.supplementaryRequest("1111111111111111") << std::endl;
        std::cout << "Max sum: " << m.maxSumRequest("1111111111111111") << std::endl;
    }
    return 0;
}

