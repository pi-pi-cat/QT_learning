# Qt 深度学习指南：QComboBox 组件详解

<details> <summary><b>1️⃣ 原理深度解构层</b></summary>

## QComboBox 核心机制解析

### 三线解析法

#### ① 运行时行为

- **对象生命周期**：QComboBox 继承自 QWidget，遵循 Qt 对象树管理机制
- **交互流程**：点击 → showPopup() → 弹出列表 → 用户选择 → 触发 activated()/currentIndexChanged() 信号
- **数据管理**：内部使用 QStandardItemModel 存储条目数据，内嵌 QListView 显示选项
- **编辑流程**：setEditable(true) → 添加 QLineEdit → 编辑后触发 editTextChanged() 信号

#### ② 框架源码线索

- **核心类**：`QComboBox` 在 `qcombobox.h`/`qcombobox.cpp`
- **私有实现**：`QComboBoxPrivate` 在 `qcombobox_p.h`
- **父类关系**：QComboBox → QWidget → QObject → QPaintDevice
- **内部视图**：使用 `QAbstractItemView` 的派生类（默认为 `QListView`）显示下拉列表

#### ③ 计算机科学映射

- **设计模式**：采用 MVC (Model-View-Controller) 模式，分离数据和表现层
- **复合模式**：组合了按钮和列表视图（Composite Pattern）
- **委托模式**：使用 QStyledItemDelegate 控制项目渲染和编辑行为

### 内存可视化

```
QComboBox (数据选择控件)
├── QStandardItemModel (内部数据模型)
│   ├── item1 (QStandardItem)
│   ├── item2 (QStandardItem)
│   └── ... (更多选项)
├── QListView (下拉列表视图) - 仅在弹出时创建
└── QLineEdit (编辑框) - 仅在可编辑模式下创建
```

### 功能分类表

| 功能类型 | 主要API方法                           | 内部实现机制                |
| -------- | ------------------------------------- | --------------------------- |
| 条目管理 | addItem(), insertItem(), removeItem() | 操作内部 QStandardItemModel |
| 状态控制 | setCurrentIndex(), currentIndex()     | 维护内部选择状态            |
| 编辑功能 | setEditable(), lineEdit()             | 动态添加/管理 QLineEdit     |
| 外观定制 | setItemDelegate(), view()             | 定制项目表现和列表视图      |
| 模型交互 | setModel(), model(), setModelColumn() | 直接操作数据源              |
| 弹出控制 | showPopup(), hidePopup()              | 管理下拉列表的显示逻辑      |

</details> <details> <summary><b>2️⃣ 代码多维训练场</b></summary>

## 基础层 - 核心API展示

```cpp
// 基础QComboBox用法展示
QComboBox *combo = new QComboBox(parentWidget);  // 创建组合框
combo->addItem("选项1");                         // 添加文本选项
combo->addItem(QIcon(":/icons/item2.png"), "选项2");  // 添加带图标的选项
combo->setCurrentIndex(0);                       // 设置当前选中项
// 连接选择变化信号
connect(combo, QOverload<int>::of(&QComboBox::currentIndexChanged),
        [=](int index) { qDebug() << "选中索引:" << index; });
// 注：QComboBox非线程安全，只能在UI线程中使用
```

## 进阶层 - 场景化案例

```cpp
// 进阶：可编辑组合框与数据验证
QComboBox *countryCombo = new QComboBox(parentWidget);
countryCombo->setEditable(true);          // 设置可编辑模式
countryCombo->setInsertPolicy(QComboBox::NoInsert);  // 防止编辑添加新项
countryCombo->setMaxVisibleItems(15);     // 限制可见项数量
countryCombo->setSizeAdjustPolicy(QComboBox::AdjustToContents);  // 自动调整尺寸

// 添加国家列表
QStringList countries = {"China", "USA", "Japan", "Germany", "France"};
countryCombo->addItems(countries);

// 获取和自定义QLineEdit
QLineEdit *lineEdit = countryCombo->lineEdit();
if (lineEdit) {
    // 设置自动完成
    lineEdit->setPlaceholderText("选择或输入国家...");
    lineEdit->setCompleter(new QCompleter(countries, lineEdit));
    
    // 添加输入验证器
    QRegularExpressionValidator *validator = 
        new QRegularExpressionValidator(QRegularExpression("[A-Za-z ]+"), lineEdit);
    lineEdit->setValidator(validator);
    
    // 连接编辑信号
    connect(lineEdit, &QLineEdit::textChanged, 
            [=](const QString &text) {
                bool valid = !text.isEmpty();
                lineEdit->setStyleSheet(valid ? "" : "background-color: #FEE;");
            });
}

// 处理用户选择
connect(countryCombo, &QComboBox::currentTextChanged,
        [=](const QString &text) {
            if (!text.isEmpty()) {
                qDebug() << "Selected country:" << text;
            }
        });

// 错误处理：确保在动态添加时检查数据有效性
void addCountry(QComboBox *combo, const QString &country) {
    if (country.isEmpty()) {
        qWarning() << "Cannot add empty country name";
        return;
    }
    
    // 检查重复项
    if (combo->findText(country) == -1) {
        combo->addItem(country);
    } else {
        qDebug() << "Country already exists:" << country;
    }
}

// Qt5/Qt6兼容性注释
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    // Qt6中connect使用全局namespace的QOverload
    connect(countryCombo, qOverload<int>(&QComboBox::activated),
            [=](int index) { /* 处理激活事件 */ });
#else
    // Qt5中使用QOverload类模板
    connect(countryCombo, QOverload<int>::of(&QComboBox::activated),
            [=](int index) { /* 处理激活事件 */ });
#endif
```

## 专家层 - 最佳实践方案

