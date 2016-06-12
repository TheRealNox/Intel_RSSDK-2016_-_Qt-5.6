/*
 *  RecognitionHandler.cpp
 *  
 *  Created by Nicolas Aubry on 12-06-2016
 *  Copyright 2016 Nicolas Aubry. All rights reserved.
 *  
 */

#include "RecognitionHandler.h"

RecognitionHandler::RecognitionHandler()
{
}

RecognitionHandler::~RecognitionHandler()
{
}


void PXCAPI			RecognitionHandler::OnRecognition(const PXCSpeechRecognition::RecognitionData *data)
{
	emit newSpeechRecognized(QString::fromWCharArray(data->scores[0].sentence).append('\n'));
}