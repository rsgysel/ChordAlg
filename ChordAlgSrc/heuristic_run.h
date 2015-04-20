/*
 *  heuristic_run.h - class that runs a heuristic experiment
 *  Copyright (C) 2013 Rob Gysel
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef INCLUDE_HEURISTIC_RUNS_H_
#define INCLUDE_HEURISTIC_RUNS_H_

#include <cfloat>
#include <cstdlib>
#include <ctime>

#include <string>
#include <vector>
#include <utility>

#include "ChordAlgSrc/atoms.h"
#include "ChordAlgSrc/elimination_algorithm.h"
#include "ChordAlgSrc/elimination_order.h"
#include "ChordAlgSrc/file_reader.h"
#include "ChordAlgSrc/intersection_graph.h"
#include "ChordAlgSrc/graph.h"
#include "ChordAlgSrc/vertices.h"

void SetupAndRunHeuristic(std::string filename,
                          std::vector< chordalg::EliminationCriterion > criterion,
                          std::vector< chordalg::EliminationMode > modes,
                          bool atoms=false,
                          size_t runs=1,
                          float deficiency_wt=0,
                          float separation_wt=0);

class HeuristicRun {
 public:
    HeuristicRun() = delete;
    HeuristicRun(const HeuristicRun&) = delete;
    void operator=(const HeuristicRun&) = delete;

    HeuristicRun(const chordalg::Graph*, 
                 const std::vector< chordalg::EliminationParameters* >*,
                 bool,
                 size_t);
    virtual ~HeuristicRun() {}

    const std::vector< chordalg::VertexPair >& fill_edges() const;

    std::string Run();

 protected:
    const chordalg::Graph* const G_;
    const std::vector< chordalg::EliminationParameters* >* elimination_parameters_;
    bool atoms_;
    size_t runs_;

    std::vector< chordalg::VertexPair > fill_edges_;
};  // class HeuristicRun

#endif  // INCLUDE_HEURISTIC_RUNS_H_