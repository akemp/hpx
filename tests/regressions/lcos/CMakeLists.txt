# Copyright (c) 2007-2013 Hartmut Kaiser
# Copyright (c) 2011-2012 Bryce Adelstein-Lelbach
#
# Distributed under the Boost Software License, Version 1.0. (See accompanying
# file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

set(tests
    after_588
    async_unwrap_1037
    dataflow_launch_775
    async_callback_non_deduced_context
    future_790
    future_hang_on_get_629
    future_hang_on_then_629
    future_timed_wait_1025
    future_unwrap_878
    future_unwrap_1182
    ignore_while_locked_1485
    lifetime_588
    lifetime_588_1
    promise_leak_996
    set_hpx_limit_798
    shared_stated_leaked_1211
   )


set(dataflow_future_swap_FLAGS DEPENDENCIES iostreams_component)
set(dataflow_future_swap2_FLAGS DEPENDENCIES iostreams_component)
set(dataflow_future_swap_PARAMETERS THREADS_PER_LOCALITY 4)
set(tests ${tests}
    dataflow_future_swap
    dataflow_791
   )
 if(HPX_WITH_CXX11_LAMBDAS)
  set(tests ${tests}
      dataflow_const_functor_773
      dataflow_future_swap2
      dataflow_using_774
     )
endif()

set(after_588_PARAMETERS LOCALITIES 2)

set(async_callback_non_deduced_context_PARAMETERS THREADS_PER_LOCALITY 4)
set(future_hang_on_get_629_PARAMETERS LOCALITIES 2 THREADS_PER_LOCALITY 2)
set(dataflow_launch_775_PARAMETERS THREADS_PER_LOCALITY 2)

set(ignore_while_locked_1485_PARAMETERS THREADS_PER_LOCALITY 2)

set(lifetime_588_PARAMETERS LOCALITIES 2 THREADS_PER_LOCALITY 2)
set(lifetime_588_1_PARAMETERS LOCALITIES 2 THREADS_PER_LOCALITY 2)

set(set_hpx_limit_771_FLAGS DEPENDENCIES dataflow_component)

if(HPX_WITH_CXX11_LAMBDAS)
  set(future_hang_on_wait_with_callback_629_PARAMETERS THREADS_PER_LOCALITY 4)
  set(tests
    ${tests}
    future_hang_on_wait_with_callback_629)
endif()

foreach(test ${tests})
  set(sources
      ${test}.cpp)

  source_group("Source Files" FILES ${sources})

  # add example executable
  add_hpx_executable(${test}_test
                     SOURCES ${sources}
                     ${${test}_FLAGS}
                     EXCLUDE_FROM_ALL
                     HPX_PREFIX ${HPX_BUILD_PREFIX}
                     FOLDER "Tests/Regressions/LCOs")

  add_hpx_regression_test("lcos" ${test} ${${test}_PARAMETERS})

  # add a custom target for this example
  add_hpx_pseudo_target(tests.regressions.lcos.${test})

  # make pseudo-targets depend on master pseudo-target
  add_hpx_pseudo_dependencies(tests.regressions.lcos
                              tests.regressions.lcos.${test})

  # add dependencies to pseudo-target
  add_hpx_pseudo_dependencies(tests.regressions.lcos.${test}
                              ${test}_test_exe)
endforeach()

add_hpx_test(
  "tests.regressions.lcos" after_588_4
  EXECUTABLE after_588
  LOCALITIES 2
  THREADS_PER_LOCALITY 4)
