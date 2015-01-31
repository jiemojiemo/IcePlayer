#ifndef MINI_H
#define MINI_H

#include <QWidget>

class iceButton;
class QMenu;
class QAction;
class QWidgetAction;
class QActionGroup;
class IcePlayer;
class QSlider;


class miniwindow : public QWidget
{
	Q_OBJECT
public:
	miniwindow(QWidget *parent = 0);
	~miniwindow();

	void setparentwin(IcePlayer *parent);
	void initmode(int vol);

private slots:
	void goback();

	void nextsong();
	void lastsong();
	void play();
	void pause();
	void setmode();
	void exitice();
	void lyricaction();
	
	void updatevol(int);
private:
	void paintEvent(QPaintEvent *event);
	void contextMenuEvent(QContextMenuEvent *event);
	void initActions();
	
	iceButton *playButton;
	iceButton *pauseButton;
	iceButton *mininextButton;

	QMenu *contextmenu;
	QActionGroup *modegroup;
	
	QAction *mode0;
	QAction *mode1;
	QAction *mode2;
	QAction *mode3;
	QAction *mainwin;
	QAction *exit;
	QAction *next;
	QAction *last;
	QAction *lyric;
	QWidgetAction *setvol;

	QSlider *volslider;

	IcePlayer *parentwin;

};

#endif