ALL = tp1
SRC = $(wildcard *.cpp)
OBJ = $(patsubst %.cpp, %.o, $(wildcard *.cpp))

CC = g++

CPPFLAGS = -g -Wall -std=c++11 -O3 -pthread

all: $(ALL)

$(ALL): $(OBJ)
	$(CC) $(CPPFLAGS) -c $(SRC)
	$(CC) $(CPPFLAGS) $(OBJ) -o $@ 

clean:
	rm $(ALL) *.o
