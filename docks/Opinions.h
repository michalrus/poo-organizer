#ifndef OPINIONS_H
#   define OPINIONS_H

#	include "Dock.h"

class QTextEdit;

class Opinions : public Dock
{
	Q_OBJECT

public:
	Opinions (QWidget *parent = 0);

protected:
	virtual void changeText ();
};

#endif
