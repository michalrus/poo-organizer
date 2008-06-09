#ifndef BIOGRAPHIES_H
#   define BIOGRAPHIES_H

#	include "Dock.h"

class QTextEdit;

class Biographies : public Dock
{
	Q_OBJECT

public:
	Biographies (QWidget *parent = 0);

protected:
	virtual void changeText ();
};

#endif
