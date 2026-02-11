// DB_ST_DT_MOVE.cpp: 
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DB_ST_DT.h"
#include "DB_ST_DT_MOVE.h"
#include "DBDoc.h"
#include "UnitCtrl.h"

//#include "..\wg_dbLock\LockMgr.h"
//#include "..\wg_base\wg_base_AppBase.h"
//#include "..\wg_base\wg_base_MsgDll.h"

//////////////////////////////////////////////////////
// T_LANEINFO_NEW
void T_LANEINFO_NEW::Initialize()
{
	strLanename = "";
	nLane_Pre =0;
	nLane_Multi = 0;
  bHasMultiPatchLane = FALSE;
	nMovingDir=2;
	dLaneWidth=0.;
	dLineWidth=0.; 

	nInflType=1;
	nLaneType=0;
	bPatchLane = FALSE;
	bTorsionLane=FALSE;
	nTorsionType=0;
	PermitMvldK=0;
	PermitMvhlK=0;
	strSpecialName="";
	SpecialMvldK=0;
	TwoLaneK[0]=TwoLaneK[1]=0;
	nLoadDistribution=0;
	nLoadPositionType=0;
	nNumLoadPoint=0;
	dLoadDistance=0.;
	bWheelSpace=FALSE;
	nNumWheel=0;
	dWheelSpace=0.;
	dLaneWidthA=0.;
	dLaneWidthB=0.;
	bMultiLane = FALSE;
	strVehicleType = "";
	strSelVehicle = "";
// 	nUserLoadType = 0;
// 	nUserSubType = 0;
	nLaneVehicleInfo = 0;
	
	aWheelPosition.RemoveAll();
	aTruckLoadPos.RemoveAll();
	
	aWheelType.RemoveAll();
	PatchMvhlK = 0;
	dPatchW.RemoveAll();
	dPatchL.RemoveAll();
	aPatchWheel.RemoveAll();

	aInflLineLaneInfo.RemoveAll();
	arLaneInfoIlln.RemoveAll();
	arLaneInfoIlid.RemoveAll();

	aInflSurfLaneInfo.RemoveAll();
	arLaneInfoIsln.RemoveAll();
	arLaneInfoIssf.RemoveAll();
	arLaneInfoIsid.RemoveAll();
	arLaneInfoIsel.RemoveAll();

	arLaneWidthBound.RemoveAll();
}

T_LANEINFO_NEW& T_LANEINFO_NEW::operator=(const T_LANEINFO_NEW& src)
{
	strLanename=src.strLanename;
	nLane_Pre =src.nLane_Pre;
	nLane_Multi = src.nLane_Multi;
	bHasMultiPatchLane = src.bHasMultiPatchLane;
	nMovingDir=src.nMovingDir;
	dLaneWidth=src.dLaneWidth;
	dLineWidth=src.dLineWidth;

	nInflType=src.nInflType;
	nLaneType=src.nLaneType;
	bPatchLane=src.bPatchLane;
	bTorsionLane=src.bTorsionLane;
	nTorsionType=src.nTorsionType;
	PermitMvldK=src.PermitMvldK;
	PermitMvhlK=src.PermitMvhlK;
	strSpecialName=src.strSpecialName;
	SpecialMvldK=src.SpecialMvldK;
	TwoLaneK[0]=src.TwoLaneK[0];
	TwoLaneK[1]=src.TwoLaneK[1];
	nLoadDistribution=src.nLoadDistribution;
	nLoadPositionType=src.nLoadPositionType;
	nNumLoadPoint=src.nNumLoadPoint;
	dLoadDistance=src.dLoadDistance;
	bWheelSpace=src.bWheelSpace;
	nNumWheel=src.nNumWheel;
	dWheelSpace=src.dWheelSpace;
	dLaneWidthA=src.dLaneWidthA;
	dLaneWidthB=src.dLaneWidthB;
	bMultiLane=src.bMultiLane;
	strVehicleType=src.strVehicleType;
	strSelVehicle=src.strSelVehicle;
// 	nUserLoadType=src.nUserLoadType;
// 	nUserSubType=src.nUserSubType;
	nLaneVehicleInfo = src.nLaneVehicleInfo;

	aWheelPosition.Copy(src.aWheelPosition);
	aTruckLoadPos.Copy(src.aTruckLoadPos);

	aWheelType.Copy(src.aWheelType);
	PatchMvhlK = src.PatchMvhlK;
	dPatchW.Copy(src.dPatchW);
	dPatchL.Copy(src.dPatchL);
	aPatchWheel.Copy(src.aPatchWheel);

	aInflLineLaneInfo.Copy(src.aInflLineLaneInfo);
	arLaneInfoIlln.Copy(src.arLaneInfoIlln);
	arLaneInfoIlid.Copy(src.arLaneInfoIlid);

	aInflSurfLaneInfo.Copy(src.aInflSurfLaneInfo);
	arLaneInfoIsln.Copy(src.arLaneInfoIsln);
	arLaneInfoIssf.Copy(src.arLaneInfoIssf);
	arLaneInfoIsid.Copy(src.arLaneInfoIsid);
	arLaneInfoIsel.Copy(src.arLaneInfoIsel);

	arLaneWidthBound.Copy(src.arLaneWidthBound);
	return *this;
}

void T_LANEINFO_NEW::Save(CString strParentStructName, CString strValName, std::vector<CString>& aFileContexnts)
{
	SLTEXT_START_SAVE(T_LANEINFO_NEW);	

	SLTEXT_SAVE_BASIC(CString ,strLanename   );
	SLTEXT_SAVE_BASIC(int     ,nLane_Pre     );
	SLTEXT_SAVE_BASIC(int     ,nMovingDir    );
	SLTEXT_SAVE_BASIC(double  ,dLaneWidth    );
	SLTEXT_SAVE_BASIC(double  ,dLineWidth    );
	SLTEXT_SAVE_BASIC(int     ,nInflType     );
	SLTEXT_SAVE_BASIC(int     ,nLaneType     );
	SLTEXT_SAVE_BASIC(BOOL    ,bPatchLane    );
	SLTEXT_SAVE_BASIC(BOOL    ,bTorsionLane  );
	SLTEXT_SAVE_BASIC(int     ,nTorsionType  );
	SLTEXT_SAVE_BASIC(UINT    ,PermitMvldK   );
	SLTEXT_SAVE_BASIC(UINT    ,PermitMvhlK   );
	SLTEXT_SAVE_BASIC(CString	,strSpecialName);
	SLTEXT_SAVE_BASIC(UINT    ,SpecialMvldK  );
	SLTEXT_SAVE_BASIC_FIXAR(UINT, TwoLaneK, 2);
	SLTEXT_SAVE_BASIC(int     ,nLoadDistribution );
	SLTEXT_SAVE_BASIC(int     ,nLoadPositionType );
	SLTEXT_SAVE_BASIC(int     ,nNumLoadPoint     );
	SLTEXT_SAVE_BASIC(double  ,dLoadDistance     );
	SLTEXT_SAVE_BASIC(BOOL    ,bWheelSpace       );
	SLTEXT_SAVE_BASIC(int     ,nNumWheel         );
	SLTEXT_SAVE_BASIC(double  ,dWheelSpace       );
	SLTEXT_SAVE_BASIC(double  ,dLaneWidthA       );
	SLTEXT_SAVE_BASIC(double  ,dLaneWidthB       );
	SLTEXT_SAVE_BASIC_CArray(double, aWheelPosition);
	SLTEXT_SAVE_BASIC_CArray(double, aTruckLoadPos );
	SLTEXT_SAVE_BASIC_CArray(int   , aWheelType);  
	SLTEXT_SAVE_BASIC(UINT         , PatchMvhlK);
	SLTEXT_SAVE_BASIC_CArray(double, dPatchW);
	SLTEXT_SAVE_BASIC_CArray(double, dPatchL);
	SLTEXT_SAVE_ST_CArray(T_LANEINFO_PATCH_WHEEL, aPatchWheel);

	SLTEXT_SAVE_ST_CArray(T_INFLLINE_LANEINFO, aInflLineLaneInfo);
	SLTEXT_SAVE_ST_CArray(T_LANEINFO_ILLN    , arLaneInfoIlln   );
	SLTEXT_SAVE_ST_CArray(T_LANEINFO_ILID    , arLaneInfoIlid   );
	SLTEXT_SAVE_ST_CArray(T_INFLSURF_LANEINFO, aInflSurfLaneInfo);
	SLTEXT_SAVE_ST_CArray(T_LANEINFO_ISLN    , arLaneInfoIsln   );
	SLTEXT_SAVE_ST_CArray(T_LANEINFO_ISSF    , arLaneInfoIssf   );
	SLTEXT_SAVE_ST_CArray(T_LANEINFO_ISID    , arLaneInfoIsid   );
	SLTEXT_SAVE_ST_CArray(T_LANEINFO_ISEL    , arLaneInfoIsel   );

	SLTEXT_END_SAVE();	
}
BOOL T_LANEINFO_NEW::Load(CString strParentStructName, CString strValName, std::vector<CString>& aFileContexnts, int& nToken)
{
	SLTEXT_START_LOAD(T_LANEINFO_NEW);

	SLTEXT_LOAD_BASIC(CString	,strLanename   );
	SLTEXT_LOAD_BASIC(int     ,nLane_Pre     );
	SLTEXT_LOAD_BASIC(int     ,nMovingDir    );
	SLTEXT_LOAD_BASIC(double  ,dLaneWidth    );
	SLTEXT_LOAD_BASIC(double  ,dLineWidth    );
	SLTEXT_LOAD_BASIC(int     ,nInflType     );
	SLTEXT_LOAD_BASIC(int     ,nLaneType     );
	SLTEXT_LOAD_BASIC(BOOL    ,bPatchLane    );
	SLTEXT_LOAD_BASIC(BOOL    ,bTorsionLane  );
	SLTEXT_LOAD_BASIC(int     ,nTorsionType  );
	SLTEXT_LOAD_BASIC(UINT    ,PermitMvldK   );
	SLTEXT_LOAD_BASIC(UINT    ,PermitMvhlK   );
	SLTEXT_LOAD_BASIC(CString	,strSpecialName);
	SLTEXT_LOAD_BASIC(UINT    ,SpecialMvldK  );
	SLTEXT_LOAD_BASIC_FIXAR(UINT, TwoLaneK, 2);
	SLTEXT_LOAD_BASIC(int     ,nLoadDistribution );
	SLTEXT_LOAD_BASIC(int     ,nLoadPositionType );
	SLTEXT_LOAD_BASIC(int     ,nNumLoadPoint     );
	SLTEXT_LOAD_BASIC(double  ,dLoadDistance     );
	SLTEXT_LOAD_BASIC(BOOL    ,bWheelSpace       );
	SLTEXT_LOAD_BASIC(int     ,nNumWheel         );
	SLTEXT_LOAD_BASIC(double  ,dWheelSpace       );
	SLTEXT_LOAD_BASIC(double  ,dLaneWidthA       );
	SLTEXT_LOAD_BASIC(double  ,dLaneWidthB       );
	SLTEXT_LOAD_BASIC_CArray(double, aWheelPosition);
	SLTEXT_LOAD_BASIC_CArray(double, aTruckLoadPos );
	SLTEXT_LOAD_BASIC_CArray(int   , aWheelType);
	SLTEXT_LOAD_BASIC(UINT         , PatchMvhlK);
	SLTEXT_LOAD_BASIC_CArray(double, dPatchW);
	SLTEXT_LOAD_BASIC_CArray(double, dPatchL);
	SLTEXT_LOAD_ST_CArray(T_LANEINFO_PATCH_WHEEL, aPatchWheel);

	SLTEXT_LOAD_ST_CArray(T_INFLLINE_LANEINFO, aInflLineLaneInfo);
	SLTEXT_LOAD_ST_CArray(T_LANEINFO_ILLN    , arLaneInfoIlln   );
	SLTEXT_LOAD_ST_CArray(T_LANEINFO_ILID    , arLaneInfoIlid   );
	SLTEXT_LOAD_ST_CArray(T_INFLSURF_LANEINFO, aInflSurfLaneInfo);
	SLTEXT_LOAD_ST_CArray(T_LANEINFO_ISLN    , arLaneInfoIsln   );
	SLTEXT_LOAD_ST_CArray(T_LANEINFO_ISSF    , arLaneInfoIssf   );
	SLTEXT_LOAD_ST_CArray(T_LANEINFO_ISID    , arLaneInfoIsid   );
	SLTEXT_LOAD_ST_CArray(T_LANEINFO_ISEL    , arLaneInfoIsel   );

	SLTEXT_END_LOAD();

	return TRUE;
}

