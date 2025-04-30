# QLayout API参考手册（Qt 5.12.12）

1️⃣【构造体系】 ◆ 核心类结构关系

- QLayout（抽象基类） ▸ QBoxLayout ▹ QHBoxLayout（水平布局） ▹ QVBoxLayout（垂直布局） ▸ QFormLayout（表单布局） ▸ QGridLayout（网格布局） ▸ QStackedLayout（堆叠布局）
- QLayoutItem（抽象基类） ▸ QLayout ▸ QSpacerItem ▸ QWidgetItem

◆ 构造函数特性

- QHBoxLayout(QWidget *parent = nullptr) ▸ 典型用途：创建水平排列的布局管理器 ▸ 内存策略：父子对象自动回收
- QVBoxLayout(QWidget *parent = nullptr) ▸ 典型用途：创建垂直排列的布局管理器 ▸ 内存策略：父子对象自动回收

2️⃣【方法矩阵】 ▨ 核心方法（QLayout基类）

| 方法签名                                                     | 功能描述                     | 使用频率 |
| ------------------------------------------------------------ | ---------------------------- | -------- |
| void addWidget(QWidget *widget)                              | 添加部件到布局中             | ★★★★★    |
| void addLayout(QLayout *layout)                              | 添加子布局到布局中           | ★★★★     |
| void addItem(QLayoutItem *item)                              | 添加布局项目                 | ★★★      |
| int count() const                                            | 获取布局中项目数量           | ★★★      |
| QLayoutItem *itemAt(int index) const                         | 获取指定索引的布局项目       | ★★★      |
| QLayoutItem *takeAt(int index)                               | 移除并返回指定索引的布局项目 | ★★★      |
| void setContentsMargins(int left, int top, int right, int bottom) | 设置布局边距                 | ★★★★     |
| void setSpacing(int spacing)                                 | 设置布局中项目间距           | ★★★★     |
| int spacing() const                                          | 获取布局中项目间距           | ★★★      |
| QSize sizeHint() const                                       | 获取布局推荐大小             | ★★★      |
| QSize minimumSize() const                                    | 获取布局最小大小             | ★★★      |
| void setSizeConstraint(SizeConstraint)                       | 设置布局大小约束             | ★★★      |
| void activate()                                              | 更新布局                     | ★★       |

▨ 特有方法

| 布局类型    | 方法                                                         | 功能描述           | 使用频率 |
| ----------- | ------------------------------------------------------------ | ------------------ | -------- |
| QBoxLayout  | void addStretch(int stretch=0)                               | 添加可伸展空间     | ★★★★     |
| QBoxLayout  | void addSpacing(int size)                                    | 添加固定大小空间   | ★★★★     |
| QBoxLayout  | void insertWidget(int index, QWidget *widget, int stretch=0) | 在指定位置插入部件 | ★★★      |
| QBoxLayout  | void setDirection(Direction)                                 | 设置布局方向       | ★★★      |
| QFormLayout | void addRow(QWidget *label, QWidget *field)                  | 添加标签和字段     | ★★★★★    |
| QFormLayout | void addRow(const QString &labelText, QWidget *field)        | 添加文本标签和字段 | ★★★★★    |
| QGridLayout | void addWidget(QWidget*, int row, int column, int rowSpan=1, int columnSpan=1) | 添加部件到网格     | ★★★★★    |
| QGridLayout | void setRowStretch(int row, int stretch)                     | 设置行伸展因子     | ★★★★     |
| QGridLayout | void setColumnStretch(int column, int stretch)               | 设置列伸展因子     | ★★★★     |

3️⃣【信号与槽】 ◇ 输出信号（无特定信号）

◇ 输入槽

- update() → 触发布局更新
- invalidate() → 使布局无效并触发重新计算

4️⃣【枚举属性】 ◇ 相关属性

| 属性名          | 类型           | 默认值               | 描述         |
| --------------- | -------------- | -------------------- | ------------ |
| sizeConstraint  | SizeConstraint | SetDefaultConstraint | 布局大小约束 |
| spacing         | int            | 视平台而定           | 布局项目间距 |
| contentsMargins | QMargins       | 视平台而定           | 布局边距     |

◇ 相关枚举

- **QLayout::SizeConstraint**

- 作用：控制布局的缩放行为
- 涉及函数：setSizeConstraint(), sizeConstraint()

| 成员                          | 值   | 说明                                           |
| ----------------------------- | ---- | ---------------------------------------------- |
| QLayout::SetDefaultConstraint | 0    | 默认行为，只有当窗口显示时才限制大小           |
| QLayout::SetNoConstraint      | 1    | 不限制窗口大小                                 |
| QLayout::SetMinimumSize       | 2    | 设置窗口的最小大小为布局的最小大小             |
| QLayout::SetFixedSize         | 3    | 设置窗口大小为布局的固定大小                   |
| QLayout::SetMaximumSize       | 4    | 设置窗口的最大大小为布局的最大大小             |
| QLayout::SetMinAndMaxSize     | 5    | 设置窗口的最小和最大大小为布局的最小和最大大小 |

5️⃣【版本适配】 △ Qt 5.12特性

- 改进：布局计算性能优化
- 改进：对高DPI显示的支持增强

△ 跨平台差异

- Windows/macOS/Linux：默认布局边距和间距在不同平台有所不同

6️⃣【代码沙箱】 ◇ 标准用法：

```cpp
// 水平布局示例
QWidget *window = new QWidget();
QHBoxLayout *hLayout = new QHBoxLayout(window);  // 指定父部件

// 添加部件到布局
QPushButton *button1 = new QPushButton("按钮1");
QPushButton *button2 = new QPushButton("按钮2");
QPushButton *button3 = new QPushButton("按钮3");

hLayout->addWidget(button1);
hLayout->addWidget(button2);
hLayout->addWidget(button3);

// 添加弹性空间
hLayout->addStretch(1);

// 调整间距和边距
hLayout->setSpacing(10);
hLayout->setContentsMargins(15, 10, 15, 10);

// 垂直布局示例
QWidget *container = new QWidget();
QVBoxLayout *vLayout = new QVBoxLayout();
vLayout->addWidget(new QLabel("标签1"));
vLayout->addWidget(new QLineEdit());

// 布局嵌套
QHBoxLayout *buttonLayout = new QHBoxLayout();
buttonLayout->addWidget(new QPushButton("确定"));
buttonLayout->addWidget(new QPushButton("取消"));
vLayout->addLayout(buttonLayout);

// 安装布局到部件
container->setLayout(vLayout);
```

◇ 危险操作：

```cpp
// 错误：重复设置布局
QWidget *widget = new QWidget();
QHBoxLayout *layout1 = new QHBoxLayout(widget);
QVBoxLayout *layout2 = new QVBoxLayout();
widget->setLayout(layout2);  // 错误：widget已经有布局了

// 错误：将部件添加到多个布局中
QPushButton *button = new QPushButton("按钮");
QHBoxLayout *layout1 = new QHBoxLayout();
QVBoxLayout *layout2 = new QVBoxLayout();
layout1->addWidget(button);
layout2->addWidget(button);  // 错误：button已在另一个布局中

// 错误：手动设置布局管理的部件的几何属性
QWidget *widget = new QWidget();
QHBoxLayout *layout = new QHBoxLayout(widget);
QPushButton *button = new QPushButton("按钮");
layout->addWidget(button);
button->setGeometry(10, 10, 100, 30);  // 错误：将被布局覆盖

// 正确：使用布局约束而不是直接设置几何属性
button->setMinimumSize(100, 30);
button->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
```





# QSizePolicy API参考手册（Qt 5.12.12）

1️⃣【构造体系】 ◆ 核心构造函数

- QSizePolicy() ▸ 典型用途：创建默认大小策略对象 ▸ 内存策略：值类型，通常作为属性使用
- QSizePolicy(Policy horizontal, Policy vertical, ControlType type = DefaultType) ▸ 典型用途：创建指定水平和垂直策略的大小策略 ▸ 示例：QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed)

2️⃣【方法矩阵】 ▨ 核心方法

| 方法签名                         | 功能描述             | 使用频率 |
| -------------------------------- | -------------------- | -------- |
| Policy horizontalPolicy() const  | 获取水平方向策略     | ★★★★     |
| Policy verticalPolicy() const    | 获取垂直方向策略     | ★★★★     |
| void setHorizontalPolicy(Policy) | 设置水平方向策略     | ★★★★     |
| void setVerticalPolicy(Policy)   | 设置垂直方向策略     | ★★★★     |
| int horizontalStretch() const    | 获取水平方向拉伸因子 | ★★★★     |
| int verticalStretch() const      | 获取垂直方向拉伸因子 | ★★★★     |
| void setHorizontalStretch(int)   | 设置水平方向拉伸因子 | ★★★★     |
| void setVerticalStretch(int)     | 设置垂直方向拉伸因子 | ★★★★     |
| bool hasHeightForWidth() const   | 检查是否宽度决定高度 | ★★       |
| void setHeightForWidth(bool)     | 设置是否宽度决定高度 | ★★       |
| bool hasWidthForHeight() const   | 检查是否高度决定宽度 | ★★       |
| void setWidthForHeight(bool)     | 设置是否高度决定宽度 | ★★       |
| ControlType controlType() const  | 获取控件类型         | ★★       |
| void setControlType(ControlType) | 设置控件类型         | ★★       |
| void transpose()                 | 交换水平和垂直策略   | ★★       |

3️⃣【信号与槽】 ◇ 输出信号

- 无信号，QSizePolicy是值类型而非QObject派生类

◇ 输入槽

- 无槽，QSizePolicy是值类型而非QObject派生类

4️⃣【枚举属性】 ◇ 相关枚举

- **QSizePolicy::Policy**

- 作用：指定部件在布局中的大小行为
- 涉及函数：horizontalPolicy(), verticalPolicy(), setHorizontalPolicy(), setVerticalPolicy()

| 成员                          | 值   | 说明                                                         |
| ----------------------------- | ---- | ------------------------------------------------------------ |
| QSizePolicy::Fixed            | 0    | 部件不可拉伸或压缩，大小保持为sizeHint值                     |
| QSizePolicy::Minimum          | 1    | 部件可拉伸，但不能压缩小于sizeHint (sizeHint~无限)           |
| QSizePolicy::Maximum          | 4    | 部件可压缩，但不能拉伸大于sizeHint (minimumSizeHint~sizeHint) |
| QSizePolicy::Preferred        | 5    | 部件可拉伸和压缩，但sizeHint是最佳大小 (minimumSizeHint~无限) |
| QSizePolicy::Expanding        | 7    | 可拉伸和压缩，但拥有优先扩展权 (minimumSizeHint~无限)        |
| QSizePolicy::MinimumExpanding | 3    | 可拉伸不可压缩小于sizeHint，拥有优先扩展权 (sizeHint~无限)   |
| QSizePolicy::Ignored          | 13   | 忽略sizeHint，可任意拉伸和压缩 (0或minimumSizeHint~无限)     |

- **QSizePolicy::ControlType**

- 作用：指定控件类型，帮助布局更好地处理特定控件
- 涉及函数：controlType(), setControlType()

| 成员                     | 值     | 说明                            |
| ------------------------ | ------ | ------------------------------- |
| QSizePolicy::DefaultType | 0x0001 | 默认控件类型                    |
| QSizePolicy::ButtonBox   | 0x0002 | 按钮盒 (QDialogButtonBox)       |
| QSizePolicy::CheckBox    | 0x0004 | 复选框 (QCheckBox)              |
| QSizePolicy::ComboBox    | 0x0008 | 组合框 (QComboBox)              |
| QSizePolicy::Frame       | 0x0010 | 框架 (QFrame)                   |
| QSizePolicy::GroupBox    | 0x0020 | 分组框 (QGroupBox)              |
| QSizePolicy::Label       | 0x0040 | 标签 (QLabel)                   |
| QSizePolicy::Line        | 0x0080 | 线条 (QFrame shape=HLine/VLine) |
| QSizePolicy::LineEdit    | 0x0100 | 单行文本框 (QLineEdit)          |
| QSizePolicy::PushButton  | 0x0200 | 按钮 (QPushButton)              |
| QSizePolicy::RadioButton | 0x0400 | 单选按钮 (QRadioButton)         |
| QSizePolicy::Slider      | 0x0800 | 滑块 (QSlider)                  |
| QSizePolicy::SpinBox     | 0x1000 | 数值调节框 (QSpinBox)           |
| QSizePolicy::TabWidget   | 0x2000 | 标签页 (QTabWidget)             |
| QSizePolicy::ToolButton  | 0x4000 | 工具按钮 (QToolButton)          |

