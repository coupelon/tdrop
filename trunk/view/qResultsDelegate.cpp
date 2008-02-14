#include "qResultsDelegate.h"
 
#include "icons/page.xpm"
 
void qResultsDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option,
                          const QModelIndex &index) const{
    QStyleOptionViewItem opt = option;
    
    //If this item needs to be checked
    /*QRect checkRect;
    Qt::CheckState checkState = Qt::Unchecked;
    QVariant value = index.data(Qt::CheckStateRole);
    cerr << " 1 ";
    if (value.isValid()) {
        checkState = static_cast<Qt::CheckState>(value.toInt());
        checkRect = check(opt, opt.rect, value);
    }*/
    
    //Draw the background of the cell (selection)
    if (opt.showDecorationSelected && (opt.state & QStyle::State_Selected)) {
        QPalette::ColorGroup cg = opt.state & QStyle::State_Enabled
                                  ? QPalette::Normal : QPalette::Disabled;
        if (cg == QPalette::Normal && !(opt.state & QStyle::State_Active))
            cg = QPalette::Inactive;

        painter->fillRect(opt.rect, opt.palette.brush(cg, QPalette::Highlight));
    }
    
    //Draw the separation line
    painter->save();
    painter->setPen(Qt::gray);
    painter->drawLine(0,option.rect.y() + option.rect.height(),
                      option.rect.x()+option.rect.width(),option.rect.y() + option.rect.height());
    painter->drawLine(0,option.rect.y(), option.rect.x()+option.rect.width(),option.rect.y());
    painter->restore();

    opt.rect.setX(opt.rect.x() + 20);

    int offset = drawThumbnail(painter,opt,index);
    drawDisplay(painter,opt,index,offset+5);
}

int qResultsDelegate::drawThumbnail(QPainter *painter, const QStyleOptionViewItem &option,
                                    const QModelIndex &index) const {
    static map<string,rawContainer *> imgLoaded;
    static set<string> imgQueued;
    static threadPool thread;
    int lineSpacing = painter->fontMetrics().lineSpacing();
    
    QString text;
    if ((text = getField(RESULT_IMG,index)) == "") return 0;
    string image = text.toStdString();
    int imageWidth = 0;
    if (image != "") {
        if (imgLoaded.find(image) != imgLoaded.end()) {
            //Display image
            QImage img;
            rawContainer *r = imgLoaded.find(image)->second;
            img.loadFromData((uchar *) r->getContent(),r->getLength());
            int imageHeight = 3*lineSpacing-2;
            imageWidth = (int) (img.width ()/(double) img.height()*imageHeight);
            painter->drawImage(option.rect.x()+1,option.rect.y()+lineSpacing+1,img.scaledToHeight(imageHeight));
            painter->drawRect(option.rect.x(),option.rect.y()+lineSpacing,imageWidth+2,imageHeight+2);
            imageWidth+=2;
        } else {
            //Download the image
            boost::recursive_mutex::scoped_lock scoped_lock(thread.getMutex());
            //threadPool<rawUrl>::lock(thread);
            if (imgQueued.find(image) == imgQueued.end()) {
                imgQueued.insert(image);
                //threadPool<rawUrl>::unlock(thread);
                thread.createThread(*(new rawUrl(image,&imgLoaded,tdp,mw)));
            }// else threadPool<rawUrl>::unlock(thread);
        }
    }
    return imageWidth;
}

