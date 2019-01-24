#ifndef CR_OMNI_BAR_H
#define CR_OMNI_BAR_H

#include <QAction>
#include <QLineEdit>

class OmniBar : public QLineEdit
{
    Q_OBJECT
public:
    explicit OmniBar(QWidget *parent = nullptr);

    void update(const QString &address);

    QAction *siteInfoAction();
    QAction *bookmarksAction();

Q_SIGNALS:
    void loadRequested(const QString &address);

private:
    void returnPressed();

    QAction *m_siteInfoAction = nullptr;
    QAction *m_bookMarksAction = nullptr;
};

#endif