void T_LANEINFO_NEW::ConvertUnitIn()
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();     ASSERT(pDoc);
	CUnitCtrl* pUnitCtrl = pDoc->m_pUnitCtrl;

	dLaneWidth    *= pUnitCtrl->GetConvertFactorCurrent(D_UNITSYS_BASE_LENGTH);
	dLineWidth    *= pUnitCtrl->GetConvertFactorCurrent(D_UNITSYS_BASE_LENGTH);
	dLoadDistance *= pUnitCtrl->GetConvertFactorCurrent(D_UNITSYS_BASE_LENGTH);
	dWheelSpace   *= pUnitCtrl->GetConvertFactorCurrent(D_UNITSYS_BASE_LENGTH);
	dLaneWidthA   *= pUnitCtrl->GetConvertFactorCurrent(D_UNITSYS_BASE_LENGTH);
	dLaneWidthB   *= pUnitCtrl->GetConvertFactorCurrent(D_UNITSYS_BASE_LENGTH);
	for (int i=0; i<aWheelPosition.GetSize(); ++i) aWheelPosition[i] *= pUnitCtrl->GetConvertFactorCurrent(D_UNITSYS_BASE_LENGTH);
	for (int i=0; i<aTruckLoadPos .GetSize(); ++i) aTruckLoadPos[i]  *= pUnitCtrl->GetConvertFactorCurrent(D_UNITSYS_BASE_LENGTH);
	for (int i = 0; i < dPatchW.GetSize(); ++i) dPatchW[i] *= pUnitCtrl->GetConvertFactorCurrent(D_UNITSYS_BASE_LENGTH);
	for (int i = 0; i < dPatchL.GetSize(); ++i) dPatchL[i] *= pUnitCtrl->GetConvertFactorCurrent(D_UNITSYS_BASE_LENGTH);
	for (int i = 0; i < aPatchWheel.GetSize(); ++i) aPatchWheel[i].ConvertUnitIn();

	for (int i=0; i<aInflLineLaneInfo.GetSize(); ++i) aInflLineLaneInfo[i].ConvertUnitIn();
	for (int i=0; i<arLaneInfoIlln   .GetSize(); ++i) arLaneInfoIlln   [i].ConvertUnitIn();
	for (int i=0; i<arLaneInfoIlid   .GetSize(); ++i) arLaneInfoIlid   [i].ConvertUnitIn();
	for (int i=0; i<aInflSurfLaneInfo.GetSize(); ++i) aInflSurfLaneInfo[i].ConvertUnitIn();
	for (int i=0; i<arLaneInfoIsln   .GetSize(); ++i) arLaneInfoIsln   [i].ConvertUnitIn();
	for (int i=0; i<arLaneInfoIssf   .GetSize(); ++i) arLaneInfoIssf   [i].ConvertUnitIn();
	for (int i=0; i<arLaneInfoIsid   .GetSize(); ++i) arLaneInfoIsid   [i].ConvertUnitIn();
	for (int i=0; i<arLaneInfoIsel   .GetSize(); ++i) arLaneInfoIsel   [i].ConvertUnitIn();

	// Surface & Line Lane 차선폭 Label을 위해 추가, DB에 저장되는 데이터는 아님  
	for (int i=0; i< arLaneWidthBound.GetSize(); ++i)
	{
		arLaneWidthBound[i].x *= pUnitCtrl->GetConvertFactorCurrent(D_UNITSYS_BASE_LENGTH);
		arLaneWidthBound[i].y *= pUnitCtrl->GetConvertFactorCurrent(D_UNITSYS_BASE_LENGTH);
		arLaneWidthBound[i].z *= pUnitCtrl->GetConvertFactorCurrent(D_UNITSYS_BASE_LENGTH);
	}
}

void T_LANEINFO_NEW::ConvertUnitOut()
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();     ASSERT(pDoc);
	CUnitCtrl* pUnitCtrl = pDoc->m_pUnitCtrl;

	dLaneWidth    /= pUnitCtrl->GetConvertFactorCurrent(D_UNITSYS_BASE_LENGTH);
	dLineWidth    /= pUnitCtrl->GetConvertFactorCurrent(D_UNITSYS_BASE_LENGTH);
	dLoadDistance /= pUnitCtrl->GetConvertFactorCurrent(D_UNITSYS_BASE_LENGTH);
	dWheelSpace   /= pUnitCtrl->GetConvertFactorCurrent(D_UNITSYS_BASE_LENGTH);
	dLaneWidthA   /= pUnitCtrl->GetConvertFactorCurrent(D_UNITSYS_BASE_LENGTH);
	dLaneWidthB   /= pUnitCtrl->GetConvertFactorCurrent(D_UNITSYS_BASE_LENGTH);
	for (int i=0; i<aWheelPosition.GetSize(); ++i) aWheelPosition[i] /= pUnitCtrl->GetConvertFactorCurrent(D_UNITSYS_BASE_LENGTH);
	for (int i=0; i<aTruckLoadPos .GetSize(); ++i) aTruckLoadPos[i]  /= pUnitCtrl->GetConvertFactorCurrent(D_UNITSYS_BASE_LENGTH);
	for (int i = 0; i < dPatchW.GetSize(); ++i) dPatchW[i] /= pUnitCtrl->GetConvertFactorCurrent(D_UNITSYS_BASE_LENGTH);
	for (int i = 0; i < dPatchL.GetSize(); ++i) dPatchL[i] /= pUnitCtrl->GetConvertFactorCurrent(D_UNITSYS_BASE_LENGTH);
	for (int i = 0; i < aPatchWheel.GetSize(); ++i) aPatchWheel[i].ConvertUnitOut();

	for (int i=0; i<aInflLineLaneInfo.GetSize(); ++i) aInflLineLaneInfo[i].ConvertUnitOut();
	for (int i=0; i<arLaneInfoIlln   .GetSize(); ++i) arLaneInfoIlln   [i].ConvertUnitOut();
	for (int i=0; i<arLaneInfoIlid   .GetSize(); ++i) arLaneInfoIlid   [i].ConvertUnitOut();
	for (int i=0; i<aInflSurfLaneInfo.GetSize(); ++i) aInflSurfLaneInfo[i].ConvertUnitOut();
	for (int i=0; i<arLaneInfoIsln   .GetSize(); ++i) arLaneInfoIsln   [i].ConvertUnitOut();
	for (int i=0; i<arLaneInfoIssf   .GetSize(); ++i) arLaneInfoIssf   [i].ConvertUnitOut();
	for (int i=0; i<arLaneInfoIsid   .GetSize(); ++i) arLaneInfoIsid   [i].ConvertUnitOut();
	for (int i=0; i<arLaneInfoIsel   .GetSize(); ++i) arLaneInfoIsel   [i].ConvertUnitOut();

	// Surface & Line Lane 차선폭 Label을 위해 추가, DB에 저장되는 데이터는 아님  
	for (int i=0; i< arLaneWidthBound.GetSize(); ++i)
	{
		arLaneWidthBound[i].x /= pUnitCtrl->GetConvertFactorCurrent(D_UNITSYS_BASE_LENGTH);
		arLaneWidthBound[i].y /= pUnitCtrl->GetConvertFactorCurrent(D_UNITSYS_BASE_LENGTH);
		arLaneWidthBound[i].z /= pUnitCtrl->GetConvertFactorCurrent(D_UNITSYS_BASE_LENGTH);
	}
}

