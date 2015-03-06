#include "pebbleaction.h"
#include <QVariant>

PebbleAction::PebbleAction(int _index, const QString &_type, const QString &_title) : m_index(_index), m_type(_type), m_title(_title)
{

}

PebbleAction::~PebbleAction()
{

}

void PebbleAction::addAttribute(PebbleAttribute _attribute)
{
    m_attributes << _attribute;
}

QHash<QString, QVariant> PebbleAction::getValues()
{
    QHash<QString, QVariant> values;

    QString actionIndex = QString::number(m_index);

    values.insert("ACTION_" + actionIndex + "_TITLE", m_title);
    values.insert("ACTION_" + actionIndex + "_TYPE", m_type);

    values.insert("ACTION_" + actionIndex + "_ATTRIBUTES", m_attributes.size());
    for(int i=0; i<m_attributes.size(); i++)
    {
        PebbleAttribute attribute = m_attributes.at(i);
        values.unite(attribute.getValues());
    }

    return values;
}