5️⃣【版本适配】 △ Qt 5.12特性

- 改进：布局计算性能优化

△ 跨平台差异

- 不同平台上控件的默认大小策略可能略有不同

6️⃣【代码沙箱】 ◇ 标准用法：

```cpp
// 基本大小策略设置
QPushButton *button = new QPushButton("按钮");
// 设置为水平方向可扩展，垂直方向固定
button->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

// 设置拉伸因子
QSizePolicy policy = button->sizePolicy();
policy.setHorizontalStretch(2);  // 水平方向拉伸因子为2
policy.setVerticalStretch(0);    // 垂直方向拉伸因子为0
button->setSizePolicy(policy);

// 创建多个按钮并设置不同拉伸因子
QWidget *window = new QWidget();
QHBoxLayout *layout = new QHBoxLayout(window);

QPushButton *btn1 = new QPushButton("按钮1");
QPushButton *btn2 = new QPushButton("按钮2");
QPushButton *btn3 = new QPushButton("按钮3");

layout->addWidget(btn1);
layout->addWidget(btn2);
layout->addWidget(btn3);

// 设置拉伸因子为1:2:3比例
layout->setStretchFactor(btn1, 1);
layout->setStretchFactor(btn2, 2);
layout->setStretchFactor(btn3, 3);

// 设置最小尺寸和最大尺寸
btn1->setMinimumWidth(50);
btn1->setMaximumWidth(200);
```

◇ 危险操作：

```cpp
// 错误：在布局中混合使用拉伸因子和Fixed策略
QPushButton *button = new QPushButton("固定按钮");
button->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
layout->addWidget(button);
layout->setStretchFactor(button, 2);  // 无效，Fixed策略会覆盖拉伸因子

// 错误：先设置拉伸因子再添加部件
QHBoxLayout *layout = new QHBoxLayout();
QPushButton *button = new QPushButton("按钮");
layout->setStretchFactor(button, 2);  // 错误：部件还未添加到布局
layout->addWidget(button);            // 应该先添加，再设置拉伸因子

// 错误：忽略了最小尺寸约束
QLineEdit *edit = new QLineEdit();
// Ignored策略可能导致部件被压缩到看不见
edit->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
// 正确：设置最小尺寸以确保可见
edit->setMinimumSize(50, 20);
```





# QLayout (Size Constraints) API参考手册（Qt 5.12.12）

1️⃣【构造体系】 ◆ 核心概念

- 大小约束 (SizeConstraint) ▸ 典型用途：控制布局窗口的最大和最小大小 ▸ 作用方式：根据子部件的大小策略自动计算主窗口尺寸限制

2️⃣【方法矩阵】 ▨ 核心方法

| 方法签名                               | 功能描述           | 使用频率 |
| -------------------------------------- | ------------------ | -------- |
| void setSizeConstraint(SizeConstraint) | 设置布局的大小约束 | ★★★★     |
| SizeConstraint sizeConstraint() const  | 获取布局的大小约束 | ★★★      |
| QSize minimumSize() const              | 计算布局的最小大小 | ★★★      |
| QSize maximumSize() const              | 计算布局的最大大小 | ★★★      |
| void setMinimumSize(const QSize &)     | 设置布局最小大小   | ★★★      |
| void setMaximumSize(const QSize &)     | 设置布局最大大小   | ★★★      |
| void setMinimumWidth(int)              | 设置布局最小宽度   | ★★★      |
| void setMinimumHeight(int)             | 设置布局最小高度   | ★★★      |
| void setMaximumWidth(int)              | 设置布局最大宽度   | ★★★      |
| void setMaximumHeight(int)             | 设置布局最大高度   | ★★★      |

3️⃣【信号与槽】 ◇ 相关信号

- 无特定信号，布局变化通过窗口resize事件处理

4️⃣【枚举属性】 ◇ 主要属性

| 属性名         | 类型           | 默认值               | 描述                     |
| -------------- | -------------- | -------------------- | ------------------------ |
| sizeConstraint | SizeConstraint | SetDefaultConstraint | 控制布局主窗口的大小约束 |

◇ 相关枚举

- **QLayout::SizeConstraint**

- 作用：控制布局如何约束主窗口大小
- 涉及函数：setSizeConstraint(), sizeConstraint()

| 成员                          | 值   | 说明                                                         |
| ----------------------------- | ---- | ------------------------------------------------------------ |
| QLayout::SetDefaultConstraint | 0    | 默认约束，只有当窗口显示时才限制大小。如果窗口已经显示，则最小大小由布局决定 |
| QLayout::SetNoConstraint      | 1    | 不对窗口大小设置任何约束，窗口可以任意调整大小               |
| QLayout::SetMinimumSize       | 2    | 设置窗口的最小大小为布局计算的最小大小，但不限制最大大小     |
| QLayout::SetFixedSize         | 3    | 设置窗口的最小和最大大小都为布局计算的大小，窗口不能调整大小 |
| QLayout::SetMaximumSize       | 4    | 设置窗口的最大大小为布局计算的最大大小，但不限制最小大小     |
| QLayout::SetMinAndMaxSize     | 5    | 设置窗口的最小和最大大小为布局计算的最小和最大大小           |

5️⃣【版本适配】 △ Qt 5.12特性

- 无特定新功能，核心行为保持稳定

△ 跨平台差异

- Windows/macOS/Linux：默认大小约束行为在不同平台保持一致

6️⃣【代码沙箱】 ◇ 标准用法：

```cpp
// 创建窗口并设置固定大小
QWidget *window = new QWidget();
QVBoxLayout *layout = new QVBoxLayout(window);

// 添加一些部件
QPushButton *button1 = new QPushButton("按钮1");
QPushButton *button2 = new QPushButton("按钮2");
QPushButton *button3 = new QPushButton("按钮3");

// 设置部件的大小策略
button1->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
button2->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
button3->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

layout->addWidget(button1);
layout->addWidget(button2);
layout->addWidget(button3);

// 设置大小约束为固定大小
layout->setSizeConstraint(QLayout::SetFixedSize);
window->show();

// 设置最小大小窗口
QWidget *minSizeWindow = new QWidget();
QHBoxLayout *hLayout = new QHBoxLayout(minSizeWindow);

// 添加部件
QLineEdit *lineEdit = new QLineEdit();
QPushButton *okButton = new QPushButton("确定");

hLayout->addWidget(lineEdit);
hLayout->addWidget(okButton);

// 设置大小约束为最小大小
hLayout->setSizeConstraint(QLayout::SetMinimumSize);
minSizeWindow->show();
```

◇ 危险操作：

```cpp
// 错误：布局已安装后改变顶层窗口大小
QWidget *window = new QWidget();
QVBoxLayout *layout = new QVBoxLayout(window);
layout->addWidget(new QPushButton("按钮"));
layout->setSizeConstraint(QLayout::SetFixedSize);

// 错误：手动设置窗口大小将被布局覆盖
window->resize(200, 100);  // 无效，会被SetFixedSize约束覆盖

// 错误：对嵌套布局错误理解大小约束的继承关系
QWidget *window = new QWidget();
QVBoxLayout *mainLayout = new QVBoxLayout(window);
QHBoxLayout *subLayout = new QHBoxLayout();

mainLayout->addLayout(subLayout);
// 子布局的大小约束不控制窗口大小，只有主布局的约束有效
subLayout->setSizeConstraint(QLayout::SetFixedSize);  // 无效，应设置在mainLayout上

// 正确：在主布局上设置大小约束
mainLayout->setSizeConstraint(QLayout::SetFixedSize);
```



# QLayout (内容边距和间距) API参考手册（Qt 5.12.12）

1️⃣【构造体系】 ◆ 核心概念

- 内容边距 (ContentsMargins) ▸ 典型用途：控制布局四周的空白边距 ▸ 内存策略：由布局管理器管理
- 间距 (Spacing) ▸ 典型用途：控制布局中各部件之间的距离 ▸ 特点：不占用布局索引位置，仅是距离值
- QSpacerItem ▸ 典型用途：创建自定义可拉伸或固定间距 ▸ 特点：作为布局项目占用布局索引位置

◆ 构造函数

- QSpacerItem(int width, int height, QSizePolicy::Policy hPolicy = QSizePolicy::Minimum, QSizePolicy::Policy vPolicy = QSizePolicy::Minimum) ▸ 典型用途：创建指定大小和策略的间距项 ▸ 示例：QSpacerItem *spacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding)

2️⃣【方法矩阵】 ▨ 内容边距方法（QLayout类）

| 方法签名                                                     | 功能描述                   | 使用频率 |
| ------------------------------------------------------------ | -------------------------- | -------- |
| QMargins contentsMargins() const                             | 获取布局的内容边距         | ★★★★     |
| void getContentsMargins(int *left, int *top, int *right, int *bottom) const | 获取四个方向的边距值       | ★★★      |
| void setContentsMargins(int left, int top, int right, int bottom) | 设置四个方向的边距值       | ★★★★★    |
| void setContentsMargins(const QMargins &margins)             | 使用QMargins对象设置边距值 | ★★★★     |

▨ 间距方法（QLayout类）

| 方法签名                     | 功能描述           | 使用频率 |
| ---------------------------- | ------------------ | -------- |
| int spacing() const          | 获取布局中的间距值 | ★★★★     |
| void setSpacing(int spacing) | 设置布局中的间距值 | ★★★★★    |

▨ QSpacerItem方法

| 方法签名                                                     | 功能描述           | 使用频率 |
| ------------------------------------------------------------ | ------------------ | -------- |
| void changeSize(int w, int h, QSizePolicy::Policy hPolicy = QSizePolicy::Minimum, QSizePolicy::Policy vPolicy = QSizePolicy::Minimum) | 修改间距大小和策略 | ★★★      |
| QSizePolicy sizePolicy() const                               | 获取间距的大小策略 | ★★       |

▨ 添加间距方法（QBoxLayout类）

| 方法签名                                                  | 功能描述                   | 使用频率 |
| --------------------------------------------------------- | -------------------------- | -------- |
| void addSpacing(int size)                                 | 添加固定大小的不可拉伸间距 | ★★★★     |
| void insertSpacing(int index, int size)                   | 在指定位置插入固定大小间距 | ★★★      |
| void addStretch(int stretch = 0)                          | 添加可拉伸间距             | ★★★★★    |
| void insertStretch(int index, int stretch = 0)            | 在指定位置插入可拉伸间距   | ★★★★     |
| void addSpacerItem(QSpacerItem *spacerItem)               | 添加自定义间距项           | ★★★      |
| void insertSpacerItem(int index, QSpacerItem *spacerItem) | 在指定位置插入自定义间距项 | ★★★      |

3️⃣【信号与槽】 ◇ 输出信号

- 无特定信号，布局变化通过窗口大小事件处理

4️⃣【枚举属性】 ◇ 主要属性

| 属性名          | 类型     | 默认值         | 描述                   |
| --------------- | -------- | -------------- | ---------------------- |
| contentsMargins | QMargins | 平台相关(11px) | 布局的内容边距         |
| spacing         | int      | 平台相关(6px)  | 布局中各部件之间的间距 |

◇ 相关说明

- contentsMargins：默认由样式提供，大多数平台上默认为11像素
- spacing：默认由样式提供，若使用setHorizontalSpacing和setVerticalSpacing设置了不同值，spacing()返回-1

5️⃣【版本适配】 △ Qt 5.12特性

- 无特定新功能，核心行为保持稳定

△ 跨平台差异

- Windows/macOS/Linux：默认边距和间距值可能因平台和系统主题而异

6️⃣【代码沙箱】 ◇ 标准用法：

