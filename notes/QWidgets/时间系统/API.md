# QDate API参考手册（Qt 5.12.12）

1️⃣【构造体系】 ◆ 核心构造函数

- QDate() ▸ 典型用途：创建无效日期实例 ▸ 内存策略：栈对象，值类型，无需手动内存管理
- QDate(int year, int month, int day) ▸ 典型用途：使用年、月、日创建特定日期 ▸ 示例：QDate date(2023, 12, 31)

◆ 工厂方法

- static QDate currentDate() → 返回系统当前日期
- static QDate fromString(const QString &string, Qt::DateFormat format = Qt::TextDate) → 从字符串解析
- static QDate fromString(const QString &string, const QString &format) → 使用自定义格式解析
- static QDate fromJulianDay(qint64 jd) → 从儒略日创建

2️⃣【方法矩阵】 ▨ 核心方法

| 方法签名                                                     | 功能描述                   | 使用频率 |
| ------------------------------------------------------------ | -------------------------- | -------- |
| bool isValid() const                                         | 检查日期是否有效           | ★★★★★    |
| int year() const                                             | 获取年份                   | ★★★★★    |
| int month() const                                            | 获取月份                   | ★★★★★    |
| int day() const                                              | 获取日                     | ★★★★★    |
| int dayOfWeek() const                                        | 获取星期几(1=周一到7=周日) | ★★★★     |
| int dayOfYear() const                                        | 获取一年中的第几天         | ★★★      |
| int daysInMonth() const                                      | 获取当月天数               | ★★★      |
| int daysInYear() const                                       | 获取当年天数               | ★★       |
| QString toString(const QString &format) const                | 格式化为字符串             | ★★★★     |
| QString toString(Qt::DateFormat format = Qt::TextDate) const | 使用预定义格式转为字符串   | ★★★★     |
| QDate addDays(qint64 days) const                             | 添加天数返回新日期         | ★★★★     |
| QDate addMonths(int months) const                            | 添加月数返回新日期         | ★★★★     |
| QDate addYears(int years) const                              | 添加年数返回新日期         | ★★★★     |
| qint64 daysTo(const QDate &date) const                       | 计算到指定日期的天数差     | ★★★★     |
| bool operator==(const QDate &other) const                    | 日期相等比较               | ★★★      |
| bool operator<(const QDate &other) const                     | 日期大小比较               | ★★★      |

▨ 重写方法

- 无需重写方法，QDate是值类型

3️⃣【信号与槽】 ◇ 输出信号

- 无信号，QDate是值类型而非QObject派生类

◇ 输入槽

- 无槽，QDate是值类型而非QObject派生类

4️⃣【枚举属性】 ◇ 相关属性

- 无QProperty属性，QDate是值类型而非QObject派生类

◇ 相关枚举

- **MonthNameType**

- 作用：指定月份名称格式
- 涉及函数：monthName(), shortMonthName()

| 成员                    | 值   | 说明                   |
| ----------------------- | ---- | ---------------------- |
| QDate::DateFormat       | 0    | 使用长格式月份名称     |
| QDate::StandaloneFormat | 1    | 使用独立格式的月份名称 |

5️⃣【版本适配】 △ Qt 5.12特性

- 新增：weekNumber()方法添加了可选的yearNumber参数，可同时获取周数和对应年份

△ 跨平台差异

- Windows/Linux/macOS: 日期格式化受系统区域设置影响
- 部分平台的currentDate()可能存在微秒级别的差异

6️⃣【代码沙箱】 ◇ 标准用法：

```cpp
// 创建当前日期
QDate today = QDate::currentDate();

// 创建特定日期
QDate birthday(1990, 5, 17);

// 日期运算
QDate nextWeek = today.addDays(7);
int daysUntilBirthday = today.daysTo(birthday);

// 日期格式化
QString formatted = today.toString("yyyy-MM-dd");
QString localized = today.toString(Qt::LocaleDate);

// 日期信息查询
if (today.isValid() && QDate::isLeapYear(today.year())) {
    int dayOfWeek = today.dayOfWeek();
    QString weekDay = today.toString("dddd");
}
```

◇ 危险操作：

```cpp
// 错误：使用无效日期参数
QDate invalidDate(2023, 13, 45); // 超出有效范围
if (!invalidDate.isValid()) {
    // 应始终检查日期有效性
}

// 错误：假设所有月份都有31天
QDate lastDay(2023, 2, 31); // 2月没有31天
// 正确：使用有效性检查或使用辅助函数
QDate lastDayOfMonth = QDate(2023, 2, 1).addMonths(1).addDays(-1);
```





# QTime API参考手册（Qt 5.12.12）

1️⃣【构造体系】 ◆ 核心构造函数

- QTime() ▸ 典型用途：创建无效时间实例 ▸ 内存策略：栈对象，值类型，无需手动内存管理
- QTime(int hour, int minute, int second = 0, int msec = 0) ▸ 典型用途：使用时、分、秒、毫秒创建特定时间 ▸ 示例：QTime time(14, 30, 15, 500)  // 14:30:15.500

◆ 工厂方法

- static QTime currentTime() → 获取系统当前时间
- static QTime fromString(const QString &string, Qt::DateFormat format = Qt::TextDate) → 从字符串解析
- static QTime fromString(const QString &string, const QString &format) → 使用自定义格式解析
- static QTime fromMSecsSinceStartOfDay(int msecs) → 从一天开始的毫秒数创建

2️⃣【方法矩阵】 ▨ 核心方法

