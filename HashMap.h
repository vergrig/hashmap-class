#ifndef HASHMAP
#define HASHMAP

#include <vector>
#include <list>
#include <initializer_list>
#include <stdexcept>
#include <iostream>

namespace HseAlgo {
    template<class KeyType, class ValueType, class Hash = std::hash<KeyType>> 
    class HashMap {
    public:
    	using pair = std::pair<const KeyType, ValueType>;
        using pair_list = std::list<pair>;
        using iterator = typename pair_list::iterator;
        using const_iterator = typename pair_list::const_iterator;
        using map_iter = typename std::list<iterator>::iterator;

    private:
        const size_t MIN_SPACE = 32;
    	Hash hash_f;
    	size_t sz = 0;
    	size_t space = MIN_SPACE;
        pair_list elems;
    	std::vector<std::list<iterator>> map = std::vector<std::list<iterator>>(space);

        void update() {
            if (sz > space * 2) {
                space *= 2;
                map.clear();
                map.resize(space);
                for(auto it = elems.begin(); it != elems.end(); ++it){
                    size_t pos = hash_f(it->first) % space;
                    map[pos].push_back(it);
                }
            }
        }

    public:
        HashMap(Hash _hash_f = Hash()) : hash_f(_hash_f) {
        }

        template<typename iterator>
        HashMap(iterator _begin, iterator _end, Hash _hash_f = Hash()) : hash_f(_hash_f) {
            while (_begin != _end) {
                insert(*_begin);
                ++_begin;
            }
        }

        HashMap(std::initializer_list<pair> base, Hash _hash_f = Hash()) : hash_f(_hash_f) {
            for (auto i : base) {
                insert(i);
            }
        }

        const HashMap& operator=(const HashMap& other) {
            clear();
            for (auto i : other) {
                insert(i);
            }
            return *this;
        }


        HashMap(const HashMap& other) {
            for (auto i : other) {
                insert(i);
            }
        }

        HashMap(HashMap &&other) = default;

        HashMap& operator = (HashMap &&other) noexcept = default;


        size_t size() const {
            return elems.size();
        }

        bool empty() const {
            return size() == 0;
        }

        Hash hash_function() const {
            return hash_f;
        }

        iterator insert(pair elem) {
        	update();
            size_t pos = hash_f(elem.first) % space;
            elems.push_back(elem);
            map[pos].push_back(--elems.end());
            ++sz;
            return *(map[pos].rbegin());
        }

        iterator find(const KeyType key) {
            size_t pos = hash_f(key) % space;
            if (map[pos].empty()) {
                return end();
            }
            auto it = map[pos].begin();
            while (it != map[pos].end()) {
                if ((*it)->first == key) {
                    return *(it);
                }
                ++it;
            }
            return end();
        }

        const_iterator find(const KeyType key) const {
                    size_t pos = hash_f(key) % space;
            if (map[pos].empty()) {
                return end();
            }
            auto it = map[pos].begin();
            while (it != map[pos].end()) {
                if ((*it)->first == key) {
                    return *(it);
                }
                ++it;
            }
            return end();
        }

        ValueType& operator[] (const KeyType key) {
        	iterator it = find(key);
            if (it == end()) {
                it = insert({key, ValueType()});
            }
            return it->second;
        }

        const ValueType& at(const KeyType key) const {
        	const_iterator it = find(key);
            if (it == end()) {
                throw std::out_of_range("Invalid key\n");
            }
            return it->second;
        }

        void erase(const KeyType key) {
            size_t pos = hash_f(key) % space;
            if (find(key) == end()) {
            	return;
            }
            auto it = map[pos].begin();
            while (it != map[pos].end()) {
                if ((*it)->first == key) {
                    elems.erase(*it);
                    map[pos].erase(it);
                    --sz;
                    return;
                } else {
                    ++it;
                }
            }
        }

        iterator begin() {
            return elems.begin();
        }

        const_iterator begin() const {
            return elems.begin();
        }

        iterator end() {
            return elems.end();
        }

        const_iterator end() const {
            return elems.end();
        }

        void clear() {
        	sz = 0;
        	space = MIN_SPACE;
            map.clear();
            map.resize(MIN_SPACE);
            elems.clear();
        }
    };
}

#endif
