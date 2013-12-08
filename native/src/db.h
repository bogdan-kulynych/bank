/**
 * Bank subsystem
 * Author: Bogdan Kulynych
 *
 * Database calls
 *
 */


#pragma once

#include <string>
#include <exception>


namespace db
{
    bool
    verify_credentials(const std::string& card_id, const std::string& pin);

    void
    withdraw(const std::string& card_id,
        const double amount);

    void
    make_transfer(const std::string& sender_id,
        const std::string& recepient,
        const double amount);

    bool
    exists(const std::string& card_id);

    std::string
    get_name(const std::string& card_id);

    double
    get_available_funds(const std::string& card_id);

    double
    get_on_hold_funds(const std::string& card_id);

    double
    get_overflow_threshold(const std::string card_id);

    std::string
    get_overflow_recepient(const std::string card_id);

    void
    set_overflow_threshold(const std::string card_id, const double threshold);

    void
    set_overflow_recepient(const std::string card_id, const std::string recepient_id);

    // Exceptions

    struct exception: std::exception {};

    struct database_error : exception 
    {
        const char* _msg;

        database_error(const char* msg): _msg(msg) {};

        virtual const char* what() const noexcept {
            return _msg;
        }   
    };

    struct insufficient_funds : exception
    {
        virtual const char* what() const noexcept {
            return "Insufficient funds";
        }
    };

    struct recepient_does_not_exist : exception
    {
        virtual const char* what() const noexcept {
            return "Recepient does not exist";
        }
    };

    struct invalid_amount : exception
    {
        virtual const char* what() const noexcept {
            return "Invalid amount";
        }
    };

    struct card_does_not_exist : exception
    {
        virtual const char* what() const noexcept {
            return "Card does not exist";
        }
    };

    struct conflict_supplementary_account : exception
    {
        virtual const char* what() const noexcept {
            return "Adding supplementary account creates conflict situation";
        }
    };
}
