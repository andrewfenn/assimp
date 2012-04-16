/*
Open Asset Import Library (assimp)
----------------------------------------------------------------------

Copyright (c) 2006-2012, assimp team
All rights reserved.

Redistribution and use of this software in source and binary forms, 
with or without modification, are permitted provided that the 
following conditions are met:

* Redistributions of source code must retain the above
  copyright notice, this list of conditions and the
  following disclaimer.

* Redistributions in binary form must reproduce the above
  copyright notice, this list of conditions and the
  following disclaimer in the documentation and/or other
  materials provided with the distribution.

* Neither the name of the assimp team, nor the names of its
  contributors may be used to endorse or promote products
  derived from this software without specific prior
  written permission of the assimp team.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS 
"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT 
LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT 
OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT 
LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY 
THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT 
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE 
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

----------------------------------------------------------------------
*/

#include "AssimpPCH.h"

#if !defined(ASSIMP_BUILD_NO_EXPORT) && !defined(ASSIMP_BUILD_NO_OGRE_EXPORTER)

#include "OgreExporter.h"
#include "../include/assimp/version.h"

using namespace Assimp;
namespace Assimp	{

// ------------------------------------------------------------------------------------------------
// Worker function for exporting a scene to Ogre xml. Prototyped and registered in Exporter.cpp
void ExportSceneOgre(const char* pFile,IOSystem* pIOSystem, const aiScene* pScene)
{
	// invoke the exporter 
	OgreExporter exporter(pFile, pScene);

	// we're still here - export successfully completed. Write the file.
	boost::scoped_ptr<IOStream> outfile (pIOSystem->Open(pFile,"wt"));
	outfile->Write( exporter.mOutput.str().c_str(), static_cast<size_t>(exporter.mOutput.tellp()),1);
}

} // end of namespace Assimp


// ------------------------------------------------------------------------------------------------
OgreExporter :: OgreExporter(const char* _filename, const aiScene* pScene)
: filename(_filename)
, pScene(pScene)
, endl("\n") 
{
	// make sure that all formatting happens using the standard, C locale and not the user's current locale
	const std::locale& l = std::locale("C");
	mOutput.imbue(l);

    mOutput << "<mesh>" << endl;
    mOutput << "\t<submeshes>" << endl;
	for(unsigned int i = 0; i < pScene->mNumMeshes; ++i) {
		WriteMesh(pScene->mMeshes[i]);
	}
    mOutput << "\t</submeshes>";
    mOutput << "</mesh>" << endl;
}

// ------------------------------------------------------------------------------------------------
void OgreExporter :: WriteMesh(const aiMesh* m, const aiScene* p)
{

    switch (m->mNumFaces)
    {
        case 1:
            
    }

    mOutput << "\t\t<submesh material=\"\" usesharedvertices=\"true\" use32bitindexes=\"false\" operationtype=\"triangle_list\">" << endl;

    // Vertice Indices

    mOutput << "\t\t\t<faces count=\"" << m->mNumFaces << "\">" << endl;   
	for (unsigned int i = 0; i < m->mNumFaces; ++i) {
		const aiFace& f = m->mFaces[i];
		
		mOutput << "\t\t\t\t<face ";
		for(unsigned int a = 0; a < f.mNumIndices; ++a) {
		    mOutput << "v"<< a+1 <<"=\"" << f.mIndices[a] << "\" ";
	    }
	    mOutput << "/>" << endl;
    }
    mOutput << "\t\t\t</faces>" << endl;

    // Vertex data
    
    mOutput << "\t\t\t<geometry vertexcount=\"" << m->mNumVertices << "\">" << endl;
	for (unsigned int i = 0; i < m->mNumFaces; ++i) {
                <vertexbuffer positions="true" normals="true" texture_coord_dimensions_0="float2" texture_coords="1">
                    <vertex>
                        <position x="-0.146724" y="-0.159533" z="-0.140746" />
                        <normal x="-0.981293" y="-0.191754" z="0.017182" />
                        <texcoord u="0.121712" v="0.87139" />
                    </vertex>
    }


/*		mOutput << "  outer loop" << endl; 
		for(unsigned int a = 0; a < f.mNumIndices; ++a) {
			const aiVector3D& v  = m->mVertices[f.mIndices[a]];
			mOutput << "  vertex " << v.x << " " << v.y << " " << v.z << endl;
		}

		mOutput << "  endloop" << endl; 
		mOutput << " endfacet" << endl << endl; 
	}*/

    mOutput << "\t\t</submesh>" << endl;
}

#endif
