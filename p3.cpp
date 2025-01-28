#include <iostream>
#include <vector>
#include <unordered_map>
#include <limits>
#include <cmath>
#include <string>
using namespace std;

struct Edge {
    string src, dest;
    double weight;
};

bool bellmanFord(int V, vector<Edge> &edges, const unordered_map<string, int> &currencyIndex, vector<string> &path) {
    vector<double> distance(V, numeric_limits<double>::infinity());
    vector<int> predecessor(V, -1);
    distance[0] = 0; // Assuming first currency as source
    int cycleStart = -1;
    
    for (int i = 0; i < V - 1; i++) {
        for (const auto &edge : edges) {
            int u = currencyIndex.at(edge.src);
            int v = currencyIndex.at(edge.dest);
            if (distance[u] != numeric_limits<double>::infinity() && distance[u] + edge.weight < distance[v]) {
                distance[v] = distance[u] + edge.weight;
                predecessor[v] = u;
            }
        }
    }
    
    for (const auto &edge : edges) {
        int u = currencyIndex.at(edge.src);
        int v = currencyIndex.at(edge.dest);
        if (distance[u] != numeric_limits<double>::infinity() && distance[u] + edge.weight < distance[v]) {
            cycleStart = v;
            break;
        }
    }
    
    if (cycleStart == -1) return false;
    
    // Extract cycle
    vector<int> cycle;
    vector<bool> visited(V, false);
    while (!visited[cycleStart]) {
        visited[cycleStart] = true;
        cycleStart = predecessor[cycleStart];
    }
    
    int cycleVertex = cycleStart;
    double profit = 1.0; // Start with 1 unit of currency
    do {
        cycle.push_back(cycleVertex);
        cycleStart = predecessor[cycleStart];
    } while (cycleStart != cycleVertex);
    cycle.push_back(cycleVertex);
    
    // Convert back to multiplication and check profit
    for (size_t i = 1; i < cycle.size(); i++) {
        int u = cycle[i - 1];
        int v = cycle[i];
        for (const auto &edge : edges) {
            if (currencyIndex.at(edge.src) == u && currencyIndex.at(edge.dest) == v) {
                profit *= exp(-edge.weight); // Convert back from log
                break;
            }
        }
    }

    if (profit > 1.001) {
        for (int i = cycle.size() - 1; i >= 0; --i) {
            for (const auto &entry : currencyIndex) {
                if (entry.second == cycle[i]) {
                    path.push_back(entry.first);
                    break;
                }
            }
        }
        return true;
    }

    return false;
}

vector<string> find_arbitrage(int num_graphs, const vector<unordered_map<string, vector<pair<string, double>>>> &input_graphs) {
    vector<string> result;
    for (int i = 0; i < num_graphs; i++) {
        const auto &graph = input_graphs[i];
        vector<Edge> edges;
        unordered_map<string, int> currencyIndex;
        int index = 0;
        
        for (const auto &entry : graph) {
            const string &src = entry.first;
            for (const auto &dest_rate : entry.second) {
                const string &dest = dest_rate.first;
                double rate = dest_rate.second;
                
                if (currencyIndex.find(src) == currencyIndex.end()) currencyIndex[src] = index++;
                if (currencyIndex.find(dest) == currencyIndex.end()) currencyIndex[dest] = index++;
                
                edges.push_back({src, dest, -log(rate)}); // Log transformation
            }
        }
        
        vector<string> path;
        if (bellmanFord(index, edges, currencyIndex, path)) {
            string cycle_path;
            for (const auto &currency : path) {
                cycle_path += currency + " ";
            }
            result.push_back(cycle_path);
        } else {
            result.push_back("NO");
        }
    }
    return result;
}

int main() {
    int num_graphs = 2;
    vector<unordered_map<string, vector<pair<string, double>>>> input_graphs = {
        {
            {"EUR", {{"RUB", 105.94}}},
            {"RUB", {{"CNY", 0.071}}},
            {"CNY", {{"EUR", 0.132}}}
        },
        {
            {"USD", {{"INR", 86.60}}},
            {"INR", {{"GBP", 0.0095}}},
            {"GBP", {{"JPY", 189.59}}},
            {"JPY", {{"INR", 0.56}}}
        }
    };

    vector<string> output = find_arbitrage(num_graphs, input_graphs);
    for (const auto &line : output) {
        cout << line << endl;
    }

    return 0;
}
