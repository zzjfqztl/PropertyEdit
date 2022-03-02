#ifndef PROPERTYCOMMONCONTROL_H
#define PROPERTYCOMMONCONTROL_H
#include <QWidget>
#include <QFrame>
#include <QComboBox>
#include <QLabel>

class QLineEdit;
class QPushButton;
class QHBoxLayout;
class QValidator;
class QIntValidator;
class QDoubleValidator;
class QSlider;
class QTimer;
class QCheckBox;
class QHBoxLayout;
class QVBoxLayout;
class QButtonGroup;
class QStackedWidget;

/**
 * @brief 属性编辑框 控件，水平布局，需要使用者添加子控件
*/
class PropertyItemWidget : public QFrame
{
	Q_OBJECT

public:
	/**
	 * @brief 构造函数
	 * @param title QLabel内容
	*/
	explicit PropertyItemWidget(const QString& title, QWidget* parent = nullptr,const QString& propertyName = QString("normal"));
	~PropertyItemWidget();

	/**
	 * @brief 向控件中添加子控件
	 * @param widget 子控件指针
	*/
	void addWidget(QWidget* widget);
private:
	/**
	 * @brief 控件初始化
	*/
	void InitUI(const QString& propertyName);

private:
	QString titleContent_; //标题
	QLabel* titleLabel_ = nullptr; //放置标题内容的QLabel
	QHBoxLayout* mainLayout_ = nullptr;
};

/**
 * @brief 标签切换
*/
class PropertyTabWidget : public QFrame
{
	Q_OBJECT

public:
	explicit PropertyTabWidget(QWidget* parent = nullptr);
	~PropertyTabWidget();

	void addTab(const QString& tab, QWidget* widget);
	void UpdateUI();
	int GetHeight();
private:
	void InitUI();
private slots:
	void onTabClicked(int id);

private:
	QFrame* tabFrame_ = nullptr;
	QList<QPushButton*> buttonList_;
	QButtonGroup* btnGroup_ = nullptr;
	QStackedWidget* stackedWidget_ = nullptr;
	QHBoxLayout* tabLayout_ = nullptr;
};

/**
 * @brief 编辑框+按钮，可以输入任何信息
*/
class StringLineEdit : public QWidget
{
	Q_OBJECT

public:

	/**
	 * @brief 构造函数
	 * @param content 初始内容
	 * @param parent 父窗体指针
	*/
	explicit StringLineEdit(const QString& content = QString(),QWidget* parent = nullptr);
	~StringLineEdit();

	/**
	 * @brief 设置显示内容
	 * @param text 显示内容文本
	*/
	void setText(const QString& text);

	/**
	 * @brief 输入限制信息
	 * @param validator 限制对象指针
	*/
	void setValidator(const QValidator* validator);
	
signals:
	/**
	 * @brief 显示文本更改后发出的信号
	*/
	void textChangedSgn(const QString&);

protected:
	/**
	 * @brief 事件过滤器，主要实现点击显示文本QLabel显示输入框
	 * @param watched 监控对象
	 * @param event 事件对象
	 * @return true 截止，false 继续向上传递
	*/
	bool eventFilter(QObject* watched, QEvent* event);    		// 注意这里
	
	/**
	 * @brief 离开窗体事件
	 * @param event 事件指针
	*/
	//void leaveEvent(QEvent* event);

private:
	/**
	 * @brief 初始化窗体
	*/
	void InitUI();

	/**
	 * @brief 更新编辑输入框控件
	*/
	void UpdateLineEdit();

private slots:
	/**
	 * @brief 编辑按钮点击事件处理
	*/
	void onBtnEditClick();

	/**
	 * @brief 输入框输入信息变化后处理
	*/
	void onTextChanged();

private:
	QString currentContent_; //显示的文本
	QLabel* contentLabel_ = nullptr; //显示文本的QLabel
	QLineEdit* lineEdit_ = nullptr; //输入框，初始时隐藏
	QPushButton* btnEdit_ = nullptr; //编辑按钮
	bool bEdit_ = false;
};

/**
 * @brief 编辑框+按钮，只能输入整数或者浮点数，根据创造的对象而定，有数值范围
*/
class DoubleLineEdit : public QWidget
{
	Q_OBJECT

public:
	/**
	 * @brief 区分创建对象的类型
	*/
	enum class ValueType
	{
		v_Int =0, //只允许输入整数
		v_Double, //只允许输入浮点数
	};

