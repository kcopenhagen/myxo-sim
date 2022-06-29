#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "cells.cpp"
#include "waters.cpp"
#include "parameters.h"

using namespace std;
using namespace C;

vector<cell> initialize_cells() {
	vector<cell> cells;
	int fails = 0;
	int c = 0;

	while (cells.size() < maxN) {
		cell temp;
		
		// Find closest bead to cell temp.
		double mindr = 9999;
		vector<double> x = temp.get_x();
		vector<double> y = temp.get_y();
		vector<double> z = temp.get_z();
		for (int i=0; i<x.size(); i++) {
			for (int j=0; j<cells.size(); j++) {
				vector<double> xs = cells[j].get_x();
				vector<double> ys = cells[j].get_y();
				vector<double> zs = cells[j].get_z();
				for (int k=0; k<xs.size(); k++) {
					double dx = x[i] - xs[k];
					double dy = y[i] - ys[k];
					double dz = z[i] - zs[k];
					double dr = sqrt(dx * dx + dy * dy \
							+ dz * dz);
					if (dr < mindr)
						mindr = dr;
				}
			}
		}
		// If closest bead is further than beadRadius accept the cell.
		if (mindr > beadRadius) {
			cells.push_back(temp);
			cells[c].set_id(c);
			c++;
		} else {
			cells.pop_back();
			fails++;
		}

		if (fails > 10000)
		       break;	
	}
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
		cout << "Problem writing " << buffer << " file." << endl;
	}
}

void save_water(string buffer, water water_layer) {
	ofstream fw(buffer,ofstream::out);
	vector <double> x = water_layer.get_x();
	vector <double> y = water_layer.get_y();
	vector <double> z = water_layer.get_z();
	
	cout << x[0] << "\t" << y[0] << "\t" << z[0] << endl;

	int nBeads = water_layer.get_nBeads();
	if (fw.is_open()) {
		for (int i=0; i<nBeads; i++) {
			fw << x[i] << "\t" << y[i] << "\t" << z[i] << "\n";
		}
		fw.close();
	} else {
		cout << "Problem writing " << buffer << " file." << endl;
	}
}


int find_cell_idx(vector<cell> cells, int id) {
	for (int i = 0; i<cells.size(); i++) {
		if (cells[i].get_id() == id)
			return i;
	}
	return -1;
}

int find_in_nList(int *nList, int nns, int id) {
	for (int i=0; i<nns; i++) { 
		if (nList[i] == id)
			return i;
	}
	return -1;
}
