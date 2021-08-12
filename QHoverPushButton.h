#ifndef DEF_QHOVERPUSHBUTTON_H
#define DEF_QHOVERPUSHBUTTON_H

#include "QStringPlus.h"
#include "QPlus.h"
#include "main.h"
#include <QPushButton>
#include <QtGlobal>

class QHoverPushButton : public QPushButton
{
    Q_OBJECT

    public:
        QHoverPushButton(QString path, QString name, QWidget* parent = nullptr) : QPushButton(parent)
        {
            m_notHoverIcon = getQIcon(path + ".png");
            m_hoverIcon = getQIcon(path + "Hover.png");
            setCursor(Qt::PointingHandCursor);
            setIcon(m_notHoverIcon);
            //QSize s = m_notHoverIcon.size();
            //setMaximumSize(s.width(), s.height());
            setToolTip(firstUppercase(translator.translate("resources", name.toStdString().c_str())));
        }

    protected:
        void focusInEvent(QFocusEvent* e) override
        {
            setIcon(m_hoverIcon);
            //QLabel::enterEvent(ev);
        }
        void focusOutEvent(QFocusEvent* e) override
        {
            setIcon(m_notHoverIcon);
        }

    private:
        QIcon m_notHoverIcon,
              m_hoverIcon;

};

#endif
