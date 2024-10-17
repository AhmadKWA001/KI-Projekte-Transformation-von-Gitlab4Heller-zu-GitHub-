
'''
Function to read in several csv files and 
concatenate them to one merged dataframe
'''

import pandas as pd
import glob
import os


def readIn(workdir):
    all_files = glob.glob(os.path.join(workdir, "*.csv"))
    df_merged = pd.DataFrame()

    for f in all_files:
        df_from_each_file = pd.read_csv(f)
        df_from_each_file["filename"] = f
        df_merged = pd.concat([df_merged, df_from_each_file])

    df_merged.reset_index(drop=True, inplace=True)
    
    return df_merged