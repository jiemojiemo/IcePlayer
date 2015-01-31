#include "iceplayer.h"
#include "lyric.h"
#include "iceButton.h"
#include "about.h"
#include "icevolbutton.h"
#include "mini.h"
#include "icelabel.h"
#include "nofocusdelegate.h"

#include <QApplication>
#include <QPushButton>
#include <QLabel>
#include <QFrame>
#include <QSlider>
#include <QTableWidget>
#include <QMediaPlaylist>
#include <qwidgetresizehandler_p.h>
#include <QMenu>
#include <QDir>
#include <QFileDialog>
#include <QHeaderView>
#include <QTimer>
#include <QTime>
#include <QMediaMetaData>
#include <QPalette>
#include <QColor>
#include <QPainter>
#include <QBitmap>
#include <QFont>
#include <QTextCodec>
#include <QTextStream>
#include <QActionGroup>

IcePlayer::IcePlayer(QWidget *parent)
	: QWidget(parent)
{
	initUI();
	initPlayer();
	initConnections();
	initWindows();
	initActions();
	
	IceDir = QApplication::applicationDirPath();
	readList();
}

IcePlayer::~IcePlayer()
{
	writeList();
}

void IcePlayer::initWindows()
{
	aboutwindow = new about(this);
	lrc = new MyLrc(this);
	
	mini_win = new miniwindow();
	mini_win->setparentwin(this);
	mini_win->hide();

}

void IcePlayer::initActions()
{
	mode0 = new QAction(QString::fromLocal8Bit("��������"), this);
	mode1 = new QAction(QString::fromLocal8Bit("�б�ѭ��"), this);
	mode2 = new QAction(QString::fromLocal8Bit("����ѭ��"), this);
	mode3 = new QAction(QString::fromLocal8Bit("�������"), this);

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

	modemenu = new QMenu(modeButton);
	modemenu->setStyleSheet(
		"QMenu{padding:5px;background:white;border:1px solid gray;}"
		"QMenu::item{padding:0px 40px 0px 30px;height:25px;}"
		"QMenu::item:selected:enabled{background:#0096ff;color:white;}"
		"QMenu::item:selected:!enabled{background:transparent;}"
		"QMenu::separator{height:1px;background:lightgray;margin:5px 0px 5px 0px;}");
	modemenu->addActions(modegroup->actions());


	contextmenu_less = new QMenu(playlistTable);
	contextmenu_more = new QMenu(playlistTable);
	addsongs = new QAction(QString::fromLocal8Bit("��Ӹ���"),this);
	addfilediv = new QAction(QString::fromLocal8Bit("���Ŀ¼"),this);
	removethis = new QAction(QString::fromLocal8Bit("�Ƴ�����"),this);
	removeall = new QAction(QString::fromLocal8Bit("�Ƴ�����"),this);

	connect(addsongs, SIGNAL(triggered()), this, SLOT(openSongs()));
	connect(addfilediv, SIGNAL(triggered()), this, SLOT(openDir()));
	connect(removethis, SIGNAL(triggered()), this, SLOT(removeCurrentIndex()));
	connect(removeall, SIGNAL(triggered()), this, SLOT(clearList()));

	contextmenu_less->addAction(addsongs);
	contextmenu_less->addAction(addfilediv);
	contextmenu_less->addSeparator();
	contextmenu_less->addAction(removeall);

	contextmenu_more->addAction(addsongs);
	contextmenu_more->addAction(addfilediv);
	contextmenu_more->addSeparator();
	contextmenu_more->addAction(removethis);
	contextmenu_more->addAction(removeall);

	contextmenu_less->setStyleSheet(
		"QMenu{padding:5px;background:white;border:1px solid gray;}"
		"QMenu::item{padding:0px 40px 0px 30px;height:25px;}"
		"QMenu::item:selected:enabled{background:#0096ff;color:white;}"
		"QMenu::item:selected:!enabled{background:transparent;}"
		"QMenu::separator{height:1px;background:lightgray;margin:5px 0px 5px 0px;}");

	contextmenu_more->setStyleSheet(
		"QMenu{padding:5px;background:white;border:1px solid gray;}"
		"QMenu::item{padding:0px 40px 0px 30px;height:25px;}"
		"QMenu::item:selected:enabled{background:#0096ff;color:white;}"
		"QMenu::item:selected:!enabled{background:transparent;}"
		"QMenu::separator{height:1px;background:lightgray;margin:5px 0px 5px 0px;}");

}

void IcePlayer::initPlayer()
{
	mediaplayer = new QMediaPlayer(this);

	medialist = new QMediaPlaylist(this);
	mediaplayer->setPlaylist(medialist);

	mode = playmode::single;
	preIndex = -1;
	currentIndex = 0;

	medialist->setPlaybackMode(QMediaPlaylist::CurrentItemOnce);
}

