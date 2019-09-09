#include <vector>
#include <cstdio>
#include <cassert>

int main() {
    // A vector of integers
    std::vector<int> my_vec = { 1, 2, 3, 4 };

    int element_2 = my_vec[2]; // Reading from vector
    assert(element_2 == 3);

    my_vec[3] = 4;             // Writing to vector
    assert(my_vec[3] == 4);

    // The vector `[]` operator is like array dereference: the caller must
    // check bounds. But there’s another call that always checks bounds.
    element_2 = my_vec.at(2);
    assert(element_2 == 3);
    my_vec.at(3) = 61;
    assert(my_vec[3] == 61);

    my_vec.size();         // Return number of elements
    assert(my_vec.size() == 4);

    my_vec.push_back(5);   // Adds element to the end of the vector
    assert(my_vec.size() == 5);
    assert(my_vec[4] == 5);

    my_vec.back();         // Return the last element of the vector (must not be empty)
    assert(my_vec.back() == 5);

    my_vec.pop_back();     // Remove element at the end
    assert(my_vec.size() == 4);

    my_vec.empty();        // Return true iff `size() == 0`
    assert(!my_vec.empty());

    my_vec.clear();        // Erases all elements
    assert(my_vec.empty());

    printf("Done!\n");
}