```cpp
// 设置内容边距示例
QWidget *window = new QWidget();
QVBoxLayout *layout = new QVBoxLayout(window);

// 设置布局的内容边距为左20、上10、右20、下30像素
layout->setContentsMargins(20, 10, 20, 30);

// 使用QMargins设置边距
QMargins margins(15, 15, 15, 15);
layout->setContentsMargins(margins);

// 设置部件之间的间距为10像素
layout->setSpacing(10);

// 添加部件
QPushButton *button1 = new QPushButton("按钮1");
QPushButton *button2 = new QPushButton("按钮2");
layout->addWidget(button1);
layout->addWidget(button2);

// 使用QHBoxLayout添加不同类型的间距
QWidget *container = new QWidget();
QHBoxLayout *hLayout = new QHBoxLayout(container);

QPushButton *leftBtn = new QPushButton("左侧");
QPushButton *rightBtn = new QPushButton("右侧");

hLayout->addWidget(leftBtn);
hLayout->addSpacing(20);     // 添加20像素固定间距
hLayout->addStretch(1);      // 添加可拉伸间距
hLayout->addWidget(rightBtn);

// 使用QSpacerItem自定义间距
QSpacerItem *customSpacer = new QSpacerItem(
    40,                             // 宽度
    20,                             // 高度
    QSizePolicy::Minimum,          // 水平策略
    QSizePolicy::Expanding          // 垂直策略
);
layout->addSpacerItem(customSpacer);
```

◇ 危险操作：

```cpp
// 错误：混淆了spacing属性和QSpacerItem
QHBoxLayout *layout = new QHBoxLayout();
// 设置间距为10像素
layout->setSpacing(10);
// 错误理解：认为不需要添加QSpacerItem，因为已经设置了spacing
// 实际上spacing只影响已添加的部件之间的距离，不会在特定位置添加额外空间

// 错误：在QGridLayout中使用spacing()函数获取水平和垂直间距
QGridLayout *gridLayout = new QGridLayout();
gridLayout->setHorizontalSpacing(5);
gridLayout->setVerticalSpacing(10);
int spacing = gridLayout->spacing();  // 返回-1，因为水平和垂直间距不同

// 错误：在部件显示后更改边距不会立即生效
QWidget *window = new QWidget();
QVBoxLayout *layout = new QVBoxLayout(window);
window->show();
// 需要调用布局的activate()方法使变更生效
layout->setContentsMargins(20, 20, 20, 20);
layout->activate();
```







# QBoxLayout API参考手册（Qt 5.12.12）

1️⃣【构造体系】 ◆ 核心构造函数

- QBoxLayout(QBoxLayout::Direction direction, QWidget *parent = nullptr) ▸ 典型用途：创建指定方向的盒式布局 ▸ 内存策略：父子对象自动回收

◆ 继承关系

- 基类：QLayout
- 子类：QHBoxLayout、QVBoxLayout ▸ 提示：通常直接使用子类更方便

2️⃣【方法矩阵】 ▨ 核心方法

| 方法签名                                                     | 功能描述           | 使用频率 |
| ------------------------------------------------------------ | ------------------ | -------- |
| void addWidget(QWidget*, int stretch = 0, Qt::Alignment = 0) | 添加控件到布局末尾 | ★★★★★    |
| void addLayout(QLayout*, int stretch = 0)                    | 添加子布局到末尾   | ★★★★     |
| void addStretch(int stretch = 1)                             | 添加弹性空白区域   | ★★★★     |
| void addSpacing(int size)                                    | 添加固定大小空白   | ★★★      |
| void insertWidget(int index, QWidget*, int stretch = 0)      | 在指定位置插入控件 | ★★★      |
| void setStretchFactor(QWidget*, int stretch)                 | 设置控件的伸缩因子 | ★★★★     |
| void setSpacing(int spacing)                                 | 设置布局内元素间距 | ★★★      |
| void setDirection(Direction)                                 | 设置布局方向       | ★★       |
| Direction direction() const                                  | 获取当前布局方向   | ★★       |

▨ 重写方法

| 方法                            | 触发场景     | 必须调用基类 |
| ------------------------------- | ------------ | ------------ |
| QSize sizeHint() const          | 获取首选大小 | 否           |
| QSize minimumSize() const       | 获取最小大小 | 否           |
| void setGeometry(const QRect &) | 布局几何变更 | 是           |
| int count() const               | 布局项目数量 | 否           |

3️⃣【信号与槽】 ◇ 输出信号

- 无专有信号 (继承自QLayout)

◇ 输入槽

- 无专有槽 (继承自QLayout)

4️⃣【枚举属性】 ◇ 相关枚举

- **QBoxLayout::Direction**

- 作用：指定盒式布局的方向

- 涉及函数: setDirection(), direction(), 构造函数

  | 成员 | 值   | 说明 |
  | ---- | ---- | ---- |
  |      |      |      |

  | ---------------- | ----- | ---------------------------- |
  | ---------------- | ----- | ---------------------------- |
  |                  |       |                              |

  | LeftToRight | 0    | 水平布局，从左到右排列 |
  | ----------- | ---- | ---------------------- |
  |             |      |                        |

  | RightToLeft | 1    | 水平布局，从右到左排列 |
  | ----------- | ---- | ---------------------- |
  |             |      |                        |

  | TopToBottom | 2    | 垂直布局，从上到下排列 |
  | ----------- | ---- | ---------------------- |
  |             |      |                        |

  | BottomToTop | 3    | 垂直布局，从下到上排列 |
  | ----------- | ---- | ---------------------- |
  |             |      |                        |

  

◇ 相关属性

| 属性名    | 类型      | 默认值     | 描述               |
| --------- | --------- | ---------- | ------------------ |
| spacing   | int       | 取决于风格 | 布局项目之间的间距 |
| direction | Direction | -          | 布局的排列方向     |

5️⃣【版本适配】 △ Qt 5.12特性

- 保持稳定，无重大API变更

△ 跨平台差异

- 默认间距在不同平台有细微差异
- 在RTL(从右到左)语言环境中，LeftToRight可能自动变为RightToLeft

6️⃣【代码沙箱】 ◇ 标准用法：

```cpp
// 1. 垂直布局示例
QBoxLayout *vbox = new QBoxLayout(QBoxLayout::TopToBottom, this);
vbox->addWidget(new QPushButton("上"));
vbox->addWidget(new QPushButton("中"));
vbox->addStretch(1); // 下部弹性空间
vbox->addWidget(new QPushButton("下"));

// 2. 常见用法是直接使用子类
QHBoxLayout *hbox = new QHBoxLayout();
hbox->addWidget(new QLabel("姓名:"));
hbox->addWidget(new QLineEdit());
vbox->addLayout(hbox); // 布局嵌套
```

◇ 危险操作：

```cpp
// 错误：布局已设置到窗口后动态改变方向
myWidget->setLayout(vbox);
vbox->setDirection(QBoxLayout::LeftToRight); // 可能导致布局异常

// 正确：布局方向应在应用到窗口前设置
QBoxLayout *layout = new QBoxLayout(QBoxLayout::LeftToRight);
// 添加控件...
myWidget->setLayout(layout);
```

◇ 高级用法：

```cpp
// 不均匀伸缩分配 - 创建2:1:1比例布局
QBoxLayout *layout = new QBoxLayout(QBoxLayout::LeftToRight);
layout->addWidget(new QPushButton("A"), 2); // 占2/4
layout->addWidget(new QPushButton("B"), 1); // 占1/4
layout->addWidget(new QPushButton("C"), 1); // 占1/4
```



# QGridLayout API参考手册（Qt 5.12.12）

1️⃣【构造体系】 ◆ 核心构造函数

- QGridLayout(QWidget *parent = nullptr) ▸ 典型用途：创建网格布局并关联父窗口 ▸ 内存策略：父子对象自动回收
- QGridLayout() ▸ 创建独立网格布局，稍后可通过setParent()或QWidget::setLayout()关联 ▸ 注意：未设置到窗口的布局不会自动释放

◆ 继承关系

- 基类：QLayout
- 同级：QBoxLayout, QFormLayout, QStackedLayout

2️⃣【方法矩阵】 ▨ 核心方法

| 方法签名                                                     | 功能描述                 | 使用频率 |
| ------------------------------------------------------------ | ------------------------ | -------- |
| void addWidget(QWidget*, int row, int col, int rowSpan=1, int colSpan=1, Qt::Alignment=0) | 添加控件到指定网格位置   | ★★★★★    |
| void addLayout(QLayout*, int row, int col, int rowSpan=1, int colSpan=1, Qt::Alignment=0) | 添加子布局到指定网格位置 | ★★★★     |
| void setRowStretch(int row, int stretch)                     | 设置行的伸缩因子         | ★★★★     |
| void setColumnStretch(int column, int stretch)               | 设置列的伸缩因子         | ★★★★     |
| void setRowMinimumHeight(int row, int minSize)               | 设置行的最小高度         | ★★★      |
| void setColumnMinimumWidth(int column, int minSize)          | 设置列的最小宽度         | ★★★      |
| void setSpacing(int spacing)                                 | 设置水平和垂直间距       | ★★★      |
| void setHorizontalSpacing(int spacing)                       | 设置水平间距             | ★★       |
| void setVerticalSpacing(int spacing)                         | 设置垂直间距             | ★★       |
| QLayoutItem *itemAtPosition(int row, int col) const          | 获取指定位置的布局项     | ★★       |
| int rowCount() const                                         | 获取行数                 | ★★       |
| int columnCount() const                                      | 获取列数                 | ★★       |

▨ 重写方法

| 方法                                 | 触发场景         | 必须调用基类 |
| ------------------------------------ | ---------------- | ------------ |
| QSize sizeHint() const               | 获取首选大小     | 否           |
| QSize minimumSize() const            | 获取最小大小     | 否           |
| void setGeometry(const QRect &)      | 布局几何变更     | 是           |
| int count() const                    | 布局项目数量     | 否           |
| QLayoutItem *itemAt(int index) const | 按索引访问布局项 | 否           |
| QLayoutItem *takeAt(int index)       | 移除并返回布局项 | 否           |

3️⃣【信号与槽】 ◇ 输出信号

- 无专有信号 (继承自QLayout)

◇ 输入槽

- 无专有槽 (继承自QLayout)

4️⃣【枚举属性】 ◇ 相关属性

| 属性名            | 类型 | 默认值     | 描述                 |
| ----------------- | ---- | ---------- | -------------------- |
| horizontalSpacing | int  | 取决于风格 | 水平方向的网格线间距 |
| verticalSpacing   | int  | 取决于风格 | 垂直方向的网格线间距 |

◇ 相关枚举

- **Qt::Alignment** (用于控制单元格内控件的对齐方式)

- 作用：控制网格单元格内控件的对齐方式
- 涉及函数: addWidget(), addLayout() | 成员          | 值          | 说明                | |---------------|-------------|---------------------| | Qt::AlignLeft | 0x0001      | 左对齐              | | Qt::AlignRight| 0x0002      | 右对齐              | | Qt::AlignHCenter| 0x0004    | 水平居中            | | Qt::AlignTop  | 0x0020      | 顶部对齐            | | Qt::AlignBottom| 0x0040     | 底部对齐            | | Qt::AlignVCenter| 0x0080    | 垂直居中            | | Qt::AlignCenter| AlignVCenter | AlignHCenter | 居中对齐 |

5️⃣【版本适配】 △ Qt 5.12特性

- 保持稳定，无重大API变更

△ 跨平台差异

- 默认间距值在不同平台有细微差异
- 使用高DPI显示时网格间隙可能需要调整

6️⃣【代码沙箱】 ◇ 标准用法：

```cpp
// 创建表单布局
QGridLayout *grid = new QGridLayout(this);

// 添加标签和输入框
grid->addWidget(new QLabel("姓名:"), 0, 0);
grid->addWidget(new QLineEdit(), 0, 1);
grid->addWidget(new QLabel("年龄:"), 1, 0);
grid->addWidget(new QLineEdit(), 1, 1);

// 添加跨多列的控件
QPushButton *submitBtn = new QPushButton("提交");
grid->addWidget(submitBtn, 2, 0, 1, 2); // 跨2列

// 设置第二列的伸缩因子，使其获得更多空间
grid->setColumnStretch(1, 1);
```

◇ 高级用法：

```cpp
// 创建复杂的网格布局
QGridLayout *grid = new QGridLayout(this);

// 添加不同的跨行跨列项
QTextEdit *editor = new QTextEdit();
grid->addWidget(editor, 0, 1, 3, 1); // 跨3行

// 在左侧添加一组控件
grid->addWidget(new QPushButton("按钮1"), 0, 0);
grid->addWidget(new QPushButton("按钮2"), 1, 0);
grid->addWidget(new QPushButton("按钮3"), 2, 0);

// 底部添加状态栏，跨越所有列
QStatusBar *status = new QStatusBar();
grid->addWidget(status, 3, 0, 1, 2); // 跨2列

// 设置行列最小尺寸
grid->setRowMinimumHeight(0, 100);
grid->setColumnMinimumWidth(1, 300);
```

