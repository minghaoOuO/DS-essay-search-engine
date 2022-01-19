#define FILE_EXTENSION ".txt"
#include<fstream>
#include<string>
#include<vector>
#include<iostream>
#include <sys/stat.h>
#include <stack>
#include <time.h>

using namespace std;

// ver7.cpp
// using trie plus ternary search tree to implement essay search engine
// using stack to store query
// 1 / 17 am 12:12 finish
// 0.2 second faster than ver1

const int level_t = 4;  // 5 :  11 GB in 10w data

// ternaryNode class
class ternaryNode{
    public:
        ternaryNode *left, *eq, *right;
        char c;
        bool ifend;
        bool empty;
        ternaryNode();
        ~ternaryNode();
        void insert(string str, int index);
        bool search(string str);
        bool exact_search(string str);
        bool prefix_search(string str);
        bool suffix_search(string str);
        void inverse_insert(string str, int index);
};

ternaryNode::ternaryNode(){
    left = eq = right = NULL;
    ifend = false;
    c = ' ';
    empty = 1;
};

ternaryNode::~ternaryNode(){

};

void ternaryNode::insert(string str, int index){
    if (str.length() == 0) return;
    if (index > str.length() - 1) return; 
    // lowercase 
    char ch = str[index];
    if (ch < 'a') ch = ch - 'A' + 'a';

    if (this->c == ' ') {
        //this->empty = 0;
        this->c = ch;
        if (index == str.length() - 1) ifend = true;
        else {
            if (!this->eq) this->eq = new ternaryNode;
            this->eq->insert(str, index + 1);
        }
    }
    else if (this->c == ch) {
        if (index == str.length() -1) ifend = true;
        else {
            if (!this->eq)  this->eq = new ternaryNode;
            this->eq->insert(str, index + 1);
        }
    }
    else if (this->c < ch) {
        if (!this->right)  this->right = new ternaryNode;
        this->right->insert(str, index);
    }
    else if (this->c > ch) {
        if (!this->left) this->left = new ternaryNode;
        this->left->insert(str, index);
    }
};

bool ternaryNode::exact_search(string str) {
    int index = 0;
    ternaryNode *tmp = this;
    while (true) {
        if (tmp == NULL) return false;
        //if (tmp->c == ' ') return false;
        // lowercase
        char ch = str[index];
        if (ch < 'a') ch = ch - 'A' + 'a';
        if (ch == tmp->c) {
            if (index == str.length() - 1) break;
            tmp = tmp->eq;
            index++;
        }
        else if (ch > tmp->c) tmp = tmp->right;
        else if (ch < tmp->c) tmp = tmp->left;
    }
    return tmp->ifend;
}

bool ternaryNode::prefix_search(string str){
    int index = 0;
    ternaryNode *tmp = this;
    while (true) {
        if (tmp == NULL) return false;
        //if (tmp->c == ' ') return false;
        char ch = str[index];
        if (ch < 'a') c = c - 'A' + 'a';
        if (ch == tmp->c) {
            if (index == str.length() - 1) break;
            tmp = tmp->eq;
            index++;
        }
        else if (ch > tmp->c) tmp = tmp->right;
        else if (ch < tmp->c) tmp = tmp->left;
    }
    return true;
}

bool ternaryNode::suffix_search(string str){
    int index = str.length() - 1;
    ternaryNode *tmp = this;
    while (true) {
        if (tmp == NULL) return false;
        if (tmp->c == ' ') return false;
        char ch = str[index];
        if (ch < 'a') ch = ch - 'A' + 'a';
        if (ch == tmp->c) {
            if (index == 0) break;
            tmp = tmp->eq;
            index--;
        }
        else if (ch > tmp->c) tmp = tmp->right;
        else if (ch < tmp->c) tmp = tmp->left;
    }
    return true;
}
        
