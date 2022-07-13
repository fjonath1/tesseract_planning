/**
 * @file move_instruction_poly.h
 * @brief The move instruction interface
 *
 * @author Levi Armstrong
 * @date June 15, 2020
 * @version TODO
 * @bug No known bugs
 *
 * @copyright Copyright (c) 2020, Southwest Research Institute
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
#ifndef TESSERACT_COMMAND_LANGUAGE_MOVE_INSTRUCTION_POLY_H
#define TESSERACT_COMMAND_LANGUAGE_MOVE_INSTRUCTION_POLY_H

#include <tesseract_common/macros.h>
TESSERACT_COMMON_IGNORE_WARNINGS_PUSH
#include <string>
#include <boost/serialization/base_object.hpp>
#include <boost/serialization/export.hpp>
#include <boost/concept_check.hpp>
TESSERACT_COMMON_IGNORE_WARNINGS_POP

#include <tesseract_command_language/core/instruction.h>
#include <tesseract_command_language/core/waypoint.h>
#include <tesseract_common/manipulator_info.h>
#include <tesseract_common/serialization.h>
#include <tesseract_common/type_erasure.h>

#ifdef SWIG
%ignore std::vector<tesseract_planning::MoveInstructionPoly>::vector(size_type);
%ignore std::vector<tesseract_planning::MoveInstructionPoly>::resize(size_type);
%ignore tesseract_planning::MoveInstructionPoly::getType;
%pythondynamic tesseract_planning::MoveInstructionPoly;
#endif  // SWIG

/** @brief If shared library, this must go in the header after the class definition */
#define TESSERACT_MOVE_INSTRUCTION_EXPORT_KEY(N, C)                                                                    \
  namespace N                                                                                                          \
  {                                                                                                                    \
  using C##InstanceBase =                                                                                              \
      tesseract_common::TypeErasureInstance<C, tesseract_planning::detail_move_instruction::MoveInstructionInterface>; \
  using C##Instance = tesseract_planning::detail_move_instruction::MoveInstructionInstance<C>;                         \
  using C##InstanceWrapper = tesseract_common::TypeErasureInstanceWrapper<C##Instance>;                                \
  }                                                                                                                    \
  BOOST_CLASS_EXPORT_KEY(N::C##InstanceBase)                                                                           \
  BOOST_CLASS_EXPORT_KEY(N::C##Instance)                                                                               \
  BOOST_CLASS_EXPORT_KEY(N::C##InstanceWrapper)                                                                        \
  BOOST_CLASS_TRACKING(N::C##InstanceBase, boost::serialization::track_never)                                          \
  BOOST_CLASS_TRACKING(N::C##Instance, boost::serialization::track_never)                                              \
  BOOST_CLASS_TRACKING(N::C##InstanceWrapper, boost::serialization::track_never)

/** @brief If shared library, this must go in the cpp after the implicit instantiation of the serialize function */
#define TESSERACT_MOVE_INSTRUCTION_EXPORT_IMPLEMENT(inst)                                                              \
  BOOST_CLASS_EXPORT_IMPLEMENT(inst##InstanceBase)                                                                     \
  BOOST_CLASS_EXPORT_IMPLEMENT(inst##Instance)                                                                         \
  BOOST_CLASS_EXPORT_IMPLEMENT(inst##InstanceWrapper)

/**
 * @brief This should not be used within shared libraries use the two above.
 * If not in a shared library it can go in header or cpp
 */
#define TESSERACT_MOVE_INSTRUCTION_EXPORT(N, C)                                                                        \
  TESSERACT_MOVE_INSTRUCTION_EXPORT_KEY(N, C)                                                                          \
  TESSERACT_MOVE_INSTRUCTION_EXPORT_IMPLEMENT(N::C)

namespace tesseract_planning
{
enum class MoveInstructionType : int
{
  LINEAR = 0,
  FREESPACE = 1,
  CIRCULAR = 2,
  START = 3 /**< This indicates it is a start instruction. */
};
}

#ifndef SWIG
namespace tesseract_planning::detail_move_instruction
{
template <typename T>
struct MoveInstructionConcept  // NOLINT
  : boost::Assignable<T>,
    boost::CopyConstructible<T>,
    boost::EqualityComparable<T>
{
  BOOST_CONCEPT_USAGE(MoveInstructionConcept)
  {
    T cp(c);
    T assign = c;
    bool eq = (c == cp);
    bool neq = (c != cp);
    UNUSED(assign);
    UNUSED(eq);
    UNUSED(neq);

    tesseract_common::ManipulatorInfo info;
    c.setManipulatorInfo(info);

    tesseract_common::ManipulatorInfo& info_ref = c.getManipulatorInfo();
    UNUSED(info_ref);

    const tesseract_common::ManipulatorInfo& info_const_ref = c.getManipulatorInfo();
    UNUSED(info_const_ref);

    const std::string& profile_const = c.getProfile();
    UNUSED(profile_const);

    const std::string& path_profile_const = c.getPathProfile();
    UNUSED(path_profile_const);

    c.setMoveType(MoveInstructionType::FREESPACE);
    MoveInstructionType type = c.getMoveType();
    UNUSED(type);

    const std::string& desc = c.getDescription();
    UNUSED(desc);

    c.setDescription("test");
    c.print();
    c.print("prefix_");
  }

private:
  T c;
};

struct MoveInstructionInterface : tesseract_common::TypeErasureInterface
{
  virtual void setWaypoint(Waypoint waypoint) = 0;
  virtual Waypoint& getWaypoint() = 0;
  virtual const Waypoint& getWaypoint() const = 0;

  virtual void setManipulatorInfo(tesseract_common::ManipulatorInfo info) = 0;
  virtual const tesseract_common::ManipulatorInfo& getManipulatorInfo() const = 0;
  virtual tesseract_common::ManipulatorInfo& getManipulatorInfo() = 0;

  virtual void setProfile(const std::string& profile) = 0;
  virtual const std::string& getProfile() const = 0;

  virtual void setPathProfile(const std::string& profile) = 0;
  virtual const std::string& getPathProfile() const = 0;

  virtual void setMoveType(MoveInstructionType move_type) = 0;
  virtual MoveInstructionType getMoveType() const = 0;

  virtual const std::string& getDescription() const = 0;
  virtual void setDescription(const std::string& description) = 0;

  virtual void print(const std::string& prefix) const = 0;

private:
  friend class boost::serialization::access;
  friend struct tesseract_common::Serialization;
  template <class Archive>
  void serialize(Archive& ar, const unsigned int version);  // NOLINT
};

template <typename T>
struct MoveInstructionInstance : tesseract_common::TypeErasureInstance<T, MoveInstructionInterface>  // NOLINT
{
  using BaseType = tesseract_common::TypeErasureInstance<T, MoveInstructionInterface>;
  MoveInstructionInstance() = default;
  MoveInstructionInstance(const T& x) : BaseType(x) {}
  MoveInstructionInstance(MoveInstructionInstance&& x) noexcept : BaseType(std::move(x)) {}

  BOOST_CONCEPT_ASSERT((MoveInstructionConcept<T>));

  void setWaypoint(Waypoint waypoint) final { this->get().setWaypoint(std::move(waypoint)); }
  Waypoint& getWaypoint() final { return this->get().getWaypoint(); }
  const Waypoint& getWaypoint() const final { return this->get().getWaypoint(); };

  void setManipulatorInfo(tesseract_common::ManipulatorInfo info) final
  {
    this->get().setManipulatorInfo(std::move(info));
  }
  const tesseract_common::ManipulatorInfo& getManipulatorInfo() const final { return this->get().getManipulatorInfo(); }
  tesseract_common::ManipulatorInfo& getManipulatorInfo() final { return this->get().getManipulatorInfo(); }

  void setProfile(const std::string& profile) final { this->get().setProfile(profile); }
  const std::string& getProfile() const final { return this->get().getProfile(); }

  void setPathProfile(const std::string& profile) final { this->get().setPathProfile(profile); }
  const std::string& getPathProfile() const final { return this->get().getPathProfile(); }

  void setMoveType(MoveInstructionType move_type) final { this->get().setMoveType(move_type); }
  MoveInstructionType getMoveType() const final { return this->get().getMoveType(); }

  const std::string& getDescription() const final { return this->get().getDescription(); }
  void setDescription(const std::string& description) final { this->get().setDescription(description); }

  void print(const std::string& prefix) const final { this->get().print(prefix); }

private:
  friend class boost::serialization::access;
  friend struct tesseract_common::Serialization;
  template <class Archive>
  void serialize(Archive& ar, const unsigned int /*version*/)  // NOLINT
  {
    ar& boost::serialization::make_nvp("base", boost::serialization::base_object<BaseType>(*this));
  }
};
}  // namespace tesseract_planning::detail_move_instruction
#endif  // SWIG

namespace tesseract_planning
{
using MoveInstructionPolyBase = tesseract_common::TypeErasureBase<detail_move_instruction::MoveInstructionInterface,
                                                                  detail_move_instruction::MoveInstructionInstance>;
struct MoveInstructionPoly : MoveInstructionPolyBase
{
  using MoveInstructionPolyBase::MoveInstructionPolyBase;

  void setWaypoint(Waypoint waypoint);
  Waypoint& getWaypoint();
  const Waypoint& getWaypoint() const;

  void setManipulatorInfo(tesseract_common::ManipulatorInfo info);
  const tesseract_common::ManipulatorInfo& getManipulatorInfo() const;
  tesseract_common::ManipulatorInfo& getManipulatorInfo();

  void setProfile(const std::string& profile);
  const std::string& getProfile() const;

  void setPathProfile(const std::string& profile);
  const std::string& getPathProfile() const;

  void setMoveType(MoveInstructionType move_type);
  MoveInstructionType getMoveType() const;

  const std::string& getDescription() const;
  void setDescription(const std::string& description);

  void print(const std::string& prefix = "") const;

  bool isLinear() const;

  bool isFreespace() const;

  bool isCircular() const;

  bool isStart() const;

private:
  friend class boost::serialization::access;
  friend struct tesseract_common::Serialization;

  template <class Archive>
  void serialize(Archive& ar, const unsigned int /*version*/);  // NOLINT
};

}  // namespace tesseract_planning

#ifdef SWIG
%template(Instructions) std::vector<tesseract_planning::MoveInstructionPoly>;
%tesseract_command_language_add_instruction_type(MoveInstructionPoly)
#else
BOOST_SERIALIZATION_ASSUME_ABSTRACT(tesseract_planning::detail_instruction::InstructionInterface)
BOOST_CLASS_EXPORT_KEY(tesseract_planning::detail_move_instruction::MoveInstructionInterface)
BOOST_CLASS_TRACKING(tesseract_planning::detail_move_instruction::MoveInstructionInterface,
                     boost::serialization::track_never)

BOOST_CLASS_EXPORT_KEY(tesseract_planning::MoveInstructionPolyBase)
BOOST_CLASS_TRACKING(tesseract_planning::MoveInstructionPolyBase, boost::serialization::track_never)

BOOST_CLASS_EXPORT_KEY(tesseract_planning::MoveInstructionPoly)
BOOST_CLASS_TRACKING(tesseract_planning::MoveInstructionPoly, boost::serialization::track_never);

TESSERACT_INSTRUCTION_EXPORT_KEY(tesseract_planning, MoveInstructionPoly);

#endif  // SWIG

#endif  // TESSERACT_COMMAND_LANGUAGE_MOVE_INSTRUCTION_POLY_H