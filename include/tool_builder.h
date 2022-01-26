#ifndef TOOL_BUILDER_H
#define TOOL_BUILDER_H

#include <mallo.h>

struct command_d {
	char c_name[256];				// The name of the command.
	int c_argc;					// How many argcs the command require.
	char c_alias[256][5];				// The alias of the commnad. 5 is the maximum number of alias.
	/**
		This callback is user defined and is executed
		when the user request a call of the specific
		command defined in @name field from terminal.
		@param value The parameters that have been
		retrieved from the terminal when the command
		was called.
	*/
	void *(*c_call_back)(const char *value);	// The action to take when the command has been requested.
};

struct builder_d {
	struct command_d *commands;
	// TODO - Add help struct.
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
static inline void initialize_builder(struct builder_d *c_builder)
{
	c_builder = (struct builder_d *) malloc(sizeof(struct builder_d));
	if (c_builder == NULL)
		return;
}

/**
	Free the memory that has been allocated for the builder.
	@param c_builder The builder to destroy.
*/
static inline destroy_builder(struct builder_d *c_builder)
{
	free(c_builder);
	c_builder = NULL;
}

/**
	Builds a basic help action that prints informations
	about the tool.
	@param tool_name The name of the tool.
	@param c_builder The builder to add the help.
*/
extern void initialize_help(struct builder_d *c_builder, const char *tool_name);

/**
	Adds user defined format of help.
	If this function is used. Then the initialize_help
	function must be ignored.
	@param help The full help message.
	@param c_builder The builder to add the help.
*/
extern void add_full_help(struct builder_d *c_builder, const char *help);

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
*/
extern void add_help_tool_closing_description(struct builder_d *c_builder, const char *description);

/**
	Adds a new command.
	@param c_builder The builder to add the command.
	@param c_name The name of the command.
	@param c_argc The arguments of the command.
	@param c_alias The alias of the command.
	@param c_call_back The callback that is executed then the command is requested.
*/
extern void add_command(struct builder_d *c_builder, const char c_name[256], 
			int c_argc, const char c_alias[256][5], 
			void *(*c_call_back)(const char *value));


/**
	Set or change the callback function of a specific
	command.
	@param c_builder The builder.
	@param c_call_back The callback function to set or change with.
	
*/
extern void add_action(struct builder_d *c_builder, void *(*c_call_back)(const char *value));


/**
	Adds a new command using by user defined value
	using the struct.
	@param c_builder The builder to add the command.
	@param command The command to add.
*/
static inline void add_command(struct builder_d *c_builder, const struct command_d *command) 
{
	add_command(
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
*/
extern void execute_command(int argc, char **argv, struct builder_d *c_builder);



#endif


