#include <iostream>
#include "hangman.h"
#include<cstdlib>
#include<ctime>
#include<string>
using std::string;
using std::vector;
using std::ifstream;
using std::domain_error;
using std::cin;
using std::cout;
/***
    Args:
        min (int): left margin of a range
        max (int): right margin of a range
    Returns:
        number (int) : random number in range [min; max]
***/
int generateRandomNumber(const int min, const int max)
{
    srand(time(0));

    // TODO: Return a random integer number between min and max
    return min + rand() %(max-min+1);
}

vector<string> readWordListFromFile(const string& filePath)
{
    vector<string> wordList;
    string word;
    ifstream wordFile (filePath);
    if (!wordFile.is_open()) {
        throw domain_error("Unable to open file");
    }

    //while ( getline (wordFile, word) ){  // Thong thuong doc tung line.
                                           // Chuong trinh nay cung chay.
    while (wordFile >> word) {  // Nhung voi chuong trinh nay, doc tung word cung duoc
                                // Tuc ca 2 cach doc deu chay.
        wordList.push_back(word);
        //cout << word << '\n';
    }
    wordFile.close();

    return wordList;
}

/***
    Args:
        ch (char): A character
        word (string): a word
    Returns:
        result (bool) : the character ch is in the word or not.
***/
bool isCharInWord(const char ch, const string& word)
{
    for(char c:word){// TODO: return true if ch is in word else return false
    if(c==ch)return true;
    }
    return false;
}

/***
    Args:
        wordList (vector<string>): A list of words
        index (int): an integer number
    Returns:
        answer (string) : the lowercase word is in the position index of wordList
***/
string chooseWordFromList(const vector<string>& wordList, int index)
{
    // TODO: Return a lowercase word in the index position of the vector wordList.
    if (wordList.empty()) {
        std::cerr << "Error: wordList is empty!" << std::endl;
        return "";
    }
    if (index < 0 || index >= wordList.size()) {
        std::cerr << "Error: index " << index << " is out of range!" << std::endl;
        return "";
    }
    return wordList[index];
}

/***
    Args:
        answerWord (string): a word that player needs to guess
    Returns:
        secretWord (string): answerWord in hidden form (form of ---)
***/
string generateHiddenCharacters(string answerWord){

    // TODO: Based on answerWord's length, generate hidden characters in form of "---"


    return string (answerWord.length(),' ');
}

char getInputCharacter() {
    char ch;
    cin >> ch;
    return tolower(ch);
}

/***
    Args:
        secretWord (string): secret word in hidden form
        ch (char): a charater
        word (string): the answer word
    Returns:
        void
***/
void updateSecretWord(string& secretWord, const char ch, const string& word)
{
    for(size_t i=0;i<word.length();i++){
        if( word[i]==ch){
            secretWord[i]=ch;
        }
    }
    // TODO: Update the secret word if the character ch is in the answer word.
}

/***
    Args:
        ch (char): a character
        chars (string): an array of characters
    Returns:
        void
***/
void updateEnteredChars(const char ch, string& chars){
    chars += ch;
    chars += ' ';
    // TODO: append the character ch is in end of the text chars
}

/***
    Args:
        incorrectGuess (int): a number that store the number of player's wrong guess
    Returns:
        void
***/
void updateIncorrectGuess(int& incorrectGuess){
    incorrectGuess++;// TODO: increase the value of incorrectGuess by 1
}

/***
    Args:
        ch (char): a character that player enter to console
        word (string): answer word that play needs to guess
        secretWord (string): answer word in hidden form
        correctChars (string): a string that stores correct inputs of player
        incorrectGuess (int): a number that stores the number of player's wrong guess
        incorrectChars (string): a string that stores incorrect inputs of player
    Returns:
        void
***/
void processData(const char ch, const string& word,
                string& secretWord,
                string& correctChars,
                int& incorrectGuess, string& incorrectChars)
{
    // Kiểm tra nếu chữ cái đã được đoán trước đó
    if (correctChars.find(ch) != string::npos || incorrectChars.find(ch) != string::npos) {
        cout << "You already guessed '" << ch << "'. Try a different letter!\n";
        return;
    }
    if(isCharInWord(ch,word)){
        updateSecretWord(secretWord,ch,word);
        updateEnteredChars(ch,correctChars);
    }
    else {
        updateIncorrectGuess(incorrectGuess);
        updateEnteredChars(ch,incorrectChars);
    }
    /*** TODO
        If ch in word:
            update secretWord: call updateSecretWord() function
            update correctChars: call updateEnteredChars() function
        else:
            update incorrectGuess: call updateIncorrectGuess() function
            update incorrectChars: call updateEnteredChars() function
    ***/
}

