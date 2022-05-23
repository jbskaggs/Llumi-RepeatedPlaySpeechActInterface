#include "defs.h"
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include "MarkovGame.h"
#include "gameWriter.h"
#include "GameEngine.h"
#include "MatrixEngine.h"
#include "MazeEngine.h"
#include "GridWarEngine.h"
#include "LEGOEngine.h"
#include "Player.h"
#include "Xprt.h"
#include "jefe++.h"
#include "Preventer.h"
#include "rndmPlyr.h"
#include "clientAgent.h"
#include "CFR.h"
#include "client_socket.h"
//#include "delayer.h"
#include "RL.h"
#include "personality.h"

using namespace std;

typedef std::map<std::string, std::string> JSON_Type;

void error(const char *msg) { perror(msg); exit(0); }

JSON_Type Jsontify(string str) {
  JSON_Type json;

  char* buf = strdup(str.c_str());
  char* token = strtok(buf, "{},:\"");
  while (token != NULL)
  {
    string key(token);
    token = strtok (NULL,"{},:\"");
    string value(token);
    json.insert(std::pair<string,string>(key,value));
    token = strtok (NULL,"{},:\"");
  }

  delete buf;
  /* Display keys and value
  for(std::map<string, string>::iterator it = json.begin(); it != json.end(); ++it)
  {
      cout << it->first << " " << it->second << "\n";
  }

  cout << json["playerID"];
  */

  return json;
}

/*
JSON_Type Jsontify_Msg(string str){
    cout <<"Whole String: " <<str <<endl;

    int pos = str.find_last_of(":")-9;
    string t_str = str.substr(pos);
    t_str.pop_back();
    t_str.pop_back();
    cout << "Recomendation from Partner: " <<t_str <<endl;
    JSON_Type json =

    return json;
}
*/

string processMsg(string str){
    cout <<"Whole String: " <<str <<endl;

    int pos = str.find_last_of(":");
    string t_str = str.substr(pos+2);
    t_str.erase (t_str.end()-4, t_str.end());
    cout << "Recomendation from Partner: " << t_str <<endl;

    return t_str;
}

string Jsontify_Matrix(string str) {
  char* buf = strdup(str.c_str());

  char* token = strtok(buf, "{}:\"");
  token = strtok (NULL,"{}:\"");

  string matrix(token);
  matrix.pop_back();

  delete buf;

  return matrix;
}

string httpCall(char message[1024]){
  int portno = 8888;
  char *host = (char *) "127.0.0.1"; // This will eventually need to change

  struct hostent *server;
  struct sockaddr_in serv_addr;
  int sockfd, bytes, sent, received, total;
  char response[4096];
  string responseStr;
  /* create the socket */
  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd < 0) error("ERROR opening socket");

  /* lookup the ip address */
  server = gethostbyname(host);
  if (server == NULL) error("ERROR, no such host");

  /* fill in the structure */
  memset(&serv_addr,0,sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(portno);
  memcpy(&serv_addr.sin_addr.s_addr,server->h_addr,server->h_length);

  /* connect the socket */
  if (connect(sockfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr)) < 0)
      error("ERROR connecting");

  /* send the request */

  total = strlen(message);
  sent = 0;
  do {
      bytes = write(sockfd,message+sent,total-sent);
      if (bytes < 0)
          error("ERROR writing message to socket");
      if (bytes == 0)
          break;
      sent+=bytes;
  } while (sent < total);

  /* receive the response */
  memset(response,0,sizeof(response));
  total = sizeof(response)-1;
  received = 0;
  do {
      bytes = read(sockfd,response+received,total-received);
      if (bytes < 0)
          error("ERROR reading response from socket");
      if (bytes == 0)
          break;
      received+=bytes;
  } while (received < total);

  if (received == total)
      error("ERROR storing complete response from socket");

  /* close the socket */

  close(sockfd);

  /* process response */

  responseStr = response;

  return responseStr;
}

string stripHeader(string withHeader){
  string withoutHeader;
  int pos = withHeader.find("{");
  withoutHeader = withHeader.substr(pos,-1);

  return withoutHeader;
}

string login(string userIDStr){
  string json = "{\"playerID\":\"" + userIDStr + "\",\"password\":\"password\"}";

  char *message_fmt = (char *) "POST /login HTTP/1.0\r\n\
Content-Type: application/json\nContent-Length: %d\n\n%s";

  char message[1024];
  sprintf(message,message_fmt,json.length(),json.c_str());

  string response;
  response = httpCall(message);

  return response;
}

