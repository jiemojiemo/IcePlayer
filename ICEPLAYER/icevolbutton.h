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
	int volume() const; //��������ֵ��0~100��
	void setslidervisiable(bool); //�����������Ƿ�ɼ�

public slots:
		void increaseVolume(); //���������ʹ��������һ����
		void descreaseVolume(); //���������ʹ��������һ����
		void setVolume(int volume); //��������
		void button_clicked(); //��ť�����¼��������˵�

signals:
		void volumeChanged(int volume); //�����ı��źţ������������϶�ʱ���䣩

private:
	iceButton *button;
	QSlider *slider;
	QMenu *menu;
	QWidgetAction *action;
};

#endif 