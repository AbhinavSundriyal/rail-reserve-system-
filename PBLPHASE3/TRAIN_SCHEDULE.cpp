// ==============================================================================================================================================================
//                TRAIN SCHEDULE CLASS
// ========================================

class TrainSchedule
{
private:
    int trainNo;
    string departureTime;
    string arrivalTime;
    string runningDays;

public:
    TrainSchedule()
    {
        trainNo = 0;
    }

    TrainSchedule(int no, string departure,string arrival, string days)
    {
        trainNo = no;
        departureTime = departure;
        arrivalTime = arrival;
        runningDays = days;
    }

    int getTrainNo() const
    {
        return trainNo;
    }

    void displaySchedule() const
    {
        cout << "\nTrain Number: " << trainNo;
        cout << "\nDeparture Time: " << departureTime;
        cout << "\nArrival Time: " << arrivalTime;
        cout << "\nRunning Days: " << runningDays;
        cout << "\n-----------------------------\n";
    }

    void saveToFile(ofstream &file) const
    {
        file << trainNo << " " << departureTime << " " << arrivalTime << " " << runningDays << "\n";
    }

    static TrainSchedule loadFromFile(ifstream &file)
    {
        int no;
        string departure, arrival, days;

        file >> no >> departure >> arrival;
        file.ignore(numeric_limits<streamsize>::max(), '\n');
        getline(file, days);

        return TrainSchedule(no, departure, arrival, days);
    }
};