void IcePlayer::initConnections()
{
	connect(mediaplayer, SIGNAL(positionChanged(qint64)), this, SLOT(updatePosition(qint64)));
	connect(mediaplayer, SIGNAL(durationChanged(qint64)), this, SLOT(updateDuration(qint64)));
	connect(mediaplayer, SIGNAL(stateChanged(QMediaPlayer::State)),this, SLOT(updateState(QMediaPlayer::State)));
	connect(mediaplayer, SIGNAL(metaDataChanged()), this, SLOT(updateMetaData()));
	connect(playSlider, SIGNAL(valueChanged(int)), this, SLOT(setPosition(int)));

	connect(volButton, SIGNAL(volumeChanged(int)), mediaplayer, SLOT(setVolume(int)));

	connect(playlistTable, SIGNAL(cellDoubleClicked(int, int)), this, SLOT(on_playlistTableWidget_cellDoubleClicked(int, int)));
	connect(playlistTable, SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(on_playlistTable_customContextMenuRequested(const QPoint &)));
	
	connect(addButton, SIGNAL(clicked()), this, SLOT(openSongs()));
	connect(exitButton, SIGNAL(clicked()), this, SLOT(close()));
	connect(playButton, SIGNAL(clicked()), this, SLOT(playButton_clicked()));
	connect(lyricButton, SIGNAL(clicked()), this, SLOT(lyricButton_clicked()));
	connect(logoButton, SIGNAL(clicked()), this, SLOT(logoButton_clicked()));
	connect(modeButton, SIGNAL(clicked()), this, SLOT(modeButton_clicked()));
	connect(nextButton, SIGNAL(clicked()), this, SLOT(nextButton_clicked()));
	connect(lastButton, SIGNAL(clicked()), this, SLOT(lastButton_clicked()));
	connect(pauseButton, SIGNAL(clicked()), this, SLOT(pauseButton_clicked()));
	connect(minButton, SIGNAL(clicked()), this, SLOT(minButton_clicked()));
	connect(mminButton, SIGNAL(clicked()), this, SLOT(showMinimized()));

}

