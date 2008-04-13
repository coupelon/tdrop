#ifndef QINSERTEVENT_H
#define QINSERTEVENT_H

#include <QEvent>

#define QInsertEventNumber ((QEvent::Type) (QEvent::User+1))

class QInsertEvent : public QEvent {
  public:
  QInsertEvent(QString engine) : QEvent(QInsertEventNumber) {
    eng = engine;
  }
  
  QString getEngine() const { return eng; };
  
  private:
  QString eng;
};

#endif
