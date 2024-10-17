### An Interface representing the structure of the assets Handler
class AssetsHandler:
    ### The api needed to load the asset file
    _asset_model: None
    ### A virtual function to load the asset
    def load(self, file_path):
        raise NotImplementedError
    ### A virtual function to apply the functionality of the asset
    def execute(self, input_array):
        raise NotImplementedError

    def get_asset_model(self):
        return self._asset_model
    def set_asset_model(self, asset_model):
        self._asset_model= asset_model



