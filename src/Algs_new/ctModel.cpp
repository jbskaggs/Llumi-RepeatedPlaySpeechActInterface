#include "ctModel.h"

ctModel::ctModel() {
    currentStep = -1;
    lastProposed[0][0] = lastProposed[0][1] = lastProposed[1][0] = lastProposed[1][1] = -1;
    enforcer = vengeful = happy = false;
    tsProposal = 99999;
    alreadyPickedLast = false;
    notThis = -1;
    
    elapsed = -1.0;
}

ctModel::~ctModel() {
}

bool ctModel::update(char buf[10000], int _A[2], bool _XAI) {
    tsProposal ++;

    // return false;

    numMessagesReceived = (int)((strlen(buf)+18) / 20.0); // assume one message is about 15 characters

    // printf("The speech is: %s\n", buf);

    bool ax, ay, az, bx, by, bz, cx, cy, cz;
    ax = ay = az = bx = by = bz = cx = cy = cz = false;

    int count = 0;
    if (!strncmp(buf, "ax", 2) || !strncmp(buf, "AX", 2) ||
        strstr(buf, "-ax") || strstr(buf, " ax") || strstr(buf, "a-x") || strstr(buf, "-AX")  || strstr(buf, "AX") || strstr(buf, "A-X") || strstr(buf, "-Ax")  || strstr(buf, "Ax") || strstr(buf, "A-x") ||
        strstr(buf, "-xa") || strstr(buf, " xa") || strstr(buf, "x-a") || strstr(buf, "-XA")  || strstr(buf, "XA") || strstr(buf, "X-A")) {
        ax = true;
        count ++;
    }
    if (!strncmp(buf, "ay", 2) || !strncmp(buf, "AY", 2) ||
        strstr(buf, "-ay") || strstr(buf, " ay") || strstr(buf, "a-y") || strstr(buf, "-AY")  || strstr(buf, "AY") || strstr(buf, "A-Y") || strstr(buf, "-Ay")  || strstr(buf, "Ay") || strstr(buf, "A-y") ||
        strstr(buf, "-ya") || strstr(buf, " ya") || strstr(buf, "y-a") || strstr(buf, "-AY")  || strstr(buf, "YA") || strstr(buf, "Y-A")) {
        ay = true;
        count ++;
    }
    if (!strncmp(buf, "az", 2) || !strncmp(buf, "AZ", 2) ||
        strstr(buf, "-az") || strstr(buf, " az") || strstr(buf, "a-z") || strstr(buf, "-AZ") || strstr(buf, "AZ") || strstr(buf, "A-Z") || strstr(buf, "-Az") || strstr(buf, "Az") || strstr(buf, "A-z") ||
        strstr(buf, "-za") || strstr(buf, " za") || strstr(buf, "z-a") || strstr(buf, "-ZA") || strstr(buf, "ZA") || strstr(buf, "Z-A")) {
        if (_A[1] > 2) {
            az = true;
            count ++;
        }
    }

    if (!strncmp(buf, "bx", 2) || !strncmp(buf, "BX", 2) ||
        strstr(buf, "-bx") || strstr(buf, " bx") || strstr(buf, "b-x") || strstr(buf, "-BX")  || strstr(buf, "BX") || strstr(buf, "B-X") || strstr(buf, "-Bx")  || strstr(buf, "Bx") || strstr(buf, "B-x") ||
        strstr(buf, "-xb") || strstr(buf, " xb") || strstr(buf, "x-b") || strstr(buf, "-XB")  || strstr(buf, "XB") || strstr(buf, "X-B")) {
        bx = true;
        count ++;
    }
    if (!strncmp(buf, "by", 2) || !strncmp(buf, "BY", 2) ||
        strstr(buf, "-by") || strstr(buf, " by") || strstr(buf, "b-y") || strstr(buf, "-BY")  || strstr(buf, "BY") || strstr(buf, "B-Y") || strstr(buf, "-By")  || strstr(buf, "By") || strstr(buf, "B-y") ||
        strstr(buf, "-yb") || strstr(buf, " yb") || strstr(buf, "y-b") || strstr(buf, "-BY")  || strstr(buf, "YB") || strstr(buf, "Y-B")) {
        by = true;
        count ++;
    }
    if (!strncmp(buf, "bz", 2) || !strncmp(buf, "BZ", 2) ||
        strstr(buf, "-bz") || strstr(buf, " bz") || strstr(buf, "b-z") || strstr(buf, "-BZ") || strstr(buf, "BZ") || strstr(buf, "B-Z") || strstr(buf, "-Bz") || strstr(buf, "Bz") || strstr(buf, "B-z") ||
        strstr(buf, "-zb") || strstr(buf, " zb") || strstr(buf, "z-b") || strstr(buf, "-ZB") || strstr(buf, "ZB") || strstr(buf, "Z-B")) {
        if (_A[1] > 2) {
            bz = true;
            count ++;
        }
    }

    if (!strncmp(buf, "cx", 2) || !strncmp(buf, "CX", 2) ||
        strstr(buf, "-cx") || strstr(buf, " cx") || strstr(buf, "c-x") || strstr(buf, "-CX")  || strstr(buf, "CX") || strstr(buf, "C-X") || strstr(buf, "-Cx")  || strstr(buf, "Cx") || strstr(buf, "C-x") ||
        strstr(buf, "-xc") || strstr(buf, " xc") || strstr(buf, "x-c") || strstr(buf, "-XC")  || strstr(buf, "XC") || strstr(buf, "X-C")) {
        if (_A[0] > 2) {
            cx = true;
            count ++;
        }
    }
    if (!strncmp(buf, "cy", 2) || !strncmp(buf, "CY", 2) ||
        strstr(buf, "-cy") || strstr(buf, " cy") || strstr(buf, "c-y") || strstr(buf, "-CY")  || strstr(buf, "CY") || strstr(buf, "C-Y") || strstr(buf, "-Cy")  || strstr(buf, "Cy") || strstr(buf, "C-y") ||
        strstr(buf, "-yc") || strstr(buf, " yc") || strstr(buf, "y-c") || strstr(buf, "-CY")  || strstr(buf, "YC") || strstr(buf, "Y-C")) {
        if (_A[0] > 2) {
            cy = true;
            count ++;
        }
    }
    if (!strncmp(buf, "cz", 2) || !strncmp(buf, "CZ", 2) ||
        strstr(buf, "-cz") || strstr(buf, " cz") || strstr(buf, "c-z") || strstr(buf, "-CZ") || strstr(buf, "CZ") || strstr(buf, "C-Z") || strstr(buf, "-Cz") || strstr(buf, "Cz") || strstr(buf, "C-z") ||
        strstr(buf, "-zc") || strstr(buf, " zc") || strstr(buf, "z-c") || strstr(buf, "-ZC") || strstr(buf, "ZC") || strstr(buf, "Z-C")) {
        if ((_A[0] > 2) && (_A[1] > 2)) {
            cz = true;
            count ++;
        }
    }

    // printf("count = %i\n", count);

    if (count == 0) {
        printf("no new proposals found\n");
    }
    else if (count == 1) {
        if (strstr(buf, "always") || !alreadyInIt(ax, ay, az, bx, by, bz, cx, cy, cz)) {
            if (ax || ay || (az && (_A[1] > 2)))
                lastProposed[0][0] = lastProposed[1][0] = 0;
            else if (bx || by || (bz && (_A[1] > 2)))
                lastProposed[0][0] = lastProposed[1][0] = 1;
            else if ((cx || cy || cz) && (_A[1] > 2))
                lastProposed[0][0] = lastProposed[1][0] = 2;

            if (ax || bx || (cx && (_A[0] > 2)))
                lastProposed[0][1] = lastProposed[1][1] = 0;
            else if (ay || by || (cy && (_A[0] > 2)))
                lastProposed[0][1] = lastProposed[1][1] = 1;
            else if ((az || bz || cz) && (_A[0] > 2))
                lastProposed[0][1] = lastProposed[1][1] = 2;

            currentStep = 0;
            vengeful = false;
            tsProposal = 0;
            alreadyPickedLast = false;
        }
        else {
            printf("proposal consistent with previous proposal\n");
        }
    }
    else if (count == 2) {
        int index = 0;

        if (ax) {
            lastProposed[index][0] = 0;
            lastProposed[index][1] = 0;
            index ++;
        }

        if (bx) {
            lastProposed[index][0] = 1;
            lastProposed[index][1] = 0;
            index ++;
        }

        if (cx && (_A[0] > 2)) {
            lastProposed[index][0] = 2;
            lastProposed[index][1] = 0;
            index ++;
        }

        if (ay) {
            lastProposed[index][0] = 0;
            lastProposed[index][1] = 1;
            index ++;
        }

        if (by) {
            lastProposed[index][0] = 1;
            lastProposed[index][1] = 1;
            index ++;
        }

        if (cy && (_A[0] > 2)) {
            lastProposed[index][0] = 2;
            lastProposed[index][1] = 1;
            index ++;
        }

        if (az && (_A[1] > 2)) {
            lastProposed[index][0] = 0;
            lastProposed[index][1] = 2;
            index ++;
        }

        if (bz && (_A[1] > 2)) {
            lastProposed[index][0] = 1;
            lastProposed[index][1] = 2;
            index ++;
        }

        if (cz && (_A[0] > 2) && (_A[1] > 2)) {
            lastProposed[index][0] = 2;
            lastProposed[index][1] = 2;
            index ++;
        }

        currentStep = -1;
        vengeful = false;
        tsProposal = 0;
        alreadyPickedLast = false;
    }
    else {
        printf("too many proposals found -- confused\n");
    }

    printf("lastProposed: %i%i - %i%i\n", lastProposed[0][0], lastProposed[0][1], lastProposed[1][0], lastProposed[1][1]);

    return false;
}

