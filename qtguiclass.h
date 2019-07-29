#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_qtguiclass.h"

class QtGuiClass : public QMainWindow
{
	Q_OBJECT

public:
	QtGuiClass(QWidget *parent = Q_NULLPTR);
	~QtGuiClass();

public slots:
	void do_upload(const QString filename);
	void uploadDone();
	void uploadProgress(qint64, qint64);
	void error();
	void handle_button();

private:
	Ui::QtGuiClass ui;
};
