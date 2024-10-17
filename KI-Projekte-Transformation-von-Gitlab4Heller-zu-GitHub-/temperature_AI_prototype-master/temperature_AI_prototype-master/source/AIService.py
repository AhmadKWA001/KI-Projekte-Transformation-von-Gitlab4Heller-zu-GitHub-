from data_utilities.QueueBuffer import QueueBuffer
from data_utilities.DBObserver import DBObserver
from assets_utilities.OnnxHandler import OnnxHandler
from assets_utilities.PickleHandler import PickleHandler
from edge_box_utilities.EdgeboxHandler import EdgeboxHandler
from edge_box_utilities.AppConfigs import AppConfigs
from data_utilities.Record import  Record
import numpy as np
from datetime import datetime
from time import sleep

class AIService:
    __edgebox_handler: EdgeboxHandler
    __onnx_model: OnnxHandler
    __preprocessor: PickleHandler
    __appConfig: AppConfigs
    __buffer: QueueBuffer
    def __init__(self):
        self.__edgebox_handler= EdgeboxHandler()
        ## Read configs from Edgebox
        self.__appConfig = self.__edgebox_handler.read_configs()
        self.__onnx_model= OnnxHandler()
        self.__preprocessor= PickleHandler()
        ## Load the model and the preprocessor
        self.__onnx_model.load(file_path=self.__appConfig.model)
        #self.__preprocessor.load(file_path=self.__appConfig.preprocessor)
        ## Instantiate a database observer
        self.__buffer= QueueBuffer()
        dbobserver= DBObserver(queue_buffer= self.__buffer, database_uri= self.__appConfig.database_uri,database= self.__appConfig.database, collection=self.__appConfig.collection, legacy= self.__appConfig.legacy)
        self.__buffer.attach_observer(dbobserver= dbobserver)

    # A function to extract the input array from the buffer
    def __extract_time_relevance_array(self, input_array: np.ndarray, output_shape):
        input_time = input_array.shape[1]  # in case of 5 Seconds it will be 240 seconds
        output_time = output_shape[1]  # in case of 2 Minutes it will be 10
        step = int(input_time / output_time)
        result_array = np.zeros(output_shape)
        for i in range(0, output_time):
            #print('place in result: ', output_time - i - 1, ' place in original: ', input_time - i * step - 1)
            result_array[0, output_time - i - 1, :] = input_array[0, input_time - i * step - 1, :]
        return result_array
    ### A function to initialize the AIService and load the required resources
    def run(self):
        try:
            must_wait= True
            ## Prepare the parameter list
            new_parameter_list, var_offset_dict= self.__edgebox_handler.prepare_parameter_list( parameter_dict=self.__appConfig.parameter_dict)
            ## Read device name from the control
            device= self.__edgebox_handler.read_device_parameter(self.__appConfig.device_parameter_address)
            ## Instantiate the shape of the realtime storage shape
            #output_sec_shape = (self.__appConfig.shape[0], int(self.__appConfig.shape[1] * np.ceil((self.__appConfig.time_relevance / self.__appConfig.recording_frequency))), self.__appConfig.shape[2])
            to_write_parameters = set(list(self.__appConfig.parameter_dict.keys())) - set(self.__appConfig.inputsignal_list)
            #print ('buffer_shape= ', output_sec_shape)
            under_response_time = True
            go_on_prog = True
            hlpvar = False
            array: np.ndarray
            time_point = datetime.utcnow()
            progstart= datetime.utcnow()
            first_run= True
            last_prediction_time = None
            last_data_point_time=None
            given2model = False
            is_relevant_data_point = True
            while True:
                given2model = False
                running_duration = datetime.utcnow() - progstart
                running_duration_secs = running_duration.total_seconds()
                running_duration_mins = running_duration_secs / 60
                if running_duration_mins > self.__appConfig.time2run:  ### Wenn die Aufzeichungsdauer überschritten ist
                    go_on_prog = False
                    print("Recording time is over. Application will stop")
                    break
                new_result_list= self.__edgebox_handler.read_machine_data(new_parameter_list= new_parameter_list, var_offset_dict= var_offset_dict)
                current_time = datetime.utcnow()
                if last_data_point_time is not None:
                    is_relevant_data_point = (current_time - last_data_point_time).total_seconds() > self.__appConfig.recording_frequency
                    #print('IS relevant= ', is_relevant_data_point, 'duration= ', (current_time - last_data_point_time).total_seconds()  )
                if not is_relevant_data_point:
                    must_wait= False
                    continue
                else:
                    must_wait = True
                    if last_data_point_time is not None:
                        print('Duration= ',(current_time - last_data_point_time).total_seconds(),' seconds' )
                    last_data_point_time= current_time
                if last_prediction_time is not None:
                    under_response_time = (current_time - last_prediction_time).total_seconds()  < self.__appConfig.response_time
                    print('under_response_time= ', under_response_time, 'duration= ', (current_time - last_prediction_time).total_seconds() )
                new_signal_list = self.__edgebox_handler.create_right_sequence_input(
                    inputsignal_list=self.__appConfig.inputsignal_list, var_offset_dict=var_offset_dict,
                    parameter_dict=self.__appConfig.parameter_dict, new_result_list=new_result_list)
                if self.__appConfig.make_prediction:
                    array= np.float32(np.asarray([new_signal_list]))
                    outputs = self.__onnx_model.execute(input_array=array)
                    out = outputs[0][0][0]
                    last_prediction_time = current_time
                    given2model = True
                    ## Write the prediction back
                    self.__edgebox_handler.write_parameter(self.__appConfig.prediction_write_address, out, "Double")
                if self.__appConfig.write_to_db:
                    #### Prepare a record to be inserted in the database
                    row_data_dict = {}
                    nxt = 0
                    for vals in self.__appConfig.inputsignal_list:
                        row_data_dict[vals] = np.round(new_signal_list[nxt], 2)
                        nxt += 1
                    for parameter in to_write_parameters:
                        row_data_dict[parameter]= new_result_list[var_offset_dict["index_{parameter_name}".format(parameter_name= parameter)]]
                    ## Instantiate a template of the record to be inserted in the database
                    h4ailog = Record()
                    h4ailog.date = current_time
                    h4ailog.logType = "temperature"
                    h4ailog.device = device
                    if self.__appConfig.make_prediction:
                        h4ailog.preprocessor = self.__appConfig.preprocessor_version
                        h4ailog.model = self.__appConfig.model_version
                        parameter_content = [{"date": current_time,
                                            "prediction": float(out),
                                            "raw_data": row_data_dict,
                                            "given2model": given2model
                                            }]
                    else:###Only data collection
                        parameter_content = [{"date": current_time,
                                              "raw_data": row_data_dict
                                              }]
                    ####Create Document to read parameters
                    h4ailog.content= parameter_content
                    ## push the record in the buffer
                    self.__buffer.enqueue(element= h4ailog)
                if must_wait:
                    print('Sleep for ', self.__appConfig.period)
                print('###########################################################################')
        except Exception as exp:
            raise exp









