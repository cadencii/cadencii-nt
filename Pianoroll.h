#ifndef PIANOROLL_H
#define PIANOROLL_H

#include <QWidget>
#include <QPainter>

namespace Ui {
    class Pianoroll;
}

class Pianoroll : public QWidget
{
    Q_OBJECT
    
public:
    explicit Pianoroll(QWidget *parent = 0);
    ~Pianoroll();

    /**
     * 可視となっている領域を取得する
     * @return 取得した領域の格納先
     */
    QRect getVisibleArea();

protected:
    void paintEvent( QPaintEvent * );

private:
    Ui::Pianoroll *ui;
};

#endif // PIANOROLL_H
