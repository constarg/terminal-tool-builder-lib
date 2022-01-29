#ifndef TOOL_BUILDER_H
#define TOOL_BUILDER_H

#include <malloc.h>
#include <string.h>

#define TOOL_BUILDER_V 	1.0

// errors
#define WRONG_ARG_NUM			-1
#define WRONG_NAME_OR_ALIAS		-2

#define EMPTY_NAME			-3

#define NO_ACTION_DEFINED		-4

#define FAILED_TO_ADD			-5
#define BUILDER_IS_NOT_INITIALIZED	-6

#define NO_SUCH_COMMAND_EXISTS		-7

#define UNITIALIZED_HELP		-8


struct command_help 
{
	char *c_name;						// The name of the command.
	char *c_description;					// The description of the command.
};

struct help_d 
{
	char *h_usage_sec;					// The useage section. example: Usage: tool_name [OPTION]...
	char *h_description;					// The description of the help.
	struct command_help **h_commands;			// The commands of the help.
	char *h_close_description;				// The closure description.
	int h_argc;						// The number of commands in help.
};

struct builder_d 
{
	struct command_d **b_commands;				// Null terminated array of commands.
	int b_commandsc;					// The number of the commands.
	struct help_d *b_help;					// The help of the tool.
};

struct exec_info 
{
	char c_name[256];					// The name of the command that has been executed.
	char c_used_alias[256];					// The alias that has been used.
	char **c_values;					// The values that the been retrieved. Must be freed when there is no more use.
	int c_argc;						// The arguments of the command.
	struct builder_d *c_builder;				// The builder.
};

struct command_d 
{
	char c_name[256];					// The name of the command.
	int c_argc;						// How many argcs the command require.
	char c_alias[5][256];					// The alias of the commnad. 5 is the maximum number of alias.
	/**
		This callback is user defined and is executed
		when the user request a call of the specific
		command defined in @name field from terminal.
		@param value The parameters that have been
		retrieved from the terminal when the command
		was called.
	*/
	void (*c_call_back)(const struct exec_info *info);	// The action to take when the command has been requested.
};



/**
	Initialize the builder to be used for the tool.
	It must be checked if the builder is null. If the builder
	is null then it means that there is an error on memory.
	allocation and must stop the flow of the program. When
	there is no more use of the builder, the destroy_builder
	function must be collled.
	@param c_builder The builder.
*/
static inline void initialize_builder(struct builder_d **c_builder)
{
	*c_builder = (struct builder_d *) malloc(sizeof(struct builder_d));
	(*c_builder)->b_commands = (struct command_d **) calloc(1, sizeof(struct command_d **));
	(*c_builder)->b_help = (struct help_d *) calloc(1, sizeof(struct help_d));
	(*c_builder)->b_commandsc = 0;
}

/**
	Initialize the alias with zeros in order to avoid
	problems with unitialized values. The use of this	
	function is optional, but is good practice and
	safer to initialize the alias before use them later.
*/
static inline void initialize_alias(char (*c_alias)[5][256])
{
	for (int c_a = 0; c_a < 5; c_a++)
		memset((*c_alias)[c_a], 0x0, 255);
}

/**
	Free the memory that has been allocated for the builder.
	@param c_builder The builder to destroy.
*/
static inline void destroy_builder(struct builder_d **c_builder)
{
	// Free the memory for the commands.
	for (int c = 0; c < (*c_builder)->b_commandsc; c++) free((*c_builder)->b_commands[c]);
	free((*c_builder)->b_commands);
	
	// Free help.
	for (int h = 0; h < (*c_builder)->b_help->h_argc; h++) 
	{
		free((*c_builder)->b_help->h_commands[h]->c_name)
		free((*c_builder)->b_help->h_commands[h]->c_description);
		free((*c_builder)->b_help->h_commands[h]);
	}	
	free((*c_builder)->b_help->h_usage_sec);
	free((*c_builder)->b_help->h_close_description);
	free((*c_builder)->b_help->h_description);
	free((*c_builder)->b_help);

	// Free builder.
	free(*c_builder);
	*c_builder = NULL;
}	



