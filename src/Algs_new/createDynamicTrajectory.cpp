#include "defs.h"

int numExperts = 11;

void loadMessages(char mensajes[19][1024]) {
    strcpy(mensajes[0], "Do as I say, or I will punish you. ");
    strcpy(mensajes[1], "I accept your last proposal. ");
    strcpy(mensajes[2], "I don't accept your proposal. ");
    strcpy(mensajes[3], "That's not fair. ");
    strcpy(mensajes[4], "I don't trust you. ");
    strcpy(mensajes[5], "Excellent. ");
    strcpy(mensajes[6], "Sweet. We are getting rich. ");
    strcpy(mensajes[7], "Give me another chance. ");
    strcpy(mensajes[8], "I forgive you. ");
    strcpy(mensajes[9], "I'm changing my strategy. ");
    strcpy(mensajes[10], "We can both do better than this. ");
    strcpy(mensajes[11], "Curse you. ");
    strcpy(mensajes[12], "You betrayed me. ");
    strcpy(mensajes[13], "You will pay for this. ");
    strcpy(mensajes[14], "In your face! ");
    strcpy(mensajes[15], "Let's always play ax. ");
    strcpy(mensajes[16], "This round, let's play ax. ");
    strcpy(mensajes[17], "Don't play a. ");
    strcpy(mensajes[18], "Let's alternate between ax and by. ");
}

void readMessages(char mensajes[19][1024], char sentMensajes[60][2][2048], const char nombre[1024], const char playerstr[1024], int rounds) {
    char fnombre[1024];
    
    sprintf(fnombre, "../ArchivedResults/%s_%s_messages_0.txt", nombre, playerstr);
    printf("%s\n", fnombre); fflush(stdout);
    FILE *fp = fopen(fnombre, "r");

    char buf[1024], tmp[1024];
    int i = 0;
    for (int i = 0; i < rounds; i++) {
        fgets(buf, 1024, fp);
        printf("%s", buf); fflush(stdout);
        if (buf[0] == '$')
            strcpy(sentMensajes[i][0], "<none>");
        else {
            char *str = strtok(buf, "$");
            char *mess = strtok(str, ";");
            strcpy(sentMensajes[i][0], "");
            while (mess != NULL) {
                int code = atoi(mess);
                strcpy(tmp, mensajes[code]);
                if (code > 14) {
                    switch (code) {
                        case 15:
                            tmp[18] = mess[3];
                            tmp[19] = mess[4];
                            break;
                        case 16:
                            tmp[23] = mess[3];
                            tmp[24] = mess[4];
                            break;
                        case 17:
                            tmp[11] = mess[3];
                            break;
                        case 18:
                            tmp[24] = mess[3];
                            tmp[25] = mess[4];
                            tmp[31] = mess[6];
                            tmp[32] = mess[7];
                            break;
                    }
                }
                
                strcat(sentMensajes[i][0], tmp);
            
                mess = strtok(NULL, ";");
            }
        }
        printf("%s\n", sentMensajes[i][0]);

        fgets(buf, 1024, fp);
        strcpy(sentMensajes[i][1], "<none>");
        printf("%s", buf); fflush(stdout);
        if (buf[0] == '$')
            strcpy(sentMensajes[i][1], "<none>");
        else {
            char *str = strtok(buf, "$");
            char *mess = strtok(str, ";");
            strcpy(sentMensajes[i][1], "");
            while (mess != NULL) {
                int code = atoi(mess);
                strcpy(tmp, mensajes[code]);
                if (code > 14) {
                    switch (code) {
                        case 15:
                            tmp[18] = mess[3];
                            tmp[19] = mess[4];
                            break;
                        case 16:
                            tmp[23] = mess[3];
                            tmp[24] = mess[4];
                            break;
                        case 17:
                            tmp[11] = mess[3];
                            break;
                        case 18:
                            tmp[24] = mess[3];
                            tmp[25] = mess[4];
                            tmp[31] = mess[6];
                            tmp[32] = mess[7];
                            break;
                    }
                }
                
                strcat(sentMensajes[i][1], tmp);
            
                mess = strtok(NULL, ";");
            }
        }
        printf("%s\n", sentMensajes[i][0]);
    }


    fclose(fp);
}