void ternaryNode::inverse_insert(string str, int index) {
    if (str.length() == 0) return;
    if (index < 0) return;
    // lowercase
    char ch = str[index];
    if (ch < 'a') ch = ch - 'A' + 'a';
    if (this->c == ' ') {
        this->c = ch;
        //this->empty = 0;
        if (index == 0) ifend = true;
        else {
            if (!this->eq) this->eq = new ternaryNode;
            this->eq->inverse_insert(str, index - 1);
        }
    }
    else if (this->c == ch) {
        if (index == 0) ifend = true;
        else {
            if (!this->eq) this->eq = new ternaryNode;
            this->eq->inverse_insert(str, index - 1);
        }
    }
    else if (this->c < ch) {
        if (!this->right)  this->right = new ternaryNode;
        this->right->inverse_insert(str, index);
    }
    else if (this->c > ch) {
        if (!this->left) this->left = new ternaryNode;
        this->left->inverse_insert(str, index);
    }
}

bool ternaryNode::search(string str) {
    int index = 0;
    ternaryNode *tmp = this;
    while (true) {
        if (tmp->c == ' ') return false;
        if (str[index] == tmp->c) {
            if (index == str.length() - 1) break;
            tmp = tmp->eq;
            index++;
        }
        else if (str[index] > tmp->c) tmp = tmp->right;
        else if (str[index] < tmp->c) tmp = tmp->left;
    }
    if (tmp->ifend) return true;
    else return false;
};

void Inverse_insert(ternaryNode **root, string &str, int index) {
    if (str.length() == 0) return;
    // empty
    // lowercase
    char ch = str[index];
    if (ch < 'a') ch = ch - 'A' + 'a';
    if (*root == NULL) {
        *root = new ternaryNode;
        (*root)->c = ch;
    }
    else if ((*root)->c == ch) {
        if (index == 0) (*root)->ifend = true;
        else Inverse_insert(&(*root)->eq, str, index - 1);
    }
    else if ((*root)->c < ch) {
        Inverse_insert(&(*root)->right, str, index);
    }
    else if ((*root)->c > ch) {
        Inverse_insert(&(*root)->left, str, index);
    }
}

// trie Node class
const int ALPHABATSIZE = 26;

