#include "defParser.h"

void defParser::parse(std::string defFile, std::vector<std::vector<int>>& nets, std::vector<std::string>& instsName, std::vector<int>& loc, std::vector<int>& dieBox) {
	std::string tmp;
	std::ifstream ifs;
	ifs.open(defFile);
	std::cout << "Begin Read DEF: " << defFile << std::endl;
	// DIE
	while (ifs >> tmp) {
		if (tmp == "DIEAREA") break;
	}
	ifs >> tmp, ifs >> tmp;
	dieBox.resize(4, 0);
	dieBox[0] = atoi(tmp.c_str()); //xl
	ifs >> tmp;
	dieBox[1] = atoi(tmp.c_str()); //yb
	ifs >> tmp, ifs >> tmp, ifs >> tmp;
	dieBox[2] = atoi(tmp.c_str()); //xr
	ifs >> tmp;
	dieBox[3] = atoi(tmp.c_str()); //yt
	// COMPONENTS
	std::cout << "\tLoading COMPONENTS......" << std::endl;
	while (ifs >> tmp) {
		if (tmp == "COMPONENTS") break;
	}
	ifs >> tmp;
	int instsNum = atoi(tmp.c_str());
	ifs >> tmp;
	loc.clear();
	loc.resize(2 * instsNum, 0);
	std::unordered_map<std::string, int> tmpInstsMap;
	int inst_cnt = 0;
	while (ifs >> tmp) {
		if (tmp == "END") break;
		ifs >> tmp;
		std::string tmpName = tmp;
		ifs >> tmp, ifs >> tmp, ifs >> tmp;
		if (tmp == "UNPLACED") {
			instsName.emplace_back(tmpName);
			tmpInstsMap[tmpName] = inst_cnt;
			inst_cnt++;
		}
		ifs >> tmp;
	}
	// NETS
	std::cout << "\tLoading NETS......" << std::endl;
	while (ifs >> tmp) {
		if (tmp == "NETS") break;
	}
	ifs >> tmp;
	int netsNum = atoi(tmp.c_str());
	nets.clear();
	nets.resize(netsNum, {});
	int net_cnt = -1;
	while (ifs >> tmp) {
		if (tmp == "END") break;
		if (tmp == ";") net_cnt++;
		if (tmp == "(") {
			ifs >> tmp;
			int instIdx = tmpInstsMap[tmp];
			nets[net_cnt].emplace_back(instIdx);
		}
	}
	std::cout << "Read DEF Successful!" << std::endl;
}

