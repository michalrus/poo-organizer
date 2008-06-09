#include "SubWindow.h"

SubWindow::SubWindow (const QString &t, QMdiArea *p, QWidget *parent) : QObject(parent)
{
	widgetParent = parent;
	mdiArea = p;
	title = t;

	windowAct = new QAction (title, this);
	windowAct->setCheckable(1);
	connect(windowAct, SIGNAL(toggled(bool)), this, SLOT(showSubWindow(bool)));
}

QAction *SubWindow::toggleViewAction () const
{
	return windowAct;
}

void SubWindow::setWidget (QWidget *w)
{
	window->setMinimumSize(400, 400);
	window->setWidget(w);
}

void SubWindow::showSubWindow (bool b)
{
	if (b) {
		window = new QMdiSubWindow(widgetParent);
		mdiArea->addSubWindow(window);
		window->show();
		window->setAttribute(Qt::WA_DeleteOnClose);
		window->setWindowTitle(title);
		windowIsDestroyed = 0;
		emit windowCreated(this);
		connect(window, SIGNAL(destroyed(QObject *)), this, SLOT(setWindowActFalse()));
	}
	else if (!windowIsDestroyed)
		window->close();
}

void SubWindow::setWindowActFalse ()
{
	windowIsDestroyed = 1;
	windowAct->setChecked(0);
}
