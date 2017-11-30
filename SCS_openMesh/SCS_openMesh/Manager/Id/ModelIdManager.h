#pragma once
#include <string>
#include <map>
#define DEFAULT_MODEL_ID 0
using namespace std;

/************************************************************************/
/* id��������
1.��ÿһ��ģ�ͷ����һ�޶���ID
*/
/************************************************************************/
class ModelIdManager
{
public:
	ModelIdManager();
	~ModelIdManager();

	/**
	  * @Method:    getModelID
	  * @Note: 	��ģ�ͷ���ID�����û�в����������Ĭ��ID���䡣���id�ڵ�ǰ�Ѿ������д��ڣ��������ӣ�ֱ����һ��
	  * @Author:    Li Gen
	  * @ Date:     2017/11/25
	  * @Access:    public 
	  * @Returns:   int
	  * @Qualifier:
	  * @Parameter: string name,int id
	*/
	int getModelID(string name,int id=DEFAULT_MODEL_ID);

	/**
	  * @Method:    getNextID
	  * @Note: 	���ڷ�����ͻ���ظ�����id��Ѱ����һ������id
	  * @Author:    Li Gen
	  * @ Date:     2017/11/25
	  * @Returns:   int
	  * @Parameter: int id
	*/
	int getNextID(int id);

	/**
	  * @Method:    removeID
	  * @Note: 	ɾ����ģ�͵�id
	  * @Author:    Li Gen
	  * @ Date:     2017/11/25
	  * @Returns:   void
	  * @Parameter: int id
	*/
	void removeID(int id);

	void removeAll();
private:
	//id�����ֵ�ת��
	map<int, string> IdToName;
	//���ֵ�id��ת��
	map<string, int> NameToId;

	void addIDandName(int id,string name);

	void deleteIDandName(int id);

	void deleteIDandName(string name);

	bool isNameExist(string name);

	bool isIDExist(int id);
};

