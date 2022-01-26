#include <tool_builder.h>




void initialize_help(struct builder_d *c_builder, const char *tool_name)
{

}

void add_full_help(struct builder_d *c_builder, const char *help)
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


void add_command(struct builder_d *c_builder, const char c_name[256], int c_argc, const char c_alias[256][5], 
		 void (*c_call_back)(const struct exec_info *info))
{

}


void add_action(struct builder_d *c_builder, void (*c_call_back)(const struct exec_info *info))
{

}


void execute_command(int argc, char **argv, const struct builder_d *c_builder)
{


}


