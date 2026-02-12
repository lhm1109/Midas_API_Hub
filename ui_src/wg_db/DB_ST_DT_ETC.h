#ifndef __DB_ST_DT_ETC_H__
#define __DB_ST_DT_ETC_H__

#define MIDAS_SDK

#pragma pack(push, 1)

/// project configuration ///
#define T_PJCF_K unsigned int
struct T_PJCF_D
{
	CString strProject;
	CString strRevision;
	CString strUser;
	CString strMail;
	CString strAddress;
	CString strPhone;
	CString strFax;
	CString strClient;
	CString strTitle;
	CString strReviewName[5];
	CString strReviewDate[5];
	CString strComment;
	int     nVer571Mark;      // 570, 571 구분 표시:사용안됨
	UINT    nExtra[1024];
	int     nExtra2Ver;       // v720: Extra2 데이터의 포맷 버전 (항상 저장시에 재기록)
	UINT    nExtra2[128];     // v720: Project 고유 번호 저장    (항상 저장시에 재기록)
	void Initialize()
	{
		strProject.Empty();
		strRevision.Empty();
		strUser.Empty();
		strUser=AfxGetApp()->GetProfileString(_T("ENV_GENERAL"),_T("UserName"),strUser);
		strMail.Empty();
		strAddress.Empty();
		strPhone.Empty();
		strFax.Empty();
		strClient.Empty();
		strTitle.Empty();
		for(int i=0; i<5; i++)
		{
			strReviewName[i].Empty();
			strReviewDate[i].Empty();
		}
		strComment.Empty();
		nVer571Mark = 1;
		memset(nExtra, 0, sizeof(nExtra));
		nExtra2Ver = 0;
		memset(nExtra2, 0, sizeof(nExtra2));
	}
};
struct T_PJCF_D_CH_41
{
	char strProject[80];
	char strUser[80];
	int nStructureType;   // 0:3-D  1:X-Z Plane  2: Y-Z Plane  3: X-Y Plane  4:XYZRXRY
	int nStructureMass;   // 0:Not Convert  1:X,Y,Z  2:X,y
	double dblGravity;
	double dblInitTemp;
	BOOL bAutoConstraintNormal;
	BOOL bAutoConstraintRotation;
	int nTrussIteration;
	double dblTrussTolerance;
};
struct T_PJCF_D_CH_40
{
	char strProject[80];
	char strUser[80];
	int nStructureType;   // 0:3-D  1:X-Z Plane  2: Y-Z Plane  3: X-Y Plane  4:XYZRXRY
	int nStructureMass;   // 0:Not Convert  1:X,Y,Z  2:X,y
	double dblGravity;
	BOOL bAutoConstraintNormal;
	BOOL bAutoConstraintRotation;
	int nTrussIteration;
	double dblTrussTolerance;
};
struct T_PJCF_D_CH_570
{
	char strProject[80];
	char strRevision[80];
	char strUser[80];
	char strMail[80];
	char strAddress[80];
	char strPhone[80];
	char strFax[80];
	char strClient[80];
	char strTitle[80];
	char strReviewName[5][80];
	char strReviewDate[5][80];
	char strComment[500];
};
struct T_PJCF_D_CH_712
{
	char strProject[80];
	char strRevision[80];
	char strUser[80];
	char strMail[80];
	char strAddress[80];
	char strPhone[80];
	char strFax[80];
	char strClient[80];
	char strTitle[80];
	char strReviewName[5][80];
	char strReviewDate[5][80];
	char strComment[500];
	int  nVer571Mark;
	UINT nExtra[1024];
};
struct T_PJCF_D_CH
{
	char strProject[80];
	char strRevision[80];
	char strUser[80];
	char strMail[80];
	char strAddress[80];
	char strPhone[80];
	char strFax[80];
	char strClient[80];
	char strTitle[80];
	char strReviewName[5][80];
	char strReviewDate[5][80];
	char strComment[500];
	int  nVer571Mark;
	UINT nExtra[1024];
	int  nExtra2Ver;       
	UINT nExtra2[128];     
	void Convert40(T_PJCF_D_CH_40& rData)
	{
		memcpy(strProject, rData.strProject, __min(sizeof(strProject), sizeof(rData.strProject)));
		memcpy(strUser, rData.strUser, __min(sizeof(strUser), sizeof(rData.strUser)));
		memset(strRevision, ' ', sizeof(strRevision));
		memset(strMail, ' ', sizeof(strMail));
		memset(strAddress, ' ', sizeof(strAddress));
		memset(strPhone, ' ', sizeof(strPhone));
		memset(strFax, ' ', sizeof(strFax));
		memset(strClient, ' ', sizeof(strClient));
		memset(strTitle, ' ', sizeof(strTitle));
		memset(strReviewName, ' ', sizeof(strReviewName));
		memset(strReviewDate, ' ', sizeof(strReviewDate));
		memset(strComment, ' ', sizeof(strComment));
		nVer571Mark = 0;
		memset(nExtra, 0, sizeof(nExtra));
		nExtra2Ver = 0;
		memset(nExtra2, 0, sizeof(nExtra2));
	}
	void Convert41(T_PJCF_D_CH_41& rData)
	{
		memcpy(strProject, rData.strProject, __min(sizeof(strProject), sizeof(rData.strProject)));
		memcpy(strUser, rData.strUser, __min(sizeof(strUser), sizeof(rData.strUser)));
		memset(strRevision, ' ', sizeof(strRevision));
		memset(strMail, ' ', sizeof(strMail));
		memset(strAddress, ' ', sizeof(strAddress));
		memset(strPhone, ' ', sizeof(strPhone));
		memset(strFax, ' ', sizeof(strFax));
		memset(strClient, ' ', sizeof(strClient));
		memset(strTitle, ' ', sizeof(strTitle));
		memset(strReviewName, ' ', sizeof(strReviewName));
		memset(strReviewDate, ' ', sizeof(strReviewDate));
		memset(strComment, ' ', sizeof(strComment));
		nVer571Mark = 0;
		memset(nExtra, 0, sizeof(nExtra));
		nExtra2Ver = 0;
		memset(nExtra2, 0, sizeof(nExtra2));
	}
	void Convert570(T_PJCF_D_CH_570& rData)
	{
		memcpy(strProject, rData.strProject, sizeof(strProject));
		memcpy(strUser, rData.strUser, sizeof(strUser));
		memcpy(strRevision, rData.strRevision, sizeof(strRevision));
		memcpy(strMail, rData.strMail, sizeof(strMail));
		memcpy(strAddress, rData.strAddress, sizeof(strAddress));
		memcpy(strPhone, rData.strPhone, sizeof(strPhone));
		memcpy(strFax, rData.strFax, sizeof(strFax));
		memcpy(strClient, rData.strClient, sizeof(strClient));
		memcpy(strTitle, rData.strTitle, sizeof(strTitle));
		memcpy(strReviewName, rData.strReviewName, sizeof(strReviewName));
		memcpy(strReviewDate, rData.strReviewDate, sizeof(strReviewDate));
		memcpy(strComment, rData.strComment, sizeof(strComment));
		nVer571Mark = 0;
		memset(nExtra, 0, sizeof(nExtra));
		nExtra2Ver = 0;
		memset(nExtra2, 0, sizeof(nExtra2));
	}
	void Convert712(T_PJCF_D_CH_712& rData)
	{
		memcpy(strProject, rData.strProject, sizeof(strProject));
		memcpy(strUser, rData.strUser, sizeof(strUser));
		memcpy(strRevision, rData.strRevision, sizeof(strRevision));
		memcpy(strMail, rData.strMail, sizeof(strMail));
		memcpy(strAddress, rData.strAddress, sizeof(strAddress));
		memcpy(strPhone, rData.strPhone, sizeof(strPhone));
		memcpy(strFax, rData.strFax, sizeof(strFax));
		memcpy(strClient, rData.strClient, sizeof(strClient));
		memcpy(strTitle, rData.strTitle, sizeof(strTitle));
		memcpy(strReviewName, rData.strReviewName, sizeof(strReviewName));
		memcpy(strReviewDate, rData.strReviewDate, sizeof(strReviewDate));
		memcpy(strComment, rData.strComment, sizeof(strComment));
		nVer571Mark = rData.nVer571Mark;
		memcpy(nExtra, rData.nExtra, sizeof(nExtra));
		nExtra2Ver = 0;
		memset(nExtra2, 0, sizeof(nExtra2));
	}
};
struct T_PJCF_UDRD_D
{
	T_PJCF_D data;
};
struct T_PJCF_UDRD_D_CH_40
{
	T_PJCF_D_CH_40 data;
};
struct T_PJCF_UDRD_D_CH_41
{
	T_PJCF_D_CH_41 data;
};
struct T_PJCF_UDRD_D_CH_570
{
	T_PJCF_D_CH_570 data;
};
struct T_PJCF_UDRD_D_CH
{
	T_PJCF_D_CH data;
};
struct T_PJCF_UNIT
{
	T_PJCF_UNIT()
	{
		strProject=D_UNITSYS_NONE;
		strRevision=D_UNITSYS_NONE;
		strUser=D_UNITSYS_NONE;
		strMail=D_UNITSYS_NONE;
		strAddress=D_UNITSYS_NONE;
		strPhone=D_UNITSYS_NONE;
		strFax=D_UNITSYS_NONE;
		strClient=D_UNITSYS_NONE;
		strTitle=D_UNITSYS_NONE;
		strReviewName=D_UNITSYS_NONE;
		strReviewDate=D_UNITSYS_NONE;
		strComment=D_UNITSYS_NONE;
	}
	int strProject;
	int strRevision;
	int strUser;
	int strMail;
	int strAddress;
	int strPhone;
	int strFax;
	int strClient;
	int strTitle;
	int strReviewName;
	int strReviewDate;
	int strComment;
};
#define HASHSIZEPJCF 1

