# Qt 深度学习指南：QLineEdit 组件详解

<details> <summary><b>1️⃣ 原理深度解构层</b></summary>

## QLineEdit 核心机制解析

### 三线解析法

#### ① 运行时行为

- **对象生命周期**：作为 QWidget 派生类，遵循 Qt 对象树管理机制
- **输入处理流程**：keyPressEvent → inputMethodEvent → 文本修改 → textChanged/textEdited 信号
- **文本模型**：内部维护单行文本字符串，支持选区(selection)、撤销/重做(undo/redo)堆栈
- **验证流程**：输入字符 → 验证器(QValidator) → 接受/拒绝/中间状态 → 文本更新/提示

#### ② 框架源码线索

- **核心类**：`QLineEdit` 在 `qlineedit.h`/`qlineedit.cpp`
- **私有实现**：`QLineEditPrivate` 在 `qlineedit_p.h`
- **父类关系**：QLineEdit → QWidget → QObject → QPaintDevice
- **内部交互**：使用 `QTextLayout` 管理文本布局和光标

#### ③ 计算机科学映射

- **设计模式**：观察者模式（通过信号槽通知文本变化）
- **修饰器模式**：通过 QValidator 和 InputMask 扩展行为
- **栈结构**：使用撤销/重做栈实现文本操作历史
- **有限状态机**：验证器使用状态转换模型(Acceptable/Intermediate/Invalid)

### 内存可视化

```
QWidget (表单窗口)
├── usernameLabel (QLabel)           // 标签，指示输入框用途
├── usernameEdit (QLineEdit)         // 单行文本输入框
│   ├── validator (QValidator)       // 可选，验证输入
│   ├── inputMask (QString)          // 可选，掩码格式
│   └── echoMode (枚举值)            // 控制文本显示方式
└── submitButton (QPushButton)       // 提交按钮
```

### 功能分类表

| 功能类型 | 主要API方法                                   | 内部实现机制           |
| -------- | --------------------------------------------- | ---------------------- |
| 文本管理 | text(), setText(), clear()                    | 内部字符串存储与更新   |
| 编辑控制 | setReadOnly(), isReadOnly()                   | 修改输入事件处理行为   |
| 输入验证 | setValidator(), setInputMask()                | 输入过滤与格式强制     |
| 回显模式 | setEchoMode()                                 | 密码显示与文本掩码     |
| 选择操作 | selectAll(), setSelection()                   | 文本光标与选区管理     |
| 辅助功能 | setClearButtonEnabled(), setPlaceholderText() | 特殊UI元素的添加与显示 |

</details> <details> <summary><b>2️⃣ 代码多维训练场</b></summary>

## 基础层 - 核心API展示

```cpp
// 基础QLineEdit用法展示
QLineEdit *edit = new QLineEdit(parentWidget);  // 创建文本输入框
edit->setPlaceholderText("请输入用户名");       // 设置占位提示文本
edit->setText("默认文本");                      // 设置初始文本
edit->setMaxLength(30);                         // 限制最大输入长度
// 连接文本变化信号
connect(edit, &QLineEdit::textChanged, [=](const QString &text) {
    qDebug() << "文本已更改:" << text;
});
// QLineEdit在UI线程中使用，非线程安全
```

## 进阶层 - 场景化案例

```cpp
// 进阶：带验证和自动完成的QLineEdit
class ValidatedInput : public QWidget {
public:
    explicit ValidatedInput(QWidget *parent = nullptr) : QWidget(parent) {
        // 创建布局
        QVBoxLayout *layout = new QVBoxLayout(this);
        
        // 创建输入框
        m_lineEdit = new QLineEdit(this);
        m_lineEdit->setPlaceholderText("输入邮箱地址");
        m_lineEdit->setClearButtonEnabled(true);  // 添加清除按钮
        
        // 创建状态标签
        m_statusLabel = new QLabel(this);
        m_statusLabel->setStyleSheet("QLabel { color: gray; }");
        m_statusLabel->setText("请输入邮箱地址");
        
        // 添加到布局
        layout->addWidget(m_lineEdit);
        layout->addWidget(m_statusLabel);
        
        // 创建正则表达式验证器
        QRegularExpressionValidator *validator = new QRegularExpressionValidator(
            QRegularExpression("\\b[A-Za-z0-9._%+-]+@[A-Za-z0-9.-]+\\.[A-Za-z]{2,}\\b"), 
            this);
        m_lineEdit->setValidator(validator);
        
        // 添加自动完成
        QStringList commonDomains = {
            "gmail.com", "outlook.com", "hotmail.com", "yahoo.com", "qq.com", "163.com"
        };
        
        m_completer = new QCompleter(this);
        m_domainModel = new QStringListModel(this);
        m_completer->setModel(m_domainModel);
        m_completer->setFilterMode(Qt::MatchContains);
        m_lineEdit->setCompleter(m_completer);
        
        // 连接信号
        connect(m_lineEdit, &QLineEdit::textChanged, this, &ValidatedInput::validateInput);
        
        // 错误处理：防止无法添加验证器
        if (!m_lineEdit->validator()) {
            qWarning() << "无法设置邮箱验证器，将使用手动验证";
            // 备用方案：手动验证
            connect(m_lineEdit, &QLineEdit::editingFinished, [this]() {
                QRegularExpression regex("\\b[A-Za-z0-9._%+-]+@[A-Za-z0-9.-]+\\.[A-Za-z]{2,}\\b");
                if (!regex.match(m_lineEdit->text()).hasMatch()) {
                    m_statusLabel->setText("无效的邮箱格式");
                    m_statusLabel->setStyleSheet("QLabel { color: red; }");
                }
            });
        }
        
        // 设置初始状态
        validateInput(m_lineEdit->text());
    }
    
    // 获取有效的邮箱输入
    QString getValidEmail() const {
        if (m_isValid) {
            return m_lineEdit->text();
        }
        return QString();
    }
    
private slots:
    void validateInput(const QString &text) {
        m_isValid = false;
        
        // 更新自动完成域名列表
        updateDomainSuggestions(text);
        
        // 验证输入
        if (text.isEmpty()) {
            m_statusLabel->setText("请输入邮箱地址");
            m_statusLabel->setStyleSheet("QLabel { color: gray; }");
            return;
        }
        
        // 检查验证器状态
        if (m_lineEdit->validator()) {
            QString textCopy = text;
            int pos = 0;
            if (m_lineEdit->validator()->validate(textCopy, pos) == QValidator::Acceptable) {
                m_statusLabel->setText("有效的邮箱格式");
                m_statusLabel->setStyleSheet("QLabel { color: green; }");
                m_isValid = true;
            } else if (m_lineEdit->validator()->validate(textCopy, pos) == QValidator::Intermediate) {
                m_statusLabel->setText("继续输入...");
                m_statusLabel->setStyleSheet("QLabel { color: orange; }");
            } else {
                m_statusLabel->setText("无效的邮箱格式");
                m_statusLabel->setStyleSheet("QLabel { color: red; }");
            }
        } else {
            // 手动验证（备用方案）
            QRegularExpression regex("\\b[A-Za-z0-9._%+-]+@[A-Za-z0-9.-]+\\.[A-Za-z]{2,}\\b");
            if (regex.match(text).hasMatch()) {
                m_statusLabel->setText("有效的邮箱格式");
                m_statusLabel->setStyleSheet("QLabel { color: green; }");
                m_isValid = true;
            } else if (text.contains('@')) {
                m_statusLabel->setText("继续输入...");
                m_statusLabel->setStyleSheet("QLabel { color: orange; }");
            } else {
                m_statusLabel->setText("无效的邮箱格式");
                m_statusLabel->setStyleSheet("QLabel { color: red; }");
            }
        }
    }
    
    void updateDomainSuggestions(const QString &text) {
        // 从已输入文本生成自动完成建议
        QStringList suggestions;
        
        // 如果包含@字符但没有完整域名
        int atPos = text.indexOf('@');
        if (atPos != -1 && atPos < text.length() - 1) {
            // 获取@后的部分文本
            QString domainPart = text.mid(atPos + 1);
            
            QStringList commonDomains = {
                "gmail.com", "outlook.com", "hotmail.com", 
                "yahoo.com", "qq.com", "163.com"
            };
            
            // 生成完整的电子邮件建议
            QString localPart = text.left(atPos + 1);
            for (const QString &domain : commonDomains) {
                if (domain.startsWith(domainPart)) {
                    suggestions.append(localPart + domain);
                }
            }
            
            // 更新自动完成器模型
            m_domainModel->setStringList(suggestions);
            
            // 如果有建议，显示自动完成弹出窗口
            if (!suggestions.isEmpty() && m_lineEdit->hasFocus()) {
                m_completer->complete();
            }
        }
    }
    
private:
    QLineEdit *m_lineEdit;
    QLabel *m_statusLabel;
    QCompleter *m_completer;
    QStringListModel *m_domainModel;
    bool m_isValid = false;
};

// Qt5/Qt6兼容性注释
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    // Qt6中QRegularExpression成为默认正则表达式引擎
#else
    // Qt5中QRegExp和QRegularExpression并存，建议使用QRegularExpression
#endif
```

