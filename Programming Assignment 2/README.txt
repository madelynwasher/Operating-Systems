This program demonstrates the use of the Banker's Algorithm for deadlock avoidance in a system with multiple processes and resources. The program takes input from the input file, which contains information on 5 processes and resources, and uses the Banker's Algorithm to determine whether or not the system is in a safe state. If the system is in a safe state, the program determines a safe sequence of process executions to avoid deadlock.

The program determines that the system given in the input file is in a safe state and concludes that the safe sequence is P1 -> P3 -> P0 -> P2 -> P4.

To run the code, use the following commands:
$ g++ bankersAlgorithm.cpp -o bankersAlgorithm
$ ./bankersAlgorithm
