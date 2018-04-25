CXXFLAGS += -I include -g -Wall -std=c++11

NAME = filesystem

$(NAME): src/fs.cpp $(wildcard include/*.h)
	$(CXX) $(CXXFLAGS) -o $@ $<

clean:
	$(RM) $(NAME)
	$(RM) -r *.dSYM src/*.dSYM *.out src/*.out

re: clean $(NAME)
