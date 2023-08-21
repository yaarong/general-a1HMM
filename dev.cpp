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
double E[3][4] = {
    {(double)1/4, (double)1/4, (double)1/4, (double)1/4},
    {(double)1/4, (double)1/4, (double)1/4, (double)1/4},
    {(double)1/4, (double)1/4, (double)1/4, (double)1/4},
};

// Transmission
// Rows: from state
// Cols: to state
const double delta = 0.18;
const double alpha = 0.77;
const double beta = delta;
double T[4][4] = {
   {1 - delta, delta, 0, 0},
   {0, 0, 1, 0},
   {0, 0, 0, 1},
   {alpha, beta, 0, 1 - alpha - beta}
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
// Order: ACGU
double N[4][4] = {
    {0, 0,  0,  0.72},
    {0, 0,  0.96,  0},
    {0, .96, 0, 0.32},
    {0.72, 0, 0.32, 0},
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
bool endState[4] = {1, 0, 0, 1};

// Affiliations
// Row # is affiliated with col #. Row# influence must be next to col#
bool aff[4][4] = {
    {0, 0, 0, 0},
    {0, 0, 0, 0},
    {0, 1, 0, 0},
    {0, 0, 1, 1},
};

vector<string> observationKey{"A", "C", "G", "U"};
vector<string> stateKey{"L", "P", "Q", "R"};

// A : 0
// C : 1
// G : 2
// U : 3
vector<int> O{1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1,1, 3, 3, 3, 3, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 3, 3, 3, 3, 2, 2, 2, 2};

const int numStates = sizeof(T)/sizeof(T[0]);
const int numSteps = O.size();

bool validInf(vector<vector<vector<int>>> &influence, int ingState, int edState, int ingStep, int edStep){
     // Get current influence path
    // Contains which steps are influencing or influenced 
    vector<int> path = influence[ingState][edState];

    // Check if influenced is allowed. If not, return false.
    if (!allowedInfs[ingState][edState]){
        return false;
    }
    int total = 0;
    // Optional check for overlapping influences. 
    /*
    int total = 0;
    for (int i = ingStep; i < edStep; i++){
        total += path[i];
        if (total < 0){
            //cout << "false";
            return false;
        }
    }*/

    // Check if step is already influencing some laterstep
    bool used = false;
    for (int i = 0; i < numStates; i++){
        for (int j = 0; j < numStates; j++){
            if (influence[i][j][ingStep] == 1){
                used = true;
                break;
            }
        }
    }
    return (total == 0) && (!used);
}


int main(){

    // Creates array to store which states are influenceable. If influence is possible, the influence will always be chosen.
    // aka enforcing influences
    bool influenceable[numStates];
    for (int i =0; i < numStates; i++){
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
    // ([current Step] [current state] [influence step] [choice]) ([influencing state][influenced state][step]) = {1 if influencing, -1 if influenced, 0 otherwise}
    vector<vector<vector<int>>> I[numSteps + 1][numStates][numSteps+1][2];
    vector<vector<vector<int>>> blank(numStates, vector<vector<int>>(numStates, vector<int>(numSteps+1, 0)));

    // step, state, influence, influenceChoice
    tuple<int, int, int, int> choice[numSteps + 1][numStates][numSteps + 1][2];

    // Initial probabilities
    for (int i = 0; i < numStates; i++){
        m[1][i][1][1] = E[0][O[1]] * startProb[i];
        I[1][i][1][1] = blank;
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
                                // Check for affinity rules 
                                if (aff[r][q] && k != l+1 && cc == 0 && cp == 0){
                                    continue;
                                }

                                // previous probability and current calculated probability (0 at first)
                                double prev = m[j-1][q][k][cp] * T[q][r];
                                double prob = 0;

                                // Skip if last step was impossible
                                if (prev == 0){
                                    continue;
                                }
                                
                                // Set current influence matrix to the previous one
                                vector<vector<vector<int>>> lastInf = I[j-1][q][k][cp];
                                vector<vector<vector<int>>> curInf = lastInf; 

                                // Get influencing state
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
                                    //((q == r && k == l+1) || (q != r)) && 
                                    ((prev * N[O[l-1]][O[j-1]]) > prob) &&
                                    (validInf(lastInf, ingState, r, l, j)) &&
                                    noBetween && 
                                    allowedInfs[ingState][r] && 
                                    (l < j-4) &&
                                    (lastInf[ingState][r][l] != 1)
                                ){  
                                    curInf[ingState][r][j] = -1;
                                    curInf[ingState][r][l] = 1;
                                    prob = prev * N[O[l-1]][O[j-1]];
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
                                    (lastInf[ingState][r][l] != 1)
                                ){
                                    curInf = lastInf;
                                    curInf[ingState][r][j] = -1;
                                    curInf[ingState][r][l] = 1;
                                    prob = prev * N[O[l-1]][O[j-1]];
                                } 
                                if (// p:influence, c:none
                                    (cc == 1 && cp == 0) && 
                                    (!influenceable[r]) &&
                                    (k < l) && 
                                    ((prev * E[r][O[j]]) > prob)
                                ){
                                    prob = prev * E[r][O[j]];
                                }

                                if (// p:none, c:none
                                    (cc == 1 && cp == 1) && 
                                    (!influenceable[r]) &&
                                    ((k == l && r == q) || (l == j && r != q)) && 
                                    ((prev * E[r][O[j]]) > prob)
                                ){
                                    prob = prev * E[r][O[j]];

                                }
                                //curUnused[r].push(j);
                                if (prob > m[j][r][l][cc]){
                                    
                                    m[j][r][l][cc] = prob;
                                    choice[j][r][l][cc] = make_tuple(j-1, q, k, cp);
                                    I[j][r][l][cc] = curInf;
                                }
                                
                            }
                        }
                    }
                }
            }
        }
    }

    // current highest probability
    double maxprob = 0;

    // Creating output strings
    string path[numSteps];
    string og(numSteps, '*');
    string influences = og;

    // Find highest probabilit end state
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
                }
            }
        }
    }
    //iPath = I[][][][]
    // ([current Step] [current state] [influence step] [influence/no influence]) ([influencing state][influenced state][step]) = {1 if influcing, -1 if influenced, 0 otherwise}

    // traceback to find full path
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
        //cout << j << " " << r << " "<< l << " " << c << "\n";
        longest = max(longest, (int)stateKey[r].length());
        next_path = choice[j][r][l][c];
        
    }


    // Format output
    string actualO = "";
    for (auto x : observationKey){
        longest = max(longest, (int)x.length());
    }
    if (longest == 1){
        longest = 0;
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