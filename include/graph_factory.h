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

#include <memory>
#include <unordered_map>

#include "ogdf/basic/Graph_d.h"
#include "ogdf/basic/GraphAttributes.h"

#include "qv/equiv_quiver_matrix.h"
#include "qv/seed.h"

namespace qvdraw {
template<class NodeType>
struct NodeEquals {
	bool operator()(const NodeType & lhs, const NodeType & rhs) const {
		return lhs->equals(*rhs);
	}
};
template<class NodeType>
struct NodeHash {
	size_t operator()(const NodeType & node) const {
		return node->hash();
	}
};
struct ogdfnodeEquals {
	bool operator()(const ogdf::node & lhs, const ogdf::node & rhs) const {
		return lhs == rhs;
	}
};
struct ogdfnodeHash {
	size_t operator()(const ogdf::node & node) const {
		return (size_t)node;
	}
};
template<class NodeType>
using NodeMap = std::unordered_map<ogdf::node, NodeType*,
			ogdfnodeHash, ogdfnodeEquals>;
template<class NodeType>
using ReverseNodeMap = std::unordered_map<NodeType*, ogdf::node,
			NodeHash<NodeType*>, NodeEquals<NodeType*>>;
template<class NodeType>
using GraphPair = std::pair<ogdf::Graph, NodeMap<NodeType> >;
namespace graph_factory{
	/**
	 * Construct an OGDF graph from the supplied IntMatrix representation of a
	 * quiver. Each row/column corresponds to a vertex in the graph and each
	 * entry a_i,j to edges i -> j.
	 * @param matrix Quiver to convert to graph
	 * @return OGDF graph object
	 */
	std::pair<std::shared_ptr<ogdf::Graph>, std::shared_ptr<ogdf::GraphAttributes>>
	graph(const cluster::IntMatrix & matrix);
	/**
	 * Construct an ogdf graph from the given seed. The cluster variables will be
	 * assigned to labels on the nodes of the graph.
	 */
	template<class M>
	std::pair<std::shared_ptr<ogdf::Graph>, std::shared_ptr<ogdf::GraphAttributes>>
	graph(const cluster::__Seed<M> & seed);
	/**
	 * Construct an OGDF graph consisting of the relations between quivers.
	 */
	template<class NodeType, class G>
	GraphPair<NodeType>
	multi_graph(const G & graph);
}	
}

