#ifndef ICEPLAYER_H
#define ICEPLAYER_H

#define textspeed 500 //文字滚动速度

#include <QtWidgets/QWidget>
#include <QString>
//#include <QPixmap>

#include <QVector>
#include <QMap>
#include <QMediaPlayer>

class QSlider;
class QLabel;
class QTableWidget;
class QFrame;
class QMediaPlaylist;
class QMediaPlayer;
class QMenu;
class MyLrc;
class about;
class iceButton;
class QAction;
class QActionGroup;
class IceVolButton;
class miniwindow;
class IceLabel;

//枚举播放模式
enum playmode{single, listcircle, singlecircle, random};

class IcePlayer : public QWidget
{
	//设置mini窗口类为主窗口类的友元类，从而可以调用主窗口类的私有函数和私有对象
	friend class miniwindow;

	Q_OBJECT

public:
	explicit IcePlayer(QWidget *parent = 0); 
	~IcePlayer();
	void opensong(const QString& filePath);

public slots:
	void gobacktothis(); //从mini窗口返回主窗口 

	void setmodefrommini(int);//接受mini窗口传来的参数设置播放模式 
	void setvolfrommini(int);//接受mini窗口传来的参数设置音量 
	
private slots:
	void openDir();	//访问目录结构 
	void openSongs();	//添加mp3格式的文件 
	void removeCurrentIndex(); //删除指定行的歌曲 
	void clearList(); //清空播放列表 
	void on_playlistTable_customContextMenuRequested(const QPoint &pos); //请求右键菜单 
	
	void on_playlistTableWidget_cellDoubleClicked(int row, int); //双击播放歌曲 
	void updateMetaData(); //更新音乐信息 
	void updateDuration(qint64 duration); //更新进度条 
	void updateState(QMediaPlayer::State state); //更新播放状态 
	void setPosition(int position); //更新播放进度 

	void updatePosition(qint64 position); //更新歌词、时间显示 

	//设置各按钮的单击事件，激活各按钮
	void playButton_clicked(); 
	void lyricButton_clicked(); 
	void logoButton_clicked(); 
	void modeButton_clicked(); 
	void pauseButton_clicked(); 
	void minButton_clicked();
	void mminButton_clicked();
	//上/下一曲切换
	void nextButton_clicked();
	void lastButton_clicked();

	void setmode();//设置播放模式

	void resolve_lrc(const QString &source_file_name);//解析LRC歌词 

	void readList();//程序打开时读取播放列表文件 
	void writeList();//程序关闭时把现有播放列表写入列表文件 

private:
	void initUI();	//初始化主界面 
	void initPlayer();	//初始化播放模块 
	void initConnections();	//初始化信号-槽连接 
	void initWindows();//初始化窗口 
	void initActions();//初始化菜单项 
	void addList(QStringList list);//添加到播放列表 
	void paintEvent(QPaintEvent *event); 

	//拖拽添加
	void dragEnterEvent(QDragEnterEvent *event); 
	void dropEvent(QDropEvent *event); 

	MyLrc *lrc;
	QMap<qint64, QString> lrc_map;

	QStringList playList;
	QString playingfile;
	QString IceDir;
	
	int preIndex;
	int currentIndex;
	int mode;

	//int songscount;
	//int playindex;
	//qint64 songduration;


	iceButton *minButton;
	iceButton *exitButton;
	iceButton *addButton;
	iceButton *lyricButton;
	iceButton *lastButton;
	iceButton *nextButton;
	iceButton *playButton;
	iceButton *pauseButton;
	iceButton *modeButton;
	iceButton *mminButton;
	//iceButton *volButton;
	iceButton *logoButton;

	IceLabel *nameLabel;
	QLabel *musicianLabel;
	QLabel *albumLabel;
	QLabel *timeLabel;
	QLabel *picLabel;

	QSlider *playSlider;
	QSlider *volSlider;

	QTableWidget *playlistTable;

	QMediaPlayer *mediaplayer;
	QMediaPlaylist *medialist;

	QMenu *contextmenu_less;
	QMenu *contextmenu_more;
	QMenu *modemenu;

	about *aboutwindow;

	QActionGroup *modegroup;

	QAction *mode0;
	QAction *mode1;
	QAction *mode2;
	QAction *mode3;
	QAction *addsongs;
	QAction *addfilediv;
	QAction *removethis;
	QAction *removeall;

	IceVolButton *volButton;
	
	miniwindow *mini_win;

	//QPixmap pic;
	
};

#endif 