int getActions(int actions[60][2], const char nombre[1024], const char playerstr[1024], int rounds) {
    char fnombre[1024];
    
    sprintf(fnombre, "../ArchivedResults/%s_%s_activity_0.txt", nombre, playerstr);
    FILE *fp = fopen(fnombre, "r");
    
    double val1, val2;
    for (int i = 0; i < rounds; i++) {
        fscanf(fp, "%i %i %lf %lf", &(actions[i][0]), &(actions[i][1]), &val1, &val2);
        printf("%i %i\n", actions[i][0], actions[i][1]);
    }
    
    fclose(fp);
    
    return rounds;
}

void writeHeader(FILE *fp, const char nombre[1024], int me) {

    if (!strncmp("prisoners", nombre, 9)) {
        fprintf(fp, "2\n2\n");
        fprintf(fp, "0.600000\t0.600000\n");
        fprintf(fp, "0.000000\t1.000000\n");
        fprintf(fp, "1.000000\t0.000000\n");
        fprintf(fp, "0.200000\t0.200000\n");
        fprintf(fp, "0.200000 0.200000\n");
        fprintf(fp, "0.000000 1.000000\n");
        fprintf(fp, "0.000000 1.000000\n");
        fprintf(fp, "0.000000 1.000000\n");
        fprintf(fp, "0.000000 1.000000\n");
        fprintf(fp, "numExperts = 11\n");
        fprintf(fp, "{ Type = MAXIMIN ; TurnTaking = No ; Payoffs = ( 0.00 , 0.00 ) ; Target = None }\n");
        fprintf(fp, "{ Type = BR ; TurnTaking = No ; Payoffs = ( 0.00 , 0.00 ) ; Target = None }\n");
        fprintf(fp, "{ Type = PREVENT ; TurnTaking = No ; Payoffs = ( 0.00 , 0.00 ) ; Target = None }\n");
        fprintf(fp, "{ Type = FOLLOWER ; TurnTaking = No ; Payoffs = ( 0.60 , 0.60 ) ; Target = AX }\n");
        fprintf(fp, "{ Type = LEADER ; TurnTaking = No ; Payoffs = ( 0.60 , 0.60 ) ; Target = AX }\n");
        fprintf(fp, "{ Type = FOLLOWER ; TurnTaking = Yes ; Payoffs = ( 0.50 , 0.50 ) ; Target = AY-BX }\n");
        fprintf(fp, "{ Type = LEADER ; TurnTaking = Yes ; Payoffs = ( 0.50 , 0.50 ) ; Target = AY-BX }\n");
        fprintf(fp, "{ Type = FOLLOWER ; TurnTaking = Yes ; Payoffs = ( 0.30 , 0.80 ) ; Target = AY-AX }\n");
        fprintf(fp, "{ Type = LEADER ; TurnTaking = Yes ; Payoffs = ( 0.30 , 0.80 ) ; Target = AY-AX }\n");
        fprintf(fp, "{ Type = FOLLOWER ; TurnTaking = Yes ; Payoffs = ( 0.80 , 0.30 ) ; Target = AX-BX }\n");
        fprintf(fp, "{ Type = LEADER ; TurnTaking = Yes ; Payoffs = ( 0.80 , 0.30 ) ; Target = AX-BX }\n");
    }
    else if (!strncmp("chicken2", nombre, 8)) {
        //fprintf(fp, "Header for chicken2\n");
        fprintf(fp, "2\n2\n");
        fprintf(fp, "0.000000\t0.000000\n");
        fprintf(fp, "1.000000\t0.330000\n");
        fprintf(fp, "0.330000\t1.000000\n");
        fprintf(fp, "0.840000\t0.840000\n");
        fprintf(fp, "0.330000 0.330000\n");
        fprintf(fp, "0.000000 1.000000\n");
        fprintf(fp, "0.000000 1.000000\n");
        fprintf(fp, "1.000000 0.000000\n");
        fprintf(fp, "1.000000 0.000000\n");
        fprintf(fp, "numExperts = 13\n");
        fprintf(fp, "{ Type = MAXIMIN ; TurnTaking = No ; Payoffs = ( 0.00 , 0.00 ) ; Target = None }\n");
        fprintf(fp, "{ Type = BR ; TurnTaking = No ; Payoffs = ( 0.00 , 0.00 ) ; Target = None }\n");
        fprintf(fp, "{ Type = PREVENT ; TurnTaking = No ; Payoffs = ( 0.00 , 0.00 ) ; Target = None }\n");
        fprintf(fp, "{ Type = FOLLOWER ; TurnTaking = No ; Payoffs = ( 0.84 , 0.84 ) ; Target = BY }\n");
        fprintf(fp, "{ Type = LEADER ; TurnTaking = No ; Payoffs = ( 0.84 , 0.84 ) ; Target = BY }\n");
        fprintf(fp, "{ Type = FOLLOWER ; TurnTaking = No ; Payoffs = ( 1.00 , 0.33 ) ; Target = AY }\n");
        fprintf(fp, "{ Type = LEADER ; TurnTaking = No ; Payoffs = ( 1.00 , 0.33 ) ; Target = AY }\n");
        fprintf(fp, "{ Type = FOLLOWER ; TurnTaking = No ; Payoffs = ( 0.33 , 1.00 ) ; Target = BX }\n");
        fprintf(fp, "{ Type = LEADER ; TurnTaking = No ; Payoffs = ( 0.33 , 1.00 ) ; Target = BX }\n");
        fprintf(fp, "{ Type = FOLLOWER ; TurnTaking = Yes ; Payoffs = ( 0.92 , 0.58 ) ; Target = AY-BY }\n");
        fprintf(fp, "{ Type = LEADER ; TurnTaking = Yes ; Payoffs = ( 0.92 , 0.58 ) ; Target = AY-BY }\n");
        fprintf(fp, "{ Type = FOLLOWER ; TurnTaking = Yes ; Payoffs = ( 0.58 , 0.92 ) ; Target = BX-BY }\n");
        fprintf(fp, "{ Type = LEADER ; TurnTaking = Yes ; Payoffs = ( 0.58 , 0.92 ) ; Target = BX-BY }\n");
    }
    else if (!strncmp("blocks2", nombre, 7)) {
        fprintf(fp, "3\n3\n");
        fprintf(fp, "0.000000\t0.000000\n");
        fprintf(fp, "0.350000\t0.700000\n");
        fprintf(fp, "1.000000\t0.400000\n");
        fprintf(fp, "0.700000\t0.350000\n");
        fprintf(fp, "0.100000\t0.100000\n");
        fprintf(fp, "0.450000\t0.300000\n");
        fprintf(fp, "0.400000\t1.000000\n");
        fprintf(fp, "0.300000\t0.450000\n");
        fprintf(fp, "0.400000\t0.400000\n");
        fprintf(fp, "0.311111 0.311111\n");  // maximin values
        fprintf(fp, "0.222222 0.000000 0.777778\n");  // maximin strategy 0
        fprintf(fp, "0.222222 0.000000 0.777778\n");  // maximin strategy 1
        fprintf(fp, "0.111111 0.888889 0.000000\n");  // attack strategy 0
        fprintf(fp, "0.111111 0.888889 0.000000\n");  // attack strategy 1
        fprintf(fp, "numExperts = 9\n");
        fprintf(fp, "{ Type = MAXIMIN ; TurnTaking = No ; Payoffs = ( 0.00 , 0.00 ) ; Target = None }\n");
        fprintf(fp, "{ Type = BR ; TurnTaking = No ; Payoffs = ( 0.00 , 0.00 ) ; Target = None }\n");
        fprintf(fp, "{ Type = PREVENT ; TurnTaking = No ; Payoffs = ( 0.00 , 0.00 ) ; Target = None }\n");
        fprintf(fp, "{ Type = FOLLOWER ; TurnTaking = No ; Payoffs = ( 0.40 , 1.00 ) ; Target = CX }\n");
        fprintf(fp, "{ Type = LEADER ; TurnTaking = No ; Payoffs = ( 0.40 , 1.00 ) ; Target = CX }\n");
        fprintf(fp, "{ Type = FOLLOWER ; TurnTaking = Yes ; Payoffs = ( 0.70 , 0.70 ) ; Target = CX-AZ }\n");
        fprintf(fp, "{ Type = LEADER ; TurnTaking = Yes ; Payoffs = ( 0.70 , 0.70 ) ; Target = CX-AZ }\n");
        fprintf(fp, "{ Type = FOLLOWER ; TurnTaking = No ; Payoffs = ( 1.00 , 0.40 ) ; Target = AZ }\n");
        fprintf(fp, "{ Type = LEADER ; TurnTaking = No ; Payoffs = ( 1.00 , 0.40 ) ; Target = AZ }\n");
    }
    else if (!strncmp("endless", nombre, 7)) {
        fprintf(fp, "2\n2\n");
        fprintf(fp, "0.330000\t0.670000\n");
        fprintf(fp, "0.670000\t1.000000\n");
        fprintf(fp, "0.000000\t0.330000\n");
        fprintf(fp, "1.000000\t0.000000\n");
//        fprintf(fp, "0.000000 0.000000\n");
//        fprintf(fp, "0.000000 1.000000\n");
//        fprintf(fp, "0.000000 1.000000\n");
//        fprintf(fp, "1.000000 0.000000\n");
//        fprintf(fp, "1.000000 0.000000\n");
        fprintf(fp, "numExperts = 9\n");
        fprintf(fp, "{ Type = MAXIMIN ; TurnTaking = No ; Payoffs = ( 0.00 , 0.00 ) ; Target = None }\n");
        fprintf(fp, "{ Type = BR ; TurnTaking = No ; Payoffs = ( 0.00 , 0.00 ) ; Target = None }\n");
        fprintf(fp, "{ Type = PREVENT ; TurnTaking = No ; Payoffs = ( 0.00 , 0.00 ) ; Target = None }\n");
        fprintf(fp, "{ Type = FOLLOWER ; TurnTaking = No ; Payoffs = ( 0.67 , 1.00 ) ; Target = AY }\n");
        fprintf(fp, "{ Type = LEADER ; TurnTaking = No ; Payoffs = ( 0.67 , 1.00 ) ; Target = AY }\n");
        fprintf(fp, "{ Type = FOLLOWER ; TurnTaking = No ; Payoffs = ( 0.67 , 1.00 ) ; Target = AY }\n");
        fprintf(fp, "{ Type = LEADER ; TurnTaking = No ; Payoffs = ( 0.67 , 1.00 ) ; Target = AY }\n");
        fprintf(fp, "{ Type = FOLLOWER ; TurnTaking = Yes ; Payoffs = ( 0.83 , 0.50 ) ; Target = AY-BY }\n");
        fprintf(fp, "{ Type = LEADER ; TurnTaking = Yes ; Payoffs = ( 0.83 , 0.50 ) ; Target = AY-BY }\n");
    }
    else {
        fprintf(fp, "game not found -- can't write header\n");
    }
}

