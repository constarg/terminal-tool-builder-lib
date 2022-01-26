#ifndef TOOL_BUILDER_H
#define TOOL_BUILDER_H

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


/**
	Builds a basic help action that prints informations
	about the tool.
	@param tool_name The name of the tool.
*/
extern void initialize_help(const char *tool_name);

/**
	Adds user defined format of help.
	If this function is used. Then the initialize_help
	function must be ignored.
	@param help The full help message.
*/
extern void add_full_help(const char *help);

/**
	Adds description to the help message.
	@param description The description to be added.
*/
extern void add_help_tool_description(const char *description);

/**
	Adds to the help action a command and the description
	of the command.
	@command_name The name of the command to add.
	@command_description The description of the command.
*/
extern void add_help_tool_command(const char *command_name, const char *command_description);

/**
	Adds a description in the end of the help message.
	For example @copyright xxx or something like that.
	@param description The description to be added.
*/
extern void add_help_tool_closing_description(const char *description);

/**
	Adds a new command.
	@param c_name The name of the command.
	@param c_argc The arguments of the command.
	@param c_alias The alias of the command.
	@param c_call_back The callback that is executed then the command is requested.
*/
extern void add_command(const char c_name[256], int c_argc,
			const char c_alias[256][5], void *(*c_call_back)(const char *value));


/**
	Set or change the callback function of a specific
	command.
	@param c_call_back The callback function to set or change with.
	
*/
extern void add_action(void *(*c_call_back)(const char *value));


/**
	Adds a new command using by user defined value
	using the struct.
	@param command The command to add.
*/
static inline void add_command(const struct command_d *command) 
{
	add_command(
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
*/
extern void execute_command(int argc, char **argv);



#endif


