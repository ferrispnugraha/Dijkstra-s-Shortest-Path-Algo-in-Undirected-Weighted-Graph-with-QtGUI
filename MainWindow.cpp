#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <QInputDialog>
#include <QDebug>
#include <algorithm>
#include <QRandomGenerator>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->backgroundLabel->setStyleSheet("QLabel { background-color : white; }");
    ui->drawVertexButton->setCheckable(true);
    ui->drawEdgeButton->setCheckable(true);
    ui->startButton->setCheckable(true);

    connect(ui->backgroundLabel, &GraphArea::drawVertexClicked, this, &MainWindow::addVertex);
    connect(ui->backgroundLabel, &GraphArea::deleteComponent, this, &MainWindow::removeVertex);
    connect(ui->backgroundLabel, &GraphArea::drawEdgeClicked, this, &MainWindow::addEdge);
    connect(ui->backgroundLabel, &GraphArea::deleteComponent, this, &MainWindow::removeEdge);
    connect(ui->backgroundLabel, &GraphArea::startCalculating, this, &MainWindow::startingDijkstra);
    connect(ui->backgroundLabel, &GraphArea::showingPath, this, &MainWindow::goalOfDijkstra);
    connect(ui->backgroundLabel, &GraphArea::showingPathRedBlack, this, &MainWindow::goalOfDijkstraRedBlack);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_drawVertexButton_toggled(bool checked)
{
    if(checked){
        ui->drawEdgeButton->setChecked(false);
        ui->backgroundLabel->setDrawingVertex(true);
    }
    else{
        ui->backgroundLabel->setDrawingVertex(false);
    }
}

void MainWindow::on_drawEdgeButton_toggled(bool checked)
{
    if(checked){
        ui->drawVertexButton->setChecked(false);
        ui->backgroundLabel->setDrawingEdge(true);
    }
    else{
        ui->backgroundLabel->setDrawingEdge(false);
        if(target != nullptr){
            target->setColor(0);
            forEdge.clear();
        }
    }
}

void MainWindow::addVertex(QPoint pos){
    if(ui->backgroundLabel->adjacencyList.foundVertex(pos)) {//make sure no vertices overlap
        return;
    }
    Vertex* label = new Vertex(pos.x() - 12, pos.y() - 12, ui->backgroundLabel);
    label->setId(vertexCounter);
    ++vertexCounter;
    label->setAlignment(Qt::AlignCenter);
    label->setVisible(true);
    label->setGeometry(pos.x() - 12, pos.y()-12, 25, 25);
    label->setStyleSheet("background-color: rgba(255, 255,255, 0)");
    installEventFilter(label);
    ui->backgroundLabel->adjacencyList.insertVertex(label);
}

void MainWindow::removeVertex(QPoint pos)
{
    if(ui->backgroundLabel->adjacencyList.foundVertex(pos)){//make sure vertex located is not nullptr
        ui->backgroundLabel->adjacencyList.removeVertex(
                    ui->backgroundLabel->adjacencyList.vertexLocated(pos));
    }
}

void MainWindow::addEdge(QPoint pos)
{
    if(forEdge.size() == 0 || forEdge.size() == 1){
        if(ui->backgroundLabel->adjacencyList.foundVertex(pos)){
            Vertex* v = ui->backgroundLabel->adjacencyList.vertexLocated(pos);
            if(forEdge.size() == 0)//filled
            {
              target = v;
              target->setColor(1);//yellow in enum
            }
            forEdge.push_back(v);
        }
        if(forEdge.size() == 2){
            int weight = QInputDialog::getInt(this, "Input Weight", "Input Weight", 0, 0, 1000);

            Vertex* front = forEdge.front();
            Vertex* back = forEdge.back();
            front->setColor(0); //return to black

            Edge* label = new Edge(front, back, weight, ui->backgroundLabel);
            ui->backgroundLabel->adjacencyList.insertEdge(label);
            label->setAlignment(Qt::AlignCenter);
            label->setVisible(true);
            label->setStyleSheet("background-color: rgba(255, 255,255, 0)");
            label->lower();
            label->setGeom();
            installEventFilter(label);

            forEdge.clear();
        }
    }
}

void MainWindow::removeEdge(QPoint pos)
{
    if(ui->backgroundLabel->adjacencyList.foundEdge(pos)){//make sure edge located is not nullptr
        ui->backgroundLabel->adjacencyList.removeEdge(
                    ui->backgroundLabel->adjacencyList.edgeLocated(pos));
    }
}


void MainWindow::on_randGenButton_clicked()
{
    ui->drawEdgeButton->setChecked(false);
    ui->backgroundLabel->setDrawingVertex(false);
    ui->drawVertexButton->setChecked(false);
    ui->backgroundLabel->setDrawingEdge(false);

    if(!ui->backgroundLabel->getDrawingEdge() && !ui->backgroundLabel->getDrawingVertex()){
        RandomDialog randomDialog(ui->backgroundLabel);
        randomDialog.exec();
        if(randomDialog.close()){
            numOfVertex = randomDialog.getNumOfVertex();
            density = randomDialog.getDensity();
            maxEdgeWeight = randomDialog.getMaxEdgeWeight();
        }
        //clear first
        ui->backgroundLabel->adjacencyList.initialize();
        vertexCounter = 0;
        //then random generate
        randomGenerate();
    }
}

