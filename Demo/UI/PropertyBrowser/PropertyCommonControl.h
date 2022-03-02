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
 * @brief ���Ա༭�� �ؼ���ˮƽ���֣���Ҫʹ��������ӿؼ�
*/
class PropertyItemWidget : public QFrame
{
	Q_OBJECT

public:
	/**
	 * @brief ���캯��
	 * @param title QLabel����
	*/
	explicit PropertyItemWidget(const QString& title, QWidget* parent = nullptr,const QString& propertyName = QString("normal"));
	~PropertyItemWidget();

	/**
	 * @brief ��ؼ�������ӿؼ�
	 * @param widget �ӿؼ�ָ��
	*/
	void addWidget(QWidget* widget);
private:
	/**
	 * @brief �ؼ���ʼ��
	*/
	void InitUI(const QString& propertyName);

private:
	QString titleContent_; //����
	QLabel* titleLabel_ = nullptr; //���ñ������ݵ�QLabel
	QHBoxLayout* mainLayout_ = nullptr;
};

/**
 * @brief ��ǩ�л�
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
 * @brief �༭��+��ť�����������κ���Ϣ
*/
class StringLineEdit : public QWidget
{
	Q_OBJECT

public:

	/**
	 * @brief ���캯��
	 * @param content ��ʼ����
	 * @param parent ������ָ��
	*/
	explicit StringLineEdit(const QString& content = QString(),QWidget* parent = nullptr);
	~StringLineEdit();

	/**
	 * @brief ������ʾ����
	 * @param text ��ʾ�����ı�
	*/
	void setText(const QString& text);

	/**
	 * @brief ����������Ϣ
	 * @param validator ���ƶ���ָ��
	*/
	void setValidator(const QValidator* validator);
	
signals:
	/**
	 * @brief ��ʾ�ı����ĺ󷢳����ź�
	*/
	void textChangedSgn(const QString&);

protected:
	/**
	 * @brief �¼�����������Ҫʵ�ֵ����ʾ�ı�QLabel��ʾ�����
	 * @param watched ��ض���
	 * @param event �¼�����
	 * @return true ��ֹ��false �������ϴ���
	*/
	bool eventFilter(QObject* watched, QEvent* event);    		// ע������
	
	/**
	 * @brief �뿪�����¼�
	 * @param event �¼�ָ��
	*/
	//void leaveEvent(QEvent* event);

private:
	/**
	 * @brief ��ʼ������
	*/
	void InitUI();

	/**
	 * @brief ���±༭�����ؼ�
	*/
	void UpdateLineEdit();

private slots:
	/**
	 * @brief �༭��ť����¼�����
	*/
	void onBtnEditClick();

	/**
	 * @brief �����������Ϣ�仯����
	*/
	void onTextChanged();

private:
	QString currentContent_; //��ʾ���ı�
	QLabel* contentLabel_ = nullptr; //��ʾ�ı���QLabel
	QLineEdit* lineEdit_ = nullptr; //����򣬳�ʼʱ����
	QPushButton* btnEdit_ = nullptr; //�༭��ť
	bool bEdit_ = false;
};

/**
 * @brief �༭��+��ť��ֻ�������������߸����������ݴ���Ķ������������ֵ��Χ
*/
class DoubleLineEdit : public QWidget
{
	Q_OBJECT

public:
	/**
	 * @brief ���ִ������������
	*/
	enum class ValueType
	{
		v_Int =0, //ֻ������������
		v_Double, //ֻ�������븡����
	};

	/**
	 * @brief ���캯��������ֻ�����������ı༭�����
	 * @param currValue ��ǰ��ʾ��ֵ
	 * @param parent ������ָ��
	*/
	explicit DoubleLineEdit(int currValue, QWidget* parent = nullptr);

	/**
	 * @brief ���캯��������ֻ�����븡�����ı༭�����
	 * @param currValue ��ǰ��ʾ��ֵ
	 * @param parent ������ָ��
	*/
	explicit DoubleLineEdit(double currValue, QWidget* parent = nullptr);
	~DoubleLineEdit();

