/**
 * File: KDTree.h
 * Author: (your name here)
 * ------------------------
 * An interface representing a kd-tree in some number of dimensions. The tree
 * can be constructed from a set of data and then queried for membership and
 * nearest neighbors.
 */

#ifndef KDTREE_INCLUDED
#define KDTREE_INCLUDED

#include "Point.h"
#include "BoundedPQueue.h"
#include <stdexcept>
#include <cmath>
#include <tuple>

// "using namespace" in a header file is conventionally frowned upon, but I'm
// including it here so that you may use things like size_t without having to
// type std::size_t every time.
using namespace std;
#include <iostream>

template <size_t N, typename ElemType>
class KDTree {
public:
    // Constructor: KDTree();
    // Usage: KDTree<3, int> myTree;
    // ----------------------------------------------------
    // Constructs an empty KDTree.
    KDTree();
    KDTree(const Point<N>& pt, int axis);

    // Destructor: ~KDTree()
    // Usage: (implicit)
    // ----------------------------------------------------
    // Cleans up all resources used by the KDTree.
    ~KDTree();

    // KDTree(const KDTree& rhs);
    // KDTree& operator=(const KDTree& rhs);
    // Usage: KDTree<3, int> one = two;
    // Usage: one = two;
    // -----------------------------------------------------
    // Deep-copies the contents of another KDTree into this one.
    KDTree(const KDTree& rhs);
    KDTree& operator=(const KDTree& rhs);

    // size_t dimension() const;
    // Usage: size_t dim = kd.dimension();
    // ----------------------------------------------------
    // Returns the dimension of the points stored in this KDTree.
    size_t dimension() const;

    // size_t size() const;
    // bool empty() const;
    // Usage: if (kd.empty())
    // ----------------------------------------------------
    // Returns the number of elements in the kd-tree and whether the tree is
    // empty.
    size_t size() const;
    bool empty() const;

    // bool contains(const Point<N>& pt) const;
    // Usage: if (kd.contains(pt))
    // ----------------------------------------------------
    // Returns whether the specified point is contained in the KDTree.
    bool contains(const Point<N>& pt) const;

    // void insert(const Point<N>& pt, const ElemType& value);
    // Usage: kd.insert(v, "This value is associated with v.");
    // ----------------------------------------------------
    // Inserts the point pt into the KDTree, associating it with the specified
    // value. If the element already existed in the tree, the new value will
    // overwrite the existing one.
    void insert(const Point<N>& pt, const ElemType& value);

    // ElemType& operator[](const Point<N>& pt);
    // Usage: kd[v] = "Some Value";
    // ----------------------------------------------------
    // Returns a reference to the value associated with point pt in the KDTree.
    // If the point does not exist, then it is added to the KDTree using the
    // default value of ElemType as its key.
    ElemType& operator[](const Point<N>& pt);

    // ElemType& at(const Point<N>& pt);
    // const ElemType& at(const Point<N>& pt) const;
    // Usage: cout << kd.at(v) << endl;
    // ----------------------------------------------------
    // Returns a reference to the key associated with the point pt. If the point
    // is not in the tree, this function throws an out_of_range exception.
    ElemType& at(const Point<N>& pt);
    const ElemType& at(const Point<N>& pt) const;

    // ElemType kNNValue(const Point<N>& key, size_t k) const
    // Usage: cout << kd.kNNValue(v, 3) << endl;
    // ----------------------------------------------------
    // Given a point v and an integer k, finds the k points in the KDTree
    // nearest to v and returns the most common value associated with those
    // points. In the event of a tie, one of the most frequent value will be
    // chosen.
    ElemType kNNValue(const Point<N>& key, size_t k) const;

private:
    // TODO: Add implementation details here.
    std::shared_ptr<Point<N>> root_ = nullptr;
    ElemType value_;
    int axis_ = 0;
    size_t size_ = 0;
    std::shared_ptr<KDTree<N, ElemType>> left_ = nullptr;
    std::shared_ptr<KDTree<N, ElemType>> right_ = nullptr;
    const std::shared_ptr<KDTree<N, ElemType>> find_node1_(const Point<N>& pt, std::shared_ptr<KDTree<N, ElemType>> tree_ptr) const;
    std::tuple<bool, std::shared_ptr<KDTree<N, ElemType>>> find_node_(const Point<N>& pt, std::shared_ptr<KDTree<N, ElemType>> tree_ptr, bool is_insert);
    void kNNValueHelper(BoundedPQueue<ElemType>& bpq, const Point<N>& test_point) const;
};

/** KDTree class implementation details */

template <size_t N, typename ElemType>
KDTree<N, ElemType>::KDTree() {
    // TODO: Fill this in.
}

template <size_t N, typename ElemType>
KDTree<N, ElemType>::KDTree(const Point<N>& pt, int axis): size_(1), axis_(axis) {
    root_ = std::shared_ptr<Point<N>>(new Point<N>(pt));
}

template <size_t N, typename ElemType>
KDTree<N, ElemType>::~KDTree() {
    // TODO: Fill this in.
}

template <size_t N, typename ElemType>
size_t KDTree<N, ElemType>::dimension() const {
    // TODO: Fill this in.
    return N;
}

// TODO: finish the implementation of the rest of the KDTree class

template <size_t N, typename ElemType>
size_t KDTree<N, ElemType>::size() const {
    return size_;
}

template <size_t N, typename ElemType>
bool KDTree<N, ElemType>::empty() const {
    return size_ == 0;
}

