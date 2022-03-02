#include "PropertyEditorWidget.h"
#include "QBoxLayout"
#include "QScrollArea"
#include "PropertyCommonControl.h"
#include "QComboBox"
#include "QLabel"
#include "QListView"
#include <QDebug>

PropertyEditorContainer::PropertyEditorContainer(QWidget* parent /*= nullptr*/)
{
	InitUI();
}

PropertyEditorContainer::~PropertyEditorContainer()
{

}

QVBoxLayout* PropertyEditorContainer::GetVBoxLayout() const
{
	return scrollLayout_;
}

QWidget* PropertyEditorContainer::GetScrollWidget() const
{
	return scrollWidget_;
}

void PropertyEditorContainer::InitUI()
{
	QVBoxLayout* mainLayout = new QVBoxLayout(this);
	mainLayout->setSpacing(0);
	mainLayout->setMargin(0);
	mainLayout->setContentsMargins(8, 8, 8, 8);

	scrollArea_ = new QScrollArea(this);
	scrollArea_->setObjectName("scrollArea");
	scrollArea_->setWidgetResizable(true);

	scrollWidget_ = new QWidget(this);
	scrollWidget_->setObjectName("scrollWidget");
	scrollWidget_->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

	scrollLayout_ = new QVBoxLayout();
	scrollLayout_->setSpacing(2);
	scrollLayout_->setMargin(0);
	scrollLayout_->setAlignment(Qt::AlignTop);
	scrollWidget_->setLayout(scrollLayout_);

	scrollArea_->setWidget(scrollWidget_);
	mainLayout->addWidget(scrollArea_);
}

void PropertyEditorContainer::UpdateUI()
{
	int nheight = 0;
	for (int iLoop = 0; iLoop < scrollLayout_->count(); iLoop++)
	{
		QWidget* w = scrollLayout_->itemAt(iLoop)->widget();
		PropertyItemGroupWidget* sub = qobject_cast<PropertyItemGroupWidget*>(w);
		if (sub)
		{
			sub->setSubWidgetHeight();
			nheight += sub->height();
		}
		else
		{
			nheight += w->height();
		}

	}
	nheight += (scrollLayout_->count() + 1) * scrollLayout_->spacing();
	scrollWidget_->setFixedHeight(nheight);
}

AvatarBodyEditor::AvatarBodyEditor(QWidget* parent /*= nullptr*/):PropertyEditorContainer(parent)
{
	InitEditor();
}

AvatarBodyEditor::~AvatarBodyEditor()
{

}


