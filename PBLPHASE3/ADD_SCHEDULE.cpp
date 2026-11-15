// ==============================================================================================================================================================
//                ADD SCHEDULE CLASS
// ========================================

class ScheduleManager
{
private:
    vector<TrainSchedule> schedules;

public:

    // Add a new train schedule
    void addSchedule()
    {
        int trainNo;
        string departure, arrival, days;

        cout << "\nEnter Train Number: ";
        cin >> trainNo;

        if (trainNo <= 0)
        {
            cout << "Invalid Train Number!\n";
            return;
        }

        // Check duplicate train number
        for (int i = 0; i < schedules.size(); i++)
        {
            if (schedules[i].getTrainNo() == trainNo)
            {
                cout << "Schedule already exists!\n";
                return;
            }
        }

        cout << "Enter Departure Time (HH:MM): ";
        cin >> departure;

        cout << "Enter Arrival Time (HH:MM): ";
        cin >> arrival;

        cout << "Enter Running Days: ";
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        getline(cin, days);

        TrainSchedule s(trainNo, departure, arrival, days);

        schedules.push_back(s);

        cout << "Schedule added successfully!\n";
    }

    // Display all schedules
    void displaySchedules()
    {
        if (schedules.empty())
        {
            cout << "\nNo schedules available!\n";
            return;
        }

        for (int i = 0; i < schedules.size(); i++)
        {
            schedules[i].displaySchedule();
        }
    }

    // Search schedule by train number
    void searchSchedule()
    {
        int trainNo;

        cout << "\nEnter Train Number to search: ";
        cin >> trainNo;

        for (int i = 0; i < schedules.size(); i++)
        {
            if (schedules[i].getTrainNo() == trainNo)
            {
                cout << "\nSchedule Found!\n";
                schedules[i].displaySchedule();
                return;
            }
        }

        cout << "Schedule not found!\n";
    }

    // Save schedules to file
    void saveSchedules()
    {
        ofstream file("schedules.txt");

        for (int i = 0; i < schedules.size(); i++)
        {
            schedules[i].saveToFile(file);
        }

        file.close();
    }

    //loading funtion schedule
        void loadSchedules()
    {
        ifstream file("schedules.txt");

        if (!file)
        {
            return;
        }

        schedules.clear();

        while (file.peek() != EOF)
        {
            int no;
            string departure, arrival, days;

            if (!(file >> no >> departure >> arrival))
                break;

            file.ignore(numeric_limits<streamsize>::max(), '\n');

            if (!getline(file, days))
                break;

            TrainSchedule s(no, departure, arrival, days);
            schedules.push_back(s);
        }

        file.close();
    }
};