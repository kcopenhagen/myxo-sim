#ifndef PARAMS_H
#define PARAMS_H

const double lMu = 5.0; // Average of gaussian for cell lengths (microns).
const double lSigma = 1.0; // Sigma of gaussian for cell lengths (microns).
const double vMu = 1.0; // Mean of gaussian for cell speeds. (microns/min).
const double vSigma = 0.4; // Sigma of cell speed distribution. (microns/min). 
const double rotDiffSigma = 0.2; // Noise added to first bead direction. (1/s).
const double K1 = 0.5; // Spring constant connecting cell beads.
const double K2 = 0.1; // Spring constant for repulsion with other cells.
const double K3 = 0.3; // Restoring force straightening bead bonds.
const double K4 = 0.2; // Gel stiffness
const double K5 = 0.0; // Water spring stiffness.
const double K6 = 0.0; // Spring for repulsion between water layer and cells.
const double sT = 0.01; // Water downward pull towards gel.
const double revRMu = 9999.0; // Reversals per minute average.
const double revRSigma = 3.0; // Reversals per minute Sigma.

const double boxSize = 20.0; // Size of the total box (microns).
const double beadRadius = 0.25; // Size of beads making up the cells (microns).
const double waterRadius = 0.5; // Size of water beads.
const double beadSpacing = 0.3; // Spacing between beads on each cell (microns).
const double neighRadius = 2.0; // Distance to neighbors (microns).
const int maxN = 20; // Maximum number of cells.
const double maxT = 60.0; // Maximum time (minutes).
const double dt = 1.0; // Simulation timestep (seconds).
const double saveDt = 20.0; // Save timestep (seconds).

#endif
