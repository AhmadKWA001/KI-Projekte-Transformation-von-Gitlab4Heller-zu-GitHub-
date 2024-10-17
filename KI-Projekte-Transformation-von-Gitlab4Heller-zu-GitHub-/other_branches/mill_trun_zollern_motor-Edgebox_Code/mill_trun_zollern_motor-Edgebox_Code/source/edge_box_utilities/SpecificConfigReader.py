import json


class SpecificConfigReader:
    def __init__(self, appname="debug_container", file_path="/etc/runtime/config/appconfig.json"):
        file_obj = open(file_path)
        self.specificConfig = json.load(file_obj)[appname]
        file_obj.close()
        self._read_database_uri()
        self._read_database()
        self._read_collection()
        self._read_device_parameter_address()
        self._read_prediction_write_address()
        self._read_preprocessor()
        self._read_model()
        self._read_shape()
        self._read_period()
        self._read_model_version()
        self._read_preprocessor_version()
        self._read_parameter_dict()
        self._read_document_legacy()
        self._read_input_signals()
        self._read_rec_frequency()
        self._read_response_time()
        self._read_show_debugprints()
        self._read_record_timer()
        self._read_time_relevance()
        self._read_make_prediction()

    def _read_database_uri(self):
        self._database_uri = self.specificConfig["specificConfig"]["database_config"]["database_uri"]
    def _read_database(self):
        self._database = self.specificConfig["specificConfig"]["database_config"]["database"]
    def _read_collection(self):
        self._collection= self.specificConfig["specificConfig"]["database_config"]["collection"]
    def _read_prediction_write_address(self):
        self._prediction_write_address = self.specificConfig["specificConfig"]["prediction_write_address"]
    def _read_device_parameter_address(self):
        self._device_parameter_address = self.specificConfig["specificConfig"]["device_address"]
    def _read_show_debugprints(self):
        self._show_debugprints = self.specificConfig["specificConfig"]["show_debugprints"]
    def _read_make_prediction(self):
        self._make_prediction = self.specificConfig["specificConfig"]["make_prediction"]
    def _read_record_timer(self):
        self._record_timer = self.specificConfig["specificConfig"]["record_timer"]


    # def _read_tcp_z(self):
    #     self._tcp_z = self.specificConfig["specificConfig"]["tcp_z"]
    #
    # def _read_welle_z(self):
    #     self._welle_z = self.specificConfig["specificConfig"]["welle_z"]
    #
    # def _read_compensation(self):
    #     self._compensation = self.specificConfig["specificConfig"]["compensation"]

    def _read_rec_frequency(self):
        self._rec_frequency = self.specificConfig["specificConfig"]["rec_frequency"]

    def _read_response_time(self):
        self._response_time = self.specificConfig["specificConfig"]["response_time"]

    def _read_time_relevance(self):
        self._time_relevance = self.specificConfig["specificConfig"]["time_relevance"]


    # def _read_supply_temperature(self):
    #    self._supply_temperature = self.specificConfig["specificConfig"]["supply_temperature"]

    def _read_preprocessor(self):
        self._preprocessor = self.specificConfig["specificConfig"]["preprocessor"]


    def _read_model(self):
        self._model = self.specificConfig["specificConfig"]["model"]


    def _read_shape(self):
        '''
        Takes string as description for the input shape of the model!
        input: str
        output: tuple
        Example: 1,3,1
        '''
        tmp = self.specificConfig["specificConfig"]["shape"]
        self._shape = tuple(map(int, tmp.split(',')))


    def _read_period(self):
        self._period = int(self.specificConfig["specificConfig"]["period"])


    def _read_model_version(self):
        self._model_version = self.specificConfig["specificConfig"]["model_version"]


    def _read_preprocessor_version(self):
        self._preprocessor_version = self.specificConfig["specificConfig"]["preprocessor_version"]


    def _read_parameter_list(self):
        self._parameter_list = self.specificConfig["specificConfig"]["parameter_list"]


    def _read_parameter_dict(self):
        self._parameter_dict = self.specificConfig["specificConfig"]["parameter_dict"]


    def _read_input_signals(self):
        self._input_signals = self.specificConfig["specificConfig"]["input_signals"]


    def _read_sim_signals(self):
        self._sim_signals = self.specificConfig["specificConfig"]["sim_signals"]


    def _read_simfile(self):
        self._simfile = self.specificConfig["specificConfig"]["simfile"]


    def _read_document_legacy(self):
        '''
        input: legacy of the document in hours
        :return:
        '''
        self._document_legacy = self.specificConfig["specificConfig"]["document_legacy"]


    def get_show_debugprints(self):
        return self._show_debugprints


    def get_record_timer(self):
        return self._record_timer


    # def get_tcp_z(self):
    #     return self._tcp_z
    #
    # def get_welle_z(self):
    #     return self._welle_z
    #
    # def get_compensation(self):
    #     return self._compensation
    def get_database_uri(self):
        return self._database_uri
    def get_database(self):
        return self._database
    def get_collection(self):
        return self._collection
    def get_prediction_write_address(self):
        return self._prediction_write_address
    def get_device_parameter_address(self):
        return self._device_parameter_address
    def get_rec_frequency(self):
        return self._rec_frequency

    def get_response_time(self):
        return self._response_time

    def get_time_relevance(self):
        return self._time_relevance


    # def get_supply_temperature(self):
    #    return self._supply_temperature

    def get_preprocessor(self):
        return self._preprocessor


    def get_model(self):
        return self._model

    def get_make_prediction(self):
        return self._make_prediction

    def get_shape(self):
        return self._shape


    def get_period(self):
        return self._period


    def get_model_version(self):
        return self._model_version


    def get_preprocessor_version(self):
        return self._preprocessor_version


    def get_parameter_list(self):
        return self._parameter_list


    def get_parameter_dict(self):
        return self._parameter_dict


    def get_input_signals(self):
        return self._input_signals


    def get_sim_signals(self):
        return self._sim_signals


    def get_simfile(self):
        return self._simfile


    def get_document_legacy(self):
        return self._document_legacy