/// structure type ///
#define T_STYP_K unsigned int
struct T_STYP_D_413
{
	int nStructureType;   // 0:3-D  1:X-Z Plane  2: Y-Z Plane  3: X-Y Plane  4:XYZRXRY
	int nStructureMass;   // 0:Not Convert  1:X,Y,Z  2:X,Y  3:Z
	double dblGravity;
	double dblInitTemp;
	BOOL bFloorBeam;
	void Initialize()
	{
		nStructureType=0;
		nStructureMass=0;
		dblGravity=0.;
		dblInitTemp=0.;
		bFloorBeam=FALSE;
	}
};
struct T_STYP_D_690
{
	int nStructureType;   // 0:3-D  1:X-Z Plane  2: Y-Z Plane  3: X-Y Plane  4:XYZRXRY
	int nStructureMass;   // 0:Not Convert  1:X,Y,Z  2:X,Y  3:Z
	double dblGravity;
	double dblInitTemp;
	BOOL bFloorBeam;
	BOOL bFloorSlab;
	void Initialize()
	{
		nStructureType=0;
		nStructureMass=0;
		dblGravity=0.;
		dblInitTemp=0.;
		bFloorBeam=FALSE;
		bFloorSlab=FALSE;
	}
};
struct T_STYP_D_760
{
	int nStructureType;   // 0:3-D  1:X-Z Plane  2: Y-Z Plane  3: X-Y Plane  4:XYZRXRY
	int nStructureMass;   // 0:Not Convert(쓰이지 않음)  1:X,Y,Z  2:X,Y  3:Z
	double dblGravity;
	double dblInitTemp;
	BOOL bFloorBeam;
	BOOL bFloorSlab;
	int nMassType;        // 0=Do not Convert, 1=Lumped Mass, 2=Consistent Mass(6.9.1추가)
	BOOL bConsiderOffset; // (6.9.1추가)
	void Initialize()
	{
		nStructureType=0;
		nStructureMass=1;
		dblGravity=0.;
		dblInitTemp=0.;
		bFloorBeam=FALSE;
		bFloorSlab=FALSE;
		nMassType=0;
		bConsiderOffset=FALSE;
	}
};
struct T_STYP_D_805
{
	int nStructureType;   // 0:3-D  1:X-Z Plane  2: Y-Z Plane  3: X-Y Plane  4:XYZRXRY
	int nStructureMass;   // 0:Not Convert(쓰이지 않음)  1:X,Y,Z  2:X,Y  3:Z
	double dblGravity;
	double dblInitTemp;
	BOOL bFloorBeam;
	BOOL bFloorSlab;
	int nMassType;        // 0=Do not Convert, 1=Lumped Mass, 2=Consistent Mass(6.9.1추가)
	BOOL bConsiderOffset; // (6.9.1추가)
	BOOL bConvertSelfWeight; // (7.6.1 추가)
	void Initialize()
	{
		nStructureType=0;
		nStructureMass=1;
		dblGravity=0.;
		dblInitTemp=0.;
		bFloorBeam=FALSE;
		bFloorSlab=FALSE;
		nMassType=0;
		bConsiderOffset=FALSE;
		bConvertSelfWeight=FALSE;
	}
};
struct T_STYP_D_960
{
	int nStructureType;   // 0:3-D  1:X-Z Plane  2: Y-Z Plane  3: X-Y Plane  4:XYZRXRY
	int nStructureMass;   // 0:Not Convert(쓰이지 않음)  1:X,Y,Z  2:X,Y  3:Z
	double dblGravity;
	double dblInitTemp;
	BOOL bFloorBeam;
	BOOL bFloorSlab;
	int nMassType;        // 0=Do not Convert, 1=Lumped Mass, 2=Consistent Mass(6.9.1추가)
	BOOL bConsiderOffset; // (6.9.1추가)
	BOOL bConvertSelfWeight; // (7.6.1 추가)
	BOOL bConsiderRotRigidMPF; // (8.1.0 추가). Considering Rotational Rigid Body Mode for Modal Participation Factor. // MNET:XXXX-HSSHIM-20121114

