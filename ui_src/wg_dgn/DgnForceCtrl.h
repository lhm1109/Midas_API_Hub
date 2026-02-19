// DgnForceCtrl.h: interface for the CDgnForceCtrl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(__DGNFORCECTRL_H__)
#define __DGNFORCECTRL_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\wg_db\wg_db_StoryDisplDrift.h"
#include "..\wg_db\UserDefType.h"

#include "DgnMapEx.h"	// for CMapExDgn.
#include "DgnStruct.h"
#include "DgnEnum.h"
// PMS:4020-Seungjun-20090711 EC2:04 Slab/Wall Design.
#include "DgnQuadTree.h"

#include <map>

#include "HeaderPre.h"

class CDBDoc;
class CDgnDataCtrl;
class CDgnAnalRsltConverter;
class CDgnAnalRsltCtrl;
struct T_BEAM_SUMASFY;

typedef CMap<T_LCOM_K, T_LCOM_K, _DGN_LCOM, _DGN_LCOM&> _DGN_LCOM_MAP;
typedef CDgnMapEx<int, int, int, int&> CIntMap;

typedef std::pair<UINT, UINT> ADJ_INTWALLID_K;

class __MY_EXT_CLASS__ CDgnForceCtrl
{
public:
	CDgnForceCtrl();
	virtual ~CDgnForceCtrl();

	// Member Variables.
public:
	CDBDoc*			m_pDoc;
	CDgnDataCtrl*	m_pDgnDataCtrl;

protected:
	int m_iLcomStrn;
	int m_iLcomServ;
	int m_iLcomSpec;
	int m_iLcomVert;
	int m_iLcomStrnUG;
	int m_iLcomServUG;
	int m_iLcomSpecUG;

	BOOL m_bSclmCode; // Coded by Seungjun ('20070628) MNet:No.2831.
	CString m_bTestFile;

public:
	// Lcom Number for Design.
	BOOL m_bOnlyLongTerm;
	int m_iLCBDesignNum;
	int m_iLCBServiceNum;
	int m_iLCBAddDesignNum;
	int m_nCode_Length, m_nCode_Force, m_nCode_Category;
	// PMS:4020-Seungjun-20090805 EC2:04 Slab/Wall Design.
	int m_nBase_Length; // User Length Unit
	int m_nBase_Force;  // User Force Unit.

	CString m_strDgnCode;
	int m_iDgnCode;
	int m_iDgnFrameType;	// 0:3D, 1:XZ, 2: YZ, 3: XY.
	int m_iDgnMembAxis;		// 0:NOT, 1:y-axis, 2:z-axis, 3:y,z-axis.
	int m_iElemNo;
	int m_iElemType;			// Beam, Truss,...
	int m_iElemKind;			// Column, Beam, Brace,...

	BOOL	m_bDampCheck;//Damping RC CH

	// PMS:3956-Seungjun-20090610 EC8:04 Seismic Design.
	int m_iDgnSubCode;
	CString m_strDgnSubCode;
	BOOL    m_bNTC08Wall;
	double  m_dNTC08LcasN;
	// Add by ZINU.('02.12.31). Only China Code (GBJ17-88, GB50017-03, GB50010-02, JGJ138-01). Add GB50018-02(By tss)
	// m_iElemKind==D_MBTP_BEAM   -> 0=Frame Beam,  1=Lintel Beam.
	// m_iElemKind==D_MBTP_COLUMN -> 0=Frame Column, 1=Lower Frame Column, 2=Corner Column, 3=Lower Corner Column, 4=Column Supporting Frame.
	// m_iElemKind==D_MBTP_BRACE  -> Not used.
	// m_iElemKind==D_MBTP_WALL   -> 0=General Shear Wall(Strengthened part), 1=General Shear Wall(Ordinary part), 2=Shear Wall at Complicated Tall Building(Strengthened part), 3=Shear Wall at Complicated Tall Building(Ordinary part).
	int m_iElemSubKind, m_iElemSubKindAuto;
	int m_iPosiNo, m_iPosiNoNM, m_iPosiNoPM, m_iPosiNoV;

	CString m_strMatType;	// Steel(_T("S")), Concrete(_T("C")), SRC(_T("SRC")).
	int m_iSpectrumXNo, m_iSpectrumYNo;
	double m_dSpectrumXFactor, m_dSpectrumYFactor;
	// Live Load Reduction Factor (Applied Live Load Case Key).
	CArray<int, int&> m_arReduList;
	double m_dReduFactor;
	BOOL m_bReduAxl, m_bReduMom, m_bReduShr;

	BOOL m_bEqSpecial;	// Only Concrete.
	BOOL m_bSectReduction;	// Only Mexico Concrete.
	// Only Concrete.
	int m_iDgnClass;		// For GBJ17-88, GB50017-03, GB50010-02, JGJ138-01, AIJ-WSD99, AIK-USD94, KSCE-USD96, KCI-USD99, KCI-USD03.
	int m_iPMCurveType; // For AIC318-02, AIC318-99, AIC318-95, AIC318-89, KCI-USD99, KCI-USD03, TWN-USD92 For SCWB Design..
	// Modify by GAY. MQC:1860. ('08.07.18). Divide Scale up factors for Earthquake into 2 Groups (User Input Value + Applied Value).
	double m_dUserSueqLcasN, m_dUserSueqLcasM, m_dUserSueqLcasV;  // User Input Scale up Factors for Earthquake.
	double m_dUserSueqLcomN, m_dUserSueqLcomM, m_dUserSueqLcomV;
	double m_dSueqLcasN, m_dSueqLcasM, m_dSueqLcasV;  // Applied Scale up Factors for Earthquake.
	double m_dSueqLcomN, m_dSueqLcomM, m_dSueqLcomV;
	double m_dCodeLcasN, m_dCodeLcasM, m_dCodeLcasV;
	double m_dCodeLcomN, m_dCodeLcomM, m_dCodeLcomV;
	int m_iAlphaType;		// Scale up Factor for Shear. 0:Max(Ve1,Ve2) 1:Min(Ve1,Ve2) 2:Ve1 3:Ve2
	double m_dAlpha1, m_dAlpha2, m_dAlpha2_Colm;  // ACI318-14부터 Alpha2 beam/column 구분
	double m_dOrgAlpha1, m_dOrgAlpha2, m_dOrgAlpha2_Colm;
	double m_dGamma_rd_Joint;
	//*******
		// Add by ZINU.('01.12.6). To Tens/Comp only element if Steel.
	BOOL m_bFreeTensComp;	// TRUE:linear, FALSE:Non-linear.
	int m_iFreeTensComp;	// 0:General, 1:Tens-only, 2:Comp-only.
//*******
	// Add bu Seungjun (`06.05.22)
	BOOL m_bPSBeam;   // TRUE if PS Beam.
	// Add by ZINU.('06.08.30) NO:2339, Muy=SQRT(My^2+Mz^2) if Only Pipe and Symmetry at GBJ17-88, GB50017-03.
	BOOL m_bFromBiToUni;

	int m_iCantilever;	// Cantilever(+).
	// Add by ZINU.('08.03.22). NO:3294, NOT Apply Mpr if Sub-Beam, Cantilever, Under-Gound Beam & Column.
	BOOL m_bSubBeamForNoMpr;
	BOOL m_bLowMembForNoMpr;
	// Variables for only Civil.
	double m_dUserCtcUpFactor;	// Scale up Factor for Column.

	// PMS:4182-Seungjun-20100608 KBC2009 Seismic design.
	BOOL m_bAutoSeisSubBeam;
	BOOL m_bAutoSeisCantilever;
	BOOL m_bAutoSeisUnderBeamColm;

	////////////////////////
	double m_BMuy, m_BMuz;	// Mcy, Mcz.
	double m_BMuy2, m_BMuz2;
	// Add by GAY. ('13.05.09). PMS:4632. Member force for 7-DOF. W=Bi-Moment, MT=Twisting Moment, MW=Warping Moment.	
	// Total Force.
	double m_Fxx[CONST_DGN_iPOSNUM], m_Fyy[CONST_DGN_iPOSNUM], m_Fzz[CONST_DGN_iPOSNUM];   // Frame Elem : Position,  Plate Elem : 4 node
	double m_BMx[CONST_DGN_iPOSNUM], m_BMy[CONST_DGN_iPOSNUM], m_BMz[CONST_DGN_iPOSNUM];
	double m_Mbi[CONST_DGN_iPOSNUM], m_Tst[CONST_DGN_iPOSNUM], m_Twp[CONST_DGN_iPOSNUM];   // Torsion & Warping for 7 DOF.
	double m_Fxx_UF[5], m_Fyy_UF[5], m_Fxy_UF[5];                // Plate Elem Only. Unit Force : cent + 4 node
	double m_Vxx_UF[5], m_Vyy_UF[5];
	double m_Mxx_UF[5], m_Myy_UF[5], m_Mxy_UF[5];
	// by Dead Load.                                                                           
	double m_FxxD[CONST_DGN_iPOSNUM], m_FyyD[CONST_DGN_iPOSNUM], m_FzzD[CONST_DGN_iPOSNUM];  // Frame Elem : Position,  Plate Elem : 4 node
	double m_BMxD[CONST_DGN_iPOSNUM], m_BMyD[CONST_DGN_iPOSNUM], m_BMzD[CONST_DGN_iPOSNUM];
	double m_MbiD[CONST_DGN_iPOSNUM], m_TstD[CONST_DGN_iPOSNUM], m_TwpD[CONST_DGN_iPOSNUM];  // Torsion & Warping for 7 DOF.
	double m_FxxD_UF[5], m_FyyD_UF[5], m_FxyD_UF[5];               // Plate Elem Only. Unit Force : cent + 4 node
	double m_VxxD_UF[5], m_VyyD_UF[5];
	double m_MxxD_UF[5], m_MyyD_UF[5], m_MxyD_UF[5];
	// by Live Load.
	double m_FxxL[CONST_DGN_iPOSNUM], m_FyyL[CONST_DGN_iPOSNUM], m_FzzL[CONST_DGN_iPOSNUM];  // Frame Elem : Position,  Plate Elem : 4 node
	double m_BMxL[CONST_DGN_iPOSNUM], m_BMyL[CONST_DGN_iPOSNUM], m_BMzL[CONST_DGN_iPOSNUM];
	double m_MbiL[CONST_DGN_iPOSNUM], m_TstL[CONST_DGN_iPOSNUM], m_TwpL[CONST_DGN_iPOSNUM];  // Torsion & Warping for 7 DOF.
	double m_FxxL_UF[5], m_FyyL_UF[5], m_FxyL_UF[5];               // Plate Elem Only. Unit Force : cent + 4 node
	double m_VxxL_UF[5], m_VyyL_UF[5];
	double m_MxxL_UF[5], m_MyyL_UF[5], m_MxyL_UF[5];
	// by Moving Live Load (Included at Live Load).
	double m_FxxML[CONST_DGN_iPOSNUM], m_FyyML[CONST_DGN_iPOSNUM], m_FzzML[CONST_DGN_iPOSNUM]; // Frame Elem : Position,  Plate Elem : 4 node
	double m_BMxML[CONST_DGN_iPOSNUM], m_BMyML[CONST_DGN_iPOSNUM], m_BMzML[CONST_DGN_iPOSNUM];
	double m_MbiML[CONST_DGN_iPOSNUM], m_TstML[CONST_DGN_iPOSNUM], m_TwpML[CONST_DGN_iPOSNUM]; // Torsion & Warping for 7 DOF.
	double m_FxxML_UF[5], m_FyyML_UF[5], m_FxyML_UF[5];              // Plate Elem Only. Unit Force : cent + 4 node
	double m_VxxML_UF[5], m_VyyML_UF[5];
	double m_MxxML_UF[5], m_MyyML_UF[5], m_MxyML_UF[5];
	// by Remnant Load.
	double m_FxxR[CONST_DGN_iPOSNUM], m_FyyR[CONST_DGN_iPOSNUM], m_FzzR[CONST_DGN_iPOSNUM];  // Frame Elem : Position,  Plate Elem : 4 node
	double m_BMxR[CONST_DGN_iPOSNUM], m_BMyR[CONST_DGN_iPOSNUM], m_BMzR[CONST_DGN_iPOSNUM];
	double m_MbiR[CONST_DGN_iPOSNUM], m_TstR[CONST_DGN_iPOSNUM], m_TwpR[CONST_DGN_iPOSNUM];  // Torsion & Warping for 7 DOF.
	double m_FxxR_UF[5], m_FyyR_UF[5], m_FxyR_UF[5];               // Plate Elem Only. Unit Force : cent + 4 node
	double m_VxxR_UF[5], m_VyyR_UF[5];
	double m_MxxR_UF[5], m_MyyR_UF[5], m_MxyR_UF[5];
	// by Tendon Primary Load.
	double m_FxxTP[CONST_DGN_iPOSNUM], m_FyyTP[CONST_DGN_iPOSNUM], m_FzzTP[CONST_DGN_iPOSNUM]; // Frame Elem : Position,  Plate Elem : 4 node
	double m_BMxTP[CONST_DGN_iPOSNUM], m_BMyTP[CONST_DGN_iPOSNUM], m_BMzTP[CONST_DGN_iPOSNUM];
	double m_MbiTP[CONST_DGN_iPOSNUM], m_TstTP[CONST_DGN_iPOSNUM], m_TwpTP[CONST_DGN_iPOSNUM]; // Torsion & Warping for 7 DOF.
	double m_FxxTP_UF[5], m_FyyTP_UF[5], m_FxyTP_UF[5];              // Plate Elem Only. Unit Force : cent + 4 node
	double m_VxxTP_UF[5], m_VyyTP_UF[5];
	double m_MxxTP_UF[5], m_MyyTP_UF[5], m_MxyTP_UF[5];
	// by Tendon Secondary Load.
	double m_FxxTS[CONST_DGN_iPOSNUM], m_FyyTS[CONST_DGN_iPOSNUM], m_FzzTS[CONST_DGN_iPOSNUM]; // Frame Elem : Position,  Plate Elem : 4 node
	double m_BMxTS[CONST_DGN_iPOSNUM], m_BMyTS[CONST_DGN_iPOSNUM], m_BMzTS[CONST_DGN_iPOSNUM];
	double m_MbiTS[CONST_DGN_iPOSNUM], m_TstTS[CONST_DGN_iPOSNUM], m_TwpTS[CONST_DGN_iPOSNUM]; // Torsion & Warping for 7 DOF.
	double m_FxxTS_UF[5], m_FyyTS_UF[5], m_FxyTS_UF[5];              // Plate Elem Only. Unit Force : cent + 4 node
	double m_VxxTS_UF[5], m_VyyTS_UF[5];
	double m_MxxTS_UF[5], m_MyyTS_UF[5], m_MxyTS_UF[5];
	// by LCTB User Type
	double m_FxxLU[CONST_DGN_iPOSNUM], m_FyyLU[CONST_DGN_iPOSNUM], m_FzzLU[CONST_DGN_iPOSNUM]; // Frame Elem : Position,  Plate Elem : 4 node
	double m_BMxLU[CONST_DGN_iPOSNUM], m_BMyLU[CONST_DGN_iPOSNUM], m_BMzLU[CONST_DGN_iPOSNUM];
	double m_MbiLU[CONST_DGN_iPOSNUM], m_TstLU[CONST_DGN_iPOSNUM], m_TwpLU[CONST_DGN_iPOSNUM]; // Torsion & Warping for 7 DOF.
	double m_FxxLU_UF[5], m_FyyLU_UF[5], m_FxyLU_UF[5];              // Plate Elem Only. Unit Force : cent + 4 node
	double m_VxxLU_UF[5], m_VyyLU_UF[5];
	double m_MxxLU_UF[5], m_MyyLU_UF[5], m_MxyLU_UF[5];

