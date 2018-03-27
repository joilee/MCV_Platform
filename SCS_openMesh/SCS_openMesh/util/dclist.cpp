#include "dclist.h"
#include <iostream>

using namespace std;

DCList::DCList()
{
	head = NULL;
	size = 0;
}

DCList::~DCList()
{

}

void DCList::CreateDCList(Vector3d p, int id)
{
	head = new node[1];

	head->v_node = p;
	head->pre = head;
	head->next = head; 
	head->id = id;

	size = 1;
}

void DCList::AddBack(Vector3d p, int id)
{
	if (size == 0)
		CreateDCList(p, id);
	else
	{
		node* in = new node[1];
		in->v_node = p;
		in->id = id;

		node* tmp = head->pre;
		head->pre = in;
		in->next = head;

		tmp->next = in;
		in->pre = tmp;
		size++;
	}
}


int DCList::GetLength()
{
	return size;
}

node* DCList::GetNodeAt(int pos)
{
	node* tmp = head;
	for (int i = 0; i < pos; i++)
	{
		tmp = tmp->next;
	}
	return tmp;
}

void DCList::Delete(int pos)
{
	node* tmp = GetNodeAt(pos);
	if (pos == 0)
	{
		node* tail = head->pre;
		head = head->next;
		tail->next = head;
		head->pre = tail;
	}

	else if (pos == size - 1)
	{
		head->pre = tmp->pre;
		tmp->pre->next = head;
	}
	else
	{
		tmp->pre->next = tmp->next;
		tmp->next->pre = tmp->pre;
	}
	tmp->pre = NULL;
	tmp->next = NULL;
	size--;
}

int DCList::GetCurrPos(node* curr)
{
	node* tmp = head;
	int i = 0;
	while (tmp != curr)
	{
		tmp = tmp->next;
		i++;
	}
	return i;
}

bool DCList::IsInterior(node* curr, node* judge)
{
	node* p = curr->pre;
	node* r = curr->next;

	Vector3d pq = getVector(curr,p);  //q—>p
	Vector3d qr = getVector(r, curr);
	Vector3d rp = getVector(p,r);
	Vector3d sp = getVector(p,judge);
	Vector3d sq = getVector(curr,judge);
	Vector3d sr = getVector(r,judge);

	if (Dot(VectorCross(sp, pq), VectorCross(rp, pq)) > 0
		&& Dot(VectorCross(sq, qr), VectorCross(pq, qr)) > 0
		&& Dot(VectorCross(sr, rp), VectorCross(qr, rp)) > 0)
		return true;
	else
		return false;
}

bool DCList::IsConvex(node* curr)
{
	node* p = curr->pre;
	node* r = curr->next;

	Vector3d pq = getVector(curr,p);
	Vector3d qr = getVector(r,curr);

	Vector3d n(0.0, 0.0, 1.0);

	if (Dot(VectorCross(pq, qr), n) >= 0)
		return true;
	else
		return false;
}

bool DCList::IsClockwise()
{
	//node* curr = head;

	////正负值数量
	//int pCount = 0;
	//int nCount = 0;

	//for (int i = 0; i < size;i++)
	//{
	//	node* p = curr->pre;
	//	node* r = curr->next;

	//	Vector3d pq = p->v_node - curr->v_node;
	//	Vector3d qr = curr->v_node - r->v_node;

	//	if (VectorCross(pq, qr).z > 0)
	//		pCount++;
	//	if (VectorCross(pq, qr).z < 0)
	//		nCount++;

	//	curr = curr->next;
	//}

	////正值较多为逆时针，负值较多为顺时针
	//if (pCount > nCount)
	//	return false;
	//else
	//	return true;

	node* curr = head;
	double xMax = head->v_node.x;
	int pos = 0;

	for (int i = 0; i < size; i++)
	{
		if (curr->v_node.x>xMax)
		{
			xMax = curr->v_node.x;
			pos = i;
		}
		curr = curr->next;
	}

	curr = GetNodeAt(pos);
	node* p = curr->pre;
	node* r = curr->next;
	Vector3d pq = getVector(curr,p);
	Vector3d qr = getVector(r,curr);
	if (VectorCross(pq, qr).z > 0)
		return false;
	else
		return true;
}

void DCList::ConvertList()
{
	

}

Vector3d DCList::getVector(node* p, node* q)
{
	Vector3d pq = q->v_node - p->v_node;
	pq.z = 0;
	return pq;
}
