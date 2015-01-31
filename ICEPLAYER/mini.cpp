#include <qwidgetresizehandler_p.h>
#include <QPixmap>
#include <QPainter>
#include <QMenu>
#include <QAction>
#include <QWidgetAction>
#include <QActionGroup>
#include <QSlider>

#include "mini.h"
#include "iceButton.h"
#include "iceplayer.h"


miniwindow::miniwindow(QWidget *parent):QWidget(parent)
{
	setWindowFlags(Qt::FramelessWindowHint | Qt::Tool | Qt::WindowStaysOnTopHint);
	setAttribute(Qt::WA_TranslucentBackground);
	//半透明
	this->setWindowOpacity(0.9);

	//窗口拖动
	QWidgetResizeHandler *movewin = new QWidgetResizeHandler(this);
	movewin->setMovingEnabled(true);

	//固定大小
	this->setFixedSize(100, 100);

	playButton = new iceButton( this);
	playButton->setObjectName(QStringLiteral("playButton"));
	playButton->setGeometry(QRect(29, 29, 34, 34));

	pauseButton = new iceButton( this);
	pauseButton->setObjectName(QStringLiteral("pauseButton"));
	pauseButton->setGeometry(QRect(24, 29, 34, 34));
	pauseButton->setVisible(false);

	mininextButton = new iceButton( this);
	mininextButton->setObjectName(QStringLiteral("nextButton"));
	mininextButton->setGeometry(QRect(61, 48, 34, 34));

	//设置图标
	QIcon icon_play, icon_play_focus;
	icon_play.addFile(QStringLiteral(":/IcePlayer/Resources/播放按钮mini.png"), QSize(), QIcon::Normal, QIcon::Off);
	icon_play_focus.addFile(QStringLiteral(":/IcePlayer/Resources/播放按钮mini.png"), QSize(), QIcon::Normal, QIcon::Off);
	playButton->SetButtonIcons(icon_play, icon_play_focus);

	playButton->setIconSize(QSize(34,34));
	playButton->setFlat(true);
	playButton->setFocusPolicy(Qt::NoFocus);
	playButton->setStyleSheet("QPushButton{background-color:rgba(255,255,255,0);border-style:solid;border-width:0px;border-color:rgba(255,255,255,0);}");

	QIcon icon_pause, icon_pause_focus;
	icon_pause.addFile(QStringLiteral(":/IcePlayer/Resources/暂停按钮mini.png"), QSize(), QIcon::Normal, QIcon::Off);
	icon_pause_focus.addFile(QStringLiteral(":/IcePlayer/Resources/暂停按钮mini.png"), QSize(), QIcon::Normal, QIcon::Off);
	pauseButton->SetButtonIcons(icon_pause, icon_pause_focus);

	pauseButton->setIconSize(QSize(34,34));
	pauseButton->setFlat(true);
	pauseButton->setFocusPolicy(Qt::NoFocus);
	pauseButton->setStyleSheet("QPushButton{background-color:rgba(255,255,255,0);border-style:solid;border-width:0px;border-color:rgba(255,255,255,0);}");

	QIcon icon_next, icon_next_focus;
	icon_next.addFile(QStringLiteral(":/IcePlayer/Resources/前进按钮mini.png"), QSize(), QIcon::Normal, QIcon::Off);
	icon_next_focus.addFile(QStringLiteral(":/IcePlayer/Resources/前进按钮mini.png"), QSize(), QIcon::Normal, QIcon::Off);
	mininextButton->SetButtonIcons(icon_next, icon_next_focus);

	mininextButton->setIconSize(QSize(9,15));
	mininextButton->setFlat(true);
	mininextButton->setFocusPolicy(Qt::NoFocus);
	mininextButton->setStyleSheet("QPushButton{background-color:rgba(255,255,255,0);border-style:solid;border-width:0px;border-color:rgba(255,255,255,0);}");
	
	initActions();
	connect(playButton, SIGNAL(clicked()), this, SLOT(play()));
	connect(pauseButton, SIGNAL(clicked()), this, SLOT(pause()));
	connect(mininextButton, SIGNAL(clicked()), this, SLOT(nextsong()));
}
miniwindow::~miniwindow()
{
}

