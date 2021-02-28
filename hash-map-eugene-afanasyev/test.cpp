#define CATCH_CONFIG_MAIN

#include <iostream>
#include <unordered_map>
#include "hash_map.hpp"
#include "catch.hpp"

using namespace fefu;

TEST_CASE("allocator constructors") {
    allocator<uint32_t> a;
    allocator<uint64_t> b(a);
    auto ptr1 = a.allocate(1);
    auto ptr2 = b.allocate(1);
    REQUIRE(sizeof(*ptr1) == 4);
    REQUIRE(sizeof(*ptr2) == 8);
    a.deallocate(ptr1, 1);
    b.deallocate(ptr2, 1);
}

TEST_CASE("memory allocates and deallocates") {
    {
        allocator<int> int_allocator;
        int *int_ptr = int_allocator.allocate(1);
        *int_ptr = 1;
        REQUIRE_FALSE(int_ptr == nullptr);
        REQUIRE(sizeof(int_ptr) == 8u);
        REQUIRE(sizeof(*int_ptr) == 4u);
        REQUIRE(*int_ptr == 1);
        int_allocator.deallocate(int_ptr, 1);
    }
    {
        allocator<unsigned long long> ull_allocator;
        size_t n = 3;
        auto ull_ptr = ull_allocator.allocate(n);
        ull_ptr[0] = 0;
        ull_ptr[1] = 1;
        ull_ptr[2] = 2;
        REQUIRE(ull_ptr[0] == 0);
        REQUIRE(ull_ptr[1] == 1);
        REQUIRE(ull_ptr[2] == 2);
        ull_allocator.deallocate(ull_ptr, n);
    }
}

TEST_CASE("hash_map constructor and copy/move constructor") {
    {
        hash_map<std::string, float> hm(16);
        REQUIRE(hm.bucket_count() == 16 );
        REQUIRE(hm.load_factor() == 0);
        REQUIRE(hm.max_load_factor() == 0.75);
    }

    {
        hash_map<int, bool> hm;
        REQUIRE(hm.bucket_count() == 0);
        REQUIRE(hm.load_factor() == 0);
        REQUIRE(hm.max_load_factor() == 0.75);
    }

    {
        hash_map<std::string, bool> hm(4095);
        REQUIRE(hm.bucket_count() == 4096);
        REQUIRE(hm.load_factor() == 0);
        REQUIRE(hm.max_load_factor() == 0.75);
    }

    {
        typename hash_map<int, char>::allocator_type hm_alloc;
        hash_map<int, char> hm(hm_alloc);
        REQUIRE(hm.bucket_count() == 0);
        REQUIRE(hm.load_factor() == 0);
        REQUIRE(hm.max_load_factor() == 0.75);
        hash_map<int, char> hms({{1, 'f'}, {2, 's'}, {100, 'g'}, {5, 'f'}, {110, 's'}, {5, 's'}}, 8);
        hash_map<int, char> hm2(hms, hm_alloc);
        REQUIRE(hm2.bucket_count() == 8);
        REQUIRE(hm2.size() == 5);
        REQUIRE(hm2.load_factor() == 5.0f/8.0f);
        REQUIRE(hm2.max_load_factor() == 0.75);
        hash_map<int, char> hm3({{1, 'f'}, {2, 's'}, {100, 'g'}, {5, 'f'}, {110, 's'}, {5, 's'}}, hm_alloc);
        REQUIRE(hm3.bucket_count() == 8);
        REQUIRE(hm3.size() == 5);
        REQUIRE(hm3.load_factor() == 5.0f/8.0f);
        REQUIRE(hm3.max_load_factor() == 0.75);
    }

    {
        std::unordered_map<int, char> umap({{1, 'a'}, {2, 'b'}, {3, 'c'}});
        hash_map<int, char> hm(umap.begin(), umap.end());
        REQUIRE(hm.bucket_count() == 4);
        REQUIRE(hm.size() == 3);
        REQUIRE(hm.load_factor() == 0.75);
        REQUIRE(hm.max_load_factor() == 0.75f);
        hash_map<int, char> hm2(umap.begin(), umap.end(), 128);
        REQUIRE(hm2.bucket_count() == 128);
        REQUIRE(hm2.size() == 3);
        REQUIRE(hm2.max_load_factor() == 0.75f);
        hash_map<int, char> hm3(hm);
        REQUIRE(hm3.bucket_count() == 4);
        REQUIRE(hm3.size() == 3);
        REQUIRE(hm3.load_factor() == 0.75f);
        REQUIRE(hm3.max_load_factor() == 0.75f);
        hash_map<int, char> hm4(hm2);
        REQUIRE(hm4.bucket_count() == 128);
        REQUIRE(hm4.size() == 3);
        REQUIRE(hm4.load_factor() == static_cast<double>(hm4.size()) / static_cast<double>(hm4.bucket_count()));
        REQUIRE(hm4.max_load_factor() == 0.75f);

    }
    {
        hash_map<int, char> hms({{1, 'f'}, {2, 's'}, {100, 'g'}, {5, 'f'}, {110, 's'}, {5, 's'}}, 8);
        hash_map<int, char> hm(std::move(hms));
        REQUIRE(hm.bucket_count() == 8);
        REQUIRE(hm.size() == 5);
        REQUIRE(hm.load_factor() == static_cast<double>(hm.size()) / static_cast<double>(hm.bucket_count()));
        REQUIRE(hm.max_load_factor() == 0.75f);
    }
}

