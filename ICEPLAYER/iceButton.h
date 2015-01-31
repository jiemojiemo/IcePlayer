#ifndef ICEBUTTON_H
#define ICEBUTTON_H

#include <QtGui> 
#include <QPushButton>

class iceButton : public QPushButton  
{  
	Q_OBJECT  
private:  
	QIcon m_NormalIcon;  
	QIcon m_FocusIcon;  
	QIcon m_PressedIcon;  
protected:  

	//���ء����롱�͡��뿪���¼�������ʱ�л�Ϊ��һ��ͼ�꣬�뿪ʱ��ԭ 
	void enterEvent(QEvent *event);  
	void leaveEvent(QEvent *event);  

	//������갴�º��ɿ��¼� ���л�ͼ��
	void mousePressEvent(QMouseEvent *event);  
	void mouseReleaseEvent(QMouseEvent *event);  
public:
	iceButton(QWidget *parent = 0);  
	~iceButton();

	//����ͼ��
	//��ָֻ��һ��ͼ�꣬��Ĭ�ϡ����롱�����£��ɿ���Ϊ��ͼ��
	void SetButtonIcons(const QIcon &normal, const QIcon &focus = QIcon(), const QIcon &pressed = QIcon() );  
};  


#endif