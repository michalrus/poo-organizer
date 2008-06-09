#include "CategoryView.h"

#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>
#include <QMessageBox>
#include <QGridLayout>
#include <QLineEdit>
#include <QTextEdit>
#include <QCheckBox>
#include <QPushButton>
#include <QDropEvent>
#include <QDragEnterEvent>
#include <QDragMoveEvent>

CategoryView::CategoryView (const QString &tablePrefix, QWidget *parent) : QWidget(parent)
{
	this->tablePrefix = tablePrefix;

	tree = new CategoryViewTreeWidget(tablePrefix, this);
	tree->setColumnCount(1);
	tree->setHeaderLabel("");
	tree->setDragEnabled(1);
	tree->setDropIndicatorShown(1);
	tree->setDragDropMode(QAbstractItemView::InternalMove);
	tree->setSortingEnabled(1);
	tree->sortByColumn(0, Qt::AscendingOrder);

	fillTree(0, tree->invisibleRootItem());
	connect(tree, SIGNAL(currentItemChanged(QTreeWidgetItem *, QTreeWidgetItem *)),
		this, SLOT(treeItemChange(QTreeWidgetItem *, QTreeWidgetItem *)));

	saveButton = new QPushButton(tr("Save changes"), this);
	connect(saveButton, SIGNAL(clicked()), this, SLOT(saveClick()));

	deleteButton = new QPushButton(tr("Delete"), this);
	connect(deleteButton, SIGNAL(clicked()), this, SLOT(deleteClick()));

	newEntryButton = new QPushButton(tr("New entry"), this);
	connect(newEntryButton, SIGNAL(clicked()), this, SLOT(newEntryClick()));

	newCategoryButton = new QPushButton(tr("New category"), this);
	connect(newCategoryButton, SIGNAL(clicked()), this, SLOT(newCategoryClick()));

	name = new QLineEdit(this);
	connect(name, SIGNAL(textEdited(const QString &)), this, SLOT(detailsEdited()));

	lay = new QGridLayout(this);

	lay->addWidget(tree, 0, 0, 1, 4);

	lay->addWidget(saveButton, 1, 0);
	lay->addWidget(deleteButton, 1, 1);
	lay->addWidget(newEntryButton, 1, 2);
	lay->addWidget(newCategoryButton, 1, 3);

	lay->addWidget(name, 2, 0, 1, 4);

	lay->setRowStretch(0, 3);
	lay->setRowStretch(1, 1);
	lay->setRowStretch(2, 1);

	this->setLayout(lay);

	savingCancelled = 0;
	disableMyDetails();
}

void CategoryView::selectID (int id)
{
	QList<QTreeWidgetItem *> r;
	QTreeWidgetItem *i = 0;

	// why searching does not work when columns are hidden?... idiotic.

	tree->setColumnCount(3);
	r = tree->findItems(QString("%1").arg(id), Qt::MatchExactly | Qt::MatchRecursive, 2);
	tree->setColumnCount(1);

	if (!r.count())
		return;

	if (r.count() == 1 && r[0]->text(1) == "1")
		i = r[0];
	else if (r.count() == 2) {
		if (r[0]->text(1) == "1")
			i = r[0];
		else if (r[1]->text(1) == "1")
			i = r[1];
	}
	else
		return;

	tree->setCurrentItem(i);
}

void CategoryView::setDetailsLayout (QLayout *layout)
{
	lay->addLayout(layout, 3, 0, 1, 4);
	lay->setRowStretch(3, 5);
	disableDetails();
}

int CategoryView::getSelectedID ()
{
	if (tree->currentItem())
		return tree->currentItem()->text(2).toInt();
	return 0;
}

void CategoryView::prepareDeleteQuery (QSqlQuery *q, const QString &id) const
{
	q->prepare("DELETE FROM " + tablePrefix + " WHERE id = ?");
	q->addBindValue(id);
}

void CategoryView::detailsEdited ()
{
	saveButton->setEnabled(1);
}

