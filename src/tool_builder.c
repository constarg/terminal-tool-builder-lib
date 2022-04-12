#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <tool_builder.h>


// Priavte implementation of structs.
struct tool_builder_c_help 
{
	char *c_name;							// The name of the command.
	char *(*c_alias);						// The alias of the command.
	char *c_description;						// The description of the command.
};

struct tool_builder_help 
{
	char *h_usage_sec;						// The useage section. example: Usage: tool_name [OPTION]...
	char *h_description;						// The description of the help.
	struct tool_builder_c_help **h_commands;			// The commands of the help.
	char *h_close_description;					// The closure description.
	int h_commandsc;						// The number of commands in help.
};

struct tool_builder_command 
{
	char *c_name;							// The name of the command.
	int c_argc;							// How many argcs the command require.
	char *(*c_alias);						// The alias of the commnad. 5 is the maximum number of alias.
	int c_alias_c;
	/**
		This callback is user defined and is executed
		when the user request a call of the specific
		command defined in @name field from terminal.
		@param value The parameters that have been
		retrieved from the terminal when the command
		was called.
	*/
	void (*c_callback)(const struct tool_builder_args *info);	// The action to take when the command has been requested.
};


static inline struct tool_builder_command *find_command(const struct tool_builder_command **commands, const char *c_name,
					     int commandsc);

static void help_defualt_action(const struct tool_builder_args *info)
{
	// Prints the help message.
	struct tool_builder *builder = info->c_builder;
	if (builder == NULL || builder->b_help == NULL) return;
	printf("%s\n", builder->b_help->h_usage_sec);
	
	if (builder->b_help->h_description == NULL) goto skip_dec;
	printf("%s\n\n\n", builder->b_help->h_description);
skip_dec:	
	struct tool_builder_c_help **commands_h = builder->b_help->h_commands;
	for (int h = 0; h < builder->b_help->h_commandsc; h++)
	{
		printf("\t%s", commands_h[h]->c_name);
		for (int c_a = 0; commands_h[h]->c_alias[c_a]; c_a++)
			printf(" ,%s,", commands_h[h]->c_alias[c_a]);
		printf("\t\t%s\n\n", commands_h[h]->c_description);
	}

	if (builder->b_help->h_close_description == NULL) return;
	printf("%s\n", builder->b_help->h_close_description);
}

void tool_builder_init(struct tool_builder *c_builder)
{
	c_builder->b_commands = (struct tool_builder_command **) calloc(1, sizeof(struct tool_builder_command **));
	c_builder->b_help = (struct tool_builder_help *) calloc(1, sizeof(struct tool_builder_help));
	c_builder->b_commandsc = 0;
}

void tool_builder_destroy(struct tool_builder *c_builder)
{
	// Free help.
	for (int h = 0; h < c_builder->b_help->h_commandsc; h++) 
	{
		free(c_builder->b_help->h_commands[h]->c_name);
		free(c_builder->b_help->h_commands[h]->c_description);
		free(c_builder->b_help->h_commands[h]);
	}	
	free(c_builder->b_help->h_commands);
	free(c_builder->b_help->h_usage_sec);
	free(c_builder->b_help->h_close_description);
	free(c_builder->b_help->h_description);
	free(c_builder->b_help);

	// Free the memory for the commands.
	for (int c = 0; c < c_builder->b_commandsc; c++) 
	{
		free(c_builder->b_commands[c]->c_name);
		for (int a = 0; a < c_builder->b_commands[c]->c_alias_c; a++)
			free(c_builder->b_commands[c]->c_alias[a]);
		free(c_builder->b_commands[c]->c_alias);
		free(c_builder->b_commands[c]);
	}
	free(c_builder->b_commands);
}	


int tool_builder_init_help(struct tool_builder *c_builder, const char *tool_name)
{
	if (c_builder == NULL || c_builder->b_help == NULL) return TOOL_BUILDER_IS_NOT_INITIALIZED;

	const char *str_u_1 = "Usage:";
	const char *str_u_2 = "[OPTION]...";
	
	size_t usage_size = strlen(tool_name) + strlen(str_u_1)
				   + strlen(str_u_2) + 3;
 
	char *(*usage_sec) = &c_builder->b_help->h_usage_sec;
	*usage_sec = malloc(sizeof(char) * usage_size);
	if ((*usage_sec) == NULL) return TOOL_BUILDER_FAILED_TO_ADD;
	
	sprintf((*usage_sec),"%s %s %s", str_u_1, tool_name, str_u_2);
	
	// Initialize help as a command.
	// Add command.
	int error = tool_builder_add_command(
		c_builder,
		"--help",
		0,
		&help_defualt_action
	); 
	if (error != 0) return error;

	return tool_builder_add_alias(c_builder, "--help","-h", NULL);
}

