# termfetch-cpp
termfetch, but written in C++

to compile:

g++ termfetch.cpp -o termfetch.exe --static --std=c++20

NOTES:
in the namespace Speedtest, there's a function called (void)create_script().
this function creates a file (if not already present) and writes some code into it.
the file is called "speedtest.ps1", it is a powershell script.

this file is then read by powershell.exe, and the output is captured.
