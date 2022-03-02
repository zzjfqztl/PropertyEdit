#include "MainWindow.h"
#include "QBoxLayout"
#include "QPushButton"
#include "Common/CustomControls.h"
#include <QTextEdit>
#include <QMessageBox>

#include "Common/MaskWidget.h"
#include "QDateTime"
#include "QLabel"
#include "QLineEdit"
#include "QFileInfo"
#include "QFileDialog"
#include "Dialog/MessageBoxDialog.h"
#include "PropertyBrowser/PropertyCommonControl.h"
#include "PropertyBrowser/PropertyEditorWidget.h"
#include <QButtonGroup>
#include <QStackedWidget>
#include <QFrame>


MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
	InitUI();
	showMaximized();
}

void MainWindow::onButtonClicked(int nid)
{
	if (nid >= 0 && nid < stackedWidget_->count())
	{
		stackedWidget_->setCurrentIndex(nid);
	}
}

void MainWindow::InitUI()
{
	QWidget* centerWidget_ = new QWidget(this);
	centerWidget_->setObjectName("centerWidget");
	setCentralWidget(centerWidget_);

	QVBoxLayout* vLayout = new QVBoxLayout();
	vLayout->setSpacing(10);
	vLayout->setMargin(0);
	vLayout->setContentsMargins(20, 10, 20, 10);
	centerWidget_->setLayout(vLayout);

	topFrame_ = new QFrame(centerWidget_);
	topFrame_->setObjectName("topFrame");
	topFrame_->setFrameShape(QFrame::Shape::NoFrame);
	vLayout->addWidget(topFrame_);

	QHBoxLayout* topLayout = new QHBoxLayout();
	topLayout->setSpacing(10);
	topLayout->setMargin(0);
	topLayout->setContentsMargins(10, 0, 10, 0);
	topFrame_->setLayout(topLayout);
	btnGroup_ = new QButtonGroup(this);
	QPushButton *btn = new QPushButton(topFrame_);
	btn->setObjectName("btnAvatar");
	btn->setToolTip(QObject::tr("Avatar"));
	btn->setCheckable(true);
	btn->setFocusPolicy(Qt::FocusPolicy::NoFocus);
	topLayout->addWidget(btn);
	btnGroup_->addButton(btn, 0);

	btn = new QPushButton(topFrame_);
	btn->setObjectName("btnFabric");
	btn->setToolTip(QObject::tr("Fabric"));
	btn->setCheckable(true);
	btn->setFocusPolicy(Qt::FocusPolicy::NoFocus);
	topLayout->addWidget(btn);
	btnGroup_->addButton(btn, 1);

	btn = new QPushButton(topFrame_);
	btn->setObjectName("btnButton");
	btn->setToolTip(QObject::tr("Button"));
	btn->setCheckable(true);
	btn->setFocusPolicy(Qt::FocusPolicy::NoFocus);
	topLayout->addWidget(btn);
	btnGroup_->addButton(btn, 2);

	btn = new QPushButton(topFrame_);
	btn->setObjectName("btnButtonhole");
	btn->setToolTip(QObject::tr("Buttonhole"));
	btn->setCheckable(true);
	btn->setFocusPolicy(Qt::FocusPolicy::NoFocus);
	topLayout->addWidget(btn);
	btnGroup_->addButton(btn, 3);
	topLayout->addStretch();

	stackedWidget_ = new QStackedWidget(this);
	stackedWidget_->setObjectName("stackedWidget");
	stackedWidget_->setFrameShape(QFrame::Shape::NoFrame);
	vLayout->addWidget(stackedWidget_);

	AvatarBodyEditor* avatorBodyEditor = new AvatarBodyEditor(centerWidget_);
	avatorBodyEditor->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	stackedWidget_->insertWidget(0, avatorBodyEditor);

	FabricEditor* fabricEditor = new FabricEditor(centerWidget_);
	fabricEditor->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	stackedWidget_->insertWidget(1, fabricEditor);

	ButtonEditor* buttonEditor = new ButtonEditor(centerWidget_);
	buttonEditor->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	stackedWidget_->insertWidget(2, buttonEditor);

	ButtonholeEditor* buttonholeEditor = new ButtonholeEditor(centerWidget_);
	buttonholeEditor->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	stackedWidget_->insertWidget(3, buttonholeEditor);
	connect(btnGroup_, SIGNAL(buttonClicked(int)), this, SLOT(onButtonClicked(int)));
}
