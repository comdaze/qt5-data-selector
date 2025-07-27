#include "TimeWidget.h"
// #include "CustomButton.h" // Temporarily disabled
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QButtonGroup>
#include <QDateTimeEdit>
#include <QLabel>
#include <QDateTime>
#include <QDebug>
#include <QPushButton>

TimeWidget::TimeWidget(QWidget *parent)
    : QWidget(parent)
    , m_granularityGroup(nullptr)
    , m_quickTimeGroup(nullptr)
    , m_startTimeEdit(nullptr)
    , m_endTimeEdit(nullptr)
    , m_titleLabel(nullptr)
    , m_errorLabel(nullptr)
    , m_mainLayout(nullptr)
    , m_granularityLayout(nullptr)
    , m_quickTimeLayout(nullptr)
    , m_timeRangeLayout(nullptr)
    , m_currentGranularity(Hour1)
    , m_updatingTime(false)
{
    setupUI();
    
    // 设置默认时间范围（前一天）
    updateTimeRange(1);
}

void TimeWidget::setupUI()
{
    // 创建主布局
    m_mainLayout = new QVBoxLayout(this);
    m_mainLayout->setContentsMargins(10, 10, 10, 10);
    m_mainLayout->setSpacing(10);
    
    // 创建标题
    m_titleLabel = new QLabel("时间选择", this);
    m_titleLabel->setStyleSheet("font-weight: bold; font-size: 14px; color: #333;");
    m_mainLayout->addWidget(m_titleLabel);
    
    // 创建时间颗粒度按钮
    createGranularityButtons();
    
    // 创建快捷时间按钮
    createQuickTimeButtons();
    
    // 创建时间范围编辑器
    createTimeRangeEditors();
    
    // 创建错误提示标签
    m_errorLabel = new QLabel(this);
    m_errorLabel->setStyleSheet("color: red; font-size: 11px;");
    m_errorLabel->hide();
    m_mainLayout->addWidget(m_errorLabel);
}

void TimeWidget::createGranularityButtons()
{
    // 创建颗粒度按钮布局 - 响应式水平布局
    m_granularityLayout = new QHBoxLayout();
    m_granularityLayout->setSpacing(8);
    m_granularityLayout->setContentsMargins(0, 4, 0, 4);
    
    QLabel *granularityLabel = new QLabel("时间颗粒度:", this);
    granularityLabel->setStyleSheet("color: #cccccc; font-size: 12px; font-weight: bold;");
    granularityLabel->setMinimumWidth(80);
    granularityLabel->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);
    m_granularityLayout->addWidget(granularityLabel);
    
    // 创建按钮组
    m_granularityGroup = new QButtonGroup(this);
    
    // 创建颗粒度按钮
    QPushButton *btn15min = new QPushButton("15分钟", this);
    QPushButton *btn1hour = new QPushButton("1小时", this);
    QPushButton *btn1day = new QPushButton("1天", this);
    
    // 设置按钮为可选中状态和响应式大小策略
    btn15min->setCheckable(true);
    btn1hour->setCheckable(true);
    btn1day->setCheckable(true);
    
    btn15min->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    btn1hour->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    btn1day->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    
    btn15min->setMinimumWidth(70);
    btn1hour->setMinimumWidth(70);
    btn1day->setMinimumWidth(70);
    
    m_granularityGroup->addButton(btn15min, Minutes15);
    m_granularityGroup->addButton(btn1hour, Hour1);
    m_granularityGroup->addButton(btn1day, Day1);
    
    // 设置默认选中
    btn1hour->setChecked(true);
    
    m_granularityLayout->addWidget(btn15min);
    m_granularityLayout->addWidget(btn1hour);
    m_granularityLayout->addWidget(btn1day);
    
    // 添加弹性空间，使按钮在小窗口时不会过度拉伸
    m_granularityLayout->addStretch(1);
    
    m_mainLayout->addLayout(m_granularityLayout);
    
    // 连接信号
    connect(m_granularityGroup, QOverload<int>::of(&QButtonGroup::buttonClicked),
            this, &TimeWidget::onGranularityButtonClicked);
}

