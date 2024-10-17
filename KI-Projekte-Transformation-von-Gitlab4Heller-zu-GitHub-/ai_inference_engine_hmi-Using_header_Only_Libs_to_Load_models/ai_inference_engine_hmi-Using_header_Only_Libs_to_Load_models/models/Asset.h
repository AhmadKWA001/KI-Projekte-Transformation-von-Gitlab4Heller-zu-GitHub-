#pragma once
#include "QObject"
//#include<QDateTime>
#include "Assets_Handler.h"
#include "log.h"
class Asset : public QObject
{
    Q_OBJECT
protected:
    //QDateTime creation_date;
    //QString path;
    Assets_Handler* assets_handler;
public:
    Asset() = default;
    Asset(Assets_Handler* assets_handler);
    Assets_Handler*  get_assets_handler();
    //QDateTime get_creation_date();
   // QString get_path();
    //void set_path(QString path);
    //void set_creation_date(QDateTime creation_date);
    bool is_changed();
    void load();
    ~Asset();
};

