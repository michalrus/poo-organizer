#ifndef MAINWINDOW_H
#   define MAINWINDOW_H

#   include <QMainWindow>

class QMdiArea;
class QMdiSubWindow;
class SubWindow;

class QMenu;
class QToolBar;
class QAction;

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget *parent = 0);

private slots:
	void setContactsWidget (SubWindow *w);
	void setNotesWidget (SubWindow *w);
	void setPasswordsWidget (SubWindow *w);

private:
	void createActions();
	void createMenus();
	void createToolBars();
	void createStatusBar();
	void createDockWindows();
	void createSubWindows();

	QMdiArea *mdiArea;

	QMenu *windowsMenu;
	QToolBar *windowsToolBar;

};

#endif
