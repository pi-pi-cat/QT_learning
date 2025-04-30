#include <QApplication>
#include <QDialog>
#include <QDialogButtonBox>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QDebug>

class DialogButtonBoxDemo : public QDialog
{
    Q_OBJECT

public:
    DialogButtonBoxDemo(QWidget *parent = nullptr) : QDialog(parent)
    {
        // 设置窗口标题
        setWindowTitle("QDialogButtonBox 示例");
        resize(400, 300);

        // 创建一个垂直布局作为主布局
        QVBoxLayout *mainLayout = new QVBoxLayout(this);

        // 添加标签
        QLabel *label1 = new QLabel("1. 使用标准按钮的按钮框：");
        mainLayout->addWidget(label1);

        // 方法1：使用标准按钮创建按钮框
        QDialogButtonBox *buttonBox1 = new QDialogButtonBox(QDialogButtonBox::Ok |
                                                            QDialogButtonBox::Cancel |
                                                            QDialogButtonBox::Apply);
        // 水平方向布局按钮
        buttonBox1->setOrientation(Qt::Horizontal);
        mainLayout->addWidget(buttonBox1);

        // 添加分隔
        mainLayout->addSpacing(20);
        QLabel *label2 = new QLabel("2. 使用自定义按钮的按钮框：");
        mainLayout->addWidget(label2);

        // 方法2：创建一个空的按钮框，然后添加自定义按钮
        QDialogButtonBox *buttonBox2 = new QDialogButtonBox(Qt::Horizontal);

        // 创建自定义按钮
        QPushButton *customBtn1 = new QPushButton("保存文件");
        QPushButton *customBtn2 = new QPushButton("不保存");
        QPushButton *customBtn3 = new QPushButton("取消操作");

        // 添加自定义按钮到按钮框，并指定角色
        buttonBox2->addButton(customBtn1, QDialogButtonBox::AcceptRole);
        buttonBox2->addButton(customBtn2, QDialogButtonBox::DestructiveRole);
        buttonBox2->addButton(customBtn3, QDialogButtonBox::RejectRole);

        mainLayout->addWidget(buttonBox2);

        // 添加分隔
        mainLayout->addSpacing(20);
        QLabel *label3 = new QLabel("3. 垂直布局的按钮框：");
        mainLayout->addWidget(label3);

        // 创建垂直布局的按钮框
        QDialogButtonBox *buttonBox3 = new QDialogButtonBox(Qt::Vertical);
        buttonBox3->addButton("选项 A", QDialogButtonBox::ActionRole);
        buttonBox3->addButton("选项 B", QDialogButtonBox::ActionRole);
        buttonBox3->addButton("选项 C", QDialogButtonBox::ActionRole);

        mainLayout->addWidget(buttonBox3);

        // 设置按钮框之间的位置关系
        mainLayout->addStretch(1);

        // 连接信号和槽
        connect(buttonBox1, &QDialogButtonBox::accepted, this, &DialogButtonBoxDemo::onAccepted);
        connect(buttonBox1, &QDialogButtonBox::rejected, this, &DialogButtonBoxDemo::onRejected);
        connect(buttonBox1, &QDialogButtonBox::clicked, this, &DialogButtonBoxDemo::onButtonClicked);

        connect(buttonBox2, &QDialogButtonBox::accepted, this, &DialogButtonBoxDemo::onAccepted);
        connect(buttonBox2, &QDialogButtonBox::rejected, this, &DialogButtonBoxDemo::onRejected);
        connect(buttonBox2, &QDialogButtonBox::clicked, this, &DialogButtonBoxDemo::onButtonClicked);

        connect(buttonBox3, &QDialogButtonBox::clicked, this, &DialogButtonBoxDemo::onButtonClicked);
    }

private slots:
    void onAccepted()
    {
        qDebug() << "接受操作 - accepted() 信号被触发";
        // 在实际应用中，这里可以执行保存数据等操作
    }

    void onRejected()
    {
        qDebug() << "拒绝操作 - rejected() 信号被触发";
        // 在实际应用中，这里可以执行关闭对话框等操作
        close();
    }

    void onButtonClicked(QAbstractButton *button)
    {
        qDebug() << "按钮被点击：" << button->text();

        // 获取按钮所在的按钮框
        QDialogButtonBox *buttonBox = qobject_cast<QDialogButtonBox*>(sender());
        if (buttonBox) {
            // 获取按钮的角色
            QDialogButtonBox::ButtonRole role = buttonBox->buttonRole(button);

            // 根据角色执行不同操作
            switch (role) {
            case QDialogButtonBox::AcceptRole:
                qDebug() << "AcceptRole - 接受操作";
                break;
            case QDialogButtonBox::RejectRole:
                qDebug() << "RejectRole - 拒绝操作";
                break;
            case QDialogButtonBox::DestructiveRole:
                qDebug() << "DestructiveRole - 破坏性操作";
                break;
            case QDialogButtonBox::ActionRole:
                qDebug() << "ActionRole - 动作操作";
                break;
            default:
                qDebug() << "其他角色";
                break;
            }
        }
    }
};

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    DialogButtonBoxDemo dialog;
    dialog.show();

    return app.exec();
}