//////////////////////////////////////////////////////
// T_INFLLINE_LANEINFO

void T_INFLLINE_LANEINFO::Initialize()
{
	nElem=0;
	dElemCoor=0;
	dLaneCoor=0;
	dImpactFact=0.;
	dImpactFactG=0.;
	dImpactFactF=0.;
	dSpan=0.;
	nSpanId=0;
	dScaleFactor = 1.0;
	dAc = 0.0;
	dImpactFactC = 0.0;
	dPosition[0]=dPosition[1]=dPosition[2]=0.;
	aWheelLoadInfo.RemoveAll();
}

T_INFLLINE_LANEINFO& T_INFLLINE_LANEINFO::operator=(const T_INFLLINE_LANEINFO& src)
{
	nElem=src.nElem;
	dElemCoor=src.dElemCoor;
	dLaneCoor=src.dLaneCoor;
	dImpactFact=src.dImpactFact;
	dImpactFactG=src.dImpactFactG;
	dImpactFactF=src.dImpactFactF;
	dSpan=src.dSpan;
	nSpanId=src.nSpanId;
	dScaleFactor=src.dScaleFactor;
	dAc = src.dAc;
	dImpactFactC = src.dImpactFactC;
	dPosition[0]=src.dPosition[0];
	dPosition[1]=src.dPosition[1];
	dPosition[2]=src.dPosition[2];
	aWheelLoadInfo.Copy(src.aWheelLoadInfo);

	return *this;
}

void T_INFLLINE_LANEINFO::Save(CString strParentStructName, CString strValName, std::vector<CString>& aFileContexnts)
{
	SLTEXT_START_SAVE(T_INFLLINE_LANEINFO);	

	SLTEXT_SAVE_BASIC(int     ,nElem        );
	SLTEXT_SAVE_BASIC(double  ,dElemCoor    );
	SLTEXT_SAVE_BASIC(double  ,dLaneCoor    );
	SLTEXT_SAVE_BASIC(double  ,dImpactFact  );
	SLTEXT_SAVE_BASIC(double, dImpactFactG);
	SLTEXT_SAVE_BASIC(double, dImpactFactF);
	SLTEXT_SAVE_BASIC(double  ,dSpan        );
	SLTEXT_SAVE_BASIC(int     ,nSpanId      );
	SLTEXT_SAVE_BASIC(double  ,dScaleFactor );
	SLTEXT_SAVE_BASIC(double  ,dAc          );
	SLTEXT_SAVE_BASIC(double  ,dImpactFactC);
	SLTEXT_SAVE_BASIC_FIXAR(double, dPosition, 3);
	SLTEXT_SAVE_ST_CArray(T_INFLLINE_LANEINFO_BASE, aWheelLoadInfo);

	SLTEXT_END_SAVE();	
}

BOOL T_INFLLINE_LANEINFO::Load(CString strParentStructName, CString strValName, std::vector<CString>& aFileContexnts, int& nToken)
{
	SLTEXT_START_LOAD(T_INFLLINE_LANEINFO);	

	SLTEXT_LOAD_BASIC(int     ,nElem        );
	SLTEXT_LOAD_BASIC(double  ,dElemCoor    );
	SLTEXT_LOAD_BASIC(double  ,dLaneCoor    );
	SLTEXT_LOAD_BASIC(double  ,dImpactFact  );
	SLTEXT_LOAD_BASIC(double  ,dImpactFactG );
	SLTEXT_LOAD_BASIC(double  ,dImpactFactF );
	SLTEXT_LOAD_BASIC(double  ,dSpan        );
	SLTEXT_LOAD_BASIC(int     ,nSpanId      );
	SLTEXT_LOAD_BASIC(double  ,dScaleFactor );
	SLTEXT_LOAD_BASIC(double  ,dAc          );
	SLTEXT_LOAD_BASIC(double  ,dImpactFactC);
	SLTEXT_LOAD_BASIC_FIXAR(double, dPosition, 3);
	SLTEXT_LOAD_ST_CArray(T_INFLLINE_LANEINFO_BASE, aWheelLoadInfo);

	SLTEXT_END_LOAD();	

	return TRUE;
}

void T_INFLLINE_LANEINFO::ConvertUnitIn()
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();     ASSERT(pDoc);
	CUnitCtrl* pUnitCtrl = pDoc->m_pUnitCtrl;

	dElemCoor *= pUnitCtrl->GetConvertFactorCurrent(D_UNITSYS_BASE_LENGTH);
	dLaneCoor *= pUnitCtrl->GetConvertFactorCurrent(D_UNITSYS_BASE_LENGTH);
	dSpan     *= pUnitCtrl->GetConvertFactorCurrent(D_UNITSYS_BASE_LENGTH);
	for (int i=0; i<3; ++i) dPosition[i] *= pUnitCtrl->GetConvertFactorCurrent(D_UNITSYS_BASE_LENGTH);

	for (int i=0; i<aWheelLoadInfo.GetSize(); ++i) aWheelLoadInfo[i].ConvertUnitIn();
	dImpactFactG *= pUnitCtrl->GetConvertFactorCurrent(D_UNITSYS_BASE_FORCE);
}

void T_INFLLINE_LANEINFO::ConvertUnitOut()
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();     ASSERT(pDoc);
	CUnitCtrl* pUnitCtrl = pDoc->m_pUnitCtrl;

	dElemCoor /= pUnitCtrl->GetConvertFactorCurrent(D_UNITSYS_BASE_LENGTH);
	dLaneCoor /= pUnitCtrl->GetConvertFactorCurrent(D_UNITSYS_BASE_LENGTH);
	dSpan     /= pUnitCtrl->GetConvertFactorCurrent(D_UNITSYS_BASE_LENGTH);
	for (int i=0; i<3; ++i) dPosition[i] /= pUnitCtrl->GetConvertFactorCurrent(D_UNITSYS_BASE_LENGTH);

	for (int i=0; i<aWheelLoadInfo.GetSize(); ++i) aWheelLoadInfo[i].ConvertUnitOut();
	dImpactFactG /= pUnitCtrl->GetConvertFactorCurrent(D_UNITSYS_BASE_FORCE);
}

//////////////////////////////////////////////////////
// T_INFLLINE_LANEINFO_BASE

void T_INFLLINE_LANEINFO_BASE::Initialize()
{
	bWheelLoad=FALSE;
	nLcase1=0;
	nLType1=0;
	dLFact1=0;
	nCrossInfo1=0;
	nLcase2=0;
	nLType2=0;
	dLFact2=0;
	nCrossInfo2=0;
	dEccentricity=0.;
	bOnlyTorsionLane = FALSE;
	dEccenVertLoad=0.0;
	dCentFInf=0.0;
	dEccentricity4CF = 0.0;
	dWheelPosition[0]=dWheelPosition[1]=dWheelPosition[2]=0.;
	dWheelPositionOrg[0]=dWheelPositionOrg[1]=dWheelPositionOrg[2]=0.;

	KeyCentElemkeymap1 = 0;
	KeyCentElemkeymap2 = 0;
	nLcase3 = 0;
	nLType3 = 0;
	dLFact3 = 0;
	nCrossInfo3 = 0;
	nLcase4 = 0;
	nLType4 = 0;
	dLFact4 = 0;
	nCrossInfo4 = 0;

	nLcase5 = 0;
	nLType5 = 0;
	dLFact5 = 0;
	nCrossInfo5 = 0;
	nLcase6 = 0;
	nLType6 = 0;
	dLFact6 = 0;
	nCrossInfo6 = 0;
}

