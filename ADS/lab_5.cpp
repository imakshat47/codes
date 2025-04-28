#include <iostream>
#include <vector>
#include <stdexcept>

// ------------------ Unsorted Array Implementation ------------------
class PQArray {
    // stores pairs of (value, priority)
    std::vector<std::pair<int,int>> data_;
public:
    void push(int value, int priority) {
        data_.push_back(std::make_pair(value, priority));
    }
    std::pair<int,int> top() const {
        if (data_.empty()) throw std::runtime_error("PQArray::top(): empty");
        int best = 0;
        for (int i = 1; i < static_cast<int>(data_.size()); ++i) {
            if (data_[i].second > data_[best].second) {
                best = i;
            }
        }
        return data_[best];
    }
    void pop() {
        if (data_.empty()) throw std::runtime_error("PQArray::pop(): empty");
        int best = 0;
        for (int i = 1; i < static_cast<int>(data_.size()); ++i) {
            if (data_[i].second > data_[best].second) {
                best = i;
            }
        }
        data_.erase(data_.begin() + best);
    }
    bool empty() const {
        return data_.empty();
    }
};

// ------------ Sorted Linked List Implementation ------------
class PQLL {
    struct Node {
        int value, priority;
        Node* next;
        Node(int v, int p): value(v), priority(p), next(NULL) {}
    };
    Node* head_;
public:
    PQLL(): head_(NULL) {}
    ~PQLL() {
        while (head_) {
            Node* tmp = head_;
            head_ = head_->next;
            delete tmp;
        }
    }
    void push(int value, int priority) {
        Node* node = new Node(value, priority);
        if (!head_ || priority > head_->priority) {
            node->next = head_;
            head_ = node;
        } else {
            Node* cur = head_;
            while (cur->next && cur->next->priority >= priority) {
                cur = cur->next;
            }
            node->next = cur->next;
            cur->next = node;
        }
    }
    std::pair<int,int> top() const {
        if (!head_) throw std::runtime_error("PQLL::top(): empty");
        return std::make_pair(head_->value, head_->priority);
    }
    void pop() {
        if (!head_) throw std::runtime_error("PQLL::pop(): empty");
        Node* tmp = head_;
        head_ = head_->next;
        delete tmp;
    }
    bool empty() const {
        return head_ == NULL;
    }
};

// ---------------- Binary Heap Implementation ----------------
class PQHeap {
    // max-heap of (value, priority)
    std::vector<std::pair<int,int>> heap_;
    void siftUp(int i) {
        while (i > 0) {
            int parent = (i - 1) / 2;
            if (heap_[i].second <= heap_[parent].second) break;
            std::swap(heap_[i], heap_[parent]);
            i = parent;
        }
    }
    void siftDown(int i) {
        int n = heap_.size();
        while (true) {
            int left = 2*i + 1;
            int right = 2*i + 2;
            int largest = i;
            if (left < n && heap_[left].second > heap_[largest].second)
                largest = left;
            if (right < n && heap_[right].second > heap_[largest].second)
                largest = right;
            if (largest == i) break;
            std::swap(heap_[i], heap_[largest]);
            i = largest;
        }
    }
public:
    void push(int value, int priority) {
        heap_.push_back(std::make_pair(value, priority));
        siftUp(static_cast<int>(heap_.size()) - 1);
    }
    std::pair<int,int> top() const {
        if (heap_.empty()) throw std::runtime_error("PQHeap::top(): empty");
        return heap_[0];
    }
    void pop() {
        if (heap_.empty()) throw std::runtime_error("PQHeap::pop(): empty");
        heap_[0] = heap_.back();
        heap_.pop_back();
        if (!heap_.empty()) siftDown(0);
    }
    bool empty() const {
        return heap_.empty();
    }
};

// ------------------------ Demonstration ------------------------
int main() {
    // Example sequence of (value, priority)
    std::vector<std::pair<int,int>> items = {
        {100, 1}, {200, 5}, {300, 3}, {400, 4}, {500, 2}
    };

    // 1) Unsorted Array PQ
    PQArray pa;
    for (size_t i = 0; i < items.size(); ++i)
        pa.push(items[i].first, items[i].second);
    std::cout << "PQArray pop sequence (value, priority):\n";
    while (!pa.empty()) {
        std::pair<int,int> t = pa.top();
        std::cout << "  (" << t.first << "," << t.second << ")\n";
        pa.pop();
    }

    // 2) Sorted Linked List PQ
    PQLL pl;
    for (size_t i = 0; i < items.size(); ++i)
        pl.push(items[i].first, items[i].second);
    std::cout << "\nPQLL pop sequence (value, priority):\n";
    while (!pl.empty()) {
        std::pair<int,int> t = pl.top();
        std::cout << "  (" << t.first << "," << t.second << ")\n";
        pl.pop();
    }

    // 3) Binary Heap PQ
    PQHeap ph;
    for (size_t i = 0; i < items.size(); ++i)
        ph.push(items[i].first, items[i].second);
    std::cout << "\nPQHeap pop sequence (value, priority):\n";
    while (!ph.empty()) {
        std::pair<int,int> t = ph.top();
        std::cout << "  (" << t.first << "," << t.second << ")\n";
        ph.pop();
    }

    return 0;
}
