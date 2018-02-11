/* ============================================================
* Crusta - Qt5 webengine browser
* Copyright (C) 2017-2018 Anmol Gautam <anmol@crustabrowser.com>
*
* THIS FILE IS A PART OF CRUSTA
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
* ============================================================ */

#include "sidepane.h"
#include <QWebEngineView>
#include <QLabel>
#include <QDir>
#include <QMovie>

#include <iostream>


SidePaneButton::SidePaneButton(){
    click=0;
    setStyleSheet("QPushButton{border: none;margin: 0} QPushButton::hover{background-color: #d0d0d0;}");
    setFixedSize(40,40);
    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this,&SidePaneButton::customContextMenuRequested,this,&SidePaneButton::buttonContext);
    sidewebview->settings()->setAttribute(QWebEngineSettings::PluginsEnabled, true);
}

SidePane::SidePane(MainView* m){
    mainview=m;
    QWidget* left=new QWidget();
    top->setFixedSize(40,32);
    top->setStyleSheet("QPushButton{border: none;margin: 0; background-color: #00b0e3}");
    vbox->addWidget(top);
    vbox->addWidget(history);
    history->setToolTip(tr("History"));
    history->setContextMenuPolicy(Qt::NoContextMenu);
    history->setIcon(QIcon(":/res/drawables/pane_history.svg"));
    vbox->addWidget(bookmarks);
    bookmarks->setToolTip(tr("Bookmarks"));
    bookmarks->setContextMenuPolicy(Qt::NoContextMenu);
    bookmarks->setIcon(QIcon(":/res/drawables/pane_bookmark.svg"));
    vbox->addWidget(downloads);
    downloads->setToolTip(tr("Downloads"));
    downloads->setContextMenuPolicy(Qt::NoContextMenu);
    downloads->setIcon(QIcon(":/res/drawables/pane_download.svg"));
    QFile f_(QDir::homePath()+"/.crusta_db/sidepanel.txt");
    if(!f_.exists()){
        f_.open(QIODevice::WriteOnly);
        QTextStream in(&f_);
        in.setCodec("UTF-8");
        in<<"http://m.facebook.com\nhttp://m.twitter.com\nhttp://time.is\n";
        f_.close();
    }
    QFile inputFile(QDir::homePath()+"/.crusta_db/sidepanel.txt");
    if (inputFile.open(QIODevice::ReadOnly))
    {
       QTextStream in(&inputFile);
       in.setCodec("UTF-8");
       while (!in.atEnd())
       {
          QString line = in.readLine();
          if(line.split("//").length()<2) continue;
          QString icon_name=line.split("//")[1];
          if(icon_name.startsWith("www.")|| icon_name.startsWith("m.")){
              icon_name=icon_name.split(".")[1];
          }else{
              icon_name=icon_name.split(".")[0];
          }
          SidePaneButton* new_side_btn=new SidePaneButton();
          new_side_btn->url=line;
          new_side_btn->icon_=icon_name;
          new_side_btn->setToolTip(line);
          new_side_btn->setIcon(QIcon(QDir::homePath()+"/.crusta_db/sidepanel/ico/"+icon_name+".png"));
          QWebEnginePage* webpage=new QWebEnginePage();
          new_side_btn->sidewebview->hide();
          connect(webpage,&QWebEnginePage::fullScreenRequested,this,&SidePane::acceptFullScreenReuest);
          new_side_btn->sidewebview->setPage(webpage);
          new_side_btn->sidewebview->setTabletTracking(true);
          new_side_btn->url=line;
          new_side_btn->sidewebview->setMaximumWidth(395);
          new_side_btn->sidewebview->setMinimumWidth(300);
          connect(new_side_btn,&SidePaneButton::clicked,this,[this,line,new_side_btn]{
              if(new_side_btn->click==0){
                  new_side_btn->click=1;
                  new_side_btn->sidewebview->load(QUrl(new_side_btn->url));
              }
              if(hbox->count()==1){
                  new_side_btn->sidewebview->show();
                  hbox->addWidget(new_side_btn->sidewebview);
              }
              else if(hbox->indexOf(new_side_btn->sidewebview)!=1){
                  hbox->itemAt(1)->widget()->hide();
                  hbox->removeItem(hbox->itemAt(1));
                  new_side_btn->sidewebview->show();
                  hbox->addWidget(new_side_btn->sidewebview);
              }
              else{
                  hbox->itemAt(1)->widget()->hide();
                  hbox->removeItem(hbox->itemAt(1));
              }
          });
          if(new_side_btn->icon().pixmap(27,27).isNull()){
              QMovie* loader=new QMovie(":/res/videos/sidepanel_loader.gif");
              loader->start();
              connect(loader,&QMovie::frameChanged,new_side_btn,[new_side_btn,loader]{
                  new_side_btn->setIcon(QIcon(loader->currentPixmap()));
              });
              connect(new_side_btn->sidewebview,&QWebEngineView::iconChanged,this,[this,new_side_btn,loader]{
                  loader->stop();
                  new_side_btn->setIcon(new_side_btn->sidewebview->icon());
                  QString icon_name=new_side_btn->sidewebview->url().toString().split("//")[1];
                  if(icon_name.startsWith("www.")|| icon_name.startsWith("m.")){
                      icon_name=icon_name.split(".")[1];
                  }
                  if(icon_name.startsWith("mobile")){
                      icon_name=icon_name.split(".")[1];
                  }else{
                      icon_name=icon_name.split(".")[0];
                  }
                  new_side_btn->sidewebview->icon().pixmap(27,27).save(QDir::homePath()+"/.crusta_db/sidepanel/ico/"+icon_name+".png");
              });
          }
          vbox->addWidget(new_side_btn);
       }
       inputFile.close();
    }
    vbox->addWidget(flexilabel);
    vbox->addWidget(add_pane_btn);
    add_pane_btn->setToolTip(tr("Add New Pane Button"));
    add_pane_btn->setIcon(QIcon(":/res/drawables/pane_add.svg"));
    vbox->setSpacing(0);
    vbox->setContentsMargins(0,0,0,0);
    left->setLayout(vbox);
    left->setStyleSheet("background-color: #f0f0f0");
    left->setFixedWidth(40);
    left->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Expanding);
    hbox->addWidget(left);
    connect(history,&QPushButton::clicked,this,[this]{
        if(hbox->count()==1){
            history_manager=new HistoryManager(mainview);
            history_manager->createManager();
            hbox->addWidget(this->history_manager);
        }
        else if(hbox->indexOf(this->history_manager)!=1){
            hbox->itemAt(1)->widget()->hide();
            hbox->removeItem(hbox->itemAt(1));
            history_manager=new HistoryManager(mainview);
            history_manager->createManager();
            hbox->addWidget(this->history_manager);
        }
        else{
            this->history_manager->hide();
            hbox->removeWidget(this->history_manager);
        }
    });
    connect(bookmarks,&QPushButton::clicked,this,[this]{
        if(hbox->count()==1){
            bookmark_manager=new BookmarkManager(mainview);
            hbox->addWidget(this->bookmark_manager);
        }
        else if(hbox->indexOf(this->bookmark_manager)!=1){
            hbox->itemAt(1)->widget()->hide();
            hbox->removeItem(hbox->itemAt(1));
            bookmark_manager=new BookmarkManager(mainview);
            hbox->addWidget(this->bookmark_manager);
        }
        else{
            this->bookmark_manager->hide();
            hbox->removeWidget(this->bookmark_manager);
        }
    });
    connect(downloads,&QPushButton::clicked,this,[this]{
        if(hbox->count()==1){
            this->download_manager->show();
            hbox->addWidget(this->download_manager);
        }
        else if(hbox->indexOf(this->download_manager)!=1){
            hbox->itemAt(1)->widget()->hide();
            hbox->removeItem(hbox->itemAt(1));
            this->download_manager->show();
            hbox->addWidget(this->download_manager);
        }
        else{
            this->download_manager->hide();
            hbox->removeWidget(this->download_manager);
        }
    });
    connect(add_pane_btn,&SidePaneButton::clicked,this,&SidePane::addNewButton);
    hbox->setSpacing(0);
    hbox->setContentsMargins(0,0,0,0);
    setLayout(hbox);
    setMaximumWidth(40+350+45);
    setObjectName("pane");
    setStyleSheet("#pane{background-color: #f0f0f0;}");
}