void T_INFLLINE_LANEINFO_BASE::Save(CString strParentStructName, CString strValName, std::vector<CString>& aFileContexnts)
{
	SLTEXT_START_SAVE(T_INFLLINE_LANEINFO_BASE);	

	SLTEXT_SAVE_BASIC(BOOL    ,bWheelLoad       );
	SLTEXT_SAVE_BASIC(int     ,nLcase1          );
	SLTEXT_SAVE_BASIC(int     ,nLType1          );
	SLTEXT_SAVE_BASIC(double  ,dLFact1          );
	SLTEXT_SAVE_BASIC(int     ,nCrossInfo1      );
	SLTEXT_SAVE_BASIC(int     ,nLcase2          );
	SLTEXT_SAVE_BASIC(int     ,nLType2          );
	SLTEXT_SAVE_BASIC(double  ,dLFact2          );
	SLTEXT_SAVE_BASIC(int     ,nCrossInfo2      );
	SLTEXT_SAVE_BASIC(double  ,dEccentricity    );
	SLTEXT_SAVE_BASIC(BOOL    ,bOnlyTorsionLane );
	SLTEXT_SAVE_BASIC(double  ,dEccenVertLoad   );
	SLTEXT_SAVE_BASIC(double  ,dCentFInf   );
	SLTEXT_SAVE_BASIC(double  ,dEccentricity4CF);
	SLTEXT_SAVE_BASIC_FIXAR(double, dWheelPosition, 3);
	SLTEXT_SAVE_BASIC_FIXAR(double, dWheelPositionOrg, 3);
	SLTEXT_SAVE_BASIC(int     ,KeyCentElemkeymap1);
	SLTEXT_SAVE_BASIC(int     ,KeyCentElemkeymap2);
	SLTEXT_SAVE_BASIC(int     ,nLcase3          );
	SLTEXT_SAVE_BASIC(int     ,nLType3          );
	SLTEXT_SAVE_BASIC(double  ,dLFact3          );
	SLTEXT_SAVE_BASIC(int     ,nCrossInfo3      );
	SLTEXT_SAVE_BASIC(int     ,nLcase4          );
	SLTEXT_SAVE_BASIC(int     ,nLType4          );
	SLTEXT_SAVE_BASIC(double  ,dLFact4          );
	SLTEXT_SAVE_BASIC(int     ,nCrossInfo4      );
	SLTEXT_SAVE_BASIC(int     ,nLcase5          );
	SLTEXT_SAVE_BASIC(int     ,nLType5          );
	SLTEXT_SAVE_BASIC(double  ,dLFact5          );
	SLTEXT_SAVE_BASIC(int     ,nCrossInfo5      );
	SLTEXT_SAVE_BASIC(int     ,nLcase6          );
	SLTEXT_SAVE_BASIC(int     ,nLType6          );
	SLTEXT_SAVE_BASIC(double  ,dLFact6          );
	SLTEXT_SAVE_BASIC(int     ,nCrossInfo6      );

	SLTEXT_END_SAVE();	
}

BOOL T_INFLLINE_LANEINFO_BASE::Load(CString strParentStructName, CString strValName, std::vector<CString>& aFileContexnts, int& nToken)
{
	SLTEXT_START_LOAD(T_INFLLINE_LANEINFO_BASE);	

	SLTEXT_LOAD_BASIC(BOOL    ,bWheelLoad       );
	SLTEXT_LOAD_BASIC(int     ,nLcase1          );
	SLTEXT_LOAD_BASIC(int     ,nLType1          );
	SLTEXT_LOAD_BASIC(double  ,dLFact1          );
	SLTEXT_LOAD_BASIC(int     ,nCrossInfo1      );
	SLTEXT_LOAD_BASIC(int     ,nLcase2          );
	SLTEXT_LOAD_BASIC(int     ,nLType2          );
	SLTEXT_LOAD_BASIC(double  ,dLFact2          );
	SLTEXT_LOAD_BASIC(int     ,nCrossInfo2      );
	SLTEXT_LOAD_BASIC(double  ,dEccentricity    );
	SLTEXT_LOAD_BASIC(BOOL    ,bOnlyTorsionLane );
	SLTEXT_LOAD_BASIC(double  ,dEccenVertLoad   );
	SLTEXT_LOAD_BASIC(double  ,dCentFInf   );
	SLTEXT_LOAD_BASIC(double  ,dEccentricity4CF);
	SLTEXT_LOAD_BASIC_FIXAR(double, dWheelPosition, 3);
	SLTEXT_LOAD_BASIC_FIXAR(double, dWheelPositionOrg, 3);
	SLTEXT_LOAD_BASIC(int     ,KeyCentElemkeymap1);
	SLTEXT_LOAD_BASIC(int     ,KeyCentElemkeymap2);
	SLTEXT_LOAD_BASIC(int     ,nLcase3          );
	SLTEXT_LOAD_BASIC(int     ,nLType3          );
	SLTEXT_LOAD_BASIC(double  ,dLFact3          );
	SLTEXT_LOAD_BASIC(int     ,nCrossInfo3      );
	SLTEXT_LOAD_BASIC(int     ,nLcase4          );
	SLTEXT_LOAD_BASIC(int     ,nLType4          );
	SLTEXT_LOAD_BASIC(double  ,dLFact4          );
	SLTEXT_LOAD_BASIC(int     ,nCrossInfo4      );
	SLTEXT_LOAD_BASIC(int     ,nLcase5          );
	SLTEXT_LOAD_BASIC(int     ,nLType5          );
	SLTEXT_LOAD_BASIC(double  ,dLFact5          );
	SLTEXT_LOAD_BASIC(int     ,nCrossInfo5      );
	SLTEXT_LOAD_BASIC(int     ,nLcase6          );
	SLTEXT_LOAD_BASIC(int     ,nLType6          );
	SLTEXT_LOAD_BASIC(double  ,dLFact6          );
	SLTEXT_LOAD_BASIC(int     ,nCrossInfo6      );

	SLTEXT_END_LOAD();	

	return TRUE;
}

void T_INFLLINE_LANEINFO_BASE::ConvertUnitIn()
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();     ASSERT(pDoc);
	CUnitCtrl* pUnitCtrl = pDoc->m_pUnitCtrl;

	dEccentricity     *= pUnitCtrl->GetConvertFactorCurrent(D_UNITSYS_BASE_LENGTH);
	dEccenVertLoad    *= pUnitCtrl->GetConvertFactorCurrent(D_UNITSYS_BASE_LENGTH);
	dEccentricity4CF  *= pUnitCtrl->GetConvertFactorCurrent(D_UNITSYS_BASE_LENGTH);
	for (int i=0; i<3; ++i) dWheelPosition[i] *= pUnitCtrl->GetConvertFactorCurrent(D_UNITSYS_BASE_LENGTH);
	for (int i=0; i<3; ++i) dWheelPositionOrg[i] *= pUnitCtrl->GetConvertFactorCurrent(D_UNITSYS_BASE_LENGTH);
}

void T_INFLLINE_LANEINFO_BASE::ConvertUnitOut()
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();     ASSERT(pDoc);
	CUnitCtrl* pUnitCtrl = pDoc->m_pUnitCtrl;

	dEccentricity     /= pUnitCtrl->GetConvertFactorCurrent(D_UNITSYS_BASE_LENGTH);
	dEccenVertLoad    /= pUnitCtrl->GetConvertFactorCurrent(D_UNITSYS_BASE_LENGTH);
	dEccentricity4CF  /= pUnitCtrl->GetConvertFactorCurrent(D_UNITSYS_BASE_LENGTH);
	for (int i=0; i<3; ++i) dWheelPosition[i] /= pUnitCtrl->GetConvertFactorCurrent(D_UNITSYS_BASE_LENGTH);
	for (int i=0; i<3; ++i) dWheelPositionOrg[i] /= pUnitCtrl->GetConvertFactorCurrent(D_UNITSYS_BASE_LENGTH);
}

/////////////////////////////////////
// T_LANEINFO_ILLN

void T_LANEINFO_ILLN::Initialize()
{
	nElem=0;
	nDivide=0;
	nPosition=0;
	dCoor=0.;
	nLcase1=0;
	nLType1=0;
	dLFact1=0;
	nCrossInfo1=0;
	nLcase2=0;
	nLType2=0;
	dLFact2=0;
	nCrossInfo2=0;
	dEccentricity=0.;
	dEccenPosition[0]=dEccenPosition[1]=dEccenPosition[2]=0.;
	dFact=0.;
	dSpan=0.;
	nSpanId=0;
}

