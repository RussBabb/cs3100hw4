//Russell Babb
//A01537969
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <cstring>
#include <chrono>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <chrono>
#include <ctime>

//my header files
#include "shellFunctions.hpp"

using namespace std;


struct executeData{
    char** results;
    int length;
};

executeData returnCommand(vector< string > command){
    executeData sender;
    char** results = new char*[command.size()+1];
    int length = command.size() + 1;

    for(int i = 0; i < command.size(); i++){
        results[i] = new char[command[i].length() + 1];
        strcpy(results[i], command[i].c_str());
    }
    results[length-1] = NULL;

    sender.results = results;
    sender.length = length;

    return sender;
}

void cleanUp(executeData toClean){

    for(int i = 0; i < toClean.length; i++){
        delete[] toClean.results[i];
    }

    delete[] toClean.results;

    return;
}

double execute(vector< string > command){
    pid_t pid;
    int status;
     
    executeData sender = returnCommand(command);
    

    if ((pid = fork()) < 0){ // fork a child process
            printf("*** ERROR: forking child process failed\n");
            exit(1);
    }else if(pid == 0){ //for the child process
        if (execvp(sender.results[0],sender.results) < 0){ // execute the command
            printf("*** ERROR: exec failed\n");
            exit(1);
        }
    }

    auto start = chrono::system_clock::now();
    wait(NULL);
    auto end = chrono::system_clock::now();
    chrono::duration<double> elapsed_seconds = end-start;

    cleanUp(sender);
    
    return elapsed_seconds.count();
}

void printHistory(vector< vector< string > > history){
    for(int i = 0; i < history.size(); i++){
        printf("%i: ",i+1);
        for(int j = 0; j < history[i].size(); j++){
            printf(" %s",history[i][j].c_str());
        }
        printf("\n");
    }
    return;
}


double historyCmd(string number,vector< vector< string > > history, double ptime){

    int num = stoi(number);
    
    if(num < 1||num > history.size() - 1){
        printf("*** ERROR: entered an invalid number\n");
    }else{
        if(history[num-1].size() > 1){
            if(number != history[num-1][1]){
                ptime = commandParse(history[num-1],history, ptime);
            }else{
                printf("*** ERROR: running command will cause segmentation fault\n");
            }
        }else{
            ptime = commandParse(history[num-1],history, ptime);
        }

    }

    return ptime;
}

double commandParse(vector< string > command, vector< vector< string > > history, double ptime){

    if(command[0] == "history"){ //history *complete*
        printHistory(history);
    }else if(command[0] == "^"){ //^ <number> 
        ptime = historyCmd(command[1],history,ptime);
    }else if(command[0] == "ptime"){ //ptime *todo*
        printPtime(ptime);
    }else{ //execute shell commands 
        ptime = execute(command);
    }

    return ptime;
}

void printPtime(double ptime){

    printf("Time spent executing the last child process: %.4f seconds\n",ptime);
}