| 方法签名                                                     | 功能描述                  | 使用频率 |
| ------------------------------------------------------------ | ------------------------- | -------- |
| bool isValid() const                                         | 检查时间是否有效          | ★★★★★    |
| int hour() const                                             | 获取小时部分              | ★★★★★    |
| int minute() const                                           | 获取分钟部分              | ★★★★★    |
| int second() const                                           | 获取秒部分                | ★★★★★    |
| int msec() const                                             | 获取毫秒部分              | ★★★★     |
| QString toString(const QString &format) const                | 格式化为字符串            | ★★★★     |
| QString toString(Qt::DateFormat format = Qt::TextDate) const | 使用预定义格式转为字符串  | ★★★★     |
| QTime addSecs(int seconds) const                             | 添加秒数返回新时间        | ★★★★     |
| QTime addMSecs(int ms) const                                 | 添加毫秒数返回新时间      | ★★★★     |
| int secsTo(const QTime &time) const                          | 计算到指定时间的秒数差    | ★★★★     |
| int msecsTo(const QTime &time) const                         | 计算到指定时间的毫秒数差  | ★★★★     |
| bool operator==(const QTime &other) const                    | 时间相等比较              | ★★★      |
| bool operator<(const QTime &other) const                     | 时间大小比较              | ★★★      |
| int elapsed() const                                          | 从指定时间点经过的毫秒数  | ★★★★     |
| void start()                                                 | 启动计时器(用于elapsed()) | ★★★      |
| void restart()                                               | 重启计时器(用于elapsed()) | ★★★      |
| int msecsSinceStartOfDay() const                             | 返回从一天开始的毫秒数    | ★★★      |

▨ 重写方法

- 无需重写方法，QTime是值类型

3️⃣【信号与槽】 ◇ 输出信号

- 无信号，QTime是值类型而非QObject派生类

◇ 输入槽

- 无槽，QTime是值类型而非QObject派生类

4️⃣【枚举属性】 ◇ 相关属性

- 无QProperty属性，QTime是值类型而非QObject派生类

◇ 相关枚举

- 不直接定义枚举，但使用Qt::DateFormat枚举用于格式化

5️⃣【版本适配】 △ Qt 5.12特性

- 无5.12特定新增功能，核心功能在较早版本已稳定

△ 跨平台差异

- Windows/Linux/macOS: 时间格式化受系统区域设置影响
- currentTime()在不同平台可能有微妙的精度差异

6️⃣【代码沙箱】 ◇ 标准用法：

```cpp
// 获取当前时间
QTime now = QTime::currentTime();

// 创建特定时间
QTime meetingTime(9, 30);  // 09:30:00.000

// 时间运算
QTime endTime = meetingTime.addSecs(3600);  // 添加1小时
int minutesPassed = meetingTime.secsTo(endTime) / 60;  // 计算分钟差

// 时间格式化
QString formatted = now.toString("hh:mm:ss.zzz");
QString localized = now.toString(Qt::DefaultLocaleShortDate);

// 计时功能
QTime timer;
timer.start();  // 开始计时
// ... 执行某些操作
int elapsed = timer.elapsed();  // 获取经过的毫秒数
```

◇ 危险操作：

```cpp
// 错误：使用无效时间参数
QTime invalidTime(25, 70, 90);  // 超出有效范围
if (!invalidTime.isValid()) {
    // 应始终检查时间有效性
}

// 错误：跨日期的时间差计算
QTime t1(22, 0);
QTime t2(2, 0);
int hours = t1.secsTo(t2) / 3600;  // 结果为-20小时而非+4小时
// 正确：需要考虑日期，使用QDateTime进行跨日期计算
```







# QDateTime API参考手册（Qt 5.12.12）

1️⃣【构造体系】 ◆ 核心构造函数

- QDateTime() ▸ 典型用途：创建无效的日期时间实例 ▸ 内存策略：栈对象，值类型，无需手动内存管理
- QDateTime(const QDate &date, const QTime &time, Qt::TimeSpec spec = Qt::LocalTime) ▸ 典型用途：从单独的QDate和QTime对象创建日期时间 ▸ 示例：QDateTime dt(QDate(2023, 12, 31), QTime(23, 59, 59))
- QDateTime(const QDate &date, const QTime &time, const QTimeZone &timeZone) ▸ 典型用途：创建指定时区的日期时间 ▸ 示例：QDateTime dt(QDate(2023, 12, 31), QTime(23, 59, 59), QTimeZone("Europe/Berlin"))

◆ 工厂方法

- static QDateTime currentDateTime() → 获取系统当前日期时间
- static QDateTime currentDateTimeUtc() → 获取UTC当前日期时间
- static QDateTime fromString(const QString &string, Qt::DateFormat format = Qt::TextDate) → 从字符串解析
- static QDateTime fromString(const QString &string, const QString &format) → 使用自定义格式解析
- static QDateTime fromSecsSinceEpoch(qint64 secs) → 从UNIX时间戳创建
- static QDateTime fromMSecsSinceEpoch(qint64 msecs) → 从毫秒时间戳创建

2️⃣【方法矩阵】 ▨ 核心方法

| 方法签名                                                     | 功能描述                         | 使用频率 |
| ------------------------------------------------------------ | -------------------------------- | -------- |
| bool isValid() const                                         | 检查日期时间是否有效             | ★★★★★    |
| QDate date() const                                           | 获取日期部分                     | ★★★★★    |
| QTime time() const                                           | 获取时间部分                     | ★★★★★    |
| Qt::TimeSpec timeSpec() const                                | 获取时间规范(本地/UTC/偏移/时区) | ★★★★     |
| QTimeZone timeZone() const                                   | 获取时区信息                     | ★★★★     |
| qint64 toSecsSinceEpoch() const                              | 转换为UNIX时间戳(秒)             | ★★★★★    |
| qint64 toMSecsSinceEpoch() const                             | 转换为毫秒时间戳                 | ★★★★★    |
| QString toString(const QString &format) const                | 使用自定义格式转为字符串         | ★★★★★    |
| QString toString(Qt::DateFormat format = Qt::TextDate) const | 使用预定义格式转为字符串         | ★★★★     |
| QDateTime addDays(qint64 days) const                         | 添加天数返回新日期时间           | ★★★★     |
| QDateTime addMonths(int months) const                        | 添加月数返回新日期时间           | ★★★★     |
| QDateTime addYears(int years) const                          | 添加年数返回新日期时间           | ★★★★     |
| QDateTime addSecs(qint64 secs) const                         | 添加秒数返回新日期时间           | ★★★★     |
| QDateTime addMSecs(qint64 msecs) const                       | 添加毫秒数返回新日期时间         | ★★★★     |
| qint64 secsTo(const QDateTime &dt) const                     | 计算到指定日期时间的秒数差       | ★★★★     |
| qint64 msecsTo(const QDateTime &dt) const                    | 计算到指定日期时间的毫秒数差     | ★★★★     |
| QDateTime toLocalTime() const                                | 转换为本地时间                   | ★★★★     |
| QDateTime toUTC() const                                      | 转换为UTC时间                    | ★★★★     |
| QDateTime toTimeZone(const QTimeZone &tz) const              | 转换为指定时区                   | ★★★★     |
| bool isDaylightTime() const                                  | 检查是否处于夏令时               | ★★★      |
| qint64 offsetFromUtc() const                                 | 获取与UTC的秒数偏移              | ★★★      |
| bool operator==(const QDateTime &other) const                | 日期时间相等比较                 | ★★★★     |
| bool operator<(const QDateTime &other) const                 | 日期时间大小比较                 | ★★★★     |

