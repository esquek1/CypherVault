#ifndef PROMPTS_H
#define PROMPTS_H

#include <iostream>
#include <string>

#include "databaseManager.h"
using namespace std;

class Prompts {
    
    private:
        DatabaseManager newCredential;

    public:
    //**********************************************************************
    
        void newAccount (string &, string &, string &);
        void changePassword(string &);
        void changeUsername(string &);
        void deleteAccount();
        int menu();
        void printMenu();
        void printInformation();
        void displayHelp();
        string getPassword();
        bool checkPasswordValidity(string &);
        void showPassword(string &);
   

};

#endif