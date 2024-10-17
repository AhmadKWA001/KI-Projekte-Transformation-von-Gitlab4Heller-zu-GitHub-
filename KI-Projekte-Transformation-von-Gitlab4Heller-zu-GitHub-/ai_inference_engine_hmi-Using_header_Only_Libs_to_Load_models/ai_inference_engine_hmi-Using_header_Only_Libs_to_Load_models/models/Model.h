#pragma once
#include "Asset.h"

class Model :
    public Asset
{
    Q_OBJECT
private:
    QVector<int>* input_shape;
    QVector<int>* output_shape;
    int buffer_size;
public:
    Model(Assets_Handler* assets_handler);
    Model( QVector<int>* input_shape, QVector<int>* output_shape, Assets_Handler* assets_handler);
    ~Model();
    void set_input_shape(QVector<int>* input_shape);
    void set_output_shape(QVector<int>* output_shape);
    QVector<int>* get_input_shape();
    QVector<int>* get_output_shape();
    int get_buffer_size();
    QVector<float> execute(QVector<float> input, int batch_size, int sequence_length, int feature_dim, int input_dim, bool is_model);
    QVector<float> execute(QVector<float> input_data1, QVector<float> input_data2, QVector<int>* input_dim1, QVector<int>* input_dim2);

};