▨ 重写方法

- 无需重写方法，QDateTime是值类型

3️⃣【信号与槽】 ◇ 输出信号

- 无信号，QDateTime是值类型而非QObject派生类

◇ 输入槽

- 无槽，QDateTime是值类型而非QObject派生类

4️⃣【枚举属性】 ◇ 相关属性

- 无QProperty属性，QDateTime是值类型而非QObject派生类

◇ 相关枚举

- **Qt::TimeSpec**

- 作用：指定日期时间的时间规范
- 涉及函数：timeSpec(), setTimeSpec(), toTimeSpec()

| 成员              | 值   | 说明                    |
| ----------------- | ---- | ----------------------- |
| Qt::LocalTime     | 0    | 本地时间，依赖系统时区  |
| Qt::UTC           | 1    | 协调世界时(UTC)         |
| Qt::OffsetFromUTC | 2    | 基于UTC的固定偏移量     |
| Qt::TimeZone      | 3    | 使用QTimeZone指定的时区 |

5️⃣【版本适配】 △ Qt 5.12特性

- 新增：fromSecsSinceEpoch()和fromMSecsSinceEpoch()添加了可选的Qt::TimeSpec参数
- 新增：QDateTime构造函数增加对QTimeZone的直接支持

△ 跨平台差异

- Windows/Linux/macOS: 日期时间格式化受系统区域设置影响
- 时区处理在不同操作系统上可能有细微差异
- 夏令时调整规则在不同区域可能有显著不同

6️⃣【代码沙箱】 ◇ 标准用法：

```cpp
// 获取当前日期时间
QDateTime now = QDateTime::currentDateTime();

// 创建特定日期时间
QDateTime meeting(QDate(2023, 5, 15), QTime(14, 30));

// 转换时区
QDateTime utcTime = now.toUTC();
QDateTime berlinTime = now.toTimeZone(QTimeZone("Europe/Berlin"));

// 日期时间运算
QDateTime nextWeek = now.addDays(7);
QDateTime twoHoursLater = now.addSecs(7200);

// 计算时间差
qint64 secondsUntilMeeting = now.secsTo(meeting);
qint64 daysUntilMeeting = secondsUntilMeeting / 86400;

// 格式化
QString isoFormat = now.toString(Qt::ISODate);  // 2023-05-08T14:30:00
QString customFormat = now.toString("yyyy-MM-dd hh:mm:ss");

// 转换为时间戳和从时间戳转换
qint64 unixTime = now.toSecsSinceEpoch();
QDateTime fromUnix = QDateTime::fromSecsSinceEpoch(unixTime);
```

◇ 危险操作：

```cpp
// 错误：混合使用不同时区的日期时间进行比较
QDateTime localTime = QDateTime::currentDateTime();
QDateTime utcTime = QDateTime::currentDateTimeUtc();
if (localTime < utcTime) {  // 不正确的比较，时区不同
    // ...
}
// 正确：确保在相同时区下比较
if (localTime.toUTC() < utcTime) {
    // ...
}

// 错误：假设时间戳总是UTC
QDateTime dt = QDateTime::fromSecsSinceEpoch(timestamp);  // 默认转为本地时间
// 正确：明确指定时区
QDateTime dt = QDateTime::fromSecsSinceEpoch(timestamp, Qt::UTC);
```





# QTimer API参考手册（Qt 5.12.12）

1️⃣【构造体系】 ◆ 核心构造函数

- QTimer(QObject *parent = nullptr) ▸ 典型用途：创建定时器用于定时触发信号或执行操作 ▸ 内存策略：父子对象自动回收

◆ 工厂方法

- static void singleShot(int msec, const QObject *receiver, const char *member) → 创建一次性定时器（旧式连接）
- static void singleShot(int msec, Qt::TimerType timerType, const QObject *receiver, const char *member) → 带计时器类型的一次性定时器（旧式连接）
- static void singleShot(int msec, const QObject *context, std::function<void()> functor) → 创建一次性定时器（新式函数对象连接）
- static void singleShot(int msec, Qt::TimerType timerType, const QObject *context, std::function<void()> functor) → 带计时器类型的一次性定时器（新式函数对象连接）

2️⃣【方法矩阵】 ▨ 核心方法