void IcePlayer::initUI()
{
	picLabel = new QLabel( this);
	picLabel->setObjectName(QStringLiteral("picFrame"));
	picLabel->setGeometry(QRect(270, 40, 100, 100));
	
	picLabel->setScaledContents(true);
	playSlider = new QSlider( this);
	playSlider->setObjectName(QStringLiteral("playSlider"));

	playSlider->setStyleSheet("QSlider::groove:horizontal{border:0px;height:4px;}"  
		"QSlider::sub-page:horizontal{background:#0096ff;}"  
		"QSlider::add-page:horizontal{background:lightgray;} "
		"QSlider::handle:horizontal{background:white;width:10px;border:#51b5fb 10px;border-radius:5px;margin:-3px 0px -3px 0px;}");

	playSlider->setGeometry(QRect(40, 150, 290, 20));
	playSlider->setOrientation(Qt::Horizontal);

	

	timeLabel = new QLabel( this);
	timeLabel->setObjectName(QStringLiteral("timeLabel"));
	timeLabel->setGeometry(QRect(340, 150, 51, 16));
	playlistTable = new QTableWidget( this);
	playlistTable->setObjectName(QStringLiteral("playlistTable"));
	playlistTable->setGeometry(QRect(25, 240, 350, 320));
	playlistTable->setItemDelegate(new NoFocusDelegate());

	nameLabel = new IceLabel(this);
	nameLabel->setObjectName(QStringLiteral("nameLabel"));
	nameLabel->setGeometry(QRect(60, 60, 300, 20));

	musicianLabel = new QLabel(this);
	musicianLabel->setObjectName(QStringLiteral("musicianLabel"));
	musicianLabel->setGeometry(QRect(60, 90, 300, 15));

	albumLabel = new QLabel(this);
	albumLabel->setObjectName(QStringLiteral("albumLabel"));
	albumLabel->setGeometry(QRect(60, 120, 300, 15));

	addButton = new iceButton( this);
	addButton->setObjectName(QStringLiteral("addButton"));
	addButton->setGeometry(QRect(40, 190, 31, 31));
	modeButton = new iceButton( this);
	modeButton->setObjectName(QStringLiteral("modeButton"));
	modeButton->setGeometry(QRect(290, 190, 31, 31));
	lastButton = new iceButton( this);
	lastButton->setObjectName(QStringLiteral("lastButton"));
	lastButton->setGeometry(QRect(120, 185, 41, 41));
	nextButton = new iceButton( this);
	nextButton->setObjectName(QStringLiteral("nextButton"));
	nextButton->setGeometry(QRect(240, 185, 41, 41));
	lyricButton = new iceButton( this);
	lyricButton->setObjectName(QStringLiteral("lyricButton"));
	lyricButton->setGeometry(QRect(80, 190, 31, 31));
	playButton = new iceButton( this);
	playButton->setObjectName(QStringLiteral("playButton"));
	playButton->setGeometry(QRect(170, 175, 61, 61));
	pauseButton = new iceButton( this);
	pauseButton->setObjectName(QStringLiteral("pauseButton"));
	pauseButton->setGeometry(QRect(170, 175, 61, 61));
	pauseButton->setVisible(false);

	volButton = new IceVolButton( this);
	volButton->setObjectName(QStringLiteral("volButton"));
	volButton->setGeometry(QRect(333, 193, 31, 31));
	volButton->setVolume(100);

	mminButton = new iceButton( this);
	mminButton->setObjectName(QStringLiteral("mminButton"));
	mminButton->setGeometry(QRect(307, 3, 29, 31));
	minButton = new iceButton( this);
	minButton->setObjectName(QStringLiteral("minButton"));
	minButton->setGeometry(QRect(337, 3, 29, 31));
	exitButton = new iceButton( this);
	exitButton->setObjectName(QStringLiteral("exitButton"));
	exitButton->setGeometry(QRect(364, 3, 29, 31));
	logoButton = new iceButton( this);
	logoButton->setObjectName(QStringLiteral("logoButton"));
	logoButton->setGeometry(QRect(8, 3, 120, 48));

	logoButton->raise();
	picLabel->raise();
	playSlider->raise();
	timeLabel->raise();
	playlistTable->raise();
	addButton->raise();
	modeButton->raise();
	nextButton->raise();
	lyricButton->raise();
	playButton->raise();
	volButton->raise();
	mminButton->raise();
	minButton->raise();
	exitButton->raise();

	timeLabel->setText(tr("00:00"));
	nameLabel->setText(tr("Name " ));
	musicianLabel->setText(tr("Musician" ));
	albumLabel->setText(tr("Album" ));

	//����ͼ��
	QIcon icon_play, icon_play_focus;
	icon_play.addFile(QStringLiteral(":/IcePlayer/Resources/���Ű�ť-������.png"), QSize(), QIcon::Normal, QIcon::Off);
	icon_play_focus.addFile(QStringLiteral(":/IcePlayer/Resources/���Ű�ť-������2.png"), QSize(), QIcon::Normal, QIcon::Off);
	playButton->SetButtonIcons(icon_play, icon_play_focus);

	playButton->setIconSize(QSize(60,60));
	playButton->setFlat(true);
	playButton->setFocusPolicy(Qt::NoFocus);
	playButton->setStyleSheet("QPushButton{background-color:rgba(255,255,255,0);border-style:solid;border-width:0px;border-color:rgba(255,255,255,0);}");

	QIcon icon_pause, icon_pause_focus;
	icon_pause.addFile(QStringLiteral(":/IcePlayer/Resources/��ͣ��ť-������.png"), QSize(), QIcon::Normal, QIcon::Off);
	icon_pause_focus.addFile(QStringLiteral(":/IcePlayer/Resources/��ͣ��ť-������2.png"), QSize(), QIcon::Normal, QIcon::Off);
	pauseButton->SetButtonIcons(icon_pause, icon_pause_focus);
	
	pauseButton->setIconSize(QSize(60,60));
	pauseButton->setFlat(true);
	pauseButton->setFocusPolicy(Qt::NoFocus);
	pauseButton->setStyleSheet("QPushButton{background-color:rgba(255,255,255,0);border-style:solid;border-width:0px;border-color:rgba(255,255,255,0);}");


	QIcon icon_lyric, icon_lyric_focus;
	icon_lyric.addFile(QStringLiteral(":/IcePlayer/Resources/��ʰ�ť.png"), QSize(), QIcon::Normal, QIcon::Off);
	icon_lyric_focus.addFile(QStringLiteral(":/IcePlayer/Resources/��ʰ�ť2.png"), QSize(), QIcon::Normal, QIcon::Off);
	lyricButton->SetButtonIcons(icon_lyric, icon_lyric_focus);

	lyricButton->setIconSize(QSize(25,25));
	lyricButton->setFlat(true);
	lyricButton->setFocusPolicy(Qt::NoFocus);
	lyricButton->setStyleSheet("QPushButton{background-color:rgba(255,255,255,0);border-style:solid;border-width:0px;border-color:rgba(255,255,255,0);}");

	QIcon icon_exit, icon_exit_focus;
	icon_exit.addFile(QStringLiteral(":/IcePlayer/Resources/�رհ�ť.png"), QSize(), QIcon::Normal, QIcon::Off);
	icon_exit_focus.addFile(QStringLiteral(":/IcePlayer/Resources/�رհ�ť2.png"), QSize(), QIcon::Normal, QIcon::Off);
	exitButton->SetButtonIcons(icon_exit, icon_exit_focus);
	//exitButton->setIcon(icon_exit);
	exitButton->setIconSize(QSize(20,20));
	exitButton->setFlat(true);
	exitButton->setFocusPolicy(Qt::NoFocus);
	exitButton->setStyleSheet("QPushButton{background-color:rgba(255,255,255,0);border-style:solid;border-width:0px;border-color:rgba(255,255,255,0);}");

	QIcon icon_min, icon_min_focus;
	icon_min.addFile(QStringLiteral(":/IcePlayer/Resources/����ģʽ��ť.png"), QSize(), QIcon::Normal, QIcon::Off);
	icon_min_focus.addFile(QStringLiteral(":/IcePlayer/Resources/����ģʽ��ť2.png"), QSize(), QIcon::Normal, QIcon::Off);
	minButton->SetButtonIcons(icon_min, icon_min_focus);

	minButton->setIconSize(QSize(20,20));
	minButton->setFlat(true);
	minButton->setFocusPolicy(Qt::NoFocus);
	minButton->setStyleSheet("QPushButton{background-color:rgba(255,255,255,0);border-style:solid;border-width:0px;border-color:rgba(255,255,255,0);}");

	QIcon icon_mmin, icon_mmin_focus;
	icon_mmin.addFile(QStringLiteral(":/IcePlayer/Resources/��С��ť.png"), QSize(), QIcon::Normal, QIcon::Off);
	icon_mmin_focus.addFile(QStringLiteral(":/IcePlayer/Resources/��С��ť2.png"), QSize(), QIcon::Normal, QIcon::Off);
	mminButton->SetButtonIcons(icon_mmin, icon_mmin_focus);

	mminButton->setIconSize(QSize(20,20));
	mminButton->setFlat(true);
	mminButton->setFocusPolicy(Qt::NoFocus);
	mminButton->setStyleSheet("QPushButton{background-color:rgba(255,255,255,0);border-style:solid;border-width:0px;border-color:rgba(255,255,255,0);}");

	QIcon icon_logo, icon_logo_focus;
	icon_logo.addFile(QStringLiteral(":/IcePlayer/Resources/���Ͻ�logo��ť.png"), QSize(), QIcon::Normal, QIcon::Off);
	icon_logo_focus.addFile(QStringLiteral(":/IcePlayer/Resources/���Ͻ�logo��ť2.png"), QSize(), QIcon::Normal, QIcon::Off);
	logoButton->SetButtonIcons(icon_logo, icon_logo_focus);

	logoButton->setIconSize(QSize(120, 48));
	logoButton->setFlat(true);
	logoButton->setFocusPolicy(Qt::NoFocus);
	logoButton->setStyleSheet("QPushButton{background-color:rgba(255,255,255,0);border-style:solid;border-width:0px;border-color:rgba(255,255,255,0);}");

	QIcon icon_next, icon_next_focus;
	icon_next.addFile(QStringLiteral(":/IcePlayer/Resources/��һ��.png"), QSize(), QIcon::Normal, QIcon::Off);
	icon_next_focus.addFile(QStringLiteral(":/IcePlayer/Resources/��һ��2.png"), QSize(), QIcon::Normal, QIcon::Off);
	nextButton->SetButtonIcons(icon_next, icon_next_focus);

	nextButton->setIconSize(QSize(40,40));
	nextButton->setFlat(true);
	nextButton->setFocusPolicy(Qt::NoFocus);
	nextButton->setStyleSheet("QPushButton{background-color:rgba(255,255,255,0);border-style:solid;border-width:0px;border-color:rgba(255,255,255,0);}");

	QIcon icon_last, icon_last_focus;
	icon_last.addFile(QStringLiteral(":/IcePlayer/Resources/��һ��.png"), QSize(), QIcon::Normal, QIcon::Off);
	icon_last_focus.addFile(QStringLiteral(":/IcePlayer/Resources/��һ��2.png"), QSize(), QIcon::Normal, QIcon::Off);
	lastButton->SetButtonIcons(icon_last, icon_last_focus);

	lastButton->setIconSize(QSize(40,40));
	lastButton->setFlat(true);
	lastButton->setFocusPolicy(Qt::NoFocus);
	lastButton->setStyleSheet("QPushButton{background-color:rgba(255,255,255,0);border-style:solid;border-width:0px;border-color:rgba(255,255,255,0);}");

	QIcon icon_mode, icon_mode_focus;
	icon_mode.addFile(QStringLiteral(":/IcePlayer/Resources/����ģʽ.png"), QSize(), QIcon::Normal, QIcon::Off);
	icon_mode_focus.addFile(QStringLiteral(":/IcePlayer/Resources/����ģʽ2.png"), QSize(), QIcon::Normal, QIcon::Off);
	modeButton->SetButtonIcons(icon_mode, icon_mode_focus);

	modeButton->setIconSize(QSize(25,25));
	modeButton->setFlat(true);
	modeButton->setFocusPolicy(Qt::NoFocus);
	modeButton->setStyleSheet("QPushButton{background-color:rgba(255,255,255,0);border-style:solid;border-width:0px;border-color:rgba(255,255,255,0);}");
	
	QIcon icon_add, icon_add_focus;
	icon_add.addFile(QStringLiteral(":/IcePlayer/Resources/��Ӹ���.png"), QSize(), QIcon::Normal, QIcon::Off);
	icon_add_focus.addFile(QStringLiteral(":/IcePlayer/Resources/��Ӹ���2.png"), QSize(), QIcon::Normal, QIcon::Off);
	addButton->SetButtonIcons(icon_add, icon_add_focus);

	addButton->setIconSize(QSize(25,25));
	addButton->setFlat(true);
	addButton->setFocusPolicy(Qt::NoFocus);
	addButton->setStyleSheet("QPushButton{background-color:rgba(255,255,255,0);border-style:solid;border-width:0px;border-color:rgba(255,255,255,0);}");

	//ȥ��������
	this->setWindowFlags(Qt::FramelessWindowHint);
	
	//��͸��
	this->setWindowOpacity(0.9);

	//�����϶�
	QWidgetResizeHandler *movewin = new QWidgetResizeHandler(this);
	movewin->setMovingEnabled(true);

	//�̶���С
	this->setFixedSize(400, 600);
	
	//����������Ϣ������ɫ
	QColor fontcolor(30, 90, 150);

	QPalette font_palette;
	font_palette.setColor(QPalette::WindowText, fontcolor);
	nameLabel->setPalette(font_palette);
	musicianLabel->setPalette(font_palette);
	albumLabel->setPalette(font_palette);
	timeLabel->setPalette(font_palette);

	QFont namefont("΢���ź�", 13);
	nameLabel->setFont(namefont);

	QFont othersfont("΢���ź�",9);
	musicianLabel->setFont(othersfont);
	albumLabel->setFont(othersfont);
	timeLabel->setFont(othersfont);


	volSlider = new QSlider(volButton);
	volSlider->setObjectName(QStringLiteral("volSlider"));

	volSlider->setStyleSheet("QSlider::groove:horizontal{border:0px;height:4px;}"  
		"QSlider::sub-page:horizontal{background:#0096ff;}"  
		"QSlider::add-page:horizontal{background:lightgray;} "
		"QSlider::handle:horizontal{background:white;width:10px;border:#51b5fb 10px;border-radius:5px;margin:-3px 0px -3px 0px;}");

	volSlider->setGeometry(QRect(40, 150, 290, 20));
	volSlider->setOrientation(Qt::Horizontal);

	setAttribute(Qt::WA_TranslucentBackground);

	if (playlistTable->columnCount() < 1)
		playlistTable->setColumnCount(2);
	QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
	playlistTable->setHorizontalHeaderItem(0, __qtablewidgetitem);

	//�����п�
	playlistTable->setColumnWidth(0,245);
	playlistTable->setColumnWidth(1,100);

	//��ֹ�༭��Ԫ������
	playlistTable->setEditTriggers(QAbstractItemView::NoEditTriggers); 
	//ѡ��һ��
	playlistTable->setSelectionBehavior(QAbstractItemView::SelectRows);
	//ȥ��ˮƽ������
	playlistTable->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	//��ֱ�����������ƶ�
	playlistTable->setVerticalScrollMode(QAbstractItemView::ScrollPerItem);
	//ȥ��ˮƽ��ͷ
	playlistTable->horizontalHeader()->setFixedHeight(0);
	//ȥ����ֱ��ͷ
	playlistTable->verticalHeader()->setFixedWidth(0);
	//�����ޱ߿�
	playlistTable->setFrameShape(QFrame::NoFrame); 
	//���ò���ʾ������
	playlistTable->setShowGrid(false); 
	//�����Ҽ��˵�
	playlistTable->setContextMenuPolicy(Qt::CustomContextMenu);
	//���ñ�����ɫ
	QPalette pal;
	pal.setBrush(QPalette::Base, QBrush(QColor(255, 255, 255)));
	playlistTable->setPalette(pal);
	//������ֱ��������ʽ
	playlistTable->setStyleSheet("QScrollBar{background:transparent; width: 10px;}"
		"QScrollBar::handle{background:lightgray; border:2px solid transparent; border-radius:5px;}"
		"QScrollBar::handle:hover{background:gray;}"
		"QScrollBar::sub-line{background:transparent;}"
		"QScrollBar::add-line{background:transparent;}");
	//ʹ�϶�������Ч
	playlistTable->setAcceptDrops(true);
	setAcceptDrops(true);

}

