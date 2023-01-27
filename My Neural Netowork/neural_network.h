#ifndef _Neural_Network_H
#define _Neural_Network_H

#include <vector>
#include <fstream>
#include <cctype>
#include <string>
#include <algorithm>
#include <iostream>
#include <chrono>
#include <ctime>   

using namespace std;

class neural_net{
    public:
        neural_net(vector<int> layerSizes_input, string &filename);
        int outstream_neural_net(string &filename);
        int train(string &filename, int epoch, double lr);
        int test(string &test_file, string &metrics_file);

    private:
        class Node{
            public:
                int index;
                int layer;
                
                vector<double> weights;
                bool bias = false;
                 
                double delta;
                double weightedSum;

                void reset_nodes();
                int set_value(double value_new);
                double get_value();
            
            private:
                double value;
        };
        vector<int> size_layers;
        vector<vector<Node>> neuralNet;
};


#endif //_Neural_Network_H