#include "MainWindow.h"

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent),
	_sm(nullptr),
	_source(nullptr)
{
	_ui.setupUi(this);

	this->connectUIActions();
	this->initIntelSDK();
}

MainWindow::~MainWindow()
{
	if (this->_sm != nullptr)
		this->_sm->Release();
}

void				MainWindow::connectUIActions()
{
	connect(this->_ui.audioSourcesComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(audioSessionItemchanged(int)));
}

void				MainWindow::initIntelSDK()
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

void				MainWindow::audioSessionItemchanged(int newIndex)
{
	QVariant		rawItemData = this->_ui.audioSourcesComboBox->itemData(newIndex);

	PXCAudioSource::DeviceInfo dinfo;

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