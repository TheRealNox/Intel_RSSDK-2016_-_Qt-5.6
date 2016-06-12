/*
 *  ResourceHelper.h
 *  
 *  Created by Nicolas Aubry on 12-06-2016
 *  Copyright 2016 Nicolas Aubry. All rights reserved.
 *  
 */

#ifndef __RESOURCEHELPER_H__
#define __RESOURCEHELPER_H__

//Qt includes
#include <QMap>

//Intel includes
#include "pxcspeechrecognition.h"


class								ResourceHelper
{
	// -- Attributs
private:
	static ResourceHelper *			_instance;
	QMap<PXCSpeechRecognition::LanguageType, QString> _userReadableLanguageType;
	// --!Attributs

	// -- CTors & DTor
public:

private:
	ResourceHelper();
	ResourceHelper(const ResourceHelper &);
	ResourceHelper &				operator=(const ResourceHelper &);
	~ResourceHelper();
	static void						destroyInstance();
	// --!CTors & DTor

	// -- Methods
public:
	static ResourceHelper &			getInstance();
	const QString					getUserReadableLanguageType(PXCSpeechRecognition::LanguageType &) const;

private:
	void							populateUILookUp();
	// --!Methods
};

#endif//__RESOURCEHELPER_H__
