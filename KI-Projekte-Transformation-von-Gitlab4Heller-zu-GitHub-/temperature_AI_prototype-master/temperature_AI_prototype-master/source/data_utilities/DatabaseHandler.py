import pymongo
from pymongo import MongoClient
class DatabaseHandler:
    def __init__(self, database_uri, database, collection):
        self._database_uri = database_uri
        self._database = database
        self._collection = collection
        self._get_the_table()

    def _get_the_table(self):
        _client = MongoClient(self._database_uri)
        self._db = _client[self._database]
        self._collection = self._db[self._collection]

    def add_document(self, document):
        if "_id" in document:
            del document["_id"]
        self._collection.insert_one(document).inserted_id

    def read_timestamp_query(self, start_time, end_time):
        # return self._collection.find({"date": {"$and": {"$gte": start_time, "$lt": end_time}}})
        return self._collection.find({"$and":
                                          [{"date": {"$gte": start_time}},
                                           {"date": {"$lt": end_time}}]
                                      })

    def read_field_last_doc(self, document_type, field_name):
        try:
            _id = self._collection.find_one({"logType": document_type}, sort=[(field_name, pymongo.DESCENDING)])["_id"]
            timestamp = self._collection.find_one({"logType": document_type}, sort=[(field_name, pymongo.DESCENDING)])[
                "date"]
        except:
            _id= None
            timestamp= None
        return _id, timestamp

    def append_array(self, _id, array):
        self._collection.update_one({"_id": _id}, {"$push": {"content": array}})