| 方法签名                                   | 功能描述                   | 使用频率 |
| ------------------------------------------ | -------------------------- | -------- |
| void start()                               | 启动定时器使用当前间隔     | ★★★★★    |
| void start(int msec)                       | 设置间隔并启动定时器       | ★★★★★    |
| void stop()                                | 停止定时器                 | ★★★★★    |
| bool isActive() const                      | 检查定时器是否在运行       | ★★★★     |
| int interval() const                       | 获取定时器间隔(毫秒)       | ★★★★     |
| void setInterval(int msec)                 | 设置定时器间隔(毫秒)       | ★★★★★    |
| int remainingTime() const                  | 获取距离下次触发的剩余时间 | ★★★      |
| bool isSingleShot() const                  | 检查是否为单次触发模式     | ★★★      |
| void setSingleShot(bool singleShot)        | 设置单次触发模式           | ★★★★     |
| Qt::TimerType timerType() const            | 获取定时器类型             | ★★       |
| void setTimerType(Qt::TimerType timerType) | 设置定时器类型             | ★★       |

▨ 重写方法

| 方法                          | 触发场景       | 必须调用基类 |
| ----------------------------- | -------------- | ------------ |
| void timerEvent(QTimerEvent*) | 定时器内部事件 | 否           |

3️⃣【信号与槽】 ◇ 输出信号

- timeout() → 当定时器到期时发射

◇ 输入槽

- start() → 启动定时器
- start(int msec) → 使用指定间隔启动定时器
- stop() → 停止定时器

4️⃣【枚举属性】 ◇ 相关属性

| 属性名     | 类型          | 默认值          | 描述                   |
| ---------- | ------------- | --------------- | ---------------------- |
| interval   | int           | 0               | 定时器间隔（毫秒）     |
| active     | bool          | false           | 定时器是否处于活动状态 |
| singleShot | bool          | false           | 是否为单次触发模式     |
| timerType  | Qt::TimerType | Qt::CoarseTimer | 定时器精度类型         |

◇ 相关枚举

- **Qt::TimerType**

- 作用：控制定时器精度和性能权衡
- 涉及函数：timerType(), setTimerType()

| 成员                | 值   | 说明                                          |
| ------------------- | ---- | --------------------------------------------- |
| Qt::PreciseTimer    | 0    | 精确计时，尽量保持毫秒级准确性，但CPU占用较高 |
| Qt::CoarseTimer     | 1    | 粗略计时，精度约为5%，平衡CPU占用和精度       |
| Qt::VeryCoarseTimer | 2    | 非常粗略计时，精度仅精确到秒级，CPU占用最低   |

5️⃣【版本适配】 △ Qt 5.12特性

- 增强：singleShot()静态方法增加了对Lambda表达式和std::function的支持
- 改进：跨平台计时器精度提升

△ 跨平台差异

- Windows：使用媒体计时器保证定时器精度
- Linux：精确度受系统负载和内核配置影响
- macOS：高精度定时器实现较为一致

6️⃣【代码沙箱】 ◇ 标准用法：

```cpp
// 基本定时器使用
QTimer *timer = new QTimer(this);
connect(timer, &QTimer::timeout, this, &MyWidget::updateDisplay);
timer->setInterval(1000);  // 1秒
timer->start();

// 单次触发定时器
QTimer *singleTimer = new QTimer(this);
singleTimer->setSingleShot(true);
connect(singleTimer, &QTimer::timeout, this, &MyWidget::onTimeout);
singleTimer->start(5000);  // 5秒后触发一次

// 静态单次触发（新式连接）
QTimer::singleShot(2000, this, [this]() {
    statusBar()->showMessage("操作已完成", 3000);
});

// 周期性任务的启停
QTimer *pollTimer = new QTimer(this);
pollTimer->setInterval(500);
connect(pollTimer, &QTimer::timeout, this, &MyWidget::pollData);

void MyWidget::startPolling() {
    pollTimer->start();
}

void MyWidget::stopPolling() {
    pollTimer->stop();
}
```

◇ 危险操作：

```cpp
// 错误：在槽函数中删除发送信号的定时器
connect(timer, &QTimer::timeout, this, [timer]() {
    delete timer;  // 危险：可能导致崩溃
});
// 正确：使用deleteLater()安全删除
connect(timer, &QTimer::timeout, timer, &QTimer::deleteLater);

// 错误：过短的定时器间隔
timer->setInterval(1);  // 1毫秒，可能导致CPU占用过高
// 正确：根据需求选择合理的间隔
timer->setInterval(50);  // 最小建议值，大约20帧/秒

// 错误：线程间不安全操作
// 从非GUI线程调用
QThread *workerThread = new WorkerThread(this);
connect(workerThread, &WorkerThread::started, [this]() {
    timer->start();  // 错误：QTimer必须在创建它的线程中操作
});
// 正确：为工作线程创建独立定时器
QTimer *workerTimer = new QTimer(nullptr);
workerTimer->moveToThread(workerThread);
connect(workerThread, &WorkerThread::started, workerTimer, 
        QOverload<>::of(&QTimer::start));
```







# QDateTimeEdit API参考手册（Qt 5.12.12）

1️⃣【构造体系】 ◆ 核心构造函数

- QDateTimeEdit(QWidget *parent = nullptr) ▸ 典型用途：创建显示当前日期时间的编辑器 ▸ 内存策略：父子对象自动回收
- QDateTimeEdit(const QDateTime &datetime, QWidget *parent = nullptr) ▸ 典型用途：创建显示指定日期时间的编辑器 ▸ 示例：QDateTimeEdit *edit = new QDateTimeEdit(QDateTime::currentDateTime(), this)
- QDateTimeEdit(const QDate &date, QWidget *parent = nullptr) ▸ 典型用途：创建仅显示日期的编辑器 ▸ 内存策略：父子对象自动回收
- QDateTimeEdit(const QTime &time, QWidget *parent = nullptr) ▸ 典型用途：创建仅显示时间的编辑器 ▸ 内存策略：父子对象自动回收

◆ 子类特化

- QDateEdit → 专用于日期编辑的简化版
- QTimeEdit → 专用于时间编辑的简化版

2️⃣【方法矩阵】 ▨ 核心方法

