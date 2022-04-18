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

# Docs

Below will be analyzed how the library can be used.<br><br>

## Initializing the builder
A variable with type tool_builder can be defined like below
```C
struct tool_builder builder;
``` 
To start building you need to initialize the above variable as shown below.
```C
tool_builder_init(&builder);
```
Done! now your builder is ready to build your tool!.

## Initialize help
The help command is the classic command that all terminal-based tools have. The library has the feature to create automatically this commmand based on the tool's commands. Or the user can ignore it and make his own help command.
To enable this feature, initiaize the help command as below.
```C
tool_builder_init_help(&builder, "tool-name");
```
The `tool_builder_init_help` function takes as the first parameter the builder and as a second parameter the name of your tool.

## Build the help docs
Putting your own command in the docs of the help command helps the user who will use your tool to understand it.<br>
The below function will allow you to add a description in your help docs.
```C
tool_builder_set_desc(&builder, "Your description");	
```
The `tool_builder_set_desc` function require as the first parameter the builder and as a second parameter the description of your docs.<br>
The below function will allow you to add a closure description, if you like to, in your help docs.
```C
tool_builder_set_closing_desc(&builder, "Your closure description");	
```
The `tool_builder_set_closing_desc` function require as the first parameter the builder and as a second parametr the closure description you like.<br>
The below function will allow you to add a command, you support in your tool, in the help docs.
```C
tool_builder_add_command_doc(&builder, "your_command_name", "The description of your command");
```
The `tool_builder_add_command_doc` takes 3 parameters. The first is the builder, the second is the name of your command and the third is the description of your  command.<br>
The below function will allow you to add alias to an existing command in the docs.
```C
tool_builder_add_alias_doc(&builder, "your_command_name");
```
The `tool_builder_add_alias_doc` the first parameter of this function is the builder and the second parameter is the name of the command to add the aliases.<br>
**Caution! the aliases are the same as the ones you set when creating the command, will be explained below, so you do not need to re-enter them. That is why they are not requested.**<br><br>

**Caution!! if you want to make your own version of the help command the above will not work. The docs will have to be made differently.**

## Commands
To set a new command for the tool you are creating, all you have to do is call the following function. 
```C
tool_builder_add_command(&builder, "command_name", arg, action);
            		   ^           ^            ^      ^
        		The builder  Command       args   action
```
The `tool_builder_add_command` function takes 4 arguments. The first is the builder, the second is the name of your command, the third is the argumets that your command require in order to run and the forth is the action, callback, to call when this command is called from the terminal. The action is a ballback that is has the below signature.
```C
void (*c_callback)(const struct tool_builder_args *info)
```
When your command is called from the terminal, the library will return to you the below infos, in the variable info.
```C
struct tool_builder_args 
{
	char *c_name;						// The name of the command that has been executed.
	char *c_used_alias;					// The alias that has been used.
	char *(*c_values);					// The values that the been retrieved. Must be freed when there is no more use.
	int c_argc;						// The arguments of the command.
	struct tool_builder *c_builder;				// The builder.
};
```
With the above function your command will be executed when the user types the `command_name`. But you can also add a few alias
to help the user and not make him remember a long name.
In order to add alias to a command you have to call the below function.
```C
tool_builder_add_alias(&builder, "command_name", "alias_1", "alias_2", "alias_3", NULL);
```
The `tool_builder_add_alias` takes as first parameter the builder, as the second parameter the the command you want to add the aliases and as the third parameter can take an unlimited number of aliases.
**Cation!!! the last alias must be NULL!**

## Command Action

If you want to enter or change the action of a command later you can do so using the following function. 
```C
tool_builder_set_action(&builder, "command_name", action);
```
The `tool_builder_set_action` function takes three parameters. The first parameter is the builder, the second is the command you want to add the action and the third is the action you want to add.

## Shortcuts
Some of the above functions can be done a bit faster by using only one function, instead of two. 
Το add a command and at the same time add it in the docs you can use the below function.
```C
tool_builder_add_both(&builder, "command_name", args, action, "description")
```
The `tool_builder_add_both` function has 5 parameters. The first is the builder, the second is the name of the command to add, the third is the action to take when the command is called and the fifth is the description to add in the docs, for this specific command.

Α similar shortcut applies to alias.
```C
tool_builder_add_alias_both(&builder, "command_name", "alias_1", "alias_2", "alias_3", NULL)
```
The `tool_builder_add_alias_both` function takes 2 parameters and an unlimited number of aliases. The first parameter is the builder, the second is the name of the command you want to add the aliases and the rest is the aliases. The defferent with this function is that it will also add the aliases in the docs.<br>
**Cation!!! the last alias must be NULL!**

## Execution
To execute the command that the user has typed you have to call the below function in the main.
```C
tool_builder_execute(argc, argv, &builder);
```
The `tool_builder_execute` function has 3 parameters. The first parameter is the argc of main, the second is the argv of main and the third is the builder.
This function is responsible for the determination of what command has been given from the terminal and also is the function that calls the callback of the command thet has been requested.

## Destroying
Once you have completed all the procedures you have to do with the builder you should free up the memory that the library is using. To do this you can call the following function. 
```C
tool_builder_destroy(&builder);
```
The `tool_builder_destroy` function free's the memory that has been allocated for the builder. It has only one parameter and is the builder. 

## Example
An example of the library in use can be found in the **tests** folder

## Done!
This is my library, I hope you find it useful. Any contribution or any proposal for change is acceptable!! 