## 专家层 - 最佳实践方案

```cpp
/**
 * 专家级：高性能、可定制的密码输入框
 * 
 * 特点：
 * 1. 密码强度实时评估
 * 2. 自定义密码显示切换
 * 3. 集成密码泄露检测
 * 4. 高级样式与交互效果
 * 5. 安全性保障与清理
 */
class EnhancedPasswordEdit : public QWidget {
    Q_OBJECT
    Q_PROPERTY(QString password READ password WRITE setPassword NOTIFY passwordChanged)
    Q_PROPERTY(PasswordStrength passwordStrength READ passwordStrength NOTIFY passwordStrengthChanged)
    
public:
    // 密码强度枚举
    enum PasswordStrength {
        VeryWeak = 0,
        Weak = 1,
        Medium = 2,
        Strong = 3,
        VeryStrong = 4
    };
    Q_ENUM(PasswordStrength)
    
    explicit EnhancedPasswordEdit(QWidget *parent = nullptr)
        : QWidget(parent),
          m_strength(VeryWeak),
          m_networkManager(nullptr),
          m_leakCheckEnabled(false),
          m_hasCapsLock(false),
          m_minimumPasswordLength(8),
          m_checkDelay(500)
    {
        setupUi();
        setupConnections();
        
        // 密码强度评估器初始化
        m_strengthPatterns = {
            {QRegularExpression("[a-z]"), 1},  // 小写字母
            {QRegularExpression("[A-Z]"), 2},  // 大写字母
            {QRegularExpression("[0-9]"), 1},  // 数字
            {QRegularExpression("[^a-zA-Z0-9]"), 3}  // 特殊字符
        };
        
        // 常见密码列表（实际应用中应使用更完整的列表）
        m_commonPasswords = {
            "password", "123456", "qwerty", "admin", "welcome",
            "123456789", "12345678", "letmein", "monkey", "1234567"
        };
        
        // 密码策略设置
        m_passwordPolicy.minimumLength = 8;
        m_passwordPolicy.requiresUppercase = true;
        m_passwordPolicy.requiresLowercase = true;
        m_passwordPolicy.requiresDigit = true;
        m_passwordPolicy.requiresSpecial = false;
        
        // 初始化验证定时器
        m_checkTimer.setSingleShot(true);
        connect(&m_checkTimer, &QTimer::timeout, this, &EnhancedPasswordEdit::validatePassword);
        
        // 设置初始状态
        updatePasswordVisibility(false);
        updateStrengthIndicator();
    }
    
    // 设置密码文本
    void setPassword(const QString &password) {
        if (m_passwordEdit->text() != password) {
            m_passwordEdit->setText(password);
            // 实际验证由信号触发
        }
    }
    
    // 获取密码文本
    QString password() const {
        return m_passwordEdit->text();
    }
    
    // 获取当前密码强度
    PasswordStrength passwordStrength() const {
        return m_strength;
    }
    
    // 设置密码策略
    void setPasswordPolicy(int minLength = 8, bool upper = true, 
                           bool lower = true, bool digit = true, 
                           bool special = false) {
        m_passwordPolicy.minimumLength = minLength;
        m_passwordPolicy.requiresUppercase = upper;
        m_passwordPolicy.requiresLowercase = lower;
        m_passwordPolicy.requiresDigit = digit;
        m_passwordPolicy.requiresSpecial = special;
        
        // 更新提示文本
        updatePlaceholderText();
        
        // 重新验证当前密码
        validatePassword();
    }
    
    // 启用/禁用密码泄露检测
    void setLeakCheckEnabled(bool enabled) {
        m_leakCheckEnabled = enabled;
        if (enabled && !m_networkManager) {
            // 延迟初始化网络管理器（当需要时才创建）
            m_networkManager = new QNetworkAccessManager(this);
        }
        
        // 如果启用且已有密码，检查当前密码
        if (enabled && !m_passwordEdit->text().isEmpty()) {
            checkPasswordLeak(m_passwordEdit->text());
        }
    }
    
    // 获取密码策略错误信息
    QStringList getPolicyErrors() const {
        return m_policyErrors;
    }
    
signals:
    void passwordChanged(const QString &password);
    void passwordStrengthChanged(PasswordStrength strength);
    void passwordAccepted(bool accepted);
    void passwordLeaked(bool leaked);
    
protected:
    // 重写事件过滤器以检测 Caps Lock 状态
    bool eventFilter(QObject *watched, QEvent *event) override {
        if (watched == m_passwordEdit) {
            if (event->type() == QEvent::KeyPress) {
                QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
                
                // 检测 Caps Lock 状态
                checkCapsLock(keyEvent);
                
                // 监听 Enter/Return 键作为提交
                if (keyEvent->key() == Qt::Key_Return || keyEvent->key() == Qt::Key_Enter) {
                    emit passwordAccepted(validatePassword());
                    return true;
                }
            }
        }
        return QWidget::eventFilter(watched, event);
    }
    
    // ⚡ 优化绘制性能
    void paintEvent(QPaintEvent *event) override {
        QWidget::paintEvent(event);
        
        // 使用缓存提高重绘性能
        if (m_styleCache.isEmpty() || m_cacheInvalid) {
            updateStyleCache();
        }
        
        // 应用缓存的样式表
        if (!m_styleCache.isEmpty()) {
            setStyleSheet(m_styleCache);
        }
    }
    
private slots:
    // 密码改变时的处理
    void onPasswordChanged(const QString &password) {
        // 清除旧的定时检查
        m_checkTimer.stop();
        
        // 立即更新强度条初步显示
        calculatePasswordStrength(password);
        updateStrengthIndicator();
        
        // 延迟执行完整验证（提高性能）
        m_checkTimer.start(m_checkDelay);
        
        emit passwordChanged(password);
    }
    
    // 密码可见性切换
    void togglePasswordVisibility() {
        bool visible = m_passwordEdit->echoMode() == QLineEdit::Normal;
        updatePasswordVisibility(!visible);
    }
    
    // 验证密码并更新UI
    bool validatePassword() {
        const QString password = m_passwordEdit->text();
        m_policyErrors.clear();
        
        // 空密码处理
        if (password.isEmpty()) {
            m_strength = VeryWeak;
            m_statusLabel->setText("请输入密码");
            m_statusLabel->setStyleSheet("QLabel { color: gray; }");
            updateStrengthIndicator();
            return false;
        }
        
        // 检查密码长度
        if (password.length() < m_passwordPolicy.minimumLength) {
            m_policyErrors.append(QString("密码长度需至少为 %1 个字符").arg(m_passwordPolicy.minimumLength));
        }
        
        // 检查大写字母
        if (m_passwordPolicy.requiresUppercase && !password.contains(QRegularExpression("[A-Z]"))) {
            m_policyErrors.append("密码需包含至少一个大写字母");
        }
        
        // 检查小写字母
        if (m_passwordPolicy.requiresLowercase && !password.contains(QRegularExpression("[a-z]"))) {
            m_policyErrors.append("密码需包含至少一个小写字母");
        }
        
        // 检查数字
        if (m_passwordPolicy.requiresDigit && !password.contains(QRegularExpression("[0-9]"))) {
            m_policyErrors.append("密码需包含至少一个数字");
        }
        
        // 检查特殊字符
        if (m_passwordPolicy.requiresSpecial && !password.contains(QRegularExpression("[^a-zA-Z0-9]"))) {
            m_policyErrors.append("密码需包含至少一个特殊字符");
        }
        
        // 检查常见密码
        if (m_commonPasswords.contains(password.toLower())) {
            m_policyErrors.append("该密码过于常见，请选择更复杂的密码");
        }
        
        // 密码强度完整计算
        calculatePasswordStrength(password);
        
        // 检查密码泄露
        if (m_leakCheckEnabled && password.length() >= 5) {
            checkPasswordLeak(password);
        }
        
        // 更新UI
        updateStrengthIndicator();
        updateStatusLabel();
        
        // 如果有泄露检查在进行，保存待处理的结果
        m_pendingValidation = m_policyErrors.isEmpty();
        
        // 发射强度变化信号
        emit passwordStrengthChanged(m_strength);
        
        return m_policyErrors.isEmpty();
    }
    
private:
    // UI 组件
    QLineEdit *m_passwordEdit;
    QPushButton *m_toggleButton;
    QProgressBar *m_strengthBar;
    QLabel *m_strengthLabel;
    QLabel *m_statusLabel;
    QLabel *m_capsLockLabel;
    
    // 状态变量
    PasswordStrength m_strength;
    QStringList m_policyErrors;
    QTimer m_checkTimer;
    QString m_styleCache;
    bool m_cacheInvalid;
    bool m_pendingValidation;
    
    // 网络检查相关
    QNetworkAccessManager *m_networkManager;
    bool m_leakCheckEnabled;
    
    // 辅助功能状态
    bool m_hasCapsLock;
    
    // 密码策略
    struct {
        int minimumLength;
        bool requiresUppercase;
        bool requiresLowercase;
        bool requiresDigit;
        bool requiresSpecial;
    } m_passwordPolicy;
    
    // 强度评估
    struct PatternScore {
        QRegularExpression pattern;
        int score;
    };
    QVector<PatternScore> m_strengthPatterns;
    QStringList m_commonPasswords;
    
    // 配置选项
    int m_minimumPasswordLength;
    int m_checkDelay;
    
    // 初始化UI
    void setupUi() {
        QHBoxLayout *mainLayout = new QHBoxLayout(this);
        mainLayout->setContentsMargins(0, 0, 0, 0);
        
        // 创建密码输入框
        m_passwordEdit = new QLineEdit(this);
        m_passwordEdit->setEchoMode(QLineEdit::Password);
        m_passwordEdit->setPlaceholderText("输入密码");
        m_passwordEdit->installEventFilter(this);
        
        // 创建密码可见性切换按钮
        m_toggleButton = new QPushButton(this);
        m_toggleButton->setIcon(QIcon::fromTheme("eye-slash", QIcon(":/icons/eye-slash.png")));
        m_toggleButton->setCursor(Qt::PointingHandCursor);
        m_toggleButton->setFixedSize(30, 30);
        m_toggleButton->setFlat(true);
        
        // 创建密码强度指示器
        QVBoxLayout *statusLayout = new QVBoxLayout();
        statusLayout->setContentsMargins(0, 0, 0, 0);
        statusLayout->setSpacing(2);
        
        m_strengthBar = new QProgressBar(this);
        m_strengthBar->setTextVisible(false);
        m_strengthBar->setFixedHeight(6);
        m_strengthBar->setRange(0, 4);
        m_strengthBar->setValue(0);
        
        // 创建状态标签
        m_strengthLabel = new QLabel("强度: 非常弱", this);
        m_strengthLabel->setAlignment(Qt::AlignLeft);
        m_statusLabel = new QLabel("请输入密码", this);
        m_statusLabel->setAlignment(Qt::AlignLeft);
        
        // Caps Lock 提示
        m_capsLockLabel = new QLabel("Caps Lock 已开启", this);
        m_capsLockLabel->setStyleSheet("QLabel { color: orange; }");
        m_capsLockLabel->setVisible(false);
        
        // 布局输入框和按钮
        QHBoxLayout *inputLayout = new QHBoxLayout();
        inputLayout->setContentsMargins(0, 0, 0, 0);
        inputLayout->addWidget(m_passwordEdit);
        inputLayout->addWidget(m_toggleButton);
        
        // 添加到状态布局
        statusLayout->addWidget(m_strengthBar);
        statusLayout->addWidget(m_strengthLabel);
        statusLayout->addWidget(m_statusLabel);
        statusLayout->addWidget(m_capsLockLabel);
        
        // 添加到主布局
        mainLayout->addLayout(inputLayout);
        
        // 创建右侧容器
        QWidget *statusContainer = new QWidget(this);
        statusContainer->setLayout(statusLayout);
        
        // 设置主布局为垂直布局
        QVBoxLayout *containerLayout = new QVBoxLayout(this);
        containerLayout->setContentsMargins(0, 0, 0, 0);
        containerLayout->addLayout(mainLayout);
        containerLayout->addWidget(statusContainer);
        
        setLayout(containerLayout);
        
        // 更新提示文本
        updatePlaceholderText();
    }
    
    // 设置信号连接
    void setupConnections() {
        connect(m_passwordEdit, &QLineEdit::textChanged, 
                this, &EnhancedPasswordEdit::onPasswordChanged);
        connect(m_toggleButton, &QPushButton::clicked, 
                this, &EnhancedPasswordEdit::togglePasswordVisibility);
    }
    
    // 更新占位符文本
    void updatePlaceholderText() {
        QStringList requirements;
        
        if (m_passwordPolicy.minimumLength > 0) {
            requirements.append(QString("至少%1个字符").arg(m_passwordPolicy.minimumLength));
        }
        
        if (m_passwordPolicy.requiresUppercase) {
            requirements.append("大写字母");
        }
        
        if (m_passwordPolicy.requiresLowercase) {
            requirements.append("小写字母");
        }
        
        if (m_passwordPolicy.requiresDigit) {
            requirements.append("数字");
        }
        
        if (m_passwordPolicy.requiresSpecial) {
            requirements.append("特殊字符");
        }
        
        QString placeholderText = "输入密码";
        if (!requirements.isEmpty()) {
            placeholderText = QString("输入密码 (%1)").arg(requirements.join(", "));
        }
        
        m_passwordEdit->setPlaceholderText(placeholderText);
    }
    
    // 切换密码可见性
    void updatePasswordVisibility(bool visible) {
        m_passwordEdit->setEchoMode(visible ? QLineEdit::Normal : QLineEdit::Password);
        m_toggleButton->setIcon(QIcon::fromTheme(
            visible ? "eye" : "eye-slash",
            QIcon(visible ? ":/icons/eye.png" : ":/icons/eye-slash.png")
        ));
    }
    
    // 计算密码强度
    void calculatePasswordStrength(const QString &password) {
        // 空密码处理
        if (password.isEmpty()) {
            m_strength = VeryWeak;
            return;
        }
        
        // 初始分数基于长度
        int score = qMin(20, password.length()) * 4;
        
        // 应用各种模式的分数
        for (const PatternScore &patternScore : m_strengthPatterns) {
            if (password.contains(patternScore.pattern)) {
                score += patternScore.score * 10;
            }
        }
        
        // 长度奖励
        if (password.length() >= 10) {
            score += 10;
        }
        
        // 重复字符惩罚
        QMap<QChar, int> charCount;
        for (const QChar &c : password) {
            charCount[c]++;
        }
        
        for (auto it = charCount.begin(); it != charCount.end(); ++it) {
            if (it.value() > 1) {
                score -= (it.value() - 1) * 2;
            }
        }
        
        // 连续字符惩罚
        for (int i = 1; i < password.length(); i++) {
            if (password[i] == password[i-1]) {
                score -= 2;
            }
        }
        
        // 常见密码惩罚
        if (m_commonPasswords.contains(password.toLower())) {
            score = qMin(score, 20);
        }
        
        // 根据最终分数确定强度级别
        if (score < 40) {
            m_strength = VeryWeak;
        } else if (score < 60) {
            m_strength = Weak;
        } else if (score < 80) {
            m_strength = Medium;
        } else if (score < 100) {
            m_strength = Strong;
        } else {
            m_strength = VeryStrong;
        }
    }
    
    // 更新强度指示器
    void updateStrengthIndicator() {
        m_strengthBar->setValue(static_cast<int>(m_strength));
        
        QString strengthText = "强度: ";
        QString colorStyle;
        
        switch (m_strength) {
            case VeryWeak:
                strengthText += "非常弱";
                colorStyle = "QProgressBar::chunk { background-color: #d9534f; }";
                break;
            case Weak:
                strengthText += "弱";
                colorStyle = "QProgressBar::chunk { background-color: #f0ad4e; }";
                break;
            case Medium:
                strengthText += "中等";
                colorStyle = "QProgressBar::chunk { background-color: #5bc0de; }";
                break;
            case Strong:
                strengthText += "强";
                colorStyle = "QProgressBar::chunk { background-color: #5cb85c; }";
                break;
            case VeryStrong:
                strengthText += "非常强";
                colorStyle = "QProgressBar::chunk { background-color: #00a65a; }";
                break;
        }
        
        m_strengthLabel->setText(strengthText);
        m_strengthBar->setStyleSheet(colorStyle);
        
        // 标记缓存为无效
        m_cacheInvalid = true;
    }
    
    // 更新状态标签
    void updateStatusLabel() {
        if (!m_policyErrors.isEmpty()) {
            m_statusLabel->setText(m_policyErrors.first());
            m_statusLabel->setStyleSheet("QLabel { color: red; }");
        } else {
            m_statusLabel->setText("密码可接受");
            m_statusLabel->setStyleSheet("QLabel { color: green; }");
        }
    }
    
    // 检测 Caps Lock 状态
    void checkCapsLock(QKeyEvent *event) {
        // 检查 Caps Lock 状态的最佳方法依赖于平台
        // 这里使用一个简单的启发式方法
        
        // 如果 Shift 未按下
        bool shiftPressed = event->modifiers() & Qt::ShiftModifier;
        
        // 如果是字母键
        if (event->key() >= Qt::Key_A && event->key() <= Qt::Key_Z) {
            // 获取按键文本
            QString text = event->text();
            
            // 检查大小写
            bool isUpperCase = !text.isEmpty() && text[0].isUpper();
            
            // 如果没有按 Shift 但输出大写，或者按了 Shift 但输出小写
            bool capsLockLikely = (isUpperCase && !shiftPressed) || (!isUpperCase && shiftPressed);
            
            if (m_hasCapsLock != capsLockLikely) {
                m_hasCapsLock = capsLockLikely;
                m_capsLockLabel->setVisible(m_hasCapsLock);
            }
        }
    }
    
    // 检查密码泄露
    void checkPasswordLeak(const QString &password) {
        if (!m_networkManager || password.length() < 5) {
            return;
        }
        
        // 使用 SHA-1 哈希密码
        QCryptographicHash hash(QCryptographicHash::Sha1);
        hash.addData(password.toUtf8());
        QString hashHex = QString(hash.result().toHex()).toUpper();
        
        // 使用 K-匿名模型 - 只发送前5个字符
        QString prefix = hashHex.left(5);
        QString suffix = hashHex.mid(5);
        
        // 创建请求
        QNetworkRequest request(QUrl(QString("https://api.pwnedpasswords.com/range/%1").arg(prefix)));
        request.setRawHeader("User-Agent", "Qt-EnhancedPasswordEdit-Library/1.0");
        
        // 发送请求
        QNetworkReply *reply = m_networkManager->get(request);
        
        // 处理响应
        connect(reply, &QNetworkReply::finished, this, [=]() {
            reply->deleteLater();
            
            if (reply->error() == QNetworkReply::NoError) {
                // 检查密码哈希是否在响应中
                QString response = QString::fromUtf8(reply->readAll());
                QStringList lines = response.split('\n');
                
                bool leaked = false;
                for (const QString &line : lines) {
                    QStringList parts = line.split(':');
                    if (parts.size() == 2 && parts[0] == suffix) {
                        int occurrences = parts[1].toInt();
                        leaked = occurrences > 0;
                        break;
                    }
                }
                
                // 发出泄露信号
                if (leaked) {
                    m_statusLabel->setText("警告：此密码已在数据泄露中出现！");
                    m_statusLabel->setStyleSheet("QLabel { color: red; font-weight: bold; }");
                    m_policyErrors.append("该密码已出现在已知的数据泄露中，请更换");
                }
                
                emit passwordLeaked(leaked);
                
                // 如果有待处理的验证，完成它
                if (m_pendingValidation) {
                    bool valid = m_policyErrors.isEmpty();
                    emit passwordAccepted(valid);
                }
            }
        });
    }
    
    // 更新样式缓存
    void updateStyleCache() {
        m_styleCache = QString(
            "QProgressBar {"
            "   border: none;"
            "   background-color: #f0f0f0;"
            "   border-radius: 2px;"
            "}"
        );
        
        m_styleCache += m_strengthBar->styleSheet();
        
        m_cacheInvalid = false;
    }
};

// 内存分析报告:
// 1. 通过延迟初始化QNetworkAccessManager减少不必要的内存使用
// 2. 使用定时器延迟密码强度评估，减少频繁计算
// 3. 使用样式表缓存减少重绘开销
// 4. 密码验证逻辑封装在组件内，避免密码在外部暴露
// 5. 使用常量定义提高可维护性并减少硬编码
```

