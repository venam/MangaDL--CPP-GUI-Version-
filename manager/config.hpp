/*
 *
 * COPYRIGHT AND PERMISSION NOTICE

Copyright (c) 2013, Patrick Louis <patrick at unixhub.net>

All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

    1.  The author is informed of the use of his/her code. The author does not have to consent to the use; however he/she must be informed.
    2.  If the author wishes to know when his/her code is being used, it the duty of the author to provide a current email address at the top of his/her code, above or included in the copyright statement.
    3.  The author can opt out of being contacted, by not providing a form of contact in the copyright statement.
    4.  If any portion of the author's code is used, credit must be given.
            a. For example, if the author's code is being modified and/or redistributed in the form of a closed-source binary program, then the end user must still be made somehow aware that the author's work has contributed to that program.
            b. If the code is being modified and/or redistributed in the form of code to be compiled, then the author's name in the copyright statement is sufficient.
    5.  The following copyright statement must be included at the beginning of the code, regardless of binary form or source code form.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

Except as contained in this notice, the name of a copyright holder shall not
be used in advertising or otherwise to promote the sale, use or other dealings
in this Software without prior written authorization of the copyright holder.

 * Manages the configs in the sqlite3 DB
 *
 * CREATE TABLE location ( mg_location VARCHAR);
 *
 * CREATE TABLE mg_list (mgname VARCHAR, mgchapter VARCHAR, mglastdate VARCHAR);
 *
 * CREATE TABLE dl_managers (name VARCHAR UNIQUE PRIMARY KEY,command VARCHAR UNIQUE, selected BOOL);
 *
 * TODO:
 *     Might add the providers in the DB instead of hard coding it inside the GUI
 *     Create a language for the external download manager.
 *         Ex: %o FILE OUTPUT, %f URL
 *
 */
#include <stdio.h>
#include <sqlite3.h>
#include <string>
#include <string.h>
#include <vector>
#include <iostream>

#define DB_NAME "manager/config.db"
#define DEFAULT_LOC ""

struct mg_list {
	std::string mgname;
	std::string mgchapter;
	std::string mglastdate;
};

struct dl_mngr {
	std::string name;
	std::string command;
	bool selected;
};

class db_manager {
	public:
		void exec(std::string command);
		std::string get_mg_location();
		void update_mg_location(const std::string new_location);
		std::vector <dl_mngr> get_dl_managers();
		std::vector <mg_list> get_mg_list();
		void add_dl_manager(const std::string name, const std::string command, const bool selected);
		void update_dl_manager(const std::vector <dl_mngr> new_managers);
		void remove_dl_manager(const std::string name);
		db_manager();
		~db_manager();
	protected:
	private:
		sqlite3 *db;
		char *zErrMsg;
		int rc;
		static int exec_callback(void *NotUsed, int argc, char **argv, char **azColName);
};

db_manager::db_manager()
{
	zErrMsg     = 0;
	rc          = sqlite3_open(DB_NAME, &db);
	if( rc ){
		fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
		sqlite3_close(db);
	}
}

db_manager::~db_manager()
{
	sqlite3_close(db);
}

int
db_manager::exec_callback(void *NotUsed, int argc, char **argv, char **azColName)
{
	for(int i=0; i<argc; i+=2){
		printf("%s = %s", azColName[i], argv[i] ? argv[i] : "NULL");
		printf("  %s = %s\n", azColName[i+1], argv[i+1] ? argv[i+1] : "NULL");
	}
	printf("\n");
	return 0;
}

std::string
db_manager::get_mg_location()
{
	std::string response = "";
	char **pazResult;
	int row, column;

	rc = sqlite3_get_table(
		db,          /* An open database */
		"select * from location",     /* SQL to be evaluated */
		&pazResult,    /* Results of the query */
		&row,           /* Number of result rows written here */
		&column,        /* Number of result columns written here */
		&zErrMsg       /* Error msg written here */
	);

	if ( rc!=SQLITE_OK ) {
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
		zErrMsg = 0;
	}
	else {
		if (pazResult[1])
			response = pazResult[1];
		sqlite3_free_table(pazResult);
	}
	return response;
}

