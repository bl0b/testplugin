#include "qtguiclass.h"
#include "acutads.h"
#include "dbapserv.h"
#include "dbmain.h"
#include <wchar.h>
#include <QNetworkAccessManager>
#include <QFile>
#include <QNetworkReply>
#include <QHttpMultiPart>
#include <iostream>
#include <sstream>

QtGuiClass::QtGuiClass(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

	connect(ui.button, SIGNAL(clicked()), this, SLOT(handle_button()));
}

QtGuiClass::~QtGuiClass()
{
}


void QtGuiClass::do_upload(const QString filename)
{
	QHttpMultiPart* multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType);
	QHttpPart payload;
	//imagePart.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("text/plain"));

	payload.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant(QString("form-data; name=\"payload\"; filename=\"%1\"").arg(filename)));/* version.tkt is the name on my Disk of the file that I want to upload */

	QHttpPart meta;
	meta.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"meta\""));
	meta.setBody("");

	QFile* file = new QFile(filename);
	file->open(QIODevice::ReadOnly);
	payload.setBodyDevice(file);
	file->setParent(multiPart); // we cannot delete the file now, so delete it with the multiPart

	multiPart->append(meta);
	multiPart->append(payload);

	QUrl url("http://192.168.42.81:3000/up");
	QNetworkRequest request(url);

	QNetworkAccessManager* networkManager = new QNetworkAccessManager;
	auto reply = networkManager->post(request, multiPart);
	multiPart->setParent(reply); // delete the multiPart with the reply

	connect(reply, SIGNAL(finished()),
		this, SLOT(uploadDone()));

	connect(reply, SIGNAL(uploadProgress(qint64, qint64)),
		this, SLOT(uploadProgress(qint64, qint64)));

	connect(reply, SIGNAL(error()), this, SLOT(error()));
}

void QtGuiClass::uploadDone()
{
	ui.statusbar->showMessage("File uploaded.");
}

void QtGuiClass::error()
{
	ui.statusbar->showMessage("Error uploading the file.");
}

void QtGuiClass::uploadProgress(qint64 cur, qint64 tot)
{
	std::stringstream s;
	s << cur << '/' << tot;
	ui.statusbar->showMessage(s.str().c_str());
}

void QtGuiClass::handle_button()
{
	const wchar_t* wstr = acdbHostApplicationServices()->workingDatabase()->originalFileName();
	size_t l = wcslen(wstr);
	char* str = new char[l * 3];
	size_t ofs = wcstombs(str, wstr, l);
	str[ofs] = 0;
	ui.statusbar->showMessage(str);
	do_upload(str);
	delete str;
}