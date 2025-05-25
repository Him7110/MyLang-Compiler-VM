# Makefile for MyLang on Windows (force console subsystem)
CXX       := g++
CXXFLAGS  := -std=c++17 -O2
LDFLAGS   := -mconsole

SRCS := \
    src/AST.cpp   \
    src/Lexer.cpp \
    src/Parser.cpp\
    src/CodeGen.cpp\
    src/VM.cpp    \
    src/main.cpp

all: MyLang.exe

MyLang.exe: $(SRCS)
	$(CXX) $(CXXFLAGS) $(SRCS) -o MyLang.exe $(LDFLAGS)

clean:
	del MyLang.exe 2>nul
