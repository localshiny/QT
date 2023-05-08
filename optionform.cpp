//
//
// FileName: optionform.cpp
// Creator: Zhang Runze
// Date: 2021-8-28
//
//
#include "optionform.h"
#include "ui_optionform.h"

Option::Option(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Option)
{
    ui->setupUi(this);
    ui->errorLabel->hide();

    setAttribute(Qt::WA_QuitOnClose, false);
    setWindowTitle("Option");
    this->setFixedSize(QSize(450, 350));

    m_locationPath = QCoreApplication::applicationDirPath();
    m_configPath = m_locationPath + "/config/options.ini";
    m_setting = new QSettings(m_configPath, QSettings::IniFormat);

    connect(ui->filePath,SIGNAL(textChanged(QString)),this,SLOT(judgePath(QString)));

    initOptions();
    setOptions();
    hide();
}

Option::~Option()
{
    delete ui;
}

void Option::closeEvent(QCloseEvent *event)
{
    if(isVisible())
    {
        on_cancelButton_clicked();
    }
    event->ignore();
}

bool Option::isPathValid(QString path)
{
    for (int i = 0; i < path.size(); i ++)
    {
        if (path[i] > 0xff || path[i] == "!" || path[i] == "@" || path[i] == "#" ||
            path[i] == "$" || path[i] == "%" || path[i] == "^" || path[i] == "&" ||
            path[i] == "*" || path[i] == ";" || path[i] == "~" || path[i] == "," ||
            path[i] == "<" || path[i] == ">" || path[i] == "?" || path[i] == " " ||
            path[i] == "\"" || (path[i] == ":" && (path[0] == "/" || i != 1)))
            return false;
    }

    return true;
}

void Option::judgePath(QString path)
{
    if (!isPathValid(path))
    {
        QString output = "The file storage path is invalid!";
        output = QObject::tr("<font color = red>%1</font>").arg(output);
        ui->errorLabel->setText(output);
        ui->errorLabel->show();
        ui->okButton->setEnabled(false);
    }
    else
    {
        ui->errorLabel->hide();
        ui->okButton->setEnabled(true);
    }
}


void Option::initOptions()
{
    QFile* temp = new QFile(m_configPath);
    if (!temp->exists())
    {
        m_setting->beginGroup("console_output");
        m_setting->setValue("position", "right");
        m_setting->endGroup();

        m_setting->beginGroup("server");
        m_setting->setValue("address", "https://www.localshiny.org");
        m_setting->endGroup();

        m_setting->beginGroup("file");
        QString defaultPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
        if ( !isPathValid(defaultPath) ) defaultPath = m_locationPath + "/appdata";
        if ( !isPathValid(defaultPath) )
        {
            QString outputMessage = "Cannot find a suitable file storage path, please add it manually by setting File->Options->File Management->Default Path, otherwise the software will not run normally!";
            QMessageBox::critical(NULL, "Error", outputMessage, QMessageBox::Ok, QMessageBox::Ok);
            defaultPath = "";
        }
        m_setting->setValue("defaultPath", defaultPath);
        m_setting->endGroup();

        m_setting->setValue("initApp", "");
        m_setting->setValue("isFirstTime", "Yes");

        m_setting->sync();
    }
}

void Option::setOptions()
{
    if (m_setting->value("console_output/position").toString() == "right")
    {
        ui->posRight->setChecked(true);
    }
    else
    {
        ui->posBottom->setChecked(true);
    }
    ui->servAddr->setText(m_setting->value("server/address").toString());
    ui->filePath->setText(m_setting->value("file/defaultPath").toString());
}

QMap<QString, QString> Option::getOptions()
{
    QMap<QString, QString> option_map;
    if (ui->posRight->isChecked())
    {
        option_map.insert("console_output/position", "right");
    }
    else
    {
        option_map.insert("console_output/position", "bottom");
    }
    option_map.insert("server/address", ui->servAddr->text());
    option_map.insert("file/defaultPath", ui->filePath->text());
    option_map.insert("initApp", m_setting->value("initApp").toString());
    option_map.insert("isFirstTime", m_setting->value("isFirstTime").toString());
    return option_map;
}

void Option::on_okButton_clicked()
{
    QMap<QString, QString> option_map = getOptions();
    m_setting->setValue("console_output/position", option_map["console_output/position"]);
    m_setting->setValue("server/address", option_map["server/address"]);
    m_setting->setValue("file/defaultPath", option_map["file/defaultPath"]);
    m_setting->sync();
    hide();
    emit option_changed();
}

void Option::on_cancelButton_clicked()
{
    setOptions();
    hide();
}
