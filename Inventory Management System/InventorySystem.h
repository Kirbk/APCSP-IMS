#pragma once

#include <string>
#include <map>
#include <algorithm>

#include "Command.h"

struct Item {
	std::string name;
	unsigned int quantity;
	double cost;
};

class InventorySystem
{
public:
	InventorySystem();
	~InventorySystem();

	void init(std::string name);
	void loadCompany(std::string companyName);
	void saveCompany(std::string companyName);
	
	void run();

	void updateConsole();
	void addCommand(std::string label, Command* command);

	std::string make_lowercase(const std::string& in) {
		std::string out;
		std::transform(in.begin(), in.end(), std::back_inserter(out), ::tolower);
		return out;
	}

private:
	std::string m_name;

	std::map<std::string, Item*> m_items;
	std::map<std::string, Command*> m_commandMap;
};

