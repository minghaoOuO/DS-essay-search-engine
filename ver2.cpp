#define FILE_EXTENSION ".txt"
#include<fstream>
#include<string>
#include<cstring>
#include<vector>
#include<iostream>
#include <sys/stat.h>
#include <stack>

#include <time.h>

using namespace std;

// ver2.cpp
// using trie to implement essay search engine
// using array to store query 
// search function speed up

// Utility Func

// string parser : output vector of strings (words) after parsing
vector<string> word_parse(vector<string> tmp_string){
	vector<string> parse_string;
	for(auto& word : tmp_string){
		string new_str;
    	for(auto &ch : word){
			if(isalpha(ch))
				new_str.push_back(ch);
		}
		parse_string.emplace_back(new_str);
	}
	return parse_string;
}

vector<string> split(const string& str, const string& delim) {
	vector<string> res;
	if("" == str) return res;
	//先將要切割的字串從string型別轉換為char*型別
	char * strs = new char[str.length() + 1] ; //不要忘了
	strcpy(strs, str.c_str());

	char * d = new char[delim.length() + 1];
	strcpy(d, delim.c_str());

	char *p = strtok(strs, d);
	while(p) {
		string s = p; //分割得到的字串轉換為string型別
		res.push_back(s); //存入結果陣列
		p = strtok(NULL, d);
	}

	return res;
}

const int ALPHABATSIZE = 26;

class trie{
    public:
        trie **children;
        bool ifend;
        trie(){
            children = new trie*[ALPHABATSIZE];
            for (int i = 0 ; i < ALPHABATSIZE; i++) {
                children[i] = NULL;
            }
            ifend = false;
        };
        ~trie(){
            //this->clear();
        };
        void insert(string str){
            trie *tmp = this;
            for (auto c : str) {
                int index = c - 'a';
                if (index < 0) index = c - 'A';
                if (!tmp->children[index]) {
                    tmp->children[index] = new trie;
                }
                tmp = tmp->children[index];
            }
            tmp->ifend = true;
        };
        void inverse_insert(string str) {
            trie *tmp = this;
            for (auto it = str.rbegin(); it != str.rend(); it++) {
                int index = *it - 'a';
                if (index < 0) index = *it - 'A';
                if (!tmp->children[index]) {
                    tmp->children[index] = new trie;
                }
                tmp = tmp->children[index];
            }
            tmp->ifend = true;
        };
        bool exact_search(string str){
            trie *tmp = this;
            for (auto c : str) {
                int index = c - 'a';
                if (index < 0) index = c - 'A';
                if (!tmp->children[index]) {
                    return false;
                }
                tmp = tmp->children[index];
            }
            if (tmp->ifend == true) return true;
            else return false;
        };
        bool suffix_search(string str){
            trie *tmp = this;
            for (auto it = str.rbegin(); it != str.rend(); it++) {
                int index = *it - 'a';
                if (index < 0) index = *it - 'A';
                if (!tmp->children[index]) {
                    return false;
                }
                tmp = tmp->children[index];
            }
            return true;
        };
        bool prefix_search(string str){
            trie *tmp = this;
            for (auto c : str) {
                int index = c - 'a';
                if (index < 0) index = c - 'A';
                if (!tmp->children[index]) {
                    return false;
                }
                tmp = tmp->children[index];
            }
            return true;
        };
};

class essay{
    public:
        string title;
        trie *root;
        trie *inverse_root;
        essay(){
            root = new trie;
            inverse_root = new trie;
        };
        bool search(string str) {
            if (str[0] == '"' && str[str.length() - 1] == '"') {
                string s;
                s = s.assign(str, 1, str.length() - 2);
                return this->root->exact_search(s);
            }
            else if (str[0] == '*' && str[str.length() - 1] == '*') {
                string s;
                s = s.assign(str, 1, str.length() - 2);
                return this->inverse_root->suffix_search(s);
            }
            else return this->root->prefix_search(str);
        };
        ~essay(){};
};

int main(int argc, char *argv[])
{
    clock_t start = clock();

    // INPUT :
	// 1. data directory in data folder
	// 2. number of txt files
	// 3. output route

    string data_dir = argv[1] + string("/");
	string query = string(argv[2]);
	string output = string(argv[3]);

	// Read File & Parser Example

	string file, title_name, tmp;
	fstream fi;
	vector<string> tmp_string;
    vector<essay> Essay_vector;

    int file_index = 0;
    int count = 0;
    while (true) {
        // check if file exist!
        string data_path = data_dir + to_string(file_index) + ".txt";
        struct stat buf;
        if (stat(data_path.c_str(), &buf) == -1) break;
        file_index++;
        if (file_index == 1000) {
            count++;
            if (count == 10) break;
            file_index = 0;
        }

        essay e;
        // get title name
        fi.open(data_path, ios::in);
        getline(fi, title_name);
        e.title = title_name;

        // get title word vector
        tmp_string = split(title_name, " ");
        vector<string> title = word_parse(tmp_string);
        for (auto word : title) {
            e.root->insert(word);
            e.inverse_root->inverse_insert(word);
        }

        // get content line by line
        while(getline(fi, tmp)){
		    tmp_string = split(tmp, " ");
		    vector<string> content = word_parse(tmp_string);
		    for(auto &word : content){
                //cout << word << endl;
                e.root->insert(word);
                e.inverse_root->inverse_insert(word);
		    }
	    }
        Essay_vector.push_back(e);
        fi.close();
    }

    cout << Essay_vector.size() << endl;

    int query_count = 0;
    ofstream ansfile;
    ansfile.open (output);
    while(query_count--) {
        cout << query_count << endl;
        // read query
        fi.open(query, ios::in);
        while (getline(fi, tmp)) {
            vector<string> content = split(tmp, " ");
            vector<bool> found(Essay_vector.size(), false);

            int content_index = 0;
            // do the first search
            string s = content[content_index];
            content_index++;
            //cout << s << endl;
            for (int i = 0; i < Essay_vector.size(); i++) {
                if (Essay_vector[i].search(s)) {
                    found[i] = true;
                }
            }
            while (content_index < content.size()) {
                string op = content[content_index];
                content_index++;
                string s = content[content_index];
                content_index++;
                if (op == "+") {
                    for (int i = 0; i < Essay_vector.size(); i++) {
                        if (found[i] == false) continue;
                        if (!Essay_vector[i].search(s)) {
                            found[i] = false;
                        }
                    }
                }
                else if (op == "/") {
                    for (int i = 0; i < Essay_vector.size(); i++) {
                        if (found[i] == true) continue;
                        if (Essay_vector[i].search(s)) {
                            found[i] = true;
                        }
                    }
                }
            }
            int strfound = 0;
            for (int i = 0; i < Essay_vector.size(); i++) {
                if (found[i]) {
                    strfound = 1;
                    ansfile << Essay_vector[i].title << endl;
                }
            }
            if (strfound == 0) ansfile << "Not Found!\n";
        }
        fi.close();
    }
    ansfile.close();

    clock_t end = clock();
    float seconds = (float)(end - start) / CLOCKS_PER_SEC;
    cout << seconds << " seconds\n";

    return 0;
}