#ifndef _INTERACTIVE_STL_H_
#define _INTERACTIVE_STL_H_
#include <vector>
/*just for interactive*/
namespace GeometryInteractive{
class Interactive{
	public:
		Interactive()
		{
		}

		~Interactive()
        {
		}

public:
	void vecMulmat(const GLfloat *mat,std::vector<GLfloat> &vec)
	{
		/*according to the matrix and vertex ,calculate the target vertex*/
		/*vertex multiply matrix*/
		std::vector<GLfloat> backup(vec);
		vec[0]=mat[0]*backup[0]+mat[4]*backup[1]+mat[8]*backup[2]+mat[12];
		vec[1]=mat[1]*backup[0]+mat[5]*backup[1]+mat[9]*backup[2]+mat[13];
		vec[2]=mat[2]*backup[0]+mat[6]*backup[1]+mat[10]*backup[2]+mat[14];
	}

};
}
#endif