import json
#import logging
import sys

sys.path.append("/EdgeAPI/include")
sys.path.append('../EdgeAPI/include')
from time import sleep

from edge import (
    DataSource,
    DataSourceAccess,
    InformationDefinitionObject,
    ParameterDefinition,
    ParameterDefinitionVector,
    StringVector,
    ParameterValueObject
)


# from EdgeAPI.include.edge import *
class ParameterService:
    def __init__(self):
        self.initialize_parameter_service()

    def initialize_parameter_service(self):
        DataSourceAccess.Initialize()
        dataSourceNames = StringVector()
        status = DataSourceAccess.GetDataSourceNames("SINUMERIK", dataSourceNames)
        #logging.info(f"status of data source access: {status.GetMessage()}")

        while not status.Success():
            sleep(1)
            status = DataSourceAccess.GetDataSourceNames("SINUMERIK", dataSourceNames)
        dataSourceName = dataSourceNames[0]
        #for name in dataSourceNames:
            #logging.info(name)
        ncu = DataSource()
        status = DataSourceAccess.GetDataSource(dataSourceName, ncu)
        #logging.info(f"status of ncu access: {status.GetMessage()}")
        while not status.Success():
            sleep(1)
            status = DataSourceAccess.GetDataSource(dataSourceName, ncu)
        parameterServices = ncu.GetParameterServices()
        self.paramSVC = parameterServices[0]

    def read_parameter(self, parameter: str):
        parameter_definition = ParameterDefinition(parameter)
        response = self.paramSVC.ReadParameter(parameter_definition)
        while not response.Success():
            response = self.paramSVC.ReadParameter(parameter_definition)
            sleep(0.2)
        pvo = parameter_definition.GetValueObject()
        numValues = pvo.GetValues().size()
        if 1 < numValues:
            result = []
            for i in range(numValues):
                result.append(pvo.GetValues()[i][0])
        else:
            result = pvo.GetValues()[0][0]
        return result

    def multi_read_parameters(self, parameter_list: list):
        result_list = []
        parameter_definition_vector = ParameterDefinitionVector()
        for param in parameter_list:
            parameter_definition_vector.push_back(ParameterDefinition(param))
        response = self.paramSVC.ReadParameters(parameter_definition_vector)
        while not response.Success():
            response = self.paramSVC.ReadParameters(parameter_definition_vector)
            sleep(0.2)
        for param in parameter_definition_vector:
            param_obj = param.GetValueObject()
            param_value = param_obj.GetValues()
            for val in param_value:
                result_list.append(val[0])
        return result_list

    def write_parameter(self,parameter:str, value:str, object_type:str):
        parameter_definition = ParameterDefinition(parameter)
        pvo = ParameterValueObject()
        if object_type == "String":
            pvo.AddValue(value, ParameterValueObject.String)
        elif object_type == "Int":
            pvo.AddValue(value, ParameterValueObject.Int)
        elif object_type == "Bool":
            pvo.AddValue(value, ParameterValueObject.Bool)
        elif object_type == "Char":
            pvo.AddValue(value, ParameterValueObject.Char)
        elif object_type == "Double":
            pvo.AddValue(value, ParameterValueObject.Double)
        else:
            #logging.WARN(f"objectType : {object_type} not supported! Use default String!")
            pvo.AddValue(value, ParameterValueObject.String)

        parameter_definition.SetValueObject(pvo)
        response = self.paramSVC.WriteParameter(parameter_definition)

        if (response.Success() == True):
            print("parameter_handler: Write success")
            # logging.INFO("Write success")
        else:
            print("parameter_handler: Write Failure")
            # logging.WARN("Write Failure")
