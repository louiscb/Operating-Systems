#The Process

A process is the major OS abstraction of a running program. 

Time Sharing of the CPU allows users to run as many concurrent processes as they would like by promoting the illusion that there exist infinite virtual CPUS. The cost is performance as each process runs more slowly if the CPU is shared.

Space Sharing where a resource is divided in space among those who wish to use it. E.g disk space.

Process API:
- Create: OS must be able to create new processes
- Destroy: Forcefully be able to destroy a process 
- Wait: It is useful to wait for a process to stop running
- Miscellaneous Control: E.g suspend process
- Status: Get status information on the running of a process

C programs use the stack for local variables, function parameters and return addresses.
C programs use the heap for explicitly requested dynamically allocated data. Programs request this free space with malloc().

Steps for running a program done by OS:
- Loading the code and static data into memory
- Creating & initialising stack
- Other work related to I/O setup
- Start program running at entry point, main()

A process has three states that it transitions between:
1. Running - currently executing instructions
2. Ready - obvious
3. Blocked - not ready until some other event takes place
￼
A process list contains information about all processes in the system - sometimes they are stored as entries in a process control block