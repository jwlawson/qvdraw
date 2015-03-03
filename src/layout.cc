/**
 * layout.cc
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
 * Function to layout a graph.
 */
#include "layout.h"

#include <ogdf/energybased/FMMMLayout.h>

#include <ogdf/layered/SugiyamaLayout.h>
#include <ogdf/layered/OptimalRanking.h>
#include <ogdf/layered/MedianHeuristic.h>
#include <ogdf/layered/OptimalHierarchyLayout.h>

#include <ogdf/misclayout/BalloonLayout.h>

#include <ogdf/upward/DominanceLayout.h>
#include <ogdf/upward/LayerBasedUPRLayout.h>
#include <ogdf/upward/SubgraphUpwardPlanarizer.h>
#include <ogdf/upward/UpwardPlanarizationLayout.h>
#include <ogdf/upward/VisibilityLayout.h>
 
namespace qvlayout {
namespace {
typedef ogdf::Graph Graph;
typedef ogdf::GraphAttributes GraphA;
typedef ogdf::FMMMLayout FL;
typedef ogdf::UpwardPlanarizationLayout UPL;
}

void layout(Graph & graph, GraphA & attr, int size, Method method) {
	ogdf::node v;
	forall_nodes(v, graph) {
		attr.width(v) = size;
		attr.height(v) = size;
	}
	switch(method) {
		case Method::Energy:
			{
			FL l;
			l.useHighLevelOptions(true);
			l.qualityVersusSpeed(l.qvsGorgeousAndEfficient);
			/*
			 * Setting to true changes the output graphs. Sometimes they look better,
			 * other times they do not. I have no idea why. Default is false.
			 */
			l.newInitialPlacement(true);
			l.call(attr);
			break;
			}
		case Method::Hierachy:
			{
			UPL k;
			k.setUPRLayout(new ogdf::LayerBasedUPRLayout());
			k.setUpwardPlanarizer(new ogdf::SubgraphUpwardPlanarizer());
			k.call(attr);
			break;
			}
		case Method::Layered:
			{
			ogdf::SugiyamaLayout SL;
			SL.setRanking(new ogdf::OptimalRanking());
			SL.setCrossMin(new ogdf::MedianHeuristic());
			ogdf::OptimalHierarchyLayout * l = new ogdf::OptimalHierarchyLayout();
			SL.setLayout(l);
			SL.call(attr);
			break;
			}
		case Method::Dominance:
			{
			ogdf::DominanceLayout layout;
			layout.call(attr);
			break;
			}
		case Method::Visibility:
			{
			ogdf::VisibilityLayout layout;
			layout.call(attr);
			break;
			}
		case Method::Balloon:
			{
			ogdf::BalloonLayout layout;
			layout.call(attr);
			break;
			}
	}
}
}
