/*
 * qv2tex.h
 */
/**
 * Convert quivers and graphs into TeX documents - using the Tikz package.
 */
#include <ostream>
#include <unistd.h>

#include "ogdf/basic/Graph.h"
#include "ogdf/basic/GraphAttributes.h"

#include "qv/move_graph.h"

#include "consts.h"
#include "graph_factory.h"
#include "layout.h"

namespace qv2tex {

void preamble(std::ostream & os) {
	os << "\\documentclass{standalone}" << os.widen('\n');
	os << "\\usepackage{tkz-euclide}" << os.widen('\n');
	os << "\\usetkzobj{all}" << os.widen('\n');
	os << "\\tikzstyle{every picture}+=[>=stealth']" << os.widen('\n');
}
void begin(std::ostream & os) {
	os << "\\begin{document}" << os.widen('\n');
}
void end(std::ostream & os) {
	os << "\\end{document}" << os.widen('\n');
}
void draw_quiver(std::ostream & os,
		const ogdf::Graph & graph,
		const ogdf::GraphAttributes & attr) {
	ogdf::node n;
	os << "\\begin{tikzpicture}[x=.5pt,y=.5pt]" << os.widen('\n');
	forall_nodes(n, graph) {
		os << "\\tkzDefPoint(" << attr.x(n) << "," << attr.y(n) << "){n";
		os << n->index() << "}" << os.widen('\n');
	}
	os << "\\tkzDrawPoints(";
	forall_nodes(n,graph) {
		os << "n" << n->index();
		if(n->succ()) {
			os << ",";
		}
	}
	os << ");" << os.widen('\n');
	ogdf::edge e;
	os << "\\tkzDrawSegments[->,shorten <=5,shorten >=5](";
	forall_edges(e, graph) {
		os << "n" << e->source()->index() << ",n" << e->target()->index();
		if(e->succ()) {
			os << " ";
		}
	}
	os << ");" << os.widen('\n');
	os << "\\end{tikzpicture}" << os.widen('\n');
}
void box_quiver(std::ostream & os,
		const std::string & name,
		const ogdf::Graph & graph,
		const ogdf::GraphAttributes & attr) {
	os << "\\newsavebox{\\" << name << "}" << os.widen('\n');
	os << "\\sbox{\\" << name << "}{%" << os.widen('\n');
	os << "\\scalebox{0.01}{\\framebox{%" << os.widen('\n');
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
void draw_multi_graph(std::ostream & os,
		const qvdraw::NodeMap & map,
		const ogdf::Graph & graph,
		const ogdf::GraphAttributes & attr) {
	ogdf::node node;
	forall_nodes(node, graph) {
		cluster::EquivQuiverMatrix & mat = *map.at(node);
		ogdf::Graph n_graph = qvdraw::graph_factory::graph(mat);
		ogdf::GraphAttributes n_attr(n_graph);
		qvlayout::layout(n_graph, n_attr);
		box_quiver(os, "node" + int_to_str(node->index()), n_graph, n_attr);
	}
	os << "\\begin{tikzpicture}[x=.1pt,y=.1pt]" << os.widen('\n');
	forall_nodes(node, graph) {
		std::string name = "node" + int_to_str(node->index());
//		os << "\\newlength{\\" << name << "h}" << os.widen('\n');
//		os << "\\newlength{\\" << name << "w}" << os.widen('\n');
//		os << "\\settoheight{\\" << name << "h}{\\usebox{\\" << name << "}}"
//			<< os.widen('\n') << "\\settowidth{\\" << name << "w}{\\usebox{\\"
//			<< name << "}}" << os.widen('\n');
		os << "\\node" 
//			"[minimum height=\\" << name <<"h,minimum width=\\" << name	<< "h]"
"[inner sep=.2pt]"
			" (n" << node->index() << ") at ("  << attr.x(node) << ","
			<< attr.y(node) << "){\\usebox{\\node" << int_to_str(node->index()) << "}};"
			<< os.widen('\n');
	}
	ogdf::edge e;
	forall_edges(e, graph) {
		os << "\\draw[line width=.01pt,blue](";
		os << "n" << e->source()->index() << ") -- (n" << e->target()->index()
			<< ");" << os.widen('\n');
	}
	os << "\\end{tikzpicture}" << os.widen('\n');
}
}
void usage() {
	std::cout << "qv2tex -[q|m|g] quiver" << std::endl;
	std::cout << "Takes a qv matrix and outputs the TeX to draw the quiver." << std::endl;
	std::cout << "   -q Draw a single quiver" << std::endl;
	std::cout << "   -m Draw the move graph of a quiver" << std::endl;
	std::cout << "   -g Draw the quiver graph of a quiver" << std::endl;
}
enum Func { quiver, move, graph, unset };
int main(int argc, char* argv[]) {
	Func func = unset;
	std::string mat_str;
	int c;

	while((c = getopt(argc, argv, "q:m:g:")) != -1) {
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
		ogdf::Graph graph = qvdraw::graph_factory::graph(matrix);
		ogdf::GraphAttributes attr(graph);
		qvlayout::layout(graph, attr);
		qv2tex::preamble(os);
		qv2tex::begin(os);
		qv2tex::draw_quiver(os, graph, attr);
		qv2tex::end(os);
	}
	if(func == Func::move) {
		cluster::EquivQuiverMatrix matrix(mat_str);
		cluster::MoveGraph<cluster::EquivQuiverMatrix>
			move(matrix, qvdraw::consts::Moves);
		qvdraw::GraphPair pair = std::move(qvdraw::graph_factory::multi_graph(move));
		qvdraw::NodeMap & map = pair.second;
		ogdf::Graph & graph = pair.first;
		ogdf::GraphAttributes attr(graph);
		qvlayout::layout(graph, attr, 10, qvlayout::Method::Layered);

		qv2tex::preamble(os);
		qv2tex::begin(os);
		qv2tex::draw_multi_graph(os, map, graph, attr);
		qv2tex::end(os);
	}
	if(func == Func::graph) {
		os << "Not implemented" << std::endl;
	}
	return 0;
}

