#include "docks/Habits.h"

#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>
#include <QMessageBox>

Habits::Habits (QWidget *parent) : Dock(parent)
{
}

void Habits::changeText ()
{
	QSqlQuery q;
	int count;

	q.setForwardOnly(1);
	q.exec("SELECT count() FROM habits");
	if (q.lastError().isValid())
		QMessageBox::critical(this, tr("Habits"), q.lastError().text());

	q.next();
	count = q.value(0).toInt();

	q.prepare("SELECT name FROM habits LIMIT ?, 1");
	q.addBindValue(rand() % count);
	q.exec();
	if (q.lastError().isValid())
		QMessageBox::critical(this, tr("Habits"), q.lastError().text());

	q.next();
	QString text = q.value(0).toString().replace("\n", "<br>");
	setText(text);
}
