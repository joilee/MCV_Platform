/**
* emxKdTree class.
*
**/

#ifndef __EMXKDTREE_H__
#define __EMXKDTREE_H__

#include "util/emxUtilityInc.h"
#include "geometry/emxBBoxD.h"
#include "util/vector.h"
#include "emxFace.h"
//class emxMeshInterface;
class emxFace;
class emxRay;
class emxIntersectionInfo;
class emxKdTree;
class emxModel;
static int beamIndex = 0;
 //emxBeam Declarations
class emxBeam
{
public:
	// constructor
	emxBeam() { beam_depth = 0; beam_type = -1; hitFaceID = -1; hitFace = NULL; parentBeam = NULL; bounce = 1; reflectedBeam = NULL; transmittedBeam = NULL; index = beamIndex++; maxDist[0] = maxDist[1] = maxDist[2] = maxDist[3] = INFINITY; }
	~emxBeam() {
		for(size_t i = 0; i < childBeams.size(); ++i)
			delete childBeams[i];
		childBeams.clear();
		delete reflectedBeam;
	}

	// hit beam
	bool IsHitBeam() { return hitFaceID!= -1; }

	// split beam
	bool IsSplittedBeam() { return !childBeams.empty(); }

	// generate reflected beam
	void GeneratedReflectedBeam();
	// generate transmitted beam
	void GeneratedTransmittedBeam();

	// beam area
	double BeamArea();

	// debug
	bool belongBeam(int beamID);

	Vector3d origin;		// Beam origin 三(四)角锥光束顶点
	Vector3d BeamVertex[4]; //三(四)角锥光束锥底面顶点
	Vector3d normal;		// the normal of the image plane

	//而对于辐射问题（即球面波入射）时，beam是四（三）棱锥，四（或三）个角点射线的direction不同
	Vector3d dir[4];			// Beam direction

	double   maxDist[4];	// Max distance along corner rays
	int		 cornerNum;		// Number of beam corners
	int      hitFaceID;		// the face ID of the hit beam
	int      index;			// the index in beam tree
	emxFace* hitFace;		// the face of the hit beam
	int      bounce;		// the index of the current bounce
	emxBeam* parentBeam;	// the debug parent beam
	emxBeam* reflectedBeam;	// the reflected Beam for the hit beam
	emxBeam* transmittedBeam; //the transmitted Beam for the hit beam
	vector<emxBeam*> childBeams;	// splitting children beams
	int  beam_type;  //值为0是reflectedbeam，值为1是transmittedbeam,值为2是childbeam
	int beam_depth;  //beam在当前beam_tree中的深度
};

static int nodeIndex = 0;
// emxKdTreeNode Declarations
class emxKdTreeNode 
{
	friend class emxKdTree;
public:
	emxKdTreeNode() {}
	// KdAccelNode Methods
	void initLeaf(int *primNums, int np, emxMailboxPrim *mailboxPrims) {
		++numLeafMade;
		maxLeafPrims = Max(np, maxLeafPrims);
		leafPrims += np;
		nPrims = np << 2;
		flags |= 3;
		// Store _MailboxPrim *_s for leaf node
		if (np == 0)
			onePrimitive = NULL;
		else if (np == 1)
			onePrimitive = &mailboxPrims[primNums[0]];
		else {
			primitives = new emxMailboxPrim *[np];
			for (int i = 0; i < np; ++i)
				primitives[i] = &mailboxPrims[primNums[i]];
			sortPrimitives();
		}
		index = nodeIndex++; 
	}

	// sort the triangles in leaf node according to surface area
	void sortPrimitives();
			
	void initInterior(int axis, double s) {
		++numNodesMade; // NOBOOK
		split = s;
		flags &= ~3;
		flags |= axis;
		index = nodeIndex++; 
	}

	~emxKdTreeNode() {
		if (IsLeaf() && nPrimitives() > 1) 	delete[] primitives;
	}

	double SplitPos() const { return split; }
	int nPrimitives() const { return nPrims >> 2; }
	int SplitAxis() const { return flags & 3; }
	bool IsLeaf() const { return (flags & 3) == 3; }

	emxBBox WorldBound() const { return bounds; }
	unsigned int RightChildIndex() const { return aboveChild; }

	static int numLeafMade, numNodesMade, maxLeafPrims, leafPrims;
	int index;
private:
	union {
		unsigned int flags;    // Both
		unsigned int  nPrims;  // Leaf
	};
	double split;			   // Interior
	union {
		unsigned int aboveChild;       // Interior(left nodes + 1, right nodes + aboveChild)
		emxMailboxPrim *onePrimitive;  // Leaf
		emxMailboxPrim **primitives;   // Leaf
	};
	emxBBox bounds;			// bounding box of this kdtree node

};

// emxKdTree Declarations
class  emxKdTree {
public:
	// KdTreeAccel Public Methods
	emxKdTree(emxModel* mesh, int icost, int scost, double ebonus, int maxp, int maxDepth);
	~emxKdTree();

	emxBBox WorldBound() const { return bounds; }
	emxKdTreeNode* GetRootNode() const { return nodes; }

