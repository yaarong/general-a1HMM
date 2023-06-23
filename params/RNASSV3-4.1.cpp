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
    {(double)1/3 - 0.01, (double)1/3 + 0.02, (double)1/3 - 0.01},
    {(double)1/3 - 0.01, (double)1/3 - 0.01, (double)1/3 + 0.02}
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
int type[3] = {0,1,2};
vector<string> observationKey{"A", "C", "G", "U"};
vector<string> stateKey{"S", "P", "Q", "Z"};

// A : 0
// C : 1
// G : 2
// U : 3

vector<int> O{1, 2, 2, 2, 1, 2, 2, 2, 1, 0, 0, 1, 1, 1, 1, 3, 3, 3, 2, 0, 2, 0, 0, 0, 1, 1, 1, 1};