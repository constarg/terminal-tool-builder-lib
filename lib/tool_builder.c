#include <string.h>

#include <tool_builder.h>


static inline struct command_d *find_command(const struct command_d **commands, const char *c_name);


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

/**
	Check if al the elements of the array is valid
	strings.
	@param 
	@return The valid array.
*/
static char **cleanup_alias(const char c_alias[5][256])
{
	char **tmp_alias = malloc(sizeof(char *) * 5);

	for (int i = 0; i < 5; i++)
	{
		tmp_alias[i] = NULL;
		for (int c_a = 0; c_a < 256; c_a++)
		{
			if (c_alias[i][c_a] == '\0') 
			{
				tmp_alias[i] = (char *) &c_alias[i][0];
				break;
			}
		}
	}
	return tmp_alias;
}

int add_command(struct builder_d *c_builder, const char c_name[256], int c_argc, const char c_alias[5][256], 
		 void (*c_call_back)(const struct exec_info *info))
{
	if (c_builder == NULL) return BUILDER_IS_NOT_INITIALIZED;

	// Allocate memory for the new command.
	int last_c = c_builder->b_commandsc;
	struct command_d *new_command = malloc(sizeof(struct command_d));
	struct command_d **commands = c_builder->b_commands;
	strcpy(new_command->c_name, c_name);
	// Set the alias.
	char **tmp = cleanup_alias(c_alias); 	

	for (int c_a = 0; c_a < 5; c_a++)
	{
		if (tmp[c_a] == NULL) continue;
		strcpy(new_command->c_alias[c_a], tmp[c_a]);
	}	

	commands = realloc(commands, sizeof(struct command_d *) * (last_c + 2));
	free(tmp);
}


int add_action(struct builder_d *c_builder, const char *c_name,
	       void (*c_call_back)(const struct exec_info *info))
{
	if (c_builder == NULL || c_builder->b_commands == NULL) return BUILDER_IS_NOT_INITIALIZED;

	struct command_d *ch_command = find_command((const struct command_d **) c_builder->b_commands, c_name);
	if (ch_command == NULL) return NO_SUCH_COMMAND_EXISTS;

	ch_command->c_call_back = c_call_back;	// Set the requested callback.
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
			for (int curr_a = 0; curr_c < 5 && curr_c; curr_a++)
				if (!strcmp(commands[curr_c]->c_alias[curr_a], c_name)) return (struct command_d *) commands[curr_c]; 
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
	struct command_d *command = find_command((const struct command_d **) c_builder->b_commands, argv[1]);

	if (command == NULL) return WRONG_NAME_OR_ALIAS;

	if (((argc - 1) - command->c_argc) < 0) return WRONG_ARG_NUM;

	if (command->c_call_back == NULL) return NO_ACTION_DEFINED;

	// Build the info to recieve the call back.
	struct exec_info exec_inf;
	strcpy(exec_inf.c_name, command->c_name);
	strcpy(exec_inf.c_used_alias, argv[1]);
	exec_inf.c_values = (argv + 2);	// This points to the first argument of the requested command. 

	// Call the callback with the values.
	command->c_call_back(&exec_inf);
}


