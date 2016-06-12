/*
 *  main.cpp
 *  
 *  Created by Nicolas Aubry on 12-06-2016
 *  Copyright 2016 Nicolas Aubry. All rights reserved.
 *  
 */

#include "MainWindow.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	MainWindow w;
	w.show();
	return a.exec();
}
