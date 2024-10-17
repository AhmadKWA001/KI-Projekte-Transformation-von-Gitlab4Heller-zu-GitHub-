from edge_box_utilities.SpecificConfigReader import  SpecificConfigReader
from edge_box_utilities.ParameterService import ParameterService
from edge_box_utilities.AppConfigs import AppConfigs
class EdgeboxHandler:
    __specificConfigReader= None
    __parameterService= None
    def __init__(self):
        self.__specificConfigReader= SpecificConfigReader()
        self.__parameterService= ParameterService()
    def read_configs(self):
        appconfigs= AppConfigs()
        appconfigs.database_uri= self.__specificConfigReader.get_database_uri()
        appconfigs.database= self.__specificConfigReader.get_database()
        appconfigs.collection= self.__specificConfigReader.get_collection()
        appconfigs.prediction_write_address= self.__specificConfigReader.get_prediction_write_address()
        appconfigs.device_parameter_address= self.__specificConfigReader.get_device_parameter_address()
        appconfigs.show_debugprints = self.__specificConfigReader.get_show_debugprints()
        appconfigs.recording_frequency = self.__specificConfigReader.get_rec_frequency()
        appconfigs.response_time = self.__specificConfigReader.get_response_time()
        appconfigs.time_relevance = self.__specificConfigReader.get_time_relevance()
        appconfigs.preprocessor = self.__specificConfigReader.get_preprocessor()
        appconfigs.model = self.__specificConfigReader.get_model()
        appconfigs.shape = self.__specificConfigReader.get_shape()
        appconfigs.period = self.__specificConfigReader.get_period()
        appconfigs.model_version = self.__specificConfigReader.get_model_version()
        appconfigs.preprocessor_version = self.__specificConfigReader.get_preprocessor_version()
        appconfigs.time2run = self.__specificConfigReader.get_record_timer()
        # input_signals aus config lesen --> Inhalt und Reihenfolge welche Signale an das Model Ã¼bergeben werden
        appconfigs.inputsignal_list = self.__specificConfigReader.get_input_signals()
        # parameter dictionary aus config lesen
        appconfigs.parameter_dict = self.__specificConfigReader.get_parameter_dict()
        appconfigs.legacy = self.__specificConfigReader.get_document_legacy()
        appconfigs.make_prediction= self.__specificConfigReader.get_make_prediction()
        return appconfigs
    def prepare_parameter_list(self, parameter_dict):
        var_offset_dict = {}
        nxt1 = 0
        for vals in parameter_dict:
            var_offset_dict["index_" + vals] = 99
            nxt1 += 1
        # Anzahl der zu lesenden Parameter bestimmen und dazu passende parameter-liste erstellen
        new_parameter_list = []
        varoffsetnumber = 0
        for key in parameter_dict.keys():
            if not isinstance(parameter_dict[key], int):
                for vol in var_offset_dict.keys():
                    if key.lower() == vol.lower().replace('index_',''):
                        var_offset_dict[vol] = varoffsetnumber
                        if len(new_parameter_list) < (varoffsetnumber + 1):
                            new_parameter_list.append(parameter_dict[key])
                        varoffsetnumber += 1
                        break
        return new_parameter_list, var_offset_dict
    def read_device_parameter(self, device_address):
        device = self.__parameterService.read_parameter(device_address)
        return  device
    def read_machine_data(self, new_parameter_list, var_offset_dict):
        # read data from the machine
        new_result_list = self.__parameterService.multi_read_parameters(new_parameter_list)
        new_result_list = [float(x) for x in new_result_list]
        return new_result_list
    def create_right_sequence_input(self, inputsignal_list, var_offset_dict, parameter_dict, new_result_list):
        # signal liste fÃ¼r preprocessing erstellen (Reihenfolge wie in input_signals im config-File)
        new_signal_list = []
        signaloffsetnumber = 0
        for sig in inputsignal_list:
            sig_name = sig
            for vod in var_offset_dict.keys():
                if sig.lower() in vod.lower():
                    if len(new_signal_list) < (signaloffsetnumber+1):
                        if var_offset_dict[vod] == 99:
                            new_signal_list.append(parameter_dict[sig])
                        else:
                            new_signal_list.append(new_result_list[var_offset_dict[vod]])
                    signaloffsetnumber += 1
                    break
        new_signal_list = [float(x) for x in new_signal_list]
        return new_signal_list
    def write_parameter(self, address: str, value, type: str):
        self.__parameterService.write_parameter(address, str(value), object_type=type)


