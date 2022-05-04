#ifndef PLAYERSHOTCUT_H
#define PLAYERSHORTCUT_H
#include <QMap>
#include<QPointer>
#include<QShortcut>
#include"PlayerController.h"
#include<QKeySequence>
#include<QMetaObject>
#include"player.h"
 class PlayerShortcut : public QObject
{
    Q_OBJECT
public:
   PlayerShortcut( QPointer<Player> parent=NULL  ,QPointer<PlayerController> help=NULL) ;
   bool AddShortcut(const char* shortcut,const char* controllerFName);
   //需传参的添加函数
   bool AddSliderShortcut(const char* shortcut,bool isVolume,int add);
  void RemoveShortcut(const char* shortcut);
    virtual ~PlayerShortcut() {}
  //默认的快捷键控制
  void Init();
 private :
   //关联的控制模块
  QPointer<PlayerController>playerController;
   //QShortcut关联的父部件
   QPointer<Player>parent;
   //快捷键列表
  QMap <const char*,QPointer<QShortcut>> shortcutList;
  QPointer<QShortcut>GenerateShortcut(const char* shortcut);

};
#endif // PLAYERSHOTCUT_H
