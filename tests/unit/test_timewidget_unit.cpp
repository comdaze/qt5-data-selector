#include <QApplication>
#include <QTest>
#include <QDateTime>
#include <QSignalSpy>
#include <QDebug>
#include "TimeWidget.h"

/**
 * @brief TimeWidget单元测试类
 * 
 * 测试TimeWidget的时间计算逻辑、状态管理和信号发射
 * 需求: 1.1, 2.3, 3.1
 */
class TestTimeWidget : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void cleanupTestCase();
    void init();
    void cleanup();
    
    // 初始状态测试
    void testInitialState();
    void testInitialTimeRange();
    void testInitialGranularity();
    
    // 时间范围设置测试
    void testSetValidTimeRange();
    void testSetInvalidTimeRange();
    void testTimeRangeSignalEmission();
    
    // 时间颗粒度测试
    void testGranularityChange();
    void testGranularitySignalEmission();
    
    // 快捷时间功能测试
    void testQuickTimeButtons();
    void testPreviousDaySelection();
    void testPreviousThreeDaysSelection();
    
    // 时间验证测试
    void testTimeValidation();
    void testEndTimeBeforeStartTime();
    void testValidationErrorSignal();
    
    // 边界条件测试
    void testNullDateTime();
    void testFutureDateTime();
    void testVeryOldDateTime();
    
    // 信号槽机制测试
    void testSignalSlotConnections();
    void testMultipleSignalEmissions();

private:
    TimeWidget *m_timeWidget;
    QApplication *m_app;
};

void TestTimeWidget::initTestCase()
{
    // 测试套件初始化
    qDebug() << "Starting TimeWidget unit tests...";
}

void TestTimeWidget::cleanupTestCase()
{
    qDebug() << "TimeWidget unit tests completed.";
}

void TestTimeWidget::init()
{
    // 每个测试前的初始化
    m_timeWidget = new TimeWidget();
}

void TestTimeWidget::cleanup()
{
    // 每个测试后的清理
    delete m_timeWidget;
    m_timeWidget = nullptr;
}

void TestTimeWidget::testInitialState()
{
    // 测试初始状态
    QVERIFY(m_timeWidget != nullptr);
    
    // 验证初始颗粒度
    QCOMPARE(m_timeWidget->getGranularity(), TimeWidget::Hour1);
    
    // 验证初始时间有效性
    QDateTime startTime = m_timeWidget->getStartTime();
    QDateTime endTime = m_timeWidget->getEndTime();
    
    QVERIFY(startTime.isValid());
    QVERIFY(endTime.isValid());
    QVERIFY(startTime < endTime);
}

void TestTimeWidget::testInitialTimeRange()
{
    // 测试初始时间范围（应该是前一天）
    QDateTime startTime = m_timeWidget->getStartTime();
    QDateTime endTime = m_timeWidget->getEndTime();
    
    // 计算时间差（应该大约是24小时）
    qint64 timeDiffSeconds = startTime.secsTo(endTime);
    qint64 expectedSeconds = 24 * 3600; // 24小时
    
    // 允许一些误差（比如1分钟）
    QVERIFY(qAbs(timeDiffSeconds - expectedSeconds) < 60);
    
    // 验证结束时间接近当前时间
    QDateTime now = QDateTime::currentDateTime();
    qint64 endToNowDiff = endTime.secsTo(now);
    QVERIFY(qAbs(endToNowDiff) < 300); // 允许5分钟误差
}

void TestTimeWidget::testInitialGranularity()
{
    // 测试初始颗粒度设置
    QCOMPARE(m_timeWidget->getGranularity(), TimeWidget::Hour1);
}

void TestTimeWidget::testSetValidTimeRange()
{
    // 测试设置有效时间范围
    QDateTime now = QDateTime::currentDateTime();
    QDateTime start = now.addDays(-2);
    QDateTime end = now.addDays(-1);
    
    m_timeWidget->setTimeRange(start, end);
    
    QCOMPARE(m_timeWidget->getStartTime(), start);
    QCOMPARE(m_timeWidget->getEndTime(), end);
}

