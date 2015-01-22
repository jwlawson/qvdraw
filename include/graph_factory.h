/**
 * graph_factory.h
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

