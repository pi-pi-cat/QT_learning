#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTreeView>
#include <QDebug>


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


private:
    DynamicTreeView *tree1;
    DynamicTreeView *tree2;
private:
    DynamicTreeView* createTreeView(const QString &name);
    void setupModel(DynamicTreeView *tv);
    void connectSignals();
};
#endif // MAINWINDOW_H
