# CMPT-300-Measure-Sys-Calls-Costs
A class project for CMPT 300 in Simon Fraser University, which measures the cost of a minimal function call in C/C++ (cost in terms of time taken)

### Examples

* The minimal cost could be emulated by measuring a bare function call that neither takes any parameter nor does anything inside the function.

* Unlike a regular function call, a system call traps into the operating system kernel. The minimal cost can be emulated by measuring the cost of getpid () which doesn't really do anything.

* Measure the cost of a read/write system call in C/C++. (Hint: You can fork a child process and transfer one byte between parent and child)