| 方法签名                                                     | 功能描述                 | 使用频率 |
| ------------------------------------------------------------ | ------------------------ | -------- |
| QDateTime dateTime() const                                   | 获取当前日期时间值       | ★★★★★    |
| void setDateTime(const QDateTime &datetime)                  | 设置日期时间值           | ★★★★★    |
| QDate date() const                                           | 获取当前日期值           | ★★★★     |
| void setDate(const QDate &date)                              | 设置日期值               | ★★★★     |
| QTime time() const                                           | 获取当前时间值           | ★★★★     |
| void setTime(const QTime &time)                              | 设置时间值               | ★★★★     |
| void setDisplayFormat(const QString &format)                 | 设置显示格式             | ★★★★★    |
| QString displayFormat() const                                | 获取显示格式             | ★★★      |
| void setCalendarWidget(QCalendarWidget *)                    | 设置日期选择日历控件     | ★★★      |
| QCalendarWidget *calendarWidget() const                      | 获取日历控件             | ★★       |
| void setMinimumDateTime(const QDateTime &)                   | 设置最小可选日期时间     | ★★★★     |
| QDateTime minimumDateTime() const                            | 获取最小可选日期时间     | ★★★      |
| void setMaximumDateTime(const QDateTime &)                   | 设置最大可选日期时间     | ★★★★     |
| QDateTime maximumDateTime() const                            | 获取最大可选日期时间     | ★★★      |
| void setDateTimeRange(const QDateTime &min, const QDateTime &max) | 同时设置最小最大范围     | ★★★★     |
| void setTimeSpec(Qt::TimeSpec spec)                          | 设置时间规范             | ★★       |
| Qt::TimeSpec timeSpec() const                                | 获取时间规范             | ★★       |
| QDateTimeEdit::Section currentSection() const                | 获取当前编辑的部分       | ★★★      |
| void setCurrentSection(Section section)                      | 设置当前编辑部分         | ★★★      |
| void setCalendarPopup(bool enable)                           | 启用/禁用日历弹出窗口    | ★★★★     |
| bool calendarPopup() const                                   | 检查日历弹出窗口是否启用 | ★★       |

▨ 重写方法

| 方法                                | 触发场景       | 必须调用基类 |
| ----------------------------------- | -------------- | ------------ |
| void paintEvent(QPaintEvent *)      | 控件绘制时     | 是           |
| void keyPressEvent(QKeyEvent *)     | 键盘输入时     | 视情况而定   |
| void mousePressEvent(QMouseEvent *) | 鼠标点击时     | 视情况而定   |
| void wheelEvent(QWheelEvent *)      | 鼠标滚轮事件时 | 视情况而定   |
| void focusInEvent(QFocusEvent *)    | 获得焦点时     | 是           |
| QSize sizeHint() const              | 推荐控件尺寸   | 否           |

3️⃣【信号与槽】 ◇ 输出信号

- dateTimeChanged(const QDateTime &datetime) → 日期时间值改变时发射
- dateChanged(const QDate &date) → 日期值改变时发射
- timeChanged(const QTime &time) → 时间值改变时发射

◇ 输入槽

- setDateTime(const QDateTime &datetime) → 设置日期时间值
- setDate(const QDate &date) → 设置日期值
- setTime(const QTime &time) → 设置时间值
- stepBy(int steps) → 按给定步数增加/减少当前段的值
- clear() → 清除编辑器内容

4️⃣【枚举属性】 ◇ 相关属性

| 属性名            | 类型         | 默认值                  | 描述                 |
| ----------------- | ------------ | ----------------------- | -------------------- |
| dateTime          | QDateTime    | 当前日期时间            | 显示的日期时间       |
| date              | QDate        | 当前日期                | 显示的日期部分       |
| time              | QTime        | 当前时间                | 显示的时间部分       |
| maximumDateTime   | QDateTime    | 9999-12-31T23:59:59.999 | 最大可选日期时间     |
| minimumDateTime   | QDateTime    | 1752-09-14T00:00:00.000 | 最小可选日期时间     |
| displayFormat     | QString      | "yyyy-MM-dd hh:mm:ss"   | 显示格式字符串       |
| calendarPopup     | bool         | false                   | 是否启用日历弹出窗口 |
| currentSection    | Section      | NoSection               | 当前编辑的部分       |
| displayedSections | Sections     | 所有部分                | 显示哪些部分         |
| timeSpec          | Qt::TimeSpec | Qt::LocalTime           | 时间规范             |

◇ 相关枚举

- **QDateTimeEdit::Section**

- 作用：指定日期时间的不同部分
- 涉及函数：currentSection(), setCurrentSection(), sectionAt()

| 成员                         | 值     | 说明            |
| ---------------------------- | ------ | --------------- |
| QDateTimeEdit::NoSection     | 0x0000 | 无部分          |
| QDateTimeEdit::AmPmSection   | 0x0001 | 上午/下午指示器 |
| QDateTimeEdit::MSecSection   | 0x0002 | 毫秒部分        |
| QDateTimeEdit::SecondSection | 0x0004 | 秒部分          |
| QDateTimeEdit::MinuteSection | 0x0008 | 分钟部分        |
| QDateTimeEdit::HourSection   | 0x0010 | 小时部分        |
| QDateTimeEdit::DaySection    | 0x0100 | 天部分          |
| QDateTimeEdit::MonthSection  | 0x0200 | 月份部分        |
| QDateTimeEdit::YearSection   | 0x0400 | 年份部分        |

5️⃣【版本适配】 △ Qt 5.12特性

- 改进：更好地支持高DPI显示
- 改进：国际化支持增强，更好的本地化处理

△ 跨平台差异

- Windows：尊重系统日期时间格式设置
- macOS：遵循HIG设计规范，风格略有不同
- Linux：依赖于桌面环境，KDE和GNOME风格不同

6️⃣【代码沙箱】 ◇ 标准用法：

