TEST_FILES=$(shell find . | grep Test.cpp 2>/dev/null)
TARGETS=$(TEST_FILES:.cpp=.exe)

all: $(TARGETS)

$(TARGETS): %.exe: %.cpp
	g++ -g main.cpp $< -w -ldl -lcppunit -o $@
	./$@

run: $(TARGETS)
	for TARGET in $(TARGETS); do \
	  ./$$TARGET;\
	done

clean:
	rm -rf *.exe *.exe.dSYM **/*.exe **/*.exe.dSYM

