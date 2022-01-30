#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <tool_builder.h>


// Priavte implementation of structs.
struct command_help 
{
	char *c_name;						// The name of the command.
	char *(*c_alias);					// The alias of the command.
	char *c_description;					// The description of the command.
};

struct help_d 
{
	char *h_usage_sec;					// The useage section. example: Usage: tool_name [OPTION]...
	char *h_description;					// The description of the help.
	struct command_help **h_commands;			// The commands of the help.
	char *h_close_description;				// The closure description.
	int h_commandsc;					// The number of commands in help.
};

struct command_d 
{
	char c_name[256];					// The name of the command.
	int c_argc;						// How many argcs the command require.
	char *(*c_alias);						// The alias of the commnad. 5 is the maximum number of alias.
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


static inline struct command_d *find_command(const struct command_d **commands, const char *c_name,
					     int commandsc);

static void help_defualt_action(const struct exec_info *info)
{
	// Prints the help message.
	struct builder_d *builder = info->c_builder;
	if (builder == NULL || builder->b_help == NULL) return;
	printf("%s\n", builder->b_help->h_usage_sec);
	
	if (builder->b_help->h_description == NULL) return;
	printf("%s\n\n\n", builder->b_help->h_description);
	
	struct command_help **commands_h = builder->b_help->h_commands;
	for (int h = 0; h < builder->b_help->h_commandsc; h++)
	{
		printf("%s", commands_h[h]->c_name);
		for (int c_a = 0; commands_h[h]->c_alias[c_a]; c_a++)
			printf(" ,%s,", commands_h[h]->c_alias[c_a]);
		printf("\t\t%s\n\n", commands_h[h]->c_description);
	}

	if (builder->b_help->h_close_description == NULL) return;
	printf("%s\n", builder->b_help->h_close_description);
}

void initialize_builder(struct builder_d **c_builder)
{
	*c_builder = (struct builder_d *) malloc(sizeof(struct builder_d));
	(*c_builder)->b_commands = (struct command_d **) calloc(1, sizeof(struct command_d **));
	(*c_builder)->b_help = (struct help_d *) calloc(1, sizeof(struct help_d));
	(*c_builder)->b_commandsc = 0;
}

void destroy_builder(struct builder_d **c_builder)
{
	// Free the memory for the commands.
	for (int c = 0; c < (*c_builder)->b_commandsc; c++) 
	{
		// TODO - destroy alias.
		free((*c_builder)->b_commands[c]);
	}
	free((*c_builder)->b_commands);
	
	// Free help.
	for (int h = 0; h < (*c_builder)->b_help->h_commandsc; h++) 
	{
		// TODO - destroy alias.
		free((*c_builder)->b_help->h_commands[h]->c_name);
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


int initialize_help(struct builder_d *c_builder, const char *tool_name)
{
	if (c_builder == NULL || c_builder->b_help == NULL) return BUILDER_IS_NOT_INITIALIZED;

	const char *str_u_1 = "Usage:";
	const char *str_u_2 = "[OPTION]...";
	
	size_t usage_size = strlen(tool_name) + strlen(str_u_1)
				   + strlen(str_u_2) + 3;
 
	char *(*usage_sec) = &c_builder->b_help->h_usage_sec;
	*usage_sec = malloc(sizeof(char) * usage_size);
	if ((*usage_sec) == NULL) return FAILED_TO_ADD;
	
	sprintf((*usage_sec),"%s %s %s", str_u_1, tool_name, str_u_2);
	
	// Initialize help as a command.
	// TODO - add the alias of help here.
	// help alias.
	
	// Add command.
	return add_command(
		c_builder,
		"--help",
		0,
		&help_defualt_action
	); 
}

int add_help_tool_description(struct builder_d *c_builder, const char *description)
{
	if (c_builder == NULL || c_builder->b_help == NULL) return BUILDER_IS_NOT_INITIALIZED;

	struct help_d *(*help_tmp) = &c_builder->b_help; 
	(*help_tmp)->h_description = malloc(sizeof(char) * strlen(description) + 1);
	strcpy((*help_tmp)->h_description, description);
	
	return 0;
}


int add_help_tool_command(struct builder_d *c_builder, const char *command_name, 
			  const char *command_description)
{
	if (c_builder == NULL || c_builder->b_help == NULL) return BUILDER_IS_NOT_INITIALIZED;

	struct command_help *new_command_h = malloc(sizeof(struct command_help)); 
	new_command_h->c_name = malloc(sizeof(char) * (strlen(command_name) + 1));
	new_command_h->c_description = malloc(sizeof(char) * (strlen(command_description) + 1));
	if (new_command_h->c_name == NULL || new_command_h->c_description == NULL)
		return FAILED_TO_ADD;
	
	// Copy the contents.
	strcpy(new_command_h->c_name, command_name);
	strcpy(new_command_h->c_description, command_description);	

	int last_c = c_builder->b_help->h_commandsc;
	c_builder->b_help->h_commands = (struct command_help **) realloc(c_builder->b_help->h_commands,
									 sizeof(struct command_help *) * (last_c + 1));
	if (c_builder->b_help->h_commands == NULL)
		return FAILED_TO_ADD;

	// Save the new command.
	c_builder->b_help->h_commands[last_c] = new_command_h;
	// Increase the number of commands.
	c_builder->b_help->h_commandsc += 1;
	return 0;
}

int add_help_tool_alias(struct builder_d *c_builder, const char *c_name)
{
	if (c_builder == NULL || c_builder->b_help == NULL)
		return BUILDER_IS_NOT_INITIALIZED;

	struct command_d *c_found = find_command((const struct command_d **) c_builder->b_commands,
						 c_name, c_builder->b_commandsc);
	if (c_found == NULL) return NO_SUCH_COMMAND_EXISTS;

	for (int h = 0; h <c_builder->b_help->h_commandsc; h++)
	{
		if (!strcmp(c_builder->b_help->h_commands[h]->c_name, c_found->c_name))
			c_builder->b_help->h_commands[h]->c_alias = c_found->c_alias;
	}
}

int add_help_tool_closing_description(struct builder_d *c_builder, const char *close_description)
{
	if (c_builder == NULL || c_builder->b_help == NULL) return BUILDER_IS_NOT_INITIALIZED;
	
	char *(*closing_desc) = &c_builder->b_help->h_close_description;
	*closing_desc = (char *) malloc(sizeof(char) * strlen(close_description) + 1);
	if ((*closing_desc) == NULL) return FAILED_TO_ADD;

	// Copy the contents.
	strcpy((*closing_desc), close_description);
	return 0;
}

int add_command(struct builder_d *c_builder, const char c_name[256], int c_argc,  
		 void (*c_call_back)(const struct exec_info *info))
{
	if (c_builder == NULL) return BUILDER_IS_NOT_INITIALIZED;

	// Allocate memory for the new command.
	int last_c = c_builder->b_commandsc;
	struct command_d *new_command = (struct command_d *) malloc(sizeof(struct command_d));
	strcpy(new_command->c_name, c_name);
	new_command->c_argc = c_argc;
	
	new_command->c_call_back = c_call_back;
	c_builder->b_commands = (struct command_d **) realloc(c_builder->b_commands, 
							     sizeof(struct command_d *) * (last_c + 1));
	c_builder->b_commands[last_c] = new_command;	
	c_builder->b_commandsc += 1;
	return 0;
}

int add_command_alias(struct builder_d *c_builder, const char *c_name, 
		      const char *c_alias, ...) 
{
	// TODO - Get the alias and set them into the ascociated command.
}


int add_action(struct builder_d *c_builder, const char *c_name,
	       void (*c_call_back)(const struct exec_info *info))
{
	if (c_builder == NULL || c_builder->b_commands == NULL) return BUILDER_IS_NOT_INITIALIZED;
	struct command_d *ch_command = find_command((const struct command_d **) c_builder->b_commands, c_name,
						    c_builder->b_commandsc);
	if (ch_command == NULL) return NO_SUCH_COMMAND_EXISTS;
	ch_command->c_call_back = c_call_back;	// Set the requested callback.
	return 0;
}


static inline struct command_d *find_command(const struct command_d **commands, const char *c_name,
					     int commandsc) 
{

	for (int curr_c = 0; curr_c < commandsc; curr_c++)
	{
		if (commands[curr_c] == NULL) return NULL;
		// Check if any of the names in the commands is the c_name.
		if (!strcmp(commands[curr_c]->c_name, c_name)) return (struct command_d *) commands[curr_c];
		else
		{
			// Check if any of the alias of the current command is the c_name.
			if (commands[curr_c]->c_alias == NULL) continue;
			for (int curr_a = 0; commands[curr_c]->c_alias[curr_a]; curr_a++)
		 	{
				if (!strcmp(commands[curr_c]->c_alias[curr_a], c_name)) return (struct command_d *) commands[curr_c]; 
			}
		}
	}
	return NULL;
}


int execute_command(int argc, char *argv[], const struct builder_d *c_builder)
{
	if (c_builder == NULL || c_builder->b_commands == NULL ||
	    c_builder->b_help == NULL)
	{
		return BUILDER_IS_NOT_INITIALIZED;
	}

	if (argv[1] == NULL) return EMPTY_NAME;


	// Get the requested command to execute.
	struct command_d *command = find_command((const struct command_d **) c_builder->b_commands, argv[1],
						 c_builder->b_commandsc);
	
	if (command == NULL) return WRONG_NAME_OR_ALIAS;

	if (((argc - 2) - command->c_argc) < 0) return WRONG_ARG_NUM;

	if (command->c_call_back == NULL) return NO_ACTION_DEFINED;

	// Build the info to recieve the call back.
	struct exec_info exec_inf;
	strcpy(exec_inf.c_name, command->c_name);
	strcpy(exec_inf.c_used_alias, argv[1]);
	exec_inf.c_values = (argv + 2);	// This points to the first argument of the requested command. 
	exec_inf.c_argc = command->c_argc;
	exec_inf.c_builder = (struct builder_d *) c_builder;

	// Call the callback with the values.
	command->c_call_back(&exec_inf);

	return 0;
}


