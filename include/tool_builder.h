#ifndef TOOL_BUILDER_H
#define TOOL_BUILDER_H

#include <malloc.h>
#include <string.h>

#define TOOL_BUILDER_V 			 1.0

// errors
#define WRONG_ARG_NUM			-1
#define WRONG_NAME_OR_ALIAS		-2

#define EMPTY_NAME			-3

#define NO_ACTION_DEFINED		-4

#define FAILED_TO_ADD			-5
#define BUILDER_IS_NOT_INITIALIZED	-6

#define NO_SUCH_COMMAND_EXISTS		-7

#define UNITIALIZED_HELP		-8


struct command_help;
struct help_d; 
struct command_d; 

struct builder_d 
{
	struct command_d **b_commands;				// array of commands.
	int b_commandsc;					// The number of the commands.
	struct help_d *b_help;					// The help of the tool.
};

struct exec_info 
{
	char c_name[256];					// The name of the command that has been executed.
	char c_used_alias[256];					// The alias that has been used.
	char *(*c_values);					// The values that the been retrieved. Must be freed when there is no more use.
	int c_argc;						// The arguments of the command.
	struct builder_d *c_builder;				// The builder.
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
extern void initialize_builder(struct builder_d **c_builder);

/**
	Free the memory that has been allocated for the builder.
	@param c_builder The builder to destroy.
*/
extern void destroy_builder(struct builder_d **c_builder);

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
	Adds description to the help message.
	@param description The description to be added.
	@param c_builder The builder to add the description.
	@return 0 on success or an integer error number on error.
	errors:
`		builder is not initialized: -6
	all the erros are defined as MACROS.

*/
extern int add_help_tool_description(struct builder_d *c_builder, const char *c_description);

/**
	Adds to the help action a command and the description
	of the command.
	@command_name The name of the command to add.
	@command_description The description of the command.
	@c_builder The builder to add the command and command description.
	@return 0 on success or an integer error number on error.
	errors:
`		builder is not initialized: -6
	all the erros are defined as MACROS.

*/
extern int add_help_tool_command(struct builder_d *c_builder, const char c_name[256],
				 const char *c_description);

/**
	Adds alias to the help docs
	@param c_builder The builder of the tool.
	@param c_name The name of the command in docs.
*/
extern int add_help_tool_alias(struct builder_d *c_builder, const char c_name[256]);

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
			int c_argc, void (*c_call_back)(const struct exec_info *info));


/**
	Adds alias to a command.
	@param c_builder The builder of the tool.
	@param c_name The name of the command to add the alias.
	@param c_alias variable length parameter, can get unlimited
	number of alias.
	 
*/
extern int add_command_alias(struct builder_d *c_builder, const char c_name[256], const char *c_alias, ...);

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



static int inline add_command_both(struct builder_d *c_builder, const char c_name[256], 
			int c_argc, void (*c_call_back)(const struct exec_info *info),
			const char *c_description)
{
	
	int error = add_command(
		c_builder,
		c_name,
		c_argc,
		c_call_back
	);
	if (error == 0) return error;

	return add_help_tool_command(
		c_builder,
		c_name,
		c_description
	);

}

static int inline add_command_alias_both(struct builder_d *c_builder, const char c_name[256], 
					const char *c_alias, ...)
{
	
	int error = add_command_alias(
		c_builder,
		c_name,
		c_alias
	);
	if (error != 0) return error;
	

	return add_help_tool_alias(
		c_builder,
		c_name
	);
}


#endif