void TimeWidget::createQuickTimeButtons()
{
    // 创建快捷时间按钮布局 - 响应式水平布局
    m_quickTimeLayout = new QHBoxLayout();
    m_quickTimeLayout->setSpacing(8);
    m_quickTimeLayout->setContentsMargins(0, 4, 0, 4);
    
    QLabel *quickTimeLabel = new QLabel("快捷选择:", this);
    quickTimeLabel->setStyleSheet("color: #cccccc; font-size: 12px; font-weight: bold;");
    quickTimeLabel->setMinimumWidth(80);
    quickTimeLabel->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);
    m_quickTimeLayout->addWidget(quickTimeLabel);
    
    // 创建按钮组
    m_quickTimeGroup = new QButtonGroup(this);
    
    // 创建快捷时间按钮
    QPushButton *btnYesterday = new QPushButton("前一天", this);
    QPushButton *btn3Days = new QPushButton("前三天", this);
    
    // 设置按钮为可选中状态和响应式大小策略
    btnYesterday->setCheckable(true);
    btn3Days->setCheckable(true);
    
    btnYesterday->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    btn3Days->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    
    btnYesterday->setMinimumWidth(70);
    btn3Days->setMinimumWidth(70);
    
    m_quickTimeGroup->addButton(btnYesterday, 1);
    m_quickTimeGroup->addButton(btn3Days, 3);
    
    // 设置默认选中
    btnYesterday->setChecked(true);
    
    m_quickTimeLayout->addWidget(btnYesterday);
    m_quickTimeLayout->addWidget(btn3Days);
    
    // 添加弹性空间
    m_quickTimeLayout->addStretch(1);
    
    m_mainLayout->addLayout(m_quickTimeLayout);
    
    // 连接信号
    connect(m_quickTimeGroup, QOverload<int>::of(&QButtonGroup::buttonClicked),
            this, &TimeWidget::onQuickTimeButtonClicked);
}

void TimeWidget::createTimeRangeEditors()
{
    // 创建时间范围编辑器布局 - 响应式水平布局
    m_timeRangeLayout = new QHBoxLayout();
    m_timeRangeLayout->setSpacing(12);
    m_timeRangeLayout->setContentsMargins(0, 4, 0, 4);
    
    QLabel *rangeLabel = new QLabel("时间范围:", this);
    rangeLabel->setStyleSheet("color: #cccccc; font-size: 12px; font-weight: bold;");
    rangeLabel->setMinimumWidth(80);
    rangeLabel->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);
    m_timeRangeLayout->addWidget(rangeLabel);
    
    // 创建开始时间编辑器
    m_startTimeEdit = new QDateTimeEdit(this);
    m_startTimeEdit->setDisplayFormat("yyyy-MM-dd hh:mm");
    m_startTimeEdit->setCalendarPopup(true);
    m_startTimeEdit->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    m_startTimeEdit->setMinimumWidth(150);
    m_startTimeEdit->setMaximumWidth(200);
    m_timeRangeLayout->addWidget(m_startTimeEdit);
    
    QLabel *toLabel = new QLabel("至", this);
    toLabel->setStyleSheet("color: #cccccc; font-size: 12px; font-weight: bold;");
    toLabel->setAlignment(Qt::AlignCenter);
    toLabel->setMinimumWidth(20);
    toLabel->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);
    m_timeRangeLayout->addWidget(toLabel);
    
    // 创建结束时间编辑器
    m_endTimeEdit = new QDateTimeEdit(this);
    m_endTimeEdit->setDisplayFormat("yyyy-MM-dd hh:mm");
    m_endTimeEdit->setCalendarPopup(true);
    m_endTimeEdit->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    m_endTimeEdit->setMinimumWidth(150);
    m_endTimeEdit->setMaximumWidth(200);
    m_timeRangeLayout->addWidget(m_endTimeEdit);
    
    // 添加弹性空间，使时间编辑器在大窗口时不会过度拉伸
    m_timeRangeLayout->addStretch(1);
    
    m_mainLayout->addLayout(m_timeRangeLayout);
    
    // 连接信号
    connect(m_startTimeEdit, &QDateTimeEdit::dateTimeChanged,
            this, &TimeWidget::onDateTimeChanged);
    connect(m_endTimeEdit, &QDateTimeEdit::dateTimeChanged,
            this, &TimeWidget::onDateTimeChanged);
}

QDateTime TimeWidget::getStartTime() const
{
    return m_startTimeEdit ? m_startTimeEdit->dateTime() : QDateTime();
}

QDateTime TimeWidget::getEndTime() const
{
    return m_endTimeEdit ? m_endTimeEdit->dateTime() : QDateTime();
}

