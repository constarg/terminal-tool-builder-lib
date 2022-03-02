#include <tool_builder.h>


void testing_command_action(const struct exec_info *info)
{
	printf("Command_name: %s\n", info->c_name);
	printf("Used alias: %s\n", info->c_used_alias);

	printf("Arguments: ");
	for (int i = 0; i < info->c_argc; i++)
		printf("%s, ", info->c_values[i]);	
	printf("\n");

	printf("Argument number: %d\n", info->c_argc);

}

void testing_2_command_action(const struct exec_info *info)
{
	printf("Command_name: %s\n", info->c_name);
	printf("Used alias: %s\n", info->c_used_alias);

	printf("Arguments: ");
	for (int i = 0; i < info->c_argc; i++)
		printf("%s, ", info->c_values[i]);	
	printf("\n");

	printf("Argument number: %d\n", info->c_argc);
}


int main(int argc, char *argv[]) {
	// tests.
	struct builder_d *builder;	
	initialize_builder(&builder);


	initialize_help(builder, "Testing-tool");

	// Add description to the docs of help.
	add_help_tool_description(builder, "This is a test of a description");	
	
	
	// Add a new command to the docs of help.
	add_help_tool_command(builder, "Testing", "This command is a test command");

	add_command(
		builder,
		"Testing",
		2,
		&testing_command_action	
	);

	add_command(
		builder,
		"Something",
		3,
		&testing_2_command_action
	);


	add_command_alias(builder, "Testing", "Test", "t", "te", NULL); 
	add_help_tool_alias(builder, "Testing");
	

	int error = execute_command(argc, argv, builder);
	printf("Error = %d\n", error);

	destroy_builder(&builder);	
}