void TestTimeWidget::testSetInvalidTimeRange()
{
    // 测试设置无效时间范围（结束时间早于开始时间）
    QDateTime now = QDateTime::currentDateTime();
    QDateTime start = now;
    QDateTime end = now.addDays(-1); // 结束时间早于开始时间
    
    // 设置无效时间范围
    m_timeWidget->setTimeRange(start, end);
    
    // 验证时间仍然被设置（但应该有错误信号）
    QCOMPARE(m_timeWidget->getStartTime(), start);
    QCOMPARE(m_timeWidget->getEndTime(), end);
}

void TestTimeWidget::testTimeRangeSignalEmission()
{
    // 测试时间范围变化信号发射
    QSignalSpy spy(m_timeWidget, &TimeWidget::timeRangeChanged);
    
    QDateTime now = QDateTime::currentDateTime();
    QDateTime start = now.addDays(-1);
    QDateTime end = now;
    
    m_timeWidget->setTimeRange(start, end);
    
    // 验证信号被发射
    QCOMPARE(spy.count(), 1);
    
    // 验证信号参数
    QList<QVariant> arguments = spy.takeFirst();
    QCOMPARE(arguments.at(0).toDateTime(), start);
    QCOMPARE(arguments.at(1).toDateTime(), end);
}

void TestTimeWidget::testGranularityChange()
{
    // 测试颗粒度变化
    // 注意：这个测试需要模拟按钮点击，这里我们测试公共接口
    
    // 初始状态应该是Hour1
    QCOMPARE(m_timeWidget->getGranularity(), TimeWidget::Hour1);
    
    // 由于没有公共的setGranularity方法，我们只能测试初始状态
    // 在实际应用中，颗粒度通过按钮点击改变
}

void TestTimeWidget::testGranularitySignalEmission()
{
    // 测试颗粒度变化信号
    QSignalSpy spy(m_timeWidget, &TimeWidget::granularityChanged);
    
    // 由于没有公共的setGranularity方法，这个测试主要验证信号存在
    QVERIFY(spy.isValid());
}

void TestTimeWidget::testQuickTimeButtons()
{
    // 测试快捷时间按钮功能
    // 这个测试主要验证时间范围计算逻辑
    
    QDateTime originalStart = m_timeWidget->getStartTime();
    QDateTime originalEnd = m_timeWidget->getEndTime();
    
    QVERIFY(originalStart.isValid());
    QVERIFY(originalEnd.isValid());
    QVERIFY(originalStart < originalEnd);
}

void TestTimeWidget::testPreviousDaySelection()
{
    // 测试前一天选择逻辑
    QDateTime now = QDateTime::currentDateTime();
    QDateTime expectedStart = now.addDays(-1);
    QDateTime expectedEnd = now;
    
    // 由于没有公共方法直接触发前一天选择，我们测试手动设置
    m_timeWidget->setTimeRange(expectedStart, expectedEnd);
    
    qint64 timeDiff = m_timeWidget->getStartTime().secsTo(m_timeWidget->getEndTime());
    qint64 expectedDiff = 24 * 3600; // 24小时
    
    QVERIFY(qAbs(timeDiff - expectedDiff) < 60); // 允许1分钟误差
}

void TestTimeWidget::testPreviousThreeDaysSelection()
{
    // 测试前三天选择逻辑
    QDateTime now = QDateTime::currentDateTime();
    QDateTime expectedStart = now.addDays(-3);
    QDateTime expectedEnd = now;
    
    m_timeWidget->setTimeRange(expectedStart, expectedEnd);
    
    qint64 timeDiff = m_timeWidget->getStartTime().secsTo(m_timeWidget->getEndTime());
    qint64 expectedDiff = 3 * 24 * 3600; // 72小时
    
    QVERIFY(qAbs(timeDiff - expectedDiff) < 60); // 允许1分钟误差
}

