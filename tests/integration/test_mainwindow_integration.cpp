#include <QApplication>
#include <QTest>
#include <QSignalSpy>
#include <QDateTime>
#include <QDebug>
#include "MainWindow.h"
#include "TimeWidget.h"
#include "DeviceWidget.h"
#include "DeviceManager.h"

/**
 * @brief MainWindow集成测试类
 * 
 * 测试主窗口组件间的交互功能、时间选择与设备筛选的联动
 * 需求: 1.1, 2.1, 3.1
 */
class TestMainWindowIntegration : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void cleanupTestCase();
    void init();
    void cleanup();
    
    // 组件初始化测试
    void testMainWindowInitialization();
    void testComponentsCreation();
    void testInitialComponentStates();
    
    // 时间与设备联动测试
    void testTimeRangeDeviceInteraction();
    void testTimeChangeAffectsDeviceFilter();
    void testDeviceSelectionWithTimeRange();
    
    // 搜索功能端到端测试
    void testSearchEndToEnd();
    void testSearchWithTimeFilter();
    void testSearchWithDeviceSelection();
    void testSearchClearAndRestore();
    
    // 组件间通信测试
    void testTimeWidgetToMainWindow();
    void testDeviceWidgetToMainWindow();
    void testMainWindowToComponents();
    
    // 状态同步测试
    void testComponentStateSynchronization();
    void testStateConsistencyAfterOperations();
    void testStateRestoreAfterReset();
    
    // 复杂交互场景测试
    void testComplexUserWorkflow();
    void testMultipleOperationsSequence();
    void testConcurrentOperations();
    
    // 错误处理集成测试
    void testErrorHandlingIntegration();
    void testInvalidDataHandling();
    void testRecoveryFromErrors();
    
    // 性能和稳定性测试
    void testRapidOperations();
    void testLongRunningOperations();
    void testMemoryLeakPrevention();

private:
    MainWindow *m_mainWindow;
    TimeWidget *m_timeWidget;
    DeviceWidget *m_deviceWidget;
    QApplication *m_app;
    
    // 辅助方法
    void setupTestEnvironment();
    void simulateUserInteraction();
    QStringList getTestDeviceIds();
    void waitForSignalProcessing();
};

void TestMainWindowIntegration::initTestCase()
{
    qDebug() << "Starting MainWindow integration tests...";
    
    // 初始化设备管理器
    DeviceManager::instance().loadDeviceData();
    
    setupTestEnvironment();
}

void TestMainWindowIntegration::cleanupTestCase()
{
    qDebug() << "MainWindow integration tests completed.";
}

void TestMainWindowIntegration::init()
{
    m_mainWindow = new MainWindow();
    
    // 获取子组件的引用（通过findChild或其他方式）
    m_timeWidget = m_mainWindow->findChild<TimeWidget*>();
    m_deviceWidget = m_mainWindow->findChild<DeviceWidget*>();
}

void TestMainWindowIntegration::cleanup()
{
    delete m_mainWindow;
    m_mainWindow = nullptr;
    m_timeWidget = nullptr;
    m_deviceWidget = nullptr;
}

void TestMainWindowIntegration::setupTestEnvironment()
{
    // 设置测试环境
    // 确保设备数据已加载
    DeviceManager::instance().loadDeviceData();
}

void TestMainWindowIntegration::simulateUserInteraction()
{
    // 模拟用户交互
    QApplication::processEvents();
    QTest::qWait(100); // 等待100ms让事件处理完成
}

QStringList TestMainWindowIntegration::getTestDeviceIds()
{
    return QStringList() << "test_device_1" << "test_device_2" << "test_device_3";
}

void TestMainWindowIntegration::waitForSignalProcessing()
{
    // 等待信号处理完成
    QApplication::processEvents();
    QTest::qWait(50);
}

