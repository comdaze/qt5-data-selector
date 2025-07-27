#include <QApplication>
#include <QTest>
#include <QSignalSpy>
#include <QMouseEvent>
#include <QDebug>
#include "CustomButton.h"

/**
 * @brief CustomButton单元测试类
 * 
 * 测试CustomButton的状态管理、事件处理和信号发射
 * 需求: 4.3, 4.4
 */
class TestCustomButton : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void cleanupTestCase();
    void init();
    void cleanup();
    
    // 初始状态测试
    void testInitialState();
    void testInitialText();
    
    // 选中状态测试
    void testSetSelected();
    void testSelectedStateToggle();
    void testSelectedSignalEmission();
    
    // 悬停状态测试
    void testSetHovered();
    void testHoverStateToggle();
    void testMouseEnterLeave();
    
    // 鼠标事件测试
    void testMousePressRelease();
    void testClickEvent();
    void testMouseEventSequence();
    
    // 状态组合测试
    void testSelectedAndHovered();
    void testSelectedAndPressed();
    void testAllStatesActive();
    
    // 信号测试
    void testSelectedChangedSignal();
    void testMultipleSignalEmissions();
    void testSignalParameters();
    
    // 样式更新测试
    void testStyleUpdates();
    void testStateTransitions();
    
    // 边界条件测试
    void testRapidStateChanges();
    void testStateConsistency();

private:
    CustomButton *m_button;
    QApplication *m_app;
};

void TestCustomButton::initTestCase()
{
    qDebug() << "Starting CustomButton unit tests...";
}

void TestCustomButton::cleanupTestCase()
{
    qDebug() << "CustomButton unit tests completed.";
}

void TestCustomButton::init()
{
    m_button = new CustomButton("Test Button");
}

void TestCustomButton::cleanup()
{
    delete m_button;
    m_button = nullptr;
}

void TestCustomButton::testInitialState()
{
    // 测试初始状态
    QVERIFY(m_button != nullptr);
    QCOMPARE(m_button->isSelected(), false);
    QCOMPARE(m_button->isHovered(), false);
}

void TestCustomButton::testInitialText()
{
    // 测试初始文本
    QCOMPARE(m_button->text(), QString("Test Button"));
    
    // 测试创建时设置不同文本
    CustomButton *button2 = new CustomButton("Another Button");
    QCOMPARE(button2->text(), QString("Another Button"));
    delete button2;
}

void TestCustomButton::testSetSelected()
{
    // 测试设置选中状态
    QCOMPARE(m_button->isSelected(), false);
    
    m_button->setSelected(true);
    QCOMPARE(m_button->isSelected(), true);
    
    m_button->setSelected(false);
    QCOMPARE(m_button->isSelected(), false);
}

void TestCustomButton::testSelectedStateToggle()
{
    // 测试选中状态切换
    bool initialState = m_button->isSelected();
    
    m_button->setSelected(!initialState);
    QCOMPARE(m_button->isSelected(), !initialState);
    
    m_button->setSelected(initialState);
    QCOMPARE(m_button->isSelected(), initialState);
}

void TestCustomButton::testSelectedSignalEmission()
{
    // 测试选中状态变化信号
    QSignalSpy spy(m_button, &CustomButton::selectedChanged);
    
    // 设置为选中
    m_button->setSelected(true);
    QCOMPARE(spy.count(), 1);
    QCOMPARE(spy.takeFirst().at(0).toBool(), true);
    
    // 设置为未选中
    m_button->setSelected(false);
    QCOMPARE(spy.count(), 1);
    QCOMPARE(spy.takeFirst().at(0).toBool(), false);
    
    // 重复设置相同状态不应该发射信号
    m_button->setSelected(false);
    QCOMPARE(spy.count(), 0);
}

void TestCustomButton::testSetHovered()
{
    // 测试设置悬停状态
    QCOMPARE(m_button->isHovered(), false);
    
    m_button->setHovered(true);
    QCOMPARE(m_button->isHovered(), true);
    
    m_button->setHovered(false);
    QCOMPARE(m_button->isHovered(), false);
}

void TestCustomButton::testHoverStateToggle()
{
    // 测试悬停状态切换
    bool initialState = m_button->isHovered();
    
    m_button->setHovered(!initialState);
    QCOMPARE(m_button->isHovered(), !initialState);
    
    m_button->setHovered(initialState);
    QCOMPARE(m_button->isHovered(), initialState);
}

void TestCustomButton::testMouseEnterLeave()
{
    // 测试鼠标进入和离开事件
    QCOMPARE(m_button->isHovered(), false);
    
    // 模拟鼠标进入事件
    QEvent enterEvent(QEvent::Enter);
    QApplication::sendEvent(m_button, &enterEvent);
    QCOMPARE(m_button->isHovered(), true);
    
    // 模拟鼠标离开事件
    QEvent leaveEvent(QEvent::Leave);
    QApplication::sendEvent(m_button, &leaveEvent);
    QCOMPARE(m_button->isHovered(), false);
}

