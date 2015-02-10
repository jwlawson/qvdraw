/*
 * layout.h
 */
/**
 * Function to layout a graph.
 */
#pragma once
#include "ogdf/basic/GraphAttributes.h"

namespace qvlayout {
/**
 * Use an energy based layout algorithm to try and find optimal positions for
 * the vertices. This will not always result in the best layout, but will
 * usually end up with something which is passable.
 *
 * The positions of nodes are stored in the GraphAttributes object.
 */
void layout(ogdf::Graph & graph, ogdf::GraphAttributes & attr, int size = 10);
}

