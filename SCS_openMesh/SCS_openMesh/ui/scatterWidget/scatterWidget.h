#pragma once

#include <QtWidgets/QWidget>
#include "GeneratedFiles/ui_scatterDialog.h"
#include "Document.h"
#include "util/EField.h"
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

	void setSourceEField(vector<EField>& _placePoint);
private:
	Ui::scatterDialog *ui;
	Document m_content;
	QJsonArray dataArray;

	//data
	vector<EField> placePoint;//�����վ������
	vector<EField> testPoint;//ʵ������
	void initData();
};