void AvatarBodyEditor::InitEditor()
{
	PropertyItemGroupWidget* informationGroup = new PropertyItemGroupWidget(this);
	//information
	PropertyItemWidget* itemWidget = new PropertyItemWidget(QObject::tr("Information"));
	informationGroup->addTopWidget(itemWidget);

	StringLineEdit* nameEdit = new StringLineEdit(QObject::tr("Mesh"));
	itemWidget = new PropertyItemWidget(QObject::tr("Name"));
	itemWidget->addWidget(nameEdit);
	informationGroup->addSubWidget(itemWidget);

	StringLineEdit* itemNoEdit = new StringLineEdit(QObject::tr("None"));
	itemWidget = new PropertyItemWidget(QObject::tr("Item No."));
	itemWidget->addWidget(itemNoEdit);
	informationGroup->addSubWidget(itemWidget);

	CheckBoxWidget* checkEdit = new CheckBoxWidget(QObject::tr("Include"), QObject::tr("Exclude"));
	itemWidget = new PropertyItemWidget(QObject::tr("Tech Pack(CLO-SET)"));
	itemWidget->addWidget(checkEdit);
	informationGroup->addSubWidget(itemWidget);

	informationGroup->setSubWidgetHeight();
	GetVBoxLayout()->addWidget(informationGroup);

	//model
	informationGroup = new PropertyItemGroupWidget(this);
    itemWidget = new PropertyItemWidget(QObject::tr("Model"));
	PropertyComboBox* comboBox = new PropertyComboBox();
	QStringList textList;
	QStringList dataList;
	textList << QObject::tr("Mara") << QObject::tr("Feifei") << QObject::tr("Grace") << QObject::tr("Emma") << QObject::tr("Kelly");
	dataList << QString("Mara") << QString("Feifei") << QString("Grace") << QString("Emma") << QString("Kelly");
	comboBox->AddItemList(textList,dataList);
	itemWidget->addWidget(comboBox);
	informationGroup->addTopWidget(itemWidget);
	GetVBoxLayout()->addWidget(informationGroup);

	//surface
	informationGroup = new PropertyItemGroupWidget(this);
	itemWidget = new PropertyItemWidget(QObject::tr("Surface"));
	informationGroup->addTopWidget(itemWidget);

	DoubleLineEdit* skinOffsetEdit = new DoubleLineEdit(3.0);
	skinOffsetEdit->setRange(0.0, 100.0);
	skinOffsetEdit->setSingleStep(1.0);
	skinOffsetEdit->setDecimals(1);
	itemWidget = new PropertyItemWidget(QObject::tr("Skin Offset(mm)"));
	itemWidget->addWidget(skinOffsetEdit);
	informationGroup->addSubWidget(itemWidget);

	DoubleLineEdit* staticFrictionEdit = new DoubleLineEdit(0.80);
	staticFrictionEdit->setRange(0.00, 100.00);
	staticFrictionEdit->setSingleStep(1.0);
	staticFrictionEdit->setDecimals(2);
	itemWidget = new PropertyItemWidget(QObject::tr("Static Friction"));
	itemWidget->addWidget(staticFrictionEdit);
	informationGroup->addSubWidget(itemWidget);

	DoubleLineEdit* kineticFrictionEdit = new DoubleLineEdit(0.05);
	kineticFrictionEdit->setRange(0.00, 100.00);
	kineticFrictionEdit->setSingleStep(1.0);
	kineticFrictionEdit->setDecimals(2);
	itemWidget = new PropertyItemWidget(QObject::tr("Kinetic Friction"));
	itemWidget->addWidget(kineticFrictionEdit);
	informationGroup->addSubWidget(itemWidget);
	informationGroup->setSubWidgetHeight();
	GetVBoxLayout()->addWidget(informationGroup);

	////Material(Selected)
	//informationGroup = new PropertyItemGroupWidget(this);
	//itemWidget = new PropertyItemWidget(QObject::tr("Material(Selected)"));
	//informationGroup->addTopWidget(itemWidget);

	//PropertyItemGroupWidget* subGroup = new PropertyItemGroupWidget(this);
	//itemWidget = new PropertyItemWidget(QObject::tr("Type"));
	//comboBox = new PropertyComboBox();
	//textList.clear();
	//dataList.clear();
	//textList << QObject::tr("Fabric_Matte") << QObject::tr("Fabric_Shiny") << QObject::tr("Fabric_Silk/Satin") << QObject::tr("Fabric_Velvet") << QObject::tr("Fur(Render Only)")
	//	<< QObject::tr("Gem(Render Only)")<< QObject::tr("Glass(Render Only)")<< QObject::tr("Glitter(Render Only)")<< QObject::tr("Iridescence(Render Only)")<< QObject::tr("Light(Render Only)")
	//	<< QObject::tr("Leather")<< QObject::tr("Metal")<< QObject::tr("Plastic")<< QObject::tr("Skin(Render Only)");
	//dataList << QString("Fabric_Matte") << QString("Fabric_Shiny") << QString("Fabric_Silk/Satin") << QString("Fabric_Velvet") << QString("Fur(Render Only)")
	//	<< QString("Gem(Render Only)") << QString("Glass(Render Only)") << QString("Glitter(Render Only)") << QString("Iridescence(Render Only)") << QString("Light(Render Only)")
	//	<< QString("Leather") << QString("Metal") << QString("Plastic") << QString("Skin(Render Only)");
	//comboBox->AddItemList(textList, dataList);
	//itemWidget->addWidget(comboBox);
	//subGroup->addTopWidget(itemWidget);
	//informationGroup->addSubWidget(subGroup);

	//PropertyItemGroupWidget* basicGroup = new PropertyItemGroupWidget();
	//itemWidget = new PropertyItemWidget(QObject::tr("Basic Parameters"));
	//basicGroup->addTopWidget(itemWidget);

	//QString dirPath = QString(" C:/Users/Administrator/AppData/Local/CLO/CLO Standalone/18076/1040");
	//ImageSelectWidget* textureSelect = new ImageSelectWidget(dirPath);
	//itemWidget = new PropertyItemWidget(QObject::tr("Texture"));
	//subGroup = new PropertyItemGroupWidget(this);
	//subGroup->addTopWidget(itemWidget);
	//basicGroup->addSubWidget(subGroup);

	//PropertyItemGroupWidget* normalGroup = new PropertyItemGroupWidget();
	//itemWidget = new PropertyItemWidget(QObject::tr("Normal Map"));
	//ImageSelectWidget* normalSelect = new ImageSelectWidget(dirPath);
	//itemWidget->addWidget(normalSelect);
	//normalGroup->addTopWidget(itemWidget);

	//itemWidget = new PropertyItemWidget(QObject::tr("Intensity"), nullptr, QString("expanded"));
	//LineSliderEdit* intensitySlider = new LineSliderEdit(0);
	//intensitySlider->setRange(-100, 100);
	//intensitySlider->setSingleStep(1);
	//itemWidget->addWidget(intensitySlider);
	//normalGroup->addSubWidget(itemWidget);
	//normalGroup->setSubWidgetHeight();
	//basicGroup->addSubWidget(normalGroup);

	//PropertyItemGroupWidget* displacementGroup = new PropertyItemGroupWidget();
	//itemWidget = new PropertyItemWidget(QObject::tr("Displacement Map"));
	//ImageSelectWidget* displaceSelect = new ImageSelectWidget(dirPath);
	//itemWidget->addWidget(displaceSelect);
	//displacementGroup->addTopWidget(itemWidget);

	//DoubleLineEdit* amountEdit = new DoubleLineEdit(3.00);
	//amountEdit->setRange(0.00, 100.00);
	//amountEdit->setSingleStep(1.0);
	//amountEdit->setDecimals(2);
	//itemWidget = new PropertyItemWidget(QObject::tr("Amount(mm)"));
	//itemWidget->addWidget(amountEdit);
	//displacementGroup->addSubWidget(itemWidget);

	//amountEdit = new DoubleLineEdit(0.00);
	//amountEdit->setRange(0.00, 100.00);
	//amountEdit->setSingleStep(1.0);
	//amountEdit->setDecimals(2);
	//itemWidget = new PropertyItemWidget(QObject::tr("Shift(mm)"));
	//itemWidget->addWidget(amountEdit);
	//displacementGroup->addSubWidget(itemWidget);

	//amountEdit = new DoubleLineEdit(0.00);
	//amountEdit->setRange(0.00, 100.00);
	//amountEdit->setSingleStep(1.0);
	//amountEdit->setDecimals(2);
	//itemWidget = new PropertyItemWidget(QObject::tr("Clipping(mm)"));
	//itemWidget->addWidget(amountEdit);
	//displacementGroup->addSubWidget(itemWidget);

	//amountEdit = new DoubleLineEdit(4.00);
	//amountEdit->setRange(0.00, 100.00);
	//amountEdit->setSingleStep(1.0);
	//amountEdit->setDecimals(2);
	//itemWidget = new PropertyItemWidget(QObject::tr("Particle Distance(mm)"));
	//itemWidget->addWidget(amountEdit);
	//displacementGroup->addSubWidget(itemWidget);

	//checkEdit = new CheckBoxWidget(QObject::tr("On"), QObject::tr("Off"));
	//itemWidget = new PropertyItemWidget(QObject::tr("Keep Continuity"));
	//itemWidget->addWidget(checkEdit);
	//displacementGroup->addSubWidget(itemWidget);

	//displacementGroup->setSubWidgetHeight();
	//basicGroup->addSubWidget(displacementGroup);

	//ColorLineEdit* colorEdit = new ColorLineEdit();
	//itemWidget = new PropertyItemWidget(QObject::tr("Color"));
	//itemWidget->addWidget(colorEdit);
	//subGroup = new PropertyItemGroupWidget(this);
	//subGroup->addTopWidget(itemWidget);
	//basicGroup->addSubWidget(subGroup);

	//itemWidget = new PropertyItemWidget(QObject::tr("Opacity"), nullptr, QString("expanded"));
	//LineSliderEdit* opacitySlider = new LineSliderEdit(100);
	//opacitySlider->setRange(0, 100);
	//opacitySlider->setSingleStep(1);
	//itemWidget->addWidget(opacitySlider);
	//subGroup = new PropertyItemGroupWidget(this);
	//subGroup->addTopWidget(itemWidget);
	//basicGroup->addSubWidget(subGroup);

	//PropertyItemGroupWidget* opacityGroup = new PropertyItemGroupWidget();
	//itemWidget = new PropertyItemWidget(QObject::tr("Opacity Map"));
	//ImageSelectWidget* opacitySelect = new ImageSelectWidget(dirPath);
	//itemWidget->addWidget(opacitySelect);
	//opacityGroup->addTopWidget(itemWidget);

	//itemWidget = new PropertyItemWidget(QObject::tr("Mode"));
	//comboBox = new PropertyComboBox();
	//textList.clear();
	//dataList.clear();
	//textList << QObject::tr("RGB") << QObject::tr("ALPHA");
	//dataList << QString("RGB") << QString("ALPHA");
	//comboBox->AddItemList(textList, dataList);
	//itemWidget->addWidget(comboBox);
	//opacityGroup->addSubWidget(itemWidget);
	//opacityGroup->setSubWidgetHeight();
	//basicGroup->addSubWidget(opacityGroup);
	//basicGroup->setSubWidgetHeight();
	//informationGroup->addSubWidget(basicGroup);

	////Reflection
	//PropertyItemGroupWidget* reflectionGroup = new PropertyItemGroupWidget();
	//itemWidget = new PropertyItemWidget(QObject::tr("Reflection"));
	//reflectionGroup->addTopWidget(itemWidget);

	//PropertyItemGroupWidget* roughnessGroup = new PropertyItemGroupWidget();
	//itemWidget = new PropertyItemWidget(QObject::tr("roughnessGroup"));
	//comboBox = new PropertyComboBox();
	//textList.clear();
	//dataList.clear();
	//textList << QObject::tr("Intensity") << QObject::tr("Map");
	//dataList << QString("Intensity") << QString("Map");
	//comboBox->AddItemList(textList, dataList);
	//itemWidget->addWidget(comboBox);
	//roughnessGroup->addTopWidget(itemWidget);

	//itemWidget = new PropertyItemWidget(QObject::tr(""), nullptr, QString("expanded"));
	//LineSliderEdit* sliderEdit = new LineSliderEdit(61);
	//sliderEdit->setRange(0, 100);
	//sliderEdit->setSingleStep(1);
	//itemWidget->addWidget(sliderEdit);
	//roughnessGroup->addSubWidget(itemWidget);
	//roughnessGroup->setSubWidgetHeight();
	//reflectionGroup->addSubWidget(roughnessGroup);

	//itemWidget = new PropertyItemWidget(QObject::tr("Reflection Intensity"), nullptr, QString("expanded"));
	//sliderEdit = new LineSliderEdit(15);
	//sliderEdit->setRange(0, 100);
	//sliderEdit->setSingleStep(1);
	//itemWidget->addWidget(sliderEdit);
	//subGroup = new PropertyItemGroupWidget(this);
	//subGroup->addTopWidget(itemWidget);
	//reflectionGroup->addSubWidget(subGroup);

	//itemWidget = new PropertyItemWidget(QObject::tr("Metalness"), nullptr, QString("expanded"));
	//sliderEdit = new LineSliderEdit(0);
	//sliderEdit->setRange(0, 100);
	//sliderEdit->setSingleStep(1);
	//itemWidget->addWidget(sliderEdit);
	//subGroup = new PropertyItemGroupWidget(this);
	//subGroup->addTopWidget(itemWidget);
	//reflectionGroup->addSubWidget(subGroup);

	//itemWidget = new PropertyItemWidget(QObject::tr("Metalness Map"));
	//textureSelect = new ImageSelectWidget(dirPath);
	//itemWidget->addWidget(textureSelect);
	//subGroup = new PropertyItemGroupWidget(this);
	//subGroup->addTopWidget(itemWidget);
	//reflectionGroup->addSubWidget(subGroup);

	//reflectionGroup->setSubWidgetHeight();
	//informationGroup->addSubWidget(reflectionGroup);
	//informationGroup->setSubWidgetHeight();
	//GetVBoxLayout()->addWidget(informationGroup);
	////Miscellaneous
	//informationGroup = new PropertyItemGroupWidget(this);
	//itemWidget = new PropertyItemWidget(QObject::tr("Miscellaneous"));
	//informationGroup->addTopWidget(itemWidget);

	//itemWidget = new PropertyItemWidget(QObject::tr("Mesh Type"));
	//PropertyLabel* label = new PropertyLabel();
	//label->setText("TRIANGLE");
	//itemWidget->addWidget(label);
	//informationGroup->addSubWidget(itemWidget);

	//itemWidget = new PropertyItemWidget(QObject::tr("Element Count"));
	//label = new PropertyLabel();
	//label->setText("4026");
	//itemWidget->addWidget(label);
	//informationGroup->addSubWidget(itemWidget);
	//informationGroup->setSubWidgetHeight();
	//GetVBoxLayout()->addWidget(informationGroup);
	////Material(Selected)
	informationGroup = new PropertyItemGroupWidget(this);
	itemWidget = new PropertyItemWidget(QObject::tr("Material(Selected)"));
	informationGroup->addTopWidget(itemWidget);
	MaterialEditor* materialEditor = new MaterialEditor("");
	for (auto &itr : materialEditor->GetPropertyItemGroup())
	{
		informationGroup->addSubWidget(itr);
	}
	connect(materialEditor, &MaterialEditor::updateSgn, this, &PropertyEditorContainer::UpdateUI);

	informationGroup->setSubWidgetHeight();

	GetVBoxLayout()->addWidget(informationGroup);

	//Miscellaneous
	informationGroup = new PropertyItemGroupWidget(this);
	itemWidget = new PropertyItemWidget(QObject::tr("Miscellaneous"));
	informationGroup->addTopWidget(itemWidget);

	itemWidget = new PropertyItemWidget(QObject::tr("Mesh Type"));
	PropertyLabel* label = new PropertyLabel();
	label->setText("TRIANGLE");
	itemWidget->addWidget(label);
	informationGroup->addSubWidget(itemWidget);

	itemWidget = new PropertyItemWidget(QObject::tr("Element Count"));
	label = new PropertyLabel();
	label->setText("4026");
	itemWidget->addWidget(label);
	informationGroup->addSubWidget(itemWidget);
	informationGroup->setSubWidgetHeight();
	GetVBoxLayout()->addWidget(informationGroup);


}


