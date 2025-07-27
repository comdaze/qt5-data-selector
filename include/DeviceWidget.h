#ifndef DEVICEWIDGET_H
#define DEVICEWIDGET_H

#include <QWidget>
#include <QStringList>

class QTabWidget;
class QLineEdit;
class QTreeView;
class QStandardItemModel;
class QStandardItem;
class QLabel;
class QVBoxLayout;
class QHBoxLayout;
class QCheckBox;

/**
 * @brief 设备控件类
 * 
 * 提供设备类型标签页、设备树形列表、搜索功能和选择状态管理
 * 支持按设备类型分类显示、关键字搜索和多选功能
 */
class DeviceWidget : public QWidget
{
    Q_OBJECT

public:
    /**
     * @brief 构造函数
     * @param parent 父窗口
     */
    explicit DeviceWidget(QWidget *parent = nullptr);

    /**
     * @brief 获取已选择的设备ID列表
     * @return 设备ID列表
     */
    QStringList getSelectedDevices() const;
    
    /**
     * @brief 设置选中的设备
     * @param deviceIds 设备ID列表
     */
    void setSelectedDevices(const QStringList &deviceIds);
    
    /**
     * @brief 清除所有选择
     */
    void clearSelection();
    
    /**
     * @brief 获取当前搜索文本
     * @return 搜索文本
     */
    QString getSearchText() const;
    
    /**
     * @brief 设置搜索文本
     * @param text 搜索文本
     */
    void setSearchText(const QString &text);

signals:
    /**
     * @brief 设备选择变化信号
     * @param selectedDevices 已选择的设备ID列表
     */
    void selectionChanged(const QStringList &selectedDevices);
    
    /**
     * @brief 搜索文本变化信号
     * @param text 搜索文本
     */
    void searchTextChanged(const QString &text);
    
    /**
     * @brief 设备数据加载状态信号
     * @param loading 是否正在加载
     */
    void loadingStateChanged(bool loading);
    
    /**
     * @brief 设备数据加载错误信号
     * @param error 错误信息
     */
    void dataLoadError(const QString &error);

private slots:
    /**
     * @brief 搜索文本变化槽函数
     * @param text 搜索文本
     */
    void onSearchTextChanged(const QString &text);
    
    /**
     * @brief 树形项目变化槽函数
     * @param item 变化的项目
     */
    void onItemChanged(QStandardItem *item);
    
    /**
     * @brief 标签页变化槽函数
     * @param index 标签页索引
     */
    void onTabChanged(int index);
    
    /**
     * @brief 设备数据加载完成槽函数
     */
    void onDeviceDataLoaded();
    
    /**
     * @brief 全选复选框状态变化槽函数
     * @param checked 是否选中
     */
    void onSelectAllChanged(bool checked);
    
    /**
     * @brief 设备数据加载状态变化槽函数
     * @param loading 是否正在加载
     */
    void onLoadingStateChanged(bool loading);
    
    /**
     * @brief 设备数据加载错误槽函数
     * @param error 错误信息
     */
    void onDataLoadError(const QString &error);

private:
    /**
     * @brief 设置用户界面
     */
    void setupUI();
    
    /**
     * @brief 设置设备树形视图
     */
    void setupDeviceTree();
    
    /**
     * @brief 创建标签页
     */
    void createTabs();
    
    /**
     * @brief 加载设备数据到模型
     * @param deviceType 设备类型，空字符串表示所有类型
     */
    void loadDeviceData(const QString &deviceType = QString());
    
    /**
     * @brief 根据关键字过滤设备
     * @param filter 过滤关键字
     */
    void filterDevices(const QString &filter);
    
    /**
     * @brief 更新选择状态
     */
    void updateSelection();
    
    /**
     * @brief 更新已选择设备数量显示
     */
    void updateSelectedCount();
    
    /**
     * @brief 更新全选复选框状态
     */
    void updateSelectAllCheckBox();
    
    /**
     * @brief 创建设备树项目
     * @param deviceId 设备ID
     * @return 创建的标准项目
     */
    QStandardItem* createDeviceItem(const QString &deviceId);
    
    /**
     * @brief 递归设置项目选中状态
     * @param item 项目
     * @param checked 是否选中
     */
    void setItemCheckedRecursive(QStandardItem *item, bool checked);
    
    /**
     * @brief 更新父项目的选中状态
     * @param item 子项目
     */
    void updateParentCheckState(QStandardItem *item);
    
    /**
     * @brief 收集选中的设备ID
     * @param item 根项目
     * @param selectedIds 输出的选中ID列表
     */
    void collectSelectedDevices(QStandardItem *item, QStringList &selectedIds) const;
    
    /**
     * @brief 递归设置项目可见性
     * @param item 项目
     * @param visible 是否可见
     */
    void setItemVisibleRecursive(QStandardItem *item, bool visible);
    
    /**
     * @brief 递归过滤项目
     * @param item 项目
     * @param filter 过滤关键字
     * @return 是否应该显示该项目
     */
    bool filterItemRecursive(QStandardItem *item, const QString &filter);
    
    /**
     * @brief 递归清除所有项目选择
     * @param item 根项目
     */
    void clearAllItemsSelection(QStandardItem *item);
    
    /**
     * @brief 根据设备ID设置项目选中状态
     * @param item 根项目
     * @param deviceId 设备ID
     * @param selected 是否选中
     */
    void setItemSelectedById(QStandardItem *item, const QString &deviceId, bool selected);
    
    /**
     * @brief 统计可见设备数量
     * @param item 根项目
     * @param totalVisible 总可见设备数（输出）
     * @param selectedVisible 已选中可见设备数（输出）
     */
    void countVisibleDevices(QStandardItem *item, int &totalVisible, int &selectedVisible) const;
    
    /**
     * @brief 显示加载状态
     * @param loading 是否正在加载
     */
    void showLoadingState(bool loading);
    
    /**
     * @brief 显示错误信息
     * @param error 错误信息
     */
    void showError(const QString &error);
    
    /**
     * @brief 清除错误信息
     */
    void clearError();

private:
    // UI组件
    QTabWidget *m_tabWidget;              // 设备类型标签页
    QLineEdit *m_searchEdit;              // 搜索输入框
    QTreeView *m_deviceTree;              // 设备树形视图
    QStandardItemModel *m_deviceModel;    // 设备数据模型
    QCheckBox *m_selectAllCheckBox;       // 全选复选框
    QLabel *m_selectedCountLabel;         // 已选择数量标签
    QLabel *m_noResultLabel;              // 无结果提示标签
    QLabel *m_loadingLabel;               // 加载状态标签
    QLabel *m_errorLabel;                 // 错误信息标签
    
    // 布局
    QVBoxLayout *m_mainLayout;            // 主布局
    QHBoxLayout *m_searchLayout;          // 搜索布局
    QHBoxLayout *m_statusLayout;          // 状态布局
    
    // 状态
    QString m_currentDeviceType;          // 当前设备类型
    QStringList m_selectedDeviceIds;      // 已选择的设备ID
    bool m_updatingSelection;             // 是否正在更新选择状态（防止递归）
};

#endif // DEVICEWIDGET_H