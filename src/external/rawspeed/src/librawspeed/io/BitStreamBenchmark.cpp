/*
    RawSpeed - RAW file decoder.

    Copyright (C) 2017 Roman Lebedev

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
*/

#include "io/BitPumpJPEG.h"          // for BitPumpJPEG
#include "io/BitPumpLSB.h"           // for BitPumpLSB, BitStream<>::fillCache
#include "io/BitPumpMSB.h"           // for BitPumpMSB
#include "io/BitPumpMSB16.h"         // for BitPumpMSB16
#include "io/BitPumpMSB32.h"         // for BitPumpMSB32
#include "io/Buffer.h"               // for Buffer, Buffer::size_type, Data...
#include "io/ByteStream.h"           // for ByteStream
#include <algorithm>                 // for min
#include <benchmark/benchmark_api.h> // for State, Benchmark, Initialize
#include <cassert>                   // for assert
#include <cstddef>                   // for size_t
#include <limits>                    // for numeric_limits
#include <string>                    // for string, to_string
#include <type_traits>               // for integral_constant

using rawspeed::BitPumpLSB;
using rawspeed::BitPumpMSB;
using rawspeed::BitPumpMSB16;
using rawspeed::BitPumpMSB32;
using rawspeed::BitPumpJPEG;

static constexpr const size_t STEP_MAX = 32;

template <typename Pump>
static inline void BM_BitStream(benchmark::State& state, bool inNativeByteOrder,
                                unsigned int fillSize, unsigned int Step) {
  assert(state.range(0) > 0);
  assert((size_t)state.range(0) <=
         std::numeric_limits<rawspeed::Buffer::size_type>::max());

  assert(fillSize > 0);
  assert(fillSize <= STEP_MAX);

  assert(Step > 0);
  assert(Step <= STEP_MAX);

  assert(Step <= fillSize);

  const rawspeed::Buffer b(state.range(0));
  assert(b.getSize() > 0);
  assert(b.getSize() == (size_t)state.range(0));

  const rawspeed::DataBuffer db(b, inNativeByteOrder);
  const rawspeed::ByteStream bs(db);

  Pump pump(bs);

  size_t processedBits = 0;
  while (state.KeepRunning()) {
    pump.resetBufferPosition();

    for (processedBits = 0; processedBits <= b.getSize();) {
      pump.fill(fillSize);

      // NOTE: you may want to change the callee here
      for (auto i = 0U; i < fillSize; i += Step)
        pump.skipBits(Step);

      processedBits += fillSize;
    }
  }

  state.SetComplexityN(processedBits / 8);
  state.SetItemsProcessed(state.complexity_length_n() * state.iterations());
  state.SetBytesProcessed(state.items_processed());
}

static inline void CustomArguments(benchmark::internal::Benchmark* b) {
  b->RangeMultiplier(2);
#if 1
  b->Arg(256 << 20);
#else
  const size_t maxBytes =
      std::min(static_cast<size_t>(std::numeric_limits<int>::max()),
               static_cast<size_t>(
                   std::numeric_limits<rawspeed::Buffer::size_type>::max()));
  b->Range(1, maxBytes);
  b->Complexity(benchmark::oN);
#endif
  b->Unit(benchmark::kMillisecond);
}

using Native = std::integral_constant<bool, true>;
using NonNative = std::integral_constant<bool, false>;

template <typename BO, typename PUMP>
void registerPump(const char* byteOrder, const char* pumpName) {
  for (size_t i = 1; i <= STEP_MAX; i *= 2) {
    for (size_t j = 1; j <= i && j <= STEP_MAX; j *= 2) {
      std::string name("BM_BitStream<ByteOrder<");
      name += byteOrder;
      name += ">, Spec<";
      name += pumpName;
      name += ">, Fill<";
      name += std::to_string(i);
      name += ">, Step<";
      name += std::to_string(j);
      name += ">>";

      const auto Fn = BM_BitStream<PUMP>;
      auto* b = benchmark::RegisterBenchmark(name.c_str(), Fn, BO::value, i, j);
      b->Apply(CustomArguments);
    }
  }
}

#define REG_PUMP_2(BO, PUMP) registerPump<BO, PUMP>(#BO, #PUMP);
#define REGISTER_PUMP(PUMP) REG_PUMP_2(Native, PUMP) REG_PUMP_2(NonNative, PUMP)

int main(int argc, char** argv) {
  REGISTER_PUMP(BitPumpLSB);
  REGISTER_PUMP(BitPumpMSB);
  REGISTER_PUMP(BitPumpMSB16);
  REGISTER_PUMP(BitPumpMSB32);
  REGISTER_PUMP(BitPumpJPEG);

  benchmark::Initialize(&argc, argv);
  benchmark::RunSpecifiedBenchmarks();
}
