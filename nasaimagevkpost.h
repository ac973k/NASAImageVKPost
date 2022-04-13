#ifndef NASAIMAGEVKPOST_H
#define NASAIMAGEVKPOST_H

#include <QWidget>

#include <QLCDNumber>
#include <QPushButton>

#include <QGridLayout>

#include <QTime>
#include <QTimer>

#include <QSettings>

#include "settings.h"

#include <QNetworkAccessManager> //класс для работы с сетью

class NASAImageVKPost : public QWidget
{
    Q_OBJECT

public:
    NASAImageVKPost(QWidget *parent = nullptr);
    ~NASAImageVKPost();

private:
    QLCDNumber *lcdNumber;

    QPushButton *btnStart;
    QPushButton *btnSettings;
    QPushButton *btnAbout;

    QGridLayout *mainLayout;

    QTimer *m_timer;
    QTime *m_time;

    QSettings *set;

    int Hour;
    int Min;
    int Sec;

    Settings *settings;

    QNetworkAccessManager *managerNASA;
    QNetworkAccessManager *managerVK;

    QString GroupID;
    QString NASAToken;

    QString VKToken; //тут будем хранить токен
    QString UserID; //тут id

private slots:
    void update();
    void btnStart_clicked();
    void btnSettings_clicked();
    void btnAbout_clicked();

    void onResultNASA(QNetworkReply *reply); //вывод результата
    void onResultVK(QNetworkReply *reply);
};
#endif // NASAIMAGEVKPOST_H