```cpp
// 创建基本日期时间编辑器
QDateTimeEdit *dateTimeEdit = new QDateTimeEdit(QDateTime::currentDateTime(), this);
dateTimeEdit->setDisplayFormat("yyyy-MM-dd HH:mm:ss");
dateTimeEdit->setCalendarPopup(true);  // 启用日历弹出窗口
dateTimeEdit->setMinimumDateTime(QDateTime::currentDateTime().addDays(-365));  // 最小一年前
dateTimeEdit->setMaximumDateTime(QDateTime::currentDateTime().addYears(1));  // 最大一年后

// 连接信号
connect(dateTimeEdit, &QDateTimeEdit::dateTimeChanged, 
        this, &MyWidget::onDateTimeChanged);

// 日期编辑器
QDateEdit *dateEdit = new QDateEdit(QDate::currentDate(), this);
dateEdit->setDisplayFormat("yyyy年MM月dd日");
dateEdit->setCalendarPopup(true);

// 时间编辑器
QTimeEdit *timeEdit = new QTimeEdit(QTime::currentTime(), this);
timeEdit->setDisplayFormat("HH:mm:ss");

// 自定义日期格式
QDateTimeEdit *customFormat = new QDateTimeEdit(this);
customFormat->setDisplayFormat("yyyy年MM月dd日 ddd HH时mm分");  // 2023年04月15日 周六 14时30分
```

◇ 危险操作：

```cpp
// 错误：设置无效的日期格式
dateTimeEdit->setDisplayFormat("错误格式");  // 将使用默认格式
// 正确：使用有效的日期格式字符串
dateTimeEdit->setDisplayFormat("yyyy-MM-dd HH:mm");

// 错误：设置范围时最小值大于最大值
dateTimeEdit->setMinimumDateTime(QDateTime(QDate(2023, 12, 31), QTime(0, 0)));
dateTimeEdit->setMaximumDateTime(QDateTime(QDate(2023, 1, 1), QTime(0, 0)));
// 正确：确保范围有效
dateTimeEdit->setDateTimeRange(
    QDateTime(QDate(2023, 1, 1), QTime(0, 0)),
    QDateTime(QDate(2023, 12, 31), QTime(23, 59, 59))
);

// 错误：尝试直接修改只读模式下的值
dateTimeEdit->setReadOnly(true);
dateTimeEdit->lineEdit()->setText("2023-04-15 14:30");  // 不会生效且可能导致显示不一致
// 正确：即使在只读模式下也可以通过API修改值
dateTimeEdit->setDateTime(QDateTime(QDate(2023, 4, 15), QTime(14, 30)));
```





# QDateEdit API参考手册（Qt 5.12.12）

1️⃣【构造体系】 ◆ 核心构造函数

- QDateEdit(QWidget *parent = nullptr) ▸ 典型用途：创建显示当前日期的编辑器 ▸ 内存策略：父子对象自动回收
- QDateEdit(const QDate &date, QWidget *parent = nullptr) ▸ 典型用途：创建显示指定日期的编辑器 ▸ 示例：QDateEdit *dateEdit = new QDateEdit(QDate(2023, 5, 15), this)

◆ 继承关系

- 继承自QDateTimeEdit，专注于日期编辑功能
- 自动隐藏时间相关显示部分

2️⃣【方法矩阵】 ▨ 核心方法

| 方法签名                                               | 功能描述                 | 使用频率 |
| ------------------------------------------------------ | ------------------------ | -------- |
| QDate date() const                                     | 获取当前日期值           | ★★★★★    |
| void setDate(const QDate &date)                        | 设置日期值               | ★★★★★    |
| void setDisplayFormat(const QString &format)           | 设置显示格式             | ★★★★★    |
| QString displayFormat() const                          | 获取显示格式             | ★★★      |
| void setCalendarWidget(QCalendarWidget *)              | 设置日期选择日历控件     | ★★★      |
| QCalendarWidget *calendarWidget() const                | 获取日历控件             | ★★       |
| void setMinimumDate(const QDate &min)                  | 设置最小可选日期         | ★★★★     |
| QDate minimumDate() const                              | 获取最小可选日期         | ★★★      |
| void setMaximumDate(const QDate &max)                  | 设置最大可选日期         | ★★★★     |
| QDate maximumDate() const                              | 获取最大可选日期         | ★★★      |
| void setDateRange(const QDate &min, const QDate &max)  | 同时设置最小最大范围     | ★★★★     |
| void setCalendarPopup(bool enable)                     | 启用/禁用日历弹出窗口    | ★★★★     |
| bool calendarPopup() const                             | 检查日历弹出窗口是否启用 | ★★       |
| QDateTimeEdit::Section currentSection() const          | 获取当前编辑的部分       | ★★★      |
| void setCurrentSection(QDateTimeEdit::Section section) | 设置当前编辑部分         | ★★★      |

▨ 继承自QDateTimeEdit的方法

| 方法                                        | 功能描述             | 使用频率 |
| ------------------------------------------- | -------------------- | -------- |
| void stepBy(int steps)                      | 按步数增减当前段的值 | ★★★      |
| QSize sizeHint() const                      | 获取建议的控件大小   | ★★       |
| void clear()                                | 清除编辑器内容       | ★★★      |
| QDateTimeEdit::Section sectionAt(int index) | 获取指定位置的部分   | ★★       |
| int sectionCount() const                    | 获取部分总数         | ★★       |

▨ 重写方法

| 方法                                | 触发场景       | 必须调用基类 |
| ----------------------------------- | -------------- | ------------ |
| void paintEvent(QPaintEvent *)      | 控件绘制时     | 是           |
| void keyPressEvent(QKeyEvent *)     | 键盘输入时     | 视情况而定   |
| void mousePressEvent(QMouseEvent *) | 鼠标点击时     | 视情况而定   |
| void wheelEvent(QWheelEvent *)      | 鼠标滚轮事件时 | 视情况而定   |
| void focusInEvent(QFocusEvent *)    | 获得焦点时     | 是           |

3️⃣【信号与槽】 ◇ 输出信号

- dateChanged(const QDate &date) → 日期值改变时发射
- userDateChanged(const QDate &date) → 用户修改日期时发射（Qt 5.12中不存在，5.14+添加）

