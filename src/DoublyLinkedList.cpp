#include "DoublyLinkedList.h"
#include "TextBuffer.h"
#include <stdexcept>
#include <sstream>

template <typename T>
DoublyLinkedList<T>::DoublyLinkedList() : length(0)
{
    head = new Node(); // dummy head
    tail = new Node(); // dummy tail
    head->next = tail;
    tail->prev = head;
}

// TODO implement DoublyLinkedList
template <typename T>
DoublyLinkedList<T>::~DoublyLinkedList(){
    // Delete tất cả các nút
    Node *cur = head->next;
    while (cur != tail)
    {
        Node *next = cur->next;
        delete cur;
        cur = next;
    }
    
    // Delete 2 nút đầu cuối
    delete head;
    delete tail;
}

template <typename T>
void DoublyLinkedList<T>::insertAtHead(T data){
    Node* newNode = new Node(data, head, head->next);
    head->next->prev = newNode;
    head->next = newNode;
    length++;
}

template <typename T>
void DoublyLinkedList<T>::insertAtTail(T data){
    Node* newNode = new Node(data, tail->prev, tail);
    tail->prev->next = newNode;
    tail->prev = newNode;
    length++;
}

template <typename T>
void DoublyLinkedList<T>::insertAt(int index, T data){
    if (index < 0 || index > length) {
        throw std::out_of_range("Index is invalid!");
    }

    if (index == 0) {
        insertAtHead(data);
        return;
    }

    if (index == length) {
        insertAtTail(data);
        return;
    }

    Node* nextNode = getNode(index);
    Node* prevNode = nextNode->prev;
    Node* newNode = new Node(data, prevNode, nextNode);
    prevNode->next = newNode;
    nextNode->prev = newNode;
    length++;
}

template <typename T>
void DoublyLinkedList<T>::deleteAt(int index) {
    if (index < 0 || index >= length) {
        throw std::out_of_range("Index is invalid!");
    }
    Node* node = getNode(index);
    node->prev->next = node->next;
    node->next->prev = node->prev;
    delete node;
    length--;
}

template <typename T>
int DoublyLinkedList<T>::size() const {
    return length;
}

template <typename T>
bool DoublyLinkedList<T>::contains(T item) const {
    return indexOf(item) != -1;
}

template <typename T>
int DoublyLinkedList<T>::indexOf(T item) const {
    Node* cur = head->next;
    int idx = 0;
    while (cur != tail) {
        if (cur->data == item) return idx;
        cur = cur->next;
        idx++;
    }
    return -1;
}

template <typename T>
T& DoublyLinkedList<T>::get(int index) const {
    if (index < 0 || index >= length) 
        throw std::out_of_range("Index is invalid!");
    return getNode(index)->data;
}

template <typename T>
void DoublyLinkedList<T>::reverse() {
    if(length <= 1) return;
    if (head->next == tail) { 
        return;
    }

    Node* current = head->next;
    Node* temp = nullptr;

    while (current != tail) {
        temp = current->next;
        current->next = current->prev;
        current->prev = temp;
        current = temp;
    }

    Node* oldHeadNode = head->next;
    Node* oldTailNode = tail->prev;

    head->next = oldTailNode;
    oldTailNode->prev = head;

    tail->prev = oldHeadNode;
    oldHeadNode->next = tail;
}

template <typename T>
string DoublyLinkedList<T>::toString(string (*convert2str)(T &)) const {
    stringstream ss;
    ss << "[";
    Node* cur = head->next;
    while (cur != tail) {
        if (convert2str) ss << convert2str(cur->data);
        else             ss << cur->data;  
        if (cur->next != tail) ss << ", "; // thêm dấu phẩy dô nếu chưa phải cuối
        cur = cur->next;
    }
    ss << "]";
    return ss.str();
}


// TODO

// Explicit template instantiation for char, string, int, double, float, Point, and Action
template class DoublyLinkedList<char>;
template class DoublyLinkedList<string>;
template class DoublyLinkedList<int>;
template class DoublyLinkedList<double>;
template class DoublyLinkedList<float>;
template class DoublyLinkedList<Point>;
template class DoublyLinkedList<TextBuffer::HistoryManager::Action>;