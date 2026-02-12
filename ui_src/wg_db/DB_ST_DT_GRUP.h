#ifndef __DB_ST_DT_GRUP_H__
#define __DB_ST_DT_GRUP_H__

#define MIDAS_SDK

#pragma pack(push, 1)

/// grup ///
#define T_GRUP_K unsigned int
struct T_GRUP_D_790
{
	CString GroupName;
	CArray<T_NODE_K, T_NODE_K>arKeyNode;
	CArray<T_ELEM_K, T_ELEM_K>arKeyElem;
	unsigned int nGroupId;
	void Initialize()
	{
		GroupName.Empty();
		arKeyNode.RemoveAll();
		arKeyElem.RemoveAll();
		nGroupId=0;
	}  
	T_GRUP_D_790()
	{
		Initialize();
	}
	T_GRUP_D_790(T_GRUP_D_790& src)
	{
		*this = src;
	}
	T_GRUP_D_790& operator=(T_GRUP_D_790& src)
	{
		GroupName=src.GroupName;
		arKeyNode.Copy(src.arKeyNode);
		arKeyElem.Copy(src.arKeyElem);
		nGroupId=src.nGroupId;
		return *this;
	}
};
struct T_GRUP_D
{
	CString GroupName;
	CArray<T_NODE_K, T_NODE_K>arKeyNode;
	CArray<T_ELEM_K, T_ELEM_K>arKeyElem;
	unsigned int nGroupId;
	int nPlaneType; //0=None, 1=Y-Z, 2=X-Z, 3=X-Y
	void Initialize()
	{
		GroupName.Empty();
		arKeyNode.RemoveAll();
		arKeyElem.RemoveAll();
		nGroupId=0;
		nPlaneType=0;
	}
	void Convert790(T_GRUP_D_790& rData)
	{
		GroupName= rData.GroupName;
		arKeyNode.Copy(rData.arKeyNode);
		arKeyElem.Copy(rData.arKeyElem);
		nGroupId=rData.nGroupId;
		nPlaneType=0;    
	}
	T_GRUP_D()
	{
		Initialize();
	}
	T_GRUP_D(const T_GRUP_D& src)
	{
		*this = src;
	}
	T_GRUP_D& operator=(const T_GRUP_D& src)
	{
		GroupName=src.GroupName;
		arKeyNode.Copy(src.arKeyNode);
		arKeyElem.Copy(src.arKeyElem);
		nGroupId=src.nGroupId;
		nPlaneType=src.nPlaneType;
		return *this;
	}
};

struct T_GRUP_D_CH_790
{
	char GroupName[80];
	CArray<T_NODE_K, T_NODE_K>arKeyNode;
	CArray<T_ELEM_K, T_ELEM_K>arKeyElem;
	unsigned int nGroupId;
	void ConvertToString(T_GRUP_D_790& rData)
	{
		ConvertCharStr(GroupName, rData.GroupName, sizeof(GroupName));
		rData.arKeyNode.Copy(arKeyNode);
		rData.arKeyElem.Copy(arKeyElem);
		rData.nGroupId=nGroupId;
	}
	void ConvertToChar(T_GRUP_D_790& rData)
	{
		ConvertStrChar(rData.GroupName, GroupName, sizeof(GroupName));
		arKeyNode.Copy(rData.arKeyNode);
		arKeyElem.Copy(rData.arKeyElem);
		nGroupId=rData.nGroupId;
	}
};
struct T_GRUP_D_CH
{
	char GroupName[80];
	CArray<T_NODE_K, T_NODE_K>arKeyNode;
	CArray<T_ELEM_K, T_ELEM_K>arKeyElem;
	unsigned int nGroupId;
	int nPlaneType;
	void ConvertToString(T_GRUP_D& rData)
	{
		ConvertCharStr(GroupName, rData.GroupName, sizeof(GroupName));
		rData.arKeyNode.Copy(arKeyNode);
		rData.arKeyElem.Copy(arKeyElem);
		rData.nGroupId=nGroupId;
		rData.nPlaneType=nPlaneType;
	}
	void ConvertToChar(T_GRUP_D& rData)
	{
		ConvertStrChar(rData.GroupName, GroupName, sizeof(GroupName));
		arKeyNode.Copy(rData.arKeyNode);
		arKeyElem.Copy(rData.arKeyElem);
		nGroupId=rData.nGroupId;
		nPlaneType=rData.nPlaneType;
	}
	void Convert790(T_GRUP_D_CH_790& rData)
	{
		memcpy(GroupName, rData.GroupName, sizeof(GroupName));
		arKeyNode.Copy(rData.arKeyNode);
		arKeyElem.Copy(rData.arKeyElem);    
		nGroupId=rData.nGroupId;
		nPlaneType=0;
	}
};
struct T_GRUP_UDRD_D_790
{
	T_GRUP_K key;
	T_GRUP_D_790 data;
};
struct T_GRUP_UDRD_D
{
	T_GRUP_K key;
	T_GRUP_D data;
};
struct T_GRUP_ITEM_UDRD_D
{
	T_GRUP_K key;
	T_NODE_K nodekey;
	T_ELEM_K elemkey;
};
struct T_GRUP_UDRD_D_CH_790
{
	T_GRUP_K key;
	T_GRUP_D_CH_790 data;
};
struct T_GRUP_UDRD_D_CH
{
	T_GRUP_K key;
	T_GRUP_D_CH data;
};
struct T_GRUP_UDRD_D_CH_RW_790
{
	T_GRUP_K key;
	char GroupName[80];
	unsigned int nGroupId;
	int nCountNodeKey;
	int nCountElemKey;
	void GetGrup(T_GRUP_K& rKey, T_GRUP_D_CH_790& rData)
	{
		rKey=key;
		memcpy(rData.GroupName, GroupName, sizeof(GroupName));
		rData.nGroupId=nGroupId;
	}
	void SetGrup(const T_GRUP_K Key, const T_GRUP_D_CH_790& rData)
	{
		key=Key;
		memcpy(GroupName, rData.GroupName, sizeof(GroupName));
		nGroupId=rData.nGroupId;
		nCountNodeKey=(int)rData.arKeyNode.GetSize();
		nCountElemKey=(int)rData.arKeyElem.GetSize();
	}
};
struct T_GRUP_UDRD_D_CH_RW
{
	T_GRUP_K key;
	char GroupName[80];
	unsigned int nGroupId;
	int nCountNodeKey;
	int nCountElemKey;
	int nPlaneType;
	void GetGrup(T_GRUP_K& rKey, T_GRUP_D_CH& rData)
	{
		rKey=key;
		memcpy(rData.GroupName, GroupName, sizeof(GroupName));
		rData.nGroupId=nGroupId;
		rData.nPlaneType=nPlaneType;
	}
	void SetGrup(const T_GRUP_K Key, const T_GRUP_D_CH& rData)
	{
		key=Key;
		memcpy(GroupName, rData.GroupName, sizeof(GroupName));
		nGroupId=rData.nGroupId;    
		nCountNodeKey=(int)rData.arKeyNode.GetSize();
		nCountElemKey=(int)rData.arKeyElem.GetSize();
		nPlaneType=rData.nPlaneType;
	}
	void Convert790(T_GRUP_UDRD_D_CH_RW_790& rData)
	{
		key=rData.key;
		memcpy(GroupName, rData.GroupName, sizeof(GroupName));
		nGroupId=rData.nGroupId;
		nCountNodeKey=rData.nCountNodeKey;
		nCountElemKey=rData.nCountElemKey;
		nPlaneType=0;    
	}
};
struct T_GRUP_UNIT
{
	T_GRUP_UNIT()
	{
		GroupName=D_UNITSYS_NONE;
		arKeyNode=D_UNITSYS_NONE;
		arKeyElem=D_UNITSYS_NONE;
		nGroupId=D_UNITSYS_NONE;
		nPlaneType=D_UNITSYS_NONE;
	}
	int GroupName;
	int arKeyNode;
	int arKeyElem;
	int nGroupId;
	int nPlaneType;
};
#define HASHSIZEGRUP 101
#define HASHSIZEGRUPITEM HASHSIZEELEM
/*
#define T_GRUP_K unsigned int
struct T_GRUP_D
{
	CString GroupName;
	T_GRUP_K ParentKey;
	CArray<T_GRUP_K, T_GRUP_K>arChildKey;
	CArray<T_ELEM_K, T_ELEM_K>arKeyElem;
	void Initialize()
	{
		GroupName.Empty();
		ParentKey=0;
		arChildKey.RemoveAll();
		arKeyElem.RemoveAll();
	}
	T_GRUP_D()
	{
	}
	T_GRUP_D(T_GRUP_D& src)
	{
		*this = src;
	}
	T_GRUP_D& operator=(T_GRUP_D& src)
	{
		GroupName=src.GroupName;
		ParentKey=src.ParentKey;
		arChildKey.Copy(src.arChildKey);
		arKeyElem.Copy(src.arKeyElem);
		return *this;
	}
};
struct T_GRUP_D_CH
{
	char GroupName[80];
	T_GRUP_K ParentKey;
	CArray<T_GRUP_K, T_GRUP_K>arChildKey;
	CArray<T_ELEM_K, T_ELEM_K>arKeyElem;
};
struct T_GRUP_UDRD_D
{
	T_GRUP_K key;
	T_GRUP_D data;
};
struct T_GRUP_ITEM_UDRD_D
{
	T_GRUP_K key;
	T_ELEM_K elemkey;
};
struct T_GRUP_UDRD_D_CH
{
	T_GRUP_K key;
	T_GRUP_D_CH data;
};
struct T_GRUP_UDRD_D_CH_RW
{
	T_GRUP_K key;
	char GroupName[80];
	T_GRUP_K ParentKey;
	int nCountChildKey;
	int nCountElemKey;
};
struct T_GRUP_UNIT
{
	T_GRUP_UNIT()
	{
		GroupName=D_UNITSYS_NONE;
		ParentKey=D_UNITSYS_NONE;
		arChildKey=D_UNITSYS_NONE;
		arKeyElem=D_UNITSYS_NONE;
	}
	int GroupName;
	int ParentKey;
	int arChildKey;
	int arKeyElem;
};
#define HASHSIZEGRUP 101
#define HASHSIZEGRUPITEM HASHSIZEELEM
*/

/// sdom /// Sub Domain
#define T_SDOM_K unsigned int
struct T_SDOM_D
{
	CString   strName;        // Working area name
	int       nType;          // 0:None, 1:Slab, 2:Mat, 3:Wall
	double    dAngle;         // Angle (x축에 대한 각도)  
	CArray<T_ELEM_K, T_ELEM_K>arKeyElem;
	unsigned int nDomainId;

	void Initialize()
	{
		strName.Empty();
		nType = 0;
		dAngle = 0.0;    
		arKeyElem.RemoveAll();
		nDomainId=0;
	}
	T_SDOM_D()
	{
	}
	T_SDOM_D(T_SDOM_D& src)
	{
		*this = src;
	}
	T_SDOM_D& operator=(T_SDOM_D& src)
	{
		strName=src.strName;
		nType=src.nType;
		dAngle=src.dAngle;    
		arKeyElem.Copy(src.arKeyElem);
		nDomainId=src.nDomainId;
		return *this;
	}

	BOOL IsSlab() { return (nType == 1)? TRUE : FALSE; };
	BOOL IsMat()  { return (nType == 2)? TRUE : FALSE; };
	BOOL IsWall() { return (nType == 3)? TRUE : FALSE; };
};
struct T_SDOM_D_CH
{
	char strName[80];
	int       nType;
	double    dAngle;
	CArray<T_ELEM_K, T_ELEM_K>arKeyElem;
	unsigned int nDomainId;

	void ConvertToString(T_SDOM_D& rData)
	{
		ConvertCharStr(strName, rData.strName, sizeof(strName));
		rData.nType=nType;
		rData.dAngle=dAngle;
		rData.arKeyElem.Copy(arKeyElem);    
		rData.nDomainId=nDomainId;
	}
	void ConvertToChar(T_SDOM_D& rData)
	{
		ConvertStrChar(rData.strName, strName, sizeof(strName));
		nType=rData.nType;
		dAngle=rData.dAngle;
		arKeyElem.Copy(rData.arKeyElem);  
		nDomainId=rData.nDomainId;
	}
};
struct T_SDOM_UDRD_D
{
	T_SDOM_K key;
	T_SDOM_D data;
};
struct T_SDOM_ITEM_UDRD_D
{
	T_SDOM_K key;
	T_ELEM_K elemkey;  
};
struct T_SDOM_UDRD_D_CH
{
	T_SDOM_K key;
	T_SDOM_D_CH data;
};
struct T_SDOM_UDRD_D_CH_RW
{
	T_SDOM_K key;
	char strName[80];
	int       nType;          // 0:None, 1:Slab
	double    dAngle;         // Angle (x축에 대한 각도)
	unsigned int nDomainId;
	int nCountElemKey;
	void GetSdom(T_SDOM_K& rKey, T_SDOM_D_CH& rData)
	{
		rKey=key;
		memcpy(rData.strName, strName, sizeof(strName));
		rData.nType=nType;
		rData.dAngle=dAngle;
		rData.nDomainId=nDomainId;
	}
	void SetSdom(const T_SDOM_K Key, const T_SDOM_D_CH& rData)
	{
		key=Key;
		memcpy(strName, rData.strName, sizeof(strName));
		nType=rData.nType;
		dAngle=rData.dAngle;
		nDomainId=rData.nDomainId;
		nCountElemKey=(int)rData.arKeyElem.GetSize();    
	}
};
struct T_SDOM_UNIT
{
	T_SDOM_UNIT()
	{
		strName=D_UNITSYS_NONE;
		nType=D_UNITSYS_NONE;
		dAngle=D_UNITSYS_NONE;
		arKeyElem=D_UNITSYS_NONE;
		nDomainId=D_UNITSYS_NONE;
	}
	int strName;
	int nType;
	int dAngle;
	int arKeyElem;
	int nDomainId;
};
#define HASHSIZESDOM 101
#define HASHSIZESDOMITEM HASHSIZEELEM


/////////////////////////////////////////
/// mado /// Main Domain
#define T_MADO_K unsigned int
struct T_MADO_D
{
	CString strName;
	int eltyp;    //Elem Type
	int elmat;    //MatlKey
	int elpro;    //ThckKey
	int nSubType;

	void Initialize()
	{
		strName.Empty();
		eltyp=0;
		elmat=0;
		elpro=0; 
		nSubType = 0;
	}
};
struct T_MADO_D_CH
{
	char strName[80];
	int eltyp;    //Elem Type
	int elmat;    //MatlKey
	int elpro;    //ThckKey
	int nSubType;

	void ConvertToString(T_MADO_D& rData)
	{
		ConvertCharStr(strName, rData.strName, sizeof(strName));
		rData.eltyp=eltyp;
		rData.elmat=elmat;		
		rData.elpro=elpro;
		rData.nSubType=nSubType;
	}
	void ConvertToChar(T_MADO_D& rData)
	{
		ConvertStrChar(rData.strName, strName, sizeof(strName));
		eltyp=rData.eltyp;
		elmat=rData.elmat;
		elpro=rData.elpro;
		nSubType=rData.nSubType;
	}
};
struct T_MADO_UDRD_D
{
	T_MADO_K key;
	T_MADO_D data;
};
struct T_MADO_UDRD_D_CH
{
	T_MADO_K key;
	T_MADO_D_CH data;
};
struct T_MADO_UNIT
{
	T_MADO_UNIT()
	{
		strName=D_UNITSYS_NONE;
		eltyp=D_UNITSYS_NONE;
		elmat=D_UNITSYS_NONE;
		elpro=D_UNITSYS_NONE;		
		nSubType=D_UNITSYS_NONE;		
	}
	int strName;
	int eltyp;
	int elmat;
	int elpro;	
	int nSubType;
};
#define HASHSIZEMADO 101

/////////////////////////////////////////
/// sbdo /// Sub Domain
#define T_SBDO_K unsigned int
struct T_SBDO_D_795
{
	CString strName;
	int       nType;          // 0:None, 1:Slab, 2:Mat, 3:Wall
	double    dAngle[2];      // [0]:Angle x(x축에 대한 각도), [1]:Angle y(Angle x에 대한 각도) 
	T_MADO_K  KeyMado;
	BOOL      bUseMt;
	double    dThickness;
	
	void Initialize()
	{
		strName.Empty();
		nType=0;
		dAngle[0]=0.;
		dAngle[1]=90.;
		KeyMado=0;    
		bUseMt=TRUE;
		dThickness=0.;
	}
	BOOL IsSlab() { return (nType == 1)? TRUE : FALSE; };
	BOOL IsMat()  { return (nType == 2)? TRUE : FALSE; };
	BOOL IsWall() { return (nType == 3)? TRUE : FALSE; };
};
struct T_SBDO_D_CH_795
{
	char strName[80];
	int       nType;          
	double    dAngle[2];      
	T_MADO_K  KeyMado;
	BOOL      bUseMt;
	double    dThickness;
	
	void ConvertToString(T_SBDO_D_795& rData)
	{
		ConvertCharStr(strName, rData.strName, sizeof(strName));
		rData.nType=nType;
		rData.dAngle[0]=dAngle[0];	
		rData.dAngle[1]=dAngle[1];	
		rData.KeyMado=KeyMado;
		rData.bUseMt=bUseMt;
		rData.dThickness=dThickness; 
	}
	void ConvertToChar(T_SBDO_D_795& rData)
	{
		ConvertStrChar(rData.strName, strName, sizeof(strName));
		nType=rData.nType;
		dAngle[0]=rData.dAngle[0];
		dAngle[1]=rData.dAngle[1];
		KeyMado=rData.KeyMado;
		bUseMt=rData.bUseMt;
		dThickness=rData.dThickness; 
	}
};
struct T_SBDO_UDRD_D_795
{
	T_SBDO_K key;
	T_SBDO_D_795 data;
};
struct T_SBDO_UDRD_D_CH_795
{
	T_SBDO_K key;
	T_SBDO_D_CH_795 data;
};

struct T_SBDO_D_840
{
	CString strName;
	int       nType;          // 0:None, 1:Slab, 2:Mat, 3:Wall
	double    dAngle[2];      // [0]:Angle x(x축에 대한 각도), [1]:Angle y(Angle x에 대한 각도) 
	T_MADO_K  KeyMado;
	BOOL      bUseMt;
	double    dThickness;

	BOOL    bBasicRebar;
	CString strTopRebarName_X;
	double  dTopRebarSpace_X;
	CString strBotRebarName_X;
	double  dBotRebarSpace_X;
	CString strTopRebarName_Y;
	double  dTopRebarSpace_Y;
	CString strBotRebarName_Y;
	double  dBotRebarSpace_Y;
	void Initialize()
	{
		strName.Empty();
		nType=0;
		dAngle[0]=0.;
		dAngle[1]=90.;
		KeyMado=0;    
		bUseMt=TRUE;
		dThickness=0.;

		bBasicRebar = FALSE;
		strTopRebarName_X.Empty();
		dTopRebarSpace_X = 0.0;
		strBotRebarName_X.Empty();
		dBotRebarSpace_X = 0.0;
		strTopRebarName_Y.Empty();
		dTopRebarSpace_Y = 0.0;
		strBotRebarName_Y.Empty();
		dBotRebarSpace_Y = 0.0;
	}
	BOOL IsSlab() { return (nType == 1)? TRUE : FALSE; };
	BOOL IsMat()  { return (nType == 2)? TRUE : FALSE; };
	BOOL IsWall() { return (nType == 3)? TRUE : FALSE; };
};

struct T_SBDO_D_CH_840
{
	char strName[80];
	int       nType;          
	double    dAngle[2];      
	T_MADO_K  KeyMado;
	BOOL      bUseMt;
	double    dThickness;

	BOOL      bBasicRebar;
	char strTopRebarName_X[16];
	double dTopRebarSpace_X;
	char strBotRebarName_X[16];
	double dBotRebarSpace_X;
	char strTopRebarName_Y[16];
	double dTopRebarSpace_Y;
	char strBotRebarName_Y[16];
	double dBotRebarSpace_Y;
	void ConvertToString(T_SBDO_D_840& rData)
	{
		ConvertCharStr(strName, rData.strName, sizeof(strName));
		rData.nType=nType;
		rData.dAngle[0]=dAngle[0];	
		rData.dAngle[1]=dAngle[1];	
		rData.KeyMado=KeyMado;
		rData.bUseMt=bUseMt;
		rData.dThickness=dThickness; 

		rData.bBasicRebar=bBasicRebar;
		ConvertCharStr(strTopRebarName_X, rData.strTopRebarName_X, sizeof(strTopRebarName_X));
		rData.dTopRebarSpace_X=dTopRebarSpace_X;
		ConvertCharStr(strBotRebarName_X, rData.strBotRebarName_X, sizeof(strBotRebarName_X));
		rData.dBotRebarSpace_X=dBotRebarSpace_X;
		ConvertCharStr(strTopRebarName_Y, rData.strTopRebarName_Y, sizeof(strTopRebarName_Y));
		rData.dTopRebarSpace_Y=dTopRebarSpace_Y;
		ConvertCharStr(strBotRebarName_Y, rData.strBotRebarName_Y, sizeof(strBotRebarName_Y));
		rData.dBotRebarSpace_Y=dBotRebarSpace_Y;
	}
	void ConvertToChar(T_SBDO_D_840& rData)
	{
		ConvertStrChar(rData.strName, strName, sizeof(strName));
		nType=rData.nType;
		dAngle[0]=rData.dAngle[0];
		dAngle[1]=rData.dAngle[1];
		KeyMado=rData.KeyMado;
		bUseMt=rData.bUseMt;
		dThickness=rData.dThickness; 

		bBasicRebar=rData.bBasicRebar;
		ConvertStrChar(rData.strTopRebarName_X, strTopRebarName_X, sizeof(strTopRebarName_X));
		dTopRebarSpace_X=rData.dTopRebarSpace_X;
		ConvertStrChar(rData.strBotRebarName_X, strBotRebarName_X, sizeof(strBotRebarName_X));
		dBotRebarSpace_X=rData.dBotRebarSpace_X;
		ConvertStrChar(rData.strTopRebarName_Y, strTopRebarName_Y, sizeof(strTopRebarName_Y));
		dTopRebarSpace_Y=rData.dTopRebarSpace_Y;
		ConvertStrChar(rData.strBotRebarName_Y, strBotRebarName_Y, sizeof(strBotRebarName_Y));
		dBotRebarSpace_Y=rData.dBotRebarSpace_Y;
	}
};
struct T_SBDO_UDRD_D_840
{
	T_SBDO_K key;
	T_SBDO_D_840 data;
};
struct T_SBDO_UDRD_D_CH_840
{
	T_SBDO_K key;
	T_SBDO_D_CH_840 data;
};

struct T_SBDO_D_856
{
	CString strName;
	int       nType;          // 0:None, 1:Slab, 2:Mat, 3:Wall
	double    dAngle[2];      // [0]:Angle x(x축에 대한 각도), [1]:Angle y(Angle x에 대한 각도) 
	T_MADO_K  KeyMado;
	BOOL      bUseMt;
	double    dThickness;

	BOOL    bBasicRebar;
	CString strTopRebarName_X;
	double  dTopRebarSpace_X;
	CString strBotRebarName_X;
	double  dBotRebarSpace_X;
	CString strTopRebarName_Y;
	double  dTopRebarSpace_Y;
	CString strBotRebarName_Y;
	double  dBotRebarSpace_Y;

	double dAxisVector[2][3];

	void Initialize()
	{
		strName.Empty();
		nType=0;
		dAngle[0]=0.;
		dAngle[1]=90.;
		KeyMado=0;    
		bUseMt=TRUE;
		dThickness=0.;

		bBasicRebar = FALSE;
		strTopRebarName_X.Empty();
		dTopRebarSpace_X = 0.0;
		strBotRebarName_X.Empty();
		dBotRebarSpace_X = 0.0;
		strTopRebarName_Y.Empty();
		dTopRebarSpace_Y = 0.0;
		strBotRebarName_Y.Empty();
		dBotRebarSpace_Y = 0.0;

		memset(dAxisVector,0,sizeof(dAxisVector));
	}
};

struct T_SBDO_D_CH_856
{
	char strName[80];
	int       nType;          
	double    dAngle[2];      
	T_MADO_K  KeyMado;
	BOOL      bUseMt;
	double    dThickness;

	BOOL      bBasicRebar;
	char strTopRebarName_X[16];
	double dTopRebarSpace_X;
	char strBotRebarName_X[16];
	double dBotRebarSpace_X;
	char strTopRebarName_Y[16];
	double dTopRebarSpace_Y;
	char strBotRebarName_Y[16];
	double dBotRebarSpace_Y;

	double dAxisVector[2][3];

	void ConvertToString(T_SBDO_D_856& rData)
	{
		ConvertCharStr(strName, rData.strName, sizeof(strName));
		rData.nType=nType;
		rData.dAngle[0]=dAngle[0];	
		rData.dAngle[1]=dAngle[1];	
		rData.KeyMado=KeyMado;
		rData.bUseMt=bUseMt;
		rData.dThickness=dThickness; 

		rData.bBasicRebar=bBasicRebar;
		ConvertCharStr(strTopRebarName_X, rData.strTopRebarName_X, sizeof(strTopRebarName_X));
		rData.dTopRebarSpace_X=dTopRebarSpace_X;
		ConvertCharStr(strBotRebarName_X, rData.strBotRebarName_X, sizeof(strBotRebarName_X));
		rData.dBotRebarSpace_X=dBotRebarSpace_X;
		ConvertCharStr(strTopRebarName_Y, rData.strTopRebarName_Y, sizeof(strTopRebarName_Y));
		rData.dTopRebarSpace_Y=dTopRebarSpace_Y;
		ConvertCharStr(strBotRebarName_Y, rData.strBotRebarName_Y, sizeof(strBotRebarName_Y));
		rData.dBotRebarSpace_Y=dBotRebarSpace_Y;

		memcpy(rData.dAxisVector, dAxisVector, sizeof(dAxisVector));
	}
	void ConvertToChar(T_SBDO_D_856& rData)
	{
		ConvertStrChar(rData.strName, strName, sizeof(strName));
		nType=rData.nType;
		dAngle[0]=rData.dAngle[0];
		dAngle[1]=rData.dAngle[1];
		KeyMado=rData.KeyMado;
		bUseMt=rData.bUseMt;
		dThickness=rData.dThickness; 

		bBasicRebar=rData.bBasicRebar;
		ConvertStrChar(rData.strTopRebarName_X, strTopRebarName_X, sizeof(strTopRebarName_X));
		dTopRebarSpace_X=rData.dTopRebarSpace_X;
		ConvertStrChar(rData.strBotRebarName_X, strBotRebarName_X, sizeof(strBotRebarName_X));
		dBotRebarSpace_X=rData.dBotRebarSpace_X;
		ConvertStrChar(rData.strTopRebarName_Y, strTopRebarName_Y, sizeof(strTopRebarName_Y));
		dTopRebarSpace_Y=rData.dTopRebarSpace_Y;
		ConvertStrChar(rData.strBotRebarName_Y, strBotRebarName_Y, sizeof(strBotRebarName_Y));
		dBotRebarSpace_Y=rData.dBotRebarSpace_Y;

		memcpy(dAxisVector, rData.dAxisVector, sizeof(dAxisVector));
	}
};
struct T_SBDO_UDRD_D_856
{
	T_SBDO_K key;
	T_SBDO_D_856 data;
};
struct T_SBDO_UDRD_D_CH_856
{
	T_SBDO_K key;
	T_SBDO_D_CH_856 data;
};

struct T_SBDO_D_860
{
	CString strName;
	int       nType;          // 0:None, 1:Slab, 2:Mat, 3:Wall
	double    dAngle[2];      // [0]:Angle x(x축에 대한 각도), [1]:Angle y(Angle x에 대한 각도) 
	T_MADO_K  KeyMado;
	BOOL      bUseMt;
	double    dThickness;

	BOOL    bBasicRebar;
	CString strTopRebarName_X;
	double  dTopRebarSpace_X;
	CString strBotRebarName_X;
	double  dBotRebarSpace_X;
	CString strTopRebarName_Y;
	double  dTopRebarSpace_Y;
	CString strBotRebarName_Y;
	double  dBotRebarSpace_Y;

	double dAxisVector[2][3];

	BOOL      bRebarMatl;
	UINT      RebarMatlK; // T_MATL_K

	void Initialize()
	{
		strName.Empty();
		nType=0;
		dAngle[0]=0.;
		dAngle[1]=90.;
		KeyMado=0;    
		bUseMt=TRUE;
		dThickness=0.;

		bBasicRebar = FALSE;
		strTopRebarName_X.Empty();
		dTopRebarSpace_X = 0.0;
		strBotRebarName_X.Empty();
		dBotRebarSpace_X = 0.0;
		strTopRebarName_Y.Empty();
		dTopRebarSpace_Y = 0.0;
		strBotRebarName_Y.Empty();
		dBotRebarSpace_Y = 0.0;

		memset(dAxisVector,0,sizeof(dAxisVector));

		bRebarMatl = FALSE;
		RebarMatlK = 0;
	}
};

struct T_SBDO_D_CH_860
{
	char strName[80];
	int       nType;          
	double    dAngle[2];      
	T_MADO_K  KeyMado;
	BOOL      bUseMt;
	double    dThickness;

	BOOL      bBasicRebar;
	char strTopRebarName_X[16];
	double dTopRebarSpace_X;
	char strBotRebarName_X[16];
	double dBotRebarSpace_X;
	char strTopRebarName_Y[16];
	double dTopRebarSpace_Y;
	char strBotRebarName_Y[16];
	double dBotRebarSpace_Y;

	double dAxisVector[2][3];

	BOOL      bRebarMatl;
	UINT      RebarMatlK; // T_MATL_K

	void ConvertToString(T_SBDO_D_860& rData)
	{
		ConvertCharStr(strName, rData.strName, sizeof(strName));
		rData.nType=nType;
		rData.dAngle[0]=dAngle[0];	
		rData.dAngle[1]=dAngle[1];	
		rData.KeyMado=KeyMado;
		rData.bUseMt=bUseMt;
		rData.dThickness=dThickness; 

		rData.bBasicRebar=bBasicRebar;
		ConvertCharStr(strTopRebarName_X, rData.strTopRebarName_X, sizeof(strTopRebarName_X));
		rData.dTopRebarSpace_X=dTopRebarSpace_X;
		ConvertCharStr(strBotRebarName_X, rData.strBotRebarName_X, sizeof(strBotRebarName_X));
		rData.dBotRebarSpace_X=dBotRebarSpace_X;
		ConvertCharStr(strTopRebarName_Y, rData.strTopRebarName_Y, sizeof(strTopRebarName_Y));
		rData.dTopRebarSpace_Y=dTopRebarSpace_Y;
		ConvertCharStr(strBotRebarName_Y, rData.strBotRebarName_Y, sizeof(strBotRebarName_Y));
		rData.dBotRebarSpace_Y=dBotRebarSpace_Y;

		memcpy(rData.dAxisVector, dAxisVector, sizeof(dAxisVector));

		rData.bRebarMatl=bRebarMatl; 
		rData.RebarMatlK=RebarMatlK;
	}
	void ConvertToChar(T_SBDO_D_860& rData)
	{
		ConvertStrChar(rData.strName, strName, sizeof(strName));
		nType=rData.nType;
		dAngle[0]=rData.dAngle[0];
		dAngle[1]=rData.dAngle[1];
		KeyMado=rData.KeyMado;
		bUseMt=rData.bUseMt;
		dThickness=rData.dThickness; 

		bBasicRebar=rData.bBasicRebar;
		ConvertStrChar(rData.strTopRebarName_X, strTopRebarName_X, sizeof(strTopRebarName_X));
		dTopRebarSpace_X=rData.dTopRebarSpace_X;
		ConvertStrChar(rData.strBotRebarName_X, strBotRebarName_X, sizeof(strBotRebarName_X));
		dBotRebarSpace_X=rData.dBotRebarSpace_X;
		ConvertStrChar(rData.strTopRebarName_Y, strTopRebarName_Y, sizeof(strTopRebarName_Y));
		dTopRebarSpace_Y=rData.dTopRebarSpace_Y;
		ConvertStrChar(rData.strBotRebarName_Y, strBotRebarName_Y, sizeof(strBotRebarName_Y));
		dBotRebarSpace_Y=rData.dBotRebarSpace_Y;

		memcpy(dAxisVector, rData.dAxisVector, sizeof(dAxisVector));

		bRebarMatl=rData.bRebarMatl; 
		RebarMatlK=rData.RebarMatlK;
	}
};
struct T_SBDO_UDRD_D_860
{
	T_SBDO_K key;
	T_SBDO_D_860 data;
};
struct T_SBDO_UDRD_D_CH_860
{
	T_SBDO_K key;
	T_SBDO_D_CH_860 data;
};

enum T_SBDO_TYPE
{
	SBDO_NONE=0, SBDO_SLAB=1, SBDO_MAT=2, SBDO_WALL=3, SBDO_SHEL=4, SBDO_TYPE_NUM
};

struct T_SBDO_D
{
	CString strName;
	int       nType;          // T_SBDO_TYPE, 0:None, 1:Slab, 2:Mat, 3:Wall... Civil에서는 Shell 일 때는 4가 세팅되고, 그 외는 항상 1이 세팅됨
	double    dAngle[2];      // [0]:Angle x(x축에 대한 각도), [1]:Angle y(Angle x에 대한 각도) 
	T_MADO_K  KeyMado;
	BOOL      bUseMt;
	double    dThickness;

	BOOL      bBasicRebar;
	CString   strTopRebarName_X;
	double    dTopRebarSpace_X;
	CString   strBotRebarName_X;
	double    dBotRebarSpace_X;
	CString   strTopRebarName_Y;
	double    dTopRebarSpace_Y;
	CString   strBotRebarName_Y;
	double    dBotRebarSpace_Y;
	
	double    dAxisVector[2][3];

	BOOL      bRebarMatl;
	UINT      RebarMatlK; // T_MATL_K

	int       nRebarAxisType; // 0:Local, 1:UCS, 2:Reference Axis
	CString   strUCS;
	int       nMembTypeCivil; // 0:None, 1:Plate Beam (1D), 2:Plate Column (1D)), 3:Shell

	void Initialize()
	{
		strName.Empty();
		nType=SBDO_NONE;
		dAngle[0]=0.;
		dAngle[1]=90.;
		KeyMado=0;    
		bUseMt=TRUE;
		dThickness=0.;

		bBasicRebar = FALSE;
		strTopRebarName_X.Empty();
		dTopRebarSpace_X = 0.0;
		strBotRebarName_X.Empty();
		dBotRebarSpace_X = 0.0;
		strTopRebarName_Y.Empty();
		dTopRebarSpace_Y = 0.0;
		strBotRebarName_Y.Empty();
		dBotRebarSpace_Y = 0.0;

		memset(dAxisVector,0,sizeof(dAxisVector));

		bRebarMatl = FALSE;
		RebarMatlK = NULL;

		nRebarAxisType = 2;
		strUCS = "";
		nMembTypeCivil = 0;
	}
	void InitBasicRebar()
	{
		bBasicRebar = FALSE;
		strTopRebarName_X.Empty();
		dTopRebarSpace_X = 0.0;
		strBotRebarName_X.Empty();
		dBotRebarSpace_X = 0.0;
		strTopRebarName_Y.Empty();
		dTopRebarSpace_Y = 0.0;
		strBotRebarName_Y.Empty();
		dBotRebarSpace_Y = 0.0;
	}
	BOOL IsNone() { return (nType == SBDO_NONE)? TRUE : FALSE; };
	BOOL IsSlab() { return (nType == SBDO_SLAB)? TRUE : FALSE; };
	BOOL IsMat()  { return (nType == SBDO_MAT )? TRUE : FALSE; };
	BOOL IsWall() { return (nType == SBDO_WALL)? TRUE : FALSE; };
	BOOL IsShell(){ return (nType == SBDO_SHEL)? TRUE : FALSE; }
	void Convert795(T_SBDO_D_795& rData)
	{
		strName     = rData.strName   ;
		nType       = rData.nType     ;          
		dAngle[0]   = rData.dAngle[0] ;  
		dAngle[1]   = rData.dAngle[1] ;
		KeyMado     = rData.KeyMado   ;
		bUseMt      = rData.bUseMt    ;
		dThickness  = rData.dThickness;

		bBasicRebar = FALSE;
		strTopRebarName_X.Empty();
		dTopRebarSpace_X = 0.0;
		strBotRebarName_X.Empty();
		dBotRebarSpace_X = 0.0;
		strTopRebarName_Y.Empty();
		dTopRebarSpace_Y = 0.0;
		strBotRebarName_Y.Empty();
		dBotRebarSpace_Y = 0.0;

		memset(dAxisVector,0,sizeof(dAxisVector));

		bRebarMatl = FALSE;
		RebarMatlK = NULL;

		nRebarAxisType = 2;
		strUCS = "";
		nMembTypeCivil = 0;
	}
	void Convert840(T_SBDO_D_840& rData)
	{
		strName     = rData.strName   ;
		nType       = rData.nType     ;          
		dAngle[0]   = rData.dAngle[0] ;  
		dAngle[1]   = rData.dAngle[1] ;
		KeyMado     = rData.KeyMado   ;
		bUseMt      = rData.bUseMt    ;
		dThickness  = rData.dThickness;

		bBasicRebar = rData.bBasicRebar;
		strTopRebarName_X = rData.strTopRebarName_X;
		dTopRebarSpace_X = rData.dTopRebarSpace_X;;
		strBotRebarName_X = rData.strBotRebarName_X;
		dBotRebarSpace_X = rData.dBotRebarSpace_X;
		strTopRebarName_Y = rData.strTopRebarName_Y;
		dTopRebarSpace_Y = rData.dTopRebarSpace_Y;
		strBotRebarName_Y = rData.strBotRebarName_Y;
		dBotRebarSpace_Y = rData.dBotRebarSpace_Y;

		memset(dAxisVector,0,sizeof(dAxisVector));

		bRebarMatl = FALSE;
		RebarMatlK = NULL;

		nRebarAxisType = 2;
		strUCS = "";
		nMembTypeCivil = 0;
	}
	void Convert856(T_SBDO_D_856& rData)
	{
		strName     = rData.strName   ;
		nType       = rData.nType     ;          
		dAngle[0]   = rData.dAngle[0] ;  
		dAngle[1]   = rData.dAngle[1] ;
		KeyMado     = rData.KeyMado   ;
		bUseMt      = rData.bUseMt    ;
		dThickness  = rData.dThickness;

		bBasicRebar = rData.bBasicRebar;
		strTopRebarName_X = rData.strTopRebarName_X;
		dTopRebarSpace_X = rData.dTopRebarSpace_X;;
		strBotRebarName_X = rData.strBotRebarName_X;
		dBotRebarSpace_X = rData.dBotRebarSpace_X;
		strTopRebarName_Y = rData.strTopRebarName_Y;
		dTopRebarSpace_Y = rData.dTopRebarSpace_Y;
		strBotRebarName_Y = rData.strBotRebarName_Y;
		dBotRebarSpace_Y = rData.dBotRebarSpace_Y;

		memcpy(dAxisVector, rData.dAxisVector, sizeof(dAxisVector));

		bRebarMatl = FALSE;
		RebarMatlK = NULL;

		nRebarAxisType = 2;
		strUCS = "";
		nMembTypeCivil = 0;
	}
	void Convert860(T_SBDO_D_860& rData)
	{
		strName     = rData.strName   ;
		nType       = rData.nType     ;          
		dAngle[0]   = rData.dAngle[0] ;  
		dAngle[1]   = rData.dAngle[1] ;
		KeyMado     = rData.KeyMado   ;
		bUseMt      = rData.bUseMt    ;
		dThickness  = rData.dThickness;

		bBasicRebar = rData.bBasicRebar;
		strTopRebarName_X = rData.strTopRebarName_X;
		dTopRebarSpace_X = rData.dTopRebarSpace_X;;
		strBotRebarName_X = rData.strBotRebarName_X;
		dBotRebarSpace_X = rData.dBotRebarSpace_X;
		strTopRebarName_Y = rData.strTopRebarName_Y;
		dTopRebarSpace_Y = rData.dTopRebarSpace_Y;
		strBotRebarName_Y = rData.strBotRebarName_Y;
		dBotRebarSpace_Y = rData.dBotRebarSpace_Y;

		memcpy(dAxisVector, rData.dAxisVector, sizeof(dAxisVector));

		bRebarMatl = rData.bRebarMatl;
		RebarMatlK = rData.RebarMatlK;

		nRebarAxisType = 2;
		strUCS = "";
		nMembTypeCivil = 0;
	}
};
struct T_SBDO_D_CH
{
	char strName[80];
	int       nType;          
	double    dAngle[2];      
	T_MADO_K  KeyMado;
	BOOL      bUseMt;
	double    dThickness;

	BOOL      bBasicRebar;
	char strTopRebarName_X[16];
	double dTopRebarSpace_X;
	char strBotRebarName_X[16];
	double dBotRebarSpace_X;
	char strTopRebarName_Y[16];
	double dTopRebarSpace_Y;
	char strBotRebarName_Y[16];
	double dBotRebarSpace_Y;

	double dAxisVector[2][3];

	BOOL      bRebarMatl;
	UINT      RebarMatlK; // T_MATL_K

	int       nRebarAxisType;
	char      strUCS[80];
	int       nMembTypeCivil;

	void ConvertToString(T_SBDO_D& rData)
	{
		ConvertCharStr(strName, rData.strName, sizeof(strName));
		rData.nType=nType;
		rData.dAngle[0]=dAngle[0];	
		rData.dAngle[1]=dAngle[1];	
		rData.KeyMado=KeyMado;
		rData.bUseMt=bUseMt;
		rData.dThickness=dThickness; 

		rData.bBasicRebar=bBasicRebar;
		ConvertCharStr(strTopRebarName_X, rData.strTopRebarName_X, sizeof(strTopRebarName_X));
		rData.dTopRebarSpace_X=dTopRebarSpace_X;
		ConvertCharStr(strBotRebarName_X, rData.strBotRebarName_X, sizeof(strBotRebarName_X));
		rData.dBotRebarSpace_X=dBotRebarSpace_X;
		ConvertCharStr(strTopRebarName_Y, rData.strTopRebarName_Y, sizeof(strTopRebarName_Y));
		rData.dTopRebarSpace_Y=dTopRebarSpace_Y;
		ConvertCharStr(strBotRebarName_Y, rData.strBotRebarName_Y, sizeof(strBotRebarName_Y));
		rData.dBotRebarSpace_Y=dBotRebarSpace_Y;

		memcpy(rData.dAxisVector, dAxisVector, sizeof(dAxisVector));

		rData.bRebarMatl=bRebarMatl;
		rData.RebarMatlK=RebarMatlK;

		rData.nRebarAxisType=nRebarAxisType;

		ConvertCharStr(strUCS, rData.strUCS, sizeof(strUCS));
		rData.nMembTypeCivil=nMembTypeCivil;
	}
	void ConvertToChar(T_SBDO_D& rData)
	{
		ConvertStrChar(rData.strName, strName, sizeof(strName));
		nType=rData.nType;
		dAngle[0]=rData.dAngle[0];
		dAngle[1]=rData.dAngle[1];
		KeyMado=rData.KeyMado;
		bUseMt=rData.bUseMt;
		dThickness=rData.dThickness; 

		bBasicRebar=rData.bBasicRebar;
		ConvertStrChar(rData.strTopRebarName_X, strTopRebarName_X, sizeof(strTopRebarName_X));
		dTopRebarSpace_X=rData.dTopRebarSpace_X;
		ConvertStrChar(rData.strBotRebarName_X, strBotRebarName_X, sizeof(strBotRebarName_X));
		dBotRebarSpace_X=rData.dBotRebarSpace_X;
		ConvertStrChar(rData.strTopRebarName_Y, strTopRebarName_Y, sizeof(strTopRebarName_Y));
		dTopRebarSpace_Y=rData.dTopRebarSpace_Y;
		ConvertStrChar(rData.strBotRebarName_Y, strBotRebarName_Y, sizeof(strBotRebarName_Y));
		dBotRebarSpace_Y=rData.dBotRebarSpace_Y;

		memcpy(dAxisVector, rData.dAxisVector, sizeof(dAxisVector));

		bRebarMatl=rData.bRebarMatl;
		RebarMatlK=rData.RebarMatlK;
		nRebarAxisType=rData.nRebarAxisType;
		ConvertStrChar(rData.strUCS, strUCS, sizeof(strUCS));
		nMembTypeCivil=rData.nMembTypeCivil;
	}
	void Convert795(T_SBDO_D_CH_795& rData)
	{
		memcpy(strName, rData.strName, sizeof(strName));    
		nType       = rData.nType     ;          
		dAngle[0]   = rData.dAngle[0] ;  
		dAngle[1]   = rData.dAngle[1] ;
		KeyMado     = rData.KeyMado   ;
		bUseMt      = rData.bUseMt    ;
		dThickness  = rData.dThickness;

		bBasicRebar = FALSE;
		memset(strTopRebarName_X, 0, sizeof(strTopRebarName_X));
		dTopRebarSpace_X = 0.0;
		memset(strBotRebarName_X, 0, sizeof(strBotRebarName_X));
		dBotRebarSpace_X = 0.0;
		memset(strTopRebarName_Y, 0, sizeof(strTopRebarName_Y));
		dTopRebarSpace_Y = 0.0;
		memset(strBotRebarName_Y, 0, sizeof(strBotRebarName_Y));    
		dBotRebarSpace_Y = 0.0;

		memset(dAxisVector,0,sizeof(dAxisVector));

		bRebarMatl = FALSE;
		RebarMatlK = NULL;

		nRebarAxisType=2;
		memset(strUCS, 0, sizeof(strUCS));
		nMembTypeCivil=0;
	}
	void Convert840(T_SBDO_D_CH_840& rData)
	{
		memcpy(strName, rData.strName, sizeof(strName));    
		nType       = rData.nType     ;          
		dAngle[0]   = rData.dAngle[0] ;  
		dAngle[1]   = rData.dAngle[1] ;
		KeyMado     = rData.KeyMado   ;
		bUseMt      = rData.bUseMt    ;
		dThickness  = rData.dThickness;

		bBasicRebar = rData.bBasicRebar;

		memcpy(strTopRebarName_X, rData.strTopRebarName_X, sizeof(strTopRebarName_X));   
		dTopRebarSpace_X = rData.dTopRebarSpace_X;
		memcpy(strBotRebarName_X, rData.strBotRebarName_X, sizeof(strBotRebarName_X));
		dBotRebarSpace_X = rData.dTopRebarSpace_X;
		memcpy(strTopRebarName_Y, rData.strTopRebarName_Y, sizeof(strTopRebarName_Y));   
		dTopRebarSpace_Y = rData.dTopRebarSpace_X;
		memcpy(strBotRebarName_Y, rData.strBotRebarName_Y, sizeof(strBotRebarName_Y));   
		dBotRebarSpace_Y = rData.dTopRebarSpace_X;

		memset(dAxisVector,0,sizeof(dAxisVector));

		bRebarMatl = FALSE;
		RebarMatlK = NULL;

		nRebarAxisType=2;
		memset(strUCS, 0, sizeof(strUCS));
		nMembTypeCivil=0;
	}
	void Convert856(T_SBDO_D_CH_856& rData)
	{
		memcpy(strName, rData.strName, sizeof(strName));    
		nType       = rData.nType     ;          
		dAngle[0]   = rData.dAngle[0] ;  
		dAngle[1]   = rData.dAngle[1] ;
		KeyMado     = rData.KeyMado   ;
		bUseMt      = rData.bUseMt    ;
		dThickness  = rData.dThickness;

		bBasicRebar = rData.bBasicRebar;

		memcpy(strTopRebarName_X, rData.strTopRebarName_X, sizeof(strTopRebarName_X));   
		dTopRebarSpace_X = rData.dTopRebarSpace_X;
		memcpy(strBotRebarName_X, rData.strBotRebarName_X, sizeof(strBotRebarName_X));
		dBotRebarSpace_X = rData.dTopRebarSpace_X;
		memcpy(strTopRebarName_Y, rData.strTopRebarName_Y, sizeof(strTopRebarName_Y));   
		dTopRebarSpace_Y = rData.dTopRebarSpace_X;
		memcpy(strBotRebarName_Y, rData.strBotRebarName_Y, sizeof(strBotRebarName_Y));   
		dBotRebarSpace_Y = rData.dTopRebarSpace_X;

		memcpy(dAxisVector, rData.dAxisVector, sizeof(dAxisVector));

		bRebarMatl = FALSE;
		RebarMatlK = NULL;

		nRebarAxisType=2;
		memset(strUCS, 0, sizeof(strUCS));
		nMembTypeCivil=0;
	}

	void Convert860(T_SBDO_D_CH_860& rData)
	{
		memcpy(strName, rData.strName, sizeof(strName));    
		nType       = rData.nType     ;          
		dAngle[0]   = rData.dAngle[0] ;  
		dAngle[1]   = rData.dAngle[1] ;
		KeyMado     = rData.KeyMado   ;
		bUseMt      = rData.bUseMt    ;
		dThickness  = rData.dThickness;

		bBasicRebar = rData.bBasicRebar;

		memcpy(strTopRebarName_X, rData.strTopRebarName_X, sizeof(strTopRebarName_X));   
		dTopRebarSpace_X = rData.dTopRebarSpace_X;
		memcpy(strBotRebarName_X, rData.strBotRebarName_X, sizeof(strBotRebarName_X));
		dBotRebarSpace_X = rData.dTopRebarSpace_X;
		memcpy(strTopRebarName_Y, rData.strTopRebarName_Y, sizeof(strTopRebarName_Y));   
		dTopRebarSpace_Y = rData.dTopRebarSpace_X;
		memcpy(strBotRebarName_Y, rData.strBotRebarName_Y, sizeof(strBotRebarName_Y));   
		dBotRebarSpace_Y = rData.dTopRebarSpace_X;

		memcpy(dAxisVector, rData.dAxisVector, sizeof(dAxisVector));

		bRebarMatl = rData.bRebarMatl;
		RebarMatlK = rData.RebarMatlK;

		nRebarAxisType=2;
		memset(strUCS, 0, sizeof(strUCS));
		nMembTypeCivil=0;
	}
};
struct T_SBDO_UDRD_D
{
	T_SBDO_K key;
	T_SBDO_D data;
};
struct T_SBDO_UDRD_D_CH
{
	T_SBDO_K key;
	T_SBDO_D_CH data;
};
struct T_SBDO_UNIT
{
	T_SBDO_UNIT()
	{
		strName=D_UNITSYS_NONE;
		nType=D_UNITSYS_NONE;
		dAngle=D_UNITSYS_NONE;
		KeyMado=D_UNITSYS_NONE;		
		bUseMt=D_UNITSYS_NONE;
		dThickness=D_UNITSYS_BASE_LENGTH; 

		dTopRebarSpace_X=D_UNITSYS_BASE_LENGTH;
		dBotRebarSpace_X=D_UNITSYS_BASE_LENGTH;
		dTopRebarSpace_Y=D_UNITSYS_BASE_LENGTH;
		dBotRebarSpace_Y=D_UNITSYS_BASE_LENGTH;
	}
	int strName;
	int nType;
	int dAngle;
	int KeyMado;	
	int bUseMt;
	int dThickness;	

	int dTopRebarSpace_X;
	int dBotRebarSpace_X;
	int dTopRebarSpace_Y;
	int dBotRebarSpace_Y;
};
#define HASHSIZESBDO 1001

struct T_SBDO_ELEMLIST_D
{
	T_SBDO_D SbdoD;
	T_ELEM_K_LIST ElemKList;

	void Initialize()
	{
		SbdoD.Initialize();
		ElemKList.RemoveAll();
	}
	T_SBDO_ELEMLIST_D()
	{
	}
	T_SBDO_ELEMLIST_D(T_SBDO_ELEMLIST_D& src)
	{
		*this = src;
	}
	T_SBDO_ELEMLIST_D& operator=(T_SBDO_ELEMLIST_D& src)
	{
		SbdoD=src.SbdoD;
		ElemKList.Copy(src.ElemKList);    
		return *this;
	}
};

/////////////////////////////////////////
/// doel /// Domain Relation
#define T_DOEL_K T_ELEM_K
struct T_DOEL_D
{
	int nDomainType; //0:Main Domain, 1:Sub Domain
	int KeyDomain;    //Domain  
	
	void Initialize()
	{
		nDomainType=0;
		KeyDomain=0;       
	}
};
struct T_DOEL_UDRD_D
{
	T_DOEL_K key;
	T_DOEL_D data;
};
struct T_DOEL_UNIT
{
	T_DOEL_UNIT()
	{
		NoUnit = D_UNITSYS_NONE;
	}
	int NoUnit;
};
#define HASHSIZEDOEL 10001

// boundary group
#define T_BNGR_K unsigned int
struct T_BNGR_D_830
{
	CString GroupName;
	int nGroupId;
	void Initialize()
	{
		GroupName.Empty();
		nGroupId = 0;
	}
};
struct T_BNGR_D
{
	CString GroupName;
	int nGroupId;
	int nAutoType; // 0:Creep 1:Shrinkage - Multiple Modulus of Elasticity (Creep/Shringkage) 를 통해서 만들어진 Group

	void Initialize()
	{
		GroupName.Empty();
		nGroupId = 0;
		nAutoType = FALSE;
	}
	void Convert830(T_BNGR_D_830& rData)
	{
		GroupName= rData.GroupName;
		nGroupId= rData.nGroupId;
		nAutoType = FALSE;
	}
};
struct T_BNGR_D_CH_830
{
	char GroupName[80];
	int nGroupId;
	void ConvertToString(T_BNGR_D_830& rData)
	{
		ConvertCharStr(GroupName, rData.GroupName, sizeof(GroupName));
		rData.nGroupId = nGroupId;
	}
	void ConvertToChar(T_BNGR_D_830& rData)
	{
		ConvertStrChar(rData.GroupName, GroupName, sizeof(GroupName));
		nGroupId = rData.nGroupId;
	}
};

struct T_BNGR_D_CH
{
	char GroupName[80];
	int nGroupId;
	int nAutoType; // 0:Creep 1:Shrinkage - Multiple Modulus of Elasticity (Creep/Shringkage) 를 통해서 만들어진 Group

	void ConvertToString(T_BNGR_D& rData)
	{
		ConvertCharStr(GroupName, rData.GroupName, sizeof(GroupName));
		rData.nGroupId = nGroupId;
		rData.nAutoType = nAutoType;
	}
	void ConvertToChar(T_BNGR_D& rData)
	{
		ConvertStrChar(rData.GroupName, GroupName, sizeof(GroupName));
		nGroupId = rData.nGroupId;
		nAutoType = rData.nAutoType;
	}
	void Convert830(T_BNGR_D_CH_830& rData)
	{
		memcpy(GroupName, rData.GroupName, sizeof(GroupName));
		nGroupId=rData.nGroupId;
		nAutoType = FALSE;
	}
};
struct T_BNGR_UNIT
{
	T_BNGR_UNIT()
	{
		GroupName = D_UNITSYS_NONE;
		nGroupId = D_UNITSYS_NONE;
	}
	int GroupName;
	int nGroupId;
};
struct T_BNGR_UDRD_D_830
{
	T_BNGR_K key;
	T_BNGR_D_830 data;
};
struct T_BNGR_UDRD_D
{
	T_BNGR_K key;
	T_BNGR_D data;
};
struct T_BNGR_UDRD_D_CH_830
{
	T_BNGR_K key;
	T_BNGR_D_CH_830 data;
};
struct T_BNGR_UDRD_D_CH
{
	T_BNGR_K key;
	T_BNGR_D_CH data;
};
#define HASHSIZEBNGR 101
#define D_BNGR_KEY_DEFAULT  0
#define D_BNGR_KEY_ALL      9999999

// load group
#define T_LDGR_K unsigned int
struct T_LDGR_D
{
	CString GroupName;
	int nGroupId;
	void Initialize()
	{
		GroupName.Empty();
		nGroupId = 0;
	}
};

struct T_LDGR_D_CH
{
	char GroupName[80];
	int nGroupId;
	void ConvertToString(T_LDGR_D& rData)
	{
		ConvertCharStr(GroupName, rData.GroupName, sizeof(GroupName));
		rData.nGroupId = nGroupId;
	}
	void ConvertToChar(T_LDGR_D& rData)
	{
		ConvertStrChar(rData.GroupName, GroupName, sizeof(GroupName));
		nGroupId = rData.nGroupId;
	}
};
struct T_LDGR_UNIT
{
	T_LDGR_UNIT()
	{
		GroupName = D_UNITSYS_NONE;
		nGroupId = D_UNITSYS_NONE;
	}
	int GroupName;
	int nGroupId;
};
struct T_LDGR_UDRD_D
{
	T_LDGR_K key;
	T_LDGR_D data;
};
struct T_LDGR_UDRD_D_CH
{
	T_LDGR_K key;
	T_LDGR_D_CH data;
};
#define HASHSIZELDGR 101
#define D_LDGR_KEY_DEFAULT  0
#define D_LDGR_KEY_ALL      9999999

// tendon group
#define T_TDGR_K unsigned int
struct T_TDGR_D
{
	CString GroupName;
	int nGroupId;
	void Initialize()
	{
		GroupName.Empty();
		nGroupId = 0;
	}
};

struct T_TDGR_D_CH
{
	char GroupName[80];
	int nGroupId;
	void ConvertToString(T_TDGR_D& rData)
	{
		ConvertCharStr(GroupName, rData.GroupName, sizeof(GroupName));
		rData.nGroupId = nGroupId;
	}
	void ConvertToChar(T_TDGR_D& rData)
	{
		ConvertStrChar(rData.GroupName, GroupName, sizeof(GroupName));
		nGroupId = rData.nGroupId;
	}
};
struct T_TDGR_UNIT
{
	T_TDGR_UNIT()
	{
		GroupName = D_UNITSYS_NONE;
		nGroupId = D_UNITSYS_NONE;
	}
	int GroupName;
	int nGroupId;
};
struct T_TDGR_UDRD_D
{
	T_TDGR_K key;
	T_TDGR_D data;
};
struct T_TDGR_UDRD_D_CH
{
	T_TDGR_K key;
	T_TDGR_D_CH data;
};
#define HASHSIZETDGR 101
#define D_TDGR_KEY_DEFAULT  0
#define D_TDGR_KEY_ALL      9999999

//////////////////////////////////////////////////////////////////////////
//
// Loading Area Group
// added by yskim 140227
// 풍하중 자동 계산에 들어가는 폐영역의 그룹.

#define T_LAGR_K unsigned int
struct T_LAGR_D
{
	CString GroupName;
	int nGroupId;
	void Initialize()
	{
		GroupName.Empty();
		nGroupId = 0;
	}
	T_LAGR_D& operator=(const T_LAGR_D& rData)
	{
		GroupName = rData.GroupName;
		nGroupId = rData.nGroupId;
		return *this;
	}
};

struct T_LAGR_D_CH
{
	char GroupName[80];
	int nGroupId;
	void ConvertToString(T_LAGR_D& rData)
	{
		ConvertCharStr(GroupName, rData.GroupName, sizeof(GroupName));
		rData.nGroupId = nGroupId;
	}
	void ConvertToChar(T_LAGR_D& rData)
	{
		ConvertStrChar(rData.GroupName, GroupName, sizeof(GroupName));
		nGroupId = rData.nGroupId;
	}
};
struct T_LAGR_UNIT
{
	T_LAGR_UNIT()
	{
		GroupName = D_UNITSYS_NONE;
		nGroupId = D_UNITSYS_NONE;
	}
	int GroupName;
	int nGroupId;
};
struct T_LAGR_UDRD_D
{
	T_LAGR_K key;
	T_LAGR_D data;
};
struct T_LAGR_UDRD_D_CH
{
	T_LAGR_K key;
	T_LAGR_D_CH data;
};
#define HASHSIZELAGR        101
#define D_LAGR_KEY_DEFAULT  0
#define D_LAGR_KEY_ALL      9999999

//////////////////////////////////////////////////////////////////////////
//
// Loading Area Plane
// added by yskim 140227
// 풍하중 자동 계산에 들어가는 폐영역의 면.

#define T_LAPL_K unsigned int
struct T_LAPL_D
{
	T_LAGR_K LagrK;
	CArray<T_NODE_K, T_NODE_K> arKeyNode;
	CArray<T_ELEM_K, T_ELEM_K> arKeyElem;
	void Initialize()
	{
		LagrK = 0;
		arKeyNode.RemoveAll();
		arKeyElem.RemoveAll();
	}
	T_LAPL_D() {}
	T_LAPL_D(const T_LAPL_D& src) { *this = src;}
	T_LAPL_D& operator=(const T_LAPL_D& rData)
	{
		LagrK = rData.LagrK;
		arKeyNode.Copy(rData.arKeyNode);
		arKeyElem.Copy(rData.arKeyElem);
		return *this;
	}
};

struct T_LAPL_UDRD_D
{
	T_LAPL_K key;
	T_LAPL_D data;
};
struct T_LAPL_ITEM_UDRD_D
{
	T_LAPL_K key;
	T_NODE_K nodekey;
	T_ELEM_K elemkey;
};
struct T_LAPL_UDRD_D_RW
{
	T_LAPL_K key;
	T_LAGR_K LagrK;
	int nCountNodeKey;
	int nCountElemKey;
	
	void GetLapl(T_LAPL_K& rKey, T_LAPL_D& rData)
	{
		rKey = key;
		rData.LagrK = LagrK;
	}
	void SetLapl(const T_LAPL_K rKey, const T_LAPL_D& rData)
	{
		key=rKey;
		LagrK=rData.LagrK;    
		nCountElemKey=(int)rData.arKeyElem.GetSize();
		nCountNodeKey=(int)rData.arKeyNode.GetSize();
	}
};
struct T_LAPL_UNIT
{
	T_LAPL_UNIT()
	{
		LagrK     = D_UNITSYS_NONE;
		arKeyNode = D_UNITSYS_NONE;
		arKeyElem = D_UNITSYS_NONE;
	}
	int LagrK;
	int arKeyNode;
	int arKeyElem;
};

#define HASHSIZELAPL        101
#define HASHSIZELAPLITEM    HASHSIZEELEM
#define D_LAPL_KEY_ALL      9999999

/// Virtual Section (for Virtual Beam) ///
#define T_VSEC_K unsigned int
struct T_VSEC_D
{
	CString strName;
	CArray<T_ELEM_K, T_ELEM_K>arKeyElem;  // Vect에서의 부재력 계산에 사용될 elem
	CArray<T_NODE_K, T_NODE_K>arKeyNode;  // Vect에서의 부재력 계산에 사용될 node
	int nCentCalcType;  // 도심 계산 방법 (=0: Auto, =1: User)
	double dCentX;      // centroid X (Global)
	double dCentY;      // centroid Y (Global)
	double dCentZ;      // centroid Z (Global)
	double dNormalVecX;    // 단면의 normal vector-x
	double dNormalVecY;    // 단면의 normal vector-y
	double dNormalVecZ;    // 단면의 normal vector-z

	void Initialize()
	{
		strName = _T("");
		arKeyElem.RemoveAll();
		arKeyNode.RemoveAll();
		nCentCalcType = 0;
		dCentX = 0.0;
		dCentY = 0.0;
		dCentZ = 0.0;
		dNormalVecX = 0.0;
		dNormalVecY = 0.0;
		dNormalVecZ = 0.0;
	}
	T_VSEC_D()
	{
		Initialize();
	}
	T_VSEC_D(const T_VSEC_D& src)
	{
		*this = src;
	}
	T_VSEC_D& operator=(const T_VSEC_D& src)
	{
		strName = src.strName;
		arKeyElem.Copy(src.arKeyElem);
		arKeyNode.Copy(src.arKeyNode);
		nCentCalcType = src.nCentCalcType;
		dCentX = src.dCentX;
		dCentY = src.dCentY;
		dCentZ = src.dCentZ;
		dNormalVecX = src.dNormalVecX;
		dNormalVecY = src.dNormalVecY;
		dNormalVecZ = src.dNormalVecZ;

		return *this;
	}
};

struct T_VSEC_D_CH
{
	char strName[80];    // 
	CArray<T_ELEM_K, T_ELEM_K>arKeyElem;
	CArray<T_NODE_K, T_NODE_K>arKeyNode;
	int nCentCalcType;  // 도심 계산 방법 (=0: Auto, =1: User)
	double dCentX;      // centroid X (Global)
	double dCentY;      // centroid Y (Global)
	double dCentZ;      // centroid Z (Global)
	double dNormalVecX;    // 단면의 normal vector-x
	double dNormalVecY;    // 단면의 normal vector-y
	double dNormalVecZ;    // 단면의 normal vector-z

	void ConvertToString(T_VSEC_D& rData)
	{
		ConvertCharStr(strName, rData.strName, sizeof(strName));
		rData.arKeyElem.Copy(arKeyElem);
		rData.arKeyNode.Copy(arKeyNode);
		rData.nCentCalcType = nCentCalcType;
		rData.dCentX = dCentX;
		rData.dCentY = dCentY;
		rData.dCentZ = dCentZ;
		rData.dNormalVecX = dNormalVecX;
		rData.dNormalVecY = dNormalVecY;
		rData.dNormalVecZ = dNormalVecZ;
	}
	void ConvertToChar(T_VSEC_D& rData)
	{
		ConvertStrChar(rData.strName, strName, sizeof(strName));
		arKeyElem.Copy(rData.arKeyElem);
		arKeyNode.Copy(rData.arKeyNode);
		nCentCalcType = rData.nCentCalcType;
		dCentX = rData.dCentX;
		dCentY = rData.dCentY;
		dCentZ = rData.dCentZ;
		dNormalVecX = rData.dNormalVecX;
		dNormalVecY = rData.dNormalVecY;
		dNormalVecZ = rData.dNormalVecZ;
	}
};

struct T_VSEC_UDRD_D
{
	T_VSEC_K key;
	T_VSEC_D data;
};

struct T_VSEC_ITEM_UDRD_D
{
	T_VSEC_K key;
	T_ELEM_K elemkey;
	T_NODE_K nodekey;
};

struct T_VSEC_UDRD_D_CH
{
	T_VSEC_K key;
	T_VSEC_D_CH data;
};

struct T_VSEC_UDRD_D_CH_RW
{
	T_VSEC_K key;

	char strName[80];    // 
	int nCountElemKey;    // for Array
	int nCountNodeKey;    // for Array
	int nCentCalcType;  // 도심 계산 방법 (=0: Auto, =1: User)
	double dCentX;      // centroid X (Global)
	double dCentY;      // centroid Y (Global)
	double dCentZ;      // centroid Z (Global)
	double dNormalVecX;    // 단면의 normal vector-x
	double dNormalVecY;    // 단면의 normal vector-y
	double dNormalVecZ;    // 단면의 normal vector-z

	void GetVsec(T_VSEC_K& rKey, T_VSEC_D_CH& rData)
	{
		rKey=key;

		memcpy(rData.strName, strName, sizeof(strName));
		//int nCountElemKey;    // for Array
		//int nCountNodeKey;    // for Array
		rData.nCentCalcType = nCentCalcType; 
		rData.dCentX = dCentX;
		rData.dCentY = dCentY;
		rData.dCentZ = dCentZ;
		rData.dNormalVecX = dNormalVecX;
		rData.dNormalVecY = dNormalVecY;
		rData.dNormalVecZ = dNormalVecZ;
	}
	void SetVsec(const T_VSEC_K Key, const T_VSEC_D_CH& rData)
	{
		key=Key;

		memcpy(strName, rData.strName, sizeof(strName));
		nCountElemKey=(int)rData.arKeyElem.GetSize();
		nCountNodeKey=(int)rData.arKeyNode.GetSize();
		nCentCalcType = rData.nCentCalcType;
		dCentX = rData.dCentX;
		dCentY = rData.dCentY;
		dCentZ = rData.dCentZ;
		dNormalVecX = rData.dNormalVecX;
		dNormalVecY = rData.dNormalVecY;
		dNormalVecZ = rData.dNormalVecZ;
	}
};
struct T_VSEC_UNIT
{
	T_VSEC_UNIT()
	{
		dCentX=D_UNITSYS_BASE_LENGTH;
		dCentY=D_UNITSYS_BASE_LENGTH;
		dCentZ=D_UNITSYS_BASE_LENGTH;
	}
	int dCentX;
	int dCentY;
	int dCentZ;
};
#define HASHSIZEVSEC 997
#define HASHSIZEVSECITEM HASHSIZEELEM

// virtual beam
#define T_VBEM_K unsigned int
struct T_VBEM_D
{
	T_VSEC_K Vsec1;
	T_VSEC_K Vsec2;

	void Initialize()
	{
		Vsec1=0;
		Vsec2=0;
	}
};

struct T_VBEM_UDRD_D
{
	T_VBEM_K key;
	T_VBEM_D data;
};
struct T_VBEM_UNIT
{
	T_VBEM_UNIT()
	{
		Vsec1=D_UNITSYS_NONE;
		Vsec2=D_UNITSYS_NONE;
	}
	int Vsec1;
	int Vsec2;
};
#define HASHSIZEVBEM 997



/// brep /// Batch Report(for iGen)
#define T_BREP_K unsigned int
struct T_BREP_D
{
	int nReportCase;
	CString strName;
// 	UINT nLoadMinMaxType;
// 	UINT nLoadCaseType;
// 	UINT nLoadCaseKey;
	int nElemAvg;  // 0:Element, 1:Avg. Nodal
	int nElemCell; // 0:Element, 1:Cell
	double dCellLength;

	BOOL bMomentChk;
	int  nMomPos;	// 0:Top, 1:Bottom 2:Both
	int  nMonDir;	// 0:Axis1, 1:Axis2, 2:Both

	BOOL bShearChk;
	int  nShearPos;	// 0:Top, 1:Bottom 2:Both
	
	BOOL  bAdhesiveChk;
	int   nAdhesivePos;	// 0:Top, 1:Bottom 2:Both

	CArray<T_ELEM_K, T_ELEM_K>arKeyElem;
	
	void Initialize()
	{
		nReportCase = 0;
		strName.Empty();
		arKeyElem.RemoveAll();

// 		nLoadMinMaxType = 0;
// 		nLoadCaseType = 0;
// 		nLoadCaseKey = 0;
		nElemAvg = 0;
		nElemCell = 0;
		dCellLength = 0.0;

		bMomentChk = FALSE;
		nMomPos = 0;
		nMonDir = 0;

		bShearChk = FALSE;
		nShearPos = 0;

		bAdhesiveChk = FALSE;
		nAdhesivePos = 0;

	}
	T_BREP_D()
	{
		Initialize();
	}
	T_BREP_D(T_BREP_D& src)
	{
		*this = src;
	}
	T_BREP_D& operator=(T_BREP_D& src)
	{
		nReportCase = src.nReportCase;
		strName = src.strName;
		arKeyElem.Copy(src.arKeyElem);
		
// 		nLoadMinMaxType = src.nLoadMinMaxType;
// 		nLoadCaseType = src.nLoadCaseType;
// 		nLoadCaseKey = src.nLoadCaseKey;
		nElemAvg = src.nElemAvg;
		nElemCell = src.nElemCell;
		dCellLength = src.dCellLength;

		bMomentChk = src.bMomentChk;
		nMomPos = src.nMomPos;
		nMonDir = src.nMonDir;

		bShearChk = src.bShearChk;
		nShearPos = src.nShearPos;

		bAdhesiveChk = src.bAdhesiveChk;
		nAdhesivePos = src.nAdhesivePos;

		return *this;
	}
};

struct T_BREP_D_CH
{
	int nReportCase;
	char strName[80];

// 	UINT nLoadMinMaxType;
// 	UINT nLoadCaseType;
// 	UINT nLoadCaseKey;
	int nElemAvg;  // 0:Element, 1:Avg. Nodal
	int nElemCell; // 0:Element, 1:Cell
	double dCellLength;

	BOOL bMomentChk;
	int  nMomPos;	// 0:Top, 1:Bottom 2:Both
	int  nMonDir;	// 0:Axis1, 1:Axis2, 2:Both

	BOOL bShearChk;
	int  nShearPos;	// 0:Top, 1:Bottom 2:Both

	BOOL  bAdhesiveChk;
	int   nAdhesivePos;	// 0:Top, 1:Bottom 2:Both

	CArray<T_ELEM_K, T_ELEM_K>arKeyElem;
	void ConvertToString(T_BREP_D& rData)
	{
		rData.nReportCase = nReportCase;
		ConvertCharStr(strName, rData.strName, sizeof(strName));
		rData.arKeyElem.Copy(arKeyElem);
		
// 		rData.nLoadMinMaxType = nLoadMinMaxType;
// 		rData.nLoadCaseType = nLoadCaseType;
// 		rData.nLoadCaseKey = nLoadCaseKey;
		rData.nElemAvg = nElemAvg;
		rData.nElemCell = nElemCell;
		rData.dCellLength = dCellLength;

		rData.bMomentChk = bMomentChk;
		rData.nMomPos = nMomPos;
		rData.nMonDir = nMonDir;

		rData.bShearChk = bShearChk;
		rData.nShearPos = nShearPos;

		rData.bAdhesiveChk = bAdhesiveChk;
		rData.nAdhesivePos = nAdhesivePos;

	}
	void ConvertToChar(T_BREP_D& rData)
	{
		nReportCase = rData.nReportCase;
		ConvertStrChar(rData.strName, strName, sizeof(strName));
		arKeyElem.Copy(rData.arKeyElem);
		
// 		nLoadMinMaxType = rData.nLoadMinMaxType;
// 		nLoadCaseType = rData.nLoadCaseType;
// 		nLoadCaseKey = rData.nLoadCaseKey;
		nElemAvg = rData.nElemAvg;
		nElemCell = rData.nElemCell;
		dCellLength = rData.dCellLength;

		bMomentChk = rData.bMomentChk;
		nMomPos = rData.nMomPos;
		nMonDir = rData.nMonDir;

		bShearChk = rData.bShearChk;
		nShearPos = rData.nShearPos;

		bAdhesiveChk = rData.bAdhesiveChk;
		nAdhesivePos = rData.nAdhesivePos;

	}
	
};

struct T_BREP_UDRD_D
{
	T_BREP_K key;
	T_BREP_D data;
};
struct T_BREP_ITEM_UDRD_D
{
	T_BREP_K key;
	T_ELEM_K elemkey;
};

struct T_BREP_UDRD_D_CH
{
	T_BREP_K key;
	T_BREP_D_CH data;
};

struct T_BREP_UDRD_D_CH_RW_945
{
	T_BREP_K key;
	int nReportCase;
	char strName[80];
	int nCountElemKey;

	UINT nLoadMinMaxType;
	UINT nLoadCaseType;
	UINT nLoadCaseKey;
	int nElemAvg;  // 0:Element, 1:Avg. Nodal
	int nElemCell; // 0:Element, 1:Cell
	double dCellLength;

	BOOL bMomentChk;
	int  nMomPos;	// 0:Top, 1:Bottom 2:Both
	int  nMonDir;	// 0:Axis1, 1:Axis2, 2:Both

	BOOL bShearChk;
	int  nShearPos;	// 0:Top, 1:Bottom 2:Both

	BOOL  bAdhesiveChk;
	int   nAdhesivePos;	// 0:Top, 1:Bottom 2:Both
};

struct T_BREP_UDRD_D_CH_RW
{
	T_BREP_K key;
	int nReportCase;
	char strName[80];
	int nCountElemKey;
	
// 	UINT nLoadMinMaxType;
// 	UINT nLoadCaseType;
// 	UINT nLoadCaseKey;
	int nElemAvg;  // 0:Element, 1:Avg. Nodal
	int nElemCell; // 0:Element, 1:Cell
	double dCellLength;

	BOOL bMomentChk;
	int  nMomPos;	// 0:Top, 1:Bottom 2:Both
	int  nMonDir;	// 0:Axis1, 1:Axis2, 2:Both

	BOOL bShearChk;
	int  nShearPos;	// 0:Top, 1:Bottom 2:Both

	BOOL  bAdhesiveChk;
	int   nAdhesivePos;	// 0:Top, 1:Bottom 2:Both

	void GetBrep(T_BREP_K& rKey, T_BREP_D_CH& rData)
	{
		rKey = key;
		rData.nReportCase = nReportCase;
		memcpy(rData.strName, strName, sizeof(strName));
		
// 		rData.nLoadMinMaxType = nLoadMinMaxType;
// 		rData.nLoadCaseType = nLoadCaseType;
// 		rData.nLoadCaseKey = nLoadCaseKey;
		rData.nElemAvg = nElemAvg;
		rData.nElemCell = nElemCell;
		rData.dCellLength = dCellLength;

		rData.bMomentChk = bMomentChk;
		rData.nMomPos = nMomPos;
		rData.nMonDir = nMonDir;

		rData.bShearChk = bShearChk;
		rData.nShearPos = nShearPos;

		rData.bAdhesiveChk = bAdhesiveChk;
		rData.nAdhesivePos = nAdhesivePos;

	}
	void SetBrep(const T_BREP_K Key, const T_BREP_D_CH& rData)
	{
		key = Key;
		nReportCase = rData.nReportCase;
		memcpy(strName, rData.strName, sizeof(strName));
		nCountElemKey = (int)rData.arKeyElem.GetSize();
		
// 		nLoadMinMaxType = rData.nLoadMinMaxType;
// 		nLoadCaseType = rData.nLoadCaseType;
// 		nLoadCaseKey = rData.nLoadCaseKey;
		nElemAvg = rData.nElemAvg;
		nElemCell = rData.nElemCell;
		dCellLength = rData.dCellLength;

		bMomentChk = rData.bMomentChk;
		nMomPos = rData.nMomPos;
		nMonDir = rData.nMonDir;

		bShearChk = rData.bShearChk;
		nShearPos = rData.nShearPos;

		bAdhesiveChk = rData.bAdhesiveChk;
		nAdhesivePos = rData.nAdhesivePos;

	}

	void Convert945(const T_BREP_UDRD_D_CH_RW_945& Data)
	{
		key = Data.key;
		nReportCase = Data.nReportCase;
		memcpy(strName, Data.strName, sizeof(strName));
		nCountElemKey = Data.nCountElemKey;

		nElemAvg = Data.nElemAvg;
		nElemCell = Data.nElemCell;
		dCellLength = Data.dCellLength;

		bMomentChk = Data.bMomentChk;
		nMomPos = Data.nMomPos;
		nMonDir = Data.nMonDir;

		bShearChk = Data.bShearChk;
		nShearPos = Data.nShearPos;

		bAdhesiveChk = Data.bAdhesiveChk;
		nAdhesivePos = Data.nAdhesivePos;
	}
};
struct T_BREP_UNIT
{
	T_BREP_UNIT()
	{
		dCellLength = D_UNITSYS_BASE_LENGTH;

	}
	int dCellLength;
};
#define HASHSIZEBREP 101

#pragma pack(pop)

#endif  // __DB_ST_DT_GRUP_H__