TimeWidget::TimeGranularity TimeWidget::getGranularity() const
{
    return m_currentGranularity;
}

void TimeWidget::setTimeRange(const QDateTime &start, const QDateTime &end)
{
    if (!m_startTimeEdit || !m_endTimeEdit) {
        return;
    }
    
    m_updatingTime = true;
    m_startTimeEdit->setDateTime(start);
    m_endTimeEdit->setDateTime(end);
    m_updatingTime = false;
    
    emitTimeRangeChanged();
}

void TimeWidget::onGranularityButtonClicked()
{
    int id = m_granularityGroup->checkedId();
    if (id >= 0) {
        m_currentGranularity = static_cast<TimeGranularity>(id);
        
        // 更新按钮选中状态
        for (QAbstractButton *button : m_granularityGroup->buttons()) {
            QPushButton *pushBtn = qobject_cast<QPushButton*>(button);
            if (pushBtn) {
                pushBtn->setChecked(m_granularityGroup->id(button) == id);
            }
        }
        
        emit granularityChanged(m_currentGranularity);
        qDebug() << "Granularity changed to:" << id;
    }
}

void TimeWidget::onQuickTimeButtonClicked()
{
    int daysBack = m_quickTimeGroup->checkedId();
    if (daysBack > 0) {
        // 更新按钮选中状态
        for (QAbstractButton *button : m_quickTimeGroup->buttons()) {
            QPushButton *pushBtn = qobject_cast<QPushButton*>(button);
            if (pushBtn) {
                pushBtn->setChecked(m_quickTimeGroup->id(button) == daysBack);
            }
        }
        
        updateTimeRange(daysBack);
        qDebug() << "Quick time selected:" << daysBack << "days back";
    }
}

void TimeWidget::onDateTimeChanged()
{
    if (m_updatingTime) {
        return;
    }
    
    // 验证时间格式
    if (!validateTimeFormat(m_startTimeEdit->dateTime())) {
        showError("开始时间格式无效");
        return;
    }
    
    if (!validateTimeFormat(m_endTimeEdit->dateTime())) {
        showError("结束时间格式无效");
        return;
    }
    
    // 验证时间范围
    if (!validateTimeRange()) {
        showError("结束时间必须晚于开始时间");
        return;
    }
    
    // 验证时间范围是否合理（不能超过1年）
    QDateTime start = m_startTimeEdit->dateTime();
    QDateTime end = m_endTimeEdit->dateTime();
    qint64 daysDiff = start.daysTo(end);
    
    if (daysDiff > 365) {
        showError("时间范围不能超过1年");
        return;
    }
    
    if (daysDiff < 0) {
        showError("时间范围无效");
        return;
    }
    
    // 所有验证通过
    clearError();
    emitTimeRangeChanged();
}

void TimeWidget::updateTimeRange(int daysBack)
{
    QDateTime now = QDateTime::currentDateTime();
    QDateTime start = now.addDays(-daysBack);
    
    setTimeRange(start, now);
}

bool TimeWidget::validateTimeRange() const
{
    if (!m_startTimeEdit || !m_endTimeEdit) {
        return false;
    }
    
    return m_startTimeEdit->dateTime() < m_endTimeEdit->dateTime();
}

bool TimeWidget::validateTimeFormat(const QDateTime &dateTime) const
{
    if (!dateTime.isValid()) {
        return false;
    }
    
    // 检查时间是否在合理范围内（1900年到2100年）
    QDateTime minTime(QDate(1900, 1, 1), QTime(0, 0, 0));
    QDateTime maxTime(QDate(2100, 12, 31), QTime(23, 59, 59));
    
    return dateTime >= minTime && dateTime <= maxTime;
}

void TimeWidget::showError(const QString &error)
{
    if (m_errorLabel) {
        m_errorLabel->setText(error);
        m_errorLabel->show();
        emit validationError(error);
        qDebug() << "TimeWidget validation error:" << error;
    }
}

void TimeWidget::clearError()
{
    if (m_errorLabel) {
        m_errorLabel->hide();
        m_errorLabel->clear();
    }
}

void TimeWidget::emitTimeRangeChanged()
{
    if (validateTimeRange() && validateTimeFormat(getStartTime()) && validateTimeFormat(getEndTime())) {
        emit timeRangeChanged(getStartTime(), getEndTime());
    }
}