## 错误案例展示

```cpp
// 💀 编译通过但运行时可能崩溃的典型错误
void lineEditErrors() {
    // 错误1: 获取不存在的QValidator
    QLineEdit* edit = new QLineEdit();
    QIntValidator* validator = static_cast<QIntValidator*>(edit->validator());
    validator->setBottom(0);  // 💀 空指针访问，会崩溃
    
    // 正确做法：检查validator是否存在
    QValidator* safeValidator = edit->validator();
    if (safeValidator && safeValidator->inherits("QIntValidator")) {
        QIntValidator* intValidator = static_cast<QIntValidator*>(safeValidator);
        intValidator->setBottom(0);
    }
    
    // 错误2: 过早销毁组件依赖的对象
    QCompleter* completer = new QCompleter();
    {
        QStringList items = {"item1", "item2", "item3"};
        QStringListModel* model = new QStringListModel(items);
        completer->setModel(model);  // model没有设置父对象
    }  // model被销毁
    edit->setCompleter(completer);  // completer内部保存了无效的model指针
    
    // 正确做法：确保模型的生命周期
    QCompleter* safeCompleter = new QCompleter(edit);  // 设置父对象
    QStringListModel* model = new QStringListModel(safeCompleter);  // 设置父对象
    model->setStringList({"item1", "item2", "item3"});
    safeCompleter->setModel(model);
    edit->setCompleter(safeCompleter);
    
    // 错误3: 错误的输入掩码导致无法输入
    edit->setInputMask("000-000-0000");  // 电话号码掩码
    edit->setText("文本内容");  // 不符合掩码格式，可能导致意外行为
    
    // 正确做法：确保设置的文本符合掩码格式
    QLineEdit* phoneEdit = new QLineEdit();
    phoneEdit->setInputMask("000-000-0000");
    phoneEdit->setText("123-456-7890");  // 符合掩码格式
}

// 💀 内存泄漏的隐蔽写法
void memoryLeakExample() {
    QDialog* dialog = new QDialog();
    QVBoxLayout* layout = new QVBoxLayout(dialog);
    
    // 错误1: 创建的对象未设置父对象且未被删除
    QValidator* validator = new QIntValidator(0, 100);  // 没有父对象
    QLineEdit* edit = new QLineEdit(dialog);
    edit->setValidator(validator);  // QLineEdit不接管validator的所有权
    
    // 错误2: 自定义模型未正确管理
    QCompleter* completer = new QCompleter();
    completer->setParent(edit);  // 正确设置父对象
    QStringListModel* model = new QStringListModel();  // 未设置父对象
    completer->setModel(model);  // 未转移所有权
    edit->setCompleter(completer);
    
    // 正确做法：
    QLineEdit* safeEdit = new QLineEdit(dialog);
    // 1. 为验证器设置父对象
    QValidator* safeValidator = new QIntValidator(0, 100, safeEdit);
    safeEdit->setValidator(safeValidator);
    
    // 2. 为completer和model都设置父对象
    QCompleter* safeCompleter = new QCompleter(safeEdit);
    QStringListModel* safeModel = new QStringListModel(safeCompleter);
    safeCompleter->setModel(safeModel);
    safeEdit->setCompleter(safeCompleter);
    
    layout->addWidget(edit);
    layout->addWidget(safeEdit);
    dialog->exec();
    delete dialog;  // validator和model不会被删除，造成内存泄漏
}

// 💀 跨线程访问的陷阱
void threadTrapExample() {
    QLineEdit* edit = new QLineEdit();
    
    // 错误：在工作线程直接修改UI对象
    QThread* workerThread = new QThread();
    QObject::connect(workerThread, &QThread::started, [edit]() {
        // 💀 线程安全错误：从非UI线程直接修改UI对象
        for (int i = 0; i < 100; i++) {
            edit->setText(QString("值 %1").arg(i));  // 可能导致崩溃
            QThread::msleep(100);
        }
    });
    
    // 正确做法：使用信号槽机制
    Worker* worker = new Worker();
    worker->moveToThread(workerThread);
    
    // 连接信号槽，确保UI更新在主线程执行
    QObject::connect(worker, &Worker::valueChanged,
                    edit, &QLineEdit::setText,
                    Qt::QueuedConnection);  // 关键：使用队列连接
    
    QObject::connect(workerThread, &QThread::started,
                    worker, &Worker::process);
    
    workerThread->start();
}

// 工作线程类示例（正确做法）
class Worker : public QObject {
    Q_OBJECT
public slots:
    void process() {
        for (int i = 0; i < 100; i++) {
            // 发射信号而不是直接调用UI方法
            emit valueChanged(QString("值 %1").arg(i));
            QThread::msleep(100);
        }
    }
    
signals:
    void valueChanged(const QString& text);
};
```

