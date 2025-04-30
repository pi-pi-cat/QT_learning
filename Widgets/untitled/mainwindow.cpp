#include "mainwindow.h"
#include <QMessageBox>
#include <QDateTime>
#include <QTimer>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    setWindowTitle(tr("客运信息系统"));
    resize(800, 600);

    createActions();
    createMenus();
    createToolBars();
    createStatusBar();
    createCentralWidget();
}

MainWindow::~MainWindow()
{
}

void MainWindow::createActions()
{
    // 路线管理动作
    newRouteAction = new QAction(QIcon(":/icons/new.png"), tr("新建路线"), this);
    newRouteAction->setShortcut(QKeySequence::New);
    newRouteAction->setStatusTip(tr("创建新的客运路线"));
    connect(newRouteAction, &QAction::triggered, this, &MainWindow::handleNewRoute);

    editRouteAction = new QAction(QIcon(":/icons/edit.png"), tr("编辑路线"), this);
    editRouteAction->setStatusTip(tr("编辑选中的客运路线"));
    connect(editRouteAction, &QAction::triggered, this, &MainWindow::handleEditRoute);

    deleteRouteAction = new QAction(QIcon(":/icons/delete.png"), tr("删除路线"), this);
    deleteRouteAction->setStatusTip(tr("删除选中的客运路线"));
    connect(deleteRouteAction, &QAction::triggered, this, &MainWindow::handleDeleteRoute);

    // 打印动作
    printScheduleAction = new QAction(QIcon(":/icons/print.png"), tr("打印时刻表"), this);
    printScheduleAction->setShortcut(QKeySequence::Print);
    printScheduleAction->setStatusTip(tr("打印当前的客运时刻表"));
    connect(printScheduleAction, &QAction::triggered, this, &MainWindow::handlePrintSchedule);

    exportScheduleAction = new QAction(QIcon(":/icons/export.png"), tr("导出时刻表"), this);
    exportScheduleAction->setStatusTip(tr("将时刻表导出为PDF或Excel文件"));
    connect(exportScheduleAction, &QAction::triggered, this, &MainWindow::handleExportSchedule);

    // 方向动作
    northAction = new QAction(tr("北线路线"), this);
    northAction->setStatusTip(tr("显示北线客运路线"));
    northAction->setData("north");

    southAction = new QAction(tr("南线路线"), this);
    southAction->setStatusTip(tr("显示南线客运路线"));
    southAction->setData("south");

    eastAction = new QAction(tr("东线路线"), this);
    eastAction->setStatusTip(tr("显示东线客运路线"));
    eastAction->setData("east");

    westAction = new QAction(tr("西线路线"), this);
    westAction->setStatusTip(tr("显示西线客运路线"));
    westAction->setData("west");

    // 其他动作
    searchAction = new QAction(QIcon(":/icons/search.png"), tr("搜索"), this);
    searchAction->setShortcut(QKeySequence::Find);
    searchAction->setStatusTip(tr("搜索客运路线和时刻"));
    connect(searchAction, &QAction::triggered, this, &MainWindow::handleSearch);

    settingsAction = new QAction(QIcon(":/icons/settings.png"), tr("设置"), this);
    settingsAction->setStatusTip(tr("调整应用程序设置"));
    connect(settingsAction, &QAction::triggered, this, &MainWindow::handleSettings);

    helpAction = new QAction(QIcon(":/icons/help.png"), tr("帮助"), this);
    helpAction->setShortcut(QKeySequence::HelpContents);
    helpAction->setStatusTip(tr("显示帮助内容"));
    connect(helpAction, &QAction::triggered, this, &MainWindow::handleHelp);

    aboutAction = new QAction(tr("关于"), this);
    aboutAction->setStatusTip(tr("关于客运信息系统"));
    connect(aboutAction, &QAction::triggered, this, &MainWindow::handleAbout);
}

