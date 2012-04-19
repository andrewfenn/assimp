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

/** @file OGREExporter.h
 * Declares the exporter class to write a scene to an Ogre XML file
 */
#ifndef AI_OGREEXPORTER_H_INC
#define AI_OGREEXPORTER_H_INC

#include <sstream>

struct aiScene;
struct aiNode;

namespace Assimp	
{

// ------------------------------------------------------------------------------------------------
/** Helper class to export a given scene to a Ogre XML file. */
// ------------------------------------------------------------------------------------------------
class OgreExporter
{
public:
	/// Constructor for a specific scene to export
	OgreExporter(const char* filename, const aiScene* pScene);

public:

	/// public stringstreams to write all output into
	std::ostringstream mOutput;

private:

	void WriteMesh(const aiMesh* m, const aiScene* p);
	
	void WriteSubMesh(const aiMesh* m, const aiScene* p);
    void WriteFaces(const aiMesh* m);
    void WriteVertexBuffer(const aiMesh* m, const aiScene* p);
    void WriteVertices(const aiMesh* m, const aiScene* p);
    void WriteTexCoords(const aiMesh* m, const aiScene* p);

    void tab(int num);
    
    void Tag(const aiString name, const aiVector3D value);
    void TagTex(const aiString name, const aiVector3D value, const unsigned int numUV);
    void Tag(const aiString name, const aiColor4D value);
    
    void Attribute(const aiString name, const unsigned int value);
    void Attribute(const aiString name, const aiString value);
    void Attribute(const aiString name, const bool value);

	const std::string filename;
	const aiScene* const pScene;

	// this endl() doesn't flush() the stream
	const std::string endl;
	
    aiColor4D mSpecular;
    aiColor4D mDiffuse;
    bool mHasDiffuse;
    bool mHasSpecular;
};

}

#endif