int tool_builder_set_desc(struct tool_builder *c_builder, const char *c_description)
{
	if (c_builder == NULL || c_builder->b_help == NULL) return TOOL_BUILDER_IS_NOT_INITIALIZED;

	struct tool_builder_help *(*help_tmp) = &c_builder->b_help;
	(*help_tmp)->h_description = (char *) malloc(sizeof(char) * strlen(c_description) + 1);
	strcpy((*help_tmp)->h_description, c_description);
	
	return 0;
}


int tool_builder_set_command_desc(struct tool_builder *c_builder, const char *c_name, 
			  const char *c_description)
{
	if (c_builder == NULL || c_builder->b_help == NULL) return TOOL_BUILDER_IS_NOT_INITIALIZED;

	struct tool_builder_c_help *new_command_h = (struct tool_builder_c_help *) malloc(sizeof(struct tool_builder_c_help)); 
	new_command_h->c_name = (char *) malloc(sizeof(char) * (strlen(c_name) + 1));
	new_command_h->c_description = (char *) malloc(sizeof(char) * (strlen(c_description) + 1));
	if (new_command_h->c_name == NULL || new_command_h->c_description == NULL)
		return TOOL_BUILDER_FAILED_TO_ADD;
	
	// Copy the contents.
	strcpy(new_command_h->c_name, c_name);
	strcpy(new_command_h->c_description, c_description);	

	int last_c = c_builder->b_help->h_commandsc;
	c_builder->b_help->h_commands = (struct tool_builder_c_help **) realloc(c_builder->b_help->h_commands,
									 	sizeof(struct tool_builder_c_help *) * (last_c + 1));
	if (c_builder->b_help->h_commands == NULL)
		return TOOL_BUILDER_FAILED_TO_ADD;

	// Save the new command.
	c_builder->b_help->h_commands[last_c] = new_command_h;
	// Increase the number of commands.
	c_builder->b_help->h_commandsc += 1;
	return 0;
}

int tool_builder_add_tool_alias(struct tool_builder *c_builder, const char *c_name)
{
	if (c_builder == NULL || c_builder->b_help == NULL)
		return TOOL_BUILDER_IS_NOT_INITIALIZED;

	struct tool_builder_command *c_found = find_command((const struct tool_builder_command **) c_builder->b_commands,
						 	    c_name, c_builder->b_commandsc);
	if (c_found == NULL) return TOOL_BUILDER_NO_SUCH_COMMAND_EXISTS;

	for (int h = 0; h < c_builder->b_help->h_commandsc; h++)
	{
		if (!strcmp(c_builder->b_help->h_commands[h]->c_name, c_found->c_name))
			c_builder->b_help->h_commands[h]->c_alias = c_found->c_alias;
	}
}

int tool_builder_set_closing_desc(struct tool_builder *c_builder, const char *close_description)
{
	if (c_builder == NULL || c_builder->b_help == NULL) return TOOL_BUILDER_IS_NOT_INITIALIZED;

	struct tool_builder_help *(*help_tmp) = &c_builder->b_help;

	(*help_tmp)->h_close_description = (char *) malloc(sizeof(char) * (strlen(close_description) + 1));
	if ((*help_tmp)->h_close_description == NULL) return TOOL_BUILDER_FAILED_TO_ADD;
	strcpy((*help_tmp)->h_close_description, close_description);
	
	return 0;
}

int tool_builder_add_command(struct tool_builder *c_builder, const char *c_name, int c_argc,  
		 void (*c_callback)(const struct tool_builder_args *info))
{
	if (c_builder == NULL) return TOOL_BUILDER_IS_NOT_INITIALIZED;

	// Allocate memory for the new command.
	int last_c = c_builder->b_commandsc;
	struct tool_builder_command *new_command = (struct tool_builder_command *) calloc(1, sizeof(struct tool_builder_command));
	new_command->c_name = (char *) malloc(sizeof(char) * (strlen(c_name) + 1));
	strcpy(new_command->c_name, c_name);
	new_command->c_argc = c_argc;
	
	new_command->c_callback = c_callback;
	c_builder->b_commands = (struct tool_builder_command **) realloc(c_builder->b_commands, 
							     		 sizeof(struct tool_builder_command *) * (last_c + 1));
	c_builder->b_commands[last_c] = new_command;	
	c_builder->b_commandsc += 1;
	return 0;
}

