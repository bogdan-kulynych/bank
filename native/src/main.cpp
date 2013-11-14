#include "auth.h"

#include <iostream>
#include <memory>
using namespace std;


int main()
{
    std::string card_id = "1111111111111";
    std::string pin = "1111";

    auto *s = auth::issue_token(card_id, pin);
    cout << *s << endl;
    delete s;

    return 0;
}
