/*
Open Asset Import Library (ASSIMP)
----------------------------------------------------------------------

Copyright (c) 2006-2008, ASSIMP Development Team
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

* Neither the name of the ASSIMP team, nor the names of its
  contributors may be used to endorse or promote products
  derived from this software without specific prior
  written permission of the ASSIMP Development Team.

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

/** @file Definition of the .ASE importer class. */
#ifndef AI_ASELOADER_H_INCLUDED
#define AI_ASELOADER_H_INCLUDED

#include "BaseImporter.h"
#include "../include/aiTypes.h"

struct aiNode;
#include "ASEParser.h"

namespace Assimp
{
class MaterialHelper;

using namespace ASE;

// ---------------------------------------------------------------------------
/** Used to load ASE files
*/
class ASEImporter : public BaseImporter
{
	friend class Importer;

protected:
	/** Constructor to be privately used by Importer */
	ASEImporter();

	/** Destructor, private as well */
	~ASEImporter();

public:

	// -------------------------------------------------------------------
	/** Returns whether the class can handle the format of the given file. 
	* See BaseImporter::CanRead() for details.	*/
	bool CanRead( const std::string& pFile, IOSystem* pIOHandler) const;

protected:

	// -------------------------------------------------------------------
	/** Called by Importer::GetExtensionList() for each loaded importer.
	 * See BaseImporter::GetExtensionList() for details
	 */
	void GetExtensionList(std::string& append)
	{
		append.append("*.ase;*.ask");
	}

	// -------------------------------------------------------------------
	/** Imports the given file into the given scene structure. 
	* See BaseImporter::InternReadFile() for details
	*/
	void InternReadFile( const std::string& pFile, aiScene* pScene,
		IOSystem* pIOHandler);

	// -------------------------------------------------------------------
	/** Generate normal vectors basing on smoothing groups
	 * (in some cases the normal are already contained in the file)
	 * \param mesh Mesh to work on
	 */
	void GenerateNormals(ASE::Mesh& mesh);

	// -------------------------------------------------------------------
	/** Create valid vertex/normal/UV/color/face lists.
	 *  All elements are unique, faces have only one set of indices
	 *  after this step occurs.
	 * \param mesh Mesh to work on
	 */
	void BuildUniqueRepresentation(ASE::Mesh& mesh);

	// -------------------------------------------------------------------
	/** Create one-material-per-mesh meshes ;-)
	 * \param mesh Mesh to work with
	 *  \param pcScene Scene object to be filled
	 */
	void ConvertMeshes(ASE::Mesh& mesh, aiScene* pcScene);

	// -------------------------------------------------------------------
	/** Convert a material to a MaterialHelper object
	 * \param mat Input material
	 */
	void ConvertMaterial(ASE::Material& mat);

	// -------------------------------------------------------------------
	/** Setup the final material indices for each mesh
	 *  \param pcScene Scene object to be filled
	 */
	void BuildMaterialIndices(aiScene* pcScene);

	// -------------------------------------------------------------------
	/** Build the node graph
	 *  \param pcScene Scene object to be filled
	 */
	void BuildNodes(aiScene* pcScene);

protected:

	/** Parser instance */
	ASE::Parser* mParser;

	/** Buffer to hold the loaded file */
	unsigned char* mBuffer;
};

} // end of namespace Assimp

#endif // AI_3DSIMPORTER_H_INC