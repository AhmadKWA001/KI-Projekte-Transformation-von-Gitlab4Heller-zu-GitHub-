'''
Function to create a csv file with a dataframe from data read in from a database:
Expected inputs:
    Parameters to access the dataframe from the DB:
        host
        port
        collection
        start
        end
    Path where the csv file should be saved (save_path)
    show_save: Bool, if 
            True: Plot the data read from the database and then define needed timeframe via terminal
            False: Use timeframe provided here directly, doesn't plot
Return:     Saves a dataframe with the data from the database in a csv file.
'''

import numpy as np
import pandas as pd
import pymongo as py
from datetime import datetime, timedelta 
import glob, os
import yaml

import plotly.graph_objects as go
from plotly.subplots import make_subplots
import plotly.express as ex
import plotly.io as pio

scatter_mode= 'lines'



import click
@click.command()
@click.option('--readconfig', prompt = 'Read Infos from config file? (True / False)', type = bool)
def useConfig(readconfig):
    if readconfig:
        # Using the Config file
        with open("config.yaml", "r") as configfile:
            data = yaml.load_all(configfile, Loader=yaml.FullLoader)
            for file in data:
                # for every provided block within the config file read in the configuration, 
                # interpolate and save into the provided paths
                host = file["host"]
                port = int(file["port"])
                collection = file["collection"]
                save_path = file["save_path"]
                start = file["start"]
                end = file["end"]
                timeshift = file["timeshift"]
                show_save = file["show_save"]
                save(host, port, collection, save_path, start, end, show_save, int(timeshift))
    else:
        # If the config file isn't used, all the neccessary data needs to be typed into the terminal
        @click.command()
        @click.option('--host', prompt = 'Host of machine data database')
        @click.option('--port', prompt = 'Port of machine data databasem')
        @click.option('--collection', prompt = 'Collection of machine data database')
        @click.option('--save_path', prompt = 'Filepath to save the csv file')
        @click.option('--start', prompt = 'Time when to start')
        @click.option('--end', prompt = 'Time to end')
        @click.option('--timeshift', prompt = 'Timeshift')
        @click.option('--show_save', prompt = 'Do you want to see the data before saving? [True / False]')
        def promptPaths(host, port, collection, save_path, start, end, show_save, timeshift):
            save(host, port, collection, save_path, start, end, show_save, int(timeshift))
        if __name__ == '__main__':
            promptPaths()

def show(df, save_path):
    # Function is only called if show_save is true
    # Then the data in the dataframe will be plotted
    # and the user can define the timeframe which should be saved
    nrrows = len(df.columns)
    fig= make_subplots(rows=nrrows, cols=1, shared_xaxes= True, print_grid= True, vertical_spacing=0.01)
    col_names = df.columns.values
    i = 0 
    for column in df:
        fig.add_trace(go.Scatter(x= df['date'], y = df[column], name= col_names[i], mode= scatter_mode), row= i+1, col= 1)
        i += 1
    fig.update_layout(height=10000, width=1300, title_text="Daten im Dataframe")
    fig.show()

    # Request the timeframe which should be saved and crop the dataframe to that region
    @click.command()
    @click.option('--start_save', prompt = 'Starting time for csv [Format: 2023-09-08T00:00:00.000+0000]')
    @click.option('--end_save', prompt = 'End time for csv [Format: 2023-09-08T00:00:00.000+0000]')
    def cut(start_save, end_save, df=df):
        print("start function cut")
        start_save = datetime.strptime(start_save, "%Y-%m-%dT%H:%M:%S.%f+0000")
        end_save = datetime.strptime(end_save, "%Y-%m-%dT%H:%M:%S.%f+0000")
        idStartAll = df.index[df['date']>=start_save].tolist()
        idStart = idStartAll[0]
        idEndAll = df.index[df['date']>=end_save].tolist()
        if len(idEndAll) == 0: idEnd = df.index[-1]
        else: idEnd = idEndAll[0]
        df = df[idStart:idEnd].copy()
        # Save and plot the dataframe
        finish(df, save_path)

    if __name__ == '__main__':
        cut()

def finish(df, save_path):
    # Save the dataframe to the defined path
    df.to_csv(save_path)
    # Plot the saved dataframe
    nrrows = len(df.columns)
    fig= make_subplots(rows=nrrows, cols=1, shared_xaxes= True, print_grid= True, vertical_spacing=0.01)
    col_names = df.columns.values
    i = 0 
    for column in df:
        fig.add_trace(go.Scatter(x= df['date'], y = df[column], name= col_names[i], mode= scatter_mode), row= i+1, col= 1)
        i += 1
    fig.update_layout(height=10000, width=1300, title_text="Daten im Dataframe")
    fig.show()


def save(host, port, collection, save_path, start, end, show_save, timeshift):    #df_machine_path, 
    # Read in dataframe
    # Querys based on time are imprecisely handled by MongoDB
    # Therefore a too large dataframe is queried and the cropped to the
    # # region requested by the user.

    start_date = datetime.strptime(start, "%Y-%m-%dT%H:%M:%S.%f+0000")
    end_date = datetime.strptime(end, "%Y-%m-%dT%H:%M:%S.%f+0000")

    print(host)
    print(start + " - " + end)

    start_db = start_date + timedelta(hours=-1)
    end_db = end_date + timedelta(hours=1)

    print(str(start_db) + " - " + str(end_db))

    client = py.MongoClient(host= host, port=int(port))
    db = client.h4ai
    event_list = db[collection].find({ "date" : { '$gte' : start_db, '$lt' : end_db} }).sort('date', 1)

    df = pd.DataFrame()
    for event in event_list:
        content = event["content"]
        for data in content:
            raw_data = data["raw_data"]
            raw_data["date"] = data["date"]
            raw_data["given2model"] = data["given2model"]
            raw_data["prediction"] = data["prediction"]
            df_row = pd.DataFrame(raw_data, index= ["date"])
            df = pd.concat([df, df_row], axis=0, ignore_index=True)
    df.reset_index(inplace=True)

    if len(df) == 0:    # Quit if the DF is empty
        print("The dataframe is empty, is the timeframe correct?")
        quit()
    
    # Crop to region requested by the user
    df["date"] = pd.to_datetime(df["date"])
    idStartAll = df.index[df['date']>=start_date].tolist()
    idStart = idStartAll[0]
    idEndAll = df.index[df['date']>=end_date].tolist()
    if len(idEndAll) == 0: idEnd = df.index[-1]
    else: idEnd = idEndAll[0]
    df = df[idStart:idEnd].copy()

    if len(df) == 0:    # Quit if the DF is empty
        print("The dataframe is empty, is the timeframe correct?")
        quit()

    # Apply the timeshift
    df["date"] = df["date"] + timedelta(hours=timeshift)

    # If show_save is true, the dataframe will be plotted
    # and the user is asked to input the timeframe which should be saved
    # If show_save is false, the dataframe will be saved as it is.
    # In both cases the saved dataframe will be plotted
    if show_save:
        show(df, save_path=save_path)
    else: finish(df, save_path)

if __name__ == '__main__':
    useConfig()
