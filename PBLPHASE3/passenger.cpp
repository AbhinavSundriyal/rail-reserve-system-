#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <iomanip>
#include <limits>
#include <queue>
#include <map>
#include <climits>
#include <functional>
#include <algorithm>

using namespace std;

// ================================================================================================================
//             PASSENGER CLASS
// ========================================

class Passenger
{
private:
    int passengerId;
    string passengerName;

public:
    Passenger()
    {
        passengerId = 0;
        passengerName = "";
    }

    Passenger(int id, string name)
    {
        passengerId = id;
        passengerName = name;
    }

    int getPassengerId() const
    {
        return passengerId;
    }

    string getName() const
    {
        return passengerName;
    }

    string getPassengerName() const
    {
        return passengerName;
    }
};




