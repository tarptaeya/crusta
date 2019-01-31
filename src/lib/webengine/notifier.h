#ifndef CR_NOTIFIER_H
#define CR_NOTIFIER_H

#include <QPushButton>
#include <QDialog>

class Notifier : public QDialog
{
public:
    explicit Notifier(QWidget *parent = nullptr);

private:
    QPushButton *m_acceptButton = nullptr;
    QPushButton *m_rejectButton = nullptr;
};

#endif
