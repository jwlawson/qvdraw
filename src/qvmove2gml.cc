/**
 * qvmove2gml.cc
 * Copyright 2014-2015 John Lawson
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
/**
 * Converts a matrix to gml format.
 */
#include <unistd.h>

#include <string>

#include "qv/move_graph.h"

#include "consts.h"
#include "graph_factory.h"

void usage() {
	std::cout << "qvmove2gml -m matrix" << std::endl;
}

cluster::QuiverMatrix get_matrix(const std::string& matrix) {
	return cluster::QuiverMatrix(matrix);
}

void output_gml(const cluster::MoveGraph<cluster::EquivQuiverMatrix>& mat) {
	typedef const cluster::EquivQuiverMatrix M;
	qvdraw::GraphPair<M> g = qvdraw::graph_factory::multi_graph<M>(mat);
	g.first.writeGML(std::cout);
}

int main(int argc, char* argv[]) {
	bool matrix = false;
	std::string str;
	int c;

	while( (c=getopt(argc, argv, "m:")) != -1) {
		switch(c) {
			case 'm':
				matrix = true;
				str = optarg;
				break;
			case '?':
				usage();
				return 1;
			default:
				usage();
				return 2;
		}
	}
	if(!matrix) {
		usage();
		return 1;
	}
	typedef cluster::EquivQuiverMatrix Matrix;
	typedef cluster::MoveGraph<Matrix> Move;
	Matrix mat = get_matrix(str);
	Move move_graph(mat,qvdraw::consts::Moves);
	output_gml(move_graph);
	return 0;
}

