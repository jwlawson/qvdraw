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

#include <iostream>
#include <sstream>
#include <vector>

#include <ginac/ginac.h>

#include "qv/move_graph.h"
#include "qv/template_exchange_graph.h"
#include "qv/green_exchange_graph.h"

namespace qvdraw {
namespace graph_factory {
namespace {
const size_t MAX_EQ_LENGTH = 100;
template <class K>
ogdf::node get_node(ogdf::Graph& res, K* const mat, ReverseNodeMap<K>& map) {
  if (mat == nullptr) {
    return res.newNode();
  }
  auto find = map.find(mat);
  if (find == map.end()) {
    ogdf::node new_node = res.newNode();
    map.insert(std::make_pair(mat, new_node));
    return new_node;
  }
  return find->second;
}
template <class T>
NodeMap<T> switch_map(const ReverseNodeMap<T>& map) {
  NodeMap<T> result;
  for (auto it = map.begin(); it != map.end(); ++it) {
    result.insert(std::make_pair(it->second, it->first));
  }
  return std::move(result);
}
std::string latexify(const GiNaC::ex& exp) {
  std::string top, bottom;
  std::stringstream ss;
  ss << exp;
  std::string var;
  while (std::getline(ss, var, '*')) {
    size_t hat_pos = var.find("^(");
    if (hat_pos == std::string::npos) {
      top.append(" ").append(var);
    } else {
      bottom.append(" ").append(var.substr(0, hat_pos));
      size_t min_pos = var.find('-');
      size_t end_pos = var.find(')');
      std::string exponent = var.substr(min_pos + 1, end_pos - min_pos - 1);
      if (exponent != "1") {
        bottom.append("^").append(exponent);
      }
    }
  }
  if (top.length() > MAX_EQ_LENGTH) {
    top.erase();
  }
  return "\\frac{" + top + "}{" + bottom + "}";
}
}  // anonymous namespace
std::pair<std::shared_ptr<ogdf::Graph>, std::shared_ptr<ogdf::GraphAttributes>>
graph(const cluster::IntMatrix& matrix) {
  std::shared_ptr<ogdf::Graph> graph = std::make_shared<ogdf::Graph>();
  int size = std::max(matrix.num_cols(), matrix.num_rows());
  std::vector<ogdf::node> nodes(size);
  for (int i = 0; i < size; i++) {
    nodes[i] = graph->newNode();
  }
  for (int i = 0; i < matrix.num_rows(); i++) {
    for (int j = 0; j < matrix.num_cols(); j++) {
      int k = matrix.get(i, j);
      if (k > 0) {
        graph->newEdge(nodes[i], nodes[j]);
      }
    }
  }
  std::shared_ptr<ogdf::GraphAttributes> attr =
      std::make_shared<ogdf::GraphAttributes>(*graph);
  attr->initAttributes(ogdf::GraphAttributes::edgeLabel);
  attr->initAttributes(ogdf::GraphAttributes::nodeLabel);
  for (int i = 0; i < matrix.num_rows(); i++) {
    for (int j = 0; j < matrix.num_cols(); j++) {
      int k = matrix.get(i, j);
      if (k > 0) {
        ogdf::edge e = graph->searchEdge(nodes[i], nodes[j]);
        int k_tra = matrix.get(j, i);
        if (k != -1 * k_tra) {
          /* Symmetrisable, so add pair of labels. */
          std::string l;
          if (i < j) {
            l = std::to_string(k) + "," + std::to_string(-1 * k_tra);
          } else {
            l = std::to_string(-1 * k_tra) + "," + std::to_string(k);
          }
          attr->labelEdge(e) = ogdf::String(l.length(), l.c_str());
        } else if (k != 1 && k == -1 * k_tra) {
          /* Skew-symmetric, so add single label. */
          std::string lab = std::to_string(k);
          attr->labelEdge(e) = ogdf::String(lab.length(), lab.c_str());
        }
      }
    }
  }
  return std::make_pair<std::shared_ptr<ogdf::Graph>,
                        std::shared_ptr<ogdf::GraphAttributes>>(
      std::move(graph), std::move(attr));
}
template <class M>
std::pair<std::shared_ptr<ogdf::Graph>, std::shared_ptr<ogdf::GraphAttributes>>
graph(const cluster::__Seed<M>& seed) {
  auto result = graph(seed.matrix());
  std::shared_ptr<ogdf::GraphAttributes> attr = result.second;
  const typename cluster::__Seed<M>::Cluster& cluster = seed.cluster();
  ogdf::node n;
  std::stringstream ss;
  forall_nodes(n, *result.first) {
    /* Is there a better way of getting the string form of the cluster
     * variables? */
    attr->labelNode(n) = latexify(cluster[n->index()]).c_str();
  }
  return result;
}
std::pair<std::shared_ptr<ogdf::Graph>, std::shared_ptr<ogdf::GraphAttributes>>
graph(const refl::cartan_exchange::CartanQuiver& seed) {
  auto result = graph(seed.quiver);
  std::shared_ptr<ogdf::GraphAttributes> attr = result.second;
  arma::Mat<int> const& cartan = seed.cartan;
  ogdf::edge e;
  std::stringstream ss;
  forall_edges(e, *result.first) {
    ogdf::node begin = e->source();
    ogdf::node end = e->target();
    size_t row = begin->index();
    size_t col = end->index();
    if (cartan(row, col) < 0) {
      attr->labelEdge(e) += "-";
    }
  }
  return result;
}
template <class NodeType, class G>
GraphPair<NodeType> multi_graph(const G& g) {
  ogdf::Graph result;
  ReverseNodeMap<NodeType> map;
  for (auto iter = g.begin(); iter != g.end(); ++iter) {
    ogdf::node node = get_node(result, iter->first, map);
    for (auto link_iter = (iter->second).begin();
         link_iter != (iter->second).end(); ++link_iter) {
      ogdf::node linked = get_node(result, *link_iter, map);
      result.newEdge(node, linked);
    }
  }
  NodeMap<NodeType> map_result = switch_map(map);
  return {result, map_result};
}
template GraphPair<const cluster::EquivQuiverMatrix>
multi_graph<const cluster::EquivQuiverMatrix>(
    const cluster::MoveGraph<cluster::EquivQuiverMatrix>&);
template GraphPair<const cluster::QuiverMatrix> multi_graph(
    const cluster::LabelledQuiverGraph&);
template GraphPair<const cluster::EquivQuiverMatrix> multi_graph(
    const cluster::QuiverGraph&);
template GraphPair<const cluster::Seed> multi_graph(
    const cluster::ExchangeGraph&);
template GraphPair<const cluster::LabelledSeed> multi_graph(
    const cluster::LabelledExchangeGraph&);
template GraphPair<const cluster::QuiverMatrix> multi_graph(
    const cluster::GreenLabelledQuiverGraph&);
template GraphPair<const cluster::EquivQuiverMatrix> multi_graph(
    const cluster::GreenQuiverGraph&);
template GraphPair<const cluster::Seed> multi_graph(
    const cluster::GreenExchangeGraph&);
template GraphPair<const cluster::LabelledSeed> multi_graph(
    const cluster::GreenLabelledExchangeGraph&);
template GraphPair<const refl::cartan_exchange::CartanQuiver> multi_graph(
    const refl::CartanExchangeGraph&);
template std::pair<std::shared_ptr<ogdf::Graph>,
                   std::shared_ptr<ogdf::GraphAttributes>>
graph(const cluster::Seed&);
template std::pair<std::shared_ptr<ogdf::Graph>,
                   std::shared_ptr<ogdf::GraphAttributes>>
graph(const cluster::LabelledSeed&);
}
template <>
bool NodeEquals<refl::cartan_exchange::CartanQuiver const*>::operator()(
    refl::cartan_exchange::CartanQuiver const* const& lhs,
    refl::cartan_exchange::CartanQuiver const* const& rhs) const {
  refl::cartan_exchange::Equal equals;
  return equals(lhs, rhs);
}
template <class NodeType>
bool NodeEquals<NodeType>::operator()(const NodeType& lhs,
                                      const NodeType& rhs) const {
  return lhs->equals(*rhs);
}
template <>
size_t NodeHash<refl::cartan_exchange::CartanQuiver const*>::operator()(
    refl::cartan_exchange::CartanQuiver const* const& node) const {
  refl::cartan_exchange::Hash hash;
  return hash(node);
}
template <class NodeType>
size_t NodeHash<NodeType>::operator()(const NodeType& node) const {
  return node->hash();
}
bool ogdfnodeEquals::operator()(const ogdf::node& lhs,
                                const ogdf::node& rhs) const {
  return lhs == rhs;
}
size_t ogdfnodeHash::operator()(const ogdf::node& node) const {
  return (size_t)node;
}
}
