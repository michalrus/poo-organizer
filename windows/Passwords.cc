#include "windows/Passwords.h"

#include <QGridLayout>
#include <QSqlQuery>
#include <QLabel>
#include <QLineEdit>

#include "WrapTextEdit.h"

Passwords::Passwords (QWidget *parent) : CategoryView("passwords", parent)
{
	description = new WrapTextEdit(this);
	connect(description, SIGNAL(textChanged()), this, SLOT(detailsEdited()));

	user = new QLineEdit(this);
	password = new QLineEdit(this);
	email = new QLineEdit(this);

	connect(user, SIGNAL(textEdited(const QString &)), this, SLOT(detailsEdited()));
	connect(password, SIGNAL(textEdited(const QString &)), this, SLOT(detailsEdited()));
	connect(email, SIGNAL(textEdited(const QString &)), this, SLOT(detailsEdited()));

	QGridLayout *lay = new QGridLayout();
	lay->addWidget(new QLabel(tr("User") + ":"), 0, 0);
	lay->addWidget(user, 0, 1);
	lay->addWidget(new QLabel(tr("Password") + ":"), 1, 0);
	lay->addWidget(password, 1, 1);
	lay->addWidget(new QLabel(tr("E-mail") + ":"), 2, 0);
	lay->addWidget(email, 2, 1);
	lay->addWidget(description, 3, 0, 1, 2);
	setDetailsLayout(lay);
}

void Passwords::disableDetails ()
{
	user->setEnabled(0);
	user->setText("");
	password->setEnabled(0);
	password->setText(0);
	email->setEnabled(0);
	email->setText(0);
	description->clear();
	description->setEnabled(0);
}

void Passwords::loadDetails (const QSqlQuery *q)
{
	user->setText(q->value(3).toString());
	user->setEnabled(1);
	password->setText(q->value(4).toString());
	password->setEnabled(1);
	email->setText(q->value(5).toString());
	email->setEnabled(1);
	description->setText(q->value(6).toString());
	description->setEnabled(1);
}

void Passwords::prepareUpdateQuery(QSqlQuery *q, const QString &id, const QString &name) const
{
	q->prepare("UPDATE passwords SET name = ?, user = ?, password = ?, email = ?, description = ? WHERE id = ?");
	q->addBindValue(name);
	q->addBindValue(user->text());
	q->addBindValue(password->text());
	q->addBindValue(email->text());
	q->addBindValue(description->text());
	q->addBindValue(id);
}
