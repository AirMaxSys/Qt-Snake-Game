#include "mainwindow.h"

#define LIGHT_BLUE  QColor("#6262FF")
#define LIGHT_BLACK QColor("#333333")
#define LIGHT_GREEN QColor("#90EE90")

void MainWindow::draw_background(void)
{
    m_painter.begin(this);
    // draw gaming area boxe
    m_painter.setPen(QPen(Qt::white, 8));
    m_painter.drawRect(m_gaming_area);
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

void MainWindow::draw_snake_and_food(void)
{
    // Setup pen
    m_painter.begin(this);
    m_painter.setRenderHint(QPainter::Antialiasing);
    m_painter.setPen(Qt::white);
    m_painter.setBrush(Qt::red);

    // draw snake
    m_painter.drawEllipse(m_snake[0]);
    m_painter.drawRects(&m_snake[1], m_snake.count()-1);

    // draw food
    m_painter.setBrush(LIGHT_GREEN);
    m_painter.drawRect(m_food);

    m_painter.end();
}

void MainWindow::snake_move(void)
{
    QRect snake_head = m_snake[0];

    // move body
    if (MOVING_DIRECTION_RIGHT == m_direction) {
        snake_head.translate(BS_RECT_SIDE, 0);
    } else if (MOVING_DIRECTION_LEFT == m_direction) {
        snake_head.translate(-BS_RECT_SIDE, 0);
    } else if (MOVING_DIRECTION_DOWN == m_direction) {
        snake_head.translate(0, BS_RECT_SIDE);
    } else {
        snake_head.translate(0, -BS_RECT_SIDE);
    }
    m_snake.push_front(snake_head);
    m_snake.pop_back();

    // eat food
    if (m_snake.first() == m_food) {
        auto penultimate = m_snake[m_snake.count()-2];
        auto last = m_snake.last();
        if (penultimate.x() - last.x() > 0) {
            last.translate(BS_RECT_SIDE, 0);
        } else if (penultimate.x() - last.x() < 0) {
            last.translate(-BS_RECT_SIDE, 0);
        } else if (penultimate.y() - last.y() > 0) {
            last.translate(0, BS_RECT_SIDE);
        } else if (penultimate.y() - last.y() < 0) {
            last.translate(0, -BS_RECT_SIDE);
        }
        m_snake.push_back(last);
        generate_food();
        qDebug() << "ate food";
    }

    // check if snake in gaming area 
    if (!m_gaming_area.contains(m_snake[0])) {
        qDebug() << "Game over - out of box";
        m_status = GAME_OVER;
    }

    // check if snake body is overlap
    if (m_snake.mid(1).contains(m_snake[0])) {
        qDebug() << "Game over - body overlayp";
        m_status = GAME_OVER;
    }
}

void MainWindow::generate_food(void)
{
    int rand_x = 0, rand_y = 0;

    do {
        rand_x = BS_POSX+QRandomGenerator::global()->bounded(BS_RECTS_NUM_W-1)*BS_RECT_SIDE;
        rand_y = BS_POSY+QRandomGenerator::global()->bounded(BS_RECTS_NUM_H-1)*BS_RECT_SIDE;
        m_food = QRect(rand_x, rand_y, BS_RECT_SIDE, BS_RECT_SIDE);
    } while (m_snake.indexOf(m_food) >= 0);
}

void MainWindow::generate_snake(void)
{
    int rand_x = 0, rand_y = 0;
    QRect head;

    do {
        rand_x = BS_POSX+QRandomGenerator::global()->bounded(BS_RECTS_NUM_W-1)*BS_RECT_SIDE;
        rand_y = BS_POSY+QRandomGenerator::global()->bounded(BS_RECTS_NUM_H-1)*BS_RECT_SIDE;
        head = QRect(rand_x, rand_y, BS_RECT_SIDE, BS_RECT_SIDE);
    } while (head == m_food);

    m_direction = (MOVING_DIRECTION)QRandomGenerator::global()->bounded(4);
    if (MOVING_DIRECTION_RIGHT == m_direction) {
        m_snake.append(head);
        head.translate(-BS_RECT_SIDE, 0);
        m_snake.append(head);
        head.translate(-BS_RECT_SIDE, 0);
        m_snake.append(head);
    } else if (MOVING_DIRECTION_LEFT == m_direction) {
        m_snake.append(head);
        head.translate(BS_RECT_SIDE, 0);
        m_snake.append(head);
        head.translate(BS_RECT_SIDE, 0);
        m_snake.append(head);
    } else if (MOVING_DIRECTION_DOWN == m_direction) {
        m_snake.append(head);
        head.translate(0, -BS_RECT_SIDE);
        m_snake.append(head);
        head.translate(0, -BS_RECT_SIDE);
        m_snake.append(head);
    } else {
        m_snake.append(head);
        head.translate(0, BS_RECT_SIDE);
        m_snake.append(head);
        head.translate(0, BS_RECT_SIDE);
        m_snake.append(head);
    }
    qDebug() << m_direction << m_snake;
}

void MainWindow::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    draw_background();
    if (m_status == GAME_RUNNING) {
        snake_move();
        if (m_status != GAME_OVER)
            draw_snake_and_food();
    }
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if (GAME_OVER == m_status && Qt::Key_S == event->key()) {
        qDebug() << "key S pressed and game start";
        m_status = GAME_BEGIN;
        run();
    } else if (GAME_PAUSE == m_status && Qt::Key_Space == event->key()) {
        // Game pause
        run();
    } else if (GAME_RUNNING == m_status && Qt::Key_Space == event->key()) {
        pause();
    } else {
        // Snake moving direction control
        switch (event->key()) {
        case Qt::Key_Up:
            if (MOVING_DIRECTION_DOWN != m_direction)
                m_direction = MOVING_DIRECTION_UP;
            break;

        case Qt::Key_Down:
            if (MOVING_DIRECTION_UP != m_direction)
                m_direction = MOVING_DIRECTION_DOWN;
            break;

        case Qt::Key_Left:
            if (MOVING_DIRECTION_RIGHT != m_direction)
                m_direction = MOVING_DIRECTION_LEFT;
            break;

        case Qt::Key_Right:
            if (MOVING_DIRECTION_LEFT != m_direction)
                m_direction = MOVING_DIRECTION_RIGHT;
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

    // setup gaming area
    m_gaming_area = QRect(BS_POSX, BS_POSY, BS_RECTS_W, BS_RECTS_H);

    // generate food and snake random
    generate_food();
    generate_snake();

    m_timer = new QTimer(this);
    connect(m_timer, &QTimer::timeout, this, [=] { update(); });

    // Press KEY_S to start game
    m_status = GAME_OVER;
}

void MainWindow::pause(void)
{
    if (GAME_RUNNING == m_status) {
        m_timer->stop();
        m_status = GAME_PAUSE;
    }
}

void MainWindow::run(void)
{
    if (GAME_BEGIN == m_status || GAME_PAUSE == m_status) {
        m_timer->start(200);
        m_status = GAME_RUNNING;
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
