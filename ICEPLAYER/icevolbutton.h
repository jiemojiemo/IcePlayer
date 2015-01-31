#ifndef ICEVOLBUTTON_H
#define ICEVOLBUTTON_H

#include "iceButton.h"
#include <QWidget>

class QMenu;
class QSlider;
class QWidgetAction;


class IceVolButton : public QWidget
{
	Q_OBJECT
	Q_PROPERTY(int volume READ volume WRITE setVolume NOTIFY volumeChanged)

public:
	IceVolButton(QWidget *parent = 0); 
	int volume() const; //返回音量值（0~100）
	void setslidervisiable(bool); //设置音量条是否可见

public slots:
		void increaseVolume(); //点击音量条使音量增加一部分
		void descreaseVolume(); //点击音量条使音量减少一部分
		void setVolume(int volume); //设置音量
		void button_clicked(); //按钮单击事件，弹出菜单

signals:
		void volumeChanged(int volume); //音量改变信号（当音量条被拖动时发射）

private:
	iceButton *button;
	QSlider *slider;
	QMenu *menu;
	QWidgetAction *action;
};

#endif 