#include "pebbleattribute.h"

PebbleAttribute::PebbleAttribute(int _attributeIndex, int _actionId, const QString &_type, const QStringList &_parameters) :m_attributeIndex(_attributeIndex), m_actionId(_actionId), m_type(_type), m_parameters(_parameters)
{
}

PebbleAttribute::~PebbleAttribute()
{

}

QHash<QString, QVariant> PebbleAttribute::getValues()
{
    QHash<QString, QVariant> values;

    QString actionId = QString::number(m_actionId);
    QString attributeIndex = QString::number(m_attributeIndex);

    QString prefix = "";
    if(m_actionId>-1)
        prefix = "ACTION_" + actionId + "_ATTRIBUTE_" + attributeIndex + "_";
    else
        prefix = "NOTIFICATION_ATTRIBUTE_" + attributeIndex + "_";

    values.insert(prefix + "TYPE", m_type);
    values.insert(prefix + "PARAMETERS", m_parameters.size());

    for(int i=0; i<m_parameters.size(); i++)
    {
        QString key = prefix + "PARAMETER_" + QString::number(i);
        values.insert(key, m_parameters.at(i));
    }

    return values;
}
