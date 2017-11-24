#include "widget.h"
#include "ui_widget.h"



Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    dpi = qApp->primaryScreen()->logicalDotsPerInchX() / 120.0;
    temp = QString("%1").arg(qApp->primaryScreen()->logicalDotsPerInchX() / 120.0);
    qDebug()<<temp;
    QString FileName = QCoreApplication::applicationDirPath() + "/config.ini";
    QSettings *pConfig = new QSettings(FileName, QSettings::IniFormat);
    y_Color = getColorFromArray(pConfig->value("Basic/Color").toByteArray());
    y_displayC = pConfig->value("Basic/DisplaySize").toInt();
    DownloadFlow = pConfig->value("Basic/DownloadFlow").toLongLong();
    UploadFlow = pConfig->value("Basic/UploadFlow").toLongLong();
    preDownloadFlow = pConfig->value("Basic/preDownloadFlow").toLongLong();
    preUploadFlow = pConfig->value("Basic/preUploadFlow").toLongLong();
    delete pConfig;
    qDebug()<<preDownloadFlow<<DownloadFlow<<UploadFlow<<preUploadFlow;

    QPalette pa;
    pa.setColor(QPalette::WindowText,y_Color);



    UploadSpeed_Label = new QLabel(this);
    UploadSpeed_Label->setPalette(pa);
    DownloadSpeed_Label = new QLabel(this);
    DownloadSpeed_Label->setPalette(pa);
    UploadFlow_Label = new QLabel(this);
    UploadFlow_Label->setPalette(pa);
    DownloadFlow_Label = new QLabel(this);
    DownloadFlow_Label->setPalette(pa);
    y_timer = new QTimer(this);

    youyou = QPixmap(QString(":/new/prefix1/youyou_%1").arg(y_displayC));

//    QFont font  = qApp->font();
//    font.setPixelSize(dpi*20);
//    qApp->setFont(font);



    this->setWindowFlags(Qt::FramelessWindowHint | Qt::Tool);
    this->setAttribute(Qt::WA_TranslucentBackground, true);


    y_ZeroMenu = new QAction("清空流量",this);
    y_ZeroMenu->setIcon(QIcon(youyou));

    y_YouMenu = new QMenu(this);
    y_YouMenu->addAction(y_ZeroMenu);


    y_DisplayMenu = new QMenu("Display", this);
    y_DisplayMenu->setIcon(QIcon(youyou));
    y_DisplayMenu->addAction(QIcon(youyou),"80%");
    y_DisplayMenu->addAction(QIcon(youyou),"100%");
//    y_DisplayMenu->addAction("150%");
//    y_DisplayMenu->addAction("200%");


    y_QuitMenu = new QAction("Quit",this);
    y_QuitMenu->setIcon(QIcon(youyou));

    y_TrayMenu = new QMenu(this);
    y_TrayMenu->addMenu(y_DisplayMenu);
    y_TrayMenu->addAction(y_QuitMenu);


    y_TrayIcon = new QSystemTrayIcon(this);
    y_TrayIcon->setIcon(QIcon(youyou));
    y_TrayIcon->setContextMenu(y_TrayMenu);
    y_TrayIcon->show();

    initText();

    connect(y_QuitMenu,SIGNAL(triggered()),this, SLOT(quitApp_slot()));
    connect(y_ZeroMenu,SIGNAL(triggered()),this, SLOT(clear_slot()));
    connect(y_timer, SIGNAL(timeout()), this, SLOT(timeout_slot()));
    connect(y_DisplayMenu,SIGNAL(triggered(QAction*)),this,SLOT(changeDisplay_slot(QAction*)));

    appAutoRun(true);

    y_timer->start(1000);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::quitApp_slot(void){
        QString FileName = QCoreApplication::applicationDirPath() + "/config.ini";
        QSettings *pConfig = new QSettings(FileName, QSettings::IniFormat);
        pConfig->setValue("Basic/preUploadFlow",preOut);
        pConfig->setValue("Basic/preDownloadFlow",preIn);
        pConfig->setValue("Basic/DownloadFlow",preIn+DownloadFlow-preDownloadFlow);
        pConfig->setValue("Basic/UploadFlow",preOut+UploadFlow-preUploadFlow);
        delete pConfig;
    qApp->quit();
}

void Widget::clear_slot(void){

        QString FileName = QCoreApplication::applicationDirPath() + "/config.ini";
        QSettings *pConfig = new QSettings(FileName, QSettings::IniFormat);
        UploadFlow = 0;
        DownloadFlow = 0;
        preUploadFlow =preOut;
        preDownloadFlow = preIn;
        pConfig->setValue("Basic/UploadFlow",UploadFlow);
        pConfig->setValue("Basic/DownloadFlow",DownloadFlow);
        pConfig->setValue("Basic/preUploadFlow",preOut);
        pConfig->setValue("Basic/preDownloadFlow",preIn);
//        qDebug()<<preUploadFlow<<preOut<<pConfig->value("Basic/preUploadFlow").toLongLong();

        delete pConfig;
        update();
        QMessageBox* message = new QMessageBox(QMessageBox::NoIcon,"oyou!oyou~","oyou!oyou~",QMessageBox::Yes,this);
        message->setIconPixmap(youyou);
        message->show();
}

