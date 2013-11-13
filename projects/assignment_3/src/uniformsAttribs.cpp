//uniformsAttribs.cpp

#include "uniformsAttribs.h"


namespace VertexAttribs {
	namespace Names {
		const std::string POSITION = "in_Position";
		//const std::string COLOR = "in_Color";
		const std::string NORMAL = "in_Normal";
		const std::string TEXCOORD = "in_TexCoord";
	}
	namespace Indexes {
		const GLuint POSITION = 0;
		//const GLuint COLOR = 1;
		const GLuint NORMAL = 2;
		const GLuint TEXCOORD = 3;
	}
}

namespace Uniforms {
	const std::string MATRIX = "Matrix";
	const std::string COLOR = "Color";
}

namespace UniformBlocks {
	namespace Names {
		const std::string SHARED_MATRICES = "SharedMatrices";
	}
	namespace BindPoints {
		const GLuint SHARED_MATRICES = 0;
	}
}
