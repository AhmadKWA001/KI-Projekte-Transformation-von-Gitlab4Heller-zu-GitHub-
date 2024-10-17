#pragma once
#include "Asset.h"
#include "Json_Handler.h"
#include <QJsonArray>


class Configs :
    public Asset
{
    Q_OBJECT
private:
    QVector<int>*  model_input_shape;
    QVector<int>* model_output_shape;
    QVector<QString>* input_signals;
    QVector<float>* cov_matrix;
    QVector<int>* cov_matrix_shape;
    QString model_path;
    QString preprocessor_path;
    QString prediction_address;
    QHash<QString, QString> addresses;
    int time_relevance;
    int recording_Frequency;
    float sleeping_period;
    int response_time;
    int timeout;
    ///

    QVector<int>* read_model_input_shape();
    QVector<int>* read_model_output_shape();
    QVector<float>* read_cov_matrix();
    QVector<int>* read_cov_matrix_shape();
    QVector<QString>* read_input_signals();
    QString read_model_path();
    QString read_preprocessor_path();
    QString read_prediction_address();
    QHash<QString, QString> read_addresses();
    int read_time_relevance();
    int read_recording_Frequency();
    float read_sleeping_period();
    int read_response_time();
    int read_timeout();
public:
    Configs(Assets_Handler* assets_Handler);
    ~Configs();
    void read_Configs();
    QVector<int>*  get_model_input_shape();
    QVector<int>* get_model_output_shape();
    QVector<float>* get_cov_matrix();
    QVector<int>* get_cov_matrix_shape();
    QVector<QString>* get_input_signals();
    QString get_model_path();
    QString get_preprocessor_path();
    QString get_prediction_address();
    QHash<QString, QString> get_addresses();
    int get_time_relevance();
    int get_recording_Frequency();
    float get_sleeping_period();
    int* get_response_time();
    int get_timeout();

};