```cpp
/**
 * 专家级：高性能、自定义数据的QComboBox实现
 * 
 * 特点：
 * 1. 使用自定义数据模型优化大量数据
 * 2. 提供异步数据加载机制
 * 3. 自定义委托实现复杂项目渲染
 * 4. 集成验证和自动完成功能
 * 5. 支持分层数据显示
 */
class EnhancedComboBox : public QComboBox {
    Q_OBJECT
    
public:
    // 定义自定义数据角色
    enum CustomRoles {
        IdRole = Qt::UserRole + 1,
        CategoryRole,
        IconUrlRole,
        FilterDataRole
    };
    
    explicit EnhancedComboBox(QWidget *parent = nullptr)
        : QComboBox(parent),
          m_asyncLoader(nullptr),
          m_loadingIndicator(nullptr),
          m_batchSize(50),
          m_currentBatch(0),
          m_isLoading(false)
    {
        // ⚡ 性能优化：设置视图属性
        setMaxVisibleItems(15);
        setMaxCount(10000);  // 防止无限增长
        setSizeAdjustPolicy(QComboBox::AdjustToMinimumContentsLengthWithIcon);
        setMinimumContentsLength(20);
        
        // 创建自定义模型
        m_model = new QStandardItemModel(this);
        setModel(m_model);
        
        // 创建自定义委托
        CustomItemDelegate *delegate = new CustomItemDelegate(this);
        setItemDelegate(delegate);
        
        // 使组合框可编辑，提供过滤功能
        setEditable(true);
        setInsertPolicy(QComboBox::NoInsert);
        
        // 配置编辑器
        QLineEdit *editor = lineEdit();
        if (editor) {
            // 设置自动完成器
            QCompleter *completer = new QCompleter(m_model, this);
            completer->setCaseSensitivity(Qt::CaseInsensitive);
            completer->setFilterMode(Qt::MatchContains);
            editor->setCompleter(completer);
            
            // 连接编辑信号用于过滤
            connect(editor, &QLineEdit::textChanged, 
                    this, &EnhancedComboBox::filterItems);
        }
        
        // 创建加载指示器
        m_loadingIndicator = new QProgressBar(this);
        m_loadingIndicator->setRange(0, 0);  // 无限进度条
        m_loadingIndicator->setVisible(false);
        m_loadingIndicator->setMaximumHeight(4);
        m_loadingIndicator->setTextVisible(false);
        
        // 设置自定义布局
        QVBoxLayout *layout = new QVBoxLayout(this);
        layout->setContentsMargins(0, 0, 0, 0);
        layout->addWidget(m_loadingIndicator);
        
        // 优化弹出行为
        connect(this, &QComboBox::aboutToShow, 
                this, &EnhancedComboBox::preparePopup);
                
        // 初始化缓存系统
        initCache();
    }
    
    // 加载数据API
    void loadItems(const QVector<ItemData> &items, bool clearExisting = true) {
        if (clearExisting) {
            m_model->clear();
            m_currentBatch = 0;
        }
        
        // ⚡ 性能优化：批量添加
        m_model->blockSignals(true);
        
        const int startIndex = m_model->rowCount();
        for (int i = 0; i < items.size(); ++i) {
            const ItemData &data = items[i];
            QStandardItem *item = new QStandardItem(data.displayText);
            
            // 设置自定义数据
            item->setData(data.id, IdRole);
            item->setData(data.category, CategoryRole);
            
            // 添加图标(如果有)
            if (!data.iconUrl.isEmpty()) {
                item->setData(data.iconUrl, IconUrlRole);
                
                // 检查图标缓存
                if (m_iconCache.contains(data.iconUrl)) {
                    item->setIcon(m_iconCache.value(data.iconUrl));
                } else {
                    // 稍后异步加载图标
                    m_pendingIcons.append(data.iconUrl);
                }
            }
            
            // 添加过滤文本
            item->setData(data.filterData, FilterDataRole);
            
            m_model->appendRow(item);
        }
        
        m_model->blockSignals(false);
        
        // 如果是首批数据，选择第一项
        if (clearExisting && m_model->rowCount() > 0) {
            setCurrentIndex(0);
        }
        
        // 加载图标
        if (!m_pendingIcons.isEmpty()) {
            QTimer::singleShot(50, this, &EnhancedComboBox::loadPendingIcons);
        }
    }
    
    // 异步加载数据
    void loadDataAsync(const QString &source, const QVariantMap &params = {}) {
        // 显示加载指示器
        setLoading(true);
        
        // 取消任何正在进行的加载
        if (m_asyncLoader) {
            m_asyncLoader->disconnect();
            m_asyncLoader->deleteLater();
        }
        
        // 创建新的加载器线程
        m_asyncLoader = new DataLoader(source, params, this);
        
        connect(m_asyncLoader, &DataLoader::batchLoaded,
                this, &EnhancedComboBox::handleBatchLoaded);
                
        connect(m_asyncLoader, &DataLoader::finished,
                this, &EnhancedComboBox::handleLoadFinished);
                
        connect(m_asyncLoader, &DataLoader::error,
                this, &EnhancedComboBox::handleLoadError);
                
        // 启动加载
        m_asyncLoader->start();
    }
    
    // 获取当前选中项的自定义数据
    QVariant currentItemData(int role = IdRole) const {
        int index = currentIndex();
        if (index >= 0) {
            return m_model->data(m_model->index(index, 0), role);
        }
        return QVariant();
    }
    
    // 设置分层数据
    void setHierarchicalData(const QMap<QString, QStringList> &categories) {
        m_model->clear();
        
        // 添加分层数据
        for (auto it = categories.begin(); it != categories.end(); ++it) {
            // 添加类别项
            QStandardItem *categoryItem = new QStandardItem(it.key());
            categoryItem->setSelectable(false);
            categoryItem->setData(true, Qt::UserRole);  // 标记为类别项
            m_model->appendRow(categoryItem);
            
            // 添加子项
            foreach (const QString &item, it.value()) {
                QStandardItem *childItem = new QStandardItem(item);
                categoryItem->appendRow(childItem);
            }
        }
        
        // 设置视图为树视图
        QTreeView *treeView = new QTreeView(this);
        treeView->setHeaderHidden(true);
        treeView->setRootIsDecorated(false);
        treeView->setIndentation(20);
        setView(treeView);
    }
    
protected:
    // 重写显示弹出窗口
    void showPopup() override {
        preparePopup();
        QComboBox::showPopup();
    }
    
    // 重写绘制事件以处理边缘情况
    void paintEvent(QPaintEvent *event) override {
        QComboBox::paintEvent(event);
        
        // 如果没有选中项但有占位符文本，绘制占位符
        if (currentIndex() < 0 && !m_placeholderText.isEmpty() && !isEditable()) {
            QPainter painter(this);
            painter.setPen(palette().color(QPalette::Disabled, QPalette::Text));
            
            QStyleOptionComboBox opt;
            initStyleOption(&opt);
            QRect textRect = style()->subControlRect(
                QStyle::CC_ComboBox, &opt, QStyle::SC_ComboBoxEditField, this);
            
            painter.drawText(textRect.adjusted(4, 0, -4, 0), 
                             Qt::AlignVCenter, m_placeholderText);
        }
    }
    
private slots:
    // 准备弹出前加载更多数据
    void preparePopup() {
        if (m_asyncLoader && m_asyncLoader->hasMoreData() && 
            !m_isLoading && view()->isVisible()) {
            // 如果用户向下滚动到接近底部，加载更多项目
            loadNextBatch();
        }
    }
    
    // 加载下一批数据
    void loadNextBatch() {
        if (m_asyncLoader && !m_isLoading) {
            m_currentBatch++;
            m_asyncLoader->loadBatch(m_currentBatch, m_batchSize);
        }
    }
    
    // 处理批量加载完成
    void handleBatchLoaded(const QVector<ItemData> &items) {
        loadItems(items, false);
    }
    
    // 处理加载完成
    void handleLoadFinished() {
        setLoading(false);
    }
    
    // 处理加载错误
    void handleLoadError(const QString &errorMessage) {
        setLoading(false);
        qWarning() << "Failed to load combo box data:" << errorMessage;
        
        // 显示错误提示
        QStandardItem *errorItem = new QStandardItem(
            tr("Error loading data: %1").arg(errorMessage));
        errorItem->setEnabled(false);
        errorItem->setData(QColor(Qt::red), Qt::ForegroundRole);
        m_model->clear();
        m_model->appendRow(errorItem);
    }
    
    // 过滤项目
    void filterItems(const QString &text) {
        if (!isEditable() || !m_model)
            return;
            
        // 特殊情况：空文本显示所有项
        if (text.isEmpty()) {
            for (int i = 0; i < m_model->rowCount(); ++i) {
                view()->setRowHidden(i, false);
            }
            return;
        }
        
        // 执行过滤
        for (int i = 0; i < m_model->rowCount(); ++i) {
            QModelIndex index = m_model->index(i, 0);
            QString itemText = m_model->data(index, Qt::DisplayRole).toString();
            QString filterData = m_model->data(index, FilterDataRole).toString();
            
            bool match = itemText.contains(text, Qt::CaseInsensitive) || 
                         filterData.contains(text, Qt::CaseInsensitive);
                         
            view()->setRowHidden(i, !match);
        }
    }
    
    // 加载挂起的图标
    void loadPendingIcons() {
        if (m_pendingIcons.isEmpty())
            return;
            
        // 每次批处理最多5个图标
        const int batchSize = 5;
        QStringList batch = m_pendingIcons.mid(0, batchSize);
        m_pendingIcons = m_pendingIcons.mid(batchSize);
        
        for (const QString &iconUrl : batch) {
            // 使用QNetworkAccessManager异步加载图标
            QNetworkRequest request(QUrl(iconUrl));
            QNetworkReply *reply = m_networkManager.get(request);
            
            connect(reply, &QNetworkReply::finished, this, [=]() {
                reply->deleteLater();
                
                if (reply->error() == QNetworkReply::NoError) {
                    QPixmap pixmap;
                    pixmap.loadFromData(reply->readAll());
                    
                    // 缓存图标
                    m_iconCache.insert(iconUrl, QIcon(pixmap));
                    
                    // 更新使用此图标的所有项
                    for (int i = 0; i < m_model->rowCount(); ++i) {
                        QModelIndex index = m_model->index(i, 0);
                        if (m_model->data(index, IconUrlRole).toString() == iconUrl) {
                            m_model->setData(index, QIcon(pixmap), Qt::DecorationRole);
                        }
                    }
                }
            });
        }
        
        // 如果还有挂起的图标，继续加载
        if (!m_pendingIcons.isEmpty()) {
            QTimer::singleShot(100, this, &EnhancedComboBox::loadPendingIcons);
        }
    }
    
private:
    QStandardItemModel *m_model;
    DataLoader *m_asyncLoader;      // 异步数据加载器
    QProgressBar *m_loadingIndicator;
    QString m_placeholderText;
    int m_batchSize;
    int m_currentBatch;
    bool m_isLoading;
    
    // 图标缓存
    QHash<QString, QIcon> m_iconCache;
    QStringList m_pendingIcons;
    QNetworkAccessManager m_networkManager;
    
    // 设置加载状态
    void setLoading(bool loading) {
        m_isLoading = loading;
        if (m_loadingIndicator) {
            m_loadingIndicator->setVisible(loading);
        }
    }
    
    // 初始化缓存系统
    void initCache() {
        // 限制图标缓存大小
        const int maxIcons = 200;
        if (m_iconCache.size() > maxIcons) {
            // 简单策略：清除所有缓存
            m_iconCache.clear();
        }
    }
};

// 自定义项目委托
class CustomItemDelegate : public QStyledItemDelegate {
public:
    explicit CustomItemDelegate(QObject *parent = nullptr)
        : QStyledItemDelegate(parent) {}
        
    // 自定义绘制
    void paint(QPainter *painter, const QStyleOptionViewItem &option,
               const QModelIndex &index) const override {
        if (index.data(Qt::UserRole).toBool()) {
            // 这是一个类别项，使用特殊渲染
            QStyleOptionViewItem categoryOption = option;
            categoryOption.font.setBold(true);
            categoryOption.backgroundBrush = QColor(240, 240, 240);
            
            QStyledItemDelegate::paint(painter, categoryOption, index);
        } else {
            // 常规项目
            QStyledItemDelegate::paint(painter, option, index);
            
            // 添加额外信息（如果有）
            QString category = index.data(EnhancedComboBox::CategoryRole).toString();
            if (!category.isEmpty()) {
                QRect r = option.rect;
                QFont categoryFont = option.font;
                categoryFont.setPointSize(categoryFont.pointSize() - 1);
                painter->setFont(categoryFont);
                painter->setPen(QColor(120, 120, 120));
                painter->drawText(r.adjusted(r.width() - 100, 0, -5, 0), 
                                 Qt::AlignRight | Qt::AlignVCenter, category);
            }
        }
    }
    
    // 提供自定义大小提示
    QSize sizeHint(const QStyleOptionViewItem &option,
                  const QModelIndex &index) const override {
        QSize size = QStyledItemDelegate::sizeHint(option, index);
        
        // 类别项稍高
        if (index.data(Qt::UserRole).toBool()) {
            size.setHeight(size.height() + 4);
        }
        
        return size;
    }
};

// 数据加载器（工作线程）
class DataLoader : public QThread {
    Q_OBJECT
public:
    DataLoader(const QString &source, const QVariantMap &params, QObject *parent = nullptr)
        : QThread(parent), m_source(source), m_params(params), 
          m_hasMore(true), m_error(false) {}
          
    bool hasMoreData() const {
        return m_hasMore;
    }
    
    void loadBatch(int batch, int count) {
        QMutexLocker locker(&m_mutex);
        m_requestedBatch = batch;
        m_requestedCount = count;
        m_condition.wakeOne();
    }
    
signals:
    void batchLoaded(const QVector<EnhancedComboBox::ItemData> &items);
    void finished();
    void error(const QString &errorMessage);
    
protected:
    void run() override {
        // 加载初始批次
        loadBatchData(0, m_params.value("initialCount", 100).toInt());
        
        // 等待请求加载更多
        QMutexLocker locker(&m_mutex);
        while (!isInterruptionRequested() && m_hasMore) {
            m_condition.wait(&m_mutex);
            
            if (isInterruptionRequested())
                break;
                
            int batch = m_requestedBatch;
            int count = m_requestedCount;
            locker.unlock();
            
            loadBatchData(batch, count);
            
            locker.relock();
        }
    }
    
private:
    QString m_source;
    QVariantMap m_params;
    bool m_hasMore;
    bool m_error;
    
    QMutex m_mutex;
    QWaitCondition m_condition;
    int m_requestedBatch;
    int m_requestedCount;
    
    void loadBatchData(int batch, int count) {
        // 模拟数据加载
        // 在实际应用中，这里应该是从数据库、网络等加载数据
        QVector<EnhancedComboBox::ItemData> items;
        
        // ... 加载数据的代码 ...
        
        // 判断是否还有更多数据
        m_hasMore = (items.size() == count);
        
        // 发送结果
        if (!items.isEmpty()) {
            emit batchLoaded(items);
        }
        
        if (!m_hasMore || m_error) {
            emit finished();
        }
    }
};

// 内存分析报告:
// 1. 图标缓存控制在最多200个，防止过度内存使用
// 2. 批量加载机制减少UI线程阻塞，提高响应性
// 3. 优化过滤算法，减少大量项目时的性能损失
// 4. 自定义委托使用缓存减少重复计算
// 5. 通过延迟加载图标减少初始化时间
```

