# 故障排除指南

## 常见构建问题

### 1. Qt5 未找到
**错误信息**: `Could not find Qt5`
**解决方案**:
- 确保已安装Qt5开发包
- 设置Qt5_DIR环境变量指向Qt5安装目录
- macOS: `brew install qt@5`
- Ubuntu: `sudo apt-get install qt5-default qtbase5-dev`

### 2. CMake版本过低
**错误信息**: `CMake 3.16 or higher is required`
**解决方案**:
- 升级CMake到3.16或更高版本
- macOS: `brew upgrade cmake`
- Ubuntu: 从官网下载最新版本

### 3. 编译器不支持C++11
**错误信息**: `This file requires compiler and library support for the ISO C++ 2011 standard`
**解决方案**:
- 使用支持C++11的编译器
- GCC 4.8+, Clang 3.3+, MSVC 2015+

### 4. MOC文件生成失败
**错误信息**: `moc: Too many input files specified`
**解决方案**:
- 清理构建目录: `rm -rf build/`
- 重新运行构建脚本: `./build.sh cmake`

## 运行时问题

### 1. 应用程序无法启动
**可能原因**:
- Qt5运行时库未找到
- 资源文件缺失
- 权限问题

**解决方案**:
- 检查Qt5库路径: `ldd bin/Qt5DataSelector` (Linux)
- 确保resources目录存在且包含必要文件
- 检查可执行文件权限: `chmod +x bin/Qt5DataSelector`

### 2. 样式表未加载
**症状**: 界面显示为默认Qt样式
**解决方案**:
- 检查resources.qrc文件是否包含styles.qss
- 确保resources/styles.qss文件存在
- 重新构建项目以更新资源

### 3. 设备数据未显示
**可能原因**:
- DeviceManager初始化失败
- 示例数据生成错误

**解决方案**:
- 检查控制台错误信息
- 确保DeviceManager::loadDeviceData()被正确调用

## 开发问题

### 1. 信号槽连接失败
**症状**: 界面交互无响应
**解决方案**:
- 检查Q_OBJECT宏是否存在于类声明中
- 确保MOC文件已生成
- 验证信号和槽的参数类型匹配

### 2. 内存泄漏
**检测工具**:
- Valgrind (Linux/macOS)
- Application Verifier (Windows)
- Qt Creator内置的Heob工具

**预防措施**:
- 使用智能指针管理动态内存
- 确保QObject的父子关系正确设置
- 及时断开不再需要的信号槽连接

### 3. 界面布局问题
**常见问题**:
- 控件重叠
- 响应式布局失效
- 字体显示异常

**解决方案**:
- 使用Qt Designer预览布局
- 检查布局管理器设置
- 测试不同分辨率下的显示效果

## 性能优化

### 1. 启动速度慢
**优化方法**:
- 延迟加载非关键组件
- 优化设备数据加载逻辑
- 减少启动时的文件I/O操作

### 2. 搜索响应慢
**优化方法**:
- 实现搜索防抖机制
- 使用索引加速搜索
- 限制搜索结果数量

### 3. 内存使用过高
**优化方法**:
- 实现设备数据的懒加载
- 使用对象池管理频繁创建的对象
- 定期清理不再使用的缓存

## 调试技巧

### 1. 启用Qt调试输出
```cpp
QLoggingCategory::setFilterRules("qt.qpa.input.debug=true");
```

### 2. 使用qDebug输出调试信息
```cpp
qDebug() << "Debug message:" << variable;
```

### 3. 使用Qt Creator调试器
- 设置断点
- 查看变量值
- 单步执行代码

## 获取帮助

如果以上解决方案无法解决您的问题，请：

1. 查看项目GitHub Issues页面
2. 搜索Qt官方文档
3. 在Stack Overflow上提问
4. 联系项目维护者

## 日志收集

在报告问题时，请提供以下信息：

1. 操作系统版本
2. Qt版本
3. 编译器版本
4. 完整的错误信息
5. 重现步骤
6. 相关的日志输出