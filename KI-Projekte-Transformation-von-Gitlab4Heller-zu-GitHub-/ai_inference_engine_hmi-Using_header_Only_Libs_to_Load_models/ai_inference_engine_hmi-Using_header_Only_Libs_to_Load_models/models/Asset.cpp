#include "Asset.h"

Asset::Asset(Assets_Handler* assets_handler)
{
	this->assets_handler = assets_handler;
}

Assets_Handler* Asset::get_assets_handler()
{
	return this->assets_handler;
}
//QDateTime Asset::get_creation_date()
//{
//	return this->creation_date;
//}
//void Asset::set_creation_date(QDateTime creation_date)
//{
//	this->creation_date = creation_date;
//}

/*QString Asset::get_path()
{
	return this->path;
}
void Asset::set_path(QString path)
{
	this->path = path;
}*/
bool Asset::is_changed(){
	return this->assets_handler->is_changed();
}
void Asset::load()
{
	this->assets_handler->slot_load();
}
Asset::~Asset()
{

}