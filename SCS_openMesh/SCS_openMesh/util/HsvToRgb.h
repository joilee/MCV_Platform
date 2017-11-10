#ifndef HSVTORGB_H_
#define HSVTORGB_H_ 

typedef struct {
	double r;       // percent
	double g;       // percent
	double b;       // percent
} RgbColor;

typedef struct {
	double h;       // angle in degrees
	double s;       // percent
	double v;       // percent
} HsvColor;

RgbColor HsvToRgb(HsvColor hsv);
HsvColor RgbToHsv(RgbColor rgb);

#endif