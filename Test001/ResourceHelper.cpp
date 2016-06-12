/*
 *  ResourceHelper.cpp
 *  
 *  Created by Nicolas Aubry on 12-06-2016
 *  Copyright 2016 Nicolas Aubry. All rights reserved.
 *  
 */

#include "ResourceHelper.h"

ResourceHelper*			ResourceHelper::_instance = nullptr;

ResourceHelper::ResourceHelper()
{
	this->populateUILookUp();
}

ResourceHelper::~ResourceHelper()
{

}

ResourceHelper &			ResourceHelper::getInstance()
{
	if (_instance == nullptr)
	{
		_instance = new ResourceHelper();
		std::atexit(&ResourceHelper::destroyInstance);
	}

	return *_instance;
}

void					ResourceHelper::destroyInstance()
{
	if (_instance != nullptr)
		delete _instance;
	_instance = nullptr;
}

const QString			ResourceHelper::getUserReadableLanguageType(PXCSpeechRecognition::LanguageType &type) const
{
	return this->_userReadableLanguageType.value(type);
}

void					ResourceHelper::populateUILookUp()
{
	this->_userReadableLanguageType.insert(PXCSpeechRecognition::LANGUAGE_US_ENGLISH, "English (US)");
	this->_userReadableLanguageType.insert(PXCSpeechRecognition::LANGUAGE_GB_ENGLISH, "English (UK)");
	this->_userReadableLanguageType.insert(PXCSpeechRecognition::LANGUAGE_DE_GERMAN, "German (DE)");
	this->_userReadableLanguageType.insert(PXCSpeechRecognition::LANGUAGE_US_SPANISH, "Spanish (US)");
	this->_userReadableLanguageType.insert(PXCSpeechRecognition::LANGUAGE_LA_SPANISH, "Spanish (LA)");
	this->_userReadableLanguageType.insert(PXCSpeechRecognition::LANGUAGE_FR_FRENCH, "French (FR)");
	this->_userReadableLanguageType.insert(PXCSpeechRecognition::LANGUAGE_IT_ITALIAN, "Italian (IT)");
	this->_userReadableLanguageType.insert(PXCSpeechRecognition::LANGUAGE_JP_JAPANESE, "Japanese (JP)");
	this->_userReadableLanguageType.insert(PXCSpeechRecognition::LANGUAGE_CN_CHINESE, "Chinese (CN)");
	this->_userReadableLanguageType.insert(PXCSpeechRecognition::LANGUAGE_BR_PORTUGUESE, "Portuguese (BR)");
	this->_userReadableLanguageType.insert(PXCSpeechRecognition::LANGUAGE_RU_RUSSIAN, "Russian (RU)");
}

