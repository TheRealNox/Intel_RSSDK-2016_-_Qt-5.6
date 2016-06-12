/*
 *  MainWindow.cpp
 *  
 *  Created by Nicolas Aubry on 12-06-2016
 *  Copyright 2016 Nicolas Aubry. All rights reserved.
 *  
 */

#include "MainWindow.h"

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent),
	_sm(nullptr),
	_source(nullptr),
	_sr(nullptr),
	_recoHandlerThread(nullptr),
	_recoHandler(nullptr)
{
	_ui.setupUi(this);

	this->connectUIActions();
	this->initIntelSDK();
}

MainWindow::~MainWindow()
{
	if (this->_sm != nullptr)
		this->_sm->Release();
	if (this->_sr != nullptr)
	{
		this->_sr->StopRec();
		this->_sr->Release();
	}
	if (this->_recoHandler != nullptr)
	{
		if (this->_recoHandlerThread != nullptr)
		{
			if (this->_recoHandlerThread->isRunning() == true)
			{
				this->_recoHandlerThread->quit();
				this->_recoHandlerThread->wait();
			}

			delete this->_recoHandlerThread;
		}

		delete this->_recoHandler;
	}
}

void				MainWindow::connectUIActions()
{
	connect(this->_ui.audioSourcesComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(audioSessionIndexChanged(int)));
	connect(this->_ui.speechLanguagesComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(speechLanguageIndexChanged(int)));
}

void				MainWindow::initIntelSDK()
{
	this->initIntelSessionManager();
	this->initIntelSpeechRecognition();
}

void				MainWindow::initIntelSessionManager()
{
	this->_sm = PXCSenseManager::CreateInstance();

	PXCSession::ImplVersion v = this->_sm->QuerySession()->QueryVersion();
	statusBar()->showMessage(QString("Intel RealSense SDK Version: %1.%2\n").arg(v.major).arg(v.minor));


	// session is a PXCSession instance.
	this->_source = this->_sm->QuerySession()->CreateAudioSource();

	// Scan and Enumerate audio devices
	this->_source->ScanDevices();
	PXCAudioSource::DeviceInfo dinfo;
	for (int d = this->_source->QueryDeviceNum() - 1; d >= 0; d--)
	{
		this->_source->QueryDeviceInfo(d, &dinfo);

		this->_ui.audioSourcesComboBox->addItem(QString::fromWCharArray(dinfo.name), QVariant::fromValue(QString::fromWCharArray(dinfo.did)));
	}
}

void				MainWindow::initIntelSpeechRecognition()
{
	this->_sm->QuerySession()->CreateImpl<PXCSpeechRecognition>(&this->_sr);

	if (this->_sr != nullptr)
	{
		PXCSpeechRecognition::ProfileInfo pinfo;

		this->_ui.speechLanguagesComboBox->addItem("Please choose one", -1);

		for (int k = 0;; k++)
		{
			PXCSpeechRecognition::ProfileInfo pinfo;
			if (this->_sr->QueryProfile(k, &pinfo)<PXC_STATUS_NO_ERROR) break;

			this->_ui.speechLanguagesComboBox->addItem(ResourceHelper::getInstance().getUserReadableLanguageType(pinfo.language), QVariant::fromValue<int>(pinfo.language));

			this->_ui.speechLanguagesComboBox->setSizeAdjustPolicy(QComboBox::SizeAdjustPolicy::AdjustToContents);
		}
	}
}

void				MainWindow::audioSessionIndexChanged(int newIndex)
{
	QVariant		rawItemData = this->_ui.audioSourcesComboBox->itemData(newIndex);

	PXCAudioSource::DeviceInfo dinfo;

	this->_source->Release();
	this->_source = nullptr;
	// session is a PXCSession instance.
	this->_source = this->_sm->QuerySession()->CreateAudioSource();

	// Scan and Enumerate audio devices
	this->_source->ScanDevices();

	if (rawItemData.isValid())
	{
		for (int d = this->_source->QueryDeviceNum() - 1; d >= 0; d--)
		{
			this->_source->QueryDeviceInfo(d, &dinfo);
			if (QString::fromWCharArray(dinfo.did) == rawItemData.toString())
			{
				// Set the active device
				this->_source->SetDevice(&dinfo);
				statusBar()->showMessage(QString("New device set %1").arg(QString::fromWCharArray(dinfo.name)));
				break;
			}
		}
	}
}

void				MainWindow::speechLanguageIndexChanged(int newIndex)
{
	PXCSpeechRecognition::ProfileInfo pinfo;
	QVariant		rawItemData = this->_ui.speechLanguagesComboBox->itemData(newIndex);

	if (rawItemData.toInt() == -1)
	{
		this->_ui.speechLanguagesComboBox->blockSignals(true);
		this->_ui.speechLanguagesComboBox->setCurrentIndex(0);
		this->_ui.speechLanguagesComboBox->blockSignals(false);
		this->_sr->StopRec();
		if (this->_recoHandler != nullptr)
		{
			if (this->_recoHandlerThread != nullptr)
			{
				if (this->_recoHandlerThread->isRunning() == true)
				{
					this->_recoHandlerThread->quit();
					this->_recoHandlerThread->wait();
				}
				delete this->_recoHandlerThread;
				this->_recoHandlerThread = nullptr;
			}
			delete this->_recoHandler;
			this->_recoHandler = nullptr;
		}
	}
	else
	{
		this->_ui.audioSourcesComboBox->setCurrentIndex(this->_ui.audioSourcesComboBox->currentIndex());
		this->_sr->StopRec();

		if (this->_recoHandler != nullptr)
		{
			if (this->_recoHandlerThread != nullptr)
			{
				if (this->_recoHandlerThread->isRunning() == true)
				{
					this->_recoHandlerThread->quit();
					this->_recoHandlerThread->wait();
				}

				delete this->_recoHandlerThread;
				this->_recoHandlerThread = nullptr;
			}

			delete this->_recoHandler;
			this->_recoHandler = nullptr;
		}

		this->_recoHandler = new RecognitionHandler;
		this->_recoHandlerThread = new QThread;

		this->_recoHandler->moveToThread(this->_recoHandlerThread);
		this->_recoHandlerThread->start();

		connect(this->_recoHandler, SIGNAL(newSpeechRecognized(const QString &)), this, SLOT(messageToShow(const QString &)), Qt::QueuedConnection);

		QCoreApplication::processEvents();
		this->_sr->QueryProfile(0, &pinfo);
		pinfo.language = (PXCSpeechRecognition::LanguageType)rawItemData.toInt();
		this->_sr->SetProfile(&pinfo);

		this->_sr->SetDictation();


		if (this->_sr->StartRec(this->_source, this->_recoHandler) != PXC_STATUS_NO_ERROR)
			statusBar()->showMessage("Speech Recognition NOT initialized...");
		else
			statusBar()->showMessage(QString("Speech Recognition initialized using language %1").arg(ResourceHelper::getInstance().getUserReadableLanguageType(pinfo.language)));
	}

}

void				MainWindow::messageToShow(const QString &toShow)
{
	if (toShow == "Clear\n")
		this->_ui.consoleDebugView->clear();

	this->_ui.consoleDebugView->insertPlainText(toShow);
	QScrollBar *bar = this->_ui.consoleDebugView->verticalScrollBar();
	bar->setValue(bar->maximum());
}