//Headers
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <map>

using namespace std;

bool compareValue(pair<string, double>& a, pair<string, double>& b){    //Function for sorting a map by value
    return a.second < b.second;
}

void makeCall(map<string, int>& contacts, map<string, double>& callList, map<string, int>& minTestDate, vector<string>& testDates, int callLimit){  //Function for making call list output
    //Sorting
    vector<pair<string, double>> sortedMap;

    for (auto& i : callList){       //Copy the hashmap into vector of pairs for sorting
        sortedMap.push_back(i);
    }
    sort(sortedMap.begin(), sortedMap.end(), compareValue);     

    //Result Printing
    int sortedMapSize = sortedMap.size();
    callLimit = min(callLimit, sortedMapSize);  //Prevent index going out of range
    for (int i = 0; i < callLimit; i++){
        string key = sortedMap[i].first;
        auto it = contacts.find(key);   //Search the contact hashmap for phone number
        auto it2 = minTestDate.find(key);   //Search the minTestDate hashmap for date index
        cout << i + 1 << ". " << key << " / " << it->second << " / " << testDates[it2->second] << " / " << sortedMap[i].second << endl;
    }
}

map<string, int> Read_Contact(){  //Function for reading contact from txt file
    map<string, int> contacts;
    ifstream contactFile;
    string studentName;
    int phoneNumber;    

    contactFile.open("./contactList.txt");   //Open file from path
    if (contactFile.is_open()){                        //If the file exist
        contactFile >> studentName >> phoneNumber;     //Read the file in pair
        while (!contactFile.eof()){                    //Read until end of file
            contacts.insert(contacts.end(), {studentName, phoneNumber});
            contactFile >> studentName >> phoneNumber;
        }
    }else{
        cout << "** Contact List is Missing **" << endl;
    }
    contactFile.close();
    return contacts;
}

int main(){
    //Variable declaration
    map<string, int> contacts;  //A hashmap with all student contact
    map<string, double> callList; //A hashmap containing all the student need to be called;
    map<string, int> minTestDate; //A hashmap storing the date index of the minimum result obtained by the student
    vector<string> testDates; //Array storing the test dates
    int numDays, callLimit, passingScore;

    contacts = Read_Contact(); //Get contact list from txt file

    cout << "Please Enter the Passing Score : ";
    cin >> passingScore;

    cout << "Please Enter the Duration of Tests : ";
    cin >> numDays;
    while (numDays <= 0){       //Input Checking
        cout << "The Duration of Tests should be greater than 0" << endl;
        cout << "Please Enter the Duration of Tests : ";
        cin >> numDays;
    }

    for (int i = 0; i < numDays; i++){  //For each test date
        string dateString;
        cout << "Please Enter the Date of Day " << i + 1 << " : ";
        cin >> dateString;
        testDates.push_back(dateString);    //Push new date into the end of the array

        string studentName;
        double result;
        cout << "Please Enter Students Result on " << dateString << " (Enter 0 0 to Finish Entry) " << endl;    //Read first entry of the date
        cin >> studentName >> result;

        while (studentName != "0" && result != 0){  //Student result entry loop, break when 0 0 is entered
            if (result < passingScore){                 //Check if the score less than passing score
                auto it = callList.find(studentName);   //iterator for finding student name in callList hashmap
                if (it != callList.end()){              //If current student name exist in the map
                    if (result < it->second){           //If new result lower than the stored result
                        it->second = result;            //Update the value by the lowest result
                        auto it2 = minTestDate.find(studentName);   //iterator for finding student name in minTestDate hashmap
                        it2->second = i;                //Update the minimum result date index
                    }
                }else{                                                              //If current student name not exist in the map
                    callList.insert(callList.end(), {studentName, result});         //Add new entry to callList hashmap
                    minTestDate.insert(minTestDate.end(), {studentName, i});        //Add new entry to minTestDate hashmap
                }
            }
            cin >> studentName >> result;   //Read next entry
        }
    }

    cout << "Please Enter the Call Limit : ";   //Read call limit
    cin >> callLimit;
    while (callLimit < 0){       //Input Checking
        cout << "Call Limit should not be less than 0" << endl;
        cout << "Please Enter the Call Limit : ";
        cin >> callLimit;
    }

    cout << "Call Should be Made on " << testDates[numDays - 1] << ":" << endl; //Display call list  
    makeCall(contacts, callList, minTestDate, testDates, callLimit);
    return 0;
}