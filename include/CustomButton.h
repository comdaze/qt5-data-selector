#ifndef CUSTOMBUTTON_H
#define CUSTOMBUTTON_H

#include <QPushButton>
#include <QWidget>
#include <QString>
#include <QEvent>
#include <QMouseEvent>
#include <QPaintEvent>

/**
 * @brief 自定义按钮类
 * 
 * 继承自QPushButton，提供选中状态管理、悬停效果和自定义样式
 * 支持选中/未选中状态切换，以及鼠标悬停视觉反馈
 */
class CustomButton : public QPushButton
{
    Q_OBJECT

public:
    /**
     * @brief 构造函数
     * @param text 按钮文本
     * @param parent 父窗口
     */
    explicit CustomButton(const QString &text, QWidget *parent = nullptr);
    
    /**
     * @brief 设置选中状态
     * @param selected 是否选中
     */
    void setSelected(bool selected);
    
    /**
     * @brief 获取选中状态
     * @return 如果按钮被选中返回true
     */
    bool isSelected() const;
    
    /**
     * @brief 设置悬停状态
     * @param hovered 是否悬停
     */
    void setHovered(bool hovered);
    
    /**
     * @brief 获取悬停状态
     * @return 如果鼠标悬停返回true
     */
    bool isHovered() const;

signals:
    /**
     * @brief 选中状态变化信号
     * @param selected 新的选中状态
     */
    void selectedChanged(bool selected);

protected:
    /**
     * @brief 重写绘制事件
     * @param event 绘制事件
     */
    void paintEvent(QPaintEvent *event) override;
    
    /**
     * @brief 重写鼠标进入事件
     * @param event 事件对象
     */
    void enterEvent(QEvent *event) override;
    
    /**
     * @brief 重写鼠标离开事件
     * @param event 事件对象
     */
    void leaveEvent(QEvent *event) override;
    
    /**
     * @brief 重写鼠标按下事件
     * @param event 鼠标事件
     */
    void mousePressEvent(QMouseEvent *event) override;
    
    /**
     * @brief 重写鼠标释放事件
     * @param event 鼠标事件
     */
    void mouseReleaseEvent(QMouseEvent *event) override;

private:
    /**
     * @brief 更新按钮样式
     */
    void updateStyle();
    
    /**
     * @brief 获取当前状态的样式字符串
     * @return 样式字符串
     */
    QString getStyleForCurrentState() const;

private:
    bool m_selected;    // 选中状态
    bool m_hovered;     // 悬停状态
    bool m_pressed;     // 按下状态
};

#endif // CUSTOMBUTTON_H