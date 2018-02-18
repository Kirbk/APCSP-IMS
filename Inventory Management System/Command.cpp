#include "Command.h"



Command::Command()
{
}

Command::Command(std::function<void()> fun)
{
	std::vector<std::string> args;
	std::function<void(std::vector<std::string>)> newFunc = [fun](std::vector<std::string> args) { fun(); };
	m_function = newFunc;
}

Command::Command(std::function<void(std::vector<std::string> args)> fun)
{
	m_function = fun;
}

Command::~Command()
{
}

void Command::execute() {
	std::vector<std::string> args;
	m_function(args);
}

void Command::execute(std::vector<std::string> args)
{
	m_function(args);
}
