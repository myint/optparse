WARN_FLAGS = \
	-std=c++98 -O3 -g -Wall -Wextra -Wabi -Wctor-dtor-privacy \
	-Wnon-virtual-dtor -Wreorder -Woverloaded-virtual -Wshadow \
	-Wcast-align -Wpointer-arith -Wwrite-strings -Wundef \
	-Wredundant-decls -Werror

BIN = test
OBJECTS = test.o

$(BIN): $(OBJECTS)
	$(CXX) -o $@ $(OBJECTS) $(WARN_FLAGS)

%.o: %.cpp optparse.h
	$(CXX) $(WARN_FLAGS) -c $< -o $@

# Check includes in multiple files.
check: test.o
	echo '#include "optparse.h"' > tmp_second.cc
	$(CXX) -o tmp_test test.o tmp_second.cc
	rm tmp_second.cc
	./tmp_test
	rm tmp_test

clean:
	rm -f *.o $(BIN)

.PHONY: check clean