◇ 输入槽

- setDate(const QDate &date) → 设置日期值
- stepBy(int steps) → 按给定步数增加/减少当前段的值
- clear() → 清除编辑器内容

4️⃣【枚举属性】 ◇ 相关属性

| 属性名            | 类型                    | 默认值       | 描述                 |
| ----------------- | ----------------------- | ------------ | -------------------- |
| date              | QDate                   | 当前日期     | 显示的日期           |
| maximumDate       | QDate                   | 9999-12-31   | 最大可选日期         |
| minimumDate       | QDate                   | 1752-09-14   | 最小可选日期         |
| displayFormat     | QString                 | "yyyy-MM-dd" | 显示格式字符串       |
| calendarPopup     | bool                    | false        | 是否启用日历弹出窗口 |
| currentSection    | QDateTimeEdit::Section  | NoSection    | 当前编辑的部分       |
| displayedSections | QDateTimeEdit::Sections | 日期相关部分 | 显示哪些部分         |

◇ 相关枚举（继承自QDateTimeEdit）

- **QDateTimeEdit::Section**

- 作用：指定日期的不同部分
- 涉及函数：currentSection(), setCurrentSection(), sectionAt()

| 成员                        | 值     | 说明     |
| --------------------------- | ------ | -------- |
| QDateTimeEdit::NoSection    | 0x0000 | 无部分   |
| QDateTimeEdit::DaySection   | 0x0100 | 天部分   |
| QDateTimeEdit::MonthSection | 0x0200 | 月份部分 |
| QDateTimeEdit::YearSection  | 0x0400 | 年份部分 |

5️⃣【版本适配】 △ Qt 5.12特性

- 改进：更好地支持高DPI显示
- 改进：国际化日期格式支持增强

△ 跨平台差异

- Windows：尊重系统日期格式设置
- macOS：遵循HIG设计规范，风格略有不同
- Linux：依赖于桌面环境，KDE和GNOME风格不同

6️⃣【代码沙箱】 ◇ 标准用法：

```cpp
// 创建基本日期编辑器
QDateEdit *dateEdit = new QDateEdit(this);
dateEdit->setDate(QDate::currentDate());
dateEdit->setDisplayFormat("yyyy年MM月dd日");
dateEdit->setCalendarPopup(true);  // 启用日历弹出窗口

// 设置日期范围
QDate today = QDate::currentDate();
dateEdit->setMinimumDate(today.addDays(-365));  // 最小一年前
dateEdit->setMaximumDate(today.addYears(1));    // 最大一年后

// 连接信号
connect(dateEdit, &QDateEdit::dateChanged, 
        this, &MyWidget::onDateChanged);

// 自定义日期格式示例
QDateEdit *customFormat = new QDateEdit(this);
customFormat->setDisplayFormat("yy/MM/dd (ddd)");  // 23/05/15 (周一)
customFormat->setDate(QDate(2023, 5, 15));

// 将日期编辑器集成到表单中
QFormLayout *layout = new QFormLayout(this);
layout->addRow(tr("出生日期:"), dateEdit);
layout->addRow(tr("预约日期:"), customFormat);
```

◇ 危险操作：

```cpp
// 错误：设置无效的日期格式
dateEdit->setDisplayFormat("错误格式");  // 将使用默认格式
// 正确：使用有效的日期格式字符串
dateEdit->setDisplayFormat("yyyy-MM-dd");

// 错误：设置范围时最小值大于最大值
dateEdit->setMinimumDate(QDate(2023, 12, 31));
dateEdit->setMaximumDate(QDate(2023, 1, 1));
// 正确：确保范围有效
dateEdit->setDateRange(
    QDate(2023, 1, 1),
    QDate(2023, 12, 31)
);

// 错误：尝试直接修改只读模式下的值
dateEdit->setReadOnly(true);
dateEdit->lineEdit()->setText("2023-04-15");  // 不会生效且可能导致显示不一致
// 正确：即使在只读模式下也可以通过API修改值
dateEdit->setDate(QDate(2023, 4, 15));

// 错误：尝试访问时间部分
// QDateEdit隐藏了时间相关部分，以下调用无效
dateEdit->setTime(QTime(14, 30));  // 无效操作
// 正确：只对日期部分进行操作
dateEdit->setDate(QDate(2023, 4, 15));
```



# QTimeEdit API参考手册（Qt 5.12.12）

1️⃣【构造体系】 ◆ 核心构造函数

- QTimeEdit(QWidget *parent = nullptr) ▸ 典型用途：创建显示当前时间的编辑器 ▸ 内存策略：父子对象自动回收
- QTimeEdit(const QTime &time, QWidget *parent = nullptr) ▸ 典型用途：创建显示指定时间的编辑器 ▸ 示例：QTimeEdit *timeEdit = new QTimeEdit(QTime(14, 30, 0), this)

◆ 继承关系

- 继承自QDateTimeEdit，专注于时间编辑功能
- 自动隐藏日期相关显示部分

2️⃣【方法矩阵】 ▨ 核心方法

| 方法签名                                               | 功能描述             | 使用频率 |
| ------------------------------------------------------ | -------------------- | -------- |
| QTime time() const                                     | 获取当前时间值       | ★★★★★    |
| void setTime(const QTime &time)                        | 设置时间值           | ★★★★★    |
| void setDisplayFormat(const QString &format)           | 设置显示格式         | ★★★★★    |
| QString displayFormat() const                          | 获取显示格式         | ★★★      |
| void setMinimumTime(const QTime &min)                  | 设置最小可选时间     | ★★★★     |
| QTime minimumTime() const                              | 获取最小可选时间     | ★★★      |
| void setMaximumTime(const QTime &max)                  | 设置最大可选时间     | ★★★★     |
| QTime maximumTime() const                              | 获取最大可选时间     | ★★★      |
| void setTimeRange(const QTime &min, const QTime &max)  | 同时设置最小最大范围 | ★★★★     |
| QDateTimeEdit::Section currentSection() const          | 获取当前编辑的部分   | ★★★      |
| void setCurrentSection(QDateTimeEdit::Section section) | 设置当前编辑部分     | ★★★      |