◇ 危险操作：

```cpp
// 错误：在布局应用后，尝试修改已被占用的网格位置
grid->setLayout(parentWidget);
grid->addWidget(new QPushButton("冲突"), 0, 0); // 与现有控件位置冲突

// 错误：未检查行列边界，可能导致稀疏矩阵
for(int i=0; i<10; i++)
    grid->addWidget(new QLabel("标签"), i, i); // 形成对角线，浪费空间

// 正确：应当紧凑排列或明确使用跨行跨列功能
```



# QFormLayout API参考手册（Qt 5.12.12）

1️⃣【构造体系】 ◆ 核心构造函数

- QFormLayout(QWidget *parent = nullptr) ▸ 典型用途：创建表单布局并关联父窗口 ▸ 内存策略：父子对象自动回收
- QFormLayout() ▸ 创建独立表单布局，稍后可通过QWidget::setLayout()关联

◆ 继承关系

- 基类：QLayout
- 同级：QBoxLayout, QGridLayout, QStackedLayout

2️⃣【方法矩阵】 ▨ 核心方法

| 方法签名                                                | 功能描述                     | 使用频率 |
| ------------------------------------------------------- | ---------------------------- | -------- |
| void addRow(QWidget *label, QWidget *field)             | 添加标签和字段到表单         | ★★★★★    |
| void addRow(QWidget *label, QLayout *field)             | 添加标签和字段布局到表单     | ★★★★     |
| void addRow(const QString &labelText, QWidget *field)   | 添加文本标签和字段到表单     | ★★★★★    |
| void addRow(const QString &labelText, QLayout *field)   | 添加文本标签和字段布局到表单 | ★★★★     |
| void addRow(QWidget *widget)                            | 添加跨越整行的控件           | ★★★★     |
| void addRow(QLayout *layout)                            | 添加跨越整行的布局           | ★★★      |
| void insertRow(int row, QWidget *label, QWidget *field) | 在指定位置插入行             | ★★★      |
| void setRowWrapPolicy(RowWrapPolicy policy)             | 设置标签换行策略             | ★★       |
| void setFieldGrowthPolicy(FieldGrowthPolicy policy)     | 设置字段增长策略             | ★★★      |
| void setLabelAlignment(Qt::Alignment alignment)         | 设置标签对齐方式             | ★★★      |
| void setFormAlignment(Qt::Alignment alignment)          | 设置整个表单对齐方式         | ★★       |
| int count() const                                       | 获取行数                     | ★★       |
| QWidget *labelForField(QWidget *field) const            | 获取字段对应的标签控件       | ★★       |
| void setItem(int row, ItemRole role, QLayoutItem *item) | 设置指定位置的布局项         | ★★       |

▨ 重写方法

| 方法                                 | 触发场景         | 必须调用基类 |
| ------------------------------------ | ---------------- | ------------ |
| QSize sizeHint() const               | 获取首选大小     | 否           |
| QSize minimumSize() const            | 获取最小大小     | 否           |
| void setGeometry(const QRect &)      | 布局几何变更     | 是           |
| QLayoutItem *itemAt(int index) const | 按索引访问布局项 | 否           |
| QLayoutItem *takeAt(int index)       | 移除并返回布局项 | 否           |

3️⃣【信号与槽】 ◇ 输出信号

- 无专有信号 (继承自QLayout)

◇ 输入槽

- 无专有槽 (继承自QLayout)

4️⃣【枚举属性】 ◇ 相关枚举

- **QFormLayout::RowWrapPolicy**

- 作用：定义当标签和字段在窗口缩小时如何换行
- 涉及函数: setRowWrapPolicy(), rowWrapPolicy() | 成员                     | 值  | 说明                       | |--------------------------|-----|----------------------------| | DontWrapRows             | 0   | 标签和字段总是在同一行     | | WrapLongRows             | 1   | 仅在空间不足时将字段放到下一行 | | WrapAllRows              | 2   | 标签和字段总是位于不同行   |

- **QFormLayout::FieldGrowthPolicy**

- 作用：控制字段在水平方向上如何增长
- 涉及函数: setFieldGrowthPolicy(), fieldGrowthPolicy() | 成员                      | 值  | 说明                       | |---------------------------|-----|----------------------------| | FieldsStayAtSizeHint      | 0   | 字段不扩展                 | | ExpandingFieldsGrow       | 1   | 具有扩展策略的字段会增长   | | AllNonFixedFieldsGrow     | 2   | 所有非固定大小策略的字段会增长 |

- **QFormLayout::ItemRole**

- 作用：定义表单中项目的角色
- 涉及函数: itemAt(), setItem(), getItemPosition() | 成员                      | 值  | 说明                       | |---------------------------|-----|----------------------------| | LabelRole                 | 0   | 标签角色                   | | FieldRole                 | 1   | 字段角色                   | | SpanningRole              | 2   | 跨越整行的角色             |

◇ 相关属性

| 属性名            | 类型              | 默认值                        | 描述         |
| ----------------- | ----------------- | ----------------------------- | ------------ |
| fieldGrowthPolicy | FieldGrowthPolicy | ExpandingFieldsGrow           | 字段增长策略 |
| rowWrapPolicy     | RowWrapPolicy     | DontWrapRows                  | 行换行策略   |
| labelAlignment    | Qt::Alignment     | Qt::AlignLeft                 | 标签对齐方式 |
| formAlignment     | Qt::Alignment     | Qt::AlignLeft \| Qt::AlignTop | 表单对齐方式 |
| horizontalSpacing | int               | 取决于风格                    | 水平间距     |
| verticalSpacing   | int               | 取决于风格                    | 垂直间距     |

5️⃣【版本适配】 △ Qt 5.12特性

- 保持稳定，无重大API变更

△ 跨平台差异

- 在不同平台上标签对齐方式可能有所不同
- 在macOS上默认更符合平台UI指南
- 在RTL(从右到左)语言环境中，标签和字段位置会自动交换

6️⃣【代码沙箱】 ◇ 标准用法：

```cpp
// 创建一个表单布局
QFormLayout *form = new QFormLayout(this);

// 添加标签-字段对
form->addRow("姓名:", new QLineEdit());
form->addRow("年龄:", new QSpinBox());
form->addRow("性别:", new QComboBox());

// 添加子布局作为字段
QHBoxLayout *buttonLayout = new QHBoxLayout();
buttonLayout->addWidget(new QPushButton("确定"));
buttonLayout->addWidget(new QPushButton("取消"));
form->addRow("操作:", buttonLayout);

// 添加跨越整行的控件
form->addRow(new QCheckBox("接受服务条款"));
```

◇ 高级用法：

```cpp
// 创建表单并设置属性
QFormLayout *form = new QFormLayout(this);

// 设置字段增长策略
form->setFieldGrowthPolicy(QFormLayout::AllNonFixedFieldsGrow);

// 设置标签对齐方式 - 右对齐标签
form->setLabelAlignment(Qt::AlignRight | Qt::AlignVCenter);

// 在指定位置插入行
form->insertRow(0, "优先级:", new QComboBox());

// 获取字段对应的标签
QLineEdit *nameEdit = new QLineEdit();
form->addRow("用户名:", nameEdit);
QLabel *nameLabel = qobject_cast<QLabel*>(form->labelForField(nameEdit));
nameLabel->setBuddy(nameEdit); // 设置助记键
```

◇ 危险操作：

```cpp
// 错误：重复添加同一个控件
QLineEdit *edit = new QLineEdit();
form->addRow("名称:", edit);
form->addRow("描述:", edit); // 一个控件只能有一个父布局

// 错误：混合使用表单布局和网格布局API
form->addWidget(new QPushButton("错误"), 1, 0); // QFormLayout不支持网格坐标

// 错误：不检查行索引范围
form->insertRow(99, "超出范围:", new QLineEdit()); // 可能导致不可预期行为

// 正确：使用表单布局专用方法
form->addRow("正确:", new QLineEdit());
```



# QStackedLayout API参考手册（Qt 5.12.12）

1️⃣【构造体系】 ◆ 核心构造函数

- QStackedLayout() ▸ 典型用途：创建独立的堆叠布局 ▸ 内存策略：后续需手动添加到父布局
- QStackedLayout(QWidget *parent) ▸ 典型用途：创建直接绑定到父窗口的堆叠布局 ▸ 示例：QStackedLayout layout(parentWidget);
- QStackedLayout(QLayout *parentLayout) ▸ 典型用途：作为子布局添加到现有布局中 ▸ 内存策略：跟随父布局生命周期

2️⃣【方法矩阵】 ▨ 核心方法

| 方法签名                        | 功能描述               | 使用频率 |
| ------------------------------- | ---------------------- | -------- |
| int addWidget(QWidget*)         | 添加小部件并返回索引   | ★★★★★    |
| int insertWidget(int, QWidget*) | 在指定索引处插入小部件 | ★★★      |
| void setCurrentIndex(int)       | 设置当前可见窗口索引   | ★★★★★    |
| void setCurrentWidget(QWidget*) | 设置当前可见窗口       | ★★★★★    |
| int currentIndex() const        | 获取当前窗口索引       | ★★★★     |
| QWidget *currentWidget() const  | 获取当前显示的窗口指针 | ★★★★     |
| int count() const               | 获取布局中窗口总数     | ★★★      |

▨ 重写方法

| 方法                                   | 触发场景                   | 必须调用基类 |
| -------------------------------------- | -------------------------- | ------------ |
| void addItem(QLayoutItem*)             | 添加布局项(通常不直接调用) | 否           |
| QLayoutItem *itemAt(int) const         | 访问指定索引的布局项       | 否           |
| QLayoutItem *takeAt(int)               | 移除并返回指定索引的布局项 | 否           |
| Qt::Orientations expandingDirections() | 获取布局扩展方向           | 否           |
| QSize sizeHint() const                 | 获取首选大小               | 否           |
| QSize minimumSize() const              | 获取最小大小               | 否           |

3️⃣【信号与槽】 ◇ 输出信号

- currentChanged(int index) → 当前显示窗口变化时发射
- widgetRemoved(int index) → 窗口被移除时发射

◇ 输入槽

- setCurrentIndex(int index) → 设置当前显示的窗口索引
- setCurrentWidget(QWidget *widget) → 设置当前显示的窗口

4️⃣【枚举属性】 ◇ 相关属性

| 属性名       | 类型 | 默认值 | 描述                   |
| ------------ | ---- | ------ | ---------------------- |
| currentIndex | int  | -1     | 当前可见窗口的索引     |
| count        | int  | 0      | 布局中的窗口数量(只读) |

◇ 相关枚举 QStackedLayout没有特定的枚举类型，但使用Qt标准枚举。

5️⃣【版本适配】 △ Qt 5.12特性

- 优化：改进了堆叠布局的性能和内存使用
- 兼容：保持了与Qt 4.x版本的API兼容性

△ 跨平台差异

- 所有平台下行为一致，没有特定平台差异
- 高DPI支持：窗口缩放时正确处理各页面内容

6️⃣【代码沙箱】 ◇ 标准用法：

```cpp
// 创建堆叠布局
QStackedLayout *stackedLayout = new QStackedLayout;
QWidget *page1 = new QWidget;
QWidget *page2 = new QWidget;

// 添加页面
stackedLayout->addWidget(page1);
stackedLayout->addWidget(page2);

// 设置当前页面
stackedLayout->setCurrentIndex(0);  // 显示第一页

// 连接信号
connect(stackedLayout, &QStackedLayout::currentChanged,
        this, &MyWidget::handlePageChange);

// 添加到主布局
mainLayout->addLayout(stackedLayout);
```

◇ 危险操作：

```cpp
// 错误：使用越界索引
stackedLayout->setCurrentIndex(999);  // 超出范围，不会崩溃但无效果

// 错误：对已删除的窗口操作
QWidget *widget = new QWidget;
int index = stackedLayout->addWidget(widget);
delete widget;  // 先删除窗口
stackedLayout->setCurrentIndex(index);  // 可能导致异常

// 正确：移除窗口时使用takeAt
QLayoutItem *item = stackedLayout->takeAt(index); // 先从布局中移除
delete item->widget();  // 再删除窗口
delete item;  // 最后删除布局项
```





