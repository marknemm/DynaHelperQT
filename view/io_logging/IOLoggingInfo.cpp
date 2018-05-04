#include "IOLoggingInfo.h"
#include "ui_IOLoggingInfo.h"
#include <QDesktopWidget>
#include <QDebug>
#include <QFont>


IOLoggingInfo::IOLoggingInfo(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::IOLoggingInfo),
    _corner("Left")
{
    ui->setupUi(this);
    setWindowFlags(Qt::Window | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);

    QFont infoFont("Monospace");
    infoFont.setStyleHint(QFont::Monospace);
    infoFont.setPixelSize(14);
    infoFont.setBold(true);
    ui->eventInfo->setFont(infoFont);

    swapCorner();
    initFilters();
}


IOLoggingInfo::~IOLoggingInfo()
{
    delete ui;
}


void IOLoggingInfo::setHeader(const QString &header)
{
    ui->header->setText(header);
}


void IOLoggingInfo::updateEventInfo(const QString &eventInfo)
{
    ui->eventInfo->setText(eventInfo);
}


void IOLoggingInfo::initFilters()
{
    ui->centralwidget->installEventFilter(this);
    ui->header->installEventFilter(this);
    ui->eventInfo->installEventFilter(this);
}


bool IOLoggingInfo::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::MouseMove) {
        swapCorner();
    }
    return false; // Keep processing event.
}


void IOLoggingInfo::swapCorner()
{
    if (_corner == "Left") {
        QRect rec = QApplication::desktop()->screenGeometry();
        move(rec.topRight().x() - width(), rec.topRight().y());
        _corner = "Right";
    }
    else {
        move(0, 0);
        _corner = "Left";
    }
}
