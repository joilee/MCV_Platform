#include "geometry/emxKdTreeD.h"
#include "geometry/emxVertex.h"
#include "geometry/emxFace.h"
#include "geometry/emxModel.h"
#include <functional>

//类内静态成员变量初始化    类型 类名：：变量名 = 初始值
int emxKdTree::nodesTraversed  = 0;
int emxKdTree::nRayTest        = 0;
int emxKdTree::nRayHit         = 0;

int emxKdTreeNode::numLeafMade  = 0;
int emxKdTreeNode::numNodesMade = 0;
int emxKdTreeNode::maxLeafPrims = 0;
int emxKdTreeNode::leafPrims    = 0;

// generate reflected beam
void emxBeam::GeneratedReflectedBeam()
{
	if(hitFace != NULL)
	{
		reflectedBeam = new emxBeam();
		//reflectedBeam->bounce    = bounce + 1;
		reflectedBeam->cornerNum = cornerNum;
		Vector3d hitnormal = hitFace->GetNormal();
		reflectedBeam->normal = hitnormal;	
		reflectedBeam->parentBeam = this;
		reflectedBeam->beam_type = 0;
		reflectedBeam->beam_depth = 1;
		for(int i = 0; i < cornerNum; ++i)
		{
			reflectedBeam->BeamVertex[i] = BeamVertex[i] + dir[i] * maxDist[i];
			//reflectedBeam->dir[i]    = (hitnormal * (2 * Dot(hitnormal, -dir[i])) + dir[i]).normalize();
		}
		reflectedBeam->origin = origin - hitnormal * (2 * Dot(hitnormal, origin - reflectedBeam->BeamVertex[0]));
	    swap(reflectedBeam->BeamVertex[0], reflectedBeam->BeamVertex[2]);
		for(int i = 0; i < cornerNum; ++i)
		{
			reflectedBeam->dir[i]  = (reflectedBeam->BeamVertex[i] - reflectedBeam->origin).normalize();
		}
	}
}
// generate transmitted beam
void emxBeam::GeneratedTransmittedBeam()
{
	if(hitFace != NULL)
	{
		transmittedBeam = new emxBeam();
		//transmittedBeam->bounce    = bounce + 1;
		transmittedBeam->cornerNum = cornerNum;
		Vector3d hitnormal = hitFace->GetNormal();
		transmittedBeam->normal = -hitnormal;	
		transmittedBeam->parentBeam = this;
		transmittedBeam->beam_type = 1;
		transmittedBeam->beam_depth = 1;
		for(int i = 0; i < cornerNum; ++i)
		{
			transmittedBeam->BeamVertex[i] = BeamVertex[i] + dir[i] * maxDist[i];
			transmittedBeam->dir[i]    = dir[i];
		}
		transmittedBeam->origin = origin;
	}

}
// beam area
double emxBeam::BeamArea()
{
	double area = 0.0;
	for(int i = 2; i < cornerNum; ++i) 
	{
		Vector3d A = BeamVertex[i - 1] - BeamVertex[0];
		Vector3d B = BeamVertex[i]     - BeamVertex[0];
		area += VectorCross(A, B).norm() * 0.5;
	}
	return area;
}

// debug
bool emxBeam::belongBeam(int beamID)
{
	const emxBeam* pBeam = this;
	while(pBeam != NULL)
	{
		if(pBeam->index == beamID)
			return true;
		pBeam = pBeam->parentBeam;
	}
	return false;
}

//ray intersect with the triangle(v0,v1,v2)
bool RayTriangleintersect(emxRay ray,Vector3d v0, Vector3d v1,Vector3d v2)
{
	Vector3d E1 = v1 - v0;
	Vector3d E2 = v2 - v0;
	Vector3d S  = ray.origin - v0;
	Vector3d S1 = VectorCross(ray.direction, E2);
	Vector3d S2 = VectorCross(S, E1);

	double denometor = Dot(S1, E1);
	if(fabs(denometor) > DOUBLE_EPSILON) 
	{
		denometor = 1.0 / denometor;
		double t = Dot(S2, E2) * denometor;
		//if(t <= 0.0 || t >= ray.maxt || t <= ray.mint)		// avoid intersect with the edge of two triangles
		//	return false;
		if(t < DOUBLE_EPSILON ||  t > ray.maxt - DOUBLE_EPSILON || t < ray.mint + DOUBLE_EPSILON)		// avoid intersect with the edge of two triangles
			return false;
		double v = Dot(S1, S) * denometor;
		double w = Dot(S2, ray.direction) * denometor;
		double u = 1 - v - w;
		if( (u>DOUBLE_EPSILON && u<1-DOUBLE_EPSILON) && (v>DOUBLE_EPSILON && v<1-DOUBLE_EPSILON) && (w>DOUBLE_EPSILON && u<1-DOUBLE_EPSILON)) 
		{
			//tHit = t;
			// Coordinate
			return true;
		}
	}
	return false;
}

// sort the triangles in leaf node according to surface area
void emxKdTreeNode::sortPrimitives()
{
	int np = nPrimitives();
	assert(np > 1);
	multimap<double, emxMailboxPrim *, greater<double> > areaMap;
	for(int i = 0; i < np; ++i) {
		emxMailboxPrim *mp = primitives[i];
		double area = mp->face->GetArea();
		areaMap.insert(make_pair(area, mp));
	}
	multimap<double, emxMailboxPrim *, greater<double> >::iterator iter = areaMap.begin();
	for(int i = 0; i < np; ++i, ++iter) {
		assert(iter != areaMap.end());
		primitives[i] = iter->second;
	}
}

// KdTreeAccel Method Definitions
emxKdTree::
emxKdTree(emxModel* mesh,
		  int icost, int tcost,
		  double ebonus, int maxp, int maxDepth)
		  : isectCost(icost), traversalCost(tcost),
		    maxPrims(maxp), emptyBonus(ebonus)
{
	emxKdTree::nodesTraversed  = 0;
	emxKdTree::nRayTest        = 0;
	emxKdTree::nRayHit         = 0;

	emxKdTreeNode::numLeafMade  = 0;
	emxKdTreeNode::numNodesMade = 0;
	emxKdTreeNode::maxLeafPrims = 0;
	emxKdTreeNode::leafPrims    = 0;

	curMailboxId = 0;
	// Initialize _prims_ with primitives for grid
	vector<emxFace*> prims;	
	nMailboxes = static_cast<unsigned int>(mesh->NumF());
	prims.reserve(nMailboxes);
	for (unsigned int i = 0; i < nMailboxes; ++i) 
		prims.push_back(mesh->GetFacep(i));
	
	// Initialize mailboxes for _KdTreeAccel_
	mailboxPrims = (emxMailboxPrim *)AllocAligned(nMailboxes * sizeof(emxMailboxPrim));
	for (unsigned int i = 0; i < nMailboxes; ++i)
		new (&mailboxPrims[i]) emxMailboxPrim(i, prims[i]);
	
	// Build kd-tree for accelerator
	nextFreeNode = nAllocedNodes = 0;
	if (maxDepth <= 0)
		maxDepth = Round2Int(8 + 1.3 * Log2Int(float(prims.size())));

	// Compute bounds for kd-tree construction
	mesh->GetBBox(bounds.pMin, bounds.pMax);
	bounds.Expand(0.0001);	// avoid round error in maxt
	vector<emxBBox> primBounds;
	primBounds.reserve(prims.size());
	for (unsigned i = 0; i < prims.size(); ++i) {
		emxBBox pb(prims[i]->GetVertex(0)->GetPos(), prims[i]->GetVertex(1)->GetPos(), prims[i]->GetVertex(2)->GetPos());
		primBounds.push_back(pb);
	}

	// Allocate working memory for kd-tree construction
	emxBoundEdge *edges[3];
	for (int i = 0; i < 3; ++i)
		edges[i] = new emxBoundEdge[2*prims.size()];
	int *prims0 = new int[prims.size()];
	int *prims1 = new int[(maxDepth+1) * prims.size()];
	
	// Initialize _primNums_ for kd-tree construction
	int *primNums = new int[prims.size()];
	for (unsigned int i = 0; i < prims.size(); ++i)
		primNums[i] = i;
				
	// Start recursive construction of kd-tree
	buildTree(0, bounds, primBounds, primNums,
			static_cast<int>(prims.size()), maxDepth, edges,
			prims0, prims1);

	cout<<"Num of Interior Nodes:\t"<<emxKdTreeNode::numNodesMade<<endl;
	cout<<"Num of Leaf Node:\t"<<emxKdTreeNode::numLeafMade<<endl;
	cout<<"The Total Leaf Prims:\t"<<emxKdTreeNode::leafPrims<<endl;
	cout<<"Max Leaf Prims:\t"<<emxKdTreeNode::maxLeafPrims<<endl;
				
	// Free working memory for kd-tree construction
	delete[] primNums;
	for (int i = 0; i < 3; ++i)
		delete[] edges[i];
	delete[] prims0;
	delete[] prims1;
}

emxKdTree::~emxKdTree() 
{
	for (unsigned int i = 0; i < nMailboxes; ++i)
		mailboxPrims[i].~emxMailboxPrim();
	FreeAligned(mailboxPrims);
	for (int i = 0; i < nextFreeNode; ++i)
		nodes[i].~emxKdTreeNode();
	FreeAligned(nodes);
}

void emxKdTree::buildTree(int nodeNum, const emxBBox &nodeBounds, 
							const vector<emxBBox> &allPrimBounds, int *primNums, 
							int nPrims, int depth, emxBoundEdge *edges[3], 
							int *prims0, int *prims1, int badRefines) 
{
	assert(nodeNum == nextFreeNode); // NOBOOK
	// Get next free node from _nodes_ array
	if (nextFreeNode == nAllocedNodes) {
		int nAlloc = Max(2 * nAllocedNodes, 512);
		emxKdTreeNode *n = (emxKdTreeNode *)AllocAligned(nAlloc * sizeof(emxKdTreeNode));
		if (nAllocedNodes > 0) {
			memcpy(n, nodes, nAllocedNodes * sizeof(emxKdTreeNode));
			FreeAligned(nodes);
		}
		nodes = n;
		nAllocedNodes = nAlloc;
	}
	++nextFreeNode;

	nodes[nodeNum].bounds = nodeBounds;

	// Initialize leaf node if termination criteria met
	if (nPrims <= maxPrims || depth == 0) {
		nodes[nodeNum].initLeaf(primNums, nPrims, mailboxPrims);
		return;
	}
					
	// Initialize interior node and continue recursion
	// Choose split axis position for interior node
	int bestAxis = -1, bestOffset = -1;
	double bestCost = INFINITY;
	double oldCost = isectCost * double(nPrims);
	Vector3d d = nodeBounds.pMax - nodeBounds.pMin;
	double totalSA = (2.f * (d.x*d.y + d.x*d.z + d.y*d.z));
	double invTotalSA = 1.f / totalSA;

	// Choose which axis to split along
	int axis;
	if (d.x >= d.y && d.x >= d.z) axis = 0;
	else axis = (d.y >= d.z) ? 1 : 2;
	int retries = 0;
	
retrySplit:				
	// Initialize edges for _axis_
	for (int i = 0; i < nPrims; ++i)
	{
		int pn = primNums[i];
		const emxBBox &bbox = allPrimBounds[pn];
		edges[axis][2*i]    = emxBoundEdge(bbox.pMin[axis], pn, true);
		edges[axis][2*i+1]  = emxBoundEdge(bbox.pMax[axis], pn, false);		
	}
	sort(&edges[axis][0], &edges[axis][2*nPrims]);
					
	// Compute cost of all splits for _axis_ to find best
	int nBelow = 0, nAbove = nPrims;
	for (int i = 0; i < 2*nPrims; ++i) {
		if (edges[axis][i].type == emxBoundEdge::END) --nAbove;
		double edget = edges[axis][i].t;
		if (edget > nodeBounds.pMin[axis] + DOUBLE_EPSILON && edget < nodeBounds.pMax[axis] - DOUBLE_EPSILON) {
			// Compute cost for split at _i_th edge
			int otherAxis[3][2] = { {1,2}, {0,2}, {0,1} };
			int otherAxis0 = otherAxis[axis][0];
			int otherAxis1 = otherAxis[axis][1];
			double belowSA = 2 * (d[otherAxis0] * d[otherAxis1] +
				(edget - nodeBounds.pMin[axis]) *
				(d[otherAxis0] + d[otherAxis1]));
			double aboveSA = 2 * (d[otherAxis0] * d[otherAxis1] +
				(nodeBounds.pMax[axis] - edget) *
				(d[otherAxis0] + d[otherAxis1]));
			double pBelow = belowSA * invTotalSA;
			double pAbove = aboveSA * invTotalSA;
			double eb = (nAbove == 0 || nBelow == 0) ? emptyBonus : 0.0;
			double cost = traversalCost + isectCost * (1.f - eb) *
				(pBelow * nBelow + pAbove * nAbove);
			// Update best split if this is lowest cost so far
			if (cost < bestCost)  {
				bestCost = cost;
				bestAxis = axis;
				bestOffset = i;
			}
		}
		if (edges[axis][i].type == emxBoundEdge::START) ++nBelow;
	}
	assert(nBelow == nPrims && nAbove == 0); // NOBOOK

	// Create leaf if no good splits were found
	if (bestAxis == -1 && retries < 2) {
		++retries;
		axis = (axis+1) % 3;
		goto retrySplit;
	}
	if (bestCost > oldCost) ++badRefines;
	if ((bestCost > 4.f * oldCost && nPrims < 16) || bestAxis == -1 || badRefines == 3) {
		nodes[nodeNum].initLeaf(primNums, nPrims, mailboxPrims);
		return;
	}

	// Classify primitives with respect to split
	int n0 = 0, n1 = 0;
	for (int i = 0; i < bestOffset; ++i)
		if (edges[bestAxis][i].type == emxBoundEdge::START)
			prims0[n0++] = edges[bestAxis][i].primNum;
	for (int i = bestOffset+1; i < 2*nPrims; ++i)
		if (edges[bestAxis][i].type == emxBoundEdge::END)
			prims1[n1++] = edges[bestAxis][i].primNum;
						
	// Recursively initialize children nodes
	double tsplit = edges[bestAxis][bestOffset].t;
	nodes[nodeNum].initInterior(bestAxis, tsplit);
	emxBBox bounds0 = nodeBounds, bounds1 = nodeBounds;
	bounds0.pMax[bestAxis] = bounds1.pMin[bestAxis] = tsplit;
	buildTree(nodeNum+1, bounds0,
		allPrimBounds, prims0, n0, depth-1, edges,
		prims0, prims1 + nPrims, badRefines);
	nodes[nodeNum].aboveChild = nextFreeNode;
	buildTree(nodes[nodeNum].aboveChild, bounds1, allPrimBounds,
			prims1, n1, depth-1, edges,
			prims0, prims1 + nPrims, badRefines);
}