void T_LANEINFO_ILLN::Save(CString strParentStructName, CString strValName, std::vector<CString>& aFileContexnts)
{
	SLTEXT_START_SAVE(T_LANEINFO_ILLN);	

	SLTEXT_SAVE_BASIC(int     ,nElem          );
	SLTEXT_SAVE_BASIC(int     ,nDivide        );
	SLTEXT_SAVE_BASIC(int     ,nPosition      );
	SLTEXT_SAVE_BASIC(double  ,dCoor          );
	SLTEXT_SAVE_BASIC(int     ,nLcase1        );
	SLTEXT_SAVE_BASIC(int     ,nLType1        );
	SLTEXT_SAVE_BASIC(double  ,dLFact1        );
	SLTEXT_SAVE_BASIC(int     ,nCrossInfo1    );
	SLTEXT_SAVE_BASIC(int     ,nLcase2        );
	SLTEXT_SAVE_BASIC(int     ,nLType2        );
	SLTEXT_SAVE_BASIC(double  ,dLFact2        );
	SLTEXT_SAVE_BASIC(int     ,nCrossInfo2    );
	SLTEXT_SAVE_BASIC(double  ,dEccentricity  );
	SLTEXT_SAVE_BASIC_FIXAR(double, dEccenPosition, 3);
	SLTEXT_SAVE_BASIC(double  ,dFact          );
	SLTEXT_SAVE_BASIC(double  ,dSpan          );
	SLTEXT_SAVE_BASIC(int     ,nSpanId        );

	SLTEXT_END_SAVE();	
}

BOOL T_LANEINFO_ILLN::Load(CString strParentStructName, CString strValName, std::vector<CString>& aFileContexnts, int& nToken)
{
	SLTEXT_START_LOAD(T_LANEINFO_ILLN);	

	SLTEXT_LOAD_BASIC(int     ,nElem          );
	SLTEXT_LOAD_BASIC(int     ,nDivide        );
	SLTEXT_LOAD_BASIC(int     ,nPosition      );
	SLTEXT_LOAD_BASIC(double  ,dCoor          );
	SLTEXT_LOAD_BASIC(int     ,nLcase1        );
	SLTEXT_LOAD_BASIC(int     ,nLType1        );
	SLTEXT_LOAD_BASIC(double  ,dLFact1        );
	SLTEXT_LOAD_BASIC(int     ,nCrossInfo1    );
	SLTEXT_LOAD_BASIC(int     ,nLcase2        );
	SLTEXT_LOAD_BASIC(int     ,nLType2        );
	SLTEXT_LOAD_BASIC(double  ,dLFact2        );
	SLTEXT_LOAD_BASIC(int     ,nCrossInfo2    );
	SLTEXT_LOAD_BASIC(double  ,dEccentricity  );
	SLTEXT_LOAD_BASIC_FIXAR(double, dEccenPosition, 3);
	SLTEXT_LOAD_BASIC(double  ,dFact          );
	SLTEXT_LOAD_BASIC(double  ,dSpan          );
	SLTEXT_LOAD_BASIC(int     ,nSpanId        );

	SLTEXT_END_LOAD();	

	return TRUE;
}

void T_LANEINFO_ILLN::ConvertUnitIn()
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();     ASSERT(pDoc);
	CUnitCtrl* pUnitCtrl = pDoc->m_pUnitCtrl;

	dCoor            *= pUnitCtrl->GetConvertFactorCurrent(D_UNITSYS_BASE_LENGTH);
	dEccentricity    *= pUnitCtrl->GetConvertFactorCurrent(D_UNITSYS_BASE_LENGTH);
	for (int i=0; i<3; ++i) dEccenPosition[i] *= pUnitCtrl->GetConvertFactorCurrent(D_UNITSYS_BASE_LENGTH);
	dSpan            *= pUnitCtrl->GetConvertFactorCurrent(D_UNITSYS_BASE_LENGTH);
}

void T_LANEINFO_ILLN::ConvertUnitOut()
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();     ASSERT(pDoc);
	CUnitCtrl* pUnitCtrl = pDoc->m_pUnitCtrl;

	dCoor            /= pUnitCtrl->GetConvertFactorCurrent(D_UNITSYS_BASE_LENGTH);
	dEccentricity    /= pUnitCtrl->GetConvertFactorCurrent(D_UNITSYS_BASE_LENGTH);
	for (int i=0; i<3; ++i) dEccenPosition[i] /= pUnitCtrl->GetConvertFactorCurrent(D_UNITSYS_BASE_LENGTH);
	dSpan            /= pUnitCtrl->GetConvertFactorCurrent(D_UNITSYS_BASE_LENGTH);
}

///////////////////////////////////
// T_LANEINFO_ILID

void T_LANEINFO_ILID::Initialize()
{
	nElem=0;
	dRatio=0.;
}

void T_LANEINFO_ILID::Save(CString strParentStructName, CString strValName, std::vector<CString>& aFileContexnts)
{
	SLTEXT_START_SAVE(T_LANEINFO_ILID);	

	SLTEXT_SAVE_BASIC(int     ,nElem          );
	SLTEXT_SAVE_BASIC(double  ,dRatio         );

	SLTEXT_END_SAVE();	
}

BOOL T_LANEINFO_ILID::Load(CString strParentStructName, CString strValName, std::vector<CString>& aFileContexnts, int& nToken)
{
	SLTEXT_START_LOAD(T_LANEINFO_ILID);	

	SLTEXT_LOAD_BASIC(int     ,nElem          );
	SLTEXT_LOAD_BASIC(double  ,dRatio         );

	SLTEXT_END_LOAD();	

	return TRUE;
}

void T_LANEINFO_ILID::ConvertUnitIn()
{

}

void T_LANEINFO_ILID::ConvertUnitOut()
{

}

///////////////////////////////////////
// T_INFLSURF_LANEINFO

void T_INFLSURF_LANEINFO::Initialize()
{
	nNode=0;
	dLaneCoor=0;
	dImpactFact=0.;
	dImpactFactG = 0.;
	dImpactFactF = 0.;
	dSpan=0.;
	nSpanId=0;
	nCenterNode4SpanId=0;
	dPosition[0]=dPosition[1]=dPosition[2]=0.;
	aWheelLoadInfo.RemoveAll();
}

T_INFLSURF_LANEINFO& T_INFLSURF_LANEINFO::operator=(const T_INFLSURF_LANEINFO& src)
{
	nNode=src.nNode;
	dLaneCoor=src.dLaneCoor;
	dImpactFact=src.dImpactFact;
	dImpactFactG=src.dImpactFactG;
	dImpactFactF=src.dImpactFactF;
	dSpan=src.dSpan;
	nSpanId=src.nSpanId;
	nCenterNode4SpanId=src.nCenterNode4SpanId;
	dPosition[0]=src.dPosition[0];
	dPosition[1]=src.dPosition[1];
	dPosition[2]=src.dPosition[2];
	aWheelLoadInfo.Copy(src.aWheelLoadInfo);

	return *this;
}

void T_INFLSURF_LANEINFO::Save(CString strParentStructName, CString strValName, std::vector<CString>& aFileContexnts)
{
	SLTEXT_START_SAVE(T_INFLSURF_LANEINFO);	

	SLTEXT_SAVE_BASIC(int     ,nNode          );
	SLTEXT_SAVE_BASIC(double  ,dLaneCoor      );
	SLTEXT_SAVE_BASIC(double  ,dImpactFact    );
	SLTEXT_SAVE_BASIC(double  ,dImpactFactG   );
	SLTEXT_SAVE_BASIC(double  ,dImpactFactF   );
	SLTEXT_SAVE_BASIC(double  ,dSpan          );
	SLTEXT_SAVE_BASIC(int     ,nSpanId        );
	SLTEXT_SAVE_BASIC(int     ,nCenterNode4SpanId);
	SLTEXT_SAVE_BASIC_FIXAR(double, dPosition, 3);
	SLTEXT_SAVE_ST_CArray(T_INFLSURF_LANEINFO_BASE, aWheelLoadInfo);

	SLTEXT_END_SAVE();	
}

BOOL T_INFLSURF_LANEINFO::Load(CString strParentStructName, CString strValName, std::vector<CString>& aFileContexnts, int& nToken)
{
	SLTEXT_START_LOAD(T_INFLSURF_LANEINFO);	

	SLTEXT_LOAD_BASIC(int     ,nNode          );
	SLTEXT_LOAD_BASIC(double  ,dLaneCoor      );
	SLTEXT_LOAD_BASIC(double  ,dImpactFact    );
	SLTEXT_LOAD_BASIC(double  ,dImpactFactG   );
	SLTEXT_LOAD_BASIC(double  ,dImpactFactF   );
	SLTEXT_LOAD_BASIC(double  ,dSpan          );
	SLTEXT_LOAD_BASIC(int     ,nSpanId        );
	SLTEXT_LOAD_BASIC(int     ,nCenterNode4SpanId);
	SLTEXT_LOAD_BASIC_FIXAR(double, dPosition, 3);
	SLTEXT_LOAD_ST_CArray(T_INFLSURF_LANEINFO_BASE, aWheelLoadInfo);

	SLTEXT_END_LOAD();	

	return TRUE;
}

void T_INFLSURF_LANEINFO::ConvertUnitIn()
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();     ASSERT(pDoc);
	CUnitCtrl* pUnitCtrl = pDoc->m_pUnitCtrl;

	dLaneCoor    *= pUnitCtrl->GetConvertFactorCurrent(D_UNITSYS_BASE_LENGTH);
	dSpan        *= pUnitCtrl->GetConvertFactorCurrent(D_UNITSYS_BASE_LENGTH);
	dImpactFactG *= pUnitCtrl->GetConvertFactorCurrent(D_UNITSYS_BASE_FORCE);
	for (int i=0; i<3; ++i) dPosition[i] *= pUnitCtrl->GetConvertFactorCurrent(D_UNITSYS_BASE_LENGTH);

	for (int i=0; i<aWheelLoadInfo.GetSize(); ++i) aWheelLoadInfo[i].ConvertUnitIn();
}

