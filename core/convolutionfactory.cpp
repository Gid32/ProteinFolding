#include "convolutionfactory.h"
ConvolutionFactory* ConvolutionFactory::instance = nullptr;

ConvolutionFactory::ConvolutionFactory(QObject *parent) : QObject(parent)
{
    isStarted_ = false;
}

ConvolutionFactory* ConvolutionFactory::getFactory()
{
    if(!instance)
        instance = new ConvolutionFactory();
    return instance;
}

void ConvolutionFactory::createProtein(int count, double coef)
{

    protein_.clear();
    count_ = count;
    int countFob = 0;
    int currentCountFob = 0;
    QTime time = QTime::currentTime();
    qsrand((uint)time.msec());
    for(int i=0;i<count_;i++)
    {
        if(qrand()%2)
            protein_.push_back(H_FOB);
        else
            protein_.push_back(H_FILL);
    }
    emit createdProtein(protein_);
}

void ConvolutionFactory::loadProtein(QByteArray protein)
{
    protein_.clear();
    for(int i=0;i<protein.size();i++)
        protein_.push_back(protein.at(i));
    emit createdProtein(protein_);
}

void ConvolutionFactory::createTrace()
{
    traceM_ = count_ - 1;
    traceN_ = net_->getMaxTurn();
    //trace - матрица  mxn m-количество шариков-1, n - количество возможных поворотов
    trace_ = new double*[traceM_];
    for(int i=0;i<traceM_;i++)
        trace_[i] = new double[traceN_];

    for(int i=0;i<traceM_;i++)
        for(int j=0;j<traceN_;j++)
            trace_[i][j] = traceMin_;
}

void ConvolutionFactory::clearTrace()
{
    if(traceM_ == 0)
        return;
    for (int i = 0; i < traceM_; i++)
            delete [] trace_[i];
    traceM_ = 0;
    traceN_ = 0;
}

void ConvolutionFactory::setSettings(SETTINGS settings)
{
    if(isStarted_)
    {
        emit error("уже работает");
        return;
    }
    if(protein_.isEmpty())
    {
        emit error("вначале сгенерируйте протеин");
        return;
    }
    net_ = NetFactory::getInstance()->getNetByName(settings.value("netValue").toString());
    method_ = settings.value("methodValue").toInt();
    traceMin_ = settings.value("traceMin").toDouble();
    traceMax_ = settings.value("traceMax").toDouble();
    traceEvaporation_ = settings.value("traceEvaporation").toDouble();
    traceCoef_ = settings.value("traceCoef").toDouble();

    weights_[H_FOB][H_FOB] = settings.value("weightsFOBFOB").toDouble();
    weights_[H_FOB][H_FILL] = settings.value("weightsFOBFIL").toDouble();
    weights_[H_FOB][FILL_AREA] = settings.value("weightsFOBFREE").toDouble();
    weights_[H_FOB][BLOCK_AREA] = settings.value("weightsFOBFREE").toDouble();
    weights_[H_FILL][H_FOB] = settings.value("weightsFILFOB").toDouble();
    weights_[H_FILL][H_FILL] = settings.value("weightsFILFIL").toDouble();
    weights_[H_FILL][FILL_AREA] = settings.value("weightsFILFREE").toDouble();
    weights_[H_FILL][BLOCK_AREA] = settings.value("weightsFILFREE").toDouble();

    elitizm_ = settings.value("elitizm").toInt();

    clearTrace();
    createTrace();

    isStarted_ = true;
    emit ready(settings);
}

void ConvolutionFactory::stop()
{
   isStarted_ = false;
}

int ConvolutionFactory::getCount(Convolution *convolution,QVector3D coord,QVector3D blockCoordPrev,QVector3D blockCoordNext,int currentDirection)
{
    int count = 0;
    for(int i=0;i<=net_->getMaxTurn();i++)
    {
        int direction = net_->turnToDirection(currentDirection,i);
        QVector3D testCoord = net_->getDirectionCoord(direction,coord);
        if(testCoord != blockCoordPrev && testCoord != blockCoordNext)
            count += convolution->isHydroFobByCoord(testCoord);
    }
    return count;
}

double ConvolutionFactory::getWeights(Convolution *convolution, QVector3D coord, BYTE currentAm,int currentDirection)
{
    double weights = 0.0;
    for(int i=0;i<=net_->getMaxTurn();i++)
    {
        int direction = net_->turnToDirection(currentDirection,i);
        QVector3D testCoord = net_->getDirectionCoord(direction,coord);
        BYTE value = convolution->getValueByCoord(testCoord);
        weights += weights_[currentAm][value];
    }
    return weights;
}

int ConvolutionFactory::probabilistic(Convolution *convolution,QVector<QVector3D> possible, QVector<int> turns,int currentDirection)
{
    if(possible.size()==1)
        return 0;

    int turnNumber = convolution->history_.size();//текущий номер поворота начало с 0

    //baraban
    double sumEvrist = 0;
    double sumTrace = 0;
    QVector<double> chances;
    QVector<double> weights;//для того чтобы не пересчитывать каждый раз соседей, должно ускорить
    for(int i=0;i<possible.size();i++)
    {
       double weight = getWeights(convolution,possible.at(i),protein_.at(turnNumber),currentDirection);//получаем веса
       sumEvrist += weight;
       sumTrace += trace_[turnNumber][turns.at(i)];//получаем следы
       weights.push_back(weight);
    }
    for(int i=0;i<possible.size();i++)
    {
        double trace = (trace_[turnNumber][turns.at(i)]/sumTrace)*traceCoef_;
        double evrist = (weights.at(i)/sumEvrist)*(1-traceCoef_);
        double chance = (trace+evrist);
        chances.push_back(chance);
    }

    double val = ((double) qrand()/(RAND_MAX));
    double summ = 0;
    for(int i=0;i<chances.size();i++)
    {
        summ += chances.at(i);
        if (summ >= val)
            return i;
    }
    //baraban end
    return 0;
}

