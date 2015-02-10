/**
 * layout.cc
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
 * Function to layout a graph.
 */
#include <ogdf/energybased/FMMMLayout.h>
 
namespace qvlayout {
namespace {
typedef ogdf::Graph Graph;
typedef ogdf::GraphAttributes GraphA;
typedef ogdf::FMMMLayout FL;
}

void layout(Graph & graph, GraphA & attr, int size = 10) {
	ogdf::node v;
	forall_nodes(v, graph) {
		attr.width(v) = size;
		attr.height(v) = size;
	}

	FL l;
	l.useHighLevelOptions(true);
	l.qualityVersusSpeed(l.qvsGorgeousAndEfficient);
	/*
	 * Setting to true changes the output graphs. Sometimes they look better,
	 * other times they do not. I have no idea why. Default is false.
	 */
	l.newInitialPlacement(true);
	l.call(attr);
}
}
