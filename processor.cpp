#include "processor.h"

processor::processor(vector<instruction *> list){
  instructionList = list;

  //grab first instruction so we can start
  //nextInstruction();

  
  //cycle through and complete every instruction, while instructions are NOT done

  /*  
  for(int i = 0; i < 22; i++){
    nextCycle();
  }
  */

  
  while(!instructionsDone()){
    nextCycle();
  }//end while
  

  
  printInstructions();  
  mem.printCache();
  mem.printIntRegisters();
  mem.printFPRegisters();
  mem.printMemory();
  
}//end constructor


bool processor::nextCycle(){
  //for every instruction
  for(int i = 0; i < int(orderCompleted.size()); i++){
    //check if a previous one is stalling
    if(!isPrevStalled(i)){
      //figure out what stage we are on
      int stage = orderCompleted[i]->whatStage();
      if(stage == 0){
	orderCompleted[i]->nextStage(currCycle);
      }//end if stage is 0
      else if(stage == 1){//if it wasn't 0, check if it was IF stage
	orderCompleted[i]->nextStage(currCycle);
      }else if(stage == 2){//if we are in the ID stage
	//check if we can go to the next stage... first we get parameters, then check dependancies
	//Getting parameters and then locations of dependancies
	vector<string> p = orderCompleted[i]->getParameters();
	vector<int> loc = orderCompleted[i]->getDLocation();
	//now we check if any of our dependancies are being used and update our parameters
	p = setParameters(p, loc, i);	

	//if we haven't stalled, do operation
	if(!orderCompleted[i]->stalled()){
	  //get result and store into instruction... note it isn't availible for forwarding yet
	  int result = mem.doOperation(orderCompleted[i]->getInstructNum(), p);
	  orderCompleted[i]-> setResult(result);
	  //now that we have parameters, go to next stage
	  orderCompleted[i]->nextStage(currCycle);	  
	  if(!mem.isInCache()){
	    //function will add 2 cycles to mem stage for instruction since it wasn't in cache
	    orderCompleted[i]->missPenalty();
	  }//end if

	}//end if stage 2 and we are going to stage 3
	
      }else if(stage == 3){//if EX stage
	//go to next cycle
	orderCompleted[i]->nextStage(currCycle);
	//note: if we finish EX stage we will set forwarding availible if we need... happens in instruction class
      }else if(stage == 4){//in mem stage
	//we stall if a previous instruction is in the mem stage we stall,
	//we also stall if a previous instruction hasn't finished the mem stage and is in WB stage
	if(prevInMemStage(i) || !hasPrevFinishedMem(i)){//if a prev Instruction is in MEM stage
	  orderCompleted[i]->setStall(true);
	} else{//else we do the operation
	  orderCompleted[i]->nextStage(currCycle);
	  prevInMem = true;
	}//end else
      }else if(stage == 5){//if in WB stage, write back to mem and finish instruction
	//passing in our resultant (location we write to), our result value, the instruction number, and offset if needed
	mem.writeBack(orderCompleted[i]->getResultant(), orderCompleted[i]->getResult(),
		      orderCompleted[i]->getInstructNum(), orderCompleted[i]->getOffSetNum());
	//taking back the forwarding since we have written it to memory
	orderCompleted[i]->setAvailible(false);
	orderCompleted[i]->nextStage(currCycle);
      }//end else if in WB stage
      else{//else we are finshed and do nothing
	//N/A
      }//end else      
    }//end if prev stalled
    else{
      //we do nothing to instruction
    }

  }//end for loop

  //grab next instruction
  if(!isPrevStalled(orderCompleted.size()) || orderCompleted.empty()){
    //if not stalling, grab next instruction... or if we haven't started yet, grab next instruction
    nextInstruction();
  }//end if

  
  //undo all stalls for next go around
  endStalls();

  //iterate current cycle
  currCycle++;

  //reset prevInMem since we are going to next cycle
  prevInMem = false;
  
  //what we return doesn't matter, it isn't used
  return true;
}//end nextCycle