void T_INFLSURF_LANEINFO::ConvertUnitOut()
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();     ASSERT(pDoc);
	CUnitCtrl* pUnitCtrl = pDoc->m_pUnitCtrl;

	dLaneCoor    /= pUnitCtrl->GetConvertFactorCurrent(D_UNITSYS_BASE_LENGTH);
	dSpan        /= pUnitCtrl->GetConvertFactorCurrent(D_UNITSYS_BASE_LENGTH);
	dImpactFactG /= pUnitCtrl->GetConvertFactorCurrent(D_UNITSYS_BASE_FORCE);
	for (int i=0; i<3; ++i) dPosition[i] /= pUnitCtrl->GetConvertFactorCurrent(D_UNITSYS_BASE_LENGTH);

	for (int i=0; i<aWheelLoadInfo.GetSize(); ++i) aWheelLoadInfo[i].ConvertUnitOut();
}

/////////////////////////////////////
// T_INFLSURF_LANEINFO_PATCH_BASE

void T_INFLSURF_LANEINFO_PATCH_BASE::Initialize()
{
	nNumNode = 0;
	for (int i = 0; i < 4; i++)
	{
		nNode[i] = 0;
		nLcase[i] = 0;
		ratio[i] = 0.;
	}
	dWheelPositionPatch[0] = dWheelPositionPatch[1] = dWheelPositionPatch[2] = 0.;
}

T_INFLSURF_LANEINFO_PATCH_BASE& T_INFLSURF_LANEINFO_PATCH_BASE::operator=(const T_INFLSURF_LANEINFO_PATCH_BASE& src)
{
	nNumNode = src.nNumNode;

	for (int i = 0; i < 4; i++)
	{
		nNode[i]  = src.nNode[i];
		nLcase[i] = src.nLcase[i];
		ratio[i]  = src.ratio[i];
	}
	for (int i = 0; i < 3; ++i) dWheelPositionPatch[i] = src.dWheelPositionPatch[i];

	return *this;
}

void T_INFLSURF_LANEINFO_PATCH_BASE::Save(CString strParentStructName, CString strValName, std::vector<CString>& aFileContexnts)
{
	SLTEXT_START_SAVE(T_INFLSURF_LANEINFO_PATCH_BASE);

	SLTEXT_SAVE_BASIC(int, nNumNode);
	SLTEXT_SAVE_BASIC_FIXAR(UINT,   nNode,  4);
	SLTEXT_SAVE_BASIC_FIXAR(int ,   nLcase, 4);
	SLTEXT_SAVE_BASIC_FIXAR(double, ratio,  4);
	SLTEXT_SAVE_BASIC_FIXAR(double, dWheelPositionPatch, 3);

	SLTEXT_END_SAVE();
}

BOOL T_INFLSURF_LANEINFO_PATCH_BASE::Load(CString strParentStructName, CString strValName, std::vector<CString>& aFileContexnts, int& nToken)
{
	SLTEXT_START_LOAD(T_INFLSURF_LANEINFO_PATCH_BASE);

	SLTEXT_LOAD_BASIC(int, nNumNode);
	SLTEXT_LOAD_BASIC_FIXAR(UINT,   nNode, 4);
	SLTEXT_LOAD_BASIC_FIXAR(int ,   nLcase,4);
	SLTEXT_LOAD_BASIC_FIXAR(double, ratio, 4);
	SLTEXT_LOAD_BASIC_FIXAR(double, dWheelPositionPatch, 3);

	SLTEXT_END_LOAD();

	return TRUE;
}

void T_INFLSURF_LANEINFO_PATCH_BASE::ConvertUnitIn()
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();     ASSERT(pDoc);
	CUnitCtrl* pUnitCtrl = pDoc->m_pUnitCtrl;

	for (int i = 0; i < 3; ++i) dWheelPositionPatch[i] *= pUnitCtrl->GetConvertFactorCurrent(D_UNITSYS_BASE_LENGTH);
}

void T_INFLSURF_LANEINFO_PATCH_BASE::ConvertUnitOut()
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();     ASSERT(pDoc);
	CUnitCtrl* pUnitCtrl = pDoc->m_pUnitCtrl;

	for (int i = 0; i < 3; ++i) dWheelPositionPatch[i] /= pUnitCtrl->GetConvertFactorCurrent(D_UNITSYS_BASE_LENGTH);
}


/////////////////////////////////////
// T_INFLSURF_LANEINFO_PATCH_WHEEL_BASE

void T_INFLSURF_LANEINFO_PATCH_WHEEL_BASE::Initialize()
{
	nPatchType = 0;
	for (int i = 0; i < 3; ++i) dWheelPositionPatch[i] = 0.0;
	aNodePatchR.RemoveAll();
	aLcasePatchR.RemoveAll();
	aRatioPatchR.RemoveAll();
}

T_INFLSURF_LANEINFO_PATCH_WHEEL_BASE& T_INFLSURF_LANEINFO_PATCH_WHEEL_BASE::operator=(const T_INFLSURF_LANEINFO_PATCH_WHEEL_BASE& src)
{
	nPatchType = src.nPatchType;
	for (int i = 0; i < 3; ++i) dWheelPositionPatch[i] = src.dWheelPositionPatch[i];
	aNodePatchR.Copy(src.aNodePatchR);
	aLcasePatchR.Copy(src.aLcasePatchR);
	aRatioPatchR.Copy(src.aRatioPatchR);

	return *this;
}

void T_INFLSURF_LANEINFO_PATCH_WHEEL_BASE::Save(CString strParentStructName, CString strValName, std::vector<CString>& aFileContexnts)
{
	SLTEXT_START_SAVE(T_INFLSURF_LANEINFO_PATCH_WHEEL_BASE);

	SLTEXT_SAVE_BASIC(int, nPatchType);
	SLTEXT_SAVE_BASIC_FIXAR(double   , dWheelPositionPatch, 3);
	SLTEXT_SAVE_BASIC_CArray(T_NODE_K, aNodePatchR);
	SLTEXT_SAVE_BASIC_CArray(int     , aLcasePatchR);
	SLTEXT_SAVE_BASIC_CArray(double  , aRatioPatchR);

	SLTEXT_END_SAVE();
}

BOOL T_INFLSURF_LANEINFO_PATCH_WHEEL_BASE::Load(CString strParentStructName, CString strValName, std::vector<CString>& aFileContexnts, int& nToken)
{
	SLTEXT_START_LOAD(T_INFLSURF_LANEINFO_PATCH_WHEEL_BASE);

	SLTEXT_LOAD_BASIC(int, nPatchType);
	SLTEXT_LOAD_BASIC_FIXAR(double   , dWheelPositionPatch, 3);
	SLTEXT_LOAD_BASIC_CArray(T_NODE_K, aNodePatchR);
	SLTEXT_LOAD_BASIC_CArray(int     , aLcasePatchR);
	SLTEXT_LOAD_BASIC_CArray(double  , aRatioPatchR);

	SLTEXT_END_LOAD();

	return TRUE;
}

void T_INFLSURF_LANEINFO_PATCH_WHEEL_BASE::ConvertUnitIn()
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();     ASSERT(pDoc);
	CUnitCtrl* pUnitCtrl = pDoc->m_pUnitCtrl;

	for (int i = 0; i < 3; ++i) dWheelPositionPatch[i] *= pUnitCtrl->GetConvertFactorCurrent(D_UNITSYS_BASE_LENGTH);
}

void T_INFLSURF_LANEINFO_PATCH_WHEEL_BASE::ConvertUnitOut()
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();     ASSERT(pDoc);
	CUnitCtrl* pUnitCtrl = pDoc->m_pUnitCtrl;

	for (int i = 0; i < 3; ++i) dWheelPositionPatch[i] /= pUnitCtrl->GetConvertFactorCurrent(D_UNITSYS_BASE_LENGTH);
}

/////////////////////////////////////
// T_INFLSURF_LANEINFO_BASE

void T_INFLSURF_LANEINFO_BASE::Initialize()
{
	bWheelLoad=FALSE;
	nLcase=0;
	dEccentricity=0.;
	dEccenVertLoad=0.0;
	dCentFInf=0.0;
	dEccentricity4CF = 0.0;
	dWheelPosition[0]=dWheelPosition[1]=dWheelPosition[2]=0.;
	Vector[0] = Vector[1] = Vector[2] = 0.;
	
	aPatchWheel.RemoveAll();
}

T_INFLSURF_LANEINFO_BASE& T_INFLSURF_LANEINFO_BASE::operator=(const T_INFLSURF_LANEINFO_BASE& src)
{
	bWheelLoad = src.bWheelLoad;
	nLcase = src.nLcase;
	dEccentricity  = src.dEccentricity;
	dEccenVertLoad = src.dEccenVertLoad;
	dCentFInf      = src.dCentFInf;
	dEccentricity4CF = src.dEccentricity4CF;
	for (int i = 0; i < 3; ++i) dWheelPosition[i] = src.dWheelPosition[i];
	for (int i = 0; i < 3; ++i) Vector[i] = src.Vector[i];

	aPatchWheel.Copy(src.aPatchWheel);
	return *this;
}

