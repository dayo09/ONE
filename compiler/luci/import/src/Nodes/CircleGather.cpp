/*
 * Copyright (c) 2020 Samsung Electronics Co., Ltd. All Rights Reserved
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "luci/Import/Nodes/CircleGather.h"

#include <luci/IR/Nodes/CircleGather.h>

#include <loco.h>
#include <oops/UserExn.h>

namespace luci
{

bool CircleGatherGraphBuilder::validate(const ValidateArgs &args) const
{
  const auto &inputs = args.op.inputs;
  const auto &outputs = args.op.outputs;
  const auto *options = args.op.builtin_options.AsGatherOptions();

  int32_t axis = options->axis;

  if (inputs.size() != 2)
    return false;

  if (outputs.size() != 1)
    return false;

  if (axis < 0)
    axis += inputs.size();

  if (axis < 0)
    return false;

  // TODO do indices type check
  // TODO do axis check when shape information is given

  return true;
}

CircleNode *CircleGatherGraphBuilder::build_node(const circle::OperatorT &op,
                                                 const std::vector<CircleNode *> &inputs,
                                                 loco::Graph *graph) const
{
  auto *node = graph->nodes()->create<CircleGather>();

  node->params(inputs[0]);
  node->indices(inputs[1]);

  const auto *options = op.builtin_options.AsGatherOptions();
  node->axis(options->axis);

  return node;
}

} // namespace luci