void MainWindow::createMenus()
{
    // 路线菜单
    routeMenu = new QMenu(tr("路线管理"), this);
    routeMenu->addAction(newRouteAction);
    routeMenu->addAction(editRouteAction);
    routeMenu->addAction(deleteRouteAction);

    // 打印菜单
    printMenu = new QMenu(tr("打印"), this);
    printMenu->addAction(printScheduleAction);
    printMenu->addAction(exportScheduleAction);

    // 方向菜单
    directionsMenu = new QMenu(tr("方向"), this);
    directionsMenu->addAction(northAction);
    directionsMenu->addAction(southAction);
    directionsMenu->addAction(eastAction);
    directionsMenu->addAction(westAction);

    // 连接方向菜单事件
    connect(directionsMenu, &QMenu::triggered, this, &MainWindow::handleDirections);
}

QToolButton* MainWindow::createRouteButton()
{
    QToolButton* routeButton = new QToolButton(this);
    routeButton->setDefaultAction(newRouteAction);
    routeButton->setPopupMode(QToolButton::MenuButtonPopup);
    routeButton->setMenu(routeMenu);
    routeButton->setToolTip(tr("路线管理选项"));
    return routeButton;
}

QToolButton* MainWindow::createPrintButton()
{
    QToolButton* printButton = new QToolButton(this);
    printButton->setDefaultAction(printScheduleAction);
    printButton->setPopupMode(QToolButton::MenuButtonPopup);
    printButton->setMenu(printMenu);
    printButton->setToolTip(tr("打印和导出选项"));
    return printButton;
}

QToolButton* MainWindow::createDirectionsButton()
{
    QToolButton* directionsButton = new QToolButton(this);
    directionsButton->setIcon(QIcon(":/icons/directions.png"));
    directionsButton->setText(tr("方向"));
    directionsButton->setToolTip(tr("选择客运线路方向"));
    directionsButton->setPopupMode(QToolButton::InstantPopup);
    directionsButton->setMenu(directionsMenu);
    directionsButton->setAutoRaise(true);
    return directionsButton;
}

QToolButton* MainWindow::createSearchButton()
{
    QToolButton* searchButton = new QToolButton(this);
    searchButton->setDefaultAction(searchAction);
    searchButton->setAutoRaise(true);
    return searchButton;
}

QToolButton* MainWindow::createSettingsButton()
{
    QToolButton* settingsButton = new QToolButton(this);
    settingsButton->setDefaultAction(settingsAction);
    settingsButton->setAutoRaise(true);
    return settingsButton;
}

QToolButton* MainWindow::createHelpButton()
{
    QToolButton* helpButton = new QToolButton(this);
    helpButton->setText(tr("帮助"));
    helpButton->setIcon(QIcon(":/icons/help.png"));
    helpButton->setToolTip(tr("获取帮助或查看关于信息"));
    helpButton->setPopupMode(QToolButton::DelayedPopup);

    // 创建帮助菜单
    QMenu* helpMenu = new QMenu(this);
    helpMenu->addAction(helpAction);
    helpMenu->addSeparator();
    helpMenu->addAction(aboutAction);
    helpButton->setMenu(helpMenu);

    return helpButton;
}

