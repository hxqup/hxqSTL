#pragma once

#include <cstddef>
#include "typetraits.h"
namespace hxqstl{
    // 五种迭代器类型
    struct input_iterator_tag {};
    struct output_iterator_tag {};
    struct forward_iterator_tag : public input_iterator_tag {};
    struct bidirectional_iterator_tag : public forward_iterator_tag {};
    struct random_access_iterator_tag : public bidirectional_iterator_tag {};

    template<class Category,class T,class Distance = ptrdiff_t,class Pointer = T*,class Reference = T&>
    struct iterator{
        typedef Category iterator_category;
        typedef T value_type;
        typedef Pointer pointer;
        typedef Reference reference;
        typedef Distance difference_type;
    };

    // 用于在编译期间判断一个类型是否具有iterator_category成员类型
    template<class T>
    struct has_iterator_cat{
        private:
        struct two {char a;char b};
        // 静态函数模板
        template <class U> static two test(...);
        template <class U> static char test(typename U::iterator_category* = 0);
        public:
        // sizeof的结果是编译时常量，并不会实际运行test函数
        static const bool value = sizeof(test<T>(0)) == sizeof(char);
    };

    template<class Iterator,bool>
    struct iterator_traits_impl {};

    template<class Iterator>
    struct iterator_traits_impl<Iterator,true>
    {
        typedef typename Iterator::iterator_category iterator_category;
        typedef typename Iterator::value_type value_type;
        typedef typename Iterator::pointer pointer;
        typedef typename Iterator::reference reference;
        typedef typename Iterator::difference_type difference_Type;
    };

    template<class Iterator,bool>
    struct iterator_traits_helper {};

    // std::is_convertible用于判断模板两个类型之间是否可以相互转换
    template<class Iterator>
    struct iterator_traits_helper<Iterator,true>
    :public iterator_traits_impl<Iterator,
    std::is_convertible<typename Iterator::iterator_category,input_iterator_tag>::value ||
    std::is_convertible<typename Iterator::iterator_category,output_iterator_tag>::value>{

    };

    // 萃取迭代器的特性
    template<class Iterator>
    struct iterator_traits : public iterator_traits_helper<Iterator,has_iterator_cat<Iterator>::value>{};

    // 针对原生指针的偏特化版本
    template<class T>
    struct iterator_traits<T*>{
        typedef random_access_iterator_tag iterator_category;
        typedef T value_type;
        typedef T* pointer;
        typedef T& reference;
        typedef ptrdiff_t diffrence_type;
    };

    template<class T,class U,bool = has_iterator_cat<iterator_traits<T>>::value>
    struct has_iterator_cat_of : public m_bool_constant<std::is_convertible<typename iterator_traits<T>::iterator_category,U>::value>
    {

    };

    template<class T,class U>
    struct has_iterator_cat_of<T,U,false> : public m_false_type{};

    template<class Iter>
    struct is_input_iterator : public has_iterator_cat_of<Iter,input_iterator_tag> {};

    template<class Iter>
    struct is_output_iterator : public has_iterator_cat_of<Iter,output_iterator_tag> {};

    template<class Iter>
    struct is_forward_iterator : public has_iterator_cat_of<Iter,forward_iterator_tag> {};

    template<class Iter>
    struct is_random_access_iterator : public has_iterator_cat_of<Iter,random_access_iterator_tag> {};

    template<class Iterator>
    struct is_iterator : public m_bool_constant<is_input_iterator<Iterator>::value || is_output_iterator<Iterator>::value{

    };

    template<class Iterator>
    typename iterator_traits<Iterator>::iterator_category
    iterator_category(const Iterator&){
        typedef typename iterator_traits<Iterator>::iterator_category Category;
        return Category();
    }

    template<class Iterator>
    typename iterator_traits<Iterator>::difference_type* difference_type(const Iterator&){
        return static_cast<typename iterator_traits<Iterator>::difference_type*>(0);
    }

    template<class Iterator>
    typename iterator_traits<Iterator>::value_type* value_type(const Iterator&){
        return static_cast<typename iterator_traits<Iterator>::value_type*>(0);
    }

