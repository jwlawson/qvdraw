/**
 * gmlayout.cc
 * Copyright 2014-2015 John Lawson
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0

 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
/**
 * Program to layout a graph provided in GML format. Outputs an updated GML file
 * with the layout information included.
 */
#include <unistd.h>

#include <string>

#include <ogdf/energybased/FMMMLayout.h>
 
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
	typedef ogdf::FMMMLayout FL;

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

	FL l;
	l.useHighLevelOptions(true);
	l.qualityVersusSpeed(l.qvsGorgeousAndEfficient);
	/*
	 * Setting to true changes the output graphs. Sometimes they look better,
	 * other times they do not. I have no idea why. Default is false.
	 */
	l.newInitialPlacement(true);
	l.call(GA);

	GA.writeGML(std::cout); 
 
	return 0;
}
