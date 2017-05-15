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

#include "qv/template_exchange_graph.h"
#include "qv/ginac_util.h"
#include "qv/green_exchange_graph.h"
#include "qv/move_graph.h"

#include "qvrefl/compatible_cartan_iterator.h"

#include "consts.h"
#include "graph_factory.h"
#include "layout.h"

namespace {
cluster::Seed::Cluster default_cluster(size_t size) {
  cluster::Seed::Cluster result(size);
  std::string var = "x_";
  for (size_t i = 0; i < size; ++i) {
    result[i] = cluster::ginac::symbol(var + std::to_string(i));
  }
  return result;
}
}
namespace qv2tex {
void preamble(std::ostream& os) {
  os << "\\documentclass{standalone}" << os.widen('\n');
  os << "\\usepackage{tkz-euclide}" << os.widen('\n');
  os << "\\usetkzobj{all}" << os.widen('\n');
  os << "\\tikzstyle{every picture}+=[>=stealth']" << os.widen('\n');
}
void begin(std::ostream& os) {
  os << "\\def\\qvsize{1pt}" << os.widen('\n');
  os << "\\def\\qvscale{0.05}" << os.widen('\n');
  os << "\\def\\grsize{0.5pt}" << os.widen('\n');
  os << "\\def\\grscale{1}" << os.widen('\n');
  os << "\\def\\picscale{1}" << os.widen('\n');
  os << "\\begin{document}%" << os.widen('\n');
}
void end(std::ostream& os) {
  os << "\\end{document}%" << os.widen('\n');
}
void draw_quiver(std::ostream& os,
                 const ogdf::Graph& graph,
                 const ogdf::GraphAttributes& attr,
                 const char* color = "black") {
  ogdf::node n;
  os << "\\begin{tikzpicture}[x=\\qvsize,y=\\qvsize]" << os.widen('\n');
  forall_nodes(n, graph) {
    os << "\\tkzDefPoint(" << attr.x(n) << "," << attr.y(n) << "){n";
    os << n->index() << "}%" << os.widen('\n');
  }
  ogdf::edge e;
  forall_edges(e, graph) {
    os << "\\draw[->,shorten <=5,shorten >=5," << color << "] (n"
       << e->source()->index() << ") -- (n";
    os << e->target()->index() << ")";
    if (attr.labelEdge(e).length() > 0) {
      os << "node[midway,above,sloped] {$" << attr.labelEdge(e) << "$}";
    }
    os << ";" << os.widen('\n');
  }
  os << "\\tkzDrawPoints[color=" << color << "](";
  forall_nodes(n, graph) {
    os << "n" << n->index();
    if (n->succ()) {
      os << ",";
    }
  }
  os << ");" << os.widen('\n');
  forall_nodes(n, graph) {
    if (attr.labelNode(n).length() > 0) {
      os << "\\tkzLabelPoint[color=" << color << "](n" << n->index() << "){$"
         << attr.labelNode(n);
      os << "$}%" << os.widen('\n');
    }
  }
  os << "\\draw[" << color << "](current bounding box.south west)rectangle"
                              "(current bounding box.north east);"
     << os.widen('\n');
  os << "\\end{tikzpicture}%" << os.widen('\n');
}
void box_quiver(std::ostream& os,
                const std::string& name,
                const ogdf::Graph& graph,
                const ogdf::GraphAttributes& attr,
                const char* color = "black") {
  os << "\\newsavebox{\\" << name << "}%" << os.widen('\n');
  os << "\\sbox{\\" << name << "}{%" << os.widen('\n');
  os << "\\scalebox{\\qvscale}{%" << os.widen('\n');
  draw_quiver(os, graph, attr, color);
  os << "}}%" << os.widen('\n');
}
namespace colouring {
template <class Seed>
struct GreenSeqExistence {
  const char* vertex_colour(Seed const* vertex) const {
    if (chk(vertex, 0)) {
      return "blue";
    } else {
      return "red";
    }
  }
  const char* edge_colour(Seed const* source, Seed const* dest) const {
    if (chk(source, 0) && chk(dest, 0)) {
      return "blue";
    } else {
      return "red";
    }
  }