string getUserID(char *argv[]){
  char *message_fmt = (char *) "POST /api/AIlogin/%s HTTP/1.0\r\n\r\n";
  string response;
  char message[1024];

  sprintf(message,message_fmt,argv[1]);
  response = httpCall(message);

  return response;
}

string getInGameStatus(string userIDStr){
  string json = "{\"playerID\":\"" + userIDStr + "\"}";
  char *message_fmt = (char *) "GET /checkStatus HTTP/1.0\r\n\
Content-Type: application/json\nContent-Length: %d\n\n%s";
  //"GET /api/AIinGameStatus/%s HTTP/1.0\r\n\r\n";

  char message[1024];
  sprintf(message,message_fmt,json.length(),json.c_str());//,userIDArg);

  string response;
  response = httpCall(message);

  return response;
}

string getPlayerShouldPlayStatus(string userIDStr){
    string json = "{\"playerID\":\"" + userIDStr + "\"}";
    char *message_fmt = (char *) "GET /getPlayerShouldPlayStatus HTTP/1.0\r\n\
  Content-Type: application/json\nContent-Length: %d\n\n%s";
    //"GET /api/AIinGameStatus/%s HTTP/1.0\r\n\r\n";

    char message[1024];
    sprintf(message,message_fmt,json.length(),json.c_str());//,userIDArg);

    string response;
    response = httpCall(message);

    return response;
}

string getWhich(string userIDStr){
    return "1";
}

string getMatrix(string UserIDStr){
    cout <<UserIDStr <<endl;
    string json = "{\"playerID\":\"" + UserIDStr + "\"}";;
    char *message_fmt = (char *) "GET /getMatrix HTTP/1.0\r\n\
Content-Type: application/json\nContent-Length: %d\n\n%s";
    //"GET /api/AImatrix/%s HTTP/1.0\r\n\r\n";

    //char *gameIDArg = (char *) gameIDStr.c_str();
    char message[1024];
    sprintf(message,message_fmt,json.length(),json.c_str());//,gameIDArg);

    string response;
    response = httpCall(message);

    return response;
}

string getCheapTalkStatus(string userIDStr){
  string json = "{\"playerID\":\"" + userIDStr + "\"}";
  char *message_fmt = (char *) "GET /checkMessageStatus HTTP/1.0\r\n\
Content-Type: application/json\nContent-Length: %d\n\n%s";
  //"GET /api/AIinGameStatus/%s HTTP/1.0\r\n\r\n";

  char message[1024];
  sprintf(message,message_fmt,json.length(),json.c_str());//,userIDArg);

  string response;
  response = httpCall(message);

  return response;
}

string sendCheapTalk(string userIDStr, string cheapTalkMessageStr){
  string json = "{\"playerID\":\"" + userIDStr + "\",\"msg\":\"" +cheapTalkMessageStr+ "\"}";
  char *message_fmt = (char *) "POST /sendChatMsg HTTP/1.0\r\n\
Content-Type: application/json\nContent-Length: %d\n\n%s";

  char message[1024];
  sprintf(message,message_fmt,json.length(),json.c_str());//,gameIDArg, userIDArg, cheapTalkMessageArg);

  string response;
  response = httpCall(message);

  return response;
}

string receiveCheapTalk(string userIDStr){
  string json = "{\"playerID\":\"" + userIDStr + "\"}";
  char *message_fmt = (char *) "GET /retrieveMsgs HTTP/1.0\r\n\
Content-Type: application/json\nContent-Length: %d\n\n%s";

  char message[1024];
  sprintf(message,message_fmt,json.length(),json.c_str());

  string response;
  response = httpCall(message);

    //cout << "response: " << response << endl;

  return response;
}

string submitRoundOption(string userIDStr, string optionStr){
    string json = "{\"playerID\":\"" + userIDStr + "\",\"choice\":\"" + optionStr + "\"}";
    char *message_fmt = (char *) "POST /submitChoice HTTP/1.0\r\n\
Content-Type: application/json\nContent-Length: %d\n\n%s";

    char message[1024];
    sprintf(message,message_fmt,json.length(),json.c_str());//,gameIDArg,playerNumArg,optionArg);

    string response;
    //cout <<message <<endl;
    response = httpCall(message);
    //cout <<response <<endl;
    return response;
}

string getRoundEarnings(string userIDStr){
  string json = "{\"playerID\":\"" + userIDStr + "\"}";
  char *message_fmt = (char *) "GET /getRoundResults HTTP/1.0\r\n\
  Content-Type: application/json\nContent-Length: %d\n\n%s";
  //"GET /api/AIroundEarnings/%s/%s HTTP/1.0\r\n\r\n";

  char message[1024];
  sprintf(message,message_fmt,json.length(),json.c_str());

  string response;
  response = httpCall(message);

  return response;
}

