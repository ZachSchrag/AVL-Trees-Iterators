#include "avl.h"
#include <sstream> // visualization test
#include <random> // generate values to insert
#include <unordered_map> // used to keep track of the values generated to insert
#include <algorithm> // to randomly select a single node to remove from the map (using sample)

using std::cout, std::endl;

#define black   "\033[30m"
#define red     "\033[31m"
#define green   "\033[32m"
#define yellow  "\033[33m"
#define blue    "\033[34m"
#define magenta "\033[35m"
#define cyan    "\033[36m"
#define white   "\033[37m"
#define reset   "\033[m"

#define to_be ==
#define not_to_be !=
#define is to_be
#define is_not not_to_be

#define expect(X) try {\
  if (!(X)) {\
    std::cout << red "  [fail]" reset " (" << __FILE__ << ":" << __FUNCTION__ << ":" << __LINE__ << ") " << red << "expected " << #X << "." << reset << std::endl;\
  }\
} catch(...) {\
  std::cout << red "  [fail]" reset " (" << __FILE__ << ":" << __FUNCTION__ << ":" << __LINE__ << ") " << red << #X << " threw an unexpected exception." << reset << std::endl;\
}

#define assert(X) try {\
  if (!(X)) {\
    std::cout << red "  [fail]" reset " (" << __FILE__ << ":" << __FUNCTION__ << ":" << __LINE__ << ") " << red << "failed assertion that " << #X << "." << reset << std::endl;\
    std::abort();\
  }\
} catch(...) {\
  std::cout << red "  [fail]" reset " (" << __FILE__ << ":" << __FUNCTION__ << ":" << __LINE__ << ") " << red << #X << " assertion threw an unexpected exception." << reset << std::endl;\
}

#define expect_throw(X,E) {\
  bool threw_expected_exception = false;\
  try { X; }\
  catch(const E& err) {\
    threw_expected_exception = true;\
  } catch(...) {\
    std::cout << blue << "  [help] " << #X << " threw an incorrect exception." << reset << std::endl;\
  }\
  if (!threw_expected_exception) {\
    std::cout << red <<"  [fail]" << reset << " (" << __FILE__ << ":" << __FUNCTION__ << ":" << __LINE__ << ") " << red << "expected " << #X << " to throw " << #E <<"." << reset <<std::endl;\
  }\
}