void TestCustomButton::testMousePressRelease()
{
    // 测试鼠标按下和释放事件
    m_button->show(); // 确保按钮可见以接收事件
    
    // 模拟鼠标按下事件
    QMouseEvent pressEvent(QEvent::MouseButtonPress, 
                          QPoint(10, 10), 
                          Qt::LeftButton, 
                          Qt::LeftButton, 
                          Qt::NoModifier);
    QApplication::sendEvent(m_button, &pressEvent);
    
    // 模拟鼠标释放事件
    QMouseEvent releaseEvent(QEvent::MouseButtonRelease, 
                            QPoint(10, 10), 
                            Qt::LeftButton, 
                            Qt::LeftButton, 
                            Qt::NoModifier);
    QApplication::sendEvent(m_button, &releaseEvent);
    
    // 验证按钮仍然正常工作
    QVERIFY(m_button->isEnabled());
}

void TestCustomButton::testClickEvent()
{
    // 测试点击事件
    QSignalSpy clickSpy(m_button, &CustomButton::clicked);
    
    m_button->show();
    
    // 模拟完整的点击序列
    QMouseEvent pressEvent(QEvent::MouseButtonPress, 
                          QPoint(10, 10), 
                          Qt::LeftButton, 
                          Qt::LeftButton, 
                          Qt::NoModifier);
    QApplication::sendEvent(m_button, &pressEvent);
    
    QMouseEvent releaseEvent(QEvent::MouseButtonRelease, 
                            QPoint(10, 10), 
                            Qt::LeftButton, 
                            Qt::LeftButton, 
                            Qt::NoModifier);
    QApplication::sendEvent(m_button, &releaseEvent);
    
    // 处理事件循环
    QApplication::processEvents();
    
    // 验证点击信号被发射
    QCOMPARE(clickSpy.count(), 1);
}

void TestCustomButton::testMouseEventSequence()
{
    // 测试完整的鼠标事件序列
    m_button->show();
    
    // 初始状态
    QCOMPARE(m_button->isHovered(), false);
    
    // 鼠标进入
    QEvent enterEvent(QEvent::Enter);
    QApplication::sendEvent(m_button, &enterEvent);
    QCOMPARE(m_button->isHovered(), true);
    
    // 鼠标按下
    QMouseEvent pressEvent(QEvent::MouseButtonPress, 
                          QPoint(10, 10), 
                          Qt::LeftButton, 
                          Qt::LeftButton, 
                          Qt::NoModifier);
    QApplication::sendEvent(m_button, &pressEvent);
    
    // 鼠标释放
    QMouseEvent releaseEvent(QEvent::MouseButtonRelease, 
                            QPoint(10, 10), 
                            Qt::LeftButton, 
                            Qt::LeftButton, 
                            Qt::NoModifier);
    QApplication::sendEvent(m_button, &releaseEvent);
    
    // 鼠标离开
    QEvent leaveEvent(QEvent::Leave);
    QApplication::sendEvent(m_button, &leaveEvent);
    QCOMPARE(m_button->isHovered(), false);
}

void TestCustomButton::testSelectedAndHovered()
{
    // 测试选中和悬停状态组合
    m_button->setSelected(true);
    m_button->setHovered(true);
    
    QCOMPARE(m_button->isSelected(), true);
    QCOMPARE(m_button->isHovered(), true);
    
    // 测试状态独立性
    m_button->setSelected(false);
    QCOMPARE(m_button->isSelected(), false);
    QCOMPARE(m_button->isHovered(), true);
    
    m_button->setHovered(false);
    QCOMPARE(m_button->isSelected(), false);
    QCOMPARE(m_button->isHovered(), false);
}

void TestCustomButton::testSelectedAndPressed()
{
    // 测试选中状态和按下状态的组合
    m_button->setSelected(true);
    QCOMPARE(m_button->isSelected(), true);
    
    m_button->show();
    
    // 模拟按下
    QMouseEvent pressEvent(QEvent::MouseButtonPress, 
                          QPoint(10, 10), 
                          Qt::LeftButton, 
                          Qt::LeftButton, 
                          Qt::NoModifier);
    QApplication::sendEvent(m_button, &pressEvent);
    
    // 选中状态应该保持
    QCOMPARE(m_button->isSelected(), true);
    
    // 模拟释放
    QMouseEvent releaseEvent(QEvent::MouseButtonRelease, 
                            QPoint(10, 10), 
                            Qt::LeftButton, 
                            Qt::LeftButton, 
                            Qt::NoModifier);
    QApplication::sendEvent(m_button, &releaseEvent);
    
    QCOMPARE(m_button->isSelected(), true);
}