TEST_CASE("hash map copy assignment") {
    {
        hash_map<int, char> hms({{1, 'f'}, {3, 's'}, {100, 'g'}, {100, 'f'}, {110, 's'}});
        REQUIRE(hms.bucket_count() == 8);
        REQUIRE(hms.size() == 4);
        REQUIRE(hms.load_factor() == static_cast<double>(hms.size()) / static_cast<double>(hms.bucket_count()));
        REQUIRE(hms.max_load_factor() == 0.75f);
        hash_map<int, char> hm;
        hm = hms;
        REQUIRE(hm.bucket_count() == 8);
        REQUIRE(hm.size() == 4);
        REQUIRE(hm.load_factor() == static_cast<double>(hm.size()) / static_cast<double>(hm.bucket_count()));
        REQUIRE(hm.max_load_factor() == 0.75f);
        auto it = hm.find(1);
        REQUIRE(it->first == 1);
        REQUIRE(it->second == 'f');
        it = hm.find(3);
        REQUIRE(it->first == 3);
        REQUIRE(it->second == 's');
        it = hm.find(100);
        REQUIRE(it->first == 100);
        REQUIRE(it->second == 'g');
        it = hm.find(110);
        REQUIRE(it->first == 110);
        REQUIRE(it->second == 's');
        hash_map<int, char> tmp{{33, 'a'}, {44, 'b'}};
        hm = std::move(tmp);
        it = hm.begin();
        REQUIRE(it->first == 44);
        REQUIRE(it->second == 'b');
        it++;
        REQUIRE(it->first == 33);
        REQUIRE(it->second == 'a');
        it++;
        REQUIRE(it == hm.end());
    }

    {
        hash_map<std::string, int> hm(1);
        hm = {{"odin", 1}, {"dva", 2}, {"dva plus dva", 3}};
        auto it = hm.find("odin");
        REQUIRE(it->second == 1);
        it = hm.find("dva");
        REQUIRE(it->second == 2);
        it = hm.find("dva plus dva");
        REQUIRE(it->second == 3);
        it = hm.find("desyat");
        REQUIRE(it == hm.end());
    }
}

TEST_CASE("fefu::iterator/const_iterator") {
    {
        hash_map<std::string, float> hm;
        auto it = hm.begin();
        REQUIRE(it == hm.end());

        auto cit = hm.cbegin();
        REQUIRE(cit == hm.cend());
    }

    {
        hash_map<int, double> hm(3);
        REQUIRE(hm.begin() == hm.end());
        REQUIRE(hm.cbegin() == hm.cend());
    }
}

//TEST_CASE("hash_map additional hash function") {
//    // function
//    auto buckets = (1 << 20) - 1;
//    auto int_add_hash = [buckets](unsigned value) -> size_t {
//        return (std::hash<int>{}(value) % buckets)  | 1u;
//    };
//
//    {
//        for (size_t i = 0; i < buckets; i++) {
//            auto hash = int_add_hash(i);
//            REQUIRE(hash > 0);
//            REQUIRE(hash <= buckets);
//        }
//    }
//
//    auto string_add_hash = [buckets](std::string value) -> size_t {
//        return (std::hash<std::string>{}(value) % buckets)  | 1u;
//    };
//
//    for (size_t i = 0; i < buckets; i++) {
//        auto hash = string_add_hash(std::to_string(i));
//        REQUIRE(hash > 0);
//        REQUIRE(hash <= buckets);
//    }
//}