class Node{
    public:
        Node **children;
        bool ifend;
        ternaryNode *terNode;
        int level;
        Node(){
            children = new Node*[ALPHABATSIZE];
            for (int i = 0 ; i < ALPHABATSIZE; i++) {
                children[i] = NULL;
            }
            ifend = false;
            level = -1;
            terNode = NULL;
        };
        ~Node(){
            //this->clear();
        };
        void insert(string str){
            Node *tmp = this;
            int temp_level = 0;
            int ifter = 0;
            for (auto c : str) {
                int index = c - 'a';
                if (index < 0) index = c - 'A';
                if (temp_level < level_t) {
                    if (!tmp->children[index]) {
                        tmp->children[index] = new Node;
                        tmp->children[index]->level = temp_level;
                    }
                    temp_level++;
                    tmp = tmp->children[index];
                }
                else {
                    ifter = 1;
                    if (!tmp->terNode) tmp->terNode = new ternaryNode;
                    tmp->terNode->insert(str, level_t);
                    break;
                }
            }
            if (!ifter) tmp->ifend = true;
        };
        void inverse_insert(string str) {
            Node *tmp = this;
            int temp_level = 0;
            int ifter = 0;
            for (auto it = str.rbegin(); it != str.rend(); it++) {
                int index = *it - 'a';
                if (index < 0) index = *it - 'A';
                if (temp_level < level_t) {
                    if (!tmp->children[index]) {
                        tmp->children[index] = new Node;
                        tmp->children[index]->level = temp_level;
                    }
                    tmp = tmp->children[index];
                    temp_level++;
                }
                else {
                    ifter = 1;
                    if (!tmp->terNode) tmp->terNode = new ternaryNode;
                    tmp->terNode->inverse_insert(str, str.length() - 1 - level_t);
                    break;
                }
            }
            if (!ifter) tmp->ifend = true;
        };
        bool exact_search(string str){
            Node *tmp = this;
            int temp_level = 0;
            for (auto c : str) {
                int index = c - 'a';
                if (index < 0) index = c - 'A';
                if (temp_level < level_t) {
                    if (!tmp->children[index]) {
                        return false;
                    }
                    tmp = tmp->children[index];
                    temp_level++;
                }
                else break;
            }
            if (temp_level < level_t){
                return tmp->ifend;
            } 
            else {
                string s = str;
                s.assign(s, level_t, s.length() - level_t);
                if (s.length() == 0) return tmp->ifend;
                else return tmp->terNode->exact_search(s);
            }
        };
        bool suffix_search(string str){
            Node *tmp = this;
            int temp_level = 0;
            for (auto it = str.rbegin(); it != str.rend(); it++) {
                int index = *it - 'a';
                if (index < 0) index = *it - 'A';
                if (temp_level < level_t) {
                    if (!tmp->children[index]) {
                        return false;
                    }
                    tmp = tmp->children[index];
                    temp_level++;
                }
                else break;
            }
            if (temp_level < level_t){
                return true;
            } 
            else {
                string s;
                s.assign(str, 0, str.length() - level_t);
                if (s.length() == 0) return true;
                else return tmp->terNode->suffix_search(s);
            }
        };
        bool prefix_search(string str){
            Node *tmp = this;
            int temp_level = 0;
            for (auto c : str) {
                int index = c - 'a';
                if (index < 0) index = c - 'A';
                if (temp_level < level_t) {
                    if (!tmp->children[index]) {
                        return false;
                    }
                    tmp = tmp->children[index];  
                    temp_level++;
                }
                else break;
            }
            if (temp_level < level_t){
                return true;
            } 
            else {
                string s = str;
                s.assign(s, level_t, s.length() - level_t);
                if (s.length() == 0) return true;
                else return tmp->terNode->prefix_search(s);
            }
        };
};

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

class essay{
    public:
        string title;
        Node *root;
        Node *inverse_root;
        essay(){
            root = new Node;
            inverse_root = new Node;
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
            if (count == 50) break;
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

    clock_t end = clock();
    float seconds = (float)(end - start) / CLOCKS_PER_SEC;
    cout << "implement : " << seconds << " seconds\n";

    // read query
    fi.open(query, ios::in);
    ofstream ansfile;
    ansfile.open (output);
    int query_cnt = 0;
    while (getline(fi, tmp)) {
        vector<string> content = split(tmp, " ");
        stack<string> query_stack;
        vector<bool> found(Essay_vector.size(), false);

        for (auto rit = content.rbegin(); rit != content.rend(); rit++) {
            query_stack.push(*rit);
        }
        // do the first search
        string s;

        s = query_stack.top(); query_stack.pop();
        //cout << s << endl;
        for (int i = 0; i < Essay_vector.size(); i++) {
            if (Essay_vector[i].search(s)) {
                found[i] = true;
            }
        }
        while (!query_stack.empty()) {
            string op = query_stack.top(); query_stack.pop();
            string s = query_stack.top(); query_stack.pop();
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
            if (query_stack.empty()) break;
        }
        //ansfile << "Query : " << query_cnt << endl;
        query_cnt++;
        int strfound = 0;
        for (int i = 0; i < Essay_vector.size(); i++) {
            if (found[i]) {
                strfound = 1;
                ansfile << Essay_vector[i].title << endl;
            }
        }
        if (strfound == 0) ansfile << "Not Found!\n";
    }
    ansfile.close();
    fi.close();

    clock_t s_end = clock();
    seconds = (float)(s_end - end) / CLOCKS_PER_SEC;
    cout << "search : " << seconds << " seconds\n";

    return 0;
}