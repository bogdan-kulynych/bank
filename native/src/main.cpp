/**
 * Bank subsystem
 * Author: Bogdan Kulynych
 *
 * Demo
 *
 */


#include "auth.h"
#include "ops.h"

#include <iostream>
using namespace std;


void verify(const string& token) {
    try {
        auth::process_token(token);
        cout << "Token OK" << endl;
    } catch (exception& err) {
        cout << "Token NOT OK: " << err.what() << endl;
    }
}


void balance(const string& token) {
    try {
        ops::Balance balance = ops::balance_inquiry(token);
        cout << "Balance: " << balance.available << '/' << balance.hold << endl;
    } catch (exception& err) {
        cout << "Balance ERR: " << err.what() << endl;
    }
}


void withdraw(const string& token, double amount) {
    try {
        ops::withdrawal_request(token, amount);
        cout << "Withdrawal successful" << endl;
    } catch (exception& err) {
        cout << "Withdrawal ERR: " << err.what() << endl;
    }
}


void transfer(const string& token, const string& recepient, double amount) {
    try {
        ops::transfer_request(token, recepient, amount);
        cout << "Transfer successful" << endl;
    } catch (exception& err) {
        cout << "Transfer ERR: " << err.what() << endl;
    }
}


void get_name(const string& token, const string& card_id) {
    try {
        string name = ops::name_request(token, card_id);
        cout << "Name: " << name << endl;
    } catch (exception& err) {
        cout << "Name ERR: " << err.what() << endl;
    }
}


int main()
{
    string card_id = "1111111111111111";
    string pin = "1111";

    auto s = auth::issue_token(card_id, pin);
    cout << s << endl;
    verify(s);
    balance(s);

    transfer(s, "1111111111", 100);
    transfer(s, "1111111111", -100);
    transfer(s, "1111111111", 0);
    transfer(s, "1111111111", 100000000000);

    withdraw(s, 100);
    withdraw(s, 1000000000000);

    get_name(s, "1111111111111111");


    // Test if fake tokens fail


    string fake_token1 = "asdasd01239123askdaskdasd";
    string fake_token2 = "asdasd:asd111asdas:122222222222";
    string fake_token3 = "123123:123123123:asdasdasdasdasd";

    verify(fake_token1);
    balance(fake_token1);
    withdraw(fake_token1, 123123);
    get_name(fake_token1, "1111111111111111");
    transfer(fake_token1, "111111111", 123123);

    verify(fake_token2);
    balance(fake_token2);
    withdraw(fake_token2, 123123);
    transfer(fake_token2, "111111111", 123123);
    get_name(fake_token2, "1111111111111111");

    verify(fake_token3);
    balance(fake_token3);
    transfer(fake_token3, "111111111", 123123);
    withdraw(fake_token3, 123123);
    get_name(fake_token3, "1111111111111111");

    return 0;
}
