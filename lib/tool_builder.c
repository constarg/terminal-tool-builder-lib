#include <string.h>
#include <stdlib.h>
#include <tool_builder.h>


static inline struct command_d *find_command(const struct command_d **commands, const char *c_name,
					     int commandsc);

static void help_defualt_action(const struct exec_info *info)
{
	// Prints the help message.
	struct builder_d *builder = info->c_builder;
	printf("%s\n", builder->b_help->h_usage_sec);
	printf("%s\n\n\n", builder->b_help->h_description);
	
	struct command_help **commands_h = builder->b_help->h_commands;
	for (int h = 0; h < builder->b_help->h_argc; h++)
	{
		printf("%s", commands_h[h]->c_name);
		for (int c_a = 0; c_a < 5; c_a++)
			printf(" ,%s,", commands_h[h]->c_alias[c_a]);
		printf("\t\t%s", commands_h[h]->c_description);
	}	
}

int initialize_help(struct builder_d *c_builder, const char *tool_name)
{
	if (c_builder == NULL || c_builder->b_help == NULL) return BUILDER_IS_NOT_INITIALIZED;

	const char *str_u_1 = "Usage:";
	const char *str_u_2 = "[OPTION]...";
	
	size_t usage_size = strlen(tool_name) + strlen(str_u_1)
				   + strlen(str_u_2) + 1;
 
	char *(*usage_sec) = &c_builder->b_help->h_usage_sec;
	*usage_sec = malloc(sizeof(char) * usage_size);
	if ((*usage_sec) == NULL) return FAILED_TO_ADD;
	
	sprintf((*usage_sec),"%s %s %s", str_u_1, tool_name, str_u_2);
	
	// Initialize help as a command.
	char h_alias[5][256];
	initialize_alias(&h_alias);
	// help alias.
	strcpy(h_alias[0], "-h");
	// Add command.
	return add_command(
		c_builder,
		"--help",
		0,
		h_alias,
		&help_defualt_action
	); 
}

int add_full_help(struct builder_d *c_builder, const struct help_d *t_help)
{
	if (c_builder == NULL) return BUILDER_IS_NOT_INITIALIZED;
	if (t_help == NULL) return UNITIALIZED_HELP;

	// Copy the help variable contents from the user to the builder.
	memcpy(&c_builder->b_help, &t_help, sizeof(struct help_d));	
}


void add_help_tool_description(struct builder_d *c_builder, const char *description)
{

}


void add_help_tool_command(struct builder_d *c_builder, const char *command_name, 
			   const char *command_description)
{
	
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

int add_command(struct builder_d *c_builder, const char c_name[256], int c_argc, const char c_alias[5][256], 
		 void (*c_call_back)(const struct exec_info *info))
{
	if (c_builder == NULL) return BUILDER_IS_NOT_INITIALIZED;

	// Allocate memory for the new command.
	int last_c = c_builder->b_commandsc;
	struct command_d *new_command = (struct command_d *) malloc(sizeof(struct command_d));
	strcpy(new_command->c_name, c_name);
	new_command->c_argc = c_argc;

	
	for (int c_a = 0; c_a < 5; c_a++)
	{
		if (c_alias[c_a] == NULL) continue;
		strcpy(new_command->c_alias[c_a], c_alias[c_a]);;
	}	
	new_command->c_call_back = c_call_back;
	c_builder->b_commands = (struct command_d **) realloc(c_builder->b_commands, 
							     sizeof(struct command_d *) * (last_c + 1));
	c_builder->b_commands[last_c] = new_command;	
	c_builder->b_commandsc += 1;
	return 0;
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
			for (int curr_a = 0; curr_a < 5; curr_a++)
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


