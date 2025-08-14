#ifndef __DOUBLY_LINKED_LIST_H__
#define __DOUBLY_LINKED_LIST_H__

#include "main.h"

template <typename T>
class DoublyLinkedList
{
    // TODO: may provide some attributes
private:
    struct Node
    {
        T data;
        Node *prev;
        Node *next;
        Node() : prev(nullptr), next(nullptr) {}
        Node(const T &val, Node *prev = nullptr, Node *next = nullptr) : data(val), prev(prev), next(next) {}
    };

    Node *head; // Dummy head
    Node *tail; // Dummy tail
    int length;

public:
    DoublyLinkedList();
    ~DoublyLinkedList();

    void insertAtHead(T data);
    void insertAtTail(T data);
    void insertAt(int index, T data);
    void deleteAt(int index);
    T &get(int index) const;
    int indexOf(T item) const;
    bool contains(T item) const;
    int size() const;
    void reverse();
    string toString(string (*convert2str)(T &) = 0) const;

    //void mergeSort(bool (*cmp)(const T &, const T &));
    //Node *mergeSortHelper(Node *start, int n, bool (*cmp)(const T &, const T &));
    //Node *merge(Node *left, int lenL, Node *right, int lenR, bool (*cmp)(const T &, const T &));

    Node* getNode(int index) const{
    if (index < 0 || index >= length) {
        throw std::out_of_range("Index is invalid!");
    }
    Node* cur = head->next;
    for (int i = 0; i < index; ++i) {
        cur = cur->next;
    }
    return cur;
    };

    class Iterator
    {
    private:
        Node *current;
        const DoublyLinkedList<T> *list;
    public:
         Iterator(Node *node, const DoublyLinkedList<T> *list) : current(node), list(list) {}

        T &operator*() const
        {
            return current->data;
        }

        Iterator &operator++()
        {
            if(current) current = current->next;
            return *this;
        }

         Iterator operator++(int)
        {
            Iterator temp = *this;
            ++(*this);
            return temp;
        }

        Iterator &operator--()
        {
            current = current->prev;
            return *this;
        }

        Iterator operator--(int)
        {
            Iterator temp = *this;
            --(*this);
            return temp;
        }

        bool operator==(const Iterator &other) const
        {
            return current == other.current;
        }

        bool operator!=(const Iterator &other) const
        {
            return current != other.current;
        }
    };

    Iterator begin() const
    {
        return Iterator(head->next, this);
    }

    Iterator end() const
    {
        return Iterator(tail, this);
    }
};

#endif // __DOUBLY_LINKED_LIST_H__
