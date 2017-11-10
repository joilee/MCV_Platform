#ifndef __COLOR_H__
#define __COLOR_H__

//#include "util/apibase.h"

//namespace emxUtility {

	class  Color
	{
	public:
		static const Color WHITE;
		static const Color GRAY;
		static const Color BLACK;
		static const Color RED;
		static const Color YELLOW;
		static const Color GREEN;
		static const Color CYAN;
		static const Color BLUE;
		static const Color SKY;
		static const Color PURPLE;
		static const Color LIGHT_GRAY;
		static const Color LIGHT_RED;
		static const Color LIGHT_YELLOW;
		static const Color LIGHT_GREEN;
		static const Color LIGHT_CYAN;
		static const Color LIGHT_BLUE;
		static const Color LIGHT_SKY;
		static const Color LIGHT_PURPLE;
		static const Color DARK_GRAY;
		static const Color DARK_RED;
		static const Color DARK_YELLOW;
		static const Color DARK_GREEN;
		static const Color DARK_CYAN;
		static const Color DARK_BLUE;
		static const Color DARK_PURPLE;

		// create a color
		Color (double r_, double g_, double b_);
		Color (float r_=0, float g_=0, float b_=0);
		Color(int r_, int g_, int b_);

		// mix colors
		Color  operator +  (const Color &rhs) const;
		Color& operator += (const Color &rhs);

		// scale colors
		Color  operator *  (float s) const;
		Color& operator *= (float s);

		bool operator == (const Color &rhs) const;
		bool operator != (const Color &rhs) const;

		// convert hsv color space to rgb color space
		//static Color hsv(float h, float s, float v);

		// rgb components
		float r;
		float g;
		float b;
	};

//};// namespace emxUtility
#endif