#pragma once

#include <functional>
#include <memory>
#include <utility>
#include <type_traits>
#include <algorithm>
#include <vector>
#include <math.h>

namespace fefu {

template<typename T>
class allocator {
 public:
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;
    using pointer = T *;
    using const_pointer [[maybe_unused]] = const T *;
    using reference = typename std::add_lvalue_reference<T>::type;
    using const_reference = typename std::add_lvalue_reference<const T>::type;
    using value_type = T;

    allocator() noexcept = default;

    allocator(const allocator &) noexcept = default;

    template<class U>
    explicit allocator(const allocator<U> &) noexcept { };

    ~allocator() { };

    pointer allocate(size_type n) {
        try {
            if (std::numeric_limits<std::size_t>::max() / sizeof(T) < n)
                throw std::bad_array_new_length();
        } catch (std::bad_array_new_length e) {
            std::cerr
                << "An exception occurred: " << e.what() << std::endl;
        }
        return static_cast<pointer>(::operator new(n * sizeof(value_type)));
    }

    void deallocate(pointer p, [[maybe_unused]] size_type n) noexcept {
        ::operator delete(p);
    }
};

template<typename ValueType>
class hash_map_iterator {
 public:
    using iterator_category = std::forward_iterator_tag;
    using value_type = ValueType;
    using difference_type = std::ptrdiff_t;
    using reference = ValueType &;
    using pointer = ValueType *;

    hash_map_iterator() noexcept = default;

    hash_map_iterator(const hash_map_iterator &other) noexcept :
            ptr_(other.ptr_), is_entry_free_(other.is_entry_free_),
            hm_size_(other.hm_size_), ptr_index_(other.ptr_index_) { }

    reference operator*() const {
        return *(ptr_ + ptr_index_);
    }

    pointer operator->() const {
        return ptr_ + ptr_index_;
    }

    // prefix ++
    hash_map_iterator &operator++() {
        for (size_t i = ptr_index_ + 1; i < hm_size_; i++) {
            if (!is_entry_free_[i]) {
                ptr_index_ = i;
                return *this;
            }
        }
        ptr_index_ = hm_size_;
        return *this;
    }

    // postfix ++typename hash_map_iterator<ValueType>::Node
    hash_map_iterator operator++(int) {
        hash_map_iterator tmp(*this);
        operator++();
        return tmp;
    }

    friend bool operator==(const hash_map_iterator<ValueType> &lhs, const hash_map_iterator<ValueType> &rhs) {
        return
                (lhs.ptr_ + lhs.ptr_index_ == rhs.ptr_ + rhs.ptr_index_);
    }

    friend bool operator!=(const hash_map_iterator<ValueType> &lhs, const hash_map_iterator<ValueType> &rhs) {
        return !(lhs == rhs);
    }

 private:
    template<typename T>
    friend class hash_map_const_iterator;

    template<typename K, typename T, typename Hash, typename Pred, typename Alloc>
    friend
    class hash_map;

    pointer ptr_;
    const bool* is_entry_free_;
    size_t ptr_index_;
    size_t hm_size_;

    explicit hash_map_iterator(
            ValueType* ptr, const bool* is_entry_free,
            const size_t& map_size, const size_t&  ptr_idx) :
            ptr_(ptr), is_entry_free_(is_entry_free),
            hm_size_(map_size), ptr_index_(ptr_idx) { }
};

template<typename ValueType>
class hash_map_const_iterator {
// Shouldn't give non const references on value
 public:
    using iterator_category = std::forward_iterator_tag;
    using value_type = ValueType;
    using difference_type = std::ptrdiff_t;
    using reference = const ValueType &;
    using pointer = const ValueType *;

    hash_map_const_iterator() noexcept = default;

    hash_map_const_iterator(const hash_map_const_iterator& other) noexcept :
            ptr_(other.ptr_), is_entry_free_(other.is_entry_free_),
            hm_size_(other.hm_size_), ptr_index_(other.ptr_index_) { }

    hash_map_const_iterator(const hash_map_iterator<ValueType>& other) noexcept :
            ptr_(pointer(other.ptr_)), is_entry_free_(other.is_entry_free_),
            hm_size_(other.hm_size_), ptr_index_(other.ptr_index_) { }

