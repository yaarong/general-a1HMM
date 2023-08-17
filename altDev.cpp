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
double E[7][4] = {
    {(double)1/4, (double)1/4, (double)1/4, (double)1/4},
    {(double)1/4, (double)1/4, (double)1/4, (double)1/4},
    {(double)1/4, (double)1/4, (double)1/4, (double)1/4},
    {(double)1/4, (double)1/4, (double)1/4, (double)1/4},
    {(double)1/4, (double)1/4, (double)1/4, (double)1/4},
    {(double)1/4, (double)1/4, (double)1/4, (double)1/4},
    {(double)1/4, (double)1/4, (double)1/4, (double)1/4}
};

// Transmission
// Rows: from state
// Cols: to state
const double e1 = 0.017;
const double e2 = 0.077;
const double a = 0.2;

double T[7][7] = {
   {1-e1-e2,e1,     0,      0,      e2,     0,      0},
   {0,      0,      1,      0,      0,      0,      0}, 
   {0,      0,      0,      1,      0,      0,      0},
   {0,      e1,     0,      1-e1-e2,e2,     0,      0},
   {0,      0,      0,      0,      0,      1,      0},
   {0,      0,      0,      0,      0,      0,      1},
   {a,      0,      0,      0,      0,      0,      1-a},
};
/*
double T[7][7] = {
   {0.923,  0.017,  0,      0,      0.06,   0,      0},
   {0,      0,      1,      0,      0,      0,      0}, 
   {0,      0,      0,      1,      0,      0,      0},
   {0,      0.017,  0,      0.923,  0.06,   0,      0},
   {0,      0,      0,      0,      0,      1,      0},
   {0,      0,      0,      0,      0,      0,      1},
   {0.5,    0,      0,      0,      0,      0,      0.5},
};*/
    
// iNfluence probabilities
// Rows: influencing observation
// Cols: influenced observation
//  acgu
double N[4][4] = {
    {0.008, 0.008,  0.008,  0.72},
    {0.008, 0.008,  0.96,   0.008},
    {0.008, .96,    0.008,  0.28},
    {0.72,  0.008,  0.28,   0.008},
};

// Which states influence which states
// Rows: influencing state
// Cols: influenced state
bool allowedInfs[7][7]{
    {0, 1, 1, 1, 1, 1, 1},
    {0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0}
};

// Probability of starting with each state
double startProb[7] = {1, 0, 0, 0, 0, 0, 0};

// Which end-states are allowed (1 is allowed, 0 is disallowed)
bool endState[7] = {1, 0, 0, 1, 0, 0, 1};

// Affiliations
// Row # is affiliated with col #
bool aff[7][7] = {
//   s  q1 q2 q3 p1 p2 p3
    {0, 0, 0, 0, 0, 0, 0}, // s
    {0, 0, 0, 0, 0, 0, 0}, // q1
    {0, 1, 0, 0, 0, 0, 0}, // q2 
    {0, 0, 1, 1, 0, 0, 0}, // q3
    {0, 0, 0, 0, 0, 0, 0}, // p1
    {0, 0, 0, 0, 1, 0, 0}, // p2
    {0, 0, 0, 0, 0, 1, 1}  // p3
};

// State "types"
int type[7] = {0,1,2,3,4,5,6};
vector<string> observationKey{"A", "C", "G", "U"};
vector<string> stateKey{"S", "Q1", "Q2", "Q3", "P1", "P2", "P3"};

// A : 0
// C : 1
// G : 2
// U : 3

vector<int> O{1,1,1,0,1,1,1,0,0,0,0,2,2,2,2,2,2};


const int numStates = sizeof(T)/sizeof(T[0]);
const int numSteps = O.size();

bool validInf(vector<vector<vector<int>>> &influence, int ingState, int edState, int ingStep, int edStep){
    vector<int> path = influence[type[ingState]][type[edState]];
    int total = 0;
    if (!allowedInfs[ingState][edState]){
        return false;
    }
    /*
    for (int i = ingStep; i < edStep; i++){
        total += path[i];
        if (total < 0){
            //cout << "false";
            return false;
        }
    }*/
    bool used = false;
    for (int i = 0; i < numStates; i++){
        for (int j = 0; j < numStates; j++){
            if (influence[i][j][ingStep] == 1){
                used = true;
                break;
            }
        }
    }
    //cout << path.size() << " " << ingStep << "\n";

    return (total == 0) && (!used);
}


