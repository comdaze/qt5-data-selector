#include "DeviceWidget.h"
#include "DeviceManager.h"
#include <QTabWidget>
#include <QLineEdit>
#include <QTreeView>
#include <QStandardItemModel>
#include <QStandardItem>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QCheckBox>
#include <QDebug>

DeviceWidget::DeviceWidget(QWidget *parent)
    : QWidget(parent)
    , m_tabWidget(nullptr)
    , m_searchEdit(nullptr)
    , m_deviceTree(nullptr)
    , m_deviceModel(nullptr)
    , m_selectAllCheckBox(nullptr)
    , m_selectedCountLabel(nullptr)
    , m_noResultLabel(nullptr)
    , m_loadingLabel(nullptr)
    , m_errorLabel(nullptr)
    , m_mainLayout(nullptr)
    , m_searchLayout(nullptr)
    , m_statusLayout(nullptr)
    , m_updatingSelection(false)
{
    setupUI();
    setupDeviceTree();
    
    // 连接设备管理器信号
    connect(&DeviceManager::instance(), &DeviceManager::dataLoaded,
            this, &DeviceWidget::onDeviceDataLoaded);
    
    // 如果数据已经加载，直接更新界面
    if (!DeviceManager::instance().getAllDevices().isEmpty()) {
        onDeviceDataLoaded();
    }
}

QStringList DeviceWidget::getSelectedDevices() const
{
    return m_selectedDeviceIds;
}

void DeviceWidget::setSelectedDevices(const QStringList &deviceIds)
{
    m_selectedDeviceIds = deviceIds;
    updateSelection();
}

void DeviceWidget::clearSelection()
{
    m_selectedDeviceIds.clear();
    updateSelection();
}

QString DeviceWidget::getSearchText() const
{
    return m_searchEdit ? m_searchEdit->text() : QString();
}

void DeviceWidget::setSearchText(const QString &text)
{
    if (m_searchEdit) {
        m_searchEdit->setText(text);
    }
}

void DeviceWidget::setupUI()
{
    m_mainLayout = new QVBoxLayout(this);
    m_mainLayout->setContentsMargins(8, 8, 8, 8);
    m_mainLayout->setSpacing(8);
    
    // 创建标签页
    createTabs();
    
    // 创建搜索布局 - 响应式水平布局
    m_searchLayout = new QHBoxLayout();
    m_searchLayout->setSpacing(8);
    m_searchLayout->setContentsMargins(0, 4, 0, 4);
    
    QLabel *searchLabel = new QLabel("搜索:", this);
    searchLabel->setStyleSheet("color: #cccccc; font-size: 12px; font-weight: bold;");
    searchLabel->setMinimumWidth(50);
    searchLabel->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);
    
    m_searchEdit = new QLineEdit(this);
    m_searchEdit->setPlaceholderText("输入设备名称进行搜索...");
    m_searchEdit->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    m_searchEdit->setMinimumHeight(32);
    
    m_searchLayout->addWidget(searchLabel);
    m_searchLayout->addWidget(m_searchEdit);
    
    // 创建设备树形视图 - 响应式设置
    m_deviceTree = new QTreeView(this);
    m_deviceTree->setHeaderHidden(true);
    m_deviceTree->setRootIsDecorated(true);
    m_deviceTree->setExpandsOnDoubleClick(true);
    m_deviceTree->setAlternatingRowColors(true);
    m_deviceTree->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    m_deviceTree->setMinimumHeight(200);
    
    // 设置树形视图的选择模式
    m_deviceTree->setSelectionMode(QAbstractItemView::ExtendedSelection);
    m_deviceTree->setSelectionBehavior(QAbstractItemView::SelectRows);
    
    // 创建状态布局 - 响应式水平布局
    m_statusLayout = new QHBoxLayout();
    m_statusLayout->setSpacing(12);
    m_statusLayout->setContentsMargins(0, 4, 0, 0);
    
    // 创建"全部"复选框
    m_selectAllCheckBox = new QCheckBox("全部", this);
    m_selectAllCheckBox->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    
    m_selectedCountLabel = new QLabel("已选择: 0 个设备", this);
    m_selectedCountLabel->setStyleSheet("color: #4a9eff; font-size: 12px; font-weight: bold;");
    m_selectedCountLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    
    m_noResultLabel = new QLabel("无匹配结果", this);
    m_noResultLabel->setAlignment(Qt::AlignCenter);
    m_noResultLabel->setStyleSheet("color: #cccccc; font-size: 14px; padding: 20px;");
    m_noResultLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    m_noResultLabel->hide();
    
    m_statusLayout->addWidget(m_selectAllCheckBox);
    m_statusLayout->addWidget(m_selectedCountLabel);
    m_statusLayout->addStretch();
    
    // 添加到主布局 - 设置拉伸因子
    m_mainLayout->addWidget(m_tabWidget, 0);  // 标签页固定高度
    m_mainLayout->addLayout(m_searchLayout, 0);  // 搜索框固定高度
    m_mainLayout->addWidget(m_deviceTree, 1);  // 树形视图可扩展
    m_mainLayout->addWidget(m_noResultLabel, 1);  // 无结果提示可扩展
    m_mainLayout->addLayout(m_statusLayout, 0);  // 状态栏固定高度
    
    // 连接信号槽
    connect(m_searchEdit, &QLineEdit::textChanged,
            this, &DeviceWidget::onSearchTextChanged);
    connect(m_tabWidget, QOverload<int>::of(&QTabWidget::currentChanged),
            this, &DeviceWidget::onTabChanged);
    connect(m_selectAllCheckBox, &QCheckBox::toggled,
            this, &DeviceWidget::onSelectAllChanged);
}

