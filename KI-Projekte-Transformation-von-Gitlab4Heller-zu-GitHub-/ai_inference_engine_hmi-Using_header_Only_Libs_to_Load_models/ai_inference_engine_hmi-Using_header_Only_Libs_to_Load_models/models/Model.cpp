#include "Model.h"

Model::Model(Assets_Handler* assets_handler):Asset(assets_handler)
{

}
Model::Model( QVector<int>* input_shape, QVector<int>* output_shape, Assets_Handler* assets_handler)
{
    this->input_shape = input_shape;
    this->output_shape = output_shape;
	if (input_shape->size() >= 2)
	{	
		this->buffer_size = (*input_shape)[1]; 
	}
	else {
		this->buffer_size = 1;
	}
	this->assets_handler = assets_handler;

}
Model::~Model()
{
}
void Model::set_input_shape(QVector<int>* input_shape)
{
	this->input_shape = input_shape;
}
void Model::set_output_shape(QVector<int>* output_shape)
{
	this->output_shape = output_shape;
}
QVector<int>* Model::get_input_shape()
{
	return this->input_shape;
}

QVector<int>* Model::get_output_shape()
{
	return this->output_shape;
}

int Model::get_buffer_size()
{
	return this->buffer_size;
}

QVector<float> Model::execute(QVector<float> input, int batch_size, int sequence_length, int feature_dim, int input_dim, bool is_model)
{ 
	QVector<float> result= this->assets_handler->execute(input.toStdVector(),  batch_size,  sequence_length,  feature_dim, input_dim,is_model);
	return result;
}
QVector<float> Model::execute(QVector<float> input_data1, QVector<float> input_data2, QVector<int>* input_dim1, QVector<int>* input_dim2)
{
	auto input_dim1_vec = input_dim1->toStdVector();
	auto input_dim2_vec = input_dim2->toStdVector();
	QVector<float> result = this->assets_handler->execute(input_data1.toStdVector(), input_data2.toStdVector(),  &input_dim1_vec, &input_dim2_vec);
	return result;
}
