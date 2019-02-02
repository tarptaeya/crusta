#ifndef CR_OMNI_BAR_H
#define CR_OMNI_BAR_H

#include <QAction>
#include <QCompleter>
#include <QLineEdit>
#include <QStringListModel>

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
    void loadCompleter();

    QAction *m_siteInfoAction = nullptr;
    QAction *m_bookMarksAction = nullptr;

    QCompleter *m_completer = nullptr;
    QStringListModel *m_model = nullptr;
};

#endif
