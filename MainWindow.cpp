#include <QPoint>
#include <QTime>
#include <QMessageBox>
#include <QDebug>

#include "MainWindow.h"


MainWindow::MainWindow(QMainWindow* parent) : QMainWindow(parent), matrixOrder(20),
        currentDirection(Up), gameSpeed(0), pointCount(0)
{
    setFixedSize(600, 600);
    
    qsrand(QTime::currentTime().msec());
    
    initField();
    initResourses();
    initStatusBar();
    

    connect(&timer, SIGNAL(timeout()), this, SLOT(nextIteration()));
    connect(&timer, SIGNAL(timeout()), this, SLOT(checkField()));

    timer.start(500);

}

void MainWindow::initStatusBar()
{
    statusBar = new QStatusBar;
    infoLabel = new QLabel(QString("Points count: %0 \t Speed: %1").arg(pointCount).arg(gameSpeed));
    
    statusBar->addWidget(infoLabel);
    
    setStatusBar(statusBar);
}
void MainWindow::initResourses()
{
    foodTexture = QImage("foodTexture.png");
    waterTexture = QImage("waterTexture.png");
    grassTexture = QImage("grassTexture.jpeg");
    
    foodTexture.isNull() ? qDebug() << " Failure to load \"foodTexture.png\" " : 
                           qDebug() << "Successful load \"foodTexture.png\" ";
    
    waterTexture.isNull() ? qDebug() << " Failure to load \"waterTexture.png\" " : 
                            qDebug() << "Successful load \"waterTexture.png\" ";
    
    grassTexture.isNull() ? qDebug() << " Failure to load \"grassTexture.jpeg\" " : 
                            qDebug() << "Successful load \"grassTexture.jpeg\" ";
}

void MainWindow::initField()
{
    setFoodCoord();
    setHeadCoord();

}
void MainWindow::setFoodCoord()
{
    foodCoord.first = 1 + qrand() % (matrixOrder - 2);
    foodCoord.second = 1 + qrand() % (matrixOrder - 2);
    for(int i = 0; i < body.size(); i++)
        if(body[i] == foodCoord)
            setFoodCoord();
}

void MainWindow::setHeadCoord()
{
    body.append(QPair<int, int>(matrixOrder / 2, matrixOrder / 2));
}

void MainWindow::setCurrentDirection(directions direction)
{   
    if(body.size() > 1)
    {
        if(currentDirection == Up && direction == Down)
            return;
        if(currentDirection == Down && direction == Up)
            return;
        if(currentDirection == Right && direction == Left)
            return;
        if(currentDirection == Left && direction == Right)
            return;
    }
    currentDirection = direction;

}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    switch(event->key())
    {
        case Qt::Key_Up:
            setCurrentDirection(Up);
            break;
        case Qt::Key_Down:
            setCurrentDirection(Down);
            break;
        case Qt::Key_Left:
            setCurrentDirection(Left);
            break;
        case Qt::Key_Right:
            setCurrentDirection(Right);
            break;
        case Qt::Key_Space:
            restartGame();
            break;
    };
}

void MainWindow::nextIteration()
{
    QPair<int, int> previousPos = body[0];
    QPair<int, int> tempPos = previousPos;
    for(int i = 0; i < body.size(); i++)
    {
        if(!i)
        {
            switch(currentDirection)
            {
                case Up:
                    body[0].second--;
                    break;
                case Down:
                    body[0].second++;
                    break;
                case Left:
                    body[0].first--;
                   break;
               case Right:
                   body[0].first++;
                   break;
            }
            continue;
        }
        tempPos = body[i];
        body[i] = previousPos;
        previousPos = tempPos;
    }
}

void MainWindow::checkField()
{
    //Проверка на необходимость расширения тела змейки
    if(body[0] == foodCoord)
    {
        body.append(QPair<int, int>(body[body.size() - 1]));
        
        gameSpeed = pointCount / 5;
        timer.setInterval(500 - gameSpeed * 50);
        
        setFoodCoord();
        
        infoLabel->setText(QString("Points count: %0 \t Speed: %1").arg(++pointCount).arg(gameSpeed));
    }
    
    //Проверка на столкновение головы змейки с ее телом
    for(int i = 1; i < body.size(); i++)
        if(body[0] == body[i] && body.size() > 2)
            parseLoseEvent();
    
    //Проверка на то, находится ли змейка в пределах игрового поля
    if(body[0].first == 0 || body[0].first == matrixOrder - 1)
        parseLoseEvent();
    if(body[0].second == 0 || body[0].second == matrixOrder - 1)
        parseLoseEvent();
    
}

void MainWindow::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    drawField(painter);
    drawGrid(painter);
    drawFood(painter);
    drawSnake(painter);
    update();
}

void MainWindow::parseLoseEvent()
{
    timer.stop();
    QMessageBox::information(this, "Snake", "You lose. Press SPACE to restart the game.");
}
void MainWindow::restartGame()
{
    body.clear();
    currentDirection = Up;
    infoLabel->setText(QString("Points count: %0 \t Speed: %1").arg(0).arg(0));
    pointCount = 0;
    gameSpeed = 0;
    initField();
    
    timer.setInterval(500);
    timer.start();
}
void MainWindow::drawField(QPainter& painter)
{
    

    for(int i = 0; i < matrixOrder; i++)
    {
        for(int j = 0; j < matrixOrder; j++)
        {
            QRect rect(i * width() / matrixOrder, j * height() / matrixOrder,
                       width() / matrixOrder, height() / matrixOrder);    
            if(i == 0 || i == matrixOrder - 1 || 
               j == 0 || j == matrixOrder - 1)
            {

                painter.drawImage(rect, waterTexture);
                continue;
            }
            painter.drawImage(rect, grassTexture);
        }
    }
}

void MainWindow::drawGrid(QPainter& painter)
{
    //Отрисовка вертикальных линий сетки
    for(int i = 0; i < matrixOrder; i++)
    {
        QPoint upPoint(i * width() / matrixOrder, 0);
        QPoint downPoint(i * width() / matrixOrder, height());
        painter.drawLine(upPoint, downPoint);
    }
    
    //Отрисовка горизонтальных линий
    for(int i = 0; i < matrixOrder; i++)
    {
        QPoint leftPoint(0, i * height() / matrixOrder);
        QPoint rightPoint(width(), i * height() / matrixOrder);
        painter.drawLine(leftPoint, rightPoint);
    }
}

void MainWindow::drawFood(QPainter& painter)
{
   QRect rect(foodCoord.first * width() / matrixOrder, foodCoord.second * height() / matrixOrder,
              width() / matrixOrder, height() / matrixOrder);
   painter.save();
   painter.drawImage(rect, foodTexture);
   painter.restore();

}
void MainWindow::drawSnake(QPainter& painter)
{
    painter.save();
    painter.setBrush(QBrush(Qt::red));
    for(int i = 0; i < body.size(); i++)
        painter.drawRect(body[i].first * width() / matrixOrder, body[i].second * height() / matrixOrder,
                         width() / matrixOrder, height() / matrixOrder);
    painter.restore();
}