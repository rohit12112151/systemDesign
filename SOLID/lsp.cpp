//lsp violated


#include <iostream>
using namespace std;

class Account {
public:
    virtual void deposit(double amount) = 0;
    virtual void withdraw(double amount) = 0;
};

class SavingAccount : public Account {
    private:
        double balance;

    public:
        SavingAccount() { 
            balance = 0; 
        }

        void deposit(double amount) {
            balance += amount;
            cout << "Deposited: " << amount << " in Savings Account. New Balance: " << balance << endl;
        }

        void withdraw(double amount) {
            if (balance >= amount) {
                balance -= amount;
                cout << "Withdrawn: " << amount << " from Savings Account. New Balance: " << balance << endl;
            } else {
                cout << "Insufficient funds in Savings Account!\n";
            }
        }
};

class CurrentAccount : public Account {
    private:
        double balance;

    public:
        CurrentAccount() { 
            balance = 0; 
        }

        void deposit(double amount) {
            balance += amount;
            cout << "Deposited: " << amount << " in Current Account. New Balance: " << balance << endl;
        }

        void withdraw(double amount) {
            if (balance >= amount) {
                balance -= amount;
                cout << "Withdrawn: " << amount << " from Current Account. New Balance: " << balance << endl;
            } else {
                cout << "Insufficient funds in Current Account!\n";
            }
        }
};

class FixedTermAccount : public Account {
    private:
        double balance;

    public:
        FixedTermAccount() { 
            balance = 0; 
        }

        void deposit(double amount) {
            balance += amount;
            cout << "Deposited: " << amount << " in Fixed Term Account. New Balance: " << balance << endl;
        }

        void withdraw(double amount) {
            throw logic_error("Withdrawal not allowed in Fixed Term Account!");
        }
};
    
class BankClient {
    private:
        vector<Account*> accounts;

    public:
        BankClient(vector<Account*> accounts) { 
            this->accounts = accounts; 
        }

        void processTransactions() {
            for (Account* acc : accounts) {
                acc->deposit(1000);  //All accounts allow deposits

                //Assuming all accounts support withdrawal (LSP Violation)
                try {
                    acc->withdraw(500);
                } catch (const logic_error& e) {
                    cout << "Exception: " << e.what() << endl;
                }
            }
        }
};

int main() {
    vector<Account*> accounts;
    accounts.push_back(new SavingAccount());
    accounts.push_back(new CurrentAccount());
    accounts.push_back(new FixedTermAccount());

    BankClient* client = new BankClient(accounts);
    client->processTransactions(); //  Throws exception when withdrawing from FixedTermAccount

    return 0;
}





//lsp followed
#include <iostream>
#include <vector>
#include <typeinfo>
#include <stdexcept>

using namespace std;


class DepositOnlyAccount {
    public:
        virtual void deposit(double amount) = 0;
};

class WithdrawableAccount : public DepositOnlyAccount {
    public:
        virtual void withdraw(double amount) = 0;
};

class SavingAccount : public WithdrawableAccount {
    private:
        double balance;

    public:
        SavingAccount() { 
            balance = 0; 
        }

        void deposit(double amount) {
            balance += amount;
            cout << "Deposited: " << amount << " in Savings Account. New Balance: " << balance << endl;
        }

        void withdraw(double amount) {
            if (balance >= amount) {
                balance -= amount;
                cout << "Withdrawn: " << amount << " from Savings Account. New Balance: " << balance << endl;
            } else {
                cout << "Insufficient funds in Savings Account!\n";
            }
        }
};

class CurrentAccount : public WithdrawableAccount {
    private:
        double balance;
    
    public:
        CurrentAccount() { 
            balance = 0; 
        }
    
        void deposit(double amount) {
            balance += amount;
            cout << "Deposited: " << amount << " in Current Account. New Balance: " << balance << endl;
        }
    
        void withdraw(double amount) {
            if (balance >= amount) {
                balance -= amount;
                cout << "Withdrawn: " << amount << " from Current Account. New Balance: " << balance << endl;
            } else {
                cout << "Insufficient funds in Current Account!\n";
            }
        }
    };

class FixedTermAccount : public DepositOnlyAccount {
    private:
        double balance;

    public:
        FixedTermAccount() { 
            balance = 0; 
        }

        void deposit(double amount) {
            balance += amount;
            cout << "Deposited: " << amount << " in Fixed Term Account. New Balance: " << balance << endl;
        }
};

class BankClient {
    private:
        vector<WithdrawableAccount*> withdrawableAccounts;
        vector<DepositOnlyAccount*> depositOnlyAccounts;

    public:
        BankClient( vector<WithdrawableAccount*> withdrawableAccounts, 
            vector<DepositOnlyAccount*> depositOnlyAccounts) { 
            this->withdrawableAccounts = withdrawableAccounts; 
            this->depositOnlyAccounts = depositOnlyAccounts;
        }

        void processTransactions() {
            for (WithdrawableAccount* acc : withdrawableAccounts) {
                acc->deposit(1000);
                acc->withdraw(500); 
            }
            for (DepositOnlyAccount* acc : depositOnlyAccounts) {
                acc->deposit(5000);
            }
        }
};

int main() {
    vector<WithdrawableAccount*> withdrawableAccounts;
    withdrawableAccounts.push_back(new SavingAccount());
    withdrawableAccounts.push_back(new CurrentAccount());

    vector<DepositOnlyAccount*> depositOnlyAccounts;
    depositOnlyAccounts.push_back(new FixedTermAccount());

    BankClient* client = new BankClient (withdrawableAccounts, depositOnlyAccounts);
    client->processTransactions();

    return 0;
}

    
    
    