#include<bits/stdc++.h>
#define FIO ios_base::sync_with_stdio(0); cin.tie(0); cout.tie(0);

using namespace std;

namespace Logger {

    enum class LogLevel{
        INFO,
        WARNING,
        ERROR,
    };

    bool firstWrite = true;

    // This function is used to return a string representation of the LogLevel.
    string ConvertLogLevelToString(LogLevel logLevel) {
        switch (logLevel) {
            case (LogLevel::INFO):
                return "INFO";
            case (LogLevel::WARNING):
                return "WARNING";
            case (LogLevel::ERROR):
                return "ERROR";
            default:
                return "UNKNOWN LOG TYPE";
        }
    }

    void Log(string message, LogLevel level) {
        FIO;
        string Level = ConvertLogLevelToString(level);
        string filename = "../logs.txt";
        ofstream logFile;
        // This condition verifies if the file already exists and if it’s the first write operation on it.
        // If yes, it clears the file.
        if (firstWrite && !logFile.is_open()) {
            ofstream outFile(filename, ios::out | ios::trunc);
            firstWrite = false;
        }

        // Open the file in append mode. If the file does not exist, it will be created.
        logFile.open(filename, ios_base::app);

        // This checks if the file can be opened.
        // If it can, the log is added and the file is then closed.
        if (logFile.is_open()) {
            auto now = std::chrono::system_clock::now();
            time_t time = chrono::system_clock::to_time_t(now);
            logFile << Level << ": " << message << " :: " << ctime(&time) << endl;
            logFile.close();
        } else {
            cerr << "Unable to open log file: " << filename << endl;
        }
    }
}
