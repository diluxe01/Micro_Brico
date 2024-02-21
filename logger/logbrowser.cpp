#include "logbrowser.h"

#include <QMetaType>

#include "logbrowserdialog.h"

LogBrowser::LogBrowser(QObject *parent) :
    QObject(parent)
{
    qRegisterMetaType<QtMsgType>("QtMsgType");
    browserDialog = new LogBrowserDialog;
    connect(this, SIGNAL (sendMessage(QtMsgType,QString)), browserDialog, SLOT (outputMessage(QtMsgType,QString)), Qt::QueuedConnection);
    browserDialog->show();
}

LogBrowser::~LogBrowser()
{
    delete browserDialog;
}
LogBrowser::show()
{
    browserDialog->show();
}

LogBrowser::close()
{
    browserDialog->close();
}
void LogBrowser::outputMessage(QtMsgType type, const QString &msg)
{
    emit sendMessage( type, msg );
}
