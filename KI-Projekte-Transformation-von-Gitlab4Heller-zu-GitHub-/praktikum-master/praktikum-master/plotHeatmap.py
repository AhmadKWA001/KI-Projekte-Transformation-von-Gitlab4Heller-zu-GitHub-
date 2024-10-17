
import seaborn
import matplotlib.pyplot as plt

def plotHeatmap(df, show, t_bett_abgezogen, z_interpoliert):
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