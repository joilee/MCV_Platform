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

	Vector3d pq = p->v_node-curr->v_node;
	Vector3d qr = curr->v_node - r->v_node;
	Vector3d rp = r->v_node - p->v_node;
	Vector3d sp = judge->v_node - p->v_node;
	Vector3d sq = judge->v_node - curr->v_node;
	Vector3d sr = judge->v_node - r->v_node;

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

	Vector3d pq = p->v_node - curr->v_node;
	Vector3d qr = curr->v_node - r->v_node;

	Vector3d n(0.0, 0.0, 1.0);

	if (Dot(VectorCross(qr, pq), n) <= 0)
		return true;
	else
		return false;
}