#include "credential.h"
using namespace std;

int Credential::callback(void *data, int argc, char** argv, char** azColName){
	int i;
	cerr << static_cast<const char*> (data);
	for(int i = 0; i < argc; i++) {
    	std::cout << azColName[i] << " = " << (argv[i] ? argv[i] : "NULL") << std::endl;
  	}

	cout << endl;
	return 0;
}


void Credential::createTable(){
    /* sqlite3_open:
    This routine opens a connection to an SQLite database file 
    and returns a database connection object.
    */
    int returnCode = sqlite3_open("cyphervault.db", &DB);

    // Check if database exist
    // if returnCode == 0, succeeds
    // if the database doesn't exist, it will be created in the same directory
    if(returnCode){
        cerr << "Error opening database: %s\n" << sqlite3_errmsg(DB);
        exit(-1);
    }else{
        cout << "Opened Database Successfully." << endl;
    }

    // Create the table
    string query = "CREATE TABLE CREDENTIALS("
                    "ID         INTEGER  PRIMARY KEY, "
                    "SERVICE     TEXT    NOT NULL, "
                    "USER        TEXT    NOT NULL, "
                    "PASSWORD    TEXT    NOT NULL);";
                    
    int exit = 0; 

    exit = sqlite3_exec(DB, query.c_str(), NULL, 0, &errMessage);
    
    if(exit != SQLITE_OK){
        cerr << "Error Create Table" << endl;
        sqlite3_free(errMessage);
    }else{
        cout << endl << endl << "Table created Successfully!" << endl << endl;
    }
}

// Close the connection to the database
// All prepared statement associated with the 
// connection should be finalized before closing.
void Credential::closeDatabase(){
    sqlite3_close(DB);
}

/*
The life-cycle of a prepared statement object usually goes like this:

Create the prepared statement object using sqlite3_prepare_v2().
Bind values to parameters using the sqlite3_bind_*() interfaces.
Run the SQL by calling sqlite3_step() one or more times.
Reset the prepared statement using sqlite3_reset() then go back to step 2. Do this zero or more times.
Destroy the object using sqlite3_finalize().
*/
void Credential::insertCredential(string service, string username, string password){
    sqlite3_stmt* stmt;
    // TESTING
    cout << "Inserting credentials: " << service << " " << username << " " << password << endl;
    string query = "INSERT INTO CREDENTIALS (SERVICE, USER, PASSWORD) VALUES (?, ?, ?);";
    // Prepare the statement
    int exit = sqlite3_prepare_v2(DB, query.c_str(), -1, &stmt, 0);
    if (exit != SQLITE_OK) {
        std::cerr << "Error preparing statement for INSERT: " << sqlite3_errmsg(DB) << std::endl;
        return;
    }

    // Bind the parameters to the SQL statement
    sqlite3_bind_text(stmt, 1, service.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, username.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 3, password.c_str(), -1, SQLITE_TRANSIENT);

    // Execute the statement
    exit = sqlite3_step(stmt);
    if (exit != SQLITE_DONE) {
        std::cerr << "Error executing statement: " << sqlite3_errmsg(DB) << std::endl;
    } else {
        std::cout << "Record created successfully!" << std::endl;
    }

    // Destruct - Finalize the statement to release resources
    sqlite3_finalize(stmt);
  
	// int exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &errMessage);
    
    // if(exit != 0){
	// 	cerr << "Error Insert" << endl;
	// 	sqlite3_free(errMessage);
	// }else{
	// 	cout << "Records created Successfully!" << endl;
	// }
}

void Credential::deleteCredential(){
    string query = "DELETE FROM CREDENTIALS WHERE ID = 2;" ;
	int exit = sqlite3_exec(DB, query.c_str(), NULL, 0, &errMessage);

	if(exit != SQLITE_OK){
		cerr << "Error DELETE" << endl;
		sqlite3_free(errMessage);
	}else{
		cout << "\n Record deleted Successfully!" << endl;
	}
}

/*
    Displays all credentials to the console
*/
void Credential::displayDatabase(){
    string query = "SELECT * FROM CREDENTIALS;";
    cout << "-----------------------------------------" << endl;
    cout << "|            STATE OF TABLE             |" << endl;
	cout << "-----------------------------------------" << endl;

    sqlite3_exec(DB, query.c_str(), callback, NULL, NULL);
}

void Credential::displayRecord(){

}


Credential::Credential(){
   createTable();
}

Credential::Credential(string x, string y, string z) {
    
    service = x;
    username = y;
    password = z;
}

// Destructor definition
Credential::~Credential() {}

// Getter function definitions
string Credential::getService() const {
    return service;
}

string Credential::getUsername() const {
    return username;
}

string Credential::getPassword() const {
    return password;
}

// Setter function definitions

void Credential::setCredential(string x, string y, string z){
    service = x;
    username = y;
    password = z;
}

void Credential::setService(const string& newService) {
    service = newService;
}

void Credential::setUsername(const string& newUsername) {
    username = newUsername;
}

void Credential::setPassword(const string& newPassword) {
    sqlite3* stmt;
    string query = "UPDATE CREDENTIALS SET PASSWORD = newPassword WHERE ID = 1";

}

void Credential::findCredential(const string& str){
    sqlite3_stmt* stmt;
    string query = "SELECT * FROM CREDENTIALS WHERE ID LIKE '%str%'";
    int exit = sqlite3_prepare_v2(DB, query.c_str(), -1, &stmt, 0);
    if (exit != SQLITE_OK) {
        std::cerr << "\nError preparing statement: " << sqlite3_errmsg(DB) << std::endl;
        return;
    }

    // Bind the parameters to the SQL statement

    // Execute the statement
    exit = sqlite3_step(stmt);
    if (exit != SQLITE_DONE) {
        std::cerr << "Error executing statement: " << sqlite3_errmsg(DB) << std::endl;
    } else {
        std::cout << "Record created successfully!" << std::endl;
    }
    
    // Destruct
    sqlite3_finalize(stmt);
}