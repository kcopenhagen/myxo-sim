#include <bits/stdc++.h>
#include <iostream>
#include <sys/stat.h>
#include <sys/types.h>
#include <ctime>

#include "cells.cpp"
#include "waters.cpp"
#include "parameters.h"
#include "functions.cpp"
#include "interactions.cpp"

using namespace std;
using namespace C;
using namespace W;

int main(){

	srand(time(NULL));
	time_t t1 = clock();

	mkdir("frames", 0777);
	mkdir("water", 0777);

	// Create vector of cell data.
	vector<cell> cells;
	cells = initialize_cells();

	int totBeads = 0;
	for (int c = 0; c<cells.size(); c++)
		totBeads += cells[c].get_nBeads();
	cout << "Simulating: " << cells.size() << " cells. With " << totBeads<< " total beads." <<  endl;

	// Create water sheet.
	water water_layer;
	if (K6 > 0.0001)
		cout << "Simulating: " << water_layer.get_nBeads() << " water beads." << endl;
	else
		cout << "Not simulating water layer." << endl;

	int frameCount = 0;
	double saveTimer = saveDt; //Initialize saveTimer

	//Initialize neigh calc timer to fasted time to travel neighRadius
	double maxv = 0;
	for (int c=0; c<cells.size(); c++) {
		if (cells[c].get_v() > maxv)
			maxv = cells[c].get_v();
	}
	double neighDt = (neighRadius - 2.0 * beadRadius) / maxv;
	double neighTimer = neighDt + 1.0;

	double tot_n_time = 0;
	double tot_r_time = 0;
	double tot_w_time = 0;

	for (double t=0; t < maxT * 60.0; t+=dt) {
		// Save data to file.
		saveTimer += dt;
		if (saveTimer >= saveDt) {
			cout << "Time point: " << t << ", elapsed time: " \
				<< (clock() - t1) / double(CLOCKS_PER_SEC)\
			       	<< endl;
			char fname[50];
			sprintf(fname, "%s%06d.txt", "frames/", frameCount);
			save_data(fname, cells);
			if (K6 > 0.0001) {
				char fname2[50];
				sprintf(fname2, "%s%06d.txt", "water/", frameCount);
				save_water(fname2, water_layer);
			}
			saveTimer = 0;
			frameCount++;
		}
		
		// Calculate new neighbor and touching lists.
		time_t t_n1 = clock();
		
		if (neighTimer > neighDt) {
			for (int c=0; c < cells.size(); c++) {
				cells[c].neighbor_list(cells);
				if (K6 > 0.0001) {
					vector<int> wnList;
					wnList = water_neighbor_list(water_layer, cells[c]);
					cells[c].set_wnList(wnList);
				}
			}
			neighTimer = 0.0;
		}
		
		tot_n_time += clock() - t_n1;
		neighTimer += dt;
/*		for (int c=0; c < cells.size(); c++){
			cells[c].touch_list(cells);
		}
*/

		// Calculate forces on water layer.
		if (K6 > 0.0001) {
			water_layer.sinking();
			water_layer.water_springs();
		}

		// Check for reversals.
		for (int c=0; c < cells.size(); c++)
			cells[c].check_reverse(t);

		// Update cell forces.
		for (int c=0; c<cells.size(); c++) {
			// Internal cell forces.
			cells[c].self_prop_force_substrate();
			cells[c].cell_shape();
			//cells[c].surface_tension();
			cells[c].gel_force();

			// Interactions with other cells.
			time_t t_r1 = clock();
			cells[c] = repulsion_force_neighs(c, cells);
			cells[c] = self_prop_force_neighs(c, cells);
			tot_r_time += clock() - t_r1;

			// Interactions with water layer.
			time_t t_w1 = clock();
			if (K6 > 0.0001) {
				water_interaction_plane(&cells[c], &water_layer);
			} else
				cells[c].surface_tension_k();
			tot_w_time += clock() - t_w1;

		}

		// Move cells.
		for (int c=0; c<cells.size(); c++) {
			cells[c].move_beads();
		}

		// Move water.
		if (K6 > 0.0001)
			water_layer.move_beads();

		// Reset forces to zero.
		for (int c=0; c<cells.size(); c++) {
			cells[c].reset_forces();
		}
		if (K6 > 0.0001) 
			water_layer.reset_forces();


	}
	cout << "Time neighboring: " << tot_n_time/double(CLOCKS_PER_SEC) << endl;
	cout << "Time repelling: " << tot_r_time/double(CLOCKS_PER_SEC) << endl;
	cout << "Time spent on water: " << tot_w_time/double(CLOCKS_PER_SEC) << endl;
	cout << "Elapsed time: " << 1.0 / 60.0 * (clock() - t1)/double(CLOCKS_PER_SEC) \
		<< "minutes." << endl;

	return 0;
}