## 错误案例展示

```cpp
// 💀 编译通过但运行时可能崩溃的典型错误
void comboBoxErrors() {
    QComboBox* combo = new QComboBox();
    
    // 错误1: 访问不存在的索引
    combo->addItem("选项1");
    combo->setCurrentIndex(5);  // 超出范围，索引无效
    QString text = combo->currentText();  // 可能返回空字符串
    
    // 错误2: 对非可编辑组合框获取lineEdit
    QLineEdit* lineEdit = combo->lineEdit();  // 返回nullptr，combo未设置为可编辑
    if (lineEdit) {
        lineEdit->setText("测试");  // 💀 空指针访问，会崩溃
    }
    
    // 错误3: 不检查模型操作的结果
    combo->setModelColumn(3);  // 设置不存在的列
    // 在没有检查模型有效性的情况下继续操作可能导致不可预期行为
    
    // 正确做法示例：
    // 1. 总是检查索引有效性
    int index = 5;
    if (index >= 0 && index < combo->count()) {
        combo->setCurrentIndex(index);
    }
    
    // 2. 在使用lineEdit前检查组合框是否可编辑
    if (!combo->isEditable()) {
        combo->setEditable(true);
    }
    QLineEdit* safeLineEdit = combo->lineEdit();
    
    // 3. 检查模型操作
    QAbstractItemModel* model = combo->model();
    if (model && model->columnCount() > 3) {
        combo->setModelColumn(3);
    }
}

// 💀 内存泄漏的隐蔽写法
void memoryLeakExample() {
    QDialog* dialog = new QDialog();
    QVBoxLayout* layout = new QVBoxLayout(dialog);
    
    // 错误1: 创建的自定义数据未释放
    for (int i = 0; i < 10; i++) {
        QComboBox* combo = new QComboBox(dialog);
        // 💀 内存泄漏：UserData未设置父对象且未被删除
        combo->addItem("项目" + QString::number(i), 
                      QVariant::fromValue(new CustomData()));
        layout->addWidget(combo);
    }
    
    // 错误2: 自定义模型未正确管理
    QComboBox* comboWithModel = new QComboBox(dialog);
    QStandardItemModel* model = new QStandardItemModel();  // 未设置父对象
    comboWithModel->setModel(model);
    layout->addWidget(comboWithModel);
    // 当comboWithModel被删除时，model不会自动删除
    
    // 正确做法：
    QComboBox* correctCombo = new QComboBox(dialog);
    // 1. 为自定义模型设置父对象
    QStandardItemModel* correctModel = new QStandardItemModel(correctCombo);
    correctCombo->setModel(correctModel);
    
    // 2. 使用智能指针管理自定义数据
    QSharedPointer<CustomData> sharedData(new CustomData());
    correctCombo->addItem("安全项目", QVariant::fromValue(sharedData.data()));
    
    layout->addWidget(correctCombo);
    dialog->exec();
    delete dialog;
}

// 💀 跨线程访问的陷阱
void threadTrapExample() {
    QComboBox* combo = new QComboBox();
    
    // 错误：在工作线程直接修改UI对象
    QThread* workerThread = new QThread();
    QObject::connect(workerThread, &QThread::started, [combo]() {
        // 💀 线程安全错误：从非UI线程直接修改UI对象
        for (int i = 0; i < 100; i++) {
            combo->addItem("线程项目 " + QString::number(i));  // 可能导致崩溃
            QThread::msleep(10);
        }
    });
    
    // 正确做法：使用信号槽机制
    DataLoader* loader = new DataLoader();
    loader->moveToThread(workerThread);
    
    // 连接信号槽，确保UI更新在主线程执行
    QObject::connect(loader, &DataLoader::itemLoaded,
                    [combo](const QString& item) {
                        combo->addItem(item);
                    }, Qt::QueuedConnection);  // 关键：使用队列连接
    
    QObject::connect(workerThread, &QThread::started,
                    loader, &DataLoader::loadItems);
    
    workerThread->start();
}

// 数据加载器类示例（正确做法）
class DataLoader : public QObject {
    Q_OBJECT
public slots:
    void loadItems() {
        for (int i = 0; i < 100; i++) {
            // 发射信号而不是直接调用UI方法
            emit itemLoaded("安全项目 " + QString::number(i));
            QThread::msleep(10);
        }
        emit finished();
    }
    
signals:
    void itemLoaded(const QString& item);
    void finished();
};
```

