#include "auth.h"

#include <iostream>
using namespace std;


int main()
{
    std::string card_id = "1111111111111111";
    std::string pin = "1111";

    auto *s = auth::issue_token(card_id, pin);
    cout << *s << endl;

    cout << boolalpha << auth::verify_token(*s) << endl;

    delete s;

    return 0;
}
