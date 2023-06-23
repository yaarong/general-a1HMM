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
    {0.99, 0.01, 0},
    {0.2, 0.79, 0.01},
    {0.2, 0.01, 0.79}
};
    
// iNfluence probabilities
// Rows: influencing observation
// Cols: influenced observation
//  acgu
double N[4][4] = {
    {0,     0,      0,      1},
    {0,     0,      1,      0},
    {0,     0.76,   0,      0.24},
    {0.67,  0,      0.33,   0},
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
int type[3] = {0,1,2};
vector<string> observationKey{"A", "C", "G", "U"};
vector<string> stateKey{"S", "P", "Q"};

// A : 0
// C : 1
// G : 2
// U : 3

vector<int> O{1, 0, 0, 0, 0, 0, 0, 1, 2, 2, 2, 2, 2, 2, 1, 3, 3, 3, 3, 3, 3, 1, 1, 1, 1, 1, 1};