</details> <details> <summary><b>3️⃣ 知识拓扑网络</b></summary>

## 纵向维度：Qt版本演进路线

### 版本差异表

| 功能       | Qt4               | Qt5                     | Qt6                 | 迁移成本 |
| ---------- | ----------------- | ----------------------- | ------------------- | -------- |
| 信号槽连接 | SIGNAL/SLOT宏     | 函数指针 + 旧语法支持   | 🔥推荐函数指针语法   | ★★★☆☆    |
| 模型/视图  | 基本模型/视图架构 | 改进的模型/视图架构     | 完全重构的model架构 | ★★★★☆    |
| 委托系统   | QItemDelegate     | QStyledItemDelegate推荐 | QStyledItemDelegate | ★★☆☆☆    |
| 样式系统   | 基础样式表支持    | 扩展的样式表系统        | 增强的样式引擎      | ★★★☆☆    |
| 高DPI支持  | 无                | 基础支持                | 🔥完全原生支持       | ★★★☆☆    |
| 事件处理   | 旧式事件过滤器    | 同Qt4 + 事件分发器      | 强化的事件系统      | ★★☆☆☆    |

### 🔥 Qt6中的关键变更点

1. 删除对旧式信号槽语法的推荐，强化类型安全连接
2. 重构了内部模型/视图架构以提高性能
3. 全新的图形引擎支持，改进了高DPI渲染
4. QOverload移至Qt命名空间，使用`qOverload`代替`QOverload`

## 横向维度：跨模块依赖关系

```
QtWidgets::QComboBox
├── 依赖 QtWidgets::QAbstractItemView (视图组件)
├── 依赖 QtWidgets::QLineEdit (当可编辑时)
├── 依赖 QtCore::QAbstractItemModel (数据模型)
├── 依赖 QtGui::QPainter (自定义绘制)
└── 依赖 QtGui::QStandardItemModel (默认内部模型)
```

### 模块关系图

```
QtCore (基础数据类型和模型接口)
↑
QtGui (图形绘制和标准模型)
↑
QtWidgets (UI组件) ← QComboBox位于此模块
↑
应用程序代码
```

## 深度维度：与STL/C++库的对比选择

