/*
 * Copyright (c) 2017, <copyright holder> <email>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *     * Neither the name of the <organization> nor the
 *     names of its contributors may be used to endorse or promote products
 *     derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY <copyright holder> <email> ''AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL <copyright holder> <email> BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

#ifndef DXF_TO_GRAPH_H
#define DXF_TO_GRAPH_H

#include <string>
#include <tuw_serialization/serializer.h>
#include <dxflib/dl_dxf.h>

namespace tuw_graph
{
    struct Line
    {
        Eigen::Vector2d start;
        Eigen::Vector2d end;
        Line(Eigen::Vector2d _start, Eigen::Vector2d _end) : start(_start), end(_end) {}
        Line() {}
    };
    
    class DxfToGraph
    {
        public:            
            void parseGraph(const std::string &_dxfPath, const float _segLength, const float _segWidth);
            void serializeGraph(const std::string &_graphPath);
            //void preprocessCircles();
            //void preprocessArcs();
        private:
            std::vector<Line> splitLine(const DL_LineData &_line, const float _segLength);
            std::vector<Segment> generateGraph(const std::vector<Eigen::Vector2d> &_lines, const float _segWidth);            
    };
}
#endif // PLANNER_NODE_H