bool ctModel::alreadyInIt(bool ax, bool ay, bool az, bool bx, bool by, bool bz, bool cx, bool cy, bool cz) {
    if (ax && ((lastProposed[0][0] == 0) && (lastProposed[0][1] == 0)) || ((lastProposed[1][0] == 0) && (lastProposed[1][1] == 0)))
        return true;

    if (ay && ((lastProposed[0][0] == 0) && (lastProposed[0][1] == 1)) || ((lastProposed[1][0] == 0) && (lastProposed[1][1] == 1)))
        return true;

    if (az && ((lastProposed[0][0] == 0) && (lastProposed[0][1] == 2)) || ((lastProposed[1][0] == 0) && (lastProposed[1][1] == 2)))
        return true;

    if (bx && ((lastProposed[0][0] == 1) && (lastProposed[0][1] == 0)) || ((lastProposed[1][0] == 1) && (lastProposed[1][1] == 0)))
        return true;

    if (by && ((lastProposed[0][0] == 1) && (lastProposed[0][1] == 1)) || ((lastProposed[1][0] == 1) && (lastProposed[1][1] == 1)))
        return true;

    if (bz && ((lastProposed[0][0] == 1) && (lastProposed[0][1] == 2)) || ((lastProposed[1][0] == 1) && (lastProposed[1][1] == 2)))
        return true;

    if (cx && ((lastProposed[0][0] == 2) && (lastProposed[0][1] == 0)) || ((lastProposed[1][0] == 2) && (lastProposed[1][1] == 0)))
        return true;

    if (cy && ((lastProposed[0][0] == 2) && (lastProposed[0][1] == 1)) || ((lastProposed[1][0] == 2) && (lastProposed[1][1] == 1)))
        return true;

    if (cz && ((lastProposed[0][0] == 2) && (lastProposed[0][1] == 2)) || ((lastProposed[1][0] == 2) && (lastProposed[1][1] == 2)))
        return true;

    return false;
}