void IcePlayer::dropEvent(QDropEvent *event)
{
	QList<QUrl> urls = event->mimeData()->urls();
	if(urls.isEmpty())
		return;

	QStringList dropfilename;
	dropfilename.append(urls.first().toLocalFile());

	if(!dropfilename.isEmpty()){
		addList(dropfilename);
	}
}

void IcePlayer::dragEnterEvent(QDragEnterEvent *event)
{
	if(event->mimeData()->hasFormat("text/uri-list"))
		event->acceptProposedAction();
}

void IcePlayer::paintEvent(QPaintEvent *event)
{
	QPainter p(this);
	p.drawPixmap(0, 0 , QPixmap(":/IcePlayer/Resources/background.png"));
}

void IcePlayer::on_playlistTableWidget_cellDoubleClicked(int row, int ) 
{
	QFile file(playList.at(row));
	if (!file.open(QIODevice::ReadOnly)) {
		playlistTable->item(row,1)->setText(QString::fromLocal8Bit("ʧЧ"));
		return ;
	}
	file.close();
	medialist->setCurrentIndex(row);

	mediaplayer->play();
}

void IcePlayer::removeCurrentIndex()   
{
	playList.removeAt(currentIndex);

	playlistTable->removeRow(currentIndex);

	medialist->removeMedia(currentIndex);

}

