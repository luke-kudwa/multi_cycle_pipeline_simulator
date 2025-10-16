# multi_cycle_pipeline_simulator
Simulator models MIPS instruction execution, achieving functional equivalence to real hardware behavior as validated through unit testing, clock synchronization, processor and L1 cache integration with efficient memory access.

HOW TO COMPILE AND RUN PROGRAM:
1) Download files and open folder containing files in a terminal
2) Using the make file, type "make"
   - It should automatically compile every file and generate an executable file
3) Type "make run" to run program

NOTE: If for whatever reason the file doesn't compile automatically into an executable when you type "make"
      Please type
      1) make instruction.o
      2) make memory.o
      3) make processor.o
      4) make main
      5) make run


HOW TO RUN SIMULATOR:
- I have a txt file called input... Input.txt
  Using that text file please put the MIPS code in there! It is to be 1 instruction for line... However branches are to be read like

loop: L.D F0, 0($2)

The normal version without the branch would be

L.D F0, 0($2)


SUCCESS AND PROBLEMS WITH CODE:
  - I have run into some logic errors when writing my program... This here will detail the following list
  1) The pipelining works very well! I can have my stalls working, the only problem I have is the full forwarding... sometimes it works and sometimes it doesn't... Haven't figured that out
  2) Branches: I haven't gotten everything else to work yet... Thus my program isn't branch compatible yet
  3) The program does all the operations except for BNE, BEQ and J.
  4) The program can pipeline correctly... there is inconsistency with the full forwarding part
  5) program stalls when needed... ex/ 2 instructions tryna be in memory stage

HOW DOES THE PROCESSOR FUNCTION?

The intention behind the processor is to have a list of the instructions in order... and a pointer to the next instruction I am to read.
My processor will read 1 instruction at a time and then point to the next instruction. In a pipeline format it will complete the instructions
1 stage at a time.

When we grab an instruction we send it to the IF stage. On the next cycle it'll progress to the ID stage. When in the ID stage we check if we
have dependencies a previous instruction is using. If it is still being used we stall until full forwarding is availible and use it. However you will notice that this doesn't work fully as intended for my program... sometimes it does and sometimes it doesn't it seems.
Then in the EX stage if we are a arithemetic instruction we will execute and set full forwarding availible as we progress to the MEM stage...
In the mem stage if it is a load/store we check if we have a missPenalty and take those extra cycles. We also make sure that a previous
instruction isn't in the mem stage as well, if so we stall. Otherwise we progress to the WB stage.
In the WB stage we write information back to the registers or memory and put the instruction in a completed state.

The design of my program is the main file will read and create instructions and pass them to the processor. The processor will pipeline the
instructions while communicating with memory. Unfortunately I have the memory object to the operations since I can easily access memory
variables there and to not overload the processor with functions... I tried to keep it neater that way.