	/**
	 * @brief ������ʾ�����ݣ�����
	 * @param value ��ʾ������
	*/
	void setValue(int value);

	/**
	 * @brief ������ʾ�����ݣ�����
	 * @param value ��ʾ������
	*/
	void setValue(double value);

	/**
	 * @brief �������뷶Χ
	 * @param min ��Χ��Сֵ
	 * @param max ��Χ���ֵ
	*/
	void setRange(double min, double max);

	/**
	 * @brief ��ʾ��С��λ���������ڸ���
	 * @param value С��λ��
	*/
	void setDecimals(int value);

	/**
	 * @brief ���ò���
	 * @param step ����
	*/
	void setSingleStep(int step);
signals:
	/**
	 * @brief ���������仯�ź�
	*/
	void textChangedSgn(int);

	/**
	 * @brief ���븡�����仯�ź�
	 * @param  
	*/
	void textChangedSgn(double);
protected:
	/**
	 * @brief �¼�����������Ҫʵ�ֵ����ʾ�ı�QLabel��ʾ�����
	 * @param watched ��ض���
	 * @param event �¼�����
	 * @return true ��ֹ��false �������ϴ���
	*/
	bool eventFilter(QObject* watched, QEvent* event);    		// ע������

private:
	/**
	 * @brief ��ʼ������
	*/
	void InitUI();

	/**
	 * @brief ���±༭�����
	*/
	void UpdateLineEdit();

	/**
	 * @brief ͨ�����ֻ������¼�ʵ����ֵ������߼�Сһ������
	 * @param bAdd 
	*/
	void zoomValue(bool bAdd);

private slots:
	/**
	 * @brief �༭��ť�������
	*/
	void onBtnEditClick();

	/**
	 * @brief ������������봦��
	*/
	void onTextChanged();

private:
	QLabel* contentLabel_ = nullptr; //����Label
	QLineEdit* lineEdit_ = nullptr; //�༭��
	QPushButton* btnEdit_ = nullptr; //�༭��ť
	double value_=0.0; //��ǰ��ʾֵ
	double minValue_ = -INT_MAX; //��Χ��Сֵ
	double maxValue_ = INT_MAX; //��Χ���ֵ
	int decimals_ = 0; //С��λ��
	double singleStep_ = 1.0;
	
	ValueType valueType_ = ValueType::v_Int;
	QIntValidator* intValidator_ = nullptr; //����У����
	QDoubleValidator* doubleValidator_ = nullptr; //������У����
	bool bEdit_ = false;
};

/**
 * @brief �༭��+��ť+���飬ֻ�������������߸����������ݴ���Ķ������������ֵ��Χ
*/
class LineSliderEdit : public QWidget
{
	Q_OBJECT

public:
	/**
	 * @brief ���ִ������������
	*/
	enum class ValueType
	{
		v_Int = 0, //ֻ������������
		v_Double, //ֻ�������븡����
	};

	/**
	 * @brief ���캯��������ֻ�����������ı༭�����
	 * @param currValue ��ǰ��ʾ��ֵ
	 * @param parent ������ָ��
	*/
	explicit LineSliderEdit(int currValue, QWidget* parent = nullptr);

	/**
	 * @brief ���캯��������ֻ�����븡�����ı༭�����
	 * @param currValue ��ǰ��ʾ��ֵ
	 * @param parent ������ָ��
	*/
	explicit LineSliderEdit(double currValue, QWidget* parent = nullptr);
	~LineSliderEdit();

	/**
	 * @brief ������ʾ�����ݣ�����
	 * @param value ��ʾ������
	*/
	void setValue(int value);

	/**
	 * @brief ������ʾ�����ݣ�����
	 * @param value ��ʾ������
	*/
	void setValue(double value);

	/**
	 * @brief �������뷶Χ
	 * @param min ��Χ��Сֵ
	 * @param max ��Χ���ֵ
	*/
	void setRange(double min, double max);

