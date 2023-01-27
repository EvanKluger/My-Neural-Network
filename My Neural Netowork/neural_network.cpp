#include "neural_network.h"
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


using namespace std;

double activation(double input){
    double result;
    result = (1.000/(1.000 + (1.00*exp(-input))));
    return result;
}

double activation_derivative(double input){
    double result;
    result = activation(input)*(1.000-activation(input));
    return result;
}

vector<double> deconstruct_line_double(string line){
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

vector<int> deconstruct_line_int(string line){
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


//Constructor. Constrcuts the Neural Network Object with loaded in Weights from file
neural_net::neural_net(vector<int> size_layers_input, string &filename){
    neuralNet.resize(3);
    for(int i = 0; i < 3; i++){
        size_layers.push_back(size_layers_input[i]);
        neuralNet[i].resize(size_layers_input[i]+1);
        neuralNet[i][0].set_value(-1.0);
        neuralNet[i][0].bias = true;
        neuralNet[i][0].weights.resize(0);
        neuralNet[i][0].layer = i;
        neuralNet[i][0].index = 0;
        neuralNet[i][0].weightedSum = -1.0;
        for(int j = 1; j < neuralNet[i].size();j++){
            neuralNet[i][j].layer = i;
            neuralNet[i][j].index = j;
            neuralNet[i][j].bias = false;
            neuralNet[i][j].set_value(0);
            ;
            if(i==0){
            neuralNet[i][j].weights.resize(0);
            }
            else{
            neuralNet[i][j].weights.resize(neuralNet[i-1].size());
            }
        }
           
    }
    ifstream ifs(filename);
    string line;
    int layer = 1;
    int index = 1;
    vector<double> weights1;
    int count_line = 0;
    
    while(getline(ifs, line)){
        if(count_line ==0){
            count_line = count_line + 1;
        }
        else{
            weights1 = deconstruct_line_double(line);
            for(int i = 0; i < neuralNet[layer-1].size(); i++){
                neuralNet[layer][index].weights[i] = weights1[i];
            }
            index = index + 1;
            if(index == neuralNet[layer].size()){
                index = 1;
                layer = layer + 1;
            }
        }
    }
    ifs.close();
}

//Outstream. Outputs the neural network weights to a text file. 
int neural_net::outstream_neural_net(string &filename) {
    ofstream *outfile;
    ofstream ofs(filename);
    outfile = &ofs;
    int i = 0;
    for (int num : size_layers){
        if(i == size_layers.size() -1){
            *outfile << num;
        }
        else{
            *outfile << num << ' ';
        }
        i++;
    }
    *outfile << '\n';

    for(int j = 1; j < 3; j++){
        for(int n = 1; n < neuralNet[j].size(); n++){
            int count_weights = 0;
            for (double & weight : neuralNet[j][n].weights){
                //IF at End of Line
                if(count_weights == neuralNet[j][n].weights.size() - 1){
                    *outfile << setprecision(3) << fixed << weight;

                }
                else{
                    *outfile << setprecision(3) << fixed << weight << ' ';
                }
                count_weights++;
            }
            *outfile << '\n';
        }
    }
    ofs.close();
    return 0;
}



//Train. Trains the Neural Network for a set amount of Epochs. Based on the Annotated Psudocode Provided.
int neural_net::train(string &filename, int epochs, double lr){
    //Run the Training for the Amount of Epochs
    vector<double> Inputs;
    vector<double> Outputs;
    for(int epoch = 0; epoch < epochs; epoch++){
        ifstream ifs(filename);
        string line;
        getline(ifs, line);

        
        vector<int> ex_i_o = deconstruct_line_int(line);
        
        //For Each Example that we are given
        while(getline(ifs, line)){
            //Gather the Info on the Line we are Up to
            vector<double> examples = deconstruct_line_double(line);
            Inputs.clear();
            Outputs.clear();
            //The Inputs
            for(int i = 0; i < ex_i_o[1]; i++){
                Inputs.push_back(examples[i]);
            }

            for(int i = ex_i_o[1]; i < examples.size(); i++){
                Outputs.push_back(examples[i]);
            }

            //Setting each node in the Input Layer to The Neural Net. Line 11 in PseudoCode.
            for(int i = 1; i < neuralNet[0].size(); i++){
                neuralNet[0][i].set_value(Inputs[i-1]);
            }

            //Looping through Layers. Line 12 in PseudoCode.
            for(int layer = 1; layer < 3; layer++){
                //Looping through the Nodes in Each Layer. Line 13 in PsudoCode.
                for(int j = 1; j < neuralNet[layer].size(); j++){
                    double sum_value = 0.0;
                    //Sum of Weights l J * Inputs. Line 14 in PseudoCode.
                    for(int i = 0; i < neuralNet[layer-1].size(); i++){
                        sum_value = sum_value + ((1.0)*((neuralNet[layer][j].weights[i] * neuralNet[layer-1][i].get_value())));
                    }
                    neuralNet[layer][j].weightedSum = sum_value;
                    neuralNet[layer][j].set_value(activation(sum_value));
                }
            }
            
            //Looping through all Nodes in Output Layer. Line 17 in PsudoCode.
            int layer = 2;
            for(int j= 1; j < neuralNet[layer].size(); j++){
                neuralNet[layer][j].delta = (activation_derivative(neuralNet[layer][j].weightedSum)) * (Outputs[j-1] - neuralNet[layer][j].get_value());
            }

            //Looping through the Hidden and Output Layers. Line 19 in Psudocode.
            layer = 1;
            for(int node = 0; node < neuralNet[layer].size(); node++){
                //THIS CHANGE LED TO AN OUTPUT
                double sum = 0.0;
                for(int node_layer_up = 1; node_layer_up < neuralNet[layer+1].size(); node_layer_up++){
                    sum = sum + (neuralNet[layer+1][node_layer_up].weights[node] * neuralNet[layer+1][node_layer_up].delta);
                }
                //Update Delta of Hidden layer
                neuralNet[layer][node].delta = (1.0)*(activation_derivative(neuralNet[layer][node].weightedSum)) * (sum);
            }
            
            //Looping through all Nodes in the Neural Network to update every weight in Network. Line 23 in PsudoCode.
            //For the Hidden and Output Layers. Update Weights Previous.
    
            for(layer = 2; layer > 0; layer--){
                for (int i = 0; i < neuralNet[layer].size(); i++){
                    for(int weight = 0; weight < neuralNet[layer][i].weights.size(); weight++){
                        neuralNet[layer][i].weights[weight] = neuralNet[layer][i].weights[weight] + (lr * neuralNet[layer-1][weight].get_value() * neuralNet[layer][i].delta);
                    }
                }
            }
            
            //Reseting the Nodes Values and Sums
            for(int layer = 0; layer < 3; layer++){
                for (int NodeIn = 0; NodeIn < neuralNet[layer].size(); NodeIn++){
                    neuralNet[layer][NodeIn].reset_nodes();
                }
            }
        }
        ifs.close();
        cout << "Epoch #" << epoch << " Has Finished" << endl;
    }
    return 0;
}
//Function of Class Node inside Class neural_net that sets the Node value to a new value. Double checks to make sure that it is not overiuding for a Bias Weight.
int neural_net::Node::set_value(double value_new) {
    if(!bias) {
        value = value_new;
    }
    return 0;
}

//Function of Class Node inside Class neural_net that resets the values for the next epoch of Back Propogation 
void neural_net::Node::reset_nodes() {
    if(!bias){
        value = 0;
        weightedSum = 0;
    }
    else{
        value = -1;
        weightedSum = -1;
    }
    delta = 0;
}

double neural_net::Node::get_value(){
    return value;
}

//Test. Tests Neural Network on a Test_File. Used Performance Meetrics of Accuracy, Precision, Recall, and F1.
neural_net::test(string &test_file, string &metrics_file){
    /*
    Confusion Matrix (Is it named this because I am Confused trying to find the performance matrix?)

                 |Expected = 1 | Expected = 0|
                 |---------------------------|
    Predicted = 1|      A      |      B      |
                 |---------------------------|
    Predicted = 0|      C      |      D      |
                 |---------------------------|
    */

    //Intialization of Vectors containing As,Bs,Cs,Ds for each category
    vector<int> As;
    vector<int> Bs;
    vector<int> Cs;
    vector<int> Ds;

    //Intialization of Global A,B,C,D. Used for Micro Average.
    int A_global = 0;
    int B_global = 0;
    int C_global = 0;
    int D_global = 0;
    
    //Micro Average Intializations
    double overall_accuracy_micro = 0.0;
    double precision_micro = 0.0;
    double recall_micro = 0.0;
    double f1_micro = 0.0;
    //Macro Average Initializations
    double av_overall_accuracy_macro = 0.0;
    double av_precision_macro = 0.0;
    double av_recall_macro = 0.0;
    double av_f1_macro = 0.0;

    //Parsing Variables
    ifstream ifs(test_file);
    string line;
    ofstream *outfile;
    ofstream ofs(metrics_file);
    outfile = &ofs;
    //Count Line
    int count = 0; 
    vector<int> size_examples;
    vector<double> Inputs;
    vector<double> Outputs;
    //While function that repeats until there are no more test cases
    while(getline(ifs, line)){
        if(count == 0){
            size_examples = deconstruct_line_int(line);
            count = count + 1;


            
        }
        else{
            //Gathering Test Case from Line in File and Distributing to Inputs and Outputs
            vector<double> examples = deconstruct_line_double(line);
            Inputs.clear();
            Outputs.clear();
            //The Inputs
            for(int i = 0; i < size_examples[1]; i++){
                Inputs.push_back(examples[i]);
            }
            //The Outputs
            for(int i = size_examples[1]; i < examples.size(); i++){
                Outputs.push_back(examples[i]);
            }

            //Gathering the Neural Network's prediction based on weights assigned

            //Setting each node in the Input Layer to The Neural Net. Line 11 in PseudoCode.
            for(int i = 1; i < neuralNet[0].size(); i++){
                neuralNet[0][i].set_value(Inputs[i-1]);
            }
            
            //Looping through Hidden and Output Layers. Line 12 in PseudoCode.
            for(int layer = 1; layer < 3; layer++){
                //Looping through the Nodes in Each Layer. Line 13 in PsudoCode.
                for(int j = 1; j < neuralNet[layer].size(); j++){
                    double sum_value = 0.0;
                    //Sum of Weights[l][j] * Inputs. Line 14 in PseudoCode.
                    for(int w = 0; w < neuralNet[layer-1].size(); w++){
                        sum_value = sum_value + (neuralNet[layer][j].weights[w] * neuralNet[layer-1][w].get_value());
                    }
                    neuralNet[layer][j].weightedSum = sum_value;
                    neuralNet[layer][j].set_value(activation(sum_value));
                }
            }
            //Intializing the Predictions Vector. Containing the Predicted Outputs of the Test Cases.
            vector<double> predictions;
            predictions.resize(neuralNet[2].size()-1);
            
            //Resizing the As,Bs,Cs,Ds for each category to the amount of categories
            As.resize(predictions.size());
            Bs.resize(predictions.size());
            Cs.resize(predictions.size());
            Ds.resize(predictions.size());
            
            int A_local = 0;
            int B_local = 0;
            int C_local = 0;
            int D_local = 0;

            

            //Adding the predictions from the values of the Output Layer to the Predictions Vector
            for(int i = 1; i < neuralNet[2].size(); i++){
                predictions[i-1] = neuralNet[2][i].get_value();
            }

            //Counting the A,B,C,D cases based on predicted compared to true output
            for(int i = 0; i<predictions.size(); i++){
                if(predictions[i] >= 0.5 && Outputs[i] == 1){
                    A_global = A_global + 1;
                    //A_local = A_local + 1;
                    As[i] = As[i] + 1;
                }
                
                else if(predictions[i] >= 0.5 && Outputs[i] == 0){
                    B_global = B_global + 1;
                    //B_local = B_local + 1;
                    Bs[i] = Bs[i] + 1;
                }
                
                else if(predictions[i] < 0.5 && Outputs[i] == 1){
                    C_global = C_global + 1;
                    //C_local = C_local + 1;
                    Cs[i] = Cs[i] + 1;
                }
                
                else if(predictions[i] < 0.5 && Outputs[i] == 0){
                    D_global = D_global + 1;
                    //D_local = D_local + 1;
                    Ds[i] = Ds[i] + 1;
                }
            }
            
            count = count + 1;
        }
    }
    ifs.close();

    //Calculating Macro and Micro Averages
    double overall_accuracy_macro = 0.0; 
    double precision_macro = 0.0;
    double recall_macro = 0.0;
    double f1_macro = 0.0;
    
    //Looping through the Output Categories
    for(int i = 0; i<As.size(); i++){
        overall_accuracy_macro = (1.0 *(As[i] + Ds[i])) / (1.0 *(As[i]+Bs[i]+Cs[i]+Ds[i]));
        precision_macro = (1.0*As[i])/( 1.0*(As[i]+Bs[i]));
        recall_macro = (1.0*As[i])/( 1.0 *(As[i]+Cs[i]));
        f1_macro = (2.0*precision_macro*recall_macro) / (1.0*precision_macro+recall_macro);
        
        //Outputting the A,B,C,D for each catergory along with its 4 Individual Performance Measures
        *outfile << As[i] << " " << Bs[i] << " " << Cs[i] << " " << Ds[i] << " ";
        *outfile <<  setprecision(3) << fixed << overall_accuracy_macro << " ";
        *outfile <<  setprecision(3) << fixed << precision_macro << " ";
        *outfile <<  setprecision(3) << fixed << recall_macro << " ";
        *outfile <<  setprecision(3) << fixed << f1_macro << "\n";

        //Adding the Individual Catergories Performance Measures to the Macro Average 
        av_overall_accuracy_macro = av_overall_accuracy_macro + overall_accuracy_macro;
        av_precision_macro = av_precision_macro + precision_macro;
        av_recall_macro = av_recall_macro + recall_macro;
        av_f1_macro = av_f1_macro + f1_macro;
    }

    //Micro Averaging: Using Global A,B.C.D data to find Perfomance Measures
    overall_accuracy_micro = (1.0 * (A_global+D_global))/(1.0 *(A_global+B_global+C_global+D_global));
    precision_micro = (1.0 *(A_global)) / (1.0 *(A_global+B_global));
    recall_micro = (1.0 * (A_global)) / (1.0 *(A_global+C_global));
    f1_micro = (2.0*precision_micro*recall_micro) / (precision_micro+recall_micro);

    //Outputting the Final Micro Average
    *outfile << overall_accuracy_micro << " " << precision_micro << " " << recall_micro << " " << f1_micro << endl;

    //Macro Averaging: Using the Average of the Performance Measures for each caterogory
    av_overall_accuracy_macro = av_overall_accuracy_macro/(1.0*(As.size()));
    av_precision_macro = av_precision_macro/(1.0*(As.size()));
    av_recall_macro = av_recall_macro/(1.0*(As.size()));
    av_f1_macro = (2.0 * av_precision_macro * av_recall_macro) / (av_precision_macro + av_recall_macro);

    //Outputting the Final Macro Average
    *outfile << av_overall_accuracy_macro << " " << av_precision_macro << " " << av_recall_macro << " " << av_f1_macro << endl;
    ofs.close();
}
