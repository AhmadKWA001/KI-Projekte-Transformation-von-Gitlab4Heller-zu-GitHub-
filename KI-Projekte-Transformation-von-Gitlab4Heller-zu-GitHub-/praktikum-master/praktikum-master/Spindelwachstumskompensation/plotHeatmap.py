
'''
Function to plot a heatmap. 
Depending on the required heatmap, t_bett can be substracted within the function
or the welle_z interpolated based on the time can be used in the heatmap as well.
Input:
df:                 Dataframe to use for the correlation analysis
t_bett_abgezogen:   Should t_bett be substracted? [bool]
z_interpoliert:     is welle_z_ipo_time available and should be used? [bool]
show:               [bool]
'''

import seaborn
import matplotlib.pyplot as plt

def plotHeatmap(df, t_bett_abgezogen, z_interpoliert, show=True):
    if show:
        if t_bett_abgezogen:
            df_heatmap = df.copy()
            df_heatmap["t_motor"] = df_heatmap["t_motor"] - df_heatmap["t_bett"]
            df_heatmap["t_spindle"] = df_heatmap["t_spindle"] - df_heatmap["t_bett"]
            df_heatmap = df_heatmap.drop(columns = ["t_bett", "Unnamed: 0.1", "date", "given2model", "welle_z", "welle_z_iterpolated", "filename"])
            cor = df_heatmap.corr()
            seaborn.heatmap(cor, annot=True, vmin= -1, vmax=1)
            plt.title("Korrelation mit der interpolierten Welle_z, Betttemperaturen wurden von den Motor- und Spindeltemperaturen abgezogen")
            

        else: 
            if z_interpoliert:
                df_heatmap = df.drop(columns = ["Unnamed: 0.1", "date", "given2model", "welle_z_ipo_time", "welle_z_iterpolated", "filename"])
                cor = df_heatmap.corr()
                seaborn.heatmap(cor, annot=True, vmin= -1, vmax=1)
                plt.title("Korrelation mit der gemessenen Welle_z")
                
            
            else:
                df_heatmap = df.drop(columns = ["Unnamed: 0.1", "date", "given2model", "welle_z", "welle_z_iterpolated", "filename"])
                cor = df_heatmap.corr()
                seaborn.heatmap(cor, annot=True, vmin= -1, vmax=1)
                plt.title("Korrelation mit der interpolierten Welle_z")
                
        return plt
    else: return None