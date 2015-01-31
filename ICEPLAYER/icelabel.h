#ifndef ICELABEL_H
#define ICELABEL_H

#include <QLabel>
#include <QTimer>

class IceLabel : public QLabel
{
	Q_OBJECT
public:
	explicit IceLabel(QWidget *parent=0);
	~IceLabel();

	/*
	 * �����ı�:
	 * ���msΪ0�����ֲ�����(Ĭ�ϲ�����);
	 * �������setText()���ֽ�������(��ʹ��QLabel��ͬ);
	*/
	void SetText(const QString &context=0, const int &ms=0);
	void stop();

private slots:
	void TimeOut();

private:
	QTimer *m_timer;
	int m_ms;
	QString m_context;
	int m_pos;

private:
	void init(int ms=0);
};

#endif