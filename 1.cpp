#include <assert.h>
#include <sstream>
#include "qna_tool.h"


using namespace std;

#define FILENAME "mahatma-gandhi-collected-works-volume-1.txt"


string decap1(string s){
    string d = s;
    int k = s.length();

    for(int i = 0;i<k;i++){
        if(int(s[i])<=90 && int(s[i] >=65)){int c = int(s[i])+32;d[i] = char(c);}
    }

    return d;
}
vector<string> parser1(string s){
    vector<string> ans = {};
    string d = s;
    int k = s.length();
    int i = 0;
    d.push_back('.');
    string temp= "";
    while(i<=k){//int q = int(d[i]);
        if(d[i] == ' ' || d[i] == '\"' || d[i] == '.' || d[i] == ',' || d[i] == '-' || d[i] == ':' || d[i] == '!' || d[i] == '(' || d[i] == ')' || d[i] == '?' || d[i] == '[' || d[i] == ']' || d[i] == '\'' || d[i] == ';' || d[i] == '@' ){//|| d[i] == '—' || d[i] == '˙' || d[i] == '”' || d[i] == '“' || d[i] == '’' || d[i] == '‘' ){
            //if(q<48 || (q<65&& q>57) || (q>90 && q<=96) || q>=123){
            if(temp != ""){ans.push_back(temp);
            temp = "";}

            else{
                temp = "";
            }
        }

        else{
            temp.push_back(d[i]);
        }

        i++;
    }

    int k1 = ans.size();

    for(int i = 0;i<k1;i++){
        ans[i] = decap1(ans[i]);
    }

    return ans;


}


vector<string> QNA_tool::preProcess(const string& sentence){
    vector<string> words;
    string word;
    const std::string delimiters = " .-:!()""''`\"\'?[]\"\';@,";
    for(char c: sentence){
        if(delimiters.find(c) == std::string::npos){ //if not in delimiters
            word+=tolower(c);
        }else{
            if(!word.empty()){
                if(!trie->search(word)){
                    words.push_back(word);
                    word.clear();
                }
            }
        }
    }
    if(!word.empty()){
        if(!trie->search(word)){
                words.push_back(word);
                word.clear();
        }
    }
    return words;
}

void merge(vector<vector<float>>& a,int p,int q,int r) {
    int a1 = q-p+1;
    int a2 = r-q;
    vector<vector<float>> m,n;
    for(int i =0;i<a1;i++){
        m.push_back(a[p+i]);
    }
    for(int i =0;i<a2;i++){
        n.push_back(a[q+i+1]);
    }
    int i =0;
    int j =0;
    int ind = p;
    while(i<a1 && j<a2){
        if(m[i][2] <= n[j][2]){
            a[ind] = m[i];i++;
        }else{
            a[ind] = n[j];j++;
        }
        ind++;
    }
    while(i<a1){
        a[ind] = m[i];
        ind++;
        i++;
    }
    while(j<a2){
        a[ind] = n[j];
        ind++;
        j++;
    }
}

void merge_sort(vector<vector<float>>& a,int left,int right){
    int middle;
    if(left<right){
        middle = (right-left)/2 + left;
        merge_sort(a,left,middle);
        merge_sort(a,middle + 1, right);
        merge(a,left, middle,right);
    }
}

QNA_tool::QNA_tool(){
    // Implement your function here
    dict = new Dict();  
    se = new SearchEngine();
}

QNA_tool::~QNA_tool(){
    // Implement your function here  
    delete dict;
    delete se;
}

void QNA_tool::insert_sentence(int book_code, int page, int paragraph, int sentence_no, string sentence){
    // Implement your function here
   // cout<<"h-1"<<endl;
   se->SearchEngine::insert_sentence( book_code,  page,  paragraph,  sentence_no,  sentence);
    dict->Dict::insert_sentence(book_code, page, paragraph, sentence_no,sentence);
    //cout<<"h0"<<endl;
    return;
}