    // 计算迭代器间的距离
    template<class InputIterator>
    typename iterator_traits<InputIterator>::difference_type
    distance_dispatch(InputIterator first,InputIterator last,input_iterator_tag){    
        typename iterator_traits<InputIterator>::difference_type n = 0;
        while(first != last){
            ++first;
            ++n;
        }
        return n;
    }

    template<class RandomIter>
    typename iterator_traits<RandomIter>::difference_type
    distance_dispatch(RandomIter first,RandomIter last,random_access_iterator_tag){
        return last - first;
    }

    template<class InputIterator>
    typename iterator_traits<InputIterator>::difference_type
    distance(InputIterator first,InputIterator last){
        return distance_dispatch(first,last,iterator_category(first));
    }

    template<class InputIterator,class Distance>
    void advance_dispatch(InputIterator& i,Distance n,input_iterator_tag){
        while(n--){
            ++i;
        }
    }
    
    template<class BidirectionalIterator,class Distance>
    void advance_dispatch(BidirectionalIterator& i,Distance n,bidirectional_iterator_tag){
        if(n >= 0){
            while(n--) ++i;
        } else{
            while(n++) --i;
        }
    }

    template<class RandomIter,class Distance>
    void advance_dispatch(RandomIter& i,Distance n,random_access_iterator_tag){
        i += n;
    }

    template<class InputIterator,class Distance>
    void advance(InputIterator& i,Distance n){
        advance_dispatch(i,n,iterator_category(i));
    }

    template<class Iterator>
    class reverse_iterator{
        private:
            Iterator current;
        public:
            typedef typename iterator_traits<Iterator>::iterator_category iterator_category;
            typedef typename iterator_traits<Iterator>::value_type value_type;
            typedef typename iterator_traits<Iterator>::difference_type difference_type;
            typedef typename iterator_traits<Iterator>::pointer pointer;
            typedef typename iterator_traits<Iterator>::reference referencel

            typedef Iterator iterator_type;
            typedef reverse_iterator<Iterator> self;

        public:
            reverse_iterator() {}
            explicit reverse_iterator(iterator_type i):current(i){}
            reverse_iterator(const self& rhs):current(rhs.current){}
        public:
            iterator_type base() const{
                return current;
            }

            reference operator*() const{
                auto tmp = current;
                return *--tmp;
            }

            pointer operator->() const {
                return &(operator*());
            }

            self& operator++(){
                --current;
                return *this;
            }

            self operator++(int){
                self tmp = *this;
                --current;
                return tmp;
            }

            self& operator--(){
                ++current;
                return *this;
            }

            self operator--(int){
                self tmp = *this;
                ++current;
                return tmp;
            }

            self& operator+=(difference_type n){
                current -= n;
                return *this;
            }

            self operator+(difference_type n) const {
                return self(current - n);
            }

            self operator-=(difference_type n) const {
                current += n;
                return *this;
            }

            self operator-(difference_type n) const{
                return self(current + n);
            }

            reference operator[](difference_type n) const {
                return *(*this + n);
            }
    };

    template<class Iterator>
    bool operator==(const reverse_iterator<Iterator>& lhs,
                    const reverse_iterator<Iterator>& rhs)
    {
        return lhs.base() == rhs.base();
    }
            
    template<class Iterator>
    bool operator<(const reverse_iterator<Iterator>& lhs,
                    const reverse_iterator<Iterator>& rhs)
    {
        return rhs.base() < lhs.base();
    }

    template<class Iterator>
    bool operator!=(const reverse_iterator<Iterator>& lhs,
                    const reverse_iterator<Iterator>& rhs){
                        return !(lhs == rhs);
                    }

    template<class Iterator>
    bool operator>(const reverse_iterator<Iterator>& lhs,
                    const reverse_iterator<Iterator>& rhs){
                        return rhs < lhs;
                    }
            
    template <class Iterator>
    bool operator<=(const reverse_iterator<Iterator>& lhs,
                    const reverse_iterator<Iterator>& rhs)
    {
    return !(rhs < lhs);
    }

    template <class Iterator>
    bool operator>=(const reverse_iterator<Iterator>& lhs,
                    const reverse_iterator<Iterator>& rhs)
    {
    return !(lhs < rhs);
    }
}
