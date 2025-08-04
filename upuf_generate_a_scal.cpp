#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <algorithm>
#include <map>

using namespace std;

// Structure to store script information
struct Script {
    string name;
    string command;
    int interval; // in seconds
};

// Function to load scripts from file
vector<Script> loadScripts(const string& filename) {
    vector<Script> scripts;
    ifstream file(filename);
    string line;
    while (getline(file, line)) {
        size_t pos = line.find(',');
        if (pos != string::npos) {
            Script script;
            script.name = line.substr(0, pos);
            size_t pos2 = line.find(',', pos + 1);
            script.command = line.substr(pos + 1, pos2 - pos - 1);
            script.interval = stoi(line.substr(pos2 + 1));
            scripts.push_back(script);
        }
    }
    return scripts;
}

// Function to execute script
void executeScript(const Script& script) {
    cout << "Executing script: " << script.name << endl;
    system(script.command.c_str());
    cout << "Script executed successfully!" << endl;
}

// Function to monitor and execute scripts
void monitorScripts(const vector<Script>& scripts) {
    map<string, int> lastExecuted;
    while (true) {
        for (const auto& script : scripts) {
            if (lastExecuted.find(script.name) == lastExecuted.end() || 
                lastExecuted[script.name] + script.interval <= time(0)) {
                executeScript(script);
                lastExecuted[script.name] = time(0);
            }
        }
        this_thread::sleep_for(chrono::seconds(1)); // adjust the sleep interval as needed
    }
}

int main() {
    vector<Script> scripts = loadScripts("scripts.txt");
    monitorScripts(scripts);
    return 0;
}