int main() {

    // constructor / is_empty
    {
        AVLTree<int> intTree;
        expect(intTree.is_empty() to_be true);
        expect(intTree.getRoot() to_be nullptr);
    }


    // make_empty
    {
        AVLTree<int> intTree;
        intTree.insert(1);
        intTree.insert(2);
        intTree.insert(0);
        expect(intTree.is_empty() to_be false);
        intTree.make_empty();
        expect(intTree.is_empty() to_be true);
    }


    // insert
    {
        AVLTree<int> intTree;
        intTree.insert(1);
        assert(intTree.getRoot() is_not nullptr);
        expect(intTree.getRoot()->value to_be 1);
        expect(intTree.getRoot()->height to_be 1);

        intTree.insert(2);
        expect(intTree.getRoot()->value to_be 1);
        expect(intTree.getRoot()->height to_be 2);
        assert(intTree.getRoot()->right is_not nullptr);
        expect(intTree.getRoot()->right->value to_be 2);
        expect(intTree.getRoot()->right->height to_be 1);

        // expect single left rotation
        intTree.insert(3);
        assert(intTree.getRoot() is_not nullptr);
        assert(intTree.getRoot()->left is_not nullptr);
        assert(intTree.getRoot()->right is_not nullptr);
        expect(intTree.getRoot()->value to_be 2);
        expect(intTree.getRoot()->height to_be 2);
        expect(intTree.getRoot()->left->value to_be 1);
        expect(intTree.getRoot()->left->height to_be 1);
        expect(intTree.getRoot()->right->value to_be 3);
        expect(intTree.getRoot()->right->height to_be 1);

        // insert to prepare a right rotation
        intTree.insert(0);
        assert(intTree.getRoot() is_not nullptr);
        expect(intTree.getRoot()->value to_be 2);
        expect(intTree.getRoot()->height to_be 3);
        assert(intTree.getRoot()->left is_not nullptr);
        expect(intTree.getRoot()->left->value to_be 1);
        expect(intTree.getRoot()->left->height to_be 2);
        assert(intTree.getRoot()->right is_not nullptr);
        expect(intTree.getRoot()->right->value to_be 3);
        expect(intTree.getRoot()->right->height to_be 1);
        assert(intTree.getRoot()->left->left is_not nullptr);
        expect(intTree.getRoot()->left->left->value to_be 0);
        expect(intTree.getRoot()->left->left->height to_be 1);

        // expect single right rotation 
        intTree.insert(-1);
        assert(intTree.getRoot() is_not nullptr);
        expect(intTree.getRoot()->value to_be 2);
        expect(intTree.getRoot()->height to_be 3);
        assert(intTree.getRoot()->left is_not nullptr);
        expect(intTree.getRoot()->left->value to_be 0);
        expect(intTree.getRoot()->left->height to_be 2);
        assert(intTree.getRoot()->right is_not nullptr);
        expect(intTree.getRoot()->right->value to_be 3);
        expect(intTree.getRoot()->right->height to_be 1);
        assert(intTree.getRoot()->left->left is_not nullptr);
        expect(intTree.getRoot()->left->left->value to_be -1);
        expect(intTree.getRoot()->left->left->height to_be 1);
        assert(intTree.getRoot()->left->right is_not nullptr);
        expect(intTree.getRoot()->left->right->value to_be 1);
        expect(intTree.getRoot()->left->left->height to_be 1);

        // insert to prepare for double left rotation
        intTree.insert(5);
        assert(intTree.getRoot() is_not nullptr);
        expect(intTree.getRoot()->value to_be 2);
        expect(intTree.getRoot()->height to_be 3);
        assert(intTree.getRoot()->left is_not nullptr);
        expect(intTree.getRoot()->left->value to_be 0);
        expect(intTree.getRoot()->left->height to_be 2);
        assert(intTree.getRoot()->right is_not nullptr);
        expect(intTree.getRoot()->right->value to_be 3);
        expect(intTree.getRoot()->right->height to_be 2);
        assert(intTree.getRoot()->left->left is_not nullptr);
        expect(intTree.getRoot()->left->left->value to_be -1);
        expect(intTree.getRoot()->left->left->height to_be 1);
        assert(intTree.getRoot()->left->right is_not nullptr);
        expect(intTree.getRoot()->left->right->value to_be 1);
        expect(intTree.getRoot()->left->right->height to_be 1);
        assert(intTree.getRoot()->right->right is_not nullptr);
        expect(intTree.getRoot()->right->right->value to_be 5);
        expect(intTree.getRoot()->right->right->height to_be 1);

        // insert to open place to cause double left rotation
        intTree.insert(-3);
        assert(intTree.getRoot() is_not nullptr);
        expect(intTree.getRoot()->value to_be 2);
        expect(intTree.getRoot()->height to_be 4);
        assert(intTree.getRoot()->left is_not nullptr);
        expect(intTree.getRoot()->left->value to_be 0);
        expect(intTree.getRoot()->left->height to_be 3);
        assert(intTree.getRoot()->right is_not nullptr);
        expect(intTree.getRoot()->right->value to_be 3);
        expect(intTree.getRoot()->right->height to_be 2);
        assert(intTree.getRoot()->left->left is_not nullptr);
        expect(intTree.getRoot()->left->left->value to_be -1);
        expect(intTree.getRoot()->left->left->height to_be 2);
        assert(intTree.getRoot()->left->right is_not nullptr);
        expect(intTree.getRoot()->left->right->value to_be 1);
        expect(intTree.getRoot()->left->right->height to_be 1);
        assert(intTree.getRoot()->right->right is_not nullptr);
        expect(intTree.getRoot()->right->right->value to_be 5);
        expect(intTree.getRoot()->right->right->height to_be 1);
        assert(intTree.getRoot()->left->left->left is_not nullptr);
        assert(intTree.getRoot()->left->left->left->value to_be -3);
        assert(intTree.getRoot()->left->left->left->height to_be 1);

        // expect double left rotation
        intTree.insert(-2);
        assert(intTree.getRoot() is_not nullptr);
        expect(intTree.getRoot()->value to_be 2);
        expect(intTree.getRoot()->height to_be 4);
        assert(intTree.getRoot()->left is_not nullptr);
        expect(intTree.getRoot()->left->value to_be 0);
        expect(intTree.getRoot()->left->height to_be 3);
        assert(intTree.getRoot()->right is_not nullptr);
        expect(intTree.getRoot()->right->value to_be 3);
        expect(intTree.getRoot()->right->height to_be 2);
        assert(intTree.getRoot()->left->left is_not nullptr);
        expect(intTree.getRoot()->left->left->value to_be -2);
        expect(intTree.getRoot()->left->left->height to_be 2);
        assert(intTree.getRoot()->left->right is_not nullptr);
        expect(intTree.getRoot()->left->right->value to_be 1);
        expect(intTree.getRoot()->left->right->height to_be 1);
        assert(intTree.getRoot()->right->right is_not nullptr);
        expect(intTree.getRoot()->right->right->value to_be 5);
        expect(intTree.getRoot()->right->right->height to_be 1);
        assert(intTree.getRoot()->left->left->left is_not nullptr);
        assert(intTree.getRoot()->left->left->left->value to_be -3);
        assert(intTree.getRoot()->left->left->left->value to_be -3);
        assert(intTree.getRoot()->left->left->left->height to_be 1);
        assert(intTree.getRoot()->left->left->right is_not nullptr);
        assert(intTree.getRoot()->left->left->right->value to_be -1);
        assert(intTree.getRoot()->left->left->right->height to_be 1);

        // expect double right rotation
        intTree.insert(4);
        assert(intTree.getRoot() is_not nullptr);
        expect(intTree.getRoot()->value to_be 2);
        expect(intTree.getRoot()->height to_be 4);
        assert(intTree.getRoot()->left is_not nullptr);
        expect(intTree.getRoot()->left->value to_be 0);
        expect(intTree.getRoot()->left->height to_be 3);
        assert(intTree.getRoot()->right is_not nullptr);
        expect(intTree.getRoot()->right->value to_be 4);
        expect(intTree.getRoot()->right->height to_be 2);
        assert(intTree.getRoot()->left->left is_not nullptr);
        expect(intTree.getRoot()->left->left->value to_be -2);
        expect(intTree.getRoot()->left->left->height to_be 2);
        assert(intTree.getRoot()->left->right is_not nullptr);
        expect(intTree.getRoot()->left->right->value to_be 1);
        expect(intTree.getRoot()->left->right->height to_be 1);
        assert(intTree.getRoot()->right->right is_not nullptr);
        expect(intTree.getRoot()->right->right->value to_be 5);
        expect(intTree.getRoot()->right->right->height to_be 1);
        assert(intTree.getRoot()->left->left->left is_not nullptr);
        assert(intTree.getRoot()->left->left->left->value to_be -3);
        assert(intTree.getRoot()->left->left->left->height to_be 1);
        assert(intTree.getRoot()->left->left->right is_not nullptr);
        assert(intTree.getRoot()->left->left->right->value to_be -1);
        assert(intTree.getRoot()->left->left->right->height to_be 1);
        assert(intTree.getRoot()->right->left is_not nullptr);
        assert(intTree.getRoot()->right->left->value to_be 3);
        assert(intTree.getRoot()->right->left->height to_be 1);
    }


    // remove
    {
        AVLTree<int> intTree;
        intTree.insert(6);
        intTree.insert(5);
        intTree.insert(7);
        intTree.insert(3);
        intTree.insert(8);
        intTree.insert(2);
        intTree.insert(9);
        intTree.insert(1);
        intTree.insert(10);

        // confirm pre condition
        assert(intTree.getRoot() is_not nullptr);
        expect(intTree.getRoot()->value to_be 6);
        expect(intTree.getRoot()->height to_be 4);
        assert(intTree.getRoot()->left is_not nullptr);
        expect(intTree.getRoot()->left->value to_be 3);
        expect(intTree.getRoot()->left->height to_be 3);
        assert(intTree.getRoot()->right is_not nullptr);
        expect(intTree.getRoot()->right->value to_be 8);
        expect(intTree.getRoot()->right->height to_be 3);
        assert(intTree.getRoot()->left->left is_not nullptr);
        expect(intTree.getRoot()->left->left->value to_be 2);
        expect(intTree.getRoot()->left->left->height to_be 2);
        assert(intTree.getRoot()->left->right is_not nullptr);
        expect(intTree.getRoot()->left->right->value to_be 5);
        expect(intTree.getRoot()->left->right->height to_be 1);
        assert(intTree.getRoot()->right->right is_not nullptr);
        expect(intTree.getRoot()->right->right->value to_be 9);
        expect(intTree.getRoot()->right->right->height to_be 2);
        assert(intTree.getRoot()->right->left is_not nullptr);
        assert(intTree.getRoot()->right->left->value to_be 7);
        assert(intTree.getRoot()->right->left->height to_be 1);
        assert(intTree.getRoot()->left->left->left is_not nullptr);
        assert(intTree.getRoot()->left->left->left->value to_be 1);
        assert(intTree.getRoot()->left->left->left->height to_be 1);
        assert(intTree.getRoot()->right->right->right is_not nullptr);
        assert(intTree.getRoot()->right->right->right->value to_be 10);
        assert(intTree.getRoot()->right->right->right->height to_be 1);


        // expect single right rotation
        intTree.remove(5);
        assert(intTree.getRoot() is_not nullptr);
        expect(intTree.getRoot()->value to_be 6);
        expect(intTree.getRoot()->height to_be 4);
        assert(intTree.getRoot()->left is_not nullptr);
        expect(intTree.getRoot()->left->value to_be 2);
        expect(intTree.getRoot()->left->height to_be 2);
        assert(intTree.getRoot()->right is_not nullptr);
        expect(intTree.getRoot()->right->value to_be 8);
        expect(intTree.getRoot()->right->height to_be 3);
        assert(intTree.getRoot()->left->left is_not nullptr);
        expect(intTree.getRoot()->left->left->value to_be 1);
        expect(intTree.getRoot()->left->left->height to_be 1);
        assert(intTree.getRoot()->left->right is_not nullptr);
        expect(intTree.getRoot()->left->right->value to_be 3);
        expect(intTree.getRoot()->left->right->height to_be 1);
        assert(intTree.getRoot()->right->right is_not nullptr);
        expect(intTree.getRoot()->right->right->value to_be 9);
        expect(intTree.getRoot()->right->right->height to_be 2);
        assert(intTree.getRoot()->right->left is_not nullptr);
        assert(intTree.getRoot()->right->left->value to_be 7);
        assert(intTree.getRoot()->right->left->height to_be 1);
        assert(intTree.getRoot()->right->right->right is_not nullptr);
        assert(intTree.getRoot()->right->right->right->value to_be 10);
        assert(intTree.getRoot()->right->right->right->height to_be 1);

        // expect single left rotation
        intTree.remove(7);
        assert(intTree.getRoot() is_not nullptr);
        expect(intTree.getRoot()->value to_be 6);
        expect(intTree.getRoot()->height to_be 3);
        assert(intTree.getRoot()->left is_not nullptr);
        expect(intTree.getRoot()->left->value to_be 2);
        expect(intTree.getRoot()->left->height to_be 2);
        assert(intTree.getRoot()->right is_not nullptr);
        expect(intTree.getRoot()->right->value to_be 9);
        expect(intTree.getRoot()->right->height to_be 2);
        assert(intTree.getRoot()->left->left is_not nullptr);
        expect(intTree.getRoot()->left->left->value to_be 1);
        expect(intTree.getRoot()->left->left->height to_be 1);
        assert(intTree.getRoot()->left->right is_not nullptr);
        expect(intTree.getRoot()->left->right->value to_be 3);
        expect(intTree.getRoot()->left->right->height to_be 1);
        assert(intTree.getRoot()->right->right is_not nullptr);
        expect(intTree.getRoot()->right->right->value to_be 10);
        expect(intTree.getRoot()->right->right->height to_be 1);
        assert(intTree.getRoot()->right->left is_not nullptr);
        assert(intTree.getRoot()->right->left->value to_be 8);
        assert(intTree.getRoot()->right->left->height to_be 1);
    }

    // contains
    {
        AVLTree<int> intTree;
        intTree.insert(2);
        intTree.insert(1);
        intTree.insert(3);

        expect(intTree.contains(0) to_be false);
        expect(intTree.contains(1) to_be true);
        expect(intTree.contains(2) to_be true);
        expect(intTree.contains(3) to_be true);
        expect(intTree.contains(4) to_be false);
    }


    // find_mind / find_max
    {
        AVLTree<int> intTree;
        intTree.insert(2);
        intTree.insert(1);
        intTree.insert(3);

        expect(intTree.find_max() to_be 3);
        expect(intTree.find_min() to_be 1);


        AVLTree<char> charTree;
        expect(charTree.is_empty() to_be true);
        expect_throw(charTree.find_min(), std::invalid_argument);
        expect_throw(charTree.find_max(), std::invalid_argument);
    }

    // copy constructor / copy assignment operator
    {
        AVLTree<int> intTree;
        intTree.insert(1);
        intTree.insert(2);
        intTree.insert(3);

        AVLTree<int> intTree2 = intTree;

        expect(intTree.getRoot() not_to_be intTree2.getRoot());
        assert(intTree2.getRoot() is_not nullptr);
        assert(intTree2.getRoot() is_not nullptr);
        assert(intTree2.getRoot()->left is_not nullptr);
        assert(intTree2.getRoot()->right is_not nullptr);
        expect(intTree2.getRoot()->value to_be 2);
        expect(intTree2.getRoot()->left->value to_be 1);
        expect(intTree2.getRoot()->right->value to_be 3);

        
        AVLTree<int> intTree3;
        intTree3.insert(1);
        intTree3.insert(2);
        intTree3.insert(3);

        AVLTree<int> intTree4;
        intTree4.insert(1);
        intTree4 = intTree3;

        expect(intTree3.getRoot() not_to_be intTree4.getRoot());
        assert(intTree3.getRoot() is_not nullptr);
        assert(intTree3.getRoot() is_not nullptr);
        assert(intTree3.getRoot()->left is_not nullptr);
        assert(intTree3.getRoot()->right is_not nullptr);
        expect(intTree3.getRoot()->value to_be 2);
        expect(intTree3.getRoot()->left->value to_be 1);
        expect(intTree3.getRoot()->right->value to_be 3);
    }

    // print tree
    {
        AVLTree<int> intTree;
        intTree.insert(8);
        intTree.insert(4);
        intTree.insert(2);
        intTree.insert(10);
        
        // expect tree to look like this
        //     10
        //   8
        // 4
        //   2
        std::stringstream stream;
        intTree.print_tree(stream);
        expect(stream.str() to_be "    10\n  8\n4\n  2\n");
    }

    
    // validate parent pointers after inserting
    {
        AVLTree<int> intTree;
        intTree.insert(20);
        intTree.insert(17);
        intTree.insert(23);
        intTree.insert(14);
        intTree.insert(19);
        intTree.insert(18);
        intTree.insert(16);
        intTree.insert(12);
        intTree.insert(13);
        intTree.insert(15);
        intTree.insert(4);
        intTree.insert(3);
        intTree.insert(2);
        intTree.insert(25);

        expect(intTree.getRoot()->left->parent to_be intTree.getRoot());
        expect(intTree.getRoot()->left->left->parent to_be intTree.getRoot()->left);
        expect(intTree.getRoot()->left->right->parent to_be intTree.getRoot()->left);
        expect(intTree.getRoot()->left->right->left->parent to_be intTree.getRoot()->left->right);
        expect(intTree.getRoot()->left->left->left->parent to_be intTree.getRoot()->left->left);
        expect(intTree.getRoot()->left->left->right->parent to_be intTree.getRoot()->left->left);
        expect(intTree.getRoot()->left->left->left->left->parent to_be intTree.getRoot()->left->left->left);
        expect(intTree.getRoot()->left->left->left->right->parent to_be intTree.getRoot()->left->left->left);
        expect(intTree.getRoot()->right->parent to_be intTree.getRoot());
        expect(intTree.getRoot()->right->left->parent to_be intTree.getRoot()->right);
        expect(intTree.getRoot()->right->right->parent to_be intTree.getRoot()->right);
        expect(intTree.getRoot()->right->right->left->parent to_be intTree.getRoot()->right->right);
        expect(intTree.getRoot()->right->right->right->parent to_be intTree.getRoot()->right->right);

    }

    // validate parent pointers after removal
    {
        AVLTree<int> intTree;
        intTree.insert(20);
        intTree.insert(14);
        intTree.insert(27);

        // pre condition
        expect(intTree.getRoot()->parent to_be nullptr);
        expect(intTree.getRoot()->left->parent to_be intTree.getRoot());
        expect(intTree.getRoot()->right->parent to_be intTree.getRoot());
        // remove the parent
        intTree.remove(20);
        // post condition
        expect(intTree.getRoot()->value to_be 27);
        expect(intTree.getRoot()->parent to_be nullptr);
        expect(intTree.getRoot()->left->parent to_be intTree.getRoot());
        expect(intTree.getRoot()->left->parent->value to_be 27);
    }


    // iterator for loop test
    {
        std::vector<int> nums;
        std::vector<int> itnums;
        std::unordered_map<int, int> dupes; // want to insert N unique values
        AVLTree<int> intTree;

        int N = 200000;
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<int> dist(0, N * N);

        int i = 0;
        while (i < N) {
            int value = dist(gen);
            if (dupes.find(value) != dupes.end()) continue;
            dupes.insert({value, i});
            intTree.insert(value);
            nums.push_back(value);
            i++;
        }

        for (int n : intTree) {
            itnums.push_back(n);
        }

        std::sort(nums.begin(), nums.end()); // since the iterator gives in-order traversal
        expect(nums to_be itnums);

        std::vector<int> fwd;
        for (AVLTree<int>::iterator it = intTree.begin(); it != intTree.end(); it++) {
            fwd.push_back(*it);
        }
        expect(nums to_be fwd)

        std::vector<int> rev;
        for (AVLTree<int>::iterator it = intTree.end() - 1; it >= intTree.begin(); it--) {
            rev.push_back(*it);
        }
        std::sort(nums.begin(), nums.end(), std::greater<int>{});
        expect(nums to_be rev);
    }

    // iterator operators
    {
        AVLTree<int> intTree;

        intTree.insert(10);
        intTree.insert(5);
        intTree.insert(15);
        intTree.insert(7);
        intTree.insert(13);
        intTree.insert(8);
        intTree.insert(12);
        intTree.insert(3);
        intTree.insert(17);

        expect(*(intTree.begin()) to_be intTree.find_min());
        expect(*(intTree.begin()) to_be intTree.getRoot()->left->left->left->value);
        expect(*(intTree.begin() + 1) to_be intTree.getRoot()->left->left->value);
        expect(*(intTree.begin() + 2) to_be intTree.getRoot()->left->value);
        expect(*(intTree.begin() + 3) to_be intTree.getRoot()->left->right->value);
        expect(*(intTree.begin() + 4) to_be intTree.getRoot()->value);
        expect(*(intTree.begin() + 5) to_be intTree.getRoot()->right->left->value);
        expect(*(intTree.begin() + 6) to_be intTree.getRoot()->right->value);
        expect(*(intTree.begin() + 7) to_be intTree.getRoot()->right->right->value);
        expect(*(intTree.begin() + 8) to_be intTree.getRoot()->right->right->right->value);
        expect(*(intTree.begin() + 8) to_be intTree.find_max());

        expect(*(intTree.end() - 1) to_be intTree.find_max());
        expect(*(intTree.end() - 1) to_be intTree.getRoot()->right->right->right->value);
        expect(*(intTree.end() - 2) to_be intTree.getRoot()->right->right->value);
        expect(*(intTree.end() - 3) to_be intTree.getRoot()->right->value);
        expect(*(intTree.end() - 4) to_be intTree.getRoot()->right->left->value);
        expect(*(intTree.end() - 5) to_be intTree.getRoot()->value);
        expect(*(intTree.end() - 6) to_be intTree.getRoot()->left->right->value);
        expect(*(intTree.end() - 7) to_be intTree.getRoot()->left->value);
        expect(*(intTree.end() - 8) to_be intTree.getRoot()->left->left->value);
        expect(*(intTree.end() - 9) to_be intTree.getRoot()->left->left->left->value);
        expect(*(intTree.end() - 9) to_be intTree.find_min());

        auto it = intTree.begin();
        expect(*it to_be intTree.find_min());
        it += 1;
        expect(*it to_be intTree.getRoot()->left->left->value);
        it += 1;
        expect(*it to_be intTree.getRoot()->left->value);
        it += 2;
        expect(*it to_be intTree.getRoot()->value);
        it += 3;
        expect(*it to_be intTree.getRoot()->right->right->value);


        auto it2 = intTree.end();
        it2 -= 1;
        expect(*it2 to_be intTree.find_max());
        it2 -= 1;
        expect(*it2 to_be intTree.getRoot()->right->right->value);
        it2 -= 1;
        expect(*it2 to_be intTree.getRoot()->right->value);
        it2 -= 2;
        expect(*it2 to_be intTree.getRoot()->value);
        it2 -= 3;
        expect(*it2 to_be intTree.getRoot()->left->left->value);

    }



    /*
    // commented out as .min and .max are meant to be private.
    // goal is to confirm min/max are properly being updated after a series of inserts/removes
    // at the cost of some overhead on every insert and remove. But the purpose of these min/max pointers
    // is for the iterator, not to speed up find_min/max. It is simply a side effect.
    // a typical traversal method find_min/max() is meant to be used in order to prove correctness.
    // I make find_min and find_max constant time by simply returning the value held by these pointers
    {
        AVLTree<int> intTree; // we remove only the min and max 
        AVLTree<int> intTree2; // we remove random elements 
        std::unordered_map<int, int> nums; // used to ensure N elements are inserted. faster way to check for dupes
        std::vector<int> random; // for intTree2, will be used to select a node at random to delete
        int N = 100000; // input size
        // to generate pseudo-random numbers:
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<int> dist(0, N * N);


        int i = 0;
        while (i < N) {
            int value = dist(gen);
            if (nums.find(value) != nums.end()) continue; // we want only unique values to ensure N values get inserted
            nums.insert({value, i});
            random.push_back(value);
            intTree.insert(value);
            intTree2.insert(value);
            expect(intTree.min->value to_be intTree.find_min());
            expect(intTree.max->value to_be intTree.find_max());
            i++;
        }

        for (int i = 0; i < N - 1; i++) {
            // we flip a coin to determine if we should remove the max or the min value
            // then we confirm that the min and max pointers were correctly adjusted
            // I neglect to remove any node that is not actually a min or max node as 
            // the min or max pointers (should not and is tested below) are never
            // affected by such a removal
            int flip = dist(gen) % 2;
            int value = flip ? intTree.find_min() : intTree.find_max();
            intTree.remove(value);
            expect(intTree.min->value to_be intTree.find_min());
            expect(intTree.max->value to_be intTree.find_max());
        }
        intTree.remove(intTree.find_min());
        expect(intTree.min to_be nullptr);
        expect(intTree.max to_be nullptr);


        // this test is rather slow because of the vector operations
        for (int i = 0; i < N - 1; i++) {
            // here we select a random node, which may or may not be the min or max
            // a removal of random nodes would have the potential to mess up parent links
            // which would in turn cause min and max ptrs to mess up
            // a series of random deletions will test for this

            int index = dist(gen) % random.size();
            int value = random.at(index);
            intTree2.remove(value);
            random.erase(random.begin() + index);
            expect(intTree2.min->value to_be intTree2.find_min());
            expect(intTree2.max->value to_be intTree2.find_max());
        }
        intTree2.remove(intTree2.find_min());
        expect(intTree2.min to_be nullptr);
        expect(intTree2.max to_be nullptr);
    }
    */
}

