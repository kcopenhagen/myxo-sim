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

void cell::self_prop_force() {
	
	// Rotate the heading of the cell by random gaussian number.
	default_random_engine generator;
	normal_distribution<double> angs{0.0, rotDiffSigma};
	double rot = angs(generator);

	// Convert to degrees per timestep.
	heading = heading+rot*dt;
	
	// Set force for first bead to heading.
	fx[0] = fx[0] + v*cos(heading*M_PI/180.0);
	fy[0] = fy[0] + v*sin(heading*M_PI/180.0);
	// Set forces of the rest of the beads along connecting lines.
	for (int i=1; i<x.size(); i++) {
		double dx = x[i-1]-x[i];
		double dy = y[i-1]-y[i];
		double norm = sqrt(dx*dx+dy*dy);
		
		fx[i] = fx[i] + v * dx / norm;
		fy[i] = fy[i] + v * dy / norm;

	}

}

void cell::move_beads() {
	for (int i = 0; i < x.size(); i++) {
		x[i] = x[i] + dt*fx[i];
		y[i] = y[i] + dt*fy[i];
		
	}
}

cell::cell () {
	// Default cell initialization (random position and orientation).
	// Generate random starting x, and y positions within boxSize.
	// Generate random cell orientation, 
	//    and length from normal distribution.
	//    nBeads is the number of beads for the current cell 
	//    calculated from cell length and beadSpacing.
	double stX = boxSize*double(rand())/RAND_MAX;
	double stY = boxSize*double(rand())/RAND_MAX;
	double stZ = 0;
	
	// Starting cell direction.
	double theta = 360*double(rand())/RAND_MAX;
	heading = theta+180.0;

	default_random_engine generator;
	random_device rd{};
	mt19937 gen{rd()};

	normal_distribution<double> lens{lMu, lSigma};
	double l = lens(gen);
	normal_distribution<double> vs{vMu, vSigma};
	double v = vs(gen);

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