	// Add by ZINU.('05.06.25). FyyR, FzzR at Original Lcom.
	double m_FyyOrgR[CONST_DGN_iPOSNUM], m_FzzOrgR[CONST_DGN_iPOSNUM];
	double m_FxyUFOrgR[5], m_VxxUFOrgR[5], m_VyyUFOrgR[5];

	double m_dVdu[CONST_DGN_iPOSNUM], m_dMdu[CONST_DGN_iPOSNUM];			// Force by Unfactored Self Weight.
	double m_dVdf[CONST_DGN_iPOSNUM], m_dMdf[CONST_DGN_iPOSNUM];			// Force by Factored Self Weight.
	double m_dMmaxML[CONST_DGN_iPOSNUM], m_dMminML[CONST_DGN_iPOSNUM];	// Moving Max/Min Moment per Position.
	double m_dMmaxN[CONST_DGN_iPOSNUM], m_dViN[CONST_DGN_iPOSNUM];			// Negative Maximum Moment per Position.
	double m_dMmaxP[CONST_DGN_iPOSNUM], m_dViP[CONST_DGN_iPOSNUM];			// Positive Maximum Moment per Position.
	double m_FxxGB[CONST_DGN_iPOSNUM];										// by 1.2*(D+0.5L) for GB50010-02.
	double m_FzzIS[CONST_DGN_iPOSNUM];										// by 1.2*(D+L) for IS456_2000.

	double m_BMyMidUp, m_BMyMidDn;											// For Max/Min Moments at 1/2 of Beam.

	CMap<T_FSTL_KEY, T_FSTL_KEY, T_FSTL_BASE, T_FSTL_BASE&> m_FstlBaseMap;
	CMap<T_FBEM_KEY, T_FBEM_KEY, T_FBEM_BASE, T_FBEM_BASE&> m_FbemBaseMap;
	CMap<T_FCOL_KEY, T_FCOL_KEY, T_FCOL_BASE, T_FCOL_BASE&> m_FcolBaseMap;
	CMap<T_FCOL_KEY, T_FCOL_KEY, T_FCOL_BASE, T_FCOL_BASE&> m_FbrcBaseMap;
	CMap<T_FWAL_KEY, T_FWAL_KEY, T_FWAL_BASE, T_FWAL_BASE&> m_FwalBaseMap;
	CMap<T_FSRC_KEY, T_FSRC_KEY, T_FSRC_BASE, T_FSRC_BASE&> m_FscmBaseMap;
	CMap<T_FBEM_KEY, T_FBEM_KEY, T_FBEM_BASE, T_FBEM_BASE&> m_FsbmBaseMap;

	BOOL m_bTestEnvValue; // Coded by Seungjun MNet:No.2591 ('20061214)	For SCWB Design.

protected:
	////////////////////////
	// End Moments for Unbraced Length 1st(0=Cmy,Cmz, 1=Cb), 2nd(MembPosi), 3rd(PartPosi).
	//   I          1/4         1/2         3/4          J  (MembPosi)
	//   0  1  2  3  4  1  2  3  4  1  2  3  4  1  2  3  4  (PartPosi)
	// |||-----------|-----------|-----------|-----------|||-----------|-----------|||
	// Support                                     Lateral Support               Support
	double m_BMye[2][CONST_DGN_iPOSNUM][CONST_DGN_iPOSNUM];
	double m_BMze[2][CONST_DGN_iPOSNUM][CONST_DGN_iPOSNUM];
	// by Dead Load.
	double m_BMyeD[2][CONST_DGN_iPOSNUM][CONST_DGN_iPOSNUM];
	double m_BMzeD[2][CONST_DGN_iPOSNUM][CONST_DGN_iPOSNUM];
	// by Live Load.
	double m_BMyeL[2][CONST_DGN_iPOSNUM][CONST_DGN_iPOSNUM];
	double m_BMzeL[2][CONST_DGN_iPOSNUM][CONST_DGN_iPOSNUM];
	// by Moving Live Load (Included at Live Load).
	double m_BMyeML[2][CONST_DGN_iPOSNUM][CONST_DGN_iPOSNUM];
	double m_BMzeML[2][CONST_DGN_iPOSNUM][CONST_DGN_iPOSNUM];
	// by Remnant Load.
	double m_BMyeR[2][CONST_DGN_iPOSNUM][CONST_DGN_iPOSNUM];
	double m_BMzeR[2][CONST_DGN_iPOSNUM][CONST_DGN_iPOSNUM];
	// by Tendon Primary Load.
	double m_BMyeTP[2][CONST_DGN_iPOSNUM][CONST_DGN_iPOSNUM];
	double m_BMzeTP[2][CONST_DGN_iPOSNUM][CONST_DGN_iPOSNUM];
	// For Force by Tendon Secondary Load.
	double m_BMyeTS[2][CONST_DGN_iPOSNUM][CONST_DGN_iPOSNUM];
	double m_BMzeTS[2][CONST_DGN_iPOSNUM][CONST_DGN_iPOSNUM];
	// by LCTB User Type
	double m_BMyeLU[2][CONST_DGN_iPOSNUM][CONST_DGN_iPOSNUM];
	double m_BMzeLU[2][CONST_DGN_iPOSNUM][CONST_DGN_iPOSNUM];

	// Add by ZINU.('04.02.11). For Load Contributions.
	BOOL m_bExistLctb;
	BOOL m_bUse_NLcomb;
	BOOL m_bExistLctb_CVL;
	double m_FxxLctb[CONST_DGN_iPOSNUM], m_FyyLctb[CONST_DGN_iPOSNUM], m_FzzLctb[CONST_DGN_iPOSNUM];
	double m_BMxLctb[CONST_DGN_iPOSNUM], m_BMyLctb[CONST_DGN_iPOSNUM], m_BMzLctb[CONST_DGN_iPOSNUM];
	double m_FxxUFLctb[CONST_DGN_iPOSNUM], m_FyyUFLctb[CONST_DGN_iPOSNUM], m_FxyUFLctb[CONST_DGN_iPOSNUM];
	double m_VxxUFLctb[CONST_DGN_iPOSNUM], m_VyyUFLctb[CONST_DGN_iPOSNUM];
	double m_MxxUFLctb[CONST_DGN_iPOSNUM], m_MyyUFLctb[CONST_DGN_iPOSNUM], m_MxyUFLctb[CONST_DGN_iPOSNUM];
	double m_BMyeLctb[2][CONST_DGN_iPOSNUM][CONST_DGN_iPOSNUM];
	double m_BMzeLctb[2][CONST_DGN_iPOSNUM][CONST_DGN_iPOSNUM];
	double m_MbiLctb[CONST_DGN_iPOSNUM], m_TstLctb[CONST_DGN_iPOSNUM], m_TwpLctb[CONST_DGN_iPOSNUM]; // Torsion & Warping for 7 DOF.
	// Remn Force (Key=iLcasKey Before LCB->LC).
	CMap<int, int, _DGN_LCTB_POS, _DGN_LCTB_POS> m_arRemnPos[CONST_DGN_iPOSNUM];
	CMap<int, int, _DGN_LCTB_END, _DGN_LCTB_END> m_arRemnEnd[CONST_DGN_iPOSNUM][CONST_DGN_iPOSNUM];

public:
	T_LCOM_K m_iLcomKeyTran;	// Lcom Key for Transfer Stage.
	T_STLD_K m_iStldKeySelf;	// Lcase Key for Self Weight.
	_DGN_LCOM_MAP*	m_parLcomData;// by LcomNo.
	_DGN_LCOM_MAP	m_arLcomTotl;	// by LcomNo (All).
	_DGN_LCOM_MAP	m_arLcomStrn;	// by LcomNo (Strength/Stress + Serviceability).
	_DGN_LCOM_MAP	m_arLcomSpec;	// by LcomNo (Special + Serviceability).
	_DGN_LCOM_MAP	m_arLcomVert;	// by LcomNo (Vertical + Serviceability).
	_DGN_LCOM_MAP   m_arLcomStrnUG; // Strength/Stress + Serviceability for members located at underground.
	_DGN_LCOM_MAP   m_arLcomSpecUG; // Special + Serviceability for members located at underground.

	CMap<T_ELEM_K, T_ELEM_K, int, int&>		m_arStepData;	// by ElemNo.
	// Add by ZINU.('07.07.31). NO:2921, Elem+Lcom Group(T_DLCM_D) Design for PDMS
	CMap<T_LCOM_K, T_LCOM_K, _DGN_LCOM, _DGN_LCOM&>	m_arLcomGrup;	// by LcomNo (Elem+Lcom Group + Serviceability).
	CArray<_DGN_LCOM_MAP*, _DGN_LCOM_MAP*>	m_arLcomStlGrup;	// by LcomNo Steel.
	CArray<_DGN_LCOM_MAP*, _DGN_LCOM_MAP*>	m_arLcomConGrup;	// by LcomNo Concrete.
	CArray<_DGN_LCOM_MAP*, _DGN_LCOM_MAP*>	m_arLcomSrcGrup;	// by LcomNo SRC.
	CArray<_DGN_LCOM_MAP*, _DGN_LCOM_MAP*>	m_arLcomAluGrup;	// by LcomNo Aluminum.
	CArray<_DGN_LCOM_MAP*, _DGN_LCOM_MAP*>	m_arLcomScmGrup;	// by LcomNo Steel Composite
	CArray<_DGN_LCOM_MAP*, _DGN_LCOM_MAP*>	m_arLcomCfsGrup;	// by LcomNo Cold Formed Steel
	CMapEx<T_ELEM_K, T_ELEM_K, CIntMap, CIntMap&> m_arElemStlGrup;
	CMapEx<T_ELEM_K, T_ELEM_K, CIntMap, CIntMap&> m_arElemConGrup;
	CMapEx<T_ELEM_K, T_ELEM_K, CIntMap, CIntMap&> m_arElemConWallGrup;
	CMapEx<T_ELEM_K, T_ELEM_K, CIntMap, CIntMap&> m_arElemSrcGrup;
	CMapEx<T_ELEM_K, T_ELEM_K, CIntMap, CIntMap&> m_arElemAluGrup;
	CMapEx<T_ELEM_K, T_ELEM_K, CIntMap, CIntMap&> m_arElemScmGrup;
	CMapEx<T_ELEM_K, T_ELEM_K, CIntMap, CIntMap&> m_arElemCfsGrup;
	CMap<T_LCOM_K, T_LCOM_K, _DGN_LCOM, _DGN_LCOM&>	m_arLcomNoGrup;

