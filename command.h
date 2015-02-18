#include <vector>

#ifndef COMMAND_H_
#define COMMAND_H_

class SimpleCommand {
public:

	std::vector<char*>  	 *args;

	SimpleCommand(void);
	~SimpleCommand(void);

	void pushArgument(char*);

	static SimpleCommand *current;
};

class CompoundCommand {
public:

	std::vector<SimpleCommand*>  *args;

	int								bg;
	int								nf;

	char			 			 *in;
	char			 			 *out;
	char			 			 *err;

	CompoundCommand(void);
	~CompoundCommand(void);

	void print(void);
	void execute(void);
	void clear(void);

	void pushArgument(SimpleCommand*);

	static CompoundCommand *current;
};

#endif
