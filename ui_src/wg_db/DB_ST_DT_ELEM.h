#ifndef __DB_ST_DT_ELEM_H__
#define __DB_ST_DT_ELEM_H__

#define MIDAS_SDK

#pragma pack(push, 1)

/// elem ///
#define T_ELEM_K unsigned int
#define T_ELEM_K_LIST CArray<T_ELEM_K, T_ELEM_K>
#define D_ELEM_MAXNOD 8

typedef std::pair<unsigned int, UINT> ElemPairK;
typedef CArray<ElemPairK, ElemPairK> ArrElemPairKey;

/// nucs ///
#define T_NUCS_K unsigned int
struct T_NUCS_D
{
	CString UcsName;
	double dOrg[3];
	double dVx[3];
	double dVy[3];
	void Initialize()
	{
		UcsName.Empty();
		memset(dOrg, 0, sizeof(dOrg));
		memset(dVx, 0, sizeof(dVx));
		memset(dVy, 0, sizeof(dVy));
	}
};
struct T_NUCS_D_CH
{
	char UcsName[20];
	double dOrg[3];
	double dVx[3];
	double dVy[3];
};
struct T_NUCS_UDRD_D
{
	T_NUCS_K key;
	T_NUCS_D data;
};
struct T_NUCS_UDRD_D_CH
{
	T_NUCS_K key;
	T_NUCS_D_CH data;
};
struct T_NUCS_UNIT
{
	T_NUCS_UNIT()
	{
		UcsName = D_UNITSYS_NONE;
		dOrg = D_UNITSYS_BASE_LENGTH;
		dVx = D_UNITSYS_NONE;
		dVy = D_UNITSYS_NONE;
	}
	int UcsName;
	int dOrg;
	int dVx;
	int dVy;
};
#define HASHSIZENUCS 31

enum EN_BEAM_DGN_TYPE
{
	EN_EL_BEAM = 0,
    EN_EL_VBEAM
};
typedef std::pair<EN_BEAM_DGN_TYPE, int> DgnBeamPairK;