    reference operator*() const {
        return *(ptr_ + ptr_index_);
    }

    pointer operator->() const {
        return ptr_ + ptr_index_;
    }

    // prefix ++
    hash_map_const_iterator &operator++() {
        for (size_t i = ptr_index_ + 1; i < hm_size_; i++) {
            if (is_entry_free_[i] == false) {
                ptr_index_ = i;
                return *this;
            }
        }

        ptr_index_ = hm_size_;
        return *this;
    }

    // postfix ++typename hash_map_iterator<ValueType>::Node
    hash_map_const_iterator operator++(int) {
        hash_map_const_iterator tmp(*this);
        operator++();
        return tmp;
    }

    friend bool
    operator==(const hash_map_const_iterator<ValueType> &lhs, const hash_map_const_iterator<ValueType> &rhs) {
        return
                (lhs.ptr_ + lhs.ptr_index_ == rhs.ptr_ + rhs.ptr_index_);
    }

    friend bool
    operator!=(const hash_map_const_iterator<ValueType> &lhs, const hash_map_const_iterator<ValueType> &rhs) {
        return !(lhs == rhs);
    }

 private:
    template<typename K, typename T, typename Hash, typename Pred, typename Alloc>
    friend class hash_map;

    pointer ptr_;
    const bool* is_entry_free_;
    size_t ptr_index_;
    size_t hm_size_;

    explicit hash_map_const_iterator(
            const ValueType* ptr, const bool* free_cells,
            const size_t& map_size, const size_t&  ptr_idx) :
            ptr_(ptr), is_entry_free_(free_cells),
            hm_size_(map_size), ptr_index_(ptr_idx) { }
};

template<typename K, typename T,
        typename Hash = std::hash<K>,
        typename Pred = std::equal_to<K>,
        typename Alloc = allocator<std::pair<const K, T>>>
class hash_map {
 public:
    using key_type = K;
    using mapped_type = T;
    using hasher = Hash;
    using key_equal = Pred;
    using allocator_type = Alloc;
    using value_type = std::pair<const key_type, mapped_type>;
    using reference = value_type &;
    using const_reference = const value_type &;
    using iterator = hash_map_iterator<value_type>;
    using const_iterator = hash_map_const_iterator<value_type>;
    using size_type = std::size_t;

    /// Default constructor.
    hash_map() = default;

    /**
     *  @brief  Default constructor creates no elements.
     *  @param n  Minimal initial number of buckets.
     */
    explicit hash_map(size_type n) {
        InitTable_(n);
    }

    /**
     *  @brief  Builds an %hash_map from a range.
     *  @param  first  An input iterator.
     *  @param  last  An input iterator.
     *  @param  n  Minimal initial number of buckets.
     *
     *  Create an %hash_map consisting of copies of the elements from
     *  [first,last).  This is linear in N (where N is
     *  distance(first,last)).
     */
    template<typename InputIterator>
    hash_map(InputIterator first, InputIterator last,
             size_type n = 0) : hash_map(n) {
        insert(first, last);
    }

    /// Copy constructor.
    hash_map(const hash_map &other) {
        buckets_ = other.buckets_;
        entries_ = allocator.allocate(buckets_);

        is_bucket_free_ = new bool[buckets_];
        for (unsigned i = 0; i < bucket_count(); i++)
            is_bucket_free_[i] = true;

        insert(other.begin(), other.end());
        allocator = other.allocator;
        max_load_factor_ = other.max_load_factor_;
        key_equal_ = other.key_equal_;
        hash_ = other.hash_;
    }

    /// Move constructor.
    hash_map(hash_map &&other) {
        swap(other);
    }

    /**
     *  @brief Creates an %hash_map with no elements.
     *  @param a An allocator object.
     */
    explicit hash_map(const allocator_type &a) {
        hash_map();
        allocator = a;
    }

    /*
    *  @brief Copy constructor with allocator argument.
    * @param  uset  Input %hash_map to copy.
    * @param  a  An allocator object.
    */
    hash_map(const hash_map &umap,
             const allocator_type &a) : hash_map(umap) {
        allocator = a;
    }

