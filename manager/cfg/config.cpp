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
 * CREATE TABLE provider ( name VARCHAR ); 
 * contains the current provider "mangareader"
 *
 * TODO:
 *     Language for the external download manager.
 *     replaces [INPUT] by the input file name and [OUTPUT] by the output name
 *
 * IMPLEMENTATION
 */

#include "config.hpp"


///================constructor=======================================================///
config::config()
{
	zErrMsg     = 0;
	rc          = sqlite3_open(DB_NAME, &db);
	if( rc ){
		std::cerr <<"Can't open database: "
			<< sqlite3_errmsg(db) <<"\n";
		sqlite3_close(db);
	}
	else {
	}
}
///==================================================================================///


///==========================Destructor==============================================///
config::~config()
{
	sqlite3_close(db);
}
///==================================================================================///


///===============execute an sql query without returning anything====================///
void
config::exec(std::string command)
{
	rc = sqlite3_exec(db, command.c_str(), exec_callback, 0, &zErrMsg);
	if( rc!=SQLITE_OK ){
		std::cerr<< "SQL error: "<< zErrMsg<<"\n";
		sqlite3_free(zErrMsg);
		zErrMsg = 0;
	}
	else {
		//everything went well
	}
}
///==================================================================================///


///================returns the current location to save the manga====================///
std::string
config::get_mg_location()
{
	std::string response = "";
	char **pazResult;
	int row, column;

	rc = sqlite3_get_table(
		db,                        /* An open database */
		"select * from location",  /* SQL to be evaluated */
		&pazResult,                /* Results of the query */
		&row,                      /* Number of result rows written here */
		&column,                   /* Number of result columns written here */
		&zErrMsg                   /* Error msg written here */
	);

	if ( rc!=SQLITE_OK ) {
		std::cerr<<"SQL error:" << zErrMsg <<"\n";
		sqlite3_free(zErrMsg);
		zErrMsg = 0;
	}
	else {
		if (pazResult[1]) {
			response = pazResult[1];
		}
		else {
			//response = "";
		}
		sqlite3_free_table(pazResult);
	}
	return response;
}
///==================================================================================///


///============change the location of the manga======================================///
void
config::update_mg_location(const std::string new_location)
{
	this->exec("DELETE FROM location;");
	this->exec("INSERT INTO LOCATION VALUES (\""+new_location+"\");");
}
///==================================================================================///


///================returns the current provider======================================///
std::string
config::get_provider()
{
	std::string response = "";
	char **pazResult;
	int row, column;

	rc = sqlite3_get_table(
		db,                        /* An open database */
		"select * from provider",  /* SQL to be evaluated */
		&pazResult,                /* Results of the query */
		&row,                      /* Number of result rows written here */
		&column,                   /* Number of result columns written here */
		&zErrMsg                   /* Error msg written here */
	);

	if ( rc!=SQLITE_OK ) {
		std::cerr<<"SQL error:" << zErrMsg <<"\n";
		sqlite3_free(zErrMsg);
		zErrMsg = 0;
	}
	else {
		if (pazResult[1]) {
			response = pazResult[1];
		}
		else {
			//response = "";
		}
		sqlite3_free_table(pazResult);
	}
	return response;
}
///==================================================================================///


///============change the provider ==================================================///
void
config::update_provider(const std::string new_provider)
{
	this->exec("DELETE FROM provider;");
	this->exec("INSERT INTO LOCATION VALUES (\""+new_provider+"\");");
}
///==================================================================================///


///========return a struct with the dl manager related data==========================///
std::vector <dl_mngr>
config::db_get_dl_managers()
{
	std::vector <dl_mngr> container;
	dl_mngr temp_list;
	char **pazResult;
	int row, column;

	rc = sqlite3_get_table(
		db,                        /* An open database */
		"select * from dl_managers;",/* SQL to be evaluated */
		&pazResult,                /* Results of the query */
		&row,                      /* Number of result rows written here */
		&column,                   /* Number of result columns written here */
		&zErrMsg                   /* Error msg written here */
	);

	if ( rc!=SQLITE_OK ) {
		std::cerr<< "SQL error: "<< zErrMsg <<"\n";
		sqlite3_free(zErrMsg);
		zErrMsg = 0;
	}
	else {
		for (int i = 0, k = 3; i<row; i++, k+=3) {
			pazResult[i+k] ? (temp_list.name = pazResult[i+k]): (temp_list.name="");
			pazResult[i+k+1] ? (temp_list.command = pazResult[i+k+1]): (temp_list.command="");
			if (pazResult[i+k+2]) {
				if (strcmp(pazResult[i+k+2], "1") == 0) {
					 temp_list.selected = true;
				}
				else  {
					temp_list.selected = false;
				}
			}
			else {
				temp_list.selected = false;
			}
			if (temp_list.name!="") {
				container.push_back(temp_list);
			}
			else {
			}
			k--;
		}
		sqlite3_free_table(pazResult);
	}
	return container;
}
///==================================================================================///


