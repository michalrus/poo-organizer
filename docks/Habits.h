#ifndef HABITS_H
#   define HABITS_H

#	include "Dock.h"

class QTextEdit;

class Habits : public Dock
{
	Q_OBJECT

public:
	Habits (QWidget *parent = 0);

protected:
	virtual void changeText ();
};

#endif
