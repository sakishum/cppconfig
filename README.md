# Cppconfig


## About the name

The original name of this program was "C++ read config", but it was later changed to "Cppconfig".

Despite the name, Cppconfig is designed for C++.


## Compiling

Any C++98/C++11 compiler should work. If your compiler has the C++98/C++11 features that are available in Visual Studio 2010 then it will work. 

There are multiple compilation choices:
* g++ 4.4 (and above)
* clang++ 2.9 (and above)

### g++ (for experts)

If you just want to build Cppcheck without dependencies then you can use this command:

```shell
make all
```


### Other Compiler/IDE

1. Create a empty project file / makefile.
2. Add all cpp files in the cppcheck cli and lib folders to the project file / makefile.
3. Compile.