SidePane::SidePane(PrivateMainView* m){
    pmainview=m;
    QWidget* left=new QWidget();
    QPushButton* top = new QPushButton();
    top->setFixedSize(40,32);
    top->setIcon(QIcon(":/res/drawables/incognito.svg"));
    top->setIconSize(QSize(25, 25));
    top->setStyleSheet("QPushButton{border: none;margin: 0; background-color: #404244}");
    vbox->addWidget(top);
    vbox->addWidget(bookmarks);
    bookmarks->setToolTip(tr("Bookmarks"));
    bookmarks->setContextMenuPolicy(Qt::NoContextMenu);
    bookmarks->setIcon(QIcon(":/res/drawables/pane_bookmark.svg"));
    vbox->addWidget(downloads);
    downloads->setToolTip(tr("Downloads"));
    downloads->setContextMenuPolicy(Qt::NoContextMenu);
    downloads->setIcon(QIcon(":/res/drawables/pane_download.svg"));
    QFile inputFile(QDir::homePath()+"/.crusta_db/sidepanel.txt");
    if (inputFile.open(QIODevice::ReadOnly))
    {
       QTextStream in(&inputFile);
       in.setCodec("UTF-8");
       while (!in.atEnd())
       {
          QString line = in.readLine();
          if(line.split("//").length()<2) continue;
          QString icon_name=line.split("//")[1];
          if(icon_name.startsWith("www.")|| icon_name.startsWith("m.")){
              icon_name=icon_name.split(".")[1];
          }
          else{
              icon_name=icon_name.split(".")[0];
          }
          SidePaneButton* new_side_btn=new SidePaneButton();
          new_side_btn->url=line;
          new_side_btn->icon_=icon_name;
          new_side_btn->setToolTip(line);
          new_side_btn->setIcon(QIcon(QDir::homePath()+"/.crusta_db/sidepanel/ico/"+icon_name+".png"));
          QWebEnginePage* webpage=new QWebEnginePage();
          new_side_btn->sidewebview->hide();
          connect(webpage,&QWebEnginePage::fullScreenRequested,this,&SidePane::acceptFullScreenReuest);
          new_side_btn->sidewebview->setPage(webpage);
          new_side_btn->sidewebview->setTabletTracking(true);
          new_side_btn->sidewebview->setMaximumWidth(395);
          new_side_btn->sidewebview->setMinimumWidth(300);
          connect(new_side_btn,&SidePaneButton::clicked,this,[this,line,new_side_btn]{
              if(new_side_btn->click==0){
                  new_side_btn->click=1;
                  new_side_btn->sidewebview->load(QUrl(new_side_btn->url));
              }
              if(hbox->count()==1){
                  new_side_btn->sidewebview->show();
                  hbox->addWidget(new_side_btn->sidewebview);
              }
              else if(hbox->indexOf(new_side_btn->sidewebview)!=1){
                  hbox->itemAt(1)->widget()->hide();
                  hbox->removeItem(hbox->itemAt(1));
                  new_side_btn->sidewebview->show();
                  hbox->addWidget(new_side_btn->sidewebview);
              }
              else{
                  hbox->itemAt(1)->widget()->hide();
                  hbox->removeItem(hbox->itemAt(1));
              }
          });
          if(new_side_btn->icon().pixmap(27,27).isNull()){
              QMovie* loader=new QMovie(":/res/videos/sidepanel_loader.gif");
              loader->start();
              connect(loader,&QMovie::frameChanged,new_side_btn,[new_side_btn,loader]{
                  new_side_btn->setIcon(QIcon(loader->currentPixmap()));
              });
              connect(new_side_btn->sidewebview,&QWebEngineView::iconChanged,this,[this,new_side_btn,loader]{
                  loader->stop();
                  new_side_btn->setIcon(new_side_btn->sidewebview->icon());
                  QString icon_name=new_side_btn->sidewebview->url().toString().split("//")[1];
                  if(icon_name.startsWith("www.")|| icon_name.startsWith("m.")){
                      icon_name=icon_name.split(".")[1];
                  }
                  if(icon_name.startsWith("mobile")){
                      icon_name=icon_name.split(".")[1];
                  }else{
                      icon_name=icon_name.split(".")[0];
                  }
                  new_side_btn->sidewebview->icon().pixmap(27,27).save(QDir::homePath()+"/.crusta_db/sidepanel/ico/"+icon_name+".png");
              });
          }
          vbox->addWidget(new_side_btn);
       }
       inputFile.close();
    }
    vbox->addWidget(flexilabel);
    vbox->addWidget(add_pane_btn);
    add_pane_btn->setToolTip(tr("Add New Pane Button"));
    add_pane_btn->setIcon(QIcon(":/res/drawables/pane_add.svg"));
    vbox->setSpacing(0);
    vbox->setContentsMargins(0,0,0,0);
    left->setLayout(vbox);
    left->setStyleSheet("background-color: #f0f0f0");
    left->setFixedWidth(40);
    left->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Expanding);
    hbox->addWidget(left);
    connect(bookmarks,&QPushButton::clicked,this,[this]{
        if(hbox->count()==1){
            pbookmark_manager=new PrivateBookmarkManager(pmainview);
            hbox->addWidget(this->pbookmark_manager);
        }
        else if(hbox->indexOf(this->pbookmark_manager)!=1){
            hbox->itemAt(1)->widget()->hide();
            hbox->removeItem(hbox->itemAt(1));
            pbookmark_manager=new PrivateBookmarkManager(pmainview);
            hbox->addWidget(this->pbookmark_manager);
        }
        else{
            this->pbookmark_manager->hide();
            hbox->removeWidget(this->pbookmark_manager);
        }
    });
    connect(downloads,&QPushButton::clicked,this,[this]{
        if(hbox->count()==1){
            this->download_manager->show();
            hbox->addWidget(this->download_manager);
        }
        else if(hbox->indexOf(this->download_manager)!=1){
            hbox->itemAt(1)->widget()->hide();
            hbox->removeItem(hbox->itemAt(1));
            this->download_manager->show();
            hbox->addWidget(this->download_manager);
        }
        else{
            this->download_manager->hide();
            hbox->removeWidget(this->download_manager);
        }
    });
    connect(add_pane_btn,&SidePaneButton::clicked,this,&SidePane::addNewButton);
    hbox->setSpacing(0);
    hbox->setContentsMargins(0,0,0,0);
    setLayout(hbox);
    setMaximumWidth(40+350+45);
    setObjectName("pane");
    setStyleSheet("#pane{background-color: #f0f0f0}");
}


