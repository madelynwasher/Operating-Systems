// CS 33211 - Operating Systems
// Programming Assignment 2
// Madelyn Washer

#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>

using namespace std;

// Checks whether system is in a safe state
bool isSafeState(vector<int> available, vector<vector<int>> allocation, 
                 vector<vector<int>> max, vector<vector<int>> need, vector<int>& safeSequence) {
    int numProcesses = allocation.size();
    int numResources = available.size();

    vector<bool> finished(numProcesses, false);  // Tracks finished processes
    vector<int> work = available;

    for (int count = 0; count < numProcesses; ++count) {
        bool found = false;

        for (int i = 0; i < numProcesses; ++i) {
            if (!finished[i]) {
                bool canProceed = true;

                for (int j = 0; j < numResources; ++j) {
                    if (need[i][j] > work[j]) {
                        canProceed = false;
                    }
                }

                if (canProceed) {
                    for (int j = 0; j < numResources; ++j) {
                        work[j] += allocation[i][j];
                    }
                    safeSequence.push_back(i);
                    finished[i] = true; // Process is finished
                    found = true;
                    break;
                }
            }
        }

        if (!found) {
            // If no process can proceed, then system is not in a safe state
            return false;
        }
    }
    return true;
}

int main() {
    ifstream inputFile("input.txt"); // Open input file
    if (!inputFile) {
        cerr << "Error: Unable to open input file!" << endl;
        return 1;
    }

    vector<int> available;
    vector<vector<int>> allocation, max, need;

    string line;

    // Read available resources
    getline(inputFile, line);
    stringstream ss(line.substr(line.find(":") + 1));
    int resource;
    while (ss >> resource) {
        available.push_back(resource);
    }

    // Read process data
    while (getline(inputFile, line)) {
        if (line.empty()) continue;
        vector<int> allocRow, maxRow;
        stringstream ss(line);

        string processLabel, allocLabel, maxLabel;
        ss >> processLabel >> allocLabel;
        for (int i = 0; i < available.size(); ++i) {
            ss >> resource;
            allocRow.push_back(resource);
        }

        ss >> maxLabel;
        for (int i = 0; i < available.size(); ++i) {
            ss >> resource;
            maxRow.push_back(resource);
        }

        allocation.push_back(allocRow);
        max.push_back(maxRow);
    }

    // Calculate need matrix
    for (size_t i = 0; i < allocation.size(); ++i) {
        vector<int> needRow(available.size());
        for (size_t j = 0; j < available.size(); ++j) {
            needRow[j] = max[i][j] - allocation[i][j];
        }
        need.push_back(needRow);
    }

    // Check if system is in a safe state
    vector<int> safeSequence;
    if (isSafeState(available, allocation, max, need, safeSequence)) {
        cout << "The system is in a safe state." << endl;
        cout << "Safe sequence is: ";
        for (size_t i = 0; i < safeSequence.size(); ++i) { // Print out safe sequence
            cout << "P" << safeSequence[i];
            if (i != safeSequence.size() - 1) cout << " -> ";
        }
        cout << endl;
    } else {
        cout << "The system is NOT in a safe state." << endl;
    }

    return 0;
}