	int m_iRcsTypeKind;	// BCHK, BDGN, CCHK, CDGN, RCHK, RDGN, WCHK, WDGN. see <DgnStruct.h> RCS_XXX
	int m_iSrcTypeKind;	// BCHK, BDGN, CCHK, CDGN, RCHK, RDGN.

	// Add by ZINU.('03.07.04). For Moment Redistribution Factor.
	double m_dMomRedisFact;
	// Modify by GAY. MNET:3780. ('09.03.20). For Torsion Reduction Factor.
	double m_dTorReduFact;
	// Add by GAY. PMS:4267. ('11.12.27). 중국신기준-불확정성 분항계수.
	double m_dGammard_GB10;
	// Coded by Seungjun MNet:No.2591 ('20061130) For SCWB Design of Column.
	CMap<_DGN_SCWB_KEY, _DGN_SCWB_KEY, _DGN_SCWB_D, _DGN_SCWB_D&>  m_arEqBM;
	//   CMap<T_NODE_K,T_NODE_K,double,double> m_arGirMny;
	//   CMap<T_NODE_K,T_NODE_K,double,double> m_arGirMnz;
		// Coded by Seungjun ('20070302) MNet:No.2591.
	CMap<T_NODE_K, T_NODE_K, double, double> m_arGirMnyCW;
	CMap<T_NODE_K, T_NODE_K, double, double> m_arGirMnyCCW;
	CMap<T_NODE_K, T_NODE_K, double, double> m_arGirMnzCW;
	CMap<T_NODE_K, T_NODE_K, double, double> m_arGirMnzCCW;
	// Coded by Seungjun ('20070306) MNet:No.2591.
	CMap<T_NODE_K, T_NODE_K, double, double> m_arGirMpryCW;
	CMap<T_NODE_K, T_NODE_K, double, double> m_arGirMpryCCW;
	CMap<T_NODE_K, T_NODE_K, double, double> m_arGirMprzCW;
	CMap<T_NODE_K, T_NODE_K, double, double> m_arGirMprzCCW;

	// Coded by Seungjun ('20070302) MNet:No.2591.
	CMap<T_NODE_K, T_NODE_K, double, double> m_arUpGirMnyCW;
	CMap<T_NODE_K, T_NODE_K, double, double> m_arUpGirMnyCCW;
	CMap<T_NODE_K, T_NODE_K, double, double> m_arUpGirMnzCW;
	CMap<T_NODE_K, T_NODE_K, double, double> m_arUpGirMnzCCW;
	// Coded by Seungjun ('20070306) MNet:No.2591.
	CMap<T_NODE_K, T_NODE_K, double, double> m_arUpGirMpryCW;
	CMap<T_NODE_K, T_NODE_K, double, double> m_arUpGirMpryCCW;
	CMap<T_NODE_K, T_NODE_K, double, double> m_arUpGirMprzCW;
	CMap<T_NODE_K, T_NODE_K, double, double> m_arUpGirMprzCCW;

	// PMS:3956-Seungjun-20090603 EC8:04 Seismic Design
	CMap<T_NODE_K, T_NODE_K, double, double> m_arGirAsTopy;
	CMap<T_NODE_K, T_NODE_K, double, double> m_arGirAsBoty;
	CMap<T_NODE_K, T_NODE_K, double, double> m_arGirAsTopz;
	CMap<T_NODE_K, T_NODE_K, double, double> m_arGirAsBotz;

	CMap<T_NODE_K, T_NODE_K, double, double> m_arGirVey;
	CMap<T_NODE_K, T_NODE_K, double, double> m_arGirVez;

	CMap<T_NODE_K, T_NODE_K, double, double> m_arUpGirVey;
	CMap<T_NODE_K, T_NODE_K, double, double> m_arUpGirVez;

	std::unordered_map<T_NODE_K, T_BEAM_SUMASFY> m_mBeamSumAsFyY;
	std::unordered_map<T_NODE_K, T_BEAM_SUMASFY> m_mBeamSumAsFyZ;

	// Coded by Seungjun ('20070126) MNet:No.2619 For Taiwan. Design cosidered the compression rebars.
	//int m_iDRBDType; // Doubly Reinforced Beam Design 

	///////////////////////////
	// For Tower.
	BOOL m_bAbnormalForce;
	int m_iTowerElem;		// 주주재(0), 복재(1), 암주재(2), 암대각재(3), 암조재(4), 보조재(5).
	CMap<int, int&, int, int&> m_aTowerTypeData;	// Get Element by Group.

protected:
	unsigned int m_iOrgLcomKey;
	// Add by ZINU.('04.08.19). For AASHTO-ASD96, TWN-BRG-ASD90.
	BOOL m_bLL, m_bWL, m_bWLL, m_bEL, m_bTL, m_bICE;

	double m_dModMyi, m_dModMyj;
	// Add by ZINU.('03.11.11). Get M1,M2 for Cb,Cmy,Cmz.
	CArray<T_ELEM_K, T_ELEM_K> m_apElemK_y[2];	// 0=Sta, 1=End.
	CArray<T_ELEM_K, T_ELEM_K> m_apElemK_z[2];	// 0=Sta, 1=End.

	// Add by ZINU.('05.05.11). Special Load Combination Factor. 
	CMap<T_ELEM_K, T_ELEM_K, double, double>	m_arSlcmSds;			// Special Load Combination Factor (Sds).
	CMap<T_ELEM_K, T_ELEM_K, double, double>	m_arSlcmSpecial;	// Special Load Combination Factor.
	CMap<T_ELEM_K, T_ELEM_K, double, double>	m_arSlcmVertical;	// Vertical Load Combination Factor.
	CMap<T_STLD_K, T_STLD_K, double, double>	m_arSlcfStldK;		// Omega, Static Load Case.
	CMap<T_SPLC_K, T_SPLC_K, double, double>	m_arSlcfSplcK;		// Omega, Spectrum.
	CMap<T_STLD_K, T_STLD_K, double, double>	m_arSlcfStldLLK;    // Omega, Live.  Added by Maharshi for IS800 and IS456

	CMap<UINT, UINT, int, int> m_mapSlcmType; // map of seismic load combination type. (Frame/Truss : T_ELEM_K, Wall : T_WALL_KEY)
	CMap<T_MSTR_K, T_MSTR_K, int, int> m_mapSlcmTypeStrip;  // map of seismic load combination type for STRIP.

	CMap<UINT, UINT, int, int> m_mUlct;  // Underground Load Combination Type. (Frame/Truss : T_ELEM_K, Wall : T_WALL_KEY)

	// Add by ZINU.('05.06.15). Consider Existing LL, EL.
	CMap<int, int, BOOL, BOOL> m_arExistLL;	// Key=OrgLcomId (Serial No).
	CMap<int, int, BOOL, BOOL> m_arExistEL;	// Key=OrgLcomId (Serial No).
	CMap<int, int, int, int> m_mapSeisLoodCase; // Key=OrgLcomId (Serial No). // Add by GAY. PMS:3787. ('09.12.07). 하중조합내의 Seismic Load Case Type. (10=Statis Seismic, 1=Response Spectrum)
	BOOL m_bExistLL, m_bExistEL;

	CString m_strStctEL;	// 시공단계 EL의 하중조건타입 (D,DC,DW, L, T,TPG).
	CString m_strStctEL2;
	CString m_strStctEL3;

	// Add by ZINU.('06.09.11). NO:2340. Consider Factor for Lcase(RS) at GB50010-02 (전중비, 0.2Q0, 박약층).
	CMap<T_STOR_DGN_K, T_STOR_DGN_K&, T_STOR_DGN, T_STOR_DGN&> m_mDesignInfo;
	CMap<_DGN_SHRR_KEY, _DGN_SHRR_KEY&, _DGN_SHRR_D, _DGN_SHRR_D&> m_mShrrData;
	// Add by ZINU.('08.09.01). NO:3664, Check bottom wall at same wall-ID if boundary element.
	CMap<int, int&, BOOL, BOOL&>  m_mBoundaryElemChk;
	CMap<int, int&, _DGN_BOUND_ELEM, _DGN_BOUND_ELEM&> m_mBoundaryElemData;

	BOOL m_bExistCraneLcom;   // Add by GAY. MNET:2683. ('07.05.07). Output을 위한 변수:Crane 하중 존재시, 동시발생부재력 14case 고려여부.
	BOOL m_bChkConCurForce12; // Add by GAY. MNET:XXXX. ('07.06.05). Output을 위한 변수:동시발생부재력 12case 고려여부.

	std::unordered_map<T_ELEM_K, T_MATD_D> m_mElemMatd;
	std::unordered_map<T_WALL_KEY, T_MATD_D> m_mWallMatd;

public:
	bool GetDgnElemMatd(T_ELEM_K ElemK, OUT T_MATD_D& rMatdD);
	bool GetDgnWallMatd(T_WALL_KEY WallKey, OUT int& rnMatlK, OUT T_MATD_D& rMatdD);

public:
	// PMS:3956-Seungjun-20090224 EC8:04 Seismic Design
	double m_dTB;         // the lower limit of the period of the constant spectral acceleration branch
	double m_dTC;         // the upper limit of the period of the constant spectral acceleration branch
	double m_dTD;         // the value defining the beginning of the constant displacement response range of the spectrum
	double m_dAgR;        // reference peak ground acceleration on type A ground. unit : g
	double m_dGamma_I;    // important factor
	double m_dDampingRat; // viscous damping ration(%)
	double m_dSoilF;      // the soil factor
	double m_daua1;       // for Behaviour Factor
	double m_dq0;         // Behaviour Factor
	double m_dNuf;        // concrete-to-concrete friction coefficient. default : smooth 0.6
	double m_dSCWBFactor;
	UINT   m_SpfcK;       // spectrum key.
	int    m_iFrameType;  // For EC8:04 0:Frame system 1:Dual system 2:Coupled wall system 3:Uncoupled wall system 4:Torsionally flexible system 5:Inverted pendulum system 
												// PMS:4035, Add KSSC-LSD09. For KSSC-LSD09, 지진하중저항시스템 (Special Moment Frames, ...)
	BOOL   m_bJointDgn;   // For EC8:04 Joint Design
	BOOL   m_bNAEQDgn;    // For EC8:04 National Annex. capacity design. ex) Italy:NTC2008.
	int    m_nNTCType;    //(Tel. 4022) MNET:XXXX-BSCHOI-20121017 #1 0 = NTC2008 or 1 = NTC2012
	double m_dEigenPeriod;  // Eigen-value Period at 1st-Mode.
	double m_dEigenXPeriod;  // Eigen-value Period at 1st-Mode Global X direction.
	double m_dEigenYPeriod;  // Eigen-value Period at 1st-Mode Global Y direction..
	CMapEx<_DGN_WDLF_KEY, _DGN_WDLF_KEY, _DGN_WDLF_D, _DGN_WDLF_D&> m_arWallMyFunc;
	CMapEx<_DGN_WDLF_KEY, _DGN_WDLF_KEY, _DGN_WDLF_D, _DGN_WDLF_D&> m_arWallMzFunc;
	CMapEx<_DGN_WDLF_KEY, _DGN_WDLF_KEY, _DGN_WDLF_D, _DGN_WDLF_D&> m_arWallFzFunc;
	CMapEx<T_KEY, T_KEY, BOOL, BOOL> m_arSeismicElemList;   // Include from Special Seismic Provision.  
	CMapEx<T_KEY, T_KEY, BOOL, BOOL> m_arSecondaryElemList; // Exclude from Special Seismic Provision.
	CMapEx<T_KEY, T_KEY, BOOL, BOOL> m_arNotSFRSElemList;   // Members not designated as Seismic-Force-Resisting-System.

