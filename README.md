# termfetch-cpp
termfetch, but written in C++

**to compile:**

windres resource.rc -O coff -o resource.res && g++ termfetch.cpp -o termfetch.exe resource.res --static --std=c++20
