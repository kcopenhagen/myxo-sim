#include <iostream>
#include <fstream>
#include <string>

#include "cells.cpp"
#include "parameters.h"

using namespace std;

vector<cell> initialize_cells(){
	vector<cell> cells;
	int fails = 0;
	while (cells.size() < maxN) {
		cell temp;
		temp.repulsion_force(cells);
		if (abs(temp.max_force()) < 0.00001) {
			temp.reset_forces();
			cells.push_back(temp);
		} else
			fails++;

		if (fails > 10000)
		       break;	
	
	}
	cout << "Simulating: " << cells.size() << " cells." << endl;
	return cells;
}

void save_data(string buffer, vector<cell> cells) {
	
	ofstream fw(buffer,ofstream::out);
	if (fw.is_open()) {
		for (int i=0; i<cells.size(); i++) {
			for (int j=0; j<cells[i].get_x().size(); j++) {
				fw << cells[i].get_x()[j] << "\t" << \
					cells[i].get_y()[j] << "\t" << \
					cells[i].get_z()[j] << "\t" << \
					i << "\n";
				}
		}
		fw.close();
	} else {
		cout << "Problem writing file." << endl;
	}
}