struct T_ELEM_D_40
{
	int eltyp;
	int elmat;
	int elpro;
	int nKnodeType;   // 0=angle 1=node number
	double angle;
	double dblX, dblY, dblZ;
	int elnod[D_ELEM_MAXNOD];
};
struct T_ELEM_UDRD_D_40
{
	T_ELEM_K key;
	T_ELEM_D_40 data;
};
struct T_ELEM_D_431
{
	int eltyp;
	int elmat;
	int elpro;
	int nKnodeType;   // 0=angle 1=node number
	double angle;
	double dblX, dblY, dblZ;
	int elnod[D_ELEM_MAXNOD];
	int nSubType;         // plate/wall/tension/compression 에서 사용, 
							// plate 0=thick 1=thin, wall: 1=membrane 2=plate, tension: 1=truss 2=hook 3=cable, compression : 1=truss 2=gap
	int nWallId;
	double dNonlLength;    // hook, gap 에서 사용
	double dTensionForce;  // cable에서 사용
};
struct T_ELEM_UDRD_D_431
{
	T_ELEM_K key;
	T_ELEM_D_431 data;
};
struct T_ELEM_D_590
{
	int eltyp;
	int elmat;
	int elpro;
	int nKnodeType;   // 0=angle 1=node number
	double angle;
	double dblX, dblY, dblZ;
	int elnod[D_ELEM_MAXNOD];
	int nSubType;         // plate/wall/tension/compression 에서 사용, 
							// plate 0=thick 1=thin, wall: 1=membrane 2=plate, tension: 1=truss 2=hook 3=cable, compression : 1=truss 2=gap
	int nWallId;
	double dNonlLength;    // hook, gap 에서 사용
	double dTensionForce;  // truss, cable에서 사용
	int nTemp;             // 431, 450 version 구분용(size 차이)
};
struct T_ELEM_UDRD_D_590
{
	T_ELEM_K key;
	T_ELEM_D_590 data;
};
struct T_ELEM_D_721
{
	int eltyp;
	int elmat;
	int elpro;
	int nKnodeType;   // 0=angle 1=node number
	double angle;
	double dblX, dblY, dblZ;
	int elnod[D_ELEM_MAXNOD];
	int nSubType;         // plate/wall/tension/compression/plans stress 에서 사용, 
							// plate 0=thick(w/o Drill) 1=thin(w/o Drill) 2=thick(w/ Drill) 3=thin(w/ Drill), wall: 1=membrane 2=plate, 
							// tension: 1=truss 2=hook 3=cable, compression : 1=truss 2=gap, plane stress: 0=w/o Drill 1=w/ Drill
	int nWallId;
	double dNonlLength;    // hook, gap, cable(Lu) 에서 사용
	double dTensionForce;  // truss, cable(dTensionForce, Horizontal)에서 사용 
	double dCableLengthRatio; // cable에서 사용(Lu/L)
	int    nCableOpt; // 0:dCableLengthRatio, 1:dTensionForce, 2:Horizontal, 3:Lu
};
struct T_ELEM_UDRD_D_721
{
	T_ELEM_K key;
	T_ELEM_D_721 data;
};
struct T_ELEM_D_730A
{
	int eltyp;
	int elmat;
	int elpro;
	int nKnodeType;   // 0=angle 1=node number
	double angle;
	double dblX, dblY, dblZ;
	int elnod[D_ELEM_MAXNOD];
	int nSubType;         // plate/wall/tension/compression/plans stress 에서 사용, 
							// plate 0=thick(w/o Drill) 1=thin(w/o Drill) 2=thick(w/ Drill) 3=thin(w/ Drill), wall: 1=membrane 2=plate, 
							// tension: 1=truss 2=hook 3=cable, compression : 1=truss 2=gap, plane stress: 0=w/o Drill 1=w/ Drill
	int nWallId;
	double dNonlLength;    // hook, gap, cable(Lu) 에서 사용
	double dTensionForce;  // truss, cable(dTensionForce, Horizontal)에서 사용 
	double dCableLengthRatio; // cable에서 사용(Lu/L)
	int    nCableOpt; // 0:dCableLengthRatio, 1:dTensionForce, 2:Horizontal, 3:Lu
	double dLimit; // 730 추가. Tens Limit, Comp Limit
};
struct T_ELEM_UDRD_D_730A
{
	T_ELEM_K key;
	T_ELEM_D_730A data;
};
struct T_ELEM_D_820
{
	int eltyp;
	int elmat;
	int elpro;
	int nKnodeType;   // 0=angle 1=node number
	double angle;
	double dblX, dblY, dblZ;
	int elnod[D_ELEM_MAXNOD];
	int nSubType;         // plate/wall/tension/compression/plans stress 에서 사용, 
	// plate 0=thick(w/o Drill) 1=thin(w/o Drill) 2=thick(w/ Drill) 3=thin(w/ Drill), wall: 1=membrane 2=plate, 
	// tension: 1=truss 2=hook 3=cable, compression : 1=truss 2=gap, plane stress: 0=w/o Drill 1=w/ Drill
	int nWallId;
	double dNonlLength;    // hook, gap, cable(Lu) 에서 사용
	double dTensionForce;  // truss, cable(dTensionForce, Horizontal)에서 사용 
	double dCableLengthRatio; // cable에서 사용(Lu/L)
	int    nCableOpt; // 0:dCableLengthRatio, 1:dTensionForce, 2:Horizontal, 3:Lu
	double dLimit;    // 730 추가. Tens Limit, Comp Limit
	BOOL   bUseLimit; // 730 추가. dLimit 추가 이후
};
struct T_ELEM_UDRD_D_820
{
	T_ELEM_K key;
	T_ELEM_D_820 data;
};
struct T_ELEM_D_960
{
	int eltyp; /**< 요소의 타입 */ // BEAM_EL, PLATE_EL 등
	int elmat; /**< 요소를 이루는 재질의 키 값 */
	int elpro; /**< 요소를 이루는 단면의 키 값 */
	int nKnodeType;   // 0=angle 1=node number
	double angle;  /**<  1) 프레임 타입 요소: BetaAngle  2) Plate,Plstrs,Plstrn,Axisym : 후처리에서 사용하는 Plate Local Axis */
	double dblX, dblY, dblZ;
	int elnod[D_ELEM_MAXNOD]; /**< 요소를 이루는 절점의 키 값 */
	int nSubType;         // plate/wall/tension/compression/plans stress/beam 에서 사용, 
	// plate 0=thick(w/o Drill) 1=thin(w/o Drill) 2=thick(w/ Drill) 3=thin(w/ Drill), wall: 1=membrane 2=plate, 
	// tension: 1=truss 2=hook 3=cable, compression : 1=truss 2=gap, plane stress: 0=w/o Drill 1=w/ Drill
	// beam : 0=일반보, 1=Effective beam
	int nWallId;
	double dNonlLength;    // hook, gap, cable(Lu) 에서 사용
	double dTensionForce;  // truss, cable(dTensionForce, Horizontal)에서 사용 
	double dCableLengthRatio; // cable에서 사용(Lu/L)
	int    nCableOpt; // 0:dCableLengthRatio, 1:dTensionForce, 2:Horizontal, 3:Lu
	double dLimit;    // 730 추가. Tens Limit, Comp Limit
	BOOL   bUseLimit; // 730 추가. dLimit 추가 이후
	int    nNodeConnectivity; // 820 추가. 0:Pinned, 1:Fixed
	int    nWallType;         // 820 추가. 0:Plate-Based, 1:CRB
};
struct T_ELEM_UDRD_D_960
{
	T_ELEM_K key;
	T_ELEM_D_960 data;
};
/**
@brief 요소 정보를 관리하는 구조체
*/
struct T_ELEM_D
{
	int eltyp; /**< 요소의 타입 */ // BEAM_EL, PLATE_EL 등
	int elmat; /**< 요소를 이루는 재질의 키 값 */
	int elpro; /**< 요소를 이루는 단면의 키 값 */
	int nKnodeType;   // 0=angle 1=node number
	double angle;  /**<  1) 프레임 타입 요소: BetaAngle  2) Plate,Plstrs,Plstrn,Axisym : 후처리에서 사용하는 Plate Local Axis */
	double dblX, dblY, dblZ;
	int elnod[D_ELEM_MAXNOD]; /**< 요소를 이루는 절점의 키 값 */
	int nSubType;         // plate/wall/tension/compression/plans stress/beam 에서 사용, 
							// plate 0=thick(w/o Drill) 1=thin(w/o Drill) 2=thick(w/ Drill) 3=thin(w/ Drill), wall: 1=membrane 2=plate, 
							// tension: 1=truss 2=hook 3=cable, compression : 1=truss 2=gap, plane stress: 0=w/o Drill 1=w/ Drill
							// beam : 0=일반보, 1=Effective beam
	int nWallId;
	double dNonlLength;    // hook, gap, cable(Lu) 에서 사용
	double dTensionForce;  // truss, cable(dTensionForce, Horizontal)에서 사용 
	double dCableLengthRatio; // cable에서 사용(Lu/L)
	int    nCableOpt; // 0:dCableLengthRatio, 1:dTensionForce, 2:Horizontal, 3:Lu
	double dLimit;    // 730 추가. Tens Limit, Comp Limit
	BOOL   bUseLimit; // 730 추가. dLimit 추가 이후
	int    nNodeConnectivity; // 820 추가. 0:Pinned, 1:Fixed
	int    nWallType;         // 820 추가. 0:Plate-Based, 1:CRB
	T_NUCS_K NucsK;		      // [MEC] Solid 요소의 Ref Axis
	void Initialize()
	{
		eltyp=0;
		elmat=0;
		elpro=0;
		nKnodeType=0;
		angle=0.;
		dblX=0;
		dblY=0;
		dblZ=0;
		for(int i=0; i<D_ELEM_MAXNOD; i++)elnod[i]=0;
		nSubType=0;
		nWallId=0;
		dNonlLength=0.;
		dTensionForce=0.;
		dCableLengthRatio=1.0;
		nCableOpt = 0;
		dLimit = 0.;
		bUseLimit = FALSE;
		nNodeConnectivity=0;
		nWallType=0;
		NucsK = 0;
	}
	void Convert40(T_ELEM_D_40& rData)
	{
		eltyp=rData.eltyp;
		elmat=rData.elmat;
		elpro=rData.elpro;
		nKnodeType=rData.nKnodeType;
		angle=rData.angle;
		dblX=rData.dblX;
		dblY=rData.dblY;
		dblZ=rData.dblZ;
		for(int i=0; i<D_ELEM_MAXNOD; i++)elnod[i]=rData.elnod[i];
		nSubType=0;
		nWallId=0;
		dNonlLength=0.;
		dTensionForce=0.;
		dCableLengthRatio=1.0;
		nCableOpt = 0;
		dLimit = 0.;
		bUseLimit = FALSE;
		nNodeConnectivity=0;
		nWallType=0;
		NucsK = 0;
	}
	void Convert431(T_ELEM_D_431& rData)
	{
		eltyp=rData.eltyp;
		elmat=rData.elmat;
		elpro=rData.elpro;
		nKnodeType=rData.nKnodeType;
		angle=rData.angle;
		dblX=rData.dblX;
		dblY=rData.dblY;
		dblZ=rData.dblZ;
		for(int i=0; i<D_ELEM_MAXNOD; i++)elnod[i]=rData.elnod[i];
		nSubType=rData.nSubType;
		nWallId=rData.nWallId;
		dNonlLength=rData.dNonlLength;
		dTensionForce=rData.dTensionForce;
		if ((eltyp == TRTENS_EL || eltyp == TRCOMP_EL) && nSubType == 1) // truss
			dTensionForce = 0.0;
		if (eltyp == TRTENS_EL && nSubType == 3) dTensionForce = 0.0;
		dCableLengthRatio=1.0;
		nCableOpt = 0;
		dLimit = 0.;
		bUseLimit = FALSE;
		nNodeConnectivity=0;
		nWallType=0;
		NucsK = 0;
	}
	void Convert590(T_ELEM_D_590& rData)
	{
		eltyp=rData.eltyp;
		elmat=rData.elmat;
		elpro=rData.elpro;
		nKnodeType=rData.nKnodeType;
		angle=rData.angle;
		dblX=rData.dblX;
		dblY=rData.dblY;
		dblZ=rData.dblZ;
		for(int i=0; i<D_ELEM_MAXNOD; i++)elnod[i]=rData.elnod[i];
		nSubType=rData.nSubType;
		nWallId=rData.nWallId;
		dNonlLength=rData.dNonlLength;
		dTensionForce=rData.dTensionForce;
		dCableLengthRatio=1.0;
		nCableOpt = 0;
		dLimit = 0.;
		bUseLimit = FALSE;
		nNodeConnectivity=0;
		nWallType=0;
		NucsK = 0;
	}
	void Convert721(T_ELEM_D_721& rData)
	{
		eltyp=rData.eltyp;
		elmat=rData.elmat;
		elpro=rData.elpro;
		nKnodeType=rData.nKnodeType;
		angle=rData.angle;
		dblX=rData.dblX;
		dblY=rData.dblY;
		dblZ=rData.dblZ;
		for(int i=0; i<D_ELEM_MAXNOD; i++)elnod[i]=rData.elnod[i];
		nSubType=rData.nSubType;
		nWallId=rData.nWallId;
		dNonlLength=rData.dNonlLength;
		dTensionForce=rData.dTensionForce;
		dCableLengthRatio=rData.dCableLengthRatio;
		nCableOpt = rData.nCableOpt;
		dLimit = 0.;
		bUseLimit = FALSE;
		nNodeConnectivity=0;
		nWallType=0;
		NucsK = 0;
	}
	void Convert730A(T_ELEM_D_730A& rData)
	{
		eltyp=rData.eltyp;
		elmat=rData.elmat;
		elpro=rData.elpro;
		nKnodeType=rData.nKnodeType;
		angle=rData.angle;
		dblX=rData.dblX;
		dblY=rData.dblY;
		dblZ=rData.dblZ;
		for(int i=0; i<D_ELEM_MAXNOD; i++)elnod[i]=rData.elnod[i];
		nSubType=rData.nSubType;
		nWallId=rData.nWallId;
		dNonlLength=rData.dNonlLength;
		dTensionForce=rData.dTensionForce;
		dCableLengthRatio=rData.dCableLengthRatio;
		nCableOpt = rData.nCableOpt;
		dLimit = rData.dLimit;
		bUseLimit = (dLimit != 0.);
		nNodeConnectivity=0;
		nWallType=0;
		NucsK = 0;
	}

