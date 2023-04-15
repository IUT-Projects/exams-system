# Exams-System

> Exams System - oop project for second semester.

## **Note**

Main entrypoint to this project is `main.cpp`, any other classes, utility functions ... should be implemented in `/src` folder.

Steps

- Create `cpp` file in `./src` folder.
- Write, define, implement your code.
- Open `project.h` (header file), and add your cpp:

```cpp
#include "yourcode.cpp" // done
```

- Then run main.cpp

> We may create header files for definition, and cpp files for implmentation. But it requires further steps for compiling this project. It will be very mess (linking...). Just `cpp`.

## **Requirements**

- Any C++ compiler (gcc, clang)
- `Make` - if you want to use Makefile (optional)

## **Running**

Run by main.cpp

```shell
g++ main.cpp -o program.o -std=c++20 # compile
./program.o # run executable file
# one liner: g++ main.cpp -o program.o -std=c++20 && ./program.o
```

> WHY `.o` - it is standart. And if you see gitignore, here is a list of all file format that will not be uploaded to this github repository.

Run by `make`:

```shell
make run
```