vector< vector<int> > parsestring(string toParse){
    vector< vector<int> > rows;
    vector<int> row;

    for (int i = 0; i < toParse.size(); i++){

        if (toParse[i] == ']'){
            if (row.size() != 0){
                rows.push_back(row);
            }
            row.clear();
        }
        else if (isdigit(toParse[i])){
            if (isdigit(toParse[i+1])){
              cout << "NUM: " << i << " " << (toParse[i] - '0')*10 + (toParse[i+1]-'0') << endl;

              row.push_back((toParse[i] - '0')*10 + (toParse[i+1]-'0'));
              i++;
            }
            else{
                row.push_back(toParse[i]-'0');
            }
        }

    }
    return rows;
}

string createMatrixString(string matrix, string type)
{
    return "{\"type\": \"" + type + "\", \"payoffs\": " + matrix + "}";
}

/*****************************************/


void runGame(string userIDStr, string playerString, int me, bool cheapTalk);
Player *createPlayer(int me, char partido[1024], char name[1024]);

int getWordCount(char buf[1024]);
double ***readPayoffMatrixFromFile(int _A[2], char *game);
void cleanPayoffMatrix(double ***_M, int _A[2]);
void printGame(int _A[2], double ***_M);

GameEngine *ge;

bool cheapTalk = true;
bool delaysOn = true;
bool theJBS;
bool theJBS1;

char stringToCommunicate[1024];
Personality *personality;

/* ****************************************************
//
//	old: ./Solver <game> <iters> <playertype> <me> <hostIP> <addport>
//  new: ./Solver [userIDStr] [playertype] [me] [host] [cheaptalk]
//
// **************************************************** */
int main(int argc, char** argv) {
    if (argc < 5) {
        printf("Not enough parameters. Exiting\n");
        exit(1);
    }

	srand(time(NULL));

    runGame(argv[1], argv[2], atoi(argv[3]), (strcmp(argv[4],"true") == 0));
	//double ***M;
	//int A[2];
    //M = readPayoffMatrixFromFile(A, argv[1]);
    //int iters = atoi(argv[2]);
    //printf("iters = %i\n", iters);
    //int me_o = atoi(argv[2]);
    //printf("I am %i\n", me_o);
    
    //int addport = atoi(argv[6]);

    //int numSamps = 1;
    //for (int i = 0; i < numSamps; i++) {
    //    runGame(M, A, iters, argv[1], argv[3], me_o, argv[5], addport+i*3);
    //   
    //    if (me_o == 0)
    //        usleep(1000000);
    //    else
    //        usleep(1500000);
    //}
          
	return 0;
}

double ***detectGameMatrix(string roundMatrixStr, char partido[1024], int A[2]) {
    if (roundMatrixStr == "[[[60,60],[0,100]],[[100,0],[20,20]]]") {
        strcpy(partido, "prisoners");
        A[0] = A[1] = 2;
    }
    else if (roundMatrixStr == "[[[0,0],[100,33]],[[33,100],[84,84]]]") {
        strcpy(partido, "chicken2");
        A[0] = A[1] = 2;
    }
    else if (roundMatrixStr == "[[[0,0],[70,35],[40,100]],[[35,70],[10,10],[30,45]],[[100,40],[45,30],[40,40]]]") {
        strcpy(partido, "blocks2");
        A[0] = A[1] = 3;
    }
    else if (roundMatrixStr == "[[[33,67],[67,100]],[[0,33],[100,0]]]") {
            strcpy(partido, "endless");
        A[0] = A[1] = 2;
    }
    else if (roundMatrixStr == "[[[55,55],[40,100]],[[100,40],[0,0]]]") {
            strcpy(partido, "chickenalt");
        A[0] = A[1] = 2;
    }
    else {
        cout << roundMatrixStr << " is not a game I know.  Exiting." << endl;
        exit(1);
    }

    roundMatrixStr.erase(remove(roundMatrixStr.begin(), roundMatrixStr.end(), '['), roundMatrixStr.end());
    roundMatrixStr.erase(remove(roundMatrixStr.begin(), roundMatrixStr.end(), ']'), roundMatrixStr.end());
    vector<string> tokens;
    string token;
    istringstream tokenStream(roundMatrixStr);
    while (getline(tokenStream, token, ','))
        tokens.push_back(token);  

	int i, j, k;
	double ***M = new double**[2];
	for (i = 0; i < 2; i++) {
		M[i] = new double*[A[0]];
		for (j = 0; j < A[0]; j++) {
			M[i][j] = new double[A[1]];
        }
	}

    int count = 0;
    for (i = 0; i < A[0]; i++) {
        for (j = 0; j < A[1]; j++) {
            M[0][i][j] = stod(tokens[count]) / 100.0;
            M[1][i][j] = stod(tokens[count+1]) / 100.0;
            count += 2;
        }
    }

    printf("%s\n", partido);
    printGame(A, M);

    return M;
}

