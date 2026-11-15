
// ================================================================================================================
//                TRAIN CLASS
// ========================================

class Train
{
private:
    int trainNumber;
    string trainName;
    string source;
    string destination;
    double distance;

public:
    Train()
    {
        trainNumber = 0;
        trainName = "";
        source = "";
        destination = "";
        distance = 0;
    }

    Train(int number, string name, string src,string dest, double km)
    {
        trainNumber = number;
        trainName = name;
        source = src;
        destination = dest;
        distance = km;
    }

    int getTrainNumber() const
    {
        return trainNumber;
    }

    string getTrainName() const
    {
        return trainName;
    }

    string getSource() const
    {
        return source;
    }

    string getDestination() const
    {
        return destination;
    }

    double getDistance() const
    {
        return distance;
    }
};