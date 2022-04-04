# termfetch-cpp
termfetch, but written in C++

# compiling

```shell
windres resource.rc -O coff -o resource.res && g++ termfetch.cpp -o termfetch resource.res --static --std=c++20
```

