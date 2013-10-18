
#ifndef __RENDERINFO_H__
#define __RENDERINFO_H__

namespace VertexAttribIndexes {
	const int POSITION = 0;
	const int COLOR = 1;
}

typedef struct {
	float xyzw[4];
	float rgba[4];
} Vertex;


#endif