# QStackedWidget API参考手册（Qt 5.12.12）

1️⃣【构造体系】 ◆ 核心构造函数

- QStackedWidget(QWidget *parent = nullptr) ▸ 典型用途：创建可堆叠多个页面的容器组件 ▸ 内存策略：父子对象自动回收 ▸ 特点：继承自QFrame，自带布局管理

◆ 工厂方法

- 无内置工厂方法，通常直接实例化

2️⃣【方法矩阵】 ▨ 核心方法

| 方法签名                        | 功能描述               | 使用频率 |
| ------------------------------- | ---------------------- | -------- |
| int addWidget(QWidget*)         | 添加小部件并返回索引   | ★★★★★    |
| int insertWidget(int, QWidget*) | 在指定索引处插入小部件 | ★★★      |
| void removeWidget(QWidget*)     | 移除指定小部件         | ★★★★     |
| void setCurrentIndex(int)       | 设置当前可见窗口索引   | ★★★★★    |
| void setCurrentWidget(QWidget*) | 设置当前可见窗口       | ★★★★★    |
| int currentIndex() const        | 获取当前窗口索引       | ★★★★     |
| QWidget *currentWidget() const  | 获取当前显示的窗口指针 | ★★★★     |
| int count() const               | 获取堆栈中窗口总数     | ★★★      |
| QWidget *widget(int) const      | 获取指定索引的窗口     | ★★★★     |

▨ 重写方法

| 方法                            | 触发场景     | 必须调用基类 |
| ------------------------------- | ------------ | ------------ |
| bool event(QEvent*)             | 处理内部事件 | 否           |
| void paintEvent(QPaintEvent*)   | 绘制外观     | 是           |
| void resizeEvent(QResizeEvent*) | 大小改变时   | 否           |

3️⃣【信号与槽】 ◇ 输出信号

- currentChanged(int index) → 当前显示窗口变化时发射
- widgetRemoved(int index) → 窗口被移除时发射

◇ 输入槽

- setCurrentIndex(int index) → 设置当前显示的窗口索引
- setCurrentWidget(QWidget *widget) → 设置当前显示的窗口

4️⃣【枚举属性】 ◇ 相关属性

| 属性名       | 类型 | 默认值 | 描述                   |
| ------------ | ---- | ------ | ---------------------- |
| currentIndex | int  | -1     | 当前可见窗口的索引     |
| count        | int  | 0      | 堆栈中的窗口数量(只读) |

◇ 相关枚举 QStackedWidget没有特定的枚举类型，使用Qt标准枚举。

5️⃣【版本适配】 △ Qt 5.12特性

- 性能：内部优化了小部件切换的渲染性能
- 稳定性：修复了前几个版本中的一些内存管理问题

△ 跨平台差异

- 所有平台下行为一致，不存在平台特定差异
- 样式：继承自QFrame，可通过样式表自定义边框样式

6️⃣【代码沙箱】 ◇ 标准用法：

```cpp
// 创建堆栈窗口容器
QStackedWidget *stackedWidget = new QStackedWidget(this);

// 创建页面
QWidget *page1 = new QWidget;
QWidget *page2 = new QWidget;
// 各页面添加各自的布局和控件
page1->setLayout(new QVBoxLayout);
page2->setLayout(new QVBoxLayout);

// 添加到堆栈中
stackedWidget->addWidget(page1);
stackedWidget->addWidget(page2);

// 显示第一页
stackedWidget->setCurrentIndex(0);

// 连接切换信号
connect(stackedWidget, &QStackedWidget::currentChanged,
        this, &MyWidget::handlePageChange);

// 添加切换按钮
QPushButton *nextButton = new QPushButton("下一页", this);
connect(nextButton, &QPushButton::clicked, [stackedWidget]() {
    int nextIndex = (stackedWidget->currentIndex() + 1) % stackedWidget->count();
    stackedWidget->setCurrentIndex(nextIndex);
});
```

◇ 危险操作：

```cpp
// 错误：移除当前显示的窗口后不设置新的当前窗口
stackedWidget->removeWidget(stackedWidget->currentWidget());
// 正确：先切换到其他页面再移除
stackedWidget->setCurrentIndex(0);
stackedWidget->removeWidget(widgetToRemove);

// 错误：在子页面构造函数中设置自己为当前页
// 子页面构造函数
MyPage::MyPage(QStackedWidget *parent) : QWidget(parent) {
    parent->setCurrentWidget(this); // 过早设置当前页
}
// 正确：在页面完全构建后再设置为当前页
QWidget *page = new MyPage();
stackedWidget->addWidget(page);
stackedWidget->setCurrentWidget(page);
```



# QTabBar API参考手册（Qt 5.12.12）

1️⃣【构造体系】 ◆ 核心构造函数

- QTabBar(QWidget *parent = nullptr) ▸ 典型用途：创建标签栏组件 ▸ 内存策略：父子对象自动回收 ▸ 特点：通常作为QTabWidget内部组件，也可单独使用

◆ 工厂方法

- 无内置工厂方法，通常直接实例化

2️⃣【方法矩阵】 ▨ 核心方法

| 方法签名                                       | 功能描述               | 使用频率 |
| ---------------------------------------------- | ---------------------- | -------- |
| int addTab(const QString &text)                | 添加文本标签并返回索引 | ★★★★★    |
| int addTab(const QIcon &icon, const QString &) | 添加带图标的标签       | ★★★★     |
| int insertTab(int index, const QString &)      | 在指定位置插入标签     | ★★★      |
| void removeTab(int index)                      | 移除标签               | ★★★★     |
| void moveTab(int from, int to)                 | 移动标签位置           | ★★       |
| void setTabText(int index, const QString &)    | 设置标签文本           | ★★★★     |
| void setTabIcon(int index, const QIcon &)      | 设置标签图标           | ★★★      |
| void setTabToolTip(int index, const QString &) | 设置标签提示文本       | ★★★      |
| void setTabData(int index, const QVariant &)   | 设置标签自定义数据     | ★★       |
| int currentIndex() const                       | 获取当前标签索引       | ★★★★★    |
| int count() const                              | 获取标签总数           | ★★★★     |
| void setShape(QTabBar::Shape)                  | 设置标签栏形状         | ★★★      |
| void setTabVisible(int index, bool visible)    | 设置标签可见性         | ★★★      |
| int tabAt(const QPoint &pos) const             | 获取指定位置的标签索引 | ★★       |
| QRect tabRect(int index) const                 | 获取标签矩形区域       | ★★       |

▨ 重写方法

| 方法                               | 触发场景                   | 必须调用基类 |
| ---------------------------------- | -------------------------- | ------------ |
| void paintEvent(QPaintEvent*)      | 绘制标签外观               | 否           |
| void mousePressEvent(QMouseEvent*) | 处理鼠标按下事件           | 否           |
| void mouseMoveEvent(QMouseEvent*)  | 处理鼠标移动事件(拖拽标签) | 否           |
| void tabInserted(int index)        | 标签插入后触发             | 否           |
| void tabRemoved(int index)         | 标签移除后触发             | 否           |
| void tabLayoutChange()             | 标签布局变化时             | 否           |

3️⃣【信号与槽】

 ◇ 输出信号

- currentChanged(int index) → 当前标签变化时发射
- tabCloseRequested(int index) → 点击标签关闭按钮时发射
- tabMoved(int from, int to) → 标签移动后发射
- tabBarClicked(int index) → 标签被点击时发射
- tabBarDoubleClicked(int index) → 标签被双击时发射

◇ 输入槽

- setCurrentIndex(int index) → 设置当前标签索引

4️⃣【枚举属性】

 ◇ 相关属性

| 属性名                    | 类型              | 默认值         | 描述                       |
| ------------------------- | ----------------- | -------------- | -------------------------- |
| currentIndex              | int               | -1             | 当前选中标签的索引         |
| documentMode              | bool              | false          | 文档模式(简化外观)         |
| drawBase                  | bool              | true           | 是否绘制底部线条           |
| elideMode                 | Qt::TextElideMode | Qt::ElideNone  | 文本省略模式               |
| expanding                 | bool              | false          | 标签是否扩展占满空间       |
| movable                   | bool              | false          | 标签是否可拖动             |
| tabsClosable              | bool              | false          | 标签是否显示关闭按钮       |
| usesScrollButtons         | bool              | 平台依赖       | 标签过多时是否使用滚动按钮 |
| selectionBehaviorOnRemove | SelectionBehavior | SelectRightTab | 关闭标签后选中行为         |
| shape                     | Shape             | RoundedNorth   | 标签形状                   |

◇ 相关枚举

- **QTabBar::Shape**

- 作用：定义标签栏的外观形状

- 涉及函数：setShape(), shape() 

  | 成员            | 值 | 说明                      | |-----------------|-----|--------------------------| | RoundedNorth    | 0  | 圆角标签，位于顶部        | | RoundedSouth    | 1  | 圆角标签，位于底部        | | RoundedWest     | 2  | 圆角标签，位于左侧        | | RoundedEast     | 3  | 圆角标签，位于右侧        | | TriangularNorth | 4  | 三角形标签，位于顶部      | | TriangularSouth | 5  | 三角形标签，位于底部      | | TriangularWest  | 6  | 三角形标签，位于左侧      | | TriangularEast  | 7  | 三角形标签，位于右侧      |

- **QTabBar::ButtonPosition**

- 作用：定义标签上按钮的位置
- 涉及函数：setTabButton(), tabButton() | 成员       | 值 | 说明           | |------------|-----|---------------| | LeftSide   | 0   | 左侧          | | RightSide  | 1   | 右侧          |

- **QTabBar::SelectionBehavior**

- 作用：定义关闭标签后的选择行为
- 涉及函数：setSelectionBehaviorOnRemove() | 成员             | 值 | 说明                 | |------------------|-----|---------------------| | SelectLeftTab    | 0   | 选择左侧标签        | | SelectRightTab   | 1   | 选择右侧标签        | | SelectPreviousTab| 2   | 选择前一个活动标签  |

5️⃣【版本适配】 △ Qt 5.12特性

- 改进：更好的高DPI支持和缩放行为
- 新增：标签拖放时的平滑动画效果
- 优化：文档模式下的外观渲染

△ 跨平台差异

- macOS：文档模式下外观符合macOS设计规范
- Windows：按钮位置和外观遵循Windows界面规范
- 移动平台：自动调整标签大小以适应触控操作

6️⃣【代码沙箱】 ◇ 标准用法：

```cpp
// 创建自定义标签栏
QTabBar *tabBar = new QTabBar(this);

// 添加标签
int index1 = tabBar->addTab("Tab 1");
int index2 = tabBar->addTab(QIcon(":/icons/doc.png"), "Tab 2");

// 设置标签属性
tabBar->setTabToolTip(index1, "First Tab Tooltip");
tabBar->setTabData(index2, QVariant::fromValue(someData));

// 配置标签栏属性
tabBar->setMovable(true);         // 允许拖拽重排标签
tabBar->setTabsClosable(true);    // 显示关闭按钮
tabBar->setExpanding(false);      // 不扩展标签宽度

// 连接信号
connect(tabBar, &QTabBar::currentChanged, 
        this, &MyWidget::handleTabChanged);
connect(tabBar, &QTabBar::tabCloseRequested,
        this, &MyWidget::closeTab);

// 添加到布局
layout->addWidget(tabBar);
```

◇ 危险操作：

```cpp
// 错误：移除标签后未检查空标签栏情况
tabBar->removeTab(tabBar->currentIndex());
// 正确：检查并处理
int index = tabBar->currentIndex();
if (tabBar->count() > 1) {
    tabBar->removeTab(index);
} else {
    // 处理最后一个标签的情况
}

// 错误：直接修改标签栏DOM结构
QTabBar *tabBar = findChild<QTabBar*>();  // 不要从QTabWidget获取内部QTabBar
tabBar->setTabText(0, "New Text");        // 破坏QTabWidget内部状态
// 正确：使用QTabWidget的API
tabWidget->setTabText(0, "New Text");

// 错误：标签过多导致性能问题
// 大量动态添加标签
for (int i = 0; i < 1000; i++) {
    tabBar->addTab("Tab " + QString::number(i));
}
// 正确：考虑替代UI模式
// 使用下拉菜单、分页按钮或分组标签
```



