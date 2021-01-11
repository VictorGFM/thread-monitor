ALL = tp1
SRC = $(wildcard *.cpp */*.cpp)
OBJ = $(patsubst %.cpp, %.o, $(SRC))

CC = g++

CPPFLAGS = -g -Wall -std=c++11 -O3 -lpthread

all: $(ALL)

$(ALL): $(OBJ)
	$(CC) $(CPPFLAGS) -c $(SRC)
	$(CC) $(CPPFLAGS) $(OBJ) -o $@ 

clean:
	rm $(ALL) *.o

