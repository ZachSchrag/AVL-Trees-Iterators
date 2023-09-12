/*
 *  Implmentation of a templated AVL Tree which supports iterators
 *  Written by Zach Schrag
*/

#pragma once

#include <iostream> // print_tree and size_t
#include <cstddef> // size_t
#include <stdexcept> // std::invalid_argument

template <typename Comparable>
class AVLTree {
    private:
        struct AVLNode {
            Comparable value;
            AVLNode* left;
            AVLNode* right;
            int height;
            AVLNode* parent; // for iterator

            AVLNode() : value{}, left{nullptr}, right{nullptr}, height{1}, parent{nullptr} {}
            explicit AVLNode(const Comparable& value) : value{value}, left{nullptr}, right{nullptr}, height{1}, parent{nullptr} {}
            AVLNode(const Comparable& value, AVLNode* parent) : value{value}, left{nullptr}, right{nullptr}, height{1}, parent{parent} {}
            AVLNode(const Comparable& value, AVLNode* left, AVLNode* right, int height, AVLNode* parent) 
                : value{value}, left{left}, right{right}, height{height}, parent{parent} {}

        };

        AVLNode* root;
        size_t _size;
        AVLNode* min; // for O(1) iterator creation on begin
        AVLNode* max; // for O(1) iterator creation on end

        // for copy constructor / copy assignment operator
        AVLNode* copy(const AVLNode* root) {
            if (!root) return nullptr;
            return new AVLNode(root->value, copy(root->left), copy(root->right), root->height, root->parent);
        }

        void setMinMax() {
            AVLNode* curr = root;
            // for constant iterator creation
            while (curr->left) curr = curr->left;
            min = curr;
            curr = root;
            while (curr->right) curr = curr->right;
            max = curr;
        }

        // helper method for rebalance methods
        int height(const AVLNode* node) const { return !node ? 0 : node->height; } // must avoid nullptr->height

        // methods for rebalancing
        void single_left_rotation(AVLNode* &root) {
            // adjust parent pointers
            root->right->parent = root->parent;
            root->parent = root->right;
            if (root->right->left) root->right->left->parent = root;

            // rotate actual nodes
            AVLNode* right_child = root->right;
            root->right = right_child->left;
            right_child->left = root;

            // fix height
            right_child->height = (height(right_child->left) > height(right_child->right) ? height(right_child->left) : height(right_child->right)) + 1;
            root->height = (height(root->left) > height(root->right) ? height(root->left) : height(root->right)) + 1;

            root = right_child;
        }

        void single_right_rotation(AVLNode* &root) {
            // adjust parent pointers
            root->left->parent = root->parent;
            root->parent = root->left;
            if (root->left->right) root->left->right->parent = root;

            // rotate actual nodes
            AVLNode* left_child = root->left;
            root->left = left_child->right;
            left_child->right = root;

            // fix height
            left_child->height = (height(left_child->left) > height(left_child->right) ? height(left_child->left) : height(left_child->right)) + 1;
            root->height = (height(root->left) > height(root->right) ? height(root->left) : height(root->right)) + 1;

            root = left_child;
        }

        void double_left_rotation(AVLNode* &root) {
            single_right_rotation(root->right);
            single_left_rotation(root);
        }

        void double_right_rotation(AVLNode* &root) {
            single_left_rotation(root->left);
            single_right_rotation(root);
        }

        void rebalance(AVLNode* &root) {
            if (!root) return;

            if (height(root->left) - height(root->right) > 1) {
                if (height(root->left->left) >= height(root->left->right))
                    single_right_rotation(root);
                else 
                    double_right_rotation(root);
            }
            else if (height(root->right) - height(root->left) > 1) {
                if (height(root->right->right) >= height(root->right->left))
                    single_left_rotation(root);
                else
                    double_left_rotation(root);
            }
            
            root->height = (height(root->left) > height(root->right) ? height(root->left) : height(root->right)) + 1;
        }