void T_INFLSURF_LANEINFO_BASE::Save(CString strParentStructName, CString strValName, std::vector<CString>& aFileContexnts)
{
	SLTEXT_START_SAVE(T_INFLSURF_LANEINFO_BASE);	

	SLTEXT_SAVE_BASIC(BOOL    ,bWheelLoad      );
	SLTEXT_SAVE_BASIC(int     ,nLcase          );
	SLTEXT_SAVE_BASIC(double  ,dEccentricity   );
	SLTEXT_SAVE_BASIC(double  ,dEccenVertLoad  );
	SLTEXT_SAVE_BASIC(double  ,dCentFInf  );
	SLTEXT_SAVE_BASIC(double  ,dEccentricity4CF);
	SLTEXT_SAVE_BASIC_FIXAR(double, dWheelPosition, 3);
	SLTEXT_SAVE_BASIC_FIXAR(double, Vector        , 3);
	SLTEXT_SAVE_ST_CArray(T_INFLSURF_LANEINFO_PATCH_WHEEL_BASE, aPatchWheel);
	SLTEXT_END_SAVE();	
}

BOOL T_INFLSURF_LANEINFO_BASE::Load(CString strParentStructName, CString strValName, std::vector<CString>& aFileContexnts, int& nToken)
{
	SLTEXT_START_LOAD(T_INFLSURF_LANEINFO_BASE);	

	SLTEXT_LOAD_BASIC(BOOL    ,bWheelLoad      );
	SLTEXT_LOAD_BASIC(int     ,nLcase          );
	SLTEXT_LOAD_BASIC(double  ,dEccentricity   );
	SLTEXT_LOAD_BASIC(double  ,dEccenVertLoad  );
	SLTEXT_LOAD_BASIC(double  ,dCentFInf  );
	SLTEXT_LOAD_BASIC(double  ,dEccentricity4CF);
	SLTEXT_LOAD_BASIC_FIXAR(double, dWheelPosition, 3);
	SLTEXT_LOAD_BASIC_FIXAR(double, Vector        , 3);
	SLTEXT_LOAD_ST_CArray(T_INFLSURF_LANEINFO_PATCH_WHEEL_BASE, aPatchWheel);

	SLTEXT_END_LOAD();	

	return TRUE;
}

void T_INFLSURF_LANEINFO_BASE::ConvertUnitIn()
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();     ASSERT(pDoc);
	CUnitCtrl* pUnitCtrl = pDoc->m_pUnitCtrl;

	dEccentricity     *= pUnitCtrl->GetConvertFactorCurrent(D_UNITSYS_BASE_LENGTH);
	dEccenVertLoad    *= pUnitCtrl->GetConvertFactorCurrent(D_UNITSYS_BASE_LENGTH);
	dEccentricity4CF  *= pUnitCtrl->GetConvertFactorCurrent(D_UNITSYS_BASE_LENGTH);
	for (int i=0; i<3; ++i) dWheelPosition[i] *= pUnitCtrl->GetConvertFactorCurrent(D_UNITSYS_BASE_LENGTH);

	for (int i = 0; i < aPatchWheel.GetSize(); ++i) aPatchWheel[i].ConvertUnitIn();
}

void T_INFLSURF_LANEINFO_BASE::ConvertUnitOut()
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();     ASSERT(pDoc);
	CUnitCtrl* pUnitCtrl = pDoc->m_pUnitCtrl;

	dEccentricity     /= pUnitCtrl->GetConvertFactorCurrent(D_UNITSYS_BASE_LENGTH);
	dEccenVertLoad    /= pUnitCtrl->GetConvertFactorCurrent(D_UNITSYS_BASE_LENGTH);
	dEccentricity4CF  /= pUnitCtrl->GetConvertFactorCurrent(D_UNITSYS_BASE_LENGTH);
	for (int i=0; i<3; ++i) dWheelPosition[i] /= pUnitCtrl->GetConvertFactorCurrent(D_UNITSYS_BASE_LENGTH);

	for (int i = 0; i < aPatchWheel.GetSize(); ++i) aPatchWheel[i].ConvertUnitOut();
}

//////////////////////////////////
// T_LANEINFO_ISLN

void T_LANEINFO_ISLN::Initialize()
{
	nNode=0;
	dCoor=0.;
	// V710 에서 사용안함, 삭제 예정
	nLcase=0;
	dFact=0.;
	dSpan=0.;
	nSpanId=0;
	//
}

void T_LANEINFO_ISLN::Save(CString strParentStructName, CString strValName, std::vector<CString>& aFileContexnts)
{
	SLTEXT_START_SAVE(T_LANEINFO_ISLN);	

	SLTEXT_SAVE_BASIC(int     ,nNode    );
	SLTEXT_SAVE_BASIC(double  ,dCoor    );
	SLTEXT_SAVE_BASIC(int     ,nLcase   );
	SLTEXT_SAVE_BASIC(double  ,dFact    );
	SLTEXT_SAVE_BASIC(double  ,dSpan    );
	SLTEXT_SAVE_BASIC(int     ,nSpanId  );

	SLTEXT_END_SAVE();	
}

BOOL T_LANEINFO_ISLN::Load(CString strParentStructName, CString strValName, std::vector<CString>& aFileContexnts, int& nToken)
{
	SLTEXT_START_LOAD(T_LANEINFO_ISLN);	

	SLTEXT_LOAD_BASIC(int     ,nNode    );
	SLTEXT_LOAD_BASIC(double  ,dCoor    );
	SLTEXT_LOAD_BASIC(int     ,nLcase   );
	SLTEXT_LOAD_BASIC(double  ,dFact    );
	SLTEXT_LOAD_BASIC(double  ,dSpan    );
	SLTEXT_LOAD_BASIC(int     ,nSpanId  );

	SLTEXT_END_LOAD();	

	return TRUE;
}

void T_LANEINFO_ISLN::ConvertUnitIn()
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();     ASSERT(pDoc);
	CUnitCtrl* pUnitCtrl = pDoc->m_pUnitCtrl;

	dCoor    *= pUnitCtrl->GetConvertFactorCurrent(D_UNITSYS_BASE_LENGTH);
	dSpan    *= pUnitCtrl->GetConvertFactorCurrent(D_UNITSYS_BASE_LENGTH);
}

void T_LANEINFO_ISLN::ConvertUnitOut()
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();     ASSERT(pDoc);
	CUnitCtrl* pUnitCtrl = pDoc->m_pUnitCtrl;

	dCoor    /= pUnitCtrl->GetConvertFactorCurrent(D_UNITSYS_BASE_LENGTH);
	dSpan    /= pUnitCtrl->GetConvertFactorCurrent(D_UNITSYS_BASE_LENGTH);
}

/////////////////////////////////////////
// T_LANEINFO_ISSF

void T_LANEINFO_ISSF::Initialize()
{
	nNode=0;
	nLcase=0;
	dArea=0.;
	dSpan=0.;
	nSpanId=0;
	dFactor=0.;
	dFactorG=0.;
}

void T_LANEINFO_ISSF::Save(CString strParentStructName, CString strValName, std::vector<CString>& aFileContexnts)
{
	SLTEXT_START_SAVE(T_LANEINFO_ISSF);	

	SLTEXT_SAVE_BASIC(int     ,nNode    );
	SLTEXT_SAVE_BASIC(int     ,nLcase   );
	SLTEXT_SAVE_BASIC(double  ,dArea    );
	SLTEXT_SAVE_BASIC(double  ,dSpan    );
	SLTEXT_SAVE_BASIC(int     ,nSpanId  );
	SLTEXT_SAVE_BASIC(double  ,dFactor  );
	SLTEXT_SAVE_BASIC(double  ,dFactorG );

	SLTEXT_END_SAVE();	
}

BOOL T_LANEINFO_ISSF::Load(CString strParentStructName, CString strValName, std::vector<CString>& aFileContexnts, int& nToken)
{
	SLTEXT_START_LOAD(T_LANEINFO_ISSF);	

	SLTEXT_LOAD_BASIC(int     ,nNode    );
	SLTEXT_LOAD_BASIC(int     ,nLcase   );
	SLTEXT_LOAD_BASIC(double  ,dArea    );
	SLTEXT_LOAD_BASIC(double  ,dSpan    );
	SLTEXT_LOAD_BASIC(int     ,nSpanId  );
	SLTEXT_LOAD_BASIC(double  ,dFactor  );
	SLTEXT_LOAD_BASIC(double  ,dFactorG );

	SLTEXT_END_LOAD();	

	return TRUE;
}

void T_LANEINFO_ISSF::ConvertUnitIn()
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();     ASSERT(pDoc);
	CUnitCtrl* pUnitCtrl = pDoc->m_pUnitCtrl;

	dArea    *= pUnitCtrl->GetConvertFactorCurrent(D_UNITSYS_BASE_AREA  );
	dSpan    *= pUnitCtrl->GetConvertFactorCurrent(D_UNITSYS_BASE_LENGTH);
	dFactorG *= pUnitCtrl->GetConvertFactorCurrent(D_UNITSYS_BASE_FORCE);
}

