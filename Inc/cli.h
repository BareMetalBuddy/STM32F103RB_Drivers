#ifndef CLI_H_
#define CLI_H_

#define MAX_CMDLINE_ARGS 5
#define MAX_CMDLINE_LENGTH 50

typedef void (*command_fn)(int argc, char *argv[]);

typedef struct {
	char* name;
    command_fn function;
} Command;



#endif /* CLI_H_ */
