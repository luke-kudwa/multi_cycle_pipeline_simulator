#include "instruction.h"

//this constructor takes in a instruction line and processes it to be a instruction object
instruction::instruction(string line){
  instructionLine = line;
  //checks if its a branch
  checkBranch(line);
  instructNum = defineInstruction(instructionLine);
  //sets resultants, dependancies, and offsetNum
  setPieces(instructionLine);
  setCycles();
  
}


/*returs true if string is the start of a branch, and updates private values
ex/ Loop: L.D F2 $3 would return true
 */
bool instruction::checkBranch(string line){
  //this fullLine = line; was added in later, hence the seemingly redundancy
  fullLine = line;
  
  for(int i = 0; i < int(line.size()); i++){
    //if there is a ":", it is the start of a branch
    if(line[i] == ':'){
      isBranch = true;
      branchName = line.substr(0, i);
      instructionLine = line.substr(i+1, line.size());
      //now all data is updated for the branch
      return true;
    }
  }//end for each loop

  //not the start of a branch
  return false;
}


//this function will assign an interger value for which instruction this actually is
//the number corresponds to index of instructNames in .h file
int instruction::defineInstruction(string str){
  string firstPart;

  
  //for loop finds string before first space
  for(int i = 1; i < int(str.size()); i++){
    if(str[i] == ' '){
      firstPart = str.substr(0, i);
      break;
    }
  }//end for loop
  
  int index = 0;
  //cycles through every possible instruction and determines which one it is and returns the index
  for(string s : instructNames){
    if(firstPart == instructNames[index]){
      return index;
    }
    index++;
  }
  
  //-1 means the instruction name shouldn't exist
  return -1;
}//end defineInstruction

//this function will set the dependencies and resultant to be easily accessed later
bool instruction::setPieces(string str){

  string noC = noComma(str);

  if(instructNum == 0 || instructNum == 1 || instructNum == 3 || instructNum == 4)
    storeLoad(noC);
  if(instructNum == 2)
    instructLI(noC);
  if(instructNum == 15)
    instructJ(noC);
  if(instructNum > 4 && instructNum < 14)
    multParameters(noC);
  
  return true;
}

//removes comma from string sent in
string instruction::noComma(string str){
  string result = "";
  for(char c : str){
    if(c != ',')
      result += c;
  }
  return result;
}//end noComma

//function will set dependencies and resultant if it is a J instrustruction
void instruction::instructJ(string str){
  //setting branch name of where we jump to as a dependancy
  offsetNum = str.substr(2, str.size());
}

//sets resultant and offsetNum, the comma's are already removed from the line.
void instruction::instructLI(string str){
  string variables = str.substr(3, str.size());

  //string firstHalf;
  //string secondHalf;

  //for loop to find the space, store the parameters and then break for loop
  for(int i = 0; i < int(variables.size()); i++){
    //note struction format should be 
    if(variables[i] == ' '){
      resultants.push_back(variables.substr(0, i));
      offsetNum = variables.substr(i+1, variables.size());
      break;
    }//ebd if
  }//end for loop
}//end instructLI


//this function will isolate parameters of a few instruction types and store them, see end of function for where they are stored
void instruction::storeLoad(string str){
  bool isStore = str[0] == 'S';
  
  //get string of just parameters
  string parameters = str.substr(instructNames[instructNum].size()+1, str.size());
  string firstParameter;
  string offset;
  string address;
  
  //isolate the first parameter
  int space = parameters.find(" ");
  firstParameter = parameters.substr(0, space);
  parameters = parameters.substr(space+1, parameters.size());

  //find offset and address
  int paren = parameters.find("(");
  offset = parameters.substr(0, paren);
  parameters = parameters.substr(paren+1);
  address = parameters.substr(0, int(parameters.size())-1);
  
  //store parameters
  if(!isStore){
    resultants.push_back(firstParameter);
    dependancies.push_back(address);
  }else{
    dependancies.push_back(firstParameter);
    resultants.push_back(address);
  }
  //stores second and 
  offsetNum = offset;


  /*
Location of each parameter
Load resultant offset dependancy
Store dependancy offset resultant
    
   */
}//end store load



void instruction::multParameters(string str){
  //string to hold first, second and third parameter
  string first, second, third;

  //isolate the 3 parameters by a space
  str = str.substr(str.find(" ")+1, str.size());

  //isolate first variable
  first = str.substr(0, str.find(" "));
  str = str.substr(str.find(" ")+1, str.size());

  //isolate second variable
  second = str.substr(0, str.find(" "));
  str = str.substr(str.find(" ")+1, str.size());

  //last parameter is automatically isolated
  third = str;

  //if instruction: ADD.I
  if(instructNum == 6){
    //store parameters in correct spot
    resultants.push_back(first);
    dependancies.push_back(second);
    offsetNum = third;
  }else if(instructNum == 12 || instructNum == 13){
    //if instruction: BNE or BEQ
    dependancies.push_back(first);
    dependancies.push_back(second);
    offsetNum = third;
  }else{
    //rest of math instructions
    resultants.push_back(first);
    dependancies.push_back(second);
    dependancies.push_back(third);
  }
  //by here every parameter has been stored properly
  
}//end multParameters


