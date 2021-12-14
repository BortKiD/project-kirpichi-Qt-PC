#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>

const int field=5;
const int board_row=20;
const int board_col=10;
const int block_size=30;
const QSize btnSize = QSize(300, 50);

enum Direction
{
    UP,
    DOWN,
    LEFT,
    RIGHT,
    SPACE
};


class border
{

public:
    int up_bound;
    int down_bound;
    int left_bound;
    int right_bound;

};


class piece_pos
{

public:
    int px;
    int py;

};

class RenderArea : public QWidget
{
    Q_OBJECT

public:
    explicit RenderArea(QWidget *parent = nullptr);

    void initGame();
    void startGame();
    void gameOver();

    void keyPressEvent(QKeyEvent *event) override;
    void paintEvent(QPaintEvent *event) override;
    void timerEvent(QTimerEvent *event) override;

    void resetBlock();
    void blockMove(Direction dir);
    void blockRotate(int block[4][4]);
    void createBlock(int block[4][4], int block_id);
    void getBorder(int block[4][4], border &border);
    void convertStable(int x, int y);
    bool isCollide(int x, int y, Direction dir);

public slots:
    void start();
    void pause();

private:
    int board[board_row][board_col];
    int next_block[4][4];
    int current_piece[4][4];
    piece_pos block_pos;
    border current_border;
    bool isStable;
    int score;
    int game_timer;
    int paint_timer;
    int speed_ms;
    int refresh_ms;

};

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:

    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();



private:
    Ui::MainWindow *ui;
    RenderArea *renderArea;
    QPushButton *startButton;
    QPushButton *quitButton;
    QPushButton *pauseButton;


};
#endif // MAINWINDOW_H
