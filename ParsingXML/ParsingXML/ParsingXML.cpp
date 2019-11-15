#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <string>
#include "../Utilities/rapidxml-1.13/rapidxml.hpp"

void printAll(rapidxml::xml_node<>* node, long long tabs);

void parseXmlFile(const char* filePath) {
	rapidxml::xml_document<>* doc = new rapidxml::xml_document<>;
	std::ifstream input{ filePath };

	if (!input.is_open()) {
		abort();
	}

	std::stringstream ss;
	ss << input.rdbuf();
	std::string content(ss.str());
	doc->parse<0>(&content[0]);

	printAll(doc->first_node(), 0);

	input.close();
	doc->clear();
	delete doc;
}

void printCh(char ch, long long times) {
	while (times)
	{
		std::cout << ch;
		--times;
	}
}

void printAll(rapidxml::xml_node<>* root, long long spaces) {
	for (auto node = root; node; node = node->next_sibling()) {
		printCh(' ', spaces); std::cout << node->name() << '\n';
		
		printCh(' ', spaces + 1);
		if (node->first_attribute()) {
			std::cout << "cu atributele: ";
			for (auto attrib = node->first_attribute(); attrib; attrib = attrib->next_attribute()) {
				std::cout << attrib->name() << "=" << attrib->value();
				if (attrib->next_attribute()) {
					std::cout << ", ";
				}
			}
		}
		else {
			std::cout << "fara atribute";
		}
		std::cout << std::endl; printCh(' ', spaces + 1);

		if (strlen(node->value()) > 0) {
			std::cout << "cu continutul: " << node->value() << std::endl;
		}
		else {
			std::cout << "cu fii:\n";

			for (auto child = node->first_node(); child; child = child->next_sibling()) {
				printAll(child, spaces + 2);
			}
		}
	}
}

int main()
{
	parseXmlFile("../Resources/resourceManager.xml");
}
