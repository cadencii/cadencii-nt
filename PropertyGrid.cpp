#include "PropertyGrid.h"
#include "ui_PropertyGrid.h"

PropertyGrid::PropertyGrid(QWidget *parent) :
    QDesignerPropertyEditorInterface(parent),
    ui(new Ui::PropertyGrid)
{
    ui->setupUi(this);
}

PropertyGrid::~PropertyGrid()
{
    delete ui;
}

bool PropertyGrid::isReadOnly()
{
    return true;
}

QObject* PropertyGrid::object()
{
    return NULL;
}

QString PropertyGrid::currentPropertyName()
{
    return "";
}

void PropertyGrid::setObject(QObject*)
{

}

void PropertyGrid::setPropertyValue(const QString&, const QVariant&, bool)
{

}

void PropertyGrid::setReadOnly( bool )
{

}
