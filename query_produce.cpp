#define FILE_EXTENSION ".txt"
#include<fstream>
#include<string>
#include<cstring>
#include<vector>
#include<iostream>
#include <sys/stat.h>
#include <stack>
#include <cstdlib>
#include <time.h>
using namespace std;

int main () {
    srand (time(NULL));
    string alpha = "abcdefghijklmnopqrstuvwxyz";
    string path = "random_query.txt";
    ofstream fi;
    fi.open(path);
    for (int count = 0; count < 10000; count++) {
        //cout << count << endl;
        string s = "";
        int type = rand() % 3;
        int length = 3 + rand() % 4;
        if (type == 0) {    // prefix
            for (int i = 0; i < length; i++){
                s = s + alpha[rand() % 26];
            }
            s = s + '\n';
        }
        else if (type == 1) {   // exact
            s = s + '"';
            for (int i = 0; i < length; i++){
                s = s + alpha[rand() % 26];
            }
            s = s + '"';
            s = s + '\n';
        }
        else {  // suffix
            s = s + '*';
            for (int i = 0; i < length; i++){
                s = s + alpha[rand() % 26];
            }
            s = s + '*';
            s = s + '\n';
        }
        fi << s;
    }
    fi.close();
    return 0;
}
