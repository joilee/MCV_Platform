#ifndef DCLIST_H
#define DCLIST_H
#include "util/vector.h"
struct node
{
	Vector3d v_node;
	int id;
	node* pre;
	node* next;
};

class DCList
{
public:
	DCList();
	~DCList();

	void CreateDCList(Vector3d p, int id);
	void AddBack(Vector3d p, int id);
	node* GetNodeAt(int pos);
	void Delete(int pos);
	int GetLength();
	int GetCurrPos(node* curr);
	bool IsInterior(node* curr, node* judge);
	bool IsConvex(node* curr);

private:
	node* head;
	int size;
};

#endif