/*
 *
 * Copyright (c) 1994
 * Hewlett-Packard Company
 *
 * Copyright (c) 1996,1997
 * Silicon Graphics Computer Systems, Inc.
 *
 * Copyright (c) 1997
 * Moscow Center for SPARC Technology
 *
 * Copyright (c) 1999 
 * Boris Fomitchev
 *
 * This material is provided "as is", with absolutely no warranty expressed
 * or implied. Any use is at your own risk.
 *
 * Permission to use or copy this software for any purpose is hereby granted 
 * without fee, provided the above notices are retained on all copies.
 * Permission to modify the code and to distribute modified code is granted,
 * provided the above notices are retained, and a notice that the code was
 * modified is included with the above copyright notice.
 *
 */

/* NOTE: This is an internal header file, included by other STL headers.
 *   You should not attempt to use it directly.
 */

#ifndef _STLP_INTERNAL_LIST_IMPL_H
#define _STLP_INTERNAL_LIST_IMPL_H

# ifndef _STLP_INTERNAL_ALGOBASE_H
#  include <stl/_algobase.h>
# endif

# ifndef _STLP_INTERNAL_ALLOC_H
#  include <stl/_alloc.h>
# endif

# ifndef _STLP_INTERNAL_ITERATOR_H
#  include <stl/_iterator.h>
# endif

# ifndef _STLP_INTERNAL_CONSTRUCT_H
#  include <stl/_construct.h>
# endif

# ifndef _STLP_INTERNAL_FUNCTION_BASE_H
#  include <stl/_function_base.h>
# endif

_STLP_BEGIN_NAMESPACE

# undef list
# define  list  __WORKAROUND_DBG_RENAME(list)

struct _List_node_base {
  _List_node_base* _M_next;
  _List_node_base* _M_prev;
};

template <class _Dummy>
class _List_global {
public:
  typedef _List_node_base _Node_base;
  static void  _STLP_CALL _Transfer(_Node_base* __position,
                                    _Node_base* __first, _Node_base* __last);
};

# if defined (_STLP_USE_TEMPLATE_EXPORT)
_STLP_EXPORT_TEMPLATE_CLASS _List_global<bool>;
# endif
typedef _List_global<bool> _List_global_inst;

template <class _Tp>
class _List_node : public _List_node_base {
public:
  _Tp _M_data;
  __TRIVIAL_STUFF(_List_node)

#ifdef __DMC__
  // for some reason, Digital Mars C++ needs a constructor...
 private:
  _List_node();
#endif
};

struct _List_iterator_base {
  typedef size_t                     size_type;
  typedef ptrdiff_t                  difference_type;
  typedef bidirectional_iterator_tag iterator_category;

  _List_node_base* _M_node;

  _List_iterator_base(_List_node_base* __x) : _M_node(__x) {}

  void _M_incr() { _M_node = _M_node->_M_next; }
  void _M_decr() { _M_node = _M_node->_M_prev; }
};


template<class _Tp, class _Traits>
struct _List_iterator : public _List_iterator_base {
  typedef _Tp value_type;
  typedef typename _Traits::pointer    pointer;
  typedef typename _Traits::reference  reference;

  typedef _List_iterator<_Tp, _Traits>         _Self;
  typedef typename _Traits::_NonConstTraits    _NonConstTraits;
  typedef _List_iterator<_Tp, _NonConstTraits> iterator;
  typedef typename _Traits::_ConstTraits       _ConstTraits;
  typedef _List_iterator<_Tp, _ConstTraits>    const_iterator;

  typedef bidirectional_iterator_tag iterator_category;
  typedef _List_node<_Tp> _Node;
  typedef size_t size_type;
  typedef ptrdiff_t difference_type;

  _List_iterator(_List_node_base* __x) : _List_iterator_base(__x) {}
  _List_iterator() : _List_iterator_base(0) {}
  //copy constructor for iterator and constructor from iterator for const_iterator
  _List_iterator(const iterator& __x) :  _List_iterator_base(__x._M_node) {}

