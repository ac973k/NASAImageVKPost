#include "nasaimagevkpost.h"

#include <fstream>

#include <QNetworkReply> //класс запросов

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#include <QMessageBox>

NASAImageVKPost::NASAImageVKPost(QWidget *parent)
    : QWidget(parent)
{
    std::ofstream files("Settings.ini", std::ofstream::app); //создаём конфиг файл с настройками, что бы при перезапуск не приходилось снова авторизоваться
    if(!files.is_open())
    {
        //textLog->append("Error!"); //проверяем существование файла
    }

    btnStart = new QPushButton("Start");
    btnSettings = new QPushButton("Settings");
    btnAbout = new QPushButton("About");

    set = new QSettings("Settings.ini", QSettings::IniFormat);
    QString Auth = set->value("General/Auth").toString();

    if (Auth == "False" || Auth == nullptr) {
        btnStart->setEnabled(false);

        set->setValue("General/VKToken", "");
        set->setValue("General/NASAToken", "");
        set->setValue("General/TimeOutH", "01");
        set->setValue("General/TimeOutM", "00");
        set->setValue("General/TimeOutS", "00");
        set->setValue("General/GroupID", "");
        set->setValue("General/UserID", "");
        set->setValue("General/Auth", "False");

        set->sync();
    }

    Hour = set->value("General/TimeOutH").toInt();
    Min = set->value("General/TimeOutM").toInt();
    Sec = set->value("General/TimeOutS").toInt();

    m_timer = new QTimer();
    m_time = new QTime(Hour, Min, Sec);

    lcdNumber = new QLCDNumber();
    lcdNumber->setDigitCount(8);
    lcdNumber->display(m_time->toString());

    managerNASA = new QNetworkAccessManager();
    managerVK = new QNetworkAccessManager();

    mainLayout = new QGridLayout;
    mainLayout->addWidget(lcdNumber);
    mainLayout->addWidget(btnStart);
    mainLayout->addWidget(btnSettings);
    mainLayout->addWidget(btnAbout);

    setLayout(mainLayout);

    QObject::connect(btnStart, SIGNAL(clicked()), this, SLOT(btnStart_clicked()));
    QObject::connect(btnSettings, SIGNAL(clicked()), this, SLOT(btnSettings_clicked()));
}

NASAImageVKPost::~NASAImageVKPost()
{
    delete lcdNumber;

    delete btnStart;
    delete btnSettings;
    delete btnAbout;

    delete mainLayout;

    delete m_timer;
    delete m_time;

    delete set;

    delete settings;

    delete managerNASA;
    delete managerVK;
}

void NASAImageVKPost::update() {
    QString time_str = "";

    *m_time = m_time->addSecs(-1);

    time_str = m_time->toString("hh:mm:ss");

    lcdNumber->display(time_str);

    if (m_time->hour() == 0 && m_time->minute() == 0 && m_time->second() == 0) {
            m_timer->stop();
            lcdNumber->display("OK");

            NASAToken = set->value("General/NASAToken").toString(); //получаем значение токена

            QString url1 = "https://api.nasa.gov/planetary/apod?api_key=" + NASAToken + "&count=1"; //наша ссылка со встроенными нам данными

            QObject::connect(managerNASA, SIGNAL(finished(QNetworkReply *)), this, SLOT(onResultNASA(QNetworkReply*))); //выполняем наш запрос и получаем ответ от сервера. Успешно или ошибка
            managerNASA->get(QNetworkRequest(url1));

            m_time->setHMS(Hour, Min, Sec);
            m_timer->start();
        }
}

void NASAImageVKPost::btnStart_clicked() {
    m_timer->setInterval(1000);
    QObject::connect(m_timer, SIGNAL(timeout()), SLOT(update()));
    m_timer->start();
}

void NASAImageVKPost::btnSettings_clicked() {
    settings = new Settings();
    settings->show();
}

void NASAImageVKPost::btnAbout_clicked() {

}

void NASAImageVKPost::onResultNASA(QNetworkReply *reply) {
    QString strk = "";
    QString attach = "";

    if(!reply->error())
        {
            QString str = reply->readAll();

            QJsonDocument doc = QJsonDocument::fromJson(str.toUtf8());
            QJsonArray jarr = doc.array();

            foreach(QJsonValue val, jarr){
                QJsonObject obj = val.toObject();
                strk = obj.value("copytight").toString();
                strk += "\n" + obj.value("date").toString();
                strk += "\n" + obj.value("explanation").toString();
                attach = obj.value("hdurl").toString();
            }

            reply->deleteLater();
        }

    GroupID = set->value("General/GroupID").toString();
    VKToken = set->value("General/VKToken").toString();
    QString url = "https://api.vk.com/method/wall.post?owner_id=-" + GroupID + "&message=" + strk + "&attachments=" + attach + "&access_token=" + VKToken + "&v=5.131";

    QObject::connect(managerVK, SIGNAL(finished(QNetworkReply *)), this, SLOT(onResultVK(QNetworkReply*))); //отправляем данные и получаем ответ от вк успешно или ошибка
    managerVK->get(QNetworkRequest(url));
}

void NASAImageVKPost::onResultVK(QNetworkReply *reply) {
        // Если ошибки отсутсвуют
        if(!reply->error())
        {
            qDebug() << reply->readAll();
        }
        reply->deleteLater();
}
