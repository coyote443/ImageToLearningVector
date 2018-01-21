#include "transformator.h"

Transformator::Transformator(QObject *parent): QObject(parent){}

void Transformator::setParams(int percTeach, int percTest, QString localisation, QString fileName){
    m_Learn = percTeach;
    m_Test  = percTest;
    m_Dir   = localisation;
    m_FileName = fileName;
    qsrand(QTime::currentTime().msec());
}

QString Transformator::getSignalClassName(QString val){
    int posRight = 0, posLeft = 0;
    for(int ch = val.size() - 1; ch > 0; ch--){
        if(val[ch] == '/'){
            if(posRight == 0)
                posRight = ch;
            else
                posLeft = ch;
        }
        if(posRight && posLeft)
            break;
    }
    QString out;
    for(int pos = posLeft + 1; pos < posRight; pos++)
        out += val[pos];
    return out;
}

void Transformator::addImageValsForStream(QString SEP, QString &val, QTextStream &stream){
    QImage image;

    image.load(val);
    if(image.isNull() == true)
        QMessageBox::about(0, "Problem z załadowaniem obrazu", QString("Różnica dla obrazu %1").arg(val));

    static int  wid = image.width(),
                hei = image.height();
    if(wid != image.width() || hei != image.height()){
        QMessageBox::about(0, "Różne rozmiary obrazów", QString("Różnica dla obrazu %1").arg(val));
    }

    for(int width = 0; width < image.width(); width++) {
        for(int height = 0; height < image.height(); height++) {
            QColor takeMe( image.pixel(width, height) );
            double pixVal =  1 - (double)takeMe.black() / 255; // Scaled to 0-1 borders
            stream << pixVal << SEP;
        }
    }
    stream << endl;
}

bool Transformator::startLearning(QString SEP){
    QString     signalL,
                signalT;

    QTextStream streamL(&signalL),
                streamT(&signalT);

    int classCounter    = 0,
        fileCounter     = 0,
        progressCounter = 0;

    QString oldClassVal = "";

    QDir dir;
    dir.setPath(m_Dir);

    QDirIterator qdi(dir.absolutePath(), QStringList() << "*.gif", QDir::NoSymLinks | QDir::Files, QDirIterator::Subdirectories );
    QDirIterator num(dir.absolutePath(), QStringList() << "*.gif", QDir::NoSymLinks | QDir::Files, QDirIterator::Subdirectories );

    QString tmpLoc = num.next();
    QImage tmpImg(tmpLoc);
    int inputSize = tmpImg.width() * tmpImg.height();

    if(!num.hasNext())  // to jest tak biedne ze makabra
        return false;

    while(num.hasNext()){
        num.next();
        fileCounter++;
    }

    QVector<int> progresPoints = {100, 80, 60, 40, 20, 0};

    while(qdi.hasNext()) {
        progressCounter++;
        QString fileLoc         = qdi.next();
        QString classValue      = getSignalClassName(fileLoc);
        bool    itIsLearnVectOrder = rand()%100 < m_Learn;

        if(itIsLearnVectOrder){
            streamL << classValue << endl;
            addImageValsForStream(SEP, fileLoc, streamL);
        }
        else{
            streamT << classValue << endl;
            addImageValsForStream(SEP, fileLoc, streamT);
        }

        if(oldClassVal != classValue){
            classCounter++;
            oldClassVal = classValue;
        }
        double progtmp = (double)progressCounter / (double)fileCounter * 100;
        if((int)progtmp > progresPoints.back()){
            progresPoints.pop_back();
            emit makeTwentyPercentProgress();
        }
    }

    QFile fileL(m_FileName + "L.signal"),
          fileT(m_FileName + "T.signal");

    QTextStream streamLOut(&fileL),
                streamTOut(&fileT);

    fileL.open(QIODevice::WriteOnly | QIODevice::Truncate);
    streamLOut << inputSize << SEP << classCounter << endl;
    streamLOut << signalL;
    fileL.close();

    fileT.open(QIODevice::WriteOnly | QIODevice::Truncate);
    streamTOut << inputSize << SEP << classCounter << endl;
    streamTOut << signalT;
    fileT.close();

    return true;
}
