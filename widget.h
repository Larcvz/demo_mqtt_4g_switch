#pragma once

#include <QWidget>
#include <QMqttClient>

QT_BEGIN_NAMESPACE
namespace Ui {
class Widget;
}
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private:
    void updateLightColor();

private slots:
    void on_connectButton_clicked();
    void on_switchButton_clicked();
    void on_mqttConnected();
    void on_mqttDisconnected();
    void on_mqttMessageReceived(const QByteArray &message, const QMqttTopicName &topic);

private:
    Ui::Widget *ui;
    QMqttClient *mqttClient;
    bool switchState;
};
