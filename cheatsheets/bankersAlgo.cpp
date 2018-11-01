#include <iostream>
#include <stdio.h>
#include <math.h>
#include <list>
#include <map>
#include <set>
#include <vector>
#include <queue>
#include <unordered_map>
#include <unordered_set>
#include <stack>
#include <algorithm>
#include <tuple>
#include <string.h>

#define MAX 2147483646

using namespace std;

vector<vector<int>> getNeed (vector<vector<int>> Max, vector<vector<int>> allocation) {
    // Row first then column
    for (int i = 0; i < Max.size(); i++) {
        for (int j = 0; j < Max[i].size(); j++) {
            Max[i][j] -= allocation[i][j];
        }
    }
    return Max;
}

bool lessThanEqualsTo (vector<int> need_i, vector<int> work) {
    // Check if all elemts in the row are less than or equals to target b
    for (int i = 0; i < need_i.size(); i++) {
        if (need_i[i] > work[i]) return false;
    }
    return true;
}

vector<int> workPlusAllocation (vector<int> work, vector<int> allocation) {
    // Plus all elements in the row
    for (int i = 0; i < work.size(); i++) {
        work[i] += allocation[i];
    }
    return work;
}

vector<int> getAvailable (vector<int> original, vector<vector<int>> allocation) {
    for (int i = 0; i < allocation.size(); i++) {
        for (int j = 0; j < allocation[i].size(); j++) {
            original[j] -= allocation[i][j];
        }
    }
    return original;
}


int main () {
    // Edit this part to your situation
    int numProcesses = 5;
    int numResourceTypes = 3;
    
    vector<int> defaultResources(numResourceTypes);
    defaultResources = {10,5,7};
    vector<vector<int>> Max(numProcesses, vector<int>(numResourceTypes));
    Max = {{7,5,3}, {3,2,2}, {9,0,2}, {2,2,2}, {4,3,3}}; // Max resources possible for the processes
    vector<vector<int>> allocation(numProcesses, vector<int>(numResourceTypes));
    allocation = {{0,1,0}, {2,0,0}, {3,0,2}, {2,1,1}, {0,0,2}}; // currently allocated resource for the processes
//    allocation = {{}}; // Allocation is zero for all
    vector<int> available = getAvailable(defaultResources, allocation); // Because of the currently allocated resource. what's avail = default-currently allocated
    
    
    // Do not touch this part
    vector<vector<int>> need = getNeed(Max, allocation); // Max-Allocation to get Need
    cout << "========Process Needs Before========" << endl;
    int process = 0;
    for (auto &&each : need) {
        cout << "P" << process << ": ";
        for (auto &&each_ : each) {
            cout << each_ << " ";
        }
        process++;
        cout << endl;
    }
    
    cout << "========What's Available Before========" << endl;
    for (auto each : available) {
        cout << each << " ";
    }
    cout << "\n\n";
    
    // Initialise
    vector<int> work = available;
    vector<bool> Finish (numProcesses, false); // All false
    
    queue<int> processSequence;
    
    // Safety check
    while (true) {
        // Find an i such that its not finished and need <= work
        int chosen = -1;
        for (int i = 0; i < numProcesses; i++) {
            if (lessThanEqualsTo(need[i], work) && Finish[i] == false) {
                chosen = i;
                break;
            }
        }
        
        // If there's a chosen one, it is not finished yet and we add on to the working resources for next iteration
        if (chosen != -1) {
            work = workPlusAllocation(work, allocation[chosen]);
            Finish[chosen] = true;
            processSequence.push(chosen);
        } else {
            // Check if all processes are finished
            int count = 0;
            for (auto each : Finish) {
                if (each == true) {
                    count++;
                }
            }
            if (count == numProcesses) {
                cout << "System is safe!" << endl;
                break;
            } else {
                cout << "System is unsafe!" << endl;
                break;
            }
        }
    }
    
//    cout << "========Final Work========" << endl;
//    for (auto each : work) {
//        cout << each << endl;
//    }
    
    cout << "========Sequence========" << endl;
    while (!processSequence.empty()) {
        cout << processSequence.front() << endl;
        processSequence.pop();
    }
    
}
