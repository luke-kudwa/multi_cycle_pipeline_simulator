#include "memory.h"

memory::memory(){

  //nothing happens here at this moment

  for(int i = 0; i < 8; i++){
    cacheSet1[i] = -9999;
    cacheSet2[i] = -9999;
  }
  
}//end constructor

int memory::doOperation(int num, vector<string> parameters){
  int intermediateResult;
  //this value determines whether to add 2 cycles to operation, only touched if it accesses memory
  inCache = true;
  
  //switch to call correct function to do correct operation, each one should return an int that this function returns
  switch(num){
  case 0://L.D
    intermediateResult = instruction0(num, parameters);
    break;
  case 1://S.D
    intermediateResult = instruction1(num, parameters);
    break;
  case 2://LI
    intermediateResult = instruction2(num, parameters);
    break;
  case 3://LW
    intermediateResult = instruction3(num, parameters);
    break;
  case 4://SW
    intermediateResult = instruction4(num, parameters);
    break;
  case 5://ADD
    intermediateResult = instruction5(num, parameters);
    break;
  case 6://ADDI
    intermediateResult = instruction6(num, parameters);
    break;
  case 7://ADD.D
    intermediateResult = instruction7(num, parameters);
    break;
  case 8://SUB.D
    intermediateResult = instruction8(num, parameters);
    break;
  case 9://SUB
    intermediateResult = instruction9(num, parameters);
    break;
  case 10://MUL.D
    intermediateResult = instruction10(num, parameters);
    break;
  case 11://DIV.D
    intermediateResult = instruction11(num, parameters);
    break;
  case 12://BEQ
    intermediateResult = instruction12(num, parameters);
    break;
  case 13://BNE
    intermediateResult = instruction13(num, parameters);
    break;
  case 14://J
    intermediateResult = instruction14(num, parameters);
    break;
  }  

  return intermediateResult;
}


int memory::instruction0(int num, vector<string> parameters){//L.D
  //Load will take the FP value from memory and store into FP reg

  //NOTE THIS WILL GET CHANGED WHEN I START INTRODUCING GOING TO CACHE
  int memValue = getValueFromMem(parameters[1], parameters[2]);
  //int fpAddress = getFPRegAddress(parameters[0]);

  //fpRegisters[fpAddress] = memValue;

  return memValue;
}


int memory::instruction1(int num, vector<string> parameters){//S.D
  //we are storing a value from a FP reg into memory

  //gets address using subroutine to get the value from correct FP Register
  int value = fpRegisters[getFPRegAddress(parameters[0])];

  //now we need to store value in memory
  //storeValueInMemory(parameters[1], parameters[2], value);
  
  return value;
}


int memory::instruction2(int num, vector<string> parameters){//LI
  //int p1 = getIntRegAddress(parameters[0]);
  int p2 = stoi(parameters[1]);
  //intRegisters[p1] = p2;
  
  return p2;
}


int memory::instruction3(int num, vector<string> parameters){//LW
  //Load will take the int value from memory and store into int reg

  //gets value from memory
  int memValue = getValueFromMem(parameters[1], parameters[2]);
  //gets integer reg we store to
  //int intAddress = getIntRegAddress(parameters[0]);
  //stores the value
  //intRegisters[intAddress] = memValue;

  //returns value stored
  return memValue;
}//end instruction3


int memory::instruction4(int num, vector<string> parameters){//SW
  //we are storing a value from an Int reg into memory

  //gets address using subroutine to get the value from correct Int Register
  int value = intRegisters[getIntRegAddress(parameters[0])];

  //now we need to store value in memory
  //storeValueInMemory(parameters[1], parameters[2], value);

  return value;
}//end instruction4


int memory::instruction5(int num, vector<string> parameters){//ADD
  
  //Add value in int reg2 and int reg1 and store in int reg0
  //get values from registers
  int reg2 = intRegisters[getIntRegAddress(parameters[2])];
  int reg1 = intRegisters[getIntRegAddress(parameters[1])];
  //get address to store to
  //int reg0Address = getIntRegAddress(parameters[0]);
  int value = reg2 + reg1;
  //after adding the nums, store the value
  //intRegisters[reg0Address] = value;
  
  return value;
}//end instruction5


