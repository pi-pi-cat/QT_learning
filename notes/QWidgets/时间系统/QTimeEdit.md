# Qt QTimeEdit 深度教程

<details> <summary><b>1️⃣ 原理深度解构层</b></summary>

## QTimeEdit 基本概念

QTimeEdit 是 Qt 提供的一个专门用于时间编辑的小部件，它继承自 QDateTimeEdit 类，而后者又继承自 QAbstractSpinBox。QTimeEdit 专注于时间编辑功能，去除了日期编辑部分，使用户可以方便地输入和修改时间值。

### 三线解析法

#### ① 运行时行为（对象生命周期/事件传递顺序）

- **初始化流程**：
  1. QTimeEdit 构造时创建私有数据结构 QDateTimeEditPrivate
  2. 注册内部属性变化的信号与槽
  3. 设置基本显示格式为 "HH:mm:ss"
  4. 初始化时间范围（默认从 00:00:00 到 23:59:59.999）
- **交互流程**：
  1. 用户点击上下箭头 → QAbstractSpinBox::stepBy() → QDateTimeEdit::stepBy()
  2. 用户编辑文本 → QLineEdit 处理输入 → QDateTimeEdit::validate() → 格式验证
  3. 焦点离开 → fixup() 方法被调用 → 格式化输入为有效时间
  4. 用户按下 Tab 键 → 在时、分、秒部分之间切换
- **事件传递**：
  1. 鼠标/键盘事件 → QAbstractSpinBox 事件处理器
  2. 文本编辑事件 → 内部 QLineEdit 处理
  3. 时间变更 → timeChanged() 信号触发
  4. 部分切换 → currentSectionChanged() 信号触发

#### ② 框架源码线索

- **核心类**：

  - `QTimeEdit` 在 `qdatetimeedit.h`
  - `QDateTimeEditPrivate` 在 `qdatetimeedit_p.h`
  - `QAbstractSpinBoxPrivate` 在 `qabstractspinbox_p.h`

- **关键源文件**：

  - 实现文件：`qdatetimeedit.cpp`
  - 私有实现：`qdatetimeedit_p.h`

- **内部数据结构**：

  ```cpp
  // 简化版的内部数据结构
  class QDateTimeEditPrivate : public QAbstractSpinBoxPrivate
  {
      Q_DECLARE_PUBLIC(QDateTimeEdit)
  public:
      QDateTimeEditPrivate();
  
      // 存储当前日期时间（QTimeEdit仅使用时间部分）
      QDateTime value;
      
      // 时间范围
      QDateTime minimum;
      QDateTime maximum;
      
      // 显示格式
      QString displayFormat;
      
      // 分段信息
      struct SectionInfo {
          int pos;
          int count;
          QDateTimeEdit::Section section;
          // 其他字段...
      };
      QList<SectionInfo> sectionsList;
      
      // 显示配置
      QDateTimeEdit::Sections sections;
      QDateTimeEdit::Section currentSection;
      
      // 步进值
      int sectionNodes[QDateTimeEdit::MaxSectionNodeCount];
      
      // 内部帮助函数
      int absoluteMax(QDateTimeEdit::Section s) const;
      int absoluteMin(QDateTimeEdit::Section s) const;
      // 其他辅助方法...
  };
  ```

#### ③ 计算机科学映射

- **设计模式**：
  - **MVC 模式**：QTimeEdit 作为视图和控制器，QTime 作为模型
  - **装饰者模式**：QTimeEdit 是 QDateTimeEdit 的特化装饰器
  - **策略模式**：不同的显示格式策略可以通过 displayFormat 配置
- **算法映射**：
  - 时间解析使用有限状态机实现格式字符串解析
  - 时间验证使用约束传播算法确保时间合法性
  - 区间限制实现采用双边界约束检查算法
  - 时间进位使用基于模运算的进位算法
- **理论基础**：
  - 人机交互理论中的直接操作交互模型
  - 格式化输入的语法分析理论
  - 时间计算的模运算理论

### 内存可视化

