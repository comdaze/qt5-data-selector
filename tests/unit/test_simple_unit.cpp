#include <QApplication>
#include <QTest>
#include <QDebug>

/**
 * @brief 简单单元测试类
 * 
 * 测试基本的Qt功能和测试框架本身
 */
class TestSimple : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void cleanupTestCase();
    
    // 基本测试
    void testQtBasics();
    void testStringOperations();
    void testDateTimeOperations();
    void testListOperations();

private:
};

void TestSimple::initTestCase()
{
    qDebug() << "Starting simple unit tests...";
}

void TestSimple::cleanupTestCase()
{
    qDebug() << "Simple unit tests completed.";
}

void TestSimple::testQtBasics()
{
    // 测试基本Qt功能
    QString testString = "Hello Qt";
    QCOMPARE(testString.length(), 8);
    QVERIFY(testString.contains("Qt"));
    
    QStringList list;
    list << "item1" << "item2" << "item3";
    QCOMPARE(list.size(), 3);
    QVERIFY(list.contains("item2"));
}

void TestSimple::testStringOperations()
{
    // 测试字符串操作
    QString original = "Test String";
    QString upper = original.toUpper();
    QString lower = original.toLower();
    
    QCOMPARE(upper, QString("TEST STRING"));
    QCOMPARE(lower, QString("test string"));
    
    QStringList parts = original.split(" ");
    QCOMPARE(parts.size(), 2);
    QCOMPARE(parts[0], QString("Test"));
    QCOMPARE(parts[1], QString("String"));
}

void TestSimple::testDateTimeOperations()
{
    // 测试日期时间操作
    QDateTime now = QDateTime::currentDateTime();
    QDateTime future = now.addDays(1);
    QDateTime past = now.addDays(-1);
    
    QVERIFY(now.isValid());
    QVERIFY(future.isValid());
    QVERIFY(past.isValid());
    
    QVERIFY(past < now);
    QVERIFY(now < future);
    QVERIFY(past < future);
    
    qint64 dayInSeconds = 24 * 60 * 60;
    qint64 actualDiff = past.secsTo(now);
    QVERIFY(qAbs(actualDiff - dayInSeconds) < 60); // 允许1分钟误差
}

void TestSimple::testListOperations()
{
    // 测试列表操作
    QStringList list;
    QVERIFY(list.isEmpty());
    
    list << "first" << "second" << "third";
    QCOMPARE(list.size(), 3);
    QVERIFY(!list.isEmpty());
    
    list.removeAt(1);
    QCOMPARE(list.size(), 2);
    QCOMPARE(list[0], QString("first"));
    QCOMPARE(list[1], QString("third"));
    
    list.clear();
    QVERIFY(list.isEmpty());
}

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    
    TestSimple test;
    return QTest::qExec(&test, argc, argv);
}

#include "test_simple_unit.moc"