	void Convert820(T_ELEM_D_820& rData)
	{
		eltyp=rData.eltyp;
		elmat=rData.elmat;
		elpro=rData.elpro;
		nKnodeType=rData.nKnodeType;
		angle=rData.angle;
		dblX=rData.dblX;
		dblY=rData.dblY;
		dblZ=rData.dblZ;
		for(int i=0; i<D_ELEM_MAXNOD; i++)elnod[i]=rData.elnod[i];
		nSubType=rData.nSubType;
		nWallId=rData.nWallId;
		dNonlLength=rData.dNonlLength;
		dTensionForce=rData.dTensionForce;
		dCableLengthRatio=rData.dCableLengthRatio;
		nCableOpt = rData.nCableOpt;
		dLimit = rData.dLimit;
		bUseLimit = rData.bUseLimit;
		nNodeConnectivity=0;
		nWallType=0;
		NucsK = 0;
	}
	void Convert960(T_ELEM_D_960& rData)
	{
		eltyp = rData.eltyp;
		elmat = rData.elmat;
		elpro = rData.elpro;
		nKnodeType = rData.nKnodeType;
		angle = rData.angle;
		dblX = rData.dblX;
		dblY = rData.dblY;
		dblZ = rData.dblZ;
		for (int i = 0; i < D_ELEM_MAXNOD; i++)elnod[i] = rData.elnod[i];
		nSubType = rData.nSubType;
		nWallId = rData.nWallId;
		dNonlLength = rData.dNonlLength;
		dTensionForce = rData.dTensionForce;
		dCableLengthRatio = rData.dCableLengthRatio;
		nCableOpt = rData.nCableOpt;
		dLimit = rData.dLimit;
		bUseLimit = rData.bUseLimit;
		nNodeConnectivity = rData.nNodeConnectivity;
		nWallType = rData.nWallType;
		NucsK = 0;
	}
};
struct T_ELEM_UDRD_D
{
	T_ELEM_K key;
	T_ELEM_D data;
};
struct T_ELEM_UDRD_D_VIL
{
	T_ELEM_K key;
	T_ELEM_K keytemp;
	T_ELEM_D data;
};
struct T_ELEM_UNIT
{
	T_ELEM_UNIT()
	{
		eltyp=D_UNITSYS_NONE;
		elmat=D_UNITSYS_NONE;
		elpro=D_UNITSYS_NONE;
		nKnodeType=D_UNITSYS_NONE;
		angle=D_UNITSYS_BASE_DEGREE;
		dblX=D_UNITSYS_BASE_LENGTH;
		dblY=D_UNITSYS_BASE_LENGTH;
		dblZ=D_UNITSYS_BASE_LENGTH;
		elnod=D_UNITSYS_NONE;
		nSubType=D_UNITSYS_NONE;
		nWallId=D_UNITSYS_NONE;
		dNonlLength=D_UNITSYS_BASE_LENGTH;
		dTensionForce=D_UNITSYS_BASE_FORCE;
		dCableLengthRatio=D_UNITSYS_NONE;
		dLimit=D_UNITSYS_BASE_FORCE;
	}
	int eltyp;
	int elmat;
	int elpro;
	int nKnodeType;
	int angle;
	int dblX;
	int dblY;
	int dblZ;
	int elnod;
	int nSubType;
	int nWallId;
	int dNonlLength;
	int dTensionForce;
	int dCableLengthRatio;
	int dLimit;
};
//#ifdef _MDEMO
//  #define HASHSIZEELEM 101
//#else
	#define HASHSIZEELEM 100003 // CMap 성능향상을 위해 prime number로 바꿔줌