///================add a new dl manager==============================================///
void
config::db_add_dl_manager(
	const std::string name, 
	const std::string command, 
	const bool selected)
{
	if (selected) {
		this->exec("INSERT INTO dl_managers VALUES ("+name+","+command+",1)");
	}
	else {
		this->exec("INSERT INTO dl_managers VALUES ("+name+","+command+",0)");
	}
}
///==================================================================================///


///================update the dl manager from a dl manager list======================///
void
config::db_update_dl_manager(const std::vector <dl_mngr> new_managers)
{
	this->exec("DELETE FROM dl_managers;");
	for (auto  i: new_managers) {
		this->exec(
				"INSERT INTO dl_managers VALUES (\""+
				i.name+"\",\""+
				i.command+"\","+
				( i.selected? "1);" : "0);")
			);
	}
}

///==================================================================================///


///========remove a dl manager based on its name=====================================///
void
config::remove_dl_manager(const std::string given_name)
{
	this->exec("DELETE FROM dl_managers WHERE name=\""+given_name+"\";");
}
///==================================================================================///


///==========to use later when the updater is done===================================///
std::vector <mg_list>
config::get_mg_list()
{
	std::vector <mg_list> container;
	mg_list temp_list;
	char **pazResult;
	int row, column;

	rc = sqlite3_get_table(
		db,                        /* An open database */
		"select * from mg_list;",  /* SQL to be evaluated */
		&pazResult,                /* Results of the query */
		&row,                      /* Number of result rows written here */
		&column,                   /* Number of result columns written here */
		&zErrMsg                   /* Error msg written here */
	);

	if ( rc!=SQLITE_OK ) {
		std::cerr<< "SQL error: "<< zErrMsg<<"\n";
		sqlite3_free(zErrMsg);
		zErrMsg = 0;
	}
	else {
		for (int i = 0, k = 3; i<row; i++, k+=3) {
			pazResult[i+k]   ? (temp_list.mgname = pazResult[i+k])    :(temp_list.mgname="");
			pazResult[i+k+1] ? (temp_list.mgchapter=pazResult[i+k+1]) :(temp_list.mgchapter="");
			pazResult[i+k+2] ? (temp_list.mglastdate=pazResult[i+k+2]):(temp_list.mglastdate="");
			if (temp_list.mgname!="") {
				container.push_back(temp_list);
			}
			else {
			}
			k--;
		}
		sqlite3_free_table(pazResult);
	}
	return container;
}
///==================================================================================///


///==================================================================================///
void 
config::set_current_location(std::string user_current_location)
{
	this->cfg_current_location = user_current_location;
}
///==================================================================================///


///==================================================================================///
std::string 
config::get_current_location()
{
	return this->cfg_current_location;
}
///==================================================================================///


///==================================================================================///
void 
config::set_download_managers(std::vector<dl_mngr> user_download_managers)
{
	this->cfg_download_managers = user_download_managers;
}
///==================================================================================///


///==================================================================================///
std::vector<dl_mngr> 
config::get_download_managers()
{
	return this->cfg_download_managers;
}
///==================================================================================///


///==================================================================================///
void 
config::set_queue(std::vector<manga_queue_data> user_queue)
{
	this->cfg_queue = user_queue;
}
///==================================================================================///


///==================================================================================///
std::vector<manga_queue_data> 
config::get_queue()
{
	return this->cfg_queue;
}
///==================================================================================///


///==================================================================================///
void 
config::set_current_provider(std::string user_prov)
{
	this->cfg_current_provider = user_prov;
}
///==================================================================================///


///==================================================================================///
std::string 
config::get_current_provider()
{
	return this->cfg_current_provider;
}
///==================================================================================///


///==================================================================================///
void 
config::set_revert_current_location(std::string user_revert_loc)
{
	this->cfg_revert_current_location = user_revert_loc;
}
///==================================================================================///


///==================================================================================///
std::string 
config::get_revert_current_location()
{
	return this->cfg_revert_current_location;
}
///==================================================================================///


///==================================================================================///
void 
config::set_revert_download_managers(std::vector<dl_mngr> user_dls)
{
	this->cfg_revert_download_managers = user_dls;
}
///==================================================================================///


///==================================================================================///
std::vector<dl_mngr> 
config::get_revert_download_managers()
{
	return this->cfg_revert_download_managers;
}
///==================================================================================///


///===============execute a query and outputs the result=============================///
int
config::exec_callback(
	void* NotUsed, 
	int argc, 
	char** argv, 
	char** azColName)
{
	for(int i=0; i<argc; i+=2){
		std::cout<< azColName[i] 
			<<" = "
			<< (argv[i]? argv[i] :"NULL");
		std::cout<< azColName[i+1]
			<<" = "
			<< (argv[i+1] ? argv[i+1] : "NULL")
			<<"\n";
	}
	std::cout<<"\n";
	return 0;
}
///==================================================================================///

