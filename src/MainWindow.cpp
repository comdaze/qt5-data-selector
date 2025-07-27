#include "MainWindow.h"
#include "TimeWidget.h"
#include "DeviceWidget.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QApplication>
#include <QDebug>
#include <QFile>
#include <QTextStream>
#include <QIODevice>
#include <QFrame>
#include <QSizePolicy>
#include <QResizeEvent>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , m_timeWidget(nullptr)
    , m_deviceWidget(nullptr)
    , m_centralWidget(nullptr)
    , m_mainLayout(nullptr)
    , m_isInitialized(false)
{
    initializeWindow();
    setupUI();
    setupStyles();
    connectSignals();
    
    // 标记初始化完成
    m_isInitialized = true;
    updateWindowTitle();
}

MainWindow::~MainWindow()
{
    // Qt会自动清理子对象
}

void MainWindow::initializeWindow()
{
    setWindowTitle("Qt5 Data Selector");
    setMinimumSize(800, 600);
    resize(1000, 700);
}

void MainWindow::setupUI()
{
    // 创建中央窗口部件
    m_centralWidget = new QWidget(this);
    setCentralWidget(m_centralWidget);
    
    // 创建主布局 - 使用响应式垂直布局
    m_mainLayout = new QVBoxLayout(m_centralWidget);
    
    // 设置响应式边距 - 根据窗口大小调整
    updateLayoutMargins();
    
    // 设置控件间距
    m_mainLayout->setSpacing(12);
    
    // 创建时间控件
    m_timeWidget = new TimeWidget(this);
    m_timeWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    m_timeWidget->setMinimumHeight(120);
    m_timeWidget->setMaximumHeight(150);
    m_mainLayout->addWidget(m_timeWidget);
    
    // 添加分隔线
    QFrame *separator = new QFrame(this);
    separator->setFrameShape(QFrame::HLine);
    separator->setFrameShadow(QFrame::Sunken);
    separator->setStyleSheet("QFrame { color: #3a4451; margin: 4px 0px; }");
    m_mainLayout->addWidget(separator);
    
    // 创建设备控件
    m_deviceWidget = new DeviceWidget(this);
    m_deviceWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    m_deviceWidget->setMinimumHeight(300);
    m_mainLayout->addWidget(m_deviceWidget);
    
    // 设置布局拉伸因子 - 时间控件固定高度，设备控件可扩展
    m_mainLayout->setStretchFactor(m_timeWidget, 0);
    m_mainLayout->setStretchFactor(separator, 0);
    m_mainLayout->setStretchFactor(m_deviceWidget, 1);
    
    // 设置布局对齐方式
    m_mainLayout->setAlignment(Qt::AlignTop);
}

void MainWindow::setupStyles()
{
    // 加载QSS样式文件
    QFile styleFile(":/resources/styles.qss");
    if (!styleFile.open(QIODevice::ReadOnly)) {
        // 如果资源文件加载失败，尝试从文件系统加载
        styleFile.setFileName("resources/styles.qss");
        if (!styleFile.open(QIODevice::ReadOnly)) {
            qDebug() << "Failed to load styles.qss, using fallback styles";
            // 使用备用样式
            setStyleSheet(
                "QMainWindow {"
                "    background-color: #1a2332;"
                "    color: #ffffff;"
                "    font-family: 'Microsoft YaHei', Arial, sans-serif;"
                "    font-size: 12px;"
                "}"
                "QWidget {"
                "    background-color: #1a2332;"
                "    color: #ffffff;"
                "    font-family: 'Microsoft YaHei', Arial, sans-serif;"
                "    font-size: 12px;"
                "}"
            );
            return;
        }
    }
    
    // 读取样式内容并应用
    QTextStream stream(&styleFile);
    QString styleSheet = stream.readAll();
    styleFile.close();
    
    // 应用样式到整个应用程序
    qApp->setStyleSheet(styleSheet);
    
    qDebug() << "Successfully loaded and applied styles.qss";
}

void MainWindow::connectSignals()
{
    // 连接时间控件信号
    if (m_timeWidget) {
        connect(m_timeWidget, &TimeWidget::timeRangeChanged,
                this, &MainWindow::onTimeRangeChanged);
    }
    
    // 连接设备控件信号
    if (m_deviceWidget) {
        connect(m_deviceWidget, &DeviceWidget::selectionChanged,
                this, &MainWindow::onDeviceSelectionChanged);
        connect(m_deviceWidget, &DeviceWidget::searchTextChanged,
                this, &MainWindow::onSearchTextChanged);
    }
}

void MainWindow::onTimeRangeChanged(const QDateTime &start, const QDateTime &end)
{
    m_currentStartTime = start;
    m_currentEndTime = end;
    
    qDebug() << "Time range changed:" << start.toString() << "to" << end.toString();
    
    // 通知设备控件时间范围已变化，可能需要更新数据显示
    // 这里可以添加更多的数据同步逻辑
    if (m_deviceWidget) {
        // 可以在这里触发设备数据的重新加载或过滤
        // 例如：m_deviceWidget->updateDataForTimeRange(start, end);
    }
    
    // 发出时间范围变化的通知信号
    emit timeRangeUpdated(start, end);
    emit statusChanged(getStatusSummary());
}

