#pragma once

#include <QtWidgets/QWidget>
#include "GeneratedFiles/ui_scatterDialog.h"
#include "Document.h"
class scatterWidget : public QDialog
{
	Q_OBJECT

public:
	scatterWidget(QWidget *parent = Q_NULLPTR);
	~scatterWidget();

private slots:
	
	/**
	  * @Method:    openTestFile
	  * @Note: 	打开实测文件
	  * @Author:    Li Gen
	  * @ Date:     2017/11/21
	  * @Access:    private 
	  * @Returns:   void
	  * @Qualifier:
	*/
	void openTestFile();


	/**
	  * @Method:    showResult
	  * @Note: 	 展示散点图结果
	  * @Author:    Li Gen
	  * @ Date:     2017/11/21
	  * @Access:    private 
	  * @Returns:   void
	  * @Qualifier:
	*/
	void showResult();
private:
	Ui::scatterDialog *ui;
	Document m_content;
	QJsonArray dataArray;
};