void TestCustomButton::testAllStatesActive()
{
    // 测试所有状态同时激活
    m_button->setSelected(true);
    m_button->setHovered(true);
    
    QCOMPARE(m_button->isSelected(), true);
    QCOMPARE(m_button->isHovered(), true);
    
    // 验证状态可以独立控制
    m_button->setSelected(false);
    QCOMPARE(m_button->isSelected(), false);
    QCOMPARE(m_button->isHovered(), true);
}

void TestCustomButton::testSelectedChangedSignal()
{
    // 详细测试selectedChanged信号
    QSignalSpy spy(m_button, &CustomButton::selectedChanged);
    
    // 从false到true
    m_button->setSelected(true);
    QCOMPARE(spy.count(), 1);
    QList<QVariant> args = spy.takeFirst();
    QCOMPARE(args.size(), 1);
    QCOMPARE(args.at(0).toBool(), true);
    
    // 从true到false
    m_button->setSelected(false);
    QCOMPARE(spy.count(), 1);
    args = spy.takeFirst();
    QCOMPARE(args.at(0).toBool(), false);
    
    // 重复设置相同值不应该发射信号
    m_button->setSelected(false);
    QCOMPARE(spy.count(), 0);
    
    m_button->setSelected(true);
    QCOMPARE(spy.count(), 1);
    
    m_button->setSelected(true);
    QCOMPARE(spy.count(), 0);
}

void TestCustomButton::testMultipleSignalEmissions()
{
    // 测试多次信号发射
    QSignalSpy spy(m_button, &CustomButton::selectedChanged);
    
    // 多次切换状态
    for (int i = 0; i < 5; ++i) {
        m_button->setSelected(true);
        m_button->setSelected(false);
    }
    
    // 应该发射了10次信号（每次切换发射一次）
    QCOMPARE(spy.count(), 10);
}

void TestCustomButton::testSignalParameters()
{
    // 测试信号参数的正确性
    QSignalSpy spy(m_button, &CustomButton::selectedChanged);
    
    // 测试多次状态变化的参数
    bool expectedStates[] = {true, false, true, false};
    
    for (bool state : expectedStates) {
        m_button->setSelected(state);
        QCOMPARE(spy.count(), 1);
        QList<QVariant> args = spy.takeFirst();
        QCOMPARE(args.at(0).toBool(), state);
    }
}

void TestCustomButton::testStyleUpdates()
{
    // 测试样式更新（间接测试，通过状态变化）
    QString initialStyleSheet = m_button->styleSheet();
    
    // 改变状态应该触发样式更新
    m_button->setSelected(true);
    // 注意：样式更新是内部实现，我们主要验证状态正确
    QCOMPARE(m_button->isSelected(), true);
    
    m_button->setHovered(true);
    QCOMPARE(m_button->isHovered(), true);
}

void TestCustomButton::testStateTransitions()
{
    // 测试状态转换的完整性
    struct StateTransition {
        bool selected;
        bool hovered;
    };
    
    StateTransition transitions[] = {
        {false, false},
        {true, false},
        {true, true},
        {false, true},
        {false, false}
    };
    
    for (const auto& transition : transitions) {
        m_button->setSelected(transition.selected);
        m_button->setHovered(transition.hovered);
        
        QCOMPARE(m_button->isSelected(), transition.selected);
        QCOMPARE(m_button->isHovered(), transition.hovered);
    }
}

void TestCustomButton::testRapidStateChanges()
{
    // 测试快速状态变化
    QSignalSpy spy(m_button, &CustomButton::selectedChanged);
    
    // 快速切换状态
    for (int i = 0; i < 100; ++i) {
        m_button->setSelected(i % 2 == 0);
    }
    
    // 验证最终状态
    QCOMPARE(m_button->isSelected(), true); // 100是偶数，所以最终是true
    QCOMPARE(spy.count(), 100); // 每次变化都应该发射信号
}

void TestCustomButton::testStateConsistency()
{
    // 测试状态一致性
    // 设置各种状态组合，验证状态保持一致
    
    m_button->setSelected(true);
    m_button->setHovered(true);
    
    // 验证状态保持
    QCOMPARE(m_button->isSelected(), true);
    QCOMPARE(m_button->isHovered(), true);
    
    // 模拟一些事件处理
    QApplication::processEvents();
    
    // 状态应该保持不变
    QCOMPARE(m_button->isSelected(), true);
    QCOMPARE(m_button->isHovered(), true);
    
    // 重置状态
    m_button->setSelected(false);
    m_button->setHovered(false);
    
    QCOMPARE(m_button->isSelected(), false);
    QCOMPARE(m_button->isHovered(), false);
}

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    
    TestCustomButton test;
    return QTest::qExec(&test, argc, argv);
}

#include "test_custombutton_unit.moc"