FabricEditor::FabricEditor(QWidget* parent /*= nullptr*/):PropertyEditorContainer(parent)
{
	InitEditor();
}

FabricEditor::~FabricEditor()
{

}

void FabricEditor::InitEditor()
{
	PropertyItemGroupWidget* informationGroup = new PropertyItemGroupWidget(this);
	PropertyItemWidget* itemWidget = new PropertyItemWidget(QObject::tr("Fabric"));
	ButtonSaveOpenWidget* buttonWidget = new ButtonSaveOpenWidget(this);
	itemWidget->addWidget(buttonWidget);
	informationGroup->addTopWidget(itemWidget);
	GetVBoxLayout()->addWidget(informationGroup);

	//information
	informationGroup = new PropertyItemGroupWidget(this);
	itemWidget = new PropertyItemWidget(QObject::tr("Information"));
	informationGroup->addTopWidget(itemWidget);
	StringLineEdit* nameEdit = new StringLineEdit(QObject::tr("Default Fabric"));
	itemWidget = new PropertyItemWidget(QObject::tr("Name"));
	itemWidget->addWidget(nameEdit);
	informationGroup->addSubWidget(itemWidget);

	StringLineEdit* itemNoEdit = new StringLineEdit(QObject::tr("None"));
	itemWidget = new PropertyItemWidget(QObject::tr("Item No."));
	itemWidget->addWidget(itemNoEdit);
	informationGroup->addSubWidget(itemWidget);

	PropertyItemGroupWidget* subGroup = new PropertyItemGroupWidget();
	itemWidget = new PropertyItemWidget(QObject::tr("Classification"));
	itemNoEdit = new StringLineEdit(QObject::tr("None"));
	itemWidget->addWidget(itemNoEdit);
	subGroup->addTopWidget(itemWidget);

	itemWidget = new PropertyItemWidget(QObject::tr("Content"));
	itemNoEdit = new StringLineEdit(QObject::tr("None"));
	itemWidget->addWidget(itemNoEdit);
	subGroup->addSubWidget(itemWidget);
	subGroup->setSubWidgetHeight();
	informationGroup->addSubWidget(subGroup);

	itemWidget = new PropertyItemWidget(QObject::tr("Supplier"));
	itemNoEdit = new StringLineEdit(QObject::tr("None"));
	itemWidget->addWidget(itemNoEdit);
	informationGroup->addSubWidget(itemWidget);
	informationGroup->setSubWidgetHeight();
	GetVBoxLayout()->addWidget(informationGroup);

	//Material
	informationGroup = new PropertyItemGroupWidget(this);
	itemWidget = new PropertyItemWidget(QObject::tr("Material"));
	informationGroup->addTopWidget(itemWidget);

	PropertyTabWidget* tabWidget = new PropertyTabWidget(this);
	subGroup = new PropertyItemGroupWidget();
	MaterialEditor* materialEditor = new MaterialEditor("");
	for (auto& itr : materialEditor->GetPropertyItemGroup())
	{
		subGroup->addSubWidget(itr);
	}
	connect(materialEditor, &MaterialEditor::updateSgn, this, &PropertyEditorContainer::UpdateUI);

	tabWidget->addTab(QObject::tr("Front"), subGroup);

	subGroup = new PropertyItemGroupWidget();
	MaterialEditor* materialEditor1 = new MaterialEditor("Use Same Material as Front");
	for (auto& itr : materialEditor1->GetPropertyItemGroup())
	{
		subGroup->addSubWidget(itr);
	}
	connect(materialEditor1, &MaterialEditor::updateSgn, this, &PropertyEditorContainer::UpdateUI);

	tabWidget->addTab(QObject::tr("Back"), subGroup);

	subGroup = new PropertyItemGroupWidget();
	MaterialEditor* materialEditor2 = new MaterialEditor("Use Same Material as Front");
	for (auto& itr : materialEditor2->GetPropertyItemGroup())
	{
		subGroup->addSubWidget(itr);
	}
	connect(materialEditor2, &MaterialEditor::updateSgn, this, &PropertyEditorContainer::UpdateUI);

	tabWidget->addTab(QObject::tr("Side"), subGroup);
	informationGroup->addSubWidget(tabWidget);
	informationGroup->setSubWidgetHeight();
	GetVBoxLayout()->addWidget(informationGroup);

	informationGroup = new PropertyItemGroupWidget(this);
	itemWidget = new PropertyItemWidget(QObject::tr("Physical Property"));
	informationGroup->addTopWidget(itemWidget);

	ComboBoxSaveOpenWidget* cmbOpenSave = new ComboBoxSaveOpenWidget(true,true,false);
	itemWidget = new PropertyItemWidget(QObject::tr("Preset"));
	itemWidget->addWidget(cmbOpenSave);
	informationGroup->addSubWidget(itemWidget);

	PropertyItemGroupWidget* detailGroup = new PropertyItemGroupWidget(this);
	itemWidget = new PropertyItemWidget(QObject::tr("Detail"));
	detailGroup->addTopWidget(itemWidget);

	itemWidget = new PropertyItemWidget(QObject::tr("Stretch-Weft"), nullptr, QString("expanded"));
	LineSliderEdit* sliderEdit = new LineSliderEdit(27);
	sliderEdit->setRange(0, 99);
	sliderEdit->setSingleStep(1);
	itemWidget->addWidget(sliderEdit);
	subGroup = new PropertyItemGroupWidget(this);
	subGroup->addTopWidget(itemWidget);

	DoubleLineEdit* doubleEdit = new DoubleLineEdit(100000);
	doubleEdit->setRange(1000, 10000000);
	doubleEdit->setSingleStep(1000);
	itemWidget = new PropertyItemWidget(QObject::tr("Stiffness(g/s2)"));
	itemWidget->addWidget(doubleEdit);
	subGroup->addSubWidget(itemWidget);
	subGroup->setSubWidgetHeight();
	detailGroup->addSubWidget(subGroup);

	itemWidget = new PropertyItemWidget(QObject::tr("Stretch-Wrap"), nullptr, QString("expanded"));
	sliderEdit = new LineSliderEdit(27);
	sliderEdit->setRange(0, 99);
	sliderEdit->setSingleStep(1);
	itemWidget->addWidget(sliderEdit);
	subGroup = new PropertyItemGroupWidget(this);
	subGroup->addTopWidget(itemWidget);

	doubleEdit = new DoubleLineEdit(100000);
	doubleEdit->setRange(1000, 10000000);
	doubleEdit->setSingleStep(1000);
	itemWidget = new PropertyItemWidget(QObject::tr("Stiffness(g/s2)"));
	itemWidget->addWidget(doubleEdit);
	subGroup->addSubWidget(itemWidget);
	subGroup->setSubWidgetHeight();
	detailGroup->addSubWidget(subGroup);

	itemWidget = new PropertyItemWidget(QObject::tr("Shear"), nullptr, QString("expanded"));
	sliderEdit = new LineSliderEdit(27);
	sliderEdit->setRange(0, 99);
	sliderEdit->setSingleStep(1);
	itemWidget->addWidget(sliderEdit);
	subGroup = new PropertyItemGroupWidget(this);
	subGroup->addTopWidget(itemWidget);

	doubleEdit = new DoubleLineEdit(100000);
	doubleEdit->setRange(1000, 10000000);
	doubleEdit->setSingleStep(1000);
	itemWidget = new PropertyItemWidget(QObject::tr("Stiffness(g/s2)"));
	itemWidget->addWidget(doubleEdit);
	subGroup->addSubWidget(itemWidget);
	subGroup->setSubWidgetHeight();
	detailGroup->addSubWidget(subGroup);

	itemWidget = new PropertyItemWidget(QObject::tr("Bending-Weft"), nullptr, QString("expanded"));
	sliderEdit = new LineSliderEdit(1);
	sliderEdit->setRange(0, 99);
	sliderEdit->setSingleStep(1);
	itemWidget->addWidget(sliderEdit);
	subGroup = new PropertyItemGroupWidget(this);
	subGroup->addTopWidget(itemWidget);

	doubleEdit = new DoubleLineEdit(0);
	doubleEdit->setRange(0, 2000000);
	doubleEdit->setSingleStep(10);
	itemWidget = new PropertyItemWidget(QObject::tr("Stiffness(g*mm2/s2/rad)"));
	itemWidget->addWidget(doubleEdit);
	subGroup->addSubWidget(itemWidget);
	subGroup->setSubWidgetHeight();
	detailGroup->addSubWidget(subGroup);

	itemWidget = new PropertyItemWidget(QObject::tr("Bending-Wrap"), nullptr, QString("expanded"));
	sliderEdit = new LineSliderEdit(20);
	sliderEdit->setRange(0, 99);
	sliderEdit->setSingleStep(1);
	itemWidget->addWidget(sliderEdit);
	subGroup = new PropertyItemGroupWidget(this);
	subGroup->addTopWidget(itemWidget);

	doubleEdit = new DoubleLineEdit(600);
	doubleEdit->setRange(0, 2000000);
	doubleEdit->setSingleStep(10);
	itemWidget = new PropertyItemWidget(QObject::tr("Stiffness(g*mm2/s2/rad)"));
	itemWidget->addWidget(doubleEdit);
	subGroup->addSubWidget(itemWidget);
	subGroup->setSubWidgetHeight();
	detailGroup->addSubWidget(subGroup);

	itemWidget = new PropertyItemWidget(QObject::tr("Bending-Bias"), nullptr, QString("expanded"));
	sliderEdit = new LineSliderEdit(11);
	sliderEdit->setRange(0, 99);
	sliderEdit->setSingleStep(1);
	itemWidget->addWidget(sliderEdit);
	subGroup = new PropertyItemGroupWidget(this);
	subGroup->addTopWidget(itemWidget);

	doubleEdit = new DoubleLineEdit(150);
	doubleEdit->setRange(0, 2000000);
	doubleEdit->setSingleStep(10);
	itemWidget = new PropertyItemWidget(QObject::tr("Stiffness(g*mm2/s2/rad)"));
	itemWidget->addWidget(doubleEdit);
	subGroup->addSubWidget(itemWidget);
	subGroup->setSubWidgetHeight();
	detailGroup->addSubWidget(subGroup);

	itemWidget = new PropertyItemWidget(QObject::tr("Buckling Ratio-Weft"), nullptr, QString("expanded"));
	sliderEdit = new LineSliderEdit(11);
	sliderEdit->setRange(0, 100);
	sliderEdit->setSingleStep(1);
	itemWidget->addWidget(sliderEdit);
	subGroup = new PropertyItemGroupWidget(this);
	subGroup->addTopWidget(itemWidget);

	doubleEdit = new DoubleLineEdit(0.11);
	doubleEdit->setRange(0.00, 1.00);
	doubleEdit->setSingleStep(0.01);
	doubleEdit->setDecimals(2);
	itemWidget = new PropertyItemWidget(QObject::tr("Length Ratio[0~1]"));
	itemWidget->addWidget(doubleEdit);
	subGroup->addSubWidget(itemWidget);
	subGroup->setSubWidgetHeight();
	detailGroup->addSubWidget(subGroup);

	itemWidget = new PropertyItemWidget(QObject::tr("Buckling Ratio-Wrap"), nullptr, QString("expanded"));
	sliderEdit = new LineSliderEdit(11);
	sliderEdit->setRange(0, 100);
	sliderEdit->setSingleStep(1);
	itemWidget->addWidget(sliderEdit);
	subGroup = new PropertyItemGroupWidget(this);
	subGroup->addTopWidget(itemWidget);

	doubleEdit = new DoubleLineEdit(0.11);
	doubleEdit->setRange(0.00, 1.00);
	doubleEdit->setSingleStep(0.01);
	doubleEdit->setDecimals(2);
	itemWidget = new PropertyItemWidget(QObject::tr("Length Ratio[0~1]"));
	itemWidget->addWidget(doubleEdit);
	subGroup->addSubWidget(itemWidget);
	subGroup->setSubWidgetHeight();
	detailGroup->addSubWidget(subGroup);

	itemWidget = new PropertyItemWidget(QObject::tr("Buckling Ratio-Bias"), nullptr, QString("expanded"));
	sliderEdit = new LineSliderEdit(11);
	sliderEdit->setRange(0, 100);
	sliderEdit->setSingleStep(1);
	itemWidget->addWidget(sliderEdit);
	subGroup = new PropertyItemGroupWidget(this);
	subGroup->addTopWidget(itemWidget);

	doubleEdit = new DoubleLineEdit(0.11);
	doubleEdit->setRange(0.00, 1.00);
	doubleEdit->setSingleStep(0.01);
	doubleEdit->setDecimals(2);
	itemWidget = new PropertyItemWidget(QObject::tr("Length Ratio[0~1]"));
	itemWidget->addWidget(doubleEdit);
	subGroup->addSubWidget(itemWidget);
	subGroup->setSubWidgetHeight();
	detailGroup->addSubWidget(subGroup);

	itemWidget = new PropertyItemWidget(QObject::tr("Buckling Stiffness-Weft"), nullptr, QString("expanded"));
	sliderEdit = new LineSliderEdit(11);
	sliderEdit->setRange(0, 100);
	sliderEdit->setSingleStep(1);
	itemWidget->addWidget(sliderEdit);
	subGroup = new PropertyItemGroupWidget(this);
	subGroup->addTopWidget(itemWidget);

	doubleEdit = new DoubleLineEdit(0.11);
	doubleEdit->setRange(0.00, 1.00);
	doubleEdit->setSingleStep(0.01);
	doubleEdit->setDecimals(2);
	itemWidget = new PropertyItemWidget(QObject::tr("[0~1]"));
	itemWidget->addWidget(doubleEdit);
	subGroup->addSubWidget(itemWidget);
	subGroup->setSubWidgetHeight();
	detailGroup->addSubWidget(subGroup);

	itemWidget = new PropertyItemWidget(QObject::tr("Buckling Stiffness-Warp"), nullptr, QString("expanded"));
	sliderEdit = new LineSliderEdit(11);
	sliderEdit->setRange(0, 100);
	sliderEdit->setSingleStep(1);
	itemWidget->addWidget(sliderEdit);
	subGroup = new PropertyItemGroupWidget(this);
	subGroup->addTopWidget(itemWidget);

	doubleEdit = new DoubleLineEdit(0.11);
	doubleEdit->setRange(0.00, 1.00);
	doubleEdit->setSingleStep(0.01);
	doubleEdit->setDecimals(2);
	itemWidget = new PropertyItemWidget(QObject::tr("[0~1]"));
	itemWidget->addWidget(doubleEdit);
	subGroup->addSubWidget(itemWidget);
	subGroup->setSubWidgetHeight();
	detailGroup->addSubWidget(subGroup);

	itemWidget = new PropertyItemWidget(QObject::tr("Buckling Stiffness-Bias"), nullptr, QString("expanded"));
	sliderEdit = new LineSliderEdit(11);
	sliderEdit->setRange(0, 100);
	sliderEdit->setSingleStep(1);
	itemWidget->addWidget(sliderEdit);
	subGroup = new PropertyItemGroupWidget(this);
	subGroup->addTopWidget(itemWidget);

	doubleEdit = new DoubleLineEdit(0.11);
	doubleEdit->setRange(0.00, 1.00);
	doubleEdit->setSingleStep(0.01);
	doubleEdit->setDecimals(2);
	itemWidget = new PropertyItemWidget(QObject::tr("[0~1]"));
	itemWidget->addWidget(doubleEdit);
	subGroup->addSubWidget(itemWidget);
	subGroup->setSubWidgetHeight();
	detailGroup->addSubWidget(subGroup);

	itemWidget = new PropertyItemWidget(QObject::tr("Internal Damping"), nullptr, QString("expanded"));
	sliderEdit = new LineSliderEdit(11);
	sliderEdit->setRange(0, 99);
	sliderEdit->setSingleStep(1);
	itemWidget->addWidget(sliderEdit);
	subGroup = new PropertyItemGroupWidget(this);
	subGroup->addTopWidget(itemWidget);

	doubleEdit = new DoubleLineEdit(0.001100);
	doubleEdit->setRange(0.000000, 0.009900);
	doubleEdit->setSingleStep(0.0001);
	doubleEdit->setDecimals(4);
	itemWidget = new PropertyItemWidget(QObject::tr(""));
	itemWidget->addWidget(doubleEdit);
	subGroup->addSubWidget(itemWidget);
	subGroup->setSubWidgetHeight();
	detailGroup->addSubWidget(subGroup);

	itemWidget = new PropertyItemWidget(QObject::tr("Density"), nullptr, QString("expanded"));
	sliderEdit = new LineSliderEdit(11);
	sliderEdit->setRange(0, 99);
	sliderEdit->setSingleStep(1);
	itemWidget->addWidget(sliderEdit);
	subGroup = new PropertyItemGroupWidget(this);
	subGroup->addTopWidget(itemWidget);

	doubleEdit = new DoubleLineEdit(140.00);
	doubleEdit->setRange(30.00, 2100.00);
	doubleEdit->setSingleStep(10);
	doubleEdit->setDecimals(2);
	itemWidget = new PropertyItemWidget(QObject::tr("g/m2"));
	itemWidget->addWidget(doubleEdit);
	subGroup->addSubWidget(itemWidget);
	subGroup->setSubWidgetHeight();
	detailGroup->addSubWidget(subGroup);

	itemWidget = new PropertyItemWidget(QObject::tr("Friction"), nullptr, QString("expanded"));
	sliderEdit = new LineSliderEdit(11);
	sliderEdit->setRange(0, 100);
	sliderEdit->setSingleStep(1);
	itemWidget->addWidget(sliderEdit);
	subGroup = new PropertyItemGroupWidget(this);
	subGroup->addTopWidget(itemWidget);

	doubleEdit = new DoubleLineEdit(0.11);
	doubleEdit->setRange(0.00, 1.00);
	doubleEdit->setSingleStep(0.01);
	doubleEdit->setDecimals(2);
	itemWidget = new PropertyItemWidget(QObject::tr("[0~1]"));
	itemWidget->addWidget(doubleEdit);
	subGroup->addSubWidget(itemWidget);
	subGroup->setSubWidgetHeight();
	detailGroup->addSubWidget(subGroup);

	detailGroup->setSubWidgetHeight();
	informationGroup->addSubWidget(detailGroup);
	informationGroup->setSubWidgetHeight();
	GetVBoxLayout()->addWidget(informationGroup);

	informationGroup = new PropertyItemGroupWidget(this);
	doubleEdit = new DoubleLineEdit(0.50);
	doubleEdit->setRange(0.00, 100000000.00);
	doubleEdit->setSingleStep(1.00);
	doubleEdit->setDecimals(2);
	itemWidget = new PropertyItemWidget(QObject::tr("Thickness"));
	itemWidget->addWidget(doubleEdit);
	informationGroup->addTopWidget(itemWidget);
	GetVBoxLayout()->addWidget(informationGroup);
}

