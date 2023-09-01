/*
 * Copyright (c) 2022 NVIDIA Corporation
 *
 * Licensed under the Apache License Version 2.0 with LLVM Exceptions
 * (the "License"); you may not use this file except in compliance with
 * the License. You may obtain a copy of the License at
 *
 *   https://llvm.org/LICENSE.txt
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <nvexec/stream_context.cuh>
#include <stdexec/execution.hpp>

#include <thrust/device_vector.h>

#include <cstdio>
#include <span>

namespace ex = stdexec;
using stdexec::__tag_invoke::tag_invoke;

struct sink_receiver {
  using is_receiver = void;

  friend void tag_invoke(stdexec::set_value_t, sink_receiver, auto&&...) noexcept {
  }

  friend void tag_invoke(stdexec::set_error_t, sink_receiver, auto&&) noexcept {
  }

  friend void tag_invoke(stdexec::set_stopped_t, sink_receiver) noexcept {
  }

  friend stdexec::empty_env tag_invoke(stdexec::get_env_t, sink_receiver) noexcept {
    return {};
  }
};

struct test_env { };

struct test_tag { };

struct test_sender { };

template <class _InitT, class _Fun>
struct Data {
  _InitT __initT_;
  STDEXEC_NO_UNIQUE_ADDRESS _Fun __fun_;
  static constexpr auto __mbrs_ = stdexec::__mliterals<&Data::__initT_, &Data::__fun_>();
};

struct Children { };

template <class _Tag, class... _Captures>
struct fn_struct { };



int main() {
  using namespace stdexec;

  const int n = 2 * 1024;
  thrust::device_vector<float> input(n, 1.0f);
  float* first = thrust::raw_pointer_cast(input.data());
  float* last = thrust::raw_pointer_cast(input.data()) + input.size();
    
  auto test = nvexec::reduce(42.0f);
  auto ourBasicSender = __make_basic_sender(nvexec::reduce, Data<float, cub::Sum>{});

  template <class _Tag, class _Sender, class _Env, class _Tuple, class _Variant>
  using __gather_completions_for = //
    __meval<                       //
      __gather_signal,
      _Tag,
      __completion_signatures_of_t<_Sender, _Env>,
      _Tuple,
      _Variant>;
      __gather_completions_for<nvexec::reduce, decltype(ourBasicSender, emp)>;
  stdexec::print(finished_Type);
  // stdexec::__compl_sigs_impl
  // auto [result] =
  // stdexec::sync_wait(ex::on(stream_ctx.get_scheduler(), std::move(snd))).value();
}

/*
 auto snd = ex::just(std::span{first, last})
                      | nvexec::reduce(52.0f);
  // using stdexec::__tag_invoke::tag_invoke;

  // tag_invoke(stdexec::get_completion_signatures, snd, test_env{});
  

  // typename snd_type::__t::_completion_signatures_t<snd_type, empty_environment> hey;
  //  typename snd_type::__t::_set_value_t<decltype(first)> value_typer;
  // stdexec::completion_signatures sup = hey;
  // stdexec::print(hey);

__mtry_eval<
    __try_value_types_of_t,
    decltype(ourBasicSender),
    test_env,
    set_value_t,
    __q<__compl_sigs::__ensure_concat> >
    finished_Type;
 static_assert(stdexec::__completion_signature<decltype(hey)>, "Wat");

  stdexec::print(sup);
  nvexec::stream_scheduler gpu = stream_ctx.get_scheduler();
  using stdexec::__tag_invoke::tag_invoke;

  tag_invoke(stdexec::get_completion_signatures, snd, stdexec::no_env{});
*/