    /*
    *  @brief  Move constructor with allocator argument.
    *  @param  uset Input %hash_map to move.
    *  @param  a    An allocator object.
    */
    hash_map(hash_map &&umap,
             const allocator_type &a) : hash_map(std::move(umap)) {
        allocator = a;
    }

    /**
     *  @brief  Builds an %hash_map from an initializer_list.
     *  @param  l  An initializer_list.
     *  @param n  Minimal initial number of buckets.
     *
     *  Create an %hash_map consisting of copies of the elements in the
     *  list. This is linear in N (where N is @a l.size()).
     */
    hash_map(std::initializer_list<value_type> l,
             size_type n = 0) {
        InitTable_(n);
        insert(l.begin(), l.end());
    }

    /// Copy assignment operator.
    hash_map &operator=(const hash_map &other) {
        hash_map tmp(other);
        swap(tmp);
        return *this;
    }

    /// Move assignment operator.
    hash_map &operator=(hash_map &&other) {
        swap(other);
        return *this;
    }

    /**
     *  @brief  %hash_map list assignment operator.
     *  @param  l  An initializer_list.
     *
     *  This function fills an %hash_map with copies of the elements in
     *  the initializer list @a l.
     *
     *  Note that the assignment completely changes the %hash_map and
     *  that the resulting %hash_map's size is the same as the number
     *  of elements assigned.
     */
    hash_map &operator=(std::initializer_list<value_type> l) {
        hash_map tmp(l);
        swap(tmp);
        return *this;
    }

    ~hash_map() {
        DeleteTable_();
    }

    ///  Returns the allocator object used by the %hash_map.
    allocator_type get_allocator() const noexcept {
        return allocator;
    }

    // size and capacity:

    ///  Returns true if the %hash_map is empty.
    bool empty() const noexcept {
        return !num_of_entries_;
    }

    ///  Returns the size of the %hash_map.
    size_type size() const noexcept {
        return num_of_entries_;
    }

    ///  Returns the maximum size of the %hash_map.
    size_type max_size() const noexcept {
        return std::allocator_traits<allocator_type>::max_size(allocator);
    }

    // iterators.

    /**
     *  Returns a read/write iterator that points to the first element in the
     *  %hash_map.
     */
    iterator begin() noexcept {
        return iterator(entries_, is_bucket_free_, bucket_count(), FirstEntryIndex_());
    }

    //@{
    /**
     *  Returns a read-only (constant) iterator that points to the first
     *  element in the %hash_map.
     */
    const_iterator begin() const noexcept {
        return cbegin();
    }

    const_iterator cbegin() const noexcept {
        return const_iterator(entries_, is_bucket_free_, bucket_count(), FirstEntryIndex_());
    }

    /**
     *  Returns a read/write iterator that points one past the last element in
     *  the %hash_map.
     */
    iterator end() noexcept {
        return iterator(entries_, is_bucket_free_, bucket_count(), bucket_count());
    }

    //@{
    /**
     *  Returns a read-only (constant) iterator that points one past the last
     *  element in the %hash_map.
     */
    const_iterator end() const noexcept {
        return cend();
    }

    const_iterator cend() const noexcept {
        return const_iterator(entries_, is_bucket_free_, bucket_count(), bucket_count());
    }
    //@}

    // modifiers.

    /**
     *  @brief Attempts to build and insert a std::pair into the
     *  %hash_map.
     *
     *  @param args  Arguments used to generate a new pair instance (see
     *	        std::piecewise_contruct for passing arguments to each
    *	        part of the pair constructor).
    *
    *  @return  A pair, of which the first element is an iterator that points
    *           to the possibly inserted pair, and the second is a bool that
    *           is true if the pair was actually inserted.
    *
    *  This function attempts to build and insert a (key, value) %pair into
    *  the %hash_map.
    *  An %hash_map relies on unique keys and thus a %pair is only
    *  inserted if its first element (the key) is not already present in the
    *  %hash_map.
    *
    *  Insertion requires amortized constant time.
    */
    template<typename... _Args>
    std::pair<iterator, bool> emplace(_Args &&... args) {
        return insert(value_type(std::forward<_Args>(args)...));
    }

