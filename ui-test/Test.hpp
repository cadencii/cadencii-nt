#include <QThread>
#include <QApplication>
#include <QWidget>
#include "../qt/ConcreteSettings.hpp"

class SettingsStub : public cadencii::ConcreteSettings{
public:
    SettingsStub(){
        isUnitTest = true;
    }
};

class Test : public QObject{
    Q_OBJECT

public:
    Test();

private slots:
    /**
     * @brief 読み込みに失敗するVSQファイルが渡されてもクラッシュしないこと
     */
    void openVsqFileWithException();
};
