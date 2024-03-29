#include"defParser.h"
#include"AnnealSolver.h"

int main() {
	defParser parser;
	std::vector<std::vector<int>> nets;
	std::vector<std::string> instsName;
	std::vector<int> loc;
	std::vector<int> dieBox;
	parser.parse("./floorplan.def", nets, instsName, loc, dieBox);
	AnnealSolver solver(nets, instsName, loc, dieBox);
	solver.solve();
	return 0;
}