    /**
     *  @brief Attempts to build and insert a std::pair into the
     *  %hash_map.
     *
     *  @param k    Key to use for finding a possibly existing pair in
     *                the hash_map.
     *  @param args  Arguments used to generate the .second for a
     *                new pair instance.
     *
     *  @return  A pair, of which the first element is an iterator that points
     *           to the possibly inserted pair, and the second is a bool that
     *           is true if the pair was actually inserted.
     *
     *  This function attempts to build and insert a (key, value) %pair into
     *  the %hash_map.
     *  An %hash_map relies on unique keys and thus a %pair is only
     *  inserted if its first element (the key) is not already present in the
     *  %hash_map.
     *  If a %pair is not inserted, this function has no effect.
     *
     *  Insertion requires amortized constant time.
     */
    template<typename... _Args>
    std::pair<iterator, bool> try_emplace(const key_type &k, _Args &&... args) {
        return try_emplace(key_type(k), std::forward<_Args>(args)...);
    }

    // move-capable overload
    template<typename... _Args>
    std::pair<iterator, bool> try_emplace(key_type &&k, _Args &&... args) {
        auto it = find(k);
        if (it != end())
            return {it, false};

        return emplace(std::piecewise_construct,
                       std::forward_as_tuple(std::forward<key_type>(k)),
                       std::forward_as_tuple(std::forward<_Args>(args)...));
    }

    //@{
    /**
     *  @brief Attempts to insert a std::pair into the %hash_map.
     *  @param x Pair to be inserted (see std::make_pair for easy
     *	     creation of pairs).
    *
    *  @return  A pair, of which the first element is an iterator that
    *           points to the possibly inserted pair, and the second is
    *           a bool that is true if the pair was actually inserted.
    *
    *  This function attempts to insert a (key, value) %pair into the
    *  %hash_map. An %hash_map relies on unique keys and thus a
    *  %pair is only inserted if its first element (the key) is not already
    *  present in the %hash_map.
    *
    *  Insertion requires amortized constant time.
    */
    std::pair<iterator, bool> insert(const value_type &x) {
        return insert(value_type(x));
    }

    std::pair<iterator, bool> insert(value_type &&x) {
        if (bucket_count() == 0) {
            InitTable_(1);
        }

        auto it = find(x.first);
        if (it != end())
            return std::make_pair(it, false);

        if (load_factor() >= max_load_factor_) {
            rehash(bucket_count() * 2);
        }

        size_type target_index = bucket(x.first);

        new(entries_ + target_index) value_type{x};
        is_bucket_free_[target_index] = false;
        num_of_entries_++;

        return std::make_pair(
                iterator(entries_, is_bucket_free_, bucket_count(), target_index), true);
    }

    //@}

    /**
     *  @brief A template function that attempts to insert a range of
     *  elements.
     *  @param  first  Iterator pointing to the start of the range to be
     *                   inserted.
     *  @param  last  Iterator pointing to the end of the range.
     *
     *  Complexity similar to that of the range constructor.
     */
    template<typename _InputIterator>
    void insert(_InputIterator first, _InputIterator last) {
        for (auto it = first; it != last; it++) {
            insert(*it);
        }
    }

    /**
     *  @brief Attempts to insert a list of elements into the %hash_map.
     *  @param  l  A std::initializer_list<value_type> of elements
     *               to be inserted.
     *
     *  Complexity similar to that of the range constructor.
     */
    void insert(std::initializer_list<value_type> l) {
        insert(l.begin(), l.end());
    }


    /**
     *  @brief Attempts to insert a std::pair into the %hash_map.
     *  @param k    Key to use for finding a possibly existing pair in
     *                the map.
     *  @param obj  Argument used to generate the .second for a pair
     *                instance.
     *
     *  @return  A pair, of which the first element is an iterator that
     *           points to the possibly inserted pair, and the second is
     *           a bool that is true if the pair was actually inserted.
     *
     *  This function attempts to insert a (key, value) %pair into the
     *  %hash_map. An %hash_map relies on unique keys and thus a
     *  %pair is only inserted if its first element (the key) is not already
     *  present in the %hash_map.
         *  If the %pair was already in the %hash_map, the .second of
         *  the %pair is assigned from obj.
     *
     *  Insertion requires amortized constant time.
     */
    template<typename _Obj>
    std::pair<iterator, bool> insert_or_assign(const key_type &k, _Obj &&obj) {
        return insert_or_assign(key_type(k), obj);
    }

