#pragma once

#include <cstring>
#include "iterator.h"
#include "util.h"

namespace hxqstl{
    #ifdef max
    #pragma message("#undefing macro max")
    #undef max
    #endif

    #ifdef min
    #pragma message("#undefing macro min")
    #undef min
    #endif

    // max
    // 取二者中的较大值，语义相等时返回第一个参数
    template<class T>
    const T& max(const T& lhs,const T& rhs){
        return lhs < rhs ? rhs : lhs;
    }

    // 重载版本使用函数对象comp代替比较操作
    template<class T,class Compare>
    const T& max(const T& lhs,const T& rhs,Compare comp){
        return comp(lhs,rhs) ? rhs : lhs;
    }

    // min
    // 取二者中的最小值，语义相等时返回第一个参数
    template<class T>
    const T& min(const T& lhs,const T& rhs){
        return lhs < rhs ? rhs : lhs;
    }

    // 重载版本使用函数对象comp代替比较操作
    template<class T,class Compare>
    const T& min(const T& lhs,const T& rhs,Compare comp){
        return comp(lhs,rhs) ? rhs : lhs;
    }   

    // iter_swap
    // 将两个迭代器所指对象对调
    template<class FIter1,class FIter2>
    void iter_swap(FIter1 lhs,FIter2 rhs){
        hxqstl::swap(*lhs,*rhs);
    }

    // copy
    // 把[first,last)区间的元素拷贝到[result,result + (last - first))内
    // input_iterator_tag版本
    template<class InputIter,class OutputIter>
    OutputIter unchecked_copy_cat(InputIter first,InputIter last,OutputIter result,hxqstl::input_iterator_tag){
        for(;first != last;++first,++result){
            *result = *first;
        }
        return result;
    }
    
    template<class RandomIter,class OutputIter>
    OutputIter unchecked_copy_cat(RandomIter first,RandomIter last,OutputIter result,hxqstl::random_access_iterator_tag){
        for(auto n = last - first;n > 0;--n,++first,++result){
            *result = *first;
        }
        return result;
    }

    template<class InputIter,class OutputIter>
    OutputIter unchecked_copy(InputIter first,InputIter last,OutputIter result){
        return unchecked_copy_cat(first,last,result,iterator_category(first));
    }

    // 为trivially_copy_assignable类型提供特化版本
    template<class Tp,class Up>
    typename std::enable_if<std::is_same<typename std::remove_const<Tp>::type,Up>::value &&
            std::is_trivially_copy_assignable<Up>::value,
            Up*>::type unchecked_copy(Tp* first,Tp* last,Up* result){
                const auto n = static_cast<size_t>(last - first);
                if(n != 0){
                    std::memmove(result,first,n * sizeof(Up));
                }
                return result + n;
            }

    template<class InputIter,class OutputIter>
    OutputIter copy(InputIter first,InputIter last,OutputIter result){
        return unchecked_copy(first,last,result);
    }

    template<class BidirectionalIter1,class BidirectionalIter2>
    BidirectionalIter2 unchecked_copy_backward_cat(BidirectionalIter1 first,BidirectionalIter1 last,
                                    BidirectionalIter2 result,hxqstl::bidirectional_iterator_tag){
                                        while(first != last){
                                            *--result = *--last;
                                        }
                                        return result;
                                    }

    template <class RandomIter1, class BidirectionalIter2>
    BidirectionalIter2 
    unchecked_copy_backward_cat(RandomIter1 first, RandomIter1 last,
                                BidirectionalIter2 result, hxqstl::random_access_iterator_tag)
    {
    for (auto n = last - first; n > 0; --n)
        *--result = *--last;
    return result;
    }

    template <class BidirectionalIter1, class BidirectionalIter2>
    BidirectionalIter2 
    unchecked_copy_backward(BidirectionalIter1 first, BidirectionalIter1 last,
                            BidirectionalIter2 result)
    {
    return unchecked_copy_backward_cat(first, last, result,
                                        iterator_category(first));
    }

    // 为 trivially_copy_assignable 类型提供特化版本
    template <class Tp, class Up>
    typename std::enable_if<
    std::is_same<typename std::remove_const<Tp>::type, Up>::value &&
    std::is_trivially_copy_assignable<Up>::value,
    Up*>::type
    unchecked_copy_backward(Tp* first, Tp* last, Up* result)
    {
    const auto n = static_cast<size_t>(last - first);
    if (n != 0)
    {
        result -= n;
        std::memmove(result, first, n * sizeof(Up));
    }
    return result;
    }

    template <class BidirectionalIter1, class BidirectionalIter2>
    BidirectionalIter2 
    copy_backward(BidirectionalIter1 first, BidirectionalIter1 last, BidirectionalIter2 result)
    {
    return unchecked_copy_backward(first, last, result);
    }
}