	double m_dShearStrutAngle; // EC2:04 21.8<=theta<=45
	BOOL   m_bAutoBehaviorFact;
	double m_dBehaviorFact_q;   // the behaviour factor.
	double m_dBehaviorFact_qo;  // the basic value of the behaviour factor.
	// EC2:04 for Slenderness limit for moment magnification.
	double m_dAValue;  // 0.7
	double m_dBValue;  // 1.1
	double m_dCValue;  // 1.0
	BOOL   m_bAutoA;  // PMS 5065;
	BOOL   m_bAutoC;  // TRUE;

	// Add by GAY. PMS:3787. ('09.12.07).
	BOOL m_bExistSplc; // Response Spectrum Load의 존재여부.
	BOOL m_bExistSeis; // Static Seismic Load의 존재여부.

	// PMS:4020-Seungjun-20091013 EC2:04 Slab/Wall Design.
	CMapEx<T_ELEM_K, T_ELEM_K, T_STOR_K, T_STOR_K> m_arStorbyElem;
	CMapEx<T_STOR_K, T_STOR_K, _DGN_ELEM_LIST, _DGN_ELEM_LIST&> m_arElemListbyStor;
	CArray<T_STOR_K, T_STOR_K> m_arStorK;

	BOOL m_bOneWayVec;
	double m_vecU[3][3];

	// Member Functions.
public:
	// iOpt (0=Cmy,Cmz, 1=Cb).
	double Get_EndMy1(int iOpt) const;
	double Get_EndMy2(int iOpt) const;
	double Get_EndMz1(int iOpt) const;
	double Get_EndMz2(int iOpt) const;
	double Get_EndMy1D(int iOpt) const;
	double Get_EndMy2D(int iOpt) const;
	double Get_EndMz1D(int iOpt) const;
	double Get_EndMz2D(int iOpt) const;
	double Get_EndMy1G(int iOpt) const;
	double Get_EndMy2G(int iOpt) const;
	double Get_EndMz1G(int iOpt) const;
	double Get_EndMz2G(int iOpt) const;
	double Get_EndMy1DByMy(int iOpt) const;
	double Get_EndMy2DByMy(int iOpt) const;
	double Get_EndMz1DByMz(int iOpt) const;
	double Get_EndMz2DByMz(int iOpt) const;
	double Get_EndMy1GByMy(int iOpt) const;
	double Get_EndMy2GByMy(int iOpt) const;
	double Get_EndMz1GByMz(int iOpt) const;
	double Get_EndMz2GByMz(int iOpt) const;
	double Get_EndMye(int iOpt, int iPart) const;
	double Get_EndMze(int iOpt, int iPart) const;
	double Get_EndMye(int iOpt, int iPosi, int iPart) const;
	double Get_EndMze(int iOpt, int iPosi, int iPart) const;
	double Get_EndMyeD(int iOpt, int iPart) const;
	double Get_EndMzeD(int iOpt, int iPart) const;
	double Get_EndMyeL(int iOpt, int iPart) const;
	double Get_EndMzeL(int iOpt, int iPart) const;
	double Get_EndMyeG(int iOpt, int iPart) const;
	double Get_EndMzeG(int iOpt, int iPart) const;
	double Get_EndMyeLU(int iOpt, int iPart) const;// MQC.25444 Load cases using load combinations 부재력 Non-sway로 고려
	double Get_EndMzeLU(int iOpt, int iPart) const;// MQC.25444 Load cases using load combinations 부재력 Non-sway로 고려
	double Get_EndMyeR(int iOpt, int iPart) const;
	double Get_EndMzeR(int iOpt, int iPart) const;
	double Get_EndMyeGL(int iOpt, int iPart) const;  //Lctb의 User Type 고려
	double Get_EndMzeGL(int iOpt, int iPart) const;  //Lctb의 User Type 고려

	void Reset_Force();
	void Reset_ForceMajor();
	void Reset_ForceMinor();
	// Add by ZINU.('06.09.07). NO:2442. Check Design Axial Force if Tens/Comp-Only.
	void Reset_AxialForceForTensComp(int iPosiNo, double dAxialStrn);
	// Add by ZINU.('06.08.30). NO:2339, Muy=SQRT(My^2+Mz^2) if Only Pipe and Symmetry at GBJ17-88, GB50017-03.
	void Reset_MomentFromBiToUni(int iPosiNo);
	// Add by ZINU.('06.09.19). NO:2337, Distinguish GB50010-02 from Others.
	void Reset_MomentRedistribution();
	// Add by GAY. MNET:3780. ('09.03.20). Apply Torsion Reduction.
	void Reset_TorsionReduction();
	// Add by GAY. PMS:4267. ('11.12.27). 중국신기준-불확정성 분항계수.
	void Reset_UncertainlyLoadCombFact(int iPosiNo);
	//설계할 Lcom이 없으면 false를 return한다.
	bool Set_LcomDataForDesign(int iLcomKind, BOOL bIncludeService, BOOL bWall = FALSE, int iDgnNo = -1, BOOL bStre = TRUE, BOOL bServ = TRUE);
	void Set_LcomDataForDesignRC(int iLcomKind, BOOL bIncludeService, BOOL bWall = FALSE, int iDgnNo = -1, BOOL bStre = TRUE, BOOL bServ = TRUE); // iDgnNo: 0=UserAll, 1=Beam, 2=Column, 5=Plate, 6=Wall (UserAll일 경우에만  bStre(전체강도하중조합추가), bServ(전체사용하중조하합추가)를 사용)
	void Set_LcomDataForDesignRCRating(int iLcomKind, BOOL bIncludeService, BOOL bWall = FALSE, int iDgnNo = -1, BOOL bStre = TRUE, BOOL bServ = TRUE); // iDgnNo: 0=UserAll, 1=Beam, 2=Column, 5=Plate, 6=Wall (UserAll일 경우에만  bStre(전체강도하중조합추가), bServ(전체사용하중조하합추가)를 사용)
	void Set_LcomDataDamp_CH(int iLcomKind, BOOL bIncludeService, BOOL bWall = FALSE, int iDgnNo = -1, BOOL bStre = TRUE, BOOL bServ = TRUE);
	void Set_LcomDataForDesignTower();
	void Set_LcomDataForDesignSeisEval(int nLcomKind, UINT unType, BOOL bWall);
	void Set_LcomDataForElem(int iElem);
	void Set_ForceMomentBySeperate(int iElem, int iPosi, _DGN_LCOM& LcomDesign);
	void Set_ForceMomentBySeperate(int iElem, int iPosi, _DGN_LCOM& LcomDesign, _DGN_EXPAND_LOAD_ELEM& ExpandElemD);
	// RC
	void Set_LcomDataForElemRC(int iElem);
	// Add by ZINU.('06.09.08). NO:2340. Consider Factor for Lcase(RS) at GB50010-02 (전중비, 0.2Q0, 박약층).
	void Cal_Wght_02Q0_Weak_GB();
	void Set_Wght_02Q0_Weak_GB(int iElemNo, T_ELEM_K ElemK = 0);
	BOOL Get_Wght_02Q0_Weak_GB(int iElemNo, int iLcasNo, _DGN_SHRR_D& ShrrD);
	// Add by ZINU.('08.09.01). NO:3664, Check Bottom Wall at Same Wall-ID if Boundary Element.
protected:
	void Set_BoundaryElemCondition(int iElemNo);
public:
	BOOL Get_BoundaryElemCondition(int iElemNo, _DGN_BOUND_ELEM& BoundaryElem, BOOL bGetData = FALSE);
	// For profiler
	void Profiling(TCHAR* strTxt);
	void ProfilerDestroy();
	// Coded by Seungjun MNet:No.2591 ('20061130)	For SCWB Design.
	BOOL Get_EQBendingMoment(BOOL bDuctileDesign);  // Coded by Seungjun ('20070614) MNet:No.2853. 인자추가.
	BOOL Get_EQBendingMoment4IS();  // Coded by SeungJun MNET:3433  '20080519
	BOOL Get_GirderAsMap_ExistJointCheck(const T_DCON_D& DconD);
	BOOL Get_SCWBBeamEndMoment(_DGN_SCWB_D& ScwbDI, _DGN_SCWB_D& ScwbDJ, T_ELEM_K Elemk, int LcaseType, int LcaseKey);
	BOOL Get_StrengthUCStoGCS(T_ELEM_K ElemK, double dStrength[2], double dPX, double dPY, double dPZ);  // Local to Global.
	BOOL Get_StrengthGCStoUCS(T_ELEM_K ElemK, double dStrength[2], double dPX, double dPY, double dPZ);  // Global to Local.
	BOOL Get_StrengthUCStoGCS(T_ELEM_K ElemK, double dPu, double dStrength[2], double dPX, double dPY);  // Local to Global.
	BOOL Get_StrengthGCStoUCS(T_ELEM_K ElemK, double dPu, double dStrength[2], double dPX, double dPY, BOOL bABS = FALSE);  // Global to Local.
	void AddStaticForceIS(int nPosi, double* adFx, double* adFy, double* adFz, double* adMx, double* adMy, double* adMz,
		const T_STRB_D& CurStrb, double dFactFx, double dFactFy, double dFactFz, double dFactMx, double dFactMy, double dFactMz);
protected:
	void Get_ForceMoment(int iElem, int iPosi, _DGN_LCOM& LcomDesign);
	void Get_ForceMomentTower(int iElemNo, int iPosiNo, _DGN_LCOM& LcomDesign);

	BOOL Get_StaticForce(int iElemNo, int iPosiNo, unsigned int iLcaseKey, double dFactor);
	BOOL Get_SpectrumForce(int iElemNo, int iPosiNo, unsigned int iLcaseKey, double dFactor, int iAnalType);
	BOOL Get_MovingForce(int iElemNo, int iPosiNo, unsigned int iLcaseKey, double dFactor, BOOL bMaxLcom);
	BOOL Get_RemnantForce(int iElemNo, int iPosiNo, unsigned int iLcaseKey, double dFactor, int iAnalType, BOOL bMaxLcom);
	BOOL Get_StageForce(int iElemNo, int iPosiNo, unsigned int iLcaseKey, double dFactor);
	BOOL Get_LcomForce(int iElemNo, int iPosiNo, unsigned int iLcaseKey, double dFactor, int iAnalType, BOOL bMaxLcom);
	BOOL Get_CraneForce(int iElemNo, int iPosiNo, unsigned int iLcaseKey, double dFactor, BOOL bMaxLcom); // Add by GAY. MNET:2683. ('07.05.15).

	// Add by ZINU.('03.11.11). Get M1,M2 for Cb,Cmy,Cmz.
	BOOL Get_BeamEndMoment(CString strForceType, int iPosiNo, double dFactor, BOOL bMaxLcase, BOOL bGetLctb, int iLcaseType, int iLcaseKey, T_STRB_D* pStrbData, double* pRMFactor=NULL);
	BOOL Get_WallEndMoment(CString strForceType, int iPosiNo, double dFactor, BOOL bGetLctb, int iLcaseKey, T_STRW_D* pStrwData);
	BOOL Get_TrussEndMoment(CString strForceType, int iPosiNo);
	BOOL Get_PlateEndMoment(CString strForceType, int iPosiNo); // Add by SHIN.('07.06.28) MNET:2668. : 아무작업도 하지 않음


	void AddBeamForce(int nPosi, double* adFx, double* adFy, double* adFz, double* adMx, double* adMy, double* adMz,
		const T_STRB_D& Strb, double dFactFx, double dFactFy, double dFactFz, double dFactMx, double dFactMy, double dFactMz);
	void AddBeamForceDof7(int nPosi, double* adMbi, double* adTst, double* adTwp,
		const T_STRB_D& Strb, double dFactMbi = 1.0, double dFactTst = 1.0, double dFactTwp = 1.0);
	void AddWallForce(int nPosi, double* adFx, double* adFy, double* adFz, double* adMx, double* adMy, double* adMz,
		const T_STRW_D& Strw, double dFactFx, double dFactFy, double dFactFz, double dFactMx, double dFactMy, double dFactMz);
	void AddPlatForce(int nPosi, double* adFx, double* adFy, double* adFz, double* adMx, double* adMy, double* adMz,
		const T_STRP_DL& Strp, double dFactFx, double dFactFy, double dFactFz, double dFactMx, double dFactMy, double dFactMz);
	void AddPlatForceUF(int nPosi, double* adFxx, double* adFyy, double* adFxy, double* adVxx, double* adVyy, double* adMxx, double* adMyy, double* adMxy,
		const T_STRP_DL& Strp, double dFactFxx, double dFactFyy, double dFactFxy, double dFactVxx, double dFactVyy, double dFactMxx, double dFactMyy, double dFactMxy);

