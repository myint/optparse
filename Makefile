WARN_FLAGS = -O3 -g -Wall -Wextra -Wabi -Wctor-dtor-privacy -Wnon-virtual-dtor -Wreorder -Woverloaded-virtual -Wshadow -Wcast-align -Wpointer-arith -Wwrite-strings -Wundef -Wredundant-decls -Werror # -Weffc++

BIN = test
OBJECTS = optparse.o test.o

$(BIN): $(OBJECTS)
	$(CXX) -o $@ $(OBJECTS) $(WARN_FLAGS)

%.o: %.cpp optparse.h
	$(CXX) $(WARN_FLAGS) -c $< -o $@

.PHONY: clean

clean:
	rm -f *.o $(BIN)
