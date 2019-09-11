#include "Q7zip.h"

PrivateProcessQueue* PrivateProcessQueue::s_ProcessQueue = new PrivateProcessQueue();
QTimer* PrivateProcessQueue::s_queryHeadler = new QTimer();
