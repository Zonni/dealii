//----------------------------  geometry_info.h  ---------------------------
//    Version: $Name$
//    This file is subject to QPL and may not be  distributed
//    without copyright and license information. Please refer
//    to the file deal.II/doc/license.html for the  text  and
//    further information on this license.
//
//----------------------------  geometry_info.h  ---------------------------
#ifndef __deal2__geometry_info_h
#define __deal2__geometry_info_h


#include <base/exceptions.h>



/**
 * Topological description of cells.
 *
 * This class contains as static members information on vertices and
 * faces of a @p{dim}-dimensional grid cell. The interface is the same
 * for all dimensions. If a value is of no use in a low dimensional
 * cell, it is (correctly) set to zero, e.g. @p{subfaces_per_cell} in
 * 1d.
 *
 * This information should always replace hard-coded numbers of
 * vertices, neighbors and so on, since it can be used dimension
 * independent.
 *
 * @author Wolfgang Bangerth, Guido Kanschat, 1998, 1999
 */
template <int dim_>
struct GeometryInfo
{
				     /**
				      * Present dimension. Does not
				      * look useful, but might be.
				      */
    static const unsigned int dim               = dim_;

				     /**
				      * Number of children a cell has.
				      */
    static const unsigned int children_per_cell = (1<<dim_);

				     /**
				      * Number of faces a cell has.
				      */
    static const unsigned int faces_per_cell    = 2*dim_;

				     /**
				      * Number of children each face has
				      * when the adjacent cell is refined.
				      */
    static const unsigned int subfaces_per_face = GeometryInfo<dim_-1>::children_per_cell;

				     /**
				      * Number of vertices a cell has.
				      */
    static const unsigned int vertices_per_cell = (1<<dim_);

				     /**
				      * Number of vertices each face has.
				      * Since this is not useful in one
				      * dimension, we provide a useless
				      * number (in the hope that a compiler
				      * may warn when it sees constructs like
				      * @p{for (i=0; i<vertices_per_face; ++i)},
				      * at least if @p{i} is an @p{unsigned int}.
				      */
    static const unsigned int vertices_per_face = GeometryInfo<dim_-1>::vertices_per_cell;

				     /**
				      * Number of lines each face has.
				      */
    static const unsigned int lines_per_face = GeometryInfo<dim_-1>::lines_per_cell;
    
				     /**
				      * Number of quads on each face.
				      */
    static const unsigned int quads_per_face = GeometryInfo<dim_-1>::quads_per_cell;

				     /**
				      * Number of lines of a cell.
				      *
				      * Computation of this value
				      * follows the idea, that
				      * building a hypercube of
				      * dimension @p{dim} from one of
				      * dimension @p{dim}-1 can be done
				      * in the following two steps:
				      *
				      * 1. Duplicated it in the new coordinate direction.
				      *
				      * 2. Connect all corresponding
				      * vertices of the original
				      * hypercube and the copy by
				      * lines.
				      */
    static const unsigned int lines_per_cell = (2*GeometryInfo<dim_-1>::lines_per_cell
						+ GeometryInfo<dim_-1>::vertices_per_cell);
    
				     /**
				      * Number of quadrilaterals of a cell.
				      *
				      * Computation is analogous to @p{lines_per_cell}.
				      */
    static const unsigned int quads_per_cell = (2*GeometryInfo<dim_-1>::quads_per_cell
						+ GeometryInfo<dim_-1>::lines_per_cell);

				     /**
				      * Number of hexahedra of a cell.
				      *
				      * Computation is analogous to
				      * @p{lines_per_cell}. Very
				      * important for more than three
				      * dimensions!
				      */
    static const unsigned int hexes_per_cell = (2*GeometryInfo<dim_-1>::hexes_per_cell
						+ GeometryInfo<dim_-1>::quads_per_cell);

				     /**
				      * List of numbers which is
				      * denote which face is opposite
				      * to a given face. In 1d, this
				      * list is @p{{1,0}}, in 2d @p{{2, 3, 0, 1}},
				      * in 3d @p{{1, 0, 4, 5, 2, 3}}.
				      */
    static const unsigned int opposite_face[faces_per_cell];
    
				     /**
				      * This field store which child cells
				      * are adjacent to a certain face of
				      * the mother cell.
				      *
				      * For example, in 2D the layout of
				      * a cell is as follows:
				      * @begin{verbatim}
				      * .      2
				      * .   3-->--2
				      * .   |     |
				      * . 3 ^     ^ 1
				      * .   |     |
				      * .   0-->--1
				      * .      0
				      * @end{verbatim}
				      * Vertices and faces are indicated
				      * with their numbers, faces also with
				      * their directions.
				      *
				      * Now, when refined, the layout is
				      * like this:
				      * @begin{verbatim}
				      * *--*--*
				      * | 3|2 |
				      * *--*--*
				      * | 0|1 |
				      * *--*--*
				      * @end{verbatim}
				      *
				      * Thus, the child cells on face zero
				      * are (ordered in the direction of the
				      * face) 0 and 1, on face 2 they are
				      * 3 and 2, etc.
				      *
				      * For three spatial dimensions,
				      * the exact order of the children is
				      * laid down in the documentation of
				      * the @ref{Triangulation} class.
				      */
    static unsigned int child_cell_on_face (unsigned int face,
					    unsigned int subface);
};



/**
 * Pseudo-class for recursive functions in
 * @ref{GeometryInfo<1>}. Actually this class is a starting point for
 * the induction ladder by which the higher @p{GeometryInfo} classes
 * are built.
 *
 * @author Wolfgang Bangerth, Guido Kanschat, 1998, 1999
 */
template <>
struct GeometryInfo<0>
{
    static const unsigned int vertices_per_cell = 1;
    static const unsigned int lines_per_cell = 0;
    static const unsigned int quads_per_cell = 0;
    static const unsigned int hexes_per_cell = 0;
    static const unsigned int children_per_cell = 0;
};




/*---------------------------- Inline functions --------------------------------*/

template<>
inline unsigned int
GeometryInfo<2>::child_cell_on_face (const unsigned int face,
                                     const unsigned int subface)
{
  Assert (face<faces_per_cell, ExcIndexRange(face, 0, faces_per_cell));
  Assert (subface<subfaces_per_face, ExcIndexRange(subface, 0, subfaces_per_face));
  
  static const unsigned subcells[faces_per_cell][subfaces_per_face] = {{0,1},
								       {1,2},
								       {3,2},
								       {0,3}};
  return subcells[face][subface];
};


template<>
inline
unsigned int GeometryInfo<3>::child_cell_on_face (const unsigned int face,
						  const unsigned int subface)
{
  Assert (face<faces_per_cell, ExcIndexRange(face, 0, faces_per_cell));
  Assert (subface<subfaces_per_face, ExcIndexRange(subface, 0, subfaces_per_face));
  
  static const unsigned subcells[faces_per_cell][subfaces_per_face] = {{0, 1, 2, 3},
								       {4, 5, 6, 7},
								       {0, 1, 5, 4},
								       {1, 5, 6, 2},
								       {3, 2, 6, 7},
								       {0, 4, 7, 3}};
  return subcells[face][subface];
};


#endif
