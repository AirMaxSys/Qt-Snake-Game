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
    draw_one_tiny_rect(QColor(Qt::red), snake_pos);
    draw_one_tiny_rect(LIGHT_GREEN, food_pos);
}

void MainWindow::snake_move(Snake_dir dir)
{
    if (SNAKE_DIR_RIGHT == dir) {
        snake_pos.setX(snake_pos.x() + BS_RECT_SIDE);
        if (snake_pos.x() >= BS_POSX+BS_RECTS_W)
            snake_pos.setX(BS_POSX);
    } else if (SNAKE_DIR_LEFT == dir) {
        snake_pos.setX(snake_pos.x() - BS_RECT_SIDE);
        if (snake_pos.x() < BS_POSX)
            snake_pos.setX(BS_POSX+BS_RECTS_W-BS_RECT_SIDE);
    } else if (SNAKE_DIR_DOWN == dir) {
        snake_pos.setY(snake_pos.y() + BS_RECT_SIDE);
        if (snake_pos.y() >= BS_POSY+BS_RECTS_H)
            snake_pos.setY(BS_POSY);
    } else {
        snake_pos.setY(snake_pos.y() - BS_RECT_SIDE);
        if (snake_pos.y() < BS_POSY)
            snake_pos.setY(BS_POSY+BS_RECTS_H-BS_RECT_SIDE);
    }
}

void MainWindow::make_snake_food(void)
{
    // Generate random coordinate for snake food
    // TODO:food not in the postion of snake
    if (init_flag || food_pos == snake_pos) {
        food_pos.setX(BS_POSX+QRandomGenerator::global()->bounded(BS_RECTS_NUM_W-1)*BS_RECT_SIDE);
        food_pos.setY(BS_POSY+QRandomGenerator::global()->bounded(BS_RECTS_NUM_H-1)*BS_RECT_SIDE);
        init_flag = false;
    }
}

void MainWindow::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    setup_backscr();
    snake_move(s_dir);
    make_snake_food();
    draw_snake_and_food();
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
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    // set fixed window postion
    setGeometry(WINDOW_POSX, WINDOW_POSY, WINDOW_W, WINDOW_H);
    setWindowTitle("SNAKE");
    setStyleSheet("background-color: #333333;");

    init_flag = true;

    food_pos = QPoint(BS_POSX, BS_POSY);
    snake_pos.setX(BS_POSX+(BS_RECTS_NUM_W/2-1)*BS_RECT_SIDE);
    snake_pos.setY(BS_POSY+(BS_RECTS_NUM_H/2-1)*BS_RECT_SIDE);
    s_dir = SNAKE_DIR_RIGHT;

    p_move_snake_timer = new QTimer(this);
    connect(p_move_snake_timer, &QTimer::timeout, this, [=]() {
        update();
    });
    p_move_snake_timer->start(100);
}

MainWindow::~MainWindow()
{
}
