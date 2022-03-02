#include "PropertyCommonControl.h"
#include <QBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include "qcoreevent.h"
#include <QDebug>
#include "qevent.h"
#include <QVariant>
#include <QStyle>
#include <QValidator>
#include <QSlider>
#include <QtCore/qmath.h>  
#include <QTimer>
#include <QColorDialog>
#include "QFileDialog"
#include <QCheckBox>
#include <QListView>
#include "QStackedWidget"
#include "QButtonGroup"
#include "QImage"

/**
	PropertyItemWidget
*/

PropertyItemWidget::PropertyItemWidget(const QString& title, QWidget* parent /*= nullptr*/, const QString& propertyName):QFrame(parent),titleContent_(title)
{
	InitUI(propertyName);
}

PropertyItemWidget::~PropertyItemWidget()
{

}

void PropertyItemWidget::addWidget(QWidget* widget)
{
	mainLayout_->addWidget(widget);
}

void PropertyItemWidget::InitUI(const QString& propertyName)
{
	mainLayout_ = new QHBoxLayout(this);
	mainLayout_->setSpacing(4);
	mainLayout_->setMargin(0);
	mainLayout_->setContentsMargins(0, 0, 4, 0);

	titleLabel_ = new QLabel(this);
	titleLabel_->setObjectName("titleLabel");
	titleLabel_->setText(titleContent_);
	titleLabel_->setFocusPolicy(Qt::ClickFocus);
	mainLayout_->addWidget(titleLabel_);
	mainLayout_->addStretch();
	this->setProperty("state", propertyName);
	if (this->property("state") == "expanded")
	{
		this->setFixedHeight(50);
	}
	else
	{
		this->setFixedHeight(30);
	}
}

/**
	StringLineEdit
*/

StringLineEdit::StringLineEdit(const QString& content /*= QString()*/, QWidget* parent /*= nullptr*/):QWidget(parent),currentContent_(content)
{
	InitUI();
}

StringLineEdit::~StringLineEdit()
{

}

void StringLineEdit::setText(const QString& text)
{
	currentContent_ = text;
	contentLabel_->setText(text);
}


void StringLineEdit::setValidator(const QValidator* validator)
{
	lineEdit_->setValidator(validator);
}

bool StringLineEdit::eventFilter(QObject* watched, QEvent* event)
{
	if (watched == contentLabel_)         		// 首先判断控件(这里指 lineEdit1)
	{
		if (event->type() == QEvent::MouseButtonPress)     	// 然后再判断控件的具体事件 (这里指获得焦点事件)
		{
			UpdateLineEdit();
		}
	}
	else if (watched == lineEdit_)         		// 首先判断控件(这里指 lineEdit1)
	{
		if (event->type() == QEvent::Hide)
		{
			QPoint p = QCursor::pos();
			QPoint topLeft = btnEdit_->mapToGlobal(QPoint(0, 0));
			QRect r = QRect(topLeft.x()-2, topLeft.y()-2, btnEdit_->width()+4, btnEdit_->height()+4);
			if (!r.contains(p))
			{
				bEdit_ = false;
			}
			btnEdit_->setProperty("state", "uncheck");
			style()->unpolish(btnEdit_);
			style()->polish(btnEdit_);
		}
		else if (event->type() == QEvent::Show)
		{
			btnEdit_->setProperty("state", "check");
			style()->unpolish(btnEdit_);
			style()->polish(btnEdit_);
			bEdit_ = true;

		}
	}
	return QWidget::eventFilter(watched, event);     	// 最后将事件交给上层对话框

}

//void StringLineEdit::leaveEvent(QEvent* event)
//{
//	if (!lineEdit_->isHidden() && lineEdit_->hasFocus())
//	{
//		return;
//	}
//	qDebug() << "1111" << btnEdit_->property("state");
//	btnEdit_->setProperty("state", "uncheck");
//	style()->unpolish(btnEdit_);
//	style()->polish(btnEdit_);
//	contentLabel_->setVisible(true);
//	lineEdit_->setVisible(false);
//	lineEdit_->clearFocus();
//	qDebug() << "222" << btnEdit_->property("state");
//}

void StringLineEdit::InitUI()
{
	QHBoxLayout* mainLayout = new QHBoxLayout(this);
	mainLayout->setSpacing(4);
	mainLayout->setMargin(0);

	contentLabel_ = new QLabel(this);
	contentLabel_->setObjectName("contentLabel");
	contentLabel_->setText(currentContent_);
	contentLabel_->installEventFilter(this);
	mainLayout->addWidget(contentLabel_);

	lineEdit_ = new QLineEdit(this);
	lineEdit_->setObjectName("lineEdit");
	lineEdit_->setText(currentContent_);
	lineEdit_->hide();
	lineEdit_->installEventFilter(this);
	mainLayout->addWidget(lineEdit_);

	btnEdit_ = new QPushButton(this);
	btnEdit_->setObjectName("btnEdit");
	btnEdit_->setProperty("state", "uncheck");
	btnEdit_->setFocusPolicy(Qt::NoFocus);
	connect(btnEdit_, &QPushButton::clicked, this, &StringLineEdit::onBtnEditClick);
	mainLayout->addWidget(btnEdit_);
	bEdit_ = false;
}

void StringLineEdit::UpdateLineEdit()
{
	if (!lineEdit_->isHidden())
	{
		return;
	}
	contentLabel_->setVisible(false);
	QString strtext = currentContent_;
	lineEdit_->blockSignals(true);
	lineEdit_->setText(currentContent_);
	lineEdit_->setFocus();
	lineEdit_->selectAll();
	lineEdit_->blockSignals(false);
	lineEdit_->setVisible(true);
	lineEdit_->disconnect();      //注意：因为QLineEdit是根据焦点判断是否编辑结束的，所以这里要解决重复信号的问题
	connect(lineEdit_, &QLineEdit::textChanged, this, [&, strtext] {
		QString strMsg = lineEdit_->text();
		if (!strMsg.isEmpty() && strMsg != strtext)
		{
			setText(strMsg);
			emit textChangedSgn(strMsg);
		}

		});

	connect(lineEdit_, &QLineEdit::returnPressed, this, &StringLineEdit::onTextChanged);
	connect(lineEdit_, &QLineEdit::editingFinished, this, &StringLineEdit::onTextChanged);


}

void StringLineEdit::onBtnEditClick()
{
	if (bEdit_)
	{
		if (bEdit_)
		{
			bEdit_ = false;
		}
		contentLabel_->setVisible(true);
		lineEdit_->setVisible(false);
	}
	else
	{
		if (!bEdit_)
		{
			bEdit_ = true;
		}
		UpdateLineEdit();
	}
}

void StringLineEdit::onTextChanged()
{
	QString strMsg = lineEdit_->text();
	if (!strMsg.isEmpty() && strMsg != currentContent_)
	{
		setText(strMsg);
		emit textChangedSgn(strMsg);
	}
	contentLabel_->setVisible(true);
	lineEdit_->setVisible(false);
}

/**
	DoubleLineEdit
*/

DoubleLineEdit::DoubleLineEdit(int currValue, QWidget* parent /*= nullptr*/): QWidget(parent),value_(currValue)
{
	valueType_ = ValueType::v_Int;
	InitUI();
	intValidator_ = new QIntValidator(lineEdit_);
	lineEdit_->setValidator(intValidator_);
}

DoubleLineEdit::DoubleLineEdit(double currValue, QWidget* parent /*= nullptr*/) : QWidget(parent), value_(currValue)
{
	valueType_ = ValueType::v_Double;
	InitUI();
	doubleValidator_ = new QDoubleValidator(lineEdit_);
	lineEdit_->setValidator(doubleValidator_);
}

DoubleLineEdit::~DoubleLineEdit()
{

}

void DoubleLineEdit::setValue(int value)
{
	value_ = value;
	contentLabel_->setText(QString::number((int)value_));
}

void DoubleLineEdit::setValue(double value)
{
	value_ = value;
	contentLabel_->setText(QString::number(value_,'f',decimals_));
}

void DoubleLineEdit::setRange(double min, double max)
{
	minValue_ = min;
	maxValue_ = max;
	if (valueType_ == ValueType::v_Int)
	{
		if (intValidator_)
		{
			intValidator_->setRange(minValue_, maxValue_);
		}
	}
	else if (valueType_ == ValueType::v_Double)
	{
		if (doubleValidator_)
		{
			doubleValidator_->setRange(minValue_, maxValue_);
		}
	}
}

void DoubleLineEdit::setDecimals(int value)
{
	decimals_ = value;
	if (valueType_ == ValueType::v_Double)
	{
		if (doubleValidator_)
		{
			doubleValidator_->setDecimals(decimals_);
		}
		contentLabel_->setText(QString::number(value_, 'f', decimals_));
	}
}

void DoubleLineEdit::setSingleStep(int step)
{
	singleStep_ = step;
}

bool DoubleLineEdit::eventFilter(QObject* watched, QEvent* event)
{
	if (watched == contentLabel_)         		// 首先判断控件(这里指 lineEdit1)
	{
		if (event->type() == QEvent::MouseButtonPress)     	// 然后再判断控件的具体事件 (这里指获得焦点事件)
		{
			UpdateLineEdit();
		}
	
	}
	if (watched == lineEdit_)
	{
		if (event->type() == QEvent::Wheel)
		{
			QWheelEvent* wheelEvent = static_cast<QWheelEvent*>(event);
			zoomValue(wheelEvent->delta() > 0);
		}
		else if (event->type() == QEvent::KeyPress)
		{
			QKeyEvent* keyEvent = static_cast<QKeyEvent*>(event);
			if (keyEvent->key() == Qt::Key_Up)
			{
				zoomValue(true);
			}
			else if (keyEvent->key() == Qt::Key_Down)
			{
				zoomValue(false);
			}
			
		}
		else if (event->type() == QEvent::Hide)
		{
			QPoint p = QCursor::pos();
			QPoint topLeft = btnEdit_->mapToGlobal(QPoint(0, 0));
			QRect r = QRect(topLeft.x() - 2, topLeft.y() - 2, btnEdit_->width() + 4, btnEdit_->height() + 4);
			if (!r.contains(p))
			{
				bEdit_ = false;
			}
			btnEdit_->setProperty("state", "uncheck");
			style()->unpolish(btnEdit_);
			style()->polish(btnEdit_);
		}
		else if (event->type() == QEvent::Show)
		{
			btnEdit_->setProperty("state", "check");
			style()->unpolish(btnEdit_);
			style()->polish(btnEdit_);
			bEdit_ = true;

		}
	}

	return QWidget::eventFilter(watched, event);     	// 最后将事件交给上层对话框
}


void DoubleLineEdit::InitUI()
{
	QHBoxLayout* mainLayout = new QHBoxLayout(this);
	mainLayout->setSpacing(4);
	mainLayout->setMargin(0);

	contentLabel_ = new QLabel(this);
	contentLabel_->setObjectName("contentLabel");
	if (valueType_ == ValueType::v_Int)
	{
		contentLabel_->setText(QString::number(value_,'f',0));
	}
	else if (valueType_ == ValueType::v_Double)
	{
		contentLabel_->setText(QString::number(value_, 'f', decimals_));
	}
	
	contentLabel_->installEventFilter(this);
	mainLayout->addWidget(contentLabel_);

	lineEdit_ = new QLineEdit(this);
	lineEdit_->setObjectName("lineEdit");
	lineEdit_->installEventFilter(this);
	lineEdit_->hide();
	mainLayout->addWidget(lineEdit_);

	btnEdit_ = new QPushButton(this);
	btnEdit_->setObjectName("btnEdit");
	btnEdit_->setProperty("state", "uncheck");
	btnEdit_->setFocusPolicy(Qt::NoFocus);
	connect(btnEdit_, &QPushButton::clicked, this, &DoubleLineEdit::onBtnEditClick);
	mainLayout->addWidget(btnEdit_);
	bEdit_ = false;
}

void DoubleLineEdit::UpdateLineEdit()
{
	if (!lineEdit_->isHidden())
	{
		return;
	}
	contentLabel_->setVisible(false);
	QString strtext ;
	if (valueType_ == ValueType::v_Int)
	{
		strtext =QString::number(value_, 'f', 0);
	}
	else if (valueType_ == ValueType::v_Double)
	{
		strtext = QString::number(value_, 'f', decimals_);
	}
	lineEdit_->blockSignals(true);
	lineEdit_->setText(strtext);
	lineEdit_->setFocus();
	lineEdit_->selectAll();
	lineEdit_->blockSignals(false);
	lineEdit_->setVisible(true);
	lineEdit_->disconnect();      //注意：因为QLineEdit是根据焦点判断是否编辑结束的，所以这里要解决重复信号的问题
	connect(lineEdit_, &QLineEdit::editingFinished, this, &DoubleLineEdit::onTextChanged);
	connect(lineEdit_, &QLineEdit::returnPressed, this ,&DoubleLineEdit::onTextChanged);

	connect(lineEdit_, &QLineEdit::textChanged, this, [&, strtext] {
		QString strMsg = lineEdit_->text();
		if (!strMsg.isEmpty() && strMsg != strtext)
		{
			if (valueType_ == ValueType::v_Int)
			{
				try
				{
					auto val = strMsg.toInt();
					if (val <= (int)maxValue_ && val >=(int)minValue_)
					{
						value_ = val;
						setValue((int)value_);
						emit textChangedSgn(val);
					}
					else
					{
						lineEdit_->blockSignals(true);
						lineEdit_->setText(QString::number((int)value_));
						lineEdit_->blockSignals(false);
					}


				}
				catch (...)
				{
					lineEdit_->blockSignals(true);
					lineEdit_->setText(QString::number((int)value_));
					lineEdit_->blockSignals(false);
				}
				
			}
			else if (valueType_ == ValueType::v_Double)
			{
				try
				{
					auto val = strMsg.toDouble();
					if (val <= maxValue_ && val >= minValue_)
					{
						value_ = val;
						setValue(value_);
						emit textChangedSgn(val);
					}
					else
					{
						lineEdit_->blockSignals(true);
						lineEdit_->setText(QString::number(value_, 'f', decimals_));
						lineEdit_->blockSignals(false);
					}


				}
				catch (...)
				{
					lineEdit_->blockSignals(true);
					lineEdit_->setText(QString::number(value_, 'f', decimals_));
					lineEdit_->blockSignals(false);
				}
			}
		}

		});
}

void DoubleLineEdit::zoomValue(bool bAdd)
{
	double val = 0.0;

	if (bAdd)
	{
		val = value_ + singleStep_;
		if (val > maxValue_)
		{
			val = maxValue_;
		}
	}
	else
	{
		val = value_ - singleStep_;
		if (val < minValue_)
		{
			val = minValue_;
		}
	}
	
	if (valueType_ == ValueType::v_Int)
	{
		setValue((int)val);
		emit textChangedSgn((int)val);

	}
	else if (valueType_ == ValueType::v_Double)
	{
		setValue(val);
		emit textChangedSgn(val);
	}
	lineEdit_->blockSignals(true);
	lineEdit_->setText(contentLabel_->text());
	lineEdit_->blockSignals(false);
}

void DoubleLineEdit::onBtnEditClick()
{
	if (bEdit_)
	{
		if (bEdit_)
		{
			bEdit_ = false;
		}
		contentLabel_->setVisible(true);
		lineEdit_->setVisible(false);
	}
	else
	{
		if (!bEdit_)
		{
			bEdit_ = true;
		}
		UpdateLineEdit();
	}

}

void DoubleLineEdit::onTextChanged()
{
	QString strMsg = lineEdit_->text();
	if (!strMsg.isEmpty() && strMsg != contentLabel_->text())
	{
		if (valueType_ == ValueType::v_Int)
		{
			try
			{
				auto val = strMsg.toInt();
				if (val <= (int)maxValue_ && val >= (int)minValue_)
				{
					value_ = val;
					setValue((int)value_);
					emit textChangedSgn(val);
				}
				else
				{
					lineEdit_->blockSignals(true);
					lineEdit_->setText(QString::number((int)value_));
					lineEdit_->blockSignals(false);
				}


			}
			catch (...)
			{
				lineEdit_->blockSignals(true);
				lineEdit_->setText(QString::number((int)value_));
				lineEdit_->blockSignals(false);
			}

		}
		else if (valueType_ == ValueType::v_Double)
		{
			try
			{
				auto val = strMsg.toDouble();
				if (val <= maxValue_ && val >= minValue_)
				{
					value_ = val;
					setValue(value_);
					emit textChangedSgn(val);
				}
				else
				{
					lineEdit_->blockSignals(true);
					lineEdit_->setText(QString::number(value_, 'f', decimals_));
					lineEdit_->blockSignals(false);
				}


			}
			catch (...)
			{
				lineEdit_->blockSignals(true);
				lineEdit_->setText(QString::number(value_, 'f', decimals_));
				lineEdit_->blockSignals(false);
			}
		}
	}

	contentLabel_->setVisible(true);
	lineEdit_->setVisible(false);
}

/**
	LineSliderEdit
*/

LineSliderEdit::LineSliderEdit(int currValue, QWidget* parent /*= nullptr*/) : QWidget(parent), value_(currValue)
{
	valueType_ = ValueType::v_Int;
	InitUI();
	intValidator_ = new QIntValidator(lineEdit_);
	lineEdit_->setValidator(intValidator_);
}

LineSliderEdit::LineSliderEdit(double currValue, QWidget* parent /*= nullptr*/) : QWidget(parent), value_(currValue)
{
	valueType_ = ValueType::v_Double;
	InitUI();
	doubleValidator_ = new QDoubleValidator(lineEdit_);
	lineEdit_->setValidator(doubleValidator_);
}

LineSliderEdit::~LineSliderEdit()
{

}

void LineSliderEdit::setValue(int value)
{
	value_ = value;
	contentLabel_->setText(QString::number((int)value_));
	slider_->blockSignals(true);
	slider_->setValue(value);
	slider_->blockSignals(false);
}

void LineSliderEdit::setValue(double value)
{
	value_ = value;
	contentLabel_->setText(QString::number(value_, 'f', decimals_));
	slider_->blockSignals(true);
	slider_->setValue(value * qPow(10,decimals_));
	slider_->blockSignals(false);
}

void LineSliderEdit::setRange(double min, double max)
{
	minValue_ = min;
	maxValue_ = max;
	if (valueType_ == ValueType::v_Int)
	{
		if (intValidator_)
		{
			intValidator_->setRange(minValue_, maxValue_);
			slider_->setRange(minValue_, maxValue_);
		}
	}
	else if (valueType_ == ValueType::v_Double)
	{
		if (doubleValidator_)
		{
			doubleValidator_->setRange(minValue_, maxValue_);
			slider_->setRange(minValue_ * qPow(10, decimals_), maxValue_ * qPow(10, decimals_));
		}
	}
}

void LineSliderEdit::setDecimals(int value)
{
	decimals_ = value;
	if (valueType_ == ValueType::v_Double)
	{
		if (doubleValidator_)
		{
			doubleValidator_->setDecimals(decimals_);
			slider_->setRange(minValue_ *qPow(10, decimals_), maxValue_ * qPow(10, decimals_));
		}
		contentLabel_->setText(QString::number(value_, 'f', decimals_));
	}
}

void LineSliderEdit::setSingleStep(int step)
{
	singleStep_ = step;
}

bool LineSliderEdit::eventFilter(QObject* watched, QEvent* event)
{

	if (watched == contentLabel_)         		// 首先判断控件(这里指 lineEdit1)
	{
		if (event->type() == QEvent::MouseButtonPress)     	// 然后再判断控件的具体事件 (这里指获得焦点事件)
		{
			UpdateLineEdit();
			//UpdateBtnEdit();;
		}

	}
	if (watched == lineEdit_)
	{
		if (event->type() == QEvent::Wheel)
		{
			QWheelEvent* wheelEvent = static_cast<QWheelEvent*>(event);
			zoomValue(wheelEvent->delta() > 0);
		}
		else if (event->type() == QEvent::KeyPress)
		{
			QKeyEvent* keyEvent = static_cast<QKeyEvent*>(event);
			if (keyEvent->key() == Qt::Key_Up)
			{
				zoomValue(true);
			}
			else if (keyEvent->key() == Qt::Key_Down)
			{
				zoomValue(false);
			}

		}
		else if (event->type() == QEvent::Hide)
		{
			if (slider_->hasFocus())
			{
				slider_->clearFocus();
			}
			QPoint p = QCursor::pos();
			QPoint topLeft = btnEdit_->mapToGlobal(QPoint(0, 0));
			QRect r = QRect(topLeft.x() - 2, topLeft.y() - 2, btnEdit_->width() + 4, btnEdit_->height() + 4);
			if (!r.contains(p))
			{
				bEdit_ = false;
			}
			btnEdit_->setProperty("state", "uncheck");
			style()->unpolish(btnEdit_);
			style()->polish(btnEdit_);
			slider_->setProperty("state", "uncheck");
			style()->unpolish(slider_);
			style()->polish(slider_);
		}
		else if (event->type() == QEvent::Show)
		{
			btnEdit_->setProperty("state", "check");
			style()->unpolish(btnEdit_);
			style()->polish(btnEdit_);
			slider_->setProperty("state", "check");
			style()->unpolish(slider_);
			style()->polish(slider_);
			bEdit_ = true;

		}
	}

	return QWidget::eventFilter(watched, event);     	// 最后将事件交给上层对话框
}

void LineSliderEdit::leaveEvent(QEvent* event)
{
	if (!lineEdit_->isHidden() && lineEdit_->hasFocus() || slider_->hasFocus())
	{
		if (slider_->hasFocus())
		{
			if (sliderTimer_->isActive())
			{
				sliderTimer_->stop();
			}
			sliderTimer_->start(200);
		}
		return;
	}
	contentLabel_->setVisible(true);
	lineEdit_->disconnect();
	lineEdit_->setVisible(false);
	//slider_->clearFocus();
	/*lineEdit_->clearFocus();
	slider_->clearFocus();*/
	//UpdateBtnEdit();
}

void LineSliderEdit::InitUI()
{
	QVBoxLayout* mainLayout = new QVBoxLayout(this);
	mainLayout->setSpacing(2);
	mainLayout->setMargin(0);

	QHBoxLayout* hLayout = new QHBoxLayout();
	hLayout->setSpacing(4);
	hLayout->setMargin(0);

	contentLabel_ = new QLabel(this);
	contentLabel_->setObjectName("contentLabel");

	contentLabel_->installEventFilter(this);
	hLayout->addWidget(contentLabel_);

	lineEdit_ = new QLineEdit(this);
	lineEdit_->setObjectName("lineEdit");
	lineEdit_->installEventFilter(this);
	lineEdit_->hide();
	hLayout->addWidget(lineEdit_);

	btnEdit_ = new QPushButton(this);
	btnEdit_->setObjectName("btnEdit");
	btnEdit_->setProperty("state", "uncheck");
	btnEdit_->setFocusPolicy(Qt::NoFocus);
	connect(btnEdit_, &QPushButton::clicked, this, &LineSliderEdit::onBtnEditClick);
	hLayout->addWidget(btnEdit_);

	mainLayout->addLayout(hLayout);

	hLayout = new QHBoxLayout();
	hLayout->setSpacing(4);
	hLayout->setMargin(0);

	slider_ = new QSlider(Qt::Horizontal,this);
	slider_->setObjectName("slider");
	slider_->setProperty("state", "uncheck");
	connect(slider_, &QSlider::valueChanged, this, &LineSliderEdit::onSliderValueChanged);
	if (valueType_ == ValueType::v_Int)
	{
		contentLabel_->setText(QString::number(value_, 'f', 0));
		slider_->blockSignals(true);
		slider_->setValue(value_);
		slider_->blockSignals(false);
	}
	else if (valueType_ == ValueType::v_Double)
	{
		contentLabel_->setText(QString::number(value_, 'f', decimals_));
		slider_->blockSignals(true);
		slider_->setValue(value_ * qPow(10, decimals_));
		slider_->blockSignals(false);
	}
	hLayout->addWidget(slider_);
	mainLayout->addLayout(hLayout);
	//定时器，用来监控滑块滑动后，离开该控件后，失去焦点后处理
	sliderTimer_ = new QTimer(this);
	connect(sliderTimer_, &QTimer::timeout, this, &LineSliderEdit::onTimeOut);
}

void LineSliderEdit::UpdateLineEdit()
{
	if (!lineEdit_->isHidden())
	{
		return;
	}
	contentLabel_->setVisible(false);
	QString strtext;
	if (valueType_ == ValueType::v_Int)
	{
		strtext = QString::number(value_, 'f', 0);
	}
	else if (valueType_ == ValueType::v_Double)
	{
		strtext = QString::number(value_, 'f', decimals_);
	}
	lineEdit_->blockSignals(true);
	lineEdit_->setText(strtext);
	lineEdit_->setFocus();
	lineEdit_->selectAll();
	lineEdit_->blockSignals(false);
	lineEdit_->setVisible(true);
	lineEdit_->disconnect();      //注意：因为QLineEdit是根据焦点判断是否编辑结束的，所以这里要解决重复信号的问题
	connect(lineEdit_, &QLineEdit::editingFinished, this, &LineSliderEdit::onTextChanged);
	connect(lineEdit_, &QLineEdit::returnPressed, this, &LineSliderEdit::onTextChanged);

	connect(lineEdit_, &QLineEdit::textChanged, this, [&, strtext] {
		QString strMsg = lineEdit_->text();
		if (!strMsg.isEmpty() && strMsg != strtext)
		{
			if (valueType_ == ValueType::v_Int)
			{
				try
				{
					auto val = strMsg.toInt();
					if (val <= (int)maxValue_ && val >= (int)minValue_)
					{
						value_ = val;
						setValue((int)value_);
						emit textChangedSgn(val);
					}
					else
					{
						lineEdit_->blockSignals(true);
						lineEdit_->setText(QString::number((int)value_));
						lineEdit_->blockSignals(false);
						slider_->blockSignals(true);
						slider_->setValue((int)value_);
						slider_->blockSignals(false);
					}


				}
				catch (...)
				{
					lineEdit_->blockSignals(true);
					lineEdit_->setText(QString::number((int)value_));
					lineEdit_->blockSignals(false);
					slider_->blockSignals(true);
					slider_->setValue((int)value_);
					slider_->blockSignals(false);
				}

			}
			else if (valueType_ == ValueType::v_Double)
			{
				try
				{
					auto val = strMsg.toDouble();
					if (val <= maxValue_ && val >= minValue_)
					{
						value_ = val;
						setValue(value_);
						emit textChangedSgn(val);
					}
					else
					{
						lineEdit_->blockSignals(true);
						lineEdit_->setText(QString::number(value_, 'f', decimals_));
						lineEdit_->blockSignals(false);
						slider_->blockSignals(true);
						slider_->setValue(value_ * qPow(10, decimals_));
						slider_->blockSignals(false);
					}


				}
				catch (...)
				{
					lineEdit_->blockSignals(true);
					lineEdit_->setText(QString::number(value_, 'f', decimals_));
					lineEdit_->blockSignals(false);
					slider_->blockSignals(true);
					slider_->setValue(value_ * qPow(10, decimals_));
					slider_->blockSignals(false);
				}
			}
		}

		});
}

void LineSliderEdit::UpdateBtnEdit()
{
	lineEdit_->isHidden() ? btnEdit_->setProperty("state", "uncheck") : btnEdit_->setProperty("state", "check");
	lineEdit_->isHidden() ? slider_->setProperty("state", "uncheck") : slider_->setProperty("state", "check");
	style()->unpolish(btnEdit_);
	style()->polish(btnEdit_);
	style()->unpolish(slider_);
	style()->polish(slider_);
}

void LineSliderEdit::zoomValue(bool bAdd)
{
	double val = 0.0;

	if (bAdd)
	{
		val = value_ + singleStep_;
		if (val > maxValue_)
		{
			val = maxValue_;
		}
	}
	else
	{
		val = value_ - singleStep_;
		if (val < minValue_)
		{
			val = minValue_;
		}
	}

	if (valueType_ == ValueType::v_Int)
	{
		setValue((int)val);
		emit textChangedSgn((int)val);

	}
	else if (valueType_ == ValueType::v_Double)
	{
		setValue(val);
		emit textChangedSgn(val);
	}
	lineEdit_->blockSignals(true);
	lineEdit_->setText(contentLabel_->text());
	lineEdit_->blockSignals(false);
}

void LineSliderEdit::onBtnEditClick()
{
	if (bEdit_)
	{
		if (bEdit_)
		{
			bEdit_ = false;
		}
		contentLabel_->setVisible(true);
		lineEdit_->setVisible(false);
	}
	else
	{
		if (!bEdit_)
		{
			bEdit_ = true;
		}
		UpdateLineEdit();
	}
}

void LineSliderEdit::onTextChanged()
{
	QString strMsg = lineEdit_->text();
	if (!strMsg.isEmpty() && strMsg != contentLabel_->text())
	{
		if (valueType_ == ValueType::v_Int)
		{
			try
			{
				auto val = strMsg.toInt();
				if (val <= (int)maxValue_ && val >= (int)minValue_)
				{
					value_ = val;
					setValue((int)value_);
					emit textChangedSgn(val);
				}
				else
				{
					lineEdit_->blockSignals(true);
					lineEdit_->setText(QString::number((int)value_));
					lineEdit_->blockSignals(false);
					slider_->blockSignals(true);
					slider_->setValue((int)value_);
					slider_->blockSignals(false);
				}


			}
			catch (...)
			{
				lineEdit_->blockSignals(true);
				lineEdit_->setText(QString::number((int)value_));
				lineEdit_->blockSignals(false);
				slider_->blockSignals(true);
				slider_->setValue((int)value_);
				slider_->blockSignals(false);
			}

		}
		else if (valueType_ == ValueType::v_Double)
		{
			try
			{
				auto val = strMsg.toDouble();
				if (val <= maxValue_ && val >= minValue_)
				{
					value_ = val;
					setValue(value_);
					emit textChangedSgn(val);
				}
				else
				{
					lineEdit_->blockSignals(true);
					lineEdit_->setText(QString::number(value_, 'f', decimals_));
					lineEdit_->blockSignals(false);
					slider_->blockSignals(true);
					slider_->setValue(value_ * qPow(10, decimals_));
					slider_->blockSignals(false);
				}


			}
			catch (...)
			{
				lineEdit_->blockSignals(true);
				lineEdit_->setText(QString::number(value_, 'f', decimals_));
				lineEdit_->blockSignals(false);
				slider_->blockSignals(true);
				slider_->setValue(value_ * qPow(10, decimals_));
				slider_->blockSignals(false);
			}
		}
	}
	contentLabel_->setVisible(true);
	lineEdit_->setVisible(false);

}

void LineSliderEdit::onSliderValueChanged(int val)
{
	if (valueType_ == ValueType::v_Int)
	{
		value_ = val;
		contentLabel_->setText(QString::number(value_, 'f', 0));
	}
	else if (valueType_ == ValueType::v_Double)
	{
		value_ = val / qPow(10, decimals_);
		contentLabel_->setText(QString::number(value_, 'f', decimals_));
	}
	if (lineEdit_->isHidden())
	{
		lineEdit_->setVisible(true);
		contentLabel_->setVisible(false);
		//UpdateBtnEdit();
	}
	if (!lineEdit_->isHidden())
	{
		lineEdit_->blockSignals(true);
		lineEdit_->setText(contentLabel_->text());
		lineEdit_->blockSignals(false);
	}
	bEdit_ = true;
}

void LineSliderEdit::onTimeOut()
{
	if (!slider_->hasFocus())
	{
		sliderTimer_->stop();
		contentLabel_->setVisible(true);
		lineEdit_->setVisible(false);
	}
	
}

ColorLineEdit::ColorLineEdit(const QString& content /*= QString("None")*/, const QColor& color /*= QColor("#FFFFFF")*/, QWidget* parent /*= nullptr*/):QWidget(parent),currentContent_(content),color_(color)
{
	InitUI();
}

ColorLineEdit::~ColorLineEdit()
{

}

void ColorLineEdit::setText(const QString& text)
{
	currentContent_ = text;
	contentLabel_->setText(text);
}

void ColorLineEdit::setColor(const QColor& color)
{
	if (color_ != color)
	{
		color_ = color;
		btnColorSet_->setStyleSheet(QString("background:%1").arg(color.name()));
	}
	
}

bool ColorLineEdit::eventFilter(QObject* watched, QEvent* event)
{
	if (watched == contentLabel_)         		// 首先判断控件(这里指 lineEdit1)
	{
		if (event->type() == QEvent::MouseButtonPress)     	// 然后再判断控件的具体事件 (这里指获得焦点事件)
		{
			UpdateLineEdit();
		}
	}

	return QWidget::eventFilter(watched, event);     	// 最后将事件交给上层对话框

}

void ColorLineEdit::leaveEvent(QEvent* event)
{
	if (!lineEdit_->isHidden() && lineEdit_->hasFocus())
	{
		return;
	}
	contentLabel_->setVisible(true);
	lineEdit_->setVisible(false);
	lineEdit_->clearFocus();
}

void ColorLineEdit::InitUI()
{
	QHBoxLayout* mainLayout = new QHBoxLayout(this);
	mainLayout->setSpacing(4);
	mainLayout->setMargin(0);

	contentLabel_ = new QLabel(this);
	contentLabel_->setObjectName("contentLabel");
	contentLabel_->setText(currentContent_);
	contentLabel_->installEventFilter(this);
	mainLayout->addWidget(contentLabel_);

	lineEdit_ = new QLineEdit(this);
	lineEdit_->setObjectName("lineEdit");
	lineEdit_->setText(currentContent_);
	lineEdit_->hide();
	mainLayout->addWidget(lineEdit_);

	btnColorSet_ = new QPushButton(this);
	btnColorSet_->setObjectName("btnColorSet");
	btnColorSet_->setFocusPolicy(Qt::NoFocus);
	btnColorSet_->setStyleSheet(QString("background:%1").arg(color_.name()));
	connect(btnColorSet_, &QPushButton::clicked, this, &ColorLineEdit::onBtnSetColorClick);
	mainLayout->addWidget(btnColorSet_);
}

void ColorLineEdit::UpdateLineEdit()
{
	if (!lineEdit_->isHidden())
	{
		return;
	}
	contentLabel_->setVisible(false);
	QString strtext = currentContent_;
	lineEdit_->setText(currentContent_);
	lineEdit_->setVisible(true);
	lineEdit_->setFocus();
	lineEdit_->selectAll();
	lineEdit_->disconnect();      //注意：因为QLineEdit是根据焦点判断是否编辑结束的，所以这里要解决重复信号的问题
	connect(lineEdit_, &QLineEdit::textChanged, this, [&, strtext] {
		QString strMsg = lineEdit_->text();
		if (!strMsg.isEmpty() && strMsg != strtext)
		{
			setText(strMsg);
			emit colorChangedSgn(strMsg,color_);
		}

		});

	connect(lineEdit_, &QLineEdit::returnPressed, this, &ColorLineEdit::onTextChanged);
	connect(lineEdit_, &QLineEdit::editingFinished, this, &ColorLineEdit::onTextChanged);

}


void ColorLineEdit::onBtnSetColorClick()
{
	QColor color = QColorDialog::getColor(btnColorSet_->palette().color(QPalette::Background));
	if (color.isValid())
	{
		if (color_ != color)
		{
			setColor(color);
			setText(color.name());
			if (!lineEdit_->isHidden())
			{
				lineEdit_->blockSignals(true);
				lineEdit_->setText(color.name());
				lineEdit_->blockSignals(false);
			}

			emit colorChangedSgn(color.name(), color);
		}
		
	}
}

void ColorLineEdit::onTextChanged()
{
	QString strMsg = lineEdit_->text();
	if (!strMsg.isEmpty() && strMsg != currentContent_)
	{
		setText(strMsg);
		emit colorChangedSgn(strMsg,color_);
	}

	lineEdit_->disconnect();
	lineEdit_->setVisible(false);
	lineEdit_->clear();
	contentLabel_->setVisible(true);
	lineEdit_->clearFocus();
}

ImageSelectWidget::ImageSelectWidget(const QString& imagePath, QWidget* parent /*= nullptr*/) : QWidget(parent)
{
	InitUI(false);
	setImagePath(imagePath);
}

ImageSelectWidget::ImageSelectWidget(const QString& imagePath, bool bOpenEditor, QWidget* parent /*= nullptr*/) : QWidget(parent)
{
	InitUI(bOpenEditor);
	setImagePath(imagePath);
}

ImageSelectWidget::~ImageSelectWidget()
{

}

void ImageSelectWidget::setImagePath(const QString& imagePath)
{
	if (imagePath != imagePath_)
	{
		if (QFile::exists(imagePath))
		{
			imagePath_ = imagePath;
			QImage image(imagePath);
			if (!image.isNull())
			{
				image = image.scaled(imageLabel_->width(), imageLabel_->height(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
			}
			else
			{
				image = QImage();
			}

			imageLabel_->setPixmap(QPixmap::fromImage(image));
			imageDirPath_ = QFileInfo(imagePath_).absolutePath();
		}
		else
		{
			QImage image = QImage();
			if (!image.isNull())
			{
				image = image.scaled(imageLabel_->width(), imageLabel_->height(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
			}
			imageLabel_->setPixmap(QPixmap::fromImage(image));
		}
		
	}
}

void ImageSelectWidget::setImageFilter(const QString& imageFilter)
{
	if (imageFilter != imageFilter_)
	{
		imageFilter_ = imageFilter;
	}
}

void ImageSelectWidget::setImageDirPath(const QString& imageDir)
{
	if (imageDir != imageDirPath_)
	{
		imageDirPath_ = imageDir;
	}
}

bool ImageSelectWidget::eventFilter(QObject* watched, QEvent* event)
{
	if (watched == imageLabel_)         		// 首先判断控件(这里指 lineEdit1)
	{
		if (event->type() == QEvent::MouseButtonPress)     	// 然后再判断控件的具体事件 (这里指获得焦点事件)
		{
			onBtnSelectClick();
		}
	}

	return QWidget::eventFilter(watched, event);     	// 最后将事件交给上层对话框
}

void ImageSelectWidget::InitUI(bool bOpenEditor)
{
	QHBoxLayout* mainLayout = new QHBoxLayout(this);
	mainLayout->setSpacing(4);
	mainLayout->setMargin(0);

	imageLabel_ = new QLabel(this);
	imageLabel_->setObjectName("imageLabel");
	imageLabel_->installEventFilter(this);
	mainLayout->addWidget(imageLabel_);

	btnSelect_ = new QPushButton(this);
	btnSelect_->setObjectName("btnSelect");
	btnSelect_->setFocusPolicy(Qt::NoFocus);
	connect(btnSelect_, &QPushButton::clicked, this, &ImageSelectWidget::onBtnSelectClick);
	mainLayout->addWidget(btnSelect_);

	if (bOpenEditor)
	{
		btnOpenEditor_ = new QPushButton(this);
		btnOpenEditor_->setObjectName("btnOpenEditor");
		btnOpenEditor_->setToolTip(QObject::tr("Open Texture Editor"));
		btnOpenEditor_->setFocusPolicy(Qt::NoFocus);
		connect(btnOpenEditor_, &QPushButton::clicked, this, &ImageSelectWidget::onBtnOpenEditorClick);
		mainLayout->addWidget(btnOpenEditor_);
	}

	btnDelete_ = new QPushButton(this);
	btnDelete_->setObjectName("btnDelete");
	btnDelete_->setToolTip(QObject::tr("Delete"));
	btnDelete_->setFocusPolicy(Qt::NoFocus);
	connect(btnDelete_, &QPushButton::clicked, this, &ImageSelectWidget::onBtnDeleteClick);
	mainLayout->addWidget(btnDelete_);
}

void ImageSelectWidget::onBtnSelectClick()
{
	QString strImagePath = QFileDialog::getOpenFileName(nullptr, QObject::tr("Open File"), imageDirPath_, imageFilter_);
	if (strImagePath.isEmpty())
	{
		return;
	}

	if (!QFile::exists(strImagePath))
		return;

	if (strImagePath != imagePath_)
	{
		setImagePath(strImagePath);
		emit imageChangedSgn(strImagePath);
	}
}

void ImageSelectWidget::onBtnOpenEditorClick()
{
	//
}

void ImageSelectWidget::onBtnDeleteClick()
{
	//todo 若是Texture Editor 打开当前纹理，则要提示先关闭纹理编辑器
	setImagePath(QString());
	emit imageChangedSgn(imagePath_);
}

CheckBoxWidget::CheckBoxWidget(const QString& checkText, const QString& uncheckText, QWidget* parent /*= nullptr*/): QWidget(parent),checkText_(checkText),uncheckText_(uncheckText)
{
	InitUI();
}

CheckBoxWidget::~CheckBoxWidget()
{

}

void CheckBoxWidget::setShowText(const QString& checkText, const QString& uncheckText)
{
	checkText_ = checkText;
	uncheckText_ = uncheckText;
}

void CheckBoxWidget::setChecked(bool bCheck)
{
	checkBox_->setChecked(bCheck);
	onToggled(bCheck);
}

bool CheckBoxWidget::isChcecked() const
{
	return checkBox_->isChecked();
}

void CheckBoxWidget::InitUI()
{
	QHBoxLayout* mainLayout = new QHBoxLayout(this);
	mainLayout->setSpacing(4);
	mainLayout->setAlignment(Qt::AlignLeft);
	mainLayout->setMargin(0);

	checkBox_ = new QCheckBox(this);
	checkBox_->setObjectName("checkBox");
	mainLayout->addWidget(checkBox_);
	connect(checkBox_, &QCheckBox::toggled, this, &CheckBoxWidget::onToggled);
	mainLayout->addStretch();
	setChecked(false);
}

void CheckBoxWidget::onToggled(bool bCheck)
{
	checkBox_->setText(checkBox_->isChecked() ? checkText_ : uncheckText_);
	emit checkStateSgn(checkBox_->isChecked());
}

ComboBoxSaveOpenWidget::ComboBoxSaveOpenWidget(bool bOpen, bool bSave, bool bReset, QWidget* parent /*= nullptr*/):QWidget(parent)
{
	InitUI(bOpen, bSave, bReset);
}

ComboBoxSaveOpenWidget::~ComboBoxSaveOpenWidget()
{

}

void ComboBoxSaveOpenWidget::setItemList(const QStringList& itemList, const QStringList& itemDataList)
{
	if (itemList.count() != itemDataList.count())
	{
		return;
	}
	comboBox_->blockSignals(true);
	comboBox_->clear();
	for (int iLoop = 0; iLoop < itemList.count(); iLoop++)
	{
		comboBox_->addItem(itemList[iLoop], itemDataList[iLoop]);
	}
	comboBox_->blockSignals(false);
}

void ComboBoxSaveOpenWidget::setCurrentItemData(const QString& data)
{
	for (int iLoop = 0; iLoop < comboBox_->count(); iLoop++)
	{
		if (comboBox_->itemData(iLoop).toString() == data)
		{
			comboBox_->blockSignals(true);
			comboBox_->setCurrentIndex(iLoop);
			comboBox_->blockSignals(false);
			break;
		}
	}
}

QString ComboBoxSaveOpenWidget::getCurrentData() const
{
	return QString();
}

void ComboBoxSaveOpenWidget::setOpenFilter(const QString& filter)
{
	openFilter_ = filter;
}

void ComboBoxSaveOpenWidget::setSaveFilter(const QString& filter)
{
	saveFilter_ = filter;
}

void ComboBoxSaveOpenWidget::setDirPath(const QString& dir)
{
	dirPath_ = dir;
}

void ComboBoxSaveOpenWidget::InitUI(bool bOpen, bool bSave, bool bReset)
{
	QHBoxLayout* mainLayout = new QHBoxLayout(this);
	mainLayout->setSpacing(4);
	mainLayout->setMargin(0);

	comboBox_ = new QComboBox(this);
	comboBox_->setView(new QListView());
	comboBox_->setObjectName("comboBox");
	connect(comboBox_, SIGNAL(currentIndexChanged(int)), this, SLOT(onCurrentChanged(int)));
	mainLayout->addWidget(comboBox_);
	if (bOpen)
	{
		btnOpenFile_ = new QPushButton(this);
		btnOpenFile_->setObjectName("btnOpenFile");
		btnOpenFile_->setFocusPolicy(Qt::NoFocus);
		connect(btnOpenFile_, &QPushButton::clicked, this, &ComboBoxSaveOpenWidget::onOpenFileClick);
		mainLayout->addWidget(btnOpenFile_);
	}
	
	if (bSave)
	{
		btnSaveFile_ = new QPushButton(this);
		btnSaveFile_->setObjectName("btnSaveFile");
		btnSaveFile_->setFocusPolicy(Qt::NoFocus);
		connect(btnSaveFile_, &QPushButton::clicked, this, &ComboBoxSaveOpenWidget::onSaveFileClick);
		mainLayout->addWidget(btnSaveFile_);
	}
	if (bReset)
	{
		btnReset_ = new QPushButton(this);
		btnReset_->setObjectName("btnReset");
		btnReset_->setFocusPolicy(Qt::NoFocus);
		connect(btnReset_, &QPushButton::clicked, this, &ComboBoxSaveOpenWidget::onResetClick);
		mainLayout->addWidget(btnReset_);
	}
}

void ComboBoxSaveOpenWidget::onOpenFileClick()
{
	QString strImagePath = QFileDialog::getOpenFileName(nullptr, QObject::tr("Open File"), dirPath_, openFilter_);
	if (strImagePath.isEmpty())
	{
		return;
	}

	if (!QFile::exists(strImagePath))
		return;
	dirPath_ = QFileInfo(strImagePath).absolutePath();



}

void ComboBoxSaveOpenWidget::onSaveFileClick()
{
	QString strImagePath = QFileDialog::getSaveFileName(nullptr, QObject::tr("Save File"), dirPath_, saveFilter_);
	if (strImagePath.isEmpty())
	{
		return;
	}
	dirPath_ = strImagePath.mid(0, strImagePath.lastIndexOf("/"));
}

void ComboBoxSaveOpenWidget::onResetClick()
{

}

void ComboBoxSaveOpenWidget::onCurrentChanged(int index)
{
	emit currentDataChangedSgn(comboBox_->itemData(index).toString());
}

ButtonSaveOpenWidget::ButtonSaveOpenWidget(QWidget* parent /*= nullptr*/):QWidget(parent)
{
	InitUI();
}

ButtonSaveOpenWidget::~ButtonSaveOpenWidget()
{

}

void ButtonSaveOpenWidget::setOpenFilter(const QString& filter)
{
	if (openFilter_ != filter)
	{
		openFilter_ = filter;
	}
}

void ButtonSaveOpenWidget::setSaveFilter(const QString& filter)
{
	if (saveFilter_ != filter)
	{
		saveFilter_ = filter;
	}
}

void ButtonSaveOpenWidget::setDirPath(const QString& dir)
{
	if (dirPath_ != dir)
	{
		dirPath_ = dir;
	}
}

void ButtonSaveOpenWidget::InitUI()
{
	QHBoxLayout* mainLayout = new QHBoxLayout(this);
	mainLayout->setSpacing(4);
	mainLayout->setMargin(0);


	btnOpenFile_ = new QPushButton(this);
	btnOpenFile_->setObjectName("btnOpenFile");
	btnOpenFile_->setFocusPolicy(Qt::NoFocus);
	connect(btnOpenFile_, &QPushButton::clicked, this, &ButtonSaveOpenWidget::onOpenFileClick);
	mainLayout->addWidget(btnOpenFile_);

	btnSaveFile_ = new QPushButton(this);
	btnSaveFile_->setObjectName("btnSaveFile");
	btnSaveFile_->setFocusPolicy(Qt::NoFocus);
	connect(btnSaveFile_, &QPushButton::clicked, this, &ButtonSaveOpenWidget::onSaveFileClick);
	mainLayout->addWidget(btnSaveFile_);
}

void ButtonSaveOpenWidget::onOpenFileClick()
{
	QString strImagePath = QFileDialog::getOpenFileName(nullptr, QObject::tr("Open File"), dirPath_, openFilter_);
	if (strImagePath.isEmpty())
	{
		return;
	}

	if (!QFile::exists(strImagePath))
		return;
	dirPath_ = QFileInfo(strImagePath).absolutePath();

	emit openFileSgn(strImagePath);
	
}

void ButtonSaveOpenWidget::onSaveFileClick()
{
	QString strImagePath = QFileDialog::getSaveFileName(nullptr, QObject::tr("Save File"), dirPath_, saveFilter_);
	if (strImagePath.isEmpty())
	{
		return;
	}
	dirPath_ = strImagePath.mid(0,strImagePath.lastIndexOf("/"));
	emit saveFileSgn(strImagePath);
}

ShapeImageWidget::ShapeImageWidget(const QString& imagePath, QWidget* parent /*= nullptr*/):QWidget(parent)
{
	InitUI();
	setImagePath(imagePath);
}

ShapeImageWidget::~ShapeImageWidget()
{

}

void ShapeImageWidget::setImagePath(const QString& imagePath)
{
	if (imagePath != imagePath_)
	{
		if (QFile::exists(imagePath))
		{
			imagePath_ = imagePath;
			QImage image(imagePath);
			if (!image.isNull())
			{
				image = image.scaled(imageLabel_->width(), imageLabel_->height(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
			}
			else
			{
				image = QImage();
			}

			imageLabel_->setPixmap(QPixmap::fromImage(image));
		}
		else
		{
			QImage image = QImage();

			if (!image.isNull())
			{
				image = image.scaled(imageLabel_->width(), imageLabel_->height(), Qt::KeepAspectRatio, Qt::SmoothTransformation);

			}
			
			imageLabel_->setPixmap(QPixmap::fromImage(image));
		}

	}
}

void ShapeImageWidget::InitUI()
{
	QHBoxLayout* mainLayout = new QHBoxLayout(this);
	mainLayout->setSpacing(4);
	mainLayout->setMargin(0);

	imageLabel_ = new QLabel(this);
	imageLabel_->setObjectName("imageLabel");
	imageLabel_->installEventFilter(this);
	mainLayout->addWidget(imageLabel_);

	btnSelect_ = new QPushButton(this);
	btnSelect_->setObjectName("btnSelect");
	btnSelect_->setFocusPolicy(Qt::NoFocus);
	connect(btnSelect_, &QPushButton::clicked, this, &ShapeImageWidget::onBtnSelectClick);
	mainLayout->addWidget(btnSelect_);

	btnAdd_ = new QPushButton(this);
	btnAdd_->setObjectName("btnAdd");
	btnAdd_->setFocusPolicy(Qt::NoFocus);
	connect(btnAdd_, &QPushButton::clicked, this, &ShapeImageWidget::onBtnAddClick);
	mainLayout->addWidget(btnAdd_);
}

void ShapeImageWidget::onBtnSelectClick()
{

}

void ShapeImageWidget::onBtnAddClick()
{

}

PropertyItemGroupWidget::PropertyItemGroupWidget(QWidget* parent /*= Q_NULLPTR*/):QWidget(parent)
{
	InitUI();
}

PropertyItemGroupWidget::~PropertyItemGroupWidget()
{

}

void PropertyItemGroupWidget::addTopWidget(QWidget* w)
{
	if (topWidget_ == nullptr)
	{
		topWidget_ = new QWidget(this);
		topWidget_->setObjectName("topWidget");
		mainLayout_->addWidget(topWidget_);
		topLayout_ = new QHBoxLayout(topWidget_);
		topLayout_->setSpacing(6);
		topLayout_->setMargin(0);
		//topLayout_->setAlignment(Qt::AlignVCenter);

		btnExpanded_ = new QPushButton(this);
		btnExpanded_->setObjectName("btnExpanded");
		btnExpanded_->setProperty("state", "expanded"); //collapsed
		btnExpanded_->hide();
		connect(btnExpanded_, &QPushButton::clicked, this, &PropertyItemGroupWidget::onExpandedClicked);
		topLayout_->addWidget(btnExpanded_);
		leftWidget_ = new QWidget(this);
		leftWidget_->setObjectName("leftWidget");
		topLayout_->addWidget(leftWidget_);
	}
	topLayout_->addWidget(w);
	topWidget_->setFixedHeight(w->height());
	UpdateUI();
}

void PropertyItemGroupWidget::addSubWidget(QWidget* w)
{
	if (subWidget_ == nullptr)
	{
		subWidget_ = new QWidget(this);
		subWidget_->setObjectName("subWidget");
		mainLayout_->addWidget(subWidget_);

		subLayout_ = new QVBoxLayout(subWidget_);
		subLayout_->setSpacing(2);
		subLayout_->setMargin(0);
		subLayout_->setAlignment(Qt::AlignTop);
		if (topWidget_)
		{
			subLayout_->setContentsMargins(40, 0, 0, 0);
		}
		else
		{
			subLayout_->setContentsMargins(0, 0, 0, 0);
		}

	}
	if (subLayout_)
	{
		subLayout_->addWidget(w);
	}
}

void PropertyItemGroupWidget::setSubWidgetHeight()
{
	if (subWidget_)
	{
		/*subWidgetHeight_ = nHeight;
		subWidget_->setFixedHeight(nHeight);*/
		int nheight = 0;
		int nCount = 0;
		for (int iLoop =0; iLoop < subLayout_->count(); iLoop++)
		{
			QWidget* w = subLayout_->itemAt(iLoop)->widget();
			PropertyItemGroupWidget* sub = qobject_cast<PropertyItemGroupWidget*>(w);
			if (sub)
			{
				if (!sub->isHidden())
				{
					nheight += sub->GetHeight();
					nCount++;
				}

			}
			else
			{
				PropertyTabWidget* tab = qobject_cast<PropertyTabWidget*>(w);
				if (tab)
				{
					if (!tab->isHidden())
					{
						nheight += tab->GetHeight();
						nCount++;
					}
				}
				else if (!w->isHidden())
				{
					nheight += w->height();
					nCount++;
				}

			}
			
		}
		nheight +=( nCount +1) * subLayout_->spacing();
		subWidget_->setFixedHeight(nheight);
	}
	UpdateUI();
}

void PropertyItemGroupWidget::onExpandedClicked()
{
	if (btnExpanded_->property("state") == "expanded")
	{
		btnExpanded_->setProperty("state", "collapsed");
	}
	else
	{
		btnExpanded_->setProperty("state", "expanded");
	}
	style()->unpolish(btnExpanded_);
	style()->polish(btnExpanded_);
	setSubWidgetHeight();
	emit uiStateSgn();
}

void PropertyItemGroupWidget::InitUI()
{
	mainLayout_ = new QVBoxLayout(this);
	mainLayout_->setSpacing(2);
	mainLayout_->setMargin(0);
	mainLayout_->setAlignment(Qt::AlignTop);

}

void PropertyItemGroupWidget::UpdateUI()
{
	if (subWidget_ && subLayout_)
	{
		if (btnExpanded_)
		{
			btnExpanded_->setVisible(subLayout_->count() > 0);
			
		}
		if (leftWidget_)
		{
			leftWidget_->setVisible(!(subLayout_->count() > 0));
		}

	}
	int nHeight = 30;
	if (topWidget_)
	{
		nHeight = topWidget_->height();
	}
	if (btnExpanded_)
	{
		if (!btnExpanded_->isHidden())
		{
			if (btnExpanded_->property("state") == "expanded")
			{
				nHeight += subWidget_->height();
				subWidget_->setVisible(true);
			}
			else
			{
				subWidget_->setVisible(false);
			}
		}
		else
		{
			if (subWidget_)
			{
				subWidget_->setVisible(false);
			}
		}
	}
	else
	{
		nHeight = 0;
		if (subWidget_)
		{
			nHeight += subWidget_->height();
			subWidget_->setVisible(true);
		}
		
	}
	
	setFixedHeight(nHeight);
}

int PropertyItemGroupWidget::GetHeight()
{
	int nHeight = 0;
	if (topWidget_ && !topWidget_->isHidden())
	{
		nHeight += topWidget_->height();
	}
	if (subWidget_ && !subWidget_->isHidden())
	{
		int nCount = 0;
		int nSubHeight = 0;
		for (int iLoop = 0; iLoop < subLayout_->count(); iLoop++)
		{
			QWidget* w = subLayout_->itemAt(iLoop)->widget();
			PropertyItemGroupWidget* sub = qobject_cast<PropertyItemGroupWidget*>(w);
			if (sub)
			{
				if (!sub->isHidden())
				{
					nSubHeight += sub->GetHeight();
					nCount++;
				}

			}
			else
			{
				PropertyTabWidget* tab = qobject_cast<PropertyTabWidget*>(w);
				if (tab)
				{
					if (!tab->isHidden())
					{
						nSubHeight += tab->GetHeight();
						nCount++;
					}
				}
				else if (!w->isHidden())
				{
					nSubHeight += w->height();
					nCount++;
				}

			}
		}
		nSubHeight += (nCount + 1) * subLayout_->spacing();
		subWidget_->setFixedHeight(nSubHeight);
		nHeight += nSubHeight;
	}

	return nHeight;
}

PropertyLabel::PropertyLabel(QWidget* parent /*= nullptr*/, Qt::WindowFlags f /*= Qt::WindowFlags()*/):QLabel(parent,f)
{

}

PropertyLabel::PropertyLabel(const QString& text, QWidget* parent /*= nullptr*/, Qt::WindowFlags f /*= Qt::WindowFlags()*/):QLabel(text,parent,f)
{

}

PropertyLabel::~PropertyLabel()
{

}

PropertyComboBox::PropertyComboBox(QWidget* parent /*= nullptr*/): QComboBox(parent)
{
	setView(new QListView());
}

PropertyComboBox::~PropertyComboBox()
{

}

void PropertyComboBox::AddItemList(const QStringList& textList, const QStringList& dataList)
{
	if (textList.count() != dataList.count())
	{
		return;
	}
	blockSignals(true);
	clear();
	for (int iLoop = 0; iLoop < textList.count(); iLoop++)
	{
		addItem(textList[iLoop], dataList[iLoop]);
	}
	blockSignals(false);
}

PropertyTabWidget::PropertyTabWidget(QWidget* parent /*= nullptr*/):QFrame(parent)
{
	InitUI();
}

PropertyTabWidget::~PropertyTabWidget()
{

}

void PropertyTabWidget::addTab(const QString& tab, QWidget* widget)
{
	QPushButton* btn = new QPushButton(tabFrame_);
	int nIndex = buttonList_.count();
	btn->setObjectName(QString("btn%1").arg(nIndex));
	btn->setText(tab);
	btn->setCheckable(true);
	buttonList_.append(btn);
	btnGroup_->addButton(btn, nIndex);
	tabLayout_->addWidget(btn);

	stackedWidget_->insertWidget(nIndex, widget);
	UpdateUI();

}

void PropertyTabWidget::UpdateUI()
{
	int nHeight = GetHeight();
	setFixedHeight(nHeight);
}

int PropertyTabWidget::GetHeight()
{
	int nHeight = 0;
	if (tabFrame_ && !tabFrame_->isHidden())
	{
		nHeight += tabFrame_->height();
	}
	int nMax = 0;
	for (int iLoop = 0; iLoop < stackedWidget_->count(); iLoop++)
	{
		int nSubHeight = 0;
		QWidget* w = stackedWidget_->widget(iLoop);
		PropertyItemGroupWidget* sub = qobject_cast<PropertyItemGroupWidget*>(w);
		if (sub)
		{
			if (!sub->isHidden())
			{
				nSubHeight += sub->GetHeight();
			}

		}
		else
		{
			if (!w->isHidden())
			{
				nSubHeight += w->height();
			}

		}
		nMax = qMax(nMax, nSubHeight);
	}
    if (stackedWidget_->count()> 0)
    {
		nHeight += 2;
    }
	nHeight += nMax;
	return nHeight;
}

void PropertyTabWidget::InitUI()
{
	QVBoxLayout* mainLayout = new QVBoxLayout(this);
	mainLayout->setSpacing(2);
	mainLayout->setMargin(0);
	mainLayout->setAlignment(Qt::AlignTop);
	mainLayout->setContentsMargins(0, 0, 0, 0);

	tabFrame_ = new QFrame(this);
	tabFrame_->setObjectName("tabFrame");
	tabFrame_->setFrameShape(QFrame::NoFrame);
	mainLayout->addWidget(tabFrame_);
	QHBoxLayout* hLayout = new QHBoxLayout(tabFrame_);
	hLayout->setSpacing(4);
	hLayout->setMargin(0);

	tabLayout_ = new QHBoxLayout();
	tabLayout_->setSpacing(4);
	tabLayout_->setMargin(0);
	hLayout->addLayout(tabLayout_);
	hLayout->addStretch();

	stackedWidget_ = new QStackedWidget(this);
	stackedWidget_->setObjectName("stackedWidget");
	stackedWidget_->setFrameShape(QFrame::Shape::NoFrame);
	mainLayout->addWidget(stackedWidget_);
	btnGroup_ = new QButtonGroup(this);
	connect(btnGroup_, &QButtonGroup::idClicked, this, &PropertyTabWidget::onTabClicked);
}

void PropertyTabWidget::onTabClicked(int id)
{
	if (id >=0 && id < stackedWidget_->count())
	{
		stackedWidget_->setCurrentIndex(id);
	}
}
