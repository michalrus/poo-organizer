#ifndef NOTES_H
#   define NOTES_H

#	include "CategoryView.h"

class WrapTextEdit;

class Notes : public CategoryView
{
	Q_OBJECT

public:
	Notes (QWidget *parent = 0);

protected:
    virtual void disableDetails ();
	virtual void loadDetails (const QSqlQuery *q);
	virtual void prepareUpdateQuery (QSqlQuery *q, const QString &id, const QString &name) const;

private:
	WrapTextEdit *what;
};

#endif
