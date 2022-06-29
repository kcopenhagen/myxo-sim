#pragma once
#include <iostream>
#include <random>
#include <vector>
#include <math.h>
#include <stdio.h>

#include "cells.h"
// #include "waters.cpp"
#include "parameters.h"
#include "functions.cpp"

using namespace std;
using namespace C;
using namespace W;

void cell::neighbor_list(vector<cell> cells) {
	// Calculate neighbor lists, all cells with at least 1 bead\
	//      within neighRadius of the current cell.

	for (int i=0; i < x.size(); i++) {
		for (int j=0; j<cells.size(); j++) {
			vector<double> xs = cells[j].get_x();
			vector<double> ys = cells[j].get_y();
			vector<double> zs = cells[j].get_z();
			for (int k=0; k<xs.size(); k++) {
				double dx = x[i] - xs[k];
				double dy = y[i] - ys[k];
				double dz = z[i] - zs[k];

				if ( dx > boxSize / 2.0 )
					dx = dx - boxSize;
				else if (dx < -boxSize / 2.0)
					dx = dx + boxSize;
				if ( dy > boxSize / 2.0 )
					dy = dy - boxSize;
				else if (dx < -boxSize / 2.0)
					dy = dy + boxSize;
				double dr = sqrt(dx * dx + dy * dy + dz * dz);
				if (dr < neighRadius) {
					// Check if ID of cell j is already 
					//     store in nList, if not, add it.

					int nidx = find_in_nList(nList, nList.size(), j);
					if (nidx==-1)
						nList.push_back(j);
				}
			}
		}
	}
	return;
}

void cell::touch_list(vector<cell> cells) {
	tList.clear();

	// Go through neighbors and see if anything is touching.
	for (int i=0; i<x.size(); i++) {
		vector <vector <int> > temp;
		tList.push_back(temp);

		for (int j=0; j<nList.size(); j++) {
			vector <int> temp2;
			tList[i].push_back(temp2);

			int idx = find_cell_idx(cells,nList[j]);
			vector<double> xs = cells[idx].get_x();
			vector<double> ys = cells[idx].get_y();
			vector<double> zs = cells[idx].get_z();

			for (int k=0; k<xs.size(); k++) {
				double dx = xs[k] - x[i];
				double dy = ys[k] - y[i];
				double dz = zs[k] - z[i];
				if (dx > boxSize / 2.0)
					dx = dx - boxSize;
				else if (dx < -boxSize / 2.0)
					dx = dx + boxSize;
				if (dy > boxSize / 2.0)
					dy = dy - boxSize;
				else if (dy < -boxSize / 2.0)
					dy = dy + boxSize;

				double dr = sqrt(dx * dx + dy * dy + dz * dz);
				if (dr < 2.0 * beadRadius) {
					tList[i][j].push_back(k);
					// If neighbor is the current cell,
					//     exclude current and adjacent beads.
					if (id == nList[j] && abs(i - k) < 2) {

						tList[i][j].pop_back();
					}
				}
				
			}
		}
	}
	return;
}

int cell::n_touching() {
	int tot_touch = 0;
	for (int i=0; i<tList.size(); i++) {
		for (int j=0; j<tList[i].size(); j++) {
			tot_touch+=tList[i][j].size();
		}
	}
	return tot_touch;
}

void cell::reset_forces () {
	if (fx.size() == x.size()) {
		for (int i=0; i<fx.size(); i++) {
			fx[i] = 0.0;
			fy[i] = 0.0;
			fz[i] = 0.0;
		}
	} else {
		fx.clear();
		fy.clear();
		fz.clear();
		for (int i=0; i<x.size(); i++) {
			fx.push_back(0.0);
			fy.push_back(0.0);
			fz.push_back(0.0);
		}
	}
	return;
}

double cell::max_force() {
	double fmax = 0;

	for (int i = 0; i < fx.size(); i++) {
		if (sqrt(fx[i] * fx[i] + fy[i] * fy[i] + fz[i] * fz[i]) > fmax)
			fmax = sqrt(fx[i] * fx[i] + fy[i] * fy[i]\
				       	+ fz[i] * fz[i]);
	}
	return fmax;
}