void runGame(string userIDStr, string playerString, int me, bool cheapTalk) {
    cout << "playerString: " << playerString << endl;

    string stateInfo_txt = string("src/theJBS/") + userIDStr + string("_stateInfo.txt");
    string stateInfo_tmp = string("src/theJBS/") + userIDStr + string("_stateInfo.tmp");
    string speech_txt = string("src/theJBS/") + userIDStr + string("_speech.txt");

    int microElapsed = 9;

    // Significant Data for GamePlay
    string userIDCheck = "Name already in use!";
    string inGameStatus = "false";
    string cheapTalkStatus = "";
    string gameIDStr = "";
    string whichStr = "";
    string roundNumStr = "";
    string roundMatrixStr = "";
    string roundOptionStr = "";
    string submittedStatusStr = "";
    string readyStatusStr = "";
    string gameIsDoneStr = "";
    string response = "";
    int roundOptionInt = 0;
    bool done = false;
    bool waiting = true;
    vector<string> myEarnings;
    vector<string> myChoices;
    vector<string> theirEarnings;
    vector<string> theirChoices;

    cout << "Waiting to be added to game ..." << endl;

    // Check inGameStatus
    while(inGameStatus != "playing a game") {
        inGameStatus = stripHeader(getInGameStatus(userIDStr));
        inGameStatus = Jsontify(inGameStatus)["status"];
        //sleep(3);
    }

    // Set gameID
    //gameIDStr = inGameStatus;   // Conversion from char* to string

    // Set Which Player (0 || 1)
    //whichStr = getWhich(userIDStr);

    cout << "Successfully added to game " << gameIDStr << " as player " << me << "." << endl << endl;


    /*****************************************************************************/
    /*** Run algorithm ***/
	int i;
	int act;//, *acts = new int[2];
	char buf[10000], bufcopy[10000];
    double payoffs[2];

    char dummy[1024];

    strcpy(stringToCommunicate, "");
    Player *player;
/*
	Player *player = createPlayer(me_o, partido, name);
    ClientSocket *cs = new ClientSocket(host, 3000+me_o+addport);
    
    if (cheapTalk)
        cs->SendMessage(name, strlen(name));
    else {
        char nombre2[1024];
        sprintf(nombre2, "%s-", name);
        cs->SendMessage(nombre2, strlen(nombre2));
    }
    
    if (player == NULL) {
        if (player != NULL)
            delete player;
        return;
    }
    
    cs->ReadMessage(buf);
*/    
    //gettimeofday(&eltiempo, NULL);
    //sTime = eltiempo.tv_sec + (eltiempo.tv_usec / 1000000.0);
    string rec = "";

    int g = 0;
    double score[2] = {0.0, 0.0};

    double ttalk = (double)(rand() % 10);
    double tact[2];
    int amount, wordCount;
    tact[0] = tact[1] = (double)(rand() % 10000);
    int A[2];
    double ***M;
    string speech1 = " "; //todo:
    string speech2 = " ";
    
	while (inGameStatus == "playing a game") {
        printf("\nRound: %i\n", g);
        cout << "------------------------------------------" << endl;
        
        roundMatrixStr = Jsontify_Matrix(stripHeader(getMatrix(userIDStr)));
        if (g == 0) {
            char partido[1024];
            cout << "detectGameMatrix" << endl;
            M = detectGameMatrix(roundMatrixStr, partido, A);
            cout << "createPlayer" << endl;
            player = createPlayer(me, partido, (char *)playerString.c_str());

            if (theJBS) {
                // initialize the files
                ofstream output(stateInfo_txt);
                output << "round:-1" << endl;
                output.close();

                ofstream output2(speech_txt);
                output2 << "round:-1" << endl;
                output.close();

                string path_to_python_project = "/home/jskaggs93/PycharmProjects/Learing_to_Comunicate/";
                string path_to_python = "/home/jskaggs93/anaconda3/envs/neuralnet/bin/python";
                string path_to_java_project = "/home/jskaggs93/IdeaProjects/RepeatedPlaySpeechActInterface/";

                string agent = "2";
                if (theJBS1){
                    agent = "1";
                }

                string cmd = "PYTHONPATH=" + path_to_python_project + "src/modeling/live_model/:" + path_to_python_project + " ";
                cmd += path_to_python + " live_model.py " + path_to_java_project + stateInfo_txt + " " + path_to_java_project + speech_txt + " " + partido + " " + agent;
                // PYTHONPATH=/Users/jonathanskaggs/PycharmProjects/Learing_to_Comunicate/src:/Users/jonathanskaggs/PycharmProjects/Learing_to_Comunicate /Users/jonathanskaggs/opt/anaconda3/envs/machine_learning/bin/python src/live_model.py ssharpoutput.txt JBSagentoutput.txt prisoners

                string cd = "cd " + path_to_python_project + "src/modeling/live_model/";

                cmd = cd + " && " + cmd + " > " + path_to_java_project + "src/theJBS/python_output.txt &";

                ofstream output3("src/theJBS/ssharp_output.txt");
                output3 << "starting python program: " << cmd << endl;

                // spawn Jonathan's program
                int cmd_length = cmd.length();
                char char_cmd[cmd_length + 1];
                strcpy(char_cmd, cmd.c_str());
                system(char_cmd);

                output3 << "started python program!" << endl;
                output.close();

            }
        }
        //player->select();
    
        //printf("a"); fflush(stdout);
        //printf("stringToCommunicate: %s\n", stringToCommunicate);
        ge->initRound();
        player->Reset();
        
        //player->logReplay_Round();
        
        if (cheapTalk) {
            // delay cheap talk
            //printf("-------------- stringToCommunicate length: %i\n", (int)strlen(stringToCommunicate));
            //if (ttalk < ((int)strlen(stringToCommunicate)))
            //    ttalk = (int)strlen(stringToCommunicate);

            //printf("1"); fflush(stdout);
            
            //if (ttalk > 20.0)
            //    ttalk = 20.0;
            
            //amount = 0;//(int)(ttalk * 1000000);
            //printf("stringToCommunicate: %s\n", stringToCommunicate);

            if (theJBS) {
                // write the file indicating what has happened recently
                ofstream output(stateInfo_tmp);
                output << "round:" << g << endl;
                if (g == 0) {
                    output << "actions:-1:-1" << endl;
                }
                else {
                    output << "actions:" << ge->actions[0] << ":" << ge->actions[1] << endl;
                }
                output << "speeches:" << speech1 << ":" << speech2 << endl;   
                output << "state:" << player->getTheStateString() << endl;
                output.close();

                char mandato[1024];
                strcpy(mandato, ("mv " + stateInfo_tmp + " " + stateInfo_txt).c_str());
                system(mandato);
            }
            personality->createCheapTalk(stringToCommunicate, A);
            printf("numMessagesLast = %i\n", personality->numMessagesLast);
//            fflush(stdout);
//            if (personality->numMessagesLast < 1) {
//                //printf("!!!!!delay by the none_rate: %.2lf\n", player->none_rate); fflush(stdout);
//                amount = player->none_rate * 1000;
//            }
//            else {
//                //printf("delay by the some_rate: %.2lf\n", player->some_rate); fflush(stdout);
//                amount = personality->numMessagesLast * player->some_rate * 1000;
//            }
//            if (amount > 20000000)
//                amount = 20000000;

//                if (delaysOn)
//                    usleep(amount);

            if (strlen(stringToCommunicate) < 2) {
                strcpy(stringToCommunicate, " ");
            }
            //cs->SendMessage(personality->personalityString, strlen(personality->personalityString));
            string cheapTalkMessage(personality->personalityString);
            if (theJBS) {
                // read the file and update the string cheapTalkMessage
                while (true) {
                    cout << "checking the jbs!" << endl;
                    ifstream input(speech_txt);
                    string line;
                    getline(input, line);
                    int round = stoi(line.substr(6));
                    getline(input, line);
                    input.close();
                    if (round == g) {
                        cheapTalkMessage = line.substr(7);
                        break;
                    }
                    usleep(1000000);
                }
            }
            if (me == 0)
                speech1 = cheapTalkMessage;
            else
                speech2 = cheapTalkMessage;
            cout << "CTM: " << cheapTalkMessage << endl;
            int str_len = cheapTalkMessage.length();
            int wait_time = 100000 * str_len;
            if (wait_time > 20000000){
                wait_time = 20000000;
            }
            usleep(wait_time);

            sendCheapTalk(userIDStr, cheapTalkMessage);

            //cs->ReadMessage(buf);
            cout <<"Waiting for cheap talk..." <<endl;
            cheapTalkStatus = "";
            while (cheapTalkStatus != "Recommendation is ready") {
                //cout <<cheapTalkStatus <<endl;
                cheapTalkStatus = stripHeader(getCheapTalkStatus(userIDStr));
                cheapTalkStatus = Jsontify(cheapTalkStatus)["status"];
                sleep(1);
            }

            rec = processMsg(stripHeader(receiveCheapTalk(userIDStr)));

            if (me == 1)
                speech1 = rec;
            else
                speech2 = rec;

            wordCount = getWordCount((char *) rec.c_str());
            //printf("Received a %i-word chat\n", wordCount);
            //cout << endl;
            strcpy(bufcopy, (char *) rec.c_str());
            player->logReplay_CheapTalk(personality->personalityString, bufcopy);
            
            //printf("read msg: %s\n", buf);
            //printf("3"); fflush(stdout);
            
            ttalk = player->processCheapTalk((char *) rec.c_str());
            
            printf("received cheap talk: %s\n", rec.c_str());
            //printf("ttalk = %lf\n", ttalk);
            //printf("none_rate = %.2lf\nsome_rate = %.2lf\n", player->none_rate, player->some_rate);
            
            //printf("4"); fflush(stdout);

        }
        
        //printf("b"); fflush(stdout);

        strcpy(stringToCommunicate, "");
 /*   
        if (tact[1-me] > 20000.0)
            amount = 20000000;
        else
            amount = (int)(tact[1-me] * 1000);
        
        //printf("initial delay: %i\n", amount);
        
        amount += (wordCount / 220.0) * 60000000;
        
        //printf("totalDelay = %i (%i - %i)\n", amount, (int)((wordCount / 220.0) * 60000000), wordCount);
        
        if (delaysOn)
            usleep(amount);
*/    
		act = player->Move(ge);
        //sprintf(buf, "%i$ 0.000000", act);
        roundOptionStr = to_string(act);
        myChoices.push_back(roundOptionStr);

        //printf("act = %i\n", act); fflush(stdout);

        printf("delaying (%i) ... \n", microElapsed * 250000);
        
        if (delaysOn)
            usleep(microElapsed * 250000);

        cout << "Round Option: " << roundOptionStr << endl;

        // Send Decision
        submitRoundOption(userIDStr, roundOptionStr);
        
        printf("Submitted round option\n");

        // Reset Waiting Bool
        waiting = true;

        // Wait for other player to play
        
        //struct timeval startTime, endTime;
        //gettimeofday(&startTime, NULL);

        //printf("waiting\n");

        microElapsed = 0;
        while (waiting) {
            if ((microElapsed % 4) == 0)
                printf("waiting (%i)\n", microElapsed);
            inGameStatus = stripHeader(getInGameStatus(userIDStr));
            inGameStatus = Jsontify(inGameStatus)["status"];
            submittedStatusStr = stripHeader(getPlayerShouldPlayStatus(userIDStr));
            submittedStatusStr = Jsontify(submittedStatusStr)["playerShouldPlay"];
            if (submittedStatusStr == "true"){
                //cout << "Ready to continue. " << endl;
                waiting = false;
            }
            else if (inGameStatus != "playing a game"){
                waiting = false;
            }
            //cout <<inGameStatus <<endl;
            usleep(250000);
            microElapsed ++;
        }
        
        if (microElapsed < 3) {
            microElapsed = rand() % 5 + 3;
        }
        else if (microElapsed > 40) {
            microElapsed = 40;
        }
        
        printf("TimeElapsed: %lf seconds\n", microElapsed / 4.0);

        /********************************************************************/
        JSON_Type roundResult = Jsontify(stripHeader(getRoundEarnings(userIDStr)));
        /********************************************************************/


        string my_p = "p"+ std::to_string(me+1);
        string opp_p = "";

        if (my_p == "p1"){
            opp_p = "p2";
        }
        else{
            opp_p = "p1";
        }

        myEarnings.push_back(roundResult[my_p+"Payoff"]);
        theirEarnings.push_back(roundResult[opp_p+"Payoff"]);
        theirChoices.push_back(roundResult[opp_p+"Choice"]);


        ge->actions[me] = act;
        ge->actions[1-me] = stoi(theirChoices[theirChoices.size()-1]);

        // Have an error check here that sees if there is a nulled value
        if( myEarnings[myEarnings.size()-1] == "undefined"){
          cout << "yes 2" << endl;
        }


        cout << "My: " << myEarnings[myEarnings.size()-1] << endl;
        cout << "Their: " << theirEarnings[theirEarnings.size()-1] << endl;

        //dineros[me] = stod(myEarnings[myEarnings.size()-1]);
        //dineros[1-me] = stod(theirEarnings[theirEarnings.size()-1]);

        score[0] += stod(myEarnings[myEarnings.size()-1]);
        score[1] += stod(theirEarnings[theirEarnings.size()-1]);


//        cs->SendMessage(buf, strlen(buf));

        //printf("c"); fflush(stdout);
        
        // read the result
//        cs->ReadMessage(buf);
        //printf("message: %s\n", buf);
        
        //strcat(buf, " 0.000000 0.000000");
        
//        sscanf(buf, "%i %i %s %lf %lf", &(ge->actions[0]), &(ge->actions[1]), dummy, &(tact[0]), &(tact[1]));
        //printf("%i: actions: %i %i\n", i, acts[0], acts[1]); fflush(stdout);
        //printf("he took %lf miliseconds\n", tact[1-me_o]);
        //printf("d"); fflush(stdout);

        
        if (ge->actions[me] != act) {
            printf("Server isn't properly listenting to me.  I QUIT!!!\n");
            exit(1);
        }
        
        ge->MoveAgents();
        
        player->logReplay_Moves(ge->actions);
        //printf("acting done\n");
        //printf("t"); fflush(stdout);
        
        //printf("acts: %i %i\n", ge->actions[0], ge->actions[1]);
        payoffs[0] = M[0][ge->actions[0]][ge->actions[1]];
        payoffs[1] = M[1][ge->actions[0]][ge->actions[1]];
		player->moveUpdate(ge, ge->actions, payoffs);

        //printf("updated\n");
        //printf("e"); fflush(stdout);
        
        player->roundUpdate();

        cout << "stringToCommunicate after round update: " << stringToCommunicate << endl;
        
        //printf("f\n"); fflush(stdout);
        g++;
	}

    if (theJBS) {
        // tell Jonathan's program the game is over
        ofstream output(stateInfo_txt);
        output << "round:99999" << endl;                
        output.close();
    }

    
    printf("finished\n");
    
    //cs->ReadMessage(buf);
    //printf("My percentile is %s\n", buf);

    //if (me == 0)
    //    usleep(500000);
    //else
    //    usleep(250000);

    //delete cs;
	delete player;

	cleanPayoffMatrix(M, A);
	delete personality;
}

