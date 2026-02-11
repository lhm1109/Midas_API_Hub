#ifndef __DB_ST_DT_DY2E_H__
#define __DB_ST_DT_DY2E_H__

#define MIDAS_SDK

#pragma pack(push, 1)

/// analysis type ///
#define T_ATYP_K unsigned int
struct T_ATYP_D
{
	CString strName;
	CString Description;  
	int     nAnalType;           // 0=Static, 1=Modal, 2=Time History, 3=Response Spectrum
	UINT    ThisKey;             // Time History Load Case Key (nAnalType == 2인 경우 사용)
	UINT    SplcKey;             // Response Spectrum Load Case Key (nAnalType == 3인 경우 사용)   
	int     nModalAnalMethod;    // 0=None, 1=HOUS, 2=SUBS, 3=COMP
	int     nResponseMethod;     // 0=None, 1=MODE, 2=DIRE, 3=COMP, 4=FREQ
	int     nDampingType;        // 0=None, 1=MODE, 2=STRA, 3=PROP, 4=PRO2, 5=HYST 
	int     nExcitationType;     // 0=EART, 1=GRD
	int     nDynamicDataType;    // 0=ACC, 1=SA, 2=SV, 3=SD, 4=TGFC, 5=FGFC
	int     nExternalForceType;  // 0=EQFC, 1=UNIF, 2=MULT, 3=MULT1, 4=MULT2, 5=MULT3, 6=MULT4
	int     nIntegrationMethod;  // 0=NEWM, 1=WILS, 2=DUHA, 3=NIGA
	double  dBetaInverse;
	double  dWilsonTheta;
	int     nModalSuperMethod;   // 0=SRSS, 1=CQC
	BOOL    bImportInitialStress;
	unsigned int nSeq;    // 순번
	void Initialize()
	{
		strName.Empty();
		Description.Empty();
		nAnalType            = 2;
		ThisKey              = 0;
		SplcKey              = 0;    
		nModalAnalMethod     = 2;            
		nResponseMethod      = 1;         
		nDampingType         = 1;            
		nExcitationType      = 0;         
		nDynamicDataType     = 0;       
		nExternalForceType   = 0;      
		nIntegrationMethod   = 0;      
		dBetaInverse         = 4.;    
		dWilsonTheta         = 1.4;    
		nModalSuperMethod    = 0;       
		bImportInitialStress = FALSE;
		nSeq                 = 0;
	}  
	T_ATYP_D()
	{
	}
	T_ATYP_D(T_ATYP_D& src)
	{
		*this = src;
	}
	T_ATYP_D& operator=(T_ATYP_D& src)
	{
		strName              = src.strName;
		Description          = src.Description;
		nAnalType            = src.nAnalType           ;
		ThisKey              = src.ThisKey             ;
		SplcKey              = src.SplcKey             ;    
		nModalAnalMethod     = src.nModalAnalMethod    ;            
		nResponseMethod      = src.nResponseMethod     ;         
		nDampingType         = src.nDampingType        ;            
		nExcitationType      = src.nExcitationType     ;         
		nDynamicDataType     = src.nDynamicDataType    ;       
		nExternalForceType   = src.nExternalForceType  ;      
		nIntegrationMethod   = src.nIntegrationMethod  ;      
		dBetaInverse         = src.dBetaInverse        ;    
		dWilsonTheta         = src.dWilsonTheta        ;    
		nModalSuperMethod    = src.nModalSuperMethod   ;       
		bImportInitialStress = src.bImportInitialStress;
		nSeq                 = src.nSeq                ;
		return *this;
	}
};
struct T_ATYP_D_CH
{
	char    strName[20];
	char    Description[80];  
	int     nAnalType;           // 0=Static, 1=Modal, 2=Time History, 3=Response Spectrum
	UINT    ThisKey;             // Time History Load Case Key (nAnalType == 2인 경우 사용)
	UINT    SplcKey;             // Response Spectrum Load Case Key (nAnalType == 3인 경우 사용)   
	int     nModalAnalMethod;    // 0=None, 1=HOUS, 2=SUBS, 3=COMP
	int     nResponseMethod;     // 0=None, 1=MODE, 2=DIRE, 3=COMP, 4=FREQ
	int     nDampingType;        // 0=None, 1=MODE, 2=STRA, 3=PROP, 4=PRO2, 5=HYST 
	int     nExcitationType;     // 0=EART, 1=GRD
	int     nDynamicDataType;    // 0=ACC, 1=SA, 2=SV, 3=SD, 4=TGFC, 5=FGFC
	int     nExternalForceType;  // 0=EQFC, 1=UNIF, 2=MULT, 3=MULT1, 4=MULT2, 5=MULT3, 6=MULT4
	int     nIntegrationMethod;  // 0=NEWM, 1=WILS, 2=DUHA, 3=NIGA
	double  dBetaInverse;
	double  dWilsonTheta;
	int     nModalSuperMethod;   // 0=SRSS, 1=CQC
	BOOL    bImportInitialStress;
	unsigned int nSeq;    // 순번
	void ConvertToString(T_ATYP_D& rData)
	{
		ConvertCharStr(strName, rData.strName, sizeof(strName));
		ConvertCharStr(Description, rData.Description, sizeof(Description));
		rData.nAnalType            = nAnalType           ;
		rData.ThisKey              = ThisKey             ;
		rData.SplcKey              = SplcKey             ;    
		rData.nModalAnalMethod     = nModalAnalMethod    ;            
		rData.nResponseMethod      = nResponseMethod     ;         
		rData.nDampingType         = nDampingType        ;            
		rData.nExcitationType      = nExcitationType     ;         
		rData.nDynamicDataType     = nDynamicDataType    ;       
		rData.nExternalForceType   = nExternalForceType  ;      
		rData.nIntegrationMethod   = nIntegrationMethod  ;      
		rData.dBetaInverse         = dBetaInverse        ;    
		rData.dWilsonTheta         = dWilsonTheta        ;    
		rData.nModalSuperMethod    = nModalSuperMethod   ;       
		rData.bImportInitialStress = bImportInitialStress;
		rData.nSeq                 = nSeq                ;    
	}
	void ConvertToChar(T_ATYP_D& rData)
	{
		ConvertStrChar(rData.strName, strName, sizeof(strName));
		ConvertStrChar(rData.Description, Description, sizeof(Description));
		nAnalType            = rData.nAnalType           ;
		ThisKey              = rData.ThisKey             ;
		SplcKey              = rData.SplcKey             ;    
		nModalAnalMethod     = rData.nModalAnalMethod    ;            
		nResponseMethod      = rData.nResponseMethod     ;         
		nDampingType         = rData.nDampingType        ;            
		nExcitationType      = rData.nExcitationType     ;         
		nDynamicDataType     = rData.nDynamicDataType    ;       
		nExternalForceType   = rData.nExternalForceType  ;      
		nIntegrationMethod   = rData.nIntegrationMethod  ;      
		dBetaInverse         = rData.dBetaInverse        ;    
		dWilsonTheta         = rData.dWilsonTheta        ;    
		nModalSuperMethod    = rData.nModalSuperMethod   ;       
		bImportInitialStress = rData.bImportInitialStress;
		nSeq                 = rData.nSeq                ;
	}
};
struct T_ATYP_UDRD_D
{
	T_ATYP_K key;
	T_ATYP_D data;
};
struct T_ATYP_UDRD_D_CH
{
	T_ATYP_K key;
	T_ATYP_D_CH data;
};
struct T_ATYP_UNIT
{
	T_ATYP_UNIT()
	{
		nNoUnit=D_UNITSYS_NONE;    
	}
	int nNoUnit;  
};
#define HASHSIZEATYP 11

/// data format ///
#define T_METH_K unsigned int
struct T_METH_D
{
	int nCardType;    // 0=double, 1=single
	int nVectType;    // 0=V-vector, 1=beta angle
	void Initialize()
	{
		nCardType = 0;               
		nVectType = 0;            
	}  
};
struct T_METH_UDRD_D
{
	T_METH_D data;
};
struct T_METH_UNIT
{
	T_METH_UNIT()
	{
		nNoUnit=D_UNITSYS_NONE;    
	}
	int nNoUnit;  
};
#define HASHSIZEMETH 1

/// Nonlinear Output Option ///
#define T_NLOP_K unsigned int
struct T_NLOP_D
{
	int nPrintOut;    
	int nFileOut;    
	void Initialize()
	{
		nPrintOut = 1;               
		nFileOut = 1;            
	}  
};
struct T_NLOP_UDRD_D
{
	T_NLOP_D data;
};
struct T_NLOP_UNIT
{
	T_NLOP_UNIT()
	{
		nNoUnit=D_UNITSYS_NONE;    
	}
	int nNoUnit;  
};
#define HASHSIZENLOP 1

/// Eigen Value Analysis Control ///
#define T_EVCT_K unsigned int
struct T_EVCT_D
{
	double dFreqShift;  
	int    nNumPrint;   
	int    nNumFile;    
	int    nNumItr;     
	double dTolerance;  
	void Initialize()
	{
		dFreqShift = 0.;
		nNumPrint  = 0; 
		nNumFile   = 0;
		nNumItr    = 10;
		dTolerance = 1.e-6;
	}  
};
struct T_EVCT_UDRD_D
{
	T_EVCT_D data;
};
struct T_EVCT_UNIT
{
	T_EVCT_UNIT()
	{
		nNoUnit=D_UNITSYS_NONE;    
	}
	int nNoUnit;  
};
#define HASHSIZEEVCT 1

/// model degree of freedom ///
#define T_MFRD_K unsigned int
struct T_MFRD_D
{
	unsigned char Freedom[8];    // 0-5 까지만 사용 '0'=fix '1'=free  
	void Initialize()
	{
		for(int i=0; i<8; i++) Freedom[i]='1';    
	}  
};
struct T_MFRD_UDRD_D
{
	T_MFRD_D data;
};
struct T_MFRD_UNIT
{
	T_MFRD_UNIT()
	{
		nNoUnit=D_UNITSYS_NONE;    
	}
	int nNoUnit;  
};
#define HASHSIZEMFRD 1

/// Print Option ///
#define T_PRNT_K unsigned int
struct T_PRNT_D
{
	BOOL bInput[2];
	BOOL bResult[15];  
	void Initialize()
	{
		for(int i = 0; i < 2; i++) bInput[i] = FALSE;
		for(int i = 0; i < 15; i++) bResult[i] = FALSE;    
	}  
};
struct T_PRNT_UDRD_D
{
	T_PRNT_D data;
};
struct T_PRNT_UNIT
{
	T_PRNT_UNIT()
	{
		nNoUnit=D_UNITSYS_NONE;    
	}
	int nNoUnit;  
};
#define HASHSIZEPRNT 1

// Nonlinear Analysis Control
#define T_NONL_K unsigned int
struct T_NONL_BASE
{
	UINT ElemKey;
	int  nNonlType;  // 0=AVER, 1=IGRD, 2=JGRD  
	void Initialize()
	{
		ElemKey   = 0;
		nNonlType = 0;
	}
};
struct T_NONL_D
{
	CString strName;
	CString Description;  
	UINT ThisKey;
	int nNonlAnalMethod;   // 0=Newton-Raphson, 1=Modified Newton-Raphson
	int nUnequalForce;     // 0=FORC, 1=Desert
	int nConvergence;      // 0=CONV, 1=Do not
	int nNumConvergence;
	double dTolerance;
	int nTotalNonlType;       // 0=AVER, 1=IGRD, 2=JGRD  
	BOOL bDamping;
	CArray<T_NONL_BASE, T_NONL_BASE&> aElemAssign;  
	unsigned int nSeq;
	void Initialize()
	{
		strName.Empty();
		Description.Empty();
		ThisKey         = 0;
		nNonlAnalMethod = 0;
		nUnequalForce   = 0;
		nConvergence    = 1;
		nNumConvergence = 4;
		dTolerance      = 0.01;
		nTotalNonlType  = 0;
		bDamping        = FALSE;
		aElemAssign.RemoveAll();    
		nSeq            = 0;
	}
	T_NONL_D() {}
	T_NONL_D(T_NONL_D& rData) { *this = rData; }
	T_NONL_D& operator=(T_NONL_D& src)
	{
		strName         = src.strName        ;
		Description     = src.Description    ;    
		ThisKey         = src.ThisKey        ;
		nNonlAnalMethod = src.nNonlAnalMethod;
		nUnequalForce   = src.nUnequalForce  ;
		nConvergence    = src.nConvergence   ;
		nNumConvergence = src.nNumConvergence;
		dTolerance      = src.dTolerance     ;    
		nTotalNonlType  = src.nTotalNonlType ;
		bDamping        = src.bDamping       ;
		aElemAssign.Copy(src.aElemAssign)    ;    
		nSeq            = src.nSeq           ; 
		return *this;
	}
};
struct T_NONL_D_CH
{
	char strName[20];
	char Description[80];  
	UINT ThisKey;
	int nNonlAnalMethod;   // 0=Newton-Raphson, 1=Modified Newton-Raphson
	int nUnequalForce;     // 0=FORC, 1=Desert
	int nConvergence;      // 0=CONV, 1=Do not
	int nNumConvergence;
	double dTolerance;
	int nTotalNonlType;       // 0=AVER, 1=IGRD, 2=JGRD  
	BOOL bDamping;
	CArray<T_NONL_BASE, T_NONL_BASE&> aElemAssign;
	unsigned int nSeq;
	void ConvertToString(T_NONL_D& rData)
	{
		ConvertCharStr(strName, rData.strName, sizeof(strName));
		ConvertCharStr(Description, rData.Description, sizeof(Description));
		rData.ThisKey           = ThisKey        ;
		rData.nNonlAnalMethod   = nNonlAnalMethod;
		rData.nUnequalForce     = nUnequalForce  ;    
		rData.nConvergence      = nConvergence   ;            
		rData.nNumConvergence   = nNumConvergence;         
		rData.dTolerance        = dTolerance     ;            
		rData.nTotalNonlType    = nTotalNonlType ;         
		rData.bDamping          = bDamping       ;       
		rData.aElemAssign.Copy(aElemAssign)      ;      
		rData.nSeq              = nSeq           ;
	}
	void ConvertToChar(T_NONL_D& rData)
	{
		ConvertStrChar(rData.strName, strName, sizeof(strName));
		ConvertStrChar(rData.Description, Description, sizeof(Description));
		ThisKey           = rData.ThisKey        ;
		nNonlAnalMethod   = rData.nNonlAnalMethod;
		nUnequalForce     = rData.nUnequalForce  ;    
		nConvergence      = rData.nConvergence   ;            
		nNumConvergence   = rData.nNumConvergence;         
		dTolerance        = rData.dTolerance     ;            
		nTotalNonlType    = rData.nTotalNonlType ;         
		bDamping          = rData.bDamping       ;       
		aElemAssign.Copy(rData.aElemAssign)      ;      
		nSeq              = rData.nSeq           ;
	}
};
struct T_NONL_UDRD_D
{
	T_NONL_K key;
	T_NONL_D data;
};
struct T_NONL_UDRD_D_CH
{
	T_NONL_K key;
	T_NONL_D_CH data;
};
struct T_NONL_UDRD_D_CH_RW
{
	T_NONL_K key;
	char strName[20];
	char Description[80];  
	UINT ThisKey;
	int nNonlAnalMethod;   // 0=Newton-Raphson, 1=Modified Newton-Raphson
	int nUnequalForce;     // 0=FORC, 1=Desert
	int nConvergence;      // 0=CONV, 1=Do not
	int nNumConvergence;
	double dTolerance;
	int nTotalNonlType;       // 0=AVER, 1=IGRD, 2=JGRD  
	BOOL bDamping;  
	unsigned int nSeq;
	unsigned int nElemAssign;
	void GetNonl(T_NONL_K& rKey, T_NONL_D_CH& rData)
	{
		rKey                  = key;
		memcpy(rData.strName, strName, sizeof(strName));
		memcpy(rData.Description, Description, sizeof(Description));    
		rData.ThisKey         = ThisKey        ;
		rData.nNonlAnalMethod = nNonlAnalMethod;
		rData.nUnequalForce   = nUnequalForce  ;
		rData.nConvergence    = nConvergence   ;
		rData.nNumConvergence = nNumConvergence;
		rData.dTolerance      = dTolerance     ;    
		rData.nTotalNonlType  = nTotalNonlType ;
		rData.bDamping        = bDamping       ;  
		rData.nSeq            = nSeq           ;
	}
	void SetNonl(const T_NONL_K Key, const T_NONL_D_CH& rData)
	{
		key             = Key;
		memcpy(strName, rData.strName, sizeof(strName));
		memcpy(Description, rData.Description, sizeof(Description));
		ThisKey         = rData.ThisKey        ;
		nNonlAnalMethod = rData.nNonlAnalMethod;
		nUnequalForce   = rData.nUnequalForce  ;
		nConvergence    = rData.nConvergence   ;
		nNumConvergence = rData.nNumConvergence;
		dTolerance      = rData.dTolerance     ;
		nTotalNonlType  = rData.nTotalNonlType ;
		bDamping        = rData.bDamping       ;
		nSeq            = rData.nSeq           ;
		nElemAssign     = (int)rData.aElemAssign.GetSize();    
	}
};
struct T_NONL_UNIT
{
	T_NONL_UNIT()
	{
		nNoUnit = D_UNITSYS_NONE;
	}
	int nNoUnit;
};
#define HASHSIZENONL 11

// Multiple Point Constraint 
#define T_MPCT_K unsigned int

struct T_MPCT_D
{
	CString strName;
	CString Description;  
	int     nMethod;         // 0=Rigid, 1=Relative, 2=Etc
	UINT    NodeKeyTgt;
	UINT    NodeKeyRgd;  
	UINT    NodeKeyRel[2];
	UINT    NodeKeyEtc[4];
	int     nDirTgt;         // 0=DX, 1=DY, 2=DZ, 3=RX, 4=RY, 5=RZ
	int     nDirRgd;         // 0=DX, 1=DY, 2=DZ, 3=RX, 4=RY, 5=RZ
	int     nDirRel;         // 0=DX, 1=DY, 2=DZ, 3=RX, 4=RY, 5=RZ  
	int     nDirEtc[4];      // 0=DX, 1=DY, 2=DZ, 3=RX, 4=RY, 5=RZ
	double  dAi[4];
	BOOL    bUseEtc[4];
	unsigned int nSeq;    // 순번
	void Initialize()
	{
		strName.Empty();
		Description.Empty();
		nMethod = 0;
		NodeKeyTgt = 0;
		NodeKeyRgd = 0;

		for(int i = 0; i < 2; i++) NodeKeyRel[i] = 0;    
		for(int i = 0; i < 4; i++) NodeKeyEtc[i] = 0;
		nDirTgt = 0;
		nDirRgd = 0;
		nDirRel = 0;
		for(int i = 0; i < 4; i++) nDirEtc[i] = 0;
		for(int i = 0; i < 4; i++) dAi[i] = 0.;
		for(int i = 0; i < 4; i++) bUseEtc[i] = FALSE;
		nSeq = 0;
	}
	T_MPCT_D()
	{
	}
	T_MPCT_D(T_MPCT_D& src)
	{
		*this = src;
	}
	T_MPCT_D& operator=(T_MPCT_D& src)
	{
		strName = src.strName;
		Description = src.Description;
		nMethod = src.nMethod;
		NodeKeyTgt = src.NodeKeyTgt;
		NodeKeyRgd = src.NodeKeyRgd;

		for(int i = 0; i < 2; i++) NodeKeyRel[i] = src.NodeKeyRel[i];    
		for(int i = 0; i < 4; i++) NodeKeyEtc[i] = src.NodeKeyEtc[i];
		nDirTgt = src.nDirTgt;
		nDirRgd = src.nDirRgd;
		nDirRel = src.nDirRel;
		for(int i = 0; i < 4; i++) nDirEtc[i] = src.nDirEtc[i];
		for(int i = 0; i < 4; i++) dAi[i] = src.dAi[i];
		for(int i = 0; i < 4; i++) bUseEtc[i] = src.bUseEtc[i];
		nSeq = src.nSeq;
		return *this;
	}  
};
struct T_MPCT_D_CH
{
	char    strName[20];
	char    Description[80];  
	int     nMethod;         // 0=Rigid, 1=Relative, 2=Etc
	UINT    NodeKeyTgt;
	UINT    NodeKeyRgd;  
	UINT    NodeKeyRel[2];
	UINT    NodeKeyEtc[4];
	int     nDirTgt;
	int     nDirRgd;
	int     nDirRel;
	int     nDirEtc[4];  
	double  dAi[4];
	BOOL    bUseEtc[4];
	unsigned int nSeq;    // 순번  
	void ConvertToString(T_MPCT_D& rData)
	{
		ConvertCharStr(strName, rData.strName, sizeof(strName));
		ConvertCharStr(Description, rData.Description, sizeof(Description));
		rData.nMethod = nMethod;
		rData.NodeKeyTgt = NodeKeyTgt;
		rData.NodeKeyRgd = NodeKeyRgd;    

		for(int i = 0; i < 2; i++) rData.NodeKeyRel[i] = NodeKeyRel[i];    
		for(int i = 0; i < 4; i++) rData.NodeKeyEtc[i] = NodeKeyEtc[i];
		rData.nDirTgt = nDirTgt;
		rData.nDirRgd = nDirRgd;
		rData.nDirRel = nDirRel;
		for(int i = 0; i < 4; i++) rData.nDirEtc[i] = nDirEtc[i];
		for(int i = 0; i < 4; i++) rData.dAi[i] = dAi[i];
		for(int i = 0; i < 4; i++) rData.bUseEtc[i] = bUseEtc[i];
		rData.nSeq = nSeq;
	}
	void ConvertToChar(T_MPCT_D& rData)
	{
		ConvertStrChar(rData.strName, strName, sizeof(strName));
		ConvertStrChar(rData.Description, Description, sizeof(Description));
		nMethod = rData.nMethod;
		NodeKeyTgt = rData.NodeKeyTgt;
		NodeKeyRgd = rData.NodeKeyRgd;

		for(int i = 0; i < 2; i++) NodeKeyRel[i] = rData.NodeKeyRel[i];    
		for(int i = 0; i < 4; i++) NodeKeyEtc[i] = rData.NodeKeyEtc[i];
		nDirTgt = rData.nDirTgt;
		nDirRgd = rData.nDirRgd;
		nDirRel = rData.nDirRel;
		for(int i = 0; i < 4; i++) nDirEtc[i] = rData.nDirEtc[i];
		for(int i = 0; i < 4; i++) dAi[i] = rData.dAi[i];
		for(int i = 0; i < 4; i++) bUseEtc[i] = rData.bUseEtc[i];
		nSeq = rData.nSeq;
	}
};
struct T_MPCT_UDRD_D
{
	T_MPCT_K key;
	T_MPCT_D data;
};
struct T_MPCT_UDRD_D_CH
{
	T_MPCT_K key;
	T_MPCT_D_CH data;
};
struct T_MPCT_UNIT
{
	T_MPCT_UNIT()
	{
		nNoUnit = D_UNITSYS_NONE;    
	}
	int nNoUnit;  
};
#define HASHSIZEMPCT 11

#define T_MNTH_K unsigned int
struct T_MNTH_D
{
	int nPosition;   // 0=None, 1=IGRD, 2=JGRD
	void Initialize()
	{
		nPosition = 1;
	}  
};
struct T_MNTH_UDRD_D
{
	T_MNTH_K key;
	T_MNTH_D data;
};
struct T_MNTH_UNIT
{
	T_MNTH_UNIT()
	{
		nNoUnit = D_UNITSYS_NONE;
	}
	int nNoUnit;
};
#define HASHSIZEMNTH HASHSIZEELEM

#pragma pack(pop)

#endif  // __DB_ST_DT_DY2E_H__