	/**
	 * @brief ��ʾ��С��λ���������ڸ���
	 * @param value С��λ��
	*/
	void setDecimals(int value);

	/**
	 * @brief ���ò���
	 * @param step ����
	*/
	void setSingleStep(int step);
signals:
	/**
	 * @brief ���������仯�ź�
	*/
	void textChangedSgn(int);

	/**
	 * @brief ���븡�����仯�ź�
	 * @param
	*/
	void textChangedSgn(double);
protected:
	/**
	 * @brief �¼�����������Ҫʵ�ֵ����ʾ�ı�QLabel��ʾ�����
	 * @param watched ��ض���
	 * @param event �¼�����
	 * @return true ��ֹ��false �������ϴ���
	*/
	bool eventFilter(QObject* watched, QEvent* event);    		// ע������

	/**
	 * @brief �뿪�����¼�
	 * @param event �����¼�ָ��
	*/
	void leaveEvent(QEvent* event);
private:
	/**
	 * @brief ��ʼ������
	*/
	void InitUI();

	/**
	 * @brief ���±༭�����
	*/
	void UpdateLineEdit();

	/**
	 * @brief ���±༭��ť����
	*/
	void UpdateBtnEdit();

	/**
	 * @brief ͨ�����ֻ������¼�ʵ����ֵ������߼�Сһ������
	 * @param bAdd
	*/
	void zoomValue(bool bAdd);

private slots:
	/**
	 * @brief �༭��ť�������
	*/
	void onBtnEditClick();

	/**
	 * @brief ������������봦��
	*/
	void onTextChanged();

	/**
	 * @brief ���黬������
	 * @param val �������ֵ
	*/
	void onSliderValueChanged(int val);

	/**
	 * @brief �뿪�ÿؼ��󣬶�ʱ������
	*/
	void onTimeOut();

private:
	QLabel* contentLabel_ = nullptr; //����Label
	QLineEdit* lineEdit_ = nullptr; //�༭��
	QPushButton* btnEdit_ = nullptr; //�༭��ť
	QSlider* slider_ = nullptr; //����ؼ�
	double value_ = 0.0; //��ǰ��ʾֵ
	double minValue_ = -INT_MAX; //��Χ��Сֵ
	double maxValue_ = INT_MAX; //��Χ���ֵ
	int decimals_ = 0; //С��λ��
	double singleStep_ = 1.0;

	ValueType valueType_ = ValueType::v_Int;
	QIntValidator* intValidator_ = nullptr; //����У����
	QDoubleValidator* doubleValidator_ = nullptr; //������У����
	QTimer* sliderTimer_ = nullptr;
	bool bEdit_ = false;
};

/**
 * @brief �༭��+��ť�����������κ���Ϣ��ʵ����ɫ����
*/
class ColorLineEdit : public QWidget
{
	Q_OBJECT

public:

	/**
	 * @brief ���캯��
	 * @param content ��ʼ����
	 * @param parent ������ָ��
	*/
	explicit ColorLineEdit(const QString& content = QString("None"), const QColor& color = QColor("#FFFFFF"), QWidget* parent = nullptr);
	~ColorLineEdit();

	/**
	 * @brief ������ʾ����
	 * @param text ��ʾ�����ı�
	*/
	void setText(const QString& text);

	/**
	 * @brief ������ɫֵ
	 * @param color Ҫ���õ���ɫ
	*/
	void setColor(const QColor& color);

signals:

	/**
	 * @brief ��ɫ���ĺ󷢳����ź�
	 * @param  
	*/
	void colorChangedSgn(const QString&, const QColor&);

protected:
	/**
	 * @brief �¼�����������Ҫʵ�ֵ����ʾ�ı�QLabel��ʾ�����
	 * @param watched ��ض���
	 * @param event �¼�����
	 * @return true ��ֹ��false �������ϴ���
	*/
	bool eventFilter(QObject* watched, QEvent* event);    		// ע������

	/**
	 * @brief �뿪�����¼�
	 * @param event �¼�ָ��
	*/
	void leaveEvent(QEvent* event);

private:
	/**
	 * @brief ��ʼ������
	*/
	void InitUI();

	/**
	 * @brief ���±༭�����ؼ�
	*/
	void UpdateLineEdit();


private slots:
	/**
	 * @brief �༭��ť����¼�����
	*/
	void onBtnSetColorClick();

	/**
	 * @brief �����������Ϣ�仯����
	*/
	void onTextChanged();

private:
	QString currentContent_; //��ʾ���ı�
	QLabel* contentLabel_ = nullptr; //��ʾ�ı���QLabel
	QLineEdit* lineEdit_ = nullptr; //����򣬳�ʼʱ����
	QPushButton* btnColorSet_ = nullptr; //�༭��ť
	QColor color_;

};

/**
 * @brief �������ͼƬѡ��
*/
class ImageSelectWidget: public QWidget
{
	Q_OBJECT

public:
	/**
	 * @brief ���캯��
	 * @param imagePath ͼ��·��
	 * @param parent ������ָ��
	*/
	explicit ImageSelectWidget(const QString& imagePath, QWidget* parent = nullptr);

	/**
	 * @brief ���캯��
	 * @param imagePath ͼ��·��
	 * @param bOpenEditor �Ƿ����Open Texture Editor
	 * @param parent ������ָ��
	*/
	explicit ImageSelectWidget(const QString& imagePath, bool bOpenEditor, QWidget* parent = nullptr);
	~ImageSelectWidget();

	/**
	 * @brief ����ͼƬ·��
	 * @param imagePath ͼƬ·��
	*/
	void setImagePath(const QString& imagePath);

	/**
	 * @brief ���ô�ͼ��ʱ��ɸѡ��
	 * @param imageFilter ɸѡ��Ϣ
	*/
	void setImageFilter(const QString& imageFilter);

	/**
	 * @brief ���ô��ļ����ڵ�Ŀ¼
	 * @param imageDir �ļ�����Ŀ¼
	*/
	void setImageDirPath(const QString& imageDir);
signals:
	/**
	 * @brief ͼƬ�ļ�����
	*/
	void imageChangedSgn(const QString&);

protected:
	/**
	 * @brief �¼�����������Ҫʵ�ֵ����ʾ�ı�QLabel��ʾ�����
	 * @param watched ��ض���
	 * @param event �¼�����
	 * @return true ��ֹ��false �������ϴ���
	*/
	bool eventFilter(QObject* watched, QEvent* event);    		// ע������
private:
	/**
	 * @brief ��ʼ������
	 * @param bOpenEditor �Ƿ����Open Texture Editor ����
	*/
	void InitUI(bool bOpenEditor);
private slots:

	/**
	 * @brief ѡ��ͼ�����
	*/
	void onBtnSelectClick();
	
	/**
	 * @brief ��Open Texture Editor����
	*/
	void onBtnOpenEditorClick();

	/**
	 * @brief ɾ��ͼ�����
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
 * @brief CheckBox �ؼ���ѡ��δѡ��ʱ��ʾ�ı���ͬ
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
	 * @brief ���캯��
	 * @param imagePath ͼ��·��
	 * @param parent ������ָ��
	*/
	explicit ShapeImageWidget(const QString& imagePath, QWidget* parent = nullptr);

	~ShapeImageWidget();

	/**
	 * @brief ����ͼƬ·��
	 * @param imagePath ͼƬ·��
	*/
	void setImagePath(const QString& imagePath);

signals:
	/**
	 * @brief ͼƬ�ļ�����
	*/
	void imageChangedSgn(const QString&);

private:
	/**
	 * @brief ��ʼ������
	*/
	void InitUI();

private slots:

	/**
	 * @brief ѡ��ͼ�����
	*/
	void onBtnSelectClick();

	/**
	 * @brief ���ͼ�����
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
	QWidget* leftWidget_ = nullptr; //��������ǰ���������ť��λ��
	
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


