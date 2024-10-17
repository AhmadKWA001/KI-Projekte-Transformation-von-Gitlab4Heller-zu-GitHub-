'''
Function to synchronize two dataframes based on a date index.
Expected inputs:
df_machine: Dataframe with machine data, column = "date" is needed as datetime index
df_magnet:  Dataframe with the magnet temperatures, column = "date" is needed as datetime index
timedelta:  If the dataframes are collected based on different timesettings, 
            the shift needs to be considered, default is 1 (df_magnet in german time, df_machine in UTC time)
Return:     Returns a combined dataframe
'''

import numpy as np
import pandas as pd
import pymongo as py
from datetime import datetime, timedelta 
import glob, os
import yaml
import plotly.graph_objects as go
from plotly.subplots import make_subplots

import click
@click.command()
@click.option('--readconfig', prompt = 'Read Infos from config file? (True / False)', type = bool)
def useConfig(readconfig):
    if readconfig:
        # Using the Config file
        os.chdir("C:\\Users\\wch002\\rotor_sync")
        with open("config.yaml", "r") as configfile:
            data = yaml.load_all(configfile, Loader=yaml.FullLoader)
            for file in data:
                # for every provided block within the config file read in the configuration, 
                # interpolate and save into the provided paths
                host = file["df_machine_host"]
                port = int(file["df_machine_port"])
                collection = file["df_machine_collection"]
                df_magnet_path = file["df_magnet_path"]
                combined_path = file["combined_path"]
                start = file["start_machine"]
                end = file["end_machine"]
                timeshift = int(file["timeshift"])
                synchronize(host, port, collection, df_magnet_path, combined_path, start, end, timeshift)
    else:
        # If the config file isn't used, all the neccessary data needs to be typed into the terminal
        @click.command()
        @click.option('--host', prompt = 'Host of machine data database')
        @click.option('--port', prompt = 'Port of machine data databasem')
        @click.option('--collection', prompt = 'Collection of machine data database')
        #@click.option('--df_machine_path', prompt = 'Filepath to read machine data from')
        @click.option('--df_magnet_path', prompt = 'Filepath to read magnet data from')
        @click.option('--combined_path', prompt = 'Filepath to write the synchronized file')
        @click.option('--start', prompt = 'Time when to start')
        @click.option('--end', prompt = 'Time to end')
        @click.option('--timeshift', prompt = 'Timeshift between data (1 in summer, 2 in winter)')
        def promptPaths(host, port, collection, df_magnet_path, combined_path, start, end, timeshift):
            synchronize(host, port, collection, df_magnet_path, combined_path, start, end, int(timeshift))
        if __name__ == '__main__':
            promptPaths()


def readIn(workdir):
# Reads the data from one or several csv-files
    if workdir[-4:] == ".csv":
        # if the path to one specific csv file is provided
        df_merged= pd.read_csv(workdir)
        if "date" not in df_merged.columns and "Date/Time" in df_merged.columns:
            print("rename Date/Time to date")
            df_merged = df_merged.rename(columns = {"Date/Time": "date"})
        #print(df_merged.head(5))

    else:
        # if the path to a folder with several csv files is provided
        # in that case, these csv are concatenated and all used to 
        # synchronize with the provided timeperiod from the DB
        all_files = glob.glob(os.path.join(workdir, "*.csv"))
        df_merged = pd.DataFrame()

        for f in all_files:
            df_from_each_file = pd.read_csv(f)
            df_from_each_file["filename"] = f
            if "date" not in df_from_each_file.columns and "Date/Time" in df_from_each_file.columns:
                print("rename Date/Time to date")
                df_from_each_file=df_from_each_file.rename(columns = {"Date/Time": "date"})
            df_merged = pd.concat([df_merged, df_from_each_file])

    df_merged.reset_index(drop=True, inplace=True)
    return df_merged

