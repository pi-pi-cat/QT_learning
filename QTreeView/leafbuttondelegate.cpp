#include "leafbuttondelegate.h"
#include <QPainter>
#include <QMouseEvent>
#include <QMessageBox>
#include <QDialog>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QApplication>
#include <QListWidget>

LeafButtonDelegate::LeafButtonDelegate(QObject *parent)
    : QStyledItemDelegate(parent)
{
}

LeafButtonDelegate::~LeafButtonDelegate()
{
}

void LeafButtonDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QStyledItemDelegate::paint(painter, option, index);

    if (isChildNode(index)) {
        // 如果这是一个子节点，将其叶节点绘制为按钮
        updateLeafLayouts(painter, option, index);
        paintLeafButtons(painter, option, index);
    }
}

bool LeafButtonDelegate::editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index)
{
    if (isChildNode(index)) {
        switch (event->type()) {
        case QEvent::MouseMove: {
            QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
            QPoint pos = mouseEvent->pos();

            QPersistentModelIndex oldHoverIndex = m_hoverIndex;
            m_hoverIndex = QPersistentModelIndex(); // 重置悬停索引

            // 检查是否悬停在任何叶节点上
            auto &leafMap = m_leafButtonsInfo[QPersistentModelIndex(index)];
            for (auto it = leafMap.begin(); it != leafMap.end(); ++it) {
                if (it.value().leafRect.contains(pos)) {
                    m_hoverIndex = it.key();
                    break;
                }
            }

            // 检查是否悬停在"..."按钮上
            auto it = m_moreButtonsInfo.find(QPersistentModelIndex(index));
            if (it != m_moreButtonsInfo.end() && it.value().leafRect.contains(pos)) {
                // 对于"..."按钮使用特殊的悬停索引
                m_hoverIndex = QPersistentModelIndex();
            }

            // 如果悬停状态改变，请求重绘
            if (oldHoverIndex != m_hoverIndex) {
                if (oldHoverIndex.isValid()) {
                    emit const_cast<LeafButtonDelegate*>(this)->sizeHintChanged(oldHoverIndex.parent());
                }
                if (m_hoverIndex.isValid()) {
                    emit const_cast<LeafButtonDelegate*>(this)->sizeHintChanged(m_hoverIndex.parent());
                }
                emit const_cast<LeafButtonDelegate*>(this)->sizeHintChanged(index);
            }
            break;
        }
        case QEvent::MouseButtonRelease: {
            QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
            QPoint pos = mouseEvent->pos();

            // 检查是否点击了"..."按钮
            auto moreIt = m_moreButtonsInfo.find(QPersistentModelIndex(index));
            if (moreIt != m_moreButtonsInfo.end() && moreIt.value().leafRect.contains(pos)) {
                // 点击了"..."按钮，展开显示所有叶节点
                m_expandedNodes.insert(QPersistentModelIndex(index));
                emit const_cast<LeafButtonDelegate*>(this)->sizeHintChanged(index);
                return true;
            }

            // 检查是否点击了任何叶节点或删除按钮
            auto &leafMap = m_leafButtonsInfo[QPersistentModelIndex(index)];
            for (auto it = leafMap.begin(); it != leafMap.end(); ++it) {
                if (it.value().deleteButtonRect.contains(pos) && it.key() == m_hoverIndex) {
                    // 点击了删除按钮(X)
                    emit leafDeleted(it.key());
                    return true;
                } else if (it.value().leafRect.contains(pos) && !it.value().deleteButtonRect.contains(pos)) {
                    // 点击了叶节点按钮(但不在X上)
                    emit leafClicked(it.key());
                    showLeafDetailsDialog(it.key());
                    return true;
                }
            }
            break;
        }
        default:
            break;
        }
    }

    return QStyledItemDelegate::editorEvent(event, model, option, index);
}

QSize LeafButtonDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QSize size = QStyledItemDelegate::sizeHint(option, index);

    if (isChildNode(index)) {
        // 为叶节点按钮预留空间
        size.setHeight(qMax(size.height(), 40)); // 子节点的最小高度
    }

    return size;
}

bool LeafButtonDelegate::isLeafNode(const QModelIndex &index) const
{
    // 叶节点是没有子节点但其父节点有子节点的节点
    return index.isValid() && !index.model()->hasChildren(index) &&
           index.parent().isValid() && index.model()->hasChildren(index.parent());
}

bool LeafButtonDelegate::isChildNode(const QModelIndex &index) const
{
    // 子节点是有子节点且其父节点有效的节点
    return index.isValid() && index.model()->hasChildren(index) && index.parent().isValid();
}

