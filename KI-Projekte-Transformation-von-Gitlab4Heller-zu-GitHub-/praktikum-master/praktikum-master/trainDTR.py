
import plotly.graph_objects as go
from plotly.subplots import make_subplots
import plotly.express as ex
import plotly.io as pio
from sklearn import tree

def trainDTR(x_transform, y_train, x_validation, y_validation, scatter_mode):
    dtr = tree.DecisionTreeRegressor(min_samples_leaf=2)
    dtr.fit(x_transform, y_train)
    score_dtr = dtr.score(x_validation, y_validation)
    print(score_dtr)
    dtr_pred_train = dtr.predict(x_transform)
    figTrain= make_subplots(rows=1, cols=1,shared_xaxes= True, print_grid= True)

    figTrain.add_trace(go.Scatter( y= y_train[100000:120000], name= 'y-train', mode= scatter_mode), row= 1, col= 1)
    figTrain.add_trace(go.Scatter( y= dtr_pred_train[100000:120000], name= 'y-predicted', mode= scatter_mode), row= 1, col= 1)
    figTrain.update_yaxes(title_text= 'y-train', row= 1, col= 1)

    figTrain.update_layout(height=600, width=1200, title_text="Predictions for training data Decision Tree Reg.")
    #figTrain.show() 
    #figTrain.write_image("X:\\KI Praktikum\\validate_Data\\Bilder\\plot_training_data.jpeg")


    dtr_pred_validation = dtr.predict(x_validation)
    figValidation= make_subplots(rows=1, cols=1,shared_xaxes= True, print_grid= True)

    figValidation.add_trace(go.Scatter( y= y_validation[0:10000], name= 'y-values', mode= scatter_mode), row= 1, col= 1)
    figValidation.add_trace(go.Scatter( y= dtr_pred_validation[10:10000], name= 'y-predicted', mode= scatter_mode), row= 1, col= 1)
    figValidation.update_yaxes(title_text= 'y-train', row= 1, col= 1)

    figValidation.update_layout(height=600, width=1200, title_text="Predictions for validation data Decision Tree Reg.")
    #figValidation.show()
    #fig.write_image("X:\\KI Praktikum\\validate_Data\\Bilder\\plot_validation_data.jpeg") 

    return score_dtr, figTrain, figValidation