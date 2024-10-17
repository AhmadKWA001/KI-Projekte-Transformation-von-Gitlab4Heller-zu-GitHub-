from assets_utilities.AssetsHandler import  AssetsHandler
import onnxruntime as ort
class OnnxHandler (AssetsHandler):
    ### An implemented function to load onnx session and save it in the inherited private variable
    def load(self, file_path):
        try:
            loaded_asset = ort.InferenceSession(file_path,)
            self.set_asset_model(asset_model= loaded_asset)
        except FileNotFoundError as e:
            raise e
    ### An implemented function to calculate the prediction of onnx asset
    def execute(self, input_array):
        try:
            result= self.get_asset_model().run(None, {'x': input_array})
            return result
        except Exception as e:
            raise e

