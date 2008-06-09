#ifndef DOCK_H
#   define DOCK_H

#	include <QWidget>

class QTextEdit;
class QHBoxLayout;
class QTimer;

class Dock : public QWidget
{
	Q_OBJECT

public:
	Dock (QWidget *parent = 0);

protected:
	virtual void changeText () = 0;
	void setText (const QString &text);
    static QString htmlize (QString s);

private slots:
	void timerTimeout ();

private:
	QTextEdit *edit;
	QTimer *timer;
};

#endif