void writeRounds(FILE *fp, const char playerstr[1024], const char nombre[1024], int me, int numRounds, int actions[60][2], char sentMensajes[60][2][2048]) {
    int i;
    for (i = 0; i < numRounds; i++) {
        fprintf(fp, "\nRound %i\n", i);
        fprintf(fp, "CurrentExpert: 0\n");
        fprintf(fp, "AspirationLevel: 1.0\n");
        fprintf(fp, "ExpertSuccess: 0\n");
        fprintf(fp, "CycleStatus: <none>\n");
        fprintf(fp, "tsProposal: 99999\n");
        fprintf(fp, "lastProposal: <none>\n");
        if (numExperts == 11) {     // prisoners
            fprintf(fp, "Potentials: 0.000000 1.000000 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000\n");
            fprintf(fp, "historicalAve: 1.000000 1.000000 1.000000 1.000000 1.000000 1.000000 1.000000 1.000000 1.000000 1.000000 1.000000\n");
            fprintf(fp, "Congruence: 0 0 0 0 0 0 0 0 0 0 0\n");
        }
        else if (numExperts == 13) { // chicken2
            fprintf(fp, "Potentials: 0.000000 1.000000 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000\n");
            fprintf(fp, "historicalAve: 1.000000 1.000000 1.000000 1.000000 1.000000 1.000000 1.000000 1.000000 1.000000 1.000000 1.000000 1.000000 1.000000\n");
            fprintf(fp, "Congruence: 0 0 0 0 0 0 0 0 0 0 0 0 0\n");
        }
        else if (numExperts == 9) {  // blocks2 and endless
            fprintf(fp, "Potentials: 0.000000 1.000000 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000\n");
            fprintf(fp, "historicalAve: 1.000000 1.000000 1.000000 1.000000 1.000000 1.000000 1.000000 1.000000 1.000000\n");
            fprintf(fp, "Congruence: 0 0 0 0 0 0 0 0 0\n");
        }
        fprintf(fp, "currentState: 0\n");
        if (i != (numRounds-1)) {
            fprintf(fp, "My_CT:%s\n", sentMensajes[i][me]);
            fprintf(fp, "Their_CT:%s\n", sentMensajes[i][1-me]);
            fprintf(fp, "Re-CurrentExpert: 1\n");
            fprintf(fp, "Re-tsProposal: 0\n");
            fprintf(fp, "Re-lastProposal: 00\n");
            if (numExperts == 11) // prisoners
                fprintf(fp, "re-Congruence: 0 0 0 0 0 0 0 0 0 0 0\n");
            else if (numExperts == 13) // chicken2
                fprintf(fp, "re-Congruence: 0 0 0 0 0 0 0 0 0 0 0 0 0\n");
            else if (numExperts == 9) // blocks2 or endless
                fprintf(fp, "re-Congruence: 0 0 0 0 0 0 0 0 0\n");
            fprintf(fp, "re-currentState: 0\n");
            fprintf(fp, "meMove: %i\n", actions[i][me]);
            fprintf(fp, "theirMove: %i\n", actions[i][1-me]);
            fprintf(fp, "newExpertSuccess: 1.000000\n");
            fprintf(fp, "newCycleStatus: 10\n");
        }
    }
}

