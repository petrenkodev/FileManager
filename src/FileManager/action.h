#ifndef ACTION_H
#define ACTION_H

#include <QAction>
#include <QObject>

class Action : public QObject
{
    Q_OBJECT

public:
    QAction *copy;
    QAction *properties;

    explicit Action(QObject *parent = nullptr);
};

#endif // ACTION_H
