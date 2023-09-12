all: avl

clean: 
	rm -f *.gcov *.gcda *.gcno a.out

avl: clean avl.h avl_tests.cpp
	g++ -std=c++17 -Wall -Wextra -Weffc++ -pedantic-errors -g --coverage avl_tests.cpp && ./a.out && gcov -mr avl_tests.cpp

avl_memory_errors: clean avl.h avl_tests.cpp
	g++ -std=c++17 -Wall -Wextra -Weffc++ -pedantic-errors -g avl_tests.cpp && valgrind --leak-check=full ./a.out
