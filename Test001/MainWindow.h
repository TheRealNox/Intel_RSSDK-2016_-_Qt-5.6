#ifndef MAINWINDOW_H
#define MAINWINDOW_H

//Qt includes
#include <QtWidgets/QMainWindow>

//Intel includes
#include "pxcsensemanager.h"
#include "pxcaudiosource.h"

//Local includes
#include "ui_MainWindow.h"

class						MainWindow : public QMainWindow
{
	Q_OBJECT

private:
	Ui::MainWindowClass		_ui; 
	PXCSenseManager *		_sm;
	PXCAudioSource *		_source;

public:
	MainWindow(QWidget *parent = 0);
	~MainWindow();

private:
	void					connectUIActions();
	void					initIntelSDK();

public slots:
	void					audioSessionItemchanged(int newIndex);

};

#endif // MAINWINDOW_H
