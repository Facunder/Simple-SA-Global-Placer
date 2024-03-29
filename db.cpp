#include "db.h"

int net::hpwl() {
	int max_x = 0;
	int max_y = 0;
	int min_x = INT16_MAX;
	int min_y = INT16_MAX;
	for (auto idx : instsIdx) {
		int tmp_x = loc[idx];
		int tmp_y = loc[idx + instsNum];
 		max_x = tmp_x > max_x ? tmp_x : max_x;
		min_x = tmp_x < min_x ? tmp_x : min_x;
		max_y = tmp_y > max_y ? tmp_y : max_y;
		min_y = tmp_y < min_y ? tmp_y : min_y;
 	}
	return (max_x - min_x) + (max_y - max_y);
}

void net::setLoc(std::vector<int>& loc_n) {
	loc = loc_n;
}