// ./dynamicTrajectory [playerstr] [game] [me]
int main(int argc, char *argv[]) {
    if (argc < 4) {
        printf("not enough arguments\n");
        exit(1);
    }
    char nombre[1024], playerstr[1024];
    strcpy(playerstr, argv[1]);
    strcpy(nombre, argv[2]);
    int me = atoi(argv[3]);
    
    int nr = 1;
    if (!strcmp(nombre, "prisoners")) {
        numExperts = 11;
        nr = 51;
    }
    else if (!strcmp(nombre, "chicken2")) {
        numExperts = 13;
        nr = 54;
    }
    else if (!strcmp(nombre, "blocks2")) {
        numExperts = 9;
        nr = 47;
    }
    else if (!strcmp(nombre, "endless")) {
        numExperts = 9;
        nr = 50;
    }
    
    int actions[60][2];
    int numRounds = getActions(actions, nombre, playerstr, nr);
    
    printf("numRounds = %i\n", numRounds);
    
    char mensajes[19][1024], sentMensajes[60][2][2048];
    loadMessages(mensajes);
    readMessages(mensajes, sentMensajes, nombre, playerstr, nr);
    
    char fnombre[1024];
    sprintf(fnombre, "../../CurrentData/dynamictrajectory_%s_%s_%i.txt", playerstr, nombre, me);
    FILE *fp = fopen(fnombre, "w");
    
    writeHeader(fp, nombre, me);
    writeRounds(fp, playerstr, nombre, me, numRounds, actions, sentMensajes);

    fclose(fp);

    return 0;
}