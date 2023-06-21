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
    {(double)1/4, (double)1/4, (double)1/4, (double)1/4}
};

// Transmission
// Rows: from state
// Cols: to state
double T[3][3] = {
    {(double)1/3, (double)1/3, (double)1/3},
    {(double)1/3, (double)1/3, (double)1/3},
    {(double)1/3, (double)1/3, (double)1/3}
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
bool allowedInfs[3][3]{
    {0, 1, 1},
    {0, 0, 0},
    {0, 0, 0},
};

// Probability of starting with each state
double startProb[3] = {1, 0, 0};

// Which end-states are allowed (1 is allowed, 0 is disallowed)
bool endState[3] = {1, 1, 1};

// Affiliations
// Row # is affiliated with col #
bool aff[3][3] = {
    {0, 0, 0},
    {0, 1, 0},
    {0, 0, 1},
};

// State "types"
int type[3] = {0,1,1};
vector<string> observationKey{"A", "C", "G", "U"};
vector<string> stateKey{"S", "P", "Q", "Z"};

// A : 0
// C : 1
// G : 2
// U : 3

vector<int> O{1, 2, 2, 2, 1, 2, 2, 2, 1, 0, 0, 1, 1, 1, 1, 3, 3, 3, 2, 0, 2, 0, 0, 0, 1, 1, 1, 1};


const int numStates = sizeof(T)/sizeof(T[0]);
const int numSteps = O.size();

bool validInf(vector<vector<vector<int>>> &influence, int ingState, int edState, int ingStep, int edStep){
    vector<int> path = influence[type[ingState]][type[edState]];
    int total = 0;
    if (!allowedInfs[ingState][edState]){
        return false;
    }
    for (int i = ingStep; i < edStep; i++){
        total += path[i];
        if (total < 0){
            //cout << "false";
            return false;
        }
    }

    //cout << path.size() << " " << ingStep << "\n";

    return (total == 0) && (path[ingStep] != 1);
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
                                //curUnused[r].push(j);
                                //cout << j << " " << r << " " << l <<" " << q << " " << k << " " << cc <<" " << cp << "\n";
                                //cout << cc;
                                double prev = m[j-1][q][k][cp] * T[q][r];
                                double prob = 0;

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
                                    (l < j-3) &&
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
                                    (l < j-3) &&
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

                                
                                if (j == 12 && r == 1 && cc==0 && l == 8){

                                    cout << "(" << j << " " << stateKey[r] << " " << l << " " << cc << ") " << "(" << j-1 << " " << stateKey[q] << " " << k << " " << cp << ") " << prob << " " << m[j-1][q][k][cp] << "\n"; 
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
                    if (c == 0){
                        influences[j-1] = ')';
                        influences[l-1] = '(';
                    }
                    
                    path[j-1] = stateKey[r];
                    longest = stateKey[r].length();
                    
                    cout << j << " " << r << " "<< l << " " << c << ": " << m[j][r][l][c] << "\n";
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
            influences[j-1] = ')';
            influences[l-1] = '(';
        }
        path[j-1]= stateKey[r];
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