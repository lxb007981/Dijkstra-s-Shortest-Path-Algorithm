#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <canvas.h>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QGraphicsScene>
#include <QGraphicsView>
class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget()override;

private:
    myScene* scene;
    QGraphicsView* view;
    QVBoxLayout * mainlayout;
    QHBoxLayout* buttonLayout;
    QPushButton* drawVertexButton;
    QPushButton* drawEdgeButton;
    QPushButton* randGenerateButton;
    QPushButton* startButton;
    QPushButton* stepButton;
    QPushButton* clearButton;
    QPushButton* showPathButton;
    void createButtons();
    void resetAllButtons();

private slots:
    void drawVertex(bool);
    void drawEdge(bool);
    void randGenerate();
    void start(bool);
    void step();
    void clear();
    void showPath(bool);
    void getWeight();
    void algorithmFinishedBox();
    void startButtonUpAndSetWaitingMode();
};
#endif // WIDGET_H
