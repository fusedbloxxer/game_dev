#include "stdafx.h"
#include "Parser.h"
#include "Parser.h"
#include <fstream>
#include <filesystem>
#include <string>
#include <regex>
#include <sstream>
#include "../Utilities/rapidxml-1.13/rapidxml.hpp"

template<typename Type>
void fillContainer(std::vector<Type>& container, const char* format, std::ifstream& fs, int lines)
{
	Type temp;
	bool ok = false;
	std::smatch matches; std::stringstream ss;
	std::regex pattern{ format }; std::string str;

	while (lines >= 0)
	{
		// Get new line.
		std::getline(fs, str);

		while (std::regex_search(str, matches, pattern))
		{
			if (matches.size() > 0)
			{
				ss << matches[0] << std::string(" ");
			}
			str = matches.suffix().str();
		}

		if (ok)
		{
			ss >> temp;
			container.push_back(temp);
			// std::cout << ss.str() << std::endl;
		}
		else
		{
			ok = true;
		}
		ss.clear();

		--lines;
	}
}

template<>
void fillContainer<GLushort>(std::vector<GLushort>& container, const char* format, std::ifstream& fs, int lines)
{
	GLushort temp;
	bool ok = false;
	std::string str;
	std::stringstream ss;

	while (lines >= 0)
	{
		// Get new line.
		std::getline(fs, str);

		std::string const delims{ " .,:;!?" };

		size_t beg, pos = 0; int count = 1;
		GLint x = 0, y = 0, z = 0;

		while ((beg = str.find_first_not_of(delims, pos)) != std::string::npos)
		{
			pos = str.find_first_of(delims, beg + 1);
			std::string tem = str.substr(beg, pos - beg);

			if (count % 4 == 2)
			{
				ss << tem;
				ss >> x;
			}
			else if (count % 4 == 3)
			{
				ss << tem;
				ss >> y;
			}
			else if (count % 4 == 0)
			{
				ss << tem;
				ss >> z;
			}
			++count;
			ss.clear();
		}

		if (ok)
		{
			container.push_back(x);
			container.push_back(y);
			container.push_back(z);
		}
		else
		{
			ok = true;
		}
		--lines;
	}
}

std::pair<std::vector<Vertex_NFG>, std::vector<GLushort>> Parser::parseFile(const char* const filePath)
{
	std::ifstream input{ filePath };

	// std::cout << std::filesystem::current_path();

	if (!input.is_open())
	{
		throw std::exception{ "Could not open file." };
	}

	std::vector<Vertex_NFG> nfgs; std::vector<GLushort> indexes; 

	std::string str; int lines; input >> str >> lines;
	fillContainer(nfgs, "-{0,1}\\d{1,}\\.\\d{1,}", input, lines);
	input >> str >> lines;
	fillContainer(indexes, "\\d{1,}", input, lines / 3);
	input.close();

	return std::make_pair(nfgs, indexes);
}

void printAll(rapidxml::xml_node<>* node, long long tabs);

void parseXmlFile(const char* filePath) {

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

void Parser::xmlParse(const char* filePath)
{
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