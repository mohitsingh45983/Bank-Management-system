#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
using namespace std;

// Base class Account
class Account {
protected:
    int accountNumber;
    string name;
    double balance;

public:
    virtual void createAccount() = 0;  // Pure virtual function (abstraction)
    virtual void showAccountDetails() const = 0;
    virtual void deposit(double amount);
    virtual void withdraw(double amount);
    virtual void calculateInterest() = 0;  // Interest calculation varies by account type
    virtual char getAccountType() const = 0;
    int getAccountNumber() const;
    double getBalance() const;
};

// Derived class for Savings Account
class SavingsAccount : public Account {
public:
    void createAccount() override;
    void showAccountDetails() const override;
    void calculateInterest() override;
    char getAccountType() const override { return 'S'; }
};

// Derived class for Current Account
class CurrentAccount : public Account {
public:
    void createAccount() override;
    void showAccountDetails() const override;
    void calculateInterest() override;
    char getAccountType() const override { return 'C'; }
};

// Implementation of Account methods
void Account::deposit(double amount) {
    balance += amount;
    cout << "\nAmount deposited successfully!";
}

void Account::withdraw(double amount) {
    if (amount <= balance) {
        balance -= amount;
        cout << "\nAmount withdrawn successfully!";
    } else {
        cout << "\nInsufficient balance!";
    }
}

int Account::getAccountNumber() const {
    return accountNumber;
}

double Account::getBalance() const {
    return balance;
}

// Implementation of SavingsAccount methods
void SavingsAccount::createAccount() {
    cout << "\nEnter account number: ";
    cin >> accountNumber;
    cout << "\nEnter account holder name: ";
    cin.ignore();
    getline(cin, name);
    cout << "\nEnter initial balance (>=500 for Savings): ";
    cin >> balance;
    cout << "\nSavings Account created successfully!";
}

void SavingsAccount::showAccountDetails() const {
    cout << "\nAccount Number: " << accountNumber;
    cout << "\nAccount Holder: " << name;
    cout << "\nAccount Type: Savings";
    cout << "\nBalance: " << balance;
    cout<<endl;
}

void SavingsAccount::calculateInterest() {
    double interest = balance * 0.05;
    balance += interest;
    cout << "\nInterest of 5% applied. New Balance: " << balance;
}

// Implementation of CurrentAccount methods
void CurrentAccount::createAccount() {
    cout << "\nEnter account number: ";
    cin >> accountNumber;
    cout << "\nEnter account holder name: ";
    cin.ignore();
    getline(cin, name);
    cout << "\nEnter initial balance (>=1000 for Current): ";
    cin >> balance;
    cout << "\nCurrent Account created successfully!";
}

void CurrentAccount::showAccountDetails() const {
    cout << "\nAccount Number: " << accountNumber;
    cout << "\nAccount Holder: " << name;
    cout << "\nAccount Type: Current";
    cout << "\nBalance: " << balance;
    cout<<endl;
}

void CurrentAccount::calculateInterest() {
    cout << "\nNo interest for Current Accounts.";
}

// File handling functions prototypes
void createNewAccount();
void displayAccount(int);
void modifyAccount(int);
void deleteAccount(int);
void displayAllAccounts();
void depositOrWithdraw(int, int);

int main() {
    int choice;
    int accountNumber;

    do {
        cout << "\n\nBank Management System\n";
        cout << "1. Create Account\n";
        cout << "2. Deposit Amount\n";
        cout << "3. Withdraw Amount\n";
        cout << "4. Balance Enquiry\n";
        cout << "5. Apply Interest (Savings Accounts)\n";
        cout << "6. All Account Holders List\n";
        cout << "7. Close an Account\n";
        cout << "8. Modify an Account\n";
        cout << "9. Exit\n";
        cout << "Select Your Option (1-9): ";
        cin >> choice;

        switch (choice) {
        case 1:
            createNewAccount();
            break;
        case 2:
            cout << "\nEnter account number: ";
            cin >> accountNumber;
            depositOrWithdraw(accountNumber, 1);
            break;
        case 3:
            cout << "\nEnter account number: ";
            cin >> accountNumber;
            depositOrWithdraw(accountNumber, 2);
            break;
        case 4:
            cout << "\nEnter account number: ";
            cin >> accountNumber;
            displayAccount(accountNumber);
            break;
        case 5:
            cout << "\nEnter account number (Savings Account only): ";
            cin >> accountNumber;
            displayAccount(accountNumber);
            break;
        case 6:
            displayAllAccounts();
            break;
        case 7:
            cout << "\nEnter account number: ";
            cin >> accountNumber;
            deleteAccount(accountNumber);
            break;
        case 8:
            cout << "\nEnter account number: ";
            cin >> accountNumber;
            modifyAccount(accountNumber);
            break;
        case 9:
            cout << "\nThank you for using the Bank Management System!";
            break;
        default:
            cout << "\nInvalid choice!";
        }
    } while (choice != 9);

    return 0;
}

// Create a new account
void createNewAccount() {
    Account* account;
    char accType;
    ofstream outFile;

    cout << "\nEnter account type (S for Savings, C for Current): ";
    cin >> accType;
    accType = toupper(accType);

    if (accType == 'S') {
        account = new SavingsAccount();
    } else if (accType == 'C') {
        account = new CurrentAccount();
    } else {
        cout << "\nInvalid account type!";
        return;
    }

    account->createAccount();
    outFile.open("account.dat", ios::binary | ios::app);
    outFile.write(reinterpret_cast<char*>(&accType), sizeof(accType));  // Write account type first
    outFile.write(reinterpret_cast<char*>(account), sizeof(*account));
    outFile.close();
    delete account;
}

