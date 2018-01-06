WARN_FLAGS = \
	-std=c++98 -O3 -g -Wall -Wextra -Wabi -Wctor-dtor-privacy \
	-Wnon-virtual-dtor -Wreorder -Woverloaded-virtual -Wshadow \
	-Wcast-align -Wpointer-arith -Wwrite-strings -Wundef \
	-Wredundant-decls -Werror

all: test test_no_intersperse

test: test.cpp optparse.h
	$(CXX) $(WARN_FLAGS) -fsanitize=address,undefined$< -o $@

test_no_intersperse: test.cpp optparse.h
	$(CXX) $(WARN_FLAGS) -DDISABLE_INTERSPERSED_ARGS=1 $< -o $@

# Check includes in multiple files.
check:
	echo '#include "optparse.h"' > tmp_second.cpp
	$(CXX) -o tmp_test test.cpp tmp_second.cpp
	rm tmp_second.cpp
	./tmp_test
	rm tmp_test

clean:
	rm -f *.o $(BIN)

.PHONY: check clean
