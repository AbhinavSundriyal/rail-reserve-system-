
// ==============================================================================================================================================================
//                TRANSACTION CLASS
// ========================================

class Transaction
{
private:
    int transactionID;
    int pnr;
    double amount;
    string paymentMethod;
    string status;

public:

    Transaction()
    {
        transactionID = 0;
        pnr = 0;
        amount = 0;
        status = "Pending";
    }

    Transaction(int tid, int p, double amt,string method, string stat)
    {
        transactionID = tid;
        pnr = p;
        amount = amt;
        paymentMethod = method;
        status = stat;
    }

    int getPNR() const
    {
        return pnr;
    }

    void updateStatus(string newStatus)
    {
        status = newStatus;
    }

    void displayTransaction() const
    {
        cout << "\nTransaction ID: " << transactionID;
        cout << "\nPNR: " << pnr;
        cout << "\nAmount: Rs. " << amount;
        cout << "\nPayment Method: " << paymentMethod;
        cout << "\nStatus: " << status;
        cout << "\n-----------------------------\n";
    }

    void saveToFile(ofstream &file) const
    {
        file << transactionID << " "<< pnr << " "<< amount << " "<< paymentMethod << " "
             << status << "\n";
    }
};