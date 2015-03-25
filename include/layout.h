/*
 * layout.h
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
#pragma once
#include "ogdf/basic/GraphAttributes.h"

namespace qvlayout {
enum Method { Energy, Hierachy, Layered, Visibility, Dominance, Balloon};
/**
 * Use an energy based layout algorithm to try and find optimal positions for
 * the vertices. This will not always result in the best layout, but will
 * usually end up with something which is passable.
 *
 * The positions of nodes are stored in the GraphAttributes object.
 */
void layout(
		ogdf::Graph & graph,
		ogdf::GraphAttributes & attr,
		int size = 10,
		Method = Method::Energy);
}