template <size_t N, typename ElemType>
const std::shared_ptr<KDTree<N, ElemType>> KDTree<N, ElemType>::find_node1_(const Point<N>& pt, std::shared_ptr<KDTree<N, ElemType>> tree_ptr) const {
    if (*root_ == pt) {
        return tree_ptr;
    }
    if (pt[axis_] < (*root_)[axis_]) {
        if (left_)
            return left_ -> find_node1_(pt, left_);
        else
            return left_;
    }
    else {
        if (right_)
            return right_ -> find_node1_(pt, right_);
        else
            return right_;
    }
}

template <size_t N, typename ElemType>
std::tuple<bool, std::shared_ptr<KDTree<N, ElemType>>> KDTree<N, ElemType>::find_node_(const Point<N>& pt, std::shared_ptr<KDTree<N, ElemType>> tree_ptr, bool is_insert) {
    if (*root_ == pt) {
        return std::make_tuple(true, tree_ptr);
    }
    if (pt[axis_] < (*root_)[axis_]) {
        if (left_) {
            bool found;
            std::shared_ptr<KDTree<N, ElemType>> tree;
            tie(found, tree) = left_ -> find_node_(pt, left_, is_insert);
            if (!found)
                ++size_;
            return std::make_tuple(found, tree);
        }
        else {
            if (is_insert) {
                left_ = std::shared_ptr<KDTree<N, ElemType>>(new KDTree(pt, (axis_ + 1) % N));
                ++size_;
            }
            return std::make_tuple(false, left_);
        }
    }
    else {
        if (right_) {
            bool found;
            std::shared_ptr<KDTree<N, ElemType>> tree;
            tie(found, tree) = right_ -> find_node_(pt, right_, is_insert);
            if (!found)
                ++size_;
            return std::make_tuple(found, tree);
        }
        else {
            if (is_insert) {
                right_ = std::shared_ptr<KDTree<N, ElemType>>(new KDTree(pt, (axis_ + 1) % N));
                ++size_;
            }
            return std::make_tuple(false, right_);
        }
    }
}

template <size_t N, typename ElemType>
void KDTree<N, ElemType>::insert(const Point<N>& pt, const ElemType& value) {
    if (!root_) {
        root_ = std::shared_ptr<Point<N>>(new Point<N>(pt));
        value_ = value;
        size_ = 1;
    }
    else if (*root_ == pt){
        value_ = value;
    }
    else {
        bool found;
        std::shared_ptr<KDTree<N, ElemType>> tree;
        tie(found, tree) = find_node_(pt, nullptr, true);
        tree -> value_ = value;
    }
}

template <size_t N, typename ElemType>
bool KDTree<N, ElemType>::contains(const Point<N>& pt) const {
    if (!root_) {
        return false;
    }
    else if (pt == *root_){
        return true;
    }
    return find_node1_(pt, nullptr) != nullptr;
}

template <size_t N, typename ElemType>
ElemType& KDTree<N, ElemType>::operator[] (const Point<N>& pt) {
    if (!root_) {
        root_ = std::shared_ptr<Point<N>>(new Point<N>(pt));
        size_ = 1;
        return value_;
    }
    else if (pt == *root_){
        return value_;
    }
    bool found;
    std::shared_ptr<KDTree<N, ElemType>> tree;
    tie(found, tree) = find_node_(pt, nullptr, true);
    return tree -> value_;
}

template <size_t N, typename ElemType>
ElemType& KDTree<N, ElemType>::at (const Point<N>& pt) {
    if (!root_) {
        throw std::out_of_range ("point does not exist!");
    }
    else if (pt == *root_){
        return value_;
    }
    auto tree = find_node1_(pt, nullptr);
    if (tree) {
        return tree -> value_;
    }
    throw std::out_of_range ("point does not exist!");
}

template <size_t N, typename ElemType>
const ElemType& KDTree<N, ElemType>::at (const Point<N>& pt) const {
    if (!root_) {
        throw std::out_of_range ("point does not exist!");
    }
    else if (pt == *root_){
        return value_;
    }
    auto tree = find_node1_(pt, nullptr);
    if (tree) {
        return tree -> value_;
    }
    throw std::out_of_range ("point does not exist!");
}


template <size_t N, typename ElemType>
void KDTree<N, ElemType>::kNNValueHelper(BoundedPQueue<ElemType>& bpq, const Point<N>& test_point) const {
    bpq.enqueue(value_, Distance(*root_, test_point));
    if (test_point[axis_] < (*root_)[axis_]) {
        if (left_)
            left_ -> kNNValueHelper(bpq, test_point);
    }
    else {
        if (right_)
            right_ -> kNNValueHelper(bpq, test_point);
    }
    if (bpq.size() < bpq.maxSize() || fabs((*root_)[axis_] - test_point[axis_]) < bpq.worst()) {
        if (test_point[axis_] < (*root_)[axis_]) {
            if (right_)
                right_ -> kNNValueHelper(bpq, test_point);
        }
        else {
            if (left_)
                left_ -> kNNValueHelper(bpq, test_point);
        }
    }
}

template <size_t N, typename ElemType>
ElemType KDTree<N, ElemType>::kNNValue(const Point<N>& key, size_t k) const {
    BoundedPQueue<ElemType> bpq(k);
    kNNValueHelper(bpq, key); 
    std::map<ElemType, int> count; 
    // KDTree<N, ElemType> tree;
    ElemType val;
    int max_count = -1;
    ElemType max_val;
    while (bpq.size() > 0) {
        val = bpq.dequeueMin();
        if (count.find(val) != count.end()) {
            ++count[val];
        }
        else {
            count.insert(pair<ElemType, int>(val, 1));
        }
        if (count[val] > max_count) {
            max_count = count[val];
            max_val = val;
        }
    }
    std::cout << max_count << std::endl;
    return max_val;
}

#endif // KDTREE_INCLUDED