  reference operator*() const { return __STATIC_CAST(_Node*, this->_M_node)->_M_data; }

  _STLP_DEFINE_ARROW_OPERATOR

  _Self& operator++() {
    this->_M_incr();
    return *this;
  }
  _Self operator++(int) {
    _Self __tmp = *this;
    this->_M_incr();
    return __tmp;
  }
  _Self& operator--() {
    this->_M_decr();
    return *this;
  }
  _Self operator--(int) {
    _Self __tmp = *this;
    this->_M_decr();
    return __tmp;
  }
  bool operator==(const_iterator __y ) const {
    return this->_M_node == __y._M_node;
  }
  bool operator!=(const_iterator __y ) const {
    return this->_M_node != __y._M_node;
  }
};

#ifdef _STLP_CLASS_PARTIAL_SPECIALIZATION
template <class _Tp, class _Traits>
struct __type_traits<_List_iterator<_Tp, _Traits> > {
  typedef __false_type   has_trivial_default_constructor;
  typedef __true_type    has_trivial_copy_constructor;
  typedef __true_type    has_trivial_assignment_operator;
  typedef __true_type    has_trivial_destructor;
  typedef __false_type   is_POD_type;
};
#endif /* _STLP_CLASS_PARTIAL_SPECIALIZATION */

#ifdef _STLP_USE_OLD_HP_ITERATOR_QUERIES
template <class _Tp, class _Traits>
inline _Tp* value_type(const _List_iterator<_Tp, _Traits>&) { return 0; }
inline bidirectional_iterator_tag iterator_category(const _List_iterator_base&) { return bidirectional_iterator_tag();}
inline ptrdiff_t* distance_type(const _List_iterator_base&) { return 0; }
#endif

// Base class that encapsulates details of allocators and helps
// to simplify EH

template <class _Tp, class _Alloc>
class _List_base {
protected:
  _STLP_FORCE_ALLOCATORS(_Tp, _Alloc)
  typedef _List_node_base _Node_base;
  typedef _List_node<_Tp> _Node;
  typedef _List_base<_Tp, _Alloc> _Self;
  typedef typename _Alloc_traits<_Node, _Alloc>::allocator_type _Node_allocator_type;
public:
  typedef _STLP_alloc_proxy<_Node_base, _Node, _Node_allocator_type> _AllocProxy;
  typedef typename _Alloc_traits<_Tp, _Alloc>::allocator_type allocator_type;

  allocator_type get_allocator() const {
    return _STLP_CONVERT_ALLOCATOR((const _Node_allocator_type&)_M_node, _Tp);
  }

  _List_base(const allocator_type& __a) : _M_node(_STLP_CONVERT_ALLOCATOR(__a, _Node), _Node_base() ) {
    _M_empty_initialize();
  }
  _List_base(__move_source<_Self> src) :
    _M_node(__move_source<_AllocProxy>(src.get()._M_node)) {
    if (src.get().empty())
      //We force this to empty.
      _M_empty_initialize();
    else {
      src.get()._M_empty_initialize();
      _M_node._M_data._M_prev->_M_next = _M_node._M_data._M_next->_M_prev = &_M_node._M_data;
    }
  }
    
  ~_List_base() {
    clear();
  }

  void clear();
  bool empty() const { return _M_node._M_data._M_next == &_M_node._M_data; }

  void _M_empty_initialize() {
    _M_node._M_data._M_next = &_M_node._M_data;
    _M_node._M_data._M_prev = _M_node._M_data._M_next;
  }

public:
  _AllocProxy _M_node;
};

# ifndef _STLP_DONT_USE_PTR_SPECIALIZATIONS
#  define _LIST_IMPL _List_impl
# else
#  define _LIST_IMPL list
# endif /* _STLP_DONT_USE_PTR_SPECIALIZATIONS */

