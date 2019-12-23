#include "widget.h"
#include <QLabel>
#include <QMessageBox>
#include <QInputDialog>
#include <QDebug>
Widget::Widget(QWidget *parent)
    : QWidget(parent)
{

    mainlayout = new QVBoxLayout;
    buttonLayout = new QHBoxLayout;
    createButtons();
    scene = new myScene;
    scene->setSceneRect(QRectF(0, 0, 800, 300));
    view = new QGraphicsView(scene);
    mainlayout->addWidget(view);
    mainlayout->addLayout(buttonLayout);
    setLayout(mainlayout);
    connect(scene, SIGNAL(requireWeight()), this, SLOT(getWeight()));
    connect(scene, SIGNAL(algorithmFinished()), this, SLOT(algorithmFinishedBox()));
    connect(scene, SIGNAL(startButtonUpAndSetWaitingMode()), this, SLOT(startButtonUpAndSetWaitingMode()));
    setFixedSize(1821, 924);
}

Widget::~Widget()
{
}
void Widget::createButtons()
{
    drawVertexButton = new QPushButton(tr("Draw Vertex"));
    drawVertexButton->setCheckable(true);
    buttonLayout->addWidget(drawVertexButton);
    connect(drawVertexButton, SIGNAL(clicked(bool)), this, SLOT(drawVertex(bool)));

    drawEdgeButton = new QPushButton(tr("Draw Edge"));
    drawEdgeButton->setCheckable(true);
    buttonLayout->addWidget(drawEdgeButton);
    connect(drawEdgeButton, SIGNAL(clicked(bool)), this, SLOT(drawEdge(bool)));

    randGenerateButton = new QPushButton(tr("Rand Generate"));
    buttonLayout->addWidget(randGenerateButton);
    connect(randGenerateButton, SIGNAL(clicked()), this, SLOT(randGenerate()));

    startButton = new QPushButton(tr("Start"));
    startButton->setCheckable(true);
    buttonLayout->addWidget(startButton);
    connect(startButton, SIGNAL(clicked(bool)), this, SLOT(start(bool)));

    stepButton = new QPushButton(tr("Step"));
    buttonLayout->addWidget(stepButton);
    connect(stepButton, SIGNAL(clicked()), this, SLOT(step()));

    clearButton = new QPushButton(tr("Clear"));
    buttonLayout->addWidget(clearButton);
    connect(clearButton, SIGNAL(clicked()), this, SLOT(clear()));

    showPathButton = new QPushButton(tr("Show Path"));
    showPathButton->setCheckable(true);
    buttonLayout->addWidget(showPathButton);
    connect(showPathButton, SIGNAL(clicked(bool)), this, SLOT(showPath(bool)));
}

void Widget::getWeight()
{
    bool ok{false};
    int weight = QInputDialog::getInt(this, tr("Input Weight"), tr("Input Weight"), 1, 1, 2147483647, 1, &ok);
    if (ok)
        scene->setProcessingWeight(weight);
}
void Widget::drawVertex(bool checked)
{
    scene->setStartItemColor(Qt::black);
    drawEdgeButton->setChecked(false);
    startButton->setChecked(false);
    showPathButton->setChecked(false);

    if (checked)
    {
        scene->setMode(myScene::drawingVertex);
    }
    else
    {
        scene->setMode(myScene::waiting);
    }
}

void Widget::drawEdge(bool checked)
{
    scene->setStartItemColor(Qt::black);
    drawVertexButton->setChecked(false);
    startButton->setChecked(false);
    showPathButton->setChecked(false);

    if (checked)
    {
        scene->setMode(myScene::drawingEdge);
    }
    else
    {
        scene->setMode(myScene::waiting);
    }
}
void Widget::randGenerate()
{
    bool numOfVerticesOk{false};
    int numOfVertices = QInputDialog::getInt(this, tr("Input number of vertices"), tr("Input number of vertices"), 1, 0, 2147483647, 1, &numOfVerticesOk);
    if(!numOfVerticesOk)
        return;
    bool edgesDensityOk{false};
    double edgesDensity = QInputDialog::getDouble(this, tr("Input edges density"), tr("Input edges density"), 0.5, 0.0, 1.0, 2, &edgesDensityOk);
    if(!edgesDensityOk)
        return;

    bool maxWeightOk{false};
    int maxWeight = QInputDialog::getInt(this, tr("Input max weight"), tr("Input max weight"), 1, 1, 2147483647, 1, &maxWeightOk);
    if(!maxWeightOk)
        return;

    int numOfEdges = int(edgesDensity * numOfVertices * (numOfVertices - 1) * 0.5);
    scene->randGenerate(numOfVertices, numOfEdges, maxWeight);

}
void Widget::start(bool checked)
{
    bool choiceOk{false};
    int choice = QInputDialog::getInt(this, tr("Choose data structure"), tr("Enter 0 for AVL Tree, 1 for Red-Black Tree"), 0, 0, 1, 1, &choiceOk);
    if (choiceOk) //TODO
    {
        scene->setPriorityQueue((choice == 0) ? myScene::AVLTree : myScene::RBTree);
    }
    scene->setMode(myScene::starting);
    scene->setStartItemColor(Qt::black);
    showPathButton->setChecked(false);
    drawVertexButton->setChecked(false);
    drawEdgeButton->setChecked(false);
}
void Widget::startButtonUpAndSetWaitingMode()
{
    startButton->setChecked(false);
    scene->setMode(myScene::waiting);
}
void Widget::step()
{
    scene->step();
}
void Widget::clear()
{
    scene->clear();
}
void Widget::algorithmFinishedBox()
{
    QMessageBox::information(this, "Finished", "The algorithm is finished.");
}
void Widget::showPath(bool checked)
{
    scene->setStartItemColor(Qt::black);
    startButton->setChecked(false);
    drawVertexButton->setChecked(false);
    drawEdgeButton->setChecked(false);
    if (checked)
    {
        scene->setMode(myScene::showPath);
    }
    else
    {
        scene->setMode(myScene::waiting);
        scene->resetArrowsColor();
    }
}

