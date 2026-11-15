//====================================================================================================================================================
//                                                 ROUTE GRAPH CLASSS 
//======================================================================================================================================================
class RouteGraph
{
private:
    // Station name and distance to connected station
    map<string, vector<pair<string, int>>> adjList;

public:

    RouteGraph()
    {
        addRoute("Dehradun", "Haridwar", 52);
        addRoute("Haridwar", "Roorkee", 35);
        addRoute("Roorkee", "Delhi", 175);
        addRoute("Dehradun", "Saharanpur", 70);
        addRoute("Saharanpur", "Delhi", 150);
    }

    // Add a route between two stations
    void addRoute(string source, string destination, int distance)
    {
        adjList[source].push_back({destination, distance});

        // Undirected route: both stations connect to each other
        adjList[destination].push_back({source, distance});
    }

    //remove route
    bool removeRoute(string source, string destination)
    {
        auto it1 = adjList.find(source);
        auto it2 = adjList.find(destination);

        if (it1 == adjList.end() || it2 == adjList.end())
        {
            return false;
        }

        auto &routes1 = it1->second;
        auto &routes2 = it2->second;

        auto pos1 = find_if(routes1.begin(), routes1.end(),
        [&](const pair<string, int> &route)
        {
            return route.first == destination;
        });

        auto pos2 = find_if(routes2.begin(), routes2.end(),[&](const pair<string, int> 
            &route)
        {
            return route.first == source;
        });

        if (pos1 == routes1.end() || pos2 == routes2.end())
        {
            return false;
        }

        routes1.erase(pos1);
        routes2.erase(pos2);

        return true;
    }

    //search direct routes
    bool searchRoute(string source, string destination)
    {
        auto it = adjList.find(source);

        if (it == adjList.end())
        {
            return false;
        }

        for (auto &route : it->second)
        {
            if (route.first == destination)
            {
                cout << "\nRoute Found!\n";
                cout << "Source: " << source << endl;
                cout << "Destination: " << destination << endl;
                cout << "Distance: " << route.second << " km\n";
                return true;
            }
        }

        cout << "\nDirect route not found!\n";
        return false;
    }

    //save routes
    void saveRoutes()
    {
        ofstream file("routes.txt");

        for (auto &city : adjList)
        {
            for (auto &route : city.second)
            {
                if (city.first < route.first)
                {
                    file << city.first << " "<< route.first << " "<< route.second << endl;
                }
            }
        }   

        file.close();
    }

    //load routes
    void loadRoutes()
    {
        ifstream file("routes.txt");

        if (!file)
        {
            return;
        }

        adjList.clear();

        string source, destination;
        int distance;

        while (file >> source >> destination >> distance)
        {
            addRoute(source, destination, distance);
        }

        file.close();
    }

    //display all routes
    void displayAllRoutes()
    {
        cout << "\n========== ALL AVAILABLE ROUTES ==========\n";

        for (auto &city : adjList)
        {
            for (auto &route : city.second)
            {
                if (city.first < route.first)
                {
                    cout << city.first << " -> "<< route.first << " : " << route.second << " km\n";
                }
            }
        }
    
    }
    // Find shortest route using Dijkstra's Algorithm
    bool findShortestRoute(string source, string destination,int &totalDistance, vector<string> &path)
    {
        map<string, int> dist;
        map<string, string> parent;

        for (auto station : adjList)
        {
            dist[station.first] = INT_MAX;
        }

        if (adjList.find(source) == adjList.end() ||
            adjList.find(destination) == adjList.end())
        {
            return false;
        }

        // Min priority queue: {distance, station}
        priority_queue<
            pair<int, string>,
            vector<pair<int, string>>,
            greater<pair<int, string>>
        > pq;

        dist[source] = 0;
        pq.push({0, source});

        while (!pq.empty())
        {
            int currentDistance = pq.top().first;
            string currentStation = pq.top().second;
            pq.pop();

            if (currentDistance > dist[currentStation])
                continue;

            for (auto neighbour : adjList[currentStation])
            {
                string nextStation = neighbour.first;
                int edgeDistance = neighbour.second;

                int newDistance = currentDistance + edgeDistance;

                if (newDistance < dist[nextStation])
                {
                    dist[nextStation] = newDistance;
                    parent[nextStation] = currentStation;

                    pq.push({newDistance, nextStation});
                }
            }
        }

        if (dist[destination] == INT_MAX)
            return false;

        totalDistance = dist[destination];

        // Reconstruct shortest path
        string current = destination;

        while (current != source)
        {
            path.push_back(current);
            current = parent[current];
        }

        path.push_back(source);

        reverse(path.begin(), path.end());

        return true;
    }
};