void IcePlayer::openSongs()  
{
	QStringList fileList = QFileDialog::getOpenFileNames(this,QString::fromLocal8Bit("�������"),QString(),QString("MP3 (*.mp3)"));
	addList(fileList);
}

void IcePlayer::openDir()     
{
	QString dirPath = QFileDialog::getExistingDirectory(this,QString::fromLocal8Bit("ѡ��Ŀ¼"));
	if(dirPath.size() == 0)
		return;
	//if(QFileDialog::Reject) return;
	QDir dir(dirPath);
	QStringList fileList = dir.entryList(QStringList()<<"*.mp3",QDir::Files);
	for(int i = 0;i < fileList.size();i ++)
		fileList[i] = dir.absoluteFilePath(fileList.at(i));
	addList(fileList);
}

void IcePlayer::clearList()  
{
	mediaplayer->stop();
	playList.clear();
	medialist->clear();
	while(playlistTable->rowCount())
		playlistTable->removeRow(0);

	timeLabel->setText(tr("00:00"));
	nameLabel->setText(tr("Name " ));
	nameLabel->stop();
	musicianLabel->setText(tr("Musician" ));
	albumLabel->setText(tr("Album" ));
}

void IcePlayer::addList(QStringList list)
{

	foreach(QString fileName,list)
	{
		if(playList.contains(fileName))
			continue;
		playList.append(fileName);

		medialist->addMedia(QUrl::fromLocalFile(fileName));
		playlistTable->insertRow(playlistTable->rowCount());

		for(int i = 0;i < playlistTable->columnCount();i ++){
			QTableWidgetItem *item  = new QTableWidgetItem;
			item->setFont(QFont("΢���ź�", 10));
			playlistTable->setItem(playlistTable->rowCount() - 1,i,item);
			
		}
		playlistTable->item(playlistTable->rowCount() - 1,0)->setText(QFileInfo(fileName).baseName());

	}
}

