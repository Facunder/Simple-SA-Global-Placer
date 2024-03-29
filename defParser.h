#pragma once
#include<string.h>
#include<vector>
#include<unordered_map>
#include<fstream>
#include<iostream>
class defParser {
public:
	void parse(std::string defFile, std::vector<std::vector<int>>& nets, std::vector<std::string>& instsName, std::vector<int>& loc, std::vector<int>& dieBox);
};