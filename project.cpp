#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
using namespace std;

class BankAccount {
    int accountNumber;
    string name;
    char accountType;
    double balance;

public:
    void createAccount();
    void showAccount() const;
    void deposit(double);
    void withdraw(double);
    void report() const;
    int getAccountNumber() const;
    double getBalance() const;
    char getAccountType() const;
};

// Function to create a new account
void BankAccount::createAccount() {
    cout << "\nEnter the account number: ";
    cin >> accountNumber;
    cout << "\nEnter the account holder name: ";
    cin.ignore();
    getline(cin, name);
    cout << "\nEnter account type (S for Saving, C for Current): ";
    cin >> accountType;
    accountType = toupper(accountType);
    cout << "\nEnter initial amount (>=500 for Saving, >=1000 for Current): ";
    cin >> balance;
    cout << "\nAccount created successfully!\n";
}

// Function to display account details
void BankAccount::showAccount() const {
    cout << "\nAccount Number: " << accountNumber;
    cout << "\nAccount Holder: " << name;
    cout << "\nAccount Type: " << accountType;
    cout << "\nBalance: " << balance;
}

// Function to deposit money into the account
void BankAccount::deposit(double amount) {
    balance += amount;
    cout << "\nAmount deposited successfully!";
}

// Function to withdraw money from the account
void BankAccount::withdraw(double amount) {
    if (amount <= balance) {
        balance -= amount;
        cout << "\nAmount withdrawn successfully!";
    } else {
        cout << "\nInsufficient balance!";
    }
}

// Function to display account report
void BankAccount::report() const {
    cout << accountNumber << setw(20) << name << setw(10) << accountType << setw(10) << balance << endl;
}

// Getters
int BankAccount::getAccountNumber() const {
    return accountNumber;
}

double BankAccount::getBalance() const {
    return balance;
}

char BankAccount::getAccountType() const {
    return accountType;
}

// Function to write account data to file
void writeAccount();
void displayAccount(int);
void modifyAccount(int);
void deleteAccount(int);
void displayAllAccounts();
void depositWithdraw(int, int);

int main() {
    int choice;
    int accountNumber;

    do {
        cout << "\n\nBank Management System\n";
        cout << "1. Create Account\n";
        cout << "2. Deposit Amount\n";
        cout << "3. Withdraw Amount\n";
        cout << "4. Balance Enquiry\n";
        cout << "5. All Account Holders List\n";
        cout << "6. Close an Account\n";
        cout << "7. Modify an Account\n";
        cout << "8. Exit\n";
        cout << "Select Your Option (1-8): ";
        cin >> choice;

        switch (choice) {
        case 1:
            writeAccount();
            break;
        case 2:
            cout << "\nEnter account number: ";
            cin >> accountNumber;
            depositWithdraw(accountNumber, 1);
            break;
        case 3:
            cout << "\nEnter account number: ";
            cin >> accountNumber;
            depositWithdraw(accountNumber, 2);
            break;
        case 4:
            cout << "\nEnter account number: ";
            cin >> accountNumber;
            displayAccount(accountNumber);
            break;
        case 5:
            displayAllAccounts();
            break;
        case 6:
            cout << "\nEnter account number: ";
            cin >> accountNumber;
            deleteAccount(accountNumber);
            break;
        case 7:
            cout << "\nEnter account number: ";
            cin >> accountNumber;
            modifyAccount(accountNumber);
            break;
        case 8:
            cout << "\nThank you for using the Bank Management System!";
            break;
        default:
            cout << "\nInvalid choice!";
        }
    } while (choice != 8);

    return 0;
}

// Function to write account to file
void writeAccount() {
    BankAccount account;
    ofstream outFile;
    outFile.open("account.dat", ios::binary | ios::app);
    account.createAccount();
    outFile.write(reinterpret_cast<char *>(&account), sizeof(BankAccount));
    outFile.close();
}