MaterialEditor::MaterialEditor(const QString& sameMaterialText, QObject* parent /*= nullptr*/):QObject(parent),sameMaterialText_(sameMaterialText)
{
	InitUI();
}

MaterialEditor::~MaterialEditor()
{

}

QList<PropertyItemGroupWidget*> MaterialEditor::GetPropertyItemGroup() const
{
	return propertyItemGroups_;
}

void MaterialEditor::InitUI()
{
	if (!sameMaterialText_.isEmpty())
	{
		ckbSameMaterial_ = new CheckBoxWidget(QObject::tr("On"), QObject::tr("Off"));
		connect(ckbSameMaterial_, &CheckBoxWidget::checkStateSgn, this, &MaterialEditor::onCheckClick);
		PropertyItemWidget* itemWidget = new PropertyItemWidget(sameMaterialText_);
		itemWidget->addWidget(ckbSameMaterial_);
		PropertyItemGroupWidget* informationGroup = new PropertyItemGroupWidget();
		informationGroup->addTopWidget(itemWidget);
		propertyItemGroups_.append(informationGroup);
	}
	materialWidget_ = new PropertyItemGroupWidget();

	PropertyItemGroupWidget* informationGroup = new PropertyItemGroupWidget();
	PropertyItemWidget* itemWidget = new PropertyItemWidget(QObject::tr("Type"));
	PropertyComboBox* comboBox = new PropertyComboBox();
	QStringList textList;
	QStringList dataList;
	textList.clear();
	dataList.clear();
	textList << QObject::tr("Fabric_Matte") << QObject::tr("Fabric_Shiny") << QObject::tr("Fabric_Silk/Satin") << QObject::tr("Fabric_Velvet") << QObject::tr("Fur(Render Only)")
		<< QObject::tr("Gem(Render Only)") << QObject::tr("Glass(Render Only)") << QObject::tr("Glitter(Render Only)") << QObject::tr("Iridescence(Render Only)") << QObject::tr("Light(Render Only)")
		<< QObject::tr("Leather") << QObject::tr("Metal") << QObject::tr("Plastic") << QObject::tr("Skin(Render Only)");
	dataList << QString("Fabric_Matte") << QString("Fabric_Shiny") << QString("Fabric_Silk/Satin") << QString("Fabric_Velvet") << QString("Fur(Render Only)")
		<< QString("Gem(Render Only)") << QString("Glass(Render Only)") << QString("Glitter(Render Only)") << QString("Iridescence(Render Only)") << QString("Light(Render Only)")
		<< QString("Leather") << QString("Metal") << QString("Plastic") << QString("Skin(Render Only)");
	comboBox->AddItemList(textList, dataList);
	itemWidget->addWidget(comboBox);
	informationGroup->addTopWidget(itemWidget);
	materialWidget_->addSubWidget(informationGroup);

	PropertyItemGroupWidget* basicGroup = new PropertyItemGroupWidget();
	itemWidget = new PropertyItemWidget(QObject::tr("Basic Parameters"));
	basicGroup->addTopWidget(itemWidget);
	connect(basicGroup, &PropertyItemGroupWidget::uiStateSgn, this, &MaterialEditor::updateSgn);

	QString dirPath = QString(" C:/Users/Administrator/AppData/Local/CLO/CLO Standalone/18076/1040");
	ImageSelectWidget* textureSelect = new ImageSelectWidget(dirPath);
	itemWidget = new PropertyItemWidget(QObject::tr("Texture"));
	PropertyItemGroupWidget* subGroup = new PropertyItemGroupWidget();
	subGroup->addTopWidget(itemWidget);

	CheckBoxWidget* checkEdit = new CheckBoxWidget(QObject::tr("On"), QObject::tr("Off"));
	itemWidget = new PropertyItemWidget(QObject::tr("Desaturation"));
	itemWidget->addWidget(checkEdit);
	subGroup->addSubWidget(itemWidget);
	subGroup->setSubWidgetHeight();
	connect(subGroup, &PropertyItemGroupWidget::uiStateSgn, this, &MaterialEditor::updateSgn);
	basicGroup->addSubWidget(subGroup);

	PropertyItemGroupWidget* normalGroup = new PropertyItemGroupWidget();
	itemWidget = new PropertyItemWidget(QObject::tr("Normal Map"));
	ImageSelectWidget* normalSelect = new ImageSelectWidget(dirPath);
	itemWidget->addWidget(normalSelect);
	normalGroup->addTopWidget(itemWidget);

	itemWidget = new PropertyItemWidget(QObject::tr("Intensity"), nullptr, QString("expanded"));
	LineSliderEdit* intensitySlider = new LineSliderEdit(0);
	intensitySlider->setRange(-100, 100);
	intensitySlider->setSingleStep(1);
	itemWidget->addWidget(intensitySlider);
	normalGroup->addSubWidget(itemWidget);
	normalGroup->setSubWidgetHeight();
	connect(normalGroup, &PropertyItemGroupWidget::uiStateSgn, this, &MaterialEditor::updateSgn);
	basicGroup->addSubWidget(normalGroup);

	PropertyItemGroupWidget* displacementGroup = new PropertyItemGroupWidget();
	itemWidget = new PropertyItemWidget(QObject::tr("Displacement Map"));
	ImageSelectWidget* displaceSelect = new ImageSelectWidget(dirPath);
	itemWidget->addWidget(displaceSelect);
	displacementGroup->addTopWidget(itemWidget);

	DoubleLineEdit* amountEdit = new DoubleLineEdit(3.00);
	amountEdit->setRange(0.00, 100.00);
	amountEdit->setSingleStep(1.0);
	amountEdit->setDecimals(2);
	itemWidget = new PropertyItemWidget(QObject::tr("Amount(mm)"));
	itemWidget->addWidget(amountEdit);
	displacementGroup->addSubWidget(itemWidget);

	amountEdit = new DoubleLineEdit(0.00);
	amountEdit->setRange(0.00, 100.00);
	amountEdit->setSingleStep(1.0);
	amountEdit->setDecimals(2);
	itemWidget = new PropertyItemWidget(QObject::tr("Shift(mm)"));
	itemWidget->addWidget(amountEdit);
	displacementGroup->addSubWidget(itemWidget);

	amountEdit = new DoubleLineEdit(0.00);
	amountEdit->setRange(0.00, 100.00);
	amountEdit->setSingleStep(1.0);
	amountEdit->setDecimals(2);
	itemWidget = new PropertyItemWidget(QObject::tr("Clipping(mm)"));
	itemWidget->addWidget(amountEdit);
	displacementGroup->addSubWidget(itemWidget);

	amountEdit = new DoubleLineEdit(4.00);
	amountEdit->setRange(0.00, 100.00);
	amountEdit->setSingleStep(1.0);
	amountEdit->setDecimals(2);
	itemWidget = new PropertyItemWidget(QObject::tr("Particle Distance(mm)"));
	itemWidget->addWidget(amountEdit);
	displacementGroup->addSubWidget(itemWidget);

	checkEdit = new CheckBoxWidget(QObject::tr("On"), QObject::tr("Off"));
	itemWidget = new PropertyItemWidget(QObject::tr("Keep Continuity"));
	itemWidget->addWidget(checkEdit);
	displacementGroup->addSubWidget(itemWidget);
	displacementGroup->setSubWidgetHeight();
	connect(displacementGroup, &PropertyItemGroupWidget::uiStateSgn, this, &MaterialEditor::updateSgn);
	basicGroup->addSubWidget(displacementGroup);

	ColorLineEdit* colorEdit = new ColorLineEdit();
	itemWidget = new PropertyItemWidget(QObject::tr("Color"));
	itemWidget->addWidget(colorEdit);
	subGroup = new PropertyItemGroupWidget();
	subGroup->addTopWidget(itemWidget);
	basicGroup->addSubWidget(subGroup);

	itemWidget = new PropertyItemWidget(QObject::tr("Opacity"), nullptr, QString("expanded"));
	LineSliderEdit* opacitySlider = new LineSliderEdit(100);
	opacitySlider->setRange(0, 100);
	opacitySlider->setSingleStep(1);
	itemWidget->addWidget(opacitySlider);
	subGroup = new PropertyItemGroupWidget();
	subGroup->addTopWidget(itemWidget);
	basicGroup->addSubWidget(subGroup);

	PropertyItemGroupWidget* opacityGroup = new PropertyItemGroupWidget();
	itemWidget = new PropertyItemWidget(QObject::tr("Opacity Map"));
	ImageSelectWidget* opacitySelect = new ImageSelectWidget(dirPath);
	itemWidget->addWidget(opacitySelect);
	opacityGroup->addTopWidget(itemWidget);

	itemWidget = new PropertyItemWidget(QObject::tr("Mode"));
	comboBox = new PropertyComboBox();
	textList.clear();
	dataList.clear();
	textList << QObject::tr("RGB") << QObject::tr("ALPHA");
	dataList << QString("RGB") << QString("ALPHA");
	comboBox->AddItemList(textList, dataList);
	itemWidget->addWidget(comboBox);
	opacityGroup->addSubWidget(itemWidget);
	opacityGroup->setSubWidgetHeight();
	connect(opacityGroup, &PropertyItemGroupWidget::uiStateSgn, this, &MaterialEditor::updateSgn);
	basicGroup->addSubWidget(opacityGroup);
	basicGroup->setSubWidgetHeight();
	materialWidget_->addSubWidget(basicGroup);

	//Reflection
	PropertyItemGroupWidget* reflectionGroup = new PropertyItemGroupWidget();
	itemWidget = new PropertyItemWidget(QObject::tr("Reflection"));
	reflectionGroup->addTopWidget(itemWidget);

	PropertyItemGroupWidget* roughnessGroup = new PropertyItemGroupWidget();
	itemWidget = new PropertyItemWidget(QObject::tr("roughnessGroup"));
	comboBox = new PropertyComboBox();
	textList.clear();
	dataList.clear();
	textList << QObject::tr("Intensity") << QObject::tr("Map");
	dataList << QString("Intensity") << QString("Map");
	comboBox->AddItemList(textList, dataList);
	itemWidget->addWidget(comboBox);
	roughnessGroup->addTopWidget(itemWidget);

	itemWidget = new PropertyItemWidget(QObject::tr(""), nullptr, QString("expanded"));
	LineSliderEdit* sliderEdit = new LineSliderEdit(61);
	sliderEdit->setRange(0, 100);
	sliderEdit->setSingleStep(1);
	itemWidget->addWidget(sliderEdit);
	roughnessGroup->addSubWidget(itemWidget);
	roughnessGroup->setSubWidgetHeight();
	connect(roughnessGroup, &PropertyItemGroupWidget::uiStateSgn, this, &MaterialEditor::updateSgn);
	reflectionGroup->addSubWidget(roughnessGroup);

	itemWidget = new PropertyItemWidget(QObject::tr("Reflection Intensity"), nullptr, QString("expanded"));
	sliderEdit = new LineSliderEdit(15);
	sliderEdit->setRange(0, 100);
	sliderEdit->setSingleStep(1);
	itemWidget->addWidget(sliderEdit);
	subGroup = new PropertyItemGroupWidget();
	subGroup->addTopWidget(itemWidget);
	reflectionGroup->addSubWidget(subGroup);

	itemWidget = new PropertyItemWidget(QObject::tr("Metalness"), nullptr, QString("expanded"));
	sliderEdit = new LineSliderEdit(0);
	sliderEdit->setRange(0, 100);
	sliderEdit->setSingleStep(1);
	itemWidget->addWidget(sliderEdit);
	subGroup = new PropertyItemGroupWidget();
	subGroup->addTopWidget(itemWidget);
	reflectionGroup->addSubWidget(subGroup);

	itemWidget = new PropertyItemWidget(QObject::tr("Metalness Map"));
	textureSelect = new ImageSelectWidget(dirPath);
	itemWidget->addWidget(textureSelect);
	subGroup = new PropertyItemGroupWidget();
	subGroup->addTopWidget(itemWidget);
	reflectionGroup->addSubWidget(subGroup);

	reflectionGroup->setSubWidgetHeight();
	connect(reflectionGroup, &PropertyItemGroupWidget::uiStateSgn, this, &MaterialEditor::updateSgn);
	materialWidget_->addSubWidget(reflectionGroup);
	materialWidget_->setSubWidgetHeight();
	propertyItemGroups_.append(materialWidget_);

	if (ckbSameMaterial_)
	{
		ckbSameMaterial_->setChecked(false);
	}
}

