all: main.cpp operate_config.cpp
	$(CXX) -o test -Wall -O2 $?

.PHONY:clean
clean: test
	$(RM) $?
