#include "widget.h"
#include "./ui_widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    mqttClient = new QMqttClient(this);
    mqttClient->setHostname("broker.emqx.io");
    mqttClient->setPort(1883);

    ui->connectButton->setText("Connect");
    ui->switchButton->setText("Switch");

    connect(mqttClient, &QMqttClient::connected, this, &Widget::on_mqttConnected);
    connect(mqttClient, &QMqttClient::disconnected, this, &Widget::on_mqttDisconnected);
    connect(mqttClient, &QMqttClient::messageReceived, this, &Widget::on_mqttMessageReceived);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_connectButton_clicked()
{
    if (mqttClient->state() == QMqttClient::Disconnected) {
        mqttClient->connectToHost();
    } else {
        mqttClient->disconnectFromHost();
    }
}

void Widget::on_switchButton_clicked()
{
    switchState = !switchState;
    QString message = switchState ? "ON" : "OFF";
    if (mqttClient->state() == QMqttClient::Connected) {
        QMqttTopicName topic("switch/state");
        mqttClient->publish(topic, message.toUtf8());
        ui->statusLabel->setText("Switch state: " + message);
    }
    updateLightColor();
}

void Widget::on_mqttConnected()
{
    ui->connectButton->setText("Disconnect");
    QMqttTopicFilter filter("switch/state");
    mqttClient->subscribe(filter);
}

void Widget::on_mqttDisconnected()
{
    ui->connectButton->setText("Connect");
}

void Widget::on_mqttMessageReceived(const QByteArray &message, const QMqttTopicName &topic)
{
    if (topic.name() == "switch/state") {
        QString state = QString::fromUtf8(message);
        ui->statusLabel->setText("Switch state: " + state);
        switchState = (state == "ON");
        updateLightColor();
    }
}

void Widget::updateLightColor()
{
    if (switchState) {
        ui->lightLabel->setStyleSheet("background-color: green; border-radius: 25px;");  // 开关打开，灯为绿色
    } else {
        ui->lightLabel->setStyleSheet("background-color: red; border-radius: 25px;");  // 开关关闭，灯为红色
    }
}