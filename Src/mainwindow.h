#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QPaintEvent>
#include <QPainter>
#include <QRectF>
#include <QList>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    enum Window_size {
        WINDOW_POSX=400, WINDOW_POSY=100,
        WINDOW_W=1100, WINDOW_H=850,
    };
    enum Back_screen_para {
        BS_POSX=100, BS_POSY=200,
        BS_RECTS_NUM_W=30, BS_RECTS_NUM_H=20,
        BS_RECT_SIDE=30,
        BS_RECTS_W = BS_RECTS_NUM_W * BS_RECT_SIDE,
        BS_RECTS_H = BS_RECTS_NUM_H * BS_RECT_SIDE,
    };
    enum Snake_dir {
        SNAKE_DIR_UP, SNAKE_DIR_DOWN,
        SNAKE_DIR_LEFT, SNAKE_DIR_RIGHT
    } s_dir;
    enum Game_stu {
        GAME_BEGIN, GAME_PAUSE, GAME_RUNING, GAME_OVER
    } game_stu;

    QPainter painter;
    QList<QPoint> lp_snake_pos;
    QPoint food_pos;
    QTimer *p_move_snake_timer;

private:
    void draw_one_tiny_rect(QColor color, QPoint pos);
    void setup_backscr(void);
    void draw_snake_and_food(void);
    void make_snake_food(void);
    void snake_move(Snake_dir dir);

protected:
    void paintEvent(QPaintEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
};
#endif // MAINWINDOW_H