	void MultiplyFxx(int nPosi, double dFactor);
	void MultiplyFyy(int nPosi, double dFactor);
	void MultiplyFzz(int nPosi, double dFactor);
	void MultiplyBMx(int nPosi, double dFactor);
	void MultiplyBMy(int nPosi, double dFactor);
	void MultiplyBMz(int nPosi, double dFactor);
	void MultiplyMbi(int nPosi, double dFactor);
	void MultiplyTst(int nPosi, double dFactor);
	void MultiplyTwp(int nPosi, double dFactor);

	void MultiplyBMye(int nPosi, double dFactor);
	void MultiplyBMze(int nPosi, double dFactor);

	void MultiplyFxxUF(int nPosi, double dFactor);
	void MultiplyFyyUF(int nPosi, double dFactor);
	void MultiplyFxyUF(int nPosi, double dFactor);
	void MultiplyVxxUF(int nPosi, double dFactor);
	void MultiplyVyyUF(int nPosi, double dFactor);
	void MultiplyMxxUF(int nPosi, double dFactor);
	void MultiplyMyyUF(int nPosi, double dFactor);
	void MultiplyMxyUF(int nPosi, double dFactor);

	void GetLcomDataNormailizedLctb(const T_LCOM_BASE_LIST& aSrcLcomBase, OUT T_LCOM_BASE_LIST& raTarLcomBase);

	int GetLcomKeyListMeshedMemb(OUT T_LCOM_K_LIST& raLcomK);
	int GetLcomKeyListMeshedMemb(int nRcsTypeKind, int nCrackDef, int nLcomKind, OUT T_LCOM_K_LIST& raLcomK);

	// Add by ZINU.('04.02.13). For Load Contributions.
	void Sta_ForceByLctb(int iElemNo, BOOL bMaxLcom);
	void Cng_ForceByLctb(int iElemNo, int iPosiNo, int iAnalType, int iLcaseKey, double dFactor, BOOL bMaxLcase);
	void End_ForceByLctb();

	void SetLcomTypeIni();
	void SetLcomTypeGen(T_LCOM_BASE& LcomBase, int& iDgnLcomType, int iOrgLcomNo);	// Add by ZINU.('04.02.02). For Lcom Type (L,S,X+/-,Y+/-).
	void SetLcomTypeJTJ(T_LCOM_BASE& LcomBase, int& iDgnLcomType);	// Add by ZINU.('02.09.11). For JTJ025-86.
	void SetLcomTypeIRC(T_LCOM_BASE& LcomBase, int& iDgnLcomType);	// Add by ZINU.('03.09.22). For IRC:21-2000.
	void SetLcomTypeAST(T_LCOM_BASE& LcomBase, int& iDgnLcomType);	// Add by ZINU.('04.08.19). For AASHTO-ASD96, TWN-BRG-ASD90.
	// Change by ZINU.('05.07.02). Consider Type for EL at Stage.
	BOOL Is_StctEL_DL(int nErLoad);
	BOOL Is_StctEL_LL(int nErLoad);
	BOOL Is_StctEL_TL(int nErLoad);

	void Save_TotalForce(int iElemNo, int iSeismicType, BOOL bLongTerm);

	BOOL GetErectionLoadType(CString strErName, T_STCT_D &rStct, CString &strLCType);

private:
	void ApplyLoadContribution();
	void ApplyScaleUpFactorForColumn(int i);
	void ApplyEQFactorForLcom(int iElem, int iSeismicType, int i, BOOL bPlate);
	void ApplySafeFactorGB(int i);
	void SetSmallValueAsZero(int i);
	void ChangeAxialForceSign(int i, BOOL bPlate);
	void SaveTotalForceForPlate(int iElem, BOOL bPlate, int iSeismicType);
	void ApplyScaleUpFactorForPlate(int i);
	void ApplyEQFactorForLcomPlate(int iElem, int iSeismicType, int i);
	void ApplySafeFactorGBPlate(int i);
	void SetSmallValueAsZeroPlate(int i, const PLATE_UNIT_FORCE &rMaxUF);
	void ChangeAxialForceSignPlate(int i);
	void GetMaxPlateUnitForce(PLATE_UNIT_FORCE &rMaxUF);

protected:
	// Add by ZINU.('05.05.11). Special Load Combination Factor.
	BOOL Get_SpecialLcomOmega(int iLcasNo, int iAnalType, double& dOmega);	// 재귀함수.
	double Get_SpecialLcomOmega(int iElemNo, int iLcasNo, int iAnalType);

	// Add by ZINU.('07.07.09). NO:2903, Distinguish Envelope from SRSS.
	int GetLoadCombType(T_LCOM_K LcomK, int nAnalType);	// 0=Add, 1=Envelope, 2=ABS, 3=SRSS.

	void SetEval1stLcomMap(_DGN_LCOM& LcomDesign, OUT int& rnTotl, OUT int& rnStrn, OUT int& rnVert);

	void SetEvalLinLcomMap(_DGN_LCOM& LcomDesign, UINT unType, OUT int& rnTotl, OUT int& rnStrn, OUT int& rnSpec, OUT int& rnVert);

	void SetDgnLcomMap(_DGN_LCOM& LcomDesign, BOOL bIncludeLcom, OUT int ranSizeLcom[]);

public:
	BOOL Get_SpecialLcomFactor(int iElemNo, double& dFactor, double& dSds);
	BOOL Get_VerticalLcomFactor(int iElemNo, double& dFactor);

	void SetSpecVertLcomInfoMap(BOOL bWall);

	void SetSlcmTypeMap();
	void SetSlcmTypeMap(CMap<UINT, UINT, int, int>& rmapSlcmType);
	int  GetSlcmType(const int& iElem);
	void SetSlcmTypeMapStrip(T_MSTR_K MstrK);

	BOOL IsElemSpecSeisLcom(int nElemNo); // in case of Wall, nElemNo = nInternalWallID.
	BOOL IsElemVertSeisLcom(int nElemNo); // in case of Wall, nElemNo = nInternalWallID.
	BOOL IsStripSpecSeisLcom(T_MSTR_K MstrK);
	BOOL IsStripVertSeisLcom(T_MSTR_K MstrK);
	int  GetStripSeisLcomType(T_MSTR_K MstrK);

	void SetDgnLcomListForCurElem(int ElemK);

	EN_DGNLCOM_APP GetAppDgnLcomTypeByMemb(T_ELEM_K ElemK); // in case of Wall, ElemK = unInternalWalID.    
	_DGN_LCOM_MAP* GetAppDgnLcomMap(const EN_DGNLCOM_APP enDgnLcomApp);
	int GetSizeAppDgnLcomMap(const EN_DGNLCOM_APP enDgnLcomApp);
	int GetSizeAppDgnLcomMapServ(const EN_DGNLCOM_APP enDgnLcomApp);

	bool IsSameDgnLcomTypeInSlab(const T_ELEM_K_LIST& aElemK);
	bool IsSameDgnLcomTypeInSlab(const _DGN_PERI_PART& PeriPart);

	bool IsValidLcomSlabStrn(int nULSnELS, bool bExcludeSeis, const _DGN_LCOM& LcomDesign);
	bool IsConcurrentLcomIndexFor7Dof(const _DGN_LCOM& LcomDesign);

	bool IsExistLcomULS(bool bExcludeSeis);
	bool IsExistLcomELS();

	// Add by ZINU.('02.12.27). To Reset Max/Min Moment by Moveing Live Load.
	void Reset_MaxMinByTotalLcom();
	// Add by ZINU.('02.12.27). To Save Max/Min Moment by Moveing Live Load.
	void Save_MaxMinByTotalLcom(int iElemNo, int iLcomNo, BOOL bPsbm);
	//void Save_UnfactoredForce(int iElemNo);
		// Add by ZINU.('03.11.07). To Search Max/Min Moment Position between 1/4 and 3/4.
	void SetMaxMinMomentPosiForBeam(int iElemNo, BOOL bMaxLcom, int nLcomKind);
	void SetElemListForBeamEndMoment(int iElemNo);
	// PMS:3956-Seungjun-20090311 EC8:04 Seismic Design
	void Get_WallMomBySpecialProv(int iElem, _DGN_LCOM& LcomDesign, double& dMuy, double& dMuz);
	double Get_WallShrBySpecialProv(int iElem, _DGN_LCOM& LcomDesign, const double dFlxRat, const double dVuzorg);

private:
	void Get_DGN_WALL_D_Map(const CArray<UINT, UINT>& aIntWallID, OUT CMapEx<int, int, _DGN_WALL_D, _DGN_WALL_D&>& rmDgnWallD,
		OUT int& rnBotElemK, OUT int& rnTopElemK, OUT double& rdBotZ, OUT double& rdTopZ);
	void Get_WallMomBySpecialProv_UseSlopeMax(int iElem, _DGN_LCOM& LcomDesign, double& dMuy, double& dMuz);
	void Get_WallMomBySpecialProv_UseSlopePrevious(int iElem, _DGN_LCOM& LcomDesign, double& dMuy, double& dMuz);
	double GetWallTotalHeight(int nUserWallId);

	void Calc_MaxShearEnvelop(const std::map<std::pair<double, int>, double>& mForce, const double& dTopVuz, _DGN_WDLF_D& FuncD);

public:
	// Etc. Functions.
	CString Get_PosiNa(int iPosiNo);
	int Get_PosiNo(CString strPosiNa);
	CString Get_LcomNa(int iLcomNo, int iElemNo = 0);
	int Get_LcomNo(CString strLcomNa, int iElemNo = 0);

    // for GDisplay String
	void Write_GDisplayAnalString(CString strTxt, BOOL bClearPre = TRUE, BOOL bSpacePost = TRUE);
    void Write_GDisplayAnalString_Raw(CString strTxt);

    template <typename... Ts> void Write_GDisplayAnalStringFormat(const char* Fmt, const Ts& ...Values)
    {
        if ( CDBLib::IsIncludeMessageSkipRange() )
        {
            return;
        }

        GDisplayAnalStringFormat(Fmt, Values...);

        return;
    }

    void Write_GDisplayAnalStringSamePos(CString strTxt);

    template <typename... Ts> void Write_GDisplayAnalStringSamePosFormat(const char* Fmt, const Ts& ...Values)
    {
        if ( CDBLib::IsIncludeMessageSkipRange() )
        {
            return;
        }

        GDisplayAnalStringSamePosFormat(Fmt, Values...);

        return;
    }

	// Add by ZINU.('02.7.8).
	int GetOkEqACI();
	// See GB50010-02 11.1.4, 3.2.3. p.166,11,  JGJ138-01 6.1.6~6.1.8. p.30~31.
	BOOL IsChinaCode();
	BOOL IsChinaCode_StlEQNEW();
	int GetOkEqGB();
	int GetNoEqGB();
	BOOL IsFrmSupBeamGB();
	BOOL IsCornerColmGB();
	BOOL IsFrmSupColmGB();
	BOOL IsStrRgnWallGB();
	BOOL IsCoTallWallGB();
	BOOL IsShortWallGB(); // Add by GAY. PMS:XXXX. ('12.02.08). ShortWall for GB/T50010-10.
	BOOL Get_DeadLiveGB(int iElemNo, int iPosiNo);
	BOOL Get_DeadLiveIS(int iElemNo, int iPosiNo);
	// Add by ZINU.('03.01.11). Agree with Kye CJ.
	BOOL Get_EqFactorForLcase(BOOL bLcaseEQ);
	BOOL Get_EqFactorForLcom(int iElemNo, int iPosi, int iSeismicType);
	double Get_Alpha2(); // for Ve2
	// Add by ZINU.('04.01.05). Agree with Kye BJ.
	int Cng_ElemSubKindColmGB(int iElemNo, int iPosiNo);
	// Add by ZINU.('03.01.16). For EQ Type(0=No, 1=Lateral, 2=Vertical).
	int GetLcomTypeEq(int iLcomNo);
	BOOL IsEqSpecialLcom(int iLcomNo);
	BOOL IsEqSeismicLcomForBCJoint(int iLcomNo);
	CString GetDgnLcomName(int nLcomNo);
	BOOL IsEqSpecialElem(T_KEY ElemK, bool bWall = false);  // PMS:3956-Seungjun-20090615 EC8:04 Seismic Design.  
	BOOL IsEqSFRSElem(T_KEY ElemK);     // Modify by GAY. PMS:4076. ('10.04.28). Is Member designated as Seismic-Force-Resisting-System.
	BOOL IsEqSpecialProv();
	BOOL GetMomByEqSpecial(int iLcomType, double& dMp, double& dMm, double& dVu, int& rnPosMp, int& rnPosMm);
	BOOL GetMomByEqSpecial(int iLcomType, double& dMp, double& dMm, double& dVu, double& dTu, int& rnPosMp, int& rnPosMm);
	BOOL GetSpecialFactor(double& dEndIF, double& dEndJF, double& dMiddleF);  // MQC.2335-Seungjun-20090204
	// Add by Jaeoh.('06.02.10)
	BOOL GetForceKCI03(double& dMp, double& dMm, double& dVu);
	BOOL GetForce_IS456(double& dMp, double& dMn, double& dVu);
	BOOL GetForce_IRS(double& dMp, double& dMn, double& dVu, double& Tu, int iElemNo);