    // move-capable overload
    template<typename _Obj>
    std::pair<iterator, bool> insert_or_assign(key_type &&k, _Obj &&obj) {
        auto it = insert(value_type(k, obj));
            if (!it.second) {
            it.first->second = obj;
        }
        return it;
    }

    //@{
    /**
     *  @brief Erases an element from an %hash_map.
     *  @param  position  An iterator pointing to the element to be erased.
     *  @return An iterator pointing to the element immediately following
     *          @a position prior to the element being erased. If no such
     *          element exists, end() is returned.
     *
     *  This function erases an element, pointed to by the given iterator,
     *  from an %hash_map.
     *  Note that this function only erases the element, and that if the
     *  element is itself a pointer, the pointed-to memory is not touched in
     *  any way.  Managing the pointer is the user's responsibility.
     */
    iterator erase(const_iterator position) {
        auto iter = ++(find(position->first));
        std::destroy_at(entries_ + position.ptr_index_);
        is_bucket_free_[position.ptr_index_] = true;
        num_of_entries_--;
        return iter;
    }

    // LWG 2059.
    iterator erase(iterator position) {
        return erase(const_iterator(position));
    }
    //@}

    /**
     *  @brief Erases elements according to the provided key.
     *  @param  x  Key of element to be erased.
     *  @return  The number of elements erased.
     *
     *  This function erases all the elements located by the given key from
     *  an %hash_map. For an %hash_map the result of this function
     *  can only be 0 (not present) or 1 (present).
     *  Note that this function only erases the element, and that if the
     *  element is itself a pointer, the pointed-to memory is not touched in
     *  any way.  Managing the pointer is the user's responsibility.
     */
    size_type erase(const key_type &x) {
        auto it = find(x);
        if (it == end())
            return 0;

        erase(it);
        return 1;
    }

    /**
     *  @brief Erases a [first,last) range of elements from an
     *  %hash_map.
     *  @param  first  Iterator pointing to the start of the range to be
     *                  erased.
     *  @param last  Iterator pointing to the end of the range to
     *                be erased.
     *  @return The iterator @a last.
     *
     *  This function erases a sequence of elements from an %hash_map.
     *  Note that this function only erases the elements, and that if
     *  the element is itself a pointer, the pointed-to memory is not touched
     *  in any way.  Managing the pointer is the user's responsibility.
     */
    iterator erase(const_iterator first, const_iterator last) {
        for (auto cit = first; cit != cend(); cit++) {
            erase(cit);
        }
        return find(last->first);
    }

    /**
     *  Erases all elements in an %hash_map.
     *  Note that this function only erases the elements, and that if the
     *  elements themselves are pointers, the pointed-to memory is not touched
     *  in any way.  Managing the pointer is the user's responsibility.
     */
    void clear() noexcept {
        for (size_t i = 0; i < bucket_count(); i++) {
            if (!is_bucket_free_[i])
                std::destroy_at(entries_ + i);
            is_bucket_free_[i] = true;
        }
        num_of_entries_ = 0;
    }

    /**
     *  @brief  Swaps data with another %hash_map.
     *  @param  x  An %hash_map of the same element and allocator
     *  types.
     *
     *  This exchanges the elements between two %hash_map in constant
     *  time.
     *  Note that the global std::swap() function is specialized such that
     *  std::swap(m1,m2) will feed to this function.
     */
    void swap(hash_map &x) {
        std::swap(allocator, x.allocator);
        std::swap(entries_, x.entries_);
        std::swap(is_bucket_free_, x.is_bucket_free_);
        std::swap(num_of_entries_, x.num_of_entries_);
        std::swap(buckets_, x.buckets_);
        std::swap(max_load_factor_, x.max_load_factor_);
        std::swap(key_equal_, x.key_equal_);
        std::swap(hash_, x.hash_);
    }

