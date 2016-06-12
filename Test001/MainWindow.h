/*
 *  MainWindow.h
 *  
 *  Created by Nicolas Aubry on 12-06-2016
 *  Copyright 2016 Nicolas Aubry. All rights reserved.
 *  
 */

#ifndef __MAINWINDOW_H__
#define __MAINWINDOW_H__

//Qt includes
#include <QScrollBar>
#include <QThread>
#include <QtWidgets/QMainWindow>

//Intel includes
#include "pxcaudiosource.h"
#include "pxcsensemanager.h"
#include "pxcspeechrecognition.h"

//Local includes
#include "RecognitionHandler.h"
#include "ResourceHelper.h"
#include "ui_MainWindow.h"

class						MainWindow : public QMainWindow
{
	Q_OBJECT

	// -- Attributs
private:
	Ui::MainWindowClass		_ui;
	PXCSenseManager *		_sm;
	PXCAudioSource *		_source;
	PXCSpeechRecognition *	_sr;
	QThread	*				_recoHandlerThread;
	RecognitionHandler *	_recoHandler;
	// --!Attributs

	// -- CTors & DTor
public:
	MainWindow(QWidget * parent = 0);
	~MainWindow();

private:
	// --!CTors & DTor

	// -- Methods
private:
	void					connectUIActions();
	void					initIntelSDK();
	void					initIntelSessionManager();
	void					initIntelSpeechRecognition();

	// --!Methods

	// -- Slots & Signals
public slots :
	void					audioSessionIndexChanged(int newIndex);
	void					speechLanguageIndexChanged(int newIndex);
	void					messageToShow(const QString &);
	// --!Slots & Signals
};

#endif//__MAINWINDOW_H__

