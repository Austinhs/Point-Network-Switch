#include <iostream>
#include <Windows.h>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>

using namespace std;

void selectFile();
void setNetwork(string service_url, string service_pass);
void clearFile();
int addNetwork();
bool file_exist(string path);
string trim(string str);

const string FILE_PATH = "C:\\CaylxSwitcher.ini";
string POINT_PATH = getenv("USERPROFILE");

int main()
{
    POINT_PATH = POINT_PATH + "\\AppData\\Local\\VirtualStore\\Windows\\winpoint.ini";

    // Setup
    system("COLOR A");
    SetConsoleTitle("Caylx Switcher");

    // Start reference
    ioPoint:


    // Clear and state it needs to be in admin
    system("CLS");
    cout << "MAKE SURE PROGRAM IS RUNNING IN ADMINISTRATOR OTHERWISE IT WILL NOT WORK" << endl << endl;

    int input;

    //Input options
    cout << "1. Create a new network" << endl << "2. Load an existing network"  << endl << "3. Clear saved networks" << endl << "4. Exit";
    cout << endl << endl;
    cout << "Option: ";
    cin >> input;

    //IO executions
    switch (input) {
        case 1:
            addNetwork();
            goto ioPoint;
        case 2:
            selectFile();
            goto ioPoint;
        case 3:
            clearFile();
            cout << "Cleared saved networks" << endl;
            Sleep(500);
            goto ioPoint;
        case 4:
            cout << "Bye!" << endl;
            Sleep(300);
            return 0;
        default:
            cout << "Unkown option selected! Please use: 1,2 or 3" << endl;
            Sleep(1000);

            goto ioPoint;
    }
}

void selectFile() {
    system("CLS");
    cout << "Opening: " << FILE_PATH << endl << endl;

    ifstream file;

    file.open(FILE_PATH, ios_base::beg);

    if (file.is_open()) {

        string line;
        int network_choice = -1;
        int networks_found = 0;
        while (getline(file, line)) {

            if (line.find("Network name:") == 0) {
                networks_found++;
                cout << networks_found << ". " << line << endl;
            }
        }

        if (networks_found == 0) {
            cout << "No networks found... returning to main menu." << endl;
            Sleep(1000);
            return;
        }

        cout << endl << "Option: ";
        cin >> network_choice;
        cout << endl;

        // Define variables for lookup
        networks_found = 0;
        bool choice_found = false;
        bool skip_first = false;
        int cred_count = 0;
        string service_url, service_pass;

        file.clear();
        file.seekg(0);
        while (getline(file, line)) {
            skip_first = false;

            if (line.find("Network name:") == 0) {
                networks_found++;

                // cout << "Found network: " << line << endl;

                if (networks_found == network_choice) {
                    // cout << "Found network match" << endl;
                    choice_found = true;
                    skip_first = true;
                }
            }

            if (choice_found && !skip_first) {
                cred_count++;
                
                int value_start_pos = line.find(": ") + 2;

                if (cred_count == 1) {
                    service_url = line.substr(value_start_pos);
                    service_url = trim(service_url);
                } else if (cred_count == 2) {
                    service_pass = line.substr(value_start_pos);
                    service_url = trim(service_pass);
                }
            }

            if (cred_count == 2) {
                break;
            }
        }

        // Option not found restart
        if (!choice_found) {
            cout << "Could not find network option you selected: " << network_choice << endl;
            Sleep(1000);
            selectFile();
        } else {
            // cout << "Found url: " << service_url << endl;
            // cout << "Found pass: " << service_pass << endl;

            setNetwork(service_url, service_pass);
        }
    } else {
        cout << "Could not open file, please run in admin if not already: " << FILE_PATH << endl;
        system("pause");
    }
}

string trim(string str) {
    str.erase(0, str.find_first_not_of(' '));       //prefixing spaces
    str.erase(str.find_last_not_of(' ') + 1);         //surfixing spaces
    return str;
}

void setNetwork(string service_url, string service_pass) {
    if (!file_exist(POINT_PATH)) {
        cout << "File does not exist, please setup point normally at least once so this file is created." << endl << POINT_PATH << endl << endl;
        system("pause");

        return;
    }

    ifstream r_point(POINT_PATH, ios::beg);
    fstream w_point(POINT_PATH);

    r_point.clear();
    r_point.seekg(0);

    if (r_point.is_open() && w_point.is_open()) {
        string line;
        while (getline(r_point, line)) {
            //cout << line << endl;

            if (line.find("WebService=https://") == 0) {
                line = "WebService=https://" + service_url;
            }

            if (line.find("SecurityKey=") == 0) {
                line = "SecurityKey=" + service_pass;
            }

            if (line == "") {
                break;
            }

            w_point << line << endl;
        }
    } else {
        cout << "Could not open file. Please make sure you are executing the file as admin: " << POINT_PATH << endl;
    }

    r_point.close();
    w_point.close();

    cout << "Network switched..." << endl;
    Sleep(500);
}

int addNetwork() {
    system("CLS");

    string service, password, network_name;
    
    cout << "Network name: ";
    cin >> network_name;

    cout << "Service URL: ";
    cin >> service;

    cout << "Service Password: ";
    cin >> password;

    fstream file;
    file.open(FILE_PATH, ios::app);

    if (file.is_open()) {
        file << "Network name: " << network_name << endl;
        file << "Service URL: " << service << endl;
        file << "Service Pass: " << password << endl;
        file << "---------------------------------------" << endl;
    } else {
        cout << "Could not write file. Please make sure you are executing the file as admin" << endl;

        system("pause");
    }

    file.close();

    int choice = 0;
    cout << "Would you like to set this as your current network? (1: Yes, 0: No): ";
    cin >> choice;

    if (choice == 1) {
        setNetwork(service, password);
    } else if (choice != 0) {
        cout << "This was an incorrect option, you picked: " << choice << ", We are considering that 'No'" << endl;
        Sleep(1000);
    }

    return 0;
}

bool file_exist(string path) {
    ifstream infile(path);
    return infile.good();
}

void clearFile() {
    ofstream file(FILE_PATH);

    if (file.is_open()) {
        file << "";

        file.close();
    } else {
        cout << "Could not open file path: " << FILE_PATH;
        system("pause");
    }
}
