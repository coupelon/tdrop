#ifndef QHIDEEVENT_H
#define QHIDEEVENT_H

#include <QEvent>

#define QHideEventNumber ((QEvent::Type) (QEvent::User+2))

class qHideEvent : public QEvent {
  public:
  qHideEvent() : QEvent(QHideEventNumber) {}
};

#endif
