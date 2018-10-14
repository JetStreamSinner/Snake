#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPaintEvent>
#include <QPainter>
#include <QPair>
#include <QKeyEvent>
#include <QStatusBar>
#include <QTimer>
#include <QLabel>



enum directions {Up, Down, Left, Right};
enum cellType {FOOD, EMPTY, SNAKE};


class MainWindow : public QMainWindow
{
    Q_OBJECT
    private:
        int matrixOrder;      
        QPair<int, int> foodCoord;
        QVector<QPair<int, int>> body;     
        directions currentDirection;    
        QTimer timer;
        
        QImage foodTexture;
        QImage waterTexture;
        QImage grassTexture;
        
        QStatusBar *statusBar;
        QLabel *infoLabel;
        
        int gameSpeed;
        int pointCount;
        
        virtual void paintEvent(QPaintEvent *event=nullptr);
        virtual void keyPressEvent(QKeyEvent *event=nullptr);
        
        void drawGrid(QPainter& painter);
        void drawFood(QPainter& painter);
        void drawSnake(QPainter& painter);
        void drawField(QPainter& painter);
        
        void parseLoseEvent();
        void restartGame();
        
        void initStatusBar();
        void initField();
        void initResourses();
        void setFoodCoord();
        void setHeadCoord();
        
        void setCurrentDirection(directions direction);
    private slots:
    
        void nextIteration();
        void checkField();
        
    public:
        explicit MainWindow(QMainWindow *parent=nullptr);
         
};



#endif 

