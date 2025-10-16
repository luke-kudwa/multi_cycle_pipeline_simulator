#ifndef PROCESSOR_H
#define PROCESSOR_H

#include <iostream>
#include <string>
#include <vector>
#include "memory.h"
#include "instruction.h"

using namespace std;

class processor{
 public:
  processor(vector<instruction *> list);
  //this will store the next instruction in orderCompleted
  void nextInstruction();
  //function returns true if it can go to next cycle... false if we are done cycling
  bool nextCycle();
  void printInstructions();
  bool instructionsDone();
  bool isPrevStalled(int index);
  //checks dependency if a prev instruction is using it, returns -1 if it isn't...
  //index refers to index of instruction being check for dependencies
  int checkDependency(string dependent, int index);
  vector<string> setParameters(vector<string> p, vector<int> dLoc, int index);
  bool prevInMemStage(int index);
  void endStalls();
  bool hasPrevFinishedMem(int index);
  
 private:
  //memory object we will use
  memory mem;

  //contains list of instructions in order
  vector<instruction *> instructionList;
  //contains list of what instruction is being completed
  vector<instruction *> orderCompleted;
  bool prevInMem = false;
  int currInstruction = 0;
  int currCycle = 0;

  
};
#endif