# QTabWidget API参考手册（Qt 5.12.12）

1️⃣【构造体系】
◆ 核心构造函数
- QTabWidget(QWidget *parent = nullptr) 
  ▸ 典型用途：创建标准选项卡容器
  ▸ 内存策略：父子对象自动回收

◆ 工厂方法
- 无内置工厂方法，通常直接实例化

2️⃣【方法矩阵】
▨ 核心方法
| 方法签名                                           | 功能描述             | 使用频率 |
| -------------------------------------------------- | -------------------- | -------- |
| int addTab(QWidget*, const QString&)               | 添加新选项卡         | ★★★★★    |
| int insertTab(int index, QWidget*, const QString&) | 在指定位置插入选项卡 | ★★★      |
| void removeTab(int index)                          | 移除选项卡           | ★★★★     |
| int currentIndex() const                           | 获取当前选项卡索引   | ★★★★     |
| void setCurrentIndex(int index)                    | 设置当前选项卡       | ★★★★★    |
| void setTabPosition(TabPosition)                   | 设置选项卡位置       | ★★       |
| QWidget* currentWidget() const                     | 获取当前页面组件     | ★★★★     |
| void setTabsClosable(bool)                         | 设置选项卡可关闭     | ★★★      |
| QTabBar* tabBar() const                            | 获取选项卡栏指针     | ★★       |
| void setTabText(int, const QString&)               | 修改选项卡文本       | ★★★      |
| void setTabIcon(int, const QIcon&)                 | 设置选项卡图标       | ★★       |
| void setTabToolTip(int, const QString&)            | 设置提示文本         | ★★       |

▨ 重写方法
| 方法                            | 触发场景     | 必须调用基类 |
| ------------------------------- | ------------ | ------------ |
| void resizeEvent(QResizeEvent*) | 容器大小变化 | 是           |
| bool event(QEvent*)             | 事件处理     | 通常需要     |
| void showEvent(QShowEvent*)     | 组件显示时   | 是           |

3️⃣【信号与槽】
◇ 输出信号
- currentChanged(int index) → 当前选项卡更改时发射
- tabBarClicked(int index) → 选项卡被点击时发射
- tabBarDoubleClicked(int index) → 选项卡被双击时发射
- tabCloseRequested(int index) → 关闭按钮被点击时发射

◇ 输入槽
- setCurrentIndex(int index) → 设置当前选项卡
- setCurrentWidget(QWidget *widget) → 通过页面组件设置当前选项卡
- clear() → 清除所有选项卡

4️⃣【枚举属性】
◇ 相关属性
| 属性名            | 类型              | 默认值        | 描述                          |
| ----------------- | ----------------- | ------------- | ----------------------------- |
| currentIndex      | int               | -1            | 当前选项卡索引                |
| count             | int (只读)        | 0             | 选项卡数量                    |
| documentMode      | bool              | false         | 是否使用文档模式（macOS风格） |
| elideMode         | Qt::TextElideMode | Qt::ElideNone | 文本省略模式                  |
| iconSize          | QSize             | 平台相关      | 选项卡图标大小                |
| tabPosition       | TabPosition       | North         | 选项卡位置                    |
| tabShape          | TabShape          | Rounded       | 选项卡形状                    |
| tabsClosable      | bool              | false         | 选项卡是否可关闭              |
| usesScrollButtons | bool              | 平台相关      | 是否使用滚动按钮              |

◇ 相关枚举
* **QTabWidget::TabPosition**
- 作用：描述选项卡栏的位置
- 涉及函数：setTabPosition(), tabPosition()
| 成员     | 值  | 说明              |
| North    | 0   | 选项卡在顶部      |
| South    | 1   | 选项卡在底部      |
| West     | 2   | 选项卡在左侧      |
| East     | 3   | 选项卡在右侧      |

* **QTabWidget::TabShape**
- 作用：描述选项卡的形状
- 涉及函数：setTabShape(), tabShape()
| 成员       | 值  | 说明              |
| Rounded    | 0   | 圆角选项卡        |
| Triangular | 1   | 三角形选项卡      |

5️⃣【版本适配】
△ Qt 5.12特性
- 优化：提高了在高DPI显示器上的渲染质量
- 改进：选项卡切换的平滑动画过渡

△ 跨平台差异
- Windows：默认使用Rounded标签并在需要时显示滚动按钮
- macOS：文档模式(documentMode=true)下更符合macOS界面规范
- Linux/KDE：遵循当前桌面主题设置

6️⃣【代码沙箱】
◇ 标准用法：
```cpp
// 创建选项卡控件
QTabWidget *tabs = new QTabWidget(this);
tabs->setTabPosition(QTabWidget::North);
tabs->setTabsClosable(true);

// 添加页面
QWidget *page1 = new QWidget();
QWidget *page2 = new QWidget();
tabs->addTab(page1, "第一页");
tabs->addTab(page2, "第二页");

// 连接信号
connect(tabs, &QTabWidget::currentChanged, 
        this, &MyWidget::handleTabChange);
connect(tabs, &QTabWidget::tabCloseRequested,
        this, &MyWidget::handleTabClose);
```

◇ 危险操作：
```cpp
// 错误：直接访问和修改tabBar内部实现细节
tabs->tabBar()->setTabButton(0, QTabBar::RightSide, myButton);
// 正确：使用QTabWidget提供的API
tabs->setTabIcon(0, myIcon);

// 错误：在选项卡被移除后仍访问其对应的页面组件
QWidget *widget = tabs->widget(index);
tabs->removeTab(index);
widget->show(); // 危险！移除后widget可能已被删除

// 正确：在移除前保存引用或设置控件所有权
QWidget *widget = tabs->widget(index);
tabs->removeTab(index);
widget->setParent(nullptr); // 更改所有权
widget->show();
```





# QSplitter API参考手册（Qt 5.12.12）

1️⃣【构造体系】 ◆ 核心构造函数

- QSplitter(QWidget *parent = nullptr) ▸ 典型用途：创建默认水平方向的分割器 ▸ 内存策略：父子对象自动回收
- QSplitter(Qt::Orientation orientation, QWidget *parent = nullptr) ▸ 特性标志：指定分割方向（水平或垂直） ▸ 示例：QSplitter(Qt::Vertical, parentWidget)

2️⃣【方法矩阵】 ▨ 核心方法

| 方法签名                                 | 功能描述                 | 使用频率 |
| ---------------------------------------- | ------------------------ | -------- |
| void addWidget(QWidget *widget)          | 添加小部件到分割器       | ★★★★★    |
| QWidget *widget(int index) const         | 获取指定索引位置的小部件 | ★★★      |
| int count() const                        | 获取分割器中小部件数量   | ★★★      |
| void setOrientation(Qt::Orientation)     | 设置分割方向(水平/垂直)  | ★★★★     |
| QList<int> sizes() const                 | 获取所有小部件的当前大小 | ★★★★     |
| void setSizes(const QList<int> &list)    | 设置所有小部件的大小     | ★★★★     |
| void setHandleWidth(int width)           | 设置分隔条宽度           | ★★★      |
| int handleWidth() const                  | 获取分隔条宽度           | ★★       |
| void setCollapsible(int index, bool)     | 设置子部件是否可折叠     | ★★★      |
| bool isCollapsible(int index) const      | 判断子部件是否可折叠     | ★★       |
| QSplitterHandle *handle(int index) const | 获取指定位置的分隔条句柄 | ★★       |
| int indexOf(QWidget *widget) const       | 获取小部件的索引位置     | ★★       |
| void refresh()                           | 刷新分割器布局           | ★★       |

▨ 重写方法

| 方法                            | 触发场景           | 必须调用基类 |
| ------------------------------- | ------------------ | ------------ |
| void childEvent(QChildEvent*)   | 子部件添加或移除时 | 是           |
| bool event(QEvent*)             | 处理各类事件       | 视情况而定   |
| void resizeEvent(QResizeEvent*) | 分割器大小改变时   | 否           |
| void changeEvent(QChangeEvent*) | 属性变化时         | 是           |
| QSize sizeHint() const          | 推荐大小计算       | 否           |
| QSize minimumSizeHint() const   | 最小大小计算       | 否           |

3️⃣【信号与槽】 ◇ 输出信号

- splitterMoved(int pos, int index) → 用户拖动分隔条时发射 ▸ pos: 分隔条的新位置 ▸ index: 分隔条的索引

◇ 输入槽

- (继承自QWidget的通用槽)

4️⃣【枚举属性】 ◇ 相关属性

| 属性名              | 类型            | 默认值         | 描述                                    |
| ------------------- | --------------- | -------------- | --------------------------------------- |
| orientation         | Qt::Orientation | Qt::Horizontal | 分割方向(水平/垂直)                     |
| opaqueResize        | bool            | true           | 拖动时实时调整(true)或显示指示器(false) |
| childrenCollapsible | bool            | true           | 子部件是否可折叠到零尺寸                |
| handleWidth         | int             | 系统默认值     | 分隔条宽度                              |

◇ 相关枚举

- **Qt::Orientation**

- 作用：指定分割器的方向
- 涉及函数：setOrientation(), orientation()

| 成员           | 值   | 说明                 |
| -------------- | ---- | -------------------- |
| Qt::Horizontal | 1    | 水平方向，垂直分隔条 |
| Qt::Vertical   | 2    | 垂直方向，水平分隔条 |

5️⃣【版本适配】 △ Qt 5.12特性

- 保持了与Qt 5.11的兼容性

△ 跨平台差异

- Windows：分隔条有特定的系统样式
- macOS：遵循系统HIG设计规范
- Linux：分隔条视觉效果取决于活动的桌面环境主题

6️⃣【代码沙箱】 ◇ 标准用法：

```cpp
// 创建水平分割器
QSplitter *splitter = new QSplitter(Qt::Horizontal, parent);

// 添加两个部件
QTextEdit *editor1 = new QTextEdit(splitter);
QTextEdit *editor2 = new QTextEdit(splitter);

// 设置初始大小比例 (2:1)
splitter->setSizes(QList<int>() << 200 << 100);

// 连接信号
connect(splitter, &QSplitter::splitterMoved, 
        this, &MyWidget::handleSplitterMoved);

// 设置为窗口中心部件
setCentralWidget(splitter);
```

◇ 危险操作：

```cpp
// 错误：在布局中直接使用handle()获取到的分隔条
QHBoxLayout *layout = new QHBoxLayout(this);
layout->addWidget(splitter->handle(0)); // 错误!

// 错误：移除已添加到分割器的部件
delete editor1; // 会导致分割器状态不一致

// 正确：使用QSplitter自己的机制
editor1->setParent(nullptr); // 安全移除
// 或
splitter->setParent(nullptr); // 整体移除分割器
```





# QSplitterHandle API参考手册（Qt 5.12.12）

1️⃣【构造体系】 ◆ 核心构造函数

- QSplitterHandle(Qt::Orientation orientation, QSplitter *parent) ▸ 典型用途：由QSplitter内部创建，很少直接实例化 ▸ 内存策略：由父QSplitter自动管理

◆ 工厂方法（若有）

- (不提供工厂方法，通常通过QSplitter::handle()获取实例)

2️⃣【方法矩阵】 ▨ 核心方法

| 方法签名                             | 功能描述                | 使用频率 |
| ------------------------------------ | ----------------------- | -------- |
| QSplitter *splitter() const          | 获取所属的QSplitter对象 | ★★★      |
| Qt::Orientation orientation() const  | 获取分隔条方向          | ★★★      |
| void setOrientation(Qt::Orientation) | 设置分隔条方向          | ★★       |
| bool opaqueResize() const            | 获取是否启用不透明调整  | ★★       |
| int closestLegalPosition(int pos)    | 获取最接近的合法位置    | ★★       |
| QSize sizeHint() const               | 获取推荐大小            | ★★       |

▨ 重写方法

