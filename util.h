#pragma once

#include "typetraits.h"
#include <cstddef>

namespace hxqstl{
    template<class T>
    typename std::remove_reference<T>::type&& move(T&& arg) noexcept{
        return static_cast<typename std::remove_reference<T>::type&&>(arg);
    }

    template<class T>
    T&& forward(typename std::remove_reference<T>::type& arg) noexcept{
        return static_cast<T&&>(arg);
    }

    template<class T>
    T&& forward(typename std::remove_reference<T>::type&& arg) noexcept{
        static_assert(!std::is_lvalue_reference<T>::value,"bad forward");
        return static_cast<T&&>(arg);
    }

    //swap
    template<class Tp>
    void swap(Tp& lhs,Tp& rhs){
        auto tmp(hxqstl::move(lhs));
        lhs = hxqstl::move(rhs);
        rhs = hxqstl::move(tmp);
    }

    template<class ForwardIter1,class ForwardIter2>
    ForwardIter2 swap_range(ForwardIter1 first1,ForwardIter1 last1,ForwardIter2 first2){
        for(;first1 != last1;++first1,(void)++first2){
            hxqstl::swap(*first1,*first2);
        }
        return first2;
    }

    // a是一个数组引用
    template<class Tp,size_t N>
    void swap(Tp(&a)[N],Tp(&b)[N]){
        hxqstl::swap_range(a,a+N,b);
    }

    template<class Ty1,class Ty2>
    struct pair
    {
        typedef Ty1 first_type;
        typedef Ty2 second_type;

        first_type first;
        second_type second;

        // default constructiable
        template<class Other1 = Ty1,class Other2 = Ty2,
            typename = typename std::enable_if<std::is_default_constructible<Other1>::value && std::is_default_constructible<Other2>::value,void>::type>
                constexpr pair() : first(),second()
                {

                }
        
        // implicit constructiable for this type
        template<class U1 = Ty1,class U2 = Ty2,typename std::enable_if<
                                                        std::is_copy_constructible<U1>::value &&
                                                        std::is_copy_constructible<U2>::value &&
                                                        std::is_convertible<const U1&,Ty1>::value &&
                                                        std::is_convertible<const U2&,Ty2>::value,int>::type = 0>
                constexpr pair(const Ty1& a,const Ty2& b) : first(a),second(b) {

                }

        //explicit constructible for this type
        template<class U1 = Ty1,class U2 = Ty2,typename std::enable_if<
                                                std::is_copy_constructible<U1>::value &&
                                                std::is_copy_constructible<U2>::value &&
                                                (!std::is_convertible<const U1&,Ty1>::value ||
                                                 !std::is_convertible<const U2&,Ty2>::value),int>::type = 0>
        explicit constexpr pair(const Ty1& a,const Ty2& b) : first(a),second(b) {

        }

        pair(const pair& rhs) = default;
        pair(pair&& rhs) = default;
        
        template<class Other1,class Other2,typename std::enabla_if<std::is_constructible<Ty1,Other1>::value &&
                                                                    std::is_constructible<Ty2,Other2>::value &&
                                                                    std::is_convertible<Other1&&,Ty1>::value &&
                                                                    std::is_convertible<Other2&&,Ty2>::value,int>::type = 0>
        constexpr pair(Other1&& a,Other2&& b) : first(hxqstl::forward<Other1>(a)),second(hxqstl::forward<Other2>(b)){
            
        }

        // Ty1可以通过类型Other1进行构造，Ty2可以通过Other2进行构造
        // Other1不能隐式转换为类型Ty1,类型Other2不能隐式转换成类型Ty2
        template<class Other1,class Other2,typename std::enable_if<std::is_constructible<Ty1,Other1>::value &&
                                                                    std::is_constructible<Ty2,Other2>::value &&
                                                                    (!std::is_convertible<Other1,Ty1>::value ||
                                                                     !std::is_convertible<Other2,Ty2>::value),int>::type = 0>
        explicit constexpr pair(Other1&& a,Other2&& b) : first(hxqstl::forward<Other1>(a)),second(hxqstl::forward<Other2>(b)){
            
        }

        template<class Other1,class Other2,typename std::enable_if<
                                            std::is_constructible<Ty1,const Other1&>::value &&
                                            std::is_constructible<Ty2,const Other2&>::value &&
                                            std::is_convertible<const Other1&,Ty1>::value &&
                                            std::is_convertible<const Other2&,Ty2>::value,int>::type = 0>
        constexpr pair(const pair<Other1,Other2>& other) : first(other.first),second(other.second){

        }

