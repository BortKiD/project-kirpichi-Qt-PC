#include <time.h>
#include <QMessageBox>
#include <QDebug>
#include <QPainter>
#include <QKeyEvent>
#include <QGridLayout>
#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    startButton = new QPushButton(tr("&Start"));
    startButton->setFixedSize(btnSize);
    startButton->setFocusPolicy(Qt::NoFocus);
    quitButton = new QPushButton(tr("&Quit"));
    quitButton->setFixedSize(btnSize);
    quitButton->setFocusPolicy(Qt::NoFocus);
    pauseButton = new QPushButton(tr("&Pause"));
    pauseButton->setFixedSize(btnSize);
    pauseButton->setFocusPolicy(Qt::NoFocus);
    renderArea = new RenderArea;

    connect(startButton, &QPushButton::clicked, renderArea, &RenderArea::start);
    connect(quitButton , &QPushButton::clicked, qApp, &QCoreApplication::quit);
    connect(pauseButton, &QPushButton::clicked, renderArea, &RenderArea::pause);

    renderArea->initGame();

    QGridLayout *layout = new QGridLayout;
    layout->addWidget(startButton, 2, 1);
    layout->addWidget(quitButton, 5, 1);
    layout->addWidget(pauseButton, 4, 1);
    layout->addWidget(renderArea, 1, 2, -1, -1);
    centralWidget()->setLayout(layout);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void RenderArea::pause(){}

void RenderArea::start(){}

RenderArea::RenderArea(QWidget *parent) : QWidget(parent)
{

    setBackgroundRole(QPalette::Base);
    RenderArea::setEnabled(true);
    setFocus();
    setFocusPolicy(Qt::StrongFocus);

}

int S[4][4]=
{
    {0,1,0,0},
    {0,1,1,0},
    {0,0,1,0},
    {0,0,0,0}
};

int Z[4][4]=
{
    {0,0,1,0},
    {0,1,1,0},
    {0,1,0,0},
    {0,0,0,0}
};

int I[4][4]=
{
    {0,0,1,0},
    {0,0,1,0},
    {0,0,1,0},
    {0,0,1,0}
};

int O[4][4]=
{
    {0,0,0,0},
    {0,1,1,0},
    {0,1,1,0},
    {0,0,0,0}
};

int J[4][4]=
{
    {0,0,1,0},
    {0,0,1,0},
    {0,1,1,0},
    {0,0,0,0}
};

int L[4][4]=
{
    {0,1,0,0},
    {0,1,0,0},
    {0,1,1,0},
    {0,0,0,0}
};

int T[4][4]=
{
    {0,0,0,0},
    {0,1,1,1},
    {0,0,1,0},
    {0,0,0,0}
};


inline void block_cppy(int dblock[4][4],int sblock[4][4])
{
    for(int i=0;i<4;i++)
        for(int j=0;j<4;j++)
            dblock[i][j]=sblock[i][j];
}


void RenderArea::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);

    painter.setBrush(QBrush(Qt::white, Qt::SolidPattern));
    painter.drawRect(field, field, board_col * block_size, board_row * block_size);


    painter.setBrush(QBrush(Qt::blue, Qt::SolidPattern));
    for(int i=0;i<4;i++)
        for(int j=0;j<4;j++)
            if(next_block[i][j]==1)
                painter.drawRect(field*3+board_col*block_size+j*block_size,field+i*block_size,block_size,block_size);

    painter.setPen(Qt::black);
    painter.setFont(QFont("Arial",14));
    painter.drawText(QRect(field*3+board_col*block_size,field*2+4*block_size,block_size*4,block_size*4),Qt::AlignCenter,"score: "+QString::number(score));


    for(int i=0;i<board_row;i++)
        for(int j=0;j<board_col;j++)
        {
            if(board[i][j]==1)
            {
                painter.setBrush(QBrush(Qt::red,Qt::SolidPattern));
                painter.drawRect(j*block_size+field,i*block_size+field,block_size,block_size);
            }

            else if(board[i][j]==2)
            {
                painter.setBrush(QBrush(Qt::green,Qt::SolidPattern));
                painter.drawRect(j*block_size+field,i*block_size+field,block_size,block_size);
            }
        }
}