void miniwindow::initActions()
{
	mode0 = new QAction(QString::fromLocal8Bit("单曲播放"), this);
	mode1 = new QAction(QString::fromLocal8Bit("列表循环"), this);
	mode2 = new QAction(QString::fromLocal8Bit("单曲循环"), this);
	mode3 = new QAction(QString::fromLocal8Bit("随机播放"), this);

	connect(mode0, SIGNAL(triggered()), this, SLOT(setmode()));
	connect(mode1, SIGNAL(triggered()), this, SLOT(setmode()));
	connect(mode2, SIGNAL(triggered()), this, SLOT(setmode()));
	connect(mode3, SIGNAL(triggered()), this, SLOT(setmode()));

	modegroup = new QActionGroup(this);
	modegroup->addAction(mode0);
	modegroup->addAction(mode1);
	modegroup->addAction(mode2);
	modegroup->addAction(mode3);

	mode0->setCheckable(true);
	mode1->setCheckable(true);
	mode2->setCheckable(true);
	mode3->setCheckable(true);
	mode0->setChecked(true);


	mainwin = new QAction(QString::fromLocal8Bit("返回主界面"),this);
	exit = new QAction(QString::fromLocal8Bit("退出"),this);
	next = new QAction(QString::fromLocal8Bit("下一曲"),this);
	last = new QAction(QString::fromLocal8Bit("上一曲"),this);
	lyric = new QAction(QString::fromLocal8Bit("桌面歌词"), this);


	volslider = new QSlider(Qt::Horizontal, this);
	volslider->setRange(0, 100);
	volslider->setGeometry(QRect(26,0,75,25));
	connect(volslider, SIGNAL(valueChanged(int)), this, SIGNAL(volumeChanged(int)));

	volslider->setStyleSheet("QSlider::groove:horizontal{border:0px;height:4px;}"  
		"QSlider::sub-page:horizontal{background:#0096ff;}"  
		"QSlider::add-page:horizontal{background:lightgray;} "
		"QSlider::handle:horizontal{background:white;width:10px;border:#51b5fb 10px;border-radius:5px;margin:-3px 0px -3px 0px;}");

	setvol = new QWidgetAction(this);
	setvol->setDefaultWidget(volslider);

	contextmenu = new QMenu(this);
	contextmenu->addAction(mainwin);
	contextmenu->addSeparator();
	contextmenu->addAction(setvol);
	contextmenu->addSeparator();
	contextmenu->addAction(next);
	contextmenu->addAction(last);
	contextmenu->addSeparator();
	contextmenu->addActions(modegroup->actions());
	contextmenu->addSeparator();
	contextmenu->addAction(lyric);
	contextmenu->addSeparator();
	contextmenu->addAction(exit);


	contextmenu->setStyleSheet(
		"QMenu{padding:5px;background:white;border:1px solid gray;}"
		"QMenu::item{padding:0px 40px 0px 30px;height:25px;}"
		"QMenu::item:selected:enabled{background:#0096ff;color:white;}"
		"QMenu::item:selected:!enabled{background:transparent;}"
		"QMenu::separator{height:1px;background:lightgray;margin:5px 0px 5px 0px;}");

	connect(exit, SIGNAL(triggered()), this, SLOT(exitice()));
	connect(mainwin, SIGNAL(triggered()), this, SLOT(goback()));
	connect(next, SIGNAL(triggered()), this, SLOT(nextsong()));
	connect(last, SIGNAL(triggered()), this, SLOT(lastsong()));
	connect(playButton, SIGNAL(clicked()), this, SLOT(play()));
	connect(pauseButton, SIGNAL(clicked()), this, SLOT(pause()));
	connect(volslider, SIGNAL(valueChanged(int)), this, SLOT(updatevol(int)));
	connect(lyric, SIGNAL(triggered()), this, SLOT(lyricaction()));

}

void miniwindow::exitice()
{
	parentwin->close();
	this->close();
}

void miniwindow::paintEvent(QPaintEvent *event)
{
	QPainter p(this);
	p.drawPixmap(0, 0 , QPixmap(":/IcePlayer/Resources/mini.png"));
}

void miniwindow::contextMenuEvent(QContextMenuEvent *event)
{
	contextmenu->exec(QCursor::pos());
}

void miniwindow::goback()
{
	parentwin->show();
	this->hide();
}

void miniwindow::setparentwin(IcePlayer *parent)
{
	parentwin = parent;
}

void miniwindow::nextsong()
{
	
	parentwin->nextButton_clicked();
}

void miniwindow::lastsong()
{
	parentwin->lastButton_clicked();
}

void miniwindow::play()
{
	parentwin->playButton_clicked();
	pauseButton->setVisible(true);
	playButton->setVisible(false);
}

void miniwindow::pause()
{
	parentwin->pauseButton_clicked();
	pauseButton->setVisible(false);
	playButton->setVisible(true);
}

void miniwindow::setmode()
{
	if(mode0->isChecked()) {
		parentwin->setmodefrommini(0);
	}
	else if(mode1->isChecked()) {
		parentwin->setmodefrommini(1);
	}
	else if(mode2->isChecked()) {
		parentwin->setmodefrommini(2);
	}
	else if(mode3->isChecked()) {
		parentwin->setmodefrommini(3);
	}
}

void miniwindow::initmode(int vol)
{
	switch(parentwin->mode){
	case 0:
		mode0->setChecked(true);
		break;
	case 1:
		mode1->setChecked(true);
		break;
	case 2:
		mode2->setChecked(true);
		break;
	case 3:
		mode3->setChecked(true);
		break;
	}

	if(parentwin->playButton->isVisible()){
		playButton->setVisible(true);
		pauseButton->setVisible(false);
	}
	else {
		playButton->setVisible(false);
		pauseButton->setVisible(true);
	}

	volslider->setValue(vol);
}

void miniwindow::updatevol(int a)
{
	parentwin->setvolfrommini(a);
}

void miniwindow::lyricaction()
{
	parentwin->lyricButton_clicked();
}