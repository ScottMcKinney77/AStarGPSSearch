#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>

/* Header file for reading from CSV (comma seperated value) files
*   Contains only one method readFromFile(string filename)
*/

using namespace std;

//Returns a 2d vector of strings from a CSV file
vector<vector<string>> readFromFile(string filename) { 
    fstream file;
    vector<vector<string>> table;

    file.open(filename, ios::in);
    if (!file){
        cout << "File Not Found\n";
    }

    vector<string> row; 
    string line, word, temp; 
  
    while (file >> temp) {   
        row.clear(); 

        stringstream s(temp);
  
        while (s.good()){
            string word;
            getline(s,word, ',');
            row.push_back(word);
        }
        table.push_back(row);
    } 
    return table;
}