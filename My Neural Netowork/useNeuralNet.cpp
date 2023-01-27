#include <string>
#include <type_traits>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <fstream>
#include <cctype>
#include <string>
#include <algorithm>
#include <iostream>
#include <chrono>
#include <ctime> 
#include <cmath>   
#include <iomanip>
#include "neural_network.h"



//There are two bad coding pratices that I do for these two fucntions. First, instead of making a class and using a template, I instead just copy the function twice for doubles and ints.
//Second, instead of creating a separate source and header file, I end up defining these both here and in the neural_netowkr.cpp file. I hope I am not penalized for this, and I will fix this after I submit
//It is just that I have been working a really long time on this project and had to focus my energy on getting everything else working and ran out of time to fix this "bad coding pratice"

vector<double> deconstruct_line_double2(string line){
    vector<double> result_doubles;
    vector<string> result_strings;
    string temp;
    double temp_d;
    for(int i = 0; i<line.size(); i++){
        if(isspace(line[i])){
            result_strings.push_back(temp);
            temp.clear();
        }
        else{
            temp = temp + line[i]; 
        }
    }
    result_strings.push_back(temp);
    
    for(int j =0; j < result_strings.size(); j++){
        temp_d = stod(result_strings[j]);
        result_doubles.push_back(temp_d);
    }
    return result_doubles;   
}

vector<int> deconstruct_line_int2(string line){
    vector<int> result_ints;
    vector<string> result_strings;
    string temp;
    int temp_i;
    for(int i = 0; i<line.size(); i++){
        if(isspace(line[i])){
            result_strings.push_back(temp);
            temp.clear();
        }
        else{
            temp = temp + line[i]; 
        }
    }
    result_strings.push_back(temp);
    
    for(int j =0; j < result_strings.size(); j++){
        temp_i = stoi(result_strings[j]);
        result_ints.push_back(temp_i);
    }
    
    return result_ints;  
}




int main(){

//Choice
int choice = 0;
bool success;

//Training
string original_neural_network;
string training_set;
int epochs;
double learning_rate;
string trained_neural_netowork;
string epochs_string;
string learning_rate_string;


//Testing
string neural_network;
string test_set;
string metrics;



cout << "Would you like to Train or Test the Model. Press 1 for Training and 2 for Testing" << endl;
cin >> choice;

if(choice == 1 || choice == 2){
    success = true;
}

while(success != true){
    cout << "Would you like to Train or Test the Model. Press 1 for Training and 2 for Testing" << endl;
    cin >> choice;

    if(choice == 1 || choice == 2){
    success = true;
    }
}

if(choice == 1){
    cout << "It's Time to Train the Model" << endl;
    cout << "Enter the name of the original neural net file: " << endl;
    cin >> original_neural_network;
    cout << "Enter the name of the training set:" << endl;
    cin >> training_set;
    cout << "Enter the name returned neural network file:" << endl;
    cin >> trained_neural_netowork;
    cout << "How many epochs do you want:" << endl;
    cin >> epochs_string;
    cout << "What do you want the learning rate to be:" << endl;
    cin >> learning_rate_string;

    int epochs = stoi(epochs_string);
    double learning_rate = stod(learning_rate_string);

    //Run Training
    ifstream ifs(original_neural_network);
    string line;
    getline(ifs, line);
    ifs.close();

    vector<int> Neural_Net_Layers;
    Neural_Net_Layers = deconstruct_line_int2(line);

    neural_net The_Net = neural_net(Neural_Net_Layers, original_neural_network);
    The_Net.train(training_set, epochs, learning_rate);
    The_Net.outstream_neural_net(trained_neural_netowork);
    return 0;
}

else{
    cout<< "It's Time to Test the Set" << endl;

    cout << "Enter the name of the trained neural network: " << endl;
    cin >> neural_network;
    cout<< "Enter the name of the test_set:"<<endl;
    cin >> test_set;
    cout<< "Enter the name of the file for the metrics:"<<endl;
    cin >> metrics;
    //Run Tests
    string line;
    ifstream ifs(neural_network);
    getline(ifs, line);
    
    ifs.close();

    vector<int> Neural_Net_Layers; 
    Neural_Net_Layers = deconstruct_line_int2(line);
    
    
    neural_net The_Net = neural_net(Neural_Net_Layers, neural_network);
    The_Net.test(test_set, metrics);
    return 0;
}
return 0;
}