/**
 * Bank subsystem
 * Author: Eugenia Oshurko
 *
 * Database manager
 */

#pragma once
#include <string>
#include <exception>

#include "sqlite3.h"

#include "account.h"


namespace db
{
	class Manager
	{
	public:
		Manager();
		~Manager();
		void createTables();
		void openDB(const std::string&);
		void closeDB();

		void insertAccount(const db::Account& a);
		bool existsRequest(const std::string& card);
		bool authRequest(const std::string& card, const std::string& pin);
		std::string nameRequest(const std::string& card);
		double availableRequest(const std::string& card);
		double holdRequest(const std::string& card);
		double maxSumRequest(const std::string& card);
		std::string supplementaryRequest(const std::string& card);
		void changeAvailable(const std::string& card, double sum);
		void changeHold(const std::string& card, double sum);
		void changeMaxSum(const std::string& card, double sum);
		void changeSupplCard(const std::string& card, const std::string& suppl_card);

	private:
		sqlite3 *database;
	};

	struct sqlite_error : std::exception
	{
		const char* _msg;

		sqlite_error(const char* msg="Database failure"): _msg(msg) {};

		virtual const char* what() const noexcept {
			return _msg;
		}
	};
}