▨ 继承自QDateTimeEdit的方法

| 方法                                        | 功能描述             | 使用频率 |
| ------------------------------------------- | -------------------- | -------- |
| void stepBy(int steps)                      | 按步数增减当前段的值 | ★★★      |
| QSize sizeHint() const                      | 获取建议的控件大小   | ★★       |
| void clear()                                | 清除编辑器内容       | ★★★      |
| QDateTimeEdit::Section sectionAt(int index) | 获取指定位置的部分   | ★★       |
| int sectionCount() const                    | 获取部分总数         | ★★       |

▨ 重写方法

| 方法                                | 触发场景       | 必须调用基类 |
| ----------------------------------- | -------------- | ------------ |
| void paintEvent(QPaintEvent *)      | 控件绘制时     | 是           |
| void keyPressEvent(QKeyEvent *)     | 键盘输入时     | 视情况而定   |
| void mousePressEvent(QMouseEvent *) | 鼠标点击时     | 视情况而定   |
| void wheelEvent(QWheelEvent *)      | 鼠标滚轮事件时 | 视情况而定   |
| void focusInEvent(QFocusEvent *)    | 获得焦点时     | 是           |

3️⃣【信号与槽】 ◇ 输出信号

- timeChanged(const QTime &time) → 时间值改变时发射
- userTimeChanged(const QTime &time) → 用户修改时间时发射（Qt 5.12中不存在，5.14+添加）

◇ 输入槽

- setTime(const QTime &time) → 设置时间值
- stepBy(int steps) → 按给定步数增加/减少当前段的值
- clear() → 清除编辑器内容

4️⃣【枚举属性】 ◇ 相关属性

| 属性名            | 类型                    | 默认值       | 描述           |
| ----------------- | ----------------------- | ------------ | -------------- |
| time              | QTime                   | 当前时间     | 显示的时间     |
| maximumTime       | QTime                   | 23:59:59.999 | 最大可选时间   |
| minimumTime       | QTime                   | 00:00:00.000 | 最小可选时间   |
| displayFormat     | QString                 | "HH:mm:ss"   | 显示格式字符串 |
| currentSection    | QDateTimeEdit::Section  | NoSection    | 当前编辑的部分 |
| displayedSections | QDateTimeEdit::Sections | 时间相关部分 | 显示哪些部分   |

◇ 相关枚举（继承自QDateTimeEdit）

- **QDateTimeEdit::Section**

- 作用：指定时间的不同部分
- 涉及函数：currentSection(), setCurrentSection(), sectionAt()

| 成员                         | 值     | 说明            |
| ---------------------------- | ------ | --------------- |
| QDateTimeEdit::NoSection     | 0x0000 | 无部分          |
| QDateTimeEdit::AmPmSection   | 0x0001 | 上午/下午指示器 |
| QDateTimeEdit::MSecSection   | 0x0002 | 毫秒部分        |
| QDateTimeEdit::SecondSection | 0x0004 | 秒部分          |
| QDateTimeEdit::MinuteSection | 0x0008 | 分钟部分        |
| QDateTimeEdit::HourSection   | 0x0010 | 小时部分        |

5️⃣【版本适配】 △ Qt 5.12特性

- 改进：更好地支持高DPI显示
- 改进：国际化时间格式支持增强

△ 跨平台差异

- Windows：尊重系统时间格式设置（12小时/24小时制）
- macOS：遵循HIG设计规范，风格略有不同
- Linux：依赖于桌面环境，KDE和GNOME风格不同

6️⃣【代码沙箱】 ◇ 标准用法：

```cpp
// 创建基本时间编辑器
QTimeEdit *timeEdit = new QTimeEdit(this);
timeEdit->setTime(QTime::currentTime());
timeEdit->setDisplayFormat("HH:mm:ss");

// 设置时间范围
timeEdit->setMinimumTime(QTime(8, 0, 0));    // 最小时间 8:00
timeEdit->setMaximumTime(QTime(18, 30, 0));  // 最大时间 18:30

// 连接信号
connect(timeEdit, &QTimeEdit::timeChanged, 
        this, &MyWidget::onTimeChanged);

// 自定义时间格式示例
QTimeEdit *customFormat = new QTimeEdit(this);
customFormat->setDisplayFormat("hh:mm AP");  // 02:30 PM (12小时制)
customFormat->setTime(QTime(14, 30, 0));

// 将时间编辑器集成到表单中
QFormLayout *layout = new QFormLayout(this);
layout->addRow(tr("预约时间:"), timeEdit);
layout->addRow(tr("提醒时间:"), customFormat);
```

◇ 危险操作：

```cpp
// 错误：设置无效的时间格式
timeEdit->setDisplayFormat("错误格式");  // 将使用默认格式
// 正确：使用有效的时间格式字符串
timeEdit->setDisplayFormat("HH:mm:ss");

// 错误：设置范围时最小值大于最大值
timeEdit->setMinimumTime(QTime(18, 0, 0));
timeEdit->setMaximumTime(QTime(9, 0, 0));
// 正确：确保范围有效
timeEdit->setTimeRange(
    QTime(9, 0, 0),
    QTime(18, 0, 0)
);

// 错误：尝试直接修改只读模式下的值
timeEdit->setReadOnly(true);
timeEdit->lineEdit()->setText("14:30:00");  // 不会生效且可能导致显示不一致
// 正确：即使在只读模式下也可以通过API修改值
timeEdit->setTime(QTime(14, 30, 0));

// 错误：尝试访问日期部分
// QTimeEdit隐藏了日期相关部分，以下调用无效
timeEdit->setDate(QDate(2023, 4, 15));  // 无效操作
// 正确：只对时间部分进行操作
timeEdit->setTime(QTime(14, 30, 0));
```