    template<typename _H2, typename _P2>
    void merge(hash_map<K, T, _H2, _P2, Alloc> &source) {
        for (auto i = source.begin(); i != source.end(); i++) {
            auto it = insert(*i);
            if (it.second)
                source.erase(i);
        }
    }

    template<typename _H2, typename _P2>
    void merge(hash_map<K, T, _H2, _P2, Alloc> &&source) {
        insert(source.begin(), source.end());
    }

    // observers.

    ///  Returns the hash functor object with which the %hash_map was
    ///  constructed.
    Hash hash_function() const {
        return Hash{};
    }

    ///  Returns the key comparison object with which the %hash_map was
    ///  constructed.
    Pred key_eq() const {
        return key_equal_;
    }

    // lookup.

    //@{
    /**
     *  @brief Tries to locate an element in an %hash_map.
     *  @param  x  Key to be located.
     *  @return  Iterator pointing to sought-after element, or end() if not
     *           found.
     *
     *  This function takes a key and tries to locate the element with which
     *  the key matches.  If successful the function returns an iterator
     *  pointing to the sought after element.  If unsuccessful it returns the
     *  past-the-end ( @c end() ) iterator.
     */
    iterator find(const key_type &x) {
        if (bucket_count() == 0) {
            return end();
        }


        size_type target_index = bucket(x);

        if (!is_bucket_free_[target_index] && key_equal_(x, entries_[target_index].first)) {
            return iterator(entries_, is_bucket_free_, bucket_count(), target_index);
        } else {
            return end();
        }
    }

    const_iterator find(const key_type &x) const {
        if (bucket_count() == 0) {
            return end();
        }

        size_type target_index = bucket(x);

        if (!is_bucket_free_[target_index] && key_equal_(x, entries_[target_index].first)) {
            return const_iterator(entries_, is_bucket_free_, bucket_count(), target_index);
        } else {
            return cend();
        }
    }
    //@}

    /**
     *  @brief  Finds the number of elements.
     *  @param  x  Key to count.
     *  @return  Number of elements with specified key.
     *
     *  This function only makes sense for %unordered_multimap; for
     *  %hash_map the result will either be 0 (not present) or 1
     *  (present).
     */
    size_type count(const key_type &x) const {
        return contains(x) ? 1 : 0;
    }

    /**
     *  @brief  Finds whether an element with the given key exists.
     *  @param  x  Key of elements to be located.
     *  @return  True if there is any element with the specified key.
     */
    bool contains(const key_type &x) const {
        if (bucket_count() == 0) {
            return false;
        }

        size_type target_index = bucket(x);
        return !is_bucket_free_[target_index];
    }

    //@{
    /**
     *  @brief  Subscript ( @c [] ) access to %hash_map data.
     *  @param  k  The key for which data should be retrieved.
     *  @return  A reference to the data of the (key,data) %pair.
     *
     *  Allows for easy lookup with the subscript ( @c [] )operator.  Returns
     *  data associated with the key specified in subscript.  If the key does
     *  not exist, a pair with that key is created using default values, which
     *  is then returned.
     *
     *  Lookup requires constant time.
     */
    mapped_type &operator[](const key_type &k) {
        return operator[](key_type(k));
    }

    mapped_type &operator[](key_type &&k) {
        auto it = insert({std::move(k), T()});
        return it.first->second;
    }
    //@}

    //@{
    /**
     *  @brief  Access to %hash_map data.
     *  @param  k  The key for which data should be retrieved.
     *  @return  A reference to the data whose key is equal to @a k, if
     *           such a data is present in the %hash_map.
     *  @throw  std::out_of_range  If no such data is present.
     */
    mapped_type &at(const key_type &k) {
        auto it = find(k);
        if (it == end()) {
            throw std::out_of_range("out of range");
        } else {
            return it->second;
        }
    }

    const mapped_type &at(const key_type &k) const {
        return at(k);
    }
    //@}

    // bucket interface.

    /// Returns the number of buckets of the %hash_map.
    size_type bucket_count() const noexcept {
        return buckets_ == 0 ? 0 : buckets_ - 1;
    }

