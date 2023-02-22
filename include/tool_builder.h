#ifndef TOOL_BUILDER_H
#define TOOL_BUILDER_H

#include <malloc.h>
#include <string.h>

#define TOOL_BUILDER_V                           1.6

// errors
#define TOOL_BUILDER_WRONG_ARG_NUM              -1
#define TOOL_BUILDER_WRONG_NAME_OR_ALIAS        -2

#define TOOL_BUILDER_EMPTY_NAME                 -3

#define TOOL_BUILDER_NO_ACTION_DEFINED          -4
#define TOOL_BUILDER_FAILED_TO_ADD              -5

#define TOOL_BUILDER_NO_SUCH_COMMAND_EXISTS     -7


struct tool_builder_c_help;                         // command help
struct tool_builder_command;

struct tool_builder_help
{
    char *t_usage_sec;                          // The useage section. example: Usage: tool_name [OPTION]...
    char *t_description;                        // The description of the help.
    struct tool_builder_c_help *t_commands;     // The commands of the help.
    char *t_close_description;                  // The closure description.
    int t_commandsc;                            // The number of commands in help.
};


/**
	The builder structure is the most basic element of the library.
	It contains all the necessary information for the construction 
	of the requested tool.
*/
struct tool_builder
{
    struct tool_builder_command *t_commands;        // array of commands.
    int t_commandsc;                                // The number of the commands.
    struct tool_builder_help t_help;                // The help of the tool.
    int t_mc: 1;                                    // enable or disable multiple commands in one line.
};

/**
	The parameter structure is the structure that is "sent" to the 
	user function that executes a command. It is used so that the
	user (tool developer) receives the necessary information about 
	the command that was executed.
*/
struct tool_builder_args
{
    char *c_name;                                   // The name of the command that has been executed.
    char *c_used_alias;                             // The alias that has been used.
    char *(*c_values);                              // The values that the been retrieved. Must be freed when there is no more use.
    int c_argc;                                     // The arguments of the command.
    struct tool_builder *c_builder;                 // The builder.
};


/**
	The tool_builder_init function initializes the tool builder.
 
	@param c_builder Is a pointer to the builder to be used.
*/
extern void tool_builder_init(struct tool_builder *c_builder);

/**
	The tool_builder_destroy function frees the memory that 
	the builder has reserved. It should be called when the 
	builder is no longer useful.

	@param c_builder Is pointer to the builder used and no longer needed.
*/
extern void tool_builder_destroy(struct tool_builder *c_builder);

/**
	The tool_builder_init_help function generates a --help 
	command along with the -h aliases and adds it to the 
	tool with a default action. This action can be changed
	if you do not please the developer.

	@param c_builder Is a pointer to the builder to be used.
	@param tool_name The name of the tool.
	@return It returns zero when everything went well. In the event of an error, one of the following may be returned.
	@errors:
		TOOL_BUILDER_FAILED_TO_ADD Failed to add the new command or alias.
`		
	all the erros are defined as MACROS.

*/
extern int tool_builder_init_help(struct tool_builder *c_builder, const char *tool_name);

/**
	The tool_builder_set_desc function sets a description of
	the tool, which will appear when someone calls the 
	--help command.

	@param c_builder Is a pointer to the builder to be used.
	@param description A general description of the tool.
	@return It returns zero when everything went well. In the event of an error, one of the following may be returned.
	@errors:
`		TOOL_BUILDER_FAILED_TO_ADD Failed to add description.
	all the erros are defined as MACROS.

*/
extern int tool_builder_set_desc(struct tool_builder *c_builder, const char *c_description);

/**
	The tool_builder_add_command_doc function adds the name 
	of a command, along with its accompanying description 
	to the tool instructions. These instructions can be
	 displayed using the --help command.

	@param c_builder Is a pointer to the builder to be used.
	@param c_name The name of the command to add.
	@param c_description  The description of the command.
	@return It returns zero when everything went well. In the event of an error, one of the following may be returned.
	@errors:
`		TOOL_BUILDER_FAILED_TO_ADD Failed to add the command or description.

	all the erros are defined as MACROS.

*/
extern int tool_builder_add_command_doc(struct tool_builder *c_builder, const char *c_name,
                                        const char *c_description);

