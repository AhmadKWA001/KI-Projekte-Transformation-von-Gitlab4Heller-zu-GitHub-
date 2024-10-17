from threading import  Thread
from data_utilities.Record import Record
from data_utilities.DatabaseHandler import DatabaseHandler
from data_utilities.QueueBuffer import QueueBuffer
class DBObserver:
    __queue_buffer: QueueBuffer
    __dbhandler: DatabaseHandler
    __writer_thread: Thread
    __legacy= None
    def __init__(self, queue_buffer, database_uri, database, collection, legacy=1):
        self.__queue_buffer= queue_buffer
        self.__dbhandler= DatabaseHandler(database_uri, database, collection)
        self.__legacy= legacy
        self.__writer_thread= Thread(target= self.__write_task(), name='writer thread')
    def update(self):
        self.__writer_thread.start()
        self.__writer_thread.join()
        self.__writer_thread.__init__(target=self.__write_task(),  name='writer thread')
    def __write_task(self):
        while True:
            try:
                element: Record
                element= self.__queue_buffer.dequeue()
                log_id, log_timestamp = self.__dbhandler.read_field_last_doc("temperature", "date")
                if (log_id is None or log_timestamp is None) or (element.date - log_timestamp).total_seconds() >= 3600 * self.__legacy:
                    self.__dbhandler.add_document(element.__dict__)
                    print("--> write to DB, Create new Document ", element.date)
                else:
                    # update document
                    data_update = element.content[0]
                    self.__dbhandler.append_array(log_id, data_update)
                    print("--> write to DB, append ", element.date)
            except IndexError as e:
                break





