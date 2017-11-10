#include "util/Color.h"
//#include "emxUtilityInc.h"

//namespace emxUtility
//{

	const Color Color::WHITE(1.f, 1.f, 1.f);
	const Color Color::GRAY (.5f, .5f, .5f);
	const Color Color::BLACK(0.f, 0.f, 0.f);

	const Color Color::RED    ( 1.f, 0.f, 0.f);
	const Color Color::YELLOW ( 1.f, 1.f, 0.f);
	const Color Color::GREEN  ( 0.f, 1.f, 0.f);
	const Color Color::CYAN   ( 0.f, 1.f, 1.f); //ÇàÉ«
	const Color Color::BLUE   ( 0.f, 0.f, 1.f);
	const Color Color::PURPLE ( 1.f, 0.f, 1.f);
	const Color Color::SKY    ( 74, 74, 210);//( 74, 74, 210) (255,204,102)

	const Color Color::LIGHT_GRAY   = Color::WHITE + Color::GRAY;
	const Color Color::LIGHT_RED    = Color::WHITE + Color::RED;
	const Color Color::LIGHT_YELLOW = Color::WHITE + Color::YELLOW;
	const Color Color::LIGHT_GREEN  = Color::WHITE + Color::GREEN;
	const Color Color::LIGHT_CYAN   = Color::WHITE + Color::CYAN;
	const Color Color::LIGHT_BLUE   = Color::WHITE + Color::BLUE;
	const Color Color::LIGHT_SKY   = Color::WHITE + Color::SKY;
	const Color Color::LIGHT_PURPLE = Color::WHITE + Color::PURPLE;

	const Color Color::DARK_GRAY   = Color::BLACK + Color::GRAY;
	const Color Color::DARK_RED    = Color::BLACK + Color::RED;
	const Color Color::DARK_YELLOW = Color::BLACK + Color::YELLOW;
	const Color Color::DARK_GREEN  = Color::BLACK + Color::GREEN;
	const Color Color::DARK_CYAN   = Color::BLACK + Color::CYAN;
	const Color Color::DARK_BLUE   = Color::BLACK + Color::BLUE;
	const Color Color::DARK_PURPLE = Color::BLACK + Color::PURPLE;

	Color::Color( double r_, double g_, double b_ ) : r((float)r_), g((float)g_), b((float)b_)
	{

	}

	Color::Color( float r_/*=0*/, float g_/*=0*/, float b_/*=0*/ ) : r(r_), g(g_), b(b_)
	{

	}

	Color::Color( int r_, int g_, int b_ )
	{
		r = r_ / 256.f;
		g = g_ / 256.f;
		b = b_ / 256.f;
	}

	Color Color::operator+( const Color &rhs ) const
	{
		return Color((r+rhs.r)*.5f, (g+rhs.g)*.5f, (b+rhs.b)*.5f);
	}

	Color& Color::operator+=( const Color &rhs )
	{
		r+=rhs.r; g+=rhs.g; b+=rhs.b; return (*this)*=.5f;
	}

	Color Color::operator*( float s ) const
	{
		return Color(r*s, g*s, b*s);
	}

	Color& Color::operator*=( float s )
	{
		r *= s; g *= s; b *= s; return *this;
	}

	bool Color::operator==( const Color &rhs ) const
	{
		return (r == rhs.r) && (g == rhs.g) && (b == rhs.b);
	}

	bool Color::operator!=( const Color &rhs ) const
	{
		return (r != rhs.r) || (g != rhs.g) || (b != rhs.b);
	}