all: part3

part3.tab.cpp: part3.ypp part3_helpers.cpp part3_helpers.hpp
	bison -vd part3.ypp

lex.yy.c: part3.lex part3_helpers.cpp part3_helpers.hpp
	flex part3.lex 

part3: lex.yy.c part3.tab.cpp code_class_helpers.cpp part3_helpers.cpp part3_helpers.hpp symbol_table.cpp
	g++ -std=c++11 -o rx-cc part3.tab.cpp lex.yy.c part3_helpers.cpp code_class_helpers.cpp symbol_table.cpp

clean:
	rm -f *.o part3.tab.cpp lex.yy.c part3.tab.h *.gch rx-cc