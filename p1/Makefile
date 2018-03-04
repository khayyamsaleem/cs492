CPPFLAGS += -I include -pthread
CXX=g++

SRC=src/main.cpp #\
#    src/function.cpp

OBJ=$(SRC:.c=.o)

OUT=prodcons


all: $(OBJ)
	$(CXX) $(CPPFLAGS) $(OBJ) -o $(OUT)

clean:
	$(RM) $(OUT)
