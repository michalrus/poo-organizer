#include "WrapTextEdit.h"

#include <QVBoxLayout>
#include <QTextEdit>
#include <QCheckBox>

WrapTextEdit::WrapTextEdit (QWidget *parent) : QWidget(parent)
{
	emitTextChanged = 1;

	edit = new QTextEdit(this);
	wrap = new QCheckBox(tr("wrapping"), this);

	wrap->setChecked(1);
	edit->setAcceptRichText(0);

	connect(edit, SIGNAL(textChanged()), this, SLOT(editChanged()));
	connect(wrap, SIGNAL(stateChanged(int)), this, SLOT(wrapChanged(int)));

	QVBoxLayout *lay = new QVBoxLayout(this);
	lay->addWidget(edit);
	lay->addWidget(wrap);
	lay->setContentsMargins(0, 0, 0, 0);
	setLayout(lay);
}

QString WrapTextEdit::text () const
{
	return edit->toPlainText();
}

void WrapTextEdit::clear ()
{
	emitTextChanged = 0;
	edit->setPlainText("");
	emitTextChanged = 1;
}

void WrapTextEdit::setText (const QString &text)
{
	edit->setPlainText(text);
}

void WrapTextEdit::wrapChanged (int s)
{
	if (s == Qt::Checked) {
		edit->setLineWrapMode(QTextEdit::WidgetWidth);
		edit->setFontFamily("");
	}
	else {
		edit->setLineWrapMode(QTextEdit::NoWrap);
		edit->setFontFamily("courier new");
	}

	emitTextChanged = 0;
	edit->setPlainText(edit->toPlainText());
	emitTextChanged = 1;
}

void WrapTextEdit::editChanged ()
{
	if (emitTextChanged)
		emit textChanged();
}