	BOOL IsLongTerm(int iLcomNo);
	int GetLcomTypeGen(int iLcomNo);	// Lcom Type (L,S,X+/-,Y+/-).
	int GetLcomTypeCode(int iLcomNo);	// by Code (JTJ, IRC, AASHTO).
	// Add by ZINU.('03.01.17). For Vertical Eq.
	void SetSeismicType(T_LCOM_BASE& LcomBase, int& iSeismicType);

	// Add by ZINU.('02.8.24).
	int IsCantilever(T_ELEM_K ElemKey);
	BOOL IsSubBeamForNoMpr(T_ELEM_K ElemK, int nCantilever);
	BOOL IsSimplySupBeam(BOOL bMajorAxis);
	BOOL IsUnderGroundMemb(T_ELEM_K ElemKey);
	int  Get_AutoEqSpecialElemType(T_KEY ElemK);
	BOOL IsSingleCurve(double dM04, double dM14, double dM12, double dM34, double dM44);
	// Add by GAY.('06.02.28) Is j-node Top Position?
	BOOL IsJNodeTop(T_ELEM_K ElemKey);
	// Add by ZINU.('02.12.03). For Lateral Displacement Control.
	void SetLongTermDgn(BOOL bLongTerm);

	bool IsUseLcomUG(T_ELEM_K ElemKorUnInterWallID);

	// Pre-stressed, Tendon.
	// Add by ZINU.('02.12.21). To Check Final Lcom for Service.
	BOOL IsFinalSerLcom(int iLcomNo);
	BOOL IsTransferStage(int iLcomNo);

	BOOL GetStrbDgn(T_ELEM_K Key, int iLcaseType, int LcaseK, T_STRB_D*  pDataMax, T_STRB_D*  pDataMin);
	BOOL GetStrtDgn(T_ELEM_K Key, int iLcaseType, int LcaseK, T_STRT_D*  pDataMax, T_STRT_D*  pDataMin);
	BOOL GetStrwDgn(T_ELEM_K Key, int iLcaseType, int LcaseK, T_STRW_D*  pDataMax, T_STRW_D*  pDataMin);
	BOOL GetStrpDgn(T_ELEM_K Key, int iLcaseType, int LcaseK, T_STRP_DL* pDataMax, T_STRP_DL* pDataMin);
	BOOL GetStrbDgn(T_ELEM_K StaElemK, T_ELEM_K EndElemK, int iLcaseType, int LcaseK, T_STRB_D* pDataMax, T_STRB_D* pDataMin);

	double Get_GravelDia();
	// Add by ZINU.('04.01.09). Calculate Distance between Rbar and Steel of SRC.
	double Calc_RbarToStlDist(double dBarDia);
	// Add by ZINU.('04.06.17). Calculate Distance between Conc and Steel of SRC.
	double Calc_ConcToStlDist();

	void InitSavedDgnForce();

	// Coded by Seungjun MNet:No.2591 ('20061214)	For SCWB Design.
	void Test_TextOutPutForSCWB(BOOL bPrint, CString strFileName, _DGN_SCWB_K& ScwbK, _DGN_SCWB_D& ScwbD);

	// Add by GAY. MNET:2683. ('07.05.04). Check exist Crane Load.
	BOOL IsExistCraneLcom() { return m_bExistCraneLcom; }

	// Add by GAY. MNET:2729. ('07.06.05). Check ConCurrent Force.     
	BOOL IsConcurrentLcomType12() { return m_bChkConCurForce12; }

	UINT GetOriginLcomKey() { return m_iOrgLcomKey; }

	// Add by GAY. ('13.06.11). PMS:4632.
	BOOL IsValidLcom(const BOOL &b7DOF, const int &nLcomCompType);

	// Add by GAY. MNET:2675. ('07.06.12). Make Load Combination Description. Reference CRCSC_DgnPrint::Print_Lcom(...).
	CString Make_LcomDescript(const T_LCOM_D_UL& LcomUL, OUT CArray<CString, CString&>& raLcasLcomName);

	// Add by GAY. MNET:2675. ('07.12.05). Make Load Combination Description. arLcaseKey is list of (AnalType + LcaseKey).
	CString Make_LcomDescript(const T_LCOM_D& LcomD, OUT T_KEY_LIST& raDgnLcaseK);

	//(2007.12.11) Add by Unsang ::(AnalType, LcaseKey)의 조합Key로 Load Case Name 및 Description 찾기.
	BOOL Get_LcaseNameDescript(UINT DgnLcasK, CString& strLcasName, CString& strLcasDesc);

	BOOL Get_ZeroFactor(double& dZeroLen, double& dZeroFor);
	void Get_CTCAppliedForce(int iElemK); // Add, Jaeoh(07.09.13)  

	// PMS:3956-Seungjun-20090224 EC8:04 Seismic Design
	double Get_ElasticResponseSpectrum(int iSeismicType, double dPeriod);
	double Get_BehaviourFactor(int iStrucType, int iDgnClass, double dAlphau_1, BOOL bNTC, int nNTCType);
	double Get_PrevailingFactor(int iStrucType);
	void   Set_nth_ModePeriod(int iMode);
	void   Set_1st_ModePeriodByDir();
	void   Set_Init_ModePeriod();
	double Get_qForEC4();

	double Get_FinalPeriod(double dPeriodAnal, double dPeriodCode, double dFact);
	void   Get_Period_StaticSeismicByDir(T_KEY SeisK, double& dPeriodX, double& dPeriodY); // Add by GAY. PMS:3787. ('09.12.06). Static Seismic Load의 기본주기.
	void   Set_Period_StaticSeismicByLcom(const T_LCOM_D_UL& LcomDUL);
	void   Set_Period_StaticSeismicByLcom(int iLcomNo);
	void   Set_Period_UserT1(const T_DCON_D& DconD);

	// Add by GAY. MQC:2972. ('09.10.28). Not Apply Special Seismic Design for RC Beam.
	BOOL IsApplyEqSpecialForceRCBeam();
	BOOL IsApplyEqSpecialForceRCColm();
	BOOL IsApplyEqSpecialForce(T_ELEM_K ElemK);
	// PMS:4182-Seungjun-20100608 KBC2009 Seismic design.
	BOOL IsSubBeam();
	BOOL IsUnderGroungElem();
	BOOL IsCaltileverBeam();

	BOOL IsNTC2018();
	BOOL IsApplyNonDissipative();

	// PMS:4230-Seungjun-20110411 일본 2차 설계 위치별 부재력
	double Get_MomentInterpolate(double dM1, double dM2, double dMmid, double dLx, double dL);

	double Get_MomentInterpolate(double dM[5], double dL[5], double dPosiLen);

	// PMS:4352-Seungjun-20110930 EC Inclined slab.
	BOOL Get_InclinedSlabPF(int ElemK, int nSerial, double dPlateF[8]); // Mxx, Myy, Mxy, Fxx, Fyy, Fxy, Vxx, Vyy

	BOOL IsFrameStructure_GB10();
	BOOL IsApplyNTC() { return m_iDgnCode == Eurocode2_04 && m_iDgnSubCode == ITALY && m_bNAEQDgn ? TRUE : FALSE; }

	// Modify by GAY. MQC:5308. ('12.11.01). Slab의 휨철근 계산 수정. 
	// (AddRebar2의 Space는 DB에 저장되지 않으므로 AddRebar1의 Space로 계산, AddRebar2가 입력되지 않았을 때의 계산식 수정)
	double GetSlabUsedAs(const T_RBMS_D &RbmsD, const double &dWidth);
	double GetSlabUsedAs(const CString& strBarName, double dSpace, double dWidth);

	void MakeAdjacentInternalWallIDMap(int nUserWallId);

private:
	void Get_SeisLcomForceFactor_Beam_GB50010_10(int nSubType, int nSeisGrade, double& dCodeLcomN, double& dCodeLcomM, double& dCodeLcomV);
	void Get_SeisLcomForceFactor_Colm_GB50010_10(int nSubType, int nSeisGrade, BOOL bFrameStruct, double& dCodeLcomN, double& dCodeLcomM, double& dCodeLcomV);
	void Get_SeisLcomForceFactor_Wall_GB50010_10(int nSubType, int nSeisGrade, double& dCodeLcomN, double& dCodeLcomM, double& dCodeLcomV);

	void Get_SeisLcasForceFactor_BeamSupportFrame_GB50010_10(int nSeisGrade, double& dCodeLcasN, double& dCodeLcasM, double& dCodeLcasV);
	void Get_SeisLcasForceFactor_ColmSupportFrame_GB50010_10(int nSeisGrade, double& dCodeLcasN);

	UINT Get_SeisFortificationMax();

	BOOL IsRegardZeroTorsion(const double *pdTorsion, const int &nPosi, const double &dLimit, const double &dTolZero);
	BOOL IsRegardZeroForce(const double *pdForce, const int &nPosi, const double &dLimit, const double &dTolZero);
	BOOL IsRegardZeroMoment(const double &dValue, const int &nPosi, const double &dLimit, const double &dTolZero, const double &dValueOrtho);

	void GetSeisReqMomentFactor_KCI_USD07_Series(T_ELEM_K ElemK, int nEqGrade, double& rdEndF, double& rdMidF);
	void GetSeisReqMomentFactor_ACI318_Series(T_ELEM_K ElemK, int nCode, int nEqGrade, double& rdEndF, double& rdMidF);
	void GetSeisReqMomentFactor_NTC_Series(T_ELEM_K ElemK, int nCode, int nEqGrade, double& rdEndF, double& rdMidF); // Mexico

	void SetUlctMap(OUT CMap<UINT, UINT, int, int>& rmUlct);
	bool GetUnderGroundTypeByUlct(UINT ElemKorUnInterWallID, OUT int& rnType);

	bool IsCalcWallDesignForce(int nElemNo, const _DGN_LCOM& LcomDesign);
	bool IsSlenderWallByEC8(double dTotalHeight, double dLength);

public:
	bool IsWall();

	CString Get_OrgLcomName(const _DGN_LCOM& Data, bool bPlate=false);

	// PMS:4020-Seungjun-20090708 Italy Slab/Wall Design.
	// Member Variables.
public:
	//Slab Design Options
	BOOL	m_bSlabDesignCheck;
	BOOL  m_bAverage;
	int		m_nTopBottom;
	int		m_nCellOption;
	double m_dCellSize;
	double m_dCreepCoef;
	int		m_nSlabDsgDir;
	int		m_nLegendType; // 0: Rebar 1:As Value, 2:ratio_value
	int		m_nCrackType; // 0: Width 1: Min As 2:Rebar 3: space // check 785 //
	BOOL	m_bOneWayMomentCheck;
	int   m_nCrackDef;
	int   m_nLongTermType; // 0 : by Time Factor ( ACI, KCI ),  1 : by Creep Factor ( EC )

	BOOL	m_bPunchingShearCheck;
	int   m_nPnChkType;	//0:Force, 1:Stress
	int   m_nPnChkStrsType; //0:Avg. by Elem, 1:Avg. by Side  
	BOOL	m_bOneWayShearCheck;
	BOOL  m_bUnbalMomIS;//Pinakin - IS456 SLAB/MAt - Unbalanced moment check

	BOOL  m_bMeshWallDgn;

	UINT  m_nSelectLcomK; // GPS에서 선택한 LcomK. 0이면 All combination.
	DGN_LCOM_MAX_TYPE m_nMaxMinType;  // env 조합인 경우, Max/Min type (1=Max,2=Min)

	int   m_PForcComponent;

	BOOL  m_bCalcOneway;
	double m_dCuttingLineAngle; // Degree.

	int m_nULSnELS;  // limit state of slab flexural checking.
	int m_nShearnAdhesive;

	// cutting line
	int   m_nCuttingDiagramLeftRight; // one way shear cutting line option.

	// Member force.
	double m_dMuTop[4];
	double m_dMuBot[4];
	double m_dMuDTop[4];
	double m_dMuDBot[4];
	double m_dMuLTop[4];
	double m_dMuLBot[4];
	double m_dStressx[4];
	double m_dStressy[4];
	double m_dStressxy[4];