int tool_builder_add_alias(struct tool_builder *c_builder, const char *c_name, 
		      const char *c_alias, ...) 
{
	struct tool_builder_command *command = find_command((const struct tool_builder_command **) c_builder->b_commands, 
							    c_name, c_builder->b_commandsc);

	if (command == NULL) return TOOL_BUILDER_NO_SUCH_COMMAND_EXISTS;
	if (c_alias == NULL) return -1;

	command->c_alias = (char **) calloc(1, sizeof(char *));
	command->c_alias_c = 0; // initialize count.	
	if (command->c_alias == NULL) TOOL_BUILDER_FAILED_TO_ADD;

	// the list of alias.
	va_list alias_list;
	va_start(alias_list, c_alias);

	char *tmp_a = (char *) c_alias;
	while(tmp_a)
	{
		// allocate space for the curr alias.
		command->c_alias[command->c_alias_c] = (char *) malloc(sizeof(char) * (strlen(tmp_a) + 1));
		// copy the alias.
		strcpy(command->c_alias[command->c_alias_c], tmp_a);
		// increase the aliases.
		command->c_alias = (char **) realloc(command->c_alias, 
						     (++command->c_alias_c + 1) * sizeof(char *));
		tmp_a = va_arg(alias_list, char*);
	}

	// null terminate the alias.
	command->c_alias[command->c_alias_c] = NULL;
	va_end(alias_list);

	return 0;
}


int tool_builder_set_action(struct tool_builder *c_builder, const char *c_name,
	       void (*c_callback)(const struct tool_builder_args *info))
{
	if (c_builder == NULL || c_builder->b_commands == NULL) return TOOL_BUILDER_IS_NOT_INITIALIZED;
	struct tool_builder_command *ch_command = find_command((const struct tool_builder_command **) c_builder->b_commands, c_name,
						   	       c_builder->b_commandsc);
	if (ch_command == NULL) return TOOL_BUILDER_NO_SUCH_COMMAND_EXISTS;
	ch_command->c_callback = c_callback;	// Set the requested callback.
	return 0;
}


static inline struct tool_builder_command *find_command(const struct tool_builder_command **commands, const char *c_name,
					     int commandsc) 
{

	for (int curr_c = 0; curr_c < commandsc; curr_c++)
	{
		if (commands[curr_c] == NULL) return NULL;
		// Check if any of the names in the commands is the c_name.
		if (!strcmp(commands[curr_c]->c_name, c_name)) return (struct tool_builder_command *) commands[curr_c];
		else
		{
			// Check if any of the alias of the current command is the c_name.
			if (commands[curr_c]->c_alias == NULL) continue;
			for (int curr_a = 0; commands[curr_c]->c_alias[curr_a]; curr_a++)
		 	{
				if (!strcmp(commands[curr_c]->c_alias[curr_a], c_name)) return (struct tool_builder_command *) commands[curr_c]; 
			}
		}
	}
	return NULL;
}


int tool_builder_execute(int argc, char *argv[], const struct tool_builder *c_builder)
{
	if (c_builder == NULL || c_builder->b_commands == NULL ||
	    c_builder->b_help == NULL)
	{
		return TOOL_BUILDER_IS_NOT_INITIALIZED;
	}

	if (argv[1] == NULL) return TOOL_BUILDER_EMPTY_NAME;


	// Get the requested command to execute.
	struct tool_builder_command *command = find_command((const struct tool_builder_command **) c_builder->b_commands, argv[1],
						 	    c_builder->b_commandsc);
	
	if (command == NULL) return TOOL_BUILDER_WRONG_NAME_OR_ALIAS;

	if (((argc - 2) - command->c_argc) < 0) return TOOL_BUILDER_WRONG_ARG_NUM;

	if (command->c_callback == NULL) return TOOL_BUILDER_NO_ACTION_DEFINED;

	// Build the info to recieve the call back.
	struct tool_builder_args exec_inf;
	exec_inf.c_name = command->c_name;
	exec_inf.c_used_alias = argv[1];
	exec_inf.c_values = (argv + 2);	// This points to the first argument of the requested command. 
	exec_inf.c_argc = command->c_argc;
	exec_inf.c_builder = (struct tool_builder *) c_builder;

	// Call the callback with the values.
	command->c_callback(&exec_inf);

	return 0;
}