	void Initialize()
	{
		nStructureType = 0;
		nStructureMass = 1;
		dblGravity = 0.;
		dblInitTemp = 0.;
		bFloorBeam = FALSE;
		bFloorSlab = FALSE;
		nMassType = 0;
		bConsiderOffset = FALSE;
		bConvertSelfWeight = FALSE;
	}
};
struct T_STYP_D
{
	int nStructureType;   // 0:3-D  1:X-Z Plane  2: Y-Z Plane  3: X-Y Plane  4:XYZRXRY
	int nStructureMass;   // 0:Not Convert(쓰이지 않음)  1:X,Y,Z  2:X,Y  3:Z
	double dblGravity;
	double dblInitTemp;
	BOOL bFloorBeam;
	BOOL bFloorSlab;
	int nMassType;        // 0=Do not Convert, 1=Lumped Mass, 2=Consistent Mass(6.9.1추가)
	BOOL bConsiderOffset; // (6.9.1추가)
	BOOL bConvertSelfWeight; // (7.6.1 추가)
	BOOL bConsiderRotRigidMPF; // (8.1.0 추가). Considering Rotational Rigid Body Mode for Modal Participation Factor. // MNET:XXXX-HSSHIM-20121114
	int  nMassAtNode;     // [MEC]

	void Initialize()
	{
		nStructureType=0;
		nStructureMass=1;
		dblGravity=0.;
		dblInitTemp=0.;
		bFloorBeam=FALSE;
		bFloorSlab=FALSE;
		nMassType=1;
		bConsiderOffset=TRUE;
		bConvertSelfWeight=FALSE;
		bConsiderRotRigidMPF=FALSE;
		nMassAtNode = 0;
	}
	void Convert413(T_STYP_D_413& rData)
	{
		nStructureType=rData.nStructureType;    
		dblGravity=rData.dblGravity;
		dblInitTemp=rData.dblInitTemp;
		bFloorBeam=rData.bFloorBeam;
		bFloorSlab=FALSE;
		bConsiderRotRigidMPF=FALSE;
		nMassAtNode = 0;
		if(rData.nStructureMass == 0)
		{
			nMassType=0;
			nStructureMass=1;      
			bConsiderOffset=FALSE;
			bConvertSelfWeight=FALSE;
		}
		else if(rData.nStructureMass == 1)
		{
			nMassType=1;
			nStructureMass=1;      
			bConsiderOffset=FALSE;
			bConvertSelfWeight=TRUE;
		}
		else if(rData.nStructureMass == 2)
		{
			nMassType=1;
			nStructureMass=2;      
			bConsiderOffset=FALSE;
			bConvertSelfWeight=TRUE;
		}
		else if(rData.nStructureMass == 3)
		{
			nMassType=1;
			nStructureMass=3;      
			bConsiderOffset=FALSE;
			bConvertSelfWeight=TRUE;
		}    
	}
	void Convert690(T_STYP_D_690& rData)
	{
		nStructureType=rData.nStructureType;    
		dblGravity=rData.dblGravity;
		dblInitTemp=rData.dblInitTemp;
		bFloorBeam=rData.bFloorBeam;
		bFloorSlab=rData.bFloorSlab;
		bConsiderRotRigidMPF=FALSE;
		nMassAtNode = 0;
		if(rData.nStructureMass == 0)
		{
			nMassType=0;
			nStructureMass=1;      
			bConsiderOffset=FALSE;
			bConvertSelfWeight=FALSE;
		}
		else if(rData.nStructureMass == 1)
		{
			nMassType=1;
			nStructureMass=1;      
			bConsiderOffset=FALSE;
			bConvertSelfWeight=TRUE;
		}
		else if(rData.nStructureMass == 2)
		{
			nMassType=1;
			nStructureMass=2;      
			bConsiderOffset=FALSE;
			bConvertSelfWeight=TRUE;
		}
		else if(rData.nStructureMass == 3)
		{
			nMassType=1;
			nStructureMass=3;      
			bConsiderOffset=FALSE;
			bConvertSelfWeight=TRUE;
		}    
	}
	void Convert760(T_STYP_D_760& rData)
	{
		nStructureType=rData.nStructureType;    
		dblGravity=rData.dblGravity;
		dblInitTemp=rData.dblInitTemp;
		bFloorBeam=rData.bFloorBeam;
		bFloorSlab=rData.bFloorSlab;
		nStructureMass=rData.nStructureMass;
		bConsiderOffset=rData.bConsiderOffset;
		bConsiderRotRigidMPF=FALSE;
		nMassAtNode = 0;
		if(rData.nMassType == 0)
		{
			nMassType = 1;
			bConvertSelfWeight = FALSE;
		}
		else if (rData.nMassType == 1)
		{
			nMassType = 1;
			bConvertSelfWeight = TRUE;
		}
		else // rData.nMassType == 2
		{
			nMassType = 2;
			bConvertSelfWeight = TRUE;
		}
	}
	void Convert805(T_STYP_D_805& rData)
	{
		nStructureType=rData.nStructureType;    
		dblGravity=rData.dblGravity;
		dblInitTemp=rData.dblInitTemp;
		bFloorBeam=rData.bFloorBeam;
		bFloorSlab=rData.bFloorSlab;
		nStructureMass=rData.nStructureMass;
		bConsiderOffset=rData.bConsiderOffset;
		nMassType=rData.nMassType;
		bConvertSelfWeight=rData.bConvertSelfWeight;
		bConsiderRotRigidMPF=FALSE;
		nMassAtNode = 0;
	}
	void Convert960(T_STYP_D_960& rData)
	{
		nStructureType = rData.nStructureType;
		dblGravity = rData.dblGravity;
		dblInitTemp = rData.dblInitTemp;
		bFloorBeam = rData.bFloorBeam;
		bFloorSlab = rData.bFloorSlab;
		nStructureMass = rData.nStructureMass;
		bConsiderOffset = rData.bConsiderOffset;
		nMassType = rData.nMassType;
		bConvertSelfWeight = rData.bConvertSelfWeight;
		bConsiderRotRigidMPF = rData.bConsiderRotRigidMPF;
		nMassAtNode = 0;
	}
};
struct T_STYP_UDRD_D
{
	T_STYP_D data;
};
struct T_STYP_UNIT
{
	T_STYP_UNIT()
	{
		nStructureType=D_UNITSYS_NONE;
		nStructureMass=D_UNITSYS_NONE;
		dblGravity=D_UNITSYS_BASE_GRAVITY;
		dblInitTemp=D_UNITSYS_BASE_TEMPER;
		bFloorBeam=D_UNITSYS_NONE;
		bFloorSlab=D_UNITSYS_NONE;
	}
	int nStructureType;
	int nStructureMass;
	int dblGravity;
	int dblInitTemp;
	int bFloorBeam;
	int bFloorSlab;
};
#define HASHSIZESTYP 1

