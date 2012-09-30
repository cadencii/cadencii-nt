TEST_FILES=$(shell ls *Test.cpp **/*Test.cpp 2>/dev/null)
TARGETS=$(TEST_FILES:.cpp=.exe)

all: $(TARGETS)

$(TARGETS): %.exe: %.cpp
	g++ main.cpp $< -w -ldl -lcppunit -o $@
	./$@

run: $(TARGETS)
	for TARGET in $(TARGETS); do \
	  ./$$TARGET;\
	done

clean:
	rm -f *.exe **/*.exe