def maybePlot(df_combined):
    @click.command()
    @click.option('--plot', prompt = 'Show plot?', type = bool)
    def plot(plot):
        if plot:
            scatter_mode= 'lines'
            params = ['date', "DRZ5", "T_KLEMMUNG", "T_LAGER", "T_MOTOR", 'prediction', 'Analytic_sol',
                    'No.1', 'No.2', 'No.3', 'No.4', 'No.5', 'No.6', 'No.7', 'No.8']
            df_plot = df_combined[params]
            nrrows = 5
            fig= make_subplots(rows=nrrows, cols=1, shared_xaxes= True, print_grid= True, vertical_spacing=0.01) #, specs=[[{"secondary_y": True}]]
            col_names = df_plot.columns.values

            i = 0 
            j = 0
            for column in df_plot:
                if column in ["DRZ5"]:
                    fig.add_trace(go.Scatter(x= df_plot['date'], y = df_plot[column], name= col_names[i+1], mode= scatter_mode), row= i+1, col= 1)
                    fig.update_yaxes(title_text="DRZ [RPM]", row=i+1, col=1)
                    i += 1
                if column in ["T_KLEMMUNG", "T_LAGER", "T_MOTOR"]:
                    fig.add_trace(go.Scatter(x= df_plot['date'], y = df_plot[column], name= col_names[i+1], mode= scatter_mode), row= i+1, col= 1)
                    fig.update_yaxes(title_text=(column + " [°C]"), row=i+1, col=1)
                    i += 1
                if column in ['prediction', 'Analytic_sol', 'No.1', 'No.2', 'No.3', 'No.4', 'No.5', 'No.6', 'No.7', 'No.8']:
                    fig.add_trace(go.Scatter(x= df_plot['date'], y = df_plot[column], name= col_names[i+j+1], mode= scatter_mode), row= i+1, col= 1)
                    fig.update_yaxes(title_text="Rotor Temperatur [°C]", row=i+1, col=1)
                    j +=1

            fig.update_layout(height=800, width=1100, title_text="Magnettemperaturen und Maschinendaten nach der Synchronisation")
            fig.show()
    if __name__ == '__main__':
        plot(standalone_mode=False)

def synchronize(host, port, collection, df_magnet_path, combined_path, start, end, timeshift=1):    #df_machine_path, 
    # Read in magnet dataframe
    df_magnet = readIn(df_magnet_path)
    
    # Read in machine dataframe
    start_date = datetime.strptime(start, "%Y-%m-%dT%H:%M:%S.%f+0000")
    end_date = datetime.strptime(end, "%Y-%m-%dT%H:%M:%S.%f+0000")

    print(host)
    print(start + " - " + end)
    client = py.MongoClient(host= host, port=int(port))
    db = client.h4ai
    event_list = db[collection].find({ "date" : { '$gte' : start_date, '$lt' : end_date} }).sort('date', 1)

    df_machine = pd.DataFrame()
    for event in event_list:
        content = event["content"]
        for data in content:
            raw_data = data["raw_data"]
            keylist = raw_data.keys()
            raw_data["date"] = data["date"]
            raw_data["given2model"] = data["given2model"]
            raw_data["prediction"] = data["prediction"]
            df_row = pd.DataFrame(raw_data, index= ["date"])
            df_machine = pd.concat([df_machine, df_row], axis=0, ignore_index=True)
    df_machine.reset_index(inplace=True)
    #print(df_machine.head(5))

    # Synchronize dataframes
    df_machine["date"] = pd.to_datetime(df_machine["date"])
    df_machine["date"] = df_machine["date"] + timedelta(hours=timeshift)
    df_magnet["date"] = pd.to_datetime(df_magnet["date"])
    df_new = df_magnet.set_index('date').resample('1s').interpolate("linear")
    df_combined = pd.merge_asof(df_machine, df_new, on="date", direction = "nearest", suffixes=('_machine', '_magnet'), )

    df_combined.to_csv(combined_path)
    maybePlot(df_combined)


if __name__ == '__main__':
    useConfig()