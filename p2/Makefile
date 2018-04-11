CXXFLAGS += -I include -O2 -g -std=c++11

VMsimulator: src/vmsim.cpp
	$(CXX) $(CXXFLAGS) -o $@ $^

clean:
	$(RM) VMsimulator
	$(RM) -r *.dSYM
	$(RM) *.out