//#endif
#define D_ELEM_MINNUM        1   // 입력가능한 최소 요소번호
#define D_ELEM_MAXNUM   999999   // 입력가능한 최대 요소번호
#define D_ELEM_NUMBERINGTYPE_SMALLEST   1
#define D_ELEM_NUMBERINGTYPE_LARGEST    2
#define D_ELEM_NUMBERINGTYPE_USERDEFINE 3
struct T_ELEM_CREATE_F
{
	UINT m_nElemCreate_NumberingType;           // 요소번호 생성 방법
													// == D_ELEM_NUMBERINGTYPE_SMALLEST   ; 사용안된 가장 작은번호 자동부여
													// == D_ELEM_NUMBERINGTYPE_LARGEST    ; 사용안된 가장 큰번호 자동부여
													// == D_ELEM_NUMBERINGTYPE_USERDEFINE ; 사용자 지정번호 이후 사용안된 가장 작은번호 자동부여
	UINT m_nElemCreate_UserNumber;              // 요소번호 사용자지정생성시 부여될 절점 번호(m_bElemCreate_AutoNumbering == FALSE)
													// 이번호 이후 사용안된 가장 작은 번호 부여, 부여후 자동 설정
};

// Tapered Section Group
#define T_TSGR_DT CArray<T_ELEM_K, T_ELEM_K>
#define T_TSGR_K unsigned int
/**
@brief Tapered Section Group 정보를 갖고 있는 구조체
*/
struct T_TSGR_D
{
	CString GroupName; /**< TSGR 그룹의 이름 */
	T_TSGR_DT aElemList; /**< 그룹의 포함된 요소들의 키 값 */
	int nVariation_z;      // 0=Linear 1=Quadratic
	double dQuadExp_z;
	int nQuadEnd_z;        // 0=i 1=j;
	double dQuadDistance_z;//
	int nVariation_y;      // 0=Linear 1=Quadratic
	double dQuadExp_y;
	int nQuadEnd_y;        // 0=i 1=j;
	double dQuadDistance_y;//
	unsigned int GroupId;
	int nStiffCalcMethod; // 0:치수로 재계산, 1:강성 보간, 7.0.0에서 추가 (강성 계산 방법)
	void Initialize()
	{
		GroupName.Empty();
		aElemList.RemoveAll();
		nVariation_z=0;
		dQuadExp_z = 2.0;
		nQuadEnd_z=0;
		dQuadDistance_z=0.;
		nVariation_y=0;
		dQuadExp_y = 2.0;
		nQuadEnd_y=0;
		dQuadDistance_y=0.;
		GroupId=0;
		nStiffCalcMethod = 0;
	}
	T_TSGR_D()
	{
	}
	T_TSGR_D(T_TSGR_D& src)
	{
		*this = src;
	}
	T_TSGR_D& operator=(T_TSGR_D& src)
	{
		GroupName=src.GroupName;
		aElemList.Copy(src.aElemList);
		nVariation_z=src.nVariation_z;
		dQuadExp_z=src.dQuadExp_z;
		nQuadEnd_z=src.nQuadEnd_z;
		dQuadDistance_z=src.dQuadDistance_z;
		nVariation_y=src.nVariation_y;
		dQuadExp_y=src.dQuadExp_y;
		nQuadEnd_y=src.nQuadEnd_y;
		dQuadDistance_y=src.dQuadDistance_y;
		GroupId=src.GroupId;
		nStiffCalcMethod=src.nStiffCalcMethod;
		return *this;
	}
};
struct T_TSGR_D_CH
{
	char GroupName[20];
	T_TSGR_DT aElemList;
	int nVariation_z;      // 0=Linear 1=Quadratic
	double dQuadExp_z;
	int nQuadEnd_z;        // 0=i 1=j;
	double dQuadDistance_z;//
	int nVariation_y;      // 0=Linear 1=Quadratic
	double dQuadExp_y;
	int nQuadEnd_y;        // 0=i 1=j;
	double dQuadDistance_y;//
	unsigned int GroupId;
	int nStiffCalcMethod;
	void ConvertToString(T_TSGR_D& rData)
	{
		ConvertCharStr(GroupName, rData.GroupName, sizeof(GroupName));
		rData.aElemList.Copy(aElemList);
		rData.nVariation_z=nVariation_z;
		rData.dQuadExp_z=dQuadExp_z;
		rData.nQuadEnd_z=nQuadEnd_z;
		rData.dQuadDistance_z=dQuadDistance_z;
		rData.nVariation_y=nVariation_y;
		rData.dQuadExp_y=dQuadExp_y;
		rData.nQuadEnd_y=nQuadEnd_y;
		rData.dQuadDistance_y=dQuadDistance_y;
		rData.GroupId=GroupId;
		rData.nStiffCalcMethod=nStiffCalcMethod;
	}
	void ConvertToChar(T_TSGR_D& rData)
	{
		ConvertStrChar(rData.GroupName, GroupName, sizeof(GroupName));
		aElemList.Copy(rData.aElemList);
		nVariation_z=rData.nVariation_z;
		dQuadExp_z=rData.dQuadExp_z;
		nQuadEnd_z=rData.nQuadEnd_z;
		dQuadDistance_z=rData.dQuadDistance_z;
		nVariation_y=rData.nVariation_y;
		dQuadExp_y=rData.dQuadExp_y;
		nQuadEnd_y=rData.nQuadEnd_y;
		dQuadDistance_y=rData.dQuadDistance_y;
		GroupId=rData.GroupId;
		nStiffCalcMethod=rData.nStiffCalcMethod;
	}
};
struct T_TSGR_UDRD_D
{
	T_TSGR_K key;
	T_TSGR_D data;
};
struct T_TSGR_UDRD_D_CH
{
	T_TSGR_K key;
	T_TSGR_D_CH data;
};
struct T_TSGR_UDRD_D_CH_RW_500
{
	T_TSGR_K key;
	char GroupName[20];
	int nVariation_z;      // 0=Linear 1=Quadratic
	int nQuadEnd_z;        // 0=i 1=j;
	double dQuadDistance_z;//
	int nVariation_y;      // 0=Linear 1=Quadratic
	int nQuadEnd_y;        // 0=i 1=j;
	double dQuadDistance_y;//
	unsigned int GroupId;
	int nCountElemList;
};
struct T_TSGR_UDRD_D_CH_RW_692
{
	T_TSGR_K key;
	char GroupName[20];
	int nVariation_z;      // 0=Linear 1=Quadratic
	double dQuadExp_z;
	int nQuadEnd_z;        // 0=i 1=j;
	double dQuadDistance_z;//
	int nVariation_y;      // 0=Linear 1=Quadratic
	double dQuadExp_y;
	int nQuadEnd_y;        // 0=i 1=j;
	double dQuadDistance_y;//
	unsigned int GroupId;
	int nCountElemList;
};
struct T_TSGR_UDRD_D_CH_RW
{
	T_TSGR_K key;
	char GroupName[20];
	int nVariation_z;      // 0=Linear 1=Quadratic
	double dQuadExp_z;
	int nQuadEnd_z;        // 0=i 1=j;
	double dQuadDistance_z;//
	int nVariation_y;      // 0=Linear 1=Quadratic
	double dQuadExp_y;
	int nQuadEnd_y;        // 0=i 1=j;
	double dQuadDistance_y;//
	unsigned int GroupId;
	int nStiffCalcMethod;
	int nCountElemList;
	void GetTsgr(T_TSGR_K& rKey, T_TSGR_D_CH& rData)
	{
		rKey=key;
		memcpy(rData.GroupName, GroupName, sizeof(GroupName));
		rData.nVariation_z=nVariation_z;
		rData.dQuadExp_z=dQuadExp_z;
		rData.nQuadEnd_z=nQuadEnd_z;
		rData.dQuadDistance_z=dQuadDistance_z;
		rData.nVariation_y=nVariation_y;
		rData.dQuadExp_y=dQuadExp_y;
		rData.nQuadEnd_y=nQuadEnd_y;
		rData.dQuadDistance_y=dQuadDistance_y;
		rData.GroupId=GroupId;
		rData.nStiffCalcMethod = nStiffCalcMethod;
	}
	void SetTsgr(const T_TSGR_K Key, const T_TSGR_D_CH& rData)
	{
		key=Key;
		memcpy(GroupName, rData.GroupName, sizeof(GroupName));
		nVariation_z=rData.nVariation_z;
		dQuadExp_z=rData.dQuadExp_z;
		nQuadEnd_z=rData.nQuadEnd_z;
		dQuadDistance_z=rData.dQuadDistance_z;
		nVariation_y=rData.nVariation_y;
		dQuadExp_y=rData.dQuadExp_y;
		nQuadEnd_y=rData.nQuadEnd_y;
		dQuadDistance_y=rData.dQuadDistance_y;
		GroupId=rData.GroupId;
		nStiffCalcMethod=rData.nStiffCalcMethod;
		nCountElemList=(int)rData.aElemList.GetSize();
	}
	void Convert500(T_TSGR_UDRD_D_CH_RW_500& rData)
	{
		key=rData.key;
		memcpy(GroupName, rData.GroupName, sizeof(GroupName));
		nVariation_z=rData.nVariation_z;
		dQuadExp_z=2.0;
		nQuadEnd_z=rData.nQuadEnd_z;
		dQuadDistance_z=rData.dQuadDistance_z;
		nVariation_y=rData.nVariation_y;
		dQuadExp_y=2.0;
		nQuadEnd_y=rData.nQuadEnd_y;
		dQuadDistance_y=rData.dQuadDistance_y;
		GroupId=rData.GroupId;
		nCountElemList=rData.nCountElemList;
		nStiffCalcMethod=0;
	}
	void Convert692(T_TSGR_UDRD_D_CH_RW_692& rData)
	{
		key=rData.key;
		memcpy(GroupName, rData.GroupName, sizeof(GroupName));
		nVariation_z=rData.nVariation_z;
		dQuadExp_z=rData.dQuadExp_z;
		nQuadEnd_z=rData.nQuadEnd_z;
		dQuadDistance_z=rData.dQuadDistance_z;
		nVariation_y=rData.nVariation_y;
		dQuadExp_y=rData.dQuadExp_y;
		nQuadEnd_y=rData.nQuadEnd_y;
		dQuadDistance_y=rData.dQuadDistance_y;
		GroupId=rData.GroupId;
		nCountElemList=rData.nCountElemList;
		nStiffCalcMethod=0;
	}
};
struct T_TSGR_UNIT
{
	T_TSGR_UNIT()
	{
		NoUnit = D_UNITSYS_NONE;
		dQuadDistance_z=D_UNITSYS_BASE_LENGTH;
		dQuadDistance_y=D_UNITSYS_BASE_LENGTH;
	}
	int dQuadDistance_z;
	int dQuadDistance_y;
	int NoUnit;
};
#define HASHSIZETSGR 101

