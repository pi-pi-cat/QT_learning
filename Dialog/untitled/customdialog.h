#ifndef CUSTOMDIALOG_H
#define CUSTOMDIALOG_H

#include <QDialog>
#include <QEvent>
#include <QResizeEvent>

class CustomDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CustomDialog(QWidget *parent = nullptr);
    ~CustomDialog();

protected:
    bool eventFilter(QObject *obj, QEvent *event);
};

#endif // CUSTOMDIALOG_H
