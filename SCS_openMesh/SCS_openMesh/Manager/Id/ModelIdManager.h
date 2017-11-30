#pragma once
#include <string>
#include <map>
#define DEFAULT_MODEL_ID 0
using namespace std;

/************************************************************************/
/* id管理者类
1.对每一个模型分配独一无二的ID
*/
/************************************************************************/
class ModelIdManager
{
public:
	ModelIdManager();
	~ModelIdManager();

	/**
	  * @Method:    getModelID
	  * @Note: 	给模型分配ID，如果没有参数，则根据默认ID分配。如果id在当前已经容器中存在，依次增加，直至下一个
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
	  * @Note: 	对于发生冲突（重复）的id，寻找下一个可用id
	  * @Author:    Li Gen
	  * @ Date:     2017/11/25
	  * @Returns:   int
	  * @Parameter: int id
	*/
	int getNextID(int id);

	/**
	  * @Method:    removeID
	  * @Note: 	删除该模型的id
	  * @Author:    Li Gen
	  * @ Date:     2017/11/25
	  * @Returns:   void
	  * @Parameter: int id
	*/
	void removeID(int id);

	void removeAll();
private:
	//id到名字的转换
	map<int, string> IdToName;
	//名字到id的转换
	map<string, int> NameToId;

	void addIDandName(int id,string name);

	void deleteIDandName(int id);

	void deleteIDandName(string name);

	bool isNameExist(string name);

	bool isIDExist(int id);
};

