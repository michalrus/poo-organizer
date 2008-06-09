#ifndef CONTACTS_H
#   define CONTACTS_H

#   include <QWidget>
#	include "CategoryView.h"

class QTabWidget;
class ContactsCategoryView;
class QTableView;
class QSqlRelationalTableModel;
class QSqlRecord;
class QSqlQueryModel;

class Contacts : public QWidget
{
	Q_OBJECT

public:
	Contacts (QWidget *parent = 0);

private:
	void fillTab ();

	QTabWidget *tab;
	ContactsCategoryView *categoryView;

private slots:
	void selectPerson (int id);
	void tabChanged (int index);
};

class ContactsTabPage : public QWidget
{
	Q_OBJECT

public:
	ContactsTabPage (int id, QWidget *parent = 0);

public slots:
	void select ();

signals:
	void selectedPerson (int id);

private:
	int id;
	QTableView *view;
    QSqlQueryModel *model;

private slots:
	void goTo (const QModelIndex &index);
};

class ContactsCategoryView : public CategoryView
{
	Q_OBJECT

public:
	ContactsCategoryView (QWidget *parent = 0);

protected:
    virtual void disableDetails ();
	virtual void loadDetails (const QSqlQuery *q);
	virtual void prepareUpdateQuery (QSqlQuery *q, const QString &id, const QString &name) const;
    virtual void prepareDeleteQuery (QSqlQuery *q, const QString &id) const;

private:
	QTableView *view;
    QSqlRelationalTableModel *model;
	QPushButton *newFieldButton;
	QPushButton *deleteFieldButton;

private slots:
	void initializeNewField (int row, QSqlRecord &record);
	void newFieldClick ();
	void deleteFieldClick ();
	void viewSelectionChanged (const QModelIndex &index);
};

#endif