void SidePane::addNewButton(){
    QDialog* dg=new QDialog();
    QLineEdit* urledit=new QLineEdit();
    urledit->setMinimumWidth(200);
    urledit->setPlaceholderText("http://example.com");
    QPushButton* ok=new QPushButton(tr("Add"));
    QPushButton* cncl=new QPushButton(tr("Cancel"));
    QHBoxLayout* box=new QHBoxLayout();
    box->addWidget(urledit);
    box->addWidget(ok);
    box->addWidget(cncl);
    dg->setLayout(box);
    dg->setWindowFlags(Qt::FramelessWindowHint | Qt::Popup);
    connect(cncl,&QPushButton::clicked,dg,&QDialog::reject);
    connect(ok,&QPushButton::clicked,dg,&QDialog::accept);
    dg->move(mapToGlobal(QPoint(add_pane_btn->x()+30,add_pane_btn->y()-30)));
    QString theme;
    QFile inputFile(QDir::homePath()+"/.crusta_db/settings.txt");
    if (inputFile.open(QIODevice::ReadOnly))
    {
       QTextStream in(&inputFile);
       in.setCodec("UTF-8");
       while (!in.atEnd())
       {
          QString line = in.readLine();
          if(line.split(">>>>>").length()<2)
              continue;
          if(line.split(">>>>>")[0]=="theme")
              theme=line.split(">>>>>")[1];
       }
       inputFile.close();
    }
    QString bgcolor = QString(QString(theme.split(" ")[1]).split("{")[1]).split("}")[0];
    dg->setObjectName("dialog");
    dg->setStyleSheet("#dialog{border: 2px solid "+ bgcolor.split(":")[1] +"}");
    urledit->setFocus();
    if(!dg->exec()==QDialog::Accepted){
        return;
    }
    QString url=urledit->text();
    if(url.isEmpty()) return;
    if(!url.startsWith("http")){
        url="http://"+url;
        urledit->setText(url);
    }
    QFile file(QDir::homePath()+"/.crusta_db/sidepanel.txt");
    file.open(QIODevice::WriteOnly | QIODevice::Append);
    QTextStream out(&file);
    out.setCodec("UTF-8");
    out << urledit->text().toUtf8()+"\n";
    file.close();
    SidePaneButton* new_btn=new SidePaneButton();
    new_btn->setIcon(QIcon(":/res/videos/sidepanel_loader.gif"));
    QMovie* loader=new QMovie(":/res/videos/sidepanel_loader.gif");
    loader->start();
    connect(loader,&QMovie::frameChanged,new_btn,[new_btn,loader]{
        new_btn->setIcon(QIcon(loader->currentPixmap()));
    });
    vbox->insertWidget(vbox->indexOf(flexilabel),new_btn);
    QWebEnginePage* webpage=new QWebEnginePage();
    new_btn->sidewebview->setPage(webpage);
    new_btn->sidewebview->setTabletTracking(true);
    new_btn->sidewebview->load(QUrl(url));
    new_btn->url=urledit->text();
    new_btn->sidewebview->setMaximumWidth(395);
    new_btn->sidewebview->setMinimumWidth(300);
    connect(new_btn->sidewebview,&QWebEngineView::iconChanged,this,[this,new_btn,loader]{
        loader->stop();
        new_btn->setIcon(new_btn->sidewebview->icon());
        QString icon_name=new_btn->sidewebview->url().toString().split("//")[1];
        if(icon_name.startsWith("www.")|| icon_name.startsWith("m.")){
            icon_name=icon_name.split(".")[1];
        }
        if(icon_name.startsWith("mobile")){
            icon_name=icon_name.split(".")[1];
        }else{
            icon_name=icon_name.split(".")[0];
        }
        new_btn->icon_=icon_name;
        new_btn->sidewebview->icon().pixmap(27,27).save(QDir::homePath()+"/.crusta_db/sidepanel/ico/"+icon_name+".png");
    });
    connect(new_btn,&SidePaneButton::clicked,this,[this,new_btn]{
        if(hbox->count()==1){
            new_btn->sidewebview->show();
            hbox->addWidget(new_btn->sidewebview);
        }
        else if(hbox->indexOf(new_btn->sidewebview)!=1){
            hbox->itemAt(1)->widget()->hide();
            hbox->removeItem(hbox->itemAt(1));
            new_btn->sidewebview->show();
            hbox->addWidget(new_btn->sidewebview);
        }
        else{
            hbox->itemAt(1)->widget()->hide();
            hbox->removeItem(hbox->itemAt(1));
        }
    });
}

