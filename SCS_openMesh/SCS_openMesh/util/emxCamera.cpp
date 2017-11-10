#include "util/emxCamera.h"

template<class T>
inline T Max(T x, T y) { return x>=y ? x :  y; }
template<class T>
inline T Max  (T x, T y, T z) { return Max(Max(x, y), z); }

	emxCamera::emxCamera()
		: m_center(0,0,0), m_right(1,0,0), m_up(0,1,0), m_bbCenter(0,0,0), m_bbScale(1.0), 
		m_zoomDefault(3.0)
	{
		m_distanceToEye =20;
		m_eye = VectorCross(m_right, m_up) * m_distanceToEye + m_center;

		m_zoom = m_zoomDefault;
		m_translateX = 0;
		m_translateY = 0;

		m_pArcBall = new ArcBall();
		m_pArcBall->setViewDefault();
		m_pArcBall->update();
		m_pArcBall->value(m_arcBallMatrix);
	}

	emxCamera::~emxCamera()
	{
		delete m_pArcBall;
		m_pArcBall = NULL;
	}

	/*
	*	set orthographic projection
	*/
	void emxCamera::setOrthoProjection()
	{
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluOrtho2D(0.0, (GLfloat)m_iWinWidth, 0.0, (GLfloat)m_iWinHeight);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
	}

	/*
	*	set perspective projection
	*/
	void emxCamera::setPerspectiveProjection()
	{
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		// perspective transform
		gluPerspective(30.f, m_dAspect, .1, 1000);//60.f
		// viewing transform
		gluLookAt(
			m_eye.x,    m_eye.y,    m_eye.z,
			m_center.x, m_center.y, m_center.z,
			m_up.x,     m_up.y,     m_up.z );
	}

	/*
	* setup model matrix
	*/
	void emxCamera::setupModelMatrix()
	{
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		//glTranslatef(m_translateX, m_translateY, m_translateX);
		glTranslatef(m_translateX, m_translateY, 0.0);

		// zoom
		glScalef(m_zoom, m_zoom, m_zoom);

		// bounding box transformation
		glScalef(m_bbScale, m_bbScale, m_bbScale);

		// arcball transformation
		glMultMatrixd((double*)m_arcBallMatrix);

		
		glTranslatef(-m_bbCenter.x, -m_bbCenter.y, -m_bbCenter.z);
	// 1. translate; 2. rotate; 3. scale
	}

	/*
	*	invoke when left mouse begin drag at position 'x' and 'y'
	*/
	void emxCamera::beginDrag(int x, int y)
	{
		Quat v = ArcBall::arcBallCoord(x, y, m_iWinWidth, m_iWinHeight);
		m_pArcBall->mouse(v);      // sets new mouse coordinates
		m_pArcBall->beginDrag();   // indictes that dragging should begin
		m_pArcBall->update();      // alters the internal state of the arcball
	}

	/*
	*	invoke when left mouse move at position 'x' and 'y'
	*/
	void emxCamera::mouseMove(int x, int y)
	{
		Quat v = ArcBall::arcBallCoord(x, y, m_iWinWidth, m_iWinHeight);
		m_pArcBall->mouse(v);					// set the mouse position
		m_pArcBall->update();				    // alters the internal state of the arcball
		m_pArcBall->value(m_arcBallMatrix);		// reads the matrix from the arcball
	}

	/*
	*	invoke when left mouse end drag at position 'x' and 'y'
	*/
	void emxCamera::endDrag(int x, int y)
	{
		Quat v = ArcBall::arcBallCoord(x, y, m_iWinWidth, m_iWinHeight);
		m_pArcBall->mouse(v);					// sets new mouse coordinates
		m_pArcBall->endDrag();					// indictes that dragging should end
		m_pArcBall->update();					// alters the internal state of the arcball
		m_pArcBall->value(m_arcBallMatrix);		// extracts the current matrix transform
	}

	/*
	*	set window width and height
	*/
	void emxCamera::setWindowSize(int width, int height)
	{
		m_iWinWidth  = width;
		m_iWinHeight = height;
		m_dAspect    = (double)width / Max(height, 1);	
	}

	/*
	*	set scene bounding box
	*/
	void emxCamera::setSceneBBox(const Vector3d& minPos, const Vector3d& maxPos)
	{
		m_bbCenter = (minPos + maxPos) * 0.5;
		m_bbScale = Max(maxPos.x - m_bbCenter.x, m_bbCenter.x - minPos.x);
		m_bbScale = Max(maxPos.y - m_bbCenter.y, m_bbCenter.y - minPos.y, m_bbScale);
		m_bbScale = Max(maxPos.z - m_bbCenter.z, m_bbCenter.z - minPos.z, m_bbScale);
		if(m_bbScale)
			m_bbScale = 1.2 / m_bbScale;
		else
			m_bbScale = 1.0;
	}

	/*
	*	set view as xy plane
	*/
	void emxCamera::setViewXY()
	{
		m_pArcBall->setViewXY();
		m_pArcBall->update();
		m_pArcBall->value(m_arcBallMatrix);
	}

	/*
	*	set view as xz plane
	*/
	void emxCamera::setViewXZ()
	{
		m_pArcBall->setViewXZ();
		m_pArcBall->update();
		m_pArcBall->value(m_arcBallMatrix);
	}

	/*
	*	set view yz plane
	*/
	void emxCamera::setViewYZ()
	{
		m_pArcBall->setViewYZ();
		m_pArcBall->update();
		m_pArcBall->value(m_arcBallMatrix);
	}

	/*
	*	set view default
	*/
	void emxCamera::setViewDefault()
	{
		m_pArcBall->setViewDefault();
		m_pArcBall->update();
		m_pArcBall->value(m_arcBallMatrix);
		m_zoom = m_zoomDefault;
	}

	void emxCamera::setModelTranslate(int x, int y)
	{
		double scaleX = (m_iWinWidth>m_iWinHeight) ? ((double)m_iWinWidth/m_iWinHeight) : 1;
		double scaleY = (m_iWinHeight>m_iWinWidth) ? ((double)m_iWinHeight/m_iWinWidth) : 1;

		m_translateX += (double)x/m_iWinWidth * 10* scaleX;
		m_translateY -= (double)y/m_iWinHeight * 10* scaleY;
	}