template <class _Tp, _STLP_DEFAULT_ALLOCATOR_SELECT(_Tp) >
class _LIST_IMPL;

// helper functions to reduce code duplication
template <class _Tp, class _Alloc, class _Predicate> 
void _S_remove_if(_LIST_IMPL<_Tp, _Alloc>& __that, _Predicate __pred);

template <class _Tp, class _Alloc, class _BinaryPredicate>
void _S_unique(_LIST_IMPL<_Tp, _Alloc>& __that, _BinaryPredicate __binary_pred);

template <class _Tp, class _Alloc, class _StrictWeakOrdering>
void _S_merge(_LIST_IMPL<_Tp, _Alloc>& __that, _LIST_IMPL<_Tp, _Alloc>& __x,
              _StrictWeakOrdering __comp);

template <class _Tp, class _Alloc, class _StrictWeakOrdering>
void _S_sort(_LIST_IMPL<_Tp, _Alloc>& __that, _StrictWeakOrdering __comp);

template <class _Tp, class _Alloc>
class _LIST_IMPL : public _List_base<_Tp, _Alloc>
#if !defined (_STLP_DEBUG) && defined (_STLP_USE_PARTIAL_SPEC_WORKAROUND)
                   , public __stlport_class<_LIST_IMPL<_Tp, _Alloc> >
