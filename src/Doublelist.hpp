#ifndef DOUBLELIST_H
#define DOUBLELIST_H

#include <memory>
#include <iterator>

template <typename T>
class Doublelist {
    class dl_iterator;
public:
// Types
    using value_type = T;
    using size_type = size_t;
    using reference = value_type&;
    // using const_reference = const value_type&;
    using iterator = dl_iterator;

// Member functions
    // Constructor
    Doublelist();
    //Doublelist(const Doublelist&);
    //Doublelist(size_type);

    // Element Access
    auto front() -> reference;
    auto back() -> reference;

    // Iterators (just pointers in this case)
    auto begin() noexcept -> iterator;
    auto end() noexcept -> iterator;

    // Capacity
    auto empty() -> bool;
    auto size() const -> size_type;

    // Modifiers
    auto insert(iterator pos, const T& value) -> iterator;
    auto erase(iterator pos) -> iterator;
    auto push_back(const T&) -> void;
    auto push_front(const T&) -> void;

    // Destructor
    ~Doublelist();

private:
    struct Node {
        // Constructor
        Node(const T& value):
            next(nullptr), prev(nullptr), _data(value) { }
        // Directions
        Node* next;
        Node* prev;
        T _data;
    };

    auto del() -> void;

    Node* sentinel;
    size_type node_count = 0;

    std::allocator<Node> alloc;
    std::allocator_traits<decltype(alloc)> alloc_t;

};

///////////////////////////////////////

// Constructor

// Default Constructor
template <typename T>
Doublelist<T>::Doublelist():
    sentinel(nullptr) {
        auto end_node = alloc_t.allocate(alloc, 1);
        alloc_t.construct(alloc, end_node, T());
        sentinel = end_node;

       //++node_count;
}

// Member functions

// Element Access
template <typename T>
auto Doublelist<T>::front() -> reference {
    return begin()->_data;
}


template <typename T>
auto Doublelist<T>::back() -> reference {
    auto rear = end();
    --rear;
    return rear->_data;
}

// Iterators (just pointers in this case)
template <typename T>
auto Doublelist<T>::begin() noexcept -> iterator {
    if (sentinel->next == nullptr) {
        return end();
    }
    return dl_iterator(sentinel->next);
}

template <typename T>
auto Doublelist<T>::end() noexcept -> iterator {
    return dl_iterator(sentinel);
}

// Capacity
template <typename T>
auto Doublelist<T>::empty() -> bool {
    return begin() == end();
}

template <typename T>
auto Doublelist<T>::size() const -> size_type {
    return node_count;
}


// Modifiers

template <typename T>
auto Doublelist<T>::insert(iterator pos, const T& value) -> iterator {
    // Insert new Node of type T before location pos,
    // then return iterator to new node

    // Make new node
    auto new_node = alloc_t.allocate(alloc, 1);
    alloc_t.construct(alloc, new_node, value);

    // Make the node before pos point to new Node via next
    pos->prev->next = new_node;

    new_node->prev = pos->prev;

    // Make the pos node  point to new node
    pos->prev = new_node;

    // Have new node's next member point to current head node
    new_node->next = pos;

    ++node_count;

    return dl_iterator(new_node);
}

template <typename T>
auto Doublelist<T>:: erase(iterator pos) -> iterator {
    // Delete node at pos, then return iterataor to the following node
    if (pos == end()) {
        return pos;
    }
    auto new_iter = dl_iterator(pos->next);

    // Have the node before pos
    // point to the node after pos

    auto removed = pos->prev->next;
    pos->prev->next = pos->next;

    // Have the node after pos
    // point to the node before pos
    pos->next->prev = pos->prev;

    // Delete pos
    alloc_t.destroy(alloc, removed);
    alloc_t.deallocate(alloc, removed, 1);
    --node_count;

    return new_iter;
}

template <typename T>
auto Doublelist<T>::push_back(const T& value) -> void {
    // Add node to the back of the Doublelist

    // Make a new node with T value
    auto item = alloc_t.allocate(alloc, 1);
    alloc_t.construct(alloc, item, value);

    if (empty()) {
        // Have sentinel point to the only node
        sentinel->next = item;
        sentinel->prev = item;

        // Have item point to sentinel
        item->next = sentinel;
        item->prev = sentinel;
    } else {
        auto rear = sentinel->prev;

        rear->next = item;   
        item->next = sentinel;
        item->prev = rear;
        sentinel->prev = item;
    }

    ++node_count;
}

template <typename T>
auto Doublelist<T>::push_front(const T& value) -> void {
    // Add node to the front of the Doublelist

    // Make a new node with T value
    auto item = alloc_t.allocate(alloc, 1);
    alloc_t.construct(alloc, item, value);

    // Make sentinel->next point to item
    if (empty()) {
        // Have sentinel point to the only node
        sentinel->next = item;
        sentinel->prev = item;

        // Have item point to sentinel
        item->next = sentinel;
        item->prev = sentinel;
    } else {
        auto top = sentinel->next;

        // Take former head node's prev
        item->prev = top->prev;
        // Make former head node point to head via prev
        top->prev = item;
        // Link former head node with new node
        item->next = top;
        // Make new node the new begin() target
        sentinel->next = item;
    }

    ++node_count;
}

template <typename T>
Doublelist<T>::~Doublelist() {
    del();
    sentinel = nullptr;
}

///// Private member functions /////

template <typename T>
auto Doublelist<T>::del() -> void {
    auto del_iter = begin();
    while (del_iter != end()) {
        del_iter = erase(del_iter);   
    }

    if (empty()) {
        alloc_t.destroy(alloc, sentinel);
        alloc_t.deallocate(alloc, sentinel, 1);
    }
}


////////
template <typename T>
class Doublelist<T>::dl_iterator {
public:
    using iterator_category = std::bidirectional_iterator_tag;
    using difference_type = std::ptrdiff_t;
    using value_type = T;
    using pointer = value_type*;
    using reference = value_type&;

    dl_iterator(Node* ptr): _ptr(ptr) { }

    auto operator*() const -> reference {
        return _ptr->_data;
    }

    auto operator->() const -> Node* {
        return _ptr;
    }

    // postfix increment
    auto operator++(int) -> dl_iterator {
        auto copy = *this;
        if (_ptr == sentinel) {
            return copy;
        }
        _ptr = _ptr->next;
        return copy;
    }

    // prefix increment
    auto operator++() -> dl_iterator& {
        _ptr = _ptr->next;
        return *this;
    }

    // postfix decrement
    auto operator--(int) -> dl_iterator {
        auto copy = *this;

        _ptr = _ptr->prev;
        return copy;
    }

    // prefix decrement
    auto operator--() -> dl_iterator& {
        _ptr = _ptr->prev;
        return *this;
    }

    friend auto operator==(const dl_iterator& lhs, const dl_iterator& rhs) -> bool {
        return lhs._ptr == rhs._ptr;
    }

    friend auto operator!=(const dl_iterator& lhs, const dl_iterator& rhs) -> bool {
        return !(lhs._ptr == rhs._ptr);
    }

private:
    Node* _ptr;
};

#endif