void cell::self_prop_force() {
	
	random_device rd{};
	mt19937 gen{rd()};
	normal_distribution<double> noisevec{0.0, rotDiffSigma};
	
	double dx = x[0] - x[1];
	double dy = y[0] - y[1];
	double dz = z[0] - z[1];

	//Preiodic boundaries.
	if (dx > boxSize / 2.0)
		dx = dx - boxSize;
	else if (dx < -boxSize/2.0)
		dx = dx + boxSize;
	
	if (dy > boxSize / 2.0)
		dy = dy - boxSize;
	else if (dy < -boxSize / 2.0)
		dy = dy + boxSize;

	dx = dx + noisevec(gen);
	dy = dy + noisevec(gen);
	dz = dz + noisevec(gen);

	double norm = sqrt(dx * dx + dy * dy + dz * dz);

	// Set force of first bead to direction
	fx[0] = fx[0] + v / 60.0 * dx / norm;
	fy[0] = fy[0] + v / 60.0 * dy / norm;
	fz[0] = fz[0] + v / 60.0 * dz / norm;
	
	// Set forces of the rest of the beads along connecting lines.
	for (int i=1; i<x.size(); i++) {
		double dx = x[i-1] - x[i];
		double dy = y[i-1] - y[i];
		double dz = z[i-1] - z[i];
		
		//Preiodic boundaries.
		if (dx > boxSize/2.0) {
			dx = dx - boxSize;
		} else if (dx < -boxSize/2.0) {
			dx = dx + boxSize;
		}
		if (dy > boxSize/2.0) {
			dy = dy-boxSize;
		} else if (dy < -boxSize/2.0) {
			dy = dy+boxSize;
		}
		double norm = sqrt(dx * dx + dy * dy + dz * dz);


		// Convert v from microns / min to microns / sec.
		fx[i] = fx[i] + v / 60.0 * dx / norm;
		fy[i] = fy[i] + v / 60.0 * dy / norm;
		fz[i] = fz[i] + v / 60.0 * dz / norm;

	}

}

void cell::surface_tension_g() {
	for (int i = 0; i < fz.size(); i++) {
		fz[i] = fz[i] - sT;
	}
}

void cell::gel_force() {
	for (int i = 0; i < z.size(); i++) {
		if (z[i] < 0)
			fz[i] = fz[i] - K4 * z[i];
	}
}

void cell::add_forces(int i, double fxt, double fyt, double fzt) {
	fx[i] = fx[i] + fxt;
	fy[i] = fy[i] + fyt;
	fz[i] = fz[i] + fzt;
	return;
}

void cell::move_beads() {
	for (int i = 0; i < x.size(); i++) {
		x[i] = x[i] + dt * fx[i];
		y[i] = y[i] + dt * fy[i];
		z[i] = z[i] + dt * fz[i];
		//Periodic boundaries!
		if (x[i] > boxSize)
			x[i] = x[i] - boxSize;
		else if (x[i] < 0)
			x[i] = x[i] + boxSize;
		
		if (y[i] > boxSize)
			y[i] = y[i] - boxSize;
		else if (y[i] < 0)
			y[i] = y[i] + boxSize;
		
	}
}

void cell::cell_shape() {
	// Fix bead spacing to proper spacing.
	
	for (int i=1; i<x.size(); i++) {
		double dx = x[i] - x[i-1];
		double dy = y[i] - y[i-1];
		double dz = z[i] - z[i-1];
		if (dx > boxSize / 2.0)
			dx = dx - boxSize;
		else if (dx < -boxSize / 2.0)
			dx = dx + boxSize;
		if (dy > boxSize / 2.0)
			dy = dy - boxSize;
		else if (dy < -boxSize / 2.0)
			dy = dy + boxSize;
		double norm = sqrt(dx * dx + dy * dy + dz * dz);

		// Push beads towards beadSpacing separation.
		fx[i] = fx[i] - K1 * (norm - beadSpacing) * dx / norm;
		fy[i] = fy[i] - K1 * (norm - beadSpacing) * dy / norm;
		fz[i] = fz[i] - K1 * (norm - beadSpacing) * dz / norm;

		fx[i-1] = fx[i-1] + \
			  K1 * (norm - beadSpacing) * dx / norm;
		fy[i-1] = fy[i-1] + \
			  K1 * (norm - beadSpacing) * dy / norm;
		fz[i-1] = fz[i-1] + \
			  K1 * (norm - beadSpacing) * dz / norm;

		// Pull beads towards both neighbors. (Straighten cells).
		if (i > 0 && i < x.size()-1) {
			double im1ix = dx;
			double im1iy = dy;
			double im1iz = dz;
			double im1ip1x = x[i + 1] - x[i - 1];
			double im1ip1y = y[i + 1] - y[i - 1];
			double im1ip1z = z[i + 1] - z[i - 1];

			// Vector from bead i to the line connecting i - 1 and i + 1
			double mag = (im1ix * im1ip1x + im1iy * im1ip1y + \
				im1iz * im1ip1z) / ( im1ip1x * im1ip1x + \
				im1ip1y * im1ip1y + im1ip1z * im1ip1z);
			double strx = mag * im1ip1x - im1ix;
			double stry = mag * im1ip1y - im1iy;
			double strz = mag * im1ip1z - im1iz;
			
			fx[i] = fx[i] + K3 * strx;
			fy[i] = fy[i] + K3 * stry;
			fz[i] = fz[i] + K3 * strz;
		}
	}
}

