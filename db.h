#pragma once
#include <vector>
#include <unordered_map>

class net {
public:
	net(std::vector<int>& loc_n, int instsNum_n, std::vector<int> instsIdx_n) : loc(loc_n), instsNum(instsNum_n), instsIdx(instsIdx_n) {}	
	int hpwl();
	void setLoc(std::vector<int>& loc_n);
private:
	std::vector<int>& loc;
	int instsNum;
	std::vector<int> instsIdx;
};