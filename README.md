# terminal-tool-builder-lib 
This library can be used to create terminal tools easily and quickly. 

# Installation requirements
The following packages must be installed.<br>
```
sudo apt-get install build-essential cmake git gcc
```

# Download & Build

First download the program from GitHub and go to the terminal-tool-builder-lib folder.

```
% git clone https://github.com/rounnus/terminal-tool-builder-lib.git
% cd terminal-tool-builder-lib/
```

After installation the program must be built. In order to build the program, the following instructions must be
followed.<br>

```
% mkdir build
% cd build/
% cmake ../
% make
% sudo make install // This does not work at the time. But i will make it soon.
```

`
The cmake should be from version 3.21 and above, if you do not have this version please go to the following site and download the latest version:
https://cmake.org/download/
`

After this the program will be installed and ready to run.

# Documentation
This library is designed to make it easy to create tools that run on the terminal. Below it will be analyzed how it can be used.

## Library Macros

### Errors
```C
#define TOOL_BUILDER_WRONG_ARG_NUM              -1	// Wrong argument number.
#define TOOL_BUILDER_WRONG_NAME_OR_ALIAS        -2      // Wrong name or alias.

#define TOOL_BUILDER_EMPTY_NAME	                -3	// No command has been requested.

#define TOOL_BUILDER_NO_ACTION_DEFINED	        -4	// No function has been defined for the specific command.
#define TOOL_BUILDER_FAILED_TO_ADD              -5	// Failed to add an element to the builder.

#define TOOL_BUILDER_NO_SUCH_COMMAND_EXISTS	-7	// The command that was requested does not exists.
```

### Version
```C
#define TOOL_BUILDER_V 	1.6

```

## Library Structures

### tool_builder
The builder structure is the most basic element of the library. It contains all the necessary information for the construction of the requested tool.
#### Stracture definition
```C
struct tool_builder 
{
	struct tool_builder_command *t_commands;		// array of commands.
	int t_commandsc;					// The number of the commands.
	struct tool_builder_help t_help;			// The help of the tool.
	int t_mc: 1;						// enable or disable multiple commands in one line.
};
```
**The user should not be interested in the above definition.**

### tool_builder_args
The parameter structure is the structure that is "sent" to the user function that executes a command. It is used so that the user (**tool developer**) receives the necessary information about the command that was executed.
#### Structure definition
```C
struct tool_builder_args 
{
	char *c_name;						// The name of the command that has been executed.
	char *c_used_alias;					// The alias that has been used.
	char *(*c_values);					// The values that has been retrieved.
	int c_argc;						// The arguments of the command.
	struct tool_builder *c_builder;				// The builder.
};
```
**Above is the information he receives.**

## Library functions

___
### tool_builder_init
___
#### Description
The **tool_builder_init** function initializes the tool builder. <br>

#### Function signature
```C
void tool_builder_init(struct tool_builder *c_builder);
```

#### Arguments
`c_builder` Is a pointer to the builder to be used.

#### Return
--

#### Errors
--

#### Example
```C
struct tool_builder builder;
tool_builder_init(&builder);
```
___
### tool_builder_destroy
___
#### Description
The **tool_builder_destroy** function frees the memory that the builder has reserved. It should be called when the builder is no longer useful.

#### Function signature
```C
void tool_builder_destroy(struct tool_builder *c_builder);
```

#### Arguments
`c_builder` Is pointer to the builder used and no longer needed.
#### Return
--

#### Errors
--

#### Example
```C
struct tool_builder builder;
tool_builder_init(&builder);
... code ...
tool_builder_destroy(&builder);
```

___
### tool_builder_add_command
___
#### Description
The **tool_builder_add_command** function adds a new command to the tool. This means that if a user using the tool you created calls the command name specified in this function, a certain action that is also defined in this function will be performed.

#### Function signature
```C
int tool_builder_add_command(struct tool_builder *c_builder, const char *c_name, 
                             int c_argc, void (*c_callback)(const struct tool_builder_args *info));
```

#### Arguments
`c_builder` Is a pointer to the builder to be used.<br>
`c_name` The name of the command.<br>
`c_argc` The number of parameters required by the command.<br>
`c_callback` The action to be taken if the user requests this command. The function that plays the role of the command action must have the below signature 
```C
void your_action(const struct tool_builder_args *info);
```
All information about the command inputs is given as a parameter in this action. Check **tool_builder_args** struct for more details.

#### Return
It returns zero when everything went well. In the event of an error, one of the following may be returned.

