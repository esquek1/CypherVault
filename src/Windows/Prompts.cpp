
#include "prompts.h"
#include <unistd.h>
#include <conio.h> 
#include <limits>

using namespace std;
string commonPasswords[]  = {"qwerty", "password", "abc123", "uiop"};
//**********************************************************************
// Enumerator 
enum IN { 
	// 13 is ASCII for carriage 
	// return 
	IN_BACK = 8, 
	IN_RET = 13 

}; 

/*
	Creates new account. 
*/
void Prompts::newAccount (string &service, string &username, string &password){
	cout << "\n\n********************************************************************" << endl;
	cout << "|                          Account Setup                           |" << endl;
	cout << "********************************************************************" << endl;
	cout << "Enter service or type 'exit' to quit: ";
	cin >> service;
	if(service == "exit"){
		return;
	}
	cout << "Enter email or username for " << service << ": ";
	cin >> username;
	cout << "Enter password: ";

	password = getPassword();

	// Checks validity of password
	bool isValid = true;
	while(!checkPasswordValidity(password)){
		cout << "Invalid password. Please try again.\n";
		cout << "Enter password or 'exit' to quit: " ;
		password = getPassword();
		if(password == "exit"){
			return;
		}
	}
	showPassword(password);
	newCredential.insertCredential(service, username, password);
}

void Prompts::showPassword(string &password){
	bool validInput = true;
	cout << "Show password? Type 'yes' or 'no': ";
	string seePassword;

	while(validInput){
		cin >> seePassword;

		if(seePassword == "yes"){
			cout << "Password: " << password << endl;
			validInput = false;
		}else if(seePassword == "no"){
			validInput = false;
		}else{
			cout << "Invalid input: Type 'yes' or 'no': " << endl;
		}
	}
}

// Checks the validity of a password
bool Prompts::checkPasswordValidity(string &password){
	if(password.length() < 12){
		cout << "\n\nPassword length needs to be at least 12 characters.\n\n" ;
		return false;
	}

	bool hasUpper = false, hasLower = false, hasDigit = false, hasSpecial = false;

	// check complexity
	// Does it include at least one special character, uppercase, lowercase, and number
	for (char ch : password){
		if (isupper(ch)) hasUpper = true;
		else if (islower(ch)) hasLower = true;
		else if (isdigit(ch)) hasDigit = true;
		else if (ispunct(ch)) hasSpecial = true;
	}

	cout << "\n\n";
	if(!hasUpper){
		cout << "Password is missing an uppercase letter. \n";
	}
	if(!hasLower){
		cout << "Password is missing a lowercase letter. \n";
	}
	if(!hasDigit){
		cout << "Password is missing a number. \n";
	}
	if(!hasSpecial){
		cout << "Password is missing a special character. \n";
	}

	cout << "\n\n";
	// check if common password

	// check history
	return hasUpper && hasLower && hasDigit && hasSpecial;
	
}
/*
	Changes password while hiding user input as user types

*/
void Prompts::changePassword(string &password){
	cout << "\n\n********************************************************************" << endl;
	cout << "|                         Changing Password                        |" << endl;
	cout << "********************************************************************" << endl;

	string service;
	string user;
	// Search for the service in the database
	// Prompt user for a valid service, with an option to exit
    while (true) {
        cout << "Enter service to change password for (or 'exit' to cancel): ";
        cin >> service;

        if (service == "exit") {
            cout << "Changing password canceled.\n";
            return;
        }

        // Check if the service exists
        if (newCredential.findCredential(service, user)) {
            break;  // Exit loop if service is found
        } else {
            cout << "Service not found. Please try again.\n";
        }
    }

	// get user input for password
	cout << "New password: ";
	cin >> password;

	// Checks validity of password
	bool isValid = true;
	while(!checkPasswordValidity(password)){
		cout << "Invalid password. Please try again.\n";
		cout << "Enter password for " << service << ": ";
		cout << "or 'exit' to quit: " ;
		password = getPassword();
		if(password == "exit"){
			return;
		}
	}
	showPassword(password);
	cout << "Updating password.\n\n";
	newCredential.setPassword(service, user, password);
}

/*
	Accepts and hides the password with '*' as user types.
*/
string Prompts::getPassword(){ 
	// Stores the password 
	char sp = '*';
	string passwd = ""; 
	char input; 

	// Until condition is true 
	while (true) { 
		// Retrieves characters from the keyboard like cin
		input = getch(); 

		// if input is ENTER, return 
		if (input == '\r') { 
			cout << endl; 
			return passwd; 
		} else if (input == IN::IN_BACK && passwd.length() != 0) { 
			passwd.pop_back(); 

			// Cout statement is very 
			// important as it will erase 
			// previously printed character 
			cout << "\b \b"; 
			continue; 
		} 

		// Without using this, program 
		// will crash as \b can't be 
		// print in beginning of line 
		else if (input == IN::IN_BACK && passwd.length() == 0) { 
			continue; 
		} 

		passwd.push_back(input); 
		cout << sp; 
	} 
} 
// Only letters and numbers can be used; no special characters, spaces nor symbols
void Prompts::changeUsername(string &username){
	cout << "\n\n********************************************************************" << endl;
	cout << "|                         Changing Username                        |" << endl;
	cout << "********************************************************************" << endl;


	// Get service 
	string service;
	string user;
	while (true) {
        cout << "Enter service to change username for (or 'exit' to cancel): ";
        cin >> service;

        if (service == "exit") {
            cout << "Changing username canceled.\n";
            return;
        }

        // Check if the service exists
		// If exist, prompts user to choose which specific user
        if (newCredential.findCredential(service, user)) {
            break;  // Exit loop if service is found
        } else {
            cout << "Service not found. Please try again.\n";
        }
    }

	// Get username
	cout << "New username: ";
	cin >> username;
	
	cout << "Updating username from '" << user << "' to '" << username << "'\n\n";
	newCredential.setUsername(service, user, username);
}

