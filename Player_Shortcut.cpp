#include "Player_Shortcut.h"
#include<QMetaObject>
#include "ui_Player.h"
//添加一个快捷键
//shortcut指类似“Ctrl+D"等的描述
//controllerFName指对应控制模块的信号
Player_Shortcut::Player_Shortcut(QPointer<Player> parent_)
{
    parent=parent_;

      shortcut_list.clear();
      Init();

}


bool Player_Shortcut::AddShortcut(const char*shortcut, QPushButton *button)
{ if(shortcut_list.contains(shortcut))
        return false;
  QPointer<QShortcut> newShortcut=GenerateShortcut(shortcut);
 connect(newShortcut,&QShortcut::activated,button,&QPushButton::click);
 return true;
}
bool Player_Shortcut::AddSliderShortcut(const char *shortcut,  int add,bool choose)
{
    if(shortcut_list.contains(shortcut))
            return false;
      QPointer<QShortcut> newShortcut=GenerateShortcut(shortcut);
     connect( newShortcut,&QShortcut::activated,this,[=](){
          //接收到匹配快捷键，立即调用控制模块相应的函数
         if(choose)
        emit  changeVolume(add);
         else emit changeProgress(add);
        });
     return true;
}


void Player_Shortcut::RemoveShortcut(const char* shortcut)
{
 auto target=shortcut_list.find(shortcut);
 if(target!=shortcut_list.end()){
     disconnect(target.value(),0,0,0);
     delete target.value();
     shortcut_list.remove(shortcut);
 }
}

void Player_Shortcut::Init()
{
  AddShortcut("space",parent->ui()->play);
 AddShortcut("ctrl+left",parent->ui()->prev);
AddShortcut("ctrl+Right",parent->ui()->next);
AddShortcut("ctrl+i",parent->ui()->open);
 AddShortcut("ctrl+f",parent->ui()->isfullScreen);
AddSliderShortcut("ctrl+down",-20,true);
AddSliderShortcut("ctrl+Up",+20,true);
AddSliderShortcut("down",+1,false);
AddSliderShortcut("up",-1,false);
AddSliderShortcut("left",-5,false);
AddSliderShortcut("right",+5,false);
}

void Player_Shortcut::playAudio()
{
    if(shortcut_list.contains("down"))
            return;
    RemoveShortcut("down");
    RemoveShortcut("up");
    RemoveShortcut("left");
    RemoveShortcut("right");

}

void Player_Shortcut::playVideo()
{if(!shortcut_list.contains("down"))
        return;
    AddSliderShortcut("down",+1,false);
    AddSliderShortcut("up",-1,false);
    AddSliderShortcut("left",-5,false);
    AddSliderShortcut("right",+5,false);
}

//shortcut指类似“Ctrl+D"等的描述
QPointer<QShortcut> Player_Shortcut::GenerateShortcut(const char*shortcut)
{
    QPointer<QShortcut> newShortcut= new QShortcut(QKeySequence(shortcut),parent);
    newShortcut->setContext(Qt::ApplicationShortcut);
    shortcut_list.insert(shortcut,newShortcut);
   return newShortcut;
}

