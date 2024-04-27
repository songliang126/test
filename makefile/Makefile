CPP=g++
SRC=$(wildcard *.cpp)
OBJS=$(patsubst %.cpp, %.o, $(SRC))
DIR=$(dir ./main.cpp)
CPP_FLAGS=-std=c++11
LIBS=-lpthread
OBJ=main
#OBJS=main.o remove_if.o find_if.o thd.o
#all:main
$(OBJ):$(OBJS)
	$(CPP) -o $(OBJ) -I $(DIR) $(OBJS) $(CPP_FLAGS) $(LIBS)

%.o:%.cpp
	$(CPP) -c $^ -o $@ $(CPP_FLAGS)

#main:$(OBJS)
#	g++ -o main -I . $(OBJS) ${CPP_FLAGS} ${LIBS}

#%.o:%.cpp
#	g++ -c $< -o $@ ${CPP_FLAGS}

.PHONY:clean
clean:
	$(RM) *.o main
#	rm -rf main
#	rm -rf *.o
