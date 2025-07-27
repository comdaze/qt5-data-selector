#include "DeviceManager.h"
#include <QDebug>

DeviceManager& DeviceManager::instance()
{
    static DeviceManager instance;
    return instance;
}

DeviceManager::DeviceManager(QObject *parent)
    : QObject(parent), m_dataLoaded(false), m_isLoading(false)
{
    // 构造函数中不加载数据，由外部调用loadDeviceData()
}

void DeviceManager::loadDeviceData()
{
    if (m_dataLoaded || m_isLoading) {
        return;
    }
    
    m_isLoading = true;
    m_lastError.clear();
    emit loadingStateChanged(true);
    
    try {
        // 清空现有数据
        m_devices.clear();
        m_deviceTypes.clear();
        
        // 模拟数据加载过程中可能的错误
        if (m_devices.isEmpty() && m_deviceTypes.isEmpty()) {
            // 初始化示例数据
            initializeSampleData();
            
            // 验证数据完整性
            if (m_devices.isEmpty()) {
                throw std::runtime_error("No device data available");
            }
            
            if (m_deviceTypes.isEmpty()) {
                throw std::runtime_error("No device types defined");
            }
            
            // 构建设备层级关系
            buildDeviceHierarchy();
            
            // 验证层级关系
            validateDeviceHierarchy();
        }
        
        m_dataLoaded = true;
        m_isLoading = false;
        emit loadingStateChanged(false);
        emit dataLoaded();
        
        qDebug() << "Device data loaded successfully:" << m_devices.size() << "devices," << m_deviceTypes.size() << "types";
        
    } catch (const std::exception &e) {
        m_lastError = QString("设备数据加载失败: %1").arg(e.what());
        m_dataLoaded = false;
        m_isLoading = false;
        emit loadingStateChanged(false);
        emit loadError(m_lastError);
        qDebug() << "Device data loading failed:" << m_lastError;
    } catch (...) {
        m_lastError = "设备数据加载失败: 未知错误";
        m_dataLoaded = false;
        m_isLoading = false;
        emit loadingStateChanged(false);
        emit loadError(m_lastError);
        qDebug() << "Device data loading failed with unknown error";
    }
}

QList<DeviceInfo> DeviceManager::getDevicesByType(const QString &type) const
{
    QList<DeviceInfo> result;
    
    if (type.isEmpty()) {
        // 返回所有设备
        return getAllDevices();
    }
    
    for (auto it = m_devices.constBegin(); it != m_devices.constEnd(); ++it) {
        if (it.value().type == type) {
            result.append(it.value());
        }
    }
    
    return result;
}

QStringList DeviceManager::getDeviceTypes() const
{
    return m_deviceTypes;
}

DeviceInfo DeviceManager::getDevice(const QString &id) const
{
    return m_devices.value(id, DeviceInfo());
}

QList<DeviceInfo> DeviceManager::getAllDevices() const
{
    return m_devices.values();
}

QList<DeviceInfo> DeviceManager::searchDevices(const QString &keyword) const
{
    QList<DeviceInfo> result;
    
    if (keyword.isEmpty()) {
        return getAllDevices();
    }
    
    QString lowerKeyword = keyword.toLower();
    
    for (auto it = m_devices.constBegin(); it != m_devices.constEnd(); ++it) {
        const DeviceInfo &device = it.value();
        if (device.name.toLower().contains(lowerKeyword) ||
            device.id.toLower().contains(lowerKeyword)) {
            result.append(device);
        }
    }
    
    return result;
}

QList<DeviceInfo> DeviceManager::getChildDevices(const QString &parentId) const
{
    QList<DeviceInfo> result;
    
    if (!m_devices.contains(parentId)) {
        return result;
    }
    
    const DeviceInfo &parent = m_devices[parentId];
    for (const QString &childId : parent.children) {
        if (m_devices.contains(childId)) {
            result.append(m_devices[childId]);
        }
    }
    
    return result;
}

