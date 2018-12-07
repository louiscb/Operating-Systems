# Thread Scheduling & Execution Contexts

Two simple programs which explore the concept of an execution context and multi-threading within a single process.

Relies on the <ucontext.h> library of functions in order to save and start contexts at various stages in the programs execution.

The switch.c program visualises saving and swapping contexts inside of a program.

The yield.c program shows how a simple program can be multithreaded for two seperate stacks. How could this program be improved for more than just a couple of stacks?
