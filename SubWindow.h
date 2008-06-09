#ifndef SUBWINDOW_H
#   define SUBWINDOW_H

#	include <QMdiArea>
#	include <QMdiSubWindow>
#   include <QAction>

class SubWindow : public QObject
{
	Q_OBJECT

public:
	SubWindow (const QString &t, QMdiArea *p, QWidget *parent = 0);
    QAction *toggleViewAction () const;
	void setWidget (QWidget *w);

signals:
	void windowCreated (SubWindow *w);

private slots:
	void showSubWindow (bool b);
	void setWindowActFalse ();

private:
	QWidget *widgetParent;
	QMdiArea *mdiArea;
	QString title;
	QMdiSubWindow *window;
	QAction *windowAct;
	bool windowIsDestroyed;
};

#endif
