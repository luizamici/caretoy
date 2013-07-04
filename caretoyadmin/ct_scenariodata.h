#ifndef CT_SCENARIODATA_H
#define CT_SCENARIODATA_H
#include <QtCore>

class CTScData
{
public:
    CTScData(){}

    QString id;
    QString xml_description;
    QString description;
    QString creation_date;
    QString execution_day;
    QString execution_order;
    QString image_description;
    QString position_image;
    QString outcome_measures;
};

//typedef struct
//{
//    QString id;
//    QString xml_description;
//    QString description;
//    QString creation_date;
//    QString execution_day;
//    QString execution_order;
//    QString image_description;
//    QString position_image;
//    QString outcome_measures;
//} ScData;


class CTScenarioData
{
public:
    static CTScenarioData& instance()
    {
        static CTScenarioData instance;
        return instance;
    }

    CTScData* data(){return _data;}

//    ScData* data() { return &_data; }

    void clearData(){
        _data->id.clear();
        _data->xml_description.clear();
        _data->description.clear();
        _data->creation_date.clear();
        _data->execution_day.clear();
        _data->execution_order.clear();
        _data->image_description.clear();
        _data->position_image.clear();
        _data->outcome_measures.clear();
    }

private:
    CTScenarioData() { _data = new CTScData();}
    ~CTScenarioData() { delete _data;}
    CTScenarioData(const CTScenarioData &);
    CTScenarioData &operator=(const CTScenarioData &);

//    ScData _data;
    CTScData *_data;

};
#endif // CT_SCENARIODATA_H