// Display an account
void displayAccount(int n) {
    Account* account = nullptr;
    char accType;
    bool found = false;
    ifstream inFile;
    inFile.open("account.dat", ios::binary);

    if (!inFile) {
        cout << "\nFile could not be opened!";
        return;
    }

    while (inFile.read(reinterpret_cast<char*>(&accType), sizeof(accType))) {
        if (accType == 'S') {
            account = new SavingsAccount();
        } else if (accType == 'C') {
            account = new CurrentAccount();
        }

        inFile.read(reinterpret_cast<char*>(account), sizeof(*account));
        if (account->getAccountNumber() == n) {
            account->showAccountDetails();
            found = true;
        }
        delete account;
    }
    inFile.close();
    if (!found) cout << "\nAccount number does not exist!";
}

// Modify an account
void modifyAccount(int n) {
    Account* account = nullptr;
    char accType;
    bool found = false;
    fstream File;
    File.open("account.dat", ios::binary | ios::in | ios::out);

    if (!File) {
        cout << "\nFile could not be opened!";
        return;
    }

    while (File.read(reinterpret_cast<char*>(&accType), sizeof(accType)) && !found) {
        if (accType == 'S') {
            account = new SavingsAccount();
        } else if (accType == 'C') {
            account = new CurrentAccount();
        }

        File.read(reinterpret_cast<char*>(account), sizeof(*account));

        if (account->getAccountNumber() == n) {
            account->showAccountDetails();
            cout << "\nModify account details:\n";
            account->createAccount();
            int pos = -static_cast<int>(sizeof(*account));
            File.seekp(pos, ios::cur);
            File.write(reinterpret_cast<char*>(&accType), sizeof(accType));
            File.write(reinterpret_cast<char*>(account), sizeof(*account));
            cout << "\nAccount details updated!";
            found = true;
        }
        delete account;
    }
    File.close();
    if (!found) cout << "\nAccount number not found!";
}

// Delete an account
void deleteAccount(int n) {
    Account* account = nullptr;
    char accType;
    ifstream inFile;
    ofstream outFile;

    inFile.open("account.dat", ios::binary);
    if (!inFile) {
        cout << "\nFile could not be opened!";
        return;
    }

    outFile.open("temp.dat", ios::binary);
    while (inFile.read(reinterpret_cast<char*>(&accType), sizeof(accType))) {
        if (accType == 'S') {
            account = new SavingsAccount();
        } else if (accType == 'C') {
            account = new CurrentAccount();
        }

        inFile.read(reinterpret_cast<char*>(account), sizeof(*account));
        if (account->getAccountNumber() != n) {
            outFile.write(reinterpret_cast<char*>(&accType), sizeof(accType));
            outFile.write(reinterpret_cast<char*>(account), sizeof(*account));
        }
        delete account;
    }
}
void depositOrWithdraw(int accountNumber, int option) {
    Account* account = nullptr;
    char accType;
    bool found = false;
    fstream File;
    File.open("account.dat", ios::binary | ios::in | ios::out);

    if (!File) {
        cout << "\nFile could not be opened!";
        return;
    }

    while (File.read(reinterpret_cast<char*>(&accType), sizeof(accType))) {
        if (accType == 'S') {
            account = new SavingsAccount();
        } else if (accType == 'C') {
            account = new CurrentAccount();
        }

        File.read(reinterpret_cast<char*>(account), sizeof(*account));

        if (account->getAccountNumber() == accountNumber) {
            found = true;
            account->showAccountDetails();

            if (option == 1) {
                double amount;
                cout << "\nEnter the amount to deposit: ";
                cin >> amount;
                account->deposit(amount);
            } else if (option == 2) {
                double amount;
                cout << "\nEnter the amount to withdraw: ";
                cin >> amount;
                account->withdraw(amount);
            }

            // Move the file pointer back to the start of this account record
            int pos = -static_cast<int>(sizeof(accType) + sizeof(*account));
            File.seekp(pos, ios::cur);  // Move the write pointer back

            // Write the account type and updated account object
            File.write(reinterpret_cast<char*>(&accType), sizeof(accType));
            File.write(reinterpret_cast<char*>(account), sizeof(*account));
            cout << "\nAccount updated!";
            break; // Exit after updating
        }

        delete account;
    }

    File.close();

    if (!found) {
        cout << "\nAccount number not found!";
    }
}


void displayAllAccounts() {
    Account* account = nullptr;
    char accType;
    ifstream inFile;
    inFile.open("account.dat", ios::binary);
    if (!inFile) {
        cout << "\nFile could not be opened!";
        return;
    }

    cout << "\n\nAccount Holder List\n\n";
    cout << "====================================================\n";
    cout << "A/c No.      NAME           Type    Balance\n";
    cout << "====================================================\n";
    while (inFile.read(reinterpret_cast<char*>(&accType), sizeof(accType))) {  // Read account type first
        if (accType == 'S') {
            account = new SavingsAccount();
        } else if (accType == 'C') {
            account = new CurrentAccount();
        }

        inFile.read(reinterpret_cast<char*>(account), sizeof(*account));
        account->showAccountDetails();
        delete account;
    }
    inFile.close();
}