void qResultsDelegate::drawDisplay(QPainter *painter, const QStyleOptionViewItem &option,
                                    const QModelIndex &index, int offset) const {
    int lineSpacing = painter->fontMetrics().lineSpacing();
    QString text;
    int width = 0;

    painter->drawPixmap(1,option.rect.y()+1,QPixmap(page_xpm));

    if ((text = getField(RESULT_TITLE,index)) != "")
        width = (int) simulDrawText(painter,option.rect.x(),option.rect.y(),option.rect.width(),lineSpacing,
                 Qt::AlignLeft|Qt::AlignVCenter, text, QFont::Bold, true).width();
        drawText(painter,option.rect.x(),option.rect.y(),option.rect.width(),lineSpacing,
                 Qt::AlignLeft|Qt::AlignVCenter, text, QFont::Bold, true);
                 
    if ((text = getField(RESULT_ENGINES,index)) != "") {
        int engines = (int) simulDrawText(painter,option.rect.x()+option.rect.width()-width,option.rect.y(),width,lineSpacing,
                 Qt::AlignRight|Qt::AlignVCenter, text, QFont::Normal, false, Qt::lightGray, QFont::StyleItalic).width();
        if (option.rect.width() - width > engines + 20)
          width = engines;
        else width = option.rect.width() - width - 20;
        drawText(painter,option.rect.x()+option.rect.width()-width,option.rect.y(),width,lineSpacing,
                 Qt::AlignRight|Qt::AlignVCenter, text, QFont::Normal, false, Qt::lightGray, QFont::StyleItalic);
    }

    if ((text = getField(RESULT_ABSTRACT,index)) != "")
        drawText(painter,option.rect.x()+offset,option.rect.y()+lineSpacing,option.rect.width()-offset,2*lineSpacing,
                 Qt::AlignLeft|Qt::AlignVCenter|Qt::TextWordWrap, text);

    if ((text = getField(RESULT_URL,index)) != "")
      if (getField(RESULT_ABSTRACT,index) != "" || getField(RESULT_IMG,index) != "")
        drawText(painter,option.rect.x()+offset,option.rect.y()+3*lineSpacing,option.rect.width()-offset,lineSpacing,
                 Qt::AlignLeft|Qt::AlignVCenter, text, QFont::Normal, true, QCOLORHYPERLINK);
      else
        drawText(painter,option.rect.x()+offset,option.rect.y()+lineSpacing,option.rect.width()-offset,lineSpacing,
                 Qt::AlignLeft|Qt::AlignVCenter, text, QFont::Normal, true, QCOLORHYPERLINK);
}

void qResultsDelegate::drawText(QPainter *painter, int x, int y, int width, int height, int flags,
                                const QString & text, int weight, bool underline,
                                const QColor & color, const QFont::Style & style) const {
    painter->save();
    QRectF rect(x,y,width,height);
    QFont ft =  painter->font();
    ft.setWeight(weight);
    ft.setUnderline(underline);
    ft.setStyle(style);
    painter->setFont(ft);
    painter->setPen(color);
    painter->drawText(rect,flags,text);
    painter->restore();
}

QRectF qResultsDelegate::simulDrawText(QPainter *painter, int x, int y, int width, int height, int flags,
                                const QString & text, int weight, bool underline,
                                const QColor & color, const QFont::Style & style) const {
    QRectF ret;
    painter->save();
    QRectF rect(x,y,width,height);
    QFont ft =  painter->font();
    ft.setWeight(weight);
    ft.setUnderline(underline);
    ft.setStyle(style);
    painter->setFont(ft);
    painter->setPen(color);
    ret = painter->boundingRect(rect,flags,text);
    painter->restore();
    return ret;
}

QSize qResultsDelegate::sizeHint(const QStyleOptionViewItem &option,
                              const QModelIndex &index) const {
    if (getField(RESULT_ABSTRACT,index) != "" || getField(RESULT_IMG,index) != "")
      return QSize(option.rect.width(),(int) (option.fontMetrics.lineSpacing()*4)+2);
    else
      return QSize(option.rect.width(),(int) (option.fontMetrics.lineSpacing()*2)+2);
}

QString qResultsDelegate::getField(string column, const QModelIndex &index) const {
  unsigned int rowNumber = index.row();
  if (mw == NULL) return QString();
  if (rowNumber >= mw->getSManager()->getResults().size()) return QString();
  if (column == RESULT_ENGINES) 
    return mw->getSManager()->getResults()[rowNumber].getEngineList(3).c_str();
  return QString::fromLocal8Bit(mw->getSManager()->getResults()[rowNumber].getField(column).c_str());
}