| 方法                                 | 触发场景         | 必须调用基类 |
| ------------------------------------ | ---------------- | ------------ |
| void paintEvent(QPaintEvent*)        | 绘制分隔条外观时 | 否           |
| void mouseMoveEvent(QMouseEvent*)    | 鼠标拖动分隔条时 | 否           |
| void mousePressEvent(QMouseEvent*)   | 鼠标按下分隔条时 | 否           |
| void mouseReleaseEvent(QMouseEvent*) | 鼠标释放分隔条时 | 否           |
| bool event(QEvent*)                  | 处理通用事件     | 视情况而定   |
| void resizeEvent(QResizeEvent*)      | 分隔条大小改变时 | 视情况而定   |

3️⃣【信号与槽】 ◇ 输出信号

- (不提供自定义信号，移动事件通过父QSplitter的splitterMoved信号传递)

◇ 输入槽

- (不提供自定义槽)

4️⃣【枚举属性】 ◇ 相关属性

| 属性名      | 类型            | 默认值            | 描述       |
| ----------- | --------------- | ----------------- | ---------- |
| orientation | Qt::Orientation | 由父QSplitter决定 | 分隔条方向 |

◇ 相关枚举

- **Qt::Orientation**

- 作用：定义分隔条的方向
- 涉及函数：orientation(), setOrientation()

| 成员           | 值   | 说明                 |
| -------------- | ---- | -------------------- |
| Qt::Horizontal | 1    | 水平方向，垂直分隔条 |
| Qt::Vertical   | 2    | 垂直方向，水平分隔条 |

5️⃣【版本适配】 △ Qt 5.12特性

- 保持与Qt 5.11的兼容性

△ 跨平台差异

- Windows：分隔条有特定的系统风格
- macOS：遵循系统HIG设计规范
- Linux：分隔条外观取决于桌面环境主题

6️⃣【代码沙箱】 ◇ 标准用法：

```cpp
// 获取QSplitter的第一个分隔条
QSplitter *splitter = new QSplitter(Qt::Horizontal, parent);
// 添加小部件
splitter->addWidget(new QTextEdit);
splitter->addWidget(new QTextEdit);
// 获取第一个分隔条句柄（索引为0）
QSplitterHandle *handle = splitter->handle(0);

// 自定义分隔条外观
class MyHandle : public QSplitterHandle {
public:
    MyHandle(Qt::Orientation orientation, QSplitter *parent)
        : QSplitterHandle(orientation, parent) {}
    
protected:
    void paintEvent(QPaintEvent *event) override {
        QPainter painter(this);
        painter.fillRect(event->rect(), QColor(200, 200, 200));
        // 绘制自定义样式...
    }
};

// 设置自定义分隔条创建函数
class MySplitter : public QSplitter {
protected:
    QSplitterHandle *createHandle() override {
        return new MyHandle(orientation(), this);
    }
};
```

◇ 危险操作：

```cpp
// 错误：直接删除分隔条
QSplitterHandle *handle = splitter->handle(0);
delete handle; // 错误！会导致QSplitter内部状态不一致

// 错误：向布局中添加分隔条句柄
QVBoxLayout *layout = new QVBoxLayout(this);
layout->addWidget(splitter->handle(0)); // 错误！

// 错误：手动设置分隔条大小
handle->resize(10, 100); // 应由QSplitter控制

// 正确：通过QSplitter控制分隔条宽度
splitter->setHandleWidth(10);
```





# QLayout API参考手册（Qt 5.12.12）

1️⃣【构造体系】 ◆ 核心构造函数

- QLayout(QWidget *parent = nullptr) ▸ 典型用途：创建布局并设置父窗口 ▸ 内存策略：与父窗口同生命周期
- QLayout() ▸ 创建无父窗口的布局，需手动设置父布局 ▸ 注意：QLayout是抽象类，通常使用其派生类

◆ 工厂方法

- 无直接工厂方法，通常使用派生类实例化

2️⃣【方法矩阵】 ▨ 核心方法

| 方法签名                                 | 功能描述             | 使用频率 |
| ---------------------------------------- | -------------------- | -------- |
| void addWidget(QWidget *)                | 添加窗口部件到布局   | ★★★★★    |
| void setContentsMargins(int,int,int,int) | 设置布局内容边距     | ★★★★     |
| QMargins contentsMargins() const         | 获取布局内容边距     | ★★★      |
| void setSpacing(int)                     | 设置元素间距         | ★★★★     |
| int spacing() const                      | 获取元素间距         | ★★★      |
| void setSizeConstraint(SizeConstraint)   | 设置布局尺寸约束     | ★★★      |
| bool isEnabled() const                   | 检查布局是否启用     | ★★       |
| void setEnabled(bool)                    | 设置布局启用状态     | ★★       |
| QLayoutItem *itemAt(int) const           | 获取指定索引的布局项 | ★★★      |
| int count() const                        | 获取布局项数量       | ★★★      |
| QSize sizeHint() const                   | 获取布局推荐尺寸     | ★★★      |
| QSize minimumSize() const                | 获取布局最小尺寸     | ★★★      |

▨ 重写方法

| 方法                                | 触发场景             | 必须调用基类 |
| ----------------------------------- | -------------------- | ------------ |
| virtual void addItem(QLayoutItem *) | 添加新布局项时       | 否(纯虚函数) |
| virtual QLayoutItem *takeAt(int)    | 移除布局项时         | 否(纯虚函数) |
| virtual int count() const           | 获取布局项数量时     | 否(纯虚函数) |
| virtual QSize sizeHint() const      | 计算推荐尺寸时       | 否           |
| virtual QSize minimumSize() const   | 计算最小尺寸时       | 否           |
| virtual void invalidate()           | 使布局失效需重新计算 | 是           |

3️⃣【信号与槽】 ◇ 输出信号

- 无直接信号

◇ 输入槽

- update() → 更新布局
- activate() → 激活布局重新计算

4️⃣【枚举属性】 ◇ 相关属性

| 属性名          | 类型           | 默认值               | 描述             |
| --------------- | -------------- | -------------------- | ---------------- |
| enabled         | bool           | true                 | 布局是否启用     |
| sizeConstraint  | SizeConstraint | SetDefaultConstraint | 布局尺寸约束行为 |
| spacing         | int            | 取决于样式           | 布局元素间距     |
| contentsMargins | QMargins       | 取决于样式           | 布局内容边距     |

◇ 相关枚举

- **QLayout::SizeConstraint**

- 作用：控制布局如何调整大小

| 成员                 | 值   | 说明                                   |
| -------------------- | ---- | -------------------------------------- |
| SetDefaultConstraint | 0    | 使用默认约束（通常可调整大小）         |
| SetNoConstraint      | 1    | 不受布局项约束，可自由调整大小         |
| SetMinimumSize       | 2    | 窗口尺寸不小于布局最小尺寸             |
| SetFixedSize         | 3    | 窗口尺寸与布局sizeHint相同且固定       |
| SetMaximumSize       | 4    | 窗口尺寸不大于布局最大尺寸             |
| SetMinAndMaxSize     | 5    | 窗口尺寸在最小和最大尺寸之间且不可改变 |

5️⃣【版本适配】 △ Qt 5.12特性

- 改进：布局计算性能优化
- 无重大API变更

△ 跨平台差异

- Windows/macOS/Linux：默认边距和间距可能略有不同
- 高DPI支持：布局在高DPI显示器上自动缩放

6️⃣【代码沙箱】 ◇ 标准用法：

```cpp
// 注意：QLayout是抽象类，通常使用其派生类
QVBoxLayout *layout = new QVBoxLayout(widget);
layout->setContentsMargins(10, 10, 10, 10);
layout->setSpacing(5);
layout->addWidget(new QPushButton("Button 1"));
layout->addWidget(new QPushButton("Button 2"));
widget->setLayout(layout);
```

◇ 危险操作：

```cpp
// 错误：创建布局但不设置父窗口或父布局
QVBoxLayout *layout = new QVBoxLayout();
layout->addWidget(new QPushButton("Button"));
// 内存泄漏：布局及其子项未被添加到窗口层次结构

// 正确：
QVBoxLayout *layout = new QVBoxLayout();
widget->setLayout(layout); // 或在构造时指定父窗口
```

// 错误：向已有布局的窗口添加新布局 QWidget *widget = new QWidget(); widget->setLayout(new QVBoxLayout()); widget->setLayout(new QHBoxLayout()); // 会导致资源泄漏和未定义行为

// 正确：使用嵌套布局结构或先清除旧布局 QWidget *widget = new QWidget(); QLayout *oldLayout = widget->layout(); if (oldLayout) { delete oldLayout; } widget->setLayout(new QHBoxLayout());



# QLayoutItem API参考手册（Qt 5.12.12）

1️⃣【构造体系】 ◆ 核心构造函数

- QLayoutItem(Qt::Alignment alignment = Qt::Alignment()) ▸ 典型用途：创建布局项基类实例 ▸ 内存策略：通常需要手动删除（无父子对象关系）

◆ 工厂方法

- 无工厂方法，QLayoutItem是抽象基类，通常使用其派生类

2️⃣【方法矩阵】 ▨ 核心方法

| 方法签名                                             | 功能描述                 | 使用频率 |
| ---------------------------------------------------- | ------------------------ | -------- |
| virtual QSize sizeHint() const                       | 获取推荐尺寸             | ★★★★     |
| virtual QSize minimumSize() const                    | 获取最小尺寸             | ★★★★     |
| virtual QSize maximumSize() const                    | 获取最大尺寸             | ★★★      |
| virtual Qt::Orientations expandingDirections() const | 获取可扩展方向           | ★★★      |
| virtual void setGeometry(const QRect &)              | 设置几何区域             | ★★★★     |
| virtual QRect geometry() const                       | 获取几何区域             | ★★★★     |
| virtual bool isEmpty() const                         | 检查是否为空             | ★★★      |
| virtual bool hasHeightForWidth() const               | 检查是否高度依赖宽度     | ★★       |
| virtual int heightForWidth(int) const                | 获取指定宽度对应的高度   | ★★       |
| virtual QLayoutItem *widget()                        | 如果是小部件项返回小部件 | ★★★      |
| virtual QLayout *layout()                            | 如果是布局项返回布局     | ★★★      |
| virtual QSpacerItem *spacerItem()                    | 如果是间隔项返回间隔项   | ★★       |
| Qt::Alignment alignment() const                      | 获取对齐方式             | ★★★      |
| void setAlignment(Qt::Alignment)                     | 设置对齐方式             | ★★★      |
| virtual void invalidate()                            | 使布局项失效需重新计算   | ★★★      |

▨ 重写方法

| 方法                                                 | 触发场景           | 必须调用基类 |
| ---------------------------------------------------- | ------------------ | ------------ |
| virtual QSize sizeHint() const                       | 计算推荐尺寸时     | 否(纯虚函数) |
| virtual QSize minimumSize() const                    | 计算最小尺寸时     | 否(纯虚函数) |
| virtual QSize maximumSize() const                    | 计算最大尺寸时     | 否(纯虚函数) |
| virtual Qt::Orientations expandingDirections() const | 确定扩展方向时     | 否(纯虚函数) |
| virtual void setGeometry(const QRect &)              | 布局更新几何区域时 | 否(纯虚函数) |
| virtual bool isEmpty() const                         | 检查项是否为空时   | 否           |

3️⃣【信号与槽】 ◇ 输出信号

- 无信号（QLayoutItem不是QObject子类）

◇ 输入槽

- 无槽（QLayoutItem不是QObject子类）

4️⃣【枚举属性】 ◇ 相关属性

| 属性名    | 类型          | 默认值          | 描述             |
| --------- | ------------- | --------------- | ---------------- |
| alignment | Qt::Alignment | Qt::Alignment() | 布局项的对齐方式 |

◇ 相关枚举

- **Qt::Alignment**

- 作用：控制布局项在其可用空间中的对齐方式

| 成员             | 值                           | 说明         |
| ---------------- | ---------------------------- | ------------ |
| Qt::AlignLeft    | 0x0001                       | 水平向左对齐 |
| Qt::AlignRight   | 0x0002                       | 水平向右对齐 |
| Qt::AlignHCenter | 0x0004                       | 水平居中     |
| Qt::AlignTop     | 0x0020                       | 垂直顶部对齐 |
| Qt::AlignBottom  | 0x0040                       | 垂直底部对齐 |
| Qt::AlignVCenter | 0x0080                       | 垂直居中     |
| Qt::AlignCenter  | AlignHCenter \| AlignVCenter | 水平垂直居中 |

- **Qt::Orientations** (用于expandingDirections方法)

- 作用：指定布局项可扩展的方向

