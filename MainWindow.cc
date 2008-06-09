#include <QSqlDatabase>

#include <QWidget>
#include <QMdiArea>
#include <QMenuBar>
#include <QToolBar>
#include <QAction>
#include <QMessageBox>
#include <QDockWidget>

#include "MainWindow.h"
#include "Dock.h"
#include "SubWindow.h"

#include "docks/Biographies.h"
#include "docks/Habits.h"
#include "docks/Opinions.h"

#include "windows/Contacts.h"
#include "windows/Notes.h"
#include "windows/Passwords.h"

MainWindow::MainWindow (QWidget *parent) : QMainWindow(parent)
{
	setWindowTitle(tr("Poo!"));
	mdiArea = new QMdiArea;
	setCentralWidget(mdiArea);

	createActions();
	createMenus();
	createToolBars();
	createStatusBar();

	QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
	db.setDatabaseName("data.db");
	if (!db.open()) {
		QMessageBox::critical(this, tr("Poo"), tr("Cannot open database file (data.db)... do something. :<"));
		exit(-1);
	}

	createSubWindows();
	createDockWindows();
}

void MainWindow::setContactsWidget  (SubWindow *w) { w->setWidget (new Contacts); }
void MainWindow::setNotesWidget     (SubWindow *w) { w->setWidget (new Notes); }
void MainWindow::setPasswordsWidget (SubWindow *w) { w->setWidget (new Passwords); }

void MainWindow::createActions ()
{
}

void MainWindow::createMenus ()
{
	windowsMenu = menuBar()->addMenu(tr("Windows"));
}

void MainWindow::createToolBars ()
{
	windowsToolBar = addToolBar(tr("Windows"));
}

void MainWindow::createStatusBar ()
{
	statusBar();
}

void MainWindow::createDockWindows ()
{
	windowsToolBar->addSeparator();
	windowsMenu->addSeparator();

	QDockWidget *dock;

	dock = new QDockWidget(tr("Opinions"), this);
	dock->setAllowedAreas(Qt::AllDockWidgetAreas);
	dock->setFeatures(QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetClosable);
	dock->setWidget(new Opinions(dock));
    addDockWidget(Qt::RightDockWidgetArea, dock);
	windowsToolBar->addAction(dock->toggleViewAction());
	windowsMenu->addAction(dock->toggleViewAction());

	dock = new QDockWidget(tr("Habits"), this);
	dock->setAllowedAreas(Qt::AllDockWidgetAreas);
	dock->setFeatures(QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetClosable);
	dock->setWidget(new Habits(dock));
    addDockWidget(Qt::RightDockWidgetArea, dock);
	windowsToolBar->addAction(dock->toggleViewAction());
	windowsMenu->addAction(dock->toggleViewAction());

	dock = new QDockWidget(tr("Biographies"), this);
	dock->setAllowedAreas(Qt::AllDockWidgetAreas);
	dock->setFeatures(QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetClosable);
	dock->setWidget(new Biographies(dock));
    addDockWidget(Qt::RightDockWidgetArea, dock);
	windowsToolBar->addAction(dock->toggleViewAction());
	windowsMenu->addAction(dock->toggleViewAction());
}

void MainWindow::createSubWindows ()
{
	SubWindow *win;

	win = new SubWindow(tr("Contacts"), mdiArea, this);
	connect (win, SIGNAL(windowCreated(SubWindow *)), this, SLOT(setContactsWidget(SubWindow *)));
	windowsToolBar->addAction(win->toggleViewAction());
	windowsMenu->addAction(win->toggleViewAction());

	win = new SubWindow(tr("Notes"), mdiArea, this);
	connect (win, SIGNAL(windowCreated(SubWindow *)), this, SLOT(setNotesWidget(SubWindow *)));
	windowsToolBar->addAction(win->toggleViewAction());
	windowsMenu->addAction(win->toggleViewAction());

	win = new SubWindow(tr("Passwords"), mdiArea, this);
	connect (win, SIGNAL(windowCreated(SubWindow *)), this, SLOT(setPasswordsWidget(SubWindow *)));
	windowsToolBar->addAction(win->toggleViewAction());
	windowsMenu->addAction(win->toggleViewAction());
}