| Qt类               | STL/C++等效类            | 优势对比                 | 使用场景建议           |
| ------------------ | ------------------------ | ------------------------ | ---------------------- |
| QComboBox          | 无直接对应               | Qt提供完整GUI控件        | GUI应用程序            |
| QStandardItemModel | std::vector<T>           | Qt模型支持视图和信号/槽  | 与Qt UI交互时用Qt模型  |
| QStringList        | std::vector<std::string> | Qt更适合Unicode          | 用于ComboBox选项管理   |
| QVariant           | std::any/std::variant    | Qt支持更广泛类型且跨平台 | 存储ComboBox的项目数据 |

</details> <details> <summary><b>4️⃣ 认知强化体系</b></summary>

## 对比学习表

### QComboBox vs 其他选择控件

| 特性           | QComboBox      | QListWidget  | QRadioButton组 | 推荐场景              |
| -------------- | -------------- | ------------ | -------------- | --------------------- |
| 空间占用       | ★☆☆☆☆ (最小)   | ★★★★★ (最大) | ★★★☆☆ (中等)   | 空间受限用QComboBox   |
| 可见选项数     | ★☆☆☆☆ (仅当前) | ★★★★★ (全部) | ★★★★★ (全部)   | 大量选项用QComboBox   |
| 编辑/过滤      | ★★★★★          | ★★☆☆☆        | ★☆☆☆☆ (不支持) | 需过滤选择用QComboBox |
| 多选支持       | ★☆☆☆☆ (不支持) | ★★★★★        | ★☆☆☆☆ (不支持) | 多选场景用QListWidget |
| 层次数据       | ★★★☆☆          | ★★☆☆☆        | ★☆☆☆☆ (不支持) | 简单层次用QComboBox   |
| 性能(大量项目) | ★★★★☆          | ★★☆☆☆        | ★☆☆☆☆ (不适用) | 大数据集用QComboBox   |

### QComboBox模型/视图特性对比

| 特性       | 使用内置模型 | 使用自定义模型 | 推荐场景                  |
| ---------- | ------------ | -------------- | ------------------------- |
| 开发复杂度 | ★☆☆☆☆ (最简) | ★★★★☆ (较复杂) | 简单列表用内置模型        |
| 数据灵活性 | ★★☆☆☆        | ★★★★★          | 复杂/动态数据用自定义模型 |
| 内存效率   | ★★★☆☆        | ★★★★★          | 大量数据用自定义模型      |
| 过滤/排序  | ★★☆☆☆        | ★★★★★          | 需高级过滤用自定义模型    |
| 更新效率   | ★★★☆☆        | ★★★★★          | 频繁更新用自定义模型      |

## 速查口诀

### QComboBox基础口诀

- "点击弹出选择列，当前显示常一项" *(点击组合框时弹出下拉列表供选择，平时只显示当前选中项)*
- "添加移除当索引，文本图标带数据" *(通过索引操作项目，每项可包含文本、图标和用户数据)*
- "可编辑带输入框，不可编辑仅选择" *(可编辑模式带QLineEdit，不可编辑模式只能从列表选择)*

### 信号连接口诀

- "改变索引激活选，文本改变都有信" *(currentIndexChanged, activated, currentTextChanged 是主要信号)*
- "类型重载需指明，新式连接保类型" *(信号重载需使用QOverload，新式连接语法保证类型安全)*

### 错误防范口诀

- "索引访问先检查，编辑模式再取框" *(访问索引前检查有效性，获取lineEdit前确认是可编辑模式)*
- "自定义数据需释放，模型设父防泄漏" *(自定义数据需管理内存，设置模型父对象防止泄漏)*
- "线程之隔不直接，队列连接保平安" *(不从工作线程直接操作QComboBox，使用Qt::QueuedConnection)*

</details> <details> <summary><b>5️⃣ 工程化实践框架</b></summary>

## QComboBox开发阶段指南

### [设计期]

- **对象树规划**
  - 确定QComboBox在视图层级中的位置
  - 规划数据源与组合框的连接方式
  - 设计项目数据结构和获取方式
- **信号槽拓扑图示例**

```
[数据模型] ──> dataChanged() ───┐
                                ↓
                            [适配器层]
                                ↓
                  addItems() ──> [QComboBox]
                                ↑      │
                                │      │ currentIndexChanged()
[用户操作] ────────────────────┘      │
                                       ↓
                                  [业务逻辑]
```

- 线程边界划分
  - ✅ 在UI线程创建和管理所有QComboBox实例
  - ✅ 数据加载和处理在工作线程进行
  - ✅ 使用信号槽机制将处理后的数据传递到UI线程
  - ❌ 避免在非UI线程直接操作QComboBox

### [编码期]

- **QComboBox特有检查项**
  1. 是否正确处理空数据/初始状态？
  2. 是否需要自定义委托处理特殊显示需求？
  3. 大量数据时是否考虑了性能优化？
  4. 编辑模式下是否处理了数据验证？
  5. 是否正确处理了自定义数据的内存管理？
- **最佳实践代码模板**

```cpp
// QComboBox最佳实践模板
QComboBox* createStandardComboBox(QWidget* parent, bool editable = false) {
    QComboBox* combo = new QComboBox(parent);
    
    // 1. 设置合适的大小策略
    combo->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    
    // 2. 设置适当的尺寸调整策略
    combo->setSizeAdjustPolicy(QComboBox::AdjustToContentsOnFirstShow);
    
    // 3. 限制可见项数量（改善性能和用户体验）
    combo->setMaxVisibleItems(15);
    
    // 4. 可编辑模式设置
    if (editable) {
        combo->setEditable(true);
        combo->setInsertPolicy(QComboBox::NoInsert);  // 防止意外添加项目
        
        // 获取编辑框并自定义
        QLineEdit* editor = combo->lineEdit();
        if (editor) {
            // 设置验证器（示例：只允许字母和空格）
            editor->setValidator(new QRegularExpressionValidator(
                QRegularExpression("[A-Za-z ]+"), editor));
                
            // 启用清除按钮
            editor->setClearButtonEnabled(true);
        }
    }
    
    // 5. 添加占位符文本（Qt 5.2+）
    combo->setPlaceholderText("请选择...");
    
    return combo;
}

// 安全添加自定义数据
template<typename T>
void addItemWithData(QComboBox* combo, const QString& text, 
                    const QIcon& icon, T* data) {
    if (!combo) return;
    
    // 使用QVariant包装指针，但要确保适当管理内存
    // 方法1：使用QObject继承并设置父对象
    if (std::is_base_of<QObject, T>::value) {
        static_cast<QObject*>(data)->setParent(combo);
        combo->addItem(icon, text, QVariant::fromValue(data));
        return;
    }
    
    // 方法2：使用智能指针（推荐）
    QSharedPointer<T>* sharedPtr = new QSharedPointer<T>(data);
    combo->addItem(icon, text, QVariant::fromValue(sharedPtr));
    
    // 在组合框销毁时清理数据
    QObject::connect(combo, &QObject::destroyed, [sharedPtr]() {
        delete sharedPtr;
    });
}
```