/**
	The tool_builder_add_command_doc function adds the aliases 
	of a command to the tool instructions. These instructions 
	can be displayed using the --help command. The aliases do
	 not need to be given again because they already exist in 
	the builder. They are in the command structure.

	@param c_builder The builder of the tool.
	@param c_name The name of the command in docs.
	@return It returns zero when everything went well. In the event of an error, one of the following may be returned.
	@errors:
		TOOL_BUILDER_NO_SUCH_COMMAND_EXISTS The command does not exists.

	all the erros are defined as MACROS.
*/
extern int tool_builder_add_alias_doc(struct tool_builder *c_builder, const char *c_name);

/**
	The tool_builder_set_closing_desc function sets another description, 
	which appears at the end of the document that appears when the 
	--help command is executed.

	@param c_builder Is a pointer to the builder to be used.
	@param description Something like an epilogue to the reference to a source.
	@return It returns zero when everything went well. In the event of an error, one of the following may be returned.
	@errors:
		TOOL_BUILDER_FAILED_TO_ADD Failed to add closing description.

`	all the erros are defined as MACROS.

*/
extern int tool_builder_set_closing_desc(struct tool_builder *c_builder, const char *close_description);

/**
	The tool_builder_add_command function adds a new command to the tool. 
	This means that if a user using the tool you created calls the command 
	name specified in this function, a certain action that is also defined 
	in this function will be performed.	

	@param c_builder Is a pointer to the builder to be used.
	@param c_name The name of the command.
	@param c_argc The number of parameters required by the command.
	@param c_callback The action to be taken if the user requests this command. 
			  The function that plays the role of the command action 
			  must have the below signature:

			  	void your_action(const struct tool_builder_args *info);

			  All information about the command inputs is given as a parameter in this action. Check tool_builder_args struct for more details.
	@return It returns zero when everything went well. In the event of an error, one of the following may be returned.
	@errors:
		TOOL_BUILDER_FAILED_TO_ADD Failed to add the new command.

	all the erros are defined as MACROS.

*/
extern int tool_builder_add_command(struct tool_builder *c_builder, const char *c_name,
                                    int c_argc, void (*c_callback)(const struct tool_builder_args *info));


/**
	The tool_builder_add_alias function looks like the tool_builder_add_command 
	function. The difference is that instead of adding a new command, it adds 
	to a specific command 1, 2, .... ,n names that describe the same command. 
	Such names could be abbreviations, in order to make it easier for the user to remember.

	@param c_builder Is a pointer to the builder to be used.
	@param c_name The name of the command to add the aliases.
	@param c_alias The multitude of aliases. The last alias must be NULL.
	@return It returns zero when everything went well. In the event of an error, one of the following may be returned.
	@errors:
		TOOL_BUILDER_NO_SUCH_COMMAND_EXISTS The command to which it went to add the aliases does not exist.
		TOOL_BUILDER_FAILED_TO_ADD Failed to add the aliases.

	all the erros are defined as MACROS.

	 
*/
extern int tool_builder_add_alias(struct tool_builder *c_builder, const char *c_name, const char *c_alias, ...);

/**
	The tool_builder_set_action function changes or sets an action on a command.

	@param c_builder Is a pointer to the builder to be used. 
	@param c_name The name of the command that should perform this action.
	@param c_callback The action to be performed on the command.
	@return It returns zero when everything went well. In the event of an error, one of the following may be returned.
	@errors:
		TOOL_BUILDER_NO_SUCH_COMMAND_EXISTS The command c_name does not exists in the builder.
		
	all the erros are defined as MACROS.
	
*/
extern int tool_builder_set_action(struct tool_builder *c_builder, const char *c_name,
                                   void (*c_callback)(const struct tool_builder_args *info));


