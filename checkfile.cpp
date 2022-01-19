#include <iostream>
#include <fstream>
#include <string>
using namespace std;

int main(int argc, char* argv[]){
    string path1, path2;
    path1 = argv[1];
    path2 = argv[2];
    fstream f1, f2;
    f1.open(path1, ios::in);
    f2.open(path2, ios::in);
    string s1, s2;
    int same = 1;
    int i = 0;
    while(getline(f1, s1)) {
        i++;
        getline(f2, s2);
        if (s1 == s2) continue;
        else {
            same = 0;
            cout << "Not the same!\n";
            break;
        }
    }
    if (same) cout << "The same!\n";
    cout << "Lines " << i << endl;
    return 0;
}