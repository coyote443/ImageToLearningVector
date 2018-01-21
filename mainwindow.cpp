#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_Transformator = new Transformator(this);
    connect(m_Transformator, SIGNAL(makeTwentyPercentProgress()), this, SLOT(on_ProgressBar_SetValue()) );
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_ProgressBar_SetValue(){
    m_ProgresVal += 20;
    if(m_ProgresVal > 100) m_ProgresVal = 100;
    ui->progressBar->setValue(m_ProgresVal);
}

void MainWindow::on_pushButtonAddLocalisation_clicked(){
    QString localisation = QFileDialog::getExistingDirectory(this, "Dodaj lokalizację dla klas");
    ui->lineEditLocalisation->setText(localisation);
    ui->pushButtonGenerate->setEnabled(true);
    m_ProgresVal = 0;
    ui->progressBar->setValue(m_ProgresVal);
}

void MainWindow::on_spinBoxLearning_valueChanged(int arg1){
    int value = abs(100 - arg1);
    ui->spinBoxTrening->setValue(value);
}

void MainWindow::on_spinBoxTrening_valueChanged(int arg1){
    int value = abs(100 - arg1);
    ui->spinBoxLearning->setValue(value);
}

void MainWindow::on_pushButtonGenerate_clicked(){
    QString loc = ui->lineEditLocalisation->text();
    QString nam = ui->lineEditName->text();

    int test = ui->spinBoxTrening->value();
    int learn= ui->spinBoxLearning->value();
    m_Transformator->setParams(learn, test, loc, nam);

    if(m_Transformator->startLearning("[::]") != true)
        QMessageBox::information(this, "ERROR", "Nie wykrywam bazy danych. Zmień lokalizację, wybierz [tenFolder]/[jeden z n-folderów z obrazami gif] ");
}
