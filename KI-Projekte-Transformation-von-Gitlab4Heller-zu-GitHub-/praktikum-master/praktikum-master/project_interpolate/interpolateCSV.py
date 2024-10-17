import numpy as np
import pandas as pd
import yaml

import click
@click.command()
@click.option('--readconfig', prompt = 'Read Infos from config file? (True / False)', type = bool)
def useConfig(readconfig):
    if readconfig:
        with open("X:\\KI Praktikum\\praktikum\\project_interpolate\\config.yaml", "r") as configfile:
            data = yaml.load_all(configfile, Loader=yaml.FullLoader)
            print(data)
            for file in data:
                filepath_in = file["filepath"]
                ipofilepath_in = file["ipofilepath"]
                interpolateCSV(filepath_in, ipofilepath_in)
    else:
        @click.command()
        @click.option('--filepath', prompt = 'Filepath to read in from')
        @click.option('--ipofilepath', prompt = 'Filepath to write the interpolated file')
        def promptPaths(filepath, ipofilepath):
            filepath_in = filepath
            ipofilepath_in = ipofilepath
            interpolateCSV(filepath_in, ipofilepath_in)
        if __name__ == '__main__':
            promptPaths()



def interpolateCSV(filepath, ipofilepath):
    df = pd.read_csv(filepath)
    df_cols = df.copy()
    df_cols['date'] = pd.to_datetime(df_cols['date'])
    df_cols['welle_z_ipo'] = df_cols['welle_z']
    newVal = df_cols['welle_z_ipo'].iloc[0]
    lastVal = df_cols['welle_z_ipo'].iloc[-1]

    for index, row in df_cols.iterrows():
        if index > 0 and row['welle_z_ipo'] == newVal:
            df_cols['welle_z_ipo'].iloc[index] = np.nan
        else: 
            if index > 0:
                newVal = row['welle_z_ipo']

    df_cols.index = df_cols['date']
    df_cols['welle_z_ipo'].interpolate(method='time', inplace = True)
    df_cols.index = df_cols['index']

    df_cols.to_csv(ipofilepath)



if __name__ == '__main__':
    useConfig()


