
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

def synchronize(df_machine, df_magnet, timeshift=-1):
    df_machine["date"] = df_machine["date"] + timedelta(hours=timeshift)
    df_magnet["date"] = pd.to_datetime(df_magnet["date"])
    df_new = df_magnet.set_index('date').resample('1s').interpolate("linear")
    df_combined = pd.merge_asof(df_machine, df_new, on="date", direction = "nearest")

    return df_combined