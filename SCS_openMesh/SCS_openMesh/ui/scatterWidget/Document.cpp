#include "Document.h"

Document::Document(QObject *parent)
	: QObject(parent)
{

}

Document::~Document()
{
}
void Document::setSendText(const QJsonArray &data)
{
	emit sendText(data);
}