### [调试期]

1. **QComboBox特定调试技巧**

   - 使用`combo->count()`检查项目数量
   - 使用`combo->findText("text")`确认项目是否存在
   - 检查`combo->model()->rowCount()`与预期是否一致
   - 设置`combo->view()->setVisible(true)`永久显示下拉列表进行调试

2. **常见问题排查列表**

   - 弹出列表为空：检查是否添加了项目，模型是否正确
   - 项目显示不正确：检查文本和图标设置，委托是否正常工作
   - 编辑框行为异常：检查validator和completer设置
   - 信号没有触发：检查连接语法，特别是重载信号

3. **调试环境变量**

   ```
   QT_SCALE_FACTOR=1.5           # 测试高DPI环境
   QT_STYLE_OVERRIDE=Fusion      # 测试不同样式下的外观
   QT_STYLESHEET_DEBUG=1         # 调试样式表问题
   ```

### [优化期]

- **QComboBox渲染优化清单**

  1. ⚡ 限制最大可见项数`setMaxVisibleItems()`
  2. ⚡ 延迟数据加载：使用自定义模型分批加载数据
  3. ⚡ 使用`setSizeAdjustPolicy()`控制大小调整行为
  4. ⚡ 避免频繁添加/删除项目，可批量操作
  5. ⚡ 图标使用共享资源`QIcon::fromTheme()`或缓存机制

- **内存/性能优化表**

  | 优化措施     | 性能提升 | 内存影响       | 复杂度 |
  | ------------ | -------- | -------------- | ------ |
  | 限制可见项数 | ★★★★☆    | ★☆☆☆☆ (无影响) | ★☆☆☆☆  |
  | 自定义模型   | ★★★★★    | ★★★★☆ (降低)   | ★★★★☆  |
  | 延迟加载     | ★★★★★    | ★★★★★ (最小)   | ★★★☆☆  |
  | 图标共享     | ★★☆☆☆    | ★★★★☆ (降低)   | ★★☆☆☆  |
  | 批量操作     | ★★★★☆    | ★☆☆☆☆ (无影响) | ★★☆☆☆  |

## 安全红线清单

- ❌ **禁止** 在非UI线程直接操作QComboBox
- ❌ **禁止** 不检查索引有效性就访问项目
- ❌ **禁止** 向QVariant传递裸指针而不管理内存
- ❌ **避免** 在大型数据集上使用普通QComboBox，应使用自定义模型
- ❌ **避免** 频繁添加/删除项目，应批量操作
- ❌ **避免** 在下拉列表显示时执行耗时操作

## 设计模式应用

- **适配器模式**：将不同数据源转换为ComboBox可用格式

```cpp
// 将外部数据源适配到ComboBox
class DatabaseComboAdapter : public QObject {
public:
    void populateComboBox(QComboBox* combo, const QString& query) {
        combo->clear();
        // 查询数据库并填充ComboBox
        QSqlQuery sql(query);
        while (sql.next()) {
            combo->addItem(sql.value(1).toString(), sql.value(0));
        }
    }
};
```

- **装饰器模式**：扩展QComboBox功能

```cpp
// 添加搜索功能的ComboBox
class SearchableComboBox : public QComboBox {
public:
    SearchableComboBox(QWidget* parent = nullptr) : QComboBox(parent) {
        setEditable(true);
        setInsertPolicy(QComboBox::NoInsert);
        
        // 自定义事件过滤器
        lineEdit()->installEventFilter(this);
        
        // 连接信号槽
        connect(lineEdit(), &QLineEdit::textChanged, 
                this, &SearchableComboBox::filterItems);
    }
    
protected:
    // 自定义过滤
    void filterItems(const QString& text) {
        // 实现搜索逻辑
    }
};
```

- **工厂模式**：创建不同类型的组合框

```cpp
// 组合框工厂
class ComboBoxFactory {
public:
    enum ComboType { Standard, Editable, Searchable, Hierarchical };
    
    static QComboBox* createComboBox(ComboType type, QWidget* parent = nullptr) {
        switch (type) {
            case Editable:
                return createEditableCombo(parent);
            case Searchable:
                return new SearchableComboBox(parent);
            case Hierarchical:
                return createHierarchicalCombo(parent);
            default:
                return new QComboBox(parent);
        }
    }
    
private:
    static QComboBox* createEditableCombo(QWidget* parent) {
        QComboBox* combo = new QComboBox(parent);
        combo->setEditable(true);
        // 配置其他属性...
        return combo;
    }
    
    static QComboBox* createHierarchicalCombo(QWidget* parent) {
        QComboBox* combo = new QComboBox(parent);
        QTreeView* treeView = new QTreeView(combo);
        combo->setView(treeView);
        // 配置树视图...
        return combo;
    }
};
```

</details> <details> <summary><b>应用实例：高级QComboBox应用场景</b></summary>

## 1. 级联下拉菜单

```cpp
// 级联下拉菜单示例：省/市选择器
class RegionSelector : public QWidget {
    Q_OBJECT
public:
    RegionSelector(QWidget *parent = nullptr) : QWidget(parent) {
        // 创建布局
        QHBoxLayout *layout = new QHBoxLayout(this);
        
        // 创建省份选择器
        m_provinceCombo = new QComboBox(this);
        m_provinceCombo->setPlaceholderText("选择省份");
        
        // 创建城市选择器
        m_cityCombo = new QComboBox(this);
        m_cityCombo->setPlaceholderText("选择城市");
        m_cityCombo->setEnabled(false);  // 初始禁用
        
        // 添加到布局
        layout->addWidget(m_provinceCombo);
        layout->addWidget(m_cityCombo);
        
        // 连接省份变化信号
        connect(m_provinceCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
                this, &RegionSelector::updateCities);
        
        // 加载省份数据
        loadProvinces();
    }
    
    // 获取选中的省份和城市
    QString getSelectedRegion() const {
        if (m_provinceCombo->currentIndex() < 0 || m_cityCombo->currentIndex() < 0) {
            return QString();
        }
        
        return QString("%1-%2").arg(m_provinceCombo->currentText(),
                                   m_cityCombo->currentText());
    }
    
private slots:
    void updateCities(int provinceIndex) {
        m_cityCombo->clear();
        
        if (provinceIndex < 0) {
            m_cityCombo->setEnabled(false);
            return;
        }
        
        // 获取当前省份ID
        QVariant provinceId = m_provinceCombo->currentData();
        
        // 加载对应城市
        m_cityCombo->setEnabled(true);
        
        // 假设这是省份对应城市的数据结构
        const QMap<int, QStringList> provinceCities = {
            {1, {"北京"}},
            {2, {"上海"}},
            {3, {"广州", "深圳", "珠海"}},
            {4, {"杭州", "宁波", "温州"}},
            {5, {"南京", "苏州", "无锡"}}
        };
        
        // 加载城市数据
        m_cityCombo->addItems(provinceCities.value(provinceId.toInt()));
        
        // 如果有城市，选中第一个
        if (m_cityCombo->count() > 0) {
            m_cityCombo->setCurrentIndex(0);
        }
        
        // 发出区域变化信号
        emit regionChanged(getSelectedRegion());
    }
    
private:
    QComboBox *m_provinceCombo;
    QComboBox *m_cityCombo;
    
    void loadProvinces() {
        // 模拟加载省份数据
        struct Province {
            int id;
            QString name;
        };
        
        const QVector<Province> provinces = {
            {1, "北京"},
            {2, "上海"},
            {3, "广东"},
            {4, "浙江"},
            {5, "江苏"}
        };
        
        for (const auto &province : provinces) {
            m_provinceCombo->addItem(province.name, province.id);
        }
    }
    
signals:
    void regionChanged(const QString &region);
};
```

