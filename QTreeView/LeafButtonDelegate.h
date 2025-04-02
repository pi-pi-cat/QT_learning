#ifndef LEAFBUTTONDELEGATE_H
#define LEAFBUTTONDELEGATE_H

#include <QStyledItemDelegate>
#include <QMap>
#include <QModelIndex>
#include <QRect>

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
    };

    // Map to store leaf buttons info: parent index -> (leaf index -> leaf info)
    mutable QMap<QPersistentModelIndex, QMap<QPersistentModelIndex, LeafInfo>> m_leafButtonsInfo;

    // Current hovering leaf index
    mutable QPersistentModelIndex m_hoverIndex;

    // Helper methods
    bool isLeafNode(const QModelIndex &index) const;
    bool isChildNode(const QModelIndex &index) const;
    void updateLeafLayouts(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void paintLeafButtons(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void showLeafDetailsDialog(const QModelIndex &leafIndex) const;
};

#endif // LEAFBUTTONDELEGATE_H
