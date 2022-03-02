#ifndef PROPERTYEDITORWIDGET_H
#define PROPERTYEDITORWIDGET_H
#include <QWidget>
#include "PropertyCommonControl.h"

class QScrollArea;
class QVBoxLayout;

class PropertyEditorContainer : public QWidget 
{
	Q_OBJECT

public:
	explicit PropertyEditorContainer(QWidget* parent = nullptr);
	~PropertyEditorContainer();
	virtual void InitEditor() = 0;
	QVBoxLayout* GetVBoxLayout() const;
	QWidget* GetScrollWidget() const;
public slots:
	void UpdateUI();
private:
	void InitUI();

private:
	QScrollArea* scrollArea_ = nullptr;
	QWidget* scrollWidget_ = nullptr;
	QVBoxLayout* scrollLayout_ = nullptr;
};

class AvatarBodyEditor : public PropertyEditorContainer
{
	Q_OBJECT

public:
	explicit AvatarBodyEditor(QWidget* parent = nullptr);
	~AvatarBodyEditor();

	void InitEditor();
};

class FabricEditor : public PropertyEditorContainer
{
	Q_OBJECT

public:
	explicit FabricEditor(QWidget* parent = nullptr);
	~FabricEditor();

	void InitEditor();
};

class ButtonEditor : public PropertyEditorContainer
{
	Q_OBJECT

public:
	explicit ButtonEditor(QWidget* parent = nullptr);
	~ButtonEditor();

	void InitEditor();
};

class ButtonholeEditor : public PropertyEditorContainer
{
	Q_OBJECT

public:
	explicit ButtonholeEditor(QWidget* parent = nullptr);
	~ButtonholeEditor();

	void InitEditor();
};

class MaterialEditor: public QObject
{
	Q_OBJECT

public:
	explicit MaterialEditor(const QString& sameMaterialText, QObject* parent = nullptr);
	~MaterialEditor();
	QList<PropertyItemGroupWidget*> GetPropertyItemGroup() const;
signals:
	void updateSgn();
private:
	void InitUI();
private slots:
	void onCheckClick(bool bcheck);
private:
	QString sameMaterialText_;
	PropertyItemGroupWidget* materialWidget_ = nullptr;
	QList<PropertyItemGroupWidget*> propertyItemGroups_;
	CheckBoxWidget* ckbSameMaterial_ = nullptr;
};
#endif // PROPERTYEDITORWIDGET_H