</details> <details> <summary><b>3️⃣ 知识拓扑网络</b></summary>

## 纵向维度：Qt版本演进路线

### 版本差异表

| 功能       | Qt4            | Qt5                          | Qt6                     | 迁移成本 |
| ---------- | -------------- | ---------------------------- | ----------------------- | -------- |
| 正则表达式 | QRegExp        | QRegExp + QRegularExpression | 🔥QRegularExpression     | ★★★☆☆    |
| 输入验证   | setValidator() | setValidator()               | setValidator()          | ★☆☆☆☆    |
| 占位文本   | 无直接支持     | setPlaceholderText()         | setPlaceholderText()    | ★☆☆☆☆    |
| 清除按钮   | 无直接支持     | setClearButtonEnabled()      | setClearButtonEnabled() | ★☆☆☆☆    |
| 回显模式   | 基本支持       | 增强支持                     | 相同支持                | ★☆☆☆☆    |
| 信号槽连接 | SIGNAL/SLOT宏  | 函数指针 + 旧语法支持        | 🔥推荐函数指针语法       | ★★★☆☆    |
| 高DPI支持  | 无             | 基础支持                     | 🔥完全原生支持           | ★★★☆☆    |

### 🔥 Qt6中的关键变更点

1. QRegularExpression成为默认的正则表达式API，QRegExp被废弃
2. 信号槽连接强烈推荐使用函数指针语法
3. 高DPI缩放默认启用
4. QOverload移至Qt命名空间，使用`qOverload`代替`QOverload`

