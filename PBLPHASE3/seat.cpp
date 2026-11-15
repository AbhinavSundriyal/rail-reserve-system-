
// ========================================================================================================================================================
//             SEAT MANAGER CLASS
// ========================================

class SeatManager
{
private:
    map<int, vector<bool>> seats;
    int totalSeats;

public:
    SeatManager()
    {
        totalSeats = 50;
    }

    // Check whether a seat is available
    bool isAvailable(int trainNo, int seatNo)
    {
        if (seatNo < 1 || seatNo > totalSeats)
            return false;

        if (seats.find(trainNo) == seats.end())
        {
            seats[trainNo] = vector<bool>(totalSeats + 1, false);
        }

        return !seats[trainNo][seatNo];
    }

    // Book a seat
    bool bookSeat(int trainNo, int seatNo)
    {
        if (!isAvailable(trainNo, seatNo))
        {
            return false;
        }

        seats[trainNo][seatNo] = true;
        return true;
    }

    // Release a seat after cancellation
    void releaseSeat(int trainNo, int seatNo)
    {
        if (seats.find(trainNo) != seats.end() &&
            seatNo >= 1 && seatNo <= totalSeats)
        {
            seats[trainNo][seatNo] = false;
        }
    }

    // Display available seats
    void displayAvailableSeats(int trainNo)
    {
        if (seats.find(trainNo) == seats.end())
        {
            seats[trainNo] =
                vector<bool>(totalSeats + 1, false);
        }

        cout << "\nAvailable Seats for Train "
             << trainNo << ":\n";

        bool found = false;

        for (int i = 1; i <= totalSeats; i++)
        {
            if (!seats[trainNo][i])
            {
                cout << i << " ";
                found = true;
            }
        }

        if (!found)
        {
            cout << "No seats available!";
        }

        cout << endl;
    }
};