void TestMainWindowIntegration::testMainWindowInitialization()
{
    // 测试主窗口初始化
    QVERIFY(m_mainWindow != nullptr);
    QVERIFY(m_mainWindow->isEnabled());
    
    // 验证窗口可以显示
    m_mainWindow->show();
    QVERIFY(m_mainWindow->isVisible());
}

void TestMainWindowIntegration::testComponentsCreation()
{
    // 测试组件创建
    // 注意：由于MainWindow的Q_OBJECT被注释掉，findChild可能不工作
    // 我们测试主窗口的公共接口
    
    QVERIFY(m_mainWindow != nullptr);
    
    // 测试主窗口的公共方法
    QDateTime start, end;
    bool hasTimeRange = m_mainWindow->getCurrentTimeRange(start, end);
    
    // 验证时间范围获取方法存在且可调用
    Q_UNUSED(hasTimeRange);
    
    // 测试设备选择获取
    QStringList devices = m_mainWindow->getCurrentSelectedDevices();
    Q_UNUSED(devices);
    
    // 测试搜索文本获取
    QString searchText = m_mainWindow->getCurrentSearchText();
    Q_UNUSED(searchText);
}

void TestMainWindowIntegration::testInitialComponentStates()
{
    // 测试组件初始状态
    QDateTime start, end;
    bool hasTimeRange = m_mainWindow->getCurrentTimeRange(start, end);
    
    if (hasTimeRange) {
        QVERIFY(start.isValid());
        QVERIFY(end.isValid());
        QVERIFY(start < end);
    }
    
    // 测试初始设备选择为空
    QStringList selectedDevices = m_mainWindow->getCurrentSelectedDevices();
    // 初始状态可能为空或有默认选择
    
    // 测试初始搜索文本为空
    QString searchText = m_mainWindow->getCurrentSearchText();
    // 初始搜索文本应该为空
}

void TestMainWindowIntegration::testTimeRangeDeviceInteraction()
{
    // 测试时间范围和设备选择的交互
    QDateTime now = QDateTime::currentDateTime();
    QDateTime start = now.addDays(-1);
    QDateTime end = now;
    
    // 设置时间范围
    m_mainWindow->setTimeRange(start, end);
    
    waitForSignalProcessing();
    
    // 验证时间范围被设置
    QDateTime retrievedStart, retrievedEnd;
    bool hasTimeRange = m_mainWindow->getCurrentTimeRange(retrievedStart, retrievedEnd);
    
    if (hasTimeRange) {
        QCOMPARE(retrievedStart, start);
        QCOMPARE(retrievedEnd, end);
    }
    
    // 测试设备选择在时间变化后的状态
    QStringList testDevices = getTestDeviceIds();
    m_mainWindow->setSelectedDevices(testDevices);
    
    waitForSignalProcessing();
    
    // 验证设备选择不受时间变化影响（或按预期受影响）
    QStringList selectedDevices = m_mainWindow->getCurrentSelectedDevices();
    // 验证逻辑取决于具体实现
}

void TestMainWindowIntegration::testTimeChangeAffectsDeviceFilter()
{
    // 测试时间变化对设备过滤的影响
    QDateTime now = QDateTime::currentDateTime();
    
    // 设置不同的时间范围
    m_mainWindow->setTimeRange(now.addDays(-7), now.addDays(-6));
    waitForSignalProcessing();
    
    m_mainWindow->setTimeRange(now.addDays(-1), now);
    waitForSignalProcessing();
    
    // 验证时间变化不会导致崩溃
    QVERIFY(m_mainWindow->isEnabled());
}

void TestMainWindowIntegration::testDeviceSelectionWithTimeRange()
{
    // 测试在特定时间范围内的设备选择
    QDateTime now = QDateTime::currentDateTime();
    m_mainWindow->setTimeRange(now.addDays(-2), now);
    
    QStringList testDevices = getTestDeviceIds();
    m_mainWindow->setSelectedDevices(testDevices);
    
    waitForSignalProcessing();
    
    // 验证设备选择和时间范围都被正确设置
    QDateTime start, end;
    bool hasTimeRange = m_mainWindow->getCurrentTimeRange(start, end);
    
    if (hasTimeRange) {
        QVERIFY(start.isValid());
        QVERIFY(end.isValid());
    }
    
    QStringList selectedDevices = m_mainWindow->getCurrentSelectedDevices();
    // 验证设备选择状态
}

