#ifndef LIST_H
#define LIST_H
#include <iostream>
#include <stdexcept>

template<typename T>
class List {
private:
    struct Node {
        T data;
        Node* next;
        Node() : data(), next(nullptr) {}
        Node(const T& value) : data(value), next(nullptr) {}
    };

    Node* head;
    size_t list_size;

public:
    class Iterator {
    private:
        Node* curr;
        friend class List;

    public:
        Iterator(Node* node = nullptr) : curr(node) {}

        T& operator*() { return curr->data; }
        T* operator->() { return &curr->data; }

        Iterator& operator++() {
            curr = curr->next;
            return *this;
        }

        Iterator operator++(int) {
            Iterator tmp = *this;
            ++(*this);
            return tmp;
        }

        bool operator==(const Iterator& other) const { return curr == other.curr; }
        bool operator!=(const Iterator& other) const { return curr != other.curr; }
    };

    List() : head(nullptr), list_size(0) {}
    ~List() { clear(); }

    T& front() {
        if (empty()) throw std::out_of_range("List is empty");
        return head->data;
    }

    const T& front() const {
        if (empty()) throw std::out_of_range("List is empty");
        return head->data;
    }

    void push_front(const T& value) {
        Node* tmp = new Node(value);
        tmp->next = head;
        head = tmp;
        ++list_size;
    }

    void pop_front() {
        if (empty()) return;
        Node* tmp = head;
        head = head->next;
        delete tmp;
        --list_size;
    }

    void clear() {
        while (head != nullptr) {
            Node* tmp = head;
            head = head->next;
            delete tmp;
        }
        list_size = 0;
    }

    Iterator insert_after(Iterator pos, const T& value) {
        if (pos.curr == nullptr)
            throw std::out_of_range("Invalid iterator");

        Node* tmp = new Node(value);
        tmp->next = pos.curr->next;
        pos.curr->next = tmp;
        ++list_size;
        return Iterator(tmp);
    }

    Iterator erase_after(Iterator pos) {
        if (pos.curr == nullptr || pos.curr->next == nullptr)
            throw std::out_of_range("Invalid iterator");

        Node* tmp = pos.curr->next;
        pos.curr->next = tmp->next;
        delete tmp;
        --list_size;
        return Iterator(pos.curr->next);
    }

    bool empty() const { return head == nullptr; }
    size_t size() const { return list_size; }

    Iterator before_begin() { return Iterator(nullptr); }
    Iterator begin() { return Iterator(head); }
    Iterator end() { return Iterator(nullptr); }

    const Iterator begin() const { return Iterator(head); }
    const Iterator end() const { return Iterator(nullptr); }



    // 3 Задание на поиск k-ого элемента с конца списка за 1 проход



    T& findkelemfte(size_t k) {
        if (empty()) throw std::out_of_range("List is empty");

        Node* fast = head;
        Node* slow = head;

        for (size_t i = 0; i < k; ++i) {
            if (fast == nullptr)
                throw std::out_of_range("k > than list size");
            fast = fast->next;
        }

        while (fast != nullptr) {
            fast = fast->next;
            slow = slow->next;
        }

        return slow->data;
    }

};

#endif // LIST_H
