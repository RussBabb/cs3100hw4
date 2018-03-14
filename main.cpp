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

	vector< string > command;
	vector< vector< string > > history;
    
    signal(SIGINT, sigint);

    stringstream ss;
    string input;
    string token;

	
	bool on = true;

	while(on){
		printf("[cmd]: ");
        
        input.clear();
        ss.clear();
        token.clear();
        command.clear();


        getline(cin,input);
		ss << input;
       
        while(ss >> token){
            command.push_back(token);
        }

        history.push_back(command);


        //printf("history length %i \n",history.size());





        //// built in commands
        //exit *complete*
        if(command[0] == "exit"){
            break;
        }else{
            commandParse(command,history); 
        }
    }



	return 0;
}
