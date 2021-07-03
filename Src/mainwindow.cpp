#include "mainwindow.h"

#include <QDebug>
#include <QTimer>
#include <QRandomGenerator>

#define LIGHT_BLUE  QColor("#6262FF")
#define LIGHT_BLACK QColor("#333333")
#define LIGHT_GREEN QColor("#90EE90")

void MainWindow::setup_backscr(void)
{
    bool flag = true;

    painter.begin(this);

    // Draw back screen frame
    painter.setPen(QPen(Qt::white, 8));
    painter.drawRect(BS_POSX, BS_POSY,
                     BS_RECT_SIDE*BS_RECTS_NUM_W,
                     BS_RECT_SIDE*BS_RECTS_NUM_H);

    // Fill with small rects
    for (int x = 0; x < BS_RECTS_NUM_W; ++x) {
        flag = !flag;
        for (int y = 0; y < BS_RECTS_NUM_H; ++y) {
            QRectF rect = QRectF(BS_POSX+BS_RECT_SIDE*x,
                                 BS_POSY+BS_RECT_SIDE*y,
                                 BS_RECT_SIDE,
                                 BS_RECT_SIDE);
            if (flag) {
                painter.setPen(QPen(LIGHT_BLUE));
                painter.setBrush(LIGHT_BLUE);
            } else {
                painter.setPen(QPen(LIGHT_BLACK));
                painter.setBrush(LIGHT_BLACK);
            }
            flag = !flag;
            painter.drawRect(rect);
        }
    }
    painter.end();
}

void MainWindow::draw_one_tiny_rect(QColor color, QPoint pos)
{
    painter.begin(this);
    painter.setPen(Qt::white);
    painter.setBrush(color);
    painter.drawRect(pos.x(), pos.y(), BS_RECT_SIDE, BS_RECT_SIDE);
    painter.end();
}

void MainWindow::draw_snake_and_food(void)
{
    painter.begin(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(Qt::white);
    painter.setBrush(Qt::red);
    // draw snake head
    painter.drawEllipse(QPoint(lp_snake_pos.first().x()-1 + BS_RECT_SIDE/2,
                               lp_snake_pos.first().y()-1 + BS_RECT_SIDE/2),
                        BS_RECT_SIDE/2-2, BS_RECT_SIDE/2-2);
    // draw snake body
    for (QList<QPoint>::iterator it = lp_snake_pos.begin()+1; it != lp_snake_pos.end(); ++it) {
        painter.drawRect(it->x(), it->y(), BS_RECT_SIDE, BS_RECT_SIDE);
    }
    painter.end();

    // draw snake food
    draw_one_tiny_rect(LIGHT_GREEN, food_pos);
}

void MainWindow::snake_move(Snake_dir dir)
{
    QPoint s_head_pos = lp_snake_pos.first();

    // move body
    if (SNAKE_DIR_RIGHT == dir) {
        lp_snake_pos.push_front(QPoint((s_head_pos.x() + BS_RECT_SIDE), s_head_pos.y()));
    } else if (SNAKE_DIR_LEFT == dir) {
        lp_snake_pos.push_front(QPoint((s_head_pos.x() - BS_RECT_SIDE), s_head_pos.y()));
    } else if (SNAKE_DIR_DOWN == dir) {
        lp_snake_pos.push_front(QPoint(s_head_pos.x(), (s_head_pos.y() + BS_RECT_SIDE)));
    } else {
        lp_snake_pos.push_front(QPoint(s_head_pos.x(), (s_head_pos.y() - BS_RECT_SIDE)));
    }
    // eras last item of snake body
    lp_snake_pos.pop_back();

    // eat food
    if (lp_snake_pos.first() == food_pos) {
        lp_snake_pos.push_back(QPoint((lp_snake_pos.last().x() - BS_RECT_SIDE), lp_snake_pos.last().y()));
        qDebug() << "hit food";
    }

    // snake hit window bound or it self
    if ((lp_snake_pos.first().x() >= BS_POSX+BS_RECTS_W) ||
        (lp_snake_pos.first().x() < BS_POSX) ||
        (lp_snake_pos.first().y() >= BS_POSY+BS_RECTS_H) ||
        (lp_snake_pos.first().y() < BS_POSY)) {
        qDebug() << "Game over";
        game_stu = GAME_OVER;
    }
}

void MainWindow::make_snake_food(void)
{
    // Generate random coordinate for snake food
    // TODO:food not in the postion of snake
    if (game_stu == GAME_BEGIN || lp_snake_pos.indexOf(food_pos) != -1) {
        food_pos.setX(BS_POSX+QRandomGenerator::global()->bounded(BS_RECTS_NUM_W-1)*BS_RECT_SIDE);
        food_pos.setY(BS_POSY+QRandomGenerator::global()->bounded(BS_RECTS_NUM_H-1)*BS_RECT_SIDE);
        game_stu = GAME_RUNING;
    }
}

void MainWindow::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    setup_backscr();
    draw_snake_and_food();
    make_snake_food();
    snake_move(s_dir);
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()) {
    case Qt::Key_Up:
        if (SNAKE_DIR_UP != s_dir && SNAKE_DIR_DOWN != s_dir) {
            s_dir = SNAKE_DIR_UP;
        }
        break;
    case Qt::Key_Down:
        if (SNAKE_DIR_UP != s_dir && SNAKE_DIR_DOWN != s_dir) {
            s_dir = SNAKE_DIR_DOWN;
        }
        break;
    case Qt::Key_Left:
        if (SNAKE_DIR_LEFT != s_dir && SNAKE_DIR_RIGHT != s_dir) {
            s_dir = SNAKE_DIR_LEFT;
        }
        break;
    case Qt::Key_Right:
        if (SNAKE_DIR_LEFT != s_dir && SNAKE_DIR_RIGHT != s_dir) {
            s_dir = SNAKE_DIR_RIGHT;
        }
        break;
    case Qt::Key_Escape:
        break;
    }
    update();
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    // set fixed window postion
    setGeometry(WINDOW_POSX, WINDOW_POSY, WINDOW_W, WINDOW_H);
    setWindowTitle("SNAKE");
    setStyleSheet("background-color: #333333;");

    game_stu = GAME_BEGIN;
    food_pos = QPoint(BS_POSX, BS_POSY);
    s_dir = SNAKE_DIR_RIGHT;
    for (qint32 i = 0; i < 3; ++i)
        lp_snake_pos.push_back(QPoint((BS_POSX+(BS_RECTS_NUM_W/2-i)*BS_RECT_SIDE),
                                      (BS_POSY+BS_RECTS_NUM_H/2*BS_RECT_SIDE)));
    qDebug() << lp_snake_pos;
#if 1
    p_move_snake_timer = new QTimer(this);
    connect(p_move_snake_timer, &QTimer::timeout, this, [=]() {
        update();
    });
    p_move_snake_timer->start(100);
#else
    QTimer::singleShot(100, this, [=]() {update();});
#endif

}

MainWindow::~MainWindow()
{
}
