#include "windows/Contacts.h"

#include <QVBoxLayout>
#include <QGridLayout>
#include <QTabWidget>
#include <QSqlQuery>
#include <QPushButton>
#include <QTableView>
#include <QSqlRelationalTableModel>
#include <QSqlRelation>
#include <QSqlRelationalDelegate>
#include <QSqlRecord>

#include <QMessageBox>

Contacts::Contacts (QWidget *parent) : QWidget(parent)
{
	tab = new QTabWidget(this);

	categoryView = new ContactsCategoryView(this);
	tab->addTab(categoryView, tr("Whole list"));

	fillTab();
	connect (tab, SIGNAL(currentChanged(int)), this, SLOT(tabChanged(int)));

	QVBoxLayout *lay = new QVBoxLayout(this);
	lay->addWidget(tab);
	setLayout(lay);
}

void Contacts::fillTab ()
{
	QSqlQuery q;
	ContactsTabPage *page;

	q.prepare("SELECT id, name FROM contacts_info_type ORDER BY name");
	q.exec();

	while (q.next()) {
		page = new ContactsTabPage(q.value(0).toInt(), this);
		connect(page, SIGNAL(selectedPerson(int)), this, SLOT(selectPerson(int)));
		tab->addTab(page, q.value(1).toString());
	}
}

void Contacts::selectPerson (int id)
{
	tab->setCurrentIndex(0);
	categoryView->selectID(id);
}

void Contacts::tabChanged (int index)
{
	if (!index)
		return;

	((ContactsTabPage *)(tab->widget(index)))->select();
}

ContactsTabPage::ContactsTabPage (int id, QWidget *parent) : QWidget(parent)
{
	this->id = id;

	model = new QSqlQueryModel;
	view = new QTableView;

	connect(view, SIGNAL(doubleClicked(const QModelIndex &)), this, SLOT(goTo(const QModelIndex &)));

	QVBoxLayout *lay = new QVBoxLayout(this);
	lay->addWidget(view);
	setLayout(lay);
}

void ContactsTabPage::select ()
{
	QSqlQuery q;
	q.prepare("SELECT contacts.id, contacts_info.what, contacts.name FROM contacts, contacts_info WHERE contacts.id = contacts_info.contacts_id AND contacts_info.contacts_info_type_id = ? ORDER BY contacts_info.what, contacts.name");
	q.addBindValue(id);
	q.exec();

	model->setQuery(q);
	model->setHeaderData(1, Qt::Horizontal, tr("Value"));
	model->setHeaderData(2, Qt::Horizontal, tr("Name"));

	view->setModel(model);
	view->setColumnHidden(0, 1);
}

void ContactsTabPage::goTo (const QModelIndex &index)
{
	if (!index.isValid())
		return;

	emit selectedPerson(model->record(index.row()).value(0).toInt());
}

ContactsCategoryView::ContactsCategoryView (QWidget *parent) : CategoryView("contacts", parent)
{
	model = new QSqlRelationalTableModel;

	model->setEditStrategy(QSqlTableModel::OnManualSubmit);
	model->setTable("contacts_info");
	model->setRelation(2, QSqlRelation("contacts_info_type", "id", "name"));
	model->relationModel(2)->sort(1, Qt::AscendingOrder);
	model->setSort(2, Qt::AscendingOrder);

	view = new QTableView;
	view->setModel(model);
	view->setItemDelegate(new QSqlRelationalDelegate(view));
	view->setSelectionMode(QAbstractItemView::SingleSelection);
	view->setCornerButtonEnabled(0);

	model->setHeaderData(2, Qt::Horizontal, tr("Field"));
	model->setHeaderData(3, Qt::Horizontal, tr("Value"));

	connect(model, SIGNAL(dataChanged(const QModelIndex &, const QModelIndex &)), this, SLOT(detailsEdited()));
	connect(model, SIGNAL(primeInsert(int, QSqlRecord &)), this, SLOT(initializeNewField(int, QSqlRecord &)));

	connect(view, SIGNAL(activated(const QModelIndex &)), this, SLOT(viewSelectionChanged(const QModelIndex &)));
	connect(view, SIGNAL(clicked(const QModelIndex &)), this, SLOT(viewSelectionChanged(const QModelIndex &)));
	connect(view, SIGNAL(doubleClicked(const QModelIndex &)), this, SLOT(viewSelectionChanged(const QModelIndex &)));
	connect(view, SIGNAL(pressed(const QModelIndex &)), this, SLOT(viewSelectionChanged(const QModelIndex &)));

	newFieldButton = new QPushButton(tr("New field"));
	connect(newFieldButton, SIGNAL(clicked()), this, SLOT(newFieldClick()));
	deleteFieldButton = new QPushButton(tr("Delete field"));
	connect(deleteFieldButton, SIGNAL(clicked()), this, SLOT(deleteFieldClick()));

	QGridLayout *lay = new QGridLayout();
	lay->addWidget(view, 0, 0, 1, 4);
	lay->addWidget(newFieldButton, 1, 0);
	lay->addWidget(deleteFieldButton, 1, 1);
	setDetailsLayout(lay);
}

void ContactsCategoryView::disableDetails ()
{
	view->setColumnHidden(0, 1);
	view->setColumnHidden(1, 1);
	view->setColumnHidden(2, 1);
	view->setColumnHidden(3, 1);
	model->setFilter("0");
	model->select();
	view->setDisabled(1);
	newFieldButton->setDisabled(1);
	deleteFieldButton->setDisabled(1);
}

void ContactsCategoryView::loadDetails (const QSqlQuery *q)
{
	newFieldButton->setDisabled(0);
	deleteFieldButton->setDisabled(0);
	view->setDisabled(0);
	model->setFilter(QString("contacts_id = '%1'").arg(q->value(0).toInt()));
	model->select();
	view->setColumnHidden(2, 0);
	view->setColumnHidden(3, 0);
	deleteFieldButton->setEnabled(0);
}

void ContactsCategoryView::prepareUpdateQuery (QSqlQuery *q, const QString &id, const QString &name) const
{
	model->submitAll();
	q->prepare("UPDATE contacts SET name = ? WHERE id = ?");
	q->addBindValue(name);
	q->addBindValue(id);
}

void ContactsCategoryView::prepareDeleteQuery (QSqlQuery *q, const QString &id) const
{
	QSqlQuery mq;

	mq.prepare("DELETE FROM contacts_info WHERE contacts_id = ?");
	mq.addBindValue(id);
	mq.exec();

	CategoryView::prepareDeleteQuery(q, id);
}

void ContactsCategoryView::initializeNewField (int row, QSqlRecord &record)
{
//	for (int i = 0; i < 10; i++)
//		QMessageBox::information(this, "blabla", record.fieldName(i) + " = \"" + record.value(i).toString() + "\"");

	record.setValue("contacts_id", getSelectedID());
//	model->setData(model->index(row, 1), getSelectedID());
//	model->setData(model->index(row, 2), "1");

	// ?????????????????????????
}

void ContactsCategoryView::newFieldClick ()
{
	model->insertRows(model->rowCount(), 1);
	detailsEdited();
}

void ContactsCategoryView::deleteFieldClick ()
{
	model->removeRows(view->currentIndex().row(), 1);
	detailsEdited();
}

void ContactsCategoryView::viewSelectionChanged (const QModelIndex &index)
{
	if (index.isValid())
		deleteFieldButton->setEnabled(1);
	else
		deleteFieldButton->setEnabled(1);
}
