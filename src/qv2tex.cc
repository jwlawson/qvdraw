/*
 * qv2tex.h
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
 * Convert quivers and graphs into TeX documents - using the Tikz package.
 */
#include <ostream>
#include <unistd.h>

#include "ogdf/basic/Graph.h"
#include "ogdf/basic/GraphAttributes.h"

#include "qv/exchange_graph.h"
#include "qv/ginac_util.h"
#include "qv/move_graph.h"

#include "consts.h"
#include "graph_factory.h"
#include "layout.h"

namespace {
cluster::Seed::Cluster
default_cluster(size_t size) {
	cluster::Seed::Cluster result(size);
	std::string var = "x_";
	for(size_t i = 0; i < size; ++i) {
		result[i] = cluster::ginac::symbol(var + std::to_string(i));
	}
	return result;
}
}
namespace qv2tex {

void preamble(std::ostream & os) {
	os << "\\documentclass{standalone}" << os.widen('\n');
	os << "\\usepackage{tkz-euclide}" << os.widen('\n');
	os << "\\usetkzobj{all}" << os.widen('\n');
	os << "\\tikzstyle{every picture}+=[>=stealth']" << os.widen('\n');
}
void begin(std::ostream & os) {
	os << "\\def\\qvsize{0.7pt}"<< os.widen('\n');
	os << "\\def\\qvscale{0.05}"<< os.widen('\n');
	os << "\\def\\grsize{0.7pt}"<< os.widen('\n');
	os << "\\def\\grscale{2}"<< os.widen('\n');
	os << "\\begin{document}" << os.widen('\n');
}
void end(std::ostream & os) {
	os << "\\end{document}" << os.widen('\n');
}
void draw_quiver(std::ostream & os,
		const ogdf::Graph & graph,
		const ogdf::GraphAttributes & attr) {
	ogdf::node n;
	os << "\\begin{tikzpicture}[x=\\qvsize,y=\\qvsize]" << os.widen('\n');
	forall_nodes(n, graph) {
		os << "\\tkzDefPoint(" << attr.x(n) << "," << attr.y(n) << "){n";
		os << n->index() << "}" << os.widen('\n');
	}
	ogdf::edge e;
	forall_edges(e, graph) {
		os << "\\draw[->,shorten <=5,shorten >=5] (n" << e->source()->index() << ") -- (n";
		os << e->target()->index() << ")";
		if(attr.labelEdge(e).length() > 0) {
			os << "node[midway,above,sloped] {$"
				<< attr.labelEdge(e) << "$}";
		}
		os << ";" << os.widen('\n');
	}
	os << "\\tkzDrawPoints(";
	forall_nodes(n,graph) {
		os << "n" << n->index();
		if(n->succ()) {
			os << ",";
		}
	}
	os << ");" << os.widen('\n');
	forall_nodes(n, graph) {
		if(attr.labelNode(n).length() > 0) {
			os << "\\tkzLabelPoint(n" << n->index() << "){$" << attr.labelNode(n);
			os << "$}" << os.widen('\n');
		}
	}
	os << "\\end{tikzpicture}" << os.widen('\n');
}
void box_quiver(std::ostream & os,
		const std::string & name,
		const ogdf::Graph & graph,
		const ogdf::GraphAttributes & attr) {
	os << "\\newsavebox{\\" << name << "}" << os.widen('\n');
	os << "\\sbox{\\" << name << "}{%" << os.widen('\n');
	os << "\\scalebox{\\qvscale}{\\framebox{%" << os.widen('\n');
	draw_quiver(os, graph, attr);
	os << "}}}" << os.widen('\n');
}
std::string int_to_str(int a) {
	char lookup[] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'k', 'i', 'j'};
	std::string result;
	if(a == 0 ) {
		result += lookup[0];
	}
	while(a > 0) {
		result += lookup[a % 10];
		a = a / 10;
	}
	return result;
}
template<class M>
void draw_multi_graph(std::ostream & os,
		const qvdraw::NodeMap<M> & map,
		const ogdf::Graph & graph,
		const ogdf::GraphAttributes & attr) {
	ogdf::node node;
	forall_nodes(node, graph) {
		auto found = map.find(node);
		if(found != map.end()) {
			const M* mat = found->second;
			std::pair<std::shared_ptr<ogdf::Graph>, std::shared_ptr<ogdf::GraphAttributes>> pair = qvdraw::graph_factory::graph(*mat);
			ogdf::Graph & n_graph = *pair.first;
			ogdf::GraphAttributes & n_attr = *pair.second;
			qvlayout::layout(n_graph, n_attr);
			box_quiver(os, "node" + int_to_str(node->index()), n_graph, n_attr);
		} else {
			/* Node is not a quiver/seed.
			 * This happens when the graph is not completely contstructed e.g. in the
			 * case where the exchange graph would otherwise be infinite. */
			ogdf::Graph n_graph;
			ogdf::GraphAttributes n_attr(n_graph);
			box_quiver(os, "node" + int_to_str(node->index()), n_graph, n_attr);
		}
	}
	os << "\\begin{tikzpicture}[x=\\grsize,y=\\grsize,scale=\\grscale]" << os.widen('\n');
	forall_nodes(node, graph) {
		std::string name = "node" + int_to_str(node->index());
		os << "\\node[inner sep=1pt]"
			" (n" << node->index() << ") at ("  << attr.x(node) << ","
			<< attr.y(node) << "){\\usebox{\\node" << int_to_str(node->index()) << "}};"
			<< os.widen('\n');
	}
	ogdf::edge e;
	forall_edges(e, graph) {
		os << "\\draw[line width=.05pt,blue](";
		os << "n" << e->source()->index() << ") -- (n" << e->target()->index()
			<< ");" << os.widen('\n');
	}
	os << "\\end{tikzpicture}" << os.widen('\n');
}
template<class M, class Graph>
void 
output_multi_graph(const Graph & multi_gr, std::ostream & os) {
	/* 
	 * NB: The std::move here is important. Otherwise the graph ends up being
	 * copied for some stupid reason. Then the nodes in the graph are different
	 * to the nodes in the map.
	 */
	qvdraw::GraphPair<M> pair = std::move(qvdraw::graph_factory::multi_graph<M>(multi_gr));
	qvdraw::NodeMap<M> & map = pair.second;
	ogdf::Graph & graph = pair.first;
	ogdf::GraphAttributes attr(graph);
	qvlayout::layout(graph, attr, 10, qvlayout::Method::Energy);

	qv2tex::preamble(os);
	qv2tex::begin(os);
	qv2tex::draw_multi_graph(os, map, graph, attr);
	qv2tex::end(os);
}
}
void usage() {
	std::cout << "qv2tex -l [-n number] [-q|m|g|e quiver]" << std::endl;
	std::cout << "Takes a qv matrix and outputs the TeX to draw the quiver." << std::endl;
	std::cout << "   -q Draw a single quiver" << std::endl;
	std::cout << "   -m Draw the move graph of a quiver" << std::endl;
	std::cout << "   -g Draw the quiver graph of a quiver" << std::endl;
	std::cout << "   -e Draw the exchange graph of a quiver with cluster (x1 ... )" << std::endl;
	std::cout << "   -l Draw the labelled exchange/quiver graph" << std::endl;
	std::cout << "   -n Limit the number of seeds computed to given number" << std::endl;
}
enum Func { quiver, move, graph, exchange, unset };
int main(int argc, char* argv[]) {
	Func func = unset;
	bool labelled = false;
	std::string mat_str;
	size_t limit = SIZE_MAX;
	int c;

	while((c = getopt(argc, argv, "q:m:g:e:ln:")) != -1) {
		switch(c) {
			case 'q':
				func = Func::quiver;
				mat_str = optarg;
				break;
			case 'm':
				func = Func::move;
				mat_str = optarg;
				break;
			case 'g':
				func = Func::graph;
				mat_str = optarg;
				break;
			case 'e':
				func = Func::exchange;
				mat_str = optarg;
				break;
			case 'l':
				labelled = true;
				break;
			case 'n':
				limit = std::stoul(optarg);
				break;
			case '?':
				usage();
				return 1;;
			default:
				usage();
				return 1;
		}
	}
	if(func == Func::unset) {
		usage();
		return 1;
	}
	std::ostream & os = std::cout;
	if(func == Func::quiver) {
		cluster::IntMatrix matrix(mat_str);
		std::pair<std::shared_ptr<ogdf::Graph>, std::shared_ptr<ogdf::GraphAttributes>> pair = qvdraw::graph_factory::graph(matrix);
		ogdf::Graph & graph = *pair.first;
		ogdf::GraphAttributes & attr = *pair.second;
		qvlayout::layout(graph, attr);
		qv2tex::preamble(os);
		qv2tex::begin(os);
		qv2tex::draw_quiver(os, graph, attr);
		qv2tex::end(os);
	}
	else if(func == Func::move) {
		typedef cluster::EquivQuiverMatrix M;
		M matrix(mat_str);
		cluster::MoveGraph<M> move(matrix, qvdraw::consts::Moves);
		qv2tex::output_multi_graph<M>(move, os);
	}
	else if(labelled && func == Func::graph) {
		typedef cluster::QuiverMatrix M;
		M matrix(mat_str);
		cluster::LabelledQuiverGraph move(matrix, limit);
		qv2tex::output_multi_graph<M>(move, os);
	}
	else if(func == Func::graph) {
		typedef cluster::EquivQuiverMatrix M;
		M matrix(mat_str);
		cluster::QuiverGraph move(matrix, limit);
		qv2tex::output_multi_graph<M>(move, os);
	}
	else if(labelled && func == Func::exchange) {
		typedef cluster::LabelledSeed M;
		cluster::QuiverMatrix matrix(mat_str);
		M::Cluster cluster = default_cluster(matrix.num_rows());
		M seed(matrix, cluster);
		cluster::LabelledExchangeGraph move(seed, limit);
		qv2tex::output_multi_graph<M>(move, os);
	}
	else if(func == Func::exchange) {
		typedef cluster::Seed M;
		cluster::QuiverMatrix matrix(mat_str);
		M::Cluster cluster = default_cluster(matrix.num_rows());
		M seed(matrix, cluster);
		cluster::ExchangeGraph move(seed, limit);
		qv2tex::output_multi_graph<M>(move, os);
	}
	return 0;
}

