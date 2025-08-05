#pragma once

#ifndef CONFIG_H
#define CONFIG_H

#include <fstream>
#include <string>
#include <cassert>
#include <unordered_map>

class ConfigReader {
public:
	void read(std::string fname) {
		std::ifstream infile(fname);
		assert(infile.is_open());
		std::string key, value;

		while (infile >> key >> value) { dict[key] = value; }
		
		assert(dict.find("seed") != dict.end());
		assert(dict.find("A") != dict.end());
		assert(dict.find("V") != dict.end());
		
		assert(dict.find("startsize") != dict.end()); 


		assert(dict.find("lambda") != dict.end()); 
		assert(dict.find("alpha") != dict.end());
		assert(dict.find("epsilon") != dict.end());
				
		assert(dict.find("steps") != dict.end()); 
		assert(dict.find("thermal") != dict.end()); 
		assert(dict.find("sweeps") != dict.end()); 
		
		assert(dict.find("name") != dict.end()); 

	}

	int getInt(std::string key) { return std::stoi(dict[key]); }

	double getDouble(std::string key) { return std::stod(dict[key]); }

	std::string getString(std::string key) { return dict[key]; }

private:
	std::unordered_map<std::string, std::string> dict;
};


#endif
