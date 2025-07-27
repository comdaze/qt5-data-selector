#ifndef TIMEWIDGET_H
#define TIMEWIDGET_H

#include <QWidget>
#include <QDateTime>

class CustomButton;
class QButtonGroup;
class QDateTimeEdit;
class QVBoxLayout;
class QHBoxLayout;
class QLabel;

/**
 * @brief 时间控件类
 * 
 * 提供时间颗粒度选择、时间范围设置和快捷时间选择功能
 * 支持15分钟、1小时、1天的时间颗粒度，以及前一天、前三天的快捷选择
 */
class TimeWidget : public QWidget
{
    Q_OBJECT

public:
    /**
     * @brief 时间颗粒度枚举
     */
    enum TimeGranularity {
        Minutes15,  // 15分钟
        Hour1,      // 1小时
        Day1        // 1天
    };

    /**
     * @brief 构造函数
     * @param parent 父窗口
     */
    explicit TimeWidget(QWidget *parent = nullptr);

    /**
     * @brief 获取当前开始时间
     * @return 开始时间
     */
    QDateTime getStartTime() const;
    
    /**
     * @brief 获取当前结束时间
     * @return 结束时间
     */
    QDateTime getEndTime() const;
    
    /**
     * @brief 获取当前时间颗粒度
     * @return 时间颗粒度
     */
    TimeGranularity getGranularity() const;
    
    /**
     * @brief 设置时间范围
     * @param start 开始时间
     * @param end 结束时间
     */
    void setTimeRange(const QDateTime &start, const QDateTime &end);

signals:
    /**
     * @brief 时间范围变化信号
     * @param start 开始时间
     * @param end 结束时间
     */
    void timeRangeChanged(const QDateTime &start, const QDateTime &end);
    
    /**
     * @brief 时间颗粒度变化信号
     * @param granularity 时间颗粒度
     */
    void granularityChanged(TimeGranularity granularity);
    
    /**
     * @brief 时间验证错误信号
     * @param error 错误信息
     */
    void validationError(const QString &error);

private slots:
    /**
     * @brief 时间颗粒度按钮点击槽函数
     */
    void onGranularityButtonClicked();
    
    /**
     * @brief 快捷时间按钮点击槽函数
     */
    void onQuickTimeButtonClicked();
    
    /**
     * @brief 日期时间变化槽函数
     */
    void onDateTimeChanged();

private:
    /**
     * @brief 设置用户界面
     */
    void setupUI();
    
    /**
     * @brief 创建时间颗粒度按钮组
     */
    void createGranularityButtons();
    
    /**
     * @brief 创建快捷时间按钮组
     */
    void createQuickTimeButtons();
    
    /**
     * @brief 创建时间范围编辑器
     */
    void createTimeRangeEditors();
    
    /**
     * @brief 更新时间范围（根据天数）
     * @param daysBack 往前的天数
     */
    void updateTimeRange(int daysBack);
    
    /**
     * @brief 验证时间范围
     * @return 如果时间范围有效返回true
     */
    bool validateTimeRange() const;
    
    /**
     * @brief 验证时间格式
     * @param dateTime 要验证的时间
     * @return 如果时间格式有效返回true
     */
    bool validateTimeFormat(const QDateTime &dateTime) const;
    
    /**
     * @brief 显示错误信息
     * @param error 错误信息
     */
    void showError(const QString &error);
    
    /**
     * @brief 清除错误信息
     */
    void clearError();
    
    /**
     * @brief 发射时间范围变化信号
     */
    void emitTimeRangeChanged();

private:
    // UI组件
    QButtonGroup *m_granularityGroup;    // 时间颗粒度按钮组
    QButtonGroup *m_quickTimeGroup;      // 快捷时间按钮组
    QDateTimeEdit *m_startTimeEdit;      // 开始时间编辑器
    QDateTimeEdit *m_endTimeEdit;        // 结束时间编辑器
    QLabel *m_titleLabel;                // 标题标签
    QLabel *m_errorLabel;                // 错误提示标签
    
    // 布局
    QVBoxLayout *m_mainLayout;           // 主布局
    QHBoxLayout *m_granularityLayout;    // 颗粒度按钮布局
    QHBoxLayout *m_quickTimeLayout;      // 快捷时间按钮布局
    QHBoxLayout *m_timeRangeLayout;      // 时间范围布局
    
    // 状态
    TimeGranularity m_currentGranularity; // 当前时间颗粒度
    bool m_updatingTime;                   // 是否正在更新时间（防止递归）
};

#endif // TIMEWIDGET_H