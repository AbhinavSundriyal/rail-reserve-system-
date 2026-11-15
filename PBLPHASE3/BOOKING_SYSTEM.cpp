
// ===============================================================================================================================================
//            BOOKING SYSTEM CLASS
// ========================================

class BookingSystem
{
private:
    vector<Ticket> tickets;
    int nextPNR;

    SeatManager seatManager;
    WaitingList waitingList;
    RouteGraph routeGraph;
    ScheduleManager scheduleManager;

    vector<Transaction> transactions;
    int nextTransactionID = 1;

public:
    BookingSystem()
    {
        nextPNR = 1001;
        loadBookings();
        scheduleManager.loadSchedules();
        loadTransactions();
        routeGraph.loadRoutes();
        waitingList.loadWaitingList();
    }

    // Save all bookings
    void saveBookings() const
    {
        ofstream out("bookings.txt");

        if (!out)
        {
            cout << "Error saving booking records!\n";
            return;
        }

        for (int i = 0; i < tickets.size(); i++)
        {
            tickets[i].saveToFile(out);
        }

        out.close();
    }

    // Load bookings when program starts
    void loadBookings()
    {
        ifstream in("bookings.txt");

        if (!in)
        {
            return;
        }

        Ticket t;

        while (t.loadFromFile(in))
        {
            tickets.push_back(t);
            seatManager.bookSeat(t.getTrainNumber(),t.getSeatNumber());

            if (t.getPNR() >= nextPNR)
            {
                nextPNR = t.getPNR() + 1;
            }

            t = Ticket();
        }

        in.close();
    }

    // Book a ticket
    void bookTicket()
    {
        int pid, trainNo, seat;
        double km;

        string passengerName;
        string trainName;
        string source;
        string destination;

        cout << "\nEnter Passenger ID: ";

        if (!(cin >> pid) || pid <= 0)
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid Passenger ID!\n";
            return;
        }

        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        for (int i = 0; i < tickets.size(); i++)
        {
            if (tickets[i].getPassenger().getPassengerId() == pid)
            {
                cout << "\nPassenger ID already exists!\n";
                return;
            }
        }

        cout << "Enter Passenger Name: ";
        getline(cin, passengerName);

        if (passengerName.empty())
        {
            cout << "Passenger name cannot be empty!\n";
            return;
        }

        cout << "Enter Train Number: ";