void T_LANEINFO_ISSF::ConvertUnitOut()
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();     ASSERT(pDoc);
	CUnitCtrl* pUnitCtrl = pDoc->m_pUnitCtrl;

	dArea    /= pUnitCtrl->GetConvertFactorCurrent(D_UNITSYS_BASE_AREA  );
	dSpan    /= pUnitCtrl->GetConvertFactorCurrent(D_UNITSYS_BASE_LENGTH);
	dFactorG /= pUnitCtrl->GetConvertFactorCurrent(D_UNITSYS_BASE_FORCE);
}

/////////////////////////////////////
// T_LANEINFO_ISID

void T_LANEINFO_ISID::Initialize()
{
	nType=0;
	nElem=0;
	nNumNode=0;
	for(int i=0; i<4; i++)
	{
		nNode[i]=0;
		nLcase[i]=0;
		ratio[i]=0.;
	}
	px=0., py=0., pz=0.;
	nNode1=nNode2=0;
	nLcase1=nLcase2=0;
	dRatio=0.;          // 삭제 대상..
	nCenterNode4SpanId=0;
}

void T_LANEINFO_ISID::Save(CString strParentStructName, CString strValName, std::vector<CString>& aFileContexnts)
{
	SLTEXT_START_SAVE(T_LANEINFO_ISID);	

	SLTEXT_SAVE_BASIC(int   ,nType     );
	SLTEXT_SAVE_BASIC(UINT  ,nElem     );
	SLTEXT_SAVE_BASIC(int   ,nNumNode  );
	SLTEXT_SAVE_BASIC_FIXAR(UINT  , nNode , 4);
	SLTEXT_SAVE_BASIC_FIXAR(int   , nLcase, 4);
	SLTEXT_SAVE_BASIC_FIXAR(double, ratio , 4);
	SLTEXT_SAVE_BASIC(double  ,px       );
	SLTEXT_SAVE_BASIC(double  ,py       );
	SLTEXT_SAVE_BASIC(double  ,pz       );
	SLTEXT_SAVE_BASIC(UINT    ,nNode1   );
	SLTEXT_SAVE_BASIC(UINT    ,nNode2   );
	SLTEXT_SAVE_BASIC(int     ,nLcase1  );
	SLTEXT_SAVE_BASIC(int     ,nLcase2  );
	SLTEXT_SAVE_BASIC(double  ,dRatio   );
	SLTEXT_SAVE_BASIC(int     ,nCenterNode4SpanId);

	SLTEXT_END_SAVE();	
}

BOOL T_LANEINFO_ISID::Load(CString strParentStructName, CString strValName, std::vector<CString>& aFileContexnts, int& nToken)
{
	SLTEXT_START_LOAD(T_LANEINFO_ISID);	

	SLTEXT_LOAD_BASIC(int   ,nType     );
	SLTEXT_LOAD_BASIC(UINT  ,nElem     );
	SLTEXT_LOAD_BASIC(int   ,nNumNode  );
	SLTEXT_LOAD_BASIC_FIXAR(UINT  , nNode , 4);
	SLTEXT_LOAD_BASIC_FIXAR(int   , nLcase, 4);
	SLTEXT_LOAD_BASIC_FIXAR(double, ratio , 4);
	SLTEXT_LOAD_BASIC(double  ,px       );
	SLTEXT_LOAD_BASIC(double  ,py       );
	SLTEXT_LOAD_BASIC(double  ,pz       );
	SLTEXT_LOAD_BASIC(UINT    ,nNode1   );
	SLTEXT_LOAD_BASIC(UINT    ,nNode2   );
	SLTEXT_LOAD_BASIC(int     ,nLcase1  );
	SLTEXT_LOAD_BASIC(int     ,nLcase2  );
	SLTEXT_LOAD_BASIC(double  ,dRatio   );
	SLTEXT_LOAD_BASIC(int     ,nCenterNode4SpanId);

	SLTEXT_END_LOAD();	

	return TRUE;
}

void T_LANEINFO_ISID::ConvertUnitIn()
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();     ASSERT(pDoc);
	CUnitCtrl* pUnitCtrl = pDoc->m_pUnitCtrl;

	px *= pUnitCtrl->GetConvertFactorCurrent(D_UNITSYS_BASE_LENGTH);
	py *= pUnitCtrl->GetConvertFactorCurrent(D_UNITSYS_BASE_LENGTH);
	pz *= pUnitCtrl->GetConvertFactorCurrent(D_UNITSYS_BASE_LENGTH);
}

void T_LANEINFO_ISID::ConvertUnitOut()
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();     ASSERT(pDoc);
	CUnitCtrl* pUnitCtrl = pDoc->m_pUnitCtrl;

	px /= pUnitCtrl->GetConvertFactorCurrent(D_UNITSYS_BASE_LENGTH);
	py /= pUnitCtrl->GetConvertFactorCurrent(D_UNITSYS_BASE_LENGTH);
	pz /= pUnitCtrl->GetConvertFactorCurrent(D_UNITSYS_BASE_LENGTH);
}

//////////////////////////////////////////
// T_LANEINFO_ISEL

void T_LANEINFO_ISEL::Initialize()
{
	nElem=0;
	bCut=FALSE;
	aNode.RemoveAll();
	nIsid1=0;
	nIsid2=0;
}

T_LANEINFO_ISEL& T_LANEINFO_ISEL::operator=(const T_LANEINFO_ISEL& src)
{
	nElem=src.nElem;
	bCut=src.bCut;
	aNode.Copy(src.aNode);
	nIsid1=src.nIsid1;
	nIsid2=src.nIsid2;
	return *this;
}

void T_LANEINFO_ISEL::Save(CString strParentStructName, CString strValName, std::vector<CString>& aFileContexnts)
{
	SLTEXT_START_SAVE(T_LANEINFO_ISEL);	

	SLTEXT_SAVE_BASIC(int   ,nElem  );
	SLTEXT_SAVE_BASIC(BOOL	,bCut   );
	SLTEXT_SAVE_BASIC_CArray(int, aNode);
	SLTEXT_SAVE_BASIC(int   ,nIsid1  );
	SLTEXT_SAVE_BASIC(int   ,nIsid2  );

	SLTEXT_END_SAVE();	
}

BOOL T_LANEINFO_ISEL::Load(CString strParentStructName, CString strValName, std::vector<CString>& aFileContexnts, int& nToken)
{
	SLTEXT_START_LOAD(T_LANEINFO_ISEL);	

	SLTEXT_LOAD_BASIC(int   ,nElem  );
	SLTEXT_LOAD_BASIC(BOOL	,bCut   );
	SLTEXT_LOAD_BASIC_CArray(int, aNode);
	SLTEXT_LOAD_BASIC(int   ,nIsid1  );
	SLTEXT_LOAD_BASIC(int   ,nIsid2  );

	SLTEXT_END_LOAD();	

	return TRUE;
}

void T_LANEINFO_ISEL::ConvertUnitIn()
{

}

void T_LANEINFO_ISEL::ConvertUnitOut()
{

}

//////////////////////////////////////////
// T_LANEINFO_PATCH_WHEEL

void T_LANEINFO_PATCH_WHEEL::Initialize()
{
	aPatchWheelSpacing.RemoveAll();
}

T_LANEINFO_PATCH_WHEEL& T_LANEINFO_PATCH_WHEEL::operator=(const T_LANEINFO_PATCH_WHEEL& src)
{
	aPatchWheelSpacing.Copy(src.aPatchWheelSpacing);
	return *this;
}

void T_LANEINFO_PATCH_WHEEL::Save(CString strParentStructName, CString strValName, std::vector<CString>& aFileContexnts)
{
	SLTEXT_START_SAVE(T_LANEINFO_PATCH_WHEEL);

	SLTEXT_SAVE_BASIC_CArray(double, aPatchWheelSpacing);

	SLTEXT_END_SAVE();
}

BOOL T_LANEINFO_PATCH_WHEEL::Load(CString strParentStructName, CString strValName, std::vector<CString>& aFileContexnts, int& nToken)
{
	SLTEXT_START_LOAD(T_LANEINFO_PATCH_WHEEL);

	SLTEXT_LOAD_BASIC_CArray(double, aPatchWheelSpacing);

	SLTEXT_END_LOAD();

	return TRUE;
}

void T_LANEINFO_PATCH_WHEEL::ConvertUnitIn()
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();     ASSERT(pDoc);
	CUnitCtrl* pUnitCtrl = pDoc->m_pUnitCtrl;

	for (int i = 0; i < aPatchWheelSpacing.GetSize(); ++i)
	{
		aPatchWheelSpacing[i] *= pUnitCtrl->GetConvertFactorCurrent(D_UNITSYS_BASE_LENGTH);
	}
}

void T_LANEINFO_PATCH_WHEEL::ConvertUnitOut()
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();     ASSERT(pDoc);
	CUnitCtrl* pUnitCtrl = pDoc->m_pUnitCtrl;

	for (int i = 0; i < aPatchWheelSpacing.GetSize(); ++i)
	{
		aPatchWheelSpacing[i] /= pUnitCtrl->GetConvertFactorCurrent(D_UNITSYS_BASE_LENGTH);
	}
}
