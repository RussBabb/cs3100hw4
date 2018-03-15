#ifndef SHELLFUNCTIONS_HPP
#define SHELLFUNCTIONS_HPP


#include <iostream>
#include <vector>
#include <string>


using namespace std;


double execute( vector< string > );
void printHistory(vector< vector< string > >);
double historyCmd(string, vector< vector< string > >, double);
double commandParse(vector< string >, vector< vector< string > >,double);
void printPtime(double);



#endif