std::vector <dl_mngr>
db_manager::get_dl_managers()
{
	std::vector <dl_mngr> container;
	dl_mngr temp_list;
	char **pazResult;
	int row, column;

	rc = sqlite3_get_table(
		db,          /* An open database */
		"select * from dl_managers;",     /* SQL to be evaluated */
		&pazResult,    /* Results of the query */
		&row,           /* Number of result rows written here */
		&column,        /* Number of result columns written here */
		&zErrMsg       /* Error msg written here */
	);

	if ( rc!=SQLITE_OK ) {
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
		zErrMsg = 0;
	}
	else {
		for (int i = 0, k = 3; i<row; i++, k+=3) {
			if (pazResult[i+k]) temp_list.name       = pazResult[i+k];
			else temp_list.name     = "";
			if (pazResult[i+k+1]) temp_list.command  = pazResult[i+k+1];
			else  temp_list.command = "";
			if (pazResult[i+k+2]) {
				if (strcmp(pazResult[i+k+2], "1") == 0)
					 temp_list.selected = true;
				else temp_list.selected = false;
			}
			else temp_list.selected = false;
			if (temp_list.name!="") container.push_back(temp_list);
			k--;
		}
		sqlite3_free_table(pazResult);
	}
	return container;

}

void
db_manager::add_dl_manager(const std::string name, const std::string command, const bool selected)
{
	if (selected)
	exec("INSERT INTO dl_managers VALUES ("+name+","+command+",1)");
	else
	exec("INSERT INTO dl_managers VALUES ("+name+","+command+",0)");
}

void
db_manager::remove_dl_manager(const std::string given_name)
{
	exec("DELETE FROM dl_managers WHERE name=\""+given_name+"\";");
}

std::vector <mg_list>
db_manager::get_mg_list()
{
	std::vector <mg_list> container;
	mg_list temp_list;
	char **pazResult;
	int row, column;

	rc = sqlite3_get_table(
		db,          /* An open database */
		"select * from mg_list;",     /* SQL to be evaluated */
		&pazResult,    /* Results of the query */
		&row,           /* Number of result rows written here */
		&column,        /* Number of result columns written here */
		&zErrMsg       /* Error msg written here */
	);

	if ( rc!=SQLITE_OK ) {
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
		zErrMsg = 0;
	}
	else {
		for (int i = 0, k = 3; i<row; i++, k+=3) {
			if (pazResult[i+k]) temp_list.mgname       = pazResult[i+k];
			else temp_list.mgname     = "";
			if (pazResult[i+k+1]) temp_list.mgchapter  = pazResult[i+k+1];
			else  temp_list.mgchapter = "";
			if (pazResult[i+k+2]) temp_list.mglastdate = pazResult[i+k+2];
			else temp_list.mglastdate = "";
			if (temp_list.mgname!="") container.push_back(temp_list);
			k--;
		}
		sqlite3_free_table(pazResult);
	}
	return container;
}

void
db_manager::update_mg_location(const std::string new_location)
{
	exec("DELETE FROM location;");
	exec("INSERT INTO LOCATION VALUES (\""+new_location+"\");");
}

void
db_manager::update_dl_manager(const std::vector <dl_mngr> new_managers)
{
	exec("DELETE FROM dl_managers;");
	for (unsigned int i=0; i< new_managers.size(); i++) {
		exec(
				"INSERT INTO dl_managers VALUES (\""+
				new_managers[i].name+"\",\""+
				new_managers[i].command+"\","+
				( new_managers[i].selected? "1);" : "0);")
			);
	}
}


void
db_manager::exec(std::string command)
{
	rc = sqlite3_exec(db, command.c_str(), exec_callback, 0, &zErrMsg);
	if( rc!=SQLITE_OK ){
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
		zErrMsg = 0;
	}
}

/*
int
main()
{
	db_manager db;
	std::cout<< db.get_mg_location()<<"\n";
	db.update_mg_location("/home/raptor/");
	std::cout<< db.get_mg_location()<<"\n";
	db.update_mg_location("/home/raptor/mangaDL");
	std::cout<< db.get_mg_location()<<"\n";

	std::vector <mg_list> test =  db.get_mg_list();
	for(int i=0;i<test.size();i++) {
		std::cout<<test[i].mgname<<"-"
			<<test[i].mgchapter<<"-"
			<<test[i].mglastdate<<"\n";
	}

	std::vector <dl_mngr> test2 =  db.get_dl_managers();
	for(int i=0;i<test2.size();i++) {
		std::cout<<test2[i].name<<"-"
			<<test2[i].command<<"-"
			<<test2[i].selected<<"\n";
	}

	return 0;
}
*/