TEST_CASE("hash map rehash and load factor") {
    {
        hash_map<int, unsigned> hm;
        REQUIRE(hm.max_load_factor() == 0.75f);
        REQUIRE(hm.bucket_count() == 0);
        hm.rehash(200);
        REQUIRE(hm.bucket_count() == 256);
        hm.rehash(10);
        REQUIRE(hm.bucket_count() == 16);
        REQUIRE(hm.max_load_factor() == 0.75f);
        hm.max_load_factor(10);
        REQUIRE(hm.max_load_factor() == 0.75f);
        hm.max_load_factor(0.4f);
        REQUIRE(hm.max_load_factor() == 0.4f);
    }
}

TEST_CASE("hash_map insertion") {
    {
        hash_map<int, unsigned> hm;
        REQUIRE(hm.empty());
        auto it = hm.insert({1, 1});
        REQUIRE(it.second == true);
        auto iterator = it.first;
        REQUIRE(hm.begin() != hm.end());
        REQUIRE(iterator->first == 1);
        REQUIRE(iterator->second == 1);
        REQUIRE(!hm.empty());
        hash_map<int, unsigned>::key_type key = 10;
        it = hm.insert({10, 2});
        REQUIRE(it.second == true);
        REQUIRE(it.first->first == 10);
        REQUIRE(it.first->second == 2);
        REQUIRE(hm.bucket_count() == 2);
        REQUIRE(hm.size() == 2);
    }
    {
        hash_map<int, int> hm(2);
        REQUIRE(hm.empty());
        REQUIRE(hm.bucket_count() == 2);
        auto it1 = hm.insert({1, 1});
        REQUIRE(!hm.empty());
        auto it2 = hm.insert({1, 3});
        REQUIRE(hm.bucket_count() == 2);
        REQUIRE(it1.second == true);
        REQUIRE(it2.second == false);
        REQUIRE(hm.insert({2, 3}).second);
        hm.insert({10, 1}).second;
        REQUIRE(hm.bucket_count() == 4);
        auto it3 = hm.begin();
        REQUIRE(hm.begin()->first == 1);
        REQUIRE(hm.begin()->second == 1);
        hash_map<int,int> hm2;
        hm2.insert(hm.begin(), hm.end());
        auto it = hm2.begin();
        REQUIRE(it->first == 1);
        REQUIRE(it->second == 1);
        it++;
        REQUIRE(it->first == 2);
        REQUIRE(it->second == 3);
        it++;
        REQUIRE(it->first == 10);
        REQUIRE(it->second == 1);
    }

    {
        hash_map<int, char> hm;
        REQUIRE(hm.max_size() == std::allocator_traits<hash_map<int, char>::allocator_type>::max_size(hm.get_allocator()));
        hm.insert({{1, 's'}, {2, 'f'}, {3, 'a'}});
        auto it = hm.find(1);
        REQUIRE(it->first == 1);
        REQUIRE(it->second == 's');
        it++;
        REQUIRE(it->first == 2);
        REQUIRE(it->second == 'f');
        it++;
        REQUIRE(it->first == 3);
        REQUIRE(it->second == 'a');
    }
}

TEST_CASE("hash map insert or assign") {
    {
        hash_map<int, char> hm;
        hm.insert({30, 's'});
        REQUIRE(hm.begin()->first == 30);
        REQUIRE(hm.begin()->second == 's');
        auto it = hm.insert_or_assign(30, 'y');
        REQUIRE(it.second == false);
        REQUIRE(hm.begin()->first == 30);
        REQUIRE(hm.begin()->second == 'y');
        it = hm.insert_or_assign(40, 's');
        REQUIRE(it.second == true);
        REQUIRE(it.first->first == 40);
        REQUIRE(it.first->second == 's');
        hash_map<int, char>::key_type key = 10;
        it = hm.insert_or_assign(key, 'u');
        REQUIRE(it.second == true);
        REQUIRE(it.first->first == 10);
        REQUIRE(it.first->second == 'u');
    }
}

TEST_CASE("hash map find, count and contains") {
    {
        hash_map<std::string, int> hm;
        hm.insert({"aaa", 1});
        hm.insert({"baa", 11});
        hm.insert({"caa", 12});
        hm.insert({"daa", 13});
        auto it = hm.find("aaa");
        REQUIRE(it->first == "aaa");
        REQUIRE(it->second == 1);
        REQUIRE(hm.find("foo") == hm.end());
        hash_map<std::string, int>::const_iterator cit = hm.find("daa");
        REQUIRE(cit->first == "daa");
        REQUIRE(cit->second == 13);
        hash_map<std::string, int>::key_type key = "caa";
        it = hm.find(key);
        REQUIRE(it->second == 12);

        REQUIRE(hm.count("aaa") == 1);
        REQUIRE(hm.count("baa") == 1);
        REQUIRE(hm.count("caa") == 1);
        REQUIRE(hm.count("daa") == 1);

        REQUIRE(hm.contains("aaa"));
        REQUIRE(hm.contains("baa"));
        REQUIRE(hm.contains("caa"));
        REQUIRE(hm.contains("daa"));
    }
}