    /*
    * @brief  Returns the bucket index of a given element.
    * @param  _K  A key instance.
    * @return  The key bucket index.
    */
    size_type bucket(const key_type &_K) const {
        if (bucket_count() == 0)
            return 0;

        size_t prim_hash = Hash{}(_K) % bucket_count();
        size_t sec_hash = AdditionalHash_(_K);
        size_t i = 0;
        size_t target_index = (prim_hash + i * sec_hash) % bucket_count();

        if (bucket_count() == num_of_entries_)
            return target_index;

        while (!key_equal_(_K, entries_[target_index].first) && !is_bucket_free_[target_index]) {
            target_index = (prim_hash + i * sec_hash) % bucket_count();
            i++;
        }

        return target_index;
    }

    // hash policy.

    /// Returns the average number of elements per bucket.
    float load_factor() const noexcept {
        if (bucket_count() == 0) {
            return 0;
        } else {
            return
                static_cast<double>(num_of_entries_) / static_cast<double>(bucket_count());
        }
    }

    /// Returns a positive number that the %hash_map tries to keep the
    /// load factor less than or equal to.
    float max_load_factor() const noexcept {
        return max_load_factor_;
    }

    /**
     *  @brief  Change the %hash_map maximum load factor.
     *  @param  z The new maximum load factor.
     */
    void max_load_factor(float z) {
        max_load_factor_ = (z > 0 && z <= 1) ? z : max_load_factor_;
    }

    /**
     *  @brief  May rehash the %hash_map.
     *  @param  n The new number of buckets.
     *
     *  Rehash will occur only if the new number of buckets respect the
     *  %hash_map maximum load factor.
     */
    void rehash(size_type n) {
        std::vector<value_type> tmp(begin(), end());
        DeleteTable_();

        size_t pow_of_two = 1;
        while(pow_of_two < n)
            pow_of_two <<= 1u;

        buckets_ = pow_of_two + 1;
        entries_ = allocator.allocate(buckets_);

        is_bucket_free_ = new bool[buckets_];
        for (size_t i = 0; i < bucket_count(); i++)
            is_bucket_free_[i] = true;

        insert(tmp.begin(), tmp.end());
    }

    /**
     *  @brief  Prepare the %hash_map for a specified number of
     *          elements.
     *  @param  n Number of elements required.
     *
     *  Same as rehash(ceil(n / max_load_factor())).
     */
    void reserve(size_type n) {
        rehash(ceil(n / max_load_factor()));
    }

    bool operator==(const hash_map &other) const {
        if (size() != other.size())
            return false;

        for (auto i : *this) {
            auto it = other.find(key_type(i.first));
            if (it == other.end())
                return false;
            if (it->second != i.second)
                return false;
        }

        return true;
    }

 private:
    Alloc allocator;
    value_type* entries_ = nullptr;
    bool* is_bucket_free_ = nullptr;
    size_type num_of_entries_ = 0;
    size_type buckets_ = 0;
    double max_load_factor_ = 0.75f;
    Pred key_equal_;
    Hash hash_;

    void InitTable_(size_type buckets = 1) {
        size_t pow_of_tow = 1;

        while (pow_of_tow < buckets)
            pow_of_tow <<= 1;

        buckets_ = pow_of_tow + 1;

        entries_ = allocator.allocate(buckets_);
        is_bucket_free_ = new bool[buckets_];
        for (size_t i = 0; i < bucket_count(); i++)
            is_bucket_free_[i] = true;
    }

    void DeleteTable_() {
        clear();
        allocator.deallocate(entries_, buckets_);
        delete[] is_bucket_free_;
        is_bucket_free_ = nullptr;
        num_of_entries_ = 0;
    }

    [[nodiscard]] size_type FirstEntryIndex_() const noexcept {
        if (bucket_count() == 0)
            return 0;

        size_t index = bucket_count();

        for (size_t i = 0; i < bucket_count(); i++)  {
            if (!is_bucket_free_[i]) {
                index = i;
                break;
            }
        }
        return index;
    }

    size_type AdditionalHash_(const key_type& _K) const {
        return ((Hash{}(_K) << 1) | 1u) % bucket_count();
    }
};
}  // namespace fefu