#### Errors
`TOOL_BUILDER_FAILED_TO_ADD` Failed to add the new command.

#### Example
```C
void testing_command_action(const struct tool_builder_args *info)
{
	// code.
}

int main(int argc, char *argv[])
{
	struct tool_builder builder;
	tool_builder_init(&builder);
	tool_builder_add_command(&builder, "Testing", 2, &testing_command_action);
	// code.
	tool_builder_destroy(&builder);
}
```

___
### tool_builder_add_alias
___
#### Description
The **tool_builder_add_alias** function looks like the tool_builder_add_command function. The difference is that instead of adding a new command, it adds to a specific command 1, 2, .... , n names that describe the same command. Such names could be abbreviations, in order to make it easier for the user to remember.

#### Function signature
```C
int tool_builder_add_alias(struct tool_builder *c_builder, const char *c_name, const char *c_alias, ...);
```

#### Arguments
`c_builder` Is a pointer to the builder to be used.<br>
`c_name` The name of the command to add the aliases.<br>
`c_alias` The multitude of aliases. The last alias must be NULL.

#### Return
It returns zero when everything went well. In the event of an error, one of the following may be returned.

#### Errors
`TOOL_BUILDER_NO_SUCH_COMMAND_EXISTS` The command to which it went to add the aliases does not exist.<br>
`TOOL_BUILDER_FAILED_TO_ADD` Failed to add the aliases.

#### Example
```C
void testing_command_action(const struct tool_builder_args *info)
{
	// code.
}

int main(int argc, char *argv[])
{
	struct tool_builder builder;
	tool_builder_init(&builder);
	tool_builder_add_command(&builder, "Testing", 2, &testing_command_action);
	// add the aliases.
	tool_builder_add_alias(&builder, "Testing", "Test", "t", "T", NULL);
	// code.
	tool_builder_destroy(&builder);
}
```

___
### tool_builder_set_actions
___

#### Description
The **tool_builder_set_action** function changes or sets an action on a command.

#### Function signature
```C
int tool_builder_set_action(struct tool_builder *c_builder, const char *c_name, 
                            void (*c_callback)(const struct tool_builder_args *info));

```

#### Arguments
`c_builder` Is a pointer to the builder to be used.<br>
`c_name` The name of the command that should perform this action.<br>
`c_callback` The action to be performed on the command.

#### Return
It returns zero when everything went well. In the event of an error, one of the following may be returned.

#### Errors
`TOOL_BUILDER_NO_SUCH_COMMAND_EXISTS` The command `c_name` does not exists in the builder.

#### Example
```C
void testing_command_action(const struct tool_builder_args *info)
{
	// code.
}

int main(int argc, char *argv[])
{
	struct tool_builder builder;
	tool_builder_init(&builder);
	// adding a command without setting an action.
	tool_builder_add_command(&builder, "Testing", 2, NULL);
	// set the action.
	tool_builder_set_action(&builder, "Testing", &testing_command_action);
	
	// code.
	tool_builder_destroy(&builder);
}

```


___
### tool_builder_prepare
___

#### Description
The **tool_builder_prepare** function makes all the necessary preparations in the builder to execute the command(s) requested by the terminal.

#### Function signature
```C
int tool_builder_prepare(int argc, char *argv[], const struct tool_builder *c_builder);

```

#### Arguments
`argc` The number of parameters given in the executable. Given by the main function.<br>
`argv` Pointer that pointes to the first parameter. Given by the main function.<br>
`c_builder` Is a pointer to the builder to be used.

#### Return
It returns zero when everything went well. In the event of an error, one of the following may be returned.

#### Errors
`TOOL_BUILDER_EMPTY_NAME` No executable parameters were given<br>
`TOOL_BUILDER_NO_SUCH_COMMAND_EXISTS` The parameter given to the executable does not correspond to a command.<br>
`TOOL_BUILDER_NO_ACTION_DEFINED` The requested command does not have a specified action.<br>
`TOOL_BUILDER_WRONG_ARG_NUM` The parameters provided are not sufficient for the requested command.

#### Example
```C
void testing_command_action(const struct tool_builder_args *info)
{
	// code.
}

int main(int argc, char *argv[])
{
	struct tool_builder builder;
	tool_builder_init(&builder);
	tool_builder_add_command(&builder, "Testing", 2, &testing_command_action);
	// prepare for execution.
	tool_builder_prepare(argc, argv, &builder);
	// code.
	tool_builder_destroy(&builder);
}

```


