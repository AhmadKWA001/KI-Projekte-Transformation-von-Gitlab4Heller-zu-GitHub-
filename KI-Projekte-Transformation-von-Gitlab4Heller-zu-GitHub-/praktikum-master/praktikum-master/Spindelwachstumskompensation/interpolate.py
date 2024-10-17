
'''
The function interpolates the column of a dataframe and saves the result into 
a new column. 
Expected column names:
"date" :                Used as index to interpolate with respect to
"welle_z" :             Column which will be interpolated
"welle_z_ipo_time" :    Result of the interpolation
'''

import numpy as np
import pandas as pd

from datetime import datetime, timedelta 

import plotly.graph_objects as go
from plotly.subplots import make_subplots
import plotly.express as ex
import plotly.io as pio


def interpolate(df):
    # Compute the interpolated values for welle_z and plot the original and interpolated values of welle_z
    df['date'] = pd.to_datetime(df['date'])
    # Based on time and pandas
    df['welle_z_ipo_time'] = df['welle_z']
    newVal = df['welle_z_ipo_time'].iloc[0]
    lastVal = df['welle_z_ipo_time'].iloc[-1]

    for index, row in df.iterrows():
        if index > 0 and row['welle_z_ipo_time'] == newVal:
            df['welle_z_ipo_time'].iloc[index] = np.nan
        else: 
            if index > 0:
                newVal = row['welle_z_ipo_time']

    df.index = df['date']
    df['welle_z_ipo_time'].interpolate(method='time', inplace = True)
    df.index = df['Unnamed: 0']

    
    return df