void RenderArea::timerEvent(QTimerEvent *event)
{

    if(event->timerId()==game_timer)
        blockMove(DOWN);

    if(event->timerId()==paint_timer)
        update();
}

void RenderArea::keyPressEvent(QKeyEvent *event)
{
    switch(event->key())
    {
        case Qt::Key_Up:
            blockMove(UP);
            break;
        case Qt::Key_Down:
            blockMove(DOWN);
            break;
        case Qt::Key_Left:
            blockMove(LEFT);
            break;
        case Qt::Key_Right:
            blockMove(RIGHT);
            break;
        case Qt::Key_Space:
            blockMove(SPACE);
            break;
        default:
            break;
    }
}

void RenderArea::createBlock(int block[4][4],int block_id)
{
    switch (block_id)
    {
    case 0:
        block_cppy(block, O);
        break;
    case 1:
        block_cppy(block, L);
        break;
    case 2:
        block_cppy(block, J);
        break;
    case 3:
        block_cppy(block, S);
        break;
    case 4:
        block_cppy(block, Z);
        break;
    case 5:
        block_cppy(block, T);
        break;
    case 6:
        block_cppy(block, I);
        break;
    default:
        break;
    }
}

void RenderArea::getBorder(int block[4][4], border &border)
{

    for(int i = 0 ; i < 4; i++)
        for(int j = 0; j < 4; j++)
            if( block[i][j] == 1 )
            {
                border.down_bound = i;
                break;
            }
    for(int i=3;i>=0;i--)
        for(int j=0;j<4;j++)
            if(block[i][j]==1)
            {
                border.up_bound = i;
                break;
            }
    for(int j=0;j<4;j++)
        for(int i=0;i<4;i++)
            if(block[i][j]==1)
            {
                border.right_bound = j;
                break;
            }
    for(int j=3;j>=0;j--)
        for(int i=0;i<4;i++)
            if(block[i][j]==1)
            {
                border.left_bound = j;
                break;
            }

}

void RenderArea::initGame()
{
    for(int i=0;i<board_row;i++)
        for(int j=0;j<board_col;j++)
            board[i][j]=0;

    speed_ms = 800;
    refresh_ms = 30;

    srand(time(0));

    score=0;

    startGame();
}

void RenderArea::resetBlock()
{

    block_cppy(current_piece, next_block);
    getBorder(current_piece, current_border);

    int block_id = rand() % 7;
    createBlock(next_block, block_id);

    piece_pos start_point;
    start_point.px = board_col / 2 - 2;
    start_point.py = 0;
    block_pos = start_point;
}

void RenderArea::startGame()
{
    game_timer = startTimer(speed_ms);
    paint_timer = startTimer(refresh_ms);

    int block_id = rand() % 7;
    createBlock(next_block, block_id);
    resetBlock();
}

void RenderArea::gameOver()
{

    killTimer(game_timer);
    killTimer(paint_timer);
    QMessageBox::information(this, "Failure", "Game Over!");

}

void RenderArea::blockRotate(int block[4][4])
{
    int temp_block[4][4];
    for(int i=0;i<4;i++)
        for(int j=0;j<4;j++)
            temp_block[3-j][i] = block[i][j];

    for(int i = 0; i < 4; i++)
        for(int j = 0; j < 4; j++)
            block[i][j] = temp_block[i][j];
}

void RenderArea::convertStable(int px,int py)
{
    for(int i = current_border.up_bound; i <= current_border.down_bound; i++)
        for(int j = current_border.left_bound; j <= current_border.right_bound; j++)
            if(current_piece[i][j] == 1)
                board[py+i][px+j] = 2;
}

bool RenderArea::isCollide(int px,int py,Direction dir)
{

    int temp_block[4][4];
    block_cppy(temp_block, current_piece);
    border temp_border;
    getBorder(temp_block, temp_border);

    switch(dir)
    {
    case UP:
        blockRotate(temp_block);
        getBorder(temp_block, temp_border);
        break;
    case DOWN:
        py += 1;
        break;
    case LEFT:
        px -= 1;
        break;
    case RIGHT:
        px += 1;
        break;
    default:
        break;
    }
    for(int i = temp_border.up_bound; i <= temp_border.down_bound; i++)
        for(int j = temp_border.left_bound; j <= temp_border.right_bound; j++)
            if( (board[py + i][px + j] == 2 && temp_block[i][j] == 1)
               || ( px + temp_border.left_bound < 0 || px+temp_border.right_bound>board_col-1 ) )
                return true;
    return false;
}

