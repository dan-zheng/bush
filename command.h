#include <vector>

#ifndef COMMAND_H_
#define COMMAND_H_

class SimpleCommand {
public:

	std::vector<char*>  	 *args;

	SimpleCommand();
	~SimpleCommand();

	void print();

	void push(char*);

	static SimpleCommand *current;
};

class CompoundCommand {
public:

	std::vector<SimpleCommand*>  *args;

	int								bg;
	int								append;

	char			 			 *in;
	char			 			 *out;
	char			 			 *err;

	CompoundCommand();
	~CompoundCommand();

	void print();
	void execute();
	void clear();

	void push(SimpleCommand*);

	static CompoundCommand *current;
};

#endif
