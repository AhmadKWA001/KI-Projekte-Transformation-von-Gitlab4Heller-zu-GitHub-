

'''
Function to read in several csv files and 
concatenate them to one merged dataframe.
If needed: 
    - the welle_z can be interpolated
    - the single csv files can be saved
'''

import pandas as pd

import glob
import os
from interpolate import interpolate


def readIn(workdir, ipo=False, safeSingleCsv=False):
    all_files = glob.glob(os.path.join(workdir, "*.csv"))
    df_merged = pd.DataFrame()

    if ipo:
        for f in all_files:
            df_from_each_file = pd.read_csv(f)
            df_ipo = interpolate(df_from_each_file)
            df_ipo["filename"] = f
            df_merged = pd.concat([df_merged, df_ipo])

            if safeSingleCsv:
                goalpath = f[:-4] + str("_time_ipo.csv")
                df_ipo.to_csv(goalpath)
    else:
        for f in all_files:
            df_from_each_file = pd.read_csv(f)
            df_from_each_file["filename"] = f
            df_merged = pd.concat([df_merged, df_from_each_file])

    df_merged.reset_index(drop=True, inplace=True)
    
    return df_merged