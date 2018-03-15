//Russell Babb 
//A01537969

//header files
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <signal.h>

//my header files
#include "shellFunctions.hpp"



bool quittingTime = false;

void sigint(int signal){
    quittingTime = true;
}



using namespace std;


int main(int argc, char** argv){
    
    //setup CTRL-C capablility
    signal(SIGINT, sigint);

    //setup variables
	vector< string > command;
	vector< vector< string > > history;
    stringstream ss;
    string input;
    string token;
    double ptime = 0;

	while(true){
        //clear variables
        input.clear();
        ss.clear();
        token.clear();
        command.clear();

        //print display
		printf("[cmd]: ");

        //get input
        getline(cin,input);
		ss << input;
       
        //parse input
        while(ss >> token){
            command.push_back(token);
        }

        //record history
        history.push_back(command);

        //// built in commands
        if(command[0] == "exit"){
            break;
        }else{
            ptime = commandParse(command,history,ptime); 
        }

    }



	return 0;
}
