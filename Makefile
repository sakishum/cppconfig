all: main.cpp operate_config.cpp
	$(CXX) -o test -Wall -O2 $?

check:
	valgrind --tool=memcheck --leak-check=yes --leak-check=full --dsymutil=yes ./test

.PHONY:clean
clean: test
	$(RM) $?