void IcePlayer::on_playlistTable_customContextMenuRequested(const QPoint &pos)  
{
	if(playlistTable->itemAt(pos))
	{
		currentIndex = playlistTable->rowAt(pos.y());
		contextmenu_more->exec(QCursor::pos());
	}
	else contextmenu_less->exec(QCursor::pos());
}

void IcePlayer::setPosition(int position)
{
	if (qAbs(mediaplayer->position() - position) > 99)
		mediaplayer->setPosition(position);
}

void IcePlayer::updatePosition(qint64 position)
{
	playSlider->setValue(position);

	//QTime duration(0, position / 60000, qRound((position % 60000) / 1000.0));
	//timeLabel->setText(duration.toString(tr("mm:ss")));
	
	qint64 total_time_value = mediaplayer->duration();//��ȡ����Ƶ�ļ�����ʱ����������λΪ����
	//��3�������ֱ������ʱ���֣��룻60000����Ϊ1���ӣ����Է��ӵڶ����������ȳ�6000,��3��������ֱ�ӳ�1s
	QTime total_time(0, (total_time_value/60000)%60, (total_time_value/1000)%60);
	QTime current_time(0, (position/60000)%60, (position/1000)%60);//��������time���������˵�ǰ��ʱ��
	timeLabel->setText(current_time.toString(tr("mm:ss")));

	// ��ȡ����ʱ���Ӧ�ĸ��
	if(!lrc_map.isEmpty()) {
		// ��ȡ��ǰʱ���ڸ���е�ǰ������ʱ���
		qint64 previous = 0;
		qint64 later = 0;
		//keys()��������lrc_map�б�
		foreach (qint64 value1, lrc_map.keys()) {
			if (position >= value1) {
				previous = value1;
			} else {
				later = value1;
				break;
			}
		}
		// �ﵽ���һ��,��later����Ϊ������ʱ���ֵ
		if (later == 0)
			later = total_time_value;

		// ��ȡ��ǰʱ������Ӧ�ĸ������
		QString current_lrc = lrc_map.value(previous);

		// ������µ�һ�и�ʣ���ô���¿�ʼ��ʾ�������
		QString tmp = lrc->text();
		if(current_lrc != tmp) {
			lrc->setText(current_lrc);
			tmp = lrc->text();
			//top_label->setText(current_lrc);
			qint64 interval_time = later - previous;
			lrc->start_lrc_mask(interval_time);
		}
	}
}