void MainWindow::createToolBars()
{
    mainToolBar = addToolBar(tr("主工具栏"));
    mainToolBar->setMovable(false);

    // 添加按钮到工具栏
    mainToolBar->addWidget(createRouteButton());
    mainToolBar->addWidget(createPrintButton());
    mainToolBar->addSeparator();

    // 添加方向按钮 - 使用箭头类型
    QToolButton* northBtn = new QToolButton(this);
    northBtn->setArrowType(Qt::UpArrow);
    northBtn->setToolTip(tr("北线路线"));
    northBtn->setAutoRaise(true);
    connect(northBtn, &QToolButton::clicked, [=]() {
        handleDirections(northAction);
    });
    mainToolBar->addWidget(northBtn);

    QToolButton* southBtn = new QToolButton(this);
    southBtn->setArrowType(Qt::DownArrow);
    southBtn->setToolTip(tr("南线路线"));
    southBtn->setAutoRaise(true);
    connect(southBtn, &QToolButton::clicked, [=]() {
        handleDirections(southAction);
    });
    mainToolBar->addWidget(southBtn);

    QToolButton* eastBtn = new QToolButton(this);
    eastBtn->setArrowType(Qt::RightArrow);
    eastBtn->setToolTip(tr("东线路线"));
    eastBtn->setAutoRaise(true);
    connect(eastBtn, &QToolButton::clicked, [=]() {
        handleDirections(eastAction);
    });
    mainToolBar->addWidget(eastBtn);

    QToolButton* westBtn = new QToolButton(this);
    westBtn->setArrowType(Qt::LeftArrow);
    westBtn->setToolTip(tr("西线路线"));
    westBtn->setAutoRaise(true);
    connect(westBtn, &QToolButton::clicked, [=]() {
        handleDirections(westAction);
    });
    mainToolBar->addWidget(westBtn);

    mainToolBar->addWidget(createDirectionsButton());
    mainToolBar->addSeparator();
    mainToolBar->addWidget(createSearchButton());

    // 添加样式选择器
    mainToolBar->addSeparator();
    mainToolBar->addWidget(new QLabel(tr("按钮样式: ")));

    styleComboBox = new QComboBox(this);
    styleComboBox->addItem(tr("仅图标"), Qt::ToolButtonIconOnly);
    styleComboBox->addItem(tr("仅文本"), Qt::ToolButtonTextOnly);
    styleComboBox->addItem(tr("文本在图标旁"), Qt::ToolButtonTextBesideIcon);
    styleComboBox->addItem(tr("文本在图标下"), Qt::ToolButtonTextUnderIcon);
    connect(styleComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &MainWindow::changeToolButtonStyle);
    mainToolBar->addWidget(styleComboBox);

    // 右侧添加设置和帮助按钮
    QWidget* spacer = new QWidget(this);
    spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    mainToolBar->addWidget(spacer);

    mainToolBar->addWidget(createSettingsButton());
    mainToolBar->addWidget(createHelpButton());
}