___
### tool_builder_execute
___

#### Description
The **tool_builder_execute** function is responsible for executing any command requested by the terminal in the executable. If more than one command is given, it will be executed in the order given, from left to right.

#### Function signature
```C
void tool_builder_execute();
```

#### Arguments
--

#### Return
--

#### Errors
--

#### Example
```C
void testing_command_action(const struct tool_builder_args *info)
{
	// code.
}

int main(int argc, char *argv[])
{
	struct tool_builder builder;
	tool_builder_init(&builder);
	tool_builder_add_command(&builder, "Testing", 2, &testing_command_action);
	// prepare for execution.
	tool_builder_prepare(argc, argv, &builder);
	// exete the commands.
	tool_builder_execute();
	// code.
	tool_builder_destroy(&builder);
}
```


___
### tool_builder_call_command
___

#### Description
The **tool_builder_call_command** function allows the tool developer to execute a tool command without the user requesting it from the terminal.

#### Function signature
```C
int tool_builder_call_command(const char *c_name, const struct tool_builder *c_builder);
```

#### Arguments
`c_name` The name of the command to be executed.<br>
`c_builder` The builder who has the information for this command.

#### Return
It returns zero when everything went well. In the event of an error, one of the following may be returned.

#### Errors
`TOOL_BUILDER_WRONG_NAME_OR_ALIAS` There is no command with this name.
#### Example
```C
void testing_command_action(const struct tool_builder_args *info)
{
	// code.
}

int main(int argc, char *argv[])
{
	struct tool_builder builder;
	tool_builder_init(&builder);
	tool_builder_add_command(&builder, "Testing", 2, &testing_command_action);
	tool_builder_init_help(&c_builder, "your-tool-name");
	// prepare for execution.
	if (tool_builder_prepare(argc, argv, &builder) == TOOL_BUILDER_NO_SUCH_COMMAND_EXISTS)
	{
		// call the --help command.
		tool_builder_call_command("--help", &builder);
	}
	// code.
	tool_builder_destroy(&builder);
}
```


___
### tool_builder_init_help
___

#### Description
The **tool_builder_init_help** function generates a --help command along with the -h aliases and adds it to the tool with a default action. This action can be changed if you do not please the developer.

#### Function signature
```C
int tool_builder_init_help(struct tool_builder *c_builder, const char *tool_name);
```

#### Arguments
`c_builder` Is a pointer to the builder to be used.<br>
`tool_name` The name of the tool.

#### Return
It returns zero when everything went well. In the event of an error, one of the following may be returned.

#### Errors
`TOOL_BUILDER_FAILED_TO_ADD` Failed to add the new command or alias.

#### Example
```C
void testing_command_action(const struct tool_builder_args *info)
{
	// code.
}

int main(int argc, char *argv[])
{
	struct tool_builder builder;
	tool_builder_init(&builder);
	tool_builder_add_command(&builder, "Testing", 2, &testing_command_action);
	// initialize the pre build help command.
	tool_builder_init_help(&builder, "your-tool-name");
	
	tool_builder_destroy(&builder);
}
```


___
### tool_builder_set_desc
___
#### Description
The **tool_builder_set_desc** function sets a description of the tool, which will appear when someone calls the --help command.

#### Function signature
```C
int tool_builder_set_desc(struct tool_builder *c_builder, const char *c_description);
```

#### Arguments
`c_builder` Is a pointer to the builder to be used.<br>
`c_description` A general description of the tool.

#### Return
It returns zero when everything went well. In the event of an error, one of the following may be returned.

#### Errors
`TOOL_BUILDER_FAILED_TO_ADD` Failed to add description.

#### Example
```C
void testing_command_action(const struct tool_builder_args *info)
{
	// code.
}

int main(int argc, char *argv[])
{
	struct tool_builder builder;
	tool_builder_init(&builder);
	tool_builder_add_command(&builder, "Testing", 2, &testing_command_action);
	// initialize the pre build help command.
	tool_builder_init_help(&builder, "your-tool-name");
	// set description.
	tool_builder_set_desc(&builder, "A general description of your tool.");
	
	tool_builder_destroy(&builder);
}
```


___
### tool_builder_add_command_doc
___

#### Description
The **tool_builder_add_command_doc** function adds the name of a command, along with its accompanying description to the tool instructions. These instructions can be displayed using the --help command.

#### Function signature
```C
int tool_builder_add_command_doc(struct tool_builder *c_builder, const char *c_name,
                                 const char *c_description);

```