	/**
	 * @brief 构造函数，创建只能输入整数的编辑输入框
	 * @param currValue 当前显示的值
	 * @param parent 父窗体指针
	*/
	explicit DoubleLineEdit(int currValue, QWidget* parent = nullptr);

	/**
	 * @brief 构造函数，创建只能输入浮点数的编辑输入框
	 * @param currValue 当前显示的值
	 * @param parent 父窗体指针
	*/
	explicit DoubleLineEdit(double currValue, QWidget* parent = nullptr);
	~DoubleLineEdit();

	/**
	 * @brief 设置显示的内容，整数
	 * @param value 显示的内容
	*/
	void setValue(int value);

	/**
	 * @brief 设置显示的内容，浮点
	 * @param value 显示的内容
	*/
	void setValue(double value);

	/**
	 * @brief 设置输入范围
	 * @param min 范围最小值
	 * @param max 范围最大值
	*/
	void setRange(double min, double max);

	/**
	 * @brief 显示的小数位数，适用于浮点
	 * @param value 小数位数
	*/
	void setDecimals(int value);

	/**
	 * @brief 设置步长
	 * @param step 步长
	*/
	void setSingleStep(int step);
signals:
	/**
	 * @brief 输入整数变化信号
	*/
	void textChangedSgn(int);

	/**
	 * @brief 输入浮点数变化信号
	 * @param  
	*/
	void textChangedSgn(double);
protected:
	/**
	 * @brief 事件过滤器，主要实现点击显示文本QLabel显示输入框
	 * @param watched 监控对象
	 * @param event 事件对象
	 * @return true 截止，false 继续向上传递
	*/
	bool eventFilter(QObject* watched, QEvent* event);    		// 注意这里

private:
	/**
	 * @brief 初始化窗体
	*/
	void InitUI();

	/**
	 * @brief 更新编辑框操作
	*/
	void UpdateLineEdit();

	/**
	 * @brief 通过滑轮或者上下键实现数值增大或者减小一定步长
	 * @param bAdd 
	*/
	void zoomValue(bool bAdd);

private slots:
	/**
	 * @brief 编辑按钮点击后处理
	*/
	void onBtnEditClick();

	/**
	 * @brief 输入框内容输入处理
	*/
	void onTextChanged();

private:
	QLabel* contentLabel_ = nullptr; //内容Label
	QLineEdit* lineEdit_ = nullptr; //编辑框
	QPushButton* btnEdit_ = nullptr; //编辑按钮
	double value_=0.0; //当前显示值
	double minValue_ = -INT_MAX; //范围最小值
	double maxValue_ = INT_MAX; //范围最大值
	int decimals_ = 0; //小数位数
	double singleStep_ = 1.0;
	
	ValueType valueType_ = ValueType::v_Int;
	QIntValidator* intValidator_ = nullptr; //整数校验器
	QDoubleValidator* doubleValidator_ = nullptr; //浮点数校验器
	bool bEdit_ = false;
};

/**
 * @brief 编辑框+按钮+滑块，只能输入整数或者浮点数，根据创造的对象而定，有数值范围
*/
class LineSliderEdit : public QWidget
{
	Q_OBJECT

public:
	/**
	 * @brief 区分创建对象的类型
	*/
	enum class ValueType
	{
		v_Int = 0, //只允许输入整数
		v_Double, //只允许输入浮点数
	};

	/**
	 * @brief 构造函数，创建只能输入整数的编辑输入框
	 * @param currValue 当前显示的值
	 * @param parent 父窗体指针
	*/
	explicit LineSliderEdit(int currValue, QWidget* parent = nullptr);

	/**
	 * @brief 构造函数，创建只能输入浮点数的编辑输入框
	 * @param currValue 当前显示的值
	 * @param parent 父窗体指针
	*/
	explicit LineSliderEdit(double currValue, QWidget* parent = nullptr);
	~LineSliderEdit();

	/**
	 * @brief 设置显示的内容，整数
	 * @param value 显示的内容
	*/
	void setValue(int value);

	/**
	 * @brief 设置显示的内容，浮点
	 * @param value 显示的内容
	*/
	void setValue(double value);