// Line Grid
#define T_GDLN_K unsigned int
#define T_GDLN_DT CArray<double, double>
struct T_GDLN_D
{
	CString GridName;   // 20자이내 허용
//  char GridName[20];
	T_GDLN_DT* pXGrid;
	T_GDLN_DT* pYGrid;
	double xAxisAngle;
	double yAxisAngle;
	double originPos[3];
};
struct T_GDLN_UDRD_D
{
	T_GDLN_K key;
	T_GDLN_D data;
};

struct T_GDLN_RDWR_D_955
{
	char GridName[20];
	unsigned int nXGrid;
	unsigned int nYGrid;
};

struct T_GDLN_RDWR_D
{
	char GridName[20];
	unsigned int nXGrid;
	unsigned int nYGrid;
	double xAxisAngle;
	double yAxisAngle;
	double originPos[3];

	void Convert955(const T_GDLN_RDWR_D_955& rData)
	{
		memcpy(GridName, rData.GridName, sizeof(GridName));
		nXGrid = rData.nXGrid;
		nYGrid = rData.nYGrid;
		xAxisAngle = 0.0;
		yAxisAngle = 0.0;
		originPos[0] = 0.0;
		originPos[1] = 0.0;
		originPos[2] = 0.0;
	}
};

struct T_GDLN_UNIT
{
	T_GDLN_UNIT()
	{
		nCount=3;
		nUnit[0] = D_UNITSYS_NONE;
		nUnit[1] = D_UNITSYS_BASE_LENGTH;
		nUnit[2] = D_UNITSYS_BASE_LENGTH;
		nUnit[3] = D_UNITSYS_BASE_DEGREE;
		nUnit[4] = D_UNITSYS_BASE_DEGREE;
		nUnit[5] = D_UNITSYS_BASE_LENGTH;
		nUnit[6] = D_UNITSYS_BASE_LENGTH;
		nUnit[7] = D_UNITSYS_BASE_LENGTH;
		GridName=nUnit[0];
		pXGrid=nUnit[1];
		pYGrid=nUnit[2];
		xAxisAngle = nUnit[3];
		yAxisAngle = nUnit[4];
		originPos[0] = nUnit[5];
		originPos[1] = nUnit[6];
		originPos[2] = nUnit[7];

	}
	int GridName;
	int pXGrid;
	int pYGrid;
	int xAxisAngle;
	int yAxisAngle;
	int originPos[3];
	int nCount;
	int nUnit[8];
};