void Widget::timeout_slot(void){
    if(getNetworkInfo()){
        UploadSpeed_Label->setText(QString("↑ "+UploadSpeed));
//        UploadSpeed_Label->setText(QString("%1").arg(dpi));
        UploadSpeed_Label->adjustSize();
        DownloadSpeed_Label->setText(QString("↓ "+DownloadSpeed));
//        DownloadFlow_Label->setText(temp);
//        qDebug()<<temp;
        DownloadSpeed_Label->adjustSize();
        UploadFlow_Label->setText(QString("↑ "+DisplayUploadFlow));
//        QScreen *pscreen = qApp->primaryScreen();
//        UploadFlow_Label->setText(QString("%1").arg(pscreen->physicalDotsPerInch() * pscreen->physicalSize().width() * y_displayC / 5316000));
        UploadFlow_Label->adjustSize();
        qDebug()<<DisplayDownloadFlow;
        DownloadFlow_Label->setText(QString("↓ "+DisplayDownloadFlow));
        DownloadFlow_Label->adjustSize();
    }
    update();
}

void Widget::changeDisplay_slot(QAction* action)
{
    y_displayC = action->text().left(action->text().length() - 1).toInt();
    QString FileName = QCoreApplication::applicationDirPath() + "/config.ini";
    QSettings *pConfig = new QSettings(FileName, QSettings::IniFormat);
    pConfig->setValue("Basic/DisplaySize", y_displayC);
    delete pConfig;


    youyou = QPixmap(QString(":/new/prefix1/youyou_%1").arg(y_displayC));

    QFont font  = qApp->font();
    QScreen *pscreen = qApp->primaryScreen();
    dpi = (pscreen->physicalDotsPerInch() * pscreen->physicalSize().width() * y_displayC / 5316000)*1.5;
    font.setPixelSize(20 * dpi);
    qApp->setFont(font);

}

bool Widget::getNetworkInfo(void){
    bool bRet = false;
#ifdef Q_OS_WIN32

    GetIfTable func_GetIfTable;
    QLibrary lib;
    lib.setFileName("iphlpapi.dll");
    lib.load();
    func_GetIfTable = (GetIfTable)lib.resolve("GetIfTable");
    if(NULL != func_GetIfTable){
        PMIB_IFTABLE y_Table = NULL;
        DWORD y_dwAdapters = 0;

        func_GetIfTable(y_Table,&y_dwAdapters,false);

        y_Table = (PMIB_IFTABLE)new BYTE[y_dwAdapters];

        int nowTime = QDateTime().currentDateTime().toString("zzz").toInt();
        func_GetIfTable(y_Table,&y_dwAdapters,false);
        long long NowIn = 0;
        long long NowOut = 0;
        qDebug()<<NowIn<<"NowIn";
        QList<unsigned int> typeList;

        for(unsigned int i = 0 ; i < y_Table->dwNumEntries ; i ++){
            MIB_IFROW Row = y_Table->table[i];
            bool yExist = false;
            for(int j = 0 ; j < typeList.size() ; j ++){
                if(typeList[j] == Row.dwType){
                    yExist = true;
                    break;
                }
            }
            if(yExist == false && (Row.dwInOctets!=0||Row.dwOutOctets!=0)){
                typeList.append(Row.dwType);
                NowIn += (long long)Row.dwInOctets;
                NowOut += (long long)Row.dwOutOctets;
            }
        }
        delete [] y_Table;
//        qDebug("%lld %lld",preIn,preOut);
        qDebug()<<NowIn<<"Now";
        if(preIn!=0 && preOut!=0){
            double coeffcient = (1000 + nowTime - preTime) / 1000.0;
//            qDebug("%lf %lf %lld %lld",double(NowIn - preIn)/coeffcient,double(NowOut - preOut)/coeffcient,NowIn,NowOut);
            QStringList speedList = getSpeedInfo(double(NowIn - preIn)/coeffcient , double(NowOut - preOut)/coeffcient).split("|");
            UploadSpeed = speedList.at(1);
            DownloadSpeed = speedList.at(0);
            if(NowIn-preDownloadFlow>=0 && NowOut-preUploadFlow>=0){
//                qDebug()<<NowOut<<UploadFlow.toLongLong()<<preUploadFlow<<"!!!!!!!!!!!!!!";
                QStringList flowList = getFlowInfo((long double)(NowIn+DownloadFlow-preDownloadFlow),(long double)(NowOut+UploadFlow-preUploadFlow)).split("|");
//                qDebug()<<DisplayUploadFlow;
                DisplayUploadFlow = flowList.at(1);
                DisplayDownloadFlow = flowList.at(0);

                bRet = true;
            }
        }
        preOut = NowOut;
        preIn = NowIn;
        preTime = nowTime;
    }
#endif
    return bRet;
}

QString Widget::getSpeedInfo(double downloadSpeed ,double uploadSpeed){
    QString speedTitle[4] = {"K/s","KB/s","MB/s","GB/s"};
    int down = 0 ;int up = 0;
    for(int i = 0 ; i < 3 ; i ++){
        if(downloadSpeed >= 1024.0){
            down++;
            downloadSpeed/=1024.0;
        }
        if(uploadSpeed >= 1024.0){
            up++;
            uploadSpeed/=1024.0;
        }
    }
    downloadSpeed = (double)((int)(downloadSpeed * 100)) / 100;
    uploadSpeed = (double)((int)(uploadSpeed * 100)) / 100;
    QString ret = QString("%1%2|%3%4").arg(downloadSpeed).arg(speedTitle[down]).arg(uploadSpeed).arg(speedTitle[up]);
    return ret;
}

QString Widget::getFlowInfo(long double downloadFlow1,long double uploadFlow1){
    QString flowTitle[4] = {"K","KB","MB","GB"};
    int down = 0 ;int up = 0;
    for(int i = 0 ; i < 3 ; i ++){
        if(downloadFlow1 >= 1024.0){
            down++;
            downloadFlow1/=1024.0;
        }
        if(uploadFlow1 >= 1024.0){
            up++;
            uploadFlow1 /=1024.0;
        }
    }
    downloadFlow1 = (long double)((long long)(downloadFlow1 * 100)) / 100;
    uploadFlow1 = (long double)((long long)(uploadFlow1 * 100)) / 100;
    QString ret = QString("%1%2|%3%4").arg(double(downloadFlow1)).arg(flowTitle[down]).arg(double(uploadFlow1)).arg(flowTitle[up]);
    return ret;
}

QColor Widget::getColorFromArray(QByteArray array){
    QTextStream stream(array);
    QString r,g,b;
    stream>>r>>g>>b;
    return QColor(r.toInt(),g.toInt(),b.toInt());
}

void Widget::paintEvent(QPaintEvent *event)
{
    QScreen *pscreen = qApp->primaryScreen();
    QSize screenSize = pscreen->size();
    //if screen change,to confirm widget will fit new screen
    if(y_preScreenSize != screenSize)
    {
        QFont font  = qApp->font();
        dpi = (pscreen->physicalDotsPerInch() * pscreen->physicalSize().width() * y_displayC / 5316000) *1.5;
        font.setPixelSize(dpi*20);
        qApp->setFont(font);
        y_preScreenSize = screenSize;
    }

    QPoint topleft(0,0);

    QPainter painter(this);
    painter.drawPixmap(topleft,youyou);


    setFixedSize(600 * dpi, 330 * dpi);

    initText();

    QWidget::paintEvent(event);
}

void Widget::mousePressEvent(QMouseEvent *event){
    if(event->buttons() & Qt::LeftButton){
        y_dragPosition = event->globalPos() - frameGeometry().topLeft();
        event->accept();
    }else if(event->buttons() & Qt::RightButton){

    }
    QWidget::mousePressEvent(event);
}

void Widget::mouseMoveEvent(QMouseEvent *event){
    if(event->buttons() == Qt::LeftButton){
        move(event->globalPos()-y_dragPosition);
        event->accept();
    }
    QWidget::mouseMoveEvent(event);
}

//void Widget::mouseReleaseEvent(QMouseEvent *event){

//}

void Widget::contextMenuEvent(QContextMenuEvent *event){
    y_YouMenu->exec(QCursor::pos());
    QWidget::contextMenuEvent(event);
}
void Widget::appAutoRun(bool bAutoRun)
{
    QSettings *reg = new QSettings("HKEY_CURRENT_USER\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run",QSettings::NativeFormat);
    QString appName = "oyouoyou";
    if (bAutoRun)
    {
        QString strAppPath=QCoreApplication::applicationFilePath();
        strAppPath.replace('/','\\');
        reg->setValue(appName,strAppPath);
    }
    else
    {
        reg->setValue(appName,"");
    }
}

void Widget::initText(){
    UploadSpeed_Label->move(110 * dpi,10*dpi);
    DownloadSpeed_Label->move(110 * dpi, 30 * dpi);
    UploadFlow_Label->move(110 * dpi, 50 * dpi);
    DownloadFlow_Label->move(110 * dpi, 70 * dpi);
}
