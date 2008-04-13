#ifndef QRESULTSDELEGATE_H_
#define QRESULTSDELEGATE_H_

#include <QModelIndex>
#include <QPainter>
#include <QItemDelegate>
#include <QTextDocument>
//#include "qResultsRow.h"
#include <map>
#include <set>
#include "threadPool.h"
#include "rawContainer.h"
#include "../control/rawUrl.h"
#include "../control/resultsColumns.h"
#include "myWindow.h"

#include <iostream>
using namespace std;

#define QCOLORHYPERLINK QColor(0,0,153)

class qResultsDelegate : public QItemDelegate {
  Q_OBJECT

public:
  qResultsDelegate(myWindow *mw, tdParam *param) : QItemDelegate(0) { this->mw = mw; tdp = param; }

  /** This method is called when a row needs to be painted
   * It manages the position of every peace of information
   * */
  void paint(QPainter *painter, const QStyleOptionViewItem &option,
            const QModelIndex &index) const;
  /** This method computes the minimum size of the row
   * */
  QSize sizeHint(const QStyleOptionViewItem &option,
                const QModelIndex &index) const;
  /* This method paints the textual informations of the row
   * */
  void drawDisplay(QPainter *painter, const QStyleOptionViewItem &option,
                                const QModelIndex &index, int offset) const;
  /* This method shows the image thumbnail, if available
   * */
  int drawThumbnail(QPainter *painter, const QStyleOptionViewItem &option,
                                const QModelIndex &index) const;
  /* This method paints a single textual information with different styles
   * */
  void drawText(QPainter *painter, int x, int y, int width, int heigt, int flags,
                            const QString & text, int weight = QFont::Normal,
                            bool underline = false, const QColor & color = Qt::black,
                            const QFont::Style &style = QFont::StyleNormal) const;
  /* The method is used to get the column data from the index.
   * */
  QString getField(string column, const QModelIndex &index) const;
  
  /* This method paints a single textual information with different styles
   * */
  QRectF simulDrawText(QPainter *painter, int x, int y, int width, int heigt, int flags,
                            const QString & text, int weight = QFont::Normal,
                            bool underline = false, const QColor & color = Qt::black,
                            const QFont::Style &style = QFont::StyleNormal) const;
  
private:
  myWindow *mw;
  tdParam *tdp;
};

#endif /*QRESULTSDELEGATE_H_*/