int main(){
    cout << numStates << " " << numSteps << "\n";
    bool influenceable[numStates];
    int numTypes = 1;
    for (int i =0; i < numStates; i++){
        numTypes = max(numTypes, type[i] + 1);
        for (int j = 0; j < numStates; j++){
            if (allowedInfs[i][j]){
                influenceable[j] = true;
            }
        }
    }
   

    // probabilities
    // current step, current state, influencing step, choice (actual influence or fake influence)
    double m[numSteps+1][numStates][numSteps+1][2];
    for (int i = 0; i < numSteps+1; i++){
        for (int j = 0; j < numStates; j++){
            for (int k = 0; k < numSteps+1; k++){
                m[i][j][k][0] = 0;
                m[i][j][k][1] = 0;
            }
        }
    }


    // Influence matrix 
    // ([current Step] [current state] [influence step] [choice]) ([influencing type][influenced type][step]) = {1 if influencing, -1 if influenced, 0 otherwise}
    vector<vector<vector<int>>> I[numSteps + 1][numStates][numSteps+1][2];
    vector<vector<vector<int>>> blank(numTypes, vector<vector<int>>(numTypes, vector<int>(numSteps+1, 0)));

    // step, state, influence, influenceChoice
    tuple<int, int, int, int> choice[numSteps + 1][numStates][numSteps + 1][2];


    // ([current step][current state][influencing step][choice])([influencing state])
    //vector<pair<int, int>> unused[numSteps + 1][numStates][numSteps+1][2];

    for (int i = 0; i < numStates; i++){
        //reverse(stateKey[i].begin(), stateKey[i].end());
        m[1][i][1][1] = E[0][O[1]] * startProb[i];
        I[1][i][1][1] = blank;
        /*
        for (int j = 0; j < numStates; j++){
            priority_queue<int> temp;
            unused[1][i][1][1].push_back(temp);
            unused[1][i][1][1][j].push(1);
        }*/
    }    

    // Current step
    for (int j = 2; j < numSteps + 1; j++){
        // Current state
        for (int r = 0; r < numStates; r++){
            //Current influencing step
            for (int l = j; l > 0; l--){
                // Previous step = j-1
                // Previous state
                for (int q = 0; q < numStates; q++){
                    // previous influence
                    for (int k = j-1; k > 0; k--){
                        //current condition
                        for (int cc = 0; cc <=1; cc++){
                            // previous condition
                            for (int cp = 0; cp <= 1; cp++){
                                if (aff[r][q] && k != l+1 && cc == 0 && cp == 0){
                                    continue;
                                }
                                double prev = m[j-1][q][k][cp] * T[q][r];
                                double prob = 0;
                                //curUnused[r].push(j);
                                //cout << j << " " << r << " " << l <<" " << q << " " << k << " " << cc <<" " << cp << "\n";
                                //cout << cc;
                                

                                if (prev == 0){

                                    continue;
                                }
                                //vector<priority_queue<int>> curUnused = unused[j-1][q][k][cp];
                                
                                vector<vector<vector<int>>> lastInf = I[j-1][q][k][cp];
                                //influence 
                                vector<vector<vector<int>>> curInf = lastInf; 

                                // Get influcing state
                                // step, state, influence, choice
                                tuple<int, int, int, int> next_step = make_tuple(j-1, q, k, cp);
                                vector<bool> betweenStates(numStates, 0);
                                
                                // trace path back to influencing step
                                while (get<0>(next_step) > l){
                                    next_step = choice[get<0>(next_step)][get<1>(next_step)][get<2>(next_step)][get<3>(next_step)];
                                }
                                
                                // get influencing state
                                int ingState = get<1>(next_step); 

                                bool noBetween = true;
                                

                                
                                if (// p:influence, c:influence
                                    (cc == 0 && cp == 0) && 
                                    //(r == 2) &&
                                    //((q == r && k == l+1) || (q != r)) && 
                                    ((prev * N[O[l-1]][O[j-1]]) > prob) &&
                                    (validInf(lastInf, ingState, r, l, j)) &&
                                    noBetween && 
                                    allowedInfs[ingState][r] && 
                                    (l < j-4) &&
                                    (lastInf[type[ingState]][type[r]][l] != 1)
                                ){  
                                    curInf[type[ingState]][type[r]][j] = -1;
                                    curInf[type[ingState]][type[r]][l] = 1;
                                    prob = prev * N[O[l-1]][O[j-1]];
                                    //curUnused[ingState].pop();
                                    //cout << "m(" << j <<  ", " << r <<  ", "<<  l << "): 0,0 -----"<<prob<<"\n"; 
                                }
                                if (// p:none, c:influence
                                    (cc == 0 && cp == 1) && 
                                    //(r == 2) &&
                                    //((l == j-1) || (l < k)) && 
                                    ((prev * N[O[l-1]][O[j-1]]) > prob) &&
                                    (validInf(lastInf, ingState, r, l, j)) &&
                                    noBetween && 
                                    allowedInfs[ingState][r]&& 
                                    (l < j-4) &&
                                    (lastInf[type[ingState]][type[r]][l] != 1)
                                ){
                                    curInf = lastInf;
                                    curInf[type[ingState]][type[r]][j] = -1;
                                    curInf[type[ingState]][type[r]][l] = 1;
                                    //curUnused[ingState].pop();
                                    //cout << "m(" << j <<  ", " << r <<  ", "<<  l << "): 0,1-----"<<prob<<"\n"; 
                                    prob = prev * N[O[l-1]][O[j-1]];
                                } 
                                if (// p:influence, c:none
                                    (cc == 1 && cp == 0) && 
                                    (!influenceable[r]) &&
                                    (k < l) && 
                                    ((prev * E[r][O[j]]) > prob)
                                ){
                                    //cout << "m(" << j <<  ", " << r <<  ", "<<  l << "): 1,0-----"<<prob<<"\n"; 
                                    prob = prev * E[r][O[j]];
                                }

                                if (// p:none, c:none
                                    (cc == 1 && cp == 1) && 
                                    (!influenceable[r]) &&
                                    ((k == l && r == q) || (l == j && r != q)) && 
                                    ((prev * E[r][O[j]]) > prob)
                                ){
                                    //cout << "m(" << j <<  ", " << r <<  ", "<<  l << "): 1,1-----"<<prob<<"\n"; 
                                    prob = prev * E[r][O[j]];

                                }
                                //curUnused[r].push(j);
                                if (prob > m[j][r][l][cc]){
                                    
                                    m[j][r][l][cc] = prob;
                                    choice[j][r][l][cc] = make_tuple(j-1, q, k, cp);
                                    I[j][r][l][cc] = curInf;
                                    //unused[j][r][l][cc] = curUnused;
                                }

                                if (j == 8 && r == 0 && cc==1 && l == 1){
                                    //cout << "(" << j << " " << stateKey[r] << " " << l << " " << cc << ") " << "(" << j-1 << " " << stateKey[q] << " " << k << " " << cp << ") " << prob << " " << m[j-1][q][k][cp] << "\n"; 
                                }
                                
                            }
                        }
                    }
                }
            }
        }
    }

    double maxprob = 0;
    string path[numSteps];
    string og(numSteps, '*');
    string influences = og;
    tuple<int, int, int, int> next_path;
    int j = numSteps;

    int longest = 0;

    for (int r = 0; r < numStates; r++){
        for (int l = j; l >0; l--){
            for (int c = 0; c < 2; c++){
                if (m[j][r][l][c] > maxprob && endState[r]){
                    maxprob = m[j][r][l][c];
                    next_path = choice[j][r][l][c];
                    influences = og;
                    if (r == 1 || r == 2 || r == 3){
                        influences[j-1] = ')';
                        influences[l-1] = '(';
                    } else if (r != 0){
                        influences[j-1] = '}';
                        influences[l-1] = '{';
                    }
                    
                    path[j-1] = stateKey[r];
                    longest = stateKey[r].length();
                    
                    //cout << j << " " << r << " "<< l << " " << c << ": " << m[j][r][l][c] << "\n";
                    //cout << get<3>(next_path) << "\n";
                }
            }
        }
    }
    //iPath = I[][][][]
    // ([current Step] [current state] [influence step] [influence/no influence]) ([influencing state][influenced state][step]) = {1 if influcing, -1 if influenced, 0 otherwise}

    for (int j = numSteps-1; j > 0; j--){
        int r = get<1>(next_path);
        int l = get<2>(next_path);
        int c = get<3>(next_path);
        if (c == 0){
            if (r == 1 || r == 2 || r == 3){
                influences[j-1] = ')';
                influences[l-1] = '(';
            } else if (r != 0){
                influences[j-1] = '}';
                influences[l-1] = '{';
            }
            
        }
        path[j-1]= stateKey[r];
        cout << j << " " << r << " "<< l << " " << c << "\n";
        longest = max(longest, (int)stateKey[r].length());
        next_path = choice[j][r][l][c];
        
    }


    string actualO = "";
    for (auto x : observationKey){
        longest = max(longest, (int)x.length());
    }

    for (auto x : O){
        actualO+= observationKey[x];
        for (int j = 0; j < longest - observationKey[x].length() + 1; j++){
            actualO += " ";
        }
    }
    cout << actualO << "\n";

    for (int i = 0; i < numSteps; i++){
        cout << path[i];
        for (int j = 0; j < longest - path[i].length() + 1; j++){
            cout << " ";
        }
    }
    cout << "\n";
    for (int i = 0; i < influences.length(); i++){
        cout << influences[i];
        for (int j = 0; j < longest; j++){
            cout << " ";
        }
    }
}