void DeviceManager::initializeSampleData()
{
    // 设备类型
    m_deviceTypes << "全部模型" << "根模型" << "子模型" << "传感器";
    
    // 创建示例设备数据
    // 根模型组
    DeviceInfo rootGroup("root_group", "根模型", "根模型", "", true);
    m_devices.insert(rootGroup.id, rootGroup);
    
    // 根模型设备
    DeviceInfo root1("root_001", "主控制器", "根模型", "root_group");
    DeviceInfo root2("root_002", "备用控制器", "根模型", "root_group");
    m_devices.insert(root1.id, root1);
    m_devices.insert(root2.id, root2);
    
    // 子模型组
    DeviceInfo childGroup("child_group", "子模型", "子模型", "", true);
    m_devices.insert(childGroup.id, childGroup);
    
    // 子模型设备
    DeviceInfo child1("child_001", "温度模块", "子模型", "child_group");
    DeviceInfo child2("child_002", "湿度模块", "子模型", "child_group");
    DeviceInfo child3("child_003", "压力模块", "子模型", "child_group");
    m_devices.insert(child1.id, child1);
    m_devices.insert(child2.id, child2);
    m_devices.insert(child3.id, child3);
    
    // 传感器组
    DeviceInfo sensorGroup("sensor_group", "传感器", "传感器", "", true);
    m_devices.insert(sensorGroup.id, sensorGroup);
    
    // 传感器设备
    DeviceInfo sensor1("sensor_001", "温度传感器A", "传感器", "sensor_group");
    DeviceInfo sensor2("sensor_002", "温度传感器B", "传感器", "sensor_group");
    DeviceInfo sensor3("sensor_003", "湿度传感器A", "传感器", "sensor_group");
    DeviceInfo sensor4("sensor_004", "压力传感器A", "传感器", "sensor_group");
    m_devices.insert(sensor1.id, sensor1);
    m_devices.insert(sensor2.id, sensor2);
    m_devices.insert(sensor3.id, sensor3);
    m_devices.insert(sensor4.id, sensor4);
}

void DeviceManager::buildDeviceHierarchy()
{
    // 构建父子关系
    for (auto it = m_devices.begin(); it != m_devices.end(); ++it) {
        DeviceInfo &device = it.value();
        
        if (!device.parentId.isEmpty() && m_devices.contains(device.parentId)) {
            DeviceInfo &parent = m_devices[device.parentId];
            parent.addChild(device.id);
        }
    }
}

void DeviceManager::validateDeviceHierarchy()
{
    // 验证父子关系的完整性
    for (auto it = m_devices.constBegin(); it != m_devices.constEnd(); ++it) {
        const DeviceInfo &device = it.value();
        
        // 检查父设备是否存在
        if (!device.parentId.isEmpty() && !m_devices.contains(device.parentId)) {
            throw std::runtime_error(QString("Device %1 references non-existent parent %2")
                                    .arg(device.id, device.parentId).toStdString());
        }
        
        // 检查子设备是否存在
        for (const QString &childId : device.children) {
            if (!m_devices.contains(childId)) {
                throw std::runtime_error(QString("Device %1 references non-existent child %2")
                                        .arg(device.id, childId).toStdString());
            }
        }
        
        // 检查循环引用
        QStringList visited;
        if (hasCircularReference(device.id, visited)) {
            throw std::runtime_error(QString("Circular reference detected in device hierarchy starting from %1")
                                    .arg(device.id).toStdString());
        }
    }
}

bool DeviceManager::hasCircularReference(const QString &deviceId, QStringList &visited) const
{
    if (visited.contains(deviceId)) {
        return true;
    }
    
    if (!m_devices.contains(deviceId)) {
        return false;
    }
    
    visited.append(deviceId);
    const DeviceInfo &device = m_devices[deviceId];
    
    for (const QString &childId : device.children) {
        if (hasCircularReference(childId, visited)) {
            return true;
        }
    }
    
    visited.removeLast();
    return false;
}