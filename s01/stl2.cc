#include <unordered_map>
#include <string>
#include <cstdio>
#include <cassert>

int main() {
    // Map strings to integers
    std::unordered_map<std::string, int> my_map;

    // Insert without overwriting (leaves map unchanged if key present)
    my_map.insert({"one", 1});

    // Number of keys in map
    assert(my_map.size() == 1);
    assert(!my_map.empty());

    // Test if key is in map
    size_t exists = my_map.count("one");
    assert(exists == 1);

    exists = my_map.count("two");
    assert(exists == 0);

    // Find matching element; returns `my_map.end()` if not found
    auto it0 = my_map.find("one");
    assert(it0 != my_map.end());
    // Iterator points to a {key, value} pair
    assert(it0->first == "one");
    assert(it0->second == 1);

    auto it1 = my_map.find("two");
    assert(it1 == my_map.end());

    // Array syntax: insert or modify
    my_map["one"] = 61;               // Insert into map (with overwrite semantics)
    assert(my_map["one"] == 61);
    // But beware; array syntax inserts a default if not found!
    assert(my_map["two"] == 0);
    assert(my_map.size() == 2);
    assert(my_map.find("two") != my_map.end());

    // Remove key
    my_map.erase("two");
    assert(my_map.size() == 1);

    // Iterate in sorted order
    my_map.insert({"two", 2});
    my_map.insert({"three", 3});
    my_map.insert({"four", 4});
    my_map.insert({"five", 5});
    for (auto it = my_map.begin(); it != my_map.end(); ++it) {
        // `it->first` is the key, `it->second` the value
        // (`it->first.c_str()` transforms a C++ string to printf form)
        printf("Found %s -> %d\n", it->first.c_str(), it->second);
    }

    printf("Done!\n");
}
