/**
 * 主窗体部分
 */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class ComboBoxFrame;
class QTextEdit;
class QCheckBox;
class QPushButton;
class FileSelectFrame;
class QLineEdit;
class QStackedWidget;
class QButtonGroup;
class QFrame;
class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);

public slots:
	void onButtonClicked(int nid);
private:
    /**
     * @brief 主界面初始化
    */
    void InitUI();

private:
	QFrame* topFrame_ = nullptr;
	QButtonGroup* btnGroup_ = nullptr;
    QStackedWidget* stackedWidget_ = nullptr;
    
};

#endif // MAINWINDOW_H