## 横向维度：跨模块依赖关系

```
QtWidgets::QLineEdit
├── 依赖 QtGui::QPainter (用于绘制文本和控件)
├── 依赖 QtGui::QTextLayout (文本布局管理)
├── 依赖 QtCore::QValidator (可选，用于输入验证)
├── 依赖 QtWidgets::QCompleter (可选，用于自动完成)
└── 依赖 QtGui::QClipboard (剪贴板操作)
```

### 模块关系图

```
QtCore (基础数据类型、验证器接口)
↑
QtGui (绘图和文本处理)
↑
QtWidgets (UI组件) ← QLineEdit位于此模块
↑
应用程序代码
```

## 深度维度：与STL/C++库的对比选择

| Qt类       | STL/C++等效类 | 优势对比                | 使用场景建议     |
| ---------- | ------------- | ----------------------- | ---------------- |
| QLineEdit  | 无直接对应    | Qt提供完整GUI控件       | GUI应用程序      |
| QString    | std::string   | Qt更适合Unicode和国际化 | 处理用户输入文本 |
| QValidator | std::regex    | Qt提供更高级的验证框架  | 用户输入验证     |
| QCompleter | 无直接对应    | Qt提供自动完成功能      | 提升用户输入体验 |

</details> <details> <summary><b>4️⃣ 认知强化体系</b></summary>

## 对比学习表

### QLineEdit vs 其他输入控件

| 特性         | QLineEdit      | QTextEdit      | QPlainTextEdit | 推荐场景               |
| ------------ | -------------- | -------------- | -------------- | ---------------------- |
| 单行/多行    | ★☆☆☆☆ (仅单行) | ★★★★★ (多行)   | ★★★★★ (多行)   | 单行输入用QLineEdit    |
| 输入验证     | ★★★★★          | ★★☆☆☆          | ★★☆☆☆          | 需验证输入用QLineEdit  |
| 富文本支持   | ★☆☆☆☆ (无)     | ★★★★★          | ★★☆☆☆ (纯文本) | 富文本编辑用QTextEdit  |
| 内存占用     | ★☆☆☆☆ (最小)   | ★★★★☆ (较大)   | ★★★☆☆ (中等)   | 大量输入框用QLineEdit  |
| 密码输入     | ★★★★★          | ★☆☆☆☆ (不支持) | ★☆☆☆☆ (不支持) | 密码输入用QLineEdit    |
| 性能(大文本) | ★★★☆☆          | ★★☆☆☆          | ★★★★★          | 大文本用QPlainTextEdit |

### 验证器对比表

| 特性         | QIntValidator | QDoubleValidator | QRegularExpressionValidator | 推荐场景                   |
| ------------ | ------------- | ---------------- | --------------------------- | -------------------------- |
| 数值范围验证 | ★★★★★         | ★★★★★            | ★★☆☆☆                       | 简单数值范围用专用验证器   |
| 格式灵活性   | ★☆☆☆☆ (固定)  | ★★☆☆☆ (可配置)   | ★★★★★ (最灵活)              | 复杂格式用正则表达式验证器 |
| 使用复杂度   | ★☆☆☆☆ (最简)  | ★★☆☆☆ (简单)     | ★★★★☆ (较复杂)              | 简单需求用专用验证器       |
| 性能消耗     | ★☆☆☆☆ (最小)  | ★★☆☆☆ (小)       | ★★★★☆ (较大)                | 性能敏感场景用专用验证器   |
| 国际化支持   | ★★★☆☆         | ★★★☆☆            | ★★★★★                       | 本地化格式需要正则验证器   |

## 速查口诀

### QLineEdit基础口诀

- "单行输入有验证，掩码格式密码藏" *(单行文本输入控件，支持输入验证和掩码格式，提供密码隐藏功能)*
- "自动完成提速度，撤销重做有历史" *(支持自动完成加速输入，内置撤销/重做操作历史)*
- "只读模式锁编辑，清除按钮快复位" *(可设置只读模式阻止编辑，提供清除按钮快速清空内容)*

### 信号连接口诀

- "文本改变两种触，编辑完成失焦点" *(textChanged和textEdited是主要信号，editingFinished在失去焦点时触发)*
- "选择变化可捕获，光标位置能跟踪" *(可监控选择状态变化和光标位置)*

### 错误防范口诀

- "验证器须先检空，Completer父需明确" *(使用验证器前检查其是否为空，Completer及其Model需正确设置父对象)*
- "线程之隔莫直连，信号队列保安全" *(不从工作线程直接操作QLineEdit，使用Qt::QueuedConnection)*
- "掩码合文本不符合，意外行为会产生" *(设置的文本必须符合输入掩码格式，否则会产生意外行为)*

</details> <details> <summary><b>5️⃣ 工程化实践框架</b></summary>

## QLineEdit开发阶段指南

### [设计期]

- **对象树规划**
  - 确定QLineEdit在表单中的位置和关联标签
  - 规划验证逻辑和反馈机制
  - 设计自动完成数据源
- **信号槽拓扑图示例**

```
[QLineEdit] ──> textChanged() ────┐
                                  ↓
[验证器] ────> validate() ────> [验证处理器]
                                  │
                                  │ validationResult()
                                  ↓
                             [UI反馈层]
                                  │
                                  │ setStyleSheet()
                                  ↓
                             [QLineEdit]
```

- 线程边界划分
  - ✅ 在UI线程创建和管理所有QLineEdit实例
  - ✅ 数据验证和处理可在工作线程进行
  - ✅ 使用信号槽机制将处理结果传递到UI线程
  - ❌ 避免在非UI线程直接操作QLineEdit

### [编码期]

- **QLineEdit特有检查项**
  1. 是否需要输入验证？选择合适的QValidator实现
  2. 是否需要设置最大长度限制？
  3. 密码字段是否使用了正确的回显模式？
  4. 是否处理了输入法事件(IME)？
  5. 必填字段是否有明确的视觉提示？
- **最佳实践代码模板**

```cpp
// QLineEdit最佳实践模板
QLineEdit* createStandardLineEdit(QWidget* parent, bool required = false) {
    QLineEdit* edit = new QLineEdit(parent);
    
    // 1. 设置合适的大小策略
    edit->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    
    // 2. 添加清除按钮方便用户
    edit->setClearButtonEnabled(true);
    
    // 3. 根据是否必填设置不同样式
    if (required) {
        // 必填字段样式
        edit->setProperty("required", true);
        edit->setStyleSheet("QLineEdit[required=true] { border: 1px solid #5c7cfa; }");
        
        // 添加必填指示器（可以是边框颜色或标签）
        QLabel* requiredIndicator = new QLabel("*", parent);
        requiredIndicator->setStyleSheet("QLabel { color: red; }");
        requiredIndicator->setBuddy(edit);
        
        // 将指示器放入布局（需要适当调整以适应您的布局）
        QWidget* container = qobject_cast<QWidget*>(edit->parent());
        if (container && container->layout()) {
            static_cast<QHBoxLayout*>(container->layout())->addWidget(requiredIndicator);
        }
    }
    
    // 4. 连接验证信号
    connect(edit, &QLineEdit::textChanged, [edit, required](const QString& text) {
        // 简单验证：必填字段不能为空
        if (required && text.isEmpty()) {
            edit->setStyleSheet("QLineEdit { border: 1px solid red; background: #fff8f8; }");
        } else {
            // 恢复默认样式或应用有效样式
            edit->setStyleSheet(required ? 
                              "QLineEdit { border: 1px solid #5c7cfa; }" : 
                              "");
        }
    });
    
    return edit;
}

// 创建数字输入框
QLineEdit* createNumberInput(QWidget* parent, int min = 0, int max = 100) {
    QLineEdit* edit = createStandardLineEdit(parent);
    
    // 设置数字验证器
    QIntValidator* validator = new QIntValidator(min, max, edit);
    edit->setValidator(validator);
    
    // 设置右对齐（数字通常右对齐）
    edit->setAlignment(Qt::AlignRight);
    
    // 设置占位符提示范围
    edit->setPlaceholderText(QString("%1-%2").arg(min).arg(max));
    
    return edit;
}

// 创建安全的密码输入框
QLineEdit* createPasswordInput(QWidget* parent, bool showToggle = true) {
    QLineEdit* edit = createStandardLineEdit(parent);
    
    // 设置密码模式
    edit->setEchoMode(QLineEdit::Password);
    
    // 禁用自动完成
    edit->setAutoFillBackground(false);
    edit->setAttribute(Qt::WA_InputMethodEnabled, false);
    
    // 如果需要切换可见性
    if (showToggle) {
        QAction* toggleAction = new QAction(edit);
        toggleAction->setIcon(QIcon::fromTheme("eye-slash"));
        toggleAction->setToolTip("显示密码");
        
        connect(toggleAction, &QAction::triggered, [edit, toggleAction]() {
            bool isPassword = edit->echoMode() == QLineEdit::Password;
            edit->setEchoMode(isPassword ? QLineEdit::Normal : QLineEdit::Password);
            toggleAction->setIcon(QIcon::fromTheme(isPassword ? "eye" : "eye-slash"));
            toggleAction->setToolTip(isPassword ? "隐藏密码" : "显示密码");
        });
        
        edit->addAction(toggleAction, QLineEdit::TrailingPosition);
    }
    
    return edit;
}
```

