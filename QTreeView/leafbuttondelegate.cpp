#include "leafbuttondelegate.h"
#include <QPainter>
#include <QMouseEvent>
#include <QMessageBox>
#include <QDialog>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QApplication>

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
        // If this is a child node, draw its leaf nodes as buttons
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
            m_hoverIndex = QPersistentModelIndex(); // Reset hover index

            // Check if hovering over any leaf
            for (auto it = m_leafButtonsInfo[QPersistentModelIndex(index)].begin();
                 it != m_leafButtonsInfo[QPersistentModelIndex(index)].end(); ++it) {
                if (it.value().leafRect.contains(pos)) {
                    m_hoverIndex = it.key();
                    break;
                }
            }

            // Request repaint if hover state changed
            if (oldHoverIndex != m_hoverIndex) {
                if (oldHoverIndex.isValid()) {
                    emit const_cast<LeafButtonDelegate*>(this)->sizeHintChanged(oldHoverIndex.parent());
                }
                if (m_hoverIndex.isValid()) {
                    emit const_cast<LeafButtonDelegate*>(this)->sizeHintChanged(m_hoverIndex.parent());
                }
            }
            break;
        }
        case QEvent::MouseButtonRelease: {
            QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
            QPoint pos = mouseEvent->pos();

            // Check if clicked on any leaf or delete button
            auto &leafMap = m_leafButtonsInfo[QPersistentModelIndex(index)];
            for (auto it = leafMap.begin(); it != leafMap.end(); ++it) {
                if (it.value().deleteButtonRect.contains(pos) && it.key() == m_hoverIndex) {
                    // Clicked on delete button (X)
                    emit leafDeleted(it.key());
                    return true;
                } else if (it.value().leafRect.contains(pos) && !it.value().deleteButtonRect.contains(pos)) {
                    // Clicked on leaf button (but not on X)
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
        // Make space for leaf buttons
        size.setHeight(qMax(size.height(), 40)); // Minimum height for child nodes
    }

    return size;
}

bool LeafButtonDelegate::isLeafNode(const QModelIndex &index) const
{
    // A leaf node is one that has no children and its parent has children
    return index.isValid() && !index.model()->hasChildren(index) &&
           index.parent().isValid() && index.model()->hasChildren(index.parent());
}

bool LeafButtonDelegate::isChildNode(const QModelIndex &index) const
{
    // A child node is one that has children and its parent is valid
    return index.isValid() && index.model()->hasChildren(index) && index.parent().isValid();
}

void LeafButtonDelegate::updateLeafLayouts(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    // Clear existing layout for this parent
    m_leafButtonsInfo[QPersistentModelIndex(index)].clear();

    const int LEAF_BUTTON_WIDTH = 80;
    const int LEAF_BUTTON_HEIGHT = 30;
    const int LEAF_BUTTON_SPACING = 5;
    const int DELETE_BUTTON_SIZE = 16;

    // Calculate available space for leaf buttons
    QRect contentRect = option.rect;
    int textWidth = painter->fontMetrics().width(index.data().toString()) + 40; // For Qt 5.12 use width() instead of horizontalAdvance()

    // Starting position for leaf buttons
    int startX = contentRect.left() + textWidth + 20; // 20px padding after text
    int centerY = contentRect.center().y();

    // Iterate through all children of this index
    for (int i = 0; i < index.model()->rowCount(index); i++) {
        QModelIndex leafIndex = index.model()->index(i, 0, index);
        if (isLeafNode(leafIndex)) {
            // Calculate leaf button rectangle
            QRect leafRect(startX, centerY - LEAF_BUTTON_HEIGHT/2,
                           LEAF_BUTTON_WIDTH, LEAF_BUTTON_HEIGHT);

            // Calculate delete button (X) rectangle
            QRect deleteRect(
                leafRect.right() - DELETE_BUTTON_SIZE - 2,
                leafRect.top() + 2,
                DELETE_BUTTON_SIZE,
                DELETE_BUTTON_SIZE
                );

            // Store info for this leaf
            LeafInfo info;
            info.leafRect = leafRect;
            info.deleteButtonRect = deleteRect;
            m_leafButtonsInfo[QPersistentModelIndex(index)][QPersistentModelIndex(leafIndex)] = info;

            // Move to next position
            startX += LEAF_BUTTON_WIDTH + LEAF_BUTTON_SPACING;
        }
    }
}

void LeafButtonDelegate::paintLeafButtons(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    painter->save();

    auto &leafMap = m_leafButtonsInfo[QPersistentModelIndex(index)];
    for (auto it = leafMap.begin(); it != leafMap.end(); ++it) {
        QModelIndex leafIndex = it.key();
        const LeafInfo &info = it.value();

        // Draw leaf button
        QColor buttonColor = (leafIndex == m_hoverIndex) ? QColor(220, 230, 255) : QColor(230, 230, 230);
        painter->setPen(QPen(Qt::gray));
        painter->setBrush(buttonColor);
        painter->drawRoundedRect(info.leafRect, 5, 5);

        // Draw leaf text
        painter->setPen(Qt::black);
        painter->drawText(info.leafRect, Qt::AlignCenter, leafIndex.data().toString());

        // Draw delete button (X) if this leaf is being hovered
        if (leafIndex == m_hoverIndex) {
            painter->setPen(QPen(Qt::red, 2));
            QRect xRect = info.deleteButtonRect;
            painter->drawLine(xRect.topLeft(), xRect.bottomRight());
            painter->drawLine(xRect.topRight(), xRect.bottomLeft());
        }
    }

    painter->restore();
}

void LeafButtonDelegate::showLeafDetailsDialog(const QModelIndex &leafIndex) const
{
    QDialog dialog(QApplication::activeWindow());
    dialog.setWindowTitle("Leaf Details");
    dialog.setMinimumSize(300, 200);

    QVBoxLayout *layout = new QVBoxLayout(&dialog);

    // Add leaf information
    QLabel *titleLabel = new QLabel(QString("<h2>%1</h2>").arg(leafIndex.data().toString()), &dialog);
    layout->addWidget(titleLabel);

    // Add placeholder for more details
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

    // Add close button
    QPushButton *closeButton = new QPushButton("Close", &dialog);
    layout->addWidget(closeButton);
    QObject::connect(closeButton, &QPushButton::clicked, &dialog, &QDialog::accept);

    dialog.exec();
}
