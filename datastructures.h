#ifndef DATASTRUCTURES_H
#define DATASTRUCTURES_H

#include <QString>

// Dynamic Array class - because we cant use vector
// This is our own implementation
template<typename T>
class DynamicArray {
private:
    T* arr;
    int cap;  // capacity of array
    int sz;   // size currently used

    // function to resize array when it gets full
    void resize() {
        cap = cap * 2;  // double it
        T* newArr = new T[cap];
        // copy everything over
        for(int i = 0; i < sz; i++) {
            newArr[i] = arr[i];
        }
        delete[] arr;  // free old memory
        arr = newArr;
    }

public:
    // constructor with default capacity 10
    DynamicArray(int initialCap = 10) : cap(initialCap), sz(0) {
        arr = new T[cap];
    }

    // destructor to clean up
    ~DynamicArray() {
        delete[] arr;
    }

    // add new element at end
    void add(const T& val) {
        if(sz >= cap) resize();  // need more space
        arr[sz++] = val;
    }

    T& get(int idx) { return arr[idx]; }
    int size() const { return sz; }
    void clear() { sz = 0; }
    T& operator[](int idx) { return arr[idx]; }

    // bubble sort - simple but works fine for our purpose
    void sort() {
        // outer loop
        for(int i = 0; i < sz - 1; i++) {
            // inner loop for comparing
            for(int j = 0; j < sz - i - 1; j++) {
                if(arr[j] > arr[j + 1]) {
                    // swap them
                    T temp = arr[j];
                    arr[j] = arr[j + 1];
                    arr[j + 1] = temp;
                }
            }
        }
    }
};

// Hash table for frequency counting
// Using chaining in case of collisions
class FrequencyTable {
private:
    // node structure for linked list
    struct Node {
        unsigned char key;
        unsigned long long value;
        Node* next;
        Node(unsigned char k, unsigned long long v) : key(k), value(v), next(nullptr) {}
    };

    Node* table[256];  // 256 slots for all possible chars
    int sz;            // how many entries we have

public:
    FrequencyTable() : sz(0) {
        // initialize everything to null
        for(int i = 0; i < 256; i++) {
            table[i] = nullptr;
        }
    }

    // destructor - need to delete all nodes
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

    // insert or update a key-value pair
    void insert(unsigned char key, unsigned long long value) {
        int idx = key;  // hash function is just the key itself
        Node* curr = table[idx];

        // check if key already exists
        while(curr) {
            if(curr->key == key) {
                curr->value = value;  // update existing
                return;
            }
            curr = curr->next;
        }

        // key doesn't exist, add new node at front
        Node* newNode = new Node(key, value);
        newNode->next = table[idx];
        table[idx] = newNode;
        sz++;
    }

    // get value for a key
    unsigned long long* get(unsigned char key) {
        int idx = key;
        Node* curr = table[idx];

        while(curr) {
            if(curr->key == key) {
                return &(curr->value);
            }
            curr = curr->next;
        }
        return nullptr;  // not found
    }

    // increment counter for a character
    void increment(unsigned char key) {
        unsigned long long* val = get(key);
        if(val) {
            (*val)++;  // already exists, just increment
        } else {
            insert(key, 1);  // first time seeing this char
        }
    }

    int size() const { return sz; }

    // get all entries as two parallel arrays
    void getAllEntries(DynamicArray<unsigned char>& keys,
                       DynamicArray<unsigned long long>& values) {
        keys.clear();
        values.clear();
        // go through all slots
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

// Store huffman codes for each character
// Direct addressing - index is the character itself
class CodeTable {
private:
    QString codes[256];   // the huffman code for each char
    bool exists[256];     // track which ones we have

public:
    CodeTable() {
        // mark all as non-existent initially
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

// forward declaration
struct HuffmanNode;

// Min heap for building huffman tree
// Need to repeatedly get the two minimum frequency nodes
class MinHeap {
private:
    HuffmanNode** heap;  // array of pointers to nodes
    int cap;             // capacity
    int sz;              // current size

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
