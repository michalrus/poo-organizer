#include "docks/Opinions.h"

#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>
#include <QMessageBox>

Opinions::Opinions (QWidget *parent) : Dock(parent)
{
}

void Opinions::changeText ()
{
	QSqlQuery q;
	int count;

	q.setForwardOnly(1);
	q.exec("SELECT count() FROM contacts, contacts_info WHERE contacts.id = contacts_info.contacts_id AND contacts_info.contacts_info_type_id = 12");
	if (q.lastError().isValid())
		QMessageBox::critical(this, tr("Opinions"), q.lastError().text());

	q.next();
	count = q.value(0).toInt();

	q.prepare("SELECT contacts.name, contacts_info.what FROM contacts, contacts_info WHERE contacts.id = contacts_info.contacts_id AND contacts_info.contacts_info_type_id = 12 LIMIT ?, 1");
	q.addBindValue(rand() % count);
	q.exec();
	if (q.lastError().isValid())
		QMessageBox::critical(this, tr("Habits"), q.lastError().text());

	q.next();
	QString text = "\"" + htmlize(q.value(1).toString()) + "\"<center><i>- " + htmlize(q.value(0).toString()) + "</i></center>";
	setText(text);
}