//this function will grab the next instruction and put it in orderComplete
void processor::nextInstruction(){

  //if our next instruction is within confines
  if(currInstruction < int(instructionList.size())){
    string instruct = instructionList[currInstruction]->getInstruction();
    instruction *i = new instruction(instruct);
    orderCompleted.push_back(i);
    orderCompleted[orderCompleted.size()-1]->nextStage(currCycle);
    currInstruction++;
  }
}//end nextInstruction

void processor::printInstructions(){

  cout << "Ins:\tIF\tID\tEX\tMEM\tWB" << endl;
  for(int i = 0; i < int(orderCompleted.size()); i++){
    orderCompleted[i]->printDone();
  }//end for loop
}//end printInstructions

bool processor::instructionsDone(){
  //if we haven't started yet, we say we haven't finished instructions
  if(orderCompleted.empty())
    return false;

  //cout << "before for loop" << endl;

  //if any instruction isn't done, return false
  for(int i = 0; i < int(orderCompleted.size()); i++){
    //cout << "Order size: " << orderCompleted.size() << "\tindex: " << i << endl;

    if(!orderCompleted[i]->isStructionDone())
      return false;
  }//end for loop
  //every instruction has finished
  return true;
}//end instructionsDOne


//recieves the index of the current function we are checking
bool processor::isPrevStalled(int index){

  //checks if any previous instruction is stalled
  for(int i = 0; i < index; i++){
    //if a previous instruction is being stalled, return true
    if(orderCompleted[i]->stalled())
      return true;
  }
  
  return false;
}


//returns -1 if there is no dependency on anything
int processor::checkDependency(string dependent, int index){
  //checks closest instructions for dependencies
  for(int i = index-1; i >= 0; i--){
    //get resultant
    //if our dependent matches the resultant and that the instruction hasn't written back to mmory, return it's index
    if(dependent == orderCompleted[i]->getResultant() && orderCompleted[i]->whatStage() != 6){
      return i;
    }//end if
  }//end for loop
  return -1;
}//end checkDependency


vector<string> processor::setParameters(vector<string> p, vector<int> dLoc, int index){
  //if there is no dependancy
  if(dLoc.empty())
    return p;

  //for every dependancy
  for(int i = 0; i < int(dLoc.size()); i++){
    //index holds the location of the dependancy in vector p
    int ind = dLoc[i];
    //dependancyLoc gets the address of the dependancy if there is one
    int dependancyLoc = checkDependency(p[ind], index); 

    //if there is a dependancy
    if(dependancyLoc != -1){
      //check if the data is availible...
      if(orderCompleted[dependancyLoc]->isAvailible()){

	//we will replace currParameter we are looking at with integer value that is availible
	int value = orderCompleted[dependancyLoc]->getResult();
	string sValue = to_string(value);
	//store int value as a string in vector p at index of dependancy location
	p[ind] = sValue;
      }//end inside if
      else{//else there is a dependancy but it isn't availible
	orderCompleted[index]->setStall(true);
      }//end else
    }//end if there is a dependancy
  }//end for loop

  return p;
}//end setParameters


bool processor::prevInMemStage(int index){
  return prevInMem;
}//end prevInMemStage


void processor::endStalls(){
  //for every instruction
  for(int i = 0; i < int(orderCompleted.size()); i++){
    //if it is stalled, make it no longer stalled
    if(orderCompleted[i]->stalled())
      orderCompleted[i]->setStall(false);
  }//end for loop
}//endStalls


//returns whether or not the previous instruction is past MEM stage
bool processor::hasPrevFinishedMem(int index){
  if(index == 0)
    return true;
  if(orderCompleted[index-1]->whatStage() > 4)
    return true;

  return false;
}//end hasPrevFinishedMem
