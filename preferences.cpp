#include "preferences.h"
#include "ui_preferences.h"
#include "soulnote.h"

Preferences::Preferences(SoulNote *parent) :
    ui(new Ui::Preferences)
{
    ui->setupUi(this);
    m_parent = parent;
    ui->fontComboBox->setCurrentFont(parent->textEditor->font());
    ui->lineWrapCheckBox->setChecked(parent->textEditor->lineWrapMode());
    ui->fontSizeSpinBox->setValue(parent->textEditor->font().pointSize());
}

Preferences::~Preferences()
{
    delete ui;
}

void Preferences::on_canelButton_clicked()
{
    close();
}

void Preferences::on_applyButton_clicked()
{
    m_parent->textEditor->setLineWrapMode(QPlainTextEdit::LineWrapMode(ui->lineWrapCheckBox->isChecked()));
    m_parent->textEditor->setFont(QFont(ui->fontComboBox->currentFont().family(), ui->fontSizeSpinBox->value()));
}

void Preferences::on_okButton_clicked()
{
    m_parent->textEditor->setLineWrapMode(QPlainTextEdit::LineWrapMode(ui->lineWrapCheckBox->isChecked()));
    m_parent->textEditor->setFont(QFont(ui->fontComboBox->currentFont().family(), ui->fontSizeSpinBox->value()));
    close();
}