#define HASHSIZEGDLN 10

/// npln ///
#define T_NPLN_K unsigned int
struct T_NPLN_D
{
	CString PlaneName;
	int nPlaneType;
	double dPoint[3][3];  // nPlaneType = 1
	double dCoor;        // nPlaneType = 2, 3, 4;
	double dTolerance;
	void Initialize()
	{
		PlaneName.Empty();
		nPlaneType=0;
		memset(dPoint, 0, sizeof(dPoint));
		dCoor=0.;
		dTolerance=0.;
	}
};
struct T_NPLN_D_CH
{
	char PlaneName[20];
	int nPlaneType;
	double dPoint[3][3];  // nPlaneType = 1
	double dCoor;        // nPlaneType = 2, 3, 4;
	double dTolerance;
};
struct T_NPLN_UDRD_D
{
	T_NPLN_K key;
	T_NPLN_D data;
};
struct T_NPLN_UDRD_D_CH
{
	T_NPLN_K key;
	T_NPLN_D_CH data;
};
struct T_NPLN_UNIT
{
	T_NPLN_UNIT()
	{
		PlaneName=D_UNITSYS_NONE;
		nPlaneType=D_UNITSYS_NONE;
		dPoint=D_UNITSYS_BASE_LENGTH;
		dCoor=D_UNITSYS_BASE_LENGTH;
		dTolerance=D_UNITSYS_BASE_LENGTH;
	}
	int PlaneName;
	int nPlaneType;
	int dPoint;
	int dCoor;
	int dTolerance;
};
#define HASHSIZENPLN 101

/// dimm ///
#define T_DIMM_K unsigned int
struct T_DIMM_D
{
	CString strName;			//Dimension Name
	double dScaleFactor;  // Scale Factor
	double dx;						// Origin point
	double dy;
	double dz;
	double dRotX;					// Rotation Angle 
	double dRotY;
	double dRotZ;

	void Initialize()
	{
		strName.Empty();
		dScaleFactor = 0.;
		dx=0.;
		dy=0.;
		dz=0.;
		dRotX=0.;
		dRotY=0.;
		dRotZ=0.;    
	}
};
struct T_DIMM_D_CH
{
	char strName[40];
	double dScaleFactor;  // Scale Factor
	double dx;						// Origin point
	double dy;
	double dz;
	double dRotX;					// Rotation Angle 
	double dRotY;
	double dRotZ;
};
struct T_DIMM_UDRD_D
{
	T_DIMM_K key;
	T_DIMM_D data;
};
struct T_DIMM_UDRD_D_CH
{
	T_DIMM_K key;
	T_DIMM_D_CH data;
};
struct T_DIMM_UNIT
{
	T_DIMM_UNIT()
	{
		dScaleFactor=D_UNITSYS_NONE;
		dx=D_UNITSYS_BASE_LENGTH;
		dy=D_UNITSYS_BASE_LENGTH;
		dz=D_UNITSYS_BASE_LENGTH;
		dRotX=D_UNITSYS_NONE;    
		dRotY=D_UNITSYS_NONE;    
		dRotZ=D_UNITSYS_NONE;    
	}
	int dScaleFactor;
	int dx;
	int dy;
	int dz;
	int dRotX;
	int dRotY;
	int dRotZ;  
};
#define HASHSIZEDIMM 101

/// dmlt ///
#define T_DMLT_K unsigned int
struct T_DMLT_D
{
	T_DIMM_K DimmK;
	int nType;						// 0=Line, 1=Text
	CString strText;
	double dDimLength;
	double dTextHeight;
	double dStartX;				// Start point
	double dStartY;
	double dStartZ;
	double dEndX;					// End point 
	double dEndY;
	double dEndZ;

	void Initialize()
	{
		DimmK = 0;
		nType = 0;
		strText.Empty();
		dDimLength = 0.;
		dTextHeight = 0.;
		dStartX=0.;
		dStartY=0.;
		dStartZ=0.;
		dEndX=0.;
		dEndY=0.;
		dEndZ=0.;    
	}
};
struct T_DMLT_D_CH
{
	T_DIMM_K DimmK;
	int nType;						// 0=Line, 1=Text
	char strText[40];
	double dDimLength;
	double dTextHeight;
	double dStartX;				// Start point
	double dStartY;
	double dStartZ;
	double dEndX;					// End point 
	double dEndY;
	double dEndZ;
};
struct T_DMLT_UDRD_D
{
	T_DMLT_K key;
	T_DMLT_D data;
};
struct T_DMLT_UDRD_D_CH
{
	T_DMLT_K key;
	T_DMLT_D_CH data;
};
struct T_DMLT_UNIT
{
	T_DMLT_UNIT()
	{
		dDimLength	= D_UNITSYS_BASE_LENGTH; 
		dTextHeight = D_UNITSYS_BASE_LENGTH;
		dStartX			= D_UNITSYS_BASE_LENGTH;
		dStartY			= D_UNITSYS_BASE_LENGTH;
		dStartZ			= D_UNITSYS_BASE_LENGTH;
		dEndX				= D_UNITSYS_BASE_LENGTH;    
		dEndY				= D_UNITSYS_BASE_LENGTH;    
		dEndZ				= D_UNITSYS_BASE_LENGTH;    
	}
	int dDimLength;
	int dTextHeight;
	int dStartX;
	int dStartY;
	int dStartZ;
	int dEndX;
	int dEndY;
	int dEndZ;  
};
#define HASHSIZEDMLT 101

