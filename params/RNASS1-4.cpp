// Emission
// Rows: State
// Col: Observered
double E[11][4] = {
    {(double)1/4, (double)1/4, (double)1/4, (double)1/4},
    {(double)1/4, (double)1/4, (double)1/4, (double)1/4},
    {(double)1/4, (double)1/4, (double)1/4, (double)1/4},
    {(double)1/4, (double)1/4, (double)1/4, (double)1/4},
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
double T[11][11] = {
    {(double)1/5, (double)4/5, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0},
    {(double)1/5, (double)1/5, 0, (double)1/5, (double)2/5, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, (double)4/5, (double)1/5, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, (double)1/5, (double)4/5, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0},
    {0, 0, 0, 0, 0, 0, 0, (double)1/5, 0, 0, (double)4/5},
    {0, 0, 0, 0, 0, 0, 0, (double)1/5, 0, 0, (double)4/5},
//   b  l1 l2 l3 m1 m2 m3 r3 r2 r1 e
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
bool allowedInfs[11][11]{
    {0, 0,0,0, 0,0,0, 0,0,0, 0},
    {0, 0,0,0, 0,0,0, 0,0,1, 0},
    {0, 0,0,0, 0,0,0, 0,1,0, 0},
    {0, 0,0,0, 0,0,0, 1,0,0, 0},
    {0, 0,0,0, 0,0,0, 0,0,0, 0},
    {0, 0,0,0, 0,0,0, 0,0,0, 0},
    {0, 0,0,0, 0,0,0, 0,0,0, 0},
    {0, 0,0,0, 0,0,0, 0,0,0, 0},
    {0, 0,0,0, 0,0,0, 0,0,0, 0},
    {0, 0,0,0, 0,0,0, 0,0,0, 0},
    {0, 0,0,0, 0,0,0, 0,0,0, 0},
};

// Probability of starting with each state
double startProb[11] = {0,1,0,0,0,0,0,0,0,0,0};

// Which end-states are allowed (1 is allowed, 0 is disallowed)
bool endState[11] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1};

vector<string> observationKey{"A", "C", "G", "U"};
vector<string> stateKey{"B", "L1", "L2", "L3", "M1", "M2", "M3", "R3", "R2", "R1", "E"};

// A : 0
// C : 1
// G : 2
// U : 3
vector<int> O{0, 0, 0, 0, 1, 1, 1, 1, 0, 2, 2, 2, 0, 0, 0, 0, 1, 1, 1, 2, 2, 2, 2, 0, 0, 0, 3, 3, 3};