bool emxKdTree::Intersect(const emxRay &ray, emxIntersectionInfo &iInfo) const
{
	// Compute initial parametric range of ray inside kd-tree extent
	double tmin, tmax;
	if (!bounds.IntersectP(ray, &tmin, &tmax))
		return false;

	// Prepare to traverse kd-tree for ray
	double tHit;
	int rayId = curMailboxId++;
	tmin = Max(tmin, 0.0);
	ray.mint = tmin, ray.maxt = tmax;
	Vector3d invDir(1.f/ray.direction.x, 1.f/ray.direction.y, 1.f/ray.direction.z);
	#define MAX_TODO 64
	emxKdToDo todo[MAX_TODO];
	int todoPos = 0;
	// Traverse kd-tree from the root nodes in order for ray
	bool hit = false;
	const emxKdTreeNode *node = &nodes[0];
	while (node != NULL) {
		// Bail out if we found a hit closer than the current node
		if (ray.maxt < tmin) break;
		++nodesTraversed; //NOBOOK
		if (!node->IsLeaf()) {
			// Process kd-tree interior node
			// Compute parametric distance along ray to split plane
			int axis = node->SplitAxis();
			double tplane = (node->SplitPos() - ray.origin[axis]) * invDir[axis];
			// Get node children pointers for ray
			const emxKdTreeNode *firstChild = NULL, *secondChild = NULL;
			bool below = (ray.origin[axis] < node->SplitPos());
			bool equal = (ray.origin[axis] == node->SplitPos());
			if (below || (equal && ray.direction[axis] >= 0)) {
				firstChild = node + 1;
				secondChild = &nodes[node->aboveChild];
			}
			else {
				firstChild = &nodes[node->aboveChild];
				secondChild = node + 1;
			}
			// Advance to next child node, possibly enqueue other child
			if (tplane > tmax || tplane < 0)
				node = firstChild;
			else if (tplane < tmin)
				node = secondChild;
			else {
				// Enqueue _secondChild_ in todo list
				todo[todoPos].node = secondChild;
				todo[todoPos].tmin = tplane;
				todo[todoPos].tmax = tmax;
				++todoPos;
				assert(todoPos < MAX_TODO);
				node = firstChild;
				tmax = tplane;
			}
		}
		else {
			// Check for intersections inside leaf node
			unsigned int nPrimitives = node->nPrimitives();
			if (nPrimitives == 1) {
				emxMailboxPrim *mp = node->onePrimitive;
				// Check one primitive inside leaf node
				if (mp->faceID != ray.lastID && mp->lastMailboxId != rayId) {
					nRayTest++;
					mp->lastMailboxId = rayId;
					if (mp->face->Intersect(ray, tHit, iInfo)) {
						nRayHit++;
						ray.maxt          = tHit;
						iInfo.faceID      = mp->faceID;
						hit = true;
					}
				}
			}
			else {
				emxMailboxPrim **prims = node->primitives;
				for (unsigned int i = 0; i < nPrimitives; ++i) {
					emxMailboxPrim *mp = prims[i];
					if (mp->faceID != ray.lastID && mp->lastMailboxId != rayId) {
						nRayTest++;
						mp->lastMailboxId = rayId;
						// Check one primitive inside leaf node
						if (mp->face->Intersect(ray, tHit, iInfo)) {
							nRayHit++;						
							ray.maxt          = tHit;
							iInfo.faceID      = mp->faceID;
							hit = true;
						}
					}
				}
			}
			// Grab next node to process from todo list
			if (todoPos > 0) {
				--todoPos;
				node = todo[todoPos].node;
				tmin = todo[todoPos].tmin;
				tmax = todo[todoPos].tmax;
			}
			else
				break;
		}
	}
	return hit;
}

bool emxKdTree::Intersect(const emxRay &ray, double& tHit, int& faceID) const
{
	tHit = 0.0, faceID = -1;
	// Compute initial parametric range of ray inside kd-tree extent
	double tmin, tmax;
	if (!bounds.IntersectP(ray, &tmin, &tmax))
		return false;

	// Prepare to traverse kd-tree for ray
	int rayId = curMailboxId++;
	tmin = Max(tmin, 0.0);
	ray.mint = tmin, ray.maxt = tmax;
	Vector3d invDir(1.0/ray.direction.x, 1.0/ray.direction.y, 1.0/ray.direction.z);
	#define MAX_TODO 64
	emxKdToDo todo[MAX_TODO];
	int todoPos = 0;
	// Traverse kd-tree nodes in order for ray
	bool hit = false;
	const emxKdTreeNode *node = &nodes[0];
	while (node != NULL) {
		// Bail out if we found a hit closer than the current node
		if (ray.maxt < tmin) break;
		++nodesTraversed; //NOBOOK
		if (!node->IsLeaf()) {
			// Process kd-tree interior node
			// Compute parametric distance along ray to split plane
			int axis = node->SplitAxis();
			double tplane = (node->SplitPos() - ray.origin[axis]) * invDir[axis];
			// Get node children pointers for ray
			const emxKdTreeNode *firstChild = NULL, *secondChild = NULL;
			bool below = (ray.origin[axis] < node->SplitPos());
			bool equal = (ray.origin[axis] == node->SplitPos());
			if (below || (equal && ray.direction[axis] >= 0)) {
				firstChild = node + 1;
				secondChild = &nodes[node->aboveChild];
			}
			else {
				firstChild = &nodes[node->aboveChild];
				secondChild = node + 1;
			}
			// Advance to next child node, possibly enqueue other child
			if (tplane > tmax || tplane < 0)
				node = firstChild;
			else if (tplane < tmin)
				node = secondChild;
			else {
				// Enqueue _secondChild_ in todo list
				todo[todoPos].node = secondChild;
				todo[todoPos].tmin = tplane;
				todo[todoPos].tmax = tmax;
				++todoPos;
				assert(todoPos < MAX_TODO);
				node = firstChild;
				tmax = tplane;
			}
		}
		else {
			// Check for intersections inside leaf node
			unsigned int nPrimitives = node->nPrimitives();
			if (nPrimitives == 1) {
				emxMailboxPrim *mp = node->onePrimitive;
				// Check one primitive inside leaf node
				if (mp->faceID != ray.lastID && mp->lastMailboxId != rayId) {
					nRayTest++;
					mp->lastMailboxId = rayId;
					if (mp->face->Intersect(ray, tHit)) {
						nRayHit++;
						ray.maxt = tHit;
						faceID   = mp->faceID;
						hit = true;
					}
				}
			}
			else {
				emxMailboxPrim **prims = node->primitives;
				for (unsigned int i = 0; i < nPrimitives; ++i) {
					emxMailboxPrim *mp = prims[i];
					if (mp->faceID != ray.lastID && mp->lastMailboxId != rayId) {
						nRayTest++;
						mp->lastMailboxId = rayId;
						// Check one primitive inside leaf node
						if (mp->face->Intersect(ray, tHit)) {
							nRayHit++;						
							ray.maxt = tHit;
							faceID   = mp->faceID;
							hit = true;
						}
					}
				}
			}
			// Grab next node to process from todo list
			if (todoPos > 0) {
				--todoPos;
				node = todo[todoPos].node;
				tmin = todo[todoPos].tmin;
				tmax = todo[todoPos].tmax;
			}
			else
				break;
		}
	}
	return hit;
}

bool emxKdTree::IntersectP(const emxRay &ray) const 
{
	// Compute initial parametric range of ray inside kd-tree extent
	double tmin, tmax;
	if (!bounds.IntersectP(ray, &tmin, &tmax))
		return false;

	// Prepare to traverse kd-tree for ray
	int rayId = curMailboxId++;
	tmin = Max(tmin, 0.0);
	ray.mint = tmin, ray.maxt = tmax;
	Vector3d invDir(1.f/ray.direction.x, 1.f/ray.direction.y, 1.f/ray.direction.z);
	#define MAX_TODO 64
	emxKdToDo todo[MAX_TODO];
	int todoPos = 0;
	const emxKdTreeNode *node = &nodes[0];
	while (node != NULL) {
		++nodesTraversed;
		if (node->IsLeaf()) {
			// Check for shadow ray intersections inside leaf node
			unsigned int nPrimitives = node->nPrimitives();
			if (nPrimitives == 1) {
				emxMailboxPrim *mp = node->onePrimitive;
				if (mp->faceID != ray.lastID && mp->lastMailboxId != rayId) {
					nRayTest++;
					mp->lastMailboxId = rayId;
					if (mp->face->IntersectP(ray)) {
						nRayHit++;					
						return true;
					}
				}
			}
			else {
				emxMailboxPrim **prims = node->primitives;
				for (unsigned int i = 0; i < nPrimitives; ++i) {
					emxMailboxPrim *mp = prims[i];
					if (mp->faceID != ray.lastID && mp->lastMailboxId != rayId) {
						nRayTest++;
						mp->lastMailboxId = rayId;
						if (mp->face->IntersectP(ray)) {
							nRayHit++;						
							return true;
						}
					}
				}
			}

			// Grab next node to process from todo list
			if (todoPos > 0) {
				--todoPos;
				node = todo[todoPos].node;
				tmin = todo[todoPos].tmin;
				tmax = todo[todoPos].tmax;
			}
			else
				break;
		}
		else {
			// Process kd-tree interior node
			// Compute parametric distance along ray to split plane
			int axis = node->SplitAxis();
			double tplane = (node->SplitPos() - ray.origin[axis]) * invDir[axis];
			// Get node children pointers for ray
			const emxKdTreeNode *firstChild = NULL, *secondChild = NULL;
			bool below = (ray.origin[axis] < node->SplitPos());
			bool equal = ray.origin[axis] == node->SplitPos();
			if (below || (equal && ray.direction[axis] >= 0)) {
				firstChild = node + 1;
				secondChild = &nodes[node->aboveChild];
			}
			else {
				firstChild = &nodes[node->aboveChild];
				secondChild = node + 1;
			}
			// Advance to next child node, possibly enqueue other child
			if (tplane > tmax || tplane < 0) 
				node = firstChild;
			else if (tplane < tmin) 
				node = secondChild;
			else {
				// Enqueue _secondChild_ in todo list
				todo[todoPos].node = secondChild;
				todo[todoPos].tmin = tplane;
				todo[todoPos].tmax = tmax;
				++todoPos;
				assert(todoPos < MAX_TODO);
				node = firstChild;
				tmax = tplane;
			}
		}
	}

	return false;
}

/*
 *	fill the intersected face list of the intersected triangle with the prism
 */
bool emxKdTree::IntersectPrism(emxRay rays[3], vector<unsigned int>& intersectedFaceList) const
{
	intersectedFaceList.clear();
	vector<unsigned int> PVSFaceList;
	//emxModel::Instance()->GetPVF()->GetPVF(PVSFaceList, rays[0].lastID);
	//vector<unsigned int>& PVSFaceList = mailboxPrims[rays[0].lastID].face->GetVisibleFaceList();

	// Compute initial parametric range of rays inside kd-tree extent
	double tmin[3], tmax[3];
	bool intersected = false;
	for(size_t i = 0; i < 3; ++i) 
	{
		if(bounds.IntersectP(rays[i], &tmin[i], &tmax[i]))
		{
			tmin[i] = Max(tmin[i], 0.0);
			intersected = true;
		}
		else 
		{
			tmin[i] = 0.0, tmax[i] = 0.0;
		}
	}
	if(!intersected)
		return false;

	// Prepare to traverse kd-tree for rays
	Vector3d invDir(1.0 / rays[0].direction.x, 1.0 / rays[0].direction.y, 1.0 / rays[0].direction.z);
	#define MAX_TODO 64
	emxKdToDo todo[MAX_TODO][3];
	int todoPos = 0;

	// Traverse kd-tree nodes in order for ray
	const emxKdTreeNode *node = &nodes[0];
	while (node != NULL)
	{
		if (!node->IsLeaf())
		{
			// Process kd-tree interior node
			// Compute parametric distance along ray to split plane
			int axis = node->SplitAxis();
			double tplane[3] = {
				(node->SplitPos() - rays[0].origin[axis]) * invDir[axis],
				(node->SplitPos() - rays[1].origin[axis]) * invDir[axis],
				(node->SplitPos() - rays[2].origin[axis]) * invDir[axis] };

			// Get node children pointers for ray
			const emxKdTreeNode *firstChild = NULL, *secondChild = NULL;
			bool allBelow = (rays[0].origin[axis] < node->SplitPos()) && (rays[1].origin[axis] < node->SplitPos()) && (rays[2].origin[axis] < node->SplitPos());
			bool allAbove = (rays[0].origin[axis] > node->SplitPos()) && (rays[1].origin[axis] > node->SplitPos()) && (rays[2].origin[axis] > node->SplitPos());
			if(allBelow || (!allAbove && rays[0].direction[axis] >= 0))
			{
				firstChild = node + 1;
				secondChild = &nodes[node->aboveChild];
			} 
			else 
			{
				firstChild = &nodes[node->aboveChild];
				secondChild = node + 1;
			}

			// Advance to next child node, possibly enqueue other child
			bool allFirst = (tplane[0] > tmax[0] || tplane[0] < 0) && (tplane[1] > tmax[1] || tplane[1] < 0) && (tplane[2] > tmax[2] || tplane[2] < 0);
			bool allSecond = (tplane[0] < tmin[0]) && (tplane[1] < tmin[1]) && (tplane[2] < tmin[2]);
			if((allBelow || allAbove) && allFirst)
				node = firstChild;
			else if ((allBelow || allAbove) && allSecond)
				node = secondChild;
			else 
			{
				// Enqueue _secondChild_ in todo list
				todo[todoPos][0].node = secondChild;
				todo[todoPos][0].tmin = Max(tplane[0], 0.0);
				todo[todoPos][0].tmax = tmax[0];
				todo[todoPos][1].node = secondChild;
				todo[todoPos][1].tmin = Max(tplane[1], 0.0);
				todo[todoPos][1].tmax = tmax[1];
				todo[todoPos][2].node = secondChild;
				todo[todoPos][2].tmin = Max(tplane[2], 0.0);
				todo[todoPos][2].tmax = tmax[2];
				++todoPos;
				assert(todoPos < MAX_TODO);
				node = firstChild;
				tmax[0] = Max(tplane[0], 0.0);
				tmax[1] = Max(tplane[1], 0.0);
				tmax[2] = Max(tplane[2], 0.0);
			}
		}
		else
		{
			// Check for intersections inside leaf node
			unsigned int nPrimitives = node->nPrimitives();
			if (nPrimitives == 1)
			{
				emxMailboxPrim *mp = node->onePrimitive;
				// Check one primitive inside leaf node
				if (mp->faceID != rays[0].lastID) 
				{
					if(find(intersectedFaceList.begin(), intersectedFaceList.end(), mp->faceID) == intersectedFaceList.end()) 
					{
						int faceID = 0;
						for(size_t i = 0; i < PVSFaceList.size(); ++i)
						{
							faceID = PVSFaceList[i];
							if(faceID == mp->faceID) 
								intersectedFaceList.push_back(mp->faceID);
							if(faceID >= mp->faceID)
								break;
						}
					}
				}
			}
			else 
			{
				emxMailboxPrim **prims = node->primitives;
				for (unsigned int i = 0; i < nPrimitives; ++i) 
				{
					emxMailboxPrim *mp = prims[i];
					if (mp->faceID != rays[0].lastID) 
					{
						if(find(intersectedFaceList.begin(), intersectedFaceList.end(), mp->faceID) == intersectedFaceList.end())
						{
							int faceID = 0;
							for(size_t i = 0; i < PVSFaceList.size(); ++i)
							{
								faceID = PVSFaceList[i];
								if(faceID == mp->faceID) 
									intersectedFaceList.push_back(mp->faceID);
								if(faceID >= mp->faceID)
									break;
							}
						}
					}
				}
			}
			// Grab next node to process from todo list
			if (todoPos > 0) 
			{
				--todoPos;
				node    = todo[todoPos][0].node;
				tmin[0] = todo[todoPos][0].tmin;
				tmax[0] = todo[todoPos][0].tmax;
				tmin[1] = todo[todoPos][1].tmin;
				tmax[1] = todo[todoPos][1].tmax;
				tmin[2] = todo[todoPos][2].tmin;
				tmax[2] = todo[todoPos][2].tmax;
			}
			else
				break;
		}
	}
	return !intersectedFaceList.empty();
}

