#ifndef PRODUCERCONSUMERQUEUE_H
#define PRODUCERCONSUMERQUEUE_H


#include <QQueue>
#include <QMutex>
#include <QWaitCondition>


/**
 * @brief The ProducerConsumerQueue class
 * A thread safe consumer producer queue.
 */
template <typename T> class ProducerConsumerQueue
{

public:

    explicit ProducerConsumerQueue();
    ~ProducerConsumerQueue();

    /**
     * @brief enqueue
     * Safely enqueues a new element. Will block if any other write operation is occuring on the queue.
     * @param elem The element to enqueue.
     */
    void enqueue(const T &elem);

    /**
     * @brief dequeue
     * Safely dequeues an element form the queue. Will block if any other write operation is occuring.
     * @return The element dequeued from the end of the queue.
     */
    T dequeue();

    /**
     * @brief size
     * The current size of the queue. will block if enqueue or dequeue are currently in middle of call.
     * @return The size (number of elements) of the queue.
     */
    uint size() const;

private:

    /**
     * @brief _queue
     * The unsafe queue to use as a base for the thread safe queue.
     */
    QQueue<T> _queue;

    /**
     * @brief _queueLock
     * The lock to use when write operations are happening on the queue.
     */
    QMutex _queueLock;

    /**
     * @brief _consumerWaitCondition
     * The wait condition for the Consumer that will be checked in the dequeue method, and
     * whenever enqueue is called by a producer, all consumers will be awakened through this
     * wait condition.
     */
    QWaitCondition _consumerWaitCondition;
};


template <typename T>
ProducerConsumerQueue<T>::ProducerConsumerQueue()
    : _queue(),
      _queueLock(),
      _consumerWaitCondition()
{}


template <typename T>
ProducerConsumerQueue<T>::~ProducerConsumerQueue()
{
    // Make sure we safely wake evrything up if going out of scope.
    _queueLock.lock();
    _consumerWaitCondition.wakeAll();
    _queueLock.unlock();
}


template <typename T>
void ProducerConsumerQueue<T>::enqueue(const T &elem)
{
    _queueLock.lock();
    _queue.enqueue(elem);
    // Make sure we wake consumer!
    _consumerWaitCondition.wakeAll();
    _queueLock.unlock();
}


template <typename T>
T ProducerConsumerQueue<T>::dequeue()
{
    T elem;
    _queueLock.lock();

    // If we have no elements to dequeue for consumer, then we must wait for one!
    if (_queue.size() == 0) {
        // This will wait to be awakened, and release its hold on _queueLock.
        _consumerWaitCondition.wait(&_queueLock);
    }

    // We could be forced to stop waiting without element being enqueued because of destructor!
    if (_queue.size() != 0) {
        elem = _queue.dequeue();
    }

    _queueLock.unlock();
    return elem;
}


template <typename T>
uint ProducerConsumerQueue<T>::size() const
{
    return _queue.size();
}


#endif // PRODUCERCONSUMERQUEUE_H
