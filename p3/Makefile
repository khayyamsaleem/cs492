CXXFLAGS += -I include -g -Wall -std=c++11

NAME = filesystem

all: $(NAME)

$(NAME): src/fs.cpp
	$(CXX) $(CXXFLAGS) -o $@ $^

clean:
	$(RM) $(NAME)
	$(RM) -r *.dSYM src/*.dSYM *.out src/*.out

re: clean $(NAME)
