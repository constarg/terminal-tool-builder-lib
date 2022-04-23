#ifndef TOOL_BUILDER_H
#define TOOL_BUILDER_H

#include <malloc.h>
#include <string.h>

#define TOOL_BUILDER_V 				 1.6

// errors
#define TOOL_BUILDER_WRONG_ARG_NUM              -1
#define TOOL_BUILDER_WRONG_NAME_OR_ALIAS        -2

#define TOOL_BUILDER_EMPTY_NAME	                -3

#define TOOL_BUILDER_NO_ACTION_DEFINED	        -4
#define TOOL_BUILDER_FAILED_TO_ADD              -5

#define TOOL_BUILDER_NO_SUCH_COMMAND_EXISTS	-7



struct tool_builder_c_help; 					// command help
struct tool_builder_command;

struct tool_builder_help
{
        char *t_usage_sec;					// The useage section. example: Usage: tool_name [OPTION]...
        char *t_description;					// The description of the help.
        struct tool_builder_c_help *t_commands;			// The commands of the help.
        char *t_close_description;				// The closure description.
        int t_commandsc;					// The number of commands in help.
};

struct tool_builder 
{
	struct tool_builder_command *t_commands;		// array of commands.
	int t_commandsc;					// The number of the commands.
	struct tool_builder_help t_help;			// The help of the tool.
	int t_mc: 1;						// enable or disable multiple commands in one line.
};

struct tool_builder_args 
{
	char *c_name;						// The name of the command that has been executed.
	char *c_used_alias;					// The alias that has been used.
	char *(*c_values);					// The values that the been retrieved. Must be freed when there is no more use.
	int c_argc;						// The arguments of the command.
	struct tool_builder *c_builder;				// The builder.
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
extern void tool_builder_init(struct tool_builder *c_builder);

/**
	Free the memory that has been allocated for the builder.
	@param c_builder The builder to destroy.
*/
extern void tool_builder_destroy(struct tool_builder *c_builder);

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
extern int tool_builder_init_help(struct tool_builder *c_builder, const char *tool_name);

/**
	Adds description to the help message.
	@param description The description to be added.
	@param c_builder The builder to add the description.
	@return 0 on success or an integer error number on error.
	errors:
`		builder is not initialized: -6
	all the erros are defined as MACROS.

*/
extern int tool_builder_set_desc(struct tool_builder *c_builder, const char *c_description);

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
extern int tool_builder_add_command_doc(struct tool_builder *c_builder, const char *c_name,
                                        const char *c_description);

/**
	Adds alias to the help docs
	@param c_builder The builder of the tool.
	@param c_name The name of the command in docs.
*/
extern int tool_builder_add_alias_doc(struct tool_builder *c_builder, const char *c_name);

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
extern int tool_builder_set_closing_desc(struct tool_builder *c_builder, const char *close_description);

/**
	Adds a new command.
	@param c_builder The builder to add the command.
	@param c_name The name of the command.
	@param c_argc The arguments of the command.
	@param c_alias The alias of the command.
	@param c_callback The callback that is executed then the command is requested.
	@return 0 on success or an integer error number on error.
	errors:
`		builder is not initialized: -6
	all the erros are defined as MACROS.

*/
extern int tool_builder_add_command(struct tool_builder *c_builder, const char *c_name, 
                                    int c_argc, void (*c_callback)(const struct tool_builder_args *info));


/**
	Adds alias to a command.
	@param c_builder The builder of the tool.
	@param c_name The name of the command to add the alias.
	@param c_alias variable length parameter, can get unlimited
	number of alias.
	 
*/
extern int tool_builder_add_alias(struct tool_builder *c_builder, const char *c_name, const char *c_alias, ...);

/**
	Set or change the callback function of a specific
	command.
	@param c_builder The builder.
	@param c_name	The name of the command.
	@param c_callback The callback function to set or change with.
	@return 0 on success or an integer error number on error.
	errors:
		no such command exists: -7
		builder is not initialized: -6
	all the erros are defined as MACROS.
	
*/
extern int tool_builder_set_action(struct tool_builder *c_builder, const char *c_name, 
                                   void (*c_callback)(const struct tool_builder_args *info));



extern int tool_builder_prepare(int argc, char *argv[], const struct tool_builder *c_builder);

/**
	Execute the actions that has been requested.
*/
extern void tool_builder_execute();

/**
 *     Execute a command with no values!. With this function you can
 *     call any command that has no arguments.
 *     @param c_name The command to execute it can be an alias as well.
 *     @param c_builder The builder.
 *     @return On success 0 is returned. On error on integer error is returned.
 *     Errors can be:
 *          Wrong command name or alias: -2
 */
extern int tool_builder_call_command(const char *c_name, const struct tool_builder *c_builder);


/**
 *     Enable or diable the feature to execute multiple commands in one
 *     line.
 *     @param state The new state of multiple commands enable variable.
 *     @param c_builder The builder.
*/
static void inline tool_builder_set_mc(struct tool_builder *c_builder, int state)
{
	c_builder->t_mc = state & 0x01;
}

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


