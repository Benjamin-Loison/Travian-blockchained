#ifndef DEF_QHOVERLABEL_H
#define DEF_QHOVERLABEL_H

#include "QStringPlus.h"
#include "QPlus.h"
#include <QLabel>
#include <QtGlobal>

class QHoverLabel : public QLabel
{
    Q_OBJECT

    public:
        QHoverLabel(QString path, QWidget* parent = nullptr) : QLabel(parent)
        {
            m_notHoverPixmap = getQPixmap(path + ".png");
            m_hoverPixmap = getQPixmap(path + "Hover.png");
            setCursor(Qt::PointingHandCursor);
            //setScaledContents(true);
            setPixmap(m_notHoverPixmap);
            QSize s = m_notHoverPixmap.size();
            setMaximumSize(s.width(), s.height()); // otherwise QLabel bounding box goes crazy
            //setAlignment(Qt::AlignTop); // only if removes setMaximumSize setAlignement works
            setToolTip(QObject::tr(firstUppercase(path).toStdString().c_str()));
        }

    protected:
        void enterEvent(QEnterEvent* ev) override
        {
            setPixmap(m_hoverPixmap);
            //QLabel::enterEvent(ev);
        }
        void leaveEvent(QEvent* ev) override
        {
            setPixmap(m_notHoverPixmap);
        }

    private:
        QPixmap m_notHoverPixmap,
                m_hoverPixmap;

};

#endif
