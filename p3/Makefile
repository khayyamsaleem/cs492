CXXFLAGS += -I include -O2 -g -std=c++11

NAME = filesystem

$(NAME): src/fs.cpp
	$(CXX) $(CXXFLAGS) -o $@ $^

clean:
	$(RM) $(NAME)
	$(RM) -r *.dSYM src/*.dSYM *.out src/*.out

re: clean $(NAME)
