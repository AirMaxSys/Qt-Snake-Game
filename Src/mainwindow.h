#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QPaintEvent>
#include <QPainter>
#include <QRectF>
#include <QVector>
#include <QDebug>
#include <QRandomGenerator>

enum Window_size
{
    WINDOW_POSX = 400,
    WINDOW_POSY = 100,
    WINDOW_W = 1100,
    WINDOW_H = 850,
};

enum Back_screen_para
{
    BS_POSX = 100,
    BS_POSY = 200,
    BS_RECTS_NUM_W = 30,
    BS_RECTS_NUM_H = 20,
    BS_RECT_SIDE = 30,
    BS_RECTS_W = BS_RECTS_NUM_W * BS_RECT_SIDE,
    BS_RECTS_H = BS_RECTS_NUM_H * BS_RECT_SIDE,
};

enum MOVING_DIRECTION
{
    MOVING_DIRECTION_UP,
    MOVING_DIRECTION_DOWN,
    MOVING_DIRECTION_LEFT,
    MOVING_DIRECTION_RIGHT
};

enum GAME_STATUS
{
    GAME_BEGIN,
    GAME_PAUSE,
    GAME_RUNNING,
    GAME_OVER,
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    QPainter m_painter;
    QVector<QRect> m_snake;
    QRect m_food;
    QRect m_gaming_area;
    QTimer *m_timer;
    MOVING_DIRECTION m_direction;
    GAME_STATUS m_status;

    void setup(void);
    void run(void);
    void pause(void);
    void draw_background(void);
    void draw_snake_and_food(void);
    void generate_food(void);
    void generate_snake(void);
    void snake_move();

protected:
    void paintEvent(QPaintEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
};
#endif // MAINWINDOW_H
