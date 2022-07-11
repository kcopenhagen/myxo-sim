#ifndef PARAMS_H
#define PARAMS_H

const double lMu = 5.0; // Average of gaussian for cell lengths (microns).
const double lSigma = 1.0; // Sigma of gaussian for cell lengths (microns).
const double vMu = 0.02; // Mean of gaussian for cell speeds. (um/s).
const double vSigma = 0.01; // Sigma of cell speed distribution. (um/s). 
const double rotDiffSigma = 0.005; // Noise added to first bead direction. (um/s)
const double K1 = 0.5; // Spring constant connecting cell beads. (1/s)
const double K2 = 0.5; // Spring constant for repulsion with other cells. (1/s)
const double K3 = 0.5; // Restoring force straightening bead bonds. (1/s)
const double K4 = 0.3; // Gel stiffness (1/s)
const double K5 = 0.005; // Water spring stiffness. (1/s)
const double K6 = 0.01; // Spring for repulsion water layer and cells. (1/s)
const double sT = 0.001; // Water downward pull towards gel. (1/s)
const double revRMu = 999999.0; // Seconds per reversal average. 
const double revRSigma = 180.0; // Seconds per reversal Sigma.

const double boxSize = 20.0; // Size of the total box (microns).
const double beadRadius = 0.25; // Size of beads making up the cells (microns).
const double waterRadius = 0.2; // Size of water beads. (microns)
const double waterRadiusFactor = 1.0; // Water bead radius that cells feel.
const double beadSpacing = 0.3; // Spacing between beads on each cell (microns).
const double neighRadius = 2.0; // Distance to neighbors (microns).
const int maxN = @N; // Maximum number of cells.
const double maxT = 600.0; // Maximum time (minutes).
const double dt = 1; // Simulation timestep (seconds).
const double saveDt = 20.0; // Save timestep (seconds).

#endif