void IcePlayer::updateDuration(qint64 duration)
{
	playSlider->setRange(0, duration);
	playSlider->setEnabled(duration > 0);
	playSlider->setPageStep(duration / 10);
}

void IcePlayer::updateMetaData()
{
	//if(preIndex == -1) preIndex = medialist->currentIndex();
	//else preIndex = currentIndex;
	
	currentIndex = medialist->currentIndex();
	playingfile = playList.at(currentIndex);
/*

	for(int i = 0;i < playlistTable->rowCount();++i){
		playlistTable->item(i, 0)->setBackgroundColor(QColor(255, 255, 255));
		playlistTable->item(i, 1)->setBackgroundColor(QColor(255, 255, 255));
	}

	playlistTable->item(currentIndex, 0)->setBackgroundColor(QColor(180, 200, 220));
	playlistTable->item(currentIndex, 1)->setBackgroundColor(QColor(180, 200, 220));
*/


	QString name = mediaplayer->metaData(QMediaMetaData::Title).toString();
	if(name.size() > 35)
		nameLabel->SetText(name, textspeed);
	else{
		nameLabel->stop();
		nameLabel->setText(name);
	}

	musicianLabel->setText(mediaplayer->metaData(QMediaMetaData::Author).toString());
	albumLabel->setText(mediaplayer->metaData(QMediaMetaData::AlbumTitle).toString());

	if(nameLabel->text() == "")
		nameLabel->setText(QString::fromLocal8Bit("δ֪����"));
	if(musicianLabel->text() == "")
		musicianLabel->setText(QString::fromLocal8Bit("δ֪���ּ�"));
	if(albumLabel->text() == "")
		albumLabel->setText(QString::fromLocal8Bit("δ֪ר��"));
	
	if(playlistTable->rowCount() == 0){
		timeLabel->setText(tr("00:00"));
		nameLabel->setText(tr("Name " ));
		musicianLabel->setText(tr("Musician" ));
		albumLabel->setText(tr("Album" ));
	}
	
	/*
	QString tmpfilename = playingfile;
	std::string filename((const char*)tmpfilename.toLocal8Bit()); //QString to string (Chinese)
	getmp3pic(filename);
	
	QPixmap mp3pic("test.jpeg");
	picLabel->setPixmap(mp3pic);*/

	resolve_lrc(playingfile);
}

void IcePlayer::updateState(QMediaPlayer::State state)
{
	if (state == QMediaPlayer::PlayingState) {
		playButton->setVisible(false);
		pauseButton->setVisible(true);
		
	} else {
		playButton->setVisible(true);
		pauseButton->setVisible(false);
	}

	if (state != QMediaPlayer::StoppedState){
		playingfile = playList.at(medialist->currentIndex());
		resolve_lrc(playingfile);
	} 
}

void IcePlayer::playButton_clicked()
{
	if(mediaplayer->state() == QMediaPlayer::PausedState){
		mediaplayer->play();
	}
}

void IcePlayer::pauseButton_clicked()
{
	
	if(mediaplayer->state() == QMediaPlayer::PlayingState){
		mediaplayer->pause();
	}
}

void IcePlayer::nextButton_clicked()
{
	medialist->setPlaybackMode(QMediaPlaylist::Loop);

	if(medialist->currentIndex() != medialist->nextIndex())
		medialist->setCurrentIndex(medialist->nextIndex());
	else if(medialist->currentIndex() != medialist->previousIndex())
		medialist->setCurrentIndex(medialist->previousIndex());

	mediaplayer->play();

	setmode();
}

void IcePlayer::lastButton_clicked()
{
	
	medialist->setPlaybackMode(QMediaPlaylist::Loop);

	if(medialist->currentIndex() != medialist->previousIndex())
		medialist->setCurrentIndex(medialist->previousIndex());
	else
		medialist->setCurrentIndex(medialist->nextIndex());
	mediaplayer->play();

	setmode();
}

void IcePlayer::lyricButton_clicked()
{
	if(lrc->isHidden()) lrc->show();
	else lrc->hide();
}

void IcePlayer::logoButton_clicked()
{
	aboutwindow->show();
}

void IcePlayer::modeButton_clicked()
{
	modemenu->exec(QCursor::pos());
}

void IcePlayer::minButton_clicked()
{
	mini_win->show();
	mini_win->initmode(volButton->volume());
	this->hide();
}

void IcePlayer::mminButton_clicked()
{
	
}

