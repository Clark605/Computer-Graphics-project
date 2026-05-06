g++.exe -w -g -std=c++17 -I./src -c main.cpp -o main.o

g++.exe -w -g -std=c++17 -I./src -c src/app/game_loop.cpp -o game_loop.o

g++.exe -w -g -std=c++17 -I./src -c src/gameplay/arena.cpp -o arena.o

g++.exe -w -g -std=c++17 -I./src -c src/gameplay/snake.cpp -o snake.o

g++.exe -w -g -std=c++17 -I./src -c src/gameplay/food.cpp -o food.o

g++.exe -w -g -std=c++17 -I./src -c src/input/controls.cpp -o controls.o

g++.exe -w -g -std=c++17 -I./src -c src/render/camera.cpp -o camera.o

g++.exe -w -g -std=c++17 -I./src -c src/render/renderer.cpp -o renderer.o

g++.exe -o main.exe main.o game_loop.o arena.o snake.o food.o controls.o camera.o renderer.o -lfreeglut -lopengl32 -lglu32 -lole32 -lwindowscodecs -luuid

./main.exe
