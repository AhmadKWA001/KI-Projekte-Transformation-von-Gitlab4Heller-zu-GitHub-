
'''
Function to plot the signals usually used to predict the spindle expansion.
Either only 'welle_z' and the interpolated version 'welle_z_iterpolated' are plotted
or all signals, spread over 2 rows and 2 columns.
'''

import plotly.graph_objects as go
from plotly.subplots import make_subplots
import plotly.express as ex
import plotly.io as pio




def plot(df, plot_ipo, scatter_mode):
    if plot_ipo:
        # Plot the interpolated data frame
        fig= make_subplots(rows=1, cols=1, shared_xaxes= True, print_grid= True, vertical_spacing=0.02)

        fig.add_trace(go.Scatter( y= df['welle_z'], name= 'welle_z', mode= scatter_mode), row= 1, col= 1)
        fig.add_trace(go.Scatter( y= df['welle_z_iterpolated'], name= 'welle_z_interpolated', mode= scatter_mode), row= 1, col= 1)
        fig.update_yaxes(title_text= 'Abweichungen [mm]', row= 1, col= 1)

        fig.update_layout(height=600, width=1300, title_text="Welle_z original und interpoliert")
        fig.show()

    else:
        # Plot data in the dataframe
        fig= make_subplots(rows=2, cols=2, shared_xaxes= True, print_grid= True, vertical_spacing=0.02)
        fig.add_trace(go.Scatter(x= df['date'], y= df['t_bett'], name= 't_bett', mode= scatter_mode), row= 1, col= 1)
        fig.add_trace(go.Scatter(x= df['date'], y= df['t_motor'], name= 't_motor', mode= scatter_mode), row= 1, col= 1)
        fig.add_trace(go.Scatter(x= df['date'], y= df['t_spindle'], name= 't_spindle', mode= scatter_mode), row= 1, col= 1)
        fig.add_trace(go.Scatter(x= df['date'], y= df['welle_z'], name= 'welle_z', mode= scatter_mode), row= 1, col= 1)
        fig.update_yaxes(title_text= 'Abweichungen [mm]', row= 1, col= 1)

        fig.add_trace(go.Scatter(x= df['date'], y= df['DRZ2'], name= 'DRZ2', mode= scatter_mode), row= 1, col= 2)
        fig.update_yaxes(title_text= 'Drehzahl [1/min]', row= 1, col= 2)

        fig.add_trace(go.Scatter(x= df['date'], y= df['M8'], name= 'M8', mode= scatter_mode), row= 2, col= 1)
        fig.add_trace(go.Scatter(x= df['date'], y= df['M121'], name= 'M121', mode= scatter_mode), row= 2, col= 1)
        fig.add_trace(go.Scatter(x= df['date'], y= df['M127'], name= 'M127', mode= scatter_mode), row= 2, col= 1)
        fig.add_trace(go.Scatter(x= df['date'], y= df['M7'], name= 'M7', mode= scatter_mode), row= 2, col= 1)
        fig.update_yaxes(title_text= 'Kühlmittelbits', row= 2, col= 1)

        fig.update_layout(height=600, width=1300, title_text="Daten im Dataframe")
        fig.show()