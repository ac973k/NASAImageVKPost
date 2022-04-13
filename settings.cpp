#include "settings.h"

#include <QUrlQuery>

#include <QMessageBox>

Settings::Settings(QWidget *parent)
    : QWidget{parent}
{
    view = new QWebEngineView;

    set = new QSettings("Settings.ini", QSettings::IniFormat);
    Auth = set->value("General/Auth").toString();
    GroupID = set->value("General/GroupID").toString();
    NASAToken = set->value("General/NASAToken").toString();
    Hour = set->value("General/TimeOutH").toInt();
    Min = set->value("General/TimeOutM").toInt();
    Sec = set->value("General/TimeOutS").toInt();

    labelAuth = new QLabel("Auth: " + Auth);

    //Add GroupBox with Timeout settings

    boxTimeOut = new QGroupBox();
    boxTimeOut->setTitle("Timeout Settings");

    labelH = new QLabel("Hour");
    spinH = new QSpinBox();
    spinH->setValue(Hour);

    labelM = new QLabel("Min");
    spinM = new QSpinBox();
    spinM->setValue(Min);

    labelS = new QLabel("Sec");
    spinS = new QSpinBox();
    spinS->setValue(Sec);

    spinboxHourLayout = new QHBoxLayout;
    spinboxHourLayout->addWidget(labelH);
    spinboxHourLayout->addWidget(spinH);

    spinboxMinLayout = new QHBoxLayout;
    spinboxMinLayout->addWidget(labelM);
    spinboxMinLayout->addWidget(spinM);

    spinboxSecLayout = new QHBoxLayout;
    spinboxSecLayout->addWidget(labelS);
    spinboxSecLayout->addWidget(spinS);

    spinboxVLayout = new QVBoxLayout;
    spinboxVLayout->addLayout(spinboxHourLayout);
    spinboxVLayout->addLayout(spinboxMinLayout);
    spinboxVLayout->addLayout(spinboxSecLayout);

    boxTimeOut->setLayout(spinboxVLayout);



    labelIDGroup = new QLabel("ID Group");
    lineIDGroup = new QLineEdit;
    lineIDGroup->setText(GroupID);

    labelNASAToken = new QLabel("NASA Token");
    lineNASAToken = new QLineEdit;
    lineNASAToken->setText(NASAToken);

    btnAuth = new QPushButton("Auth");
    btnSave = new QPushButton("Save Settings");

    if (Auth == "True") {
        btnAuth->setEnabled(false);
    }

    mainLayout = new QGridLayout;
    mainLayout->addWidget(labelAuth, 0, 0, 1, 2);
    mainLayout->addWidget(boxTimeOut, 1, 0, 1, 2);
    mainLayout->addWidget(labelIDGroup, 2, 0, 1, 1);
    mainLayout->addWidget(lineIDGroup, 2, 1, 1, 1);
    mainLayout->addWidget(labelNASAToken, 3, 0, 1, 1);
    mainLayout->addWidget(lineNASAToken, 3, 1, 1, 1);
    mainLayout->addWidget(btnAuth, 4, 0, 1, 2);
    mainLayout->addWidget(btnSave, 5, 0, 1, 2);

    setLayout(mainLayout);

    QObject::connect(btnAuth, SIGNAL(clicked()), this, SLOT(VKAuth()));
    QObject::connect(btnSave, SIGNAL(clicked()), this, SLOT(SaveSettings()));
}

void Settings::VKAuth() {
    view->load(QUrl("https://oauth.vk.com/authorize?client_id=7660871&display=mobile&redirect_uri=https://oauth.vk.com/blank.html&scope=offline,status,wall,groups,&response_type=token&v=5.126&state=123456"));
    view->show();
    url = view->url();

    QObject::connect(view, SIGNAL(urlChanged(QUrl)), this, SLOT(GetToken(QUrl)));
}

void Settings::GetToken(QUrl url) {
    url = url.toString().replace("%2523","#");
    url = url.toString().replace("#","&");//меняем # на &///// %2523-># %253D->=
    url = url.toString().replace("%253D","=");
    url = url.toString().replace("%253A",":");
    url = url.toString().replace("%252F","/");
    url = url.toString().replace("%2526","&");
    VKToken = QUrlQuery(url).queryItemValue("access_token"); //получаем значение токена
    UserID = QUrlQuery(url).queryItemValue("user_id"); //получаем значение ID

    set->setValue("General/VKToken", VKToken); //сохраняем значение токена в наш файл конфига
    set->setValue("General/UserID", UserID); //сохраняем значение ID в наш файл конфига
    set->setValue("General/Auth", "True");

    set->sync();

    view->close(); //закрываем окно авторизации
}

Settings::~Settings() {
    delete labelAuth;

    delete boxTimeOut;
    delete labelH;
    delete labelM;
    delete labelS;
    delete spinH;
    delete spinM;
    delete spinS;

    delete labelIDGroup;
    delete labelNASAToken;
    delete lineIDGroup;
    delete lineNASAToken;

    delete btnAuth;
    delete btnSave;

    delete mainLayout;
    delete spinboxHourLayout;
    delete spinboxMinLayout;
    delete spinboxSecLayout;
    delete spinboxVLayout;

    delete set;

    delete view; //инициализируем браузер
}

void Settings::SaveSettings() {
    if (lineIDGroup->text() == nullptr || lineNASAToken->text() == nullptr) {
        QMessageBox msg;
        msg.setText("ID Group or NASA Token is empty");
        msg.show();
    }
    else {
        Hour = spinH->value();
        set->setValue("General/TimeoutH", Hour);
        Min = spinM->value();
        set->setValue("General/TimeoutM", Min);
        Sec = spinS->value();
        set->setValue("General/TimeoutS", Sec);

        GroupID = lineIDGroup->text();
        set->setValue("General/GroupID", GroupID);

        NASAToken = lineNASAToken->text();
        set->setValue("General/NASAToken", NASAToken);

        set->sync();

        QMessageBox msg;
        msg.setText("Saved =)");
        msg.show();
    }
}