//---------------------
// Program Invo
struct T_PGIF_D
{
	int nVersion;     // version
	int nCountry;     // 1: ORG, 2:CH, 3:JP, 4:US, 5:RUS
	int nProduct;     // 0: Other, 1: Renewal Gen, 2: Renewal Civil
	int nBuildYear;
	int nBuildMonth;
	int nBuildDay;
	int nIDEVer;    // 1: vs2012
	int nPlatform;    // 64: x64, 32: x86
	int nCharSet;     // 1: MBCS, 2:Unicode
	int nExt1; // 나중을 위해 예약
	int nExt2; // 나중을 위해 예약
	int nExt3; // 나중을 위해 예약
	int nExt4; // 나중을 위해 예약
	void Initialize()
	{
		nVersion = 0;     // version
		nCountry = 0;     // 0: ORG, 1:CH, 2:CH, 3:US, 4:RUS
		nProduct = 0;     // 0: Other, 1: Renewal Gen, 2: Renewal Civil
		nBuildYear = 0;
		nBuildMonth = 0;
		nBuildDay = 0;
		nIDEVer = 0;    // 1: vs2012
		nPlatform = 0;    // 64: x64, 32: x86
		nCharSet = 0;     // 1: MBCS, 2: Unicode
		nExt1 = 0; // 나중을 위해 예약
		nExt2 = 0; // 나중을 위해 예약
		nExt3 = 0; // 나중을 위해 예약
		nExt4 = 0; // 나중을 위해 예약
	}

};

// Drop Panel // JWKWON-20141014 : 바닥판 Plate 설계(지판)
#define T_DPAN_K unsigned int
typedef CArray<T_DPAN_K, T_DPAN_K> T_DPAN_K_LIST;

struct T_DPAN_D
{
	CString strName;
	CString strDecs;
	double dB1;
	double dB2;
	double dH1;
	double dH2;
	BOOL bAngle;
	double dAngle;
	BOOL bThik;
	double dThikness;
	int nLocation; // 0:Top of Column, 1:Bottom of Column, 2:Both

	void Initialize()
	{
		strName.Empty();
		strDecs.Empty();
		dB1 = dB2 = dH1 = dH2 = 0.;
		bAngle = FALSE;
		dAngle = 0.;
		bThik = TRUE;
		dThikness = 0.;
		nLocation = 0;
	}

	T_DPAN_D() {}
	T_DPAN_D& operator=(T_DPAN_D& src)
	{
		strName   = src.strName;
		strDecs   = src.strDecs;
		dB1       = src.dB1;
		dB2       = src.dB2;
		dH1       = src.dH1;
		dH2       = src.dH2;
		bAngle    = src.bAngle;
		dAngle    = src.dAngle;                
		bThik     = src.bThik;
		dThikness = src.dThikness;
		nLocation = src.nLocation;
		return *this;
	}
};
struct T_DPAN_D_CH
{
	char   strName[20];
	char   strDecs[20];

	double dB1;
	double dB2;
	double dH1;
	double dH2;
	BOOL bAngle;
	double dAngle;
	BOOL bThik;
	double dThikness;
	int nLocation; // 0:Top of Column, 1:Bottom of Column, 2:Both

	void ConvertToString(T_DPAN_D& rData)
	{
		ConvertCharStr(strName, rData.strName, sizeof(strName));
		ConvertCharStr(strDecs, rData.strDecs, sizeof(strDecs));

		rData.dB1 = dB1;
		rData.dB2 = dB2;
		rData.dH1 = dH1;
		rData.dH2 = dH2;
		rData.bAngle = bAngle;
		rData.dAngle = dAngle;
		rData.bThik = bThik;
		rData.dThikness = dThikness;
		rData.nLocation = nLocation;
	}
	void ConvertToChar(T_DPAN_D& rData)
	{
		ConvertStrChar(rData.strName, strName, sizeof(strName));
		ConvertStrChar(rData.strDecs, strDecs, sizeof(strDecs));
		dB1 = rData.dB1;
		dB2 = rData.dB2;
		dH1 = rData.dH1;
		dH2 = rData.dH2;
		bAngle = rData.bAngle;
		dAngle = rData.dAngle;
		bThik  = rData.bThik;
		dThikness = rData.dThikness;
		nLocation = rData.nLocation;
	}
};
struct T_DPAN_UDRD_D
{
	T_DPAN_K key;
	T_DPAN_D data;
};
struct T_DPAN_UDRD_D_CH
{
	T_DPAN_K key;
	T_DPAN_D_CH data;
};
struct T_DPAN_UDRD_D_CH_RW_875
{
	T_DPAN_K key;
	char   strName[20];
	char   strDecs[20];