void DeviceWidget::createTabs()
{
    m_tabWidget = new QTabWidget(this);
    
    // 设置标签页的响应式属性
    m_tabWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    m_tabWidget->setMinimumHeight(40);
    m_tabWidget->setMaximumHeight(50);
    
    // 设置标签页可滚动（当标签页过多时）
    m_tabWidget->setUsesScrollButtons(true);
    m_tabWidget->setElideMode(Qt::ElideRight);
    
    // 添加设备类型标签页
    QStringList deviceTypes = DeviceManager::instance().getDeviceTypes();
    
    // 添加"全部模型"标签页
    QWidget *allTab = new QWidget();
    allTab->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    m_tabWidget->addTab(allTab, "全部模型");
    
    // 添加其他设备类型标签页
    for (const QString &type : deviceTypes) {
        if (type != "全部模型") {
            QWidget *typeTab = new QWidget();
            typeTab->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
            m_tabWidget->addTab(typeTab, type);
        }
    }
}

void DeviceWidget::setupDeviceTree()
{
    m_deviceModel = new QStandardItemModel(this);
    m_deviceModel->setHorizontalHeaderLabels(QStringList() << "设备名称");
    
    if (m_deviceTree) {
        m_deviceTree->setModel(m_deviceModel);
        
        // 连接模型信号
        connect(m_deviceModel, &QStandardItemModel::itemChanged,
                this, &DeviceWidget::onItemChanged);
    }
}

void DeviceWidget::onSearchTextChanged(const QString &text)
{
    filterDevices(text);
    emit searchTextChanged(text);
}

void DeviceWidget::onItemChanged(QStandardItem *item)
{
    if (m_updatingSelection || !item) {
        return;
    }
    
    m_updatingSelection = true;
    
    // 获取设备ID
    QString deviceId = item->data(Qt::UserRole).toString();
    bool checked = (item->checkState() == Qt::Checked);
    
    // 递归设置子项目状态
    setItemCheckedRecursive(item, checked);
    
    // 更新父项目状态
    updateParentCheckState(item);
    
    // 更新选中设备列表
    m_selectedDeviceIds.clear();
    collectSelectedDevices(m_deviceModel->invisibleRootItem(), m_selectedDeviceIds);
    
    // 更新显示
    updateSelectedCount();
    updateSelectAllCheckBox();
    
    m_updatingSelection = false;
    
    emit selectionChanged(m_selectedDeviceIds);
}

void DeviceWidget::onTabChanged(int index)
{
    if (index < 0 || index >= m_tabWidget->count()) {
        return;
    }
    
    QString tabText = m_tabWidget->tabText(index);
    m_currentDeviceType = (tabText == "全部模型") ? QString() : tabText;
    
    loadDeviceData(m_currentDeviceType);
}

void DeviceWidget::onDeviceDataLoaded()
{
    // 重新创建标签页
    m_tabWidget->clear();
    createTabs();
    
    // 加载当前类型的设备数据
    loadDeviceData(m_currentDeviceType);
}

