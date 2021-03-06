/**
 * consts.cc
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
 * Contains all the class representatives and all moves.
 *
 * This just initialises the data structures used by the program to check
 * moves. There are some helper functions to create the moves, matrices and
 * graphs.
 */
#include "consts.h"

#include "qv/mass_finite_check.h"

namespace qvdraw {
namespace consts {

namespace {
	cluster::MMIMove make_move(const std::string& a,
			const std::string& b, std::initializer_list<int> c,
			std::initializer_list<cluster::MMIMove::ConnReq> r) {
		return cluster::MMIMove(
				cluster::IntMatrix(a), cluster::IntMatrix(b),
				std::vector<int>(c), std::vector<cluster::MMIMove::ConnReq>(r));
	}
	template<typename F>
	cluster::MMIMove make_move(const std::string& a,
			const std::string& b, std::initializer_list<int> c,
			std::initializer_list<cluster::MMIMove::ConnReq> r,
			cluster::mmi_conn::Finite<F> atob) {
		auto res = cluster::MMIMove(
				cluster::IntMatrix(a), cluster::IntMatrix(b),
				std::vector<int>(c), std::vector<cluster::MMIMove::ConnReq>(r));
		res.finite_req_atob(atob);
		return res;
	}
	template<typename F1, typename F2>
	cluster::MMIMove make_move(const std::string& a,
			const std::string& b, std::initializer_list<int> c,
			std::initializer_list<cluster::MMIMove::ConnReq> r,
			cluster::mmi_conn::Finite<F1> atob,
			cluster::mmi_conn::Finite<F2> btoa) {
		auto res = cluster::MMIMove(
				cluster::IntMatrix(a), cluster::IntMatrix(b),
				std::vector<int>(c), std::vector<cluster::MMIMove::ConnReq>(r));
		res.finite_req_atob(atob);
		res.finite_req_btoa(btoa);
		return res;
	}
	struct MassFinite {
		bool operator()(const cluster::EquivQuiverMatrix & mat) {
			return chk.is_finite(mat);
		}
		private:
		static cluster::MassFiniteCheck chk;
	};
	cluster::MassFiniteCheck MassFinite::chk;
	typedef cluster::mmi_conn::Finite<MassFinite> FinReq;
}
using namespace cluster::mmi_conn;
std::vector<cluster::MMIMove> Moves = {
	/* Moves for 6 x 6 */
	make_move("{ { 0 1 0 } { -1 0 1 } { 0 -1 0 } }",
			"{ { 0 -1 1 } { 1 0 -1 } { -1 1 0 } }", {0, 2}, {Unconnected(), Line()}),
	make_move("{ { 0 -1 0 } { 1 0 -1 } { 0 1 0 } }",/*Transpose*/
			"{ { 0 1 -1 } { -1 0 1 } { 1 -1 0 } }", {0, 2}, {Unconnected(), Line()}),
	make_move("{ { 0 1 0 0 } { -1 0 1 1 } { 0 -1 0 0 } { 0 -1 0 0 } }",
			"{ { 0 -1 1 1 } { 1 0 -1 -1 } { -1 1 0 0 } { -1 1 0 0 } }",
			{ 0, 2, 3 }, { Line(), ConnectedTo(3), ConnectedTo(2) }),
	make_move("{ { 0 -1 0 0 } { 1 0 -1 -1 } { 0 1 0 0 } { 0 1 0 0 } }", /*Transpose */
			"{ { 0 1 -1 -1 } { -1 0 1 1 } { 1 -1 0 0 } { 1 -1 0 0 } }",
			{ 0, 2, 3 }, { Line(), ConnectedTo(3), ConnectedTo(2) }),
	make_move("{ { 0 0 -1 0 0 } { 0 0 -1 0 0 } { 1 1 0 1 -1 } { 0 0 -1 0 2 } "
			"{ 0 0 1 -2 0 } }", "{ { 0 -1 0 0 1 } { 1 0 -1 1 -1 } { 0 1 0 0 1 } "
			"{ 0 -1 0 0 1 } { -1 1 -1 -1 0 } }", {0}, {Unconnected()}),
	make_move("{ { 0 1 0 0 0 } { -1 0 -1 -1 1 } { 0 1 0 0 0 } { 0 1 0 0 0 } "
			"{ 0 -1 0 0 0 } }", "{ { 0 -1 0 0 1 } { 1 0 1 1 -1 } { 0 -1 0 0 1 } "
			"{ 0 -1 0 0 1 } { -1 1 -1 -1 0 } }", {0, 4}, {Unconnected(), Line()}),
	make_move("{ { 0 -1 0 0 0 } { 1 0 1 1 -1 } { 0 -1 0 0 0 } { 0 -1 0 0 0 } " /*Transpose*/
			"{ 0 1 0 0 0 } }", "{ { 0 1 0 0 -1 } { -1 0 -1 -1 1 } { 0 1 0 0 -1 } "
			"{ 0 1 0 0 -1 } { 1 -1 1 1 0 } }", {0, 4}, {Unconnected(), Line()}),
	/* 6 x 6 'specials' */
	make_move("{ { 0 -1 -1 -1 -1 1 } { 1 0 0 0 0 0 } { 1 0 0 0 0 0 } "
			"{ 1 0 0 0 0 0 } { 1 0 0 0 0 0 } { -1 0 0 0 0 0 } }",
			"{ { 0 1 1 1 1 -1 } { -1 0 0 0 0 1 } { -1 0 0 0 0 1 } { -1 0 0 0 0 1 } "
			"{ -1 0 0 0 0 1 } { 1 -1 -1 -1 -1 0 } }", {} , {}),
	make_move("{ { 0 -1 -1 -1 1 -1 } { 1 0 0 0 0 0 } { 1 0 0 0 0 0 } "
			"{ 1 0 0 0 0 0 } { -1 0 0 0 0 2 } { 1 0 0 0 -2 0 } }",
			"{ { 0 1 1 1 -1 1 } { -1 0 0 0 1 0 } { -1 0 0 0 1 0 } { -1 0 0 0 1 0 } "
			"{ 1 -1 -1 -1 0 1 } { -1 0 0 0 -1 0 } }", {}, {}),
	/* Moves for 7 x 7 */
	make_move("{ { 0 1 0 -1 } { -1 0 1 1 } { 0 -1 0 -1 } { 1 -1 1 0 } }",
			"{ { 0 1 0 0 } { -1 0 -1 1 } { 0 1 0 -2 } { 0 -1 2 0 } }",
			{ 0 }, {Unconnected()}),
	make_move("{ { 0 -1 0 1 } { 1 0 -1 -1 } { 0 1 0 1 } { -1 1 -1 0 } }", /*Transpose*/
			"{ { 0 -1 0 0 } { 1 0 1 -1 } { 0 -1 0 2 } { 0 1 -2 0 } }",
			{ 0 }, {Unconnected()}),
	make_move("{ { 0 1 0 0 } { -1 0 1 -1 } { 0 -1 0 1 } { 0 1 -1 0 } }",
			"{ { 0 -1 1 0 } { 1 0 -1 1 } { -1 1 0 0 } { 0 -1 0 0 } }",
			{0, 2, 3}, { Unconnected(), LineTo(3), LineTo(2) }),
	make_move("{ { 0 -1 0 0 } { 1 0 -1 1 } { 0 1 0 -1 } { 0 -1 1 0 } }",
			"{ { 0 1 -1 0 } { -1 0 1 -1 } { 1 -1 0 0 } { 0 1 0 0 } }",
			{0, 2, 3}, { Unconnected(), LineTo(3), LineTo(2) }),
	make_move("{ { 0 1 0 0 0 } { -1 0 -1 1 -1 } { 0 1 0 0 0 } { 0 -1 0 0 1 } "
			"{ 0 1 0 -1 0 } }",
			"{ { 0 -1 0 1 0 } { 1 0 1 -1 1 } { 0 -1 0 1 0 } { -1 1 -1 0 0 } "
			"{ 0 -1 0 0 0 } }",
			{0, 3, 4 }, {Unconnected(), LineTo(4), LineTo(3) }),
	make_move("{ { 0 -1 0 0 0 } { 1 0 1 -1 1 } { 0 -1 0 0 0 } { 0 1 0 0 -1 } "/*Transpose*/
			"{ 0 -1 0 1 0 } }",
			"{ { 0 1 0 -1 0 } { -1 0 -1 1 -1 } { 0 1 0 -1 0 } { 1 -1 1 0 0 } "
			"{ 0 1 0 0 0 } }",
			{0, 3, 4 }, {Unconnected(), LineTo(4), LineTo(3) }),
	/* 7 x 7 'special' */
	make_move("{ { 0 0 -1 0 0 0 } { 0 0 -1 0 0 0 } { 1 1 0 1 -1 0 } "
			"{ 0 0 -1 0 1 -1 } { 0 0 1 -1 0 1 } { 0 0 0 1 -1 0 } }",
			"{ { 0 1 0 0 -1 0 } { -1 0 -1 -1 1 0 } { 0 1 0 0 0 -1 } "
			"{ 0 1 0 0 -1 0 } { 1 -1 0 1 0 1 } { 0 0 1 0 -1 0 } }",
			{5},{Line()}),
	make_move("{ { 0 0 1 0 0 0 } { 0 0 1 0 0 0 } { -1 -1 0 -1 1 0 } " /*Transpose*/
			"{ 0 0 1 0 -1 1 } { 0 0 -1 1 0 -1 } { 0 0 0 -1 1 0 } }",
			"{ { 0 -1 0 0 1 0 } { 1 0 1 1 -1 0 } { 0 -1 0 0 0 1 } "
			"{ 0 -1 0 0 1 0 } { -1 1 0 -1 0 -1 } { 0 0 -1 0 1 0 } }",
			{5},{Line()}),
	/* 8 x 8 moves */
	make_move("{ { 0 1 0 0 0 } { -1 0 -1 0 0 } { 0 1 0 -1 1 } { 0 0 1 0 0 } "
			"{ 0 0 -1 0 0 } }",
			"{ { 0 -1 1 0 0 } { 1 0 -1 1 0 } { -1 1 0 0 -1 } { 0 -1 0 0 1 } "
			"{ 0 0 1 -1 0 } }",
			{0, 3, 4}, {Unconnected(), Line(), Line() }),
	make_move("{ { 0 1 0 -1 } { -1 0 -1 1 } { 0 1 0 0 } { 1 -1 0 0 } }",
			"{ { 0 -1 0 0 } { 1 0 1 -1 } { 0 -1 0 1 } { 0 1 -1 0 } }",
			{0, 2, 3}, {Unconnected(), Unconnected(), Unconnected() }),
	make_move("{ { 0 1 0 0 } { -1 0 1 -1 } { 0 -1 0 1 } { 0 1 -1 0 } }",
			"{ { 0 -1 1 0 } { 1 0 -1 1 } { -1 1 0 0 } { 0 -1 0 0 } }",
			{0, 2, 3}, {Unconnected(), LineTo(3), LineTo(2) }),
	make_move("{ { 0 -1 0 0 0 0 } { 1 0 1 0 0 0 } { 0 -1 0 1 -1 0 } "
			"{ 0 0 -1 0 0 0 } { 0 0 1 0 0 1 } { 0 0 0 0 -1 0 } }",
			"{ { 0 1 -1 0 0 0 } { -1 0 1 -1 0 0 } { 1 -1 0 1 -1 1 } { 0 1 -1 0 1 0 } "
			"{ 0 0 1 -1 0 -1 } { 0 0 -1 0 1 0 } }",
			{0, 3, 5}, {Line(), Line(), Line() }),
	make_move("{ { 0 1 0 0 0 0 } { -1 0 -1 0 0 0 } { 0 1 0 -1 1 0 } " /*Transpose*/
			"{ 0 0 1 0 0 0 } { 0 0 -1 0 0 -1 } { 0 0 0 0 1 0 } }",
			"{ { 0 -1 1 0 0 0 } { 1 0 -1 1 0 0 } { -1 1 0 -1 1 -1 } { 0 -1 1 0 -1 0 } "
			"{ 0 0 -1 1 0 1 } { 0 0 1 0 -1 0 } }",
			{0, 3, 5}, {Line(), Line(), Line() }),
	make_move("{ { 0 0 0 -1 1 } { 0 0 0 0 -1 } { 0 0 0 -1 1 } { 1 0 1 0 -1 } "
			"{ -1 1 -1 1 0 } }",
			"{ { 0 1 0 0 -1 } { -1 0 -1 0 1 } { 0 1 0 0 -1 } { 0 0 0 0 1 } "
			"{ 1 -1 1 -1 0 } }",
			{ 0, 1, 2, 3}, {Unconnected(),Unconnected(),Unconnected(),Unconnected()}),
	/* 8 x 8 special */
	make_move("{ { 0 1 0 0 0 0 0 0 } { -1 0 -1 1 0 0 0 0 } { 0 1 0 0 0 0 0 0 } "
			"{ 0 -1 0 0 -1 0 0 0 } { 0 0 0 1 0 1 -1 0 } { 0 0 0 0 -1 0 0 0 } "
			"{ 0 0 0 0 1 0 0 1 } { 0 0 0 0 0 0 -1 0 } }",
			"{ { 0 -1 0 0 1 0 0 0 } { 1 0 1 1 -1 0 0 0 } { 0 -1 0 0 1 0 0 0 } "
			"{ 0 -1 0 0 0 0 1 0 } { -1 1 -1 0 0 -1 0 0 } { 0 0 0 0 1 0 -1 0 } "
			"{ 0 0 0 -1 0 1 0 1 } { 0 0 0 0 0 0 -1 0 } }",
			{}, {}),
	make_move("{ { 0 1 0 0 -1 0 0 -1 } { -1 0 -1 0 1 0 -1 0 } { 0 1 0 -1 0 0 0 0 } "
			"{ 0 0 1 0 -1 0 0 0 } { 1 -1 0 1 0 1 0 0 } { 0 0 0 0 -1 0 1 0 } "
			"{ 0 1 0 0 0 -1 0 0 } { 1 0 0 0 0 0 0 0 } }",
			"{ { 0 0 0 0 1 0 0 -1 } { 0 0 -1 1 -1 1 -1 0 } { 0 1 0 -1 0 0 0 0 } "
			"{ 0 -1 1 0 1 0 0 0 } { -1 1 0 -1 0 -1 0 0 } { 0 -1 0 0 1 0 1 0 } "
			"{ 0 1 0 0 0 -1 0 0 } { 1 0 0 0 0 0 0 0 } }",
			{}, {}),
	/* 9 x 9 moves */
	make_move("{ { 0 1 0 0 0 -1 } { -1 0 -1 0 0 1 } { 0 1 0 -1 0 0 } "
			"{ 0 0 1 0 -1 0 } { 0 0 0 1 0 -1 } { 1 -1 0 0 1 0 } }",
			"{ { 0 0 0 0 0 1 } { 0 0 -1 0 1 -1 } { 0 1 0 -1 0 0 } { 0 0 1 0 -1 0 } "
			"{ 0 -1 0 1 0 1 } { -1 1 0 0 -1 0 } }",
			{0, 3, 4}, {Unconnected(), Line(), Unconnected()}),
	make_move("{ { 0 -1 1 -1 0 } { 1 0 -1 0 0 } { -1 1 0 1 -1 } { 1 0 -1 0 1 } "
			"{ 0 0 1 -1 0 } }",
			"{ { 0 -1 0 0 0 } { 1 0 -1 1 0 } { 0 1 0 0 0 } { 0 -1 0 0 1 } "
			"{ 0 0 0 -1 0 } }",
			{0, 4}, {Line(), Line()}),
	make_move("{ { 0 1 -1 1 0 } { -1 0 1 0 0 } { 1 -1 0 -1 1 } { -1 0 1 0 -1 } "/*Transpose*/
			"{ 0 0 -1 1 0 } }",
			"{ { 0 1 0 0 0 } { -1 0 1 -1 0 } { 0 -1 0 0 0 } { 0 1 0 0 -1 } "
			"{ 0 0 0 1 0 } }",
			{0, 4}, {Line(), Line()}),
	make_move("{ { 0 -1 0 1 } { 1 0 -1 0 } { 0 1 0 -1 } { -1 0 1 0 } }",
			"{ { 0 1 0 0 } { -1 0 1 1 } { 0 -1 0 0 } { 0 -1 0 0 } }",
			{0, 3}, {Line(), Line()}),
	make_move("{ { 0 -1 1 0 0 0 } { 1 0 -1 0 0 0 } { -1 1 0 1 0 0 } "
			"{ 0 0 -1 0 1 1 } { 0 0 0 -1 0 0 } { 0 0 0 -1 0 0 } }",
			"{ { 0 -1 1 0 0 0 } { 1 0 -1 0 0 0 } { -1 1 0 -1 1 1 } "
			"{ 0 0 1 0 -1 -1 } { 0 0 -1 1 0 0 } { 0 0 -1 1 0 0 } }",
			{0, 1, 5}, {ConnectedTo(5), Line(), ConnectedTo(0)}),
	make_move("{ { 0 1 -1 0 0 0 } { -1 0 1 0 0 0 } { 1 -1 0 -1 0 0 } "/*Transpose*/
			"{ 0 0 1 0 -1 -1 } { 0 0 0 1 0 0 } { 0 0 0 1 0 0 } }",
			"{ { 0 1 -1 0 0 0 } { -1 0 1 0 0 0 } { 1 -1 0 1 -1 -1 } "
			"{ 0 0 -1 0 1 1 } { 0 0 1 -1 0 0 } { 0 0 1 -1 0 0 } }",
			{0, 1, 5}, {ConnectedTo(5), Line(), ConnectedTo(0)}),
	make_move("{ { 0 1 -1 0 0 0 } { -1 0 1 -1 0 0 } { 1 -1 0 1 -1 0 } "
			"{ 0 1 -1 0 1 -1 } { 0 0 1 -1 0 1 } { 0 0 0 1 -1 0 } }",
			"{ { 0 -1 0 0 0 0 } { 1 0 1 0 0 0 } { 0 -1 0 1 -1 0 } { 0 0 -1 0 0 0 } "
			"{ 0 0 1 0 0 -1 } { 0 0 0 0 1 0 } }",
			{0, 5}, {Line(), Line()}),
	make_move("{ { 0 -1 1 0 0 0 } { 1 0 -1 1 0 0 } { -1 1 0 -1 1 0 } "/*Transpose*/
			"{ 0 -1 1 0 -1 1 } { 0 0 -1 1 0 -1 } { 0 0 0 -1 1 0 } }",
			"{ { 0 1 0 0 0 0 } { -1 0 -1 0 0 0 } { 0 1 0 -1 1 0 } { 0 0 1 0 0 0 } "
			"{ 0 0 -1 0 0 1 } { 0 0 0 0 -1 0 } }",
			{0, 5}, {Line(), Line()}),
	make_move("{ { 0 1 -1 0 0 0 } { -1 0 1 1 0 -1 } { 1 -1 0 0 -1 1 } "
			"{ 0 -1 0 0 0 1 } { 0 0 1 0 0 -1 } { 0 1 -1 -1 1 0 } }",
			"{ { 0 1 0 0 0 0 } { -1 0 -1 0 0 0 } { 0 1 0 -1 0 0 } { 0 0 1 0 -1 1 } "
			"{ 0 0 0 1 0 0 } { 0 0 0 -1 0 0 } } ",
			{0, 5}, {Line(), Line()}),
	make_move("{ { 0 -1 0 1 0 0 0 } { 1 0 -1 -1 1 0 0 } { 0 1 0 0 0 0 0 } "
			"{ -1 1 0 0 -1 1 -1 } { 0 -1 0 1 0 -1 0 } { 0 0 0 -1 1 0 1 } "
			"{ 0 0 0 1 0 -1 0 } }",
			"{ { 0 1 0 0 0 0 0 } { -1 0 -1 0 0 0 0 } { 0 1 0 -1 -1 0 0 } "
			"{ 0 0 1 0 0 0 0 } { 0 0 1 0 0 1 0 } { 0 0 0 0 -1 0 1 } "
			"{ 0 0 0 0 0 -1 0 } }",
			{0, 6}, {Line(), Line()}),
	make_move("{ { 0 1 0 -1 0 0 0 } { -1 0 1 1 -1 0 0 } { 0 -1 0 0 0 0 0 } "/* Transpose*/
			"{ 1 -1 0 0 1 -1 1 } { 0 1 0 -1 0 1 0 } { 0 0 0 1 -1 0 -1 } "
			"{ 0 0 0 -1 0 1 0 } }",
			"{ { 0 -1 0 0 0 0 0 } { 1 0 1 0 0 0 0 } { 0 -1 0 1 1 0 0 } "
			"{ 0 0 -1 0 0 0 0 } { 0 0 -1 0 0 -1 0 } { 0 0 0 0 1 0 -1 } "
			"{ 0 0 0 0 0 1 0 } }",
			{0, 6}, {Line(), Line()}),
	make_move("{ { 0 1 -1 0 0 0 } { -1 0 1 -1 0 0 } { 1 -1 0 0 -1 1 } "
			"{ 0 1 0 0 0 -1 } { 0 0 1 0 0 -1 } { 0 0 -1 1 1 0 } }",
			"{ { 0 1 0 0 0 0 } { -1 0 1 0 0 0 } { 0 -1 0 -1 0 0 } { 0 0 1 0 1 -1 } "
			"{ 0 0 0 -1 0 0 } { 0 0 0 1 0 0 } }",
			{0, 5}, {Line(), Line()}),
	make_move("{ { 0 -1 1 0 0 0 } { 1 0 -1 1 0 0 } { -1 1 0 0 1 -1 } "/*Transpose*/
			"{ 0 -1 0 0 0 1 } { 0 0 -1 0 0 1 } { 0 0 1 -1 -1 0 } }",
			"{ { 0 -1 0 0 0 0 } { 1 0 -1 0 0 0 } { 0 1 0 1 0 0 } { 0 0 -1 0 -1 1 } "
			"{ 0 0 0 1 0 0 } { 0 0 0 -1 0 0 } }",
			{0, 5}, {Line(), Line()}),
	make_move("{ { 0 1 -1 0 0 0 } { -1 0 1 0 0 -1 } { 1 -1 0 -1 1 0 } "
			"{ 0 0 1 0 0 0 } { 0 0 -1 0 0 1 } { 0 1 0 0 -1 0 } }",
			"{ { 0 0 -1 0 0 0 } { 0 0 -1 -1 1 0 } { 1 1 0 0 0 0 } { 0 1 0 0 0 -1 } "
			"{ 0 -1 0 0 0 0 } { 0 0 0 1 0 0 } }",
			{0, 5}, {Line(), Line()}),
	make_move("{ { 0 -1 1 0 0 0 } { 1 0 -1 0 0 1 } { -1 1 0 1 -1 0 } "/*Transpose*/
			"{ 0 0 -1 0 0 0 } { 0 0 1 0 0 -1 } { 0 -1 0 0 1 0 } }",
			"{ { 0 0 1 0 0 0 } { 0 0 1 1 -1 0 } { -1 -1 0 0 0 0 } { 0 -1 0 0 0 1 } "
			"{ 0 1 0 0 0 0 } { 0 0 0 -1 0 0 } }",
			{0, 5}, {Line(), Line()}),
	make_move("{ { 0 -1 0 1 0 0 0 } { 1 0 1 -1 0 0 0 } { 0 -1 0 1 0 -1 0 } "
			"{ -1 1 -1 0 1 0 0 } { 0 0 0 -1 0 1 -1 } { 0 0 1 0 -1 0 1 } "
			"{ 0 0 0 0 1 -1 0 } }",
			"{ { 0 1 0 0 0 0 0 } { -1 0 -1 0 0 0 0 } { 0 1 0 -1 -1 0 0 } "
			"{ 0 0 1 0 0 0 0 } { 0 0 1 0 0 1 0 } { 0 0 0 0 -1 0 1 } "
			"{ 0 0 0 0 0 -1 0 } }",
			{0, 6}, {Line(), Line()}),
	make_move("{ { 0 1 0 -1 0 0 0 } { -1 0 -1 1 0 0 0 } { 0 1 0 -1 0 1 0 } "/*Transpose*/
			"{ 1 -1 1 0 -1 0 0 } { 0 0 0 1 0 -1 1 } { 0 0 -1 0 1 0 -1 } "
			"{ 0 0 0 0 -1 1 0 } }",
			"{ { 0 -1 0 0 0 0 0 } { 1 0 1 0 0 0 0 } { 0 -1 0 1 1 0 0 } "
			"{ 0 0 -1 0 0 0 0 } { 0 0 -1 0 0 -1 0 } { 0 0 0 0 1 0 -1 } "
			"{ 0 0 0 0 0 1 0 } }",
			{0, 6}, {Line(), Line()}),
	make_move("{ { 0 -1 1 0 0 0 0 0 } { 1 0 -1 1 0 0 0 0 } { -1 1 0 0 -1 0 0 0 } { 0 -1 0 0 1 -1 0 0 } { 0 0 1 -1 0 0 1 0 } { 0 0 0 1 0 0 -1 1 } { 0 0 0 0 -1 1 0 -1 } { 0 0 0 0 0 -1 1 0 } }",
			"{ { 0 1 0 0 0 0 0 0 } { -1 0 -1 0 0 0 0 0 } { 0 1 0 1 0 0 0 0 } { 0 0 -1 0 1 -1 0 0 } { 0 0 0 -1 0 0 0 0 } { 0 0 0 1 0 0 1 0 } { 0 0 0 0 0 -1 0 -1 } { 0 0 0 0 0 0 1 0 } }",
			{0, 7}, {Line(), Line()}),
	make_move("{ { 0 -1 1 0 0 -1 } { 1 0 -1 0 0 0 } { -1 1 0 1 0 0 } { 0 0 -1 0 1 1 } { 0 0 0 -1 0 0 } { 1 0 0 -1 0 0 } }",
			"{ { 0 -1 1 0 0 -1 } { 1 0 -1 0 0 0 } { -1 1 0 -1 1 1 } { 0 0 1 0 -1 -1 } { 0 0 -1 1 0 0 } { 1 0 -1 1 0 0 } }",
			{0, 1, 5}, {ConnectedTo(5), Line(), ConnectedTo(0)}),
	/* 9 x 9 special */
	make_move("{ { 0 -1 0 0 0 0 0 0 0 } { 1 0 1 0 0 0 -1 0 0 } { 0 -1 0 -1 0 0 1 0 0 } { 0 0 1 0 -1 0 0 1 0 } { 0 0 0 1 0 -1 0 0 0 } { 0 0 0 0 1 0 -1 0 1 } { 0 1 -1 0 0 1 0 0 0 } { 0 0 0 -1 0 0 0 0 0 } { 0 0 0 0 0 -1 0 0 0 } }",
			"{ { 0 1 0 0 0 0 0 0 0 } { -1 0 1 0 0 0 0 0 0 } { 0 -1 0 -1 0 0 0 0 0 } { 0 0 1 0 1 0 0 0 0 } { 0 0 0 -1 0 -1 1 0 0 } { 0 0 0 0 1 0 0 0 0 } { 0 0 0 0 -1 0 0 1 0 } { 0 0 0 0 0 0 -1 0 -1 } { 0 0 0 0 0 0 0 1 0 } }",
			{}, {}),
	/* 31 */
	make_move("{ { 0 1 0 } { -1 0 1 } { 0 -1 0 } }",
			"{ { 0 -1 1 } { 1 0 -1 } { -1 1 0 } }",
			{0, 2},
			{ConnectedTo(2), ConnectedTo(0)},
			FinReq(cluster::EquivQuiverMatrix("{ { 0 0 1 } { 0 0 0 } { -1 0 0 } }")),
			FinReq(cluster::EquivQuiverMatrix("{ { 0 0 0 } { 0 0 0 } { 0 0 0 } }")) ),
	make_move(/*Transpose*/
			"{ { 0 -1 0 } { 1 0 -1 } { 0 1 0 } }",
			"{ { 0 1 -1 } { -1 0 1 } { 1 -1 0 } }",
			{0, 2},
			{ConnectedTo(2), ConnectedTo(0)},
			FinReq(cluster::EquivQuiverMatrix("{ { 0 0 -1 } { 0 0 0 } { 1 0 0 } }")),
			FinReq(cluster::EquivQuiverMatrix("{ { 0 0 0 } { 0 0 0 } { 0 0 0 } }")) ),
	/* 29 & 30 */
	make_move("{ { 0 1 -1 0 0 } { -1 0 1 0 -1 } { 1 -1 0 1 0 } { 0 0 -1 0 1 } { 0 1 0 -1 0 } }",
			"{ { 0 -1 0 0 0 } { 1 0 -1 0 1 } { 0 1 0 1 -1 } { 0 0 -1 0 1 } { 0 -1 1 -1 0 } }",
			{2, 4},
			{Unconnected(), Unconnected()},
			FinReq(cluster::EquivQuiverMatrix("{ { 0 0 0 0 0 } { 0 0 -1 0 0 } { 0 1 0 1 0 } { 0 0 -1 0 1 } { 0 0 0 -1 0 } }")) ),
	make_move(/*Transpose*/
			"{ { 0 -1 1 0 0 } { 1 0 -1 0 1 } { -1 1 0 -1 0 } { 0 0 1 0 -1 } { 0 -1 0 1 0 } }",
			"{ { 0 1 0 0 0 } { -1 0 1 0 -1 } { 0 -1 0 -1 1 } { 0 0 1 0 -1 } { 0 1 -1 1 0 } }",
			{2, 4},
			{Unconnected(), Unconnected()},
			FinReq(cluster::EquivQuiverMatrix("{ { 0 0 0 0 0 } { 0 0 1 0 0 } { 0 -1 0 -1 0 } { 0 0 1 0 -1 } { 0 0 0 1 0 } }")) ),
	make_move("{ { 0 -1 0 0 0 0 0 0 } { 1 0 -1 0 0 0 0 1 } { 0 1 0 1 0 0 0 -1 } { 0 0 -1 0 1 0 0 0 } { 0 0 0 -1 0 1 0 0 } { 0 0 0 0 -1 0 1 0 } { 0 0 0 0 0 -1 0 0 } { 0 -1 1 0 0 0 0 0 } }",
			"{ { 0 1 -1 0 0 0 0 0 } { -1 0 1 0 0 0 0 -1 } { 1 -1 0 1 0 0 0 0 } { 0 0 -1 0 1 0 0 0 } { 0 0 0 -1 0 1 0 0 } { 0 0 0 0 -1 0 1 0 } { 0 0 0 0 0 -1 0 0 } { 0 1 0 0 0 0 0 0 } }",
			{0, 5, 7},
			{Line(), LineTo(7), LineTo(5)},
			FinReq(cluster::EquivQuiverMatrix("{ { 0 0 -1 0 0 0 0 0 } { 0 0 0 0 0 0 0 0 } { 1 0 0 1 0 0 0 0 } { 0 0 -1 0 1 0 0 0 } { 0 0 0 -1 0 1 0 0 } { 0 0 0 0 -1 0 1 0 } { 0 0 0 0 0 -1 0 0 } { 0 0 0 0 0 0 0 0 } }")) ),
	make_move("{ { 0 1 0 0 0 0 0 0 } { -1 0 1 0 0 0 0 -1 } { 0 -1 0 -1 0 0 0 1 } { 0 0 1 0 -1 0 0 0 } { 0 0 0 1 0 -1 0 0 } { 0 0 0 0 1 0 -1 0 } { 0 0 0 0 0 1 0 0 } { 0 1 -1 0 0 0 0 0 } }",
			"{ { 0 -1 1 0 0 0 0 0 } { 1 0 -1 0 0 0 0 1 } { -1 1 0 -1 0 0 0 0 } { 0 0 1 0 -1 0 0 0 } { 0 0 0 1 0 -1 0 0 } { 0 0 0 0 1 0 -1 0 } { 0 0 0 0 0 1 0 0 } { 0 -1 0 0 0 0 0 0 } }",
			{0, 5, 7},
			{Line(), LineTo(7), LineTo(5)},
			FinReq(cluster::EquivQuiverMatrix("{ { 0 0 1 0 0 0 0 0 } { 0 0 0 0 0 0 0 0 } { -1 0 0 -1 0 0 0 0 } { 0 0 1 0 -1 0 0 0 } { 0 0 0 1 0 -1 0 0 } { 0 0 0 0 1 0 -1 0 } { 0 0 0 0 0 1 0 0 } { 0 0 0 0 0 0 0 0 } }")) ),
	make_move("{ { 0 -1 0 0 0 0 0 } { 1 0 -1 0 0 1 0 } { 0 1 0 1 0 -1 0 } { 0 0 -1 0 1 0 0 } { 0 0 0 -1 0 1 0 } { 0 -1 1 0 -1 0 1 } { 0 0 0 0 0 -1 0 } }",
			"{ { 0 1 -1 0 0 0 0 } { -1 0 1 0 0 -1 0 } { 1 -1 0 1 0 0 0 } { 0 0 -1 0 1 0 0 } { 0 0 0 -1 0 1 0 } { 0 1 0 0 -1 0 1 } { 0 0 0 0 0 -1 0 } }",
			{ 0 }, {Line()}),
	make_move("{ { 0 -1 0 0 0 0 0 0 } { 1 0 1 -1 0 0 0 0 } { 0 -1 0 1 0 0 -1 0 } { 0 1 -1 0 1 0 0 0 } { 0 0 0 -1 0 1 0 0 } { 0 0 0 0 -1 0 1 0 } { 0 0 1 0 0 -1 0 1 } { 0 0 0 0 0 0 -1 0 } }",
			"{ { 0 1 0 -1 0 0 0 0 } { -1 0 -1 1 0 0 0 0 } { 0 1 0 0 0 0 -1 0 } { 1 -1 0 0 1 0 0 0 } { 0 0 0 -1 0 1 0 0 } { 0 0 0 0 -1 0 1 0 } { 0 0 1 0 0 -1 0 1 } { 0 0 0 0 0 0 -1 0 } }",
			{ 0 }, { Line() } ),
	make_move("{ { 0 1 0 0 0 0 0 0 } { -1 0 -1 1 0 0 0 0 } { 0 1 0 -1 0 0 1 0 } { 0 -1 1 0 -1 0 0 0 } { 0 0 0 1 0 -1 0 0 } { 0 0 0 0 1 0 -1 0 } { 0 0 -1 0 0 1 0 -1 } { 0 0 0 0 0 0 1 0 } }",
			"{ { 0 -1 0 1 0 0 0 0 } { 1 0 1 -1 0 0 0 0 } { 0 -1 0 0 0 0 1 0 } { -1 1 0 0 -1 0 0 0 } { 0 0 0 1 0 -1 0 0 } { 0 0 0 0 1 0 -1 0 } { 0 0 -1 0 0 1 0 -1 } { 0 0 0 0 0 0 1 0 } }",
			{ 0 }, { Line() } ),
	/* 32 */
	make_move("{ { 0 1 0 0 0 0 } { -1 0 1 0 0 0 } { 0 -1 0 -1 0 1 } { 0 0 1 0 1 -1 } { 0 0 0 -1 0 0 } { 0 0 -1 1 0 0 } }",
		"{ { 0 -1 1 0 0 0 } { 1 0 -1 0 0 0 } { -1 1 0 1 0 0 } { 0 0 -1 0 -1 1 } { 0 0 0 1 0 -1 } { 0 0 0 -1 1 0 } }",
		{0,5,4},{ConnectedTo(5),ConnectedTo(0),Line()},
		FinReq(cluster::EquivQuiverMatrix("{ { 0 -1 1 0 0 0 } { 1 0 -1 0 0 0 } { -1 1 0 0 0 0 } { 0 0 0 0 0 0 } { 0 0 0 0 0 -1 } { 0 0 0 0 1 0 } }")),
		FinReq(cluster::EquivQuiverMatrix("{ { 0 0 0 0 0 0 } { 0 0 0 0 0 0 } { 0 0 0 -1 0 1 } { 0 0 1 0 1 -1 } { 0 0 0 -1 0 0 } { 0 0 -1 1 0 0 } }")) ),
	make_move(/* Transpose */
		"{ { 0 -1 0 0 0 0 } { 1 0 -1 0 0 0 } { 0 1 0 1 0 -1 } { 0 0 -1 0 -1 1 } { 0 0 0 1 0 0 } { 0 0 1 -1 0 0 } }",
		"{ { 0 1 -1 0 0 0 } { -1 0 1 0 0 0 } { 1 -1 0 -1 0 0 } { 0 0 1 0 1 -1 } { 0 0 0 -1 0 1 } { 0 0 0 1 -1 0 } }",
		{0,5,4},{ConnectedTo(5),ConnectedTo(0),Line()},
		FinReq(cluster::EquivQuiverMatrix("{ { 0 1 -1 0 0 0 } { -1 0 1 0 0 0 } { 1 -1 0 0 0 0 } { 0 0 0 0 0 0 } { 0 0 0 0 0 1 } { 0 0 0 0 -1 0 } }")),
		FinReq(cluster::EquivQuiverMatrix("{ { 0 0 0 0 0 0 } { 0 0 0 0 0 0 } { 0 0 0 1 0 -1 } { 0 0 -1 0 -1 1 } { 0 0 0 1 0 0 } { 0 0 1 -1 0 0 } }")) ),
	/* 33 & 34 */
	make_move("{ { 0 1 0 -1 0 0 } { -1 0 1 1 0 0 } { 0 -1 0 0 0 0 } { 1 -1 0 0 1 -1 } { 0 0 0 -1 0 1 } { 0 0 0 1 -1 0 } }",
			"{ { 0 -1 1 0 0 0 } { 1 0 -1 -1 0 0 } { -1 1 0 0 0 0 } { 0 1 0 0 1 -1 } { 0 0 0 -1 0 1 } { 0 0 0 1 -1 0 } }",
			{0,5,2},{ConnectedTo(5),ConnectedTo(0),Line()},
			FinReq(cluster::EquivQuiverMatrix("{ { 0 0 1 0 0 0 } { 0 0 0 0 0 0 } { -1 0 0 0 0 0 } { 0 0 0 0 -1 0 } { 0 0 0 1 0 -1 } { 0 0 0 0 1 0 } }")) ),
	make_move(/*Transpose*/
			"{ { 0 -1 0 1 0 0 } { 1 0 -1 -1 0 0 } { 0 1 0 0 0 0 } { -1 1 0 0 -1 1 } { 0 0 0 1 0 -1 } { 0 0 0 -1 1 0 } }",
			"{ { 0 1 -1 0 0 0 } { -1 0 1 1 0 0 } { 1 -1 0 0 0 0 } { 0 -1 0 0 -1 1 } { 0 0 0 1 0 -1 } { 0 0 0 -1 1 0 } }",
			{0,5,2},{ConnectedTo(5),ConnectedTo(0),Line()},
			FinReq(cluster::EquivQuiverMatrix("{ { 0 0 -1 0 0 0 } { 0 0 0 0 0 0 } { 1 0 0 0 0 0 } { 0 0 0 0 1 0 } { 0 0 0 -1 0 1 } { 0 0 0 0 -1 0 } }")) ),
	/* 35 */
	make_move(
			"{ { 0 -1 0 0 0 } { 1 0 1 0 -1 } { 0 -1 0 -1 1 } { 0 0 1 0 0 } { 0 1 -1 0 0 } }",
			"{ { 0 1 -1 0 0 } { -1 0 1 0 0 } { 1 -1 0 1 -1 } { 0 0 -1 0 1 } { 0 0 1 -1 0 } }",
			{0, 4, 3}, {ConnectedTo(4), ConnectedTo(0), Line()},
			FinReq(cluster::EquivQuiverMatrix("{ { 0 1 0 0 0 } { -1 0 0 0 0 } { 0 0 0 0 0 } { 0 0 0 0 1 } { 0 0 0 -1 0 } }")),
			FinReq(cluster::EquivQuiverMatrix("{ { 0 0 0 0 0 } { 0 0 0 0 0 } { 0 0 0 1 0 } { 0 0 -1 0 1 } { 0 0 0 -1 0 } }")) ),
	make_move(/*Transpose*/
			"{ { 0 1 0 0 0 } { -1 0 -1 0 1 } { 0 1 0 1 -1 } { 0 0 -1 0 0 } { 0 -1 1 0 0 } }",
			"{ { 0 -1 1 0 0 } { 1 0 -1 0 0 } { -1 1 0 -1 1 } { 0 0 1 0 -1 } { 0 0 -1 1 0 } }",
			{0, 4, 3}, {ConnectedTo(4), ConnectedTo(0), Line()},
			FinReq(cluster::EquivQuiverMatrix("{ { 0 -1 0 0 0 } { 1 0 0 0 0 } { 0 0 0 0 0 } { 0 0 0 0 -1 } { 0 0 0 1 0 } }")),
			FinReq(cluster::EquivQuiverMatrix("{ { 0 0 0 0 0 } { 0 0 0 0 0 } { 0 0 0 -1 0 } { 0 0 1 0 -1 } { 0 0 0 1 0 } }")) ),
	/* 25 & 26 */
	make_move(
			"{ { 0 -1 0 0 0 0 1 0 } { 1 0 1 0 0 0 -1 0 } { 0 -1 0 1 0 0 0 0 } { 0 0 -1 0 -1 0 1 0 } { 0 0 0 1 0 1 -1 -1 } { 0 0 0 0 -1 0 1 0 } { -1 1 0 -1 1 -1 0 0 } { 0 0 0 0 1 0 0 0 } }",
			"{ { 0 -1 0 0 0 0 0 0 } { 1 0 -1 0 0 0 1 0 } { 0 1 0 1 0 0 -1 0 } { 0 0 -1 0 1 0 0 0 } { 0 0 0 -1 0 0 1 -1 } { 0 0 0 0 0 0 1 -1 } { 0 -1 1 0 -1 -1 0 1 } { 0 0 0 0 1 1 -1 0 } }",
			{0,7},{Line(), Line()},
			FinReq(cluster::EquivQuiverMatrix("{ { 0 -1 0 0 0 0 0 0 } { 1 0 -1 0 0 0 0 0 } { 0 1 0 1 0 0 0 0 } { 0 0 -1 0 1 0 0 0 } { 0 0 0 -1 0 0 0 -1 } { 0 0 0 0 0 0 0 0 } { 0 0 0 0 0 0 0 -1 } { 0 0 0 0 1 0 1 0 } }") ),
			FinReq(cluster::EquivQuiverMatrix("{ { 0 -1 0 0 0 0 0 0 } { 1 0 1 0 0 0 0 0 } { 0 -1 0 1 0 0 0 0 } { 0 0 -1 0 -1 0 0 0 } { 0 0 0 1 0 1 0 -1 } { 0 0 0 0 -1 0 0 0 } { 0 0 0 0 0 0 0 0 } { 0 0 0 0 1 0 0 0 } }") ) ),
	make_move(/* Transpose */
			"{ { 0 1 0 0 0 0 -1 0 } { -1 0 -1 0 0 0 1 0 } { 0 1 0 -1 0 0 0 0 } { 0 0 1 0 1 0 -1 0 } { 0 0 0 -1 0 -1 1 1 } { 0 0 0 0 1 0 -1 0 } { 1 -1 0 1 -1 1 0 0 } { 0 0 0 0 -1 0 0 0 } }",
			"{ { 0 1 0 0 0 0 0 0 } { -1 0 1 0 0 0 -1 0 } { 0 -1 0 -1 0 0 1 0 } { 0 0 1 0 -1 0 0 0 } { 0 0 0 1 0 0 -1 1 } { 0 0 0 0 0 0 -1 1 } { 0 1 -1 0 1 1 0 -1 } { 0 0 0 0 -1 -1 1 0 } }",
			{0,7},{Line(), Line()},
			FinReq(cluster::EquivQuiverMatrix("{ { 0 1 0 0 0 0 0 0 } { -1 0 1 0 0 0 0 0 } { 0 -1 0 -1 0 0 0 0 } { 0 0 1 0 -1 0 0 0 } { 0 0 0 1 0 0 0 1 } { 0 0 0 0 0 0 0 0 } { 0 0 0 0 0 0 0 1 } { 0 0 0 0 -1 0 -1 0 } }") ),
			FinReq(cluster::EquivQuiverMatrix("{ { 0 1 0 0 0 0 0 0 } { -1 0 -1 0 0 0 0 0 } { 0 1 0 -1 0 0 0 0 } { 0 0 1 0 1 0 0 0 } { 0 0 0 -1 0 -1 0 1 } { 0 0 0 0 1 0 0 0 } { 0 0 0 0 0 0 0 0 } { 0 0 0 0 -1 0 0 0 } }") ) ),
	make_move(
			"{ { 0 1 0 -1 0 1 } { -1 0 1 1 0 0 } { 0 -1 0 0 0 0 } { 1 -1 0 0 1 -1 } { 0 0 0 -1 0 1 } { -1 0 0 1 -1 0 } }",
			"{ { 0 -1 1 0 0 1 } { 1 0 -1 -1 0 0 } { -1 1 0 0 0 0 } { 0 1 0 0 1 -1 } { 0 0 0 -1 0 1 } { -1 0 0 1 -1 0 } }",
			{0,5,2,4},{ConnectedTo(5),ConnectedTo(0),Line(),Line()},
			FinReq(cluster::EquivQuiverMatrix("{ { 0 0 1 0 0 1 } { 0 0 0 0 0 0 } { -1 0 0 0 0 0 } { 0 0 0 0 1 -1 } { 0 0 0 -1 0 1 } { -1 0 0 1 -1 0 } }")) ),
	make_move(/*Transpose*/
			"{ { 0 -1 0 1 0 -1 } { 1 0 -1 -1 0 0 } { 0 1 0 0 0 0 } { -1 1 0 0 -1 1 } { 0 0 0 1 0 -1 } { 1 0 0 -1 1 0 } }",
			"{ { 0 1 -1 0 0 -1 } { -1 0 1 1 0 0 } { 1 -1 0 0 0 0 } { 0 -1 0 0 -1 1 } { 0 0 0 1 0 -1 } { 1 0 0 -1 1 0 } }",
			{0,5,2,4},{ConnectedTo(5),ConnectedTo(0),Line(),Line()},
			FinReq(cluster::EquivQuiverMatrix("{ { 0 0 -1 0 0 -1 } { 0 0 0 0 0 0 } { 1 0 0 0 0 0 } { 0 0 0 0 -1 1 } { 0 0 0 1 0 -1 } { 1 0 0 -1 1 0 } }")) ),
				/* 10x10 special */
	make_move(
			"{ { 0 -1 0 0 0 0 0 0 0 0 } { 1 0 -1 0 0 0 0 0 0 0 } { 0 1 0 -1 0 0 0 0 0 1 } { 0 0 1 0 1 0 0 0 0 -1 } { 0 0 0 -1 0 1 0 0 0 0 } { 0 0 0 0 -1 0 -1 0 1 0 } { 0 0 0 0 0 1 0 1 -1 0 } { 0 0 0 0 0 0 -1 0 1 0 } { 0 0 0 0 0 -1 1 -1 0 1 } { 0 0 -1 1 0 0 0 0 -1 0 } }",
			"{ { 0 -1 0 0 0 0 0 0 0 0 } { 1 0 -1 0 0 0 0 0 0 1 } { 0 1 0 1 0 0 0 0 0 -1 } { 0 0 -1 0 1 0 0 0 0 0 } { 0 0 0 -1 0 -1 0 0 1 0 } { 0 0 0 0 1 0 -1 1 -1 0 } { 0 0 0 0 0 1 0 0 0 0 } { 0 0 0 0 0 -1 0 0 1 0 } { 0 0 0 0 -1 1 0 -1 0 1 } { 0 -1 1 0 0 0 0 0 -1 0 } }",
			{},{}),
	make_move(/*Transpose*/
			"{ { 0 1 0 0 0 0 0 0 0 0 } { -1 0 1 0 0 0 0 0 0 0 } { 0 -1 0 1 0 0 0 0 0 -1 } { 0 0 -1 0 -1 0 0 0 0 1 } { 0 0 0 1 0 -1 0 0 0 0 } { 0 0 0 0 1 0 1 0 -1 0 } { 0 0 0 0 0 -1 0 -1 1 0 } { 0 0 0 0 0 0 1 0 -1 0 } { 0 0 0 0 0 1 -1 1 0 -1 } { 0 0 1 -1 0 0 0 0 1 0 } }",
			"{ { 0 1 0 0 0 0 0 0 0 0 } { -1 0 1 0 0 0 0 0 0 -1 } { 0 -1 0 -1 0 0 0 0 0 1 } { 0 0 1 0 -1 0 0 0 0 0 } { 0 0 0 1 0 1 0 0 -1 0 } { 0 0 0 0 -1 0 1 -1 1 0 } { 0 0 0 0 0 -1 0 0 0 0 } { 0 0 0 0 0 1 0 0 -1 0 } { 0 0 0 0 1 -1 0 1 0 -1 } { 0 1 -1 0 0 0 0 0 1 0 } }",
			{},{}),
	make_move(
			"{ { 0 0 0 -1 1 0 } { 0 0 0 1 0 -1 } { 0 0 0 -1 0 0 } { 1 -1 1 0 -1 1 } { -1 0 0 1 0 0 } { 0 1 0 -1 0 0 } }",
			"{ { 0 -1 0 1 0 0 } { 1 0 1 -1 0 0 } { 0 -1 0 1 -1 0 } { -1 1 -1 0 1 -1 } { 0 0 1 -1 0 1 } { 0 0 0 1 -1 0 } }",
			{ 0, 4 }, { Line(), Line() },
			FinReq(cluster::EquivQuiverMatrix("{ { 0 -1 0 0 0 0 } { 1 0 1 0 0 0 } { 0 -1 0 0 -1 0 } { 0 0 0 0 0 0 } { 0 0 1 0 0 1 } { 0 0 0 0 -1 0 } }")) ),
	make_move(/*Transpose*/
			"{ { 0 0 0 1 -1 0 } { 0 0 0 -1 0 1 } { 0 0 0 1 0 0 } { -1 1 -1 0 1 -1 } { 1 0 0 -1 0 0 } { 0 -1 0 1 0 0 } }",
			"{ { 0 1 0 -1 0 0 } { -1 0 -1 1 0 0 } { 0 1 0 -1 1 0 } { 1 -1 1 0 -1 1 } { 0 0 -1 1 0 -1 } { 0 0 0 -1 1 0 } }",
			{ 0, 4 }, { Line(), Line() },
			FinReq(cluster::EquivQuiverMatrix("{ { 0 1 0 0 0 0 } { -1 0 -1 0 0 0 } { 0 1 0 0 1 0 } { 0 0 0 0 0 0 } { 0 0 -1 0 0 -1 } { 0 0 0 0 1 0 } }")) ),
	make_move(
			"{ { 0 -1 0 1 0 0 } { 1 0 1 -1 0 0 } { 0 -1 0 1 -1 0 } { -1 1 -1 0 1 -1 } { 0 0 1 -1 0 1 } { 0 0 0 1 -1 0 } }",
			"{ { 0 -1 0 1 0 0 } { 1 0 -1 0 0 0 } { 0 1 0 -1 1 0 } { -1 0 1 0 0 -1 } { 0 0 -1 0 0 1 } { 0 0 0 1 -1 0 } }",
			{ 0, 4 }, {Line(), Line()},
			FinReq(cluster::EquivQuiverMatrix("{ { 0 -1 0 1 0 0 } { 1 0 0 0 0 0 } { 0 0 0 0 0 0 } { -1 0 0 0 0 -1 } { 0 0 0 0 0 1 } { 0 0 0 1 -1 0 } }")),
			FinReq(cluster::EquivQuiverMatrix("{ { 0 -1 0 1 0 0 } { 1 0 0 -1 0 0 } { 0 0 0 0 0 0 } { -1 1 0 0 1 -1 } { 0 0 0 -1 0 1 } { 0 0 0 1 -1 0 } }")) ),
	make_move(/*Transpose*/
			"{ { 0 1 0 -1 0 0 } { -1 0 -1 1 0 0 } { 0 1 0 -1 1 0 } { 1 -1 1 0 -1 1 } { 0 0 -1 1 0 -1 } { 0 0 0 -1 1 0 } }",
			"{ { 0 1 0 -1 0 0 } { -1 0 1 0 0 0 } { 0 -1 0 1 -1 0 } { 1 0 -1 0 0 1 } { 0 0 1 0 0 -1 } { 0 0 0 -1 1 0 } }",
			{ 0, 4 }, {Line(), Line()},
			FinReq(cluster::EquivQuiverMatrix("{ { 0 1 0 -1 0 0 } { -1 0 0 0 0 0 } { 0 0 0 0 0 0 } { 1 0 0 0 0 1 } { 0 0 0 0 0 -1 } { 0 0 0 -1 1 0 } }")),
			FinReq(cluster::EquivQuiverMatrix("{ { 0 1 0 -1 0 0 } { -1 0 0 1 0 0 } { 0 0 0 0 0 0 } { 1 -1 0 0 -1 1 } { 0 0 0 1 0 -1 } { 0 0 0 -1 1 0 } }")) ),
	make_move(
			"{ { 0 1 -1 0 0 0 0 } { -1 0 1 -1 0 1 0 } { 1 -1 0 1 0 0 0 } { 0 1 -1 0 1 -1 0 } { 0 0 0 -1 0 1 0 } { 0 -1 0 1 -1 0 -1 } { 0 0 0 0 0 1 0 } }",
			"{ { 0 -1 0 0 0 1 0 } { 1 0 -1 1 0 -1 0 } { 0 1 0 0 0 0 0 } { 0 -1 0 0 1 0 0 } { 0 0 0 -1 0 1 0 } { -1 1 0 0 -1 0 -1 } { 0 0 0 0 0 1 0 } }",
			{0},{Line()}),
	make_move(/*Tranpose*/
			"{ { 0 -1 1 0 0 0 0 } { 1 0 -1 1 0 -1 0 } { -1 1 0 -1 0 0 0 } { 0 -1 1 0 -1 1 0 } { 0 0 0 1 0 -1 0 } { 0 1 0 -1 1 0 1 } { 0 0 0 0 0 -1 0 } }",
			"{ { 0 1 0 0 0 -1 0 } { -1 0 1 -1 0 1 0 } { 0 -1 0 0 0 0 0 } { 0 1 0 0 -1 0 0 } { 0 0 0 1 0 -1 0 } { 1 -1 0 0 1 0 1 } { 0 0 0 0 0 -1 0 } }",
			{0},{Line()}),
	make_move(
			"{ { 0 -1 0 0 0 1 0 } { 1 0 -1 1 0 -1 0 } { 0 1 0 0 0 0 0 } { 0 -1 0 0 1 0 0 } { 0 0 0 -1 0 1 0 } { -1 1 0 0 -1 0 -1 } { 0 0 0 0 0 1 0 } }",
			"{ { 0 0 0 0 0 -1 0 } { 0 0 -1 1 -1 1 -1 } { 0 1 0 0 0 0 0 } { 0 -1 0 0 1 0 0 } { 0 1 0 -1 0 -1 0 } { 1 -1 0 0 1 0 1 } { 0 1 0 0 0 -1 0 } }",
			{0},{Line()}),
	make_move(/*Transpose*/
			"{ { 0 1 0 0 0 -1 0 } { -1 0 1 -1 0 1 0 } { 0 -1 0 0 0 0 0 } { 0 1 0 0 -1 0 0 } { 0 0 0 1 0 -1 0 } { 1 -1 0 0 1 0 1 } { 0 0 0 0 0 -1 0 } }",
			"{ { 0 0 0 0 0 1 0 } { 0 0 1 -1 1 -1 1 } { 0 -1 0 0 0 0 0 } { 0 1 0 0 -1 0 0 } { 0 -1 0 1 0 1 0 } { -1 1 0 0 -1 0 -1 } { 0 -1 0 0 0 1 0 } }",
			{0},{Line()}),
	make_move(
			"{ { 0 -1 0 1 0 0 0 0 -1 0 } { 1 0 1 -1 0 0 0 0 0 0 } { 0 -1 0 1 -1 0 0 0 0 0 } { -1 1 -1 0 1 -1 0 0 0 0 } { 0 0 1 -1 0 0 1 0 0 0 } { 0 0 0 1 0 0 -1 0 0 0 } { 0 0 0 0 -1 1 0 1 0 0 } { 0 0 0 0 0 0 -1 0 0 1 } { 1 0 0 0 0 0 0 0 0 0 } { 0 0 0 0 0 0 0 -1 0 0 } }",
			"{ { 0 -1 0 1 0 0 0 0 -1 0 } { 1 0 1 -1 0 0 0 0 0 0 } { 0 -1 0 1 -1 0 0 0 0 0 } { -1 1 -1 0 1 -1 0 0 0 0 } { 0 0 1 -1 0 1 -1 1 0 0 } { 0 0 0 1 -1 0 1 0 0 0 } { 0 0 0 0 1 -1 0 -1 0 0 } { 0 0 0 0 -1 0 1 0 0 1 } { 1 0 0 0 0 0 0 0 0 0 } { 0 0 0 0 0 0 0 -1 0 0 } }",
			{},{}),
	make_move(/*Transpose*/
			"{ { 0 1 0 -1 0 0 0 0 1 0 } { -1 0 -1 1 0 0 0 0 0 0 } { 0 1 0 -1 1 0 0 0 0 0 } { 1 -1 1 0 -1 1 0 0 0 0 } { 0 0 -1 1 0 0 -1 0 0 0 } { 0 0 0 -1 0 0 1 0 0 0 } { 0 0 0 0 1 -1 0 -1 0 0 } { 0 0 0 0 0 0 1 0 0 -1 } { -1 0 0 0 0 0 0 0 0 0 } { 0 0 0 0 0 0 0 1 0 0 } }",
			"{ { 0 1 0 -1 0 0 0 0 1 0 } { -1 0 -1 1 0 0 0 0 0 0 } { 0 1 0 -1 1 0 0 0 0 0 } { 1 -1 1 0 -1 1 0 0 0 0 } { 0 0 -1 1 0 -1 1 -1 0 0 } { 0 0 0 -1 1 0 -1 0 0 0 } { 0 0 0 0 -1 1 0 1 0 0 } { 0 0 0 0 1 0 -1 0 0 -1 } { -1 0 0 0 0 0 0 0 0 0 } { 0 0 0 0 0 0 0 1 0 0 } }",
			{},{}),
	make_move(
			"{ { 0 -1 0 1 0 0 0 0 -1 0 } { 1 0 1 -1 0 0 0 0 0 0 } { 0 -1 0 1 -1 0 0 0 0 0 } { -1 1 -1 0 1 -1 0 0 0 0 } { 0 0 1 -1 0 1 -1 1 0 0 } { 0 0 0 1 -1 0 1 0 0 0 } { 0 0 0 0 1 -1 0 -1 0 0 } { 0 0 0 0 -1 0 1 0 0 1 } { 1 0 0 0 0 0 0 0 0 0 } { 0 0 0 0 0 0 0 -1 0 0 } }",
			"{ { 0 -1 0 1 0 0 0 0 -1 0 } { 1 0 1 -1 0 0 0 0 0 0 } { 0 -1 0 0 1 0 -1 0 0 0 } { -1 1 0 0 -1 0 0 1 0 0 } { 0 0 -1 1 0 -1 1 -1 0 0 } { 0 0 0 0 1 0 0 0 0 0 } { 0 0 1 0 -1 0 0 0 0 0 } { 0 0 0 -1 1 0 0 0 0 1 } { 1 0 0 0 0 0 0 0 0 0 } { 0 0 0 0 0 0 0 -1 0 0 } }",
			{},{}),
};
}
}