// Change Element Dependent Material Property
#define T_EDMP_K T_ELEM_K
struct T_EDMP_D_600
{
	double h;  
};
struct T_EDMP_D
{
	double h;
	int    nType;   // 6.1.0.추가  0 : Notational Size of Member, 1 : Volume-Surface Ratio 
	void Initialize()
	{
		h = 0.0;
		nType = 0;
	}
	void Convert600(T_EDMP_D_600& rData)
	{
		h = rData.h;
		nType = 0;
	}
};
struct T_EDMP_UDRD_D_600
{
	T_EDMP_K key;
	T_EDMP_D_600 data;
};
struct T_EDMP_UDRD_D
{
	T_EDMP_K key;
	T_EDMP_D data;
};
struct T_EDMP_UNIT
{
	T_EDMP_UNIT()
	{
		h = D_UNITSYS_BASE_LENGTH;
	}
	int h;
};
#define HASHSIZEEDMP 1001
// Wall Shear Stiffness Reduction Factor
//#define T_WSSF_K T_ELEM_K
#define T_WSSF_KEY unsigned int
union T_WSSF_K
{
	T_WSSF_KEY keymap;
	struct
	{
		unsigned int entity : 20;   // 0-1048576
		unsigned int serial : 12;   // 0-4096
	}key;
};
struct T_WSSF_D_670
{
	double dShear;
	double dBending;
};
struct T_WSSF_D_820
{
	double dShear;
	double dBending;
	UINT   GroupKey;   // Boundary Group Key
};
struct T_WSSF_D
{
	double dShear;
	double dBending;
	double dAxial;

