#include "datastructures.h"
#include "huffmancompressor.h"

// MinHeap implementation for huffman tree building

MinHeap::MinHeap(int capacity) : cap(capacity), sz(0) {
    heap = new HuffmanNode*[cap];
}

MinHeap::~MinHeap() {
    delete[] heap;
}

void MinHeap::heapifyUp(int idx) {
    // bubble up until heap property is satisfied
    while(idx > 0 && heap[idx]->frequency < heap[parent(idx)]->frequency) {
        swap(idx, parent(idx));
        idx = parent(idx);
    }
}

void MinHeap::heapifyDown(int idx) {
    int smallest = idx;
    int left = leftChild(idx);
    int right = rightChild(idx);

    // find smallest among node and its children
    if(left < sz && heap[left]->frequency < heap[smallest]->frequency)
        smallest = left;
    if(right < sz && heap[right]->frequency < heap[smallest]->frequency)
        smallest = right;

    // if smallest is not current node, swap and continue
    if(smallest != idx) {
        swap(idx, smallest);
        heapifyDown(smallest);
    }
}

void MinHeap::insert(HuffmanNode* val) {
    if(sz >= cap) return;  // heap full

    heap[sz] = val;
    heapifyUp(sz);
    sz++;
}

HuffmanNode* MinHeap::extractMin() {
    if(sz == 0) return nullptr;

    HuffmanNode* minVal = heap[0];
    heap[0] = heap[sz - 1];
    sz--;

    if(sz > 0) {
        heapifyDown(0);
    }

    return minVal;
}