 private:
  cluster::green_exchange::MultiArrowTriangleCheck chk;
};
struct AllBlack {
  const char* vertex_colour(void const* /* ignored */) const { return "black"; }
  const char* edge_colour(void const* /* ignored */,
                          void const* /* ignored */) const {
    return "black";
  }
};
struct FullyCompatible {
  typedef refl::cartan_exchange::CartanQuiver Quiver;
  const char* vertex_colour(Quiver const* quiv) const {
    if (quiv->fully_compatible) {
      return "black";
    } else {
      return "red";
    }
  }
  const char* edge_colour(Quiver const* source, Quiver const* dest) const {
    if (source->fully_compatible && dest->fully_compatible) {
      return "black";
    } else {
      return "red";
    }
  }
};
}
std::string int_to_str(int a) {
  char lookup[] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'k', 'i', 'j'};
  std::string result;
  if (a == 0) {
    result += lookup[0];
  }
  while (a > 0) {
    result += lookup[a % 10];
    a = a / 10;
  }
  return result;
}
template <class M, class Colour>
void draw_multi_graph(std::ostream& os,
                      const qvdraw::NodeMap<M>& map,
                      const ogdf::Graph& graph,
                      const ogdf::GraphAttributes& attr) {
  static Colour colouring;
  ogdf::node node;
  std::vector<ogdf::node> nodes_to_remove;
  forall_nodes(node, graph) {
    auto found = map.find(node);
    if (found != map.end()) {
      const M* mat = found->second;
      std::pair<std::shared_ptr<ogdf::Graph>,
                std::shared_ptr<ogdf::GraphAttributes>>
          pair = qvdraw::graph_factory::graph(*mat);
      ogdf::Graph& n_graph = *pair.first;
      ogdf::GraphAttributes& n_attr = *pair.second;
      qvlayout::layout(n_graph, n_attr);
      box_quiver(os, "node" + int_to_str(node->index()), n_graph, n_attr,
                 colouring.vertex_colour(mat));
    } else {
      /* Node is not a quiver/seed.
       * This happens when the graph is not completely contstructed e.g. in the
       * case where the exchange graph would otherwise be infinite. */
      nodes_to_remove.push_back(node);
    }
  }
  os << "\\scalebox{\\picscale}{%" << os.widen('\n');
  os << "\\begin{tikzpicture}[x=\\grsize,y=\\grsize,scale=\\grscale]"
     << os.widen('\n');
  forall_nodes(node, graph) {
    if (std::find(nodes_to_remove.begin(), nodes_to_remove.end(), node) !=
        nodes_to_remove.end()) {
      continue;
    }
    std::string name = "node" + int_to_str(node->index());
    os << "\\node[inner sep=0pt,outer sep=0pt]"
          " (n"
       << node->index() << ") at (" << attr.x(node) << "," << attr.y(node)
       << "){\\usebox{\\node" << int_to_str(node->index()) << "}};"
       << os.widen('\n');
  }
  ogdf::edge e;
  forall_edges(e, graph) {
    if (std::find_if(nodes_to_remove.begin(), nodes_to_remove.end(),
                     [&e](ogdf::node node) -> bool {
                       return (e->source()->index() == node->index()) ||
                              (e->target()->index() == node->index());
                     }) != nodes_to_remove.end()) {
      continue;
    }
    os << "\\draw[line width=.05pt,";
    os << colouring.edge_colour(map.find(e->source())->second,
                                map.find(e->target())->second);
    os << "](n" << e->source()->index() << ") -- (n" << e->target()->index()
       << ");" << os.widen('\n');
  }
  os << "\\end{tikzpicture}}%" << os.widen('\n');
}
template <class M, class Colouring, class Graph>
void output_multi_graph(const Graph& multi_gr, std::ostream& os) {
  /*
   * NB: The std::move here is important. Otherwise the graph ends up being
   * copied for some stupid reason. Then the nodes in the graph are different
   * to the nodes in the map.
   */
  qvdraw::GraphPair<M> pair =
      std::move(qvdraw::graph_factory::multi_graph<M>(multi_gr));
  qvdraw::NodeMap<M>& map = pair.second;
  ogdf::Graph& graph = pair.first;
  ogdf::GraphAttributes attr(graph);
  qvlayout::layout(graph, attr, 10, qvlayout::Method::Energy);

  qv2tex::preamble(os);
  qv2tex::begin(os);
  qv2tex::draw_multi_graph<M, Colouring>(os, map, graph, attr);
  qv2tex::end(os);
}
}
void usage() {
  std::cout << "qv2tex -lr [-n number] [-q|m|g|e|c quiver]" << std::endl;
  std::cout << "Takes a qv matrix and outputs the TeX to draw the quiver."
            << std::endl;
  std::cout << "   -q Draw a single quiver" << std::endl;
  std::cout << "   -m Draw the move graph of a quiver" << std::endl;
  std::cout << "   -g Draw the quiver graph of a quiver" << std::endl;
  std::cout
      << "   -e Draw the exchange graph of a quiver with cluster (x1 ... )"
      << std::endl;
	std::cout << "   -c Draw the quasi-Cartan companion exchange graph" << std::endl;
  std::cout << "   -l Draw the labelled exchange/quiver graph" << std::endl;
  std::cout << "   -n Limit the number of seeds computed to given number"
            << std::endl;
  std::cout
      << "   -r Don't compute mutations which do not lead to green sequences"
      << std::endl;
}
enum Func { quiver, move, graph, exchange, cartan, unset };
int main(int argc, char* argv[]) {
  Func func = unset;
  bool labelled = false;
  bool green = false;
  std::string mat_str;
  size_t limit = SIZE_MAX;
  int c;

  while ((c = getopt(argc, argv, "c:q:m:g:e:ln:r")) != -1) {
    switch (c) {
      case 'c':
        func = Func::cartan;
        mat_str = optarg;
        break;
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
      case 'r':
        green = true;
        break;
      case '?':
        usage();
        return 1;
        ;
      default:
        usage();
        return 1;
    }
  }
  if (func == Func::unset) {
    usage();
    return 1;
  }
  std::ostream& os = std::cout;
  if (func == Func::quiver) {
    cluster::IntMatrix matrix(mat_str);
    std::pair<std::shared_ptr<ogdf::Graph>,
              std::shared_ptr<ogdf::GraphAttributes>>
        pair = qvdraw::graph_factory::graph(matrix);
    ogdf::Graph& graph = *pair.first;
    ogdf::GraphAttributes& attr = *pair.second;
    qvlayout::layout(graph, attr);
    qv2tex::preamble(os);
    qv2tex::begin(os);
    qv2tex::draw_quiver(os, graph, attr);
    qv2tex::end(os);
  } else if (func == Func::move) {
    typedef cluster::EquivQuiverMatrix M;
    M matrix(mat_str);
    cluster::MoveGraph<M> move(matrix, qvdraw::consts::Moves);
    qv2tex::output_multi_graph<const M, qv2tex::colouring::AllBlack>(move, os);
  } else if (labelled && func == Func::graph) {
    typedef const cluster::QuiverMatrix M;
    M matrix(mat_str);
    if (green) {
      cluster::GreenLabelledQuiverGraph move(matrix, matrix.num_rows(), limit);
      qv2tex::output_multi_graph<M, qv2tex::colouring::GreenSeqExistence<M>>(
          move, os);
    } else {
      cluster::LabelledQuiverGraph move(matrix, matrix.num_rows(), limit);
      qv2tex::output_multi_graph<M, qv2tex::colouring::AllBlack>(move, os);
    }
  } else if (func == Func::graph) {
    typedef const cluster::EquivQuiverMatrix M;
    M matrix(mat_str);
    if (green) {
      cluster::GreenQuiverGraph move(matrix, matrix.num_rows(), limit);
      qv2tex::output_multi_graph<M, qv2tex::colouring::GreenSeqExistence<M>>(
          move, os);
    } else {
      cluster::QuiverGraph move(matrix, matrix.num_rows(), limit);
      qv2tex::output_multi_graph<M, qv2tex::colouring::AllBlack>(move, os);
    }
  } else if (labelled && func == Func::exchange) {
    typedef const cluster::LabelledSeed M;
    cluster::QuiverMatrix matrix(mat_str);
    M::Cluster cluster = default_cluster(matrix.num_rows());
    M seed(matrix, cluster);
    if (green) {
      cluster::LabelledExchangeGraph move(seed, seed.size(), limit);
      qv2tex::output_multi_graph<M, qv2tex::colouring::GreenSeqExistence<M>>(
          move, os);
    } else {
      cluster::LabelledExchangeGraph move(seed, seed.size(), limit);
      qv2tex::output_multi_graph<M, qv2tex::colouring::AllBlack>(move, os);
    }
  } else if (func == Func::exchange) {
    typedef const cluster::Seed M;
    cluster::QuiverMatrix matrix(mat_str);
    M::Cluster cluster = default_cluster(matrix.num_rows());
    M seed(matrix, cluster);
    if (green) {
      cluster::ExchangeGraph move(seed, seed.size(), limit);
      qv2tex::output_multi_graph<M, qv2tex::colouring::GreenSeqExistence<M>>(
          move, os);
    } else {
      cluster::ExchangeGraph move(seed, seed.size(), limit);
      qv2tex::output_multi_graph<M, qv2tex::colouring::AllBlack>(move, os);
    }
  } else if (func == Func::cartan) {
    typedef const refl::cartan_exchange::CartanQuiver M;
    cluster::EquivQuiverMatrix m(mat_str);
    refl::CompatibleCartanIterator init_cartan_iter(m);

    if (!init_cartan_iter.has_next()) {
      std::cerr << "Quiver has no fully compatible matrices"
                << std::cerr.widen('\n');
      return 5;
    }

    refl::cartan_exchange::CartanQuiver initial{m, init_cartan_iter.next(),
                                                true};

    refl::CartanExchangeGraph graph(initial, m.num_rows(), limit);
    qv2tex::output_multi_graph<M, qv2tex::colouring::FullyCompatible>(graph,
                                                                      os);
  }
  return 0;
}
