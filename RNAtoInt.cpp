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

int main(){
    map<char, int> key;
    key['A'] = 0;
    key['C'] = 1;
    key['G'] = 2;
    key['U'] = 3;
    string toConvert = "CAAACCAAACCCUUUUUUCCAAACCCCUUUC";
    for (int i = 0; i < toConvert.length()-1; i++){
        cout << key[toConvert[i]] << ", ";
    }
    cout << key[toConvert[toConvert.length()-1]];

}