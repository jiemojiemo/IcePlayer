#ifndef ICEPLAYER_H
#define ICEPLAYER_H

#define textspeed 500 //���ֹ����ٶ�

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

//ö�ٲ���ģʽ
enum playmode{single, listcircle, singlecircle, random};

class IcePlayer : public QWidget
{
	//����mini������Ϊ�����������Ԫ�࣬�Ӷ����Ե������������˽�к�����˽�ж���
	friend class miniwindow;

	Q_OBJECT

public:
	explicit IcePlayer(QWidget *parent = 0); 
	~IcePlayer();
	void opensong(const QString& filePath);

public slots:
	void gobacktothis(); //��mini���ڷ��������� 

	void setmodefrommini(int);//����mini���ڴ����Ĳ������ò���ģʽ 
	void setvolfrommini(int);//����mini���ڴ����Ĳ����������� 
	
private slots:
	void openDir();	//����Ŀ¼�ṹ 
	void openSongs();	//���mp3��ʽ���ļ� 
	void removeCurrentIndex(); //ɾ��ָ���еĸ��� 
	void clearList(); //��ղ����б� 
	void on_playlistTable_customContextMenuRequested(const QPoint &pos); //�����Ҽ��˵� 
	
	void on_playlistTableWidget_cellDoubleClicked(int row, int); //˫�����Ÿ��� 
	void updateMetaData(); //����������Ϣ 
	void updateDuration(qint64 duration); //���½����� 
	void updateState(QMediaPlayer::State state); //���²���״̬ 
	void setPosition(int position); //���²��Ž��� 

	void updatePosition(qint64 position); //���¸�ʡ�ʱ����ʾ 

	//���ø���ť�ĵ����¼����������ť
	void playButton_clicked(); 
	void lyricButton_clicked(); 
	void logoButton_clicked(); 
	void modeButton_clicked(); 
	void pauseButton_clicked(); 
	void minButton_clicked();
	void mminButton_clicked();
	//��/��һ���л�
	void nextButton_clicked();
	void lastButton_clicked();

	void setmode();//���ò���ģʽ

	void resolve_lrc(const QString &source_file_name);//����LRC��� 

	void readList();//�����ʱ��ȡ�����б��ļ� 
	void writeList();//����ر�ʱ�����в����б�д���б��ļ� 

private:
	void initUI();	//��ʼ�������� 
	void initPlayer();	//��ʼ������ģ�� 
	void initConnections();	//��ʼ���ź�-������ 
	void initWindows();//��ʼ������ 
	void initActions();//��ʼ���˵��� 
	void addList(QStringList list);//��ӵ������б� 
	void paintEvent(QPaintEvent *event); 

	//��ק���
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
