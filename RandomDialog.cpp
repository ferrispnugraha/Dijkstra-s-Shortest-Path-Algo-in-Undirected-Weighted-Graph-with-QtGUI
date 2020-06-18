#include "RandomDialog.h"
#include "ui_RandomDialog.h"
#include <QDebug>

RandomDialog::RandomDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RandomDialog)
{
    ui->setupUi(this);
}

RandomDialog::~RandomDialog()
{
    delete ui;
}

int RandomDialog::getNumOfVertex() const
{
    return numOfVertex;
}

int RandomDialog::getDensity() const
{
    return density;
}

int RandomDialog::getMaxEdgeWeight() const
{
    return maxEdgeWeight;
}

void RandomDialog::on_pushButton_clicked()
{
    numOfVertex = ui->numOfVertexBox->value();
    density = ui->densityBox->value();
    maxEdgeWeight = ui->maxEdgeWeightBox->value();
    this->close();
}