/*
 * check whether the node or its children nodes contains the face with index = faceIndex
 */
bool emxKdTree::containFace(const emxKdTreeNode* node, size_t faceIndex) const
{
	if(node->IsLeaf())
	{
		int np = node->nPrimitives();
		if(np == 1) return (node->onePrimitive->faceID == faceIndex);
		else 
		{
			for(int i = 0; i < np; ++i)
			{
				if(node->primitives[i]->faceID == faceIndex)
					return true;
			}
		}
	} 
	else 
	{
		return containFace(node + 1, faceIndex) || containFace(&nodes[node->aboveChild], faceIndex);
	}
	return false;
}

/*
 * check if curBeam hits the scene's bounding box, and initialize tmin and tmax, tmin、tmax 是三个轴向对应射线管beam角点数目的二维数组
 */
bool emxKdTree::BeamVsAABBox(const emxBeam *curBeam, const emxBBox& bounds, Vector3d tmin[4], Vector3d tmax[4]) const
{
	// if the ray miss the kd-node slabs, ray direction parallels axis
	tmin[0] = tmin[1] = tmin[2] = tmin[3] = Vector3d(0, 0, 0);
	tmax[0] = tmax[1] = tmax[2] = tmax[3] = Vector3d(INFINITY, INFINITY, INFINITY);
	int      cornerNum = curBeam->cornerNum;
	

	// check if curBeam hits the scene's bounding box
	// above the triangle of hit beam?
	// check if the bounding box is wholly outside of the side
	for(int i = 0; i < cornerNum; ++i) 
	{
		Vector3d beamVertex  = curBeam->BeamVertex[i];
		Vector3d edgeDir = curBeam->BeamVertex[(i + 1) % cornerNum] - beamVertex;
		Vector3d direction = curBeam->dir[i];
		Vector3d outDir  = VectorCross(edgeDir, direction).normalize();
		bool fullOut = true;
		for(int j = 0; j < 8; ++j)
		{
			fullOut &= Dot(bounds.CornerPoint(j) - beamVertex, outDir) >= -DOUBLE_EPSILON;
		}			
		if(fullOut)
			return false;
	}
	// check if the bounding box is wholly behind the image plane
	bool fullBehind = true;
	for(int i = 0; i < 8; ++i)
	{
		fullBehind &= Dot(bounds.CornerPoint(i) - curBeam->BeamVertex[0], curBeam->normal) <=  DOUBLE_EPSILON;
	}
	if(fullBehind)
		return false;
	// check if the beam is outside of the bounding box
	Vector3d boundsNormal[] = { Vector3d(1, 0, 0), Vector3d(-1, 0, 0), Vector3d(0, 1, 0), Vector3d(0, -1, 0), Vector3d(0, 0, 1), Vector3d(0, 0, -1) };
	Vector3d boundsPoint[]  = { bounds.pMax, bounds.pMin, bounds.pMax, bounds.pMin, bounds.pMax, bounds.pMin };
	// maximum distance of beam ray?
	double maxDistance = (this->bounds.pMax - this->bounds.pMin).norm() * 2.0; // beam maximum distance
	for(int i = 0; i < 6; ++i)
	{
		bool fullOut = true;
		for(int j = 0; j < cornerNum; ++j) 
		{
			Vector3d direction = curBeam->dir[j];
			fullOut &= Dot(curBeam->BeamVertex[j] - boundsPoint[i], boundsNormal[i]) >= -DOUBLE_EPSILON;
			fullOut &= Dot(curBeam->BeamVertex[j] + direction * maxDistance - boundsPoint[i], boundsNormal[i]) >= -DOUBLE_EPSILON;
		}
		if(fullOut) 
			return false;
	}

	// init tmin and tmax of each ray and axis
	for(int i = 0; i < cornerNum; ++i) 
	{
		Vector3d beamVertex = curBeam->BeamVertex[i];
		Vector3d direction = curBeam->dir[i];
		//Vector3d invDir = curBeam->invDir[i];

		for(int j = 0; j < 3; ++j) 
		{
			if(fabs(direction[j]) > DOUBLE_EPSILON) 
			{
				// update interval for _j_th (axis) bounding box slab
				/*double tNear = (bounds.pMin[j] - beamVertex[j]) * invDir[j];
				double tFar  = (bounds.pMax[j] - beamVertex[j]) * invDir[j];*/
				double tNear = (bounds.pMin[j] - beamVertex[j]) / direction[j];
				double tFar  = (bounds.pMax[j] - beamVertex[j]) / direction[j];
				// Update parametric interval from slab intersection $t$s
				if (tNear > tFar) swap(tNear, tFar);
				tmin[i][j] = Max(0.0, tNear);	// should above 0.0?
				tmax[i][j] = Max(0.0, tFar);
			}
		}
	}
	
	return true;
}

/*
 * intersect the beam with the spliting plane， tsplit是对应射线管角点数目的一维数组
 */
void emxKdTree::IntersectSplitPlane(const emxKdTreeNode* node, const emxBeam* curBeam, double tplane[4]) const
{
	int axis = node->SplitAxis();
	double splitPos = node->SplitPos();
	for(int i = 0; i < curBeam->cornerNum; ++i) 
	{
		if(fabs(curBeam->dir[i][axis]) > DOUBLE_EPSILON)
			/*tplane[i] = (splitPos - curBeam->BeamVertex[i][axis]) * curBeam->invDir[i][axis];*/
				tplane[i] = (splitPos - curBeam->BeamVertex[i][axis]) / curBeam->dir[i][axis];
		else
			tplane[i] = INFINITY;
	}
}

/*
 * classify the near and far children according the direction of the ray，   beam kd树遍历时远近子节点的判定
 */
void emxKdTree::GetNearFarChild(const emxKdTreeNode* node, emxBeam* curBeam, 
								const emxKdTreeNode* &nearNode, const emxKdTreeNode* &farNode) const
{
	int axis = node->SplitAxis();
	double splitPos = node->SplitPos();
	bool allBelow = true, allAbove = true, allForward = true;
	for(int i = 0; i < curBeam->cornerNum; ++i) 
	{
		allBelow &= (curBeam->BeamVertex[i][axis] < splitPos - DOUBLE_EPSILON);
		allAbove &= (curBeam->BeamVertex[i][axis] > splitPos + DOUBLE_EPSILON);
		allForward &= (curBeam->dir[i][axis] >= 0.0);  //***************
	}
	if(allBelow) 
	{
		nearNode = node + 1;
		farNode = &nodes[node->aboveChild];
	}
	else if(allAbove) 
	{
		nearNode = &nodes[node->aboveChild];
		farNode = node + 1;
	}
	else if(allForward) 
	{
		nearNode = node + 1;
		farNode = &nodes[node->aboveChild];
	} 
	else 
	{		
		nearNode = &nodes[node->aboveChild];
		farNode = node + 1;
	}
}

/*
 * get next beam and KDNode from the stack
 */
void emxKdTree::GetNextBeam(vector<emxKdStackNode>& KDStack, vector<emxBeam*>& MissStack, Vector3d tmin[4], 
							Vector3d tmax[4], emxBeam*& nextBeam, const emxKdTreeNode*& nextKDNode) const
{
	nextBeam = NULL;
	nextKDNode = NULL;

	while(!KDStack.empty()) 
	{
		emxKdStackNode& KdStackNode = KDStack.back();
		if(KdStackNode.nBeamSize == -1) 
		{// mark the number of beam need to traverse this KDNode
			if(MissStack.size() < KdStackNode.nMissStackSize)
				cout<<"Error in MissStack.size() < KdStackNode.nMissStackSize"<<endl;
			KdStackNode.nBeamSize = static_cast<int>(MissStack.size() - KdStackNode.nMissStackSize);
		}
		while(KdStackNode.nBeamSize > 0) 
		{
			emxBeam* beam = MissStack[KdStackNode.nMissStackSize];
			vector<emxBeam*>::iterator pos = find(MissStack.begin(), MissStack.end(), beam);
			assert(pos != MissStack.end());
			MissStack.erase(pos);
			KdStackNode.nBeamSize--;
			if(BeamVsAABBox(beam, KdStackNode.node->bounds, tmin, tmax))
			{
				if(beam->IsHitBeam()) 
				{	
					// If the entry distance to current kd-cell's slabs in any one dimension(tmin) are larger than
					// the distances to the hits triangle's plane, then the cell is further
					bool fullFurther = true;
					bool fullInNode  = true;
					
					for(int i = 0; i < beam->cornerNum; ++i) 
					{
						emxRay ray;
						ray.direction = beam->dir[i];
						fullFurther &= (tmin[i].x > beam->maxDist[i]) || (fabs(beam->dir[i].x) < DOUBLE_EPSILON);	// slab ?
						fullFurther &= (tmin[i].y > beam->maxDist[i]) || (fabs(beam->dir[i].y) < DOUBLE_EPSILON);
						fullFurther &= (tmin[i].z > beam->maxDist[i]) || (fabs(beam->dir[i].z) < DOUBLE_EPSILON);
						ray.origin   = beam->BeamVertex[i];
						fullInNode  &= KdStackNode.node->bounds.IntersectP(ray);
					}
					if(fullFurther) 
					{	// the hit beam
						if(!fullInNode)
							MissStack.push_back(beam);
						continue;	// the kd-node is wholly further than the hit beam
					}
				}
			
				nextBeam = beam;
				nextKDNode = KdStackNode.node;
				break;
			} 
			else 
			{	// beam doesn't hit this KDNode
				MissStack.push_back(beam);
			}
		}
		if(KdStackNode.nBeamSize == 0)
			KDStack.pop_back();
		if(nextBeam != NULL)
			break;
	}
}