	// PMS:4352-Seungjun-20111011 EC Inclined slab.
	double m_dMxx[4];
	double m_dMyy[4];
	double m_dMxy[4];
	double m_dFxx[4];
	double m_dFyy[4];
	double m_dFxy[4];
	double m_dVxx[4];
	double m_dVyy[4];
	double m_dSigMaxTop[4];
	double m_dSigMinTop[4];
	double m_dSigMaxBot[4];
	double m_dSigMinBot[4];

	// Cutting Contour & Diagram
	CMapEx<UINT, UINT, DGN_CUTLINE_AR, DGN_CUTLINE_AR&>  m_arPnChkCutl;

	// 설계 결과
	// 2way For Flexural.
	CMap<UINT, UINT&, _DGN_1WAY_SLAB_RES, _DGN_1WAY_SLAB_RES&> m_OnewaySlabResTopData;
	CMap<UINT, UINT&, _DGN_1WAY_SLAB_RES, _DGN_1WAY_SLAB_RES&> m_OnewaySlabResBotData;
	// 1way For Flexural.
	CMap<UINT, UINT&, _DGN_1WAY_SLAB_RES, _DGN_1WAY_SLAB_RES&> m_OnewaySlabResData;
	// 2way For Shear(punching shear)
	_DGN_PUNCH_RES_MAP m_mapPunchingData;
	// 1way For Shear.
	CMap<UINT, UINT&, _DGN_1WAY_RES, _DGN_1WAY_RES&> m_OnewayResData;

	// Element and sub-domain
	CMapEx<T_ELEM_K, T_ELEM_K, T_SBDO_K, T_SBDO_K> m_aSbdoByElem;
	CMapEx<T_SBDO_K, T_SBDO_K, T_SBDO_K, T_SBDO_K> m_aSbdoList;
	CMapEx<T_SBDO_K, T_SBDO_K, T_MADO_K, T_MADO_K> m_aMadoBySbdo;

	CArray <unsigned int, unsigned int>		m_PlateKeyList;
	CArray <unsigned int, unsigned int>		m_ValidPlateKeyList;
	CMapEx<UINT, UINT, _DGN_CELL_FORC, _DGN_CELL_FORC&> m_mapCellElem;

	BOOL   m_bPlateForce;
	double m_dPlateAngle;
	double m_dTimeChk[10];

	CDgnQuadTree m_QuadTree;

	// PMS:xxx-Seungjun-20100830 sub-domain이 동일 평면이 아닐 경우 사용. only wall. 
	std::map<T_ELEM_K, int> m_aSubSubDomain;
	std::map<int, T_VECTOR3BY3D> m_aSubSubDomainVec;


protected:
	CMap<T_ELEM_K, T_ELEM_K, int, int>	m_mapNodeInElem;
	CMap<UINT, UINT, T_SLAB_FORCE_COMPACT, T_SLAB_FORCE_COMPACT&>	m_mapSlabForc_Compact;
	CMap<UINT, UINT, T_SLAB_FORCE_COMPACT, T_SLAB_FORCE_COMPACT&>	m_mapSlabForc_Compact_D;
	CMap<UINT, UINT, T_SLAB_FORCE_COMPACT, T_SLAB_FORCE_COMPACT&>	m_mapSlabForc_Compact_L;

	CMap<UINT, UINT, T_SLAB_FORCE_COMPACT, T_SLAB_FORCE_COMPACT&>	m_mapSlabForc_CompactbyCell;

private:
	// Add by SHIN - 2011.06.14 : PMS:4215 : 일본2차설계시 1차설계에서 응력할증되는 Elem와 Posi정보 (냉간성형강,  Brace분배율 등에 의해 지진하중만 증가시킬때 사용)
	_DGN_EXPAND_LOAD_ELEM m_ExpandElemD;
	// Add by SHIN - 2011.06.15 : PMS:4215 : 일본2차설계시 1차설계에서 응력할증되는 정보
	_DGN_EXPAND_LOAD_D m_ExpandLoad;

	void GetExpandEqFactor(T_ELEM_K ElemK, int nPosi, CString strLcaseType, double dFactor, double& dExFactor_P, double& dExFactor_V, double& dExFactor_M, double& dExFactor_T);
	// ※주의!!! - SetExpandElem과 InitExpandElem은 항상 쌍으로 움직입니다.
	void SetExpandElem(_DGN_EXPAND_LOAD_ELEM& ExpandElemD); // 검토할 Elem변경시 적용
	void InitExpandElem();                                  // 해당 Elem으로 계산완료후 필히 초기화 할것

	void GetRSFactor(const T_DCON_D& Dstl, T_ELEM_K ElemK, int iLcaseType, CString strLcaseType, double& dRFx, double& dRFy, double& dRFz, double& dRMx, double& dRMy, double& dRMz);

public:
	// Add by SHIN - 2011.06.15 : PMS:4215 : 일본2차설계시 1차설계에서 응력할증되는 정보 생성 및 초기화, 계산정 됭보 가져오기 기능
	void Make_ExpandLoad();
	void Make_ExpandLoad_Elem(T_ELEM_K ElemK);
	void Init_ExpandLoad();
	BOOL Get_ExpandLoadElemD(T_LCOM_K LcomK, T_ELEM_K ElemK, _DGN_EXPAND_LOAD_ELEM& ExpandElemD);

private:
	// Seungjun-20110411 일본2차설계용 단부 거리 계산 방법. 0-강역(현재 그대로), 1-node to node(모델링 길이), 2-face to face(순길이)
	// [0]:장기 [1]:단기
	int m_nMLengTypei[2];  // for I-End
	int m_nMLengTypej[2];  // for J-End
	int m_nVLengTypei[2];  // for I-End
	int m_nVLengTypej[2];  // for J-End  
	// Offset 길이의 비율입니다. (OffsetLength/NetLength) (비율만 있으면 계산이 가능하며 단위계에 영향을 받지 않습니다.)
	double m_dOffSetRyi[2];
	double m_dOffSetRzi[2];
	double m_dOffSetRyj[2];
	double m_dOffSetRzj[2];
public:
	void Init_LengType()
	{
		m_nMLengTypei[0] = m_nMLengTypej[0] = m_nVLengTypei[0] = m_nVLengTypej[0] = m_nMLengTypei[1] = m_nMLengTypej[1] = m_nVLengTypei[1] = m_nVLengTypej[1] = 0;
		m_dOffSetRyi[0] = m_dOffSetRzi[0] = m_dOffSetRyj[0] = m_dOffSetRzj[0] = m_dOffSetRyi[1] = m_dOffSetRzi[1] = m_dOffSetRyj[1] = m_dOffSetRzj[1] = 0.0;
	}

	void Set_JUDLengType(int ElemKey);
	BOOL Get_JUDLengType(int ElemKey, int nMLengTypei[2], int nMLengTypej[2]);

	// Member Functions.
public:
	void   Init_PlateData();

	BOOL   MakePForcDataforDesign_Compact(_DGN_LCOM& LcomDesign, UINT SbdoK = NULL);
	BOOL   MakePForcDataforDesign_Compact(int nType);  // use only GPS. nType : 0:slab 1:wall  

	void   GetAverageNodalDataList_Compact(CArray<T_ELEM_K, T_ELEM_K>& aElemKeys, CMap<UINT, UINT, T_SLAB_FORCE_COMPACT, T_SLAB_FORCE_COMPACT&>& mapSlabForc);
	void   CalcWoodArmer(double dmxx, double dmyy, double dmxy, double dalpha, double dphi, double& dUpper, double& dLower, double dVectrSBDO[3][3], BOOL bUseWoodArmer);
	void   CalcWoodArmer(double dmxx, double dmyy, double dmxy, double dalpha, double dphi, double& dUpper, double& dLower, double dVectrSBDO[3][3]);
	void   ConvertForce(UINT iElem, double &dmxx, double &dmyy, double dalpha, double dphi, double dVectrSBDO[3][3]);
	void   CalcNotWoodArmer(double dmxx, double dmyy, double dmxy, double dalpha, double dphi, double& dUpper, double& dLower, double dVectrSBDO[3][3]);
	void   CalcMeshWallForce(int ElemK, double dThk, double dSigx, double dSigy, double dSigxy, double& dftdx, double& dftdy, double& dSigcd);
	double CalcPForcValue_G(const T_STRP_DG &pforc, int node, int nComponent);
	double CalcPForcValue_L(const T_STRP_DL &pforc, int node, int nComponent);

	double CalcMomentByDgnDir(int nDgnDir, double dmxx, double dmyy, double dAlpha, double dPhi, double dVectrSBDO[3][3]);
	double CalcShearByDgnDir(int nDgnDir, double dvxx, double dvyy, double dAlpha, double dPhi, double dVectrSBDO[3][3]);

	BOOL   Set_PlateForceMoment(UINT iElem);
	BOOL   Set_SlabDesignData_byElem(UINT iElem);

	BOOL   Set_SlabDesignData_byCell(UINT iElem);
	BOOL   Set_SlabDesignData_byCell_SlabShrJP(UINT ElemK);
	BOOL   Set_WallDesignData_byCell(UINT iElem);
	BOOL   Set_ShelDesignData_byCell(UINT iElem);

	int    Get_SlabNode_In_Cell(UINT DomnK, T_NODE_K NodeK, _DGN_CELL_FORC& arNodeElemList);
	int    Get_WallNode_In_Cell(UINT DomnK, T_NODE_K NodeK, _DGN_CELL_FORC& arNodeElemList);
	int    Get_ShelNode_In_Cell(UINT DomnK, T_NODE_K NodeK, _DGN_CELL_FORC& arNodeElemList);

	void   Set_OneWayMomentCheckData();
	void   Set_OneWayShearCheckData();

	BOOL   SubMakePeripartForTwowayForceShearCheck(_DGN_PERI_PART_MAP& mPeriPart);
	void   Set_OneWaySlabDataParts(_DGN_1WAY_SLAB_RES& _dgn_1way_slab_data, unsigned int lcom_k, double dMu, CArray<DGN_CUTLINE_D, DGN_CUTLINE_D&>& PartData, BOOL bTop);
	void   Set_OneWayDataParts(_DGN_1WAY_RES& _dgn_1way_data, unsigned int lcom_k, double dVu, CArray<DGN_CUTLINE_D, DGN_CUTLINE_D&>& PartData);

	BOOL   SubMakePeripartForTwowayStressShearCheck(_DGN_PERI_PART_MAP& mapPeriData);
	BOOL   SubMakeCutlLineForTwowayShearCheck(_DGN_PUNCH_RES_MAP& mapPunchingData, _DGN_PERI_PART_MAP& mapPeriData);
	BOOL   SubMakePeripartForTwowayRebarCheck(_DGN_PUNCH_RES_MAP& mapPunchingData, _DGN_PERI_PART_MAP& mapPeriData);

	BOOL   MakeSbdoQuadTree(T_SBDO_K SbdoK);

	BOOL   IsExistSamePeriResData(UINT PntK, _DGN_PERI_RES &PeriRes, CArray<_DGN_PERI_RES, _DGN_PERI_RES> &arPeriRes);
	BOOL   IsPossibleCuttingLine(T_ELEM_K ElemK, double dCutLinePt1[3], double dCutLinePt2[3],
		T_NODE_D &ptCutted1, T_NODE_D &ptCutted2, int nLeftRight);
	void   GetActivePlateList(BOOL bWall = FALSE);
	int    GetAllPlateListForShear(CArray<UINT, UINT>& aAllKeys, BOOL bWall = FALSE);
	int    Get_Position(UINT ElemK, int iPunchShp, CArray<int, int>& aPeriSectNum, int& iTopIdx);
	int    GetPositionByUser(UINT ElemK);

	void   Set_CuttingLine(CDgnQuadTree &QuadTree, CArray<int, int>& aPeriSectNum, _DGN_PERI_PART& PeriPartD);

	void   Calc_RebarAreaByCutLine(double dUseAs, double dAngel, double dX[2], double dY[2], double dAs[2]);
	void   ResetStoryData();
	void   SetStoryData();
	BOOL   ExistStory();// void CDgnForceCtrl::SetStoryData() 호출 후 사용

	int GetStoryCountAboveGround(); // Add by GAY. PMS:3787. ('09.12.05). 지상층의 개수 가져오기.

	// For Strip Design
	void GetActiveStripList(T_MSTR_K_LIST &aDgnStripList);
	T_MSTR_K GetMstrKByElemK(T_ELEM_K ElemK);

	static int Compare_Cutline_Dist(const void *Data1, const void *Data2)  // sorting compare function
	{
		double d1 = __min(((DGN_CUTLINE_D*)Data1)->dDist1, ((DGN_CUTLINE_D*)Data1)->dDist2);
		double d2 = __min(((DGN_CUTLINE_D*)Data2)->dDist1, ((DGN_CUTLINE_D*)Data2)->dDist2);

		if (d1 > d2) return 1;
		else if (d1 == d2) return 0;
		else              return -1;
	};

