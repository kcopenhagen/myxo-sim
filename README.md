# myxo-sim
Created by Katie Copenhagen.
Simulate self propelled flexible rods as beads connected by springs.
Water layer pushes down on everything also made of beads on springs (or other stuff).

# cpp 
Simulation files. c++
main.cpp - run the simulation. Outputs txt files with x, y, z, id for each cell bead into frames/, and for water beads into water/
cells.h, cells.cpp - Class files defining a cell, bead positions forces etc.
waters.h, waters.cpp - Class files for the water layer, mesh of beads organized randomly connected by springs.
functions.cpp - misc functions.
interactions.cpp -Involving interactions between beads (cells, water, ... etc.)
parameters.h - Header file with parameters accessible by all functions.

# mat
Visualization files. matlab
MakeVideoCluster.mat - Run the video making script on the cluster.


