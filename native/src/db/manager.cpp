/**
 * Bank subsystem
 * Author: Eugenia Oshurko
 *
 * Database manager
 */


#include <iostream>
#include <sstream>
#include <exception>

#include "manager.h"
#include "../utils/hash.h"
using namespace db;


db::Manager::Manager():
    database(nullptr)
{
    //
}


db::Manager::~Manager()
{
    closeDB();
}


void db::Manager::createTables() {
    std::string createAccountQuery = "CREATE TABLE IF NOT EXISTS account "
            "(card VARCHAR(16) PRIMARY KEY,"
             "salt VARCHAR(64),"
             "pin_hash VARCHAR(64),"
             "name VARCHAR(70),"
             "available REAL,"
             "hold REAL,"
             "max_sum REAL,"
             "suppl_card VARCHAR(16))";

    sqlite3_stmt * createStmt;
    // std::cout << "Creating Table Statement" << std::endl;
    sqlite3_prepare(database,
                    createAccountQuery.c_str(),
                    createAccountQuery.size(),
                    &createStmt,
                    NULL);
    // std::cout << "Stepping Table Statement" << std::endl;
    if (sqlite3_step(createStmt) != SQLITE_DONE) {
        // std::cout << "Didn't Create Table Account!" << std::endl;
        throw sqlite_error("Didn't create table account");
    }
}

void db::Manager::openDB(const std::string& fname) {
    int ret = 0;
    // open connection to a DB
    if (SQLITE_OK != (ret = sqlite3_open_v2(fname.c_str(), &database, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, NULL)))
    {
        throw sqlite_error("Failed to connect");
    }
}

void db::Manager::closeDB() {
    if (database != 0)
        sqlite3_close(database);
    sqlite3_shutdown();
    database = 0;
}


void db::Manager::insertAccount(const Account& a) {
    std::stringstream strs;
    strs << a.available();
    std::string available = strs.str();

    std::stringstream hstrs;
    hstrs << a.hold();
    std::string hold = hstrs.str();

    std::stringstream mstrs;
    mstrs << a.maxSum();
    std::string max_sum = mstrs.str();

    std::string sup = "NULL";
    if (a.supplemAccount()) {
        sup = a.supplemAccount()->cardNumber();
    }
    std::string insertQuery = std::string("INSERT INTO ACCOUNT "
            "VALUES ( \"") + a.cardNumber() +  std::string("\", \"")
            + a.salt() +  std::string("\", \"")
            + a.pinHash() +  std::string("\", \"")
            + a.name() +  std::string("\", ")
            + available +  std::string(", ")
            + hold +  std::string(", ")
            + max_sum +  std::string(", ");
    if (a.supplemAccount()) {
        insertQuery += "\"" +a.supplemAccount()->cardNumber()  +  std::string("\");");;
    } else {
        insertQuery += "NULL" +  std::string(");");;
    }
    sqlite3_stmt * insertStmt;
    sqlite3_prepare(database,
                    insertQuery.c_str(),
                    insertQuery.size(),
                    &insertStmt,
                    NULL);
    if (sqlite3_step(insertStmt) != SQLITE_DONE)
        throw sqlite_error("Didn't insert into table!");
}

bool db::Manager::existsRequest(const std::string& card) {
    bool res = false;
    std::string selectQuery = std::string("SELECT COUNT(*) FROM ACCOUNT "
                                                 "WHERE card = \"") + card + std::string("\";");
    sqlite3_stmt * getStmt;
    int ret = 0;
    if (SQLITE_OK != (ret = sqlite3_prepare(database,
                    selectQuery.c_str(),
                    selectQuery.size(),
                    &getStmt,
                    NULL))) {
        throw sqlite_error("Failed to prepare select statement");
    }
    if (SQLITE_ROW != (ret = sqlite3_step(getStmt)))
    {
        throw sqlite_error("Failed to step select (exists)");
    }
    std::istringstream hs(reinterpret_cast<const char*>(sqlite3_column_text(getStmt, 0)));
    int count;
    hs >> count;

    if (count != 0) {
        res = true;
    }
    return res;
}

