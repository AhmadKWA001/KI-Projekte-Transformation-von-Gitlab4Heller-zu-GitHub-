from collections import  deque

class QueueBuffer:

    __data_queue: deque= None
    __observer= None
    def __init__(self):
        self.__data_queue= deque()
    def enqueue(self, element):
        try:
            self.__data_queue.append(element)
            self.__notify_observer()
        except Exception as e:
            raise e
    def dequeue(self):
        element= self.__data_queue.popleft()
        return element
    def attach_observer(self, dbobserver):
        self.__observer= dbobserver
    def __notify_observer(self):
        self.__observer.update()




