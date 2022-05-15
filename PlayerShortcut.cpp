#include "PlayerShortcut.h"
#include"player.h"
#include "ui_Player.h"
#include"PlayerController.h"

//添加一个快捷键
//shortcut指类似“Ctrl+D"等的描述
//controllerFName指对应控制模块的信号
PlayerShortcut::PlayerShortcut(QPointer<Player> parent1, QPointer<PlayerController> help)
{
    parent=parent1;
    playerController= help;
      shortcutList.clear();
      Init();
}

bool PlayerShortcut::AddShortcut(const char*shortcut, const char* controllerFName)
{ if(shortcutList.contains(shortcut))
        return false;
  QPointer<QShortcut> newShortcut=GenerateShortcut(shortcut);
 connect( newShortcut,&QShortcut::activated,this,[=](){
      //接收到匹配快捷键，立即调用控制模块相应的函数
   playerController->doShortcutEvent(controllerFName);
   } );

 return true;
}

bool PlayerShortcut::AddSliderShortcut(const char *shortcut, bool isVolume, int add)
{
    if(shortcutList.contains(shortcut))
            return false;
      QPointer<QShortcut> newShortcut=GenerateShortcut(shortcut);
     connect( newShortcut,&QShortcut::activated,this,[=](){
          //接收到匹配快捷键，立即调用控制模块相应的函数
         if(!isVolume)
      playerController->setProgressValue(add);
         else
              playerController->setVolumeValue(add);
       } );

     return true;
}

void PlayerShortcut::RemoveShortcut(const char* shortcut)
{
 auto target=shortcutList.find(shortcut);
 if(target!=shortcutList.end()){
     disconnect(target.value(),0,0,0);
     delete target.value();
     shortcutList.remove(shortcut);
 }
}

void PlayerShortcut::Init()
{
  AddShortcut("space","onClickPlay");
 AddShortcut("ctrl+left","onClickPrev");
AddShortcut("ctrl+Right","onClickNext");
AddSliderShortcut("ctrl+down",true,-20);
AddSliderShortcut("ctrl+Up",true,+20);
AddSliderShortcut("down",false,-1);
AddSliderShortcut("up",false,+1);
AddSliderShortcut("left",false,-5);
AddSliderShortcut("right",false,+5);
//唤起添加资源导入弹窗
 QPointer<QShortcut> newShortcut=GenerateShortcut("ctrl+i");
 connect(newShortcut,&QShortcut::activated,parent->ui()->open,&QPushButton::click);
newShortcut=GenerateShortcut("ctrl+F");
 connect(newShortcut,&QShortcut::activated,parent->ui()->isfullScreen,&QPushButton::click);
}

//shortcut指类似“Ctrl+D"等的描述
QPointer<QShortcut> PlayerShortcut::GenerateShortcut(const char*shortcut)
{
    QPointer<QShortcut> newShortcut= new QShortcut(QKeySequence(shortcut),parent);
    newShortcut->setContext(Qt::ApplicationShortcut);
    shortcutList.insert(shortcut,newShortcut);
   return newShortcut;
}