//tester function
void instruction::printInstruction(){
  cout << instructionLine << "\t";
  if(!resultants.empty())
    cout << "Resultant: " << resultants[0];
  if(!dependancies.empty()){
    cout << "\tDependancies: ";
    for(string s : dependancies){
      cout << s << "\t";
    }//end for loop
  }
  if(!offsetNum.empty())
    cout << "\tExtra: " << offsetNum;
  cout << endl;

}//end printInstruction

string instruction::getInstruction(){
  return fullLine;
}


bool instruction::nextStage(int cycle){
  //switch to progress to next stage... EX and MEM stage checks if they've done a correct amount of cycles
  switch(currStage){
  case NONE:
    currStage = IF;
    isStalled = false;
    break;
  case IF:
    stageCompleted[0] = cycle;
    currStage = ID;
    isStalled = false;
    break;
  case ID:
    stageCompleted[1] = cycle;
    currStage = EX;
    isStalled = false;
    //this is the only time we do our operation as that is when we grab our parameters
    return true;
    break;
  case EX:
    //we only progress if our cycles left for exStage is 0
    if(exCyclesLeft <= 1){
      stageCompleted[2] = cycle;
      currStage = MEM;
      isStalled = false;
      /*****************/
      //IF WE ARE ARITHMETIC, SET FORWARDING AVAILIBLE
      if(instructNum > 4 && instructNum < 12){
	resultAvailible = true;
      }//end set forwarding avalible
    }//end if going to MEM stage
    //we have spent a cycle at the exStage, decrement... if 0 Cycles left still decrement since it doesn't matter no more
    exCyclesLeft--;
    break;
  case MEM:
    if(memCyclesLeft <= 1){
      stageCompleted[3] = cycle;
      currStage = WB;
      isStalled = false;
      /******************************/
      //IF WE ARE LOAD/STORE, SET FORWARDING AVAILIBLE
      if(instructNum < 5){
	resultAvailible = true;
      }//end setting forwarding avalible
    }//end go to WB stage
    memCyclesLeft--;
    break;
  case WB:
    stageCompleted[4] = cycle;
    resultAvailible = false;
    currStage = DONE;
    isStalled = false;
    break;
  case DONE:
    break;
    
  }//end switch

  //now we check if we are at stage to do operation
  return false;
}//end nextStage

void instruction::printDone(){

  cout << instructNames[instructNum] << "\t";
  for(int i = 0; i < 5; i++){
    cout << stageCompleted[i] << "\t";
  }
  cout << endl;
}//end printDone


bool instruction::isStructionDone(){
  
  //if instruction has finished all 5 stages, return true, else return false
  if(currStage == DONE)
    return true;
  
  return false;
}//end isStructionDone

//returns whether or not the instruction is actively being stalled
bool instruction::stalled(){
  return isStalled;
}

//if load/store, do operation when we finish mem stage, otherwise we do when we finish EX stage
bool instruction::doOperation(){
  //THIS SUBROUTINE HAS BECOME OBSOLETE
  if(instructNum >= 0 && instructNum <= 4 && currStage == WB)
    return true;
  if(instructNum >= 5 && instructNum <= 14 && currStage == MEM)
    return true;
  
  return false;
}