        // helper methods for public methods
        bool contains(const Comparable& value, const AVLNode* root) const {
            if (!root) return false;
            else if (value < root->value) return contains(value, root->left);
            else if (value > root->value) return contains(value, root->right);
            else return true; 
        }

        const Comparable& find_min(const AVLNode* root) const {
            if (!root) throw std::invalid_argument("Tree is empty.");

            if ( !(root->left) ) return root->value;
            else return find_min(root->left);
        }

        const Comparable& find_max(const AVLNode* root) const {
            if (!root) throw std::invalid_argument("Tree is empty.");

            if ( !(root->right) ) return root->value;
            else return find_max(root->right);
        }

        void print_tree(AVLNode* root, std::ostream& os = std::cout, unsigned int depth = 0) const {
            if (is_empty()) {
                os << "<empty>" << std::endl;
                return;
            }
            if (root->right) print_tree(root->right, os, depth + 1);
            os << std::string(depth * 2, ' ') << root->value << std::endl;
            if (root->left) print_tree(root->left, os, depth + 1);
        }

        void insert(const Comparable& value, AVLNode* &root, AVLNode* parent) {
            if (!root) {
                root = new AVLNode(value, parent);

                if (_size == 0) {
                    min = root;
                    max = root;
                }
                else if (min->value > value) 
                    min = root; 
                else if (max->value < value) 
                    max = root;

                _size++; 
            }

            else if (value < root->value)
                insert(value, root->left, root);
            else if (value > root->value)
                insert(value, root->right, root);

            rebalance(root);
        }

        void remove(const Comparable& value, AVLNode* &root) {
            if (!root) return;

            if (value < root->value) remove(value, root->left);

            else if (value > root->value) remove(value, root->right);

            else if (root->left && root->right) {
                root->value = find_min(root->right);
                remove(root->value, root->right);
            } 

            else {
                if (min == root) {
                    if (root->right)
                        min = root->right;
                    else
                        min = root->parent;
                }
                if (max == root) {
                    if (root->left)
                        max = root->left;
                    else
                        max = root->parent;
                }

                // update tree structure and parent pointers
                AVLNode* old_root = root;
                if (root->left) {
                    root->left->parent = root->parent;
                    root = root->left;
                }
                else {
                    if (root->right) root->right->parent = root->parent;
                    root = root->right;
                }

                delete old_root;
                _size--;
            }

            rebalance(root);
        }

    public:
        class iterator;

        iterator begin() noexcept { return iterator(min, max); }
        iterator end() noexcept { return iterator(max->right, max); }
        
        AVLTree() : root{nullptr}, _size{}, min{nullptr}, max{nullptr} {}
        // lookup
        bool contains(const Comparable& value) const { return contains(value, root); }
        const Comparable& find_min() const { 
            if (!min) throw std::invalid_argument("The tree is empty");
            return min->value; 
        }
        const Comparable& find_max() const { 
            if (!max) throw std::invalid_argument("The tree is empty");
            return max->value;
        }

        // visualization
        void print_tree(std::ostream& os = std::cout) const { print_tree(root, os); }
        
        // modifiers
        void insert(const Comparable& value) { insert(value, root, nullptr); }
        void remove(const Comparable& value) { remove(value, root); }

        // capacity
        size_t size() const noexcept { return _size; }
        bool is_empty() const noexcept{ return !root; }
        void make_empty() { clear(); }

        // rule of three
        void clear() {
            while(!is_empty()) {
                remove(root->value);
            }
        }

        AVLTree(const AVLTree& other) : root{}, _size{}, min{}, max{} { 
            root = copy(other.root);
            setMinMax(); // for constant iterator creation
        }

        ~AVLTree() { clear(); }
        AVLTree& operator=(const AVLTree& rhs) {
            if (this != &rhs) {
                clear();
                root = copy(rhs.root);
                setMinMax(); // for constant iterator creation
            }
            return *this;
        }

        // FOR TESTING ONLY
        const AVLNode* getRoot() const { return root; }
        
