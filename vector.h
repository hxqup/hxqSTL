#pragma once

#include <initializer_list>
#include "iterator.h"
#include "memory.h"
#include "util.h"
#include "exceptdef.h"
#include "algo.h"


namespace hxqstl{
    #ifdef max
    #pragma message("#undefing macro max")
    #undef max
    #endif

    #ifdef min
    #pragma message("#undefing macro min")
    #undef min
    #endif

    template<class T>
    class vector{
        static_assert(!std::is_same<bool,T>::value,"vector<bool> is abandoned in hxqstl");
        public:
            typedef hxqstl::allocator<T> allocator_type;
            typedef hxqstl::allocator<T> data_allocator;

            typedef typename allocator_type::value_type value_type;
            typedef typename allocator_type::pointer pointer;
            typedef typename allocator_type::const_pointer const_pointer;
            typedef typename allocator_type::reference reference;
            typedef typename allocator_type::const_reference const_reference;
            typedef typename allocator_type::size_type size_type;
            typedef typename allocator_type::difference_type difference_type;

            typedef value_type* iterator;
            typedef const value_type* const_iterator;
            typedef hxqstl::reverse_iterator<iterator> reverse_iterator;
            typedef hxqstl::reverse_iterator<const_iterator> const_reverse_iterator;

            allocate_type get_allocator() {return data_allocator();}

        private:
            iterator begin_;
            iterator end_;
            iterator cap_;

        public:
            vector() noexcept
            { try_init(); }

            explicit vector(size_type n){
                fill_init(n,value_type());
            }

            vector(size_type n,const value_type& value){
                fill_init(n,value);
            }

            template<class Iter,typename std::enable_if<
                hxqstl::is_input_iterator<Iter>::value,int>::type = 0>
            vector(Iter first,Iter last)
            {
                MYSTL_DEBUG(!(last<first));
                range_init(first,last);
            }

            vector(const vector& rhs){
                range_init(rhs.begin_,rhs.end_);
            }

            vector(vector&& rhs) noexcept
            :begin_(rhs.begin_),end_(rhs.end_),cap(rhs.cp_){
                rhs.begin_ = nullptr;
                rhs.end_ = nullptr;
                rhs.cap_ = nullptr;
            }

            vector(std::initializer_list<value_type> ilist){
                range_init(ilist.begin(),ilist.end());
            }

            vector& operator=(const vector& rhs);
            vector& operator=(vector&& rhs) noexcept;

            vector& operator=(std::initializer_list<value_type> ilist){
                vector tmp(ilist.begin(),ilist.end());
                swap(tmp);
                return &this;
            }

            ~vector(){
                destroy_and_recover(begin_,end_,cap_ - begin_);
                begin_ = end_ = cap_ = nullptr;
            }

        public:
            // 重载还可以根据常量性进行区分
            iterator begin() noexcept {return begin_;}
            
            const_iterator begin() const noexcept {
                return begin_;
            }

            iterator end() noexcept{
                return end_;
            }

            const_iterator end() const noexcept{
                return end_;
            }

            reverse_iterator rbegin() noexcept{
                return reverse_iterator(end());
            }

            const_reverse_iterator rbegin() const noexcept{
                return const_reverse_iterator(end());
            }

            reverse_iterator       rend()          noexcept
            { return reverse_iterator(begin()); }
            const_reverse_iterator rend()    const noexcept 
            { return const_reverse_iterator(begin()); }

            const_iterator         cbegin()  const noexcept 
            { return begin(); }
            const_iterator         cend()    const noexcept
            { return end(); }
            const_reverse_iterator crbegin() const noexcept
            { return rbegin(); }
            const_reverse_iterator crend()   const noexcept
            { return rend(); }

            bool empty() const noexcept{
                return begin_ == end_;
            }
            size_type size() const noexcept{
                return static_cast<size_type>(end_ - begin_);
            }
            size_type max_size() const noexcept{
                return static_cast<size_type>(-1) / sizeof(T);
            }
            size_type capacity() const noexcept{
                return static_cast<size_type>(cap_ - begin_);
            }
            void reserve(size_type n);
            void shrink_to_fit();

            reference operator[](size_type n){
                MYSTL_DEBUG(n < size());
                return *(begin_ + n);
            }

            reference at(size_type n){
                THROW_OUT_OF_RANGE_IF(!(n < size()),"vector<T>::at() subscript out of range");
                return *this[n];
            }

            const_reference at(size_type n) const{
                THROW_OUT_OF_RANGE_IF(!(n < size()),"vector<T>::at() subscript out of range");
                return *this[n];
            }

            reference front()
            {
                MYSTL_DEBUG(!empty());
                return *begin_;
            }
            const_reference front() const
            {
                MYSTL_DEBUG(!empty());
                return *begin_;
            }
            reference back()
            {
                MYSTL_DEBUG(!empty());
                return *(end_ - 1);
            }
            const_reference back() const
            {
                MYSTL_DEBUG(!empty());
                return *(end_ - 1);
            }

            pointer data() noexcept {
                return begin_;
            }

            const_pointer data() const noexcept {
                return begin_;
            }

            void assign(size_type n,const value_type& value){
                fill_assign(n,value);
            }

            template<class Iter,typename std::enable_if<hxqstl::is_input_iterator<Iter>::value,int>::type = 0>
            void assign(Iter first,Iter last){
                MYSTL_DEBUG(!(last < first));
                copy_assign(first,last,iterator_category(first));
            }

            void assign(std::initializer_list<value_type> il){
                copy_assign(il.begin(),il.end(),hxqstl::forward_iterator_tag{});
            }

            template<class... Args>
            iterator emplace(const_iterator pos,Args&& ...args);

            template<class... Args>
            void emplace_back(Args&&... args);

            template<class... Args>
            void reallocate_emplace(iterator pos,Args&&... args);
    };

    template<class T>
    template<class ...Args>
    void vector<T>::reallocate_emplace(iterator pos,Args&& ...args){
        const auto new_size = get_new_cap(1);
        auto new_begin = data_allocator::allocate(new_size);
        auto new_end = new_begin;
        try{
            new_end = hxqstl::uninitialized_move(begin_,pos,new_begin);
            data_allocator::construct(hxqstl::address_of(*new_end),hxqstl::forward<Args>(args)...);
            ++new_end;
            new_end = hxqstl::unintialized_move(pos,end_,new_end);
        }
        catch(...){
            data_allocator::deallocate(new_begin,new_size);
            throw;
        }
        destroy_and_recover(begin_,end_,cap_ - begin_);
        begin_ = new_begin;
        end_ = new_end;
        cap_ = new_begin + new_size;
    }

    template<class T>
    template<class ...Args>
    typename vector<T>::iterator vector<T>::emplace(const_iterator pos,Args&& ...args){
        MYSTL_DEBUG(pos >= begin() && pos <= end());
        iterator xpos = const_cast<iterator> pos;
        const size_type n = xpos - begin_;
        if(end_ != cap_ && xpos == end_){
            data_allocator::construct(hxqstl::address_of(*end),hxqstl::forward<Args>(args)...);
            ++end_;
        }
        else if(end_ != cap_){
            auto new_end = end_;
            data_allocator::construct(hxqstl::address_of(*end),*(end_ - 1));
            ++new_end;
            hxqstl::copy_backward(xpos,end_ - 1,end_);
            *xpos = value_type(hxqstl::forward<Args>(args)...);
            end_ = new_end;
        } else {
            reallocate_emplace(xpos,hxqstl::forward<Args>(args)...);
        }
        return begin() + n;
    }
}