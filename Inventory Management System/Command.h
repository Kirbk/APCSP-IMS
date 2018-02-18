#pragma once

#include <vector>
#include <functional>
#include <string>

class Command
{
public:
	Command();
	Command(std::function<void()> fun);
	Command(std::function<void(std::vector<std::string> args)> fun);
	virtual ~Command();

	virtual void execute();
	virtual void execute(std::vector<std::string> args);

protected:
	std::function<void(std::vector<std::string> args)> m_function;
};