	BOOL Get_bExistLctb() { return m_bExistLctb; };

	INT_PTR Get_StldKeyListByLcaseType(const CString& strType, OUT T_KEY_LIST& raStldK);
	BOOL  Get_BeamForcebyStld(T_KEY ElemK, const T_KEY_LIST& aStldK, int nComType, double rForce[CONST_DGN_iPOSNUM]);

private:
	void GetCutLinePoints(const _DGN_PERI_POS& PeriPos, double dLevel, OUT double adCutPnt1[3], OUT double adCutPnt2[3]);


	// RC Plate 관련
public:
	// Plate
	// Set_ForceMomentBySeperate_Rcpl()호출시에 채워지는 Rcpl영역의 최대(0), 최소(1) UnitForce, 발생Elem Key	
	// Total
	double m_Fxx_Rcpl[2], m_Fyy_Rcpl[2], m_Fxy_Rcpl[2];
	double m_Vxx_Rcpl[2], m_Vyy_Rcpl[2];
	double m_Mxx_Rcpl[2], m_Myy_Rcpl[2], m_Mxy_Rcpl[2];
	// Dead
	double m_FxxDL_Rcpl[2], m_FyyDL_Rcpl[2], m_FxyDL_Rcpl[2];
	double m_VxxDL_Rcpl[2], m_VyyDL_Rcpl[2];
	double m_MxxDL_Rcpl[2], m_MyyDL_Rcpl[2], m_MxyDL_Rcpl[2];
	// Moving
	double m_FxxML_Rcpl[2], m_FyyML_Rcpl[2], m_FxyML_Rcpl[2];
	double m_VxxML_Rcpl[2], m_VyyML_Rcpl[2];
	double m_MxxML_Rcpl[2], m_MyyML_Rcpl[2], m_MxyML_Rcpl[2];
	_DGN_PSTR_LCOMRC m_Rcpl_Strp; // Rcpl영역의 최대(0), 최소(1) 및 발생 Elem Key 정보를 가짐

	void Reset_RcplForce();
	void Set_LcomDataForRcpl(int iRcpl);
	void Set_ForceMomentBySeperate_Rcpl(int iRcpl, _DGN_LCOM& LcomDesign);
	void Get_ForceMoment_Rcpl(int iRcpl, _DGN_LCOM& LcomDesign);
	BOOL Get_DgnPlateForcDataList(CArray<T_ELEM_K, T_ELEM_K>& arElemK, _DGN_LCOM& LcomDesign, _DGN_PSTR_LCOMRC& RcplForce, int nType = 0);
	void Get_STRP_DgnForcCtrl(T_STRP_DL& StrpDL);
	void Get_STRP_DgnForcCtrl_ML(T_STRP_DL& StrpDL);
	void Get_STRP_DgnForcCtrl_DL(T_STRP_DL& StrpDL);

	double GetMovingLiveLoad_Rcpl(T_RCPL_K RcplK);

	// Plate Rating
	void Reset_RrplForce();
	void Set_LcomDataForRrpl(int iRrpl);
	void Set_ForceMomentBySeperate_Rrpl(int iRrpl, _DGN_LCOM& LcomDesign);
	void Get_ForceMoment_Rrpl(int iRcpl, _DGN_LCOM& LcomDesign);

private:
	BOOL m_bChkStatic;
	BOOL m_bEndLCase;
	BOOL IsCheckStaticDL();
	BOOL IsRatingCode();
	BOOL IsEndLCase();
	void AddPlateDLForce(int nIndex, T_STRP_DL &StrpMaxData, double dFactor);
	void AddPlateDLForce(int i, T_STRP_DL &StrpMaxData, double dFactor, double dReduAxlFactor, double dReduShrFactor, double dReduMomFactor);

	BOOL MakePForcDataforDesign_Compact4LType(T_LCOM_D& LcomD, int LcaseType, double dFactor, UINT SbdoK = NULL);
	void MakePForcDataforDesign_Compact_SaveSlab();
	void MakePForcDataforDesign_Compact_SaveWall();
	void MakePForcDataforDesign_Compact_SaveShel();

	// Add by GAY. ('13.05.09). 해석부재력을 받아오는 Index 얻어오기.
	void GetAnalForceComponentIndex(const int &nPosi, int &rnPart, int &rnDOF6, int &rnDOF7);
	void GetLcomCompTypeString(int nDgnNo, CArray<CString, CString&> &raCompString);

	BOOL Set_SlabDesignData_byElem_Slab(UINT iElem);
	BOOL Set_SlabDesignData_byElem_SlabServ(UINT iElem);
	BOOL Set_SlabDesignData_byElem_SlabShrJP(UINT iElem);
	BOOL Set_SlabDesignData_byElem_Wall(UINT iElem);
	BOOL Set_SlabDesignData_byElem_IncSlab(UINT iElem);
	BOOL Set_SlabDesignData_byElem_Code_CH(UINT iElem);
	BOOL Set_SlabDesignData_byElem_Shell(UINT nElem);

	int  GetPlateNodeNum(const T_ELEM_K& ElemK, const T_ELEM_D& ElemD);

	double GetShelForc4Cell(EN_SHELFORC_IDX enForcIdx, const T_SLAB_FORCE_COMPACT& Forc1, int nNodeIdx1, const T_SLAB_FORCE_COMPACT& Forc2, int nNodeIdx2, double dLeng);

private:
	// CIVIL-1898 Longterm load case인 경우 DL로 처리; Eurocode2 and Serviceability
	bool m_bService;
	void SetService(bool bService) { m_bService = bService; }
	bool IsService() { return m_bService; }

	bool IsDeadLoadType(CString strLcaseType, int iAnalType, int iLcaseKey);
	bool IsLiveLoadType(CString strLcaseType, int iAnalType);

	void AddDeadSum(const int& iPosi,
		const double& dFxx, const double& dFyy, const double& dFzz,
		const double& dBMx, const double& dBMy, const double& dBMz,
		const double& dMbi, const double& dTst, const double& dTwp);
	void AddLiveSum(const int& iPosi,
		const double& dFxx, const double& dFyy, const double& dFzz,
		const double& dBMx, const double& dBMy, const double& dBMz,
		const double& dMbi, const double& dTst, const double& dTwp);
	void AddMovingLiveSum(const int& iPosi,
		const double& dFxx, const double& dFyy, const double& dFzz,
		const double& dBMx, const double& dBMy, const double& dBMz,
		const double& dMbi, const double& dTst, const double& dTwp);
	void AddRemnantSum(const int& iPosi,
		const double& dFxx, const double& dFyy, const double& dFzz,
		const double& dBMx, const double& dBMy, const double& dBMz,
		const double& dMbi, const double& dTst, const double& dTwp,
		const double& dFyyOrg, const double& dFzzOrg);

public:
	BOOL IsElem7DOF(const T_ELEM_K &ElemK);

private:
	// for jfd.
	T_FIFF_D m_GroundBeamForce;

private:
	CMapEx<T_SBDO_K, T_SBDO_K&, T_VECTOR3BY3D, T_VECTOR3BY3D&> m_mSbdoVectr;
	BOOL m_bWoodArmer;
public:
	void InitSbdoVector(int nSbdoNo);
	void GetOrSetSbdoVector(T_SBDO_K SbdoK, double dVectrSBDO[3][3]);
	void SetWoodArmerOption();

	int GetCutlKeyList(OUT CArray<UINT, UINT>& raCutlK);
	int GetCutlElemList(T_CUTL_K CutlK, const T_ELEM_K_LIST& aElemK, OUT T_ELEM_K_LIST& raCutlElemK);

public:
	BOOL IsColdFormDesign(T_ELEM_K ElemK, BOOL bSteel);

	bool IsLcomStrn(int nActive);
	bool IsLcomServ(int nActive);

	bool IsDgnLcomStrn(int nLcomType, int nDgnLcomNo);
	bool IsDgnLcomServ(int nLcomType, int nDgnLcomNo);

	int GetSlcmTypeSlab(T_ELEM_K ElemK);

	bool IsJointCodeUseVcol(const int& iCode);

	T_ELEM_K Get_LowerColmNo(T_NODE_K NodeK);
	T_ELEM_K Get_UpperColmNo(T_NODE_K NodeK); // MQC:3642-Seungjun-20100908 상부 기둥이 존재하는 경우.
	bool     Is_BeamPoint(T_NODE_K NodeK, bool bExcludeEbeam=false); // 보가 연결된 부재인가?

	// option of smoothing optimal timing for wood-armer force.
private:
	BOOL m_bWA_Before; // FALSE; 부재력 smoothing 후 wood-armer 계산, FALSE; 각각 부재력 상태의 wood-armer 계산 먼저하고 각각 smoothing
public:
	void SetWoodArmerBefore(int bWA_Before) { m_bWA_Before = bWA_Before; }
	BOOL IsWoodArmerBefore() { return m_bWA_Before; }

private:
	BOOL CheckOverlapCutLine(const CMapEx<UINT, UINT, DGN_CUTLINE_AR, DGN_CUTLINE_AR&>& mCutl);
	BOOL MakeRgnCutLine(const DGN_CUTLINE_AR& Cutl, OUT CRgn& rRgn);
	BOOL MakeRgnPunchingCutLine(const CArray<T_RSS2_CTLN,T_RSS2_CTLN&>& arPnChkCutl, OUT CRgn& rRgn);

public:
	BOOL CheckOverlapPunchingCutLine();

private:
	// PMS.5537 Civil Shell Design Eurocode2_2_05
	void CalcPlateLocalForce4Civil(const int& nPlateForceLocalGlobal, const CString& strUSC, const T_SBDO_K& SbdoK, const int& elem_key, T_STRP_DL&	data_max, T_STRP_DL& data_min, double LVec[3][3]);

	void GetShellDgnVector(double dAngle, double adSrcVec[3][3], OUT double adDgnVec[3][3]);

	int GetForceComponentSize(BOOL bExistCrane, BOOL bConCurrent12, int nDgnCode, BOOL bPlateBC);

	bool GetDgnLcomGroupMap(int nLcomKind, OUT CArray<_DGN_LCOM_MAP*, _DGN_LCOM_MAP*>& raLcomMap);

	CString GetLcasLcomName(T_KEY LcasLcomK, int nLcomAnalType);
	CString GetMovingLcaseName(T_KEY LcasK);

private:
	CMapEx<T_KEY, T_KEY, EN_DGN_SEIS_CLASS, EN_DGN_SEIS_CLASS> m_mSeisClassFram;
	CMapEx<T_KEY, T_KEY, EN_DGN_SEIS_CLASS, EN_DGN_SEIS_CLASS> m_mSeisClassWall;
	CMapEx<T_KEY, T_KEY, EN_DGN_SEIS_CLASS, EN_DGN_SEIS_CLASS> m_mSeisClassSlab;

	std::unordered_map<UINT, ADJ_INTWALLID_K> m_mAdjIntwall;

public:
	EN_DGN_SEIS_CLASS GetSeisClassFram(T_ELEM_K ElemK);
	EN_DGN_SEIS_CLASS GetSeisClassWall(int nInternalWallID);
	EN_DGN_SEIS_CLASS GetSeisClassSlab(T_ELEM_K ElemK);

	void SetSeisClassMap();

public:
	void SetDataForSeismic();
	void SetMemberDataForSeismic(const UINT& iMemb);
	void SetDataForSeismic4Steel();

private:
	CMap<UINT, UINT, UINT, UINT> m_mPerformDgnLcomIdx;

public:
	void InitPerformDgnLcomIdx() { m_mPerformDgnLcomIdx.RemoveAll(); }
	void SetPerformDgnLcomIdx(const UINT& iSerialKey, const UINT& DgnLcomIdx) { m_mPerformDgnLcomIdx.SetAt(iSerialKey, DgnLcomIdx); }
	bool GetPerformDgnLcomIdx(const UINT& iSerialKey, UINT& DgnLcomIdx);

private:
	T_UNIT_INDEX m_CurUnit;

public:
	void SetCurrentUnit();
	T_UNIT_INDEX GetCurrentUnit() const;

private:
	CDgnAnalRsltConverter* m_pAnalRsltConverter;
	CDgnAnalRsltCtrl*      m_pAnalRsltCtrl;

public:
	bool Set_DgnAnalRslt(T_KEY_LIST& aElem);
	void Del_DgnAnalRslt();
	CDgnAnalRsltCtrl* Get_DgnAnalRsltCtrl() const { return m_pAnalRsltCtrl; }

	bool IsIncludeServiceCode();
	bool IsIncludeEnvelopCode();
	bool IsSteelConcurrentForceCode();
};

#include "HeaderPost.h"

#endif // !defined(__DGNFORCECTRL_H__)