void MainWindow::createStatusBar()
{
    statusLabel = new QLabel(tr("就绪"), this);
    statusBar()->addPermanentWidget(statusLabel);

    // 添加当前时间到状态栏
    QLabel* timeLabel = new QLabel(this);
    statusBar()->addPermanentWidget(timeLabel);

    // 更新时间的定时器
    QTimer* timer = new QTimer(this);
    connect(timer, &QTimer::timeout, [=]() {
        timeLabel->setText(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"));
    });
    timer->start(1000);

    statusBar()->showMessage(tr("客运信息系统已启动"), 3000);
}

void MainWindow::createCentralWidget()
{
    QWidget* centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    QVBoxLayout* mainLayout = new QVBoxLayout(centralWidget);

    // 创建日期选择器
    QHBoxLayout* dateLayout = new QHBoxLayout();
    dateLayout->addWidget(new QLabel(tr("选择日期:")));
    QDateEdit* dateEdit = new QDateEdit(QDate::currentDate(), this);
    dateEdit->setCalendarPopup(true);
    dateLayout->addWidget(dateEdit);
    dateLayout->addStretch();
    mainLayout->addLayout(dateLayout);

    // 创建时刻表
    scheduleTable = new QTableWidget(0, 6, this);
    scheduleTable->setHorizontalHeaderLabels(
        {tr("车次"), tr("始发站"), tr("终点站"), tr("发车时间"), tr("到达时间"), tr("票价")}
        );
    scheduleTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    scheduleTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    scheduleTable->setAlternatingRowColors(true);
    mainLayout->addWidget(scheduleTable);

    // 添加一些示例数据
    scheduleTable->setRowCount(5);

    QStringList routeIds = {"G1234", "D5678", "K9012", "T3456", "Z7890"};
    QStringList departures = {"北京", "上海", "广州", "深圳", "杭州"};
    QStringList arrivals = {"上海", "广州", "深圳", "杭州", "北京"};
    QStringList departTimes = {"08:00", "09:30", "10:15", "12:00", "14:30"};
    QStringList arriveTimes = {"12:30", "14:00", "15:45", "16:30", "20:00"};
    QStringList prices = {"553元", "428元", "372元", "289元", "498元"};

    for (int row = 0; row < 5; ++row) {
        scheduleTable->setItem(row, 0, new QTableWidgetItem(routeIds[row]));
        scheduleTable->setItem(row, 1, new QTableWidgetItem(departures[row]));
        scheduleTable->setItem(row, 2, new QTableWidgetItem(arrivals[row]));
        scheduleTable->setItem(row, 3, new QTableWidgetItem(departTimes[row]));
        scheduleTable->setItem(row, 4, new QTableWidgetItem(arriveTimes[row]));
        scheduleTable->setItem(row, 5, new QTableWidgetItem(prices[row]));
    }

    scheduleTable->resizeColumnsToContents();
}

void MainWindow::changeToolButtonStyle(int index)
{
    Qt::ToolButtonStyle style = static_cast<Qt::ToolButtonStyle>(
        styleComboBox->itemData(index).toInt());

    mainToolBar->setToolButtonStyle(style);
    refreshStatusMessage(tr("工具按钮样式已更改"));
}

void MainWindow::refreshStatusMessage(const QString& message)
{
    statusLabel->setText(message);
    statusBar()->showMessage(message, 3000);
}

// 槽函数实现
void MainWindow::handleNewRoute()
{
    refreshStatusMessage(tr("新建路线功能已触发"));
    QMessageBox::information(this, tr("新建路线"), tr("此功能将允许创建新的客运路线。"));
}

void MainWindow::handleEditRoute()
{
    refreshStatusMessage(tr("编辑路线功能已触发"));
    QMessageBox::information(this, tr("编辑路线"), tr("此功能将允许编辑选中的客运路线。"));
}

void MainWindow::handleDeleteRoute()
{
    refreshStatusMessage(tr("删除路线功能已触发"));
    QMessageBox::warning(this, tr("删除路线"), tr("确定要删除选中的客运路线吗？"));
}

void MainWindow::handlePrintSchedule()
{
    refreshStatusMessage(tr("打印时刻表功能已触发"));
    QMessageBox::information(this, tr("打印时刻表"), tr("此功能将允许打印当前显示的客运时刻表。"));
}

void MainWindow::handleExportSchedule()
{
    refreshStatusMessage(tr("导出时刻表功能已触发"));
    QMessageBox::information(this, tr("导出时刻表"), tr("此功能将允许将时刻表导出为PDF或Excel文件。"));
}

void MainWindow::handleDirections(QAction* action)
{
    QString direction = action->data().toString();
    QString directionText;

    if (direction == "north")
        directionText = tr("北线路线");
    else if (direction == "south")
        directionText = tr("南线路线");
    else if (direction == "east")
        directionText = tr("东线路线");
    else if (direction == "west")
        directionText = tr("西线路线");

    refreshStatusMessage(tr("已选择 ") + directionText);
    QMessageBox::information(this, tr("方向选择"), tr("已选择 ") + directionText);
}

void MainWindow::handleSearch()
{
    refreshStatusMessage(tr("搜索功能已触发"));
    QMessageBox::information(this, tr("搜索"), tr("此功能将允许搜索客运路线和时刻。"));
}

void MainWindow::handleSettings()
{
    refreshStatusMessage(tr("设置功能已触发"));
    QMessageBox::information(this, tr("设置"), tr("此功能将允许调整应用程序设置。"));
}

void MainWindow::handleHelp()
{
    refreshStatusMessage(tr("帮助功能已触发"));
    QMessageBox::information(this, tr("帮助"), tr("此功能将显示客运信息系统的帮助内容。"));
}

void MainWindow::handleAbout()
{
    QMessageBox::about(this, tr("关于客运信息系统"),
                       tr("<h3>客运信息系统</h3>"
                          "<p>版本 1.0</p>"
                          "<p>这是一个演示QToolButton类用法的示例应用。</p>"
                          "<p>© 2023 Qt学习示例</p>"));
}
