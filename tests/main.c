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


	char test_alias[5][256];
	char test_2_alias[5][256];
	// Initialize memory.
	initialize_alias(&test_alias);
	initialize_alias(&test_2_alias);

	initialize_help(builder, "Testing-tool");

	
	// One character aliases.
	test_alias[0][0] = 't';
	test_alias[1][0] = 'p';	

	test_2_alias[0][0] = 's';
	test_2_alias[1][0] = 'v';	

	// String aliases.
	strcpy(test_alias[2], "test");
	
	strcpy(test_2_alias[2], "Some");
	
	add_command(
		builder,
		"Testing",
		2,
		test_alias,
		&testing_command_action	
	);

	add_command(
		builder,
		"Something",
		3,
		test_2_alias,
		&testing_2_command_action
	);


	int error = execute_command(argc, argv, builder);
	printf("Error = %d\n", error);

	destroy_builder(&builder);	
}
