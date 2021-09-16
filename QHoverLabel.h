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
        QHoverLabel(QString path, QString name, bool isChoosen = false, QWidget* parent = nullptr) : QLabel(parent)
        {
            m_isChoosen = isChoosen;
            if(!m_isChoosen)
            {
                setCursor(Qt::PointingHandCursor);
                m_notHoverPixmap = getQPixmap(path + ".png");
            }
            m_hoverPixmap = getQPixmap(path + "Hover.png");
            setPixmap(m_isChoosen ? m_hoverPixmap : m_notHoverPixmap);
            QSize s = m_hoverPixmap.size();
            setMaximumSize(s.width(), s.height()); // otherwise QLabel bounding box goes crazy
            setToolTip(firstUppercase(translator.translate("resources", name.toStdString().c_str())));
        }

    // https://wiki.qt.io/Clickable_QLabel
    signals:
        void clicked();

    protected:
        void mousePressEvent(QMouseEvent* event) override
        {
            Q_UNUSED(event) // can't do another way ?

            if(!m_isChoosen)
                emit clicked();
        }

        void enterEvent(QEnterEvent* ev) override
        {
            Q_UNUSED(ev)

            if(!m_isChoosen)
                setPixmap(m_hoverPixmap);
            //QLabel::enterEvent(ev);
        }
        void leaveEvent(QEvent* ev) override
        {
            Q_UNUSED(ev)

            if(!m_isChoosen)
                setPixmap(m_notHoverPixmap);
        }

    private:
        bool m_isChoosen;
        QPixmap m_notHoverPixmap,
                m_hoverPixmap;

};

#endif