void MainWindow::onDeviceSelectionChanged(const QStringList &selectedDevices)
{
    m_selectedDevices = selectedDevices;
    
    qDebug() << "Device selection changed:" << selectedDevices.size() << "devices selected";
    
    // 更新窗口标题显示选中的设备数量
    updateWindowTitle();
    
    // 可以在这里添加设备选择变化的处理逻辑
    // 例如：保存选择状态、通知其他组件等
    
    // 发出设备选择变化的通知信号
    emit deviceSelectionUpdated(selectedDevices);
    emit statusChanged(getStatusSummary());
}

void MainWindow::onSearchTextChanged(const QString &text)
{
    m_searchText = text;
    
    qDebug() << "Search text changed:" << text;
    
    // 实时搜索功能已经在DeviceWidget中实现
    // 这里可以添加额外的搜索相关逻辑
    // 例如：保存搜索历史、统计搜索结果等
    
    // 更新窗口标题和状态
    updateWindowTitle();
    emit statusChanged(getStatusSummary());
}

void MainWindow::updateWindowTitle()
{
    if (!m_isInitialized) {
        return;
    }
    
    QString title = "Qt5 Data Selector";
    
    // 添加选中设备数量信息
    if (!m_selectedDevices.isEmpty()) {
        title += QString(" - %1 devices selected").arg(m_selectedDevices.size());
    }
    
    // 添加搜索状态信息
    if (!m_searchText.isEmpty()) {
        title += QString(" - Searching: \"%1\"").arg(m_searchText);
    }
    
    setWindowTitle(title);
}

QString MainWindow::getStatusSummary() const
{
    QStringList statusParts;
    
    // 时间范围信息
    if (m_currentStartTime.isValid() && m_currentEndTime.isValid()) {
        statusParts << QString("Time: %1 to %2")
                      .arg(m_currentStartTime.toString("yyyy-MM-dd hh:mm"))
                      .arg(m_currentEndTime.toString("yyyy-MM-dd hh:mm"));
    }
    
    // 设备选择信息
    statusParts << QString("Devices: %1 selected").arg(m_selectedDevices.size());
    
    // 搜索信息
    if (!m_searchText.isEmpty()) {
        statusParts << QString("Search: \"%1\"").arg(m_searchText);
    }
    
    return statusParts.join(" | ");
}bool
 MainWindow::getCurrentTimeRange(QDateTime &start, QDateTime &end) const
{
    if (m_currentStartTime.isValid() && m_currentEndTime.isValid()) {
        start = m_currentStartTime;
        end = m_currentEndTime;
        return true;
    }
    return false;
}

QStringList MainWindow::getCurrentSelectedDevices() const
{
    return m_selectedDevices;
}

QString MainWindow::getCurrentSearchText() const
{
    return m_searchText;
}

void MainWindow::setTimeRange(const QDateTime &start, const QDateTime &end)
{
    if (m_timeWidget && start.isValid() && end.isValid() && start <= end) {
        m_timeWidget->setTimeRange(start, end);
        // The signal will be emitted by TimeWidget, which will update our internal state
    }
}

void MainWindow::setSelectedDevices(const QStringList &deviceIds)
{
    if (m_deviceWidget) {
        m_deviceWidget->setSelectedDevices(deviceIds);
        // The signal will be emitted by DeviceWidget, which will update our internal state
    }
}

void MainWindow::setSearchText(const QString &text)
{
    if (m_deviceWidget) {
        m_deviceWidget->setSearchText(text);
        // The signal will be emitted by DeviceWidget, which will update our internal state
    }
}

void MainWindow::updateLayoutMargins()
{
    if (!m_mainLayout) return;
    
    // 根据窗口大小动态调整边距
    int windowWidth = width();
    int windowHeight = height();
    
    int margin;
    if (windowWidth < 900 || windowHeight < 650) {
        // 小窗口 - 较小边距
        margin = 8;
    } else if (windowWidth < 1200 || windowHeight < 800) {
        // 中等窗口 - 标准边距
        margin = 12;
    } else {
        // 大窗口 - 较大边距
        margin = 16;
    }
    
    m_mainLayout->setContentsMargins(margin, margin, margin, margin);
    
    // 根据窗口大小调整控件间距
    int spacing = qMax(8, margin - 4);
    m_mainLayout->setSpacing(spacing);
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    QMainWindow::resizeEvent(event);
    
    // 窗口大小变化时更新布局边距
    updateLayoutMargins();
    
    // 根据窗口大小调整时间控件的高度
    if (m_timeWidget) {
        int windowHeight = height();
        if (windowHeight < 650) {
            m_timeWidget->setMaximumHeight(120);
        } else if (windowHeight < 800) {
            m_timeWidget->setMaximumHeight(140);
        } else {
            m_timeWidget->setMaximumHeight(160);
        }
    }
}