void MaterialEditor::onCheckClick(bool bcheck)
{
	if (materialWidget_)
	{
		materialWidget_->setEnabled(bcheck);
	}
}

ButtonEditor::ButtonEditor(QWidget* parent /*= nullptr*/):PropertyEditorContainer(parent)
{
	InitEditor();
}

ButtonEditor::~ButtonEditor()
{

}

void ButtonEditor::InitEditor()
{
	PropertyItemGroupWidget* informationGroup = new PropertyItemGroupWidget(this);
	PropertyItemWidget* itemWidget = new PropertyItemWidget(QObject::tr("Button"));
	ButtonSaveOpenWidget* buttonWidget = new ButtonSaveOpenWidget(this);
	itemWidget->addWidget(buttonWidget);
	informationGroup->addTopWidget(itemWidget);
	GetVBoxLayout()->addWidget(informationGroup);

	//information
	informationGroup = new PropertyItemGroupWidget(this);
	itemWidget = new PropertyItemWidget(QObject::tr("Information"));
	informationGroup->addTopWidget(itemWidget);
	StringLineEdit* nameEdit = new StringLineEdit(QObject::tr("Default Button"));
	itemWidget = new PropertyItemWidget(QObject::tr("Name"));
	itemWidget->addWidget(nameEdit);
	informationGroup->addSubWidget(itemWidget);

	StringLineEdit* itemNoEdit = new StringLineEdit(QObject::tr("None"));
	itemWidget = new PropertyItemWidget(QObject::tr("Item No."));
	itemWidget->addWidget(itemNoEdit);
	informationGroup->addSubWidget(itemWidget);

	CheckBoxWidget* checkEdit = new CheckBoxWidget(QObject::tr("Include"), QObject::tr("Exclude"));
	itemWidget = new PropertyItemWidget(QObject::tr("Tech Pack(CLO-SET)"));
	itemWidget->addWidget(checkEdit);
	informationGroup->addSubWidget(itemWidget);

	informationGroup->setSubWidgetHeight();
	GetVBoxLayout()->addWidget(informationGroup);

	informationGroup = new PropertyItemGroupWidget();
	itemWidget = new PropertyItemWidget(QObject::tr("Shape"));
	itemNoEdit = new StringLineEdit(QObject::tr("None"));
	itemWidget->addWidget(itemNoEdit);
	informationGroup->addTopWidget(itemWidget);

	itemWidget = new PropertyItemWidget(QObject::tr("Thread Type"));
	PropertyComboBox* cmb = new PropertyComboBox();
	cmb->AddItemList(QStringList() << QObject::tr("Cross") << QObject::tr("Parallel") << QObject::tr("Square"), QStringList() <<QString("Cross") << QString("Parallel") << QString("Square"));
	itemWidget->addWidget(cmb);
	informationGroup->addSubWidget(itemWidget);
	informationGroup->setSubWidgetHeight();
	GetVBoxLayout()->addWidget(informationGroup);

	informationGroup = new PropertyItemGroupWidget();
	itemWidget = new PropertyItemWidget(QObject::tr("Dimension && Weight"));
	informationGroup->addTopWidget(itemWidget);

	DoubleLineEdit* doubleEdit = new DoubleLineEdit(20.0);
	doubleEdit->setRange(0.0, 100.0);
	doubleEdit->setSingleStep(1.0);
	doubleEdit->setDecimals(1);
	itemWidget = new PropertyItemWidget(QObject::tr("Width(mm)"));
	itemWidget->addWidget(doubleEdit);
	informationGroup->addSubWidget(itemWidget);

	doubleEdit = new DoubleLineEdit(3.0);
	doubleEdit->setRange(0.0, 100.0);
	doubleEdit->setSingleStep(1.0);
	doubleEdit->setDecimals(1);
	itemWidget = new PropertyItemWidget(QObject::tr("Thickness(mm)"));
	itemWidget->addWidget(doubleEdit);
	informationGroup->addSubWidget(itemWidget);

	doubleEdit = new DoubleLineEdit(0.4);
	doubleEdit->setRange(0.0, 1000000000.0);
	doubleEdit->setSingleStep(1.0);
	doubleEdit->setDecimals(1);
	itemWidget = new PropertyItemWidget(QObject::tr("Weight(g)"));
	itemWidget->addWidget(doubleEdit);
	informationGroup->addSubWidget(itemWidget);
	informationGroup->setSubWidgetHeight();
	GetVBoxLayout()->addWidget(informationGroup);
	//Material
	informationGroup = new PropertyItemGroupWidget(this);
	itemWidget = new PropertyItemWidget(QObject::tr("Material"));
	informationGroup->addTopWidget(itemWidget);

	PropertyTabWidget* tabWidget = new PropertyTabWidget(this);
	PropertyItemGroupWidget* subGroup = new PropertyItemGroupWidget();
	MaterialEditor* materialEditor = new MaterialEditor("");
	for (auto& itr : materialEditor->GetPropertyItemGroup())
	{
		subGroup->addSubWidget(itr);
	}
	connect(materialEditor, &MaterialEditor::updateSgn, this, &PropertyEditorContainer::UpdateUI);

	tabWidget->addTab(QObject::tr("Button"), subGroup);

	subGroup = new PropertyItemGroupWidget();
	MaterialEditor* materialEditor1 = new MaterialEditor("Use Same Material as Button");
	for (auto& itr : materialEditor1->GetPropertyItemGroup())
	{
		subGroup->addSubWidget(itr);
	}
	connect(materialEditor1, &MaterialEditor::updateSgn, this, &PropertyEditorContainer::UpdateUI);

	tabWidget->addTab(QObject::tr("Thread"), subGroup);

	
	informationGroup->addSubWidget(tabWidget);
	informationGroup->setSubWidgetHeight();
	GetVBoxLayout()->addWidget(informationGroup);
}