//check the beam and triangle whether exist intersection area
bool emxKdTree::is_beamTriangle_intersection(emxBeam *curBeam, emxMailboxPrim *mp, bool abovezero[3]) const
{
	Vector3d beamVertex[4];
	int beamVertexNum = curBeam->cornerNum;
	for(int i = 0; i < beamVertexNum; ++i)
		beamVertex[i] = curBeam->BeamVertex[i];
	Vector3d normal    = curBeam->normal;
	Vector3d triangleVertex[3] = { mp->face->GetVertex(0)->GetPos(), mp->face->GetVertex(1)->GetPos(), mp->face->GetVertex(2)->GetPos() };
	Vector3d triangleNormal = mp->face->GetNormal();

	//判断目标三角形是否与beam存在相交，即beam的源点必须在三角面片之外
	if (Dot(curBeam->origin - triangleVertex[0], triangleNormal) <= DOUBLE_EPSILON)
	{
		//debug用
//		double a = Dot(curBeam->origin - triangleVertex[0], triangleNormal);

		return false;
	}
	bool  fullOut = false;
	//判断三角面片是否在beam覆盖区域之外
	for(int i = 0; i < curBeam->cornerNum; ++i) 
	{
		Vector3d edgeDir = curBeam->BeamVertex[(i + 1) %curBeam->cornerNum] - curBeam->BeamVertex[i];
		Vector3d direction = curBeam->dir[i];
		Vector3d outDir  = VectorCross(edgeDir, direction).normalize();
		bool fullOut = true;
		for(int j = 0; j < 3; ++j)
		{
			//debug用
		//	double a = Dot(triangleVertex[j] - beamVertex, outDir);

			fullOut &= Dot(triangleVertex[j] - curBeam->BeamVertex[i], outDir) >= -DOUBLE_EPSILON;
			if (!fullOut)
			{
				break;
			}
		}			
		if(fullOut)
			return false;
	}
	//判断三角面片是否在beam所在平面之前
	bool fullBehind = true;
	for(int i = 0; i < 3; ++i)
	{
		//debug用
	//	double a = Dot(triangleVertex[i] - curBeam->BeamVertex[0], curBeam->normal) ;

		abovezero[i] = (Dot(triangleVertex[i] - curBeam->BeamVertex[0], curBeam->normal) >=  DOUBLE_EPSILON);  //在beam所在平面之外
		fullBehind &= !abovezero[i] ;
	}
	if(fullBehind)
		return false;

	return true;
}
//the beam kd-tree traversal
bool emxKdTree::IntersectBeam(emxBeam *curBeam) const
{
	const int mode[] = { 1, 2, 0, 1 };
	Vector3d tmin[4], tmax[4];	// entry and exit distances for each beam ray and axis
	// Check if the curBeam hits the scene's bounding box and initialize tmin and tmax
	if(!BeamVsAABBox(curBeam, bounds, tmin, tmax))
		return false;

	// Prepare to traverse kd-tree for rays ? beams
	vector<emxKdStackNode> KDStack; //节点堆栈，维护待遍历节点以及需要访问该节点的射线管
	vector<emxBeam*> MissStack;  //射线管堆栈，维护新生成的待遍历射线管

	// Traverse kd-tree nodes in order for beam
	const emxKdTreeNode *node = &nodes[0];
	while(curBeam != NULL)
	{
		// Process kd-tree interior node
		while(!node->IsLeaf())
		{
			//cout<<curBeam->index<<" "<<node->index<<endl;
			// Compute parametric distance along ray to split plane
			double tplane[4];
			int axis = node->SplitAxis();
			IntersectSplitPlane(node, curBeam, tplane);

			// Get node children pointers for beam
			const emxKdTreeNode *nearNode = NULL, *farNode = NULL;
			GetNearFarChild(node, curBeam, nearNode, farNode);

			// Check which node to traverse
			bool validDir = true;
			for (int j = 0; j < curBeam->cornerNum; ++j)
			{
				validDir &= !(fabs(curBeam->dir[j][axis]) < DOUBLE_EPSILON);
			}
			bool behindOrigin = validDir, furtherAxis1 = validDir, furtherAxis2 = validDir, nearAxis1 = validDir, nearAxis2 = validDir;
			for(int i = 0; i < curBeam->cornerNum; ++i) 
			{	// strict compare
				behindOrigin &= tplane[i] < -DOUBLE_EPSILON;
				furtherAxis1 &= tplane[i] < tmin[i][mode[axis]] + DOUBLE_EPSILON;
				furtherAxis2 &= tplane[i] < tmin[i][mode[axis + 1]] + DOUBLE_EPSILON;
				nearAxis1    &= tplane[i] > tmax[i][mode[axis]] - DOUBLE_EPSILON;
				nearAxis2    &= tplane[i] > tmax[i][mode[axis + 1]] - DOUBLE_EPSILON;
			}

			if(behindOrigin)	// Split plane passes behind image plane
				node = nearNode;
			else if(furtherAxis1 || furtherAxis2)	// Traverse far node only
				node = farNode;
			else if(nearAxis1 || nearAxis2)			// Traverse near node only
				node = nearNode;
			else 
			{		// Traverse both nodes
				// Save the far kd-node for later
				emxKdStackNode stackNode;
				stackNode.node = farNode;
				stackNode.nMissStackSize = MissStack.size();
				stackNode.nBeamSize = -1;	// unknown now
				KDStack.push_back(stackNode);
				// Traverse the near node
				for(int i = 0; i < curBeam->cornerNum; ++i) 
					tmax[i][axis] = tplane[i];
				node = nearNode;
			}
		}

		// Intersect with the triangles in this leaf node
		size_t nPrimitives = node->nPrimitives();
		if(nPrimitives > 0) 
		{
			vector<emxBeam*> splitBeams;
			splitBeams.push_back(curBeam);
			emxMailboxPrim **prims = node->primitives;
			for (size_t i = 0; i < nPrimitives; ++i) 
			{
				emxMailboxPrim *mp = nPrimitives == 1 ? node->onePrimitive : prims[i];
				bool abovezero[3] = {false,false,false};  //三角面片的三个顶点与beam所在平面的关系：之前，之后
				if (!is_beamTriangle_intersection(curBeam,mp,abovezero))
				{
					continue;
				}

				vector<emxBeam*> hitMissBeams;
				for(size_t j = 0; j < splitBeams.size(); ++j) 
				{
					if ( /*1*/splitBeams[j]->beam_depth <= 10)
					{
						BeamTriangleIntersection(splitBeams[j], mp, abovezero);
						if(splitBeams[j]->childBeams.empty())
							hitMissBeams.push_back(splitBeams[j]);
						else 
							hitMissBeams.insert(hitMissBeams.end(), splitBeams[j]->childBeams.begin(), splitBeams[j]->childBeams.end());
					}		
				}
				splitBeams = hitMissBeams;
			}
			MissStack.insert(MissStack.end(), splitBeams.begin(), splitBeams.end());
		}
		else 
		{
			MissStack.push_back(curBeam);
		}

		// Get the next beam and KDNode from the stack 从堆栈中获得下一个待处理的射线管和节点对
		GetNextBeam(KDStack, MissStack, tmin, tmax, curBeam, node);
	}
	return true;
}

/*
 * intersect current beam with triangle
 */
//void emxKdTree::BeamTriangleIntersection(emxBeam *curBeam, emxMailboxPrim *mp) const
//{
//	Vector3d beamVertex[4];
//	int beamVertexNum = curBeam->cornerNum;
//	for(int i = 0; i < beamVertexNum; ++i)
//		beamVertex[i] = curBeam->BeamVertex[i];
//	Vector3d normal    = curBeam->normal;
//	Vector3d triangleVertex[3] = { mp->face->GetVertex(0)->GetPos(), mp->face->GetVertex(1)->GetPos(), mp->face->GetVertex(2)->GetPos() };
//	Vector3d triangleNormal = mp->face->GetNormal();
//
//	bool  fullOut = false;
//	//判断三角面片是否在beam覆盖区域之外
//	for(int i = 0; i < curBeam->cornerNum; ++i) 
//	{
//		Vector3d beamVertex  = curBeam->BeamVertex[i];
//		Vector3d edgeDir = curBeam->BeamVertex[(i + 1) %curBeam->cornerNum] - beamVertex;
//		Vector3d direction = curBeam->dir[i];
//		Vector3d outDir  = VectorCross(edgeDir, direction).normalize();
//		bool fullOut = true;
//		for(int j = 0; j < 3; ++j)
//		{
//			fullOut &= Dot(triangleVertex[j] - beamVertex, outDir) >= -DOUBLE_EPSILON;
//		}			
//		if(fullOut)
//			return;
//	}
//	//判断三角面片是否在beam所在平面之前
//	bool fullBehind = true;
//	for(int i = 0; i < 3; ++i)
//	{
//		fullBehind &= Dot(triangleVertex[i] - curBeam->BeamVertex[0], curBeam->normal) <=  DOUBLE_EPSILON;
//	}
//	if(fullBehind)
//		return;
//
//	//判断目标三角形朝向是否背向beam射出去的方向，当beam 至少有一条棱边与目标三角形面片相交时，则判断为存在相交
//	int true_Num = 0;
//	Vector3d approach_Vertex;
//	for (int i = 0; i < beamVertexNum; i++)
//	{
//		if (Dot(curBeam->dir[i], triangleNormal) < -DOUBLE_EPSILON)  //相交，其他的平行或者背离目标三角形面片
//		{
//			true_Num++;
//			approach_Vertex += curBeam->BeamVertex[i];
//		}
//	}
//	if(true_Num == 0)  //beam没有棱边与目标面片相交
//		return;
//	approach_Vertex /= true_Num;
//
//	//判断目标三角形可相交区域是否在beam之前
//	double tmax[4];	// distance from beam origin to triangle
//	bool behind1 = true;
//	Vector3d trianlgeOrigin = mp->face->GetVertex(0)->GetPos();
//	for(int i = 0; i < beamVertexNum; ++i)
//	{
//		Vector3d direction = curBeam->dir[i];  
//		if (Dot(direction,triangleNormal) >= -DOUBLE_EPSILON )  //射线方向与面片方向平行或者背离时
//		{
//			//tmax[i] = INFINITY;	
//			Vector3d Vertex = curBeam->BeamVertex[i];
//			int j = 1;
//			while (Dot(direction,triangleNormal) >= -DOUBLE_EPSILON  && j<=10000)
//			{
//				Vertex = 0.0001 * j * approach_Vertex + (1- 0.0001 * j) * Vertex;
//				direction = (Vertex - curBeam->origin).normalize();
//
//				j++;
//			}
//			double invDDotN = 1.0 / Dot(direction, triangleNormal);	
//			tmax[i] = -Dot(Vertex - trianlgeOrigin, triangleNormal) * invDDotN;
//
//			behind1 &= (tmax[i] < DOUBLE_EPSILON); 
//		}
//		else
//		{
//			double invDDotN = 1.0 / Dot(direction, triangleNormal);	
//			tmax[i] = -Dot(beamVertex[i] - trianlgeOrigin, triangleNormal) * invDDotN;
//
//			behind1 &= (tmax[i] < DOUBLE_EPSILON);  	// 0.0 or DOUBLE_EPSILON?
//		}			
//	}	
//	if(behind1)
//	{
//		return;
//		//if (true_Num == beamVertexNum)  	// triangle fully behind the beam's plane)
//		//	return;
//	}
//
//	// 将目标三角形投影到射线管beam起始面所在平面上
//	double dist[3];
//	Vector3d triangle_direction[3];
//	bool behind2 = true;
//	//找出所有能够和beam有效相交的三角形顶点
//	Vector3d approach_TriangleVertex;
//	int approach_TriangleVertexNum = 0;
//	for (int l =0;l<3;l++)
//	{
//		triangle_direction[l] = (triangleVertex[l] - curBeam->origin).normalize();
//		if (Dot(triangle_direction[l],normal) >= DOUBLE_EPSILON)
//		{
//			approach_TriangleVertex += triangleVertex[l];
//			approach_TriangleVertexNum ++;
//		}
//	}
//
//	if (approach_TriangleVertexNum > 0)
//	{
//		approach_TriangleVertex /= approach_TriangleVertexNum;
//	}
//	else
//	{
//		return;
//	}
//	for(int i = 0; i < 3; ++i)
//	{
//		//dist[i] = Dot(triangleVertex[i] - beamVertex[0], normal) / Dot(triangle_direction[i], normal);
//		double invDDotN;
//		if (Dot(triangle_direction[i],normal) <  DOUBLE_EPSILON) 
//		{
//			//cout<<"BeamTriangleIntersection abnormal triangle"<<endl;
//			int j = 1;
//			while (Dot(triangle_direction[i],normal)< DOUBLE_EPSILON && j<=10000)
//			{				
//				triangleVertex[i] = 0.0001* j * approach_TriangleVertex + (1 - 0.0001*j)*triangleVertex[i];
//			//	triangleVertex[i] = 0.001* j * triangleVertex[approach_TrianglevertexID] + (1 - 0.001*j)*triangleVertex[i];
//				triangle_direction[i] = (triangleVertex[i] - curBeam->origin).normalize();
//				
//				j++;
//			} 
//		}
//		invDDotN = 1.0 / Dot(triangle_direction[i],normal);
//		dist[i] = Dot(triangleVertex[i] - beamVertex[0], normal) * invDDotN;
//		behind2 &= (dist[i] < DOUBLE_EPSILON);
//	}	
//	if (behind2)
//		return;
//
//	// clip triangles which one or two vertex in front of the beam plane
//	Vector3d projVertex_p[4];
//	int projVertexNum_p = 0;
//	//先求出triangle各点在beam初始面上的投影点，并裁剪掉在beam plane之前的部分
//	for(int i = 0; i < 3; ++i) 
//	{
//		if(dist[i] >= -DOUBLE_EPSILON) 
//			projVertex_p[projVertexNum_p++] = triangleVertex[i] - triangle_direction[i] * dist[i];
//		//穿透时的交点
//		if(fabs(dist[i]) > DOUBLE_EPSILON && fabs(dist[(i + 1) % 3]) > DOUBLE_EPSILON && dist[i] * dist[(i + 1) % 3] < 0.0) 
//		{
//			Vector3d A = triangleVertex[i];
//			Vector3d B = triangleVertex[i] - triangle_direction[i] * dist[i];
//			Vector3d C = triangleVertex[(i + 1) % 3];
//			Vector3d D = triangleVertex[(i + 1) % 3] - triangle_direction[(i + 1) % 3] * dist[(i + 1) % 3];
//			double Area_ABC = 0.5 * (VectorCross(B-A,C-A).norm());
//			double Area_ACD = 0.5 * (VectorCross(C-A,D-A).norm());
//			double split_ratio = Area_ABC/Area_ACD;
//			projVertex_p[projVertexNum_p++] = ( D * split_ratio + B * 1) / (1 + split_ratio); //???????
//		}
//	}
//	// reverse order
//	swap(projVertex_p[0], projVertex_p[2]);
//	assert(projVertexNum_p <= 4);
//
//	//剔除投影点邻近的情况
//	Vector3d projVertex[4];
//	int projVertexNum = 0;
//	for (int i = 0; i < projVertexNum_p; i++)
//	{
//		if ((projVertex_p[i] - projVertex_p[(i+1)%projVertexNum_p]).norm() > DOUBLE_EPSILON)
//		{
//			projVertex[projVertexNum++] = projVertex_p[i];
//		}
//	}
//	if (projVertexNum < 3)
//		return;
//	
//
//	// check trivial cases
//	int type = CheckTrivialCases(beamVertex, beamVertexNum, curBeam->normal, projVertex, projVertexNum); //-1 full miss, 0 need beam split, 1 full hit
//	if(type == 1) //fully hit
//	{
//		vector<emxBeam*> splitBeams;
//		CheckHitBeam(curBeam, mp, splitBeams); //判断当前三角形和之前相交的三角形之间的前后关系，保留最近的三角形信息
//		if(!splitBeams.empty())
//		{
//			for(size_t i = 0; i < splitBeams.size(); ++i) 
//			{
//				splitBeams[i]->parentBeam = curBeam;
//				curBeam->childBeams.push_back(splitBeams[i]);
//			}
//		}
//	}
//	else if(type == 0) //partially hit
//	 	SplitBeam(curBeam, mp, projVertex, projVertexNum);
//}


