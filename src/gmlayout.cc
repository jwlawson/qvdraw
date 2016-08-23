/**
 * gmlayout.cc
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
 * Program to layout a graph provided in GML format. Outputs an updated GML file
 * with the layout information included.
 */
#include <unistd.h>

#include <string>

#include "layout.h"
 
void usage() {
	std::cout << "gmlayout [-i input]" << std::endl;
	std::cout << "Layout a graph in GML format in a planar way." << std::endl;
	std::cout << "  -i Input file to read. Defualt is stdin" << std::endl;
}

int main(int argc, char* argv[]) {
	std::string str;
	int c;

	while((c = getopt(argc, argv, "i:")) != -1) {
		switch(c) {
			case 'i':
				str = optarg;
				break;
			case '?':
				usage();
				break;
			default:
				usage();
				break;
		}
	}
	typedef ogdf::Graph Graph;
	typedef ogdf::GraphAttributes GraphA;

	Graph G;
	GraphA GA(G);
	GA.initAttributes(ogdf::GraphAttributes::edgeLabel);
	GA.initAttributes(ogdf::GraphAttributes::nodeLabel);
	if(str.empty()) {
		if(!GA.readGML(G, std::cin)) {
			std::cerr << "Error reading GML from stdin" << std::endl;
		}
	} else {
		if(!GA.readGML(G, str.data())) {
			std::cerr << "Could not load " << str << std::endl;
			return 1;
		}
	}
	qvlayout::layout(G, GA);

	GA.writeGML(std::cout); 
 
	return 0;
}
