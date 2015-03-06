#ifndef PEBBLENOTIFICATION_H
#define PEBBLENOTIFICATION_H

#include <QObject>
#include <QHash>
#include <QStringList>
#include "pebbleaction.h"
#include "pebbleattribute.h"

class PebbleNotification
{
public:
    PebbleNotification(const QString &_title, const QString &_subtitle, const QString &_body);
    ~PebbleNotification();

    void addDismissAction(const QString &_title);
    void addGenericAction(const QString &_title);
    void addResponseAction(const QString &_title, const QStringList &_parameter = QStringList());

    QString getId();
    QHash<QString, QVariant> getValues();

private:
    QString m_title;
    QString m_subtitle;
    QString m_body;

    QString m_id;

    int m_actionIndex;
    int m_attributeIndex;

    QList<PebbleAction> m_actions;
    QList<PebbleAttribute> m_attributes;
};

#endif // PEBBLENOTIFICATION_H