void LeafButtonDelegate::updateLeafLayouts(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    // 清除该父节点的现有布局
    m_leafButtonsInfo[QPersistentModelIndex(index)].clear();

    const int LEAF_BUTTON_WIDTH = 80;
    const int LEAF_BUTTON_HEIGHT = 30;
    const int LEAF_BUTTON_SPACING = 5;
    const int DELETE_BUTTON_SIZE = 16;
    const int MAX_VISIBLE_LEAFS = 2;  // 最多显示2个叶节点

    // 计算叶节点按钮的可用空间
    QRect contentRect = option.rect;
    int textWidth = painter->fontMetrics().width(index.data().toString()) + 40;

    // 叶节点按钮的起始位置
    int startX = contentRect.left() + textWidth + 20; // 文本后20px的间距
    int centerY = contentRect.center().y();

    QPersistentModelIndex persistentIndex(index);
    bool isExpanded = m_expandedNodes.contains(persistentIndex);

    // 获取此索引的所有子节点数量
    int totalLeafs = index.model()->rowCount(index);
    int visibleLeafs = isExpanded ? totalLeafs : qMin(totalLeafs, MAX_VISIBLE_LEAFS);

    // 遍历所有需要显示的叶节点
    for (int i = 0; i < visibleLeafs; i++) {
        QModelIndex leafIndex = index.model()->index(i, 0, index);
        if (isLeafNode(leafIndex)) {
            // 计算叶节点按钮矩形
            QRect leafRect(startX, centerY - LEAF_BUTTON_HEIGHT/2,
                           LEAF_BUTTON_WIDTH, LEAF_BUTTON_HEIGHT);

            // 计算删除按钮(X)矩形
            QRect deleteRect(
                leafRect.right() - DELETE_BUTTON_SIZE - 2,
                leafRect.top() + 2,
                DELETE_BUTTON_SIZE,
                DELETE_BUTTON_SIZE
                );

            // 存储该叶节点的信息
            LeafInfo info;
            info.leafRect = leafRect;
            info.deleteButtonRect = deleteRect;
            m_leafButtonsInfo[persistentIndex][QPersistentModelIndex(leafIndex)] = info;

            // 移动到下一个位置
            startX += LEAF_BUTTON_WIDTH + LEAF_BUTTON_SPACING;
        }
    }

    // 如果有更多叶节点且没有展开，添加"..."按钮
    if (!isExpanded && totalLeafs > MAX_VISIBLE_LEAFS) {
        QRect moreRect(startX, centerY - LEAF_BUTTON_HEIGHT/2,
                       LEAF_BUTTON_WIDTH/2, LEAF_BUTTON_HEIGHT);

        LeafInfo moreInfo;
        moreInfo.leafRect = moreRect;
        moreInfo.isMoreButton = true;
        m_moreButtonsInfo[persistentIndex] = moreInfo;
    } else {
        // 如果已展开或没有更多叶节点，移除"..."按钮
        m_moreButtonsInfo.remove(persistentIndex);
    }
}

void LeafButtonDelegate::paintLeafButtons(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    painter->save();

    QPersistentModelIndex persistentIndex(index);

    // 绘制叶节点按钮
    auto &leafMap = m_leafButtonsInfo[persistentIndex];
    for (auto it = leafMap.begin(); it != leafMap.end(); ++it) {
        QModelIndex leafIndex = it.key();
        const LeafInfo &info = it.value();

        // 绘制叶节点按钮
        QColor buttonColor = (leafIndex == m_hoverIndex) ? QColor(220, 230, 255) : QColor(230, 230, 230);
        painter->setPen(QPen(Qt::gray));
        painter->setBrush(buttonColor);
        painter->drawRoundedRect(info.leafRect, 5, 5);

        // 绘制叶节点文本
        painter->setPen(Qt::black);
        painter->drawText(info.leafRect, Qt::AlignCenter, leafIndex.data().toString());

        // 如果此叶节点正被悬停，绘制删除按钮(X)
        if (leafIndex == m_hoverIndex) {
            painter->setPen(QPen(Qt::red, 2));
            QRect xRect = info.deleteButtonRect;
            painter->drawLine(xRect.topLeft(), xRect.bottomRight());
            painter->drawLine(xRect.topRight(), xRect.bottomLeft());
        }
    }

    // 绘制"..."按钮（如果存在）
    auto moreIt = m_moreButtonsInfo.find(persistentIndex);
    if (moreIt != m_moreButtonsInfo.end()) {
        const LeafInfo &moreInfo = moreIt.value();

        // 绘制"..."按钮
        QColor moreButtonColor = QColor(200, 200, 200);
        painter->setPen(QPen(Qt::gray));
        painter->setBrush(moreButtonColor);
        painter->drawRoundedRect(moreInfo.leafRect, 5, 5);

        // 绘制"..."文本
        painter->setPen(Qt::black);
        painter->drawText(moreInfo.leafRect, Qt::AlignCenter, "...");
    }

    painter->restore();
}

void LeafButtonDelegate::showLeafDetailsDialog(const QModelIndex &leafIndex) const
{
    QDialog dialog(QApplication::activeWindow());
    dialog.setWindowTitle("Leaf Details");
    dialog.setMinimumSize(300, 200);

    QVBoxLayout *layout = new QVBoxLayout(&dialog);

    // 添加叶节点信息
    QLabel *titleLabel = new QLabel(QString("<h2>%1</h2>").arg(leafIndex.data().toString()), &dialog);
    layout->addWidget(titleLabel);

    // 添加更多详细信息的占位符
    QLabel *infoLabel = new QLabel(QString("Details for item %1:").arg(leafIndex.data().toString()), &dialog);
    layout->addWidget(infoLabel);

    QLabel *pathLabel = new QLabel(QString("Path: Root > %1 > %2")
                                       .arg(leafIndex.parent().data().toString())
                                       .arg(leafIndex.data().toString()), &dialog);
    layout->addWidget(pathLabel);

    QLabel *indexLabel = new QLabel(QString("Index: Row %1, Column %2")
                                        .arg(leafIndex.row())
                                        .arg(leafIndex.column()), &dialog);
    layout->addWidget(indexLabel);

    layout->addStretch();

    // 添加关闭按钮
    QPushButton *closeButton = new QPushButton("Close", &dialog);
    layout->addWidget(closeButton);
    QObject::connect(closeButton, &QPushButton::clicked, &dialog, &QDialog::accept);

    dialog.exec();
}
