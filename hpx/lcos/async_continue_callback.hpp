//  Copyright (c) 2007-2013 Hartmut Kaiser
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

///////////////////////////////////////////////////////////////////////////////

#if !defined(HPX_LCOS_ASYNC_CONTINUE_CALLBACK_MAR_30_2015_1132AM)
#define HPX_LCOS_ASYNC_CONTINUE_CALLBACK_MAR_30_2015_1132AM

#include <hpx/lcos/async_continue.hpp>

namespace hpx
{
    ///////////////////////////////////////////////////////////////////////////
    namespace detail
    {
        template <
            typename Action, typename RemoteResult, typename Cont,
            typename Target, typename Callback, typename ...Ts>
        lcos::future<
            typename traits::promise_local_result<
                typename result_of_async_continue<Action, Cont>::type
            >::type
        >
        async_continue_r_cb(Cont&& cont, Target const& target,
            Callback&& cb, Ts&&... vs)
        {
            typedef
                typename traits::promise_local_result<
                    typename result_of_async_continue<Action, Cont>::type
                >::type
            result_type;

            typedef
                typename hpx::actions::extract_action<
                    Action
                >::result_type
            continuation_result_type;

            lcos::promise<result_type, RemoteResult> p;
            apply_cb<Action>(
                boost::make_shared<
                    hpx::actions::typed_continuation<continuation_result_type>
                >(p.get_gid(), std::forward<Cont>(cont))
              , target, std::forward<Callback>(cb), std::forward<Ts>(vs)...);
            return p.get_future();
        }
    }

    ///////////////////////////////////////////////////////////////////////////
    template <typename Action, typename Cont, typename Callback, typename ...Ts>
    lcos::future<
        typename traits::promise_local_result<
            typename detail::result_of_async_continue<Action, Cont>::type
        >::type
    >
    async_continue_cb(Cont&& cont, naming::id_type const& gid, Callback&& cb,
        Ts&&... vs)
    {
        typedef
            typename traits::promise_local_result<
                typename detail::result_of_async_continue<Action, Cont>::type
            >::type
        result_type;

        return detail::async_continue_r_cb<Action, result_type>(
            std::forward<Cont>(cont), gid, std::forward<Callback>(cb),
            std::forward<Ts>(vs)...);
    }

    template <
        typename Component, typename Signature, typename Derived,
        typename Cont, typename Callback, typename ...Ts>
    lcos::future<
         typename traits::promise_local_result<
            typename detail::result_of_async_continue<Derived, Cont>::type
        >::type
    >
    async_continue_cb(
        hpx::actions::basic_action<Component, Signature, Derived> /*act*/
      , Cont&& cont, naming::id_type const& gid, Callback&& cb, Ts&&... vs)
    {
        return async_continue_cb<Derived>(
            std::forward<Cont>(cont), gid, std::forward<Callback>(cb),
            std::forward<Ts>(vs)...);
    }

    ///////////////////////////////////////////////////////////////////////////
    template <typename Action, typename Cont, typename DistPolicy,
        typename Callback, typename ...Ts>
    typename boost::enable_if_c<
        traits::is_distribution_policy<DistPolicy>::value,
        lcos::future<
            typename traits::promise_local_result<
                typename detail::result_of_async_continue<Action, Cont>::type
            >::type>
    >::type
    async_continue_cb(Cont&& cont, DistPolicy const& policy, Callback&& cb,
        Ts&&... vs)
    {
        typedef
            typename traits::promise_local_result<
                typename detail::result_of_async_continue<Action, Cont>::type
            >::type
        result_type;

        return detail::async_continue_r_cb<Action, result_type>(
            std::forward<Cont>(cont), policy, std::forward<Callback>(cb),
            std::forward<Ts>(vs)...);
    }

    template <
        typename Component, typename Signature, typename Derived,
        typename Cont, typename DistPolicy, typename Callback, typename ...Ts>
    typename boost::enable_if_c<
        traits::is_distribution_policy<DistPolicy>::value,
        lcos::future<
            typename traits::promise_local_result<
                typename detail::result_of_async_continue<Derived, Cont>::type
            >::type>
    >::type
    async_continue_cb(
        hpx::actions::basic_action<Component, Signature, Derived> /*act*/
      , Cont&& cont, DistPolicy const& policy, Callback&& cb, Ts&&... vs)
    {
        return async_continue_cb<Derived>(
            std::forward<Cont>(cont), policy, std::forward<Callback>(cb),
            std::forward<Ts>(vs)...);
    }
}

#endif
