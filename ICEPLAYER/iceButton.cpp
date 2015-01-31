#include "iceButton.h"
#include <qwidgetresizehandler_p.h>

iceButton::iceButton(QWidget *parent)  
	: QPushButton(parent)  
{  
	//setDown(false);  

	//设置图标无焦点
	setFocusPolicy(Qt::NoFocus);  

	//QWidgetResizeHandler *movewin = new QWidgetResizeHandler(this);
	//movewin->setMovingEnabled(true);
}  

iceButton::~iceButton()  
{  

}  

void iceButton::enterEvent(QEvent *event)  
{  
	if( isEnabled() && !m_FocusIcon.isNull() )  
		setIcon(m_FocusIcon);  
}  

void iceButton::leaveEvent(QEvent *event)  
{  
	if( isEnabled() )  
		setIcon(m_NormalIcon);  
}  


void iceButton::mousePressEvent(QMouseEvent *event)  
{  
	if( isEnabled() && !m_PressedIcon.isNull() )  
		setIcon(m_PressedIcon);  
	QPushButton::mousePressEvent(event);  
}  

void iceButton::mouseReleaseEvent(QMouseEvent *event)  
{  
	if( isEnabled() && m_FocusIcon.isNull() )  
		setIcon(m_FocusIcon);  
	QPushButton::mouseReleaseEvent(event);  
}  

void iceButton::SetButtonIcons(const QIcon &normal, const QIcon &focus, const QIcon &pressed)  
{  
	m_NormalIcon = normal;  
	m_FocusIcon = focus;  
	m_PressedIcon = pressed;  
	setIcon(m_NormalIcon);  
}  