        class iterator {
        public:
            using value_type        = AVLNode;
            using difference_type   = ptrdiff_t;
            using pointer           = AVLNode*;
            using reference         = Comparable;
        private:
            pointer ptr;
            pointer max; // to allow --end()

        public:
            iterator() : ptr{nullptr}, max{nullptr} {}
            iterator(pointer ptr, pointer max) : ptr{ptr}, max{max} {}

            iterator& operator=(const iterator&) noexcept = default;

            [[nodiscard]] reference operator*() const noexcept { return ptr->value; }
            [[nodiscard]] pointer operator->() const noexcept { return ptr; }

            iterator& operator++() noexcept { 
                if (!ptr) return *this;

                if (ptr->right) {
                    ptr = ptr->right;
                    while (ptr->left) {
                        ptr = ptr->left;
                    }
                }
                else {
                    AVLNode* parent = ptr->parent;
                    while (parent && ptr == parent->right) {
                        ptr = parent;
                        parent = parent->parent;
                    }
                    ptr = parent;
                }

                return *this;
            }

            iterator operator++(int) noexcept { iterator copy = *this; ++(*this); return copy; }

            iterator& operator--() noexcept {
                if (!ptr) {
                    ptr = max;
                    return *this;
                }

                if (ptr->left) {
                    ptr = ptr->left;
                    while (ptr->right) {
                        ptr = ptr->right;
                    }
                }
                else {
                    AVLNode* parent = ptr->parent;
                    while (parent && ptr == parent->left) {
                        ptr = parent;
                        parent = parent->parent;
                    }
                    ptr = parent;
                }
                return *this;
            }

            iterator operator--(int) noexcept { iterator copy = *this; --(*this); return copy; }

            iterator& operator+=(difference_type offset) noexcept { 
                for (difference_type i = 0; i < offset; i++) {
                    ++(*this);
                }
                return *this;
            }

            [[nodiscard]] iterator operator+(difference_type offset) const noexcept {
                iterator copy(*this);
                for (difference_type i = 0; i < offset; i++) {
                    ++(copy);
                }
                return copy;
            }

            iterator& operator-=(difference_type offset) noexcept { 
                for (difference_type i = 0; i < offset; i++) {
                    --(*this);
                }
                return *this;
            }

            [[nodiscard]] iterator operator-(difference_type offset) const noexcept {
                iterator copy(*this);
                for (difference_type i = 0; i < offset; i++) {
                    --(copy);
                }
                return copy;
            }

            [[nodiscard]] difference_type operator-(const iterator& rhs) const noexcept {
                // undefined behavior if this and rhs are not in the same tree
                difference_type cnt = 0;
                if (!rhs.ptr || !ptr) return cnt;

                iterator copy(*this);
                while (copy.ptr && copy.ptr->value < rhs.ptr->value) {
                    cnt--;
                    ++copy;
                }

                while (copy.ptr && copy.ptr->value > rhs.ptr->value) {
                    cnt++;
                    --copy;
                }

                return cnt;   
            }


            [[nodiscard]] bool operator==(const iterator& rhs) const noexcept { return (!rhs.ptr && !ptr) || (!(!rhs.ptr || !ptr) && (ptr->value == rhs.ptr->value)); }
            [[nodiscard]] bool operator!=(const iterator& rhs) const noexcept { return !(rhs.ptr == ptr); }
            [[nodiscard]] bool operator<(const iterator& rhs) const noexcept {  return !rhs.ptr || (ptr && ptr->value < rhs.ptr->value); }
            [[nodiscard]] bool operator>(const iterator& rhs) const noexcept {  return !rhs.ptr || (ptr && ptr->value > rhs.ptr->value); }
            [[nodiscard]] bool operator<=(const iterator& rhs) const noexcept { return !rhs.ptr || (ptr && ptr->value <= rhs.ptr->value); }
            [[nodiscard]] bool operator>=(const iterator& rhs) const noexcept { return !rhs.ptr || (ptr && ptr->value >= rhs.ptr->value); }
        };
};
