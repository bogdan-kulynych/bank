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


void set_overflow_recepient(const string& token, const string& recepient_id) {
    try {
        ops::set_overflow_recepient(token, recepient_id);
        cout << "Set Overflow Recepient OK" << endl;
    } catch (exception& err) {
        cout << "Set Overflow Recepient ERR: " << err.what() << endl;
    }   
}


void set_overflow_threshold(const string& token, const double threshold) {
    try {
        ops::set_overflow_threshold(token, threshold);
        cout << "Set Overflow Threshold OK" << endl;
    } catch (exception& err) {
        cout << "Set Overflow Threshold ERR: " << err.what() << endl;
    }   
}


void get_overflow_threshold(const string& token) {
    try {
        double res = ops::get_overflow_threshold(token);
        cout << "Get Overflow Threshold OK: " << res << endl;
    } catch (exception& err) {
        cout << "Get Overflow Threshold ERR: " << err.what() << endl;
    }   
}


void get_overflow_recepient(const string& token) {
    try {
        string res = ops::get_overflow_recepient(token);
        cout << "Get Overflow Recepient OK: " << res << endl;
    } catch (exception& err) {
        cout << "Get Overflow Recepient ERR: " << err.what() << endl;
    }   
}


int main()
{
    string card_id = "1111111111111111";
    string card_id1 = "2222222222222222";
    string card_id2 = "3333333333333333";
    string pin = "1111";
    string pin1 = "2222";
    string pin2 = "3333";

    auto s = auth::issue_token(card_id, pin);
    cout << s << endl;
    verify(s);

    balance(s);

    auto q = auth::issue_token(card_id1, pin1);
    auto r = auth::issue_token(card_id2, pin2);

    set_overflow_recepient(q, "3333333333333333");
    set_overflow_threshold(q, 400);
    get_overflow_recepient(q);
    get_overflow_threshold(q);

    transfer(s, "2222222222222222", 100);
    balance(s);
    balance(q);
    balance(r);

    transfer(s, "2222222222222222", -100);
    transfer(s, "2222222222222222", 0);
    transfer(s, "2222222222222222", 100000000000);

    // withdraw(s, 50);
    withdraw(s, 1000000000000);

    get_name(s, card_id);

    // Test if fake tokens fail
    cout << endl << "Fake tokens" << endl << endl;

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
