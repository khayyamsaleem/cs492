CPPFLAGS += -I include -pthread -g -std=c++11

SRC=src/main.cpp #\
#    src/function.cpp

OBJ=$(SRC:.c=.o)

OUT=prodcons


all: $(OBJ)
	$(CXX) $(CPPFLAGS) $(OBJ) -o $(OUT)

clean:
	$(RM) $(OUT)
