#pragma once
#include <iostream>
#include <random>
#include <vector>
#include <math.h>
#include <stdio.h>

#include "cells.h"
#include "parameters.h"

using namespace std;
using namespace C;

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
		if (sqrt(fx[i] * fx[i] + fy[i] * fy[i] + fz[i] * fz[i]))
			fmax = sqrt(fx[i] * fx[i] + fy[i] * fy[i]\
				       	+ fz[i] * fz[i]);
	}
	return fmax;
}

void cell::self_prop_force() {
	
	// Rotate the heading of the cell by random gaussian number.
	random_device rd{};
	mt19937 gen{rd()};
	normal_distribution<double> angs{0.0, rotDiffSigma};
	double rot = angs(gen);
	
	// Convert to degrees per timestep.
	double headx = cos(heading * M_PI / 180.0);
	double heady = sin(heading * M_PI / 180.0);

	double dx = x[0]-x[1];
	double dy = y[0]-y[1];
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
	double norm = sqrt(dx*dx+dy*dy);

	// Set force of first bead to direction
	fx[0] = fx[0] + v / 60.0 * dx / norm;
	fy[0] = fy[0] + v / 60.0 * dy / norm;

	heading = heading+rot*dt;
	
	// Set force for first bead to include heading.
	fx[0] = fx[0] + v / 60.0 * cos(heading*M_PI/180.0);
	fy[0] = fy[0] + v / 60.0 * sin(heading*M_PI/180.0);
	// Set forces of the rest of the beads along connecting lines.
	for (int i=1; i<x.size(); i++) {
		double dx = x[i-1]-x[i];
		double dy = y[i-1]-y[i];
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
		double norm = sqrt(dx*dx+dy*dy);


		// Convert v from microns / min to microns / sec.
		fx[i] = fx[i] + v / 60.0 * dx / norm;
		fy[i] = fy[i] + v / 60.0 * dy / norm;

	}

}

void cell::move_beads() {
	for (int i = 0; i < x.size(); i++) {
		x[i] = x[i] + dt*fx[i];
		y[i] = y[i] + dt*fy[i];
		//Periodic boundaries!
		if (x[i]>boxSize)
			x[i] = x[i]-boxSize;
		else if (x[i]<0)
			x[i] = x[i]+boxSize;
		
		if (y[i]>boxSize)
			y[i] = y[i]-boxSize;
		else if (y[i]<0)
			y[i] = y[i]+boxSize;
		
	}
}

void cell::bead_spacing() {
	// Fix bead spacing to proper spacing.
	
	for (int i=1; i<x.size(); i++) {
		double dx = x[i] - x[i-1];
		double dy = y[i] - y[i-1];
		if (dx > boxSize / 2.0)
			dx = dx - boxSize;
		else if (dx < -boxSize / 2.0)
			dx = dx + boxSize;
		if (dy > boxSize / 2.0)
			dy = dy - boxSize;
		else if (dy < -boxSize / 2.0)
			dy = dy + boxSize;
		double norm = sqrt(dx * dx + dy * dy);
		fx[i] = fx[i] - K * (norm - beadSpacing) * dx / norm;
		fy[i] = fy[i] - K * (norm - beadSpacing) * dy / norm;
		fx[i-1] = fx[i-1] + \
			  K * (norm - beadSpacing) * dx / norm;
		fy[i-1] = fy[i-1] + \
			  K * (norm - beadSpacing) * dy / norm;
	}
}

void cell::repulsion_force(vector<cell> cells) {
	// Repulsion between cells and their neighbors.
	
	for (int i = 0; i < x.size(); i++) {
		for (int j = 0; j < cells.size(); j++) {

			vector<double> xs = cells[j].get_x();
			vector<double> ys = cells[j].get_y();
			vector<double> zs = cells[j].get_z();

			for (int k = 0; k < xs.size(); k++) {
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
	
				double norm = sqrt(dx * dx + dy * dy + dz * dz);
				if (norm < beadRadius * 2.0) {
					fx[i] = fx[i] + \
						K2 * (norm - 2.0 * beadRadius)\
					       	* dx / norm;
					fy[i] = fy[i] + \
						K2 * (norm - 2.0 * beadRadius)\
					       	* dy / norm;
					fz[i] = fz[i] + \
						K2 * (norm - 2.0 * beadRadius)\
					       	* dz / norm;
				}
			}
		}
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
	double stZ = 0;
	
	// Starting cell direction.
	double theta = 360.0*double(rand())/double(RAND_MAX);
	heading = theta+180.0;

	random_device rd{};
	mt19937 gen{rd()};

	normal_distribution<double> lens{lMu, lSigma};
	double l = lens(gen);
	normal_distribution<double> vs{vMu, vSigma};
	v = vs(gen);
	while (v <= 0)
		v = vs(gen);

	int nBeads = round(l/beadSpacing);
	vector<double> xs;
	vector<double> ys;
	vector<double> zs;
	
	vector<double> fxs;
	vector<double> fys;
	vector<double> fzs;

	// Loop through beads for current cell and assign positions.
	for (int i=0; i < nBeads; i++) {
		xs.push_back(stX+i*beadSpacing*cos(theta*M_PI/180.0));
		ys.push_back(stY+i*beadSpacing*sin(theta*M_PI/180.0));
		zs.push_back(0.0);
	}

	// Save generated x, y, and z positions.
	x = xs;
	y = ys;
	z = zs;

	// Set x, y, and z forces to zero for each bead.
	reset_forces();
}
