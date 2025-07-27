#include <QApplication>
#include <QStyleFactory>
#include <QDir>
#include <QDebug>
#include <QFile>
#include <QMessageBox>
#include <QStandardPaths>
#include <QIcon>
#include "MainWindow.h"
#include "DeviceManager.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    
    // 设置应用程序基本信息
    app.setApplicationName("Qt5 Data Selector");
    app.setApplicationDisplayName("Qt5数据筛选器");
    app.setApplicationVersion("1.0.0");
    app.setOrganizationName("Qt5DataSelector");
    app.setOrganizationDomain("qt5dataselector.com");
    
    // 设置应用程序图标
    app.setWindowIcon(QIcon(":/resources/app_icon.png"));
    
    // 设置应用程序样式
    app.setStyle(QStyleFactory::create("Fusion"));
    
    // 加载样式表
    QFile styleFile(":/resources/styles.qss");
    if (styleFile.open(QFile::ReadOnly)) {
        QString styleSheet = QString::fromUtf8(styleFile.readAll());
        app.setStyleSheet(styleSheet);
        styleFile.close();
        qDebug() << "样式表加载成功";
    } else {
        qWarning() << "无法加载样式文件，使用默认样式";
        // 尝试从文件系统加载
        QFile fallbackStyleFile("resources/styles.qss");
        if (fallbackStyleFile.open(QFile::ReadOnly)) {
            QString styleSheet = QString::fromUtf8(fallbackStyleFile.readAll());
            app.setStyleSheet(styleSheet);
            fallbackStyleFile.close();
            qDebug() << "从文件系统加载样式表成功";
        }
    }
    
    // 初始化设备管理器
    try {
        DeviceManager::instance().loadDeviceData();
    } catch (const std::exception& e) {
        QMessageBox::critical(nullptr, "初始化错误", 
                            QString("设备数据加载失败: %1").arg(e.what()));
        return -1;
    }
    
    // 创建并显示主窗口
    MainWindow window;
    window.show();
    
    return app.exec();
}