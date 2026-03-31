#include "RequestQueue.h"
#include <new> // for std::nothrow

RequestQueue::RequestQueue()
    : data(nullptr),
      capacity(0),
      front(0),
      rear(0),
      count(0) {
    // start with a small default capacity
    resize(4);
}

RequestQueue::RequestQueue(int initialCapacity)
    : data(nullptr),
      capacity(0),
      front(0),
      rear(0),
      count(0) {
    if (initialCapacity < 1) {
        initialCapacity = 4;
    }
    resize(initialCapacity);
}

RequestQueue::~RequestQueue() {
    delete[] data;
}

bool RequestQueue::isEmpty() const {
    return count == 0;
}

bool RequestQueue::isFull() const {
    return count == capacity;
}

int RequestQueue::size() const {
    return count;
}

int RequestQueue::nextIndex(int idx) const {
    if (capacity == 0) return 0;
    return (idx + 1) % capacity;
}

bool RequestQueue::enqueue(const Request& req) {
    //Implement enqueue function as explained in the PDF.
    if (isFull()) {
        if (!resize(capacity * 2)) {
            return false;
        }
    }
    data[rear] = req;
    rear = nextIndex(rear);
    count++;
    return true;
}

bool RequestQueue::dequeue(Request& outReq) {
    if (isEmpty()) {
        return false;
    }
    outReq = data[front];
    front = nextIndex(front);
    count--;
    return true;
}

bool RequestQueue::peek(Request& outReq) const {
    if (isEmpty()) {
        return false;
    }
    outReq = data[front];
    return true;
}

void RequestQueue::clear() {
    front = 0;
    rear = 0;
    count = 0;
}

bool RequestQueue::removeById(const std::string& id) {
    for (int i = 0; i < count; i++) {
        int currentIdx = (front + i) % capacity;
        if (data[currentIdx].getId() == id) {
            for (int j = i; j < count - 1; j++) {
                int outreq = (front + j) % capacity;
                int nextreq = (front + j + 1) % capacity;
                data[outreq] = data[nextreq];
            }
            rear = (rear - 1 + capacity) % capacity;
            count--;
            return true;
        }
    }
    return false;
}

bool RequestQueue::resize(int newCapacity) {
    Request* newData = new (std::nothrow) Request[newCapacity];

    if (newData == nullptr) {
        return false;
    }

    int current = front;
    for (int i = 0; i < count; i++) {
        newData[i] = data[current];
        current = nextIndex(current);
    }
    delete[] data;
    data = newData;
    capacity = newCapacity;
    front = 0;
    rear = count;
    return true;
}