TEST_CASE("hash map erase") {
    {
        hash_map<std::string, int> hm;
        hm.insert({"aaa", 1});
        hm.insert({"baa", 3});
        hm.insert({"caa", 5});
        hm.insert({"daa", 10});
        REQUIRE(hm.size() == 4);
        REQUIRE(hm.erase("sss") == 0);
        REQUIRE(hm.erase("baa") == 1);
        REQUIRE(hm.size() == 3);
        REQUIRE(hm.find("baa") == hm.end());
        REQUIRE(hm.erase("baa") == 0);
    }
    {
        hash_map<int, char> hm({{1, '1'}, {2, '2'}, {3, '3'}});
        REQUIRE(hm.size() == 3);
        hm.erase(hm.begin(), hm.end());
        REQUIRE(hm.size() == 0);
        REQUIRE(hm.find(1) == hm.end());
    };
}

TEST_CASE("hash map emplace") {
    {
        hash_map<int, std::string> hm;
        REQUIRE(hm.size() == 0);
        REQUIRE(hm.bucket_count() == 0);
        auto it = hm.emplace(1, "aa");
        REQUIRE(it.second == true);
        REQUIRE(it.first->second == "aa");
        REQUIRE(hm.size() == 1);
        REQUIRE(hm.bucket_count() == 1);
        it = hm.emplace(1, "BBB");
        REQUIRE(it.second == false);
        REQUIRE(hm.size() == 1);
        REQUIRE(hm.bucket_count() == 1);
        it = hm.emplace(10, "fff");
        REQUIRE(it.second == true);
        REQUIRE(it.first->second == "fff");
        REQUIRE(hm.size() == 2);
        REQUIRE(hm.bucket_count() == 2);
    }
}

TEST_CASE("hash map try_emplace") {
    {
        hash_map<std::string, std::string> hm;
        hm.try_emplace("a", "a");
        hm.try_emplace("b", "abcd");
        hm.try_emplace("c", 10, 'c');
        hm.try_emplace("c", "Won't be inserted");
        REQUIRE(hm.size() == 3);
        REQUIRE(hm.bucket_count() == 4);
        auto it = hm.find("a");
        REQUIRE(it->second == "a");
        it = hm.find("b");
        REQUIRE(it->second == "abcd");
        it = hm.find("c");
        REQUIRE(it->second == std::string(10, 'c'));
        auto str = std::string(100, 'f');
        auto it1 = hm.try_emplace("c", str);
        REQUIRE(it1.second == false);
        REQUIRE(hm.size() == 3);
        REQUIRE(hm.bucket_count() == 4);
        it1 = hm.try_emplace(str, "c");
        it = hm.find(std::string(100, 'f'));
        REQUIRE(it != hm.end());
        REQUIRE(it->second == "c");
        REQUIRE(it1.second == true);
        REQUIRE(hm.size() == 4);
        REQUIRE(hm.bucket_count() == 8);
    }
}

TEST_CASE("hash map subscript operator") {
    {
        hash_map<char, int> hm;
        hm.insert({'a', 1});
        hm.insert({'b', 10});
        hm.insert({'c', 3});
        hm.insert({'d', 11});
        REQUIRE(hm['a'] == 1);
        REQUIRE(hm['b'] == 10);
        REQUIRE(hm['c'] == 3);
        REQUIRE(hm['d'] == 11);
        REQUIRE(hm.size() == 4);
        REQUIRE(hm.bucket_count() == 8);
        REQUIRE(!hm.contains('f'));
        hm['f'] = 90;
        REQUIRE(hm['f'] == 90);
        REQUIRE(hm.contains('f'));
        REQUIRE(hm.size() == 5);
        hash_map<char, int>::key_type g = 'g';
        hm[g] = 100;
        REQUIRE(hm.contains(g));
        REQUIRE(hm[g] == 100);
        REQUIRE(hm.size() == 6);
    }

    {
        std::initializer_list<std::pair<const int, int>> l{{1, 2}, {3, 4}};
        hash_map<int, int> m1(l.begin(), l.end(), 2048);
        hash_map<int, int> m2(std::move(m1));
        REQUIRE(m2[1] == 2);
        REQUIRE(m2[3] == 4);
    }
}