#### Arguments
`c_builder` Is a pointer to the builder to be used.<br>
`c_name` The name of the command to add.<br>
`c_description` The description of the command.<br>

#### Return
It returns zero when everything went well. In the event of an error, one of the following may be returned.

#### Errors
`TOOL_BUILDER_FAILED_TO_ADD` Failed to add the command or description.

#### Example
```C
void testing_command_action(const struct tool_builder_args *info)
{
	// code.
}

int main(int argc, char *argv[])
{
	struct tool_builder builder;
	tool_builder_init(&builder);
	tool_builder_add_command(&builder, "Testing", 2, &testing_command_action);
	// initialize the pre build help command.
	tool_builder_init_help(&builder, "your-tool-name");
	// set description.
	tool_builder_set_desc(&c_builder, "A general description of your tool.");
	// add testing command to docs.
	tool_builder_add_command_doc(&builder, "Testing", "Command description");
	
	tool_builder_destroy(&builder);
}
```


___
### tool_builder_add_alias_doc
___

#### Description
The **tool_builder_add_command_doc** function adds the aliases of a command to the tool instructions. These instructions can be displayed using the --help command. The aliases do not need to be given again because they already exist in the builder. They are in the command structure.

#### Function signature
```C
int tool_builder_add_alias_doc(struct tool_builder *c_builder, const char *c_name);
```

#### Arguments
`c_builder` Is a pointer to the builder to be used.<br>
`c_name` The name of the command that has these aliases.

#### Return
It returns zero when everything went well. In the event of an error, one of the following may be returned.

#### Errors
`TOOL_BUILDER_NO_SUCH_COMMAND_EXISTS` The command does not exists.

#### Example
```C
void testing_command_action(const struct tool_builder_args *info)
{
	// code.
}

int main(int argc, char *argv[])
{
	struct tool_builder builder;
	tool_builder_init(&builder);
	tool_builder_add_command(&builder, "Testing", 2, &testing_command_action);
	// initialize the pre build help command.
	tool_builder_init_help(&builder, "your-tool-name");
	// set description.
	tool_builder_set_desc(&c_builder, "A general description of your tool.");
	// add testing command to docs.
	tool_builder_add_command_doc(&builder, "Testing", "Command description");
	// add aliases to docs.
	tool_builder_add_alias_doc(&builder, "Testing");
	
	tool_builder_destroy(&builder);
}
```



___
### tool_builder_set_closing_desc
___

#### Description
The tool_builder_set_closing_desc function sets another description, which appears at the end of the document that appears when the --help command is executed.

#### Function signature
```C
int tool_builder_set_closing_desc(struct tool_builder *c_builder, const char *close_description);
```

#### Arguments
`c_builder` Is a pointer to the builder to be used.<br>
`close_description` Something like an epilogue to the reference to a source.

#### Return
It returns zero when everything went well. In the event of an error, one of the following may be returned.

#### Errors
`TOOL_BUILDER_FAILED_TO_ADD` Failed to add closing description.

#### Example
```C
void testing_command_action(const struct tool_builder_args *info)
{
	// code.
}

int main(int argc, char *argv[])
{
	struct tool_builder builder;
	tool_builder_init(&builder);
	tool_builder_add_command(&builder, "Testing", 2, &testing_command_action);
	// initialize the pre build help command.
	tool_builder_init_help(&c_builder, "your-tool-name");
	// set description.
	tool_builder_set_desc(&builder, "A general description of your tool.");
	// set closing description.
	tool_builder_set_closing_desc(&builder, "For more informations look at https://example.com");
	
	tool_builder_destroy(&builder);
}
```



___
### tool_builder_add_both
___

#### Description


#### Function signature
```C
int inline tool_builder_add_both(struct tool_builder *c_builder, const char *c_name,
                                 int c_argc, void (*c_callback)(const struct tool_builder_args *info),
                                 const char *c_description)

```

#### Arguments

#### Return

#### Errors

#### Example
```C

```

___
### tool_builder_add_alias_both
___

#### Description


#### Function signature
```C
int inline tool_builder_add_alias_both(struct tool_builder *c_builder, const char *c_name, 
                                       const char *c_alias, ...)

```

#### Arguments

#### Return

#### Errors

#### Example
```C

```



## Example
An example of the library in use can be found in the **tests** folder

## Done!
This is my library, I hope you find it useful. Any contribution or any proposal for change is acceptable!! 