## 2. 自动完成与过滤

```cpp
// 高级自动完成与过滤ComboBox
class FilteringComboBox : public QComboBox {
    Q_OBJECT
public:
    FilteringComboBox(QWidget *parent = nullptr) : QComboBox(parent) {
        // 设置可编辑
        setEditable(true);
        setInsertPolicy(QComboBox::NoInsert);
        setMaxVisibleItems(15);
        
        // 创建自定义过滤器模型
        m_filterModel = new QSortFilterProxyModel(this);
        m_filterModel->setFilterCaseSensitivity(Qt::CaseInsensitive);
        m_filterModel->setSourceModel(model());
        
        // 设置自定义视图
        QListView *view = new QListView(this);
        view->setModel(m_filterModel);
        view->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
        setView(view);
        
        // 获取编辑器
        QLineEdit *editor = lineEdit();
        if (editor) {
            // 移除编辑器上的自动完成器（我们将自己处理过滤）
            editor->setCompleter(nullptr);
            
            // 连接文本变化信号
            connect(editor, &QLineEdit::textEdited,
                    this, &FilteringComboBox::filterItems);
                    
            // 自定义键盘事件处理
            editor->installEventFilter(this);
        }
        
        // 连接激活信号以更新显示文本
        connect(this, QOverload<int>::of(&QComboBox::activated),
                this, &FilteringComboBox::onItemActivated);
    }
    
    // 设置额外的过滤列
    void setFilterRole(int role) {
        m_filterRole = role;
    }
    
    // 设置过滤模式
    void setFilterMode(Qt::MatchFlags mode) {
        m_filterMode = mode;
    }
    
    // 添加项目，支持额外过滤数据
    void addItemWithFilterData(const QString &text, const QVariant &userData = QVariant(),
                              const QStringList &filterKeywords = QStringList()) {
        addItem(text, userData);
        
        if (!filterKeywords.isEmpty()) {
            QModelIndex index = model()->index(count() - 1, 0);
            model()->setData(index, filterKeywords.join(" "), m_filterRole);
        }
    }
    
protected:
    bool eventFilter(QObject *watched, QEvent *event) override {
        if (watched == lineEdit() && event->type() == QEvent::KeyPress) {
            QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
            
            // 处理特殊键
            switch (keyEvent->key()) {
                case Qt::Key_Down:
                    showPopup();
                    return true;
                    
                case Qt::Key_Return:
                case Qt::Key_Enter:
                    // 如果有匹配的过滤结果，选择第一项
                    if (m_filterModel->rowCount() > 0) {
                        QModelIndex index = m_filterModel->index(0, 0);
                        view()->setCurrentIndex(index);
                        view()->activated(index);
                        return true;
                    }
                    break;
                    
                default:
                    break;
            }
        }
        
        return QComboBox::eventFilter(watched, event);
    }
    
    void showPopup() override {
        // 应用当前过滤器然后显示
        filterItems(currentText());
        
        // 设置视图模型和选择
        view()->setCurrentIndex(QModelIndex());
        
        QComboBox::showPopup();
    }
    
private slots:
    void filterItems(const QString &text) {
        // 保存当前选择
        QVariant currentData = currentData();
        
        // 使用自定义过滤模型
        if (m_filterMode == Qt::MatchContains) {
            // 简单包含过滤
            m_filterModel->setFilterFixedString(text);
        } else {
            // 高级过滤
            QRegularExpression regex(
                QRegularExpression::escape(text),
                QRegularExpression::CaseInsensitiveOption);
            m_filterModel->setFilterRegularExpression(regex);
        }
        
        // 如果弹出菜单已显示，更新大小
        if (view()->isVisible()) {
            view()->setMinimumWidth(width());
        }
        
        // 找回之前的选择（如果存在）
        if (!currentData.isNull()) {
            for (int i = 0; i < m_filterModel->rowCount(); ++i) {
                QModelIndex index = m_filterModel->index(i, 0);
                if (m_filterModel->data(index, Qt::UserRole) == currentData) {
                    view()->setCurrentIndex(index);
                    break;
                }
            }
        }
    }
    
    void onItemActivated(int index) {
        if (index >= 0) {
            // 更新编辑框的文本
            lineEdit()->setText(itemText(index));
        }
    }
    
private:
    QSortFilterProxyModel *m_filterModel;
    int m_filterRole = Qt::UserRole + 1;
    Qt::MatchFlags m_filterMode = Qt::MatchContains;
};

// 使用示例
void usageExample() {
    FilteringComboBox *combo = new FilteringComboBox(parentWidget);
    
    // 添加带关键词的项目
    combo->addItemWithFilterData("C++", QVariant(1), 
                                QStringList{"编程", "语言", "开发", "代码"});
    combo->addItemWithFilterData("Python", QVariant(2), 
                               QStringList{"编程", "脚本", "AI", "机器学习"});
    combo->addItemWithFilterData("JavaScript", QVariant(3), 
                               QStringList{"网页", "前端", "开发", "浏览器"});
    
    // 用户可以通过输入"编程"、"AI"等关键词来过滤选项
}
```

## 3. 自定义绘制的ComboBox (带评分显示)