int memory::instruction6(int num, vector<string> parameters){//ADDI
  //converts string to int for the immediate
  int reg2 = stoi(parameters[2]);
  int reg1 = intRegisters[getIntRegAddress(parameters[1])];

  //get address to store to
  //int reg0Address = getIntRegAddress(parameters[0]);
  int value = reg2 + reg1;
  //after adding the nums, store the value
  //intRegisters[reg0Address] = value;

  return value;
}//end instruction6


int memory::instruction7(int num, vector<string> parameters){//ADD.D
  //Add value in int reg2 and int reg1 and store in int reg0
  //get values from registers
  int reg2 = fpRegisters[getFPRegAddress(parameters[2])];
  int reg1 = fpRegisters[getFPRegAddress(parameters[1])];
  //get address to store to
  //int reg0Address = getFPRegAddress(parameters[0]);
  int value = reg2 + reg1;
  //after adding the nums, store the value
  //intRegisters[reg0Address] = value;

  return value;  
}//end instruction7


int memory::instruction8(int num, vector<string> parameters){//SUB.D
  //Sub value in int reg2 and int reg1 and store in int reg0
  //get values from registers
  int reg2 = fpRegisters[getFPRegAddress(parameters[2])];
  int reg1 = fpRegisters[getFPRegAddress(parameters[1])];
  //get address to store to
  //int reg0Address = getFPRegAddress(parameters[0]);
  int value = reg1 - reg2;
  //after subtracting the nums, store the value
  //intRegisters[reg0Address] = value;

  return value;
}


int memory::instruction9(int num, vector<string> parameters){//SUB
  //Sub value in int reg2 and int reg1 and store in int reg0
  //get values from registers
  int reg2 = intRegisters[getIntRegAddress(parameters[2])];
  int reg1 = intRegisters[getIntRegAddress(parameters[1])];
  //get address to store to
  //int reg0Address = getIntRegAddress(parameters[0]);
  int value = reg1 - reg2;
  //after subtracting the nums, store the value
  //intRegisters[reg0Address] = value;

  return value;
}


int memory::instruction10(int num, vector<string> parameters){//MUL.D
  //Add value in int reg2 and int reg1 and store in int reg0
  //get values from registers
  int reg2 = fpRegisters[getFPRegAddress(parameters[2])];
  int reg1 = fpRegisters[getFPRegAddress(parameters[1])];
  //get address to store to
  //int reg0Address = getFPRegAddress(parameters[0]);
  int value = reg2 * reg1;
  //after adding the nums, store the value
  //fpRegisters[reg0Address] = value;

  return value;
 
}


int memory::instruction11(int num, vector<string> parameters){

  //Add value in int reg2 and int reg1 and store in int reg0
  //get values from registers
  float reg2 = fpRegisters[getFPRegAddress(parameters[2])];
  float reg1 = fpRegisters[getFPRegAddress(parameters[1])];
  //get address to store to
  //int reg0Address = getFPRegAddress(parameters[0]);
  float value = reg1 / reg2;
  //after adding the nums, store the value
  //fpRegisters[reg0Address] = value;
  
  return value;
}


int memory::instruction12(int num, vector<string> parameters){
  cout << "BEQ: " << parameters[0] << " " << parameters[1] << "(" << parameters[2] << ") - memObj" << endl;
  return -13;
}


int memory::instruction13(int num, vector<string> parameters){
  cout << "BNE: " << parameters[0] << " " << parameters[1] << "(" << parameters[2] << ") - memObj" << endl;
  return -14;
}


int memory::instruction14(int num, vector<string> parameters){
  cout << "J: " << parameters[0] << endl;
  return -15;
}

int memory::getIntRegAddress(string str){
  //find which register address it is, looking through Int Registers
  for(int i = 0; i < 32; i++){
    if(intRegNames[i] == str)
      return i;
  }//end for loop
  //it should never return -1!!!!!!
  return -1;
}//end getIntRegAddress

int memory::getFPRegAddress(string str){
  //find which register address it is, looking through Int Registers
  for(int i = 0; i < 32; i++){
    if(fpRegNames[i] == str)
      return i;
  }//end for loop
  //it should never return -1!!!!!!
  return -1;
}//end getFPRegAddress

void memory::printIntRegisters(){
  cout << "\nINTEGER REGISTERS: " << endl;
  for(int i = 0; i < 32; i++){
    cout << "$" << i << ": " << intRegisters[i] << endl;
  }//end for loop
}//end printRegisters