	bool Intersect(const emxRay &ray, emxIntersectionInfo &iInfo) const;
	bool Intersect(const emxRay &ray, double& tHit, int& faceID) const;
	bool IntersectP(const emxRay &ray) const;
	bool IntersectPrism(emxRay rays[3], vector<unsigned int>& intersectedFaceList) const;

	bool IntersectBeam(emxBeam *pCurBeam) const;

	// check whether the node or its children nodes contains the face with index = faceIndex
	bool containFace(const emxKdTreeNode* node, size_t faceIndex) const;

	static int nodesTraversed, nRayTest, nRayHit;

protected:
	struct emxBoundEdge {
		// emxBoundEdge Public Methods
		emxBoundEdge() { }
		emxBoundEdge(double tt, int pn, bool starting) {
			t = tt;
			primNum = pn;
			type = starting ? START : END;
		}
		bool operator<(const emxBoundEdge &e) const {
			if (t == e.t)
				return (int)type < (int)e.type;
			else return t < e.t;
		}
		double t;
		int primNum;
		enum { START, END } type;
	};

	struct emxKdToDo {
		const emxKdTreeNode *node;
		double tmin, tmax;
	};

	struct emxKdStackNode {
		const emxKdTreeNode *node;
		size_t nMissStackSize;
		int nBeamSize;
	};

protected:
	void buildTree(int nodeNum, const emxBBox &bounds,
		const vector<emxBBox> &primBounds,
		int *primNums, int nprims, int depth,
		emxBoundEdge *edges[3],
		int *prims0, int *prims1, int badRefines = 0);

	// check if curBeam hits the scene's bounding box, and initialize tmin and tmax
	bool BeamVsAABBox(const emxBeam *curBeam, const emxBBox& bounds, Vector3d tmin[4], Vector3d tmax[4]) const;

	// intersect current beam with split plane
	void IntersectSplitPlane(const emxKdTreeNode* node, const emxBeam* curBeam, double tplane[4]) const;

	// get near far child of current KdTree Node for current beam
	void GetNearFarChild(const emxKdTreeNode* node, emxBeam* curBeam, 
					     const emxKdTreeNode* &firstChild, const emxKdTreeNode* &secondChild) const;

	// get next beam and KDNode from the stack
	void GetNextBeam(vector<emxKdStackNode>& KDStack, vector<emxBeam*>& MissStack, Vector3d tmin[4], 
					 Vector3d tmax[4], emxBeam*& nextBeam, const emxKdTreeNode*& nextKDNode) const;

	// intersect current beam with triangle
	void BeamTriangleIntersection(emxBeam *curBeam, emxMailboxPrim *mp, bool abovezero[3]) const;

	// check trivial cases: -1 full miss, 0 need beam split, 1 full hit
	int CheckTrivialCases(Vector3d beamVertex[4], int beamVertexNum, Vector3d& normal, Vector3d projVertex[4], int projVertexNum) const;

	// split beam
	void SplitBeam(emxBeam *parentBeam, emxMailboxPrim *mp, Vector3d projVertex[4], int projVertexNum) const;
	
	// generate miss beam
	void GenerateBeams(emxBeam *parentBeam, Vector3d beamVertex[8], int beamVertexNum, vector<emxBeam*>& beams) const;

	// check the hit beams' vertices against the old hit beams's plane and update the tmax of corner rays
	void CheckHitBeam(emxBeam *curBeam, emxMailboxPrim *mp, vector<emxBeam*>& splitBeams) const;

	//check the beam and object trianlge  whether exist the intersection area
	bool is_beamTriangle_intersection(emxBeam *curBeam, emxMailboxPrim *mp, bool abovezero[3]) const;

	//// intersect current beam with triangle
	//void BeamTriangleIntersection(emxBeam *curBeam, emxMailboxPrim *mp) const;

	//// check trivial cases: -1 full miss, 0 need beam split, 1 full hit
	//int CheckTrivialCases(Vector3d beamVertex[4], int beamVertexNum, Vector3d& normal, Vector3d projVertex[5], int projVertexNum) const;

	//// split beam
	//void SplitBeam(emxBeam *parentBeam, emxMailboxPrim *mp, Vector3d projVertex[5], int projVertexNum) const;

	//// generate miss beam
	//void GenerateBeams(emxBeam *parentBeam, Vector3d beamVertex[9], int beamVertexNum, vector<emxBeam*>& beams) const;

	//// check the hit beams' vertices against the old hit beams's plane and update the tmax of corner rays
	//void CheckHitBeam(emxBeam *curBeam, emxMailboxPrim *mp, vector<emxBeam*>& splitBeams) const;

private:
	// KdTreeAccel Private Data
	int isectCost, traversalCost, maxPrims;
	double emptyBonus;
	unsigned int nMailboxes;
	emxMailboxPrim *mailboxPrims;
	mutable int curMailboxId;
	emxKdTreeNode *nodes;
	int nAllocedNodes, nextFreeNode;
	emxBBox bounds;
};



#endif