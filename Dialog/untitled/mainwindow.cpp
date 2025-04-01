#include "mainwindow.h"
#include "customdialog.h"
#include <QVBoxLayout>
#include <QWidget>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    // 创建中央部件
    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    // 创建布局
    QVBoxLayout *layout = new QVBoxLayout(centralWidget);

    // 创建按钮
    m_showDialogButton = new QPushButton("显示对话框", this);
    layout->addWidget(m_showDialogButton);

    // 连接按钮信号到槽
    connect(m_showDialogButton, &QPushButton::clicked, this, &MainWindow::onShowDialogClicked);

    // 设置窗口属性
    setMinimumSize(400, 300);
    setWindowTitle("对话框示例");
}

MainWindow::~MainWindow()
{
}

void MainWindow::onShowDialogClicked()
{
    // 创建对话框，将本窗口作为父窗口
    CustomDialog *dialog = new CustomDialog(this);

    // 模态显示对话框
    dialog->show();
}
