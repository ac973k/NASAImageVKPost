#ifndef SETTINGS_H
#define SETTINGS_H

#include <QWidget>

#include <QLabel>
#include <QGroupBox>
#include <QSpinBox>
#include <QLineEdit>
#include <QPushButton>

#include <QGridLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>

#include <QSettings>

#include <QWebEngineView> //класс раузера(надо для авторизации в вк)

class Settings : public QWidget
{
    Q_OBJECT
public:
    explicit Settings(QWidget *parent = nullptr);
    ~Settings();

private:
    QLabel *labelAuth;

    QGroupBox *boxTimeOut;
    QLabel *labelH;
    QLabel *labelM;
    QLabel *labelS;
    QSpinBox *spinH;
    QSpinBox *spinM;
    QSpinBox *spinS;

    QLabel *labelIDGroup;
    QLabel *labelNASAToken;
    QLineEdit *lineIDGroup;
    QLineEdit *lineNASAToken;

    QPushButton *btnAuth;
    QPushButton *btnSave;

    QGridLayout *mainLayout;
    QHBoxLayout *spinboxHourLayout;
    QHBoxLayout *spinboxMinLayout;
    QHBoxLayout *spinboxSecLayout;
    QVBoxLayout *spinboxVLayout;

    QSettings *set;

    QString Auth;
    QString GroupID;
    QString NASAToken;

    QString VKToken; //тут будем хранить токен
    QString UserID; //тут id

    QWebEngineView *view; //инициализируем браузер

    QUrl url; //тут будем хранить наш url

    int Hour;
    int Min;
    int Sec;

private slots:
    void VKAuth(); //метод авторизации
    void GetToken(QUrl url); //метод получения токена
    void SaveSettings();
};

#endif // SETTINGS_H