void emxKdTree::BeamTriangleIntersection(emxBeam *curBeam, emxMailboxPrim *mp, bool abovezero[3]) const
{
	Vector3d beamVertex[4];
	int beamVertexNum = curBeam->cornerNum;
	for(int i = 0; i < beamVertexNum; ++i)
		beamVertex[i] = curBeam->BeamVertex[i];
	Vector3d normal    = curBeam->normal;
	Vector3d triangleVertex[3] = { mp->face->GetVertex(0)->GetPos(), mp->face->GetVertex(1)->GetPos(), mp->face->GetVertex(2)->GetPos() };
	Vector3d triangleNormal = mp->face->GetNormal();

	////判断目标三角形是否与beam存在相交，即beam的源点必须在三角面片之上
	//if (Dot(curBeam->origin - triangleVertex[0], triangleNormal) <= DOUBLE_EPSILON)
	//{
	//	return;
	//}
	bool  fullOut = false;
	//判断三角面片是否在beam覆盖区域之外
	for(int i = 0; i < curBeam->cornerNum; ++i) 
	{
		Vector3d edgeDir = curBeam->BeamVertex[(i + 1) %curBeam->cornerNum] - curBeam->BeamVertex[i];
		Vector3d direction = curBeam->dir[i];
		Vector3d outDir  = VectorCross(edgeDir, direction).normalize();
		bool fullOut = true;
		for(int j = 0; j < 3; ++j)
		{
			fullOut &= Dot(triangleVertex[j] - curBeam->BeamVertex[i], outDir) >= -DOUBLE_EPSILON;
			if (!fullOut)
			{
				break;
			}
		}			
		if(fullOut)
			return;
	}
	////判断三角面片是否在beam所在平面之前
	//bool fullBehind = true;
	//bool abovezero[3] = {false,false,false};  //三角面片的三个顶点与beam所在平面的关系：之前，之后
	//for(int i = 0; i < 3; ++i)
	//{
	//	abovezero[i] = (Dot(triangleVertex[i] - curBeam->BeamVertex[0], curBeam->normal) >=  DOUBLE_EPSILON);  //在beam所在平面之后
 //		fullBehind &= !abovezero[i] ;
	//}
	//if(fullBehind)
	//	return;
	// 将目标三角形投影到射线管beam起始面所在平面上
	double dist[3]= {-1,-1,-1};
	Vector3d triangle_direction[3];
	for (int l =0;l<3;l++)
	{
		double invDDotN;
		triangle_direction[l] = (triangleVertex[l] - curBeam->origin).normalize();
		if (abovezero[l])
		{
			invDDotN = 1.0 / Dot(triangle_direction[l],normal);
			dist[l] = Dot(triangleVertex[l] - beamVertex[0], normal) * invDDotN;
		}
	}

	// clip triangles which one or two vertex in front of the beam plane
	Vector3d projVertex_p[4];
	int projVertexNum_p = 0;
	//先求出triangle各点在beam初始面上的投影点，并裁剪掉在beam plane之前的部分
	for(int i = 0; i < 3; ++i) 
	{
		if(abovezero[i]) 
			projVertex_p[projVertexNum_p++] = triangleVertex[i] - triangle_direction[i] * dist[i];
		//穿透时的交点
		if(abovezero[i] == !abovezero[(i+1)%3]) 
		{
			//三角形边AC穿透beam所在面片，求出边AC和beam的交点即可
			Vector3d A = triangleVertex[i];
			Vector3d C = triangleVertex[(i + 1) % 3];
			Vector3d direction = (C-A).normalize();
			double t = Dot((beamVertex[0]-A),normal)/Dot(direction,normal);
			projVertex_p[projVertexNum_p++] = A + direction*t;
		}
	}
	// reverse order
	swap(projVertex_p[0], projVertex_p[2]);
	assert(projVertexNum_p <= 4);

	//剔除投影点邻近的情况
	Vector3d projVertex[4];
	int projVertexNum = 0;
	for (int i = 0; i < projVertexNum_p; i++)
	{
		if ((projVertex_p[i] - projVertex_p[(i+1)%projVertexNum_p]).norm() > DOUBLE_EPSILON)
		{
			projVertex[projVertexNum++] = projVertex_p[i];
		}
	}
	if (projVertexNum < 3)
		return;


	// check trivial cases
	int type = CheckTrivialCases(beamVertex, beamVertexNum, curBeam->normal, projVertex, projVertexNum); //-1 full miss, 0 need beam split, 1 full hit
	if(type == 1) //fully hit
	{
		vector<emxBeam*> splitBeams;
		CheckHitBeam(curBeam, mp, splitBeams); //判断当前三角形和之前相交的三角形之间的前后关系，保留最近的三角形信息
		if(!splitBeams.empty())
		{
			for(size_t i = 0; i < splitBeams.size(); ++i) 
			{
				splitBeams[i]->parentBeam = curBeam;
				curBeam->childBeams.push_back(splitBeams[i]);
			}
		}
	}
	else if(type == 0) //partially hit
		SplitBeam(curBeam, mp, projVertex, projVertexNum);
}
/*
 * check trivial cases: -1 full miss, 0 need beam split, 1 full hit
 */
int emxKdTree::CheckTrivialCases(Vector3d beamVertex[4], int beamVertexNum, Vector3d& normal, Vector3d projVertex[4], int projVertexNum) const
{
	// check projected triangle using beam
	for(int i = 0; i < beamVertexNum; ++i)
	{
		Vector3d edgeDir = beamVertex[(i + 1) % beamVertexNum] - beamVertex[i];
		Vector3d outDir  = VectorCross(edgeDir, normal).normalize();
		bool fullMiss = true;
		for(int j = 0; j < projVertexNum; ++j)
		{
			fullMiss &= (Dot(outDir, projVertex[j] - beamVertex[i]) >= -DOUBLE_EPSILON);
		}			
		if(fullMiss) return -1;
	}

	// check beam using projected triangle
	bool fullHit = true;
	for(int i = 0; i < projVertexNum; ++i)
	{
		Vector3d edgeDir = projVertex[(i + 1) % projVertexNum] - projVertex[i];
		Vector3d outDir  = VectorCross(edgeDir, normal).normalize();
		bool fullMiss = true;
		for(int j = 0; j < beamVertexNum; ++j)
		{
			double t = Dot(outDir, beamVertex[j] - projVertex[i]);
			fullMiss &= (t >= -DOUBLE_EPSILON);
			fullHit &= (t <= DOUBLE_EPSILON);
		}
		if(fullMiss) 
			return -1; //如果投影beam均在目标三角形某一条边的外侧，则miss掉了
	}
	if (fullHit) 
		return 1; //如果投影beam均在目标三角形所有边的内侧，则完全hit
	return 0; //其他情况则是部分相交，需分裂beam
}

/*
 * Beam Splitting 
 */
void emxKdTree::SplitBeam(emxBeam *parentBeam, emxMailboxPrim *mp, Vector3d projVertex[4], int projVertexNum) const
{
	Vector3d beamVertex[8];		// 4 vertices beam at most split to 8 vertices beam
	Vector3d normal = parentBeam->normal;
	int beamVertexNum = parentBeam->cornerNum;
	for(int i = 0; i < beamVertexNum; ++i)
		beamVertex[i] = parentBeam->BeamVertex[i];

	// clip one edge at a time
	for(int i = 0; i < projVertexNum; ++i)
	{
		Vector3d edgeDir = projVertex[(i + 1) % projVertexNum] - projVertex[i];
		Vector3d outDir  = VectorCross(edgeDir, normal).normalize();
		double dist[8];
		for(int j = 0; j < beamVertexNum; ++j)
			dist[j] = Dot(outDir, beamVertex[j] - projVertex[i]);
		Vector3d missBeamVertex[8];		// at most 5 vertices
		Vector3d hitBeamVertex[8];
		int missVertexNum = 0, hitVertexNum = 0;
		for(int j = 0; j < beamVertexNum; ++j) 
		{
			double t     = dist[j];
			double tnext = dist[(j + 1) % beamVertexNum];
			if(t > DOUBLE_EPSILON)			// out of the beam
				missBeamVertex[missVertexNum++] = beamVertex[j];
			else if(t < -DOUBLE_EPSILON)	// inside the beam
				hitBeamVertex[hitVertexNum++] = beamVertex[j];
			else 
			{							// vertex on the edge
				missBeamVertex[missVertexNum++] = beamVertex[j];
				hitBeamVertex[hitVertexNum++] = beamVertex[j];
			}
			if(fabs(t) > DOUBLE_EPSILON && fabs(tnext) > DOUBLE_EPSILON && t * tnext < 0.0)
			{	// beam edge across the edge
				Vector3d crossPos = (beamVertex[j] * fabs(tnext) + beamVertex[(j + 1) % beamVertexNum] * fabs(t)) / (fabs(t) + fabs(tnext));
				missBeamVertex[missVertexNum++] = crossPos;
				hitBeamVertex[hitVertexNum++] = crossPos;
			}
		}
		// update the remaining beam
		beamVertexNum = 0;
		if (hitVertexNum > 2)
		{		
			for(int j = 0; j < hitVertexNum; ++j) 
			{
				if((hitBeamVertex[j] - hitBeamVertex[(j + 1) % hitVertexNum]).norm() > DOUBLE_EPSILON)  //handle the numerical error
					beamVertex[beamVertexNum++] = hitBeamVertex[j];
			}
		}	
	
		if (beamVertexNum < 3)
		{ 
			for(size_t i = 0; i < parentBeam->childBeams.size(); ++i)
				delete parentBeam->childBeams[i];
			parentBeam->childBeams.clear();
			break; 
		}
		// handle miss beam for current triangle
		if(missVertexNum > 2) 
		{
			// handle the numerical error
			int reducedVertexNum = 0;
			for(int j = 0; j < missVertexNum; ++j) 
			{
				if((missBeamVertex[j] - missBeamVertex[(j + 1) % missVertexNum]).norm() > DOUBLE_EPSILON)
					missBeamVertex[reducedVertexNum++] = missBeamVertex[j];
			}
			missVertexNum = reducedVertexNum;

			if(missVertexNum > 2) 
			{
				vector<emxBeam*> beams;
				GenerateBeams(parentBeam, missBeamVertex, missVertexNum, beams);
				parentBeam->childBeams.insert(parentBeam->childBeams.end(), beams.begin(), beams.end());
			}
		}
	}
	// Handling hit beam for current triangle
	if(beamVertexNum >= 3) 
	{
		vector<emxBeam*> beams;
		GenerateBeams(parentBeam, beamVertex, beamVertexNum, beams);
		for(size_t i = 0; i < beams.size(); ++i)
		{
			vector<emxBeam*> splitBeams;
			CheckHitBeam(beams[i], mp, splitBeams);
			if(splitBeams.empty()) 
			{
				parentBeam->childBeams.push_back(beams[i]);
			} 
			else 
			{
				delete beams[i];
				parentBeam->childBeams.insert(parentBeam->childBeams.end(), splitBeams.begin(), splitBeams.end());
			}
		}
	}
}

/*
 * generate new child beams using parent beam and beamVertex
 */
void emxKdTree::GenerateBeams(emxBeam *parentBeam, Vector3d beamVertex[8], int beamVertexNum, vector<emxBeam*>& beams) const
{
	double tmax[8];	// distance from beamvertex  to triangle, 原始Beam射出去到相交面上的距离
	bool hitBeam = parentBeam->IsHitBeam();
	if(hitBeam) 
	{	// update the tmax of new beam for hit parent beam 
		Vector3d trianlgeOrigin = parentBeam->hitFace->GetVertex(0)->GetPos();
		Vector3d triangleNormal = parentBeam->hitFace->GetNormal();

		for(int i = 0; i < beamVertexNum; ++i)
		{
			Vector3d direction = (beamVertex[i] - parentBeam->origin).normalize();  //??????????

			double invDDotN;
			/*invDDotN = 1.0 / Dot(direction, triangleNormal);
			tmax[i] = -Dot(beamVertex[i] - trianlgeOrigin, triangleNormal) * invDDotN;

			if (tmax[i] < -DOUBLE_EPSILON)
			{
				cout<<"GeneratesBeams: "<<"error, tmax[i] is less than 0:"<<tmax[i]<<endl;
			}*/
			if (fabs(Dot(beamVertex[i] - trianlgeOrigin, triangleNormal)) <= DOUBLE_EPSILON)  //位于三角面片上的beamVertex，tmax[i] = 0
			{
				tmax[i] = 0;
			}
			else
			{
				if 	(Dot(direction,triangleNormal) >= -DOUBLE_EPSILON)  //beam棱边平行或者远离三角面片
				{
					cout<<"GeneratesBeams abnormal beam"<<endl;
				}
				double invDDotN;
				invDDotN = 1.0 / Dot(direction, triangleNormal);
				tmax[i] = -Dot(beamVertex[i] - trianlgeOrigin, triangleNormal) * invDDotN;

				if (tmax[i] < -DOUBLE_EPSILON)
				{
			//		cout<<"GeneratesBeams: "<<"error, tmax[i] is less than 0:"<<tmax[i]<<endl;
				}
			}			
		}
	}
	for(int i = 1; i < beamVertexNum - 1; i += 2)
	{
		emxBeam *beam = new emxBeam();
		beam->cornerNum  = beamVertexNum - i + 1;
		beam->cornerNum  = beam->cornerNum > 4 ? 4 : beam->cornerNum;
		beam->BeamVertex[0]  = beamVertex[0];
		beam->maxDist[0] = hitBeam ? tmax[0] : INFINITY;
		beam->dir[0] = (beamVertex[0] - parentBeam->origin).normalize();
		//beam->invDir[0] = Vector3d(1.0 / beam->dir[0].x, 1.0 / beam->dir[0].y, 1.0 / beam->dir[0].z);
		for(int j = 1; j < beam->cornerNum; ++j) 
		{
			beam->BeamVertex[j]  = beamVertex[i + j - 1];
			beam->maxDist[j] = hitBeam ? tmax[i + j - 1] : INFINITY;
			beam->dir[j] = (beamVertex[i + j - 1] - parentBeam->origin).normalize();
			//beam->invDir[j] = Vector3d(1.0 / beam->dir[j].x, 1.0 / beam->dir[j].y, 1.0 / beam->dir[j].z);
		}
		beam->beam_depth = parentBeam->beam_depth + 1;
		beam->origin = parentBeam->origin;
		beam->normal    = parentBeam->normal;
		beam->hitFaceID = parentBeam->hitFaceID;
		beam->hitFace   = parentBeam->hitFace;
		beam->bounce    = parentBeam->bounce;
		beam->parentBeam = parentBeam;
		beam->beam_type = 2;
		assert(beam->cornerNum > 2);
		beams.push_back(beam);
	}
	assert((beamVertexNum == 3 && beams.size() == 1) ||
		   (beamVertexNum == 4 && beams.size() == 1) ||
		   (beamVertexNum == 5 && beams.size() == 2) ||
		   (beamVertexNum == 6 && beams.size() == 2) ||
		   (beamVertexNum == 7 && beams.size() == 3) ||
		   (beamVertexNum == 8 && beams.size() == 3));
}
/*
 * check the hit beams' vertices against the old hit beams's plane and update the tmax of corner rays
 * 	               ______________oldNear
 *    curNear______|________
 *                 |       |
 *    ---------------------------------- tmax[i] - curBeam->maxDist[i]
 *                     0
 */
