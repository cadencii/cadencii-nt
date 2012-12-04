ROOT_DIR=$(shell dirname $(realpath $(lastword $(MAKEFILE_LIST))))
TEST_FILES=$(shell find $(ROOT_DIR) | grep Test.cpp 2>/dev/null)
TARGETS=$(TEST_FILES:.cpp=.exe)

all: $(TARGETS)

$(TARGETS): %.exe: %.cpp
	g++ -g $(ROOT_DIR)/main.cpp $< -w -ldl -lcppunit -o $@
	$@

run: $(TARGETS)
	for TARGET in $(TARGETS); do \
	  ./$$TARGET;\
	done

clean:
	rm -rf $(TARGETS)

#**/*.exe.dSYM

test.exe: main.cpp $(TEST_FILES)
	g++ main.cpp $(TEST_FILES) -w -coverage -ldl -lcppunit -o test.exe