bool ConvolutionFactory::getNext(Convolution *convolution, int currentDirection, QVector3D currentCoords, QVector3D &coords, int &direction,int &turn)
{
    QVector<QVector3D> possible;
    QVector<int> turns;
    QVector<int> directions;
    for(int i=net_->getMinTurn();i<=net_->getMaxTurn();i++)
    {
        direction = net_->turnToDirection(currentDirection,i);
        coords = net_->getDirectionCoord(direction,currentCoords);
        if(convolution->getValueByCoord(coords)==FILL_AREA)
        {
            possible.push_back(coords);
            turns.push_back(i);
            directions.push_back(direction);
        }
    }

    if(possible.empty())
        return false;

    int n;
//    if(method_ == 0)
//        n = randomMethod(possible);
//    else if(method_ == 1)
//        n = withoutProbabilistic(possible);
//    else
//        n = probabilistic(possible);
    n = probabilistic(convolution,possible,turns,currentDirection);

    coords = possible.at(n);
    turn = turns.at(n);
    direction = directions.at(n);
    return true;
}

Convolution* ConvolutionFactory::getConvolution()
{
    QVector3D currentCoords;
    int currentDirection;
    Convolution *currentConv;

    currentConv = new Convolution();
    currentConv->history_.clear();
    currentDirection = 0;
    int currentTurnNumber = 1;
    currentCoords = QVector3D(0,0,0);//начальная координата в массиве откуда начинается свертка
    currentConv->setValueByCoord(currentCoords, protein_.at(0));//записываем в массив первую аминокислоту
    QVector3D newCoords;
    int direction;
    int turn;
    while(currentTurnNumber!=protein_.size())
    {
        if(!getNext(currentConv,currentDirection,currentCoords,newCoords,direction,turn))//нельзя ходить
        {
            currentTurnNumber--;
            History history = currentConv->history_.last();
            currentConv->setValueByCoord(history.coords,BLOCK_AREA);
            currentDirection = history.direction;
            currentConv->history_.removeLast();
            currentCoords = currentConv->history_.last().coords;
            continue;
        }
        currentConv->setValueByCoord(newCoords,protein_.at(currentTurnNumber));
        currentConv->history_.push_back(History(newCoords,currentDirection,turn));//в историю добавляем координату и дайрекшин который был перед этим
        currentDirection = direction;
        currentCoords = newCoords;
        currentTurnNumber++;
    }
    return currentConv;
}

int ConvolutionFactory::getResult(Convolution *convolution)
{
   int result = 0;
   QVector3D blockCoordPrev(0,0,0);
   QVector3D blockCoordNext = convolution->history_[0].coords;
   if(convolution->isHydroFobByCoord(blockCoordPrev))
       result += getCount(convolution,blockCoordPrev, blockCoordNext, blockCoordNext, convolution->history_[0].direction);
   for (int i = 0; i < convolution->history_.size(); ++i)
   {
        if(i!=convolution->history_.size()-1)
            blockCoordNext = convolution->history_[i+1].coords;
        QVector3D coords = convolution->history_[i].coords;
        if(convolution->isHydroFobByCoord(coords))
            result += getCount(convolution,coords,blockCoordPrev,blockCoordNext, convolution->history_[i].direction);
        blockCoordPrev = coords;
   }
   return result;
}

QVector<QVector3D> ConvolutionFactory::getVectorCoords(Convolution *convolution)
{
    QVector<QVector3D> vectorCoords;
    for (int i = 0; i < convolution->history_.size(); ++i)
        vectorCoords.push_back(net_->getCoords(convolution->history_[i].coords));
    return vectorCoords;
}

void ConvolutionFactory::changeTrace(QVector<Convolution*> convolutions)
{
    int minresult = getResult(convolutions.at(0));
    int maxresult = minresult;
    for(int i=1;i<convolutions.size();i++)
    {
        int result = getResult(convolutions.at(i));
        if(minresult < result)
            minresult = result;
        else if(maxresult > result)
            maxresult = result;
    }
    int del = maxresult-minresult;
    if(del == 0)
        del = 1;

    for(int i=0;i<convolutions.size();i++)
    {
        QVector<History> history = convolutions.at(i)->history_;
        double result = (double)(getResult(convolutions.at(i))-minresult)/del;
        for(int j=0;j<history.size();j++)
            trace_[j][history.at(j).turn-1] += result;
    }

    //evaporation
    switch(elitizm_)
    {
    case 0: evoparationMMAS();
        break;
    case 1: evoparationEXP();
        break;
    case 2: evoparationONLY();
        break;
    default: evoparationMMAS();
    }

}

void ConvolutionFactory::evoparationMMAS()
{
    for(int i=0;i<traceM_;i++)
        for(int j=0;j<traceN_;j++)
        {
            trace_[i][j] *= traceEvaporation_;
            if(trace_[i][j] > traceMax_)
                trace_[i][j] = traceMax_;
            else if(trace_[i][j] < traceMin_)
                trace_[i][j] = traceMin_;
        }
}

void ConvolutionFactory::evoparationEXP()
{
    for(int i=0;i<traceM_;i++)
        for(int j=0;j<traceN_;j++)
             trace_[i][j] *= qExp(-traceEvaporation_*trace_[i][j]);
}

void ConvolutionFactory::evoparationONLY()
{
    for(int i=0;i<traceM_;i++)
        for(int j=0;j<traceN_;j++)
            trace_[i][j] *= traceEvaporation_;
}