bool db::Manager::authRequest(const std::string& card, const std::string& pin) {
    bool success = false;

    // we get salt from database by card number
    std::string selectSaltQuery = std::string("SELECT salt FROM ACCOUNT "
                                                 "WHERE card = \"") + card + std::string("\";");
    sqlite3_stmt * getSaltStmt;
    int ret = 0;
    if (SQLITE_OK != (ret = sqlite3_prepare(database,
                    selectSaltQuery.c_str(),
                    selectSaltQuery.size(),
                    &getSaltStmt,
                    NULL))) {
        throw sqlite_error("Failed to prepare select statement");
    }
    if (SQLITE_ROW != (ret = sqlite3_step(getSaltStmt)))
    {
        throw sqlite_error("Failed to step select (auth)");
    }
    std::string salt = reinterpret_cast<const char*>(sqlite3_column_text(getSaltStmt, 0));

    // we generate hash
    std::string pin_hash = HashUtils::generateHash(pin, salt);

    // we get hash from db
    std::string selectHashQuery = std::string("SELECT pin_hash FROM ACCOUNT "
                                                 "WHERE card = \"") + card + std::string("\";");
    sqlite3_stmt * getHashStmt;
    if (SQLITE_OK != (ret = sqlite3_prepare(database,
                    selectHashQuery.c_str(),
                    selectHashQuery.size(),
                    &getHashStmt,
                    NULL))) {
        throw sqlite_error("Failed to prepare select statement");
    }
    if (SQLITE_ROW != (ret = sqlite3_step(getHashStmt)))
    {
        throw sqlite_error("Failed to step select (auth1)");
    }
    std::string hash_from_db = reinterpret_cast<const char*>(sqlite3_column_text(getHashStmt, 0));

    // and compare them
    if (pin_hash == hash_from_db) {
        success = true;
    }

    return success;
}

std::string db::Manager::nameRequest(const std::string& card) {

    std::string name = "";

    std::string selectNameQuery = std::string("SELECT name FROM ACCOUNT "
                                                 "WHERE card = \"") + card + std::string("\";");
    sqlite3_stmt * getNameStmt;
    int ret = 0;
    if (SQLITE_OK != (ret = sqlite3_prepare(database,
                    selectNameQuery.c_str(),
                    selectNameQuery.size(),
                    &getNameStmt,
                    NULL))) {
        throw sqlite_error("Failed to prepare select statement");
    }
    if (SQLITE_ROW != (ret = sqlite3_step(getNameStmt)))
    {
        throw sqlite_error("Failed to step select (name)");
    }
    name = reinterpret_cast<const char*>(sqlite3_column_text(getNameStmt, 0));
    return name;
}


double db::Manager::availableRequest(const std::string& card) {
    std::string selectAvailableQuery = std::string("SELECT available FROM ACCOUNT "
                                                 "WHERE card = \"") + card + std::string("\";");
    sqlite3_stmt * getAvailableStmt;
    int ret = 0;
    if (SQLITE_OK != (ret = sqlite3_prepare(database,
                    selectAvailableQuery.c_str(),
                    selectAvailableQuery.size(),
                    &getAvailableStmt,
                    NULL))) {
        throw sqlite_error("Failed to prepare select statement");
    }
    if (SQLITE_ROW != (ret = sqlite3_step(getAvailableStmt)))
    {
        throw sqlite_error("Failed to step select (available)");
    }

    std::istringstream as(reinterpret_cast<const char*>(sqlite3_column_text(getAvailableStmt, 0)));
    double available;
    as >> available;
    return available;
}


double db::Manager::holdRequest(const std::string& card) {
    std::string selectHoldQuery = std::string("SELECT hold FROM ACCOUNT "
                                                 "WHERE card = \"") + card + std::string("\";");
    sqlite3_stmt * getHoldStmt;
    int ret = 0;
    if (SQLITE_OK != (ret = sqlite3_prepare(database,
                    selectHoldQuery.c_str(),
                    selectHoldQuery.size(),
                    &getHoldStmt,
                    NULL))) {
        throw sqlite_error("Failed to prepare select statement");
    }
    if (SQLITE_ROW != (ret = sqlite3_step(getHoldStmt)))
    {
        throw sqlite_error("Failed to step select (hold)");
    }
    std::istringstream hs(reinterpret_cast<const char*>(sqlite3_column_text(getHoldStmt, 0)));
    double hold;
    hs >> hold;
    return hold;
}


