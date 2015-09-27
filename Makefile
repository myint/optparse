WARN_FLAGS = -O3 -g -Wall -Wextra -Wabi -Wctor-dtor-privacy -Wnon-virtual-dtor -Wreorder -Woverloaded-virtual -Wshadow -Wcast-align -Wpointer-arith -Wwrite-strings -Wundef -Wredundant-decls -Werror # -Weffc++

BIN = test
OBJECTS = option_parser.o test.o

$(BIN): $(OBJECTS)
	$(CXX) -o $@ $(OBJECTS) $(WARN_FLAGS)

%.o: %.cpp option_parser.h
	$(CXX) $(WARN_FLAGS) -c $< -o $@

.PHONY: clean

clean:
	rm -f *.o $(BIN)