	/**
	 * @brief 设置输入范围
	 * @param min 范围最小值
	 * @param max 范围最大值
	*/
	void setRange(double min, double max);

	/**
	 * @brief 显示的小数位数，适用于浮点
	 * @param value 小数位数
	*/
	void setDecimals(int value);

	/**
	 * @brief 设置步长
	 * @param step 步长
	*/
	void setSingleStep(int step);
signals:
	/**
	 * @brief 输入整数变化信号
	*/
	void textChangedSgn(int);

	/**
	 * @brief 输入浮点数变化信号
	 * @param
	*/
	void textChangedSgn(double);
protected:
	/**
	 * @brief 事件过滤器，主要实现点击显示文本QLabel显示输入框
	 * @param watched 监控对象
	 * @param event 事件对象
	 * @return true 截止，false 继续向上传递
	*/
	bool eventFilter(QObject* watched, QEvent* event);    		// 注意这里

	/**
	 * @brief 离开窗体事件
	 * @param event 窗体事件指针
	*/
	void leaveEvent(QEvent* event);
private:
	/**
	 * @brief 初始化窗体
	*/
	void InitUI();

	/**
	 * @brief 更新编辑框操作
	*/
	void UpdateLineEdit();

	/**
	 * @brief 更新编辑按钮操作
	*/
	void UpdateBtnEdit();

	/**
	 * @brief 通过滑轮或者上下键实现数值增大或者减小一定步长
	 * @param bAdd
	*/
	void zoomValue(bool bAdd);

private slots:
	/**
	 * @brief 编辑按钮点击后处理
	*/
	void onBtnEditClick();

	/**
	 * @brief 输入框内容输入处理
	*/
	void onTextChanged();

	/**
	 * @brief 滑块滑动后处理
	 * @param val 滑动后的值
	*/
	void onSliderValueChanged(int val);

	/**
	 * @brief 离开该控件后，定时器处理
	*/
	void onTimeOut();

private:
	QLabel* contentLabel_ = nullptr; //内容Label
	QLineEdit* lineEdit_ = nullptr; //编辑框
	QPushButton* btnEdit_ = nullptr; //编辑按钮
	QSlider* slider_ = nullptr; //滑块控件
	double value_ = 0.0; //当前显示值
	double minValue_ = -INT_MAX; //范围最小值
	double maxValue_ = INT_MAX; //范围最大值
	int decimals_ = 0; //小数位数
	double singleStep_ = 1.0;

	ValueType valueType_ = ValueType::v_Int;
	QIntValidator* intValidator_ = nullptr; //整数校验器
	QDoubleValidator* doubleValidator_ = nullptr; //浮点数校验器
	QTimer* sliderTimer_ = nullptr;
	bool bEdit_ = false;
};

/**
 * @brief 编辑框+按钮，可以输入任何信息，实现颜色设置
*/
class ColorLineEdit : public QWidget
{
	Q_OBJECT

public:

	/**
	 * @brief 构造函数
	 * @param content 初始内容
	 * @param parent 父窗体指针
	*/
	explicit ColorLineEdit(const QString& content = QString("None"), const QColor& color = QColor("#FFFFFF"), QWidget* parent = nullptr);
	~ColorLineEdit();

	/**
	 * @brief 设置显示内容
	 * @param text 显示内容文本
	*/
	void setText(const QString& text);

	/**
	 * @brief 设置颜色值
	 * @param color 要设置的颜色
	*/
	void setColor(const QColor& color);

signals:

	/**
	 * @brief 颜色更改后发出的信号
	 * @param  
	*/
	void colorChangedSgn(const QString&, const QColor&);

protected:
	/**
	 * @brief 事件过滤器，主要实现点击显示文本QLabel显示输入框
	 * @param watched 监控对象
	 * @param event 事件对象
	 * @return true 截止，false 继续向上传递
	*/
	bool eventFilter(QObject* watched, QEvent* event);    		// 注意这里

	/**
	 * @brief 离开窗体事件
	 * @param event 事件指针
	*/
	void leaveEvent(QEvent* event);

private:
	/**
	 * @brief 初始化窗体
	*/
	void InitUI();

