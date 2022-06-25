#include <bits/stdc++.h>
#include <iostream>
#include <sys/stat.h>
#include <sys/types.h>

#include "cells.cpp"
#include "parameters.h"
#include "functions.cpp"

using namespace std;
using namespace C;

int main(){

	srand(time(NULL));
	time_t t1 = clock();
	// Create output folder for data.
	char buffer[50];
	int i = 0;
	sprintf(buffer,"../temp/Sim%06d/",i);
	do {
		sprintf(buffer, "../temp/Sim%06d/",i);
		i++;
		cout << "Creating directory: " << buffer << endl;
	}
	while (mkdir(buffer, 0777)!=0);

	cout << "Saving to directory: " << buffer << endl;

	// Copy parameters file into Simulation directory.
	string line;
	ifstream ini_file {"parameters.h"};
	
	char buffer2[50];
	sprintf(buffer2,"%sparameters.h",buffer);
	ofstream out_file {buffer2};
	if (ini_file && out_file) {
		while (getline(ini_file,line)) {
			out_file << line << "\n";
		}
		cout << "Copy Finished" << endl;
	} else 
		cout << "Error copying parameters file." << endl;
	ini_file.close();
	out_file.close();


	sprintf(buffer,"%sframes/",buffer);
	mkdir(buffer, 0777);

	// Create vector of cell data.
	vector<cell> cells;
	cells = initialize_cells();


	int frameCount = 0;
	double saveTimer = saveDt; //Initialize saveTimer

	for (int t=0; t<maxT*60.0; t+=dt) {
		// Save data to file.
		saveTimer += dt;
		if (saveTimer >= saveDt) {
			char fname[50];
			sprintf(fname, "%s%06d.txt", buffer, frameCount);
			save_data(fname, cells);
			saveTimer = 0;
			frameCount++;
		}
		
		// Up date cell forces.
		for (i=0; i<cells.size(); i++) {
			cells[i].self_prop_force();
			cells[i].bead_spacing();
			vector<cell> otherCells = cells;
			otherCells.erase(otherCells.begin()+i);
			cells[i].repulsion_force(otherCells);
		}

		// Move cells.
		for (i=0; i<cells.size(); i++) {
			cells[i].move_beads();
		}
		
		// Reset forces to zero.
		for (i=0; i<cells.size(); i++) {
			cells[i].reset_forces();
		}


	}
	cout << "Elapsed time: " << 1 / 60 * (clock() - t1)/double(CLOCKS_PER_SEC) \
		<< "minutes." << endl;
	return 0;
}