```cpp
// 带评分显示的组合框
class RatingComboBox : public QComboBox {
    Q_OBJECT
public:
    struct RatingItem {
        QString name;
        int rating;        // 1-5评分
        QString category;
        QColor color;      // 与评分相关的颜色
    };
    
    RatingComboBox(QWidget *parent = nullptr) : QComboBox(parent) {
        // 自定义委托
        setItemDelegate(new RatingDelegate(this));
    }
    
    // 添加带评分的项目
    void addRatingItem(const QString &name, int rating, 
                      const QString &category = QString(),
                      const QColor &color = QColor()) {
        RatingItem item;
        item.name = name;
        item.rating = qBound(1, rating, 5);  // 限制在1-5
        item.category = category;
        
        // 如果没有指定颜色，根据评分生成
        if (!color.isValid()) {
            // 从红色(1)到绿色(5)的渐变
            switch (rating) {
                case 1: item.color = QColor(255, 30, 30); break;
                case 2: item.color = QColor(255, 120, 30); break;
                case 3: item.color = QColor(255, 200, 30); break;
                case 4: item.color = QColor(120, 225, 30); break;
                case 5: item.color = QColor(30, 200, 30); break;
                default: item.color = QColor(120, 120, 120); break;
            }
        } else {
            item.color = color;
        }
        
        // 将数据保存为QVariant
        QVariant itemData;
        itemData.setValue(item);
        
        // 添加到组合框
        addItem(name, itemData);
    }
    
    // 获取当前选中项的评分
    int currentRating() const {
        QVariant data = currentData();
        if (data.isValid()) {
            RatingItem item = data.value<RatingItem>();
            return item.rating;
        }
        return 0;
    }
    
    // 委托类，负责自定义绘制
    class RatingDelegate : public QStyledItemDelegate {
    public:
        RatingDelegate(QObject *parent = nullptr) : QStyledItemDelegate(parent) {}
        
        void paint(QPainter *painter, const QStyleOptionViewItem &option,
                  const QModelIndex &index) const override {
            if (!index.isValid())
                return;
                
            // 检查数据是否有效
            QVariant var = index.data(Qt::UserRole);
            if (!var.isValid() || !var.canConvert<RatingItem>()) {
                QStyledItemDelegate::paint(painter, option, index);
                return;
            }
            
            // 获取项目数据
            RatingItem item = var.value<RatingItem>();
            
            // 自定义绘制
            QStyleOptionViewItem opt = option;
            initStyleOption(&opt, index);
            
            // 绘制背景
            if (opt.state & QStyle::State_Selected) {
                painter->fillRect(opt.rect, opt.palette.highlight());
            } else if (opt.state & QStyle::State_MouseOver) {
                painter->fillRect(opt.rect, opt.palette.midlight());
            }
            
            // 绘制文本
            painter->setPen(opt.state & QStyle::State_Selected ? 
                           opt.palette.highlightedText().color() : 
                           opt.palette.text().color());
            
            QRect textRect = opt.rect.adjusted(5, 5, -100, -5);
            painter->drawText(textRect, Qt::AlignLeft | Qt::AlignVCenter, item.name);
            
            // 如果有类别，绘制类别
            if (!item.category.isEmpty()) {
                QFont categoryFont = painter->font();
                categoryFont.setPointSize(categoryFont.pointSize() - 1);
                painter->setFont(categoryFont);
                painter->setPen(QColor(120, 120, 120));
                
                QRect categoryRect = opt.rect.adjusted(5, textRect.height() + 2, -100, -2);
                painter->drawText(categoryRect, Qt::AlignLeft | Qt::AlignVCenter, 
                                 item.category);
                
                painter->setFont(opt.font);
            }
            
            // 绘制评分
            QRect ratingRect = opt.rect.adjusted(opt.rect.width() - 95, 5, -5, -5);
            drawRating(painter, ratingRect, item.rating, item.color);
        }
        
        QSize sizeHint(const QStyleOptionViewItem &option,
                      const QModelIndex &index) const override {
            QSize size = QStyledItemDelegate::sizeHint(option, index);
            
            // 检查是否有类别文本
            QVariant var = index.data(Qt::UserRole);
            if (var.isValid() && var.canConvert<RatingItem>()) {
                RatingItem item = var.value<RatingItem>();
                if (!item.category.isEmpty()) {
                    // 增加高度以容纳类别文本
                    size.setHeight(size.height() + 16);
                }
            }
            
            return size;
        }
        
    private:
        // 绘制星级评分
        void drawRating(QPainter *painter, const QRect &rect, 
                       int rating, const QColor &color) const {
            // 保存当前绘制状态
            painter->save();
            
            // 设置画笔和画刷
            painter->setPen(Qt::NoPen);
            painter->setBrush(color);
            
            // 画星星
            const int starCount = 5;  // 总共5颗星
            const int starSize = rect.height() - 4;
            const int spacing = 2;
            
            for (int i = 0; i < starCount; ++i) {
                QRect starRect(
                    rect.left() + i * (starSize + spacing),
                    rect.top() + 2,
                    starSize,
                    starSize
                );
                
                if (i < rating) {
                    // 填充的星
                    drawStar(painter, starRect, true);
                } else {
                    // 空心的星
                    painter->setBrush(Qt::transparent);
                    painter->setPen(QPen(color, 1));
                    drawStar(painter, starRect, false);
                }
            }
            
            // 恢复绘制状态
            painter->restore();
        }
        
        // 绘制一个星形
        void drawStar(QPainter *painter, const QRect &rect, bool filled) const {
            // 计算星形的点
            QPolygonF star;
            const int centerX = rect.center().x();
            const int centerY = rect.center().y();
            const qreal outerRadius = rect.width() / 2.0;
            const qreal innerRadius = outerRadius * 0.4;
            
            for (int i = 0; i < 5; ++i) {
                // 外点
                qreal angle = i * 2 * 3.141592 / 5 - 3.141592 / 2;
                star << QPointF(
                    centerX + outerRadius * qCos(angle),
                    centerY + outerRadius * qSin(angle)
                );
                
                // 内点
                angle += 3.141592 / 5;
                star << QPointF(
                    centerX + innerRadius * qCos(angle),
                    centerY + innerRadius * qSin(angle)
                );
            }
            
            // 绘制星形
            if (filled) {
                painter->drawPolygon(star);
            } else {
                painter->drawPolyline(star);
            }
        }
    };
};

// 注册自定义类型以供QVariant使用
Q_DECLARE_METATYPE(RatingComboBox::RatingItem)

// 使用示例
void usageExample() {
    RatingComboBox *combo = new RatingComboBox(parentWidget);
    
    // 添加评分项目
    combo->addRatingItem("五星级酒店", 5, "豪华");
    combo->addRatingItem("四星级酒店", 4, "高档");
    combo->addRatingItem("三星级酒店", 3, "舒适");
    combo->addRatingItem("经济型酒店", 2, "实惠");
    combo->addRatingItem("招待所", 1, "基础");
    
    // 显示和处理选择
    connect(combo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            [=](int index) {
                if (index >= 0) {
                    qDebug() << "选择的酒店评分:" << combo->currentRating();
                }
            });
}
```

</details>

# 应用场景总结

QComboBox是一个强大灵活的选择控件，通过本指南的系统化学习，您现在可以根据不同场景选择合适的实现方式：

1. **基础选择需求**：使用标准QComboBox，通过addItem/addItems快速添加选项
2. **大数据集**：采用自定义模型和延迟加载机制，确保UI响应性能
3. **复杂显示需求**：使用自定义委托绘制特殊项目格式（如评分、多行文本）
4. **数据过滤**：实现可编辑模式并自定义过滤逻辑，提升用户查找体验
5. **级联选择**：通过信号槽连接多个QComboBox实现省市选择等场景

记住实践中的关键点：

- 始终在UI线程操作QComboBox
- 谨慎管理自定义数据内存
- 对大量数据应用分批加载策略
- 访问索引前验证有效性
- 获取lineEdit前确认是可编辑模式

这些最佳实践将帮助您构建高性能、用户友好的选择控件，满足各种应用需求。