### [调试期]

1. **QLineEdit特定调试技巧**

   - 查看文本实际内容：`qDebug() << "文本:" << edit->text();`
   - 检查验证器状态：`qDebug() << "验证器:" << (edit->hasAcceptableInput() ? "有效" : "无效");`
   - 验证掩码正确性：通过手动设置各种输入测试掩码行为
   - 使用`edit->setTextMargins()`增加边距可视化编辑区域

2. **常见问题排查列表**

   - 无法输入：检查validator、readOnly状态和inputMask设置
   - 文本不显示：检查echoMode是否为Password或NoEcho
   - 验证不生效：确认validator是否正确设置，检查validate()返回值
   - 信号未触发：区分textChanged和textEdited信号的差异

3. **调试环境变量**

   ```
   QT_STYLE_OVERRIDE=Fusion           # 测试不同样式下的外观
   QT_QPA_PLATFORM=windows:fontengine=freetype  # 测试不同字体引擎
   ```

### [优化期]

- **QLineEdit渲染优化清单**

  1. ⚡ 减少实时验证，使用editingFinished信号进行延迟验证
  2. ⚡ 对高频更新的输入字段，考虑节流(throttling)处理
  3. ⚡ 避免复杂的正则表达式验证器，拆分为多个简单验证
  4. ⚡ 使用setInputMethodHints()优化输入法行为
  5. ⚡ 大量QLineEdit时考虑延迟创建验证器和completer

- **内存/性能优化表**

  | 优化措施   | 性能提升 | 内存影响       | 复杂度 |
  | ---------- | -------- | -------------- | ------ |
  | 延迟验证   | ★★★★☆    | ★☆☆☆☆ (无影响) | ★★☆☆☆  |
  | 验证器共享 | ★★☆☆☆    | ★★★★☆ (降低)   | ★★☆☆☆  |
  | 简化正则   | ★★★★☆    | ★☆☆☆☆ (无影响) | ★★★☆☆  |
  | 输入法优化 | ★★★☆☆    | ★☆☆☆☆ (无影响) | ★☆☆☆☆  |
  | 模型共享   | ★★☆☆☆    | ★★★★☆ (降低)   | ★★☆☆☆  |

## 安全红线清单

- ❌ **禁止** 在非UI线程直接操作QLineEdit
- ❌ **禁止** 存储明文密码在QLineEdit的文本属性中
- ❌ **禁止** 对未验证的输入直接用于SQL查询（防止SQL注入）
- ❌ **避免** 使用过于复杂的正则表达式验证器（可能导致性能问题）
- ❌ **避免** 在验证回调中执行耗时操作阻塞UI线程
- ❌ **避免** 在文本变化时频繁更新样式表（性能消耗大）

## 设计模式应用

- **装饰器模式**：使用QValidator扩展输入验证行为

```cpp
// 自定义验证器
class CustomValidator : public QValidator {
public:
    State validate(QString &input, int &pos) const override {
        // 实现自定义验证逻辑
        if (input.isEmpty()) {
            return Intermediate;
        }
        
        // 特定规则验证
        if (meetsCustomRules(input)) {
            return Acceptable;
        }
        
        return Invalid;
    }
    
private:
    bool meetsCustomRules(const QString &text) const {
        // 自定义规则实现
        return true;
    }
};
```

- **观察者模式**：通过信号槽监听文本变化

```cpp
// 表单验证观察者
class FormValidator : public QObject {
public:
    void registerField(QLineEdit* field, bool required) {
        connect(field, &QLineEdit::textChanged, 
                this, [this, field, required]() {
            validateField(field, required);
            updateFormState();
        });
        m_fields.append({field, required});
    }
    
    bool isFormValid() const {
        return m_isValid;
    }
    
signals:
    void formValidityChanged(bool valid);
    
private:
    struct FieldInfo {
        QLineEdit* field;
        bool required;
    };
    QVector<FieldInfo> m_fields;
    bool m_isValid = false;
    
    void validateField(QLineEdit* field, bool required) {
        // 字段验证逻辑
    }
    
    void updateFormState() {
        bool valid = true;
        for (const auto &info : m_fields) {
            if (info.required && info.field->text().isEmpty()) {
                valid = false;
                break;
            }
            
            if (!info.field->hasAcceptableInput()) {
                valid = false;
                break;
            }
        }
        
        if (m_isValid != valid) {
            m_isValid = valid;
            emit formValidityChanged(valid);
        }
    }
};
```

- **策略模式**：不同类型的输入处理策略

```cpp
// 输入格式化策略
class InputFormatterStrategy {
public:
    virtual ~InputFormatterStrategy() {}
    virtual QString format(const QString &input) = 0;
};

// 电话号码格式化
class PhoneFormatter : public InputFormatterStrategy {
public:
    QString format(const QString &input) override {
        // 将数字格式化为电话号码格式
        QString digits = input;
        digits.remove(QRegularExpression("[^0-9]"));
        
        if (digits.length() >= 10) {
            return QString("(%1) %2-%3")
                .arg(digits.mid(0, 3))
                .arg(digits.mid(3, 3))
                .arg(digits.mid(6, 4));
        }
        return input;
    }
};

// 使用策略
void applyFormattingStrategy(QLineEdit* edit, InputFormatterStrategy* strategy) {
    connect(edit, &QLineEdit::editingFinished, [edit, strategy]() {
        QString formatted = strategy->format(edit->text());
        if (formatted != edit->text()) {
            edit->setText(formatted);
        }
    });
}
```

</details> <details> <summary><b>应用实例：高级QLineEdit应用场景</b></summary>

## 1. 信用卡输入框

