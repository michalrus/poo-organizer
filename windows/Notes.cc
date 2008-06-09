#include "windows/Notes.h"

#include <QVBoxLayout>
#include <QSqlQuery>

#include "WrapTextEdit.h"

Notes::Notes (QWidget *parent) : CategoryView("notes", parent)
{
	what = new WrapTextEdit(this);
	connect(what, SIGNAL(textChanged()), this, SLOT(detailsEdited()));

	QVBoxLayout *lay = new QVBoxLayout();
	lay->addWidget(what);
	setDetailsLayout(lay);
}

void Notes::disableDetails ()
{
	what->clear();
	what->setEnabled(0);
}

void Notes::loadDetails (const QSqlQuery *q)
{
	what->setText(q->value(3).toString());
	what->setEnabled(1);
}

void Notes::prepareUpdateQuery(QSqlQuery *q, const QString &id, const QString &name) const
{
	q->prepare("UPDATE notes SET name = ?, what = ? WHERE id = ?");
	q->addBindValue(name);
	q->addBindValue(what->text());
	q->addBindValue(id);
}