void DeviceWidget::onSelectAllChanged(bool checked)
{
    if (m_updatingSelection || !m_deviceModel) {
        return;
    }
    
    m_updatingSelection = true;
    
    // 设置所有根级项目的选中状态
    for (int i = 0; i < m_deviceModel->rowCount(); ++i) {
        QStandardItem *item = m_deviceModel->item(i);
        if (item) {
            setItemCheckedRecursive(item, checked);
        }
    }
    
    // 更新选中设备列表
    m_selectedDeviceIds.clear();
    if (checked) {
        collectSelectedDevices(m_deviceModel->invisibleRootItem(), m_selectedDeviceIds);
    }
    
    // 更新显示
    updateSelectedCount();
    
    m_updatingSelection = false;
    
    emit selectionChanged(m_selectedDeviceIds);
}

void DeviceWidget::loadDeviceData(const QString &deviceType)
{
    if (!m_deviceModel) {
        return;
    }
    
    m_deviceModel->clear();
    m_deviceModel->setHorizontalHeaderLabels(QStringList() << "设备名称");
    
    QList<DeviceInfo> devices;
    if (deviceType.isEmpty()) {
        // 显示所有设备
        devices = DeviceManager::instance().getAllDevices();
    } else {
        // 显示指定类型的设备
        devices = DeviceManager::instance().getDevicesByType(deviceType);
    }
    
    // 创建设备组映射
    QHash<QString, QStandardItem*> itemMap;
    
    // 首先创建所有组节点
    for (const DeviceInfo &device : devices) {
        if (device.isGroup) {
            QStandardItem *item = createDeviceItem(device.id);
            itemMap.insert(device.id, item);
            m_deviceModel->appendRow(item);
        }
    }
    
    // 然后添加非组设备
    for (const DeviceInfo &device : devices) {
        if (!device.isGroup) {
            QStandardItem *item = createDeviceItem(device.id);
            
            if (!device.parentId.isEmpty() && itemMap.contains(device.parentId)) {
                // 添加到父组
                itemMap[device.parentId]->appendRow(item);
            } else {
                // 添加到根级别
                m_deviceModel->appendRow(item);
            }
            
            itemMap.insert(device.id, item);
        }
    }
    
    // 展开所有组节点
    m_deviceTree->expandAll();
    
    // 更新选择状态
    updateSelection();
    updateSelectedCount();
    updateSelectAllCheckBox();
}

void DeviceWidget::filterDevices(const QString &filter)
{
    if (!m_deviceModel || !m_deviceTree) {
        return;
    }
    
    if (filter.isEmpty()) {
        // 显示所有项目
        for (int i = 0; i < m_deviceModel->rowCount(); ++i) {
            QStandardItem *item = m_deviceModel->item(i);
            if (item) {
                m_deviceTree->setRowHidden(i, QModelIndex(), false);
                // 递归显示所有子项目
                setItemVisibleRecursive(item, true);
            }
        }
        m_noResultLabel->hide();
        m_deviceTree->show();
    } else {
        // 过滤显示
        bool hasVisibleItems = false;
        QString lowerFilter = filter.toLower();
        
        for (int i = 0; i < m_deviceModel->rowCount(); ++i) {
            QStandardItem *item = m_deviceModel->item(i);
            if (item) {
                bool visible = filterItemRecursive(item, lowerFilter);
                m_deviceTree->setRowHidden(i, QModelIndex(), !visible);
                if (visible) {
                    hasVisibleItems = true;
                }
            }
        }
        
        if (hasVisibleItems) {
            m_noResultLabel->hide();
            m_deviceTree->show();
        } else {
            m_deviceTree->hide();
            m_noResultLabel->show();
        }
    }
}

void DeviceWidget::updateSelection()
{
    if (!m_deviceModel || m_updatingSelection) {
        return;
    }
    
    m_updatingSelection = true;
    
    // 首先清除所有选择
    clearAllItemsSelection(m_deviceModel->invisibleRootItem());
    
    // 然后设置选中的项目
    for (const QString &deviceId : m_selectedDeviceIds) {
        setItemSelectedById(m_deviceModel->invisibleRootItem(), deviceId, true);
    }
    
    m_updatingSelection = false;
    updateSelectedCount();
    updateSelectAllCheckBox();
}

