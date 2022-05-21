#ifndef PLAYER_SHOTCUT_H
#define PLAYER_SHOTCUT_H
#include<QMap>
#include<QPointer>
#include<QShortcut>
#include<QKeySequence>
#include<QPushButton>
#include"player.h"
 class Player_Shortcut : public QObject
{
    Q_OBJECT
public:
   Player_Shortcut( QPointer<Player> parent=NULL ) ;
   bool AddShortcut(const char* shortcut, QPushButton *button);
   //需传参的添加函数
   bool AddSliderShortcut(const char* shortcut,int add,bool);
  void RemoveShortcut(const char* shortcut);
   ~Player_Shortcut() {}
  //默认的快捷键控制
  void Init();
  //播放音频可能需要移出进度微调的快捷键
 void playAudio();
 //播放视频可能需要增加进度微调的快捷键
 void playVideo();
  signals:
   void changeVolume(const int add);
   void changeProgress(const int add);
 private :
   //关联的控制模块
  //QPointer<PlayerController>playerController;
   //QShortcut关联的父部件
   QPointer<Player>parent;
   //快捷键列表
  QMap <const char*,QPointer<QShortcut>> shortcut_list;
  QPointer<QShortcut>GenerateShortcut(const char* shortcut);

};
#endif // PLAYER_SHOTCUT_H
