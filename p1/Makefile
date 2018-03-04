CXXFLAGS += -I include -pthread -g -std=c++11

prodcons: src/main.cpp
	$(CXX) $(CXXFLAGS) -o $@ $^

clean:
	$(RM) prodcons
	$(RM) -r *.dSYM
