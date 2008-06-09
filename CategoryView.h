#ifndef CATEGORYVIEW_H
#   define CATEGORYVIEW_H

#	include <QWidget>
#   include <QTreeWidget>

class QGridLayout;
class QTreeWidgetItem;
class QLineEdit;
class QPushButton;
class QLayout;
class QSqlQuery;
class QDropEvent;
class QDragEnterEvent;
class QDragMoveEvent;
class QPoint;

class CategoryViewTreeWidget;

class CategoryView : public QWidget
{
	Q_OBJECT

public:
	CategoryView (const QString &tablePrefix, QWidget *parent = 0);

public slots:
   	void selectID (int id);

protected:
	void setDetailsLayout (QLayout *layout);
	int getSelectedID ();

	virtual void disableDetails () = 0;
	virtual void loadDetails (const QSqlQuery *q) = 0;

	// bad encapsulation... instead, QList<QPair> (or stuff...) should be used
	// to return a list of fields and their values
	virtual void prepareUpdateQuery (QSqlQuery *q, const QString &id, const QString &name) const = 0;
	virtual void prepareDeleteQuery (QSqlQuery *q, const QString &id) const;

protected slots:
	void detailsEdited ();

private:
	void fillTree (int pid, QTreeWidgetItem *parent);
	void disableMyDetails ();

	QGridLayout *lay;
	QPushButton *saveButton;
	QPushButton *newCategoryButton;
	QPushButton *newEntryButton;
	QPushButton *deleteButton;

	QLineEdit *name;
	CategoryViewTreeWidget *tree;

	QString tablePrefix;
	bool savingCancelled;

private slots:
	void treeItemChange (QTreeWidgetItem *i, QTreeWidgetItem *prev);

	void saveClick ();
	void deleteClick ();
	void newCategoryClick ();
	void newEntryClick ();
};

class CategoryViewTreeWidget : public QTreeWidget
{
	Q_OBJECT

public:
	CategoryViewTreeWidget (const QString &tablePrefix, QWidget *parent = 0);

protected:
	virtual void dropEvent (QDropEvent *event);
	virtual void dragEnterEvent (QDragEnterEvent *event);
	virtual void dragMoveEvent (QDragMoveEvent *event);

private:
	bool isDroppable (const QPoint &where);
	QString tablePrefix;
};

#endif
