// Emission
// Rows: State
// Col: Observered
double E[2][4] = {
    {(double)1/4, (double)1/4, (double)1/4, (double)1/4},
    {(double)1/4, (double)1/4, (double)1/4, (double)1/4},
};

// Transmission
// Rows: from state
// Cols: to state
double T[2][2] = {
    {(double)1/2, (double)1/2},
    {(double)1/2, (double)1/2},
};
    
// iNfluence probabilities
// Rows: influencing observation
// Cols: influenced observation
double N[4][4] = {
    {(double)1/10, (double)1/10, (double)1/10, (double)7/10},
    {(double)1/10, (double)1/10, (double)7/10, (double)1/10},
    {(double)1/10, (double)7/10, (double)1/10, (double)1/10},
    {(double)7/10, (double)1/10, (double)1/10, (double)1/10}    
};

// Which states influence which states
// Rows: influencing state
// Cols: influenced state
bool allowedInfs[2][2]{
    {0,1},
    {0,0}
};

// Probability of starting with each state
double startProb[2] = {1,0};

// Which end-states are allowed (1 is allowed, 0 is disallowed)
bool endState[2] = {1, 1};

vector<string> observationKey{"A", "C", "G", "U"};
vector<string> stateKey{"S", "P"};

// A : 0
// C : 1
// G : 2
// U : 3

vector<int> O{1, 0, 0, 0, 1, 1, 0, 0, 0, 1, 1, 1, 3, 3, 3, 3, 3, 3, 1, 1, 0, 0, 0, 1, 1, 1, 1, 3, 3, 3, 1};