```cpp
// 信用卡输入框 - 带格式化和验证
class CreditCardInput : public QWidget {
    Q_OBJECT
public:
    CreditCardInput(QWidget *parent = nullptr) : QWidget(parent) {
        // 创建布局
        QVBoxLayout *layout = new QVBoxLayout(this);
        layout->setContentsMargins(0, 0, 0, 0);
        
        // 创建信用卡输入框
        m_cardEdit = new QLineEdit(this);
        m_cardEdit->setPlaceholderText("信用卡号码");
        m_cardEdit->setClearButtonEnabled(true);
        
        // 设置信用卡图标
        QLabel *cardIconLabel = new QLabel(this);
        cardIconLabel->setPixmap(QPixmap(":/icons/credit-card.png").scaled(16, 16));
        m_cardEdit->addAction(QIcon(":/icons/credit-card.png"), QLineEdit::LeadingPosition);
        
        // 设置验证状态标签
        m_statusLabel = new QLabel(this);
        m_statusLabel->setStyleSheet("QLabel { color: gray; font-size: 11px; }");
        m_statusLabel->setText("请输入有效的信用卡号码");
        
        // 连接信号
        connect(m_cardEdit, &QLineEdit::textChanged, this, &CreditCardInput::formatCardNumber);
        connect(m_cardEdit, &QLineEdit::editingFinished, this, &CreditCardInput::validateCardNumber);
        
        // 添加到布局
        layout->addWidget(m_cardEdit);
        layout->addWidget(m_statusLabel);
        
        // 设置最大长度(19字符: 16数字+3空格)
        m_cardEdit->setMaxLength(19);
        
        // 设置固定宽度
        setFixedWidth(280);
    }
    
    // 获取清理后的卡号(仅数字)
    QString getCardNumber() const {
        QString cleanNumber = m_cardEdit->text();
        cleanNumber.remove(QRegularExpression("\\s"));
        return cleanNumber;
    }
    
    // 是否有效的卡号
    bool isValid() const {
        return m_isValid;
    }
    
private slots:
    // 格式化信用卡号码(每4位后插入空格)
    void formatCardNumber(const QString &text) {
        int cursorPos = m_cardEdit->cursorPosition();
        int cursorDelta = 0;
        
        // 移除所有非数字字符
        QString cleanNumber = text;
        cleanNumber.remove(QRegularExpression("[^0-9]"));
        
        // 格式化: 每4个数字后加一个空格
        QString formattedNumber;
        for (int i = 0; i < cleanNumber.length(); ++i) {
            if (i > 0 && i % 4 == 0) {
                formattedNumber.append(' ');
                // 如果光标位于空格之后，需要调整光标位置
                if (i < cursorPos) {
                    cursorDelta++;
                }
            }
            formattedNumber.append(cleanNumber[i]);
        }
        
        // 检测卡类型并更新图标
        detectCardType(cleanNumber);
        
        // 避免无限递归调用
        if (formattedNumber != text) {
            m_cardEdit->blockSignals(true);
            m_cardEdit->setText(formattedNumber);
            m_cardEdit->setCursorPosition(qMin(cursorPos + cursorDelta, formattedNumber.length()));
            m_cardEdit->blockSignals(false);
        }
    }
    
    // 验证卡号
    void validateCardNumber() {
        QString number = getCardNumber();
        
        // 空检查
        if (number.isEmpty()) {
            setValidationStatus(false, "请输入信用卡号码");
            return;
        }
        
        // 长度检查(大多数卡是16位)
        if (number.length() < 13 || number.length() > 19) {
            setValidationStatus(false, "卡号长度无效");
            return;
        }
        
        // Luhn算法验证
        if (!validateLuhn(number)) {
            setValidationStatus(false, "卡号校验失败");
            return;
        }
        
        // 通过所有检查
        setValidationStatus(true, QString("有效的%1").arg(m_cardType));
    }
    
private:
    QLineEdit *m_cardEdit;
    QLabel *m_statusLabel;
    bool m_isValid = false;
    QString m_cardType = "信用卡";
    
    // 设置验证状态和消息
    void setValidationStatus(bool valid, const QString &message) {
        m_isValid = valid;
        
        if (valid) {
            m_statusLabel->setText(message);
            m_statusLabel->setStyleSheet("QLabel { color: green; font-size: 11px; }");
            m_cardEdit->setStyleSheet("QLineEdit { border: 1px solid green; }");
        } else {
            m_statusLabel->setText(message);
            m_statusLabel->setStyleSheet("QLabel { color: red; font-size: 11px; }");
            m_cardEdit->setStyleSheet("QLineEdit { border: 1px solid red; }");
        }
    }
    
    // 检测卡类型
    void detectCardType(const QString &number) {
        if (number.isEmpty()) {
            m_cardType = "信用卡";
            return;
        }
        
        // Visa卡以4开头
        if (number.startsWith('4')) {
            m_cardType = "Visa";
            m_cardEdit->addAction(QIcon(":/icons/visa.png"), QLineEdit::LeadingPosition);
            return;
        }
        
        // MasterCard以51-55开头
        if (number.length() >= 2) {
            int prefix = number.left(2).toInt();
            if (prefix >= 51 && prefix <= 55) {
                m_cardType = "MasterCard";
                m_cardEdit->addAction(QIcon(":/icons/mastercard.png"), QLineEdit::LeadingPosition);
                return;
            }
        }
        
        // American Express以34或37开头
        if (number.length() >= 2) {
            int prefix = number.left(2).toInt();
            if (prefix == 34 || prefix == 37) {
                m_cardType = "American Express";
                m_cardEdit->addAction(QIcon(":/icons/amex.png"), QLineEdit::LeadingPosition);
                return;
            }
        }
        
        // 其他卡类型...
        m_cardType = "信用卡";
        m_cardEdit->addAction(QIcon(":/icons/credit-card.png"), QLineEdit::LeadingPosition);
    }
    
    // Luhn算法验证卡号
    bool validateLuhn(const QString &number) {
        int sum = 0;
        bool alternate = false;
        
        for (int i = number.length() - 1; i >= 0; i--) {
            int n = number[i].digitValue();
            if (alternate) {
                n *= 2;
                if (n > 9) {
                    n = n - 9;
                }
            }
            
            sum += n;
            alternate = !alternate;
        }
        
        return (sum % 10 == 0);
    }
};
```

## 2. 带搜索能力的输入框

```cpp
// 高级搜索输入框组件
class SearchLineEdit : public QLineEdit {
    Q_OBJECT
public:
    enum SearchMode {
        InstantSearch,    // 实时搜索(输入时)
        DelayedSearch,    // 延迟搜索(停止输入后)
        ManualSearch      // 手动搜索(按回车或点击按钮)
    };
    
    SearchLineEdit(QWidget *parent = nullptr) : QLineEdit(parent),
        m_searchMode(InstantSearch),
        m_delayMs(300),
        m_minLength(1),
        m_historyLimit(10),
        m_isSearching(false)
    {
        // 设置基本属性
        setPlaceholderText("搜索...");
        setClearButtonEnabled(true);
        
        // 添加搜索图标
        m_searchAction = new QAction(this);
        m_searchAction->setIcon(QIcon::fromTheme("search", QIcon(":/icons/search.png")));
        addAction(m_searchAction, QLineEdit::LeadingPosition);
        
        // 添加历史菜单
        m_historyMenu = new QMenu(this);
        m_historyAction = new QAction("搜索历史", this);
        m_historyAction->setIcon(QIcon::fromTheme("history", QIcon(":/icons/history.png")));
        m_historyAction->setMenu(m_historyMenu);
        addAction(m_historyAction, QLineEdit::TrailingPosition);
        
        // 初始化定时器
        m_delayTimer.setSingleShot(true);
        connect(&m_delayTimer, &QTimer::timeout, this, &SearchLineEdit::performSearch);
        
        // 连接信号
        connect(this, &QLineEdit::textChanged, this, &SearchLineEdit::onTextChanged);
        connect(this, &QLineEdit::returnPressed, this, &SearchLineEdit::onReturnPressed);
        connect(m_searchAction, &QAction::triggered, this, &SearchLineEdit::performSearch);
        
        // 更新UI状态
        updateUiState();
    }
    
    // 设置搜索模式
    void setSearchMode(SearchMode mode) {
        m_searchMode = mode;
        updateUiState();
    }
    
    // 设置延迟时间(对于DelayedSearch模式)
    void setSearchDelay(int milliseconds) {
        m_delayMs = milliseconds;
    }
    
    // 设置最小搜索长度
    void setMinimumSearchLength(int length) {
        m_minLength = length;
    }
    
    // 设置历史记录上限
    void setHistoryLimit(int limit) {
        m_historyLimit = limit;
        while (m_searchHistory.size() > m_historyLimit) {
            m_searchHistory.removeLast();
        }
        updateHistoryMenu();
    }
    
    // 清除搜索历史
    void clearHistory() {
        m_searchHistory.clear();
        updateHistoryMenu();
    }
    
    // 设置自动完成列表
    void setSuggestions(const QStringList &suggestions) {
        if (!m_completer) {
            m_completer = new QCompleter(this);
            m_completer->setCaseSensitivity(Qt::CaseInsensitive);
            m_completerModel = new QStringListModel(this);
            m_completer->setModel(m_completerModel);
            setCompleter(m_completer);
        }
        
        m_completerModel->setStringList(suggestions);
    }
    
signals:
    void searchStarted(const QString &query);
    void searchFinished(const QString &query);
    void searchCancelled();
    
public slots:
    // 执行搜索
    void performSearch() {
        QString query = text().trimmed();
        
        // 检查最小长度
        if (query.length() < m_minLength) {
            return;
        }
        
        // 设置正在搜索状态
        m_isSearching = true;
        updateUiState();
        
        // 添加到历史记录
        if (!query.isEmpty() && !m_searchHistory.contains(query)) {
            m_searchHistory.prepend(query);
            
            // 保持历史记录在限制范围内
            while (m_searchHistory.size() > m_historyLimit) {
                m_searchHistory.removeLast();
            }
            
            updateHistoryMenu();
        }
        
        // 发出搜索信号
        emit searchStarted(query);
        
        // 在实际应用中，这里可能异步请求数据
        // 然后在结果返回时调用finishSearch()
        
        // 对于示例，我们直接调用完成
        QTimer::singleShot(500, this, [this, query]() {
            finishSearch(query);
        });
    }
    
    // 完成搜索
    void finishSearch(const QString &query) {
        m_isSearching = false;
        updateUiState();
        emit searchFinished(query);
    }
    
    // 取消搜索
    void cancelSearch() {
        if (m_isSearching) {
            m_isSearching = false;
            m_delayTimer.stop();
            updateUiState();
            emit searchCancelled();
        }
    }
    
protected:
    void focusOutEvent(QFocusEvent *event) override {
        QLineEdit::focusOutEvent(event);
        
        // 焦点丢失时可能取消搜索
        if (m_searchMode == InstantSearch && m_isSearching) {
            finishSearch(text());
        }
    }
    
    void keyPressEvent(QKeyEvent *event) override {
        // Esc键取消搜索
        if (event->key() == Qt::Key_Escape) {
            if (m_isSearching) {
                cancelSearch();
                return;
            } else if (!text().isEmpty()) {
                clear();
                return;
            }
        }
        
        QLineEdit::keyPressEvent(event);
    }
    
private slots:
    void onTextChanged(const QString &text) {
        // 取消之前的延迟搜索
        m_delayTimer.stop();
        
        // 如果输入被清空且正在搜索，取消搜索
        if (text.isEmpty() && m_isSearching) {
            cancelSearch();
            return;
        }
        
        // 根据搜索模式处理
        if (m_searchMode == InstantSearch) {
            // 立即搜索
            performSearch();
        } else if (m_searchMode == DelayedSearch) {
            // 延迟搜索
            m_delayTimer.start(m_delayMs);
        }
        // ManualSearch模式下不做任何处理
    }
    
    void onReturnPressed() {
        // 回车键总是触发搜索，无论当前模式如何
        m_delayTimer.stop();  // 取消任何待定的延迟搜索
        performSearch();
    }
    
    void handleHistoryItemClicked() {
        QAction *action = qobject_cast<QAction*>(sender());
        if (action) {
            setText(action->text());
            performSearch();
        }
    }
    
private:
    SearchMode m_searchMode;
    int m_delayMs;
    int m_minLength;
    int m_historyLimit;
    bool m_isSearching;
    
    QAction *m_searchAction;
    QAction *m_historyAction;
    QMenu *m_historyMenu;
    QCompleter *m_completer = nullptr;
    QStringListModel *m_completerModel = nullptr;
    QStringList m_searchHistory;
    QTimer m_delayTimer;
    
    // 更新历史记录菜单
    void updateHistoryMenu() {
        m_historyMenu->clear();
        
        if (m_searchHistory.isEmpty()) {
            QAction *noHistoryAction = m_historyMenu->addAction("无搜索历史");
            noHistoryAction->setEnabled(false);
        } else {
            for (const QString &query : m_searchHistory) {
                QAction *action = m_historyMenu->addAction(query);
                connect(action, &QAction::triggered, 
                        this, &SearchLineEdit::handleHistoryItemClicked);
            }
            
            m_historyMenu->addSeparator();
            QAction *clearAction = m_historyMenu->addAction("清除历史记录");
            connect(clearAction, &QAction::triggered, this, &SearchLineEdit::clearHistory);
        }
        
        // 更新历史图标可见性
        m_historyAction->setVisible(!m_searchHistory.isEmpty());
    }
    
    // 更新UI状态
    void updateUiState() {
        if (m_isSearching) {
            // 正在搜索状态
            m_searchAction->setIcon(QIcon::fromTheme("process-stop", 
                                   QIcon(":/icons/stop.png")));
            setReadOnly(true);
            
            // 取消搜索时点击搜索图标
            disconnect(m_searchAction, &QAction::triggered, 
                      this, &SearchLineEdit::performSearch);
            connect(m_searchAction, &QAction::triggered, 
                   this, &SearchLineEdit::cancelSearch);
        } else {
            // 正常状态
            m_searchAction->setIcon(QIcon::fromTheme("search", 
                                   QIcon(":/icons/search.png")));
            setReadOnly(false);
            
            // 点击搜索图标执行搜索
            disconnect(m_searchAction, &QAction::triggered, 
                      this, &SearchLineEdit::cancelSearch);
            connect(m_searchAction, &QAction::triggered, 
                   this, &SearchLineEdit::performSearch);
        }
        
        // 根据搜索模式设置提示
        switch (m_searchMode) {
            case InstantSearch:
                setPlaceholderText("搜索 (实时)...");
                break;
            case DelayedSearch:
                setPlaceholderText(QString("搜索 (%1ms)...").arg(m_delayMs));
                break;
            case ManualSearch:
                setPlaceholderText("搜索 (按回车)...");
                break;
        }
    }
};
```