Player *createPlayer(int me, char partido[1024], char name[1024]) {
    char buf[1024], fnombre[1024];

    printf("partido: %s\n", partido); fflush(stdout);

    strcpy(buf, partido);
    if (!strcmp(buf, "prisoners"))
        strcpy(buf, "pd");
    
    if ((!strcmp(buf, "pd")) || (!strcmp(buf, "chicken")) || (!strcmp(buf, "chicken2")) || (!strcmp(buf, "blocks")) ||
    (!strcmp(buf, "blocks2")) || (!strcmp(buf, "shapleys")) || (!strcmp(buf, "tricky")) ||  (!strcmp(buf, "endless")) ||
      (!strcmp(buf, "chickenalt"))) {
        ge = new MatrixEngine();
    }
    else {
        printf("game not found\n");
        exit(1);
    }
    
    //sprintf(fnombre, "/Users/crandall/Projects/jbsagent/RepeatedPlaySpeechActInterface/src/Algs_new/MG_%s.txt", buf);
    sprintf(fnombre, "src/Algs_new/MG_%s.txt", buf);
    
    Player *pl = NULL;
    
    printf("nombre: %s\n", name); fflush(stdout);
    cout << endl;

    if (!strcmp(name, "maxmin")) {
        printf("player: maxmin\n");
        pl = new Xprt(me, new MarkovGame(fnombre), buf);
    }
    else if (!strcmp(name, "mbrl")) {
        printf("player: mbrl\n");
        pl = new Xprt(me, new MarkovGame(fnombre), buf);
    }
    else if (!strcmp(name, "umbrl")) {
        printf("player: umbrl\n");
        strcpy(buf, "umbrl");
        pl = new Xprt(me, new MarkovGame(fnombre), buf, true);
    }
    else if (!strcmp(name, "folk")) {
        printf("player: folk\n");
        strcpy(buf, "folk");
        pl = new Xprt(me, new MarkovGame(fnombre), buf);
    }
    else if (!strcmp(name, "bully")) {
        printf("player: bully\n");
        strcpy(buf, "bully");
        pl = new Xprt(me, new MarkovGame(fnombre), buf);
    }
    else if (!strncmp(name, "Ssharp", 6)) {
        printf("player: %s\n", name);
        cout << endl;

        if (!strcmp(name+6,"JBS1")) {
            personality = new Personality("Silent");
            theJBS = true;
            theJBS1 = true;
        }
        else if (!strcmp(name+6,"JBS2")) {
            personality = new Personality("Silent");
            theJBS = true;
            theJBS1 = false;
        }
        else {
            personality = new Personality(name+6);
            theJBS = false;
        }
        cout << "created personality: " << (name+6) << endl;
        bool XAI = true;
        //printf("XAI postfix: [%s]\n", name + strlen(name) - 4);
        //if (!strcmp("NXAI", name + strlen(name) - 4)) {
        //    printf("No XAI\n");
        //    XAI = false;
        //}
        cout << "jefe plus plus!" << endl;
        cout << fnombre << endl;
        pl = new jefe_plus("S++", me, new MarkovGame(fnombre), 0.99, me, true, XAI);
        cout << "player created" << endl;
    }
    else if (!strncmp(name, "EEE#", 4)) {
        printf("player: %s\n", name);
        personality = new Personality(name+5);
        bool XAI = true;
        printf("XAI postfix: [%s]\n", name + strlen(name) - 4);
        if (!strcmp("NXAI", name + strlen(name) - 4)) {
            printf("No XAI\n");
            XAI = false;
        }
        pl = new jefe_plus("eee", me, new MarkovGame(fnombre), 0.99, me, false, XAI);
    }
    else if (!strcmp(name, "bouncer")) {
        printf("player: Preventer\n");
        pl = new Preventer(me, new MarkovGame(fnombre), 0.0);  // 0.2
    }
    else if (!strcmp(name, "cfr")) {
        printf("player: CFV\n");
        pl = new CFR(me, new MarkovGame(fnombre), 40, partido, false);
    }
    else if (!strcmp(name, "cfru")) {
        printf("player: CFV\n");
        pl = new CFR(me, new MarkovGame(fnombre), 40, partido, true);
    }
    else if (!strcmp(name, "friend")) {
        printf("player: Friend-VI\n");
        pl = new RL(me, new MarkovGame(fnombre), FRIENDVI);
    }
    else {
        printf("player type %s not found\n", name);
        exit(1);
    }
    
    return pl;
}

