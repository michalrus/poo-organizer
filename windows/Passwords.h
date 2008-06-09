#ifndef PASSWORDS_H
#   define PASSWORDS_H

#	include "CategoryView.h"

class WrapTextEdit;

class Passwords : public CategoryView
{
	Q_OBJECT

public:
	Passwords (QWidget *parent = 0);

protected:
    virtual void disableDetails ();
	virtual void loadDetails (const QSqlQuery *q);
	virtual void prepareUpdateQuery (QSqlQuery *q, const QString &id, const QString &name) const;

private:
	QLineEdit *user;
	QLineEdit *password;
	QLineEdit *email;
	WrapTextEdit *description;
};

#endif
