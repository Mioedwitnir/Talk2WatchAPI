#ifndef PEBBLEATTRIBUTE_H
#define PEBBLEATTRIBUTE_H

#include <QObject>
#include <QHash>
#include <QStringList>
#include <QVariant>

class PebbleAttribute
{
public:
    PebbleAttribute(int _attributeIndex, int _actionId, const QString &_type, const QStringList &_parameters);
    ~PebbleAttribute();

    QHash<QString, QVariant> getValues();

private:
    int m_attributeIndex;
    int m_actionId;
    QString m_type;
    QStringList m_parameters;
};

#endif // PEBBLEATTRIBUTE_H