void DeviceWidget::updateSelectedCount()
{
    if (m_selectedCountLabel) {
        m_selectedCountLabel->setText(QString("已选择: %1 个设备").arg(m_selectedDeviceIds.count()));
    }
}

void DeviceWidget::updateSelectAllCheckBox()
{
    if (!m_selectAllCheckBox || !m_deviceModel || m_updatingSelection) {
        return;
    }
    
    // 计算当前可见的设备总数和已选中数
    int totalVisibleDevices = 0;
    int selectedVisibleDevices = 0;
    
    countVisibleDevices(m_deviceModel->invisibleRootItem(), totalVisibleDevices, selectedVisibleDevices);
    
    // 更新全选复选框状态
    bool wasUpdating = m_updatingSelection;
    m_updatingSelection = true;
    
    if (selectedVisibleDevices == 0) {
        m_selectAllCheckBox->setCheckState(Qt::Unchecked);
    } else if (selectedVisibleDevices == totalVisibleDevices) {
        m_selectAllCheckBox->setCheckState(Qt::Checked);
    } else {
        m_selectAllCheckBox->setCheckState(Qt::PartiallyChecked);
    }
    
    m_updatingSelection = wasUpdating;
}

QStandardItem* DeviceWidget::createDeviceItem(const QString &deviceId)
{
    DeviceInfo device = DeviceManager::instance().getDevice(deviceId);
    if (!device.isValid()) {
        return nullptr;
    }
    
    QStandardItem *item = new QStandardItem(device.name);
    item->setCheckable(true);
    item->setCheckState(Qt::Unchecked);
    item->setData(device.id, Qt::UserRole);
    
    // 设置图标或样式（可选）
    if (device.isGroup) {
        item->setIcon(QIcon()); // 可以设置文件夹图标
    }
    
    return item;
}

void DeviceWidget::setItemCheckedRecursive(QStandardItem *item, bool checked)
{
    if (!item) {
        return;
    }
    
    Qt::CheckState checkState = checked ? Qt::Checked : Qt::Unchecked;
    item->setCheckState(checkState);
    
    // 递归设置子项目
    for (int i = 0; i < item->rowCount(); ++i) {
        QStandardItem *child = item->child(i);
        if (child) {
            setItemCheckedRecursive(child, checked);
        }
    }
}

void DeviceWidget::updateParentCheckState(QStandardItem *item)
{
    if (!item) {
        return;
    }
    
    QStandardItem *parent = item->parent();
    if (!parent) {
        return;
    }
    
    // 检查所有兄弟项目的状态
    int checkedCount = 0;
    int totalCount = parent->rowCount();
    
    for (int i = 0; i < totalCount; ++i) {
        QStandardItem *sibling = parent->child(i);
        if (sibling && sibling->checkState() == Qt::Checked) {
            checkedCount++;
        }
    }
    
    // 设置父项目状态
    if (checkedCount == 0) {
        parent->setCheckState(Qt::Unchecked);
    } else if (checkedCount == totalCount) {
        parent->setCheckState(Qt::Checked);
    } else {
        parent->setCheckState(Qt::PartiallyChecked);
    }
    
    // 递归更新上级父项目
    updateParentCheckState(parent);
}

void DeviceWidget::collectSelectedDevices(QStandardItem *item, QStringList &selectedIds) const
{
    if (!item) {
        return;
    }
    
    // 检查当前项目
    if (item->checkState() == Qt::Checked) {
        QString deviceId = item->data(Qt::UserRole).toString();
        if (!deviceId.isEmpty() && !selectedIds.contains(deviceId)) {
            DeviceInfo device = DeviceManager::instance().getDevice(deviceId);
            // 只收集非组设备
            if (!device.isGroup) {
                selectedIds.append(deviceId);
            }
        }
    }
    
    // 递归检查子项目
    for (int i = 0; i < item->rowCount(); ++i) {
        QStandardItem *child = item->child(i);
        if (child) {
            collectSelectedDevices(child, selectedIds);
        }
    }
}

void DeviceWidget::setItemVisibleRecursive(QStandardItem *item, bool visible)
{
    if (!item) {
        return;
    }
    
    QModelIndex index = item->index();
    if (index.isValid()) {
        m_deviceTree->setRowHidden(index.row(), index.parent(), !visible);
    }
    
    // 递归设置子项目
    for (int i = 0; i < item->rowCount(); ++i) {
        QStandardItem *child = item->child(i);
        if (child) {
            setItemVisibleRecursive(child, visible);
        }
    }
}