Node* QNA_tool::get_top_k_para(string question, int k) {          //errors 
    // Implement your function here
   //    cout<<k<<endl;
    cout<<"1";
    Node* ans = NULL;
    //cout<<"h1"<<endl;
    Node* tail = ans;
    //cout<<"h2"<<endl;
    vector<string> a = parser1(question); 
    int k1 = a.size();
    vector<vector<vector<float>>> scores;

    //cout<<"h30"<<endl;

    for(int i = 0;i<k1;i++){

    string filename = "unigram_freq.csv";
    string targetWord = a[i];
    long long count = 1;
    ifstream file(filename);

    string line;
    while (getline(file, line)) {
    istringstream iss(line);
    string word;
    string countStr;

    if (getline(iss, word, ',') && getline(iss, countStr)) {
        if (word == targetWord) {
            count = std::stoll(countStr);

        }
    }
}
cout<<"2";
file.close(); 
    
//cout<<count<<endl;

file.close();  
//cout<<"h3"<<endl;

        int c = 0;
        //cout<<c<<endl;
        Node* head = se->SearchEngine::search(a[i],c);
//cout<<c<<endl;
        for(int h = 0;h<c;h++){
            int k2 = scores.size();
            if(k2<head->paragraph +1){
                while(k2<head->paragraph+1){
                    vector<vector<float>> temp = {};
                    scores.push_back(temp);
                    k2+=1;
                }
            }
            int l = scores[head->paragraph].size();
            int j;

            for(j=0;j<l;j++){

                if(scores[head->paragraph][j][0] == head->page){break;}


             }

             if(j == l){
                float score = (1 + dict->Dict::get_word_count(a[i]))/(1+count);
                //cout<<score<<endl;
                vector<float> temp = {float(head->page),score,float(head->book_code)};

                scores[head->paragraph].push_back(temp);
             }
             else{float score = (1 + dict->Dict::get_word_count(a[i]))/(1+count);
                scores[head->paragraph][j][1]+= score;
             }

             head = head->right;
            
        }

        
    }
cout<<"3";
    vector<vector<float>> rankings;

    int k2 = scores.size();

   // cout<<k<<" "<<k2<<endl;

    for(int i = 0;i<k2;i++){
        if(scores[i].size()!=1){
            int l = scores[i].size();

            for(int j = 1;j<l;j++){
                vector<float> temp = {float(i),scores[i][j][0],scores[i][j][1],scores[i][j][2]};
                rankings.push_back(temp);
            }
        }
    }
//cout<<"h4"<<endl;
k2 = rankings.size();
cout<<"4";
//cout<<k2<<endl;

merge_sort(rankings ,0,k2-1);
k2 = min(k2,k);
    for(int i = 0;i<k2;i++){

Node* ournode = new Node(static_cast<int>(rankings[i][3]),static_cast<int>(rankings[i][1]),static_cast<int>(rankings[i][0]),0,0);

if(ans == NULL){
ans = ournode;
tail = ournode;
} 
else{
ournode->left = tail;
tail->right = ournode;
tail = ournode;
}
    }
    cout<<"5";
   // cout<<"h5"<<endl;
    return ans;
}

//don't change 
void QNA_tool::query(string question, string filename){
    // Implement your function here  
    std::cout << "Q: " << question << std::endl;
    std::cout << "A: Studying COL106 :)" << std::endl;
    return;
}

std::string QNA_tool::get_paragraph(int book_code, int page, int paragraph){

    cout << "Book_code: " << book_code << " Page: " << page << " Paragraph: " << paragraph << endl;
    
    std::string filename = "mahatma-gandhi-collected-works-volume-";
    filename += to_string(book_code);
    filename += ".txt";

    std::ifstream inputFile(filename);

    std::string tuple;
    std::string sentence;

    if (!inputFile.is_open()) {
        std::cerr << "Error: Unable to open the input file " << filename << "." << std::endl;
        exit(1);
    }

    std::string res = "";

    while (std::getline(inputFile, tuple, ')') && std::getline(inputFile, sentence)) {
        // Get a line in the sentence
        tuple += ')';

        int metadata[5];
        std::istringstream iss(tuple);

        // Temporary variables for parsing
        std::string token;

        // Ignore the first character (the opening parenthesis)
        iss.ignore(1);

        // Parse and convert the elements to integers
        int idx = 0;
        while (std::getline(iss, token, ',')) {
            // Trim leading and trailing white spaces
            size_t start = token.find_first_not_of(" ");
            size_t end = token.find_last_not_of(" ");
            if (start != std::string::npos && end != std::string::npos) {
                token = token.substr(start, end - start + 1);
            }
            
            // Check if the element is a number or a string
            if (token[0] == '\'') {
                // Remove the single quotes and convert to integer
                int num = std::stoi(token.substr(1, token.length() - 2));
                metadata[idx] = num;
            } else {
                // Convert the element to integer
                int num = std::stoi(token);
                metadata[idx] = num;
            }
            idx++;
        }

        if(
            (metadata[0] == book_code) &&
            (metadata[1] == page) &&
            (metadata[2] == paragraph)
        ){
            res += sentence;
        }
    }

    inputFile.close();
    return res;
}


void QNA_tool::query_llm(string filename, Node* root, int k, string API_KEY, string question){

    // first write the k paragraphs into different files

    Node* traverse = root;
    int num_paragraph = 0;

    while(num_paragraph < k){
        assert(traverse != nullptr);
        string p_file = "paragraph_";
        p_file += to_string(num_paragraph);
        p_file += ".txt";
        // delete the file if it exists
        remove(p_file.c_str());
        ofstream outfile(p_file);
        string paragraph = get_paragraph(traverse->book_code, traverse->page, traverse->paragraph);
        assert(paragraph != "$I$N$V$A$L$I$D$");
        outfile << paragraph;
        outfile.close();
        traverse = traverse->right;
        num_paragraph++;
    }

    // write the query to query.txt
    ofstream outfile("query.txt");
    outfile << "These are the excerpts from Mahatma Gandhi's books.\nOn the basis of this, ";
    outfile << question;
    // You can add anything here - show all your creativity and skills of using ChatGPT
    outfile.close();
 
    // you do not need to necessarily provide k paragraphs - can configure yourself

    // python3 <filename> API_KEY num_paragraphs query.txt
    string command = "python3 ";
    command += filename;
    command += " ";
    command += API_KEY;
    command += " ";
    command += to_string(k);
    command += " ";
    command += "query.txt";

    system(command.c_str());
    return;
}