#ifndef DEVICEINFO_H
#define DEVICEINFO_H

#include <QString>
#include <QStringList>

/**
 * @brief 设备信息数据结构
 * 
 * 用于存储设备的基本信息，包括ID、名称、类型等
 * 支持层级结构，可以表示设备组和子设备关系
 */
struct DeviceInfo {
    QString id;           // 设备唯一标识符
    QString name;         // 设备显示名称
    QString type;         // 设备类型（如"全部模型"、"根模型"等）
    QString parentId;     // 父设备ID，用于构建层级结构
    bool isGroup;         // 是否为设备组（容器节点）
    QStringList children; // 子设备ID列表
    
    /**
     * @brief 默认构造函数
     */
    DeviceInfo() : isGroup(false) {}
    
    /**
     * @brief 构造函数
     * @param deviceId 设备ID
     * @param deviceName 设备名称
     * @param deviceType 设备类型
     * @param parent 父设备ID
     * @param group 是否为组
     */
    DeviceInfo(const QString &deviceId, 
               const QString &deviceName, 
               const QString &deviceType,
               const QString &parent = QString(),
               bool group = false)
        : id(deviceId), name(deviceName), type(deviceType), 
          parentId(parent), isGroup(group) {}
    
    /**
     * @brief 判断设备是否有效
     * @return 如果设备ID和名称都不为空则返回true
     */
    bool isValid() const {
        return !id.isEmpty() && !name.isEmpty();
    }
    
    /**
     * @brief 添加子设备
     * @param childId 子设备ID
     */
    void addChild(const QString &childId) {
        if (!childId.isEmpty() && !children.contains(childId)) {
            children.append(childId);
        }
    }
    
    /**
     * @brief 移除子设备
     * @param childId 子设备ID
     */
    void removeChild(const QString &childId) {
        children.removeAll(childId);
    }
};

#endif // DEVICEINFO_H