#include "pebblenotification.h"
#include <QVariant>
#include <QDateTime>

PebbleNotification::PebbleNotification(const QString &_title, const QString &_subtitle, const QString &_body) : m_title(_title), m_subtitle(_subtitle), m_body(_body), m_actionIndex(0), m_attributeIndex(0)
{
    //m_attributes << PebbleAttribute(m_attributeIndex++, -1, "TITLE", QStringList() << _title);

    m_attributes << PebbleAttribute(m_attributeIndex++, -1, "BODY", QStringList() << _body);

    if(_subtitle!="")
        m_attributes << PebbleAttribute(m_attributeIndex++, -1, "SUBTITLE", QStringList() << _subtitle);

    m_id = QString::number(QDateTime::currentMSecsSinceEpoch());
}

PebbleNotification::~PebbleNotification()
{

}

void PebbleNotification::addDismissAction(const QString &_title)
{
    m_actions << PebbleAction(m_actionIndex++, "DISMISS", _title);
}

void PebbleNotification::addGenericAction(const QString &_title)
{
    m_actions << PebbleAction(m_actionIndex++, "GENERIC", _title);
}

void PebbleNotification::addResponseAction(const QString &_title, const QStringList &_parameter)
{
    PebbleAction action(m_actionIndex++, "RESPONSE", _title);
    action.addAttribute(PebbleAttribute(0, m_actionIndex-1, "ACTION_CANNED_RESPONSE", _parameter));
    m_actions << action;
}

QString PebbleNotification::getId()
{
    return m_id;
}

QHash<QString, QVariant> PebbleNotification::getValues()
{
    QHash<QString, QVariant> values;

    values.insert("NOTIFICATION_ID", m_id);
    values.insert("NOTIFICATION_TITLE", m_title);
    values.insert("NOTIFICATION_ACTIONS", m_actions.size());
    for(int i=0; i<m_actions.size(); i++)
    {
        PebbleAction action = m_actions.at(i);
        values.unite(action.getValues());
    }

    values.insert("NOTIFICATION_ATTRIBUTES", m_attributes.size());
    for(int i=0; i<m_attributes.size(); i++)
    {
        PebbleAttribute attribute = m_attributes.at(i);
        values.unite(attribute.getValues());
    }

    return values;
}