void CategoryView::fillTree (int pid, QTreeWidgetItem *parent)
{
	QTreeWidgetItem *child;
	QSqlQuery q, q2;

	q.setForwardOnly(1);

	q.prepare("SELECT id, pid, name FROM " + tablePrefix + "_category WHERE pid = ?");
	q.addBindValue(pid);
	q.exec();

	child = parent;

	while (q.next()) {
		child = new QTreeWidgetItem(parent,
			QStringList(q.value(2).toString())
			<< QString("0")
			<< q.value(0).toString());

		fillTree(q.value(0).toInt(), child);

		q2.prepare("SELECT id, name FROM " + tablePrefix + " WHERE " + tablePrefix + "_category_id = ?");
		q2.addBindValue(q.value(0).toString());
		q2.exec();

		while (q2.next()) {
			new QTreeWidgetItem(child,
				QStringList(q2.value(1).toString())
				<< QString("1")
				<< q2.value(0).toString());
		}
	}
}

void CategoryView::disableMyDetails ()
{
	name->setText("");
	name->setEnabled(0);

	saveButton->setEnabled(0);
	deleteButton->setEnabled(0);
	newEntryButton->setEnabled(0);
	newCategoryButton->setEnabled(1);
}

void CategoryView::treeItemChange (QTreeWidgetItem *i, QTreeWidgetItem *prev)
{
	if (savingCancelled)
		return;

	if (saveButton->isEnabled()) {
		// int r = QMessageBox::question(this, "CategoryView : " + tablePrefix, tr("Do you want to save your changes?"), QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
		int r = QMessageBox::question(this, "CategoryView : " + tablePrefix, tr("Do you want to save your changes?"), QMessageBox::Save | QMessageBox::Discard);

		// cancelling doesn't work ;< idiotic.
		// multiple selection occurs...
		// QTreeWidget sucks

		if (r == QMessageBox::Save) {
			savingCancelled = 1;
			tree->setCurrentItem(prev);
			saveClick();
			tree->setCurrentItem(i);
			savingCancelled = 0;
		}
		else if (r == QMessageBox::Cancel) {
			savingCancelled = 1;
			tree->setCurrentItem(prev);
			savingCancelled = 0;
			return;
		}
	}

	disableDetails();
	disableMyDetails();

	if (!i)
		return;

	QSqlQuery q;
	q.setForwardOnly(1);

	deleteButton->setEnabled(1);

	if (i->text(1) == "0") {
		name->setText(i->text(0));
		name->setEnabled(1);
		newEntryButton->setEnabled(1);
	}
	else {
		newCategoryButton->setEnabled(0);

		q.prepare("SELECT * FROM " + tablePrefix + " WHERE id = ?");
		q.addBindValue(i->text(2));
		q.exec();
		q.next();

		name->setText(q.value(2).toString());
		name->setEnabled(1);

		loadDetails(&q);

		saveButton->setEnabled(0);
	}
}

void CategoryView::saveClick ()
{
	QSqlQuery q;
	q.setForwardOnly(1);

	if (tree->currentItem()->text(1) == "0") {
		q.prepare("UPDATE " + tablePrefix + "_category SET name = ? WHERE id = ?");
		q.addBindValue(name->text());
		q.addBindValue(tree->currentItem()->text(2));
	}
	else
		prepareUpdateQuery(&q, tree->currentItem()->text(2), name->text());

	q.exec();
	tree->currentItem()->setText(0, name->text());
	saveButton->setEnabled(0);
}

void CategoryView::deleteClick ()
{
    QTreeWidgetItem *i = tree->currentItem();

	int r = QMessageBox::question(this, "CategoryView : " + tablePrefix, tr("Are you sure you want to delete '%1'?").arg(i->text(0)), QMessageBox::Yes | QMessageBox::No);
	if (r == QMessageBox::No)
		return;

	saveButton->setEnabled(0);

	QSqlQuery q;
	q.setForwardOnly(1);

	if (i->text(1) == "0") {
		if (i->childCount()) {
			QMessageBox::warning(this, "CategoryView : " + tablePrefix, tr("Category not empty, cannot delete."));
			return;
		}

		q.prepare("DELETE FROM " + tablePrefix + "_category WHERE id = ?");
		q.addBindValue(i->text(2));
	}
	else
		prepareDeleteQuery(&q, i->text(2));

	q.exec();
	delete i;
}

