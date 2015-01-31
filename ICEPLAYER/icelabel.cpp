#include "icelabel.h"


IceLabel::IceLabel(QWidget *parent)
	:	QLabel(parent)
{
	init();
}

IceLabel::~IceLabel()
{
	stop();
}

void IceLabel::stop()
{
	if (m_timer)
	{
		if (m_timer->isActive())
		{
			m_timer->stop();
		}
		delete m_timer;
		m_timer = NULL;
	}
}

void IceLabel::init(int ms)
{
	m_timer = NULL;
	m_ms = ms;
	m_context = "";
	m_pos = 0;
}

void IceLabel::SetText(const QString &context, const int &ms)
{
	bool b=false;

	m_context = context;
	m_ms = ms;

	if (m_ms != 0)
	{
		if (m_timer == NULL)
		{
			m_timer = new QTimer(this);

			if (m_timer)
			{
				b = connect(m_timer, SIGNAL(timeout()), this, SLOT(TimeOut()));
				m_timer->start(m_ms);
			}
		}
	}
}

void IceLabel::TimeOut()
{
	
	QString str="";

	if (m_pos > m_context.count())
	{
		m_pos = 0;
	}

	str = m_context.mid(m_pos);
			
	setText(str);
	m_pos++;
}
