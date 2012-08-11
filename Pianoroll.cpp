#include "Pianoroll.h"
#include "QScrollArea"
#include <string>
#include "ui_Pianoroll.h"

Pianoroll::Pianoroll(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Pianoroll)
{
    ui->setupUi(this);
}

Pianoroll::~Pianoroll()
{
    delete ui;
}

void Pianoroll::paintEvent( QPaintEvent * )
{
    QPainter p( this );
    int width = this->width();
    int height = this->height();
    p.fillRect( QRect( 0, 0, width, height ), QColor( 80, 80, 80 ) );

    int margin = 10;
    QRect rect = this->getVisibleArea();
    p.fillRect( rect.x() + margin, rect.y() + margin, rect.width() - 2 * margin, rect.height() - 2 * margin, QColor( 200, 200, 200 ) );
    char text[5120];
    sprintf( text, "{x=%d, y=%d, width=%d, height=%d}", rect.x(), rect.y(), rect.width(), rect.height() );
    p.drawText( 200, 200, text );
}

QRect Pianoroll::getVisibleArea()
{
    QScrollArea *scroll = (QScrollArea *)this->parent();
    QRect rect = scroll->childrenRect();
    int x = -rect.x();
    int y = -rect.y();
    int width = scroll->width();
    int height = scroll->height();
    return QRect( x, y, width, height );
}
