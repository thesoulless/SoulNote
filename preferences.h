#ifndef PREFERENCES_H
#define PREFERENCES_H

#include <QDialog>

namespace Ui {
class Preferences;
}
class SoulNote;

class Preferences : public QDialog
{
    Q_OBJECT
    
public:
    explicit Preferences(SoulNote *parent = 0);
    ~Preferences();
    
private slots:
    void on_canelButton_clicked();

    void on_applyButton_clicked();

    void on_okButton_clicked();

private:
    Ui::Preferences *ui;
    SoulNote *m_parent;
};

#endif // PREFERENCES_H
