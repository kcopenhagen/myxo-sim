#ifndef PARAMS_H
#define PARAMS_H

const double lMu = 5.0; // Average of gaussian for cell lengths (microns).
const double lSigma = 1.0; // Sigma of gaussian for cell lengths (microns).
const double vMu = 1.0; // Mean of gaussian for cell speeds. (microns/min).
const double vSigma = 0.5; // Sigma of cell speed distribution. (microns/min). 
const double rotDiffSigma = 1; // Rot by gaussian with this sigma. (deg/s).

const double boxSize = 100.0; // Size of the total box (microns).
const double beadRadius = 0.25; // Size of beads making up the cells (microns).
const double beadSpacing = 0.5; // Spacing between beads on each cell (microns).
const int maxN = 10; // Maximum number of cells.
const double maxT = 1.0; // Maximum time (minutes).
const double dt = 1.0; // Simulation timestep (seconds).
const double saveDt = 20.0; // Save timestep (seconds).

#endif
