/* 
 * File:   prosumer.cpp
 * Author: H.-J. Eikerling
 *
 * Producer / consumer System.
 * Modifikation der Variante von Andrae Muys. 
 * Hier werden jetzt aber anstelle von pthreads und den 
 * dort vorhandenen Synchronisationsmechanismen hier die 
 * Konstrukte von C++ (2011). Der g++ Compiler (zum. Variante 4.8) muss dazu 
 * mit den Optionen -std=c++11 -pthread aufgerufen werden.
 * 
 * Created on 29. April 2016, 21:27
 */


#include <thread>
#include <mutex>
#include <atomic>
#include <condition_variable>

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

using namespace std;

#define QUEUESIZE 10
#define LOOP 20

void *producer(void *args);
void *consumer(void *args);

/* Klasse Queue (Warteschlange) */
class Queue {
private:
    int buf[QUEUESIZE];
    long head, tail;
    bool full, empty;
public:
    std::mutex mut;
    std::condition_variable notFull, notEmpty;
public:
    Queue();
    ~Queue();
    void addItem(int in);
    void delItem(int *out);
    bool isFull() { return full; }
    bool isEmpty() { return empty; }
};

Queue::Queue () {
    empty = true;
    full = false;
    head = 0;
    tail = 0;
}

Queue::~Queue () { }

void Queue::addItem(int in) {
    buf[tail] = in;
    tail++;
    if (tail == QUEUESIZE)
        tail = 0;
    if (tail == head)
        full = 1;
    empty = 0;
}

void Queue::delItem (int *out) {
    *out = buf[head];
    head++;
    if (head == QUEUESIZE)
        head = 0;
    if (head == tail)
        empty = 1;
    full = 0;
}

/* Producer */
void *producer(void *q) {
    Queue * fifo = (Queue *) q;

    for (int j = 0; j < 2; j++) {
        for (int i = 0; i < LOOP; i++) {
            std::unique_lock<std::mutex> lck(fifo->mut);
            // while -> spinlock
            if (fifo->isFull()) {
                printf("producer: queue FULL.\n");
                fifo->notFull.wait(lck);
            }
            fifo->addItem (i);
            printf("producer: produced %d (round %d).\n", i, j);
            fifo->notEmpty.notify_one();
            usleep(100000 + j * 100000);
        }
    }
    return (NULL);
}

/* Consumer */
void *consumer(void *q) {
    Queue *fifo;
    int i, d;
    int j;

    fifo = (Queue *) q;
    for (j = 0; j < 2; j++) {
        for (i = 0; i < LOOP; i++) {
            std::unique_lock<std::mutex> lck(fifo->mut);
            // while -> spinlock
            if (fifo->isEmpty()) {
                printf("consumer: queue EMPTY.\n");
                fifo->notEmpty.wait(lck);
            }
            fifo->delItem (&d);
            printf("consumer: consumed %d.\n", d);
            fifo->notFull.notify_one();
            usleep(200000 + j * 300000);
        }
    }
    return (NULL);
}

int main() {
    Queue fifo;

    thread pro(producer, &fifo);
    thread con(consumer, &fifo);

    pro.join();
    con.join();

    exit(0);
}