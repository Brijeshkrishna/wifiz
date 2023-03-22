#include <iostream>
#include <fstream>
#include <string>

using namespace std;

bool setMacAddress(const string& ifaceName, const string& macAddress) {
    // Construct the file path to the MAC address file
    string filePath = "/sys/class/net/" + ifaceName + "/address";
    
    // Open the file for writing
    ofstream macFile(filePath);
    if (!macFile) {
        cerr << "Error: Failed to open file " << filePath << " for writing." << endl;
        return false;
    }
    
    // Write the MAC address to the file
    macFile << macAddress;
    
    // Check for errors
    if (macFile.fail()) {
        cerr << "Error: Failed to write MAC address to file " << filePath << "." << endl;
        return false;
    }
    
    // Close the file
    macFile.close();
    
    // Success
    return true;
}

int main() {
    string ifaceName = "wlp4s0f3u3";
    string macAddress = "c6:b8:e3:83:1e:d9";
    
    if (setMacAddress(ifaceName, macAddress)) {
        cout << "Successfully set MAC address for interface " << ifaceName << "." << endl;
    } else {
        cerr << "Failed to set MAC address for interface " << ifaceName << "." << endl;
    }
    
    return 0;
}
