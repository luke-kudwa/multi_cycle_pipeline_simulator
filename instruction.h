#ifndef INSTRUCTION_H
#define INSTRUCTION_H

#include <iostream>
#include <string>
#include <vector>

using namespace std;

class instruction{
public:
  instruction(string line);
  bool checkBranch(string line);
  bool setPieces(string str);
  int defineInstruction(string str);
  string noComma(string str);
  void instructJ(string str);
  void instructLI(string str);
  void storeLoad(string str);
  void multParameters(string str);
  void printInstruction();
  string getInstruction();
  bool nextStage(int cycle);
  void printDone();
  bool isStructionDone();  
  bool stalled();
  bool doOperation();
  vector<string> getParameters();
  int getInstructNum();
  void setResult(int r);
  int getResult();
  void setCycles();
  void missPenalty();
  string getResultant();
  int whatStage();
  void setAvailible(bool t);
  vector<string> getDependancies();
  vector<int> getDLocation();
  bool isAvailible();
  void setStall(bool s);
  string getOffSetNum();
  //similar to other func with same name, but uses that and now also stores in private variable
  vector<string> getParameters2();
  
private:

  //string of instruction line, excluding possible branch part
  string instructionLine;
  //^^^ but with branch part
  string fullLine;
  
  //string of every possible instruction to account for
  vector<string> instructNames = {"L.D", "S.D", "LI", "LW", "SW", "ADD", "ADDI", "ADD.D",
		     "SUB.D", "SUB", "MUL.D", "DIV.D", "BEQ", "BNE", "J"};

  //this number corresponds to which instruction this is
  int instructNum;
  //this tells us if the istruction contains a saved address we can jump to
  bool isBranch = false;
  string branchName = "";
  int exCyclesLeft;
  int memCyclesLeft;
  //vector that contains a string name of each dependancy for instruction
  vector<string> dependancies;
  //vector contains a string name of all affected places in mem or registers
  vector<string> resultants;
  //this string is used to store the offset, jump to branch name, or Immediate integer
  string offsetNum;
  //this int stores the intermediate result
  int result;
  //is intermediate result availible
  bool resultAvailible = false;

  //when something defines parameters, it is stored here
  vector<string> parameters;
  
  //ENUM to tell what stage the instruction is in
  enum Stage{NONE, IF, ID, EX, MEM, WB, DONE};
  enum Stage currStage = NONE;
  bool isStalled = false;

  //stores the location of the dependancies in a vector when it is stored
  vector<int> dLocation;
  //stores when each stage was completed
  int stageCompleted[5];


};

#endif
