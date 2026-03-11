#ifndef UNORDERED_TAB_H
#define UNORDERED_TAB_H

#include <vector>
#include <stdexcept>
#include <utility>

template <class TKey, class TVal>
class Unordered_tab {
private:
    std::vector<std::pair<TKey, TVal>> data;

public:
    class Iterator {
        typename std::vector<std::pair<TKey, TVal>>::iterator iter;
    public:
        explicit Iterator(typename std::vector<std::pair<TKey, TVal>>::iterator it) : iter(it) {}
        std::pair<TKey, TVal>& operator*() { return *iter; }
        std::pair<TKey, TVal>* operator->() { return &(*iter); }
        Iterator& operator++() {
            ++iter;
            return *this;
        }
        bool operator!=(const Iterator& other) const { return iter != other.iter; }
        bool operator==(const Iterator& other) const { return iter == other.iter; }
    };

    class ConstIterator {
        typename std::vector<std::pair<TKey, TVal>>::const_iterator iter;
    public:
        explicit ConstIterator(typename std::vector<std::pair<TKey, TVal>>::const_iterator it) : iter(it) {}
        const std::pair<TKey, TVal>& operator*() const { return *iter; }
        const std::pair<TKey, TVal>* operator->() const { return &(*iter); }
        ConstIterator& operator++() { ++iter; return *this; }
        bool operator!=(const ConstIterator& other) const { return iter != other.iter; }
        bool operator==(const ConstIterator& other) const { return iter == other.iter; }
    };

    bool empty() const { return data.empty();}
    size_t size()  const { return data.size(); }
    void clear() { data.clear(); }

    Iterator      begin()  { return Iterator(data.begin()); }
    Iterator      end()    { return Iterator(data.end());   }
    ConstIterator begin()  const { return ConstIterator(data.begin()); }
    ConstIterator end()    const { return ConstIterator(data.end());   }
    ConstIterator cbegin() const { return ConstIterator(data.cbegin()); }
    ConstIterator cend()   const { return ConstIterator(data.cend());   }

    Iterator find(const TKey& k) {
        for (auto it = data.begin(); it != data.end(); ++it)
            if (it->first == k) return Iterator(it);
        return end();
    }

    ConstIterator find(const TKey& k) const {
        for (auto it = data.begin(); it != data.end(); ++it)
            if (it->first == k) return ConstIterator(it);
        return end();
    }

    void insert(const std::pair<TKey, TVal>& p) {
        if (find(p.first) == end()) data.push_back(p);
    }

    size_t erase(const TKey& k) {
        for (auto it = data.begin(); it != data.end(); ++it) {
            if (it->first == k) { data.erase(it); return 1; }
        }
        return 0;
    }

    TVal& operator[](const TKey& k) {
        auto it = find(k);
        if (it != end()) return it->second;
        data.push_back(std::pair<TKey, TVal>{k, TVal{}});
        return data.back().second;
    }

    TVal& at(const TKey& k) {
        auto it = find(k);
        if (it == end()) throw std::out_of_range("Key not found");
        return it->second;
    }

    const TVal& at(const TKey& k) const {
        auto it = find(k);
        if (it == end()) throw std::out_of_range("Key not found");
        return it->second;
    }
};

#endif // UNORDERED_TAB_H