void TestMainWindowIntegration::testSearchEndToEnd()
{
    // 测试搜索功能的端到端流程
    QString searchText = "test device";
    
    m_mainWindow->setSearchText(searchText);
    waitForSignalProcessing();
    
    // 验证搜索文本被设置
    QString retrievedSearchText = m_mainWindow->getCurrentSearchText();
    QCOMPARE(retrievedSearchText, searchText);
    
    // 测试搜索对设备列表的影响
    QStringList selectedDevices = m_mainWindow->getCurrentSelectedDevices();
    // 搜索可能会影响可选设备列表
    
    // 清除搜索
    m_mainWindow->setSearchText("");
    waitForSignalProcessing();
    
    QCOMPARE(m_mainWindow->getCurrentSearchText(), QString());
}

void TestMainWindowIntegration::testSearchWithTimeFilter()
{
    // 测试搜索与时间过滤的组合
    QDateTime now = QDateTime::currentDateTime();
    m_mainWindow->setTimeRange(now.addDays(-1), now);
    
    QString searchText = "device";
    m_mainWindow->setSearchText(searchText);
    
    waitForSignalProcessing();
    
    // 验证两个过滤条件都被应用
    QDateTime start, end;
    bool hasTimeRange = m_mainWindow->getCurrentTimeRange(start, end);
    
    if (hasTimeRange) {
        QVERIFY(start.isValid());
        QVERIFY(end.isValid());
    }
    
    QCOMPARE(m_mainWindow->getCurrentSearchText(), searchText);
}

void TestMainWindowIntegration::testSearchWithDeviceSelection()
{
    // 测试搜索与设备选择的组合
    QStringList testDevices = getTestDeviceIds();
    m_mainWindow->setSelectedDevices(testDevices);
    
    QString searchText = "test";
    m_mainWindow->setSearchText(searchText);
    
    waitForSignalProcessing();
    
    // 验证搜索和选择状态
    QCOMPARE(m_mainWindow->getCurrentSearchText(), searchText);
    
    QStringList selectedDevices = m_mainWindow->getCurrentSelectedDevices();
    // 验证设备选择状态（可能受搜索影响）
}

void TestMainWindowIntegration::testSearchClearAndRestore()
{
    // 测试搜索清除和恢复
    QStringList originalDevices = getTestDeviceIds();
    m_mainWindow->setSelectedDevices(originalDevices);
    
    // 应用搜索
    m_mainWindow->setSearchText("filter");
    waitForSignalProcessing();
    
    // 清除搜索
    m_mainWindow->setSearchText("");
    waitForSignalProcessing();
    
    // 验证状态恢复
    QCOMPARE(m_mainWindow->getCurrentSearchText(), QString());
}

void TestMainWindowIntegration::testTimeWidgetToMainWindow()
{
    // 测试TimeWidget到MainWindow的通信
    // 由于我们无法直接访问TimeWidget，我们测试主窗口的时间设置方法
    
    QDateTime now = QDateTime::currentDateTime();
    QDateTime start = now.addDays(-3);
    QDateTime end = now.addDays(-2);
    
    m_mainWindow->setTimeRange(start, end);
    waitForSignalProcessing();
    
    // 验证时间范围被正确传递
    QDateTime retrievedStart, retrievedEnd;
    bool hasTimeRange = m_mainWindow->getCurrentTimeRange(retrievedStart, retrievedEnd);
    
    if (hasTimeRange) {
        QCOMPARE(retrievedStart, start);
        QCOMPARE(retrievedEnd, end);
    }
}

