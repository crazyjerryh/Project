#ifndef _COLOR_STL_H_
#define _COLOR_STL_H_
namespace RenderAlgorithm{
	typedef struct color_tag{
		int r,g,b;
		struct color_tag(int _r,int _g,int _b):r(_r),g(_g),b(_b){}
		struct color_tag(){}
	}color;
}
#endif