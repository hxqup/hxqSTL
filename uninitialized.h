#pragma once

#include "algobase.h"
#include "construct.h"
#include "iterator.h"
#include "typetraits.h"
#include "util.h"

namespace hxqstl{
    // uninitialized_copy
    // 把[first,last)上的内容复制到以result起始的位置，返回复制结束的位置
    template<class InputIter,class ForwardIter>
    ForwardIter unchecked_uninit_copy(InputIter first,InputIter last,ForwardIter,std::true_type){
        return hxqstl::copy(first,last,result);
    }

    template <class InputIter,class ForwardIter>
    ForwardIter unchecked_uninit_copy(InputIter first,InputIter last,ForwardIter result,std::false_type){
         auto cur = result;
         try{
            for(;first != last;++first,++cur){
                hxqstl::construct(&*cur,*first);
            }
         }
         catch(...){
            for(;result != cur;--cur){
                for(;result != cur;--cur){
                    hxqstl::destroy(&*cur);
                }
            }
         }
         return cur;
    }

    // uninitialized_copy_n
    // 把[first,first + n)上的内容复制到以result为起始处的空间，返回复制结束的位置
    template<class InputIter,class Size,class ForwardIter>
    ForwardIter unchecked_uninit_copy_n(InputIter first,Size n,ForwardIter result,std::true_type){
        return hxqstl::copy_n(first,n,result).second;
    }

    template<class InputIter,class Size,class ForwardIter>
    ForwardIter unchecked_uninit_copy_n(InputIter first,Size n,ForwardIter result,std::false_type){
        auto cur = result;
        try
        {
            // &*是为了迭代器所指向对象的地址
            for(;n > 0;--n,++cur,++first){
                hxqstl::construct(&*cur,*first);
            }
        }
        catch(...)
        {
            for(;result != cur;--cur){
                hxqstl::destroy(&*cur);
            }
        }
        return cur;
    }

    template<class InputIter,class Size,class ForwardIter>
    ForwardIter uninitialized_copy_n(InputIter first,Size n,ForwardIter result){
        // 用初始化列表的方式 创建临时对象
        return hxqstl::unchecked_uninit_copy_n(first,n,result,
                                                std::is_trivially_copy_assignable<
                                                typename iterator_traits<InputIter>::
                                                value_type>{});
    }

    // uninitialized_fill
    // 在[first,last)区间内填充元素值
    template<class ForwardIter,class T>
    void unchecked_uninit_fill(ForwardIter first,ForwardIter last,const T& value,std::true_type){
        hxqstl::fill(first,last,value);
    }

    template<class ForwardIter,class T>
    void unchecked_uninit_fill(ForwardIter first,ForwardIter last,const T& value,std::false_type){
        auto cur = first;
        try
        {
            for(;cur != last;++cur){
                hxqstl::construct(&*cur,value);
            }
        }
        catch(...)
        {
            for(;first != cur;++first){
                hxqstl::destroy(&*first);
            }
        }
    }

    template<class ForwardIter,class T>
    void uninitialized_fill(ForwardIter first,ForwardIter last,const T& value){
        hxqstl::unchecked_uninit_fill(first,last,value,
                                        std::is_trivially_copy_assignable<
                                        typename iterator_traits<ForwardIter>::value_type>{});
    }

    // uninitialized_fill_n
    // 从first开始，填充n个元素值，返回结束填充的位置
    template <class ForwardIter, class Size, class T>
    ForwardIter 
    unchecked_uninit_fill_n(ForwardIter first, Size n, const T& value, std::true_type)
    {
    return hxqstl::fill_n(first, n, value);
    }

    template <class ForwardIter, class Size, class T>
    ForwardIter unchecked_uninit_fill_n(ForwardIter first, Size n, const T& value, std::false_type)
    {
        auto cur = first;
        try
        {
            for (; n > 0; --n, ++cur)
            {
                hxqstl::construct(&*cur, value);
            }
        }
        catch (...)
        {
            for (; first != cur; ++first)
                hxqstl::destroy(&*first);
        }
        return cur;
    }

    template<class ForwardIter,class Size,class T>
    ForwardIter uninitialized_fill_n(ForwardIter first,Size n,const T& value){
        return hxqstl::unchecked_uninit_fill_n(first,n,value,
                                                std::is_trivially_copy_assignable<
                                                typename iterator_traits<ForwardIter>::value_type>{});
    }

    // uninitialized_move把[first,last)上的内容移动到以result为起始处的空间，返回结束的位置
    template<class InputIter,class ForwardIter>
    ForwardIter unchecked_uninit_move(InputIter first,InputIter last,ForwardIter result,std::true_type){
        return hxqstl::move(first,last,result);
    }
}