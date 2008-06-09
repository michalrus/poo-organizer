#include "docks/Biographies.h"

#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>
#include <QMessageBox>

Biographies::Biographies (QWidget *parent) : Dock(parent)
{
}

void Biographies::changeText ()
{
	QSqlQuery q;
	int count;

	q.setForwardOnly(1);
	q.exec("SELECT count() FROM biographies");
	if (q.lastError().isValid())
		QMessageBox::critical(this, tr("Biographies"), q.lastError().text());

	q.next();
	count = q.value(0).toInt();

	q.prepare("SELECT who, what FROM biographies LIMIT ?, 1");
	q.addBindValue(rand() % count);
	q.exec();
	if (q.lastError().isValid())
		QMessageBox::critical(this, tr("Habits"), q.lastError().text());

	q.next();
	QString text = "<b>" + q.value(0).toString() + "</b> - " + q.value(1).toString().replace("\n", "<br>");
	setText(text);
}
