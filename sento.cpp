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
    return words;
}

int main() {
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

    vector<string> words = wordcheck(sent);

    int poscount = 0;
    int negcount = 0;
    bool containsBut = false;
    bool containsNot = false;

    for (const auto& word : words) {
        if (find(poswords.begin(), poswords.end(), word) != poswords.end()) {
            poscount++;
        }
        if (find(negwords.begin(), negwords.end(), word) != negwords.end()) {
            negcount++;
        }
        if (find(butcond.begin(), butcond.end(), word) != butcond.end()) {
            containsBut = true;
        }
        if (find(notcond.begin(), notcond.end(), word) != notcond.end()) {
            containsNot = true;
        }
    }

    // Original sentiment checking
    if (containsBut) {
        size_t butpos;
        string wordbefore;
        string wordafter;
        int wb, wa;
        wb = 0;
        wa = 0;
        for (const auto& tempo : butcond) {
            butpos = sent.find(tempo);
            if (butpos != string::npos) {
                wordbefore = sent.substr(0, butpos);
                wordafter = sent.substr(butpos + tempo.length());
                vector<string> wordbef = wordcheck(wordbefore);
                vector<string> wordaft = wordcheck(wordafter);

                for (const auto& wordb : wordbef) {
                    if (find(poswords.begin(), poswords.end(), wordb) != poswords.end()) {
                        wb++;
                    }
                }
                for (const auto& worda : wordaft) {
                    if (find(poswords.begin(), poswords.end(), worda) != poswords.end()) {
                        wa++;
                    }
                }
                butpos = sent.find(tempo, butpos + 1);
            }
        }

        // Adjusted sentiment based on "but"
        if (wb < wa) {
            cout << "the sentiment is positive";
            exit(0);
        } else if (wb > wa) {
            cout << "the sentiment is negative";
            exit(0);
        }
    } else {  // Reverse sentiment if "not" is present and "but" is not
        if (containsNot) {
            if (poscount > negcount) {
                cout << "the sentiment is negative";
                exit(0);
            } else if (poscount < negcount) {
                cout << "the sentiment is positive";
                exit(0);
            }
        }
    }

    if (poscount > negcount) {
        cout << "the sentiment is positive";
    } else if (poscount < negcount) {
        cout << "the sentiment is negativeeeee";
    } else {
        cout << "neutral";
    }

    return 0;
}
