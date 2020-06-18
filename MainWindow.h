#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVector>
#include "GraphArea.h"
#include "Edge.h"
#include "Vertex.h"
#include "RandomDialog.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


private slots:
    void on_drawVertexButton_toggled(bool checked);

    void on_drawEdgeButton_toggled(bool checked);


    void addVertex(QPoint pos);
    void removeVertex(QPoint pos);//also remove line

    void addEdge(QPoint pos);
    void removeEdge(QPoint pos);

    void on_randGenButton_clicked();
    void randomGenerate();

    void startingDijkstra(QPoint pos);
    void goalOfDijkstra(QPoint pos);
    void goalOfDijkstraRedBlack(QPoint pos);

    void on_startButton_toggled(bool checked);

    void on_showPathButton_clicked();

    void on_clearButton_clicked();


    void on_showPathRedBlackButton_clicked();

private:
    Ui::MainWindow *ui;
    QVector<Vertex* > forEdge;  //temporary storage for addEdge, only for 2
    Vertex* target = nullptr;   //pointed vertex

    int numOfVertex = 0;
    int density = 0;
    int maxEdgeWeight = 0;

    int vertexCounter = 0;
};

#endif // MAINWINDOW_H
