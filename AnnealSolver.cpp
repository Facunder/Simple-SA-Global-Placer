#include "AnnealSolver.h"


AnnealSolver::AnnealSolver(std::vector<std::vector<int>>& nets_in, std::vector<std::string>& instsName_in, std::vector<int> loc_in, std::vector<int>& dieBox_in) : instsName(instsName_in), loc(loc_in), dieBox(dieBox_in) {
	instsNum = instsName.size();
	hpwl = 0;
	total_dens = 0.0;
	nets.clear();
	for (int i = 0; i< nets_in.size(); i++) {
		nets.emplace_back(loc, instsNum, nets_in[i]);
	}
	threshold = 18;
	dens_map_partical = 50;	// 50*50 bin
	bin_w = (dieBox[2] - dieBox[0]) / dens_map_partical;
	bin_h = (dieBox[3] - dieBox[1]) / dens_map_partical;
	dens_map.clear();
	dens_map.resize(50, std::vector<double>(50, 0.0));
	max_movement = (dieBox[2] - dieBox[0]) / 10;
}

double AnnealSolver::densSum() {
	double total_dens = 0.0;
	for (auto col : dens_map) {
		for (auto bin_dens : col) {
			total_dens += bin_dens;
		}
	}
	return total_dens;
}

double AnnealSolver::getCost() {
	int die_w = dieBox[2] - dieBox[0];
	int die_h = dieBox[3] - dieBox[1];
	// std::cout << "hpwl eff: " << 0.5 * double(hpwl) / (die_w + die_h) << std::endl;
	// std::cout << "dens eff: " << 0.25*total_dens << std::endl;
	double cost = 0.05 * double(hpwl) / (die_w + die_h) + 0.02 * total_dens;
	return cost;
}

void AnnealSolver::updateDens() {
	int index_x, index_y;
	double weight = (1.0 / dens_map_partical) / dens_map_partical;
	std::vector<std::vector<int>> tmp_map(dens_map_partical, std::vector<int>(dens_map_partical, 0));
	for (int i = 0; i < instsNum; i++) {
		index_x = loc[i] / bin_w;
		index_y = loc[i + instsNum] / bin_h;
		tmp_map[index_x][index_y]++;
	}
	for (int i = 0; i < dens_map_partical; i++) {
		for (int j = 0; j < dens_map_partical; j++) {
			dens_map[i][j] = tmp_map[i][j] > threshold ? weight * exp(0.05 * (tmp_map[i][j] - threshold) / threshold) : 0.0;
		}
	}
	total_dens = densSum();
}

void AnnealSolver::updateHpwl() {
	hpwl = 0;
	for (auto net : nets) {
		hpwl += net.hpwl();
	}
	//std::cout << hpwl << std::endl;
}

void AnnealSolver::randPlace() {
	srand(unsigned(time(0)));
	int die_xl = dieBox[0];
	int die_yb = dieBox[1];
	int die_xr = dieBox[2];
	int die_yt = dieBox[3];
	for (int i = 0; i < instsNum; i++) {
		loc[i] = rand() % (die_xr - die_xl + 1) + die_xl;
		loc[i + instsNum] = rand() % (die_yt - die_yb + 1) + die_yb;
	}
}

std::vector<int> AnnealSolver::disturbPlace() {
	srand(unsigned(time(0))+loc[0]);
	std::vector<int> tmp_loc;
	tmp_loc.resize(static_cast<int>(loc.size()), 0);
	for (int i = 0; i < loc.size(); i++) {
		tmp_loc[i] = loc[i];
	}
	tmp_loc = loc;
	// shifting
	int rand_tmp1, rand_tmp2;
	int tmp_x, tmp_y;
	for (int i = 0; i < instsNum; i++) {
		rand_tmp1 = rand() % (2 * max_movement + 1) - max_movement;
		rand_tmp2 = rand() % (2 * max_movement + 1) - max_movement;
		tmp_x = loc[i] + rand_tmp1;
		tmp_y = loc[i + instsNum] + rand_tmp2;
		loc[i] = (tmp_x > dieBox[0] && tmp_x < dieBox[2]) ? tmp_x : loc[i];
		loc[i+instsNum] = (tmp_y > dieBox[1] && tmp_y < dieBox[3]) ? tmp_y : loc[i+ instsNum];
	}
	// not swap
	return tmp_loc;
}

void AnnealSolver::solve() {
	
	double T_min = 1.0;
	double T = 125.0;
	double alpha = 0.5;
	double cost_pre = 0.0;
	double cost_cur = 0.0;
	int rpt_iter_gap = 100;
	int iter = -1;
	std::cout << "Begin initial random place" << std::endl;
	randPlace();
	std::cout << "End initial random place" << std::endl;
	updateDens();
	updateHpwl();
	cost_pre = getCost();
	std::cout << "Begin Simulate Annealing" << std::endl;
	while (T > T_min) {
		int cur_temp_iter = -1;
		if (T < 5 * T_min) {
			alpha = 0.7;
		}
		while (cur_temp_iter < 1000) {
			srand(unsigned(time(0)) + cur_temp_iter);
			iter++;
			cur_temp_iter++;
			if (iter % rpt_iter_gap == 0) {
				//std::cout << "\t(iteration " << iter << ") T: " << T << ", HPWL : " << hpwl << ", total_Dens : " << total_dens << ", WeightedCost: " << cost_pre << std::endl;
				std::cout << "\t(iteration " << iter << ") T: " << T << ", HPWL : " << hpwl << ", total_Dens : " << total_dens << std::endl;
			}
			std::vector<int>tmp_loc(disturbPlace());
			updateDens();
			updateHpwl();
			cost_cur = getCost();
			double delta_cost = cost_cur - cost_pre;
			double rand_tmp = (rand() % (10001)) / 10000.0;
			 //std::cout << delta_cost << std::endl;
			 //std::cout << exp(-0.001 * delta_cost / T) << std::endl;
			if (delta_cost < 0) {
				// accept
				cost_pre = cost_cur;
			}
			else if (rand_tmp < exp(-delta_cost / T)) {
				//accept
				cost_pre = cost_cur;
			}
			else {
				// reset
				for (int i = 0; i < static_cast<int>(loc.size()); i++) {
					loc[i] = tmp_loc[i];
				}
				//updateDens();
				//updateHpwl();
			}
		}
		T = T * alpha;
	}
	std::cout << "End Simulate Annealing" << std::endl;
}