void emxKdTree::CheckHitBeam(emxBeam *curBeam, emxMailboxPrim *mp, vector<emxBeam*>& splitBeams) const
{
	if(curBeam->IsHitBeam() && curBeam->hitFace == mp->face)
		return;

	double tmax[4];	// distance from beam origin to triangle
	int beamVertexNum = curBeam->cornerNum;

	Vector3d trianlgeOrigin = mp->face->GetVertex(0)->GetPos();
	Vector3d triangleNormal = mp->face->GetNormal();

	for(int i = 0; i < beamVertexNum; ++i)
	{
		Vector3d direction = curBeam->dir[i];  //???????????

		/*double invDDotN = 1.0 / Dot(direction, triangleNormal);
		tmax[i] = -Dot(curBeam->BeamVertex[i] - trianlgeOrigin, triangleNormal) * invDDotN;
		if ( tmax[i] < -DOUBLE_EPSILON)
		{
			cout<<"CheckHitBeam: "<<"error, tmax[i] is less than 0 :"<<tmax[i]<<endl;
		}	*/
		if (fabs(Dot(curBeam->BeamVertex[i] - trianlgeOrigin, triangleNormal)) <= DOUBLE_EPSILON) //位于三角面片上的beamVertex，tmax[i] = 0
		{
			tmax[i] = 0;
		}
		else
		{
			if (Dot(direction,triangleNormal) >= -DOUBLE_EPSILON ) 
			{
				cout<<"CheckHitBeam abnormal beam"<<endl;
			}
			double invDDotN;
			invDDotN = 1.0 / Dot(direction, triangleNormal);
			tmax[i] = -Dot(curBeam->BeamVertex[i] - trianlgeOrigin, triangleNormal) * invDDotN;
			if ( tmax[i] < -DOUBLE_EPSILON)
			{
		//		cout<<"CheckHitBeam: "<<"error, tmax[i] is less than 0 :"<<tmax[i]<<endl;
			}	
		}	
	}

	// check which face is closer
	bool oldNear = false, curNear = true, fuzzyEqual = true;
	if(curBeam->hitFace != NULL) 
	{	// hit before
		oldNear = true;
		for(int i = 0; i < beamVertexNum; ++i)
		{	// fuzzy compare
			double difference = tmax[i] - curBeam->maxDist[i];
			oldNear    &= (difference > 1e-5);
			curNear    &= (difference < -1e-5);
			fuzzyEqual &= (difference >= -1e-5 && difference <= 1e-5);
		}
	}

	if(!oldNear && !curNear && !fuzzyEqual) 
	{
		// triangles are pierce, need additional split
		//cout<<"Triangles "<<curBeam->hitFaceID<<" "<<mp->faceID<<" are pierce"<<endl;
	//	cout<<curBeam->hitFace->GetVertex(0)->GetPos()<<"  "<<curBeam->hitFace->GetVertex(1)->GetPos()<<"  "<<curBeam->hitFace->GetVertex(2)->GetPos()<<endl;
		//cout<<mp->face->GetVertex(0)->GetPos()<<"  "<<mp->face->GetVertex(1)->GetPos()<<"  "<<mp->face->GetVertex(2)->GetPos()<<endl;
		double oldDist[5], curDist[5];
		Vector3d oldVertex[5], curVertex[5];
		int oldVertexNum = 0, curVertexNum = 0;
		for(int i = 0; i < beamVertexNum; ++i)
		{
			double d = tmax[i] - curBeam->maxDist[i];;
			double dnext = tmax[(i + 1) % beamVertexNum]  - curBeam->maxDist[(i + 1) % beamVertexNum];
			if(d >= -DOUBLE_EPSILON) 
			{
				oldVertex[oldVertexNum] = curBeam->BeamVertex[i];
				oldDist[oldVertexNum++] = curBeam->maxDist[i];
			}
			else if(d < -DOUBLE_EPSILON)
			{
				curVertex[curVertexNum] = curBeam->BeamVertex[i];
				curDist[curVertexNum++] = tmax[i];
			}
			if(fabs(d) > DOUBLE_EPSILON && fabs(dnext) > DOUBLE_EPSILON && d * dnext < 0.0)
			{
				Vector3d beamVertexONoldtriangle_1, beamVertexONoldtriangle_2;
				Vector3d beamVertexONcurtriangle_1, beamVertexONcurtriangle_2;
				beamVertexONoldtriangle_1 = curBeam->BeamVertex[i] + curBeam->maxDist[i] * curBeam->dir[i];
				beamVertexONoldtriangle_2 = curBeam->BeamVertex[(i + 1) % beamVertexNum] + curBeam->maxDist[(i + 1) % beamVertexNum] * curBeam->dir[(i + 1) % beamVertexNum];
				beamVertexONcurtriangle_1 = curBeam->BeamVertex[i] + tmax[i] * curBeam->dir[i];
				beamVertexONcurtriangle_2 = curBeam->BeamVertex[(i + 1) % beamVertexNum] + tmax[(i + 1) % beamVertexNum] * curBeam->dir[(i + 1) % beamVertexNum];
				Vector3d A = beamVertexONcurtriangle_1, B = beamVertexONoldtriangle_1, C =  beamVertexONcurtriangle_2, D = beamVertexONoldtriangle_2;
				double Area_ABC = 0.5 * (VectorCross(B-A,C-A).norm());
				double Area_ACD = 0.5 * (VectorCross(C-A,D-A).norm());
				double t = Area_ABC/Area_ACD;
				Vector3d posONtriangle = (B + D * t)/(1+t);
				Vector3d direction = (posONtriangle - curBeam->origin).normalize();
				double dist = Dot(posONtriangle- curBeam->BeamVertex[0], curBeam->normal) / Dot(direction, curBeam->normal);
				Vector3d posONbeam = posONtriangle - direction * dist;
				oldVertex[oldVertexNum] = posONbeam;
				oldDist[oldVertexNum++] = dist;
				curVertex[curVertexNum] = posONbeam;
				curDist[curVertexNum++] = dist;
			}
		}
		for(int i = 1; i < oldVertexNum - 1; i += 2)
		{
			emxBeam *beam = new emxBeam();
			beam->cornerNum  = oldVertexNum - i + 1;
			beam->cornerNum  = beam->cornerNum > 4 ? 4 : beam->cornerNum;
			beam->BeamVertex[0]  = oldVertex[0];
			beam->maxDist[0] = oldDist[0];
			beam->dir[0] = (oldVertex[0] - curBeam->origin).normalize();
			//beam->invDir[0] = Vector3d(1.0 / beam->dir[0].x, 1.0 / beam->dir[0].y, 1.0 / beam->dir[0].z);
			for(int j = 1; j < beam->cornerNum; ++j)
			{
				beam->BeamVertex[j]  = oldVertex[i + j - 1];
				beam->maxDist[j] = oldDist[i + j - 1];
				beam->dir[j] = (oldVertex[i + j - 1] - curBeam->origin).normalize();
				//beam->invDir[j] = Vector3d(1.0 / beam->dir[j].x, 1.0 / beam->dir[j].y, 1.0 / beam->dir[j].z);
			}
			beam->beam_depth = curBeam->beam_depth;
			beam->origin    = curBeam->origin;
			beam->normal    = curBeam->normal;
			beam->hitFaceID = curBeam->hitFaceID;
			beam->hitFace   = curBeam->hitFace;
			beam->bounce    = curBeam->bounce;
			beam->parentBeam = curBeam->parentBeam;
			beam->beam_type = 2;
			assert(beam->cornerNum > 2);
			splitBeams.push_back(beam);
		}
		for(int i = 1; i < curVertexNum - 1; i += 2) 
		{
			emxBeam *beam = new emxBeam();
			beam->cornerNum  = curVertexNum - i + 1;
			beam->cornerNum  = beam->cornerNum > 4 ? 4 : beam->cornerNum;
			beam->BeamVertex[0]  = curVertex[0];
			beam->maxDist[0] = curDist[0];
			beam->dir[0] = (curVertex[0] - curBeam->origin).normalize();
			//beam->invDir[0] = Vector3d(1.0 / beam->dir[0].x, 1.0 / beam->dir[0].y, 1.0 / beam->dir[0].z);
			for(int j = 1; j < beam->cornerNum; ++j) 
			{
				beam->BeamVertex[j]  = curVertex[i + j - 1];
				beam->maxDist[j] = curDist[i + j - 1];
				beam->dir[j] = (curVertex[i + j - 1] - curBeam->origin).normalize();
				//beam->invDir[j] = Vector3d(1.0 / beam->dir[j].x, 1.0 / beam->dir[j].y, 1.0 / beam->dir[j].z);
			}
			beam->beam_depth = curBeam->beam_depth;
			beam->origin = curBeam -> origin;
			beam->normal    = curBeam->normal;
			beam->hitFaceID = mp->faceID;
			beam->hitFace   = mp->face;
			beam->bounce    = curBeam->bounce;
			beam->parentBeam = curBeam->parentBeam;
			beam->beam_type = 2;
			assert(beam->cornerNum > 2);
			splitBeams.push_back(beam);
		}
	}
	else if(curNear) 
	{	// current face is closer
		curBeam->hitFaceID = mp->faceID;
		curBeam->hitFace   = mp->face;
		for(int i = 0; i < curBeam->cornerNum; ++i)
			curBeam->maxDist[i] = tmax[i];
	}
}

