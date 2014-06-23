/**
 * graph_factory.cc
 */
#include "graph_factory.h"

#include <vector>

namespace qvdraw {
	namespace graph_factory {

		ogdf::Graph graph(const cluster::IntMatrix& matrix) {
			typedef ogdf::Graph G;
			typedef ogdf::node N;
			G graph;
			int size = std::max(matrix.num_cols(), matrix.num_rows());
			std::vector<N> nodes(size);
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
	}
}

