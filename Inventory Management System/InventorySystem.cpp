#include "InventorySystem.h"

#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <sstream>
#include <iomanip> // setprecision
#include <sstream> // stringstream

InventorySystem::InventorySystem()
{
	// Empty
}


InventorySystem::~InventorySystem()
{
}

void InventorySystem::init(std::string name)
{
	// Initialize commands
	Command* saveCommand = new Command([this](std::vector<std::string> args) {
		if (args.size() == 1)
			m_name = args[0];

		saveCompany(m_name);
		printf("Company <%s> saved!\n", m_name.c_str());
	});

	Command* addItemCommand = new Command([this](std::vector<std::string> args) {
		if (args.size() == 3) {
			if (m_items.find(make_lowercase(args[0])) == m_items.end()) {
				Item* item = new Item();

				item->name = make_lowercase(args[0]);
				item->quantity = abs(std::stof(args[1]));
				item->cost = abs(std::stof(args[2]));

				m_items[args[0]] = item;

				printf("%i of item <%s> added for the price of $%.2f each\n", item->quantity, item->name.c_str(), item->cost);
			}
			else {
				printf("Item <%s> already exists, try the modify command\n", args[0]);
			}
		}
		else if (args.size() > 3) {
			printf("Too many arguments!\n");
			printf("Usage: additem <name> <quantity> <cost>\n");
		}
		else {
			printf("Not enought arguments!\n");
			printf("Usage: additem <name> <quantity> <cost>\n");
		}
	});

	Command* removeItemCommand = new Command([this](std::vector<std::string> args) {
		if (args.size() == 0) {
			printf("No items were removed.\n");
			printf("Usage: removeitem [item names...]\n");
		}
		else {
			for (std::string item : args) {
				std::map<std::string, Item*>::iterator it;

				it = m_items.find(make_lowercase(item));
				if (it != m_items.end()) {
					m_items.erase(it);
					printf("Removed item <%s>\n", item.c_str());
				}
				else {
					printf("Item <%s> could not be removed!\n", item.c_str());
				}
			}
		}
	});

	Command* listCommand = new Command([this]() {
		printf("\nName                Quantity            Cost\n");
		printf("============================================================\n");
		for (auto const& imap : m_items) {
			// companyFile << imap.second->name << ' ' << imap.second->quantity << ' ' << imap.second->cost << '\n';
			std::string name = imap.second->name;
			std::string quantity = std::to_string(imap.second->quantity);

			std::stringstream ss;
			ss << std::fixed << std::setprecision(2) << imap.second->cost;
			std::string costString = ss.str();

			std::string cost = "$" + costString;

			printf("%20s%20s%20s\n", name.c_str(), quantity.c_str(), cost.c_str());
		}

		printf("\n");
	});

	Command* modifyCommand = new Command([this](std::vector<std::string> args) {
		if (args.size() == 3) {
			if (m_items.find(args[0]) != m_items.end()) {
				if (make_lowercase(args[1]) == "name") {
					m_items.find(args[0])->second->name = args[2];
				}
				else if (make_lowercase(args[1]) == "quantity") {
					m_items.find(args[0])->second->quantity = atoi(args[2].c_str());
				}
				else if (make_lowercase(args[1]) == "cost") {
					m_items.find(args[0])->second->cost = std::stof(args[2]);
				}
				else {
					printf("Invalid attribute, choose [name, quantity, cost]");
				}
			}
			else {
				printf("Item <%s> could not be found!\n", args[0].c_str());
			}
		}
		else {
			printf("Usage: modify <item name> <attribute> <value>\n");
		}
	});

	Command* taxCommand = new Command([this](std::vector<std::string> args) {
		if (args.size() == 1) {
			if (m_items.find(make_lowercase(args[0])) != m_items.end()) {
				float price = m_items.find(make_lowercase(args[0]))->second->cost;
				price *= 1.07f;

				printf("Item <%s> with a 7%% tax is $%.2f from the original price of $%.2f\n", args[0].c_str(), price, m_items.find(make_lowercase(args[0]))->second->cost); // SO SO SO memory inefficient but who cares :/
			}
			else {
				printf("Item <%s> could not be found!\n", args[0].c_str());
			}
		}
		else {
			printf("Usage: tax <item name>");
		}
	});

	Command* exitCommand = new Command([this]() {
		std::cout << "Are you sure? (Y/N) ";
		char choice;
		std::cin >> choice;

		if (choice == 'Y' || choice == 'y') {
			std::cout << "Do you wish to save the company, " << m_name << "? (Y/N) ";
			char save;
			std::cin >> save;

			if (save == 'Y' || save == 'y') {
				// Create new file for company
				saveCompany(m_name);
				printf("Company <%s> saved!\n", m_name.c_str());
				exit(0);
			}
			else {
				// Exit the program
				exit(0);
			}
		}
	});

	// Register commands
	addCommand("save", saveCommand);
	addCommand("additem", addItemCommand);
	addCommand("removeitem", removeItemCommand);
	addCommand("modify", modifyCommand);
	addCommand("list", listCommand);
	addCommand("tax", taxCommand);
	addCommand("exit", exitCommand);

	std::string companyName = make_lowercase(name);
	m_name = companyName;
	// fstream of company
	std::fstream companyFile(companyName + ".cmp");

	if (companyFile.good()) {
		// Company was found
		companyFile.close();
		loadCompany(companyName);
	}
	else {
		// Company was not found
		std::cout << "Do you wish to create the company, " << companyName << "? (Y/N) ";
		char choice;
		std::cin >> choice;

		if (choice == 'Y' || choice == 'y') {
			// Create new file for company
			saveCompany(companyName);
		}
		else {
			// Exit the program
			exit(0);
		}
	}

	run();
}