        // explicit constructiable for other pair
        template <class Other1, class Other2,typename std::enable_if<
                                            std::is_constructible<Ty1, const Other1&>::value &&
                                            std::is_constructible<Ty2, const Other2&>::value &&
                                            (!std::is_convertible<const Other1&, Ty1>::value ||
                                             !std::is_convertible<const Other2&, Ty2>::value), int>::type = 0>
        explicit constexpr pair(const pair<Other1, Other2>& other) : first(other.first),second(other.second)
        {

        }

        template<class Other1,class Other2,typename std::enable_if<
                                            std::is_constructible<Ty1,const Other1&>::value &&
                                            std::is_constructible<Ty2,const Other2&>::value &&
                                            std::is_convertible<const Other1&,Ty1>::value &&
                                            std::is_convertible<const Other2&,Ty2>::value,int>::type = 0>
        constexpr pair(const pair<Other1,Other2>&& other) : first(hxqstl::forward<Other1>(other.first)),second(hxqstl::forward<Other2>(other.second)){

        }

        // explicit constructiable for other pair
        template <class Other1, class Other2,typename std::enable_if<
                                            std::is_constructible<Ty1, const Other1&>::value &&
                                            std::is_constructible<Ty2, const Other2&>::value &&
                                            (!std::is_convertible<const Other1&, Ty1>::value ||
                                             !std::is_convertible<const Other2&, Ty2>::value), int>::type = 0>
        explicit constexpr pair(const pair<Other1, Other2>&& other) : first(hxqstl::forward<Other1>(other.first)),second(hxqstl::forward<Other2>(other.second))
        {
            
        }

        pair& operator=(const pair& rhs){
            if(this != &rhs){
                first = rhs.first;
                second = rhs.second;
            }
            return *this;
        }

        pair& operator=(pair&& rhs){
            if(this != &rhs){
                first = hxqstl::move(rhs.first);
                second = hxqstl::move(rhs.second);
            }
            return *this;
        }

        template<class Other1,class Other2>
        pair& operator=(const pair<Other1,Other2>& other){
            first = other.first;
            second = other.second;
            return *this;
        }

        template<class Other1,class Other2>
        pair& operator=(pair<Other1,Other2>&& other){
            first = hxqstl::forward<Other1>(other.first);
            second = hxqstl::forward<Other2>(other.second);
            return *this;
        }

        ~pair() = default;

        void swap(pair& other){
            if(this != &other){
                hxqstl::swap(first,other.first);
                hxqstl::swap(second,other.second);
            }
        }
    };

    template<class Ty1,class Ty2>
    void swap(pair<Ty1,Ty2>& lhs,pair<Ty1,Ty2>& rhs){
        lhs.swap(rhs);
    }

    template<class Ty1,class Ty2>
    bool operator<(const pair<Ty1,Ty2>& lhs,const pair<Ty1,Ty2>& rhs){
        return lhs.first < rhs.first || (lhs.first == rhs.first && lhs.second < rhs.second);
    }

    template<class Ty1,class Ty2>
    bool operator==(const pair<Ty1,Ty2>& lhs,const pair<Ty1,Ty2>& rhs){
        return lhs.first==rhs.first && lhs.second == rhs.second;
    }

    template<class Ty1,class Ty2>
    bool operator!=(const pair<Ty1,Ty2>& lhs,const pair<Ty1,Ty2>& rhs){
        return !(lhs == rhs);
    }

    template<class Ty1,class Ty2>
    bool operator>(const pair<Ty1,Ty2>& lhs,const pair<Ty1,Ty2>& rhs){
        return rhs < lhs;
    }

    template<class Ty1,class Ty2>
    bool operator>=(const pair<Ty1,Ty2>& lhs,const pair<Ty1,Ty2>& rhs){
        return !(rhs < lhs);
    }

    template<class Ty1,class Ty2>
    bool operator<=(const pair<Ty1,Ty2>& lhs,const pair<Ty1,Ty2>& rhs){
        return !(rhs > lhs);
    }  

    template<class Ty1,class Ty2>
    pair<Ty1,Ty2> make_pair(Ty1&& first,Ty2&& second){
        return pair<Ty1,Ty2>(hxqstl::forward<Ty1>(first),hxqstl::forward<Ty2>(second));
    }
}