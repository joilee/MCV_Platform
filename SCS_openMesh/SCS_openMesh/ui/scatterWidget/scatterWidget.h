#pragma once

#include <QtWidgets/QWidget>
#include "GeneratedFiles/ui_scatterDialog.h"
#include "Document.h"
#include "util/EField.h"
#include "correctWidget.h"
class scatterWidget : public QDialog
{
	Q_OBJECT

public:
	scatterWidget(QWidget *parent = Q_NULLPTR);
	~scatterWidget();

private slots:
	
	/**
	  * @Method:    openTestFile
	  * @Note: 	��ʵ���ļ�
	  * @Author:    Li Gen
	  * @ Date:     2017/11/21
	  * @Access:    private 
	  * @Returns:   void
	  * @Qualifier:
	*/
	void openTestFile();

	void openMeasuredFile();

	/**
	  * @Method:    showResult
	  * @Note: 	 չʾɢ��ͼ���
	  * @Author:    Li Gen
	  * @ Date:     2017/11/21
	  * @Access:    private 
	  * @Returns:   void
	  * @Qualifier:
	*/
	void showResult();

	void showPara();

	void modeButtonToggled(int id, bool flag);
private:
	Ui::scatterDialog *ui;
	QButtonGroup *modeGroup;
	Document m_content;

	correctWidget * dataWidget;
	//data
	void initData();
	void enableShowMode(bool a);
	void enableCorrectMode(bool a);
};