	double dOutTorsion;
	double dOutShear;
	double dOutBending;

	UINT   GroupKey;   // Boundary Group Key
	void Initialize()
	{
		dShear = 1.0;
		dBending = 1.0;
		dAxial   = 1.0;

		dOutTorsion = 1.0;
		dOutShear   = 1.0;
		dOutBending = 1.0;

		GroupKey = 0;
	}
	void Convert670(T_WSSF_D_670& rData)
	{
		dShear = rData.dShear;
		dBending = rData.dBending;
		GroupKey = 0;
		dAxial   = rData.dShear;
		dOutTorsion = 1.0;
		dOutShear   = 1.0;
		dOutBending = 1.0;
	}
	void Convert820(T_WSSF_D_820& rData)
	{
		dShear = rData.dShear;
		dBending = rData.dBending;
		dAxial   = rData.dShear;
		dOutTorsion = 1.0;
		dOutShear   = 1.0;
		dOutBending = 1.0;

		GroupKey = 0;
	}
};
struct T_WSSF_UDRD_D_670
{
	T_WSSF_K key;
	T_WSSF_D_670 data;
};
struct T_WSSF_UDRD_D_820
{
	T_WSSF_K key;
	T_WSSF_D_820 data;
};
struct T_WSSF_UDRD_D
{
	T_WSSF_K key;
	T_WSSF_D data;
};
struct T_WSSF_UNIT
{
	T_WSSF_UNIT()
	{
		dShear = D_UNITSYS_NONE;
		dBending = D_UNITSYS_NONE;
		dAxial   = D_UNITSYS_NONE;
		dTorsion   = D_UNITSYS_NONE;
	}
	int dShear;
	int dBending;
	int dAxial;
	int dTorsion;
};
#define HASHSIZEWSSF 1001

