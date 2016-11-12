#include "RF24Interface.h"
#include "RF24Functions.h"

#include <QDebug>
#include <QThread>

bool RF24Interface::m_RF24InterfaceInitialized = false;

RF24Interface::RF24Interface(QObject *parent) : QObject(parent)
{
    qDebug() << "Constructor RF24Interface";
}

RF24Interface::~RF24Interface()
{
    qDebug() << "Destructor RF24Interface";
    if(m_thread->isRunning())
        m_functions->stop();
    delete m_functions;
}

void RF24Interface::init()
{
    qDebug() << "RF24Interface::init() " << RF24Interface::m_RF24InterfaceInitialized;
    QMutexLocker locker(&m_mutex);
    if(RF24Interface::m_RF24InterfaceInitialized)
        return;

    m_thread = new QThread();
    m_thread->setObjectName("RF24Interface");

    m_functions = new RF24Functions();
    m_functions->init();
    m_functions->moveToThread(m_thread);

    connect(m_thread, SIGNAL(started()), m_functions, SLOT(loop()));
    connect(m_functions, SIGNAL(finished()), this, SLOT(finishedSlot()));

    connect(m_thread, &QThread::finished, m_functions, &QObject::deleteLater);
    connect(m_thread, &QThread::finished, m_thread, &QObject::deleteLater);

    m_thread->start();
    qDebug() << "Thread STATUS: isRunning - " << m_thread->isRunning();
    RF24Interface::m_RF24InterfaceInitialized = true;
}

void RF24Interface::finishedSlot()
{
    qDebug() << "RF24Interface::finishedSlot()";
}
