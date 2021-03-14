CC=g++
NVCC=nvcc
CUDA_TOP=/usr/local/cuda
INC=-I$(CUDA_TOP)/include -I../cuMat2
LIB=-L$(CUDA_TOP)/lib64 -L../cuMat2 -lcublas -lcblas -lcudart -lcudnn -lm -lcumat -lcusparse -lboost_serialization -lmecab -lboost_system -lpng
#OTHER_OPTS=-std=c++11 -pthread -O2 -fpermissive
OTHER_OPTS=-std=c++11 -g -pg -pthread 

test: test.o variable.o function.o optimizer.o graph.o
	$(CC) -o test test.o variable.o function.o optimizer.o graph.o $(INC) $(LIB) $(OTHER_OPTS)

graph.o: graph.cpp
	$(CC) $(INC) $(OTHER_OPTS) $(LIB) -c graph.cpp

variable.o: variable.cpp
	$(CC) $(INC) $(OTHER_OPTS) $(LIB) -c variable.cpp

function.o: function.cpp
	$(CC) $(INC) $(OTHER_OPTS) $(LIB) -c function.cpp

optimizer.o: optimizer.cpp
	$(CC) $(INC) $(OTHER_OPTS) $(LIB) -c optimizer.cpp

test.o: test.cpp
	$(CC) -c test.cpp $(INC) $(OTHER_OPTS) $(LIB)


clean:
	rm -f test
	rm -f *.o

