#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QToolButton>
#include <QToolBar>
#include <QStatusBar>
#include <QLabel>
#include <QMenu>
#include <QAction>
#include <QTextEdit>
#include <QComboBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QTableWidget>
#include <QDateEdit>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void handleNewRoute();
    void handleEditRoute();
    void handleDeleteRoute();
    void handlePrintSchedule();
    void handleExportSchedule();
    void handleSearch();
    void handleSettings();
    void handleHelp();
    void handleDirections(QAction* action);
    void handleAbout();
    void changeToolButtonStyle(int index);
    void refreshStatusMessage(const QString& message);

private:
    void createActions();
    void createMenus();
    void createToolBars();
    void createStatusBar();
    void createCentralWidget();

    // 工具按钮
    QToolButton* createRouteButton();
    QToolButton* createPrintButton();
    QToolButton* createDirectionsButton();
    QToolButton* createSearchButton();
    QToolButton* createSettingsButton();
    QToolButton* createHelpButton();

    // 动作
    QAction* newRouteAction;
    QAction* editRouteAction;
    QAction* deleteRouteAction;
    QAction* printScheduleAction;
    QAction* exportScheduleAction;
    QAction* searchAction;
    QAction* settingsAction;
    QAction* helpAction;
    QAction* aboutAction;

    // 导航方向动作
    QAction* northAction;
    QAction* southAction;
    QAction* eastAction;
    QAction* westAction;

    // 主要组件
    QToolBar* mainToolBar;
    QMenu* routeMenu;
    QMenu* printMenu;
    QMenu* directionsMenu;
    QLabel* statusLabel;
    QComboBox* styleComboBox;
    QTableWidget* scheduleTable;
};

#endif // MAINWINDOW_H
