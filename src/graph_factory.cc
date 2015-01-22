/*
 * graph_factory.cc
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
 * Factory method to construct an ogdf::Graph from a cluster::IntMatrix.
 * This is the method which converts between the two formats.
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

