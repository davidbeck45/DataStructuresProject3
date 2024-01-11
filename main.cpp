#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <fstream>
#include <functional>
#include <set>
#include <map>
using namespace std;

//functs
void dict_file(vector<string>& dic, const string& filename);
unsigned int get_word_len(const vector<string>& dic);
char letter_funct(const set<char>& cashed);
void shrink_dict(vector<string>& dic, function<bool(string)> t);
string wordToFamily(string word, string curr, char newGuess);
void findFamilies(map<string, int>& families, const vector<string>& dic, char newGuess, string curr);
void playAGame(vector<string>& dic, int guesses, bool showAmount);

// this function is to acess the txt file and read all values into a vect
void dict_file(vector<string>& dic, const string& filename){
    ifstream my_file(filename);
    string element;
    while (my_file >> element) {
        dic.push_back(element);
    }
}
// get length of word thus letting me narrow down the list
unsigned int get_word_len(const vector<string>& dic){
    unsigned int length;
    while (true) {
        cout << "How long of a word would you like: ";
        cin >> length;
        for (string word : dic) {
            if (word.size() == length)
                return length;
        }
        cout << "Could not find any words of length " << length << endl;
    }
}
// function enabling the user to guess the owrd
char letter_funct(const set<char>& cashed){
    string letter;
    while (true) {
        cout << "Enter a letter: ";
        cin >> letter;
        if (letter.size() != 1 || !isalpha(letter[0]))
            cout << letter << " is not a letter." << endl;
        else if (cashed.count(letter[0]) > 0)
            cout << "You have already guessed " << letter[0] << endl;
        else
            break;
    }
    return letter[0];
}

void shrink_dict(vector<string>& dic, function<bool(string)> t){
    dic.erase(remove_if(dic.begin(), dic.end(), t), dic.end());
}

string wordToFamily(string word, string curr, char newGuess){
    string family = curr;
    for (size_t i = 0; i < word.length(); i++) {
        if (word[i] == newGuess)
            family[i] = newGuess;
    }
    return family;
}

void findFamilies(map<string, int>& families, const vector<string>& dic, char newGuess, string curr){
    for (string word : dic) {
        string family = wordToFamily(word, curr, newGuess);
        families[family]++;
    }
}

void playAGame(vector<string>& dic, int guesses, bool showAmount){
    set<char> cashed;
    string curr(dic[0].size(), '-');

    while (guesses > 0 && curr.find('-') != string::npos) {

        cout << "You have " << guesses << " guesses left.\n";
        cout << curr << endl;
        cout << "These are the letters you have guessed: ";
        for (char used : cashed)
            cout << used << ' ';
        cout << endl;

        if (showAmount)
            cout << "There are " << dic.size() << " words left in the dictionary.\n";


        char newGuess = letter_funct(cashed);
        cashed.insert(newGuess);


        map<string, int> families;
        findFamilies(families, dic, newGuess, curr);


        string biggestFamily;
        size_t biggestSize = 0;
        for (auto family : families) {

            string familyName = family.first;

            size_t familySize = family.second;

            if (familySize > biggestSize) {

                biggestFamily = familyName;

                biggestSize = familySize;
            }
        }


        bool madeCorrectGuess = curr != biggestFamily;

        if (!madeCorrectGuess)

            guesses--;

        curr = biggestFamily;


        shrink_dict(dic, [=](string word) {

            for (size_t i = 0; i < word.length(); i++) {

                if (curr[i] == '-' && word[i] == newGuess)
                    return true;

                if (curr[i] != '-' && curr[i] != word[i])
                    return true;
            }

            return false;
        });

        cout << endl;
    }

    if (curr.find('-') == string::npos)

        cout << "Congratulations! You found the correct word: " << curr << endl;
    else

        cout << "You lost! The word was " << dic[0] << endl;
}

int main() {
    cout << "Welcome to Hangman." << endl;

    cout << "===================" << endl;

    bool game = true;

    while (game) {

        vector<string> dic;

        dict_file(dic, "dictionary.txt");



        unsigned int length = get_word_len(dic);

        shrink_dict(dic, [=](string word) { return word.size() != length; });

        cout << "How many guesses would you like: ";

        int guesses;

        cin >> guesses;

        cout << endl;

        if(guesses <= 0){

            cout << "ERROR, choose a positive number" << endl;
        }


        cout << "Do you want to show the amount of remaining words in the dictionary? [y/n]: ";

        char i;

        cin >> i;

        bool showAmount = i == 'y';


        playAGame(dic, guesses, showAmount);

        char playAgain;

        cout << "Do you want to play again? (y/n): ";
        cin >> playAgain;
        if (playAgain == 'n')
            game = false;
    }

    return 0;
}
