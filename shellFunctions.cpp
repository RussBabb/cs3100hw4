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
#include <error.h>

//my header files
#include "shellFunctions.hpp"

using namespace std;

const int WRITE = 1;
const int READ = 0;

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

double pipeCommand(vector< string > command1, vector< string > command2,  double ptime){
    /*
    for(int i = 0; i < command1.size(); i++){
        printf("%s\n",command1[i].c_str());
    }

    for(int i = 0; i < command2.size(); i++){
        printf("%s\n",command2[i].c_str());
    }
    */

    int p[2];
    pipe(p);

    executeData sender1 = returnCommand(command1);
    executeData sender2 = returnCommand(command2);
    

    if(fork() == 0){
        close(p[READ]);
        dup2(p[WRITE], STDOUT_FILENO);
        if (execvp(sender1.results[0],sender1.results) < 0){ // execute the command
            error(1, errno, "error in first command");
        }

    }

    if (fork() == 0){


        close(p[WRITE]);
        dup2(p[READ], STDIN_FILENO);
        if (execvp(sender2.results[0],sender2.results) < 0){ // execute the command
            error(1, errno, "error in second command");
        }
    }

    close(p[READ]);
    close(p[WRITE]);


    int wstatus;
    int kids = 2;
    while(kids > 0){
        pid_t kiddo = waitpid(-1, &wstatus, 0);
        //printf("Child Proc %i exited with status %i\n",kiddo,wstatus);
        kids--;
    }




    cleanUp(sender1);
    cleanUp(sender2);

    return ptime;
}

double commandParse(vector< string > command, vector< vector< string > > history, double ptime){

    if(command[0] == "history"){ //history *complete*
        printHistory(history);
    }else if(command[0] == "^"){ //^ <number> 
        ptime = historyCmd(command[1],history,ptime);
    }else if(command[0] == "ptime"){ //ptime *todo*
        printPtime(ptime);
    }else if(command[0] == "cd"){
        const char* test = command[1].c_str();
        chdir(test);
    }else{ //execute shell commands 
        bool foundPipe = false;
        int pipeLocation = 0;
        for(int i = 0; i < command.size(); i++){
            if(command[i] == "|"){
                foundPipe = true;
                pipeLocation = i;
            }
        }
        if(foundPipe){
            vector< string > command1;
            vector< string > command2;

            for(int i = 0; i < pipeLocation; i++){
                command1.push_back(command[i]);
            }

            for(int i = pipeLocation + 1; i < command.size(); i++){
                command2.push_back(command[i]);
            }

            ptime = pipeCommand(command1,command2,ptime);

        }else{
            ptime = execute(command);
        }
    }

    return ptime;
}

void printPtime(double ptime){

    printf("Time spent executing the last child process: %.4f seconds\n",ptime);
}
