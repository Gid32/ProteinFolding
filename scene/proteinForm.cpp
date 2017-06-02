#include "proteinForm.h"
#include "ui_proteinForm.h"

ProteinForm::ProteinForm(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ProteinForm)
{
    ui->setupUi(this);
    view_ = new Qt3DExtras::Qt3DWindow();
    rootEntity_ = initWidget(ui->proteinWidget,view_,QColor(240,240,240),false,1.0f);

    connect(ui->genericButton,SIGNAL(clicked()),this,SLOT(createProtein()));
    connect(ui->saveButton,SIGNAL(clicked()),this,SLOT(saveToFileProtein()));
    connect(ui->loadButton,SIGNAL(clicked()),this,SLOT(loadFromFileProtein()));


    selfilter = tr("protein (*.protein)");
}

ProteinForm::~ProteinForm()
{
    delete ui;
}

Qt3DCore::QEntity* ProteinForm::initWidget(QWidget *widget,Qt3DExtras::Qt3DWindow *view, QColor color,bool control, float cameraZ)
{
    Qt3DCore::QEntity* rootEntity = new Qt3DCore::QEntity();
    view->setRootEntity(rootEntity);
    QHBoxLayout *mainLayout = new QHBoxLayout(widget);
    mainLayout->setMargin(0);

    //camera
    Qt3DRender::QCamera *cameraEntity = view->camera();
    cameraEntity->lens()->setPerspectiveProjection(45.0f, 4.0f/3.0f, 0.1f, 1000.0f);
    cameraEntity->setPosition(QVector3D(0, 0, cameraZ));
    cameraEntity->setUpVector(QVector3D(0, 1, 0));
    cameraEntity->setViewCenter(QVector3D(0, 0, 0));
    if(control)
    {
        Qt3DExtras::QFirstPersonCameraController *camController = new Qt3DExtras::QFirstPersonCameraController(rootEntity);
        camController->setCamera(cameraEntity);
    }
    //light
    Qt3DCore::QEntity *lightEntity = new Qt3DCore::QEntity(rootEntity);
    Qt3DRender::QPointLight *light = new Qt3DRender::QPointLight(lightEntity);
    light->setColor("white");
    light->setIntensity(3);
    lightEntity->addComponent(light);
    Qt3DCore::QTransform *lightTransform = new Qt3DCore::QTransform(lightEntity);
    lightTransform->setTranslation(cameraEntity->position());
    lightEntity->addComponent(lightTransform);
    //color
    view->defaultFrameGraph()->setClearColor(color);
    //mainconatainer
    QWidget *mainContainer = QWidget::createWindowContainer(view);
    mainLayout->addWidget(mainContainer,1);

    Qt3DInput::QInputAspect *input = new Qt3DInput::QInputAspect;
    view->registerAspect(input);
    return rootEntity;
}

void ProteinForm::saveToFileProtein()
{
    //QString selfilter = tr("protein (*.protein)");
    QString filename = QFileDialog::getSaveFileName(
            this,
            "Збереження протеїну",
            "",
            tr("protein (*.protein);;proteinN (*.proteinN);;proteinS (*.proteinS)"),
            &selfilter
    );
    if(filename.isEmpty())
        return;
    QFile file(filename);
    if(!file.open(QIODevice::WriteOnly))
        return;
    QString fileExt = QFileInfo(filename).suffix();
    setLastFileType(fileExt);
    QTextStream out(&file);
    if(fileExt == "protein")
        for(int i = 0;i < protein_.size();i++)
           out<<protein_.at(i);
    else if(fileExt == "proteinN")
        for(int i = 0;i < protein_.size();i++)
           out<<static_cast<int>(protein_.at(i));
    else if(fileExt == "proteinS")
        for(int i = 0;i < protein_.size();i++)
            if(protein_.at(i) == H_FILL)
                out<<"P";
            else
                out<<"H";
    file.close();
}

void ProteinForm::loadFromFileProtein()
{
    QString fileName = QFileDialog::getOpenFileName(
            this,
            "Звантаження протеїну",
            "",
            tr("protein (*.protein);;proteinN (*.proteinN);;proteinS (*.proteinS)"),
            &selfilter
    );
    if(fileName.isEmpty())
        return;
    QFile file(fileName);
    QString fileExt = QFileInfo(fileName).suffix();
    setLastFileType(fileExt);
    file.open(QIODevice::ReadOnly);
    QByteArray array;
    QTextStream in(&file);
    QString str = in.readAll();
    if(fileExt == "protein")
        array = file.readAll();
    else
    {
        for(int i = 0;i<str.size();i++)
            if(str.at(i)=="1" || str.at(i)=="H" || str.at(i)=="Н")
                array.append(H_FOB);
            else
                array.append((char)H_FILL);
    }
    file.close();
    emit loadedProtein(array);
}

void ProteinForm::createProtein()
{
    emit createdProtein(ui->genericCount->value(),ui->genericPercent->value());
}

void ProteinForm::getProtein(VECTORBYTE protein)
{
    protein_ = protein;
    for (int i = 0; i < nodes.size(); ++i)
        delete nodes.at(i);
    nodes.clear();

    float startX = -0.40f;
    float startY = 0.40f;
    float distance = abs(startX*2)/sqrt(protein_.size());
    float radius = distance/3;
    float connectionRadius = radius/5;
    int countInRow = abs(startX*2)/distance;
    QVector3D start = QVector3D(startX+radius,startY-radius,0);
    Node *prev = nullptr;
    for (int i = 0; i < protein_.size(); ++i)
    {
        float x,y;
        y = start.y()-(distance*(i/countInRow));
        if((i/countInRow)%2==0)
            x = start.x()+i%countInRow*distance;
        else
            x = start.x()+(countInRow-(i%countInRow+1))*distance;
        nodes.push_back(new Node(rootEntity_,protein_.at(i),QVector3D(x,y,0),prev,radius,connectionRadius,distance));
        if(i>0)
            nodes.at(i)->changeLocation(QVector3D(x,y,0));
         prev = nodes.at(i);
    }
    ui->genericCount->setValue(protein_.size());
}


void ProteinForm::setLastFileType(QString fileExt)
{
    //protein (*.protein);;proteinN (*.proteinN);;proteinS (*.proteinS)
    if(fileExt == "protein")
        selfilter = tr("protein (*.protein)");
    else if(fileExt == "proteinN")
        selfilter = tr("proteinN (*.proteinN)");
    else if(fileExt == "proteinS")
        selfilter = tr("proteinS (*.proteinS)");

}
