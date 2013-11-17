/**
 * Bank subsystem
 * Author: Bogdan Kulynych
 *
 * Demo
 *
 */


#include "auth.h"
#include "transactions.h"

#include <iostream>
using namespace std;


void verify(string& s) {
    try {
        auth::process_token(s);
        cout << "Token OK" << endl;
    } catch (exception& err) {
        cout << "Token NOT OK: " << err.what() << endl;
    }
}


void balance(string& s) {
    try {
        transactions::Balance balance = transactions::balance_inquiry(s);
        cout << "Balance: " << balance.available << '/' << balance.hold << endl;
    } catch (exception& err) {
        cout << "Balance ERR: " << err.what() << endl;
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

    string fake_token1 = "asdasd01239123askdaskdasd";
    string fake_token2 = "asdasd:asd111asdas:122222222222";
    string fake_token3 = "123123:123123123:asdasdasdasdasd";
    verify(fake_token1);
    balance(fake_token1);
    verify(fake_token2);
    balance(fake_token2);
    verify(fake_token3);
    balance(fake_token3);


    return 0;
}
