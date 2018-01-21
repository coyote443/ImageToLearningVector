#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "transformator.h"
#include "header.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_pushButtonAddLocalisation_clicked();
    void on_spinBoxLearning_valueChanged(int arg1);
    void on_spinBoxTrening_valueChanged(int arg1);
    void on_pushButtonGenerate_clicked();
    void on_ProgressBar_SetValue();

private:
    Ui::MainWindow *ui;
    Transformator * m_Transformator;
    int m_ProgresVal = 0;
};

#endif // MAINWINDOW_H
