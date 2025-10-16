CXX = g++
CXXFLAGS = -Wall

main: main.cpp instruction.o memory.o processor.o
	$(CXX) $(CXXFLAGS) main.cpp  instruction.o memory.o processor.o -o main

processor.o: processor.h processor.cpp memory.o instruction.o
	$(CXX) $(CXXFLAGS) -c processor.cpp

memory.o: memory.h memory.cpp instruction.o
	$(CXX) $(CXXFLAGS) -c memory.cpp


instruction.o: instruction.h instruction.cpp
	$(CXX) $(CXXFLAGS) -c instruction.cpp

run:
	./main


clean:
	rm *~
	rm *.o
