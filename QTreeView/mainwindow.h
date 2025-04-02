#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTreeView>
#include <QDebug>
#include <QStyledItemDelegate>
#include <QApplication>
#include <QPainter>

// Forward declarations
class LeafButtonDelegate;

class AlignDelegate : public QStyledItemDelegate {
public:
    using QStyledItemDelegate::QStyledItemDelegate;

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override {
        QStyleOptionViewItem opt = option;
        initStyleOption(&opt, index);

        // 绘制复选框
        QRect checkboxRect = QRect(opt.rect.left() + 2, opt.rect.center().y() - 8, 16, 16);
        bool checked = (index.data(Qt::CheckStateRole).toInt() == Qt::Checked);
        QStyle::State state = checked ? QStyle::State_On : QStyle::State_Off;
        QApplication::style()->drawPrimitive(QStyle::PE_IndicatorItemViewItemCheck, &opt, painter);

        // 调整文本位置
        QRect textRect = opt.rect.adjusted(24, 0, 0, 0); // 文本向右偏移24px
        painter->drawText(textRect, Qt::AlignLeft | Qt::AlignVCenter, index.data().toString());
    }

    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override {
        return QSize(200, 24); // 固定项的大小
    }
};

class DynamicTreeView : public QTreeView {
public:
    explicit DynamicTreeView(QWidget *parent = nullptr) : QTreeView(parent) {
        setStyleSheet("QTreeView { border: none; padding: 0; }");
        setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    }
    QSize sizeHint() const override {
        // 计算可见行数（包含展开的子项）
        int visibleRows = calculateVisibleRows(rootIndex());
        int height = visibleRows * sizeHintForRow(0);
        height = qMin(height, 200); // 限制最大高度
        return {width(), height};
    }
private:
    int calculateVisibleRows(const QModelIndex &parent) const {
        int count = 0;
        const int rowCount = model()->rowCount(parent);
        for (int i = 0; i < rowCount; ++i) {
            const QModelIndex index = model()->index(i, 0, parent);
            ++count; // 当前行
            if (isExpanded(index)) { // 递归计算展开的子项
                count += calculateVisibleRows(index);
            }
        }
        return count;
    }
protected:
    void updateGeometries() override {
        QTreeView::updateGeometries();
    }
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onLeafClicked(const QModelIndex &leafIndex);
    void onLeafDeleted(const QModelIndex &leafIndex);

private:
    DynamicTreeView *tree1;
    DynamicTreeView *tree2;
    LeafButtonDelegate *leafDelegate;

private:
    DynamicTreeView* createTreeView(const QString &name);
    void setupModel(DynamicTreeView *tv);
    void connectSignals();
};

#endif // MAINWINDOW_H
