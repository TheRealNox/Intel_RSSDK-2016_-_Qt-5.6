/*
 *  RecognitionHandler.h
 *  
 *  Created by Nicolas Aubry on 12-06-2016
 *  Copyright 2016 Nicolas Aubry. All rights reserved.
 *  
 */

#ifndef __RECOGNITIONHANDLER_H__
#define __RECOGNITIONHANDLER_H__

//Qt includes
#include <QObject>

//Intel includes
#include "pxcspeechrecognition.h"

class					RecognitionHandler : public QObject, public PXCSpeechRecognition::Handler
{
	Q_OBJECT
		
	// -- Attributs
private:

	// --!Attributs

	// -- CTors & DTor
public:
	RecognitionHandler();
	~RecognitionHandler();

private:
	// --!CTors & DTor

	// -- Methods
public:
	virtual void PXCAPI	OnRecognition(const PXCSpeechRecognition::RecognitionData *data);

private:
	// --!Methods

	// -- Slots & Signals
public slots :

public:
signals :
	void				newSpeechRecognized(const QString &);

	// --!Slots & Signals
};

#endif//__RECOGNITIONHANDLER_H__

