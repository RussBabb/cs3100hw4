#ifndef SHELLFUNCTIONS_HPP
#define SHELLFUNCTIONS_HPP


#include <iostream>
#include <vector>
#include <string>


using namespace std;


void execute( vector< string > );
void printHistory(vector< vector< string > >);
void historyCmd(string, vector< vector< string > >);
void commandParse(vector< string >, vector< vector< string > >);



#endif
