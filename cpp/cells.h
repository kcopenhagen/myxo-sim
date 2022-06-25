// Cell class header file.
#pragma once
#include <vector>
#include "parameters.h"

using namespace std;
namespace C
{
	class cell {
		// Cell bead positions.
		vector<double> x;
		vector<double> y;
		vector<double> z;
	
		// Cell bead forces.
		vector<double> fx;
		vector<double> fy;
		vector<double> fz;
	
		// Cell speed.
		double v;
		double heading;
	
		public:
			// Constructors
			cell();
			// Get and set x, y, and z positions and heading.
			inline vector<double> get_x() { return x; } 
			inline vector<double> get_y() { return y; } 
			inline vector<double> get_z() { return z; } 

			inline void set_x(vector<double> xt) { x = xt; }
			inline void set_y(vector<double> yt) { y = yt; }
			inline void set_z(vector<double> zt) { z = zt; }
			inline void set_heading(double ht) { heading = ht; }

			// Reset all bead forces to zero.
			void reset_forces();

			// Add forces to each bead.
			void self_prop_force();
			void repulsion_force(vector<cell> cells);

			// Move beads.
			void move_beads();
			void bead_spacing();

			// Check force magnitude
			double max_force();
	};
}