```
QTimeEdit (QWidget)
├── QDateTimeEditPrivate (内部私有对象)
│   ├── m_lineEdit (QLineEdit)       // 父对象析构时自动删除
│   ├── sectionsList (QList<SectionInfo>)  // 存储时间各部分的信息
│   └── cachedText (QString)         // 缓存显示文本
├── QAbstractSpinBoxPrivate (继承的私有对象)
│   ├── edit (QLineEdit*)           // 实际文本编辑区域
│   ├── upButton (QSpinBoxPrivateButton*) // 向上按钮
│   └── downButton (QSpinBoxPrivateButton*) // 向下按钮
└── m_contentSection (Section)       // 当前选中的时间部分 (时/分/秒/毫秒)
```

</details> <details> <summary><b>2️⃣ 代码多维训练场</b></summary>

## 基础层 - 核心API示例

```cpp
// 基本使用 - 10行内展示核心API
QTimeEdit *timeEdit = new QTimeEdit(QTime::currentTime(), parentWidget);
timeEdit->setDisplayFormat("HH:mm:ss");  // 设置显示格式
timeEdit->setTimeRange(QTime(8, 0, 0), QTime(18, 0, 0));  // 设置8:00-18:00工作时间范围
timeEdit->setCurrentSection(QTimeEdit::MinuteSection);  // 默认选中分钟部分
timeEdit->setTime(QTime(12, 30, 0));  // 设置当前时间为12:30
connect(timeEdit, &QTimeEdit::timeChanged, 
        [](const QTime &time) { qDebug() << "Time changed:" << time.toString(); });
// 线程安全: QTimeEdit 非线程安全, 必须在创建它的线程中使用
// 平台限制: 所有Qt支持平台通用, 但AM/PM显示会受系统区域设置影响
```

## 进阶层 - 场景化案例

```cpp
// 自定义时间编辑器 - 带错误处理
class WorkingHoursTimeEdit : public QTimeEdit {
    Q_OBJECT
public:
    WorkingHoursTimeEdit(QWidget *parent = nullptr) : QTimeEdit(parent) {
        // 设置工作时间范围 (9:00 - 17:30)
        setTimeRange(QTime(9, 0, 0), QTime(17, 30, 0));
        setDisplayFormat("hh:mm AP");  // 使用12小时制与AM/PM显示
        
        // 设置默认步进间隔为15分钟
        setCurrentSection(QTimeEdit::MinuteSection);
        setSingleStep(15);  // 15分钟为一步
        
        // 自定义样式
        setStyleSheet("QTimeEdit { padding: 2px; border: 1px solid #C0C0C0; }");
        
        // 连接信号以验证时间
        connect(this, &QTimeEdit::timeChanged, this, &WorkingHoursTimeEdit::validateTime);
        
        // 初始化为当前时间或9:00
        QTime current = QTime::currentTime();
        if (current < QTime(9, 0) || current > QTime(17, 30)) {
            setTime(QTime(9, 0));
        } else {
            // 将分钟四舍五入到最近的15分钟
            int minutes = current.minute();
            int roundedMinutes = ((minutes + 7) / 15) * 15;
            if (roundedMinutes == 60) {
                setTime(QTime(current.hour() + 1, 0));
            } else {
                setTime(QTime(current.hour(), roundedMinutes));
            }
        }
    }
    
protected:
    void keyPressEvent(QKeyEvent *event) override {
        // 增强键盘导航: 允许左右箭头切换部分
        if (event->key() == Qt::Key_Left) {
            switch (currentSection()) {
                case QTimeEdit::SecondSection:
                    setCurrentSection(QTimeEdit::MinuteSection);
                    break;
                case QTimeEdit::MinuteSection:
                    setCurrentSection(QTimeEdit::HourSection);
                    break;
                default:
                    // 已经是最左侧部分
                    break;
            }
            event->accept();
            return;
        } else if (event->key() == Qt::Key_Right) {
            switch (currentSection()) {
                case QTimeEdit::HourSection:
                    setCurrentSection(QTimeEdit::MinuteSection);
                    break;
                case QTimeEdit::MinuteSection:
                    if (displayFormat().contains("ss")) {
                        setCurrentSection(QTimeEdit::SecondSection);
                    }
                    break;
                default:
                    // 已经是最右侧部分
                    break;
            }
            event->accept();
            return;
        }
        
        // 其他情况调用基类处理
        QTimeEdit::keyPressEvent(event);
    }
    
    void focusOutEvent(QFocusEvent *event) override {
        // 离开时确保时间有效
        QTime currentTime = time();
        if (!isTimeValid(currentTime)) {
            setTime(QTime(9, 0));
        }
        QTimeEdit::focusOutEvent(event);
    }
    
private slots:
    void validateTime(const QTime &time) {
        // 检查周末或工作时间外的时间
        if (!isTimeValid(time)) {
            setStyleSheet("QTimeEdit { background-color: #FFEEEE; padding: 2px; border: 1px solid #FFA0A0; }");
            emit timeWarning(time, tr("Selected time is outside working hours"));
        } else {
            setStyleSheet("QTimeEdit { padding: 2px; border: 1px solid #C0C0C0; }");
        }
    }
    
private:
    bool isTimeValid(const QTime &time) {
        // 检查工作时间
        return time >= QTime(9, 0) && time <= QTime(17, 30);
    }
    
signals:
    void timeWarning(const QTime &time, const QString &message);
};

// ⚠️ 注意: 以上代码兼容 Qt 5.6+ 和 Qt 6.x, 较早版本可能需要调整信号槽连接方式
```

