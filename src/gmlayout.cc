/**
 * gmlayout.cc
 *
 * Program to layout a graph provided in GML format. Outputs an updated GML file
 * with the layout information included.
 */
#include <unistd.h>

#include <string>

#include "ogdf/planarlayout/PlanarStraightLayout.h"
#include <ogdf/basic/PreprocessorLayout.h>
 
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
	typedef ogdf::PlanarStraightLayout PSL;
	typedef ogdf::PreprocessorLayout PL;

	Graph G;
	GraphA GA(G);
	if(str.empty()) {
		if(!G.readGML(std::cin)) {
			std::cerr << "Error reading GML from stdin" << std::endl;
		}
	} else {
		if(!G.readGML(str.data())) {
			std::cerr << "Could not load " << str << std::endl;
			return 1;
		}
	}
	ogdf::node v;
	forall_nodes(v, G) {
		GA.shapeNode(v) = GraphA::oval;
		GA.width(v) = 10.0;
		GA.height(v) = 10.0;
	}
 	
	PL pre;
	pre.setLayoutModule(new PSL);
	pre.call(GA);

	GA.writeGML(std::cout); 
 
	return 0;
}
