#include "mainwindow.h"
#include <QVBoxLayout>
#include <QStandardItemModel>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    QWidget *central = new QWidget;
    QVBoxLayout *layout = new QVBoxLayout(central);
    layout->setSpacing(0);
    layout->setContentsMargins(0, 0, 0, 0);

    // Tree1 设置
    tree1 = createTreeView("Tree A");
    tree1->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

    // Tree2 固定高度设置
    tree2 = createTreeView("Tree B");
    tree2->setFixedHeight(200); // 固定高度
    tree2->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    layout->addWidget(tree1);
    layout->addSpacing(20); // 固定间隔
    layout->addWidget(tree2);
    layout->addStretch(); // 添加拉伸保证间隔不变

    setupModel(tree1);
    setupModel(tree2);
    connectSignals();

    setCentralWidget(central);
    resize(400, 500); // 固定窗口高度
}

MainWindow::~MainWindow()
{
}

DynamicTreeView *MainWindow::createTreeView(const QString &name)
{
    DynamicTreeView *tv = new DynamicTreeView(this);
    tv->setHeaderHidden(true);
    tv->setExpandsOnDoubleClick(false);
    return tv;
}


void MainWindow::setupModel(DynamicTreeView *tv)
{
    QStandardItemModel *model = new QStandardItemModel(tv);
   model->setHorizontalHeaderLabels({"Dynamic Content"});

   // 创建三级嵌套结构
   for (int i = 1; i <= 3; ++i) {
       QStandardItem *root = new QStandardItem(QString("Root %1").arg(i));
       root->setCheckable(true);  // 启用复选框
       root->setEditable(false);  // 禁止编辑文本
       model->appendRow(root);

       for (int j = 1; j <= 2; ++j) {
           QStandardItem *child = new QStandardItem(QString("Child %1-%2").arg(i).arg(j));
           root->appendRow(child);
           child->setCheckable(true);
           child->setEditable(false);

           for (int k = 1; k <= 2; ++k) {
               child->appendRow(new QStandardItem(QString("Leaf %1-%2-%3").arg(i).arg(j).arg(k)));
           }
       }
   }

   tv->setModel(model);
   tv->expandAll();
}

void MainWindow::connectSignals()
{
    auto updateLayout = [this]{
        tree1->updateGeometry(); //会强制出发sizehint进行重新计算
        tree2->updateGeometry();
        centralWidget()->layout()->activate();
        qDebug() << "tree1";
        qDebug() << tree1->geometry();
        qDebug() << "tree2";
        qDebug() << tree2->geometry();
    };

    connect(tree1, &QTreeView::expanded, updateLayout);
    connect(tree1, &QTreeView::collapsed, updateLayout);
    connect(tree2, &QTreeView::expanded, updateLayout);
    connect(tree2, &QTreeView::collapsed, updateLayout);
}

