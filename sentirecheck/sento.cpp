#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <fstream>
#include <sstream>
using namespace std;

vector<string> wordcheck(const auto& wordbreak) {   //this function breaks sentence into separate words, ignoring spaces.
    vector<string> words;
    istringstream iss(wordbreak);    //istringstream is the main function used
    string word;
    while(iss >> word) {
        words.push_back(word);
    }
    return words;}

    void layout(void){                             //function used for design layout
    cout<<"**********************************\n";
    cout<<"\tSentiment Analysis\n"<<endl;
    cout<<"**********************************\n";
    cout<<"Group members:\t M.Zain-ul-Abideen\n\t\t M.Owais\n\t\t Nimrah Saleem\n\t\t Laiba Khan\n";
    cout<<"**********************************\n"<<endl;
    }


int main() {
    layout();   //calling design from the function
    string sent;
    cout << "enter any sentence= ";    //enter your sentence here
    getline(cin, sent);

    vector<string> poswords;       //vector storing positive words from external file
    ifstream inputFile;
    inputFile.open("goodwords.txt");
    if (inputFile) {
        string positives;
        while (getline(inputFile, positives)) {
            poswords.push_back(positives);
        }
        inputFile.close();
    }

    vector<string> negwords;       //vector storing negative words from external file
    inputFile.open("badwords.txt");
    if (inputFile) {
        string negatives;
        while (getline(inputFile, negatives)) {
            negwords.push_back(negatives);
        }
        inputFile.close();
    }

    vector<string> notcond = {"not","NOT","Not"};  //vector for not condition

    vector<string> butcond;        //vector storing but words from external file used for but condtion
    inputFile.open("buts.txt");
    if (inputFile) {
        string buttemp;
        while (getline(inputFile, buttemp)) {
            butcond.push_back(buttemp);
        }
        inputFile.close();
    }


    // IMPORTION OF FILES TILL HERE.

    // converted to lowercase
    transform(sent.begin(), sent.end(), sent.begin(), ::tolower);

    vector<string> words = wordcheck(sent);   //sending our sentence to user-defined function to break it and store it in the vector

    int poscount = 0;            //assigned value 0 to variables so that they may not contain garbage value
    int negcount = 0;
    bool containsBut = false;     //similarly assigned false to bool variables
    bool containsNot = false;

    if(sent.empty()){cout<<"Input Error";}
    else{
    for (const auto& word : words) {              //accessing vector and storing each word from vector into a variable word
        if (find(poswords.begin(), poswords.end(), word) != poswords.end()) {  //if element from poswords vector are detected into the variable "word" then increment positive count
            poscount++;
        }
        if (find(negwords.begin(), negwords.end(), word) != negwords.end()) { //similarly for negative count
            negcount++;
        }
        if (find(butcond.begin(), butcond.end(), word) != butcond.end()) {  //if sentence contain elements from butcond vector then make the bool variable containsbut true
            containsBut = true;
        }
        if (find(notcond.begin(), notcond.end(), word) != notcond.end()) {  //similarly for vectors in notcond vector
            containsNot = true;
        }
    }

    if (containsBut) {    //if containsBut is true then we apply the condition for "but"
        size_t butpos;     //used size_t to measure its position in the string
        string wordbefore;      //variable to store words before the keyword "but"
        string wordafter;       //variable to store words after the keyword "but"
        int wb=0, wa=0;

        for (const auto& tempo : butcond) { //extracting elements from butcond variable and storing it in a variable tempo
            butpos = sent.find(tempo);             //if the keyword stored in tempo is found in the sentence, then save it in butpos meaning it will save its location.
            if (butpos != string::npos) {            // if the butpos isn't null then perform the action in if statement
                wordbefore = sent.substr(0, butpos);       //saving word before the keyword "but" by using substr function on our string
                wordafter = sent.substr(butpos + tempo.length());  //words after the "but" from the but position till the end.
                vector<string> wordbef = wordcheck(wordbefore);  //storing these words into vector by sending it in the function
                vector<string> wordaft = wordcheck(wordafter);

                for (const auto& wordb : wordbef) {
                    if (find(poswords.begin(), poswords.end(), wordb) != poswords.end()) {  //then perform the similar action done for positive count increment
                        wb++;
                    }
                }
                for (const auto& worda : wordaft) {
                    if (find(poswords.begin(), poswords.end(), worda) != poswords.end()) {
                        wa++;
                    }
                }
                butpos = sent.find(tempo, butpos + 1);   //used this statement to cover all the keywords present in the vector butcond
            }
        }


        if (wb < wa) {                              //checking for inequality and printing the output accordingly
            cout << "The Sentiment is Positive\n";
            exit(0);
        } else if (wb > wa) {
            cout << "The Sentiment is Negative\n";
            exit(0);
        }
    }      //if containsBut is false then perform this else below
     else {                  // Reverse sentiment if "not" is present and "but" is not
        if (containsNot) {
            if (poscount > negcount) {
                cout << "The Sentiment is Negative\n";
                exit(0);
            } else if (poscount < negcount) {
                cout << "The Sentiment is Positive\n";
                exit(0);
            }
        }
    }

    if (poscount > negcount) {                 //regular sentiment if neither but or not is found in the string
        cout << "The Sentiment is Positive\n";
    } else if (poscount < negcount) {
        cout << "The Sentiment is Negative\n";
    } else {
        cout << "The Sentiment is Neutral\n";
    }
    }
    return 0;
}
