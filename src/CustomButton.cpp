#include "CustomButton.h"
#include <QPainter>
#include <QMouseEvent>
#include <QPropertyAnimation>
#include <QGraphicsOpacityEffect>
#include <QTimer>

CustomButton::CustomButton(const QString &text, QWidget *parent)
    : QPushButton(text, parent)
    , m_selected(false)
    , m_hovered(false)
    , m_pressed(false)
{
    // 设置按钮基本属性
    setCheckable(false);
    setFocusPolicy(Qt::NoFocus);
    
    // 初始化样式
    updateStyle();
    
    // 设置最小尺寸
    setMinimumSize(80, 32);
}

void CustomButton::setSelected(bool selected)
{
    if (m_selected != selected) {
        m_selected = selected;
        updateStyle();
        emit selectedChanged(selected);
    }
}

bool CustomButton::isSelected() const
{
    return m_selected;
}

void CustomButton::setHovered(bool hovered)
{
    if (m_hovered != hovered) {
        m_hovered = hovered;
        updateStyle();
    }
}

bool CustomButton::isHovered() const
{
    return m_hovered;
}

void CustomButton::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)
    
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    
    // 获取按钮矩形
    QRect buttonRect = rect().adjusted(1, 1, -1, -1);
    
    // 根据状态设置颜色
    QColor backgroundColor;
    QColor borderColor;
    QColor textColor = QColor(255, 255, 255); // 白色文字
    
    if (m_selected) {
        backgroundColor = QColor(74, 158, 255); // #4a9eff 亮蓝色
        borderColor = QColor(74, 158, 255);
    } else if (m_pressed) {
        backgroundColor = QColor(58, 68, 81); // #3a4451 灰蓝色
        borderColor = QColor(58, 68, 81);
    } else if (m_hovered) {
        backgroundColor = QColor(58, 68, 81); // #3a4451 灰蓝色
        borderColor = QColor(58, 68, 81);
    } else {
        backgroundColor = QColor(42, 52, 65); // #2a3441 中蓝色
        borderColor = QColor(58, 68, 81); // #3a4451 灰蓝色
    }
    
    // 绘制背景
    painter.setBrush(QBrush(backgroundColor));
    painter.setPen(QPen(borderColor, 1));
    painter.drawRoundedRect(buttonRect, 4, 4);
    
    // 绘制文字
    painter.setPen(textColor);
    painter.setFont(font());
    painter.drawText(buttonRect, Qt::AlignCenter, text());
}

void CustomButton::enterEvent(QEvent *event)
{
    Q_UNUSED(event)
    setHovered(true);
    QPushButton::enterEvent(event);
}

void CustomButton::leaveEvent(QEvent *event)
{
    Q_UNUSED(event)
    setHovered(false);
    QPushButton::leaveEvent(event);
}

void CustomButton::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        m_pressed = true;
        updateStyle();
        
        // 创建点击动画效果
        QTimer::singleShot(100, this, [this]() {
            m_pressed = false;
            updateStyle();
        });
    }
    QPushButton::mousePressEvent(event);
}

void CustomButton::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        m_pressed = false;
        updateStyle();
    }
    QPushButton::mouseReleaseEvent(event);
}

void CustomButton::updateStyle()
{
    // 触发重绘
    update();
}

QString CustomButton::getStyleForCurrentState() const
{
    QString baseStyle = QString(
        "CustomButton {"
        "    border: 1px solid %1;"
        "    border-radius: 4px;"
        "    padding: 8px 16px;"
        "    color: #ffffff;"
        "    font-size: 12px;"
        "    font-weight: normal;"
        "    background-color: %2;"
        "}"
    );
    
    if (m_selected) {
        return baseStyle.arg("#4a9eff", "#4a9eff");
    } else if (m_pressed) {
        return baseStyle.arg("#3a4451", "#3a4451");
    } else if (m_hovered) {
        return baseStyle.arg("#3a4451", "#3a4451");
    } else {
        return baseStyle.arg("#3a4451", "#2a3441");
    }
}