CXXFLAGS += -I include -pthread -O2 -g -std=c++11

prodcons: src/main.cpp
	$(CXX) $(CXXFLAGS) -o $@ $^

clean:
	$(RM) prodcons
	$(RM) -r *.dSYM