void memory::printFPRegisters(){
  cout << "\nFP REGISTERS: " << endl;
  for(int i = 0; i < 32; i++){
    cout << "F" << i << ": " << fpRegisters[i] << endl;
  }//end for loop
}//end printRegisters


void memory::printMemory(){
  cout << "\nMain Memory: " << endl;
  for(int i = 0; i < 19; i++){
    cout << "Mem index: " << i << "\tValue: " << memValues[i] << endl;
  }//end for loop
}//end print registers


void memory::printCache(){
  cout << "\nCache: \tSet1, Set2" << endl;
  for(int i = 0; i < 8; i++){
    cout << "Block: " << i << "\t" << cacheSet1[i] << "    " << cacheSet2[i] << endl;
  }//end for loop
}//end printCache



int memory::getValueFromMem(string offset, string address){
  //first get value from int register
  int memAddress = intRegisters[getIntRegAddress(address)];
  memAddress += stoi(offset);

  //first chech if memAddress is stored in cache1
  for(int i = 0; i < 8; i++){
    //if cache has memaddress, we have a hit
    if(cacheSet1[i] == memAddress){
      cout << "We have a hit" << endl;
      inCache = true;
      return cacheSet2[i];
    }//end if
  }//end for loop

  //if we reach he, the value isn't in cache, now we have set bool inCache to false since it was a miss
  inCache = false;

  //find value in memory
  int memValue = memValues[memAddress];
  //check if spot in cache is being replaced, if so... update it in memory
  if(cacheNextAccess > 7)
    updateMemory(cacheNextAccess % 8);

  //bring memory to cache
  cacheSet1[cacheNextAccess % 8] = memAddress;
  cacheSet2[cacheNextAccess % 8] = memValue;
  //iterate to next spot in block to be replaced
  cacheNextAccess++;

  
  return memValue;
}//end get value from memory


void memory::updateMemory(int address){
  int addressToGoTo;
  int valueToStore;
  //mem address we go to
  addressToGoTo = cacheSet1[address];
  //value we store in memory
  valueToStore = cacheSet2[address];

  //update memory
  memValues[addressToGoTo] = valueToStore;

}//updateMemory

void memory::storeValueInMemory(string offset, string address, int value){
  int memAddress;
  //if the address is to a register
  if(address.substr(0, 1) == "$"){
    memAddress = intRegisters[getIntRegAddress(address)];
  } else{//else we must already have the value at the register
    memAddress = stoi(address);
  }
  memAddress += stoi(offset);

  //now we need to check if the cache has that address
  //first chech if memAddress is stored in cache1
  for(int i = 0; i < 8; i++){
    //if cache has memaddress, we have a hit
    if(cacheSet1[i] == memAddress){
      inCache = true;
      //now we need to store the value
      cacheSet2[i] = value;
      //exit subroutine since we have stored the value
      return;
    }//end if
  }//end for loop

  //since mem address isn't in cache, we need to put it there
  inCache = false;

  //clear space in cache for memory stuff
  if(cacheNextAccess > 7)
    updateMemory(cacheNextAccess % 8);
  //store value into cache spot
  cacheSet1[cacheNextAccess] = memAddress;
  cacheSet2[cacheNextAccess] = value;
  cacheNextAccess++;
  
}//end storeValueInMemory

//helper function
bool memory::isInCache(){
  return inCache;
}

void memory::writeBack(string resultant, int value, int iNum, string offset){
  //first we need to figure out where the value is going to go

  //if S.D or SW
  if(iNum == 1 || iNum == 4){
    storeValueInMemory(offset, resultant, value);
  }//end if
  
  //if LI, LW, ADD, ADD.I, SUB
  //2 3 5 6 9
  if(iNum == 2 || iNum == 3 || iNum == 5 || iNum == 6 || iNum == 9){
    int address = getIntRegAddress(resultant);
    intRegisters[address] = value;
  }//end if
  
  //if L.D, ADD.D, SUB.D, MUL.D, DIV.D
  // 0 7 8 10 11
  if(iNum == 0 || iNum == 7 || iNum == 8 || iNum == 10 || iNum == 11){
    int address = getFPRegAddress(resultant);
    fpRegisters[address] = value;
  }//end if
  
}//end writeBack
