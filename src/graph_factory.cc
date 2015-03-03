/*
 * graph_factory.cc
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
 * Factory method to construct an ogdf::Graph from a cluster::IntMatrix.
 * This is the method which converts between the two formats.
 */
#include "graph_factory.h"

#include <vector>

#include "qv/move_graph.h"
#include "qv/quiver_graph.h"

namespace qvdraw {
namespace graph_factory {
	using namespace graph_types;
	ogdf::Graph graph(const cluster::IntMatrix& matrix) {
		typedef ogdf::Graph G;
		G graph;
		int size = std::max(matrix.num_cols(), matrix.num_rows());
		std::vector<Node> nodes(size);
		for(int i = 0; i < size; i++) {
			nodes[i] = graph.newNode();
		}
		for(int i = 0; i < matrix.num_rows(); i++) {
			for(int j = 0; j < matrix.num_cols(); j++) {
				for(int k = matrix.get(i,j); k > 0; k--) {
					graph.newEdge(nodes[i], nodes[j]);
				}
			}
		}
		return std::move(graph);
	}
	namespace {
		template<class Map>
		Node get_node(ogdf::Graph& res, EQPtr mat, Map& map) {
			auto find = map.find(mat); 
			if(find == map.end()) {
				Node new_node = res.newNode();
				map.insert(std::make_pair(mat, new_node));
				return new_node;
			}
			return find->second;
		}
		NodeMap switch_map(const EQPtrMap & map) {
			NodeMap result;
			for(auto it = map.begin(); it != map.end(); ++it) {
				result.insert(std::make_pair(it->second, it->first));
			}
			return std::move(result);
		}
	}
	template<class G>
	GraphPair multi_graph(const G & g) {
		ogdf::Graph result;
		std::unordered_map<EQPtr, Node, PtrHash, PtrEqual> map;
		for(auto iter = g.begin(); iter != g.end(); ++iter) {
			Node node = get_node(result, iter->first, map);
			for(auto link_iter = (iter->second).begin();
					link_iter != (iter->second).end();
					++link_iter) {
				Node linked = get_node(result, *link_iter, map);
				result.newEdge(node, linked);
			}
		}
		return std::make_pair(std::move(result),std::move(switch_map(map)));
	}
	template GraphPair
		multi_graph<cluster::MoveGraph<EQ>>(const cluster::MoveGraph<EQ> &);
	template GraphPair
		multi_graph<cluster::QuiverGraph<EQ>>(const cluster::QuiverGraph<EQ> &);
}
}

