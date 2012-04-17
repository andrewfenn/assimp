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

    if (pScene->HasMeshes())
    {
        tab(1); mOutput << "<submeshes>" << endl;
	    for(unsigned int i = 0; i < pScene->mNumMeshes; ++i) {
		    WriteSubMesh(pScene->mMeshes[i], pScene);
	    }
        tab(1); mOutput << "</submeshes>" << endl;
    }

    mOutput << "</mesh>" << endl;
}

void OgreExporter::tab(int num) {
    if (num < 1)
        return;

    mOutput << "    ";
    tab(--num);
}

void OgreExporter :: Tag(const aiString name, const aiVector3D value)
{
    mOutput << "<" << name.C_Str() << " ";
    mOutput << "x=\"" << value.x << "\" ";
    mOutput << "y=\"" << value.y << "\" ";
    mOutput << "z=\"" << value.z << "\" />";
}

void OgreExporter :: TagTex(const aiString name, const aiVector3D value)
{
    mOutput << "<" << name.C_Str() << " ";
    mOutput << "u=\"" << value.x << "\" ";
    mOutput << "v=\"" << value.y << "\" ";
    mOutput << "w=\"" << value.z << "\" />";
}

void OgreExporter :: Tag(const aiString name, const aiColor4D value)
{
    mOutput << "<" << name.C_Str() << " ";
    mOutput << "value=\"" << value.r << " " << value.g << " " << value.b << " " << value.a << "\" />";
}

void OgreExporter :: Attribute(const aiString name, const unsigned int value)
{
    mOutput << " " << name.C_Str() << "=\"" << value << "\"";
}

void OgreExporter :: Attribute(const aiString name, const bool value)
{
    mOutput << " " << name.C_Str() << "=\"";
    if (value) {
        mOutput << "true";
    } else {
        mOutput << "false";
    }
    mOutput << "\"";
}
void OgreExporter :: Attribute(const aiString name, const aiString value)
{
    mOutput << " " << name.C_Str() << "=\"" << value.C_Str() << "\"";
}
// ------------------------------------------------------------------------------------------------
void OgreExporter :: WriteSubMesh(const aiMesh* m, const aiScene* p)
{
    tab(2); mOutput << "<submesh";

    if (p->HasMaterials() && p->mNumMaterials != 0)
    {
        aiString sName;
        aiGetMaterialString(p->mMaterials[m->mMaterialIndex],AI_MATKEY_NAME,&sName);
        Attribute(aiString("material"), sName);
    }

    // TODO: add sharedvertices support
    Attribute(aiString("usesharedvertices"), false);

    // Check number of indexes is over 65535 set use32bitindexes to true
    Attribute(aiString("use32bitindexes"), (m->mFaces->mNumIndices > 65535));

    if (m->mPrimitiveTypes == aiPrimitiveType_TRIANGLE) {
        Attribute(aiString("operationtype"), aiString("triangle_list"));
    } else if (m->mPrimitiveTypes == aiPrimitiveType_LINE) {
        Attribute(aiString("operationtype"), aiString("triangle_strip"));
    } else {
        // aiPrimitiveType_POINT
        Attribute(aiString("operationtype"), aiString("triangle_fan"));
    }
    mOutput << ">" << endl;

    if (m->HasFaces())
        WriteFaces(m);

    tab(3); mOutput << "<geometry vertexcount=\"" << m->mNumVertices << "\">" << endl;
    
    WriteVertexBuffer(m,p);
    
    tab(3); mOutput << "</geometry>" << endl;
    tab(2); mOutput << "</submesh>" << endl;
}
// ------------------------------------------------------------------------------------------------
void OgreExporter :: WriteVertexBuffer(const aiMesh* m, const aiScene* p)
{
    tab(4); mOutput << "<vertexbuffer";
    
    Attribute(aiString("positions"), m->HasPositions());
    Attribute(aiString("normals"), m->HasNormals());

  //  mOutput << "colours_diffuse=\"";  
  	aiMaterial* material;
  	material = p->mMaterials[m->mMaterialIndex];
  	
    aiColor4D diffuse;

    mHasDiffuse = false;
    if (AI_SUCCESS == aiGetMaterialColor(material, AI_MATKEY_COLOR_DIFFUSE, &diffuse)) {
        mHasDiffuse = true;
    }
    Attribute(aiString("colours_diffuse"), mHasDiffuse);

    aiColor4D specular;

    mHasSpecular = false;
    if (AI_SUCCESS == aiGetMaterialColor(material, AI_MATKEY_COLOR_SPECULAR, &specular)) {
        mHasSpecular = true;
    }
    Attribute(aiString("colours_specular"), mHasSpecular);

    unsigned int total_texcoords;
    for(total_texcoords = 0; m->HasTextureCoords(total_texcoords); ++total_texcoords)
    {
        Attribute(aiString("texture_coord_dimensions_"+boost::lexical_cast<std::string>(total_texcoords)),
                  aiString("float"+boost::lexical_cast<std::string> (m->mNumUVComponents[total_texcoords])) );
    }
    Attribute(aiString("texture_coords"), total_texcoords);

    if (m->HasTangentsAndBitangents())
    {
        Attribute(aiString("tangents"), true);
        Attribute(aiString("tangent_dimensions"), 3u);
    }
    Attribute(aiString("binormals"), m->HasNormals());

    mOutput << ">" << endl;

    WriteVertices(m,p);

    tab(4); mOutput << "</vertexbuffer>" << endl;
}
// ------------------------------------------------------------------------------------------------
void OgreExporter :: WriteVertices(const aiMesh* m, const aiScene* p)
{
    for(unsigned int i = 0; i < m->mNumVertices; ++i) {
        
        tab(5); mOutput << "<vertex>" << endl;
        if (m->HasPositions()) { 
            tab(6); Tag(aiString("position"), m->mVertices[i]); mOutput << endl;
        }
        if (m->HasNormals()) { 
            tab(6); Tag(aiString("normal"), m->mNormals[i]); mOutput << endl;
        }
        if (m->HasTangentsAndBitangents())
        {
            tab(6); Tag(aiString("tangent"), m->mTangents[i]); mOutput << endl;
            tab(6); Tag(aiString("binormal"), m->mBitangents[i]); mOutput << endl;
        }
/*        if (mHasDiffuse)
        {
            tab(6); Tag(aiString("colour_diffuse"), m->mColors[m->mMaterialIndex][i]); mOutput << endl;
        }
        tab(5); mOutput << "</vertex>" << endl;
        if (mHasSpecular)
        {
            tab(6); Tag(aiString("colour_specular"), m->mColors[m->mMaterialIndex][i]); mOutput << endl;
        }
        tab(5); mOutput << "</vertex>" << endl;
        
*/
        if (m->HasTangentsAndBitangents())
        {
            tab(6); Tag(aiString("tangent"), m->mTangents[i]); mOutput << endl;
            tab(6); Tag(aiString("binormal"), m->mBitangents[i]); mOutput << endl;
        }
        for(unsigned int tex= 0; m->HasTextureCoords(tex); ++tex)
        {
            tab(6); TagTex(aiString("texcoord"), m->mTextureCoords[tex][i]); mOutput << endl;
        }
    }
}
// ------------------------------------------------------------------------------------------------
void OgreExporter :: WriteFaces(const aiMesh* m)
{
    tab(3); mOutput << "<faces count=\"" << m->mNumFaces << "\">" << endl;   
	for (unsigned int i = 0; i < m->mNumFaces; ++i) {
		const aiFace& f = m->mFaces[i];
		
		tab(4); mOutput << "<face ";
		for(unsigned int a = 0; a < f.mNumIndices; ++a) {
		    mOutput << "v"<< a+1 <<"=\"" << f.mIndices[a] << "\" ";
	    }
	    mOutput << "/>" << endl;
    }
    tab(3); mOutput << "</faces>" << endl;
}
// ------------------------------------------------------------------------------------------------
void OgreExporter :: WriteMesh(const aiMesh* m, const aiScene* p)
{
/*
    switch (m->mNumFaces)
    {
        case 1:
            
    }*/

    

    

    // Vertex data
    
    mOutput << "\t\t\t<geometry vertexcount=\"" << m->mNumVertices << "\">" << endl;
	for (unsigned int i = 0; i < m->mNumFaces; ++i) {
	/*
                <vertexbuffer positions="true" normals="true" texture_coord_dimensions_0="float2" texture_coords="1">
                    <vertex>
                        <position x="-0.146724" y="-0.159533" z="-0.140746" />
                        <normal x="-0.981293" y="-0.191754" z="0.017182" />
                        <texcoord u="0.121712" v="0.87139" />
                    </vertex>*/
    }


/*		mOutput << "  outer loop" << endl; 
		for(unsigned int a = 0; a < f.mNumIndices; ++a) {
			const aiVector3D& v  = m->mVertices[f.mIndices[a]];
			mOutput << "  vertex " << v.x << " " << v.y << " " << v.z << endl;
		}

		mOutput << "  endloop" << endl; 
		mOutput << " endfacet" << endl << endl; 
	}*/

    
}

#endif
