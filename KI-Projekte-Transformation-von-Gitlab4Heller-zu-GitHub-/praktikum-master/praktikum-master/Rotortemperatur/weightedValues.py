
'''
Function to compute the weight of the y-values
Input:
df:                     Dataframe
weightMin, weightMax:   boundaries of the weighting
namey:                  Name of the column of the dataframe which should be considered for the weighting
nameWeights:            Name of a column which will be added to the dataframe with the computed weights
'''

import numpy as np

def weightValues(df, weightMin=1, weightMax=2, namey="y", nameWeights="weights"):
                # berechne Gewichtung in Bereich 0, 1
    b = 1       # transformiere dann in den gewünschten Bereich (weightMin bis weightMax)
    # c = weightMin      
    # d = weightMax
    num_val = len(df)
    hist, bin_edges = np.histogram(df[namey])
    weight_arr = np.zeros(df.shape[0])
    for index, row in df.iterrows():
        for e in range(len(bin_edges)-1):
            if row[namey] == max(bin_edges):
                weight_arr[index] = 1 - (hist[-1]/num_val)
                break
            if row[namey] < bin_edges[e+1]:
                weight_arr[index] = 1 - (hist[e]/num_val)
                break
    weight_arr = ((weightMax-weightMin)/b)*weight_arr + weightMin
    df[nameWeights] = weight_arr.tolist()
    return df