//
///*
// * intersect current beam with triangle
// */
//void emxKdTree::BeamTriangleIntersection(emxBeam *curBeam, emxMailboxPrim *mp) const
//{
//	Vector3d beamVertex[4];
//	int beamVertexNum = curBeam->cornerNum;
//	for(int i = 0; i < beamVertexNum; ++i)
//		beamVertex[i] = curBeam->BeamVertex[i];
//	Vector3d normal    = curBeam->normal;
//	Vector3d triangleVertex[3] = { mp->face->GetVertex(0)->GetPos(), mp->face->GetVertex(1)->GetPos(), mp->face->GetVertex(2)->GetPos() };
//	Vector3d triangleNormal = mp->face->GetNormal();
//
//	//判断目标三角形朝向是否背向beam射出去的方向，当beam 至少有一条棱边与目标三角形面片相交时，则判断为存在相交
//	int true_Num = 0;
//	for (int i = 0; i < beamVertexNum; i++)
//	{
//		if (Dot(curBeam->dir[i], triangleNormal) < -DOUBLE_EPSILON)  //相交，其他的平行或者背离目标三角形面片
//		{
//			true_Num++;
//		}
//	}
//	if(true_Num == 0)  //beam没有棱边与目标面片相交
//		return;
//
//	//判断目标三角形是否在beam之前
//	double tmax[4];	// distance from beam origin to triangle
//	bool behind1 = true;
//	Vector3d trianlgeOrigin = mp->face->GetVertex(0)->GetPos();
//	for(int i = 0; i < beamVertexNum; ++i)
//	{
//		Vector3d direction = curBeam->dir[i];  
//		if (Dot(direction,triangleNormal) >= -DOUBLE_EPSILON )  //射线方向与面片方向平行或者背离时
//		{
//			tmax[i] = INFINITY;	
//		}
//		else
//		{
//			double invDDotN = 1.0 / Dot(direction, triangleNormal);	
//			tmax[i] = -Dot(beamVertex[i] - trianlgeOrigin, triangleNormal) * invDDotN;
//
//			behind1 &= (tmax[i] < DOUBLE_EPSILON);  	// 0.0 or DOUBLE_EPSILON?
//		}			
//	}	
//	if(behind1)
//	{
//		if (true_Num == beamVertexNum)  	// triangle fully behind the beam's plane)
//			return;
//		//else  //beam 的某些棱边与三角面片相交异常时，这时候如何判断三角面片是否在beam之前，若三角形面片和beam底面不相交则三角面片在beam 之前
//		//{
//		//	int intersectNum1 = 0, intersectNum2 = 0;
//		//	bool valid1 = false,valid2 = false;
//		//	for (int triangleVertexId = 0; triangleVertexId < 3;triangleVertexId++)
//		//	{
//		//		emxRay ray;
//		//		ray.origin = triangleVertex[triangleVertexId];
//		//		ray.direction = (triangleVertex[(triangleVertexId+1)%3] - triangleVertex[triangleVertexId]).normalize();
//		//		ray.mint = 0;
//		//		ray.maxt = (triangleVertex[(triangleVertexId+1)%3] - triangleVertex[triangleVertexId]).norm();
//		//		if (curBeam->cornerNum == 3)
//		//		{
//		//			valid1 = RayTriangleintersect(ray, curBeam->BeamVertex[0], curBeam->BeamVertex[1],curBeam->BeamVertex[2]);
//		//		}
//		//		else if (curBeam->cornerNum  == 4)
//		//		{
//		//			valid1 = RayTriangleintersect(ray, curBeam->BeamVertex[0], curBeam->BeamVertex[1], curBeam->BeamVertex[2]) ||
//		//				RayTriangleintersect(ray, curBeam->BeamVertex[2], curBeam->BeamVertex[3], curBeam->BeamVertex[0]);
//		//		}
//		//		if (valid1)
//		//		{
//		//			intersectNum1++;
//		//			break;
//		//		}
//		//	}
//		//	if (intersectNum1 == 0)
//		//	{
//		//		for (int beamVertexId = 0; beamVertexId < curBeam->cornerNum;beamVertexId++)
//		//		{
//		//			emxRay ray;
//		//			ray.origin = curBeam->BeamVertex[beamVertexId];
//		//			ray.direction = (curBeam->BeamVertex[(beamVertexId+1)%curBeam->cornerNum] - curBeam->BeamVertex[beamVertexId]).normalize();
//		//			ray.mint = 0;
//		//			ray.maxt = (curBeam->BeamVertex[(beamVertexId+1)%curBeam->cornerNum] - curBeam->BeamVertex[beamVertexId]).norm();				
//		//			valid2 = RayTriangleintersect(ray, triangleVertex[0], triangleVertex[1],triangleVertex[2]);			
//		//			if (valid2)
//		//			{
//		//				intersectNum2++;
//		//				break;
//		//			}
//		//		}
//		//		if (intersectNum2 == 0)
//		//		{
//		//			return;
//		//		}
//		//	}			
//		//}
//		//else  //beam 的某些棱边与三角面片相交异常时，这时候如何判断三角面片是否在beam覆盖区域内部
//		//{
//		//	int intersectNum1 = 0, intersectNum2 = 0;
//		//	bool valid1 = false, valid2 = false;
//		//	for (int triangleVertexId = 0; triangleVertexId < 3;triangleVertexId++)
//		//	{
//		//		emxRay ray;
//		//		ray.origin = curBeam->origin;
//		//		ray.direction = (triangleVertex[triangleVertexId] - ray.origin).normalize();
//		//		ray.mint = 0;
//		//		ray.maxt = (ray.origin - triangleVertex[triangleVertexId]).norm();
//		//		if (curBeam->cornerNum == 3)
//		//		{
//		//			valid1 = RayTriangleintersect(ray, curBeam->BeamVertex[0], curBeam->BeamVertex[1],curBeam->BeamVertex[2]);
//		//		}
//		//		else if (curBeam->cornerNum  == 4)
//		//		{
//		//			valid1 = RayTriangleintersect(ray, curBeam->BeamVertex[0], curBeam->BeamVertex[1], curBeam->BeamVertex[2]) ||
//		//				RayTriangleintersect(ray, curBeam->BeamVertex[2], curBeam->BeamVertex[3], curBeam->BeamVertex[0]);
//		//		}
//		//		if (valid1)
//		//		{
//		//			intersectNum1 ++;
//		//			break;
//		//		}
//		//		if (intersectNum1 == 0)
//		//		{
//		//			for (int beamVertexId = 0; beamVertexId < curBeam->cornerNum;beamVertexId++)
//		//			{
//		//				emxRay ray;
//		//				ray.origin = curBeam->BeamVertex[beamVertexId];
//		//				ray.direction = curBeam->dir[beamVertexId];
//		//				ray.mint = 0;
//		//				ray.maxt = INFINITY;				
//		//				valid2 = RayTriangleintersect(ray, triangleVertex[0], triangleVertex[1],triangleVertex[2]);			
//		//				if (valid2)
//		//				{
//		//					intersectNum2++;
//		//					break;
//		//				}
//		//			}
//		//			if (intersectNum2 == 0)
//		//			{
//		//				return;
//		//			}
//		//		}
//		//	}		
//		//}
//	}
//	
//	//找出所有能够和beam有效相交的三角形顶点
//	Vector3d triangle_direction[3];
//	vector<int> intersection_VertexId;
//	for (int l =0;l<3;l++)
//	{
//		 triangle_direction[l] = (triangleVertex[l] - curBeam->origin).normalize();
//		if (Dot( triangle_direction[l],normal) >= DOUBLE_EPSILON)
//		{
//			intersection_VertexId.push_back(l);
//		}
//	}
//	if (intersection_VertexId.size() == 0)
//	{
//		return;
//	}
//	// 将目标三角形投影到射线管beam起始面所在平面上
//	double dist[4];
//	Vector3d Newtriangle_direction[4];
//	Vector3d Newtriangle_Vertex[4];
//	int triangle_vertexNum = 0;
//	bool behind2 = true;
//	for(int i = 0; i < 3; ++i)
//	{
//		double invDDotN;
//		if (Dot(triangle_direction[i],normal) <  DOUBLE_EPSILON) 
//		{
//			//cout<<"BeamTriangleIntersection abnormal triangle"<<endl;
//			if (intersection_VertexId.size()  == 2)
//			{
//				Vector3d triangle_direction1 = triangle_direction[i];
//				int j = 1;
//				while (Dot(triangle_direction1,normal)< DOUBLE_EPSILON && j<=1000)
//				{				
//					Newtriangle_Vertex[triangle_vertexNum] = 0.001* j * triangleVertex[(i-1+3)%3] + (1 - 0.001*j)*triangleVertex[i];
//					Newtriangle_direction[triangle_vertexNum] = (Newtriangle_Vertex[triangle_vertexNum] - curBeam->origin).normalize();
//
//					triangle_direction1 = Newtriangle_direction[triangle_vertexNum];
//					j++;
//				} 
//				invDDotN = 1.0 / Dot(Newtriangle_direction[triangle_vertexNum],normal);
//				dist[triangle_vertexNum] = Dot(Newtriangle_Vertex[triangle_vertexNum] - beamVertex[0], normal) * invDDotN;
//				behind2 &= (dist[triangle_vertexNum] < DOUBLE_EPSILON);
//				triangle_vertexNum++;
//
//				Vector3d triangle_direction2 = triangle_direction[i];
//				j = 1;
//				while (Dot(triangle_direction2,normal)< DOUBLE_EPSILON && j<=1000)
//				{				
//					Newtriangle_Vertex[triangle_vertexNum] = 0.001* j * triangleVertex[(i+1)%3] + (1 - 0.001*j)*triangleVertex[i];
//
//					Newtriangle_direction[triangle_vertexNum] = (Newtriangle_Vertex[triangle_vertexNum] - curBeam->origin).normalize();
//					triangle_direction2 = Newtriangle_direction[triangle_vertexNum] ;
//					j++;
//				} 
//				invDDotN = 1.0 / Dot(Newtriangle_direction[triangle_vertexNum],normal);
//				dist[triangle_vertexNum] = Dot(Newtriangle_Vertex[triangle_vertexNum] - beamVertex[0], normal) * invDDotN;
//				behind2 &= (dist[triangle_vertexNum] < DOUBLE_EPSILON);
//				triangle_vertexNum++;
//			}
//			else if (intersection_VertexId.size()  == 1)
//			{
//				Vector3d triangle_direction = triangle_direction[i];
//				int j = 1;
//				while (Dot(triangle_direction,normal)< DOUBLE_EPSILON && j<=1000)
//				{				
//					int intersection_VertexID = intersection_VertexId[intersection_VertexId.size()-1];
//					Newtriangle_Vertex[triangle_vertexNum] = 0.001* j * triangleVertex[intersection_VertexID] + (1 - 0.001*j)*triangleVertex[i];
//					Newtriangle_direction[triangle_vertexNum] = (Newtriangle_Vertex[triangle_vertexNum] - curBeam->origin).normalize();
//					triangle_direction = Newtriangle_direction[triangle_vertexNum] ;
//					j++;
//				} 
//				invDDotN = 1.0 / Dot(Newtriangle_direction[triangle_vertexNum],normal);
//				dist[triangle_vertexNum] = Dot(Newtriangle_Vertex[triangle_vertexNum] - beamVertex[0], normal) * invDDotN;
//				behind2 &= (dist[triangle_vertexNum] < DOUBLE_EPSILON);
//				triangle_vertexNum++;
//			}
//		}
//		else
//		{
//			Newtriangle_Vertex[triangle_vertexNum] = triangleVertex[i];
//			Newtriangle_direction[triangle_vertexNum] = (Newtriangle_Vertex[triangle_vertexNum] - curBeam->origin).normalize();
//
//			invDDotN = 1.0 / Dot(Newtriangle_direction[triangle_vertexNum],normal);
//			dist[triangle_vertexNum] = Dot(Newtriangle_Vertex[triangle_vertexNum] - beamVertex[0], normal) * invDDotN;
//			behind2 &= (dist[triangle_vertexNum] < DOUBLE_EPSILON);
//
//			triangle_vertexNum++;
//		}
//	
//	}	
//	if (behind2)
//		return;
//
//	// clip triangles which one or two vertex in front of the beam plane
//	Vector3d projVertex_p[5];
//	int projVertexNum_p = 0;
//	//先求出triangle各点在beam初始面上的投影点，并裁剪掉在beam plane之前的部分
//	for(int i = 0; i < triangle_vertexNum; ++i) 
//	{
//		if(dist[i] >= -DOUBLE_EPSILON) 
//			projVertex_p[projVertexNum_p++] = Newtriangle_Vertex[i] - Newtriangle_direction[i] * dist[i];
//		//穿透时的交点
//		if(fabs(dist[i]) > DOUBLE_EPSILON && fabs(dist[(i + 1) % triangle_vertexNum]) > DOUBLE_EPSILON && dist[i] * dist[(i + 1) % triangle_vertexNum] < 0.0) 
//		{
//			Vector3d A = Newtriangle_Vertex[i];
//			Vector3d B = Newtriangle_Vertex[i] - Newtriangle_direction[i] * dist[i];
//			Vector3d C = Newtriangle_Vertex[(i + 1) % triangle_vertexNum];
//			Vector3d D = Newtriangle_Vertex[(i + 1) % triangle_vertexNum] - Newtriangle_direction[(i + 1) % triangle_vertexNum] * dist[(i + 1) % triangle_vertexNum];
//			double Area_ABC = 0.5 * (VectorCross(B-A,C-A).norm());
//			double Area_ACD = 0.5 * (VectorCross(C-A,D-A).norm());
//			double split_ratio = Area_ABC/Area_ACD;
//			projVertex_p[projVertexNum_p++] = ( D * split_ratio + B * 1) / (1 + split_ratio); //???????
//		}
//	}
//	// reverse order
//	assert(projVertexNum_p <= 5);
//	if (projVertexNum_p == 5)
//	{
//		swap(projVertex_p[0],projVertex_p[4]);
//		swap(projVertex_p[1], projVertex_p[3]);
//	}
//	else
//	{
//		swap(projVertex_p[0], projVertex_p[2]);
//	}
//	//剔除投影点邻近的情况
//	Vector3d projVertex[5];
//	int projVertexNum = 0;
//	for (int i = 0; i < projVertexNum_p; i++)
//	{
//		if ((projVertex_p[i] - projVertex_p[(i+1)%projVertexNum_p]).norm() > DOUBLE_EPSILON)
//		{
//			projVertex[projVertexNum++] = projVertex_p[i];
//		}
//	}
//	if (projVertexNum < 3)
//		return;
//	
//
//	// check trivial cases
//	int type = CheckTrivialCases(beamVertex, beamVertexNum, curBeam->normal, projVertex, projVertexNum); //-1 full miss, 0 need beam split, 1 full hit
//	if(type == 1) //fully hit
//	{
//		vector<emxBeam*> splitBeams;
//		CheckHitBeam(curBeam, mp, splitBeams); //判断当前三角形和之前相交的三角形之间的前后关系，保留最近的三角形信息
//		if(!splitBeams.empty())
//		{
//			for(size_t i = 0; i < splitBeams.size(); ++i) 
//			{
//				splitBeams[i]->parentBeam = curBeam;
//				curBeam->childBeams.push_back(splitBeams[i]);
//			}
//		}
//	}
//	else if(type == 0) //partially hit
//	 	SplitBeam(curBeam, mp, projVertex, projVertexNum);
//}
//
///*
// * check trivial cases: -1 full miss, 0 need beam split, 1 full hit
// */
//int emxKdTree::CheckTrivialCases(Vector3d beamVertex[4], int beamVertexNum, Vector3d& normal, Vector3d projVertex[5], int projVertexNum) const
//{
//	// check projected triangle using beam
//	for(int i = 0; i < beamVertexNum; ++i)
//	{
//		Vector3d edgeDir = beamVertex[(i + 1) % beamVertexNum] - beamVertex[i];
//		Vector3d outDir  = VectorCross(edgeDir, normal).normalize();
//		bool fullMiss = true;
//		for(int j = 0; j < projVertexNum; ++j)
//		{
//			Vector3d a = projVertex[j] - beamVertex[i];
//			double  b = Dot(outDir, projVertex[j] - beamVertex[i]);
//			fullMiss &= (Dot(outDir, projVertex[j] - beamVertex[i]) >= -DOUBLE_EPSILON);
//		}
//			
//		if(fullMiss) return -1;
//	}
//
//	// check beam using projected triangle
//	bool fullHit = true;
//	for(int i = 0; i < projVertexNum; ++i)
//	{
//		Vector3d edgeDir = projVertex[(i + 1) % projVertexNum] - projVertex[i];
//
//		/*if (edgeDir.norm() < DOUBLE_EPSILON)
//		{
//			cout <<"CheckTrivialCases: "<< "the adjacent projvertex is almost overlap"<<endl;
//		}*/
//		Vector3d outDir  = VectorCross(edgeDir, normal).normalize();
//		bool fullMiss = true;
//		for(int j = 0; j < beamVertexNum; ++j)
//		{
//			double t = Dot(outDir, beamVertex[j] - projVertex[i]);
//			fullMiss &= (t >= -DOUBLE_EPSILON);
//			fullHit &= (t <= DOUBLE_EPSILON);
//		}
//		if(fullMiss) 
//			return -1; //如果投影beam均在目标三角形某一条边的外侧，则miss掉了
//	}
//	if (fullHit) 
//		return 1; //如果投影beam均在目标三角形所有边的内侧，则完全hit
//	return 0; //其他情况则是部分相交，需分裂beam
//}
//
///*
// * Beam Splitting 
// */
//void emxKdTree::SplitBeam(emxBeam *parentBeam, emxMailboxPrim *mp, Vector3d projVertex[5], int projVertexNum) const
//{
//	Vector3d beamVertex[9];		// 4 vertices beam at most split to 8 vertices beam
//	Vector3d normal = parentBeam->normal;
//	int beamVertexNum = parentBeam->cornerNum;
//	for(int i = 0; i < beamVertexNum; ++i)
//		beamVertex[i] = parentBeam->BeamVertex[i];
//
//	//Vector3d triangleVertex[3] = { mp->face->GetVertex(0)->GetPos(), mp->face->GetVertex(1)->GetPos(), mp->face->GetVertex(2)->GetPos() };
//	//Vector3d triangleNormal = Vector3d(0,0,1)/*mp->face->GetNormal()*/;
//
//	// clip one edge at a time
//	for(int i = 0; i < projVertexNum; ++i)
//	{
//		Vector3d edgeDir = projVertex[(i + 1) % projVertexNum] - projVertex[i];
//		Vector3d outDir  = VectorCross(edgeDir, normal).normalize();
//		double dist[9];
//		for(int j = 0; j < beamVertexNum; ++j)
//			dist[j] = Dot(outDir, beamVertex[j] - projVertex[i]);
//		Vector3d missBeamVertex[9];		// at most 5 vertices
//		Vector3d hitBeamVertex[9];
//		int missVertexNum = 0, hitVertexNum = 0;
//		for(int j = 0; j < beamVertexNum; ++j) 
//		{
//			double t     = dist[j];
//			double tnext = dist[(j + 1) % beamVertexNum];
//			if(t > DOUBLE_EPSILON)			// out of the beam
//				missBeamVertex[missVertexNum++] = beamVertex[j];
//			else if(t < -DOUBLE_EPSILON)	// inside the beam
//				hitBeamVertex[hitVertexNum++] = beamVertex[j];
//			else 
//			{							// vertex on the edge
//				missBeamVertex[missVertexNum++] = beamVertex[j];
//				hitBeamVertex[hitVertexNum++] = beamVertex[j];
//			}
//			if(fabs(t) > DOUBLE_EPSILON && fabs(tnext) > DOUBLE_EPSILON && t * tnext < 0.0)
//			{	// beam edge across the edge
//				Vector3d crossPos = (beamVertex[j] * fabs(tnext) + beamVertex[(j + 1) % beamVertexNum] * fabs(t)) / (fabs(t) + fabs(tnext));
//				missBeamVertex[missVertexNum++] = crossPos;
//				hitBeamVertex[hitVertexNum++] = crossPos;
//			}
//		}
//		// update the remaining beam
//		beamVertexNum = 0;
//		if (hitVertexNum > 2)
//		{		
//			for(int j = 0; j < hitVertexNum; ++j) 
//			{
//				if((hitBeamVertex[j] - hitBeamVertex[(j + 1) % hitVertexNum]).norm() > DOUBLE_EPSILON)  //handle the numerical error
//					beamVertex[beamVertexNum++] = hitBeamVertex[j];
//			}
//		}	
//	
//		if (beamVertexNum < 3)
//		{ 
//			for(size_t i = 0; i < parentBeam->childBeams.size(); ++i)
//				delete parentBeam->childBeams[i];
//			parentBeam->childBeams.clear();
//			break; 
//		}
//		// handle miss beam for current triangle
//		if(missVertexNum > 2) 
//		{
//			// handle the numerical error
//			int reducedVertexNum = 0;
//			for(int j = 0; j < missVertexNum; ++j) 
//			{
//				if((missBeamVertex[j] - missBeamVertex[(j + 1) % missVertexNum]).norm() > DOUBLE_EPSILON)
//					missBeamVertex[reducedVertexNum++] = missBeamVertex[j];
//			}
//			missVertexNum = reducedVertexNum;
//
//			if(missVertexNum > 2) 
//			{
//				vector<emxBeam*> beams;
//				GenerateBeams(parentBeam, missBeamVertex, missVertexNum, beams);
//				parentBeam->childBeams.insert(parentBeam->childBeams.end(), beams.begin(), beams.end());
//			}
//		}
//	}
//	// Handling hit beam for current triangle
//	if(beamVertexNum >= 3) 
//	{
//		vector<emxBeam*> beams;
//		GenerateBeams(parentBeam, beamVertex, beamVertexNum, beams);
//		for(size_t i = 0; i < beams.size(); ++i)
//		{
//			vector<emxBeam*> splitBeams;
//			CheckHitBeam(beams[i], mp, splitBeams);
//			if(splitBeams.empty()) 
//			{
//				parentBeam->childBeams.push_back(beams[i]);
//			} 
//			else 
//			{
//				delete beams[i];
//				parentBeam->childBeams.insert(parentBeam->childBeams.end(), splitBeams.begin(), splitBeams.end());
//			}
//		}
//	}
//}
//
///*
// * generate new child beams using parent beam and beamVertex
// */
//void emxKdTree::GenerateBeams(emxBeam *parentBeam, Vector3d beamVertex[9], int beamVertexNum, vector<emxBeam*>& beams) const
//{
//	double tmax[9];	// distance from beamvertex  to triangle, 原始Beam射出去到相交面上的距离
//	bool hitBeam = parentBeam->IsHitBeam();
//	if(hitBeam) 
//	{	// update the tmax of new beam for hit parent beam 
//		Vector3d trianlgeOrigin = parentBeam->hitFace->GetVertex(0)->GetPos();
//		Vector3d triangleNormal = parentBeam->hitFace->GetNormal();
//
//		for(int i = 0; i < beamVertexNum; ++i)
//		{
//			Vector3d direction = (beamVertex[i] - parentBeam->origin).normalize();  //???????????
//
//			if (fabs(Dot(beamVertex[i] - trianlgeOrigin, triangleNormal)) <= DOUBLE_EPSILON)  //位于三角面片上的beamVertex，tmax[i] = 0
//			{
//				tmax[i] = 0;
//			}
//			else
//			{
//				if 	(Dot(direction,triangleNormal) >= -DOUBLE_EPSILON)  //beam棱边平行或者远离三角面片
//				{
//					cout<<"GeneratesBeams abnormal beam"<<endl;
//				}
//				double invDDotN;
//				invDDotN = 1.0 / Dot(direction, triangleNormal);
//				tmax[i] = -Dot(beamVertex[i] - trianlgeOrigin, triangleNormal) * invDDotN;
//
//				if (tmax[i] < 0 )
//				{
//				//	cout<<"GeneratesBeams: "<<"error, tmax[i] is less than 0:"<<tmax[i]<<endl;
//				}
//			}			
//		}
//	}
//	for(int i = 1; i < beamVertexNum - 1; i += 2)
//	{
//		emxBeam *beam = new emxBeam();
//		beam->cornerNum  = beamVertexNum - i + 1;
//		beam->cornerNum  = beam->cornerNum > 4 ? 4 : beam->cornerNum;
//		beam->BeamVertex[0]  = beamVertex[0];
//		beam->maxDist[0] = hitBeam ? tmax[0] : INFINITY;
//		beam->dir[0] = (beamVertex[0] - parentBeam->origin).normalize();
//		//beam->invDir[0] = Vector3d(1.0 / beam->dir[0].x, 1.0 / beam->dir[0].y, 1.0 / beam->dir[0].z);
//		for(int j = 1; j < beam->cornerNum; ++j) 
//		{
//			beam->BeamVertex[j]  = beamVertex[i + j - 1];
//			beam->maxDist[j] = hitBeam ? tmax[i + j - 1] : INFINITY;
//			beam->dir[j] = (beamVertex[i + j - 1] - parentBeam->origin).normalize();
//			//beam->invDir[j] = Vector3d(1.0 / beam->dir[j].x, 1.0 / beam->dir[j].y, 1.0 / beam->dir[j].z);
//		}
//		beam->beam_depth = parentBeam->beam_depth + 1;
//		beam->origin = parentBeam->origin;
//		beam->normal    = parentBeam->normal;
//		beam->hitFaceID = parentBeam->hitFaceID;
//		beam->hitFace   = parentBeam->hitFace;
//		beam->bounce    = parentBeam->bounce;
//		beam->parentBeam = parentBeam;
//		beam->beam_type = 2;
//		assert(beam->cornerNum > 2);
//		beams.push_back(beam);
//	}
//	assert((beamVertexNum == 3 && beams.size() == 1) ||
//		   (beamVertexNum == 4 && beams.size() == 1) ||
//		   (beamVertexNum == 5 && beams.size() == 2) ||
//		   (beamVertexNum == 6 && beams.size() == 2) ||
//		   (beamVertexNum == 7 && beams.size() == 3) ||
//		   (beamVertexNum == 8 && beams.size() == 3) ||
//		   (beamVertexNum == 9 && beams.size() == 4)
//		   );
//}
///*
// * check the hit beams' vertices against the old hit beams's plane and update the tmax of corner rays
// * 	               ______________oldNear
// *    curNear______|________
// *                 |       |
// *    ---------------------------------- tmax[i] - curBeam->maxDist[i]
// *                     0
// */
//void emxKdTree::CheckHitBeam(emxBeam *curBeam, emxMailboxPrim *mp, vector<emxBeam*>& splitBeams) const
//{
//	if(curBeam->IsHitBeam() && curBeam->hitFace == mp->face)
//		return;
//
//	double tmax[4];	// distance from beam origin to triangle
//	int beamVertexNum = curBeam->cornerNum;
//
//	Vector3d trianlgeOrigin = mp->face->GetVertex(0)->GetPos();
//	Vector3d triangleNormal = mp->face->GetNormal();
//
//	for(int i = 0; i < beamVertexNum; ++i)
//	{
//		Vector3d direction = curBeam->dir[i];  //???????????
//		//double invDDotN = 1.0 / Dot(direction, triangleNormal);
//
//		if (fabs(Dot(curBeam->BeamVertex[i] - trianlgeOrigin, triangleNormal)) <= DOUBLE_EPSILON) //位于三角面片上的beamVertex，tmax[i] = 0
//		{
//			tmax[i] = 0;
//		}
//		else
//		{
//			if (Dot(direction,triangleNormal) >= -DOUBLE_EPSILON ) 
//			{
//				cout<<"CheckHitBeam abnormal beam"<<endl;
//			}
//			double invDDotN;
//			invDDotN = 1.0 / Dot(direction, triangleNormal);
//			tmax[i] = -Dot(curBeam->BeamVertex[i] - trianlgeOrigin, triangleNormal) * invDDotN;
//			if ( tmax[i] < 0 )
//			{
//			//	cout<<"CheckHitBeam: "<<"error, tmax[i] is less than 0 :"<<tmax[i]<<endl;
//			}	
//		}	
//	}
//
//	// check which face is closer
//	bool oldNear = false, curNear = true/*, fuzzyEqual = true*/;
//	if(curBeam->hitFace != NULL) 
//	{	// hit before
//		oldNear = true;
//		for(int i = 0; i < beamVertexNum; ++i)
//		{	// fuzzy compare
//			double difference = tmax[i] - curBeam->maxDist[i];
//			//if (fabs(difference) >= 1e-1/*DOUBLE_EPSILON * 1e7*/)
//			//{
//			//	oldNear    &= (difference >= 0);
//			//	curNear    &= (difference <=  0);
//			//	fuzzyEqual &= (difference >= 0 && difference <= 0);
//			//}
//			oldNear    &= (difference >= - DOUBLE_EPSILON);
//			curNear    &= (difference < -DOUBLE_EPSILON);
//		//	fuzzyEqual &= (difference >= -DOUBLE_EPSILON && difference <= DOUBLE_EPSILON);
//		}
//	}
//
//	if(!oldNear && !curNear /*&& !fuzzyEqual*/) 
//	{
//		// triangles are pierce, need additional split
//		//cout<<"Triangles "<<curBeam->hitFaceID<<" "<<mp->faceID<<" are pierce"<<endl;
//	//	cout<<curBeam->hitFace->GetVertex(0)->GetPos()<<"  "<<curBeam->hitFace->GetVertex(1)->GetPos()<<"  "<<curBeam->hitFace->GetVertex(2)->GetPos()<<endl;
//		//cout<<mp->face->GetVertex(0)->GetPos()<<"  "<<mp->face->GetVertex(1)->GetPos()<<"  "<<mp->face->GetVertex(2)->GetPos()<<endl;
//		double oldDist[5], curDist[5];
//		Vector3d oldVertex[5], curVertex[5];
//		int oldVertexNum = 0, curVertexNum = 0;
//		for(int i = 0; i < beamVertexNum; ++i)
//		{
//			double d = tmax[i] - curBeam->maxDist[i];;
//			double dnext = tmax[(i + 1) % beamVertexNum]  - curBeam->maxDist[(i + 1) % beamVertexNum];
//			if(d >= -DOUBLE_EPSILON) 
//			{
//				oldVertex[oldVertexNum] = curBeam->BeamVertex[i];
//				oldDist[oldVertexNum++] = curBeam->maxDist[i];
//			}
//			else if(d < -DOUBLE_EPSILON)
//			{
//				curVertex[curVertexNum] = curBeam->BeamVertex[i];
//				curDist[curVertexNum++] = tmax[i];
//			}
//			if(fabs(d) > DOUBLE_EPSILON && fabs(dnext) > DOUBLE_EPSILON && d * dnext < 0.0)
//			{
//				Vector3d beamVertexONoldtriangle_1, beamVertexONoldtriangle_2;
//				Vector3d beamVertexONcurtriangle_1, beamVertexONcurtriangle_2;
//				beamVertexONoldtriangle_1 = curBeam->BeamVertex[i] + curBeam->maxDist[i] * curBeam->dir[i];
//				beamVertexONoldtriangle_2 = curBeam->BeamVertex[(i + 1) % beamVertexNum] + curBeam->maxDist[(i + 1) % beamVertexNum] * curBeam->dir[(i + 1) % beamVertexNum];
//				beamVertexONcurtriangle_1 = curBeam->BeamVertex[i] + tmax[i] * curBeam->dir[i];
//				beamVertexONcurtriangle_2 = curBeam->BeamVertex[(i + 1) % beamVertexNum] + tmax[(i + 1) % beamVertexNum] * curBeam->dir[(i + 1) % beamVertexNum];
//				Vector3d A = beamVertexONcurtriangle_1, B = beamVertexONoldtriangle_1, C =  beamVertexONcurtriangle_2, D = beamVertexONoldtriangle_2;
//				double Area_ABC = 0.5 * (VectorCross(B-A,C-A).norm());
//				double Area_ACD = 0.5 * (VectorCross(C-A,D-A).norm());
//				double t = Area_ABC/Area_ACD;
//				Vector3d posONtriangle = (B + D * t)/(1+t);
//				Vector3d direction = (posONtriangle - curBeam->origin).normalize();
//				double dist = Dot(posONtriangle- curBeam->BeamVertex[0], curBeam->normal) / Dot(direction, curBeam->normal);
//				Vector3d posONbeam = posONtriangle - direction * dist;
//				oldVertex[oldVertexNum] = posONbeam;
//				oldDist[oldVertexNum++] = dist;
//				curVertex[curVertexNum] = posONbeam;
//				curDist[curVertexNum++] = dist;
//			}
//		}
//		for(int i = 1; i < oldVertexNum - 1; i += 2)
//		{
//			emxBeam *beam = new emxBeam();
//			beam->cornerNum  = oldVertexNum - i + 1;
//			beam->cornerNum  = beam->cornerNum > 4 ? 4 : beam->cornerNum;
//			beam->BeamVertex[0]  = oldVertex[0];
//			beam->maxDist[0] = oldDist[0];
//			beam->dir[0] = (oldVertex[0] - curBeam->origin).normalize();
//			//beam->invDir[0] = Vector3d(1.0 / beam->dir[0].x, 1.0 / beam->dir[0].y, 1.0 / beam->dir[0].z);
//			for(int j = 1; j < beam->cornerNum; ++j)
//			{
//				beam->BeamVertex[j]  = oldVertex[i + j - 1];
//				beam->maxDist[j] = oldDist[i + j - 1];
//				beam->dir[j] = (oldVertex[i + j - 1] - curBeam->origin).normalize();
//				//beam->invDir[j] = Vector3d(1.0 / beam->dir[j].x, 1.0 / beam->dir[j].y, 1.0 / beam->dir[j].z);
//			}
//			beam->beam_depth = curBeam->beam_depth;
//			beam->origin    = curBeam->origin;
//			beam->normal    = curBeam->normal;
//			beam->hitFaceID = curBeam->hitFaceID;
//			beam->hitFace   = curBeam->hitFace;
//			beam->bounce    = curBeam->bounce;
//			beam->parentBeam = curBeam->parentBeam;
//			beam->beam_type = 2;
//			assert(beam->cornerNum > 2);
//			splitBeams.push_back(beam);
//		}
//		for(int i = 1; i < curVertexNum - 1; i += 2) 
//		{
//			emxBeam *beam = new emxBeam();
//			beam->cornerNum  = curVertexNum - i + 1;
//			beam->cornerNum  = beam->cornerNum > 4 ? 4 : beam->cornerNum;
//			beam->BeamVertex[0]  = curVertex[0];
//			beam->maxDist[0] = curDist[0];
//			beam->dir[0] = (curVertex[0] - curBeam->origin).normalize();
//			//beam->invDir[0] = Vector3d(1.0 / beam->dir[0].x, 1.0 / beam->dir[0].y, 1.0 / beam->dir[0].z);
//			for(int j = 1; j < beam->cornerNum; ++j) 
//			{
//				beam->BeamVertex[j]  = curVertex[i + j - 1];
//				beam->maxDist[j] = curDist[i + j - 1];
//				beam->dir[j] = (curVertex[i + j - 1] - curBeam->origin).normalize();
//				//beam->invDir[j] = Vector3d(1.0 / beam->dir[j].x, 1.0 / beam->dir[j].y, 1.0 / beam->dir[j].z);
//			}
//			beam->beam_depth = curBeam->beam_depth;
//			beam->origin = curBeam -> origin;
//			beam->normal    = curBeam->normal;
//			beam->hitFaceID = mp->faceID;
//			beam->hitFace   = mp->face;
//			beam->bounce    = curBeam->bounce;
//			beam->parentBeam = curBeam->parentBeam;
//			beam->beam_type = 2;
//			assert(beam->cornerNum > 2);
//			splitBeams.push_back(beam);
//		}
//	}
//	else if(curNear) 
//	{	// current face is closer
//		curBeam->hitFaceID = mp->faceID;
//		curBeam->hitFace   = mp->face;
//		for(int i = 0; i < curBeam->cornerNum; ++i)
//			curBeam->maxDist[i] = tmax[i];
//	}
//}