/*
	Prompts user for -__ to use in the query of deleteCredential();

*/
void Prompts::deleteAccount(){
	cout << "\n\n********************************************************************" << endl;
	cout << "|                         Deleting Account                         |" << endl;
	cout << "********************************************************************" << endl;

	// Get service 
	string service;
	string user;
	while(true) {
		cout << "Enter service to delete account for (or 'exit' to cancel): ";
		cin >> service;

		if (service == "exit") {
            cout << "Deleting account canceled.\n";
            return;
        }

		// Check if the service exists
        if (newCredential.findCredential(service, user)) {
            break;  // Exit loop if service is found
        } else {
            cout << "Service not found. Please try again.\n";
        }
	}

	// Delete chosen account
	cout << "Deleting account: service: " << service << " user: " << user;
	newCredential.deleteCredential(service, user);
}

/*
	Menu for Cypher Vault. Input allowed are between 1-7. Otherwise, re-prompt.
*/
int Prompts::menu(){
	int choice;
    while (true) {  // Loop until valid input is received
        cout << "Enter: ";
        cin >> choice;

        if (cin.fail()) {
            // If the input is not an integer
            cin.clear();  // Clear the error flag
            cin.ignore(numeric_limits<streamsize>::max(), '\n');  // Discard invalid input
            cout << "Invalid input. Expecting a number between 1 and 7.\n";
        } else if (choice < 1 || choice > 7) {
            // Check if the input is outside the valid range
            cout << "Invalid choice. Please enter a number between 1 and 7.\n";
        } else {
            // Valid input, break the loop and return the choice
            return choice;
        }
    }
}

void Prompts::printMenu(){
	cout << "\n\n********************************************************************" << endl;
	cout << "|                               Menu                               |" << endl;
	cout << "********************************************************************" << endl;
	cout << "| 1. New Accounts                                                  |" << endl;
	cout << "| 2. Delete Account                                                |" << endl;
	cout << "| 3. Change Account                                                |" << endl;
	cout << "| 4. Change Password                                               |" << endl;
	cout << "| 5. Help                                                          |" << endl;
	cout << "| 6. Display Credentials                                           |" << endl;
	cout << "| 7. Exit                                                          |" << endl;
	cout << "********************************************************************" << endl;
}

/*
	Calls ___

*/
void Prompts::printInformation(){
	newCredential.displayDatabase();
}


void Prompts::displayHelp(){
	cout << "\n\n********************************************************************" << endl;
	cout << "|                               Menu                               |" << endl;
	cout << "********************************************************************" << endl;
	cout << "| 1. New Accounts                                                  |" << endl;
	cout << "|    - Select this option to create a new account.                 |" << endl;
    cout << "|      You will be asked to provide necessary details              |" << endl;
    cout << "|      such as username and password.                              |" << endl;
    cout << "|                                                                  |" << endl;
    cout << "| 2. Delete Account                                                |" << endl;
    cout << "|    - Select this option to delete an existing account.           |" << endl;
    cout << "|      You will need to confirm your choice to ensure that         |" << endl;
    cout << "|      you do not delete an account by mistake.                    |" << endl;
    cout << "|                                                                  |" << endl;
    cout << "| 3. Change Username                                               |" << endl;
    cout << "|    - Select this option to change the username or email          |" << endl;
    cout << "|      associated with the current account. You will be            |" << endl;
    cout << "|      asked to provide the new username/email                     |" << endl;
    cout << "|      and confirm the change.                                     |" << endl;
    cout << "|                                                                  |" << endl;
    cout << "| 4. Change Password                                               |" << endl;
    cout << "|    - Select this option to change the password of the            |" << endl;
    cout << "|      current account. You will be asked to provide               |" << endl;
    cout << "|      your current password and the new password.                 |" << endl;
    cout << "|                                                                  |" << endl;
    cout << "| 5. Help                                                          |" << endl;
    cout << "|    - Select this option to view this help menu, which            |" << endl;
    cout << "|      provides information about the various options              |" << endl;
    cout << "|      available in the main menu.                                 |" << endl;
	cout << "|                                                                  |" << endl;
    cout << "| 6. Display Credentials                                           |" << endl;
    cout << "|    - Select this option to view all credentials.                 |" << endl;
	cout << "| 	    Passwords will be encrypted. To view the password,          |" << endl;
	cout << "| 	    provide the service and master password which will          |" << endl;
	cout << "| 	    decrypt the password.                                       |" << endl;
	cout << "|                                                                  |" << endl;
	cout << "| 7. Exit                                                          |" << endl;
    cout << "|    - Select this option to exit the application.                 |" << endl;
    cout << "********************************************************************" << endl;
}
//**********************************************************************


//**********************************************************************


