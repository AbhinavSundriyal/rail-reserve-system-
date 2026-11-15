// ==============================================================================================================================================================
//                PAYMENT CLASS
// ========================================

class Payment
{
private:
    double amount;
    string paymentMethod;
    bool paymentStatus;

public:

    Payment()
    {
        amount = 0;
        paymentStatus = false;
    }

    Payment(double amt, string method)
    {
        amount = amt;
        paymentMethod = method;
        paymentStatus = true;
    }

    double getAmount() const
    {
        return amount;
    }

    string getPaymentMethod() const
    {
        return paymentMethod;
    }

    bool getPaymentStatus() const
    {
        return paymentStatus;
    }

    void displayPayment() const
    {
        cout << "\nPayment Amount: Rs. " << amount;
        cout << "\nPayment Method: " << paymentMethod;
        cout << "\nPayment Status: "
             << (paymentStatus ? "Successful" : "Pending");
        cout << "\n";
    }
};