// Element Stiffness Scale Factor
//#define T_ESSF_K T_ELEM_K
#define T_ESSF_KEY unsigned int
union T_ESSF_K
{
	T_ESSF_KEY keymap;
	struct
	{
		unsigned int entity : 20;   // 0-1048576
		unsigned int serial : 12;   // 0-4096
	}key;
};

typedef CArray<T_ESSF_K, T_ESSF_K> T_ESSF_K_LIST;

struct T_ESSF_D
{
	double dScaleFactor[7]; // Area, ..., WArea

	UINT   GroupKey;   // Boundary Group Key
	BOOL   bBefore;
	BOOL   bAfter;
	void Initialize()
	{
		for (int i = 0; i < 7; i++) dScaleFactor[i] = 1.0;

		GroupKey = 0;
		bBefore = TRUE;
		bAfter  = FALSE;
	}
};

typedef CArray<T_ESSF_D, T_ESSF_D&> T_ESSF_D_LIST;

struct T_ESSF_UDRD_D
{
	T_ESSF_K key;
	T_ESSF_D data;
};

struct T_ESSF_UNIT
{
	T_ESSF_UNIT()
	{
		dScaleFactor = D_UNITSYS_NONE;
	}
	int dScaleFactor;
};

#define HASHSIZEESSF HASHSIZEELEM