int getWordCount(char buf[1024]) {
    int i;
    int numWords = 0;
    
    for (i = 0; i < (int)strlen(buf); i++) {
        if (buf[i] == '$')
            break;
        
        if (buf[i] == ' ')
            numWords ++;
    }
    
    if (i > 6)
        numWords ++;

    return numWords;
}

double ***readPayoffMatrixFromFile(int _A[2], char *game) {
	double ***_M;
    
	char filename[1024];
	sprintf(filename, "..//..//games//%s.txt", game);
	
	FILE *fp = fopen(filename, "r");
	if (fp == NULL) {
		// check in an alternate directory before giving up
		sprintf(filename, "..//games//%s.txt", game);
		fp = fopen(filename, "r");
		if (fp == NULL) {
			printf("file %s not found\n", filename);
			exit(1);
		}
	}

	fscanf(fp, "%i", &(_A[0]));
	fscanf(fp, "%i", &(_A[0]));
	fscanf(fp, "%i", &(_A[1]));
	
	int i, j;
	_M = new double**[2];
	for (i = 0; i < 2; i++) {
		_M[i] = new double*[_A[0]];
		for (j = 0; j < _A[0]; j++)
			_M[i][j] = new double[_A[1]];
	}

	for (i = 0; i < _A[1]; i++) {
		for (j = 0; j < _A[0]; j++) {
			fscanf(fp, "%lf %lf", &(_M[0][j][i]), &(_M[1][j][i]));
		}
	}

    printGame(_A, _M);
    
	return _M;
}

void cleanPayoffMatrix(double ***_M, int _A[2]) {
	int i, j;
	
	for (i = 0; i < 2; i++) {
		for (j = 0; j < _A[0]; j++)
			delete _M[i][j];
		delete _M[i];
	}
	delete _M;
}

void printGame(int _A[2], double ***_M) {
    int i, j;
    
    printf("\n   |      ");
    
    for (i = 0; i < _A[1]; i++)
        printf("%i      |      ", i);
    printf("\n");
    for (i = 0; i < _A[0]; i++) {
        printf("--------------------------------------\n %i | ", i);
        for (j = 0; j < _A[1]; j++) {
            printf("%.2lf , %.2lf | ", _M[0][i][j], _M[1][i][j]);
        }
        printf("\n");
    }
    printf("--------------------------------------\n\n");
}