void cell::check_reverse(double ct) {
	// Check if it's time to reverse.
	if (ct - lastRev >= lastRev + revT) {
		vector<double> newX;
		vector<double> newY;
		vector<double> newZ;

		for (int i = 0; i < x.size(); i++) {

			newX.push_back(x[x.size()-1-i]);
			newY.push_back(y[y.size()-1-i]);
			newZ.push_back(z[z.size()-1-i]);
		}
		x = newX;
		y = newY;
		z = newZ;

		random_device rd{};
		mt19937 gen{rd()};
		normal_distribution<double> revTs{revRMu, revRSigma};
		revT = revTs(gen);
		while (revT <= 0)
			revT = revTs(gen);
		lastRev = ct;
	}
}


cell::cell () {
	// Default cell initialization (random position and orientation).
	// Generate random starting x, and y positions within boxSize.
	// Generate random cell orientation, 
	//    and length from normal distribution.
	//    nBeads is the number of beads for the current cell 
	//    calculated from cell length and beadSpacing.
	double stX = boxSize*double(rand())/double(RAND_MAX);
	double stY = boxSize*double(rand())/double(RAND_MAX);
	stX = boxSize / 2.0;
	stY = boxSize / 2.0;
	double stZ = 0.5*double(rand())/double(RAND_MAX)-0.25;

	random_device rd{};
	mt19937 gen{rd()};

	// Starting cell direction.
	double theta = 360.0*double(rand())/double(RAND_MAX);
	normal_distribution<double> thetas{0.0, 10.0};

	normal_distribution<double> lens{lMu, lSigma};
	double l = lens(gen);
	while (l <= 0)
		l = lens(gen);

	normal_distribution<double> vs{vMu, vSigma};
	v = vs(gen);
	while (v <= 0)
		v = vs(gen);

	normal_distribution<double> revTs{revRMu, revRSigma};
	revT = revTs(gen);
	while (revT <= 0)
		revT = revTs(gen);

	//Start time for last reversal in the past less than revT.
	lastRev = -double(rand()) / double(RAND_MAX) * revT;
	
	//Bead positions and forces.
	nBeads = round(l/beadSpacing);
	vector<double> xs;
	vector<double> ys;
	vector<double> zs;
	
	vector<double> fxs;
	vector<double> fys;
	vector<double> fzs;

	// Loop through beads for current cell and assign positions.
	xs.push_back(stX);
	ys.push_back(stY);
	zs.push_back(stZ);
	//vector <vector <int>> temp;
	//tList.push_back(temp);
	for (int i=1; i < nBeads; i++) {
		double nx = xs[i-1] + beadSpacing * cos(theta * M_PI / 180.0);
		double ny = ys[i-1] + beadSpacing * sin(theta * M_PI / 180.0);

		xs.push_back(nx);
		ys.push_back(ny);
		zs.push_back(stZ);
		
		//tList.push_back(temp);

		theta = theta + thetas(gen);
	}

	// Save generated x, y, and z positions.
	x = xs;
	y = ys;
	z = zs;

	// Set x, y, and z forces to zero for each bead.
	reset_forces();
}