#define T_PSSF_KEY unsigned int
union T_PSSF_K
{
	T_PSSF_KEY keymap;
	struct
	{
		unsigned int entity : 20;   // 0-1048576
		unsigned int serial : 12;   // 0-4096
	}key;
};
struct T_PSSF_D
{
	double dAxialX;				// Axial(Fxx)
	double dAxialY;				// Axial(Fyy)
	double dShear;				// Shear(Fxy)

	double dOutBendingX;	// Bending(Mxx)
	double dOutBendingY;	// Bending(Myy)
	double dOutTorsion;		// Bending(Mxy)
	double dOutShearX;		// Shear(Vxx)
	double dOutShearY;		// Shear(Vyy)

	BOOL bUseUserDir;
	int	nLocalAxis;     // Local Axis : 0(Local-x), 1(Local-y)
	int	nDirType;       // Direction : 0(Corrdinate Dir.), 1(Ref. Vector)
	int nCoordDir;      // _LSX(+X), _LSX(+Y), _LSX(+Z), _LSX(+R), _LSX(+TH),_LSX(-X), _LSX(-Y), _LSX(-Z), _LSX(-R), _LSX(-TH)
	double dCoordDirX;
	double dCoordDirY;
	double dCoordDirZ;
	double dRefVectorX;
	double dRefVectorY;
	double dRefVectorZ;

	double dBetaAngle;    // User Dir에 대한 local Vector의 local z축으로의 BetaAngle (degree)

	UINT   GroupKey;   // Boundary Group Key

	void Initialize()
	{
		dAxialX = 1.0;
		dAxialY = 1.0;
		dShear  = 1.0;

		dOutBendingX = 1.0;
		dOutBendingY = 1.0;
		dOutTorsion = 1.0;
		dOutShearX = 1.0;
		dOutShearY = 1.0;

		bUseUserDir = FALSE;
		nLocalAxis = 0;
		nDirType = 0;
		nCoordDir = 2;
		dCoordDirX = 0.;
		dCoordDirY = 0.;
		dCoordDirZ = 0.;
		dRefVectorX = 0.;
		dRefVectorY = 0.;
		dRefVectorZ = 0.;

		dBetaAngle = 0.;

		GroupKey = 0;
	}
};
struct T_PSSF_UDRD_D
{
	T_PSSF_K key;
	T_PSSF_D data;
};
struct T_PSSF_UNIT
{
	T_PSSF_UNIT()
	{
		dCoord = D_UNITSYS_BASE_LENGTH;
	}
	int dCoord;
};
#define HASHSIZEPSSF 1001

#pragma pack(pop)

#endif  // __DB_ST_DT_ELEM_H__