ButtonholeEditor::ButtonholeEditor(QWidget* parent /*= nullptr*/) :PropertyEditorContainer(parent)
{
	InitEditor();
}

ButtonholeEditor::~ButtonholeEditor()
{

}

void ButtonholeEditor::InitEditor()
{
	PropertyItemGroupWidget* informationGroup = new PropertyItemGroupWidget(this);
	PropertyItemWidget* itemWidget = new PropertyItemWidget(QObject::tr("Button"));
	ButtonSaveOpenWidget* buttonWidget = new ButtonSaveOpenWidget(this);
	itemWidget->addWidget(buttonWidget);
	informationGroup->addTopWidget(itemWidget);
	GetVBoxLayout()->addWidget(informationGroup);

	//information
	informationGroup = new PropertyItemGroupWidget(this);
	itemWidget = new PropertyItemWidget(QObject::tr("Information"));
	informationGroup->addTopWidget(itemWidget);
	StringLineEdit* nameEdit = new StringLineEdit(QObject::tr("Default Button"));
	itemWidget = new PropertyItemWidget(QObject::tr("Name"));
	itemWidget->addWidget(nameEdit);
	informationGroup->addSubWidget(itemWidget);

	StringLineEdit* itemNoEdit = new StringLineEdit(QObject::tr("None"));
	itemWidget = new PropertyItemWidget(QObject::tr("Item No."));
	itemWidget->addWidget(itemNoEdit);
	informationGroup->addSubWidget(itemWidget);

	CheckBoxWidget* checkEdit = new CheckBoxWidget(QObject::tr("Include"), QObject::tr("Exclude"));
	itemWidget = new PropertyItemWidget(QObject::tr("Tech Pack(CLO-SET)"));
	itemWidget->addWidget(checkEdit);
	informationGroup->addSubWidget(itemWidget);

	informationGroup->setSubWidgetHeight();
	GetVBoxLayout()->addWidget(informationGroup);

	informationGroup = new PropertyItemGroupWidget();
	itemWidget = new PropertyItemWidget(QObject::tr("Shape"));
	itemNoEdit = new StringLineEdit(QObject::tr("None"));
	itemWidget->addWidget(itemNoEdit);
	informationGroup->addTopWidget(itemWidget);

	GetVBoxLayout()->addWidget(informationGroup);

	informationGroup = new PropertyItemGroupWidget();
	itemWidget = new PropertyItemWidget(QObject::tr("Dimension"));
	informationGroup->addTopWidget(itemWidget);

	DoubleLineEdit* doubleEdit = new DoubleLineEdit(20.0);
	doubleEdit->setRange(0.0, 100.0);
	doubleEdit->setSingleStep(1.0);
	doubleEdit->setDecimals(1);
	itemWidget = new PropertyItemWidget(QObject::tr("Width(mm)"));
	itemWidget->addWidget(doubleEdit);
	informationGroup->addSubWidget(itemWidget);
	informationGroup->setSubWidgetHeight();
	GetVBoxLayout()->addWidget(informationGroup);
	//Material
	informationGroup = new PropertyItemGroupWidget(this);
	itemWidget = new PropertyItemWidget(QObject::tr("Material"));
	informationGroup->addTopWidget(itemWidget);

	MaterialEditor* materialEditor = new MaterialEditor("");
	for (auto& itr : materialEditor->GetPropertyItemGroup())
	{
		informationGroup->addSubWidget(itr);
	}
	connect(materialEditor, &MaterialEditor::updateSgn, this, &PropertyEditorContainer::UpdateUI);

	informationGroup->setSubWidgetHeight();
	GetVBoxLayout()->addWidget(informationGroup);
}