	/**
	 * @brief 更新编辑输入框控件
	*/
	void UpdateLineEdit();


private slots:
	/**
	 * @brief 编辑按钮点击事件处理
	*/
	void onBtnSetColorClick();

	/**
	 * @brief 输入框输入信息变化后处理
	*/
	void onTextChanged();

private:
	QString currentContent_; //显示的文本
	QLabel* contentLabel_ = nullptr; //显示文本的QLabel
	QLineEdit* lineEdit_ = nullptr; //输入框，初始时隐藏
	QPushButton* btnColorSet_ = nullptr; //编辑按钮
	QColor color_;

};

/**
 * @brief 纹理或者图片选择
*/
class ImageSelectWidget: public QWidget
{
	Q_OBJECT

public:
	/**
	 * @brief 构造函数
	 * @param imagePath 图像路径
	 * @param parent 父窗体指针
	*/
	explicit ImageSelectWidget(const QString& imagePath, QWidget* parent = nullptr);

	/**
	 * @brief 构造函数
	 * @param imagePath 图像路径
	 * @param bOpenEditor 是否包含Open Texture Editor
	 * @param parent 父窗体指针
	*/
	explicit ImageSelectWidget(const QString& imagePath, bool bOpenEditor, QWidget* parent = nullptr);
	~ImageSelectWidget();

	/**
	 * @brief 设置图片路径
	 * @param imagePath 图片路径
	*/
	void setImagePath(const QString& imagePath);

	/**
	 * @brief 设置打开图像时的筛选器
	 * @param imageFilter 筛选信息
	*/
	void setImageFilter(const QString& imageFilter);

	/**
	 * @brief 设置打开文件所在的目录
	 * @param imageDir 文件所在目录
	*/
	void setImageDirPath(const QString& imageDir);
signals:
	/**
	 * @brief 图片文件更改
	*/
	void imageChangedSgn(const QString&);

protected:
	/**
	 * @brief 事件过滤器，主要实现点击显示文本QLabel显示输入框
	 * @param watched 监控对象
	 * @param event 事件对象
	 * @return true 截止，false 继续向上传递
	*/
	bool eventFilter(QObject* watched, QEvent* event);    		// 注意这里
private:
	/**
	 * @brief 初始化窗体
	 * @param bOpenEditor 是否包含Open Texture Editor 操作
	*/
	void InitUI(bool bOpenEditor);
private slots:

	/**
	 * @brief 选择图像操作
	*/
	void onBtnSelectClick();
	
	/**
	 * @brief 打开Open Texture Editor操作
	*/
	void onBtnOpenEditorClick();

	/**
	 * @brief 删除图像操作
	*/
	void onBtnDeleteClick();
private:
	QString imagePath_;
	QLabel* imageLabel_ = nullptr;
	QPushButton* btnSelect_ = nullptr;
	QPushButton* btnOpenEditor_ = nullptr;
	QPushButton* btnDelete_ = nullptr;
	QString imageFilter_= QString("Image File(*.jpg *.jpeg *.png *.bmp *.wbmp *.psd *.ico *.tga *.tif *.tiff *.gif *.mng *.jng *.dds *.hdr *.koa *.lbm *.iff *.pbm *.pcd *.pcx *.pgm *.ppm *.ras *.cut *.xmb *.xpm *.sgi *.g3 *.exr *.j2k *.jp2 *.pfm *.pict *.pct)");
	QString imageDirPath_ = QString("/");
};

/**
 * @brief CheckBox 控件，选中未选中时显示文本不同
*/
class CheckBoxWidget : public QWidget
{
	Q_OBJECT

public:
	explicit CheckBoxWidget(const QString& checkText, const QString& uncheckText, QWidget* parent = nullptr);
	~CheckBoxWidget();
	void setShowText(const QString& checkText, const QString& uncheckText);
	void setChecked(bool bCheck);
	bool isChcecked() const;

signals:
	void checkStateSgn(bool);
private:
	void InitUI();
private slots:
	void onToggled(bool bCheck);
private:
	QCheckBox* checkBox_ = nullptr;
	QString checkText_;
	QString uncheckText_;

};

