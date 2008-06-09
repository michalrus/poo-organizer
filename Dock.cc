#include "Dock.h"

#include <QTextEdit>
#include <QHBoxLayout>
#include <QTimer>

Dock::Dock (QWidget *parent) : QWidget(parent)
{
	QHBoxLayout *lay = new QHBoxLayout(this);

	edit = new QTextEdit(this);
	edit->setReadOnly(1);

	lay->addWidget(edit);
	this->setLayout(lay);

	timer = new QTimer;
	timer->setSingleShot(1);
	connect(timer, SIGNAL(timeout()), this, SLOT(timerTimeout()));
	timer->start(0);
}

void Dock::setText (const QString &text)
{
	edit->setHtml(text);
}

QString Dock::htmlize (QString s)
{
	s.replace("<", "&lt;");
	s.replace("\n", "<br>");

	return s;
}

void Dock::timerTimeout ()
{
	changeText();
	timer->start(edit->toPlainText().count(' ') * 700);
}
