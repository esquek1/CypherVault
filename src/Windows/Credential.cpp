#include "credential.h"
#include <unordered_map>

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
                    "SERVICE     TEXT    NOT NULL, "
                    "USER        TEXT    NOT NULL, "
                    "PASSWORD    TEXT    NOT NULL);";
                    
    int exit = 0; 

    exit = sqlite3_exec(DB, query.c_str(), NULL, 0, &errMessage);
    
    if(exit != SQLITE_OK){
       // cerr << "Error Create Table" << endl;
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

void Credential::deleteCredential(string service, string user){
    sqlite3_stmt* stmt;
    string query = "DELETE FROM CREDENTIALS WHERE service = ? AND user = ?" ;

    // Prepare statement
    if(sqlite3_prepare_v2(DB, query.c_str(), -1, &stmt, nullptr) != SQLITE_OK){
        std::cerr << "\nError preparing statement: " << sqlite3_errmsg(DB) << std::endl;
        return;
    }

    // Bind the new user/password to the first placeholder
    if(sqlite3_bind_text(stmt, 1, service.c_str(), -1, SQLITE_TRANSIENT) != SQLITE_OK){
        cerr << "Error binding service: " << sqlite3_errmsg(DB) << endl;
        sqlite3_finalize(stmt);
        return;
    }

    // Bind the user to the second placeholder
    if(sqlite3_bind_text(stmt, 2, user.c_str(), -1, SQLITE_TRANSIENT) != SQLITE_OK){
        cerr << "Error binding user: " << sqlite3_errmsg(DB) << endl;
        sqlite3_finalize(stmt);
        return;
    }

    // Execute the statement 
    if(sqlite3_step(stmt) != SQLITE_DONE){
        std::cerr << "Error executing DELETE statement: " << sqlite3_errmsg(DB) << std::endl;
    } else {
        std::cout << "\nRecord deleted successfully!" << std::endl;
    }

    // Finalize the statement
    sqlite3_finalize(stmt);

    // Run the VACUUM command
    char* errMsg = nullptr;
    int exit = sqlite3_exec(DB, "VACUUM;", nullptr, nullptr, &errMsg);
    if (exit != SQLITE_OK) {
        std::cerr << "SQL error: " << errMsg << std::endl;
        sqlite3_free(errMsg);
    } else {
        std::cout << "Database compacted successfully." << std::endl;
    }
}

/*
    Displays all credentials to the console
*/
void Credential::displayDatabase(){
    string query = "SELECT * FROM CREDENTIALS;";
    cout << "\n\n********************************************************************" << endl;
    cout << "|                           Cypher Vault                           |" << endl;
    cout << "********************************************************************" << endl;

    sqlite3_exec(DB, query.c_str(), callback, NULL, NULL);
}

Credential::Credential(){
   createTable();
}

// Destructor definition
Credential::~Credential() {}

/*
    Calls binding(choice, service, newUsername) to change the username
*/
void Credential::setUsername(string service, string user, const string& newUsername) {
    binding(1, service, user, newUsername);
}

/*s
    Calls binding(choice, service, newPassword) to change the password
*/
void Credential::setPassword(string service, string user, const string& newPassword) {
    binding(2, service, user, newPassword);
}

void Credential::binding(int choice, string service, string user, const string& updateCell){
    cout << " CHECKING ARGS\n\n";
    cout << choice << " " << service << " " << user << " " << updateCell;
    sqlite3_stmt* stmt;
    string query;
    if(choice == 1){
        query = "UPDATE CREDENTIALS SET USER = ? WHERE SERVICE = ? AND USER = ?";
    }else{
        query = "UPDATE CREDENTIALS SET PASSWORD = ? WHERE SERVICE = ? AND USER = ?";
    }

    // Prepare statement
    if(sqlite3_prepare_v2(DB, query.c_str(), -1, &stmt, nullptr) != SQLITE_OK){
        cerr << "\nError preparing statement: " << sqlite3_errmsg(DB) << endl;
        return;
    }

    // Bind the new user/password to the first placeholder
    if(sqlite3_bind_text(stmt, 1, updateCell.c_str(), -1, SQLITE_TRANSIENT) != SQLITE_OK){
        cerr << "Error binding: " << sqlite3_errmsg(DB) << endl;
        sqlite3_finalize(stmt);
        return;
    }

    // Bind the service to the second placeholder
    if(sqlite3_bind_text(stmt, 2, service.c_str(), -1, SQLITE_TRANSIENT) != SQLITE_OK){
        cerr << "Error binding SERVICE: " << sqlite3_errmsg(DB) << endl;
        sqlite3_finalize(stmt);
        return;
    }

    // Bind the user to the second placeholder
    if(sqlite3_bind_text(stmt, 3, user.c_str(), -1, SQLITE_TRANSIENT) != SQLITE_OK){
        cerr << "Error binding USER: " << sqlite3_errmsg(DB) << endl;
        sqlite3_finalize(stmt);
        return;
    }

    // Execute the statement
    if(sqlite3_step(stmt) != SQLITE_DONE){
        cerr << "Error executing statement: " << sqlite3_errmsg(DB) << endl;
    }else {
        cout << "Record updated successfully!" << endl;
    }
   
    // Finalize the statement
    sqlite3_finalize(stmt);
}

bool Credential::findCredential(string& str, string &userSelected) {
    sqlite3_stmt* stmt;
    string query = "SELECT * FROM CREDENTIALS WHERE SERVICE LIKE ?";  // Use a placeholder

    // Prepare the SQL statement
    int exit = sqlite3_prepare_v2(DB, query.c_str(), -1, &stmt, 0);
    if (exit != SQLITE_OK) {
        return false;  // Return false on error
    }

    // To avoid SQL injections
    // Bind the `str` parameter to the placeholder with wildcards for partial matching
    string param = "%" + str + "%";  // Add the wildcards to `str` for partial matching

    exit = sqlite3_bind_text(stmt, 1, param.c_str(), -1, SQLITE_TRANSIENT);  // Bind to the first placeholder
    if (exit != SQLITE_OK) {
        sqlite3_finalize(stmt);
        return false;  // Return false on error
    }

    // Execute the statement and check for results
    unordered_map<string, string> foundServicesMap;

    bool found = false;
    int count = 0;
    while ((exit = sqlite3_step(stmt)) == SQLITE_ROW) {
        found = true;  // At least one record was found

        // Print record 
        // NOTE: sqlite3_column_text is declared as const unsigned char*
        const unsigned char* service = sqlite3_column_text(stmt, 0); 
        // const unsigned char* service = sqlite3_column_text(stmt, 1);
        const unsigned char* user = sqlite3_column_text(stmt, 1);
        const unsigned char* password = sqlite3_column_text(stmt, 2);

        // Store id to the vector and increment count
        foundServicesMap.insert(make_pair(
                string(reinterpret_cast<const char*>(user)),
                string(reinterpret_cast<const char*>(service))
        ));

        count++;

        // Process or print the found record as needed
        cout << "Service: " << service 
                  << ", User: " << user 
                  << ", Password: " << password 
                  << endl;
    }

    // If multiple records are found, user will be prompted to enter a valid ID
    if(count > 1){
        cout << "\nMultiple credentials found under '" << str << "'\n";
        string choice;
        bool valid = false;
        while(!valid){
            cout << "\nChoose a user/e-mail (case sensitive): ";
            
            cin >> choice;

            // Check if selected service is valid
            for(auto it : foundServicesMap){
                if(it.first == choice){
                    userSelected = choice;
                    str = it.second;
                    valid = true;
                    break;
                }
            }

            if(!valid){
                cout << "Invalid service. Choose a valid service from the list: \n";
                for(auto it : foundServicesMap){
                    cout << it.first << "  " << it.second << endl;
                } 
            }
        }
        
    }
    // Finalize the statement
    sqlite3_finalize(stmt);

    return found;  // Return true if found, otherwise false
}

