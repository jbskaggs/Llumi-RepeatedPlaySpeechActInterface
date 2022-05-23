#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <unistd.h>

using namespace std;

struct RoundInfo {
    int actions[2];
    string speeches[2];
    string algState;
};

vector<string> split(const string& s) {
   vector<string> tokens;
   string token;
   istringstream tokenStream(s);
   while (getline(tokenStream, token, ':'))
      tokens.push_back(token);
   return tokens;
}

bool getRoundInformation(int g, RoundInfo &curInfo, string inputFilename) {
    while (true) {
        //cout << "theJBS getRoundInformation whileloop: " << inputFilename << endl;
        ifstream input(inputFilename);
        if (!input.good()) {
            cout << "theJBS: file name not found: " << inputFilename << endl;
            usleep(100000);
        }
        else {
            int lastRound;
            string line;
            //cout << "theJBS: antes" << endl;
            getline(input, line);
            //cout << "theJBS: " << line << endl;
            vector<string> tmp = split(line);
            //cout << "theJBS: " << tmp[1] << endl;
            lastRound = stoi(tmp[1]);
            //cout << "lastRound: " << lastRound << endl;
            if (lastRound == g) {
                getline(input, line);
                tmp = split(line);
                curInfo.actions[0] = stoi(tmp[1]);
                curInfo.actions[1] = stoi(tmp[2]);
                getline(input, line);
                tmp = split(line);
                curInfo.speeches[0] = tmp[1];
                curInfo.speeches[1] = tmp[2];
                getline(input, line);
                tmp = split(line);
                curInfo.algState = tmp[1];
                input.close();

                return false;
            }
            else {
                if (lastRound == 99999)
                    return true;
                input.close();
                usleep(100000);
            }
        }
    }
}

void printRoundInfo(int g, RoundInfo curInfo) {
    cout << "Information from round " << g << endl;
    cout << "\tActions: " << curInfo.actions[0] << ", " << curInfo.actions[1] << endl;
    cout << "\tSpeech0: " << curInfo.speeches[0] << endl;
    cout << "\tSpeech1: " << curInfo.speeches[1] << endl;
    cout << "\tState: " << curInfo.algState << endl;
}

void writeSpeech(int g, string outputFilename) {
    string speech;

    if ((g % 2) == 0)
        speech = "You are a turd";
    else
        speech = "You are a nerd";

    ofstream output(outputFilename);

    output << "round:" << g << endl;
    output << "speech:" << speech << endl;

    output.close();
}

int main(int argc, char *argv[]) {

    string game = argv[1];
    string inputFilename = argv[2];
    string outputFilename = argv[3];

    cout << "theJBS: " << game << " " << inputFilename << " " << outputFilename << endl;

    bool quit = false;
    RoundInfo curInfo;
    int g = 0;
    while (!quit) {
        //cout << "theJBS: " << g << endl;
        quit = getRoundInformation(g, curInfo, inputFilename);
        printRoundInfo(g, curInfo);
        writeSpeech(g, outputFilename);
        g++;
    }

    cout << "ending theJBS" << endl;

    return 0;
}