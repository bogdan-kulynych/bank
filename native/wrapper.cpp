#include "auth.h"
#include "transactions.h"

#include <node.h>
using namespace v8;


// Throws exception when malformed arguments list
// Returns a string with token if credentials verified or null otherwise
Handle<Value> RequestAuthToken(const Arguments& args) {
    HandleScope scope;

    if (args.Length() < 2) {
        ThrowException(
            Exception::TypeError(String::New("Wrong number of arguments"))
        );
        return scope.Close(Undefined());
    }

    if (!args[0]->IsString() || !args[1]->IsString()) {
        ThrowException(
            Exception::TypeError(String::New("Wrong arguments"))
        );
        return scope.Close(Undefined());
    }

    std::string card_id(*v8::String::Utf8Value(args[0]->ToString()));
    std::string     pin(*v8::String::Utf8Value(args[1]->ToString()));

    Local<String> result;
    try {
        std::string token = auth::issue_token(card_id, pin);
        result = String::New(token.c_str());
    } catch (std::exception& err) {
        //
    }
    return scope.Close(result);
};


Handle<Value> BalanceInquiry(const Arguments& args) {
    HandleScope scope;

    if (args.Length() != 1) {
        ThrowException(
            Exception::TypeError(String::New("Wrong number of arguments"))
        );
        return scope.Close(Undefined());
    }

    if (!args[0]->IsString()) {
        ThrowException(
            Exception::TypeError(String::New("Wrong arguments"))
        );
        return scope.Close(Undefined());
    }

    std::string token(*v8::String::Utf8Value(args[0]->ToString()));

    Local<Object> result = Object::New();
    try {
        transactions::Balance balance = transactions::balance_inquiry(token);
        result->Set(String::NewSymbol("available"),
            Number::New(balance.available));
        result->Set(String::NewSymbol("hold"),
            Number::New(balance.hold));
    } catch (std::exception& err) {
        //
    }
    return scope.Close(result);
};


void InitAll(Handle<Object> exports) {
    exports->Set(String::NewSymbol("requestAuthToken"),
        FunctionTemplate::New(RequestAuthToken)->GetFunction());

    exports->Set(String::NewSymbol("balanceInquiry"),
        FunctionTemplate::New(BalanceInquiry)->GetFunction());
};


NODE_MODULE(bank, InitAll)
