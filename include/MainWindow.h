#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDateTime>
#include <QStringList>

class QResizeEvent;

class TimeWidget;
class DeviceWidget;
class QVBoxLayout;
class QHBoxLayout;

/**
 * @brief 主窗口类
 * 
 * 应用程序的主窗口，集成时间控件和设备控件
 * 负责组件间的通信和整体界面布局
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    /**
     * @brief 构造函数
     * @param parent 父窗口
     */
    explicit MainWindow(QWidget *parent = nullptr);
    
    /**
     * @brief 析构函数
     */
    ~MainWindow();
    
    /**
     * @brief 获取当前时间范围
     * @param start 输出开始时间
     * @param end 输出结束时间
     * @return 如果时间范围有效返回true
     */
    bool getCurrentTimeRange(QDateTime &start, QDateTime &end) const;
    
    /**
     * @brief 获取当前选中的设备列表
     * @return 设备ID列表
     */
    QStringList getCurrentSelectedDevices() const;
    
    /**
     * @brief 获取当前搜索文本
     * @return 搜索文本
     */
    QString getCurrentSearchText() const;
    
    /**
     * @brief 设置时间范围（程序化设置）
     * @param start 开始时间
     * @param end 结束时间
     */
    void setTimeRange(const QDateTime &start, const QDateTime &end);
    
    /**
     * @brief 设置选中的设备（程序化设置）
     * @param deviceIds 设备ID列表
     */
    void setSelectedDevices(const QStringList &deviceIds);
    
    /**
     * @brief 设置搜索文本（程序化设置）
     * @param text 搜索文本
     */
    void setSearchText(const QString &text);

signals:
    /**
     * @brief 应用程序状态变化信号
     * @param summary 状态摘要
     */
    void statusChanged(const QString &summary);
    
    /**
     * @brief 时间范围更新信号（用于通知外部组件）
     * @param start 开始时间
     * @param end 结束时间
     */
    void timeRangeUpdated(const QDateTime &start, const QDateTime &end);
    
    /**
     * @brief 设备选择更新信号（用于通知外部组件）
     * @param selectedDevices 选中的设备列表
     */
    void deviceSelectionUpdated(const QStringList &selectedDevices);

private slots:
    /**
     * @brief 时间范围变化槽函数
     * @param start 开始时间
     * @param end 结束时间
     */
    void onTimeRangeChanged(const QDateTime &start, const QDateTime &end);
    
    /**
     * @brief 设备选择变化槽函数
     * @param selectedDevices 已选择的设备ID列表
     */
    void onDeviceSelectionChanged(const QStringList &selectedDevices);
    
    /**
     * @brief 搜索文本变化槽函数
     * @param text 搜索文本
     */
    void onSearchTextChanged(const QString &text);

private:
    /**
     * @brief 设置用户界面
     */
    void setupUI();
    
    /**
     * @brief 设置样式
     */
    void setupStyles();
    
    /**
     * @brief 连接信号槽
     */
    void connectSignals();
    
    /**
     * @brief 初始化窗口属性
     */
    void initializeWindow();
    
    /**
     * @brief 更新窗口标题
     */
    void updateWindowTitle();
    
    /**
     * @brief 获取当前应用程序状态摘要
     * @return 状态摘要字符串
     */
    QString getStatusSummary() const;
    
    /**
     * @brief 更新布局边距以适应窗口大小
     */
    void updateLayoutMargins();

protected:
    /**
     * @brief 重写窗口大小变化事件以实现响应式布局
     * @param event 大小变化事件
     */
    void resizeEvent(QResizeEvent *event) override;

private:
    TimeWidget *m_timeWidget;      // 时间控件
    DeviceWidget *m_deviceWidget;  // 设备控件
    QWidget *m_centralWidget;      // 中央窗口部件
    QVBoxLayout *m_mainLayout;     // 主布局
    
    // 当前状态
    QDateTime m_currentStartTime;  // 当前开始时间
    QDateTime m_currentEndTime;    // 当前结束时间
    QStringList m_selectedDevices; // 当前选中的设备
    QString m_searchText;          // 当前搜索文本
    
    // 状态同步标志
    bool m_isInitialized;          // 是否已初始化完成
};

#endif // MAINWINDOW_H