
g++.exe -w -g -c main.cpp -o main.o
g++.exe -o main.exe main.o -lfreeglut -lopengl32 -lglu32
./main.exe
