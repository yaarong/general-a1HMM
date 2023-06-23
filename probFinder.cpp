#include <array>
#include <cmath>
#include <fstream>
#include <cstring>
#include <iomanip>
#include <iostream>
#include <map>
#include <tuple>
#include <queue>
#include <set>
#include <string>
#include <unordered_set>
#include <vector>
#define ll long long
using namespace std;
ifstream fin ("test.in");
ofstream fout ("test.out");
// Emission
// Rows: State
// Col: Observered
double E[4][4] = {
    {(double)1/4, (double)1/4, (double)1/4, (double)1/4},
    {(double)1/4, (double)1/4, (double)1/4, (double)1/4},
    {(double)1/4, (double)1/4, (double)1/4, (double)1/4},
    {(double)1/4, (double)1/4, (double)1/4, (double)1/4},
};

// Transmission
// Rows: from state
// Cols: to state
double T[4][4] = {
    {(double)1/2, (double)1/2, 0, 0},
    {0, 0, 1 ,0},
    {0, 0, 0, 1},
    {(double)1/4, (double)1/4, 0 ,(double)1/2}
};
    
// iNfluence probabilities
// Rows: influencing observation
// Cols: influenced observation
double N[4][4] = {
    {(double)0/10, (double)0/10, (double)0/10, (double)7/10},
    {(double)0/10, (double)0/10, (double)7/10, (double)0/10},
    {(double)0/10, (double)7/10, (double)0/10, (double)0/10},
    {(double)7/10, (double)0/10, (double)0/10, (double)0/10}    
};

// Which states influence which states
// Rows: influencing state
// Cols: influenced state
bool allowedInfs[4][4]{
    {0, 1, 1, 1},
    {0, 0, 0, 0},
    {0, 0, 0, 0},
    {0, 0, 0, 0}
};

// Probability of starting with each state
double startProb[4] = {1, 0, 0, 0};

// Which end-states are allowed (1 is allowed, 0 is disallowed)
bool endState[4] = {1, 1, 1, 1};

// Affiliations
// Row # is affiliated with col #
bool aff[4][4] = {
    {1, 0, 0, 0},
    {0, 0, 0, 0},
    {0, 1, 0, 0},
    {0, 0, 1, 1}
};

// State "types"
int type[4] = {0,1,2,3};
vector<string> observationKey{"A", "C", "G", "U"};
vector<string> stateKey{"S", "X", "Y", "Z"};

// A : 0
// C : 1
// G : 2
// U : 3

vector<int> O{1, 2, 2, 2, 1, 2, 2, 2, 1, 0, 0, 1, 1, 1, 1, 3, 3, 3, 2, 0, 2, 0, 0, 0, 1, 1, 1, 1};

int main(){
    string states = "SSSSSSSSSXYZXYZ";
    string infs =   "((((((***))))))";

    map<char, int> key;
    key['S'] = 0;
    key['X'] = 1;
    key['Y'] = 2;
    key['Z'] = 3;

    double prob = (double)1/4;
    for (int i = 1; i < states.length(); i++){
        prob = prob * T[key[states[i-1]]][key[states[i]]]; 
        if (infs[i] == ')'){
            prob = prob * (double)7/10;
        } else {
            prob = prob * (double)1/4;
        }
    }
    cout << prob;


}