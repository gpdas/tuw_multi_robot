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

#ifndef PLANNER_H
#define PLANNER_H

#include <vector>
#include <memory>
#include <opencv/cv.h>
#include <tuw_global_planner/point_expander.h>
#include <tuw_global_planner/multi_robot_router.h>

class Planner
{
    public:
        Planner(uint32_t _nr_robots);
        Planner();
        void updateRobotPose(uint32_t _robot_id, const Eigen::Vector2d &_pose);

        /**
        * @brief resizes the planner to a different nr of _nr_robots
        */
        void resize(uint32_t _nr_robots);
        /**
        * @brief updates the robot start positin (normally called from odom r[_robot_id]
        */
        /**
        * @brief generates the plan from (Vertex[odom robotPose] to Vertex[_goals]
        * @param _radius a vector of the robots radius'
        * @param _map the grid_map used to find the start and goal segments of the path
        * @param _graph the full graph of the map used for planning the path
        */
        bool makePlan(const std::vector< Eigen::Vector2d > &_goals, const std::vector<float> &_radius, const cv::Mat &_map, const float &_resolution, const Eigen::Vector2d &_origin, const std::vector<Segment> &_graph);


        const std::vector<Checkpoint> &getRoute(const uint32_t _robot);
        void postprocessRoutingTable();

        uint32_t getDuration_ms();
        float getOverallPathLength();
        float getLongestPathLength();
        uint32_t getPriorityScheduleAttemps();
        uint32_t getSpeedScheduleAttemps();


        std::shared_ptr<float> potential_;

    private:
        enum class goalMode
        {
            use_segment_goal,
            use_voronoi_goal,
            use_map_goal
        };
        enum class graphType
        {
            voronoi,
            random
        };
        bool calculateStartPoints(const std::vector<float> &_radius, const cv::Mat &_map, const float &resolution, const Eigen::Vector2d &origin, const std::vector<Segment> &_graph);
        //Calculate a segment
        int32_t getSegment(const std::vector<Segment> &_graph, const Eigen::Vector2d &_pose);
        //Helper dist calculation
        float distanceToSegment(const Segment &_s, const Eigen::Vector2d &_p);
        //Checks if _seg is a leave of the graph and uses the closes neighbor as segment if the width of the leave is to small
        bool resolveSegment(const std::vector< Segment > &_graph, const uint32_t &_segId, const Eigen::Vector2d &_originPoint, const float &_radius, uint32_t &_foundSeg);

        void getTrimedGraph(const std::vector< Segment > &_graph, std::vector< Segment > &_trimmedGraph, const uint32_t minDiameter);

        uint32_t robot_nr_;
        std::vector<bool> pose_received_;
        std::vector<Eigen::Vector2d> robot_poses_;
        std::vector<Eigen::Vector2d> goals_;
        std::vector<Eigen::Vector2d> realGoals_;
        std::vector<Eigen::Vector2d> realStart_;
        std::vector<Eigen::Vector2d> voronoiGoals_;
        std::vector<Eigen::Vector2d> voronoiStart_;
        std::vector<uint32_t> startSegments_;
        std::vector<uint32_t> goalSegments_;
        std::vector<uint32_t>  diameter_;

        std::unique_ptr<PointExpander> pointExpander_;
        std::unique_ptr<MultiRobotRouter> multiRobotRouter_;
        std::vector<std::vector<Checkpoint>> routingTable_;
        float overallPathLength_;
        float longestPatLength_;
        uint32_t duration_;

    protected:
        graphType graphMode_ = graphType::voronoi;
        goalMode goalMode_ = goalMode::use_voronoi_goal;
        uint32_t optimizationSegmentNr_;
};

#endif // PLANNER_H