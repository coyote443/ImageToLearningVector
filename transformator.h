#ifndef TRANSFORMATOR_H
#define TRANSFORMATOR_H

#include <QImage>
#include <QFile>
#include <QTextStream>
#include <QDir>
#include <QDirIterator>
#include <QDebug>
#include <QTime>
#include <QMessageBox>

class Transformator : public QObject
{
       Q_OBJECT
public:
    Transformator(QObject * parent = 0);
    void setParams(int percLern, int percTest, QString localisation, QString fileName);
    bool startLearning(QString SEP = "[::]");

signals:
    void makeOnePercentProgress();

private:
    int     m_Test,
            m_Learn;
    QString m_Dir,
            m_FileName;
    QString getSignalClassName(QString val);
    void    addImageValsForStream(QString SEP, QString &val, QTextStream &stream);
};

#endif // TRANSFORMATOR_H
