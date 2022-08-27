#include <tool_builder.h>


void testing_command_action(const struct tool_builder_args *info)
{
	printf("Command_name: %s\n", info->c_name);
	printf("Used alias: %s\n", info->c_used_alias);

	printf("Arguments: ");
	for (int i = 0; i < info->c_argc; i++)
		printf("%s, ", info->c_values[i]);	
	printf("\n");

	printf("Argument number: %d\n", info->c_argc);

}


void testing_2_command_action(const struct tool_builder_args *info)
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
	struct tool_builder builder;	
	tool_builder_init(&builder);


	tool_builder_init_help(&builder, "Testing-tool");

	// Add description to the docs of help.
	tool_builder_set_desc(&builder, "This is a test of a description");	
  	tool_builder_set_closing_desc(&builder, "hello");	
	
	// Add a new command to the docs of help.
    tool_builder_add_command_doc(&builder, "Testing", "This command is a test command");

	tool_builder_add_command(
		&builder,
		"Testing",
		2,
		&testing_command_action	
	);

	tool_builder_add_command(
		&builder,
		"Something",
		3,
		&testing_2_command_action
	);

	tool_builder_add_alias(&builder, "Testing", "Test", "t", "te", NULL);
        tool_builder_add_alias_doc(&builder, "Testing");

        //tool_builder_call_command("--help", &builder);

	//tool_builder_set_mc(&builder, 0);

	int error = tool_builder_prepare(argc, argv, &builder);
	tool_builder_execute();

	printf("Error = %d\n", error);

	tool_builder_destroy(&builder);	
}