| 成员           | 值   | 说明           |
| -------------- | ---- | -------------- |
| Qt::Horizontal | 0x1  | 水平方向可扩展 |
| Qt::Vertical   | 0x2  | 垂直方向可扩展 |

5️⃣【版本适配】 △ Qt 5.12特性

- 无重大API变更
- 性能优化：布局计算算法优化

△ 跨平台差异

- 布局计算在不同平台上基本一致
- 高DPI支持：在高DPI显示器上正确缩放

6️⃣【代码沙箱】 ◇ 标准用法：

```cpp
// 通常不直接使用QLayoutItem，而是使用其派生类
// 以下展示如何遍历布局中的布局项
QLayout *layout = widget->layout();
for (int i = 0; i < layout->count(); ++i) {
    QLayoutItem *item = layout->itemAt(i);
    QWidget *widget = item->widget();
    if (widget) {
        // 处理小部件
    }
    QLayout *childLayout = item->layout();
    if (childLayout) {
        // 处理子布局
    }
    QSpacerItem *spacer = item->spacerItem();
    if (spacer) {
        // 处理间隔项
    }
}
```

◇ 危险操作：

```cpp
// 错误：未释放布局项导致内存泄漏
QLayoutItem *item = layout->takeAt(0);
// 应该手动删除
delete item;

// 错误：删除布局中的项但未从布局中移除
QLayoutItem *item = layout->itemAt(0);
delete item; // 导致悬空指针，布局仍然尝试访问已删除的项

// 正确：
QLayoutItem *item = layout->takeAt(0); // 先从布局中移除
delete item; // 然后删除
```

// 错误：尝试将QLayoutItem当作QObject使用 QLayoutItem *item = layout->itemAt(0); connect(item, SIGNAL(...), this, SLOT(...)); // 编译错误，QLayoutItem不是QObject



# QSpacerItem API参考手册（Qt 5.12.12）

1️⃣【构造体系】 ◆ 核心构造函数

- QSpacerItem(int w, int h, QSizePolicy::Policy hPolicy = QSizePolicy::Minimum, QSizePolicy::Policy vPolicy = QSizePolicy::Minimum) ▸ 典型用途：创建固定或可扩展空白间隔 ▸ 内存策略：需手动删除（无父子对象关系）

◆ 工厂方法

- 无专用工厂方法，直接实例化

2️⃣【方法矩阵】 ▨ 核心方法

| 方法签名                                                     | 功能描述                    | 使用频率 |
| ------------------------------------------------------------ | --------------------------- | -------- |
| void changeSize(int w, int h, QSizePolicy::Policy hPolicy, QSizePolicy::Policy vPolicy) | 修改间隔尺寸和策略          | ★★★      |
| QSize sizeHint() const                                       | 获取推荐尺寸                | ★★★      |
| QSize minimumSize() const                                    | 获取最小尺寸                | ★★★      |
| QSize maximumSize() const                                    | 获取最大尺寸                | ★★★      |
| Qt::Orientations expandingDirections() const                 | 获取可扩展方向              | ★★★★     |
| void setGeometry(const QRect &)                              | 设置几何区域                | ★★★      |
| QRect geometry() const                                       | 获取几何区域                | ★★★      |
| bool isEmpty() const                                         | 检查是否为空                | ★★       |
| QSpacerItem *spacerItem()                                    | 返回自身作为QSpacerItem指针 | ★★★      |

▨ 重写方法（继承自QLayoutItem）

| 方法                                         | 触发场景           | 必须调用基类 |
| -------------------------------------------- | ------------------ | ------------ |
| QSize sizeHint() const                       | 计算推荐尺寸时     | 否           |
| QSize minimumSize() const                    | 计算最小尺寸时     | 否           |
| QSize maximumSize() const                    | 计算最大尺寸时     | 否           |
| Qt::Orientations expandingDirections() const | 确定扩展方向时     | 否           |
| void setGeometry(const QRect &)              | 布局更新几何区域时 | 否           |
| bool isEmpty() const                         | 检查项是否为空时   | 否           |
| QSpacerItem *spacerItem()                    | 获取间隔项指针时   | 否           |

3️⃣【信号与槽】 ◇ 输出信号

- 无信号（QSpacerItem不是QObject子类）

◇ 输入槽

- 无槽（QSpacerItem不是QObject子类）

4️⃣【枚举属性】 ◇ 相关属性

- 无直接属性（非QObject子类）

◇ 相关枚举

- **QSizePolicy::Policy**

- 作用：定义间隔项如何响应布局调整

| 成员             | 值                                   | 说明                                  |
| ---------------- | ------------------------------------ | ------------------------------------- |
| Fixed            | 0                                    | 大小固定，不随布局变化                |
| Minimum          | GrowFlag                             | 提供最小尺寸，可以变大                |
| Maximum          | ShrinkFlag                           | 提供最大尺寸，可以变小                |
| Preferred        | GrowFlag \| ShrinkFlag               | 有首选尺寸，可以变大或变小            |
| Expanding        | GrowFlag \| ShrinkFlag \| ExpandFlag | 与Preferred类似但更倾向于占用可用空间 |
| MinimumExpanding | GrowFlag \| ExpandFlag               | 提供最小尺寸，强烈倾向于占用空间      |
| Ignored          | ShrinkFlag \| GrowFlag \| IgnoreFlag | 完全忽略sizeHint，使用可用空间        |

5️⃣【版本适配】 △ Qt 5.12特性

- 无重大API变更
- 内部优化：布局计算性能提升

△ 跨平台差异

- 所有平台行为一致
- 高DPI显示器上自动缩放

6️⃣【代码沙箱】 ◇ 标准用法：

```cpp
// 创建固定大小间隔（10x10像素）
QSpacerItem *fixedSpacer = new QSpacerItem(10, 10, QSizePolicy::Fixed, QSizePolicy::Fixed);
layout->addItem(fixedSpacer);

// 创建可伸缩的水平间隔（将小部件推向两侧）
QSpacerItem *hSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
hBoxLayout->addItem(hSpacer);

// 创建可伸缩的垂直间隔（将小部件推向顶部和底部）
QSpacerItem *vSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);
vBoxLayout->addItem(vSpacer);
```

◇ 危险操作：

```cpp
// 错误：未释放从布局中移除的间隔项
QLayoutItem *item = layout->takeAt(0); // 假设这是一个QSpacerItem
// 应当手动删除
delete item;

// 错误：尝试将QSpacerItem当作QObject使用
QSpacerItem *spacer = new QSpacerItem(10, 10);
connect(spacer, SIGNAL(...), this, SLOT(...)); // 编译错误，QSpacerItem不是QObject

// 错误：尝试将QSpacerItem直接添加到布局，但未稍后删除它
QHBoxLayout *layout = new QHBoxLayout();
layout->addItem(new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum));
// 在布局或其父窗口被销毁时，间隔项不会自动删除
```

// 提示：使用QLayout的智能间隔方法替代手动创建QSpacerItem layout->addStretch(); // 添加伸缩间隔 layout->addSpacing(10); // 添加固定像素间隔



# QWidgetItem API参考手册（Qt 5.12.12）

1️⃣【构造体系】 ◆ 核心构造函数

- QWidgetItem(QWidget *widget) ▸ 典型用途：为布局创建包装小部件的布局项 ▸ 内存策略：布局负责删除，无需手动删除（通常由布局管理）

◆ 工厂方法

- 无工厂方法，直接通过构造函数创建

2️⃣【方法矩阵】 ▨ 核心方法

| 方法签名                                     | 功能描述                      | 使用频率 |
| -------------------------------------------- | ----------------------------- | -------- |
| QSize sizeHint() const                       | 获取窗口部件的推荐尺寸        | ★★★★     |
| QSize minimumSize() const                    | 获取窗口部件的最小尺寸        | ★★★★     |
| QSize maximumSize() const                    | 获取窗口部件的最大尺寸        | ★★★      |
| Qt::Orientations expandingDirections() const | 获取窗口部件的扩展方向        | ★★★      |
| void setGeometry(const QRect &)              | 设置窗口部件的几何区域        | ★★★★     |
| QRect geometry() const                       | 获取窗口部件的几何区域        | ★★★★     |
| bool isEmpty() const                         | 检查是否为空（始终返回false） | ★★       |
| bool hasHeightForWidth() const               | 检查窗口部件是否高度依赖宽度  | ★★★      |
| int heightForWidth(int w) const              | 获取指定宽度对应的高度        | ★★★      |
| QWidget *widget()                            | 获取关联的窗口部件            | ★★★★★    |
| QLayout *layout()                            | 获取关联的布局（始终为空）    | ★★       |
| QSpacerItem *spacerItem()                    | 获取间隔项（始终为空）        | ★★       |

▨ 重写方法（继承自QLayoutItem）

| 方法                                         | 触发场景                 | 必须调用基类 |
| -------------------------------------------- | ------------------------ | ------------ |
| QSize sizeHint() const                       | 获取窗口部件的推荐尺寸时 | 否           |
| QSize minimumSize() const                    | 获取窗口部件的最小尺寸时 | 否           |
| QSize maximumSize() const                    | 获取窗口部件的最大尺寸时 | 否           |
| Qt::Orientations expandingDirections() const | 确定窗口部件扩展方向时   | 否           |
| void setGeometry(const QRect &)              | 更新窗口部件几何区域时   | 否           |
| bool isEmpty() const                         | 检查项是否为空时         | 否           |
| QWidget *widget()                            | 获取关联的窗口部件时     | 否           |

3️⃣【信号与槽】 ◇ 输出信号

- 无信号（QWidgetItem不是QObject子类）

◇ 输入槽

- 无槽（QWidgetItem不是QObject子类）

4️⃣【枚举属性】 ◇ 相关属性

- 无直接属性（非QObject子类）

◇ 相关枚举

- 无直接枚举，但利用关联QWidget的QSizePolicy属性

- **QSizePolicy** (通过QWidget使用)

- 作用：控制窗口部件在布局中的尺寸行为

| 属性            | 说明                                 |
| --------------- | ------------------------------------ |
| sizeHint        | 窗口部件的首选尺寸                   |
| minimumSizeHint | 窗口部件的最小推荐尺寸               |
| minimumSize     | 窗口部件的强制最小尺寸               |
| maximumSize     | 窗口部件的强制最大尺寸               |
| sizePolicy      | 控制如何处理尺寸策略（扩展、收缩等） |

5️⃣【版本适配】 △ Qt 5.12特性

- 无重大API变更
- 内部优化：布局计算效率提升

△ 跨平台差异

- 所有平台行为一致
- 高DPI支持：与QWidget的高DPI支持一致

6️⃣【代码沙箱】 ◇ 标准用法：

```cpp
// 通常不直接使用QWidgetItem，而是通过QLayout::addWidget添加窗口部件
QHBoxLayout *layout = new QHBoxLayout(parentWidget);
QPushButton *button = new QPushButton("Button", parentWidget);
layout->addWidget(button);  // 内部创建QWidgetItem

// 遍历布局中的所有窗口部件
QLayout *someLayout = parentWidget->layout();
for (int i = 0; i < someLayout->count(); ++i) {
    QLayoutItem *item = someLayout->itemAt(i);
    QWidget *widget = item->widget();
    if (widget) {
        // 这是一个QWidgetItem，处理关联的窗口部件
        widget->setVisible(true);
    }
}
```

◇ 危险操作：

```cpp
// 错误：直接创建QWidgetItem并添加到布局
QWidgetItem *item = new QWidgetItem(new QPushButton("Button"));
layout->addItem(item);
// 这可能导致内存管理问题，应使用QLayout::addWidget()

// 错误：删除布局项但没有考虑关联的窗口部件
QLayoutItem *item = layout->takeAt(0);
delete item; // 这不会删除关联的窗口部件，可能导致内存泄漏
// 正确做法：
QLayoutItem *item = layout->takeAt(0);
if (QWidget *w = item->widget()) {
    w->deleteLater(); // 或适当处理窗口部件
}
delete item;

// 错误：修改已添加到布局的窗口部件的父对象
QWidget *widget = layout->itemAt(0)->widget();
widget->setParent(anotherWidget); // 会导致布局混乱
```

// 注意：QWidgetItem通常由布局内部管理，不应手动创建 // 应使用 layout->addWidget(widget) 而非 layout->addItem(new QWidgetItem(widget))
