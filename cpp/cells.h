// Cell class header file.
#pragma once
#include <vector>
#include "parameters.h"

using namespace std;
namespace C
{
	class cell {
		// Cell id and bead count.
		int id;
		int nBeads;

		// Cell bead positions.
		vector<double> x;
		vector<double> y;
		vector<double> z;
	
		// Cell bead forces.
		vector<double> fx;
		vector<double> fy;
		vector<double> fz;
	
		// Neighbor (cells with any bead within neighRadius) list.
		// Touching list (beads within beadRadius of beads).
		// 	tList[i][c][k] 
		// 		i = bead in current cell.
		// 		c = neighboring cell.
		// 		k = bead in neighboring cell.
		int nList[maxN];
		int nns;
		int maxns;
		vector <vector <vector <int>>> tList;

		// Cell speed and time untill next reversal.
		double v;
		double revT; // Total time until next reversal.
		double lastRev; // Time of last reversal.
	
		public:
			// Constructors
			cell();
			// Get and set x, y, and z positions.
			inline vector<double> get_x() { return x; } 
			inline vector<double> get_y() { return y; } 
			inline vector<double> get_z() { return z; } 
			inline double get_v() { return v; }
			inline int get_id() { return id; }
			inline int get_nBeads() { return nBeads; }
			inline vector <vector <vector <int>>> get_tList() \
				{ return tList; }
			inline int *get_nList() { return nList; }
			inline int get_nns() { return nns; }
			inline int get_maxns() { return maxns; }

			inline void set_x(vector<double> xt) { x = xt; }
			inline void set_y(vector<double> yt) { y = yt; }
			inline void set_z(vector<double> zt) { z = zt; }
			inline void set_id(int idt) { id = idt; }

			// Calculate neighbor and touching lists.
			void neighbor_list(vector<cell> cells);
			void touch_list(vector<cell> cells);
			int n_touching();

			// Reset all bead forces to zero.
			void reset_forces();

			// Add forces to each bead.
			void self_prop_force();
			void add_forces(int i, double fxt, double fyt, double fzt);
			void surface_tension();
			void gel_force();

			// Move beads.
			void move_beads();
			void cell_shape();
			void check_reverse(double);

			// Check force magnitude
			double max_force();
	};
}