	double dB1;
	double dB2;
	double dH1;
	double dH2;
	BOOL bAngle;
	double dAngle;
	BOOL bThik;
	double dThikness;
};
struct T_DPAN_UDRD_D_CH_RW
{
	T_DPAN_K key;
	char   strName[20];
	char   strDecs[20];

	double dB1;
	double dB2;
	double dH1;
	double dH2;
	BOOL bAngle;
	double dAngle;
	BOOL bThik;
	double dThikness;
	int  nLocation;  //v880 추가

	void GetDpan(T_DPAN_K& rKey, T_DPAN_D_CH& rData)
	{
		rKey=key;
		memcpy(rData.strName, strName, sizeof(strName));
		memcpy(rData.strDecs, strDecs, sizeof(strDecs));

		rData.dB1 = dB1;
		rData.dB2 = dB2;
		rData.dH1 = dH1;
		rData.dH2 = dH2;
		rData.bAngle = bAngle;
		rData.dAngle = dAngle;
		rData.bThik = bThik;
		rData.dThikness = dThikness;
		rData.nLocation = nLocation;
	}
	void SetDpan(const T_DPAN_K Key, const T_DPAN_D_CH& rData)
	{
		key=Key;
		memcpy(strName, rData.strName, sizeof(strName));
		memcpy(strDecs, rData.strDecs, sizeof(strDecs));
		
		dB1 = rData.dB1;
		dB2 = rData.dB2;
		dH1 = rData.dH1;
		dH2 = rData.dH2;
		bAngle = rData.bAngle;
		dAngle = rData.dAngle;
		bThik  = rData.bThik;
		dThikness = rData.dThikness;
		nLocation = rData.nLocation;
	}
	void Convert875(T_DPAN_UDRD_D_CH_RW_875& rData)
	{
		key=rData.key;
		memcpy(strName, rData.strName, sizeof(strName));
		memcpy(strDecs, rData.strDecs, sizeof(strDecs));

		dB1 = rData.dB1;
		dB2 = rData.dB2;
		dH1 = rData.dH1;
		dH2 = rData.dH2;
		bAngle = rData.bAngle;
		dAngle = rData.dAngle;
		bThik  = rData.bThik;
		dThikness = rData.dThikness;
		nLocation = 0;
	}
};
struct T_DPAN_UNIT
{
	T_DPAN_UNIT()
	{
		dB1 = D_UNITSYS_BASE_LENGTH;
		dB2 = D_UNITSYS_BASE_LENGTH;
		dH1 = D_UNITSYS_BASE_LENGTH;
		dH2 = D_UNITSYS_BASE_LENGTH;
		dAngle = D_UNITSYS_BASE_DEGREE;
		dThikness = D_UNITSYS_BASE_LENGTH;

	}
	int dB1;
	int dB2;
	int dH1;
	int dH2;
	int dAngle;
	int dThikness;
};
#define HASHSIZEDPAN 101


// Drop Panel Assign // JWKWON-20141014 : 바닥판 Plate 설계(지판) Assign
#define T_DPAA_K unsigned int // Elem key
typedef CArray<T_DPAA_K, T_DPAA_K> T_DPAA_K_LIST;

struct T_DPAA_D
{
	T_DPAN_K DpanK;

	void Initialize()  {  DpanK = 0;  }
	T_DPAA_D() { Initialize(); }
};

struct T_DPAA_UDRD_D
{
	T_DPAA_K key;
	T_DPAA_D data;
};

#define HASHSIZEDPAA 101

// Column Capital // JWKWON-20141014 : 바닥판 Plate 설계(주두)
#define T_CCPT_K unsigned int
typedef CArray<T_CCPT_K, T_CCPT_K> T_CCPT_K_LIST;

struct T_CCPT_D
{
	CString strName;
	CString strDecs;
	double dB1;
	double dB2;
	double dH1;
	double dH2;
	//double dAngle;
	BOOL bAutoRigid;
	int nLocation; // 0:Top of Column, 1:Bottom of Column, 2:Both

	void Initialize()
	{
		strName.Empty();
		strDecs.Empty();
		dB1 = dB2 = dH1 = dH2 = 0.;
		//dAngle = 0.;
		bAutoRigid = FALSE;
		nLocation = 0;
	}

	T_CCPT_D() {}
	T_CCPT_D& operator=(T_CCPT_D& src)
	{
		strName   = src.strName;
		strDecs   = src.strDecs;
		dB1       = src.dB1;
		dB2       = src.dB2;
		dH1       = src.dH1;
		dH2       = src.dH2;
		//dAngle    = src.dAngle;                
		bAutoRigid = src.bAutoRigid;
		nLocation = src.nLocation;
		return *this;
	}
};
struct T_CCPT_D_CH
{
	char   strName[20];
	char   strDecs[20];

	double dB1;
	double dB2;
	double dH1;
	double dH2;
	//double dAngle;
	BOOL bAutoRigid;
	int nLocation; // 0:Top of Column, 1:Bottom of Column, 2:Both

	void ConvertToString(T_CCPT_D& rData)
	{
		ConvertCharStr(strName, rData.strName, sizeof(strName));
		ConvertCharStr(strDecs, rData.strDecs, sizeof(strDecs));

		rData.dB1 = dB1;
		rData.dB2 = dB2;
		rData.dH1 = dH1;
		rData.dH2 = dH2;
		//rData.dAngle = dAngle;
		rData.bAutoRigid = bAutoRigid;
		rData.nLocation = nLocation;
	}
	void ConvertToChar(T_CCPT_D& rData)
	{
		ConvertStrChar(rData.strName, strName, sizeof(strName));
		ConvertStrChar(rData.strDecs, strDecs, sizeof(strDecs));
		dB1 = rData.dB1;
		dB2 = rData.dB2;
		dH1 = rData.dH1;
		dH2 = rData.dH2;
		//dAngle = rData.dAngle;
		bAutoRigid  = rData.bAutoRigid;
		nLocation = rData.nLocation;
	}
};
struct T_CCPT_UDRD_D
{
	T_CCPT_K key;
	T_CCPT_D data;
};
struct T_CCPT_UDRD_D_CH
{
	T_CCPT_K key;
	T_CCPT_D_CH data;
};
struct T_CCPT_UDRD_D_CH_RW_860
{
	T_CCPT_K key;
	char   strName[20];
	char   strDecs[20];