/*
bool ctModel::update(char buf[10000], int _A[2], bool _XAI) {
    char *tkn;
    int msgType;
    
    tsProposal ++;
    bool secondChance = false;
    
    tkn = strtok(buf, ";");
    //printf("____________\n");
    numMessagesReceived = 0;
    while (true) {
        //printf("\t%s: ", tkn);
        
        if (!strncmp(tkn, "$", 1)) {
            //printf("\n");
            elapsed = atof(tkn+2);
            break;
        }
        
        msgType = atoi(tkn);
        
        numMessagesReceived ++;
        
        if (_XAI) {
            switch (msgType) {
                    //printf("(%i) ", msgType);
                case LETS_COOP:
                    lastProposed[0][0] = lastProposed[1][0] = lookUp(tkn[2], 0, _A);
                    lastProposed[0][1] = lastProposed[1][1] = lookUp(tkn[3], 1, _A);
                    currentStep = 0;
                    vengeful = false;
                    tsProposal = 0;
                    alreadyPickedLast = false;
                    
                    //printf("pure solution: %i %i", lastProposed[0][0], lastProposed[0][1]);
                    break;
                case TAKE_TURNS:
                    lastProposed[0][0] = lookUp(tkn[2], 0, _A);
                    lastProposed[0][1] = lookUp(tkn[3], 1, _A);
                    lastProposed[1][0] = lookUp(tkn[5], 0, _A);
                    lastProposed[1][1] = lookUp(tkn[6], 1, _A);
                    currentStep = -1;
                    vengeful = false;
                    tsProposal = 0;
                    alreadyPickedLast = false;
                    
                    //printf("alternating solution: (%i, %i) then (%i, %i)\n", lastProposed[0][0], lastProposed[0][1], lastProposed[1][0], lastProposed[1][1]);
                    break;
                case SPECIFY:
                    //printf("SPECIFY: ");
                    
                    int acts[2];
                    
                    acts[0] = lookUp(tkn[2], 0, _A);
                    acts[1] = lookUp(tkn[3], 1, _A);
                    
                    //printf("%i, %i\n", acts[0], acts[1]);
                    
                    currentStep = -1;
                    if ((acts[0] == lastProposed[0][0]) && (acts[1] == lastProposed[0][1]))
                        currentStep = 0;
                    else if ((acts[0] == lastProposed[1][0]) && (acts[1] == lastProposed[1][1]))
                        currentStep = 1;
                    
                    //if (currentStep >= 0)
                    //    printf("time to play: %i %i.  ", lastProposed[currentStep][0], lastProposed[currentStep][1]);
                    else {
                        lastProposed[0][0] = lastProposed[1][0] = acts[0];
                        lastProposed[0][1] = lastProposed[1][1] = acts[1];
                        currentStep = 0;
                        
                        vengeful = false;
                        tsProposal = 0;
                        alreadyPickedLast = false;
                        
                        //printf("pure solution: %i %i.  ", lastProposed[0][0], lastProposed[0][1]);
                    }
                    break;
                case 99: numMessagesReceived = 0; break;
                default:
                    printf("%s\n", tkn);
                    break;
            }
        }
        //printf("\n");
        
        tkn = strtok(NULL, ";");
    }
    //printf("-------------\n\n");
    
    return secondChance;
}
*/
int ctModel::lookUp(char ch, int index, int _A[2]) {
    if (index == 0) {
        if (ch == 'A')
            return 0;
        else if (ch == 'B')
            return 1;
        else if (ch == 'C')
            return 2;
    }
    else if (index == 1) {
        if (ch == 'X')
            return 0;
        else if (ch == 'Y')
            return 1;
        else if (ch == 'Z')
            return 2;
    }
    
    printf("(CT) unidentified action: %c (index = %i)\n", ch, index);
    
    return -1;
}