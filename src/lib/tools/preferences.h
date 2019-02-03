#ifndef CR_PREFERENCES_H
#define CR_PREFERENCES_H

#include <QTabWidget>

class Preferences : public QTabWidget
{
public:
    explicit Preferences(QWidget *parent = nullptr);

    void show();
private:
    void createGeneralTab();
    void createBrowsingTab();
    void createSearchEngineTab();
    void createOthersTab();
};

#endif