void CategoryView::newCategoryClick ()
{
	QSqlQuery q;
    QTreeWidgetItem *i = tree->currentItem();
	QString newId;

	q.setForwardOnly(1);

	q.exec("BEGIN TRANSACTION");

	q.prepare("INSERT INTO " + tablePrefix + "_category (pid, name) VALUES (?, ?)");
	if (i)
		q.addBindValue(i->text(2));
	else {
		q.addBindValue(0);
		i = tree->invisibleRootItem();
	}
	q.addBindValue(tr("[new category]"));
	q.exec();

	q.exec("SELECT seq FROM sqlite_sequence WHERE name = '" + tablePrefix + "_category'");
	q.next();
	newId = q.value(0).toString();

	q.exec("COMMIT TRANSACTION");

	i = new QTreeWidgetItem(i,
		QStringList(tr("[new category]"))
		<< QString("0")
		<< newId);
	tree->setCurrentItem(i);
}

void CategoryView::newEntryClick ()
{
	QSqlQuery q;
    QTreeWidgetItem *i = tree->currentItem();
	QString newId;

	q.setForwardOnly(1);

	q.exec("BEGIN TRANSACTION");

	q.prepare("INSERT INTO " + tablePrefix + " (" + tablePrefix + "_category_id, name) VALUES (?, ?)");
	q.addBindValue(i->text(2));
	q.addBindValue(tr("[new entry]"));
	q.exec();

	q.exec("SELECT seq FROM sqlite_sequence WHERE name = '" + tablePrefix + "'");
	q.next();
	newId = q.value(0).toString();

	q.exec("COMMIT TRANSACTION");

	i = new QTreeWidgetItem(i,
		QStringList(tr("[new entry]"))
		<< QString("1")
		<< newId);
	tree->setCurrentItem(i);
}

CategoryViewTreeWidget::CategoryViewTreeWidget (const QString &tablePrefix, QWidget *parent) : QTreeWidget(parent)
{
	this->tablePrefix = tablePrefix;
}

void CategoryViewTreeWidget::dropEvent (QDropEvent *event)
{
	if (!isDroppable(event->pos()))
		return;

	QSqlQuery q;
	q.setForwardOnly(1);

	if (currentItem()->text(1) == "0") {
		q.prepare("UPDATE " + tablePrefix + "_category SET pid = ? WHERE id = ?");
		if (!itemAt(event->pos()))
			q.addBindValue(0);
		else
			q.addBindValue(itemAt(event->pos())->text(2));
	}
	else {
		q.prepare("UPDATE " + tablePrefix + " SET " + tablePrefix + "_category_id = ? WHERE id = ?");
		q.addBindValue(itemAt(event->pos())->text(2));
	}

	q.addBindValue(currentItem()->text(2));

	q.exec();

	QTreeWidget::dropEvent(event);
}

// dunno why that doesn't work... strange things happen
// i'll have to create my own tree widget...

void CategoryViewTreeWidget::dragEnterEvent(QDragEnterEvent *event)
{
//	if (!isDroppable(event->pos()))
//		return;

	QTreeWidget::dragEnterEvent(event);
}

void CategoryViewTreeWidget::dragMoveEvent(QDragMoveEvent *event)
{
//	if (!isDroppable(event->pos()))
//		return;

	QTreeWidget::dragMoveEvent(event);
}

bool CategoryViewTreeWidget::isDroppable (const QPoint &where)
{
	if (!itemAt(where) && currentItem()->text(1) == "1")
		return 0;

	if (itemAt(where) && itemAt(where)->text(1) == "1")
		return 0;

	return 1;
}