bool DeviceWidget::filterItemRecursive(QStandardItem *item, const QString &filter)
{
    if (!item) {
        return false;
    }
    
    QString deviceId = item->data(Qt::UserRole).toString();
    DeviceInfo device = DeviceManager::instance().getDevice(deviceId);
    
    // 检查当前项目是否匹配
    bool currentMatches = device.name.toLower().contains(filter) || 
                         device.id.toLower().contains(filter);
    
    // 检查子项目是否有匹配
    bool childMatches = false;
    for (int i = 0; i < item->rowCount(); ++i) {
        QStandardItem *child = item->child(i);
        if (child && filterItemRecursive(child, filter)) {
            childMatches = true;
        }
    }
    
    // 如果当前项目或子项目匹配，则显示
    bool shouldShow = currentMatches || childMatches;
    
    QModelIndex index = item->index();
    if (index.isValid()) {
        m_deviceTree->setRowHidden(index.row(), index.parent(), !shouldShow);
    }
    
    return shouldShow;
}

void DeviceWidget::clearAllItemsSelection(QStandardItem *item)
{
    if (!item) {
        return;
    }
    
    item->setCheckState(Qt::Unchecked);
    
    // 递归清除子项目选择
    for (int i = 0; i < item->rowCount(); ++i) {
        QStandardItem *child = item->child(i);
        if (child) {
            clearAllItemsSelection(child);
        }
    }
}

void DeviceWidget::setItemSelectedById(QStandardItem *item, const QString &deviceId, bool selected)
{
    if (!item) {
        return;
    }
    
    QString itemDeviceId = item->data(Qt::UserRole).toString();
    if (itemDeviceId == deviceId) {
        item->setCheckState(selected ? Qt::Checked : Qt::Unchecked);
        return;
    }
    
    // 递归查找子项目
    for (int i = 0; i < item->rowCount(); ++i) {
        QStandardItem *child = item->child(i);
        if (child) {
            setItemSelectedById(child, deviceId, selected);
        }
    }
}

void DeviceWidget::countVisibleDevices(QStandardItem *item, int &totalVisible, int &selectedVisible) const
{
    if (!item || !m_deviceTree) {
        return;
    }
    
    // 检查当前项目是否可见
    QModelIndex index = item->index();
    bool isVisible = true;
    
    if (index.isValid()) {
        isVisible = !m_deviceTree->isRowHidden(index.row(), index.parent());
    }
    
    if (isVisible) {
        QString deviceId = item->data(Qt::UserRole).toString();
        if (!deviceId.isEmpty()) {
            DeviceInfo device = DeviceManager::instance().getDevice(deviceId);
            // 只统计非组设备
            if (!device.isGroup) {
                totalVisible++;
                if (item->checkState() == Qt::Checked) {
                    selectedVisible++;
                }
            }
        }
    }
    
    // 递归统计子项目
    for (int i = 0; i < item->rowCount(); ++i) {
        QStandardItem *child = item->child(i);
        if (child) {
            countVisibleDevices(child, totalVisible, selectedVisible);
        }
    }
}
v
oid DeviceWidget::onLoadingStateChanged(bool loading)
{
    showLoadingState(loading);
    emit loadingStateChanged(loading);
}

void DeviceWidget::onDataLoadError(const QString &error)
{
    showError(error);
    emit dataLoadError(error);
}

void DeviceWidget::showLoadingState(bool loading)
{
    if (m_loadingLabel) {
        m_loadingLabel->setVisible(loading);
        if (loading) {
            m_loadingLabel->setText("正在加载设备数据...");
        }
    }
    
    if (m_deviceTree) {
        m_deviceTree->setEnabled(!loading);
    }
}

void DeviceWidget::showError(const QString &error)
{
    if (m_errorLabel) {
        m_errorLabel->setText(QString("错误: %1").arg(error));
        m_errorLabel->setVisible(true);
        m_errorLabel->setStyleSheet("color: red; font-weight: bold;");
    }
    
    qWarning() << "DeviceWidget error:" << error;
}

void DeviceWidget::clearError()
{
    if (m_errorLabel) {
        m_errorLabel->setVisible(false);
        m_errorLabel->clear();
    }
}