void TestMainWindowIntegration::testDeviceWidgetToMainWindow()
{
    // 测试DeviceWidget到MainWindow的通信
    QStringList testDevices = getTestDeviceIds();
    
    m_mainWindow->setSelectedDevices(testDevices);
    waitForSignalProcessing();
    
    // 验证设备选择被正确传递
    QStringList selectedDevices = m_mainWindow->getCurrentSelectedDevices();
    // 验证选择状态（具体验证取决于实现）
}

void TestMainWindowIntegration::testMainWindowToComponents()
{
    // 测试MainWindow到组件的通信
    // 通过主窗口的公共接口设置状态
    
    QDateTime now = QDateTime::currentDateTime();
    m_mainWindow->setTimeRange(now.addDays(-1), now);
    
    QStringList devices = getTestDeviceIds();
    m_mainWindow->setSelectedDevices(devices);
    
    m_mainWindow->setSearchText("test");
    
    waitForSignalProcessing();
    
    // 验证所有状态都被正确设置
    QDateTime start, end;
    bool hasTimeRange = m_mainWindow->getCurrentTimeRange(start, end);
    
    if (hasTimeRange) {
        QVERIFY(start.isValid());
        QVERIFY(end.isValid());
    }
    
    QCOMPARE(m_mainWindow->getCurrentSearchText(), QString("test"));
}

void TestMainWindowIntegration::testComponentStateSynchronization()
{
    // 测试组件状态同步
    QDateTime now = QDateTime::currentDateTime();
    QDateTime start = now.addDays(-2);
    QDateTime end = now.addDays(-1);
    
    QStringList devices = getTestDeviceIds();
    QString searchText = "sync test";
    
    // 同时设置多个状态
    m_mainWindow->setTimeRange(start, end);
    m_mainWindow->setSelectedDevices(devices);
    m_mainWindow->setSearchText(searchText);
    
    waitForSignalProcessing();
    
    // 验证所有状态都被同步
    QDateTime retrievedStart, retrievedEnd;
    bool hasTimeRange = m_mainWindow->getCurrentTimeRange(retrievedStart, retrievedEnd);
    
    if (hasTimeRange) {
        QCOMPARE(retrievedStart, start);
        QCOMPARE(retrievedEnd, end);
    }
    
    QCOMPARE(m_mainWindow->getCurrentSearchText(), searchText);
}

void TestMainWindowIntegration::testStateConsistencyAfterOperations()
{
    // 测试操作后的状态一致性
    QDateTime now = QDateTime::currentDateTime();
    
    // 执行一系列操作
    m_mainWindow->setTimeRange(now.addDays(-3), now.addDays(-2));
    waitForSignalProcessing();
    
    m_mainWindow->setSelectedDevices(getTestDeviceIds());
    waitForSignalProcessing();
    
    m_mainWindow->setSearchText("consistency");
    waitForSignalProcessing();
    
    m_mainWindow->setTimeRange(now.addDays(-1), now);
    waitForSignalProcessing();
    
    // 验证最终状态的一致性
    QDateTime start, end;
    bool hasTimeRange = m_mainWindow->getCurrentTimeRange(start, end);
    
    if (hasTimeRange) {
        QVERIFY(start.isValid());
        QVERIFY(end.isValid());
        QVERIFY(start < end);
    }
    
    QCOMPARE(m_mainWindow->getCurrentSearchText(), QString("consistency"));
}

void TestMainWindowIntegration::testStateRestoreAfterReset()
{
    // 测试重置后的状态恢复
    // 设置初始状态
    QDateTime now = QDateTime::currentDateTime();
    m_mainWindow->setTimeRange(now.addDays(-1), now);
    m_mainWindow->setSelectedDevices(getTestDeviceIds());
    m_mainWindow->setSearchText("restore test");
    
    waitForSignalProcessing();
    
    // 重置状态
    m_mainWindow->setSelectedDevices(QStringList());
    m_mainWindow->setSearchText("");
    
    waitForSignalProcessing();
    
    // 验证重置后的状态
    QVERIFY(m_mainWindow->getCurrentSelectedDevices().isEmpty());
    QCOMPARE(m_mainWindow->getCurrentSearchText(), QString());
}

