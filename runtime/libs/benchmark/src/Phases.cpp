/*
 * Copyright (c) 2020 Samsung Electronics Co., Ltd. All Rights Reserved
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "benchmark/Phases.h"
#include "benchmark/Types.h"

#include <chrono>
#include <cassert>

namespace
{

inline uint64_t nowMicros()
{
  auto time_point = std::chrono::high_resolution_clock::now();
  auto since_epoch = time_point.time_since_epoch();
  // default precision of high resolution clock is 10e-9 (nanoseconds)
  return std::chrono::duration_cast<std::chrono::microseconds>(since_epoch).count();
}
}

namespace benchmark
{

Phases::Phases(const PhaseOption &option)
    : _option(option), _mem_poll(std::chrono::milliseconds(5), option.memory_gpu)
{
  // DO NOTHING
}

void Phases::run(const std::string &tag, const PhaseFunc &exec, const PhaseFunc *post,
                 uint32_t loop_num, bool option_disable)
{
  Phase phase{tag, loop_num};
  PhaseEnum p = getPhaseEnum(tag);
  for (uint32_t i = 0; i < loop_num; ++i)
  {
    if (!option_disable && _option.memory)
      _mem_poll.start(p);

    uint64_t t = 0u;
    t = nowMicros();

    exec(phase, i);

    t = nowMicros() - t;

    if (!option_disable && _option.memory)
      _mem_poll.end(p);

    phase.time.emplace_back(t);

    if (!option_disable && _option.memory)
    {
      phase.memory[MemoryType::RSS].emplace_back(_mem_poll.getRssMap().at(p));
      phase.memory[MemoryType::HWM].emplace_back(_mem_poll.getHwmMap().at(p));
      phase.memory[MemoryType::PSS].emplace_back(_mem_poll.getPssMap().at(p));
    }

    if (post)
      (*post)(phase, i);
  }

  if (p == PhaseEnum::END_OF_PHASE)
  {
    return;
  }

  assert(_phases.find(tag) == _phases.end());
  _phases.emplace(tag, phase);
}

} // namespace benchmark
