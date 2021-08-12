#ifndef DEF_QHOVERLABEL_H
#define DEF_QHOVERLABEL_H

#include "QStringPlus.h"
#include "QPlus.h"
#include "main.h"
#include <QLabel>
#include <QtGlobal>

class QHoverLabel : public QLabel
{
    Q_OBJECT

    public:
        QHoverLabel(QString path, QString name, QWidget* parent = nullptr) : QLabel(parent)
        {
            m_notHoverPixmap = getQPixmap(path + ".png");
            m_hoverPixmap = getQPixmap(path + "Hover.png");
            setCursor(Qt::PointingHandCursor);
            //setScaledContents(true);
            setPixmap(m_notHoverPixmap);
            QSize s = m_notHoverPixmap.size();
            setMaximumSize(s.width(), s.height()); // otherwise QLabel bounding box goes crazy
            //setAlignment(Qt::AlignTop); // only if removes setMaximumSize setAlignement works
            //setToolTip(firstUppercase(QObject::tr(/*path*/name.toStdString().c_str())));
            setToolTip(firstUppercase(translator.translate("resources", name.toStdString().c_str())));
        }

    // https://wiki.qt.io/Clickable_QLabel
    signals:
        void clicked();

    protected:
        void mousePressEvent(QMouseEvent* event) override
        {
            Q_UNUSED(event) // can't do another way ?

            emit clicked();
        }

        void enterEvent(QEnterEvent* ev) override
        {
            Q_UNUSED(ev)

            setPixmap(m_hoverPixmap);
            //QLabel::enterEvent(ev);
        }
        void leaveEvent(QEvent* ev) override
        {
            Q_UNUSED(ev)

            setPixmap(m_notHoverPixmap);
        }

    private:
        QPixmap m_notHoverPixmap,
                m_hoverPixmap;

};

#endif
