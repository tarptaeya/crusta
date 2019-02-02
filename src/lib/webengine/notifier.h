#ifndef CR_NOTIFIER_H
#define CR_NOTIFIER_H

#include <QDialog>
#include <QLabel>
#include <QPushButton>
#include <QWebEnginePage>

class Notifier : public QDialog
{
public:
    explicit Notifier(QWidget *parent = nullptr);
    void setData(const QUrl &origin, QWebEnginePage::Feature feature);

private:
    QLabel *m_featureLabel = nullptr;
    QPushButton *m_acceptButton = nullptr;
    QPushButton *m_rejectButton = nullptr;
};

#endif