/**
	Builds a basic help action that prints informations
	about the tool.
	@param tool_name The name of the tool.
	@param c_builder The builder to add the help.
	@return 0 on success or an integer error number on error.
	errors:
`		builder is not initialized: -6
		failed to add: -5	
	all the erros are defined as MACROS.

*/
extern int initialize_help(struct builder_d *c_builder, const char *tool_name);

/**
	Adds user defined format of help.
	If this function is used. Then the initialize_help
	function must be ignored.
	@param c_builder The builder to add the help.
	@param t_help	 The full help.
	@return 0 on success or an integer error number on error.
	errors:
`		builder is not initialized: -6
		unitialized help: -7
	all the erros are defined as MACROS.

*/
extern int add_full_help(struct builder_d *c_builder, const struct help_d *t_help);

/**
	Adds description to the help message.
	@param description The description to be added.
	@param c_builder The builder to add the description.
*/
extern void add_help_tool_description(struct builder_d *c_builder, const char *description);

/**
	Adds to the help action a command and the description
	of the command.
	@command_name The name of the command to add.
	@command_description The description of the command.
	@c_builder The builder to add the command and command description.
*/
extern void add_help_tool_command(struct builder_d *c_builder, const char *command_name, 
				  const char *command_description);

/**
	Adds a description in the end of the help message.
	For example @copyright xxx or something like that.
	@param description The description to be added.
	@param c_builder The builder to add a closing descripton on help.
	@return 0 on success or an integer error number on error.
	errors:
`		builder is not initialized: -6
		failed to add description: -5
	all the erros are defined as MACROS.

*/
extern int add_help_tool_closing_description(struct builder_d *c_builder, const char *close_description);

/**
	Adds a new command.
	@param c_builder The builder to add the command.
	@param c_name The name of the command.
	@param c_argc The arguments of the command.
	@param c_alias The alias of the command.
	@param c_call_back The callback that is executed then the command is requested.
	@return 0 on success or an integer error number on error.
	errors:
`		builder is not initialized: -6
	all the erros are defined as MACROS.

*/
extern int add_command(struct builder_d *c_builder, const char c_name[256], 
			int c_argc, const char c_alias[5][256], 
			void (*c_call_back)(const struct exec_info *info));


/**
	Set or change the callback function of a specific
	command.
	@param c_builder The builder.
	@param c_name	The name of the command.
	@param c_call_back The callback function to set or change with.
	@return 0 on success or an integer error number on error.
	errors:
		no such command exists: -7
		builder is not initialized: -6
	all the erros are defined as MACROS.
	
*/
extern int add_action(struct builder_d *c_builder, const char *c_name, 
		      void (*c_call_back)(const struct exec_info *info));


/**
	Adds a new command using by user defined value
	using the struct.
	@param c_builder The builder to add the command.
	@param command The command to add.
*/
static inline int add_command_easy(struct builder_d *c_builder, const struct command_d *command) 
{
	return add_command(
		c_builder,
		command->c_name,
		command->c_argc,
		command->c_alias,
		command->c_call_back
	);
}


/**
	Execute the action that has been requested.
	@param argc The argc parameter of the main function.
	@param argv The argv parameter of the main function.
	@param c_builder The bulder who has the information for each command.
	@return On success 0 is returned. On error on integer error is returned.
	Errors can be:
		Wrong argument number: -1
		Wrong command name or alias: -2
		Empty command name: -3
		No action defined: -4	-> when no call back exists.
		failed to make exec info: -5
		builder is not initiaized: -6
		
	each error has a MACRO that can be used in if statements.
*/
int execute_command(int argc, char *argv[], const struct builder_d *c_builder);



#endif