#endif
{
  typedef _List_base<_Tp, _Alloc> _Base;
  typedef _LIST_IMPL<_Tp, _Alloc> _Self;
  typedef _List_node<_Tp> _Node;
  typedef _List_node_base _Node_base;
public:      
  typedef _Tp value_type;
  typedef value_type* pointer;
  typedef const value_type* const_pointer;
  typedef value_type& reference;
  typedef const value_type& const_reference;
  typedef size_t size_type;
  typedef ptrdiff_t difference_type;
  _STLP_FORCE_ALLOCATORS(_Tp, _Alloc)
  typedef typename _Base::allocator_type allocator_type;
  typedef bidirectional_iterator_tag _Iterator_category;

public:
  typedef _List_iterator<_Tp, _Nonconst_traits<_Tp> > iterator;
  typedef _List_iterator<_Tp, _Const_traits<_Tp> >    const_iterator;
  _STLP_DECLARE_BIDIRECTIONAL_REVERSE_ITERATORS;

protected:
#if !defined(_STLP_DONT_SUP_DFLT_PARAM)
  _Node_base* _M_create_node(const_reference __x = value_type()) {
#else
  _Node_base* _M_create_node(const_reference __x) {
#endif /*!_STLP_DONT_SUP_DFLT_PARAM*/
    _Node* __p = this->_M_node.allocate(1);
    _STLP_TRY {
      _Copy_Construct(&__p->_M_data, __x);
    }
    _STLP_UNWIND(this->_M_node.deallocate(__p, 1))
    return __p;
  }

#if defined(_STLP_DONT_SUP_DFLT_PARAM)
  _Node_base* _M_create_node() {
    _Node* __p = this->_M_node.allocate(1);
    _STLP_TRY {
      _STLP_STD::_Construct(&__p->_M_data);
    }
    _STLP_UNWIND(this->_M_node.deallocate(__p, 1))
    return __p;
  }
#endif /*_STLP_DONT_SUP_DFLT_PARAM*/

public:
# if !(defined(__MRC__)||(defined(__SC__) && !defined(__DMC__)))
  explicit
# endif
  _LIST_IMPL(const allocator_type& __a = allocator_type()) :
    _List_base<_Tp, _Alloc>(__a) {}

  iterator begin()                      { return iterator(this->_M_node._M_data._M_next); }
  const_iterator begin() const          { return const_iterator(this->_M_node._M_data._M_next); }

  iterator end()                        { return iterator(&this->_M_node._M_data); }
  const_iterator end() const            { return const_iterator(__CONST_CAST(_Node_base*, &this->_M_node._M_data)); }

  reverse_iterator rbegin()             { return reverse_iterator(end()); }
  const_reverse_iterator rbegin() const { return const_reverse_iterator(end()); }

  reverse_iterator rend()               { return reverse_iterator(begin()); }
  const_reverse_iterator rend() const   { return const_reverse_iterator(begin()); }

  size_type size() const {
    size_type __result = distance(begin(), end());
    return __result;
  }
  size_type max_size() const { return size_type(-1); }

  reference front()             { return *begin(); }
  const_reference front() const { return *begin(); }
  reference back()              { return *(--end()); }
  const_reference back() const  { return *(--end()); }

  void swap(_Self& __x) {
    if (__x.empty()) {
      __x.splice(__x.begin(), *this);
    }
    else if (this->empty()) {
      this->splice(this->begin(), __x);
    } else {
      _STLP_STD::swap(this->_M_node, __x._M_node);
      _STLP_STD::swap(this->_M_node._M_data._M_prev->_M_next, __x._M_node._M_data._M_prev->_M_next);
    }
  }

#if !defined(_STLP_DONT_SUP_DFLT_PARAM) && !defined(_STLP_NO_ANACHRONISMS)
  iterator insert(iterator __position, const_reference __x = value_type()) {
#else
  iterator insert(iterator __position, const_reference __x) {
#endif /*!_STLP_DONT_SUP_DFLT_PARAM && !_STLP_NO_ANACHRONISMS*/
    _Node_base* __tmp = _M_create_node(__x);
    _Node_base* __n = __position._M_node;
    _Node_base* __p = __n->_M_prev;
    __tmp->_M_next = __n;
    __tmp->_M_prev = __p;
    __p->_M_next = __tmp;
    __n->_M_prev = __tmp;
    return __tmp;
  }

private:
#ifdef _STLP_MEMBER_TEMPLATES
  template <class _InputIterator>
  void _M_insert(iterator __pos, _InputIterator __first, _InputIterator __last) {
    typedef typename _Is_integer<_InputIterator>::_Integral _Integral;
    _M_insert_dispatch(__pos, __first, __last, _Integral());
  }
  
  // Check whether it's an integral type.  If so, it's not an iterator.
  template<class _Integer>
  void _M_insert_dispatch(iterator __pos, _Integer __n, _Integer __x,
                          const __true_type& /*_IsIntegral*/) {
    _M_fill_insert(__pos, __n, __x);
  }
  template <class _InputIter>
  void _M_insert_dispatch(iterator __position,
                          _InputIter __first, _InputIter __last,
                          const __false_type& /*_IsIntegral*/) {
#else /* _STLP_MEMBER_TEMPLATES */
  void _M_insert(iterator __position, const value_type* __first, const value_type* __last) {
    for (; __first != __last; ++__first)
      insert(__position, *__first);
  }
  void _M_insert(iterator __position, const_iterator __first, const_iterator __last) {
#endif /* _STLP_MEMBER_TEMPLATES */
    //We use a temporary list to avoid the auto reference troubles (infinite loop)
    for (; __first != __last; ++__first)
      insert(__position, *__first);
  }
  
public:
#ifdef _STLP_MEMBER_TEMPLATES
  template <class _InputIterator>
  void insert(iterator __pos, _InputIterator __first, _InputIterator __last) {
    typedef typename _Is_integer<_InputIterator>::_Integral _Integral;
    _M_splice_insert_dispatch(__pos, __first, __last, _Integral());
  }
  
private:
  // Check whether it's an integral type.  If so, it's not an iterator.
  template<class _Integer>
  void _M_splice_insert_dispatch(iterator __pos, _Integer __n, _Integer __x,
                          const __true_type& /*_IsIntegral*/) {
    _M_fill_insert(__pos, __n, __x);
  }
  template <class _InputIter>
  void _M_splice_insert_dispatch(iterator __position,
                          _InputIter __first, _InputIter __last,
                          const __false_type& /*_IsIntegral*/) {
#else /* _STLP_MEMBER_TEMPLATES */
  void insert(iterator __position, const value_type* __first, const value_type* __last) {
    _Self __tmp(__first, __last, this->get_allocator());
    splice(__position, __tmp);
  }
  void insert(iterator __position, const_iterator __first, const_iterator __last) {
#endif /* _STLP_MEMBER_TEMPLATES */
    //We use a temporary list to avoid the auto reference troubles (infinite loop)
    _Self __tmp(__first, __last, this->get_allocator());
    splice(__position, __tmp);
  }
  
public:
  void insert(iterator __pos, size_type __n, const_reference __x) 
  { _M_fill_insert(__pos, __n, __x); }
  
private:
  void _M_fill_insert(iterator __pos, size_type __n, const_reference __x) {
    for ( ; __n > 0; --__n)
      insert(__pos, __x);
  }
  
public:  
  void push_front(const_reference __x) { insert(begin(), __x); }
  void push_back (const_reference __x) { insert(end(), __x); }

#if defined(_STLP_DONT_SUP_DFLT_PARAM) && !defined(_STLP_NO_ANACHRONISMS)
  iterator insert(iterator __position) { return insert(__position, _STLP_DEFAULT_CONSTRUCTED(value_type)); }
  void push_front() {insert(begin());}
  void push_back() {insert(end());}
# endif /*_STLP_DONT_SUP_DFLT_PARAM && !_STLP_NO_ANACHRONISMS*/

  iterator erase(iterator __position) {
    _Node_base* __next_node = __position._M_node->_M_next;
    _Node_base* __prev_node = __position._M_node->_M_prev;
    _Node* __n = __STATIC_CAST(_Node*, __position._M_node);
    __prev_node->_M_next = __next_node;
    __next_node->_M_prev = __prev_node;
    _STLP_STD::_Destroy(&__n->_M_data);
    this->_M_node.deallocate(__n, 1);
    return iterator(__next_node);
  }
  
  iterator erase(iterator __first, iterator __last) {
    while (__first != __last)
      erase(__first++);
    return __last;
  }

#if !defined(_STLP_DONT_SUP_DFLT_PARAM)
  void resize(size_type __new_size, const_reference __x = value_type());
#else
  void resize(size_type __new_size, const_reference __x);
  void resize(size_type __new_size) { this->resize(__new_size, _STLP_DEFAULT_CONSTRUCTED(value_type)); }
#endif /*!_STLP_DONT_SUP_DFLT_PARAM*/

  void pop_front() { erase(begin()); }
  void pop_back() { 
    iterator __tmp = end();
    erase(--__tmp);
  }
#if !defined(_STLP_DONT_SUP_DFLT_PARAM)
  explicit _LIST_IMPL(size_type __n, const_reference __val = value_type(),
#else
  _LIST_IMPL(size_type __n, const_reference __val,
#endif /*_STLP_DONT_SUP_DFLT_PARAM*/
       const allocator_type& __a = allocator_type())
    : _List_base<_Tp, _Alloc>(__a)
    { this->insert(begin(), __n, __val); }

#if defined(_STLP_DONT_SUP_DFLT_PARAM)
  explicit _LIST_IMPL(size_type __n)
    : _List_base<_Tp, _Alloc>(allocator_type())
    { this->insert(begin(), __n, _STLP_DEFAULT_CONSTRUCTED(value_type)); }
#endif /*_STLP_DONT_SUP_DFLT_PARAM*/

#ifdef _STLP_MEMBER_TEMPLATES
  // We don't need any dispatching tricks here, because insert does all of
  // that anyway.
# ifdef _STLP_NEEDS_EXTRA_TEMPLATE_CONSTRUCTORS
  template <class _InputIterator>
  _LIST_IMPL(_InputIterator __first, _InputIterator __last)
    : _List_base<_Tp, _Alloc>(allocator_type())
  { _M_insert(begin(), __first, __last); }
# endif
  template <class _InputIterator>
  _LIST_IMPL(_InputIterator __first, _InputIterator __last,
       const allocator_type& __a _STLP_ALLOCATOR_TYPE_DFL)
    : _List_base<_Tp, _Alloc>(__a)
  { _M_insert(begin(), __first, __last); }
  
#else /* _STLP_MEMBER_TEMPLATES */

  _LIST_IMPL(const value_type* __first, const value_type* __last,
       const allocator_type& __a = allocator_type())
    : _List_base<_Tp, _Alloc>(__a)
    { _M_insert(begin(), __first, __last); }
  _LIST_IMPL(const_iterator __first, const_iterator __last,
       const allocator_type& __a = allocator_type())
    : _List_base<_Tp, _Alloc>(__a)
    { _M_insert(begin(), __first, __last); }

#endif /* _STLP_MEMBER_TEMPLATES */
  _LIST_IMPL(const _Self& __x) : _List_base<_Tp, _Alloc>(__x.get_allocator())
    { _M_insert(begin(), __x.begin(), __x.end()); }

  _LIST_IMPL(__move_source<_Self> src)
    : _List_base<_Tp, _Alloc>(__move_source<_Base>(src.get())) {}

  ~_LIST_IMPL() { }

  _Self& operator=(const _Self& __x);

public:
  // assign(), a generalized assignment member function.  Two
  // versions: one that takes a count, and one that takes a range.
  // The range version is a member template, so we dispatch on whether
  // or not the type is an integer.

  void assign(size_type __n, const_reference __val) { _M_fill_assign(__n, __val); }

  void _M_fill_assign(size_type __n, const_reference __val);

#ifdef _STLP_MEMBER_TEMPLATES
  template <class _InputIterator>
  void assign(_InputIterator __first, _InputIterator __last) {
    typedef typename _Is_integer<_InputIterator>::_Integral _Integral;
    _M_assign_dispatch(__first, __last, _Integral());
  }

  template <class _Integer>
  void _M_assign_dispatch(_Integer __n, _Integer __val,
                          const __true_type& /*_IsIntegral*/) {
    _M_fill_assign(__n, __val);
  }

  template <class _InputIterator>
  void _M_assign_dispatch(_InputIterator __first2, _InputIterator __last2,
                          const __false_type& /*_IsIntegral*/) {
#else
  void assign(const value_type *__first2, const value_type *__last2) {
    iterator __first1 = begin();
    iterator __last1 = end();
    for ( ; __first1 != __last1 && __first2 != __last2; ++__first1, ++__first2)
      *__first1 = *__first2;
    if (__first2 == __last2)
      erase(__first1, __last1);
    else
      insert(__last1, __first2, __last2);
  }
  void assign(const_iterator __first2, const_iterator __last2) {
#endif /* _STLP_MEMBER_TEMPLATES */
    iterator __first1 = begin();
    iterator __last1 = end();
    for ( ; __first1 != __last1 && __first2 != __last2; ++__first1, ++__first2)
      *__first1 = *__first2;
    if (__first2 == __last2)
      erase(__first1, __last1);
    else
      insert(__last1, __first2, __last2);
  }

public:
  void splice(iterator __position, _Self& __x) {
    if (!__x.empty())
      _List_global_inst::_Transfer(__position._M_node, __x.begin()._M_node, __x.end()._M_node);
  }
  void splice(iterator __position, _Self&, iterator __i) {
    iterator __j = __i;
    ++__j;
    if (__position == __i || __position == __j) return;
    _List_global_inst::_Transfer(__position._M_node, __i._M_node, __j._M_node);
  }
  void splice(iterator __position, _Self&, iterator __first, iterator __last) {
    if (__first != __last) 
      _List_global_inst::_Transfer(__position._M_node, __first._M_node, __last._M_node);
  }

  void remove(const_reference __val) {
    iterator __first = begin();
    iterator __last = end();
    while (__first != __last) {
      iterator __next = __first;
      ++__next;
      if (__val == *__first) erase(__first);
      __first = __next;
    }
  }
  
  void unique() {
    _S_unique(*this, equal_to<value_type>());
  }
  
  void merge(_Self& __x) {
    _S_merge(*this, __x, less<value_type>());
  }

  void reverse() {
    _Node_base* __p = &this->_M_node._M_data;
    _Node_base* __tmp = __p;
    do {
      _STLP_STD::swap(__tmp->_M_next, __tmp->_M_prev);
      __tmp = __tmp->_M_prev;     // Old next node is now prev.
    } while (__tmp != __p);
  }    
  
  void sort() {
    _S_sort(*this, less<value_type>());
  }

#ifdef _STLP_MEMBER_TEMPLATES
  template <class _Predicate> 
  void remove_if(_Predicate __pred)  {
    _S_remove_if(*this, __pred);
  }
  template <class _BinaryPredicate>
  void unique(_BinaryPredicate __binary_pred) {
    _S_unique(*this, __binary_pred);
  }

  template <class _StrictWeakOrdering>
  void merge(_Self& __x,
             _StrictWeakOrdering __comp) {
    _S_merge(*this, __x, __comp);
  }

  template <class _StrictWeakOrdering>
  void sort(_StrictWeakOrdering __comp) {
    _S_sort(*this, __comp);
  }
#endif /* _STLP_MEMBER_TEMPLATES */

};

# ifndef _STLP_DONT_USE_PTR_SPECIALIZATIONS
#  include <stl/pointers/_list.h>
# endif /* _STLP_DONT_USE_PTR_SPECIALIZATIONS */


template <class _Tp, class _Alloc>
_STLP_INLINE_LOOP bool  _STLP_CALL
operator==(const list<_Tp,_Alloc>& __x, const list<_Tp,_Alloc>& __y) {
  typedef typename list<_Tp,_Alloc>::const_iterator const_iterator;
  const_iterator __end1 = __x.end();
  const_iterator __end2 = __y.end();

  const_iterator __i1 = __x.begin();
  const_iterator __i2 = __y.begin();
  while (__i1 != __end1 && __i2 != __end2 && *__i1 == *__i2) {
    ++__i1;
    ++__i2;
  }
  return __i1 == __end1 && __i2 == __end2;
}

# define _STLP_EQUAL_OPERATOR_SPECIALIZED
# define _STLP_TEMPLATE_HEADER    template <class _Tp, class _Alloc>
# define _STLP_TEMPLATE_CONTAINER list<_Tp, _Alloc>
# include <stl/_relops_cont.h>
# undef _STLP_TEMPLATE_CONTAINER
# undef _STLP_TEMPLATE_HEADER
# undef _STLP_EQUAL_OPERATOR_SPECIALIZED

#ifdef _STLP_CLASS_PARTIAL_SPECIALIZATION
template <class _Tp, class _Alloc>
struct __move_traits<list<_Tp, _Alloc> > {
  typedef __true_type implemented;
  typedef typename __move_traits<_Alloc>::complete complete;
};
#endif /* _STLP_CLASS_PARTIAL_SPECIALIZATION */

_STLP_END_NAMESPACE

# if !defined (_STLP_LINK_TIME_INSTANTIATION)
#  include <stl/_list.c>
# endif

// do a cleanup
# undef _LIST_IMPL
# undef list

#if defined (_STLP_DEBUG)
# include <stl/debug/_list.h>
#endif

#if defined (_STLP_USE_WRAPPER_FOR_ALLOC_PARAM)
# include <stl/wrappers/_list.h>
#endif

#endif /* _STLP_INTERNAL_LIST_IMPL_H */

// Local Variables:
// mode:C++
// End:
