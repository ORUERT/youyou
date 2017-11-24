#ifndef WIDGET_H
#define WIDGET_H

#include <QtWidgets>
//#include <

#ifdef Q_OS_WIN32
//to use dll directory,define then by self
#define MAX_INTERFACE_NAME_LEN 256
#define MAXLEN_PHYSADDR 8
#define MAXLEN_IFDESCR 256
#define ANY_SIZE 1

typedef struct _MIB_IFROW
{
    WCHAR wszName[MAX_INTERFACE_NAME_LEN];
    DWORD dwIndex;
    DWORD dwType;
    DWORD dwMtu;
    DWORD dwSpeed;
    DWORD dwPhysAddrLen;
    BYTE bPhysAddr[MAXLEN_PHYSADDR];
    DWORD dwAdminStatus;
    DWORD dwOperStatus;
    DWORD dwLastChange;
    DWORD dwInOctets;
    DWORD dwInUcastPkts;
    DWORD dwInNUcastPkts;
    DWORD dwInDiscards;
    DWORD dwInErrors;
    DWORD dwInUnknownProtos;
    DWORD dwOutOctets;
    DWORD dwOutUcastPkts;
    DWORD dwOutNUcastPkts;
    DWORD dwOutDiscards;
    DWORD dwOutErrors;
    DWORD dwOutQLen;
    DWORD dwDescrLen;
    BYTE bDescr[MAXLEN_IFDESCR];
} MIB_IFROW,*PMIB_IFROW;

typedef struct _MIB_IFTABLE
{
    DWORD dwNumEntries;
    MIB_IFROW table[ANY_SIZE];
} MIB_IFTABLE, *PMIB_IFTABLE;

typedef DWORD (*GetIfTable)(PMIB_IFTABLE, PULONG, BOOL);
#endif

struct CONFIG_S
{
    int PosX;
    int PosY;
    QColor Color;
    int Version;
};

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);

    ~Widget();

public slots:
    void quitApp_slot(void);
    void timeout_slot(void);
    void clear_slot(void);
    void changeDisplay_slot(QAction* action);
protected:
    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void appAutoRun(bool bAutoRun);
//    void mouseReleaseEvent(QMouseEvent *event);
    QString getSpeedInfo(double downloadSpeed ,double uploadSpeed);
    QString getFlowInfo(long double downloadFlow , long double uploadFlow);
    void contextMenuEvent(QContextMenuEvent *event);
    QColor getColorFromArray(QByteArray array);
    bool getNetworkInfo(void);

    void initText();

private:
    Ui::Widget *ui;
    QPoint y_dragPosition;
    QPixmap youyou;
    QSystemTrayIcon* y_TrayIcon;
    QAction* y_QuitMenu;
    QMenu* y_TrayMenu;
    QMenu* y_YouMenu;
    QMenu* y_DisplayMenu;
    QAction* y_ZeroMenu;
    QLabel* UploadSpeed_Label;
    QLabel* DownloadSpeed_Label;
    QLabel* UploadFlow_Label;
    QLabel* DownloadFlow_Label;
    QTimer* y_timer;
    float y_dpi;
    float dpi;
    QString UploadSpeed;
    QString DownloadSpeed;
    long long UploadFlow;
    long long DownloadFlow;
    QString DisplayUploadFlow;
    QString DisplayDownloadFlow;
    long long preUploadFlow;
    long long preDownloadFlow;
    QColor y_Color;
    QSize y_preScreenSize;
    long long preIn;
    long long preOut;
    int y_displayC;
    int preTime;

    QString temp;
};

#endif // WIDGET_H
