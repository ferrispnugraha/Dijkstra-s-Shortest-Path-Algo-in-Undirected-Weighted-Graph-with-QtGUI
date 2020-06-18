#ifndef RANDOMDIALOG_H
#define RANDOMDIALOG_H

#include <QDialog>

namespace Ui {
class RandomDialog;
}

class RandomDialog : public QDialog
{
    Q_OBJECT

public:
    explicit RandomDialog(QWidget *parent = nullptr);
    ~RandomDialog();

    int getNumOfVertex() const;
    int getDensity() const;
    int getMaxEdgeWeight() const;

signals:
    void vertexDensityWeight(int, int, int);

private slots:
    void on_pushButton_clicked();

private:
    Ui::RandomDialog *ui;
    int numOfVertex = 0;
    int density = 0;
    int maxEdgeWeight = 0;
};

#endif // RANDOMDIALOG_H
