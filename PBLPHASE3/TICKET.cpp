
// ==============================================================================================================================================================
//                TICKET CLASS
// ========================================

class Ticket
{
private:
    int pnr;
    Passenger passenger;
    Train train;
    int seatNumber;
    double fare;

public:
    Ticket()
    {
        pnr = 0;
        seatNumber = 0;
        fare = 0;
    }

    Ticket(int p, Passenger person, Train t, int seat)
    {
        pnr = p;
        passenger = person;
        train = t;
        seatNumber = seat;

        calculateFare();
    }

    void calculateFare()
    {
        fare = train.getDistance() * 3;
    }

    int getPNR() const
    {
        return pnr;
    }

    double getFare() const
    {
        return fare;
    }

    int getSeatNumber() const
    {
        return seatNumber;
    }

    int getTrainNumber() const
    {
        return train.getTrainNumber();
    }

    void displayTicket() const
    {
        cout << "\n================================\n";
        cout << "          TRAIN TICKET\n";
        cout << "================================\n";

        cout << "PNR Number: " << pnr << endl;

        cout << "Passenger ID: "
             << passenger.getPassengerId() << endl;

        cout << "Passenger Name: "
             << passenger.getPassengerName() << endl;

        cout << "Train Number: "
             << train.getTrainNumber() << endl;

        cout << "Train Name: "
             << train.getTrainName() << endl;

        cout << "Source: "
             << train.getSource() << endl;

        cout << "Destination: "
             << train.getDestination() << endl;

        cout << "Seat Number: "
             << seatNumber << endl;

        cout << "Distance: "
             << train.getDistance() << " km" << endl;

        cout << "Fare: Rs. "
             << fare << endl;

        cout << "================================\n";
    }

    // Save one ticket to a file
    void saveToFile(ofstream &out) const
    {
        out << pnr << " "<< passenger.getPassengerId() << " "
            << quoted(passenger.getPassengerName()) << " "
            << train.getTrainNumber() << " "<< quoted(train.getTrainName()) << " "
            << quoted(train.getSource()) << " "<< quoted(train.getDestination()) << " "
            << train.getDistance() << " "<< seatNumber << " "<< fare << endl;
    }

    // Read one ticket from a file
    bool loadFromFile(ifstream &in)
    {
        int pid, trainNo;
        string passengerName, trainName;
        string src, dest;
        double km, savedFare;
        int seat;

        if (!(in >> pnr >> pid >> quoted(passengerName) >> trainNo>> quoted(trainName)
                >> quoted(src) >> quoted(dest) >> km >> seat >> savedFare))
        {
            return false;
        }

        passenger = Passenger(pid, passengerName);

        train = Train(trainNo, trainName,src, dest, km);

        seatNumber = seat;
        fare = savedFare;

        return true;
    }
    Passenger getPassenger() const
    {
        return passenger;
    }

    Train getTrain() const
    {
        return train;
    }
};