        if (!(cin >> trainNo) || trainNo <= 0)
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid Train Number!\n";
            return;
        }

        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        cout << "Enter Train Name: ";
        getline(cin, trainName);

        cout << "Enter Source Station: ";
        getline(cin, source);

        cout << "Enter Destination Station: ";
        getline(cin, destination);

        if (trainName.empty() || source.empty() ||
        destination.empty() || source == destination)
        {
            cout << "Invalid train or station details!\n";
            return;
        }

        // Find shortest route using Dijkstra's Algorithm
        int totalDistance = 0;
        vector<string> route;

        if (!routeGraph.findShortestRoute(
                source, destination, totalDistance, route))
        {
            cout << "No route found between these stations!\n";
            return;
        }

        km = totalDistance;

        cout << "\nShortest Route: ";

        for (int i = 0; i < route.size(); i++)
        {
            cout << route[i];

            if (i != route.size() - 1)
                cout << " -> ";
        }

        cout << "\nTotal Distance: " << km << " km\n";

        // Calculate fare
        cout << "Fare: Rs. " << km * 3 << endl;

        cout << "\nEnter Seat Number: ";

        if (!(cin >> seat) || seat < 1 || seat > 50)
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid seat number! Enter a seat between 1 and 50.\n";
            return;
        }

        Passenger p(pid, passengerName);
        Train t(trainNo, trainName, source, destination, km);

        // Check seat availability
        if (!seatManager.isAvailable(trainNo, seat))
        {
            cout << "This seat is already booked!\n";

            cout << "Do you want to join the waiting list? (1 for Yes, 0 for No): ";

            int choice;

            cout << "Enter your choice (1 for Yes, 0 for No): ";

            if (!(cin >> choice) || (choice != 0 && choice != 1))
            {   
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');

                    cout << "Invalid choice!\n";
                    return;
            }

            if (choice == 1)
            {
                waitingList.addPassenger(p, t);
                cout << "Added to waiting list successfully!\n";
            }

            return;
        }

        // Book the seat
        seatManager.bookSeat(trainNo, seat);

        // Create ticket
        Ticket newTicket(nextPNR, p, t, seat);

        tickets.push_back(newTicket);

        cout << "\nTicket booked successfully!\n";

        tickets.back().displayTicket();

        nextPNR++;

        saveBookings();
    }

    //payment processing
    void processPayment()
    {
        int pnr;
        cout << "\nEnter Ticket PNR: ";
        cin >> pnr;

        bool found = false;

        for (int i = 0; i < tickets.size(); i++)
        {
            if (tickets[i].getPNR() == pnr)
            {
                found = true;

                double amount = tickets[i].getFare();
                cout << "\nTicket Fare: Rs. " << amount;
                cout << "\nSelect Payment Method:";
                cout << "\n1. Cash";
                cout << "\n2. UPI";
                cout << "\n3. Card";
                cout << "\nEnter choice: ";

                int choice;
                cin >> choice;

                string method;

                switch (choice)
                {
                    case 1:
                        method = "Cash";
                        break;

                    case 2:
                        method = "UPI";
                        break;

                    case 3:
                        method = "Card";
                        break;

                    default:
                        cout << "Invalid payment method!\n";
                        return;
                }

                Payment payment(amount, method);

                Transaction transaction(
                    nextTransactionID,
                    pnr,
                    amount,
                    method,
                    "Successful"
                );

                transactions.push_back(transaction);
                saveTransactions();

                cout << "\nPayment successful!\n";
                payment.displayPayment();

                transaction.displayTransaction();

                nextTransactionID++;

                return;
            }
        }

        if (!found)
        {
            cout << "\nTicket PNR not found!\n";
        }
    }

    //display transaction history
    void displayTransactionHistory()
    {
        if (transactions.empty())
        {
            cout << "\nNo transaction history available!\n";
            return;
        }

        cout<< "\n========== TRANSACTION HISTORY ==========\n";

        for (int i = 0; i < transactions.size(); i++)
        {
            transactions[i].displayTransaction();
            cout << "-----------------------------------\n";
        }
    }

    //Save transaction
    void saveTransactions()
    {
        ofstream file("transactions.txt");

        if (!file)
        {
            cout << "\nError saving transactions!\n";
            return;
        }

        for (int i = 0; i < transactions.size(); i++)
        {
            transactions[i].saveToFile(file);
        }

        file.close();
    }

    //update transactions status
    void updateTransactionStatus(int cancelPNR)
    {
        for (int i = 0; i < transactions.size(); i++)
        {
            if (transactions[i].getPNR() == cancelPNR)
            {
                transactions[i].updateStatus("Cancelled");

                saveTransactions();

                cout << "\nTransaction status updated to Cancelled!\n";
                return;
            }
        }

        cout << "\nNo transaction found for this PNR.\n";
    }

    //Load transactions
    void loadTransactions()
    {
        ifstream file("transactions.txt");

        if (!file)
        {
            return;
        }

        transactions.clear();

        int id, pnr;
        double amount;
        string method, status;

        while (file >> id >> pnr >> amount >> method >> status)
        {
            transactions.push_back(Transaction(id, pnr, amount, method, status));

            if (id >= nextTransactionID)
            {
                nextTransactionID = id + 1;
            }
        }

        file.close();
    }

    // Display all bookings
    void displayAllBookings() const
    {
        if (tickets.empty())
        {
            cout << "\nNo bookings available!\n";
            return;
        }

        cout << "\n========== ALL BOOKINGS ==========\n";

        for (int i = 0; i < tickets.size(); i++)
        {
            tickets[i].displayTicket();
        }
    }
    //display booking count
    void displayBookingCount()
    {
        cout << "\nTotal Bookings: " << tickets.size() << endl;
    }

    //sortBookingByPNR
    void sortBookingsByPNR()
    {
        if (tickets.empty())
        {
            cout << "\nNo bookings available!\n";
            return;
        }

        sort(tickets.begin(), tickets.end(),[](const Ticket &a, const Ticket &b)
        {
            return a.getPNR() < b.getPNR();
        });

        cout << "\nBookings sorted by PNR successfully!\n";

        for (int i = 0; i < tickets.size(); i++)
        {
            tickets[i].displayTicket();
        }
    }

    //sort booking by fare
    void sortBookingsByFare()
    {
        if (tickets.empty())
        {
            cout << "\nNo bookings available!\n";
            return;
        }

        sort(tickets.begin(), tickets.end(),[](const Ticket &a, const Ticket &b)
        {
            return a.getFare() < b.getFare();
        });

        cout << "\nBookings sorted by Fare successfully!\n";

        for (int i = 0; i < tickets.size(); i++)
        {
            tickets[i].displayTicket();
        }
    }

    // Search ticket using PNR
    void searchTicket()
    {
        int searchPNR;

        cout << "\nEnter PNR Number: ";

        if (!(cin >> searchPNR))
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid PNR!\n";
            return;
        }

        for (int i = 0; i < tickets.size(); i++)
        {
            if (tickets[i].getPNR() == searchPNR)
            {
                cout << "\nTicket found!\n";
                tickets[i].displayTicket();
                return;
            }
        }

        cout << "Ticket not found!\n";
    }

    //search booking by name
    void searchBookingByName()
    {
        string name;
        bool found = false;

        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        cout << "\nEnter Passenger Name: ";
        getline(cin, name);

        for (int i = 0; i < tickets.size(); i++)
        {
            if (tickets[i].getPassenger().getName() == name)
            {
                tickets[i].displayTicket();
                found = true;
            }
        }

        if (!found)
        {
            cout << "\nNo booking found for this passenger!\n";
        }
    
    }
    // Cancel ticket using PNR
    void cancelTicket()
    {
        int cancelPNR;

        cout << "\nEnter PNR Number to cancel: ";

        if (!(cin >> cancelPNR))
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid PNR!\n";
            return;
        }

        for (int i = 0; i < tickets.size(); i++)
        {
            if (tickets[i].getPNR() == cancelPNR)
            {
                int trainNo = tickets[i].getTrainNumber();
                int seatNo = tickets[i].getSeatNumber();

                // Release the cancelled ticket's seat
                seatManager.releaseSeat(trainNo, seatNo);

                // Remove the cancelled ticket
                tickets.erase(tickets.begin() + i);

                cout << "\nTicket cancelled successfully!\n";
                updateTransactionStatus(cancelPNR);

                // Check whether a passenger is waiting
                Passenger nextPassenger(0, "");
                Train waitingTrain(0, "", "", "", 0);

                if (waitingList.getNextWaitingPassenger(trainNo, nextPassenger, waitingTrain))
                {
                    // Allocate the released seat
                    if (seatManager.bookSeat(trainNo, seatNo))
                    {
                        Ticket newTicket(
                            nextPNR,
                            nextPassenger,
                            waitingTrain,
                            seatNo
                        );

                        tickets.push_back(newTicket);

                        cout << "\nWaiting passenger promoted successfully!\n";

                        newTicket.displayTicket();

                        nextPNR++;
                    }
                }

                saveBookings();
                return;
            }
        }

        cout << "\nTicket not found!\n";
    }

    //show available sEats
    void showAvailableSeats()
    {
        int trainNo;

        cout << "\nEnter Train Number: ";
        cin >> trainNo;

        seatManager.displayAvailableSeats(trainNo);
    }

    //sHow waiting list
    void showWaitingList()
    {
        int trainNo;

        cout << "\nEnter Train Number: ";
        cin >> trainNo;

        waitingList.displayWaitingList(trainNo);
    }

    //add new route
    void addNewRoute()
    {
        string source, destination;
        int distance;

        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        cout << "\nEnter Source City: ";
        getline(cin, source);

        cout << "Enter Destination City: ";
        getline(cin, destination);

        cout << "Enter Distance (in km): ";
        cin >> distance;

        if (distance <= 0)
        {
            cout << "\nInvalid distance!\n";
            return;
        }

        routeGraph.addRoute(source, destination, distance);
        routeGraph.saveRoutes();

        cout << "\nNew route added successfully!\n";
    }

    //remove exsiting node
    void removeExistingRoute()
    {
        string source, destination;

        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        cout << "\nEnter Source City: ";
        getline(cin, source);

        cout << "Enter Destination City: ";
        getline(cin, destination);

        if (routeGraph.removeRoute(source, destination))
        {
            routeGraph.saveRoutes();
            cout << "\nRoute removed successfully!\n";
        }
        else
        {
            cout << "\nRoute not found!\n";
        }
    }

    //search existing route
    void searchExistingRoute()
    {
        string source, destination;

        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        cout << "\nEnter Source City: ";
        getline(cin, source);

        cout << "Enter Destination City: ";
        getline(cin, destination);

        routeGraph.searchRoute(source, destination);
    }

    //admin login
    bool adminLogin()
    {
        string username, password;

        cout << "\n========== ADMIN LOGIN ==========\n";

        cout << "Enter Admin Username: ";
        cin >> username;

        cout << "Enter Admin Password: ";
        cin >> password;

        if (username == "admin" && password == "admin123")
        {
            cout << "\nAdmin Login Successful!\n";
            return true;
        }
        else
        {
            cout << "\nInvalid Username or Password!\n";
            return false;
        }
    }

    // Main menu
    void menu()
    {
        int choice;

        do
        {
            cout << "\n====================================\n";
            cout << "  TRAIN TICKET BOOKING SYSTEM\n";
            cout << "====================================\n";

            cout << "\n1. Book Ticket";
            cout << "\n2.Display All Bookings";
            cout << "\n3. Search Ticket";
            cout << "\n4. Cancel Ticket";
            cout << "\n5.Show Available Seats";
            cout << "\n6. Show Waiting List";
            cout << "\n7.Add Train Schedule";
            cout << "\n8. Display All Schedules";
            cout << "\n9. Search Train Schedule";
            cout << "\n10.Payment";
            cout << "\n11. Display Transaction History";
            cout << "\n12. Sort Bookings by PNR";
            cout << "\n13. Search Booking by Passender name";
            cout << "\n14.Display total Booking count";
            cout << "\n15. Sort BOoking by fare";
            cout << "\n16. Admin - Add new route";
            cout << "\n17. Admin - Display all routes";
            cout << "\n18. Admin - Remove Existing node";
            cout << "\n19.Admin - search Direct Route";
            cout << "\n20.Exit";

            cout << "\nEnter your choice: ";

            if (!(cin >> choice))
            {
                if (cin.eof())
                    return;

                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');

                cout << "Please enter a valid number!\n";
                continue;
            }

            switch (choice)
            {
                case 1:
                    bookTicket();
                    break;

                case 2:
                    displayAllBookings();
                    break;

                case 3:
                    searchTicket();
                    break;

                case 4:
                    cancelTicket();
                    break;

                case 5:
                    showAvailableSeats();
                    break;

                case 6:
                    showWaitingList();
                    break;

                case 7:
                    scheduleManager.addSchedule();
                    scheduleManager.saveSchedules();
                    break;

                case 8:
                    scheduleManager.displaySchedules();
                    break;

                case 9:
                    scheduleManager.searchSchedule();
                    break;
                
                case 10:
                    processPayment();
                    break;

                case 11:
                    displayTransactionHistory();
                    break;

                case 12:
                    sortBookingsByPNR();
                    break;

                case 13:
                    searchBookingByName();
                    break;

                case 14:
                    displayBookingCount();
                    break;

                case 15:
                    sortBookingsByFare();
                    break;

                case 16:
                    if (adminLogin())
                    {
                        addNewRoute();
                    }
                    break;

                case 17:
                    if (adminLogin())
                    {
                        routeGraph.displayAllRoutes();
                    }
                    break;

                case 18:
                    if (adminLogin())
                    {
                        removeExistingRoute();
                    }
                    break;

                case 19:
                    if (adminLogin())
                    {
                        searchExistingRoute();
                    }   
                    break;

                case 20:
                    cout << "\nThank you for using Railway Booking System!\n";
                    break;

                default:
                    cout << "\nInvalid choice! Please try again.\n";
            }
        } while (choice != 20);
    }
};