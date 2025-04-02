#ifndef LEAFBUTTONDELEGATE_H
#define LEAFBUTTONDELEGATE_H

#include <QStyledItemDelegate>
#include <QMap>
#include <QModelIndex>
#include <QRect>
#include <QSet>

class LeafButtonDelegate : public QStyledItemDelegate
{
    Q_OBJECT

public:
    explicit LeafButtonDelegate(QObject *parent = nullptr);
    ~LeafButtonDelegate() override;

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    bool editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index) override;
    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override;

signals:
    void leafClicked(const QModelIndex &leafIndex);
    void leafDeleted(const QModelIndex &leafIndex);

private:
    struct LeafInfo {
        QRect leafRect;
        QRect deleteButtonRect;
        bool isMoreButton = false;  // 标识是否为"..."按钮
    };

    // 存储叶节点按钮信息: 父节点索引 -> (叶节点索引 -> 叶节点信息)
    mutable QMap<QPersistentModelIndex, QMap<QPersistentModelIndex, LeafInfo>> m_leafButtonsInfo;

    // 额外存储"..."按钮信息: 父节点索引 -> "..."按钮信息
    mutable QMap<QPersistentModelIndex, LeafInfo> m_moreButtonsInfo;

    // 存储已展开显示所有叶节点的父节点
    mutable QSet<QPersistentModelIndex> m_expandedNodes;

    // 当前悬停的叶节点索引
    mutable QPersistentModelIndex m_hoverIndex;

    // 辅助方法
    bool isLeafNode(const QModelIndex &index) const;
    bool isChildNode(const QModelIndex &index) const;
    void updateLeafLayouts(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void paintLeafButtons(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void showLeafDetailsDialog(const QModelIndex &leafIndex) const;
    void showAllLeafNodes(const QModelIndex &parentIndex) const;
};

#endif // LEAFBUTTONDELEGATE_H
