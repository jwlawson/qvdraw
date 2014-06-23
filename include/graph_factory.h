/**
 * graph_factory.h
 *
 * Contains functions to create graph objects from the matrix representation.
 */
#pragma once

#include "ogdf/basic/Graph_d.h"

#include "qv/int_matrix.h"

namespace qvdraw {
	namespace graph_factory{
		ogdf::Graph graph(const cluster::IntMatrix& matrix);
	}	
}

