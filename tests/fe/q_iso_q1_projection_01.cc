// ---------------------------------------------------------------------
//
// Copyright (C) 2003 - 2017 by the deal.II authors
//
// This file is part of the deal.II library.
//
// The deal.II library is free software; you can use it, redistribute
// it, and/or modify it under the terms of the GNU Lesser General
// Public License as published by the Free Software Foundation; either
// version 2.1 of the License, or (at your option) any later version.
// The full text of the license can be found in the file LICENSE at
// the top level of the deal.II distribution.
//
// ---------------------------------------------------------------------


/*
 * Project the function [1,1] onto a deformed grid and see whether the
 * FESystem elements can represent it exactly. This shouldn't be a surprise,
 * but it is nice to compare with the RT and ABF elements
 */



char logname[] = "output";
#include <deal.II/fe/fe_q_iso_q1.h>

#include "deformed_projection.h"

void
test()
{
  FESystem<2>                       fe(FE_Q_iso_Q1<2>(3), 2);
  const std::array<unsigned int, 3> min_convergence_steps = {{13, 13, 13}};
  check(fe, min_convergence_steps);
}
