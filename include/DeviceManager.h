#ifndef DEVICEMANAGER_H
#define DEVICEMANAGER_H

#include <QObject>
#include <QHash>
#include <QStringList>
#include "DeviceInfo.h"

/**
 * @brief 设备数据管理器
 * 
 * 单例模式的设备数据管理类，负责设备数据的加载、存储和查询
 * 提供设备类型分类、层级结构管理等功能
 */
class DeviceManager : public QObject
{
    Q_OBJECT

public:
    /**
     * @brief 获取单例实例
     * @return DeviceManager单例引用
     */
    static DeviceManager& instance();
    
    /**
     * @brief 加载设备数据
     * 从配置文件或数据源加载设备信息
     */
    void loadDeviceData();
    
    /**
     * @brief 根据类型获取设备列表
     * @param type 设备类型
     * @return 指定类型的设备列表
     */
    QList<DeviceInfo> getDevicesByType(const QString &type) const;
    
    /**
     * @brief 获取所有设备类型
     * @return 设备类型列表
     */
    QStringList getDeviceTypes() const;
    
    /**
     * @brief 根据ID获取设备信息
     * @param id 设备ID
     * @return 设备信息，如果不存在返回无效的DeviceInfo
     */
    DeviceInfo getDevice(const QString &id) const;
    
    /**
     * @brief 获取所有设备
     * @return 所有设备的列表
     */
    QList<DeviceInfo> getAllDevices() const;
    
    /**
     * @brief 根据关键字搜索设备
     * @param keyword 搜索关键字
     * @return 匹配的设备列表
     */
    QList<DeviceInfo> searchDevices(const QString &keyword) const;
    
    /**
     * @brief 获取设备的子设备列表
     * @param parentId 父设备ID
     * @return 子设备列表
     */
    QList<DeviceInfo> getChildDevices(const QString &parentId) const;

    /**
     * @brief 检查数据是否已加载
     * @return 如果数据已加载返回true
     */
    bool isDataLoaded() const { return m_dataLoaded; }
    
    /**
     * @brief 获取最后的错误信息
     * @return 错误信息字符串
     */
    QString getLastError() const { return m_lastError; }

signals:
    /**
     * @brief 设备数据加载完成信号
     */
    void dataLoaded();
    
    /**
     * @brief 设备数据加载失败信号
     * @param error 错误信息
     */
    void loadError(const QString &error);
    
    /**
     * @brief 数据加载状态变化信号
     * @param loading 是否正在加载
     */
    void loadingStateChanged(bool loading);

private:
    /**
     * @brief 私有构造函数（单例模式）
     */
    explicit DeviceManager(QObject *parent = nullptr);
    
    /**
     * @brief 禁用拷贝构造函数
     */
    DeviceManager(const DeviceManager&) = delete;
    
    /**
     * @brief 禁用赋值操作符
     */
    DeviceManager& operator=(const DeviceManager&) = delete;
    
    /**
     * @brief 初始化示例设备数据
     */
    void initializeSampleData();
    
    /**
     * @brief 构建设备层级关系
     */
    void buildDeviceHierarchy();
    
    /**
     * @brief 验证设备层级关系的完整性
     */
    void validateDeviceHierarchy();
    
    /**
     * @brief 检查设备层级中是否存在循环引用
     * @param deviceId 要检查的设备ID
     * @param visited 已访问的设备ID列表
     * @return 如果存在循环引用返回true
     */
    bool hasCircularReference(const QString &deviceId, QStringList &visited) const;

private:
    QHash<QString, DeviceInfo> m_devices;  // 设备ID到设备信息的映射
    QStringList m_deviceTypes;             // 设备类型列表
    bool m_dataLoaded;                     // 数据是否已加载标志
    QString m_lastError;                   // 最后的错误信息
    bool m_isLoading;                      // 是否正在加载数据
};

#endif // DEVICEMANAGER_H