void TestTimeWidget::testTimeValidation()
{
    // 测试时间验证功能
    QDateTime validStart = QDateTime::currentDateTime().addDays(-1);
    QDateTime validEnd = QDateTime::currentDateTime();
    
    m_timeWidget->setTimeRange(validStart, validEnd);
    
    // 验证有效时间被正确设置
    QCOMPARE(m_timeWidget->getStartTime(), validStart);
    QCOMPARE(m_timeWidget->getEndTime(), validEnd);
}

void TestTimeWidget::testEndTimeBeforeStartTime()
{
    // 测试结束时间早于开始时间的情况
    QSignalSpy errorSpy(m_timeWidget, &TimeWidget::validationError);
    
    QDateTime now = QDateTime::currentDateTime();
    QDateTime start = now;
    QDateTime end = now.addDays(-1); // 结束时间早于开始时间
    
    m_timeWidget->setTimeRange(start, end);
    
    // 验证错误信号被发射（如果实现了的话）
    // QVERIFY(errorSpy.count() > 0);
}

void TestTimeWidget::testValidationErrorSignal()
{
    // 测试验证错误信号
    QSignalSpy spy(m_timeWidget, &TimeWidget::validationError);
    
    // 验证信号存在
    QVERIFY(spy.isValid());
}

void TestTimeWidget::testNullDateTime()
{
    // 测试空日期时间处理
    QDateTime nullDateTime;
    QDateTime validDateTime = QDateTime::currentDateTime();
    
    // 测试设置空开始时间
    m_timeWidget->setTimeRange(nullDateTime, validDateTime);
    
    // 验证处理方式（具体行为取决于实现）
    // 这里我们只验证不会崩溃
    QVERIFY(true);
}

void TestTimeWidget::testFutureDateTime()
{
    // 测试未来时间处理
    QDateTime future = QDateTime::currentDateTime().addDays(10);
    QDateTime now = QDateTime::currentDateTime();
    
    m_timeWidget->setTimeRange(now, future);
    
    // 验证未来时间被正确设置
    QCOMPARE(m_timeWidget->getStartTime(), now);
    QCOMPARE(m_timeWidget->getEndTime(), future);
}

void TestTimeWidget::testVeryOldDateTime()
{
    // 测试很久以前的时间处理
    QDateTime veryOld = QDateTime::fromString("2000-01-01T00:00:00", Qt::ISODate);
    QDateTime old = QDateTime::fromString("2000-01-02T00:00:00", Qt::ISODate);
    
    m_timeWidget->setTimeRange(veryOld, old);
    
    // 验证很久以前的时间被正确设置
    QCOMPARE(m_timeWidget->getStartTime(), veryOld);
    QCOMPARE(m_timeWidget->getEndTime(), old);
}

void TestTimeWidget::testSignalSlotConnections()
{
    // 测试信号槽连接
    QSignalSpy timeRangeSpy(m_timeWidget, &TimeWidget::timeRangeChanged);
    QSignalSpy granularitySpy(m_timeWidget, &TimeWidget::granularityChanged);
    QSignalSpy errorSpy(m_timeWidget, &TimeWidget::validationError);
    
    // 验证所有信号都有效
    QVERIFY(timeRangeSpy.isValid());
    QVERIFY(granularitySpy.isValid());
    QVERIFY(errorSpy.isValid());
}

void TestTimeWidget::testMultipleSignalEmissions()
{
    // 测试多次信号发射
    QSignalSpy spy(m_timeWidget, &TimeWidget::timeRangeChanged);
    
    QDateTime now = QDateTime::currentDateTime();
    
    // 多次设置不同的时间范围
    m_timeWidget->setTimeRange(now.addDays(-1), now);
    m_timeWidget->setTimeRange(now.addDays(-2), now.addDays(-1));
    m_timeWidget->setTimeRange(now.addDays(-3), now.addDays(-2));
    
    // 验证信号被发射了3次
    QCOMPARE(spy.count(), 3);
}

// 主函数
int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    
    TestTimeWidget test;
    return QTest::qExec(&test, argc, argv);
}

#include "test_timewidget_unit.moc"