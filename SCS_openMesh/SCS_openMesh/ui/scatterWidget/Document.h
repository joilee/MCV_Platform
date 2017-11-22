#pragma once

#include <QObject>
#include <QString>
#include <QJsonArray>
#include <QJsonObject>
class Document : public QObject
{
	Q_OBJECT
	Q_PROPERTY(QString text MEMBER s_text NOTIFY sendText)
public:
	Document(QObject *parent = nullptr);
	~Document();
	void setSendText(const QJsonArray &data);

protected:
	QString s_text;
signals:
	void sendText(const QJsonArray &text);
};
