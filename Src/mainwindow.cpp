#include "mainwindow.h"

#define LIGHT_BLUE  QColor("#6262FF")
#define LIGHT_BLACK QColor("#333333")
#define LIGHT_GREEN QColor("#90EE90")

void MainWindow::draw_background(void)
{
    m_painter.begin(this);
    // Setup game area
    m_painter.setPen(QPen(Qt::white, 8));
    m_painter.drawRect(BS_POSX, BS_POSY, BS_RECT_SIDE*BS_RECTS_NUM_W, \
        BS_RECT_SIDE*BS_RECTS_NUM_H);

    // Draw checkerboard
    QRectF rect = QRectF(BS_POSX, BS_POSY, BS_RECT_SIDE, BS_RECT_SIDE);
    QColor col = LIGHT_BLUE;
    m_painter.setPen(Qt::black);
    for (int x = 0; x < BS_RECTS_NUM_W; ++x) {
        col = (x % 2) ? Qt::black : LIGHT_BLUE;
        for (int y = 0; y < BS_RECTS_NUM_H; ++y) {
            m_painter.setBrush(col);
            m_painter.drawRect(rect);
            rect.translate(0, BS_RECT_SIDE);
            col = (col == LIGHT_BLUE) ? Qt::black : LIGHT_BLUE;
        }
        rect.moveTo(BS_POSX+(x+1)*BS_RECT_SIDE, BS_POSY);
    }
    m_painter.end();
}

void MainWindow::draw_one_tiny_rect(QColor color, QPoint pos)
{
    m_painter.begin(this);
    m_painter.setPen(Qt::white);
    m_painter.setBrush(color);
    m_painter.drawRect(pos.x(), pos.y(), BS_RECT_SIDE, BS_RECT_SIDE);
    m_painter.end();
}

void MainWindow::draw_snake_and_food(void)
{
    m_painter.begin(this);
    m_painter.setRenderHint(QPainter::Antialiasing);
    m_painter.setPen(Qt::white);
    m_painter.setBrush(Qt::red);
    // draw snake head
    QRectF rect = QRectF(m_snake.first().x(), m_snake.first().y(), BS_RECT_SIDE, BS_RECT_SIDE);
    m_painter.drawEllipse(rect);
    // draw snake body
    for (auto it = m_snake.begin()+1; it != m_snake.end(); ++it) {
        rect.moveTo(it->x(), it->y());
        m_painter.drawRect(rect);
    }
    m_painter.end();

    // draw snake food
    draw_one_tiny_rect(LIGHT_GREEN, m_food);
}

void MainWindow::snake_move(MOVING_DIRECTION dir)
{
    QPoint s_head_pos = m_snake.first();

    // move body
    if (MOVING_DIRECTION_RIGHT == dir) {
        m_snake.push_front(QPoint((s_head_pos.x() + BS_RECT_SIDE), s_head_pos.y()));
    } else if (MOVING_DIRECTION_LEFT == dir) {
        m_snake.push_front(QPoint((s_head_pos.x() - BS_RECT_SIDE), s_head_pos.y()));
    } else if (MOVING_DIRECTION_DOWN == dir) {
        m_snake.push_front(QPoint(s_head_pos.x(), (s_head_pos.y() + BS_RECT_SIDE)));
    } else {
        m_snake.push_front(QPoint(s_head_pos.x(), (s_head_pos.y() - BS_RECT_SIDE)));
    }
    // erase last item of snake body
    m_snake.pop_back();

    // eat food
    if (m_snake.first() == m_food) {
        m_snake.push_back(QPoint((m_snake.last().x() - BS_RECT_SIDE), m_snake.last().y()));
        generate_food();
        qDebug() << "hit food";
    }

    // snake hit window bound or it self
    if ((m_snake.first().x() >= BS_POSX+BS_RECTS_W) ||
        (m_snake.first().x() < BS_POSX) ||
        (m_snake.first().y() >= BS_POSY+BS_RECTS_H) ||
        (m_snake.first().y() < BS_POSY)) {
        qDebug() << "Game over";
        m_status = GAME_OVER;
    }
}

void MainWindow::generate_food(void)
{
    int rand_x = 0, rand_y = 0;

    do {
        rand_x = BS_POSX+QRandomGenerator::global()->bounded(BS_RECTS_NUM_W-1)*BS_RECT_SIDE;
        rand_y = BS_POSY+QRandomGenerator::global()->bounded(BS_RECTS_NUM_H-1)*BS_RECT_SIDE;
        m_food = QPoint(rand_x, rand_y);
    } while (m_snake.indexOf(m_food) >= 0);
}

void MainWindow::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    draw_background();
    if (m_status == GAME_RUNING) {
        draw_snake_and_food();
        snake_move(m_direction);
    }
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if (GAME_OVER == m_status && Qt::Key_S == event->key()) {
        qDebug() << "key S pressed and game start";
        m_status = GAME_BEGIN;
        run();
    } else {
        switch (event->key()) {
        case Qt::Key_Up:
            if (MOVING_DIRECTION_UP != m_direction && MOVING_DIRECTION_DOWN != m_direction) {
                m_direction = MOVING_DIRECTION_UP;
            }
            break;

        case Qt::Key_Down:
            if (MOVING_DIRECTION_UP != m_direction && MOVING_DIRECTION_DOWN != m_direction) {
                m_direction = MOVING_DIRECTION_DOWN;
            }
            break;

        case Qt::Key_Left:
            if (MOVING_DIRECTION_LEFT != m_direction && MOVING_DIRECTION_RIGHT != m_direction) {
                m_direction = MOVING_DIRECTION_LEFT;
            }
            break;

        case Qt::Key_Right:
            if (MOVING_DIRECTION_LEFT != m_direction && MOVING_DIRECTION_RIGHT != m_direction) {
                m_direction = MOVING_DIRECTION_RIGHT;
            }
            break;

        case Qt::Key_Space:
            if (m_status == GAME_RUNING) {
                pause();
            } else if (m_status == GAME_PAUSE){
                run();
            }
            break;
        }
    }
}

void MainWindow::setup(void)
{
    // set fixed window postion
    setGeometry(WINDOW_POSX, WINDOW_POSY, WINDOW_W, WINDOW_H);
    setWindowTitle("SNAKE");
    setStyleSheet("background-color: #333333;");

    m_direction = MOVING_DIRECTION_RIGHT;
    generate_food();
    for (qint32 i = 0; i < 3; ++i)
        m_snake.push_back(QPoint((BS_POSX+(BS_RECTS_NUM_W/2-i)*BS_RECT_SIDE),
                                      (BS_POSY+BS_RECTS_NUM_H/2*BS_RECT_SIDE)));
    qDebug() << m_snake;

    m_timer = new QTimer(this);
    connect(m_timer, &QTimer::timeout, this, [=] { update(); });

    m_status = GAME_OVER;
}

void MainWindow::pause(void)
{
    if (GAME_RUNING == m_status) {
        m_timer->stop();
        m_status = GAME_PAUSE;
    }
}

void MainWindow::run(void)
{
    if (GAME_BEGIN == m_status || GAME_PAUSE == m_status) {
        m_timer->start(200);
        m_status = GAME_RUNING;
    }
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setup();
    run();
}

MainWindow::~MainWindow()
{
}