## 专家层 - 最佳实践方案

```cpp
/**
 * 高级时间选择器实现 - 企业级应用版本
 * 包含:
 * - 智能时间间隔支持
 * - 高级格式化和验证
 * - 上下文感知的时间建议
 * - 性能优化
 */
class EnhancedTimeEdit : public QTimeEdit {
    Q_OBJECT
    Q_PROPERTY(bool allowInvalidTime READ allowInvalidTime WRITE setAllowInvalidTime NOTIFY allowInvalidTimeChanged)
    Q_PROPERTY(bool useSmartInterval READ useSmartInterval WRITE setUseSmartInterval)
    Q_PROPERTY(QVector<QTime> restrictedTimes READ restrictedTimes WRITE setRestrictedTimes)
    Q_PROPERTY(int intervalMinutes READ intervalMinutes WRITE setIntervalMinutes)

public:
    // 时间段类型枚举
    enum TimeSlotType {
        Normal,        // 标准时间
        PeakHour,      // 繁忙时间
        OffHours,      // 非工作时间
        Restricted,    // 受限制时间
        Custom         // 自定义类型
    };
    Q_ENUM(TimeSlotType)
    
    struct TimeSlot {
        QTime start;
        QTime end;
        TimeSlotType type;
        QString description;
    };

    EnhancedTimeEdit(QWidget *parent = nullptr) : QTimeEdit(parent),
        m_allowInvalidTime(false), 
        m_useSmartInterval(true),
        m_intervalMinutes(15),
        m_formatCache(nullptr),
        m_validationCache(new QCache<int, bool>(500))
    {
        // 设置基本属性
        setDisplayFormat("HH:mm");
        
        // ⚡性能优化: 预编译正则表达式用于解析时间
        m_timeRegex = QRegularExpression("^(\\d{1,2}):(\\d{1,2})(?::(\\d{1,2}))?(?:\\.(\\d{1,3}))?$");
        
        // 连接信号
        connect(this, &QTimeEdit::timeChanged, this, &EnhancedTimeEdit::onTimeChanged);
        
        // 初始化时间段
        initializeTimeSlots();
        
        // 初始化事件过滤器
        installEventFilter(this);
        
        // 设置上下文菜单
        setContextMenuPolicy(Qt::CustomContextMenu);
        connect(this, &QWidget::customContextMenuRequested,
                this, &EnhancedTimeEdit::showContextMenu);
                
        // 创建格式缓存 (⚡性能优化)
        m_formatCache = new QCache<int, QString>(100);
        
        // 设置默认时间
        setTime(QTime::currentTime());
    }
    
    ~EnhancedTimeEdit() {
        // 清理资源
        delete m_validationCache;
        delete m_formatCache;
    }
    
    // 公共API
    bool allowInvalidTime() const { return m_allowInvalidTime; }
    void setAllowInvalidTime(bool allow) {
        if (m_allowInvalidTime != allow) {
            m_allowInvalidTime = allow;
            // 重新验证
            validateCurrentTime();
            emit allowInvalidTimeChanged(allow);
        }
    }
    
    bool useSmartInterval() const { return m_useSmartInterval; }
    void setUseSmartInterval(bool use) {
        if (m_useSmartInterval != use) {
            m_useSmartInterval = use;
            updateStepSize();
        }
    }
    
    QVector<QTime> restrictedTimes() const { return m_restrictedTimes; }
    void setRestrictedTimes(const QVector<QTime> &times) {
        m_restrictedTimes = times;
        // 清除缓存
        m_validationCache->clear();
        validateCurrentTime();
    }
    
    int intervalMinutes() const { return m_intervalMinutes; }
    void setIntervalMinutes(int minutes) {
        if (minutes > 0 && minutes <= 60 && m_intervalMinutes != minutes) {
            m_intervalMinutes = minutes;
            updateStepSize();
        }
    }
    
    QVector<TimeSlot> timeSlots() const { return m_timeSlots; }
    void setTimeSlots(const QVector<TimeSlot> &slots) {
        m_timeSlots = slots;
        m_validationCache->clear();
        validateCurrentTime();
    }
    
    void addTimeSlot(const QTime &start, const QTime &end, 
                    TimeSlotType type, const QString &description = QString()) {
        TimeSlot slot;
        slot.start = start;
        slot.end = end;
        slot.type = type;
        slot.description = description;
        m_timeSlots.append(slot);
        
        // 更新验证
        m_validationCache->clear();
        validateCurrentTime();
    }
    
    // 智能获取有效时间
    QTime getNextValidTime(const QTime &fromTime) {
        QTime result = fromTime;
        int attempts = 24 * 60; // 防止无限循环
        
        while (attempts-- > 0) {
            if (isTimeValid(result) || m_allowInvalidTime) {
                return result;
            }
            // 尝试下一个间隔时间
            result = result.addSecs(m_intervalMinutes * 60);
        }
        
        // 找不到有效时间，返回最早有效时间
        for (const TimeSlot &slot : m_timeSlots) {
            if (slot.type == Normal || slot.type == PeakHour) {
                return slot.start;
            }
        }
        
        return QTime(9, 0); // 默认返回9:00
    }
    
    TimeSlotType getCurrentTimeSlotType() const {
        QTime current = time();
        for (const TimeSlot &slot : m_timeSlots) {
            if (current >= slot.start && current <= slot.end) {
                return slot.type;
            }
        }
        return Normal;
    }
    
    // 格式化时间 - 面向特殊需求的函数
    QString formatTimeWithContext(const QTime &time) {
        // ⚡性能优化: 缓存格式化结果
        int cacheKey = time.msecsSinceStartOfDay();
        if (m_formatCache->contains(cacheKey)) {
            return *m_formatCache->object(cacheKey);
        }
        
        QString baseFormat = time.toString(displayFormat());
        QString result;
        
        // 查找当前时间段类型
        for (const TimeSlot &slot : m_timeSlots) {
            if (time >= slot.start && time <= slot.end) {
                if (slot.type == PeakHour) {
                    result = QString("%1 ⚡").arg(baseFormat);
                } else if (slot.type == OffHours) {
                    result = QString("%1 🔒").arg(baseFormat);
                } else if (slot.type == Restricted) {
                    result = QString("%1 ⛔").arg(baseFormat);
                } else if (slot.type == Custom && !slot.description.isEmpty()) {
                    result = QString("%1 (%2)").arg(baseFormat, slot.description);
                } else {
                    result = baseFormat;
                }
                break;
            }
        }
        
        if (result.isEmpty()) {
            result = baseFormat;
        }
        
        // 缓存结果
        m_formatCache->insert(cacheKey, new QString(result));
        return result;
    }

protected:
    bool eventFilter(QObject *watched, QEvent *event) override {
        if (watched == this) {
            if (event->type() == QEvent::Wheel) {
                // 鼠标滚轮调整 - 根据当前部分使用不同步长
                QWheelEvent *wheelEvent = static_cast<QWheelEvent*>(event);
                bool handled = false;
                
                // 获取当前部分和对应的步长
                QTimeEdit::Section currentSec = currentSection();
                int stepSize = 1;
                
                if (currentSec == QTimeEdit::HourSection) {
                    stepSize = 1;  // 小时部分步长为1
                } else if (currentSec == QTimeEdit::MinuteSection) {
                    stepSize = m_useSmartInterval ? m_intervalMinutes : 1;
                } else if (currentSec == QTimeEdit::SecondSection) {
                    stepSize = 5;  // 秒部分步长为5
                }
                
                // 计算滚动方向
                int numDegrees = wheelEvent->angleDelta().y() / 8;
                int numSteps = numDegrees / 15;  // 典型的滚轮步长
                
                if (numSteps != 0) {
                    // 执行步进
                    QTime newTime = calculateSteppedTime(time(), currentSec, stepSize * numSteps);
                    setTime(newTime);
                    handled = true;
                }
                
                if (handled) {
                    return true;  // 事件已处理
                }
            } else if (event->type() == QEvent::KeyPress) {
                QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
                
                // 快捷键处理
                if (keyEvent->key() == Qt::Key_Space) {
                    // 空格键重置为当前时间
                    setTime(QTime::currentTime());
                    return true;
                } else if (keyEvent->key() == Qt::Key_N && keyEvent->modifiers() & Qt::ControlModifier) {
                    // Ctrl+N 设置为下一个有效时间
                    setTime(getNextValidTime(time()));
                    return true;
                }
            }
        }
        return QTimeEdit::eventFilter(watched, event);
    }
    
    void keyPressEvent(QKeyEvent *event) override {
        // 增强键盘导航
        if (event->key() == Qt::Key_Up || event->key() == Qt::Key_Down) {
            QTimeEdit::Section currentSec = currentSection();
            int direction = (event->key() == Qt::Key_Up) ? 1 : -1;
            int stepSize = 1;
            
            if (currentSec == QTimeEdit::HourSection) {
                stepSize = 1;
            } else if (currentSec == QTimeEdit::MinuteSection) {
                stepSize = m_useSmartInterval ? m_intervalMinutes : 1;
            } else if (currentSec == QTimeEdit::SecondSection) {
                stepSize = 5;
            }
            
            QTime newTime = calculateSteppedTime(time(), currentSec, stepSize * direction);
            setTime(newTime);
            event->accept();
            return;
        }
        
        QTimeEdit::keyPressEvent(event);
    }
    
    void paintEvent(QPaintEvent *event) override {
        QTimeEdit::paintEvent(event);
        
        // 如果有自定义指示器，绘制它
        if (!m_allowInvalidTime && !isTimeValid(time())) {
            QPainter painter(this);
            painter.setOpacity(0.3);
            painter.fillRect(rect().adjusted(2, 2, -2, -2), QColor(255, 0, 0, 40));
            
            // 绘制指示图标
            QRect iconRect = rect().adjusted(rect().width() - 20, 2, -2, -2);
            painter.setOpacity(1.0);
            QPixmap warningIcon(":/icons/warning.png");  // 确保资源中有这个图标
            if (!warningIcon.isNull()) {
                painter.drawPixmap(iconRect, warningIcon);
            } else {
                // 绘制一个简单的警告标志
                painter.setPen(Qt::red);
                painter.drawText(iconRect, Qt::AlignCenter, "!");
            }
        }
    }
    
    void showEvent(QShowEvent *event) override {
        // 组件显示时更新当前显示
        updateDisplay();
        QTimeEdit::showEvent(event);
    }

private slots:
    void onTimeChanged(const QTime &time) {
        // 验证并更新显示
        bool valid = isTimeValid(time);
        
        // 更新显示样式
        updateDisplay();
        
        // 发出有效性变更信号
        emit timeValidityChanged(valid);
        
        // 发出上下文相关信号
        TimeSlotType slotType = getCurrentTimeSlotType();
        emit timeSlotTypeChanged(slotType);
        
        // 如果启用了间隔检查，确保时间符合间隔
        if (m_useSmartInterval && m_intervalMinutes > 1) {
            int minutes = time.minute();
            if (minutes % m_intervalMinutes != 0) {
                // 找到最近的间隔
                int roundedMinutes = ((minutes + m_intervalMinutes / 2) / m_intervalMinutes) * m_intervalMinutes;
                if (roundedMinutes == 60) {
                    QTime adjusted(time.hour() + 1, 0, time.second(), time.msec());
                    QTimer::singleShot(0, [this, adjusted]() { setTime(adjusted); });
                } else {
                    QTime adjusted(time.hour(), roundedMinutes, time.second(), time.msec());
                    QTimer::singleShot(0, [this, adjusted]() { setTime(adjusted); });
                }
            }
        }
    }
    
    void showContextMenu(const QPoint &pos) {
        QMenu menu(this);
        
        // 添加常用时间选项
        QTime currentTime = QTime::currentTime();
        int currentHour = currentTime.hour();
        
        menu.addAction(tr("Current time (%1)").arg(currentTime.toString("HH:mm")), [this, currentTime]() {
            setTime(currentTime);
        });
        
        // 添加工作日开始/午餐/结束时间
        menu.addSeparator();
        menu.addAction(tr("Start of day (09:00)"), [this]() {
            setTime(QTime(9, 0));
        });
        menu.addAction(tr("Lunch time (12:30)"), [this]() {
            setTime(QTime(12, 30));
        });
        menu.addAction(tr("End of day (17:30)"), [this]() {
            setTime(QTime(17, 30));
        });
        
        // 添加下一个有效时间选项
        menu.addSeparator();
        menu.addAction(tr("Next valid time"), [this]() {
            setTime(getNextValidTime(time().addSecs(60)));
        });
        
        // 添加时间段快速选择菜单
        if (!m_timeSlots.isEmpty()) {
            menu.addSeparator();
            QMenu *slotsMenu = menu.addMenu(tr("Time slots"));
            
            for (const TimeSlot &slot : m_timeSlots) {
                QString actionText = QString("%1 - %2").arg(
                    slot.start.toString("HH:mm"),
                    slot.end.toString("HH:mm")
                );
                
                if (!slot.description.isEmpty()) {
                    actionText += QString(" (%1)").arg(slot.description);
                }
                
                QAction *action = slotsMenu->addAction(actionText, [this, slot]() {
                    setTime(slot.start);
                });
                
                // 设置图标
                if (slot.type == PeakHour) {
                    action->setIcon(QIcon(":/icons/peak.png"));
                } else if (slot.type == OffHours) {
                    action->setIcon(QIcon(":/icons/off.png"));
                } else if (slot.type == Restricted) {
                    action->setIcon(QIcon(":/icons/restricted.png"));
                }
            }
        }
        
        // 显示菜单
        menu.exec(mapToGlobal(pos));
    }

private:
    // 初始化默认时间段
    void initializeTimeSlots() {
        // 工作日时间段
        addTimeSlot(QTime(9, 0), QTime(12, 0), Normal, tr("Morning"));
        addTimeSlot(QTime(12, 0), QTime(13, 0), PeakHour, tr("Lunch"));
        addTimeSlot(QTime(13, 0), QTime(17, 0), Normal, tr("Afternoon"));
        
        // 非工作时间
        addTimeSlot(QTime(0, 0), QTime(9, 0), OffHours, tr("Before hours"));
        addTimeSlot(QTime(17, 0), QTime(23, 59, 59, 999), OffHours, tr("After hours"));
        
        // 特殊限制时间 (例如维护时间)
        m_restrictedTimes.append(QTime(10, 0));
        m_restrictedTimes.append(QTime(15, 0));
    }
    
    void updateStepSize() {
        if (m_useSmartInterval && currentSection() == QTimeEdit::MinuteSection) {
            setSingleStep(m_intervalMinutes);
        } else {
            setSingleStep(1);
        }
    }
    
    bool isTimeValid(const QTime &time) const {
        if (!time.isValid()) {
            return false;
        }
        
        // ⚡性能优化: 使用缓存
        int cacheKey = time.msecsSinceStartOfDay();
        if (m_validationCache->contains(cacheKey)) {
            return *m_validationCache->object(cacheKey);
        }
        
        // 检查是否在限制时间列表中
        if (m_restrictedTimes.contains(time)) {
            m_validationCache->insert(cacheKey, new bool(false));
            return false;
        }
        
        // 检查时间段
        for (const TimeSlot &slot : m_timeSlots) {
            if (time >= slot.start && time <= slot.end) {
                if (slot.type == Normal || slot.type == PeakHour) {
                    m_validationCache->insert(cacheKey, new bool(true));
                    return true;
                } else if (slot.type == Restricted || slot.type == OffHours) {
                    m_validationCache->insert(cacheKey, new bool(false));
                    return false;
                }
            }
        }
        
        // 默认有效
        m_validationCache->insert(cacheKey, new bool(true));
        return true;
    }
    
    void validateCurrentTime() {
        // 验证当前时间
        QTime current = time();
        bool valid = isTimeValid(current);
        
        if (!valid && !m_allowInvalidTime
```