void IcePlayer::setmode()
{
	if(mode0->isChecked()) {
		mode = playmode::single;
		medialist->setPlaybackMode(QMediaPlaylist::CurrentItemOnce);
	}
	else if(mode1->isChecked()) {
		mode = playmode::listcircle;
		medialist->setPlaybackMode(QMediaPlaylist::Loop);
	}
	else if(mode2->isChecked()) {
		mode = playmode::singlecircle;
		medialist->setPlaybackMode(QMediaPlaylist::CurrentItemInLoop);
	}
	else if(mode3->isChecked()) {
		mode = playmode::random;
		medialist->setPlaybackMode(QMediaPlaylist::Random);
	}
}

void IcePlayer::setmodefrommini(int m)
{
	switch(m){
	case 0:
		mode = playmode::single;
		medialist->setPlaybackMode(QMediaPlaylist::CurrentItemOnce);
		mode0->setChecked(true);
		break;
	case 1:
		mode = playmode::listcircle;
		medialist->setPlaybackMode(QMediaPlaylist::Loop);
		mode1->setChecked(true);
		break;
	case 2:
		mode = playmode::singlecircle;
		medialist->setPlaybackMode(QMediaPlaylist::CurrentItemInLoop);
		mode2->setChecked(true);
		break;
	case 3:
		mode = playmode::random;
		medialist->setPlaybackMode(QMediaPlaylist::Random);
		mode3->setChecked(true);
		break;

	}
}

void IcePlayer::resolve_lrc(const QString &source_file_name)
{
	lrc_map.clear();
	if(source_file_name.isEmpty())
		return;
	QString file_name = source_file_name;
	QString lrc_file_name = file_name.remove(file_name.right(3)) + "lrc";//����Ƶ�ļ��ĺ�׺�ĳ�lrc��׺

	// �򿪸���ļ�
	QFile file(lrc_file_name);
	if (!file.open(QIODevice::ReadOnly)) {
		lrc->setText(mediaplayer->metaData(QMediaMetaData::Title).toString());
		return ;
	}

	QString all_text = QString(file.readAll());
	file.close();
	// ����ʰ��зֽ�Ϊ����б�
	QStringList lines = all_text.split("\n");

	//�����ʱ���ǩ�ĸ�ʽ[00:05.54]
	//������ʽd{2}��ʾƥ��2������
	QRegExp rx("\\[\\d{2}:\\d{2}\\.\\d{2}\\]");
	foreach(QString oneline, lines) {
		QString temp = oneline;
		temp.replace(rx, "");//�ÿ��ַ����滻������ʽ����ƥ��ĵط�,�����ͻ���˸���ı�
		// Ȼ�����λ�ȡ��ǰ���е�����ʱ���ǩ�����ֱ������ı�����QMap��
		//indexIn()Ϊ���ص�һ��ƥ���λ�ã��������Ϊ-1�����ʾû��ƥ��ɹ�
		//���������pos����Ӧ�ö�Ӧ���Ǹ���ļ�
		int pos = rx.indexIn(oneline, 0);
		while (pos != -1) { //��ʾƥ��ɹ�
			QString cap = rx.cap(0);//���ص�0�����ʽƥ�������
			// ��ʱ���ǩת��Ϊʱ����ֵ���Ժ���Ϊ��λ
			QRegExp regexp;
			regexp.setPattern("\\d{2}(?=:)");
			regexp.indexIn(cap);
			int minute = regexp.cap(0).toInt();
			regexp.setPattern("\\d{2}(?=\\.)");
			regexp.indexIn(cap);
			int second = regexp.cap(0).toInt();
			regexp.setPattern("\\d{2}(?=\\])");
			regexp.indexIn(cap);
			int millisecond = regexp.cap(0).toInt();
			qint64 totalTime = minute * 60000 + second * 1000 + millisecond * 10;
			// ���뵽lrc_map��
			lrc_map.insert(totalTime, temp);
			pos += rx.matchedLength();
			pos = rx.indexIn(oneline, pos);//ƥ��ȫ��
		}
	}
	// ���lrc_mapΪ��
	if (lrc_map.isEmpty()) {
		lrc->setText(mediaplayer->metaData(QMediaMetaData::Title).toString());
		return;
	}
}

void IcePlayer::gobacktothis()
{
	this->show();
}

void IcePlayer::setvolfrommini(int a)
{
	volButton->setVolume(a);
}

void IcePlayer::readList()
{
	QFile text(IceDir + "/IceList.ilst");
	QStringList inList;

	if(text.open(QIODevice::ReadOnly)){
		QDataStream in(&text);
		in >> inList;
		addList(inList);
		text.close();
	}

}

void IcePlayer::writeList()
{
	QFile text(IceDir + "/IceList.ilst");
	QStringList outList = playList;

	text.open(QIODevice::WriteOnly);

	QDataStream out(&text);

	out.setVersion(QDataStream::Qt_5_2);
	out << outList ;
	text.close();
}

void IcePlayer::opensong(const QString& filePath)
{
	QStringList name;
	name.append(filePath);

	addList(name);

	if (playList.contains(filePath)){
		medialist->setCurrentIndex(playList.indexOf(filePath));
		mediaplayer->play();
	}

}