/**
	The tool_builder_prepare function makes all the necessary preparations in 	
	the builder to execute the command(s) requested by the terminal.

	@param argc The number of parameters given in the executable. Given by the main function.
	@param argv Pointer that pointes to the first parameter. Given by the main function.
	@param c_builder Is a pointer to the builder to be used.
	@return It returns zero when everything went well. In the event of an error, one of the following may be returned.
	@errors:
		TOOL_BUILDER_EMPTY_NAME No executable parameters were given
		TOOL_BUILDER_NO_SUCH_COMMAND_EXISTS The parameter given to the executable does not correspond to a command.
		TOOL_BUILDER_NO_ACTION_DEFINED The requested command does not have a specified action.
		TOOL_BUILDER_WRONG_ARG_NUM The parameters provided are not sufficient for the requested command.

	all the erros are defined as MACROS.
*/

extern int tool_builder_prepare(int argc, char *argv[], const struct tool_builder *c_builder);

/**
	The tool_builder_execute function is responsible for executing any command 
	requested by the terminal in the executable. If more than one command 
	is given, it will be executed in the order given, from left to right.
*/
extern void tool_builder_execute();

/**
	The tool_builder_call_command function allows the tool developer
	to execute a tool command without the user requesting 
	it from the terminal.

	@param c_name The name of the command to be executed.
	@param c_builder Is a pointer to the builder to be used.
	@return It returns zero when everything went well. In the event of an error, one of the following may be returned.
	@errors:
		TOOL_BUILDER_WRONG_NAME_OR_ALIAS There is no command with this name.

	all the erros are defined as MACROS.
*/
extern int tool_builder_call_command(const char *c_name, const struct tool_builder *c_builder);


/**
	The tool_builder_set_mc function allows the developer to 
	disable the library function that allows more than one
	command to execute on the same terminal line.

	@param c_builder Is a pointer to the builder to be used.
	@param state The new state. '1' to enable or '0' to disable. By default is '1'.
*/
static void inline tool_builder_set_mc(struct tool_builder *c_builder, int state)
{
    c_builder->t_mc = state & 0x01;
}


/**
	The tool_builder_add_both function is a shortcut that executes the 
	tool_builder_add_command and tool_builder_add_command_doc functions 
	but by calling only one function.

	@param c_builder Is a pointer to the builder to be used.
	@param c_name The name of the command.
	@param c_argc The number of parameters required by the command.
	@param c_callback The action to be taken if the user requests this command. 
			  The function that plays the role of the command action 
			  must have the below signature:

				void your_action(const struct tool_builder_args *info);

			  All information about the command inputs is given as a parameter in this action. Check tool_builder_args struct for more details.
	@param c_description The description of the command.
	@return It returns zero when everything went well. In the event of an error, one of the following may be returned.
	@errors:
		TOOL_BUILDER_FAILED_TO_ADD Failed to add the new command or the description.

	all the erros are defined as MACROS.
*/
static int inline tool_builder_add_both(struct tool_builder *c_builder, const char *c_name,
                                        int c_argc, void (*c_callback)(const struct tool_builder_args *info),
                                        const char *c_description)
{

    int error = tool_builder_add_command(
            c_builder,
            c_name,
            c_argc,
            c_callback
    );
    if (error != 0) return error;

    return tool_builder_add_command_doc(
            c_builder,
            c_name,
            c_description
    );

}

/**
	The tool_builder_add_alias_both function is a shortcut that executes 
	the tool_builder_add_alias and tool_builder_add_alias_doc functions 
	but by calling only one function.

	@param c_builder Is a pointer to the builder to be used.
	@param c_name The name of the command to add the aliases.
	@param c_alias The multitude of aliases. The last alias must be NULL.
	@return It returns zero when everything went well. In the event of an error, one of the following may be returned.
	@errors:	
		TOOL_BUILDER_NO_SUCH_COMMAND_EXISTS The command to which it went to add the aliases does not exist.
		TOOL_BUILDER_FAILED_TO_ADD Failed to add the aliases.

	all the erros are defined as MACROS.
*/
static int inline tool_builder_add_alias_both(struct tool_builder *c_builder, const char *c_name,
                                              const char *c_alias, ...)
{

    int error = tool_builder_add_alias(
            c_builder,
            c_name,
            c_alias
    );
    if (error != 0) return error;


    return tool_builder_add_alias_doc(
            c_builder,
            c_name
    );
}


#endif