// Function to display account details
void displayAccount(int accountNumber) {
    BankAccount account;
    ifstream inFile;
    bool found = false;
    inFile.open("account.dat", ios::binary);
    if (!inFile) {
        cout << "\nFile could not be opened. Press any key to continue...";
        return;
    }

    while (inFile.read(reinterpret_cast<char *>(&account), sizeof(BankAccount))) {
        if (account.getAccountNumber() == accountNumber) {
            account.showAccount();
            found = true;
            break;
        }
    }
    inFile.close();
    if (!found)
        cout << "\nAccount not found!";
}

// Function to modify an account
void modifyAccount(int accountNumber) {
    BankAccount account;
    fstream file;
    bool found = false;
    file.open("account.dat", ios::binary | ios::in | ios::out);
    if (!file) {
        cout << "\nFile could not be opened. Press any key to continue...";
        return;
    }

    while (!file.eof() && found == false) {
        file.read(reinterpret_cast<char *>(&account), sizeof(BankAccount));
        if (account.getAccountNumber() == accountNumber) {
            account.showAccount();
            cout << "\n\nEnter new details: ";
            account.createAccount();
            int pos = (-1) * static_cast<int>(sizeof(BankAccount));
            file.seekp(pos, ios::cur);
            file.write(reinterpret_cast<char *>(&account), sizeof(BankAccount));
            cout << "\nAccount updated successfully!";
            found = true;
        }
    }
    file.close();
    if (!found)
        cout << "\nAccount not found!";
}

// Function to delete an account
void deleteAccount(int accountNumber) {
    BankAccount account;
    ifstream inFile;
    ofstream outFile;
    inFile.open("account.dat", ios::binary);
    if (!inFile) {
        cout << "\nFile could not be opened. Press any key to continue...";
        return;
    }

    outFile.open("temp.dat", ios::binary);
    while (inFile.read(reinterpret_cast<char *>(&account), sizeof(BankAccount))) {
        if (account.getAccountNumber() != accountNumber) {
            outFile.write(reinterpret_cast<char *>(&account), sizeof(BankAccount));
        }
    }
    inFile.close();
    outFile.close();
    remove("account.dat");
    rename("temp.dat", "account.dat");
    cout << "\nAccount deleted successfully!";
}

// Function to display all accounts
void displayAllAccounts() {
    BankAccount account;
    ifstream inFile;
    inFile.open("account.dat", ios::binary);
    if (!inFile) {
        cout << "\nFile could not be opened. Press any key to continue...";
        return;
    }

    cout << "\n\nAccount Holder List\n\n";
    cout << "====================================================\n";
    cout << "A/c No.      NAME           Type    Balance\n";
    cout << "====================================================\n";
    while (inFile.read(reinterpret_cast<char *>(&account), sizeof(BankAccount))) {
        account.report();
    }
    inFile.close();
}

// Function to deposit or withdraw money
void depositWithdraw(int accountNumber, int option) {
    BankAccount account;
    fstream file;
    bool found = false;
    double amount;
    file.open("account.dat", ios::binary | ios::in | ios::out);
    if (!file) {
        cout << "\nFile could not be opened. Press any key to continue...";
        return;
    }

    while (!file.eof() && found == false) {
        file.read(reinterpret_cast<char *>(&account), sizeof(BankAccount));
        if (account.getAccountNumber() == accountNumber) {
            account.showAccount();
            if (option == 1) {
                cout << "\n\nEnter amount to deposit: ";
                cin >> amount;
                account.deposit(amount);
            } else if (option == 2) {
                cout << "\n\nEnter amount to withdraw: ";
                cin >> amount;
                account.withdraw(amount);
            }
            int pos = (-1) * static_cast<int>(sizeof(BankAccount));
            file.seekp(pos, ios::cur);
            file.write(reinterpret_cast<char *>(&account), sizeof(BankAccount));
            found = true;
        }
    }
    file.close();
    if (!found)
        cout << "\nAccount not found!";
}
