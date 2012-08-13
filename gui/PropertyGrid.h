#ifndef PROPERTYGRID_H
#define PROPERTYGRID_H

#include <QtDesigner/QDesignerPropertyEditorInterface>

namespace Ui {
    class PropertyGrid;
}

class PropertyGrid : public QDesignerPropertyEditorInterface
{
    Q_OBJECT
    
public:
    explicit PropertyGrid(QWidget *parent = 0);
    ~PropertyGrid();

    PropertyGrid::~PropertyGrid()
    {
        delete ui;
    }

    bool PropertyGrid::isReadOnly();

    QObject* PropertyGrid::object();

    QString PropertyGrid::currentPropertyName();

public slots:
    void PropertyGrid::setObject(QObject*);

    void PropertyGrid::setPropertyValue(const QString&, const QVariant&, bool);

    void PropertyGrid::setReadOnly( bool );

private:
    Ui::PropertyGrid *ui;
};

#endif // PROPERTYGRID_H