//returns a vector of the parameters in order
//I recognize there is repetition, however doing it this way allows more organization and is easier to follow
vector<string> instruction::getParameters(){

  vector<string> parameters;
  //if L.D or LW
  switch(instructNum){
  case 0: //l.D
    parameters.push_back(resultants[0]);
    parameters.push_back(offsetNum);
    parameters.push_back(dependancies[0]);
    //stores location of dependancy in parameters
    dLocation.push_back(2);
    break;
  case 1: //s.d
    parameters.push_back(dependancies[0]);
    //stores location of dependancy in parameters
    dLocation.push_back(0);
    parameters.push_back(offsetNum);
    parameters.push_back(resultants[0]);
    break;
  case 2: //LI
    parameters.push_back(resultants[0]);
    parameters.push_back(offsetNum);
    break;
  case 3://LW
    parameters.push_back(resultants[0]);
    parameters.push_back(offsetNum);
    parameters.push_back(dependancies[0]);
    //stores location of dependancy in parameters
    dLocation.push_back(2);
    break;
  case 4://SW
    parameters.push_back(dependancies[0]);
    //stores location of dependancy in parameters
    dLocation.push_back(0);
    parameters.push_back(offsetNum);
    parameters.push_back(resultants[0]);
    break;
  case 5://ADD
    parameters.push_back(resultants[0]);
    parameters.push_back(dependancies[0]);
    parameters.push_back(dependancies[1]);
    //stores location of dependancy in parameters
    dLocation.push_back(2);
    dLocation.push_back(1);
    break;
  case 6://ADDI
    parameters.push_back(resultants[0]);
    parameters.push_back(dependancies[0]);
    //stores location of dependancy in parameters
    dLocation.push_back(1);
    parameters.push_back(offsetNum);
    break;
  case 7://ADD.D
    parameters.push_back(resultants[0]);
    parameters.push_back(dependancies[0]);
    parameters.push_back(dependancies[1]);
    //stores location of dependancy in parameters
    dLocation.push_back(2);
    dLocation.push_back(1);
    break;
  case 8://SUB.D
    parameters.push_back(resultants[0]);
    parameters.push_back(dependancies[0]);
    parameters.push_back(dependancies[1]);
    //stores location of dependancy in parameters
    dLocation.push_back(2);
    dLocation.push_back(1);
    break;
  case 9://SUB
    parameters.push_back(resultants[0]);
    parameters.push_back(dependancies[0]);
    parameters.push_back(dependancies[1]);
    //stores location of dependancy in parameters
    dLocation.push_back(2);
    dLocation.push_back(1);
    break;
  case 10://MUL.D
    parameters.push_back(resultants[0]);
    parameters.push_back(dependancies[0]);
    parameters.push_back(dependancies[1]);
    //stores location of dependancy in parameters
    dLocation.push_back(2);
    dLocation.push_back(1);
    break;
  case 11://DIV.D
    parameters.push_back(resultants[0]);
    parameters.push_back(dependancies[0]);
    parameters.push_back(dependancies[1]);
    //stores location of dependancy in parameters
    dLocation.push_back(2);
    dLocation.push_back(1);
    break;
  case 12://BEQ
    parameters.push_back(dependancies[0]);
    parameters.push_back(dependancies[1]);
    parameters.push_back(offsetNum);
    //stores location of dependancy in parameters
    dLocation.push_back(0);
    dLocation.push_back(1);
    break;
  case 13://BNE
    parameters.push_back(dependancies[0]);
    parameters.push_back(dependancies[1]);
    parameters.push_back(offsetNum);
    //stores location of dependancy in parameters
    dLocation.push_back(0);
    dLocation.push_back(1);
    break;
  case 14://J
    parameters.push_back(offsetNum);
    break;

  }//end switch;
  
  return parameters;
}//end getParameters

//returns instruction num
int instruction::getInstructNum(){
  return instructNum;
}

void instruction::setResult(int r){
  result = r;
}

int instruction::getResult(){
  return result;
}


//function determines how many cycles it'll take to execute Ex and Mem stage depending on instruction type
void instruction::setCycles(){
  //mem stage takes 1 cycle unless it is a missPenalty since it wasn't in cache
  memCyclesLeft = 1;
  //EX stage takes 1 cycle unless it is FP ADDER, Mult, or DIV
  exCyclesLeft = 1;
  if(instructNum == 7 || instructNum == 8)
    exCyclesLeft = 2;
  if(instructNum == 10)
    exCyclesLeft = 10;
  if(instructNum == 11)
    exCyclesLeft = 40;
}//end setCycles

//if when going to cache and its not there, its a miss and we call this function
void instruction::missPenalty(){
  memCyclesLeft += 2;

}//end missPenalty

string instruction::getResultant(){
  //if nothing in resultants
  if(resultants.empty())
    return "";
  return resultants[0];
}//end getResultant


int instruction::whatStage(){
  if(currStage == NONE)
    return 0;
  if(currStage == IF)
    return 1;
  if(currStage == ID)
    return 2;
  if(currStage == EX)
    return 3;
  if(currStage == MEM)
    return 4;
  if(currStage == WB)
    return 5;

  return 6;
}//end whatStage


void instruction::setAvailible(bool t){
  resultAvailible = t;
}


vector<string> instruction::getDependancies(){
  return dependancies;
}


vector<int> instruction::getDLocation(){
  return dLocation;
}


//check is forwarding is availible
bool instruction::isAvailible(){
  return resultAvailible;
}


//sets current stall state of instruction
void instruction::setStall(bool s){
  isStalled = s;
}


string instruction::getOffSetNum(){
  return offsetNum;
}


vector<string> instruction::getParameters2(){
  //if we haven't called get parameters before
  if(parameters.empty()){
    parameters = getParameters();
  }//end if 
  return parameters;
}//end getParameters
