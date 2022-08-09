/**
 * @file trajopt_motion_planner_task.h
 * @brief TrajOpt motion planning pipeline
 *
 * @author Levi Armstrong
 * @date July 29. 2022
 * @version TODO
 * @bug No known bugs
 *
 * @copyright Copyright (c) 2022, Levi Armstrong
 *
 * @par License
 * Software License Agreement (Apache License)
 * @par
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * http://www.apache.org/licenses/LICENSE-2.0
 * @par
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#ifndef TESSERACT_TASK_COMPOSER_TRAJOPT_MOTION_PIPELINE_TASK_H
#define TESSERACT_TASK_COMPOSER_TRAJOPT_MOTION_PIPELINE_TASK_H

#include <tesseract_task_composer/task_composer_graph.h>

namespace tesseract_planning
{
class TrajOptMotionPipelineTask : public TaskComposerGraph
{
public:
  using Ptr = std::shared_ptr<TrajOptMotionPipelineTask>;
  using ConstPtr = std::shared_ptr<const TrajOptMotionPipelineTask>;
  using UPtr = std::unique_ptr<TrajOptMotionPipelineTask>;
  using ConstUPtr = std::unique_ptr<const TrajOptMotionPipelineTask>;

  TrajOptMotionPipelineTask() = default;  // Required for serialization
  TrajOptMotionPipelineTask(std::string input_key,
                            std::string output_key,
                            std::string name = "TrajOptMotionPipelineTask");
  TrajOptMotionPipelineTask(std::string input_key,
                            std::string output_key,
                            bool check_input,
                            bool post_collision_check,
                            bool post_smoothing,
                            std::string name = "TrajOptMotionPipelineTask");
  virtual ~TrajOptMotionPipelineTask() = default;
  TrajOptMotionPipelineTask(const TrajOptMotionPipelineTask&) = delete;
  TrajOptMotionPipelineTask& operator=(const TrajOptMotionPipelineTask&) = delete;
  TrajOptMotionPipelineTask(TrajOptMotionPipelineTask&&) = delete;
  TrajOptMotionPipelineTask& operator=(TrajOptMotionPipelineTask&&) = delete;

  bool operator==(const TrajOptMotionPipelineTask& rhs) const;
  bool operator!=(const TrajOptMotionPipelineTask& rhs) const;

protected:
  friend class tesseract_common::Serialization;
  friend class boost::serialization::access;

  template <class Archive>
  void serialize(Archive& ar, const unsigned int version);  // NOLINT

  void ctor();

  std::string input_key_;
  std::string output_key_;
  bool check_input_{ true };
  bool post_collision_check_{ true };
  bool post_smoothing_{ false };
};
}  // namespace tesseract_planning

#include <boost/serialization/export.hpp>
#include <boost/serialization/tracking.hpp>
BOOST_CLASS_EXPORT_KEY2(tesseract_planning::TrajOptMotionPipelineTask, "TrajOptMotionPipelineTask")

#endif  // TESSERACT_TASK_COMPOSER_TRAJOPT_MOTION_PIPELINE_TASK_H
