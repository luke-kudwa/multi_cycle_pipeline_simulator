#ifndef MEMORY_H
#define MEMORY_H

#include <iostream>
#include <string>
#include <vector>

using namespace std;

class memory{
public:
  memory();
  int doOperation(int num, vector<string> parameters);
  int instruction0(int num, vector<string> parameters);
  int instruction1(int num, vector<string> parameters);
  int instruction2(int num, vector<string> parameters);
  int instruction3(int num, vector<string> parameters);
  int instruction4(int num, vector<string> parameters);
  int instruction5(int num, vector<string> parameters);
  int instruction6(int num, vector<string> parameters);
  int instruction7(int num, vector<string> parameters);
  int instruction8(int num, vector<string> parameters);
  int instruction9(int num, vector<string> parameters);
  int instruction10(int num, vector<string> parameters);
  int instruction11(int num, vector<string> parameters);
  int instruction12(int num, vector<string> parameters);
  int instruction13(int num, vector<string> parameters);
  int instruction14(int num, vector<string> parameters);
  int getIntRegAddress(string str);  
  int getFPRegAddress(string str);
  void printIntRegisters();
  void printFPRegisters();
  void printMemory();
  void printCache();
  int getValueFromMem(string offset, string address);
  void updateMemory(int address);
  void storeValueInMemory(string offset, string address, int value);
  bool isInCache();
  void writeBack(string resultant, int value, int iNum, string offset);

  
private:
  //string of every possible instruction to account for
  vector<string> instructionNames = {"L.D", "S.D", "LI", "LW", "SW", "ADD", "ADDI", "ADD.D",
				  "SUB.D", "SUB", "MUL.D", "DIV.D", "BEQ", "BNE", "J"};
  //creates the 32 FP registers
  int intRegisters[32] = {0};
  //creates the corrseponding name for each register, they share same index as array ^^
  string intRegNames[32] = {"$0", "$1","$2","$3","$4","$5","$6","$7","$8","$9","$10","$11",
			    "$12","$13","$14","$15","$16","$17","$18","$19","$20","$21",
			    "$22","$23","$24","$25","$26","$27","$28","$29","$30","$31"};
  

  //creates the 32 FP registers
  float fpRegisters[32] = {0.0};
  string fpRegNames[32] = {"F0", "F1","F2", "F3","F4","F5","F6","F7","F8","F9","F10","F11",
			   "F12","F13","F14","F15","F16","F17","F18","F19","F20","F21",
			   "F22","F23","F24","F25","F26","F27","F28","F29","F30","F31"};

  //main memory
  int memValues[19] = {45, 12, 0, 92, 10, 135, 254, 127, 18, 4, 55, 8, 2, 98, 13, 5, 233, 158, 167};

  //cache: Set 1 will hold the memory address, Set 2 will hold the value to be stored
  int cacheSet1[8] = {-9999};
  int cacheSet2[8] = {-9999};
  //when going to memory, this value is set depending on whether or not it is in the cache
  bool inCache = true;

  
  //when we move something from memory to cache, we use this index value
  int cacheNextAccess = 0;
  
};
#endif