## 3. 自动格式化的日期/时间输入框

```cpp
// 日期输入框组件
class DateLineEdit : public QLineEdit {
    Q_OBJECT
public:
    enum DateFormat {
        YYYY_MM_DD,    // 2023-01-31
        MM_DD_YYYY,    // 01/31/2023
        DD_MM_YYYY,    // 31.01.2023
        Custom         // 自定义格式
    };
    
    DateLineEdit(QWidget *parent = nullptr) : QLineEdit(parent), 
        m_format(YYYY_MM_DD),
        m_separator('-'),
        m_customFormat("yyyy-MM-dd"),
        m_isValid(false)
    {
        // 设置基本属性
        setPlaceholderText("年-月-日");
        setClearButtonEnabled(true);
        
        // 添加日历图标
        QAction *calendarAction = new QAction(this);
        calendarAction->setIcon(QIcon::fromTheme("office-calendar", 
                               QIcon(":/icons/calendar.png")));
        addAction(calendarAction, QLineEdit::TrailingPosition);
        
        // 点击图标显示日历
        connect(calendarAction, &QAction::triggered, this, &DateLineEdit::showCalendar);
        
        // 连接文本变化信号
        connect(this, &QLineEdit::textChanged, this, &DateLineEdit::onTextChanged);
        connect(this, &QLineEdit::editingFinished, this, &DateLineEdit::validateAndFormat);
        
        // 设置输入掩码
        updateInputMask();
    }
    
    // 设置日期格式
    void setDateFormat(DateFormat format) {
        if (m_format != format) {
            m_format = format;
            updateInputMask();
            updatePlaceholderText();
            validateAndFormat();
        }
    }
    
    // 设置自定义格式
    void setCustomFormat(const QString &format) {
        if (m_customFormat != format) {
            m_customFormat = format;
            if (m_format == Custom) {
                updateInputMask();
                updatePlaceholderText();
                validateAndFormat();
            }
        }
    }
    
    // 设置日期分隔符
    void setSeparator(QChar separator) {
        if (m_separator != separator) {
            m_separator = separator;
            updateInputMask();
            updatePlaceholderText();
            validateAndFormat();
        }
    }
    
    // 获取日期值
    QDate date() const {
        return m_date;
    }
    
    // 设置日期值
    void setDate(const QDate &date) {
        if (date != m_date || text().isEmpty()) {
            m_date = date;
            updateText();
        }
    }
    
    // 是否为有效日期
    bool isValid() const {
        return m_isValid;
    }
    
signals:
    // 日期变化信号
    void dateChanged(const QDate &date);
    // 日期有效性变化信号
    void validityChanged(bool valid);
    
public slots:
    // 设置为当前日期
    void setToday() {
        setDate(QDate::currentDate());
    }
    
private slots:
    // 文本变化处理
    void onTextChanged(const QString &text) {
        // 如果为空，重置状态
        if (text.isEmpty()) {
            if (m_isValid) {
                m_isValid = false;
                m_date = QDate();
                setStyleSheet("");
                emit validityChanged(false);
                emit dateChanged(m_date);
            }
            return;
        }
        
        // 仅在文本完整时进行验证
        if (text.length() == getExpectedLength()) {
            validateAndFormat();
        }
    }
    
    // 显示日历对话框
    void showCalendar() {
        QDialog dialog(this);
        dialog.setWindowTitle("选择日期");
        
        QVBoxLayout *layout = new QVBoxLayout(&dialog);
        
        QCalendarWidget *calendar = new QCalendarWidget(&dialog);
        calendar->setGridVisible(true);
        calendar->setVerticalHeaderFormat(QCalendarWidget::NoVerticalHeader);
        
        // 如果当前有效日期，设置为当前选中
        if (m_isValid) {
            calendar->setSelectedDate(m_date);
        }
        
        // 按钮区域
        QDialogButtonBox *buttonBox = new QDialogButtonBox(
            QDialogButtonBox::Ok | QDialogButtonBox::Cancel, 
            Qt::Horizontal, 
            &dialog);
        
        connect(buttonBox, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
        connect(buttonBox, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);
        
        // 添加"今天"按钮
        QPushButton *todayButton = new QPushButton("今天", &dialog);
        connect(todayButton, &QPushButton::clicked, [calendar]() {
            calendar->setSelectedDate(QDate::currentDate());
        });
        buttonBox->addButton(todayButton, QDialogButtonBox::ActionRole);
        
        layout->addWidget(calendar);
        layout->addWidget(buttonBox);
        
        // 显示对话框
        if (dialog.exec() == QDialog::Accepted) {
            setDate(calendar->selectedDate());
        }
    }
    
private:
    DateFormat m_format;
    QChar m_separator;
    QString m_customFormat;
    QDate m_date;
    bool m_isValid;
    
    // 验证并格式化输入
    void validateAndFormat() {
        QString input = text();
        
        // 空输入直接返回
        if (input.isEmpty()) {
            m_isValid = false;
            m_date = QDate();
            setStyleSheet("");
            emit validityChanged(false);
            return;
        }
        
        // 解析日期
        QDate parsedDate = parseDate(input);
        
        if (parsedDate.isValid()) {
            // 有效日期
            if (!m_isValid || m_date != parsedDate) {
                m_isValid = true;
                m_date = parsedDate;
                updateText();  // 确保格式一致
                setStyleSheet("QLineEdit { background-color: #e8f5e9; }");
                emit validityChanged(true);
                emit dateChanged(m_date);
            }
        } else {
            // 无效日期
            if (m_isValid) {
                m_isValid = false;
                m_date = QDate();
                setStyleSheet("QLineEdit { background-color: #ffebee; }");
                emit validityChanged(false);
                emit dateChanged(m_date);
            } else {
                setStyleSheet("QLineEdit { background-color: #ffebee; }");
            }
        }
    }
    
    // 解析日期文本
    QDate parseDate(const QString &input) {
        QDate result;
        
        switch (m
```