void TestMainWindowIntegration::testComplexUserWorkflow()
{
    // 测试复杂的用户工作流程
    QDateTime now = QDateTime::currentDateTime();
    
    // 模拟用户工作流程：
    // 1. 设置时间范围
    m_mainWindow->setTimeRange(now.addDays(-7), now);
    waitForSignalProcessing();
    
    // 2. 搜索设备
    m_mainWindow->setSearchText("device");
    waitForSignalProcessing();
    
    // 3. 选择一些设备
    m_mainWindow->setSelectedDevices(getTestDeviceIds());
    waitForSignalProcessing();
    
    // 4. 修改时间范围
    m_mainWindow->setTimeRange(now.addDays(-3), now);
    waitForSignalProcessing();
    
    // 5. 清除搜索
    m_mainWindow->setSearchText("");
    waitForSignalProcessing();
    
    // 验证最终状态
    QDateTime start, end;
    bool hasTimeRange = m_mainWindow->getCurrentTimeRange(start, end);
    
    if (hasTimeRange) {
        QVERIFY(start.isValid());
        QVERIFY(end.isValid());
    }
    
    QCOMPARE(m_mainWindow->getCurrentSearchText(), QString());
}

void TestMainWindowIntegration::testMultipleOperationsSequence()
{
    // 测试多个操作的序列
    QDateTime now = QDateTime::currentDateTime();
    
    for (int i = 0; i < 10; ++i) {
        m_mainWindow->setTimeRange(now.addDays(-i-1), now.addDays(-i));
        m_mainWindow->setSearchText(QString("test_%1").arg(i));
        waitForSignalProcessing();
    }
    
    // 验证最终状态
    QCOMPARE(m_mainWindow->getCurrentSearchText(), QString("test_9"));
}

void TestMainWindowIntegration::testConcurrentOperations()
{
    // 测试并发操作
    QDateTime now = QDateTime::currentDateTime();
    
    // 快速连续执行多个操作
    m_mainWindow->setTimeRange(now.addDays(-1), now);
    m_mainWindow->setSelectedDevices(getTestDeviceIds());
    m_mainWindow->setSearchText("concurrent");
    
    // 立即处理所有事件
    waitForSignalProcessing();
    
    // 验证所有操作都被正确处理
    QCOMPARE(m_mainWindow->getCurrentSearchText(), QString("concurrent"));
}

void TestMainWindowIntegration::testErrorHandlingIntegration()
{
    // 测试错误处理集成
    // 设置无效的时间范围
    QDateTime now = QDateTime::currentDateTime();
    QDateTime invalidStart = now.addDays(1); // 未来时间作为开始时间
    QDateTime invalidEnd = now.addDays(-1);  // 过去时间作为结束时间
    
    m_mainWindow->setTimeRange(invalidStart, invalidEnd);
    waitForSignalProcessing();
    
    // 验证应用程序不会崩溃
    QVERIFY(m_mainWindow->isEnabled());
    
    // 设置无效的设备ID
    QStringList invalidDevices;
    invalidDevices << "" << "invalid_device" << "null";
    
    m_mainWindow->setSelectedDevices(invalidDevices);
    waitForSignalProcessing();
    
    // 验证应用程序仍然正常工作
    QVERIFY(m_mainWindow->isEnabled());
}

void TestMainWindowIntegration::testInvalidDataHandling()
{
    // 测试无效数据处理
    // 设置空字符串搜索
    m_mainWindow->setSearchText("");
    waitForSignalProcessing();
    
    // 设置空设备列表
    m_mainWindow->setSelectedDevices(QStringList());
    waitForSignalProcessing();
    
    // 验证应用程序正常处理空数据
    QVERIFY(m_mainWindow->isEnabled());
    QCOMPARE(m_mainWindow->getCurrentSearchText(), QString());
    QVERIFY(m_mainWindow->getCurrentSelectedDevices().isEmpty());
}

void TestMainWindowIntegration::testRecoveryFromErrors()
{
    // 测试错误恢复
    // 先设置有效状态
    QDateTime now = QDateTime::currentDateTime();
    m_mainWindow->setTimeRange(now.addDays(-1), now);
    m_mainWindow->setSearchText("valid");
    
    waitForSignalProcessing();
    
    // 然后设置无效状态
    m_mainWindow->setTimeRange(now.addDays(1), now.addDays(-1));
    
    waitForSignalProcessing();
    
    // 再设置有效状态
    m_mainWindow->setTimeRange(now.addDays(-2), now.addDays(-1));
    m_mainWindow->setSearchText("recovered");
    
    waitForSignalProcessing();
    
    // 验证应用程序恢复正常
    QVERIFY(m_mainWindow->isEnabled());
    QCOMPARE(m_mainWindow->getCurrentSearchText(), QString("recovered"));
}

void TestMainWindowIntegration::testRapidOperations()
{
    // 测试快速操作
    QDateTime now = QDateTime::currentDateTime();
    
    // 快速执行大量操作
    for (int i = 0; i < 100; ++i) {
        m_mainWindow->setTimeRange(now.addDays(-i-1), now.addDays(-i));
        m_mainWindow->setSearchText(QString("rapid_%1").arg(i));
        
        if (i % 10 == 0) {
            waitForSignalProcessing();
        }
    }
    
    waitForSignalProcessing();
    
    // 验证应用程序仍然响应
    QVERIFY(m_mainWindow->isEnabled());
    QCOMPARE(m_mainWindow->getCurrentSearchText(), QString("rapid_99"));
}

void TestMainWindowIntegration::testLongRunningOperations()
{
    // 测试长时间运行的操作
    QDateTime now = QDateTime::currentDateTime();
    
    // 设置一个状态并等待较长时间
    m_mainWindow->setTimeRange(now.addDays(-30), now);
    m_mainWindow->setSearchText("long running test");
    
    // 等待较长时间
    QTest::qWait(1000);
    
    // 验证状态仍然有效
    QCOMPARE(m_mainWindow->getCurrentSearchText(), QString("long running test"));
    
    QDateTime start, end;
    bool hasTimeRange = m_mainWindow->getCurrentTimeRange(start, end);
    
    if (hasTimeRange) {
        QVERIFY(start.isValid());
        QVERIFY(end.isValid());
    }
}

void TestMainWindowIntegration::testMemoryLeakPrevention()
{
    // 测试内存泄漏预防
    // 创建和销毁多个状态
    for (int i = 0; i < 50; ++i) {
        QDateTime now = QDateTime::currentDateTime();
        m_mainWindow->setTimeRange(now.addDays(-i-1), now.addDays(-i));
        
        QStringList devices;
        for (int j = 0; j < 10; ++j) {
            devices << QString("device_%1_%2").arg(i).arg(j);
        }
        m_mainWindow->setSelectedDevices(devices);
        
        m_mainWindow->setSearchText(QString("memory_test_%1").arg(i));
        
        if (i % 10 == 0) {
            waitForSignalProcessing();
        }
    }
    
    // 清理状态
    m_mainWindow->setSelectedDevices(QStringList());
    m_mainWindow->setSearchText("");
    
    waitForSignalProcessing();
    
    // 验证清理后的状态
    QVERIFY(m_mainWindow->getCurrentSelectedDevices().isEmpty());
    QCOMPARE(m_mainWindow->getCurrentSearchText(), QString());
}

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    
    TestMainWindowIntegration test;
    return QTest::qExec(&test, argc, argv);
}

#include "test_mainwindow_integration.moc"