/*
Author: Luke Kudwa
Teacher: Prof Ivan
Assignment: Project
Date Due: 7/4/2025
 */

#include "memory.h"
#include "processor.h"
#include "instruction.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

const string fileName = "input.txt";

//put helper function headers here
vector<string> loadFile();
vector<instruction *> getList();



int main(){
  //vector that contains an instruction object 
  vector<instruction *> instructionList = getList();
  processor process(instructionList);

  return 0;
}

vector<instruction *> getList(){
  //vector that contains a string of every instruction in input.txt in order
  vector<string> instructions = loadFile();
  //vector that contains an instruction object
  vector<instruction *> instructionL;

  //populating vector with instructions
  for(string s : instructions){
    instruction *newInstruction = new instruction(s);
    instructionL.push_back(newInstruction);
  }//end for loop

  return instructionL;
}

//this function will read in every possible instruction in order and order it into a string[]
vector<string> loadFile(){
  //create obj to read textfile
  ifstream scanner (fileName);
  //create vector that stores each line as a seperate string to be processed
  vector<string> lines;
  string line;
  
  if(scanner.is_open()){
    //while loop to read each line in text file and store it in the vector
    while(getline(scanner, line)){
      lines.push_back(line);
    }//end while loop
  }//end if
  else{
    cout << "File not found!" << endl;
  }

  //close input.txt file since we have all data already stored in a vector
  scanner.close();
  return lines;
}//end load file

