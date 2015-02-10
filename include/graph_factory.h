/**
 * graph_factory.h
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
 * Contains functions to create graph objects from the matrix representation.
 */
#pragma once

#include <unordered_map>

#include "ogdf/basic/Graph_d.h"

#include "qv/equiv_quiver_matrix.h"

namespace qvdraw {
namespace graph_types {
typedef cluster::EquivQuiverMatrix EQ;
typedef EQ* EQPtr;
typedef ogdf::node Node;
struct PtrEqual {
	bool operator()(EQPtr lhs, EQPtr rhs) const {
		return lhs->equals(*rhs);
	}
};
struct PtrHash {
	size_t operator()(EQPtr ptr) const {
		return ptr->hash();
	}
};
}
typedef std::unordered_map<graph_types::EQPtr, graph_types::Node,
				graph_types::PtrHash, graph_types::PtrEqual> NodeMap;
typedef std::pair<ogdf::Graph, NodeMap> GraphPair;
namespace graph_factory{
	/**
	 * Construct an OGDF graph from the supplied IntMatrix representation of a
	 * quiver. Each row/column corresponds to a vertex in the graph and each
	 * entry a_i,j to edges i -> j.
	 * @param matrix Quiver to convert to graph
	 * @return OGDF graph object
	 */
	ogdf::Graph graph(const cluster::IntMatrix& matrix);
	/**
	 * Construct an OGDF graph consisting of the relations between quivers.
	 */
	template<class G>
	GraphPair multi_graph(const G & graph);
}	
}