void SidePane::acceptFullScreenReuest(QWebEngineFullScreenRequest request){
    request.accept();
}

void SidePaneButton::buttonContext(const QPoint &point){
    QMenu* menu=new QMenu();
    QAction* remove=new QAction(tr("Remove"));
    menu->addAction(remove);
    connect(remove,&QAction::triggered,this,[this]{
        QString forbidden=this->url;
        QFile f(QDir::homePath()+"/.crusta_db/sidepanel.txt");
        if(f.open(QIODevice::ReadWrite | QIODevice::Text))
        {
            QString s;
            QTextStream t(&f);
            t.setCodec("UTF-8");
            while(!t.atEnd())
            {
                QString line = t.readLine();
                if(line!=forbidden)
                    s.append(line + "\n");
            }
            f.resize(0);
            t << s;
            f.close();
        }
        QString icon_name=this->icon_;
        QDir dir;
        dir.remove(QDir::homePath()+"/.crusta_db/sidepanel/ico/"+icon_name+".png");
        this->sidewebview->load(QUrl("http://"));
        this->sidewebview->page()->deleteLater();
        this->parentWidget()->layout()->removeWidget(this);
        this->sidewebview->deleteLater();
        this->deleteLater();
    });
    menu->exec(this->mapToGlobal(point));
}