double db::Manager::maxSumRequest(const std::string& card) {
    std::string selectHoldQuery = std::string("SELECT max_sum FROM ACCOUNT "
                                                 "WHERE card = \"") + card + std::string("\";");
    sqlite3_stmt * getHoldStmt;
    int ret = 0;
    if (SQLITE_OK != (ret = sqlite3_prepare(database,
                    selectHoldQuery.c_str(),
                    selectHoldQuery.size(),
                    &getHoldStmt,
                    NULL))) {
        throw sqlite_error("Failed to prepare select statement");
    }
    if (SQLITE_ROW != (ret = sqlite3_step(getHoldStmt)))
    {
        throw sqlite_error("Failed to step select (maxsum)");
    }
    std::istringstream hs(reinterpret_cast<const char*>(sqlite3_column_text(getHoldStmt, 0)));
    double max_sum;
    hs >> max_sum;
    return max_sum;
}

std::string db::Manager::supplementaryRequest(const std::string& card) {
    std::string suppl_card = "";

    std::string selectNameQuery = std::string("SELECT suppl_card FROM ACCOUNT "
                                                 "WHERE card = \"") + card + std::string("\";");
    sqlite3_stmt * getNameStmt;
    int ret = 0;
    if (SQLITE_OK != (ret = sqlite3_prepare(database,
                    selectNameQuery.c_str(),
                    selectNameQuery.size(),
                    &getNameStmt,
                    NULL))) {
        throw sqlite_error("Failed to prepare select statement");
    }
    if (SQLITE_ROW != (ret = sqlite3_step(getNameStmt)))
    {
        throw sqlite_error("Failed to step select (suppl)");
    }
    if (NULL != sqlite3_column_text(getNameStmt, 0)) {
        suppl_card = reinterpret_cast<const char*>(sqlite3_column_text(getNameStmt, 0));
    } else {
        suppl_card = "";
    }

    return suppl_card;
}


void db::Manager::changeAvailable(const std::string& card, double sum) {
    std::stringstream strs;
    strs << sum;
    std::string available = strs.str();

    std::string setQuery = std::string("UPDATE ACCOUNT "
                                                "SET available=") + available +
                                   std::string(" WHERE card = \"") + card + std::string("\";");
    sqlite3_stmt * setStmt;
    sqlite3_prepare(database,
                    setQuery.c_str(),
                    setQuery.size(),
                    &setStmt,
                    NULL);
    if (sqlite3_step(setStmt) != SQLITE_DONE)
        throw sqlite_error("Didn't update table!");
}


void db::Manager::changeHold(const std::string& card, double sum) {
    std::stringstream strs;
    strs << sum;
    std::string hold = strs.str();

    std::string setQuery = std::string("UPDATE ACCOUNT "
                                                "SET hold=") + hold +
                                   std::string(" WHERE card = \"") + card + std::string("\";");
    sqlite3_stmt * setStmt;
    sqlite3_prepare(database,
                    setQuery.c_str(),
                    setQuery.size(),
                    &setStmt,
                    NULL);
    if (sqlite3_step(setStmt) != SQLITE_DONE)
        throw sqlite_error("Didn't update table");
}


void db::Manager::changeMaxSum(const std::string& card, double sum) {
    std::stringstream strs;
    strs << sum;
    std::string max_sum = strs.str();

    std::string setQuery = std::string("UPDATE ACCOUNT "
                                                "SET max_sum=") + max_sum +
                                   std::string(" WHERE card = \"") + card + std::string("\";");
    sqlite3_stmt * setStmt;
    sqlite3_prepare(database,
                    setQuery.c_str(),
                    setQuery.size(),
                    &setStmt,
                    NULL);
    if (sqlite3_step(setStmt) != SQLITE_DONE)
        throw sqlite_error("Didn't update table!");
}

void db::Manager::changeSupplCard(const std::string& card, const std::string& suppl_card) {
    std::string setQuery;
    if (suppl_card == "") {
        setQuery = std::string("UPDATE ACCOUNT "
                                                    "SET suppl_card=NULL "
                                                    "WHERE card = \"") + card + std::string("\";");
    } else {
        setQuery = std::string("UPDATE ACCOUNT "
                                                    "SET suppl_card=") + suppl_card +
                                       std::string(" WHERE card = \"") + card + std::string("\";");
    }

    sqlite3_stmt * setStmt;
    sqlite3_prepare(database,
                    setQuery.c_str(),
                    setQuery.size(),
                    &setStmt,
                    NULL);
    if (sqlite3_step(setStmt) != SQLITE_DONE)
        throw sqlite_error("Didn't update table!");    
}

