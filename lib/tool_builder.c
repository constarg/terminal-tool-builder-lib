#include <string.h>

#include <tool_builder.h>




void initialize_help(struct builder_d *c_builder, const char *tool_name)
{

}

void add_full_help(struct builder_d *c_builder, const struct help_d *t_help)
{

}


void add_help_tool_description(struct builder_d *c_builder, const char *description)
{

}


void add_help_tool_command(struct builder_d *c_builder, const char *command_name, 
			   const char *command_description)
{

}


extern void add_help_tool_closing_description(struct builder_d *c_builder, const char *description)
{

}


void add_command(struct builder_d *c_builder, const char c_name[256], int c_argc, const char c_alias[5][256], 
		 void (*c_call_back)(const struct exec_info *info))
{

}


void add_action(struct builder_d *c_builder, void (*c_call_back)(const struct exec_info *info))
{

}


static inline struct command_d *find_command(const struct command_d **commands, const char *c_name) 
{

	for (int curr_c = 0; commands[curr_c]; curr_c++)
	{
		if (commands[curr_c] == NULL) return NULL;
		// Check if any of the names in the commands is the c_name.
		if (!strcmp(commands[curr_c]->c_name, c_name)) return (struct command_d *) commands[curr_c];
		else
		{	
			// Check if any of the alias of the current command is the c_name.
			for (int curr_a = 0; commands[curr_c]->c_alias[curr_a]; curr_a++)
				if (!strcmp(commands[curr_c]->c_alias[curr_a], c_name)) return (struct command_d *) commands[curr_c]; 
		}
	}
	return NULL;
}


int execute_command(int argc, char **argv, const struct builder_d *c_builder)
{
	if (c_builder == NULL || c_builder->b_commands == NULL ||
	    c_builder->b_help == NULL)
	{
		return BUILDER_IS_NOT_INITIALIZED;
	}

	if (argv[1] == NULL) return EMPTY_NAME;

	// Get the requested command to execute.
	struct command_d *command = find_command((const struct command_d **) c_builder->b_commands, argv[1]);

	if (command == NULL) return WRONG_NAME_OR_ALIAS;

	if (((argc - 1) - command->c_argc) < 0) return WRONG_ARG_NUM;

	if (command->c_call_back == NULL) return NO_ACTION_DEFINED;

	// Build the info to recieve the call back.
	struct exec_info *exec_inf = (struct exec_info *) calloc(1, sizeof(struct exec_info));
	if (exec_inf == NULL) return FAILED_TO_MAKE_EXEC_INFO;

	// Initialize infos.
	strcpy(exec_inf->c_name, command->c_name);
	strcpy(exec_inf->c_used_alias, argv[1]);
	
	// Save the values.
	exec_inf->c_values = (char **) calloc(command->c_argc + 1, sizeof(char *));
	if (exec_inf == NULL) return FAILED_TO_MAKE_EXEC_INFO;

	
	for (int curr_opt = 0; curr_opt < command->c_argc; curr_opt++)
	{
		exec_inf->c_values[curr_opt] = calloc(strlen(argv[curr_opt + 1]) + 1, sizeof(char));
		if (exec_inf->c_values[curr_opt] == NULL) return FAILED_TO_MAKE_EXEC_INFO;
		strcpy(exec_inf->c_values[curr_opt], argv[curr_opt + 1]);
	}	

	exec_inf->c_values[command->c_argc - 1] = NULL; // Null terminate.
	// Call the callback with the values.
	command->c_call_back(exec_inf);
}


