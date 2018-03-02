CPPFLAGS += -I include

SRC=src/main.cpp #\
#    src/consumer.cpp \
#    src/producer.cpp \
#    src/function.cpp

OBJ=$(SRC:.c=.o)

OUT=prodcons


all: $(OBJ)
	g++ $(CPPFLAGS) $(OBJ) -o $(OUT) -lpthread

clean:
	$(RM) $(OUT)
