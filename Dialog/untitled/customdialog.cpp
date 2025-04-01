#include "customdialog.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QApplication>
#include <QDebug>

CustomDialog::CustomDialog(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle("自定义对话框");

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(new QLabel("这是一个自定义对话框", this));

    QPushButton *closeButton = new QPushButton("关闭", this);
    layout->addWidget(closeButton);

    // 连接关闭按钮的点击信号到对话框的关闭槽
    connect(closeButton, &QPushButton::clicked, this, &QDialog::accept);

    // 设置对话框属性
    setMinimumSize(300, 200);

    qApp->installEventFilter(this); // 应用级别过滤器


}

CustomDialog::~CustomDialog()
{
}

bool CustomDialog::eventFilter(QObject *obj, QEvent *event)
{
    // 在事件到达前拦截
    if (event->type() == QEvent::Resize) {
        // 判断事件是否真正针对自己
        if (obj == this ||
            (dynamic_cast<QResizeEvent*>(event) &&
             dynamic_cast<QWidget*>(obj) == this)) {
            qDebug() << "filter";

            return false; // 阻止继续传递
        }
    }
    return QObject::eventFilter(obj, event);
}
