#ifndef PEBBLEACTION_H
#define PEBBLEACTION_H

#include <QObject>
#include <QHash>
#include "pebbleattribute.h"

class PebbleAction
{
public:
    PebbleAction(int _index, const QString &_type, const QString &_title);
    ~PebbleAction();

    void addAttribute(PebbleAttribute _attribute);
    QHash<QString, QVariant> getValues();

private:
    int m_index;
    QString m_type;
    QString m_title;

    QList<PebbleAttribute> m_attributes;
};

#endif // PEBBLEACTION_H
