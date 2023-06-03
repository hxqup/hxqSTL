#pragma once

#include <type_traits>
namespace hxqstl{
    template<class T,T v>
    struct m_integral_const{
        static constexpr T value = v;
    };

    template<bool b>
    using m_bool_constant = m_integral_const<bool,b>;

    typedef m_bool_constant<true> m_true_type;
    typedef m_bool_constant<false> m_false_type;

    /*****************************************************/
    template<class T1,class T2>
    struct pair;

    // 这里的冒号代表继承
    template<class T>
    struct is_pair:hxqstL::m_false_type{};

    template<class T1,class T2>
    struct is_pair<hxqstl::pair<T1, T2>> : mystl::m_true_type {};
}