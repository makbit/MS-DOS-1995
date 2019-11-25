#ifndef UNIT3DO_H__
#define UNIT3DO_H__

///////////////////////////////////////////////////////////////////////////////
class Unit3DO
{
	typedef struct tagObject
	{
	    long VersionSignature;
    	long NumberOfVertexes;
	    long NumberOfPrimitives;
    	long UnknownFlag;
    	union
    	{
	    	long  nXFromParent;
	    	float fXFromParent;
	    };
    	union
    	{
	    	long  nYFromParent;
	    	float fYFromParent;
	    };
    	union
    	{
	    	long  nZFromParent;
	    	float fZFromParent;
	    };
    	long OffsetToObjectName;
	    long Always_0;
    	long OffsetToVertexArray;
	    long OffsetToPrimitiveArray;
    	long OffsetToSiblingObject;
	    long OffsetToChildObject;
	} Object;
	typedef struct tagPrimitive
	{
	    long Unknown_0;
    	long NumberOfVertexIndexes;
	    long Always_0;
    	long OffsetToVertexIndexArray;
	    long OffsetToTextureName;
    	long Unknown_1;
	    long Unknown_2;
    	long Unknown_3;
	} Primitive;
	class Unit3DObject
	{
		Unit3DObject *pSibling;   // next sibling (if any)
		Unit3DObject *pChild;     // list of child-objects (if any)
		char         *pFileData;  // ptr to file in memory
		Object       *pObject;    // object info
	public:
		Unit3DObject( char *pData, Object *pObj );
		~Unit3DObject();
		void Render( void *pD3DDevice7 );
	};
	//---------------------------------------------------------------
	char         *pFileData;
	Unit3DObject *pBase;
public:
	Unit3DO( const char *szFilename );
	~Unit3DO()                { delete [] pFileData; delete pBase; }
	void Render( void *pDev ) { pBase->Render(pDev); }
};
///////////////////////////////////////////////////////////////////////////////

// Add Ultra-Fast!!! operator new/delete for Unit3DObject.
// Textures??? - globals ???
//	void Render( void *pD3DDevice7, D3DVECTOR *pvLoc, void *pTextures );
#endif // end of file