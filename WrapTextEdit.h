#ifndef WRAPTEXTEDIT_H
#   define WRAPTEXTEDIT_H

#	include <QWidget>

class QTextEdit;
class QCheckBox;

class WrapTextEdit : public QWidget
{
	Q_OBJECT

public:
	WrapTextEdit(QWidget *parent = 0);
	QString text () const;

public slots:
	void clear ();
	void setText (const QString &text);

signals:
	void textChanged();

private:
	QTextEdit *edit;
	QCheckBox *wrap;
	bool emitTextChanged;

private slots:
	void wrapChanged (int s);
	void editChanged ();
};

#endif