void InventorySystem::loadCompany(std::string companyName)
{
	for (auto current = m_items.begin(); current != m_items.end(); ++current) {
		delete current->second;
	}
	m_items.clear();

	std::ifstream companyFile(companyName + ".cmp");
	unsigned int itemSize = 0;
	companyFile >> itemSize;

	for (int i = 0; i < itemSize; i++) {
		Item* item = new Item();
		std::string itemName;
		int quantity;
		double cost;

		companyFile >> itemName >> quantity >> cost;
		item->name = itemName;
		item->quantity = abs(quantity);
		item->cost = abs(cost);
		m_items[itemName] = item;
	}
}

void InventorySystem::saveCompany(std::string companyName)
{
	std::ofstream companyFile(companyName + ".cmp");
	companyFile << m_items.size() << '\n';

	for (auto const& imap : m_items) {
		companyFile << imap.second->name << ' ' << imap.second->quantity << ' ' << imap.second->cost << '\n';
	}
}

void InventorySystem::run()
{
	bool isRunning = true;

	while (isRunning) {
		updateConsole();
	}
}

void InventorySystem::updateConsole()
{
	std::string label = "";
	std::cout << "[" << m_name << "] ";
	std::getline(std::cin, label);

	if (label != "") {
		std::istringstream iss(label);
		std::vector<std::string> tokens;

		for (std::string each; std::getline(iss, each, ' '); tokens.push_back(each));

		auto mit = m_commandMap.find(make_lowercase(tokens[0]));

		tokens.erase(tokens.begin());

		if (mit != m_commandMap.end()) {
			mit->second->execute(tokens);
		}
		else {
			printf("Unknown command! %s could not be found.\n", label.c_str());
		}
	}
}

void InventorySystem::addCommand(std::string label, Command * command)
{
	auto mit = m_commandMap.find(make_lowercase(label));

	if (mit == m_commandMap.end()) {
		//Insert it into the map
		m_commandMap.insert(make_pair(make_lowercase(label), command));
	}
}
