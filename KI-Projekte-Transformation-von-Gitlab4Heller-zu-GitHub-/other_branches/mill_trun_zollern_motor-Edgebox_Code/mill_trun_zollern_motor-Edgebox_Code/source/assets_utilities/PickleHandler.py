from joblib import load
from assets_utilities.AssetsHandler import  AssetsHandler
class PickleHandler(AssetsHandler):
    ### An implemented function to load pickle asset and save it in the inherited private variable
    def load(self, file_path):
        try:
            loaded_asset = load(file_path)
            self.set_asset_model(asset_model= loaded_asset)
        except FileNotFoundError as e:
            raise e
    ### An implemented function to calculate the prediction of pickle asset
    def execute(self, input_array):
        try:
            result= self.get_asset_model().transform(input_array)
            return result
        except Exception as e:
            raise e

