# 设计文档

## 概述

本设计文档描述了基于Qt5的数据筛选器应用程序的技术架构和实现方案。该应用程序采用现代化的深蓝色主题界面，提供时间筛选、设备选择和关键字搜索功能。

## 架构

### 整体架构
```
┌─────────────────────────────────────────┐
│              MainWindow                 │
├─────────────────────────────────────────┤
│  ┌─────────────┐  ┌─────────────────┐   │
│  │ TimeWidget  │  │ DeviceWidget    │   │
│  │             │  │                 │   │
│  │ - 时间颗粒度  │  │ - 设备类型标签   │   │
│  │ - 时间范围    │  │ - 设备树形列表   │   │
│  │ - 快捷按钮    │  │ - 搜索功能      │   │
│  └─────────────┘  └─────────────────┘   │
└─────────────────────────────────────────┘
```

### 技术栈
- **UI框架**: Qt5 (QWidget)
- **布局管理**: QVBoxLayout, QHBoxLayout, QGridLayout
- **样式**: QSS (Qt Style Sheets)
- **数据模型**: QStandardItemModel, QTreeView
- **时间处理**: QDateTime, QDateTimeEdit

## 组件和接口

### 1. MainWindow (主窗口)
```cpp
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onTimeRangeChanged(const QDateTime &start, const QDateTime &end);
    void onDeviceSelectionChanged(const QStringList &selectedDevices);
    void onSearchTextChanged(const QString &text);

private:
    void setupUI();
    void setupStyles();
    
    TimeWidget *m_timeWidget;
    DeviceWidget *m_deviceWidget;
};
```

### 2. TimeWidget (时间控件)
```cpp
class TimeWidget : public QWidget
{
    Q_OBJECT

public:
    explicit TimeWidget(QWidget *parent = nullptr);
    
    enum TimeGranularity {
        Minutes15,
        Hour1,
        Day1
    };

signals:
    void timeRangeChanged(const QDateTime &start, const QDateTime &end);
    void granularityChanged(TimeGranularity granularity);

private slots:
    void onGranularityButtonClicked();
    void onQuickTimeButtonClicked();
    void onDateTimeChanged();

private:
    void setupUI();
    void updateTimeRange(int daysBack);
    
    QButtonGroup *m_granularityGroup;
    QButtonGroup *m_quickTimeGroup;
    QDateTimeEdit *m_startTimeEdit;
    QDateTimeEdit *m_endTimeEdit;
};
```

### 3. DeviceWidget (设备控件)
```cpp
class DeviceWidget : public QWidget
{
    Q_OBJECT

public:
    explicit DeviceWidget(QWidget *parent = nullptr);

signals:
    void selectionChanged(const QStringList &selectedDevices);

private slots:
    void onSearchTextChanged(const QString &text);
    void onItemChanged(QStandardItem *item);
    void onTabChanged(int index);

private:
    void setupUI();
    void setupDeviceTree();
    void filterDevices(const QString &filter);
    void updateSelection();
    
    QTabWidget *m_tabWidget;
    QLineEdit *m_searchEdit;
    QTreeView *m_deviceTree;
    QStandardItemModel *m_deviceModel;
    QLabel *m_selectedCountLabel;
};
```

### 4. CustomButton (自定义按钮)
```cpp
class CustomButton : public QPushButton
{
    Q_OBJECT

public:
    explicit CustomButton(const QString &text, QWidget *parent = nullptr);
    
    void setSelected(bool selected);
    bool isSelected() const;

protected:
    void paintEvent(QPaintEvent *event) override;
    void enterEvent(QEvent *event) override;
    void leaveEvent(QEvent *event) override;

private:
    bool m_selected;
    bool m_hovered;
};
```

## 数据模型

### 设备数据结构
```cpp
struct DeviceInfo {
    QString id;           // 设备ID
    QString name;         // 设备名称
    QString type;         // 设备类型
    QString parentId;     // 父设备ID
    bool isGroup;         // 是否为组
    QStringList children; // 子设备列表
};

class DeviceManager {
public:
    static DeviceManager& instance();
    
    void loadDeviceData();
    QList<DeviceInfo> getDevicesByType(const QString &type) const;
    QStringList getDeviceTypes() const;
    DeviceInfo getDevice(const QString &id) const;

private:
    QHash<QString, DeviceInfo> m_devices;
    QStringList m_deviceTypes;
};
```

## 错误处理

### 时间验证
- 验证时间格式的有效性
- 检查结束时间是否晚于开始时间
- 处理时间范围超出合理范围的情况

### 设备数据处理
- 处理设备数据加载失败的情况
- 验证设备ID的有效性
- 处理搜索无结果的情况

### UI响应
- 提供加载状态指示
- 显示错误提示信息
- 实现优雅的错误恢复

## 测试策略

### 单元测试
- TimeWidget的时间计算逻辑测试
- DeviceWidget的搜索和筛选功能测试
- CustomButton的状态管理测试
- DeviceManager的数据操作测试

### 集成测试
- 主窗口组件间的交互测试
- 时间选择与设备筛选的联动测试
- 搜索功能的端到端测试

### UI测试
- 界面布局在不同分辨率下的适应性测试
- 主题样式的一致性测试
- 用户交互的响应性测试

## 样式设计

### 主题色彩
- 主背景色: #1a2332 (深蓝色)
- 次背景色: #2a3441 (中蓝色)
- 强调色: #4a9eff (亮蓝色)
- 文字色: #ffffff (白色)
- 边框色: #3a4451 (灰蓝色)

### QSS样式示例
```css
QMainWindow {
    background-color: #1a2332;
    color: #ffffff;
}

CustomButton {
    background-color: #2a3441;
    border: 1px solid #3a4451;
    border-radius: 4px;
    padding: 8px 16px;
    color: #ffffff;
}

CustomButton:hover {
    background-color: #3a4451;
}

CustomButton:selected {
    background-color: #4a9eff;
    border-color: #4a9eff;
}
```

## 性能考虑

### 优化策略
- 使用延迟加载减少启动时间
- 实现搜索防抖避免频繁过滤
- 采用虚拟化技术处理大量设备数据
- 缓存常用的设备查询结果

### 内存管理
- 合理使用智能指针管理对象生命周期
- 及时释放不再使用的资源
- 避免循环引用导致的内存泄漏