	double dB1;
	double dB2;
	double dH1;
	double dH2;
	double dAngle;
	BOOL bAutoRigid;
};

struct T_CCPT_UDRD_D_CH_RW
{
	T_CCPT_K key;
	char   strName[20];
	char   strDecs[20];

	double dB1;
	double dB2;
	double dH1;
	double dH2;
	double dAngle;
	BOOL bAutoRigid;
	int  nLocation;  //v865 추가

	void GetCcpt(T_CCPT_K& rKey, T_CCPT_D_CH& rData)
	{
		rKey=key;
		memcpy(rData.strName, strName, sizeof(strName));
		memcpy(rData.strDecs, strDecs, sizeof(strDecs));

		rData.dB1 = dB1;
		rData.dB2 = dB2;
		rData.dH1 = dH1;
		rData.dH2 = dH2;
		//rData.dAngle = dAngle;
		rData.bAutoRigid = bAutoRigid;
		rData.nLocation = nLocation;
	}
	void SetCcpt(const T_CCPT_K Key, const T_CCPT_D_CH& rData)
	{
		key=Key;
		memcpy(strName, rData.strName, sizeof(strName));
		memcpy(strDecs, rData.strDecs, sizeof(strDecs));

		dB1 = rData.dB1;
		dB2 = rData.dB2;
		dH1 = rData.dH1;
		dH2 = rData.dH2;
		//dAngle = rData.dAngle;
		bAutoRigid  = rData.bAutoRigid;    
		nLocation   = rData.nLocation;
	}
	void Convert860(T_CCPT_UDRD_D_CH_RW_860& rData)
	{
		key=rData.key;
		memcpy(strName, rData.strName, sizeof(strName));    
		memcpy(strDecs, rData.strDecs, sizeof(strDecs));    
		dB1        = rData.dB1       ;          
		dB2        = rData.dB2       ;          
		dH1        = rData.dH1       ;          
		dH2        = rData.dH2       ;          
		bAutoRigid = rData.bAutoRigid;          
		nLocation  = 0 ;          
	}
};
struct T_CCPT_UNIT
{
	T_CCPT_UNIT()
	{
		dB1 = D_UNITSYS_BASE_LENGTH;
		dB2 = D_UNITSYS_BASE_LENGTH;
		dH1 = D_UNITSYS_BASE_LENGTH;
		dH2 = D_UNITSYS_BASE_LENGTH;
		//dAngle = D_UNITSYS_BASE_DEGREE;
	}
	int dB1;
	int dB2;
	int dH1;
	int dH2;
	//int dAngle;
};
#define HASHSIZECCPT 101

// Column Capital Assign // JWKWON-20141014 : 바닥판 Plate 설계(주두) Assign
#define T_CCPA_K unsigned int // Elem key
typedef CArray<T_CCPA_K, T_CCPA_K> T_CCPA_K_LIST;

struct T_CCPA_D
{
	T_CCPT_K CcptK;

	void Initialize()  {  CcptK = 0;  }
	T_CCPA_D() { Initialize(); }
};

struct T_CCPA_UDRD_D
{
	T_CCPA_K key;
	T_CCPA_D data;
};

#define HASHSIZECCPA 101
//---------------------
//midas Civil 2 cdn setting
#define T_MCCS_K unsigned int
struct T_MCCS_D
{
public:
	enum EnModuleType
	{
		EnModuleType_Dgn,
		EnModuleType_Ass,
		EnModuleType_Det,
		EnModuleType_Rein
	};

	enum EnDgnCodeType
	{
		EnDgnCodeType_JTGD64_2015,
		EnDgnCodeType_JTGTD64_01_2015,
		EnDgnCodeType_JTG3362_2018,
		EnDgnCodeType_JTGD62_2004,
		EnDgnCodeType_CJJ11_2011,
		EnDgnCodeType_CJJ11_2019,
		EnDgnCodeType_TB10092_2017,
		EnDgnCodeType_JTGTB02_01_2008,
		EnDgnCodeType_JTGT2231_01_2020,
		EnDgnCodeType_CJJ166_2011,
		EnDgnCodeType_JTG3363_2019,

		EnDgnCodeType_JTGTJ21_01_2015,

		EnDgnCodeType_JTGTJ22_2008,
		EnDgnCodeType_JTJ023_85,

		EnDgnCodeType_JTGTD65_06_2015,
	};

	BOOL          bExportInfl ;
	UINT          uiFileType  ;//0:cml 1:mrb 2:both
	EnModuleType  enModuleType;
	EnDgnCodeType enDgnCodeType;

	void Initialize() 
	{ 
		bExportInfl = FALSE;
		uiFileType  = 2;
		enModuleType = EnModuleType_Dgn;
		enDgnCodeType = EnDgnCodeType_JTG3362_2018;
	}
};

struct T_MCCS_UDRD_D
{

	T_MCCS_D data;
};
#define HASHSIZEMCCS 1

#pragma pack(pop)

#endif  // __DB_ST_DT_ETC_H__
