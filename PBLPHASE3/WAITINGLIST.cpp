// ===========================================================================================================================================================
//             WAITING LIST CLASS
// ========================================

class WaitingList
{
private:
    struct WaitingPassenger
    {
        Passenger passenger;
        Train train;
    };

    map<int, queue<WaitingPassenger>> waitingQueues;

public:

    // Add passenger to waiting list
    void addPassenger(Passenger p, Train t)
    {
        int trainNo = t.getTrainNumber();

        WaitingPassenger wp;
        wp.passenger = p;
        wp.train = t;

        waitingQueues[trainNo].push(wp);

        cout << "\nPassenger added to waiting list!\n";
    }

    // Check waiting list
    bool hasWaitingPassenger(int trainNo)
    {
        return !waitingQueues[trainNo].empty();
    }

    // Get next waiting passenger
    Passenger getNextPassenger(int trainNo)
    {
        Passenger p =
            waitingQueues[trainNo].front().passenger;

        waitingQueues[trainNo].pop();

        return p;
    }

    // Get complete details of next waiting passenger
    bool getNextWaitingPassenger(int trainNo, Passenger &p, Train &t)
    {
        if (!hasWaitingPassenger(trainNo))
        {
            return false;
        }

        p = waitingQueues[trainNo].front().passenger;
        t = waitingQueues[trainNo].front().train;

        waitingQueues[trainNo].pop();
        saveWaitingList();

        return true;
    }

    //save waiting list
    void saveWaitingList()
    {
        ofstream file("waitinglist.txt");

        for (auto it = waitingQueues.begin();
         it != waitingQueues.end(); it++)
        {
            int trainNo = it->first;

            queue<WaitingPassenger> temp = it->second;

            while (!temp.empty())
            {
                Passenger p = temp.front().passenger;
                Train t = temp.front().train;

                file << p.getPassengerId() << " "<< p.getPassengerName() << " "
                     << t.getTrainNumber() << " "<< t.getTrainName() << " "<< t.getSource() << " "
                     << t.getDestination() << " " << t.getDistance() << endl;

                temp.pop();
            }
        }   

        file.close();

        cout << "\nWaiting list saved successfully!\n";
    }
    
    //load waiting list
    void loadWaitingList()
    {
        ifstream file("waitinglist.txt");

        if (!file)
        {
            return;
        }

        waitingQueues.clear();

        int passengerId, trainNo;
        string passengerName, trainName;
        string source, destination;
        double distance;

        while (file >> passengerId>> passengerName >> trainNo>> trainName >> source
            >> destination>> distance)
        {
            Passenger p(passengerId, passengerName);

            Train t(trainNo, trainName, source,
                destination, distance);

            WaitingPassenger wp;
            wp.passenger = p;
            wp.train = t;

            waitingQueues[trainNo].push(wp);

            saveWaitingList();
            cout << "Passenger added to waiting list \n";
        }

        file.close();

        cout << "\nWaiting list loaded successfully!\n";
    }

    // Display waiting list
    void displayWaitingList(int trainNo)
    {
        if (waitingQueues[trainNo].empty())
        {
            cout << "\nWaiting list is empty!\n";
            return;
        }

        queue<WaitingPassenger> temp =
            waitingQueues[trainNo];

        cout << "\n===== WAITING LIST =====\n";

        while (!temp.empty())
        {
            cout << "Passenger ID: "
                 << temp.front().passenger.getPassengerId()
                 << endl;

            cout << "Passenger Name: "
                 << temp.front().passenger.getPassengerName()
                 << endl;

            temp.pop();
        }
    }
};