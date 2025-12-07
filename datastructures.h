#ifndef DATASTRUCTURES_H
#define DATASTRUCTURES_H

#include <QString>

// Custom Dynamic Array (NO STL)
template<typename T>
class DynamicArray {
private:
    T* arr;
    int cap;
    int sz;

    void resize() {
        cap = cap * 2;
        T* newArr = new T[cap];
        for(int i = 0; i < sz; i++) {
            newArr[i] = arr[i];
        }
        delete[] arr;
        arr = newArr;
    }

public:
    DynamicArray(int initialCap = 10) : cap(initialCap), sz(0) {
        arr = new T[cap];
    }

    ~DynamicArray() {
        delete[] arr;
    }

    void add(const T& val) {
        if(sz >= cap) resize();
        arr[sz++] = val;
    }

    T& get(int idx) { return arr[idx]; }
    int size() const { return sz; }
    void clear() { sz = 0; }
    T& operator[](int idx) { return arr[idx]; }

    // For sorting
    void sort() {
        // Bubble sort
        for(int i = 0; i < sz - 1; i++) {
            for(int j = 0; j < sz - i - 1; j++) {
                if(arr[j] > arr[j + 1]) {
                    T temp = arr[j];
                    arr[j] = arr[j + 1];
                    arr[j + 1] = temp;
                }
            }
        }
    }
};

// Hash Table for frequency counting
class FrequencyTable {
private:
    struct Node {
        unsigned char key;
        unsigned long long value;
        Node* next;
        Node(unsigned char k, unsigned long long v) : key(k), value(v), next(nullptr) {}
    };

    Node* table[256];
    int sz;

public:
    FrequencyTable() : sz(0) {
        for(int i = 0; i < 256; i++) {
            table[i] = nullptr;
        }
    }

    ~FrequencyTable() {
        for(int i = 0; i < 256; i++) {
            Node* curr = table[i];
            while(curr) {
                Node* temp = curr;
                curr = curr->next;
                delete temp;
            }
        }
    }

    void insert(unsigned char key, unsigned long long value) {
        int idx = key;
        Node* curr = table[idx];

        while(curr) {
            if(curr->key == key) {
                curr->value = value;
                return;
            }
            curr = curr->next;
        }

        Node* newNode = new Node(key, value);
        newNode->next = table[idx];
        table[idx] = newNode;
        sz++;
    }

    unsigned long long* get(unsigned char key) {
        int idx = key;
        Node* curr = table[idx];

        while(curr) {
            if(curr->key == key) {
                return &(curr->value);
            }
            curr = curr->next;
        }
        return nullptr;
    }

    void increment(unsigned char key) {
        unsigned long long* val = get(key);
        if(val) {
            (*val)++;
        } else {
            insert(key, 1);
        }
    }

    int size() const { return sz; }

    // Iterator-like access
    void getAllEntries(DynamicArray<unsigned char>& keys,
                       DynamicArray<unsigned long long>& values) {
        keys.clear();
        values.clear();
        for(int i = 0; i < 256; i++) {
            Node* curr = table[i];
            while(curr) {
                keys.add(curr->key);
                values.add(curr->value);
                curr = curr->next;
            }
        }
    }
};

// Code Table for Huffman codes
class CodeTable {
private:
    QString codes[256];
    bool exists[256];

public:
    CodeTable() {
        for(int i = 0; i < 256; i++) {
            exists[i] = false;
        }
    }

    void insert(unsigned char key, const QString& code) {
        codes[key] = code;
        exists[key] = true;
    }

    QString* get(unsigned char key) {
        if(exists[key]) {
            return &codes[key];
        }
        return nullptr;
    }

    bool contains(unsigned char key) const {
        return exists[key];
    }
};

// Forward declaration for HuffmanNode
struct HuffmanNode;

// Min Heap specifically for HuffmanNode pointers
class MinHeap {
private:
    HuffmanNode** heap;
    int cap;
    int sz;

    int parent(int i) { return (i - 1) / 2; }
    int leftChild(int i) { return 2 * i + 1; }
    int rightChild(int i) { return 2 * i + 2; }

    void swap(int i, int j) {
        HuffmanNode* temp = heap[i];
        heap[i] = heap[j];
        heap[j] = temp;
    }

    void heapifyUp(int idx);
    void heapifyDown(int idx);

public:
    MinHeap(int capacity = 256);
    ~MinHeap();

    void insert(HuffmanNode* val);
    HuffmanNode* extractMin();

    int size() const { return sz; }
    bool isEmpty() const { return sz == 0; }
};

#endif // DATASTRUCTURES_H
