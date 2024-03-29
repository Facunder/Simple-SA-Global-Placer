#pragma once
#include<iostream>
#include<ctime>
#include<vector>

#include "db.h"

class AnnealSolver {
public:
	AnnealSolver(std::vector<std::vector<int>>& nets_in, std::vector<std::string>& instsName, std::vector<int> loc_in, std::vector<int>& dieBox_in);
	void solve();
private:
	void randPlace();
	std::vector<int> disturbPlace();
	double getCost();
	void updateHpwl();
	void updateDens();
	double densSum();
	int instsNum;
	int threshold;
	int dens_map_partical;
	int max_movement;
	int hpwl;
	int bin_w;
	int bin_h;
	double total_dens;
	std::vector<std::string>& instsName;
	std::vector<int>& dieBox;
	std::vector<int> loc;
	std::vector<net> nets;
	std::vector<std::vector<double>> dens_map;
};