class ComboBoxSaveOpenWidget : public QWidget
{
	Q_OBJECT

public:
	explicit ComboBoxSaveOpenWidget(bool bOpen, bool bSave, bool bReset, QWidget* parent = nullptr);
	~ComboBoxSaveOpenWidget();
	void setItemList(const QStringList& itemList, const QStringList& itemDataList);
	void setCurrentItemData(const QString& data);
	QString getCurrentData() const;
	void setOpenFilter(const QString& filter);
	void setSaveFilter(const QString& filter);
	void setDirPath(const QString& dir);
signals:
	void currentDataChangedSgn(const QString&);
	void resetSgn();

private:
	void InitUI(bool bOpen, bool bSave, bool bReset);

private slots:
	void onOpenFileClick();
	void onSaveFileClick();
	void onResetClick();
	void onCurrentChanged(int index);
private:
	QComboBox* comboBox_ = nullptr;
	QPushButton* btnOpenFile_ = nullptr;
	QPushButton* btnSaveFile_ = nullptr;
	QPushButton* btnReset_ = nullptr;
	QString openFilter_;
	QString saveFilter_;
	QString dirPath_;
};

class ButtonSaveOpenWidget : public QWidget
{
	Q_OBJECT

public:
	explicit ButtonSaveOpenWidget(QWidget* parent = nullptr);
	~ButtonSaveOpenWidget();
	void setOpenFilter(const QString& filter);
	void setSaveFilter(const QString& filter);
	void setDirPath(const QString& dir);
signals:
	void openFileSgn(const QString&);
	void saveFileSgn(const QString&);
private:
	void InitUI();

private slots:
	void onOpenFileClick();
	void onSaveFileClick();
private:
	QPushButton* btnOpenFile_ = nullptr;
	QPushButton* btnSaveFile_ = nullptr;
	QString openFilter_;
	QString saveFilter_;
	QString dirPath_;
};

class ShapeImageWidget : public QWidget
{
	Q_OBJECT

public:
	/**
	 * @brief 构造函数
	 * @param imagePath 图像路径
	 * @param parent 父窗体指针
	*/
	explicit ShapeImageWidget(const QString& imagePath, QWidget* parent = nullptr);

	~ShapeImageWidget();

	/**
	 * @brief 设置图片路径
	 * @param imagePath 图片路径
	*/
	void setImagePath(const QString& imagePath);

signals:
	/**
	 * @brief 图片文件更改
	*/
	void imageChangedSgn(const QString&);

private:
	/**
	 * @brief 初始化窗体
	*/
	void InitUI();

private slots:

	/**
	 * @brief 选择图像操作
	*/
	void onBtnSelectClick();

	/**
	 * @brief 添加图像操作
	*/
	void onBtnAddClick();
private:
	QString imagePath_;
	QLabel* imageLabel_ = nullptr;
	QPushButton* btnSelect_ = nullptr;
	QPushButton* btnAdd_ = nullptr;

};

class PropertyItemGroupWidget : public QWidget
{
	Q_OBJECT
	
public:
	explicit PropertyItemGroupWidget(QWidget* parent = Q_NULLPTR);
	~PropertyItemGroupWidget();
	void addTopWidget(QWidget* w);
	void addSubWidget(QWidget* w);
	void UpdateUI();
	int GetHeight();
signals:
	void uiStateSgn();
public slots:
	void setSubWidgetHeight();
private:
	void onExpandedClicked();
private:
	void InitUI();

private:
	QVBoxLayout* mainLayout_ = nullptr;
	QPushButton* btnExpanded_ = nullptr;
	QVBoxLayout* subLayout_ = nullptr;
	QHBoxLayout* topLayout_ = nullptr;
	QWidget* subWidget_ = nullptr;
	int subWidgetHeight_;
	QWidget* topWidget_ = nullptr;
	QWidget* leftWidget_ = nullptr; //用来代替前面的收缩按钮的位置
	
};

class PropertyLabel : public QLabel
{
	Q_OBJECT

public:
	explicit PropertyLabel(QWidget* parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags());
	explicit PropertyLabel(const QString& text, QWidget* parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags());
	~PropertyLabel();
};

class PropertyComboBox : public QComboBox
{
	Q_OBJECT

public:
	explicit PropertyComboBox(QWidget* parent = nullptr);
	~PropertyComboBox();

	void AddItemList(const QStringList& textList, const QStringList& dataList);
};
#endif // PROPERTYCOMMONCONTROL_H