TEST_CASE("hash map at") {
    {
        hash_map<char, int> hm;
        hm.insert({'a', 1});
        hm.insert({'b', 10});
        hm.insert({'c', 3});
        hm.insert({'d', 11});
        hm.insert({'f', 18});
        REQUIRE(hm.at('a') == 1);
        REQUIRE(hm.at('b') == 10);
        REQUIRE(hm.at('c') == 3);
        REQUIRE(hm.at('d') == 11);
        REQUIRE(hm.at('f') == 18);
        try {
            const hash_map<char, int>::mapped_type mt = hm.at('u');
        } catch (const std::out_of_range& e) {
            REQUIRE(e.what() == std::string("out of range"));
        }
        hm.erase('d');
        try {
            const hash_map<char, int>::mapped_type mt = hm.at('d');
        } catch (const std::out_of_range& e) {
            REQUIRE(e.what() == std::string("out of range"));
        }

        const hash_map<char, int>::key_type f = 'f';
        const hash_map<char, int>::mapped_type mt(hm.at(f));
        REQUIRE(mt == 18);
    }
}

TEST_CASE("hash map merge") {
    {
        std::unordered_map<int, char> umap1({{1, 'd'}, {8, 'e'}, {4, 'd'}, {3, 'f'}});
        std::unordered_map<int, char> umap2({{1, 'f'}, {2, 's'}, {3, 'd'}, {5, 's'}});
        umap1.merge(umap2);
        hash_map<int, char> hm1({{1, 'd'}, {8, 'e'}, {4, 'd'}, {3, 'f'}});
        hash_map<int, char> hm2({{1, 'f'}, {2, 's'}, {3, 'd'}, {5, 's'}});
        hm1.merge(hm2);

        REQUIRE(hm1.size() == umap1.size());
        for (auto& i : hm1) {
            auto it = umap1.find(i.first);
            REQUIRE(it != umap1.end());
            REQUIRE(it->second == i.second);
        }

        REQUIRE(hm2.size() == umap2.size());
        for (auto& i : hm2) {
            auto it = umap2.find(i.first);
            REQUIRE(it != umap2.end());
            REQUIRE(it->second == i.second);
        }
    }

    {
        std::unordered_map<int, std::string> umap1({{1, "odin"}, {2, "dva"}, {3, "tri"}, {4, "chetiri"}});
        std::unordered_map<int, std::string> umap2({{1, "odin"}, {2, "piat`"}, {6, "shest`"}});
        umap1.merge(umap2);
        hash_map<int, std::string> hm1({{1, "odin"}, {2, "dva"}, {3, "tri"}, {4, "chetiri"}});
        hash_map<int, std::string> hm2({{1, "odin"}, {2, "piat`"}, {6, "shest`"}});
        hm1.merge(std::move(hm2));

        REQUIRE(hm1.size() == umap1.size());
        for (auto& i : hm1) {
            auto it = umap1.find(i.first);
            REQUIRE(it != umap1.end());
            REQUIRE(it->second == i.second);
        }
    }
}

TEST_CASE("hash map operator==") {
    {
        hash_map<int, std::string> hm1({{1, "odin"}, {2, "dva"}, {3, "tri"}, {4, "chetiri"}});
        hash_map<int, std::string> hm2({{1, "odin"}, {2, "piat"}, {6, "shest"}});
        REQUIRE_FALSE(hm1 == hm2);
    }

    {
        hash_map<int, char> hm1({{1, 'd'}, {8, 'e'}, {4, 'd'}, {3, 'f'}});
        hash_map<int, char> hm2({{1, 'f'}, {2, 's'}, {3, 'd'}, {5, 's'}});
        REQUIRE_FALSE(hm1 == hm2);
    }

        {
        hash_map<int, std::string> hm1({{1, "odin"}, {2, "dva"}, {3, "tri"}, {4, "chetiri"}});
        hash_map<int, std::string> hm2({{1, "odin"}, {2, "dva"}, {3, "tri"}, {4, "chetiri"}});
        REQUIRE(hm1 == hm2);
    }

    {
        hash_map<int, std::string> hm1({{1, "odin"}, {2, "piat`"}, {6, "shest`"}});
        hash_map<int, std::string> hm2({{1, "odin"}, {2, "piat`"}, {6, "shest`"}});
        REQUIRE(hm1 == hm2);
    }

    {
        hash_map<int, char> hm1({{1, 'f'}, {2, 's'}, {3, 'd'}, {5, 's'}});
        hash_map<int, char> hm2({{1, 'f'}, {2, 's'}, {3, 'd'}, {5, 's'}});
        REQUIRE(hm1 == hm2);
    }
}