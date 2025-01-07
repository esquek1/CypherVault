# CypherVault
+ [About](#about)
+ [Getting Started](#getting_started)
+ [Usage](#usage)
+ [Contributing](../CONTRIBUTING.md)

## About <a name = "about"></a>
A terminal-based, multi-factor authentication password manager using C++ and SQLite and implementing AES encryption with OpenSSL to ensure data security.## Table of Contents

## Features <a name = "features"></a>
```
- Secure Storage: Encrypts and stores passwords securely using AES-256 encryption.
- Easy Retrieval: Retrieve your passwords quickly with a user-friendly interface.
- Master Password Protection: Access your password vault with a single, secure master password.
```
## Getting Started <a name = "getting_started"></a>
These instructions will get you a copy of the project up and running on your local machine for development and testing purposes. See [deployment](#deployment) for notes on how to deploy the project on a live system.

### Prerequisites
Before you begin, make sure you have the following installed:

- **Git**: To clone the repository.
- **C++ Compiler**: Such as GCC or Clang, compatible with C++11 or later.
- **SQLite3**: Ensure that SQLite3 is installed and accessible from your compiler.

### Installation

1. **Clone the repository**

   Open a terminal and run:

   ```bash
   git clone https://github.com/esquek1/CypherVault.git
   cd CypherVault

2. **Install dependencies**

    Ensure SQLite3 and, if applicable, OpenSSL are installed. 

3. **Compile the project**

    ```bash
    g++ -o main Main.cpp Prompts.cpp Credential.cpp -lsqlite3

4. **Run the project**

    ```bash
    ./main

### Bugs
If you discover bugs, please report them in Github Issues.