void RenderArea::blockMove(Direction dir)
{
    switch (dir) {
    case UP:
        if( isCollide(block_pos.px, block_pos.py, UP) )
            break;

        blockRotate(current_piece);

        for( int i = 0; i < 4; i++ )
            for( int j = 0; j < 4; j++ )
                board[block_pos.py + i][block_pos.px + j] = current_piece[i][j];

        getBorder(current_piece, current_border);
        break;
    case DOWN:

        if(block_pos.py+current_border.down_bound==board_row-1)
        {
            convertStable(block_pos.px,block_pos.py);
            resetBlock();
            break;
        }

        if(isCollide(block_pos.px,block_pos.py,DOWN))
        {
            convertStable(block_pos.px,block_pos.py);
            resetBlock();
            break;
        }

        for(int j=current_border.left_bound;j<=current_border.right_bound;j++)
            board[block_pos.py][block_pos.px+j]=0;

        block_pos.py+=1;

        for(int i=0;i<4;i++)
            for(int j=current_border.left_bound;j<=current_border.right_bound;j++)
                if(block_pos.py+i<=board_row-1&&board[block_pos.py+i][block_pos.px+j]!=2)
                    board[block_pos.py+i][block_pos.px+j]=current_piece[i][j];
        break;
    case LEFT:

        if(block_pos.px+current_border.left_bound==0||isCollide(block_pos.px,block_pos.py,LEFT))
            break;

        for(int i=current_border.up_bound;i<=current_border.down_bound;i++)
            board[block_pos.py+i][block_pos.px+3]=0;
        block_pos.px-=1;

        for(int i=current_border.up_bound;i<=current_border.down_bound;i++)
            for(int j=0;j<4;j++)
                if(block_pos.px+j>=0&&board[block_pos.py+i][block_pos.px+j]!=2)
                    board[block_pos.py+i][block_pos.px+j]=current_piece[i][j];
        break;
    case RIGHT:
        if( (block_pos.px+current_border.right_bound==board_col-1||isCollide(block_pos.px,block_pos.py,RIGHT)) )
            break;

        for(int i=current_border.up_bound;i<=current_border.down_bound;i++)
            board[block_pos.py+i][block_pos.px]=0;
        block_pos.px+=1;

        for(int i=current_border.up_bound;i<=current_border.down_bound;i++)
            for(int j=0;j<4;j++)
                if(block_pos.px+j<=board_col-1 && board[block_pos.py+i][block_pos.px+j]!=2)
                    board[block_pos.py+i][block_pos.px+j]=current_piece[i][j];

        break;
    case SPACE:

        while(block_pos.py+current_border.down_bound<board_row-1&&!isCollide(block_pos.px,block_pos.py,DOWN))
        {

            for(int j=current_border.left_bound;j<=current_border.right_bound;j++)
                board[block_pos.py][block_pos.px+j]=0;

            block_pos.py+=1;

            for(int i=0;i<4;i++)
                for(int j=current_border.left_bound;j<=current_border.right_bound;j++)
                    if(block_pos.py+i<=board_row-1&&board[block_pos.py+i][block_pos.px+j]!=2)
                        board[block_pos.py+i][block_pos.px+j]=current_piece[i][j];
        }
        convertStable(block_pos.px, block_pos.py);
        resetBlock();
        break;
    default:
        break;
    }

    int i = board_row - 1;
    int line_count = 0;
    while(i >= 1)
    {
        bool is_line_full = true;
        for(int j=0; j < board_col; j++)
            if(board[i][j] == 0)
            {
                is_line_full = false;
                i--;
                break;
            }
        if(is_line_full)
        {
            for(int k = i; k >= 1; k--)
                for(int j = 0; j < board_col; j++)
                    board[k][j] = board[k-1][j];
            line_count++;
        }
    }
    score += line_count * 10;

    for(int j = 0; j < board_col; j++)
        if(board[0][j] == 2)
            gameOver();
}
