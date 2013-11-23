#include "auth.h"
#include "db.h"
#include "ops.h"

#include <node.h>
using namespace v8;


#define THROW_HTTP_EXCEPTION(code, message) \
{ \
    Handle<Object> result = Object::New(); \
    result->Set(String::NewSymbol("code"), Number::New(code)); \
    result->Set(String::NewSymbol("message"), String::New(message)); \
    ThrowException(result); \
}


// Returns a string with token if credentials are valid, null otherwise
// Throws exception when argument list malformed
Handle<Value> AuthTokenRequest(const Arguments& args) {
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
        THROW_HTTP_EXCEPTION(401, err.what());
    }

    return scope.Close(result);
};


// Returns an object with funds if token is valid, empty object otherwise
// Throws exception when argument list malformed
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
        ops::Balance balance = ops::balance_inquiry(token);
        result->Set(String::NewSymbol("available"),
            Number::New(balance.available));
        result->Set(String::NewSymbol("hold"),
            Number::New(balance.hold));

    } catch (auth::exception& err) {
        THROW_HTTP_EXCEPTION(401, err.what());
    } catch (db::exception& err) {
        THROW_HTTP_EXCEPTION(400, err.what());
    }

    return scope.Close(result);
};


// Attempts to withdraw funds, if not sufficient funds, etc. throws exception
// Throws exception when argument list malformed
Handle<Value> WithdrawalRequest(const Arguments& args) {
    HandleScope scope;

    if (args.Length() != 2) {
        ThrowException(
            Exception::TypeError(String::New("Wrong number of arguments"))
        );
        return scope.Close(Undefined());
    }

    if (!args[0]->IsString() || !args[1]->IsString())
    {
        ThrowException(
            Exception::TypeError(String::New("Wrong arguments"))
        );
        return scope.Close(Undefined());
    }

    std::string token(*v8::String::Utf8Value(args[0]->ToString()));
    double amount = args[1]->NumberValue();

    try {
        ops::withdrawal_request(token, amount);
    } catch (auth::exception& err) {
        THROW_HTTP_EXCEPTION(401, err.what());
    } catch (db::insufficient_funds& err) {
        THROW_HTTP_EXCEPTION(403, err.what());
    } catch (db::exception& err) {
        THROW_HTTP_EXCEPTION(400, err.what());
    }

    return scope.Close(Undefined());
};


// Attempts to make a transfer, if not sufficient funds, etc. throws exception
// Throws exception when argument list malformed
Handle<Value> TransferRequest(const Arguments& args) {
    HandleScope scope;

    if (args.Length() != 3) {
        ThrowException(
            Exception::TypeError(String::New("Wrong number of arguments"))
        );
        return scope.Close(Undefined());
    }

    if (!args[0]->IsString() || !args[1]->IsString() || !args[2]->IsNumber())
    {
        ThrowException(
            Exception::TypeError(String::New("Wrong arguments"))
        );
        return scope.Close(Undefined());
    }

    std::string token(*v8::String::Utf8Value(args[0]->ToString()));
    std::string recepient_id(*v8::String::Utf8Value(args[1]->ToString()));
    double amount = args[2]->NumberValue();

    try {
        ops::transfer_request(token, recepient_id, amount);
    } catch (auth::exception& err) {
        THROW_HTTP_EXCEPTION(401, err.what());
    } catch (db::insufficient_funds& err) {
        THROW_HTTP_EXCEPTION(403, err.what());
    } catch (db::exception& err) {
        THROW_HTTP_EXCEPTION(400, err.what());
    }

    return scope.Close(Undefined());
};


// Returns name of cardholder, throws exception if anything is wrong
// Throws exception when argument list malformed
Handle<Value> NameRequest(const Arguments& args) {
    HandleScope scope;

    if (args.Length() != 2) {
        ThrowException(
            Exception::TypeError(String::New("Wrong number of arguments"))
        );
        return scope.Close(Undefined());
    }

    if (!args[0]->IsString() || !args[1]->IsString())
    {
        ThrowException(
            Exception::TypeError(String::New("Wrong arguments"))
        );
        return scope.Close(Undefined());
    }

    std::string   token(*v8::String::Utf8Value(args[0]->ToString()));
    std::string card_id(*v8::String::Utf8Value(args[1]->ToString()));

    Local<String> result;
    try {
        std::string name = ops::name_request(token, card_id);
        result = String::New(name.c_str());
    } catch (auth::exception& err) {
        THROW_HTTP_EXCEPTION(401, err.what());
    } catch (db::exception& err) {
        THROW_HTTP_EXCEPTION(400, err.what());
    }

    return scope.Close(result);
};


void InitAll(Handle<Object> exports) {
    exports->Set(String::NewSymbol("authTokenRequest"),
        FunctionTemplate::New(AuthTokenRequest)->GetFunction());

    exports->Set(String::NewSymbol("balanceInquiry"),
        FunctionTemplate::New(BalanceInquiry)->GetFunction());

    exports->Set(String::NewSymbol("withdrawalRequest"),
        FunctionTemplate::New(WithdrawalRequest)->GetFunction());

    exports->Set(String::NewSymbol("transferRequest"),
        FunctionTemplate::New(TransferRequest)->GetFunction());

    exports->Set(String::NewSymbol("nameRequest"),
        FunctionTemplate::New(NameRequest)->GetFunction());
};



NODE_MODULE(bank, InitAll)