void MainWindow::randomGenerate()
{
    QRandomGenerator generator;
    //generate vertex first
    for(int i = 0; i < numOfVertex; ++i){
        int x = generator.bounded(12, ui->backgroundLabel->width()-12);
        int y = generator.bounded(12, ui->backgroundLabel->height()-12);
        addVertex(QPoint(x,y));
    }
    //generate edge with corresponding weight for the available vertex
    //remember the addEdge is quite different
    //we use Edge constructor
    int numOfEdge = density* numOfVertex*(numOfVertex - 1)/200;        //density/100 * N*(N-1)/2
                                                                         //density still in x  %
    for(int j=0; j<numOfEdge; ++j){
        int index1 = generator.bounded(0, ui->backgroundLabel->adjacencyList.size()-1);
        int index2 = generator.bounded(0, ui->backgroundLabel->adjacencyList.size()-1);
        if((index1 == index2) ||
                (ui->backgroundLabel->adjacencyList.indexVertex(index2)
                    ->includeNeighbor(ui->backgroundLabel->adjacencyList.indexVertex(index1))))
        {//repeat
            --j;
        }
        else{
            int weight = generator.bounded(0, maxEdgeWeight);
            Edge* e = new Edge(ui->backgroundLabel->adjacencyList.indexVertex(index1),
                               ui->backgroundLabel->adjacencyList.indexVertex(index2),
                               weight, ui->backgroundLabel);
            ui->backgroundLabel->adjacencyList.insertEdge(e);
            e->setAlignment(Qt::AlignCenter);
            e->setVisible(true);
            e->setStyleSheet("background-color: rgba(255, 255,255, 0)");
            e->lower();
            //label->setGeometry(pos.x() - 12, pos.y()-12, 25, 25);
            e->setGeom();
            installEventFilter(e);
        }
    }
//    ui->backgroundLabel->adjacencyList.printout();
}

void MainWindow::startingDijkstra(QPoint pos)
{
    if(ui->backgroundLabel->adjacencyList.foundVertex(pos)){
       Vertex* v = ui->backgroundLabel->adjacencyList.vertexLocated(pos);
       v->setColor(2);//blue in enum
       v->setWeight(0);
       ui->backgroundLabel->setUnvisited(v);
       ui->startButton->setChecked(false);
    }
}

void MainWindow::goalOfDijkstra(QPoint pos)
{
    if(ui->backgroundLabel->adjacencyList.foundVertex(pos)){
       Vertex* v = ui->backgroundLabel->adjacencyList.vertexLocated(pos);
       v->setColor(2);//blue in enum
       ui->backgroundLabel->implementDijkstra(v);
    }
}

void MainWindow::goalOfDijkstraRedBlack(QPoint pos)
{
    if(ui->backgroundLabel->adjacencyList.foundVertex(pos)){
        Vertex* v = ui->backgroundLabel->adjacencyList.vertexLocated(pos);
        v->setColor(2);//blue in enum
        ui->backgroundLabel->implementDijkstraRedBlack(v);
    }
}

void MainWindow::on_startButton_toggled(bool checked)
{
    if(checked){
      ui->drawEdgeButton->setChecked(false);
      ui->backgroundLabel->setDrawingVertex(false);
      ui->drawVertexButton->setChecked(false);
      ui->backgroundLabel->setDrawingEdge(false);
      ui->backgroundLabel->setCalculating(true);
    }
}

void MainWindow::on_showPathButton_clicked()
{
    ui->drawEdgeButton->setChecked(false);
    ui->backgroundLabel->setDrawingVertex(false);
    ui->drawVertexButton->setChecked(false);
    ui->backgroundLabel->setDrawingEdge(false);
    ui->backgroundLabel->setShowing(true);
}

void MainWindow::on_clearButton_clicked()
{
    ui->drawEdgeButton->setChecked(false);
    ui->backgroundLabel->setDrawingVertex(false);
    ui->drawVertexButton->setChecked(false);
    ui->backgroundLabel->setDrawingEdge(false);
    ui->backgroundLabel->setCalculating(false);
    ui->backgroundLabel->setShowing(false);
    ui->backgroundLabel->clearPath();

}

void MainWindow::on_showPathRedBlackButton_clicked()
{
    ui->drawEdgeButton->setChecked(false);
    ui->backgroundLabel->setDrawingVertex(false);
    ui->drawVertexButton->setChecked(false);
    ui->backgroundLabel->setDrawingEdge(false);
    ui->backgroundLabel->setShowingRedBlack(true);
}
