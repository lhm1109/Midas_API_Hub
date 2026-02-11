// ReportAutoRegeneration.cpp : implementation file
//

#include "stdafx.h"
#include "AutoReport_Gen_CH_CD.h"
#include "ReportCommonFunc.h"
#include "TreeReportCtrl.h"
#include "..\wg_xl\ReportTableProcess.h"
#include "..\wg_db\PostCtrl.h"
#include "..\wg_db\FileCtrl.h"
#include "..\wg_gps\GPSAMRUtil.h"
#include "LoadWordLib.h"
#include "..\wg_gr\GRAMRUDFData.h"
#include "..\wg_db\StagCtrl.h"
#include "..\wg_gr\GRenderView.h"
#include "..\wg_base\wg_baseAll.h"
#include "..\wg_base\wg_baseAllTemp.h"
#include "..\wg_gps\wg_gpsAll.h"
#include "..\wg_db\UnitCtrl.h"
#include "..\wg_db\DBLib.h"
#include "..\wg_db\StoryDisplDrift.h"
#include "..\wg_db\StoryData.h"
#include "..\wg_db\SelfWgt.h"
#include "..\wg_db\StoryLoad.h"
#include "..\wg_db\LateralLoad.h"
#include "AutoReport_Gen_Mgr.h"
#include "..\wg_main\wg_mainRes2.h"
#include "..\wg_gps\GpsbarBmpMaker\GpsbarInitData.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
Report_Para_CH* AutoReport_Gen_CD::Para() const
{
	return AutoReport_Gen_Mgr::Inst()->Para();
}
CString AutoReport_Gen_CD::MakePicture_1_1()
{
	CString strName = _T("3D Model");
	SetPreOrPostMode(FALSE);
	DrawNodeAll(FALSE);
	SetViewPoint(0);
	MakeImage_Current(strName, FALSE);
	return strName;
}
void AutoReport_Gen_CD::GetStoryDrift(CString aVal[])
{
	CStoryDisplDrift StDrft;
	T_LCOM_D  LcomData;
	int nStep = 0;
	T_DRIFT_METHOD DriftMethod;
	DriftMethod.Initialize();
	DriftMethod.bMethod[0] = TRUE;
	DriftMethod.bMethod[1] = TRUE;
	DriftMethod.bSameSelNode[0] = TRUE;
	DriftMethod.bSameSelNode[1] = TRUE;
	DriftMethod.bSameAverage[0] = TRUE;
	DriftMethod.bSameAverage[1] = TRUE;   
	//
	Report_Key_LIST lstCurLoads;
	Report_Key_LIST lstComb;
	Para()->CaseKeysOnly(lstCurLoads);
	Para()->LoadCombKeys(D_LCOMTYPE_GENERAL, lstComb);
	lstCurLoads.Append(lstComb);
	CArray<T_STOR_DFT, T_STOR_DFT&> aDFT;
	double dMaxVal[] = {0,0,0,0};
	BOOL bSet = FALSE;
	for (int i=0; i<lstCurLoads.GetCount(); i++)
	{        
		Report_Key LoadK = lstCurLoads[i];
		if(!MakeLoadComb(LoadK, LcomData))
			continue;
		aDFT.RemoveAll();
		if(!StDrft.GetDFT(LcomData, nStep, DriftMethod, aDFT))
			continue;
		for (int k=0; k<aDFT.GetCount(); k++)
		{
			const T_STOR_DFT &drift = aDFT[k];
			if(bSet == FALSE)
			{
				bSet = TRUE;
				dMaxVal[0] = drift.Avg[0].dRatio;
				dMaxVal[1] = drift.Avg[1].dRatio;
				dMaxVal[2] = drift.Max[0].dRatio;
				dMaxVal[3] = drift.Max[1].dRatio;
			}
			else
			{
				dMaxVal[0] = CompVal(dMaxVal[0], drift.Avg[0].dRatio);
				dMaxVal[1] = CompVal(dMaxVal[1], drift.Avg[1].dRatio);
				dMaxVal[2] = CompVal(dMaxVal[2], drift.Max[0].dRatio);
				dMaxVal[3] = CompVal(dMaxVal[3], drift.Max[1].dRatio);
			}
		}
	}
	for (int i=0; i<4; i++)
	{
		if(!bSet)
			aVal[i] = _T("-");
		else
		{
			if(fabs(dMaxVal[i]) > 1e-9)
				aVal[i].Format(_T("1/%.0f"), 1.0/dMaxVal[i]);
			else
				aVal[i] = _T("0.00");
		}
	}
}
void AutoReport_Gen_CD::GetCodeInfo(CString &strCodeMatl, CString &strCodeWind, CString &strCodeQuake)
{
	CMap< CString, LPCTSTR, int, int> mapExistMatl;
	CMap< CString, LPCTSTR, int, int> mapExistWind;
	CMap< CString, LPCTSTR, int, int> mapExistQuake;
	T_KEY_LIST lstKeys;
	m_pDoc->m_pAttrCtrl->GetMatlKeyListUsedInElem(lstKeys);
	T_MATD_D matlD;
	for (int i=0; i<lstKeys.GetCount(); i++)
	{
		if(!m_pDoc->m_pAttrCtrl->GetMatlDesign(lstKeys[i], matlD))
			continue;
		if(matlD.Data1.CodeName.GetLength() >0 && matlD.Data1.CodeName != _T("None") && mapExistMatl.PLookup(matlD.Data1.CodeName) == nullptr)
		{
			strCodeMatl += matlD.Data1.CodeName + _T(", ");
			mapExistMatl.SetAt(matlD.Data1.CodeName, 1);
		}
		if(matlD.Data2.CodeName.GetLength() >0 && matlD.Data2.CodeName != _T("None") && mapExistMatl.PLookup(matlD.Data2.CodeName) == nullptr)
		{
			strCodeMatl += matlD.Data2.CodeName + _T(", ");
			mapExistMatl.SetAt(matlD.Data2.CodeName, 1);
		}
	}
	strCodeMatl.TrimRight(',');

	int nWindCode = WIND_CODE_NUM;
	POSITION pos = m_pDoc->m_pAttrCtrl->GetStartWind();
	T_WIND_D windD;
	T_WIND_K windK;
	while (pos)
	{
		m_pDoc->m_pAttrCtrl->GetNextWind(pos, windK, windD);
		if(windD.nCodeType >= nWindCode)
		{
			ASSERT(FALSE);
			continue;
		}
		CString strCode = CDBLib::m_aDescWLCodeName[windD.nCodeType -1];
		if(mapExistWind.PLookup(strCode) == nullptr)
		{
			strCodeWind += strCode + _T(", ");
			mapExistWind.SetAt(strCode, 1);
		}
	}
	strCodeWind.TrimRight(',');
	//
	pos = m_pDoc->m_pAttrCtrl->GetStartSpfc();
	T_SPFC_D spfcD;
	T_SPFC_K spfcK;
	while (pos)
	{
		m_pDoc->m_pAttrCtrl->GetNextSpfc(pos, spfcK, spfcD);
		CString strCode = spfcD.SpecFuncName;
		if(mapExistWind.PLookup(strCode) == nullptr)
		{
			strCodeQuake += strCode + _T(", ");
			mapExistQuake.SetAt(strCode, 1);
		}
	}
	strCodeQuake.TrimRight(',');
}
void AutoReport_Gen_CD::ShowLoad(const Report_Key &CaseK, BOOL bShow)
{
	GDisplaySetting dspOpt;
	ExportDisplaySetting::GET(&dspOpt);
	dspOpt.m_Load.bCnld    = bShow;
	dspOpt.m_Load.bBmld    = bShow;
	dspOpt.m_Load.bPres    = bShow;
	dspOpt.m_Load.bFbld    = bShow;
	dspOpt.m_Load.nOption  = 1;
	dspOpt.m_Load.bTxtAll  = bShow;
	dspOpt.m_Load.nLoadCaseType = CaseK.m_nType;
	dspOpt.m_Load.nLoadCase     =  CaseK.m_nKey;
	ExportDisplaySetting::SET(&dspOpt);
}
void AutoReport_Gen_CD::MakeTBLRecord_4_2(int nDir, T_TABLE_D &TableD)
{    
	TableD.RecordD.Initialize();
	CStoryDisplDrift StDrft;
	T_LCOM_D  LcomData;
	int nStep = 0;
	T_DRIFT_METHOD DriftMethod;
	DriftMethod.Initialize();
	DriftMethod.bMethod[0] = TRUE;
	DriftMethod.bMethod[1] = TRUE;
	DriftMethod.bSameSelNode[0] = TRUE;
	DriftMethod.bSameSelNode[1] = TRUE;
	DriftMethod.bSameAverage[0] = TRUE;
	DriftMethod.bSameAverage[1] = TRUE;   
	//
	Report_Key_LIST lstCurLoads;
	Report_Key_LIST lstComb;
	Para()->CaseKeysOnly(lstCurLoads);
	Para()->LoadCombKeys(D_LCOMTYPE_GENERAL, lstComb);
	lstCurLoads.Append(lstComb);
	CArray<T_STOR_DFT, T_STOR_DFT&> aDFT;
	T_KEY_LIST lstStor;
	m_pDoc->m_pAttrCtrl->GetStorKeyList(lstStor);
	TableD.RecordD.aRecordData.SetSize(lstCurLoads.GetCount() * lstStor.GetCount());
	int nRow = 0;
	for (int i=0; i<lstCurLoads.GetCount(); i++)
	{        
		Report_Key LoadK = lstCurLoads[i];
		if(!MakeLoadComb(LoadK, LcomData))
			continue;
		aDFT.RemoveAll();
		if(!StDrft.GetDFT(LcomData, nStep, DriftMethod, aDFT) || aDFT.GetCount()<1)
			continue;
		for (int k=0; k<aDFT.GetCount(); k++)
		{
			const T_STOR_DFT &drift = aDFT[k];
			CStringArray &aFldValues = TableD.RecordD.aRecordData[nRow];
			aFldValues.SetSize(15);
			aFldValues[0] = LcomData.LoadCombName;
			aFldValues[1] = drift.strStorName;
			aFldValues[2] = Length(drift.dHeight);
			aFldValues[3] = ToString(drift.dPDFact[nDir], 2);
			aFldValues[4] = ToString(drift.dAllowableRatio, 4);          
			aFldValues[5] = ToString(drift.Max[nDir].NodeK);
			aFldValues[6] = Length  (drift.Max[nDir].dDrift);
			aFldValues[7] = Length  (drift.Max[nDir].dModDrift);
			aFldValues[8] = ToString(drift.Max[nDir].dRatio, 4);    
			aFldValues[9] = CellRltDes(drift.Max[nDir].nRemark==0);     
			aFldValues[10] = Length  (drift.Cen[nDir].dDrift);
			aFldValues[11] = Length  (drift.Cen[nDir].dModDrift);
			aFldValues[12] = ToString(drift.Cen[nDir].dRatio, 4);   
			aFldValues[13] = ToString(drift.Cen[nDir].dRatio, 4);    
			aFldValues[14] = CellRltDes(drift.Cen[nDir].nRemark==0);  
			nRow += 1;
			if(nRow >=TableD.RecordD.aRecordData.GetCount())
				break;
		}
	}

	TableD.RecordD.aRecordData.FreeExtra();
	TableD.RecordD.nRecordRowCount = nRow;
	TableD.RecordD.nRecordColCount = 15;
}

void AutoReport_Gen_CD::MakePic_4_3_Flor (GenSegmentElements &genElements, int &nIdx, LPCTSTR strCaseName)
{
	SetViewPoint(3);
	T_KEY_LIST lstStorKs;
	m_pDoc->m_pAttrCtrl->GetStorKeyList(lstStorKs);
	//
	CStoryData* pStoryData = m_pDoc->GetStoryData();
	T_STOR_D StorD;
	T_KEY_LIST lstElems;
	CString strName;
	for (int i=0; i<lstStorKs.GetCount(); i++)
	{      
		if(!m_pDoc->m_pAttrCtrl->GetStor(lstStorKs[i], StorD))
			continue;
		lstElems.RemoveAll();
		pStoryData->GetStoryElemKey(lstStorKs[i], lstElems);
		if(FilteElemByType(lstElems, BEAM_EL) < 1)
			continue;        
		ActiveElemAll(FALSE);
		ActiveObj(lstElems, TRUE);
		strName.Format(_T("EL %s Beam Force Result %s"), Length(StorD.dStoryLevel, TRUE), strCaseName);
		MakeImage_Current(strName, FALSE);
		MakePicture(genElements, strName, BookMark(_T("CHARPT_4_3_FIG"), ++nIdx));
	}
}

void AutoReport_Gen_CD::MakePic_4_3_Grup (GenSegmentElements &genElements, int &nIdx, LPCTSTR strCaseName)
{
	SetViewPoint(1);
	Report_Key_LIST lstKeys;
	Para()->GroupKeys(lstKeys);
	//
	CStoryData* pStoryData = m_pDoc->GetStoryData();
	T_GRUP_D GrupD;
	T_KEY_LIST lstElems;
	T_KEY_LIST lstNodes;
	T_KEY_LIST curGroup;
	CString strName;
	for (int i=0; i<lstKeys.GetCount(); i++)
	{      
		if(!m_pDoc->m_pAttrCtrl->GetGrup(lstKeys[i].m_nKey, GrupD))
			continue;
		curGroup.RemoveAll();
		curGroup.Add(lstKeys[i].m_nKey);
		lstElems.RemoveAll();
		m_pDoc->m_pAttrCtrl->GetKeyListGrupUser(&curGroup, &lstNodes, &lstElems);    
		if(FilteElemByType(lstElems, BEAM_EL) < 1)
			continue;     
		ActiveElemAll(FALSE);
		ActiveObj(lstElems, TRUE);
		strName.Format(_T("Vertical Plan(%s) Beam Force Result %s"), GrupD.GroupName, strCaseName);
		MakeImage_Current(strName, FALSE);
		MakePicture(genElements, strName, BookMark(_T("CHARPT_4_3_FIG"), ++nIdx));
	}
}

BOOL AutoReport_Gen_CD::CallRebarRltFig  ()
{
	SetPreOrPostMode(TRUE);
	GRenderView* pRenderView = (GRenderView*)(I_GENModelBase::GetCurMySelfST()->GetOwnerWnd()); ASSERT(pRenderView);
	SendMessage(pRenderView->GetSafeHwnd(), WM_COMMAND, MAKEWPARAM(ID_RESULT_CONCRETE_CONTOUR, 0), 0);
	CGPSBarContainer*pDlg = dynamic_cast<CGPSBarContainer*>(CGPSBarContainer::m_pMyself);
	if(pDlg && pDlg->GetSafeHwnd())
	{
		if(!pDlg->InitialCurChildDlgByData(nullptr))
			return FALSE;
		pDlg->CallExecute();    
		return TRUE;
	}
	return FALSE;
}

BOOL AutoReport_Gen_CD::CallBeamForceFig (const Report_Key &CaseK, int nComp)
{
	CGpsbarDlgInit_Common SdfD;
	SdfD.initial();
	SdfD.m_component = nComp;
	SdfD.m_load.m_caseKey  = CaseK.m_nKey;
	SdfD.m_load.m_caseType = CaseK.m_nType;
	SetPreOrPostMode(TRUE);
	GRenderView* pRenderView = (GRenderView*)(I_GENModelBase::GetCurMySelfST()->GetOwnerWnd()); ASSERT(pRenderView);
	SendMessage(pRenderView->GetSafeHwnd(), WM_COMMAND, MAKEWPARAM(ID_RESULT_FORCES_BDGRM, 0), 0);
	CGPSBarContainer*pDlg = dynamic_cast<CGPSBarContainer*>(CGPSBarContainer::m_pMyself);
	if(pDlg && pDlg->GetSafeHwnd())
	{
		if(!pDlg->InitialCurChildDlgByData(&SdfD))
			return FALSE;
		pDlg->CallExecute();      
		return TRUE;
	}
	return FALSE;
}
void AutoReport_Gen_CD::CloseCurDlgbar   ()
{
	if(CGPSBarContainer::m_pMyself == nullptr)
		return;
	CGPSBarContainer*pDlg = dynamic_cast<CGPSBarContainer*>(CGPSBarContainer::m_pMyself);
	if(pDlg && pDlg->GetSafeHwnd())
	{
		pDlg->CallClose();
		CGPSBarContainer::m_pMyself = nullptr;
	}
}


int AutoReport_Gen_CD::TempletID()
{
	return 0;
}
BOOL AutoReport_Gen_CD::PreMakeData()
{
	Initial();
	m_Segments.clear();
	SetShape(FALSE);
	//
	WriteCharpt1();
	WriteCharpt2();
	WriteCharpt3();
	WriteCharpt4();
	return TRUE;
}

void AutoReport_Gen_CD::MakeContent()
{   
	for (int i=0; i<m_Segments.size(); i++)
	{
		InsertSegmentElements(m_Segments[i]);
	}    
}
void AutoReport_Gen_CD::WriteCharpt1()
{
	T_PJCF_D data;
	if (!m_pDoc->m_pAttrCtrl->GetPjcf(data)) 
		data.Initialize();
	CString strProjectName = data.strProject;
	if(data.strProject.IsEmpty() || data.strProject == _T(""))
	{
		CString csPath = m_pDoc->GetPathName();
		CFileCtrl file(csPath);
		CFileStatus status;
		file.GetFileStatus(status);
		strProjectName = file.GetFileNameWithoutExtension(); 
	}
	GenSegmentElements genElements;
	MakeTitle(genElements, _T("GENERAL NOTES"), 1, 0);
	MakeText(genElements, _T(" The following design calculation covers the superstructure calculation of the ") + strProjectName, _T("CHAPT_1"));
	//
	WriteCharpt1_1(genElements);
	WriteCharpt1_2(genElements);
	WriteCharpt1_3(genElements);
	WriteCharpt1_4(genElements);
	WriteCharpt1_5(genElements);
	WriteCharpt1_6(genElements);
	// 
	m_Segments.push_back(genElements);
}
void AutoReport_Gen_CD::WriteCharpt1_1(GenSegmentElements &genElements)
{
	MakeTitle(genElements, _T("Geometry"), 1, 1);    
	if(Para()->SegmentIsCheck(1,1) == FALSE)
		return;
	//
	double dMaxX = 0;
	double dMaxY = 0;    
	double dMinLevel = 0;
	double dMaxLevel = 0;
	CArray<T_STOR_D, T_STOR_D&> rarStorList;
	m_pDoc->m_pAttrCtrl->GetStorData(rarStorList);
	for (int i=0; i<rarStorList.GetCount(); i++)
	{
		const T_STOR_D &StorD = rarStorList[i];
		if(i==0)
		{
			dMaxLevel = StorD.dStoryLevel;
			dMinLevel = StorD.dStoryLevel;
		}
		else
		{
			dMaxLevel = max(StorD.dStoryLevel, dMaxLevel);
			dMinLevel = min(StorD.dStoryLevel, dMinLevel);
		}
		dMaxX = max(dMaxX, StorD.WindWidthX);
		dMaxY = max(dMaxY, StorD.WindWidthY);
	}
	double dTolH = dMaxLevel - dMinLevel;
	CString strText_Type = _T("Structure Type£º") + ChangeLine();
	CString strText_Len  = _T("Length£º")  + Length(dMaxX,TRUE) + ChangeLine();
	CString strText_Wid  = _T("Width£º")   + Length(dMaxY,TRUE) + ChangeLine();
	CString strText_TolH = _T("Height£º")  + Length(dMaxY,TRUE);  
	MakeText(genElements, strText_Type+strText_Len+strText_Wid+strText_TolH, _T("CHARPT_1_1_TXT1"));
	//
	CString strName = MakePicture_1_1();
	MakePicture(genElements, strName, _T("CHARPT_1_1_FIG1"));
}
void AutoReport_Gen_CD::WriteCharpt1_2(GenSegmentElements &genElements)
{   
	MakeTitle(genElements, _T("Codes and Standard"), 1, 2);   
	if(Para()->SegmentIsCheck(1,2) == FALSE)
		return;
	CString strCodeWind, strCodeMatl, strCodeQuake;
	GetCodeInfo(strCodeMatl, strCodeWind, strCodeQuake);
	CString strText1 = _T("The structural analysis has been carried out in accordance with the following Codes and Standards£º") + ChangeLine();
	strText1 += _T("Materials Standards  : ")       + strCodeMatl  + ChangeLine();
	strText1 += _T("Wind Load Standards  : ")       + strCodeWind  + ChangeLine();
	strText1 += _T("Earthquake Load Standards  : ") + strCodeQuake + ChangeLine();
	strText1 += _T("Concrete Design Standards : ")  + CDBLib::GetConCodeName() + ChangeLine();
	strText1 += _T("Steel Design Standards : ")     + CDBLib::GetStlCodeName() + ChangeLine();
	MakeText(genElements, strText1, _T("CHARPT_1_2_TXT1"));
}
void AutoReport_Gen_CD::WriteCharpt1_3(GenSegmentElements &genElements)
{  
	MakeTitle(genElements, _T("Materials"), 1, 3);  
	if(Para()->SegmentIsCheck(1,3) == FALSE)
		return;
	CArray<T_MATL_K, T_MATL_K> rMatlKeyList;
	m_pDoc->m_pAttrCtrl->GetMatlKeyList(rMatlKeyList);  
	CArray<T_MATL_K, T_MATL_K> rConMatlKeyList;  
	m_pDoc->m_pAttrCtrl->GetMatlKeyListUsedInElem(rConMatlKeyList);  
	T_MATD_D rMatlData;
	rMatlData.Initialize();  
	int iIndex = 0;  //»ìÄýÍÁ

	T_TABLE_D TableD;
	TableD.Initialize();
	TableD.HeaderD.nHeaderRowCount = 1;
	TableD.HeaderD.nHeaderColCount = 12;
	TableD.HeaderD.aTitleName.Add(_T("ID"));
	TableD.HeaderD.aTitleName.Add(_T("Name"));
	TableD.HeaderD.aTitleName.Add(_T("Type"));
	TableD.HeaderD.aTitleName.Add(_T("Standard"));
	TableD.HeaderD.aTitleName.Add(_T("DB"));
	TableD.HeaderD.aTitleName.Add(_T("Elasticity\n")+ UnitDes(D_UNITSYS_BASE_STRESS, TRUE));  
	TableD.HeaderD.aTitleName.Add(_T("Poission"));
	TableD.HeaderD.aTitleName.Add(_T("Thermal"));
	TableD.HeaderD.aTitleName.Add(_T("Density\n") + UnitDes(D_UNITSYS_BASE_DENSITY, TRUE));
	TableD.HeaderD.aTitleName.Add(_T("Mass Density\n")+ UnitDes(D_UNITSYS_BASE_DENSITY, TRUE));
	TableD.HeaderD.aTitleName.Add(_T("fc")+ UnitDes(D_UNITSYS_BASE_STRESS, TRUE));
	TableD.HeaderD.aTitleName.Add(_T("fy")+ UnitDes(D_UNITSYS_BASE_STRESS, TRUE));  
	TableD.RecordD.aRecordData.SetSize(rConMatlKeyList.GetSize());  

	for (int i=0;i<rConMatlKeyList.GetSize();i++)
	{
		int nID = rConMatlKeyList.GetAt(i);
		if(!m_pDoc->m_pAttrCtrl->GetMatlDesign(nID,rMatlData))
			continue;
		if(rMatlData.Type == _T("C") || rMatlData.Type == _T("SRC"))
		{
			const T_MATD_ALL &Data = rMatlData.Type == _T("C")  ? rMatlData.Data1 : rMatlData.Data2;
			TableD.RecordD.aRecordData[iIndex].Add(ToString(nID));
			TableD.RecordD.aRecordData[iIndex].Add(rMatlData.Name);
			TableD.RecordD.aRecordData[iIndex].Add(rMatlData.Type);
			TableD.RecordD.aRecordData[iIndex].Add(Data.CodeName);
			TableD.RecordD.aRecordData[iIndex].Add(Data.CodeMatlName);
			TableD.RecordD.aRecordData[iIndex].Add(Stress(Data.Analysis.Elast));
			TableD.RecordD.aRecordData[iIndex].Add(ToString(Data.Analysis.Poisson, 2));
			TableD.RecordD.aRecordData[iIndex].Add(ToString(Data.Analysis.Thermal, 4));
			TableD.RecordD.aRecordData[iIndex].Add(Density (Data.Analysis.Density));
			TableD.RecordD.aRecordData[iIndex].Add(Density (Data.Analysis.MassDensity));
			TableD.RecordD.aRecordData[iIndex].Add(Stress  (Data.Design.C_fc));
			TableD.RecordD.aRecordData[iIndex].Add(_T("-"));           
			iIndex++;        
		}    
		if(rMatlData.Type == _T("S") || rMatlData.Type == _T("SRC"))
		{
			const T_MATD_ALL &Data = rMatlData.Data1;
			TableD.RecordD.aRecordData[iIndex].Add(ToString(nID));
			TableD.RecordD.aRecordData[iIndex].Add(rMatlData.Name);
			TableD.RecordD.aRecordData[iIndex].Add(rMatlData.Type);
			TableD.RecordD.aRecordData[iIndex].Add(Data.CodeName);
			TableD.RecordD.aRecordData[iIndex].Add(Data.CodeMatlName);
			TableD.RecordD.aRecordData[iIndex].Add(Stress(Data.Analysis.Elast));
			TableD.RecordD.aRecordData[iIndex].Add(ToString(Data.Analysis.Poisson, 2));
			TableD.RecordD.aRecordData[iIndex].Add(ToString(Data.Analysis.Thermal, 4));
			TableD.RecordD.aRecordData[iIndex].Add(Density (Data.Analysis.Density));
			TableD.RecordD.aRecordData[iIndex].Add(Density (Data.Analysis.MassDensity));
			TableD.RecordD.aRecordData[iIndex].Add(_T("-"));
			TableD.RecordD.aRecordData[iIndex].Add(Stress(Data.Design.S_Fy1));           
			iIndex++;        
		}    
	}  
	TableD.RecordD.aRecordData.FreeExtra();
	TableD.RecordD.nRecordRowCount = iIndex;
	TableD.RecordD.nRecordColCount = 12;     
	MakeTable(genElements, TableD     , _T("Materials Table"), _T("CHARPT_1_3_TBL1"));   
}
void AutoReport_Gen_CD::WriteCharpt1_4(GenSegmentElements &genElements)
{
	CString strSplitLine = _T("  __________________________________________________________________________\n");
	MakeTitle(genElements, _T("Wind date"), 1, 4);   
	if(Para()->SegmentIsCheck(1,4) == FALSE)
		return;
	//IBC2012£¨ASCE7-10£©/ IBC2009£¨ASCE7-05£©/ IBC2000£¨ASCE7-98£©/ UBC£¨1997£©/Eurocode-1£¨2005£©/ Eurocode-1£¨1992£©
	POSITION pos = m_pDoc->m_pAttrCtrl->GetStartWind();
	T_WIND_D windD;
	T_WIND_K windK;
	BOOL bIBC = FALSE;
	BOOL bUBC = FALSE;
	BOOL bEUR = FALSE;    
	CString strCurUnitLen = m_UnitName.strLength[m_UnitIndex.nBase_Length];
	CString strCurUnitFor = m_UnitName.strForce [m_UnitIndex.nBase_Force];
	while (pos)
	{
		CString strText = _T("");
		m_pDoc->m_pAttrCtrl->GetNextWind(pos, windK, windD);
		if(windD.nCodeType == 6 || windD.nCodeType == 17 || windD.nCodeType == 18)
		{
			if(bIBC == TRUE)
				continue;
			bIBC = TRUE;
			if(windD.nCodeType == 6)
			{
#pragma region IBC2000
				T_WIND_IBC2000 * pDataIBC2000 = &windD.CodeParam.IBC2000;
				char chrExpCategory = (char)(pDataIBC2000->nExposureCategory+64); // 2,3,4 -> B,C,D
				AddToText(strText,_T("    WIND LOADS BASED ON IBC2000(ASCE7-98)  [UNIT: %s, %s]\n"),strCurUnitFor, strCurUnitLen);
				AddToText(strText, strSplitLine);
				AddToText(strText,_T("    Basic Wind Speed [mph]                            : Vo  = %-6.2f\n"),pDataIBC2000->dBasicWindSpeed);
				AddToText(strText,_T("    Wind Directionality Factor                        : Kd  = %-6.2f\n"),pDataIBC2000->dWindDirectionFactor);
				AddToText(strText,_T("    Importance Factor                                 : I   = %-6.2f\n"),pDataIBC2000->dImportanceFactor);
				AddToText(strText,_T("    Exposure Category                                 : %c\n"),chrExpCategory);
				MakeText(genElements, strText, _T("WIND_ICB2000"));
#pragma endregion              
			}
			else if(windD.nCodeType == 18)
			{
#pragma region IBC2012
				T_WIND_IBC2012 * pData = &windD.CodeParam.IBC2012;
				char chrExpCategory = (char)(pData->nExposureCategory+64); // 2,3,4 -> B,C,D
				AddToText(strText,_T("    WIND LOADS BASED ON IBC2000(ASCE7-10)  [UNIT: %s, %s]\n"),strCurUnitFor, strCurUnitLen);
				AddToText(strText, strSplitLine);
				AddToText(strText,_T("    Basic Wind Speed [mph]                            : Vo  = %-6.2f\n"),pData->dBasicWindSpeed);
				AddToText(strText,_T("    Directional Factor for X-dir Wind Loads           : Kdx = %-6.2f\n"),pData->dDirectionFactorX);
				AddToText(strText,_T("    Directional Factor for X-dir Wind Loads           : Kdy = %-6.2f\n"),pData->dDirectionFactorY);
				AddToText(strText,_T("    Exposure Category                                 : %c\n"),chrExpCategory);
				MakeText(genElements, strText, _T("WIND_ICB2000"));
#pragma endregion              
			}
			else if(windD.nCodeType == 17)
			{
#pragma region IBC2009
				T_WIND_IBC2012 * pData = &windD.CodeParam.IBC2009;
				char chrExpCategory = (char)(pData->nExposureCategory+64); // 2,3,4 -> B,C,D
				AddToText(strText,_T("    WIND LOADS BASED ON IBC2000(ASCE7-05)  [UNIT: %s, %s]\n"),strCurUnitFor, strCurUnitLen);
				AddToText(strText, strSplitLine);
				AddToText(strText,_T("    Basic Wind Speed [mph]                            : Vo  = %-6.2f\n"),pData->dBasicWindSpeed);
				AddToText(strText,_T("    Directional Factor for X-dir Wind Loads           : Kdx = %-6.2f\n"),pData->dDirectionFactorX);
				AddToText(strText,_T("    Directional Factor for X-dir Wind Loads           : Kdy = %-6.2f\n"),pData->dDirectionFactorY);
				AddToText(strText,_T("    Importance Factor                                 : I   = %-6.2f\n"),pData->dImportanceFactor);
				AddToText(strText,_T("    Exposure Category                                 : %c\n"),chrExpCategory);
				MakeText(genElements, strText, _T("WIND_ICB2000"));
#pragma endregion              
			}
		}
		else if(windD.nCodeType == 3)
		{
			if(bUBC)
				continue;
			bUBC = TRUE;
#pragma region UBC1997
			T_WIND_UBC1997 * pData = &windD.CodeParam.UBC1997;
			char chrExpCategory = (char)(pData->nExposureCategory+64); // 2,3,4 -> B,C,D
			AddToText(strText,_T("    WIND LOADS BASED ON  UBC£¨1997£©  [UNIT: %s, %s]\n"),strCurUnitFor, strCurUnitLen);
			AddToText(strText, strSplitLine);
			AddToText(strText,_T("    Basic Wind Speed [mph]                            : V   = %-6.2f\n"),pData->dBasicWindSpeed);
			AddToText(strText,_T("    Importance Factor                                 : Iw  = %-6.2f\n"),pData->dImportanceFactor);
			AddToText(strText,_T("    Exposure Category                                 : %c\n"),chrExpCategory);
			MakeText(genElements, strText, _T("UBC_1997"));
#pragma endregion
		}
		else if(windD.nCodeType == 7)
		{
			if(bEUR)
				continue;
			bUBC = TRUE;
#pragma region EUR1992
			T_WIND_EURO1992* pData =  &windD.CodeParam.EURO1992;
			int RoughCategory = pData->nRoughCategory; // 1, 2, 3, 4
			CString strRoughCategory = _T("I");
			switch(RoughCategory)
			{
			case 1: strRoughCategory = _T("I"); break;
			case 2: strRoughCategory = _T("II"); break;
			case 3: strRoughCategory = _T("III"); break;
			case 4: strRoughCategory = _T("IV"); break;
			} 
			AddToText(strText,_T("     WIND LOADS BASED ON EUROCODE-1(1992)  [UNIT: %s, %s]\n"),strCurUnitFor, strCurUnitLen);
			AddToText(strText, strSplitLine);
			AddToText(strText,_T("    Roughness Category                         : %s\n") , strRoughCategory);
			AddToText(strText,_T("    Reference Wind Speed [m/sec]               : Vref = %-6.2f\n"),pData->dRefWindSpeed);
			AddToText(strText,_T("    Windward Pressure Coefficient              : Cpe1 = %-6.2f\n"),pData->dWindwardCoef);
			AddToText(strText,_T("    Leeward Pressure Coefficient               : Cpe2 = %-6.2f\n"),pData->dLeewardCoef);
			AddToText(strText,_T("    Leeward Pressure Coefficient               : Cpe2 = %-6.2f\n"),pData->dFrictionCoef);
			MakeText(genElements, strText, _T("EUR_1992"));
#pragma endregion EUR1992
		}
		else if(windD.nCodeType == 15)
		{
			if(bEUR)
				continue;
			bUBC = TRUE;
#pragma region EUR 2005
			T_WIND_EURO2005* pData = &windD.CodeParam.EURO2005;
			int    nTerrain      = pData->nTerrain;           // 0=0, 1=I, 2=II, 3=III, 4=IV 
			CString strTerrain = _T("I");
			if(nTerrain>=10)
			{
				switch(nTerrain)
				{
				case 10: strTerrain = _T("1");  break;
				case 11: strTerrain = _T("2");  break;
				}
			}
			else
			{
				switch(nTerrain)
				{
				case 0: strTerrain = _T("0");   break;  
				case 1: strTerrain = _T("I");   break;
				case 2: strTerrain = _T("II");  break;
				case 3: strTerrain = _T("III"); break;
				case 4: strTerrain = _T("IV");  break;
				}
			}      
			AddToText(strText,_T("    WIND LOADS BASED ON EUROCODE-1(2005)      [UNIT: %s, %s]\n"),strCurUnitFor, strCurUnitLen);
			AddToText(strText, strSplitLine);
			if(nTerrain>=10)
				AddToText(strText,_T("    Structure Type                                : %s\n"),strTerrain);
			else 
				AddToText(strText,_T("    Terrain Category                              : %s\n"),strTerrain);          

			AddToText(strText,_T("    Friction Coefficient                          : Cfr     = %-6.2f\n"), pData->dCfr);
			AddToText(strText,_T("    Fundamental Basic Wind Velocity [m/sec]       : Vb,o    = %-6.2f\n"), pData->dVb);
			AddToText(strText,_T("    Directional Factor                            : Cdir    = %-6.2f\n"), pData->dCdir);
			AddToText(strText,_T("    Seasonal Factor                               : Cseason = %-6.2f\n"), pData->dCseason);
			AddToText(strText,_T("    Turbulence Factor                             : Kl      = %-6.2f\n"), pData->dKl);
			MakeText(genElements, strText, _T("EUR_2005"));
#pragma endregion EUR 2005
		}
	}
}
void AutoReport_Gen_CD::WriteCharpt1_5(GenSegmentElements &genElements)
{
	CString strSplitLine = _T("  __________________________________________________________________________\n");
	MakeTitle(genElements, _T("Earthquake date"), 1, 5);   
	if(Para()->SegmentIsCheck(1,5) == FALSE)
		return;
	POSITION pos = m_pDoc->m_pAttrCtrl->GetStartSpfc();
	T_KEY nKey;
	T_SPFC_D SpfcD;
	CString strCurUnitLen = m_UnitName.strLength[m_UnitIndex.nBase_Length];
	CString strCurUnitFor = m_UnitName.strForce [m_UnitIndex.nBase_Force];
	CMap<int, int, int, int> mapDo;
	while (pos)
	{
		m_pDoc->m_pAttrCtrl->GetNextSpfc(pos, nKey, SpfcD);
		if(mapDo.PLookup(SpfcD.nSpecCodeType) != nullptr)
			continue;
		CString strText = _T("");
		mapDo.SetAt(SpfcD.nSpecCodeType, 1);
		if (SpfcD.nSpecCodeType == D_SPFC_CODE_NSCP_2024)
		{
			const T_SPFC_IBC2012& Data = SpfcD.CodeParam.NSCP2024;
			CString aSite[] = { _T("A"), _T("B"), _T("C"), _T("D"), _T("E") };
			CString strSite = _T("D");
			if (Data.nSc >= 0 && Data.nSc < 5)
				strSite = aSite[Data.nSc];
			AddToText(strText, _T("     DESIGN SPECTRUM BASED ON NSCP 2024  [UNIT: %s, %s]\n"), strCurUnitFor, strCurUnitLen);
			AddToText(strText, strSplitLine);
			AddToText(strText, _T("     Site Class                               : %s\n"), strSite);
			AddToText(strText, _T("     Spectral Acceleration(Ss)                : %-6.2f\n"), Data.dSs);
			AddToText(strText, _T("     Spectral Acceleration(S1)                : %-6.2f\n"), Data.dS1);
			AddToText(strText, _T("     Fa                                       : %-6.2f\n"), Data.dFa);
			AddToText(strText, _T("     Fv                                       : %-6.2f\n"), Data.dFv);
			AddToText(strText, _T("     Sds                                      : %-6.2f\n"), Data.dSds);
			AddToText(strText, _T("     Sd1                                      : %-6.2f\n"), Data.dSd1);
			AddToText(strText, _T("     Importance Factor                        : %-6.2f\n"), Data.dIe);
			AddToText(strText, _T("     Response Modification Coef.              : %-6.2f\n"), Data.dCoef);
			MakeText(genElements, strText, _T("D_SPFC_CODE_NSCP_2024"));
		}
		else if(SpfcD.nSpecCodeType == D_SPFC_CODE_IBC2012)
		{
			const T_SPFC_IBC2012 &Data = SpfcD.CodeParam.IBC2012;
			CString aSite[] = {_T("A"), _T("B"), _T("C"), _T("D"), _T("E")};
			CString strSite = _T("D");
			if(Data.nSc>=0 && Data.nSc<5)
				strSite = aSite[Data.nSc];
			AddToText(strText,_T("     DESIGN SPECTRUM BASED ON IBC2012£¨ASCE7-10£©  [UNIT: %s, %s]\n"),strCurUnitFor, strCurUnitLen);
			AddToText(strText, strSplitLine);
			AddToText(strText,_T("     Site Class                               : %s\n")    ,strSite);
			AddToText(strText,_T("     Spectral Acceleration(Ss)                : %-6.2f\n"),Data.dSs);
			AddToText(strText,_T("     Spectral Acceleration(S1)                : %-6.2f\n"),Data.dS1);
			AddToText(strText,_T("     Fa                                       : %-6.2f\n"),Data.dFa);
			AddToText(strText,_T("     Fv                                       : %-6.2f\n"),Data.dFv);
			AddToText(strText,_T("     Sds                                      : %-6.2f\n"),Data.dSds);
			AddToText(strText,_T("     Sd1                                      : %-6.2f\n"),Data.dSd1);
			AddToText(strText,_T("     Importance Factor                        : %-6.2f\n"),Data.dIe);
			AddToText(strText,_T("     Response Modification Coef.              : %-6.2f\n"),Data.dCoef); 
			MakeText(genElements, strText, _T("D_SPFC_CODE_IBC2012"));
		}
		else if(SpfcD.nSpecCodeType == D_SPFC_CODE_IBC2009)
		{
			const T_SPFC_IBC2012 &Data = SpfcD.CodeParam.IBC2009;
			CString aSite[] = {_T("A"), _T("B"), _T("C"), _T("D"), _T("E")};
			CString strSite = _T("D");
			if(Data.nSc>=0 && Data.nSc<5)
				strSite = aSite[Data.nSc];
			AddToText(strText,_T("     DESIGN SPECTRUM BASED ON IBC2009£¨ASCE7-05£©  [UNIT: %s, %s]\n"),strCurUnitFor, strCurUnitLen);
			AddToText(strText, strSplitLine);
			AddToText(strText,_T("     Site Class                               : %s\n")    ,strSite);
			AddToText(strText,_T("     Spectral Acceleration(Ss)                : %-6.2f\n"),Data.dSs);
			AddToText(strText,_T("     Spectral Acceleration(S1)                : %-6.2f\n"),Data.dS1);
			AddToText(strText,_T("     Fa                                       : %-6.2f\n"),Data.dFa);
			AddToText(strText,_T("     Fv                                       : %-6.2f\n"),Data.dFv);
			AddToText(strText,_T("     Sds                                      : %-6.2f\n"),Data.dSds);
			AddToText(strText,_T("     Sd1                                      : %-6.2f\n"),Data.dSd1);
			AddToText(strText,_T("     Importance Factor                        : %-6.2f\n"),Data.dIe);
			AddToText(strText,_T("     Response Modification Coef.              : %-6.2f\n"),Data.dCoef); 
			MakeText(genElements, strText, _T("D_SPFC_CODE_IBC2009"));
		}
		else if(SpfcD.nSpecCodeType == D_SPFC_CODE_IBC2000)
		{
			const T_SPFC_IBC2000 &Data = SpfcD.CodeParam.IBC2000;
			CString aSite[] = {_T("A"), _T("B"), _T("C"), _T("D"), _T("E")};
			CString strSite = _T("D");
			if(Data.nSc>=0 && Data.nSc<5)
				strSite = aSite[Data.nSc];
			AddToText(strText,_T("      DESIGN SPECTRUM BASED ON IBC2000£¨ASCE7-98£©  [UNIT: %s, %s]\n"),strCurUnitFor, strCurUnitLen);
			AddToText(strText, strSplitLine);
			AddToText(strText,_T("     Site Class                               : %s\n")    ,strSite);
			AddToText(strText,_T("     Spectral Acceleration(Ss)                : %-6.2f\n"),Data.dSs);
			AddToText(strText,_T("     Spectral Acceleration(S1)                : %-6.2f\n"),Data.dS1);
			MakeText(genElements, strText, _T("D_SPFC_CODE_IBC2000"));
		}
		else if(SpfcD.nSpecCodeType == D_SPFC_CODE_UBC97)
		{
			const T_SPFC_UBC97 &Data = SpfcD.CodeParam.UBC97;
			CString aSite[] = {_T("Sa"), _T("Sb"), _T("Sc"), _T("Sd"), _T("Se")};
			CString strSite = _T("Sd");
			if(Data.nSoilType>=0 && Data.nSoilType <5)
				strSite = aSite[Data.nSoilType];
			CString aSiteZ[] = {_T("Zone1(0.075)"), _T("Zone2A(0.15)"), _T("Zone2B(0.20)"), _T("Zone3(0.30)"), _T("Zone4(0.40)")};
			CString strSiteZ = _T("Zone3(0.30)");
			if(Data.nSeisZone>=0 && Data.nSeisZone <5)
				strSiteZ = aSite[Data.nSeisZone];
			AddToText(strText,_T("      DESIGN SPECTRUM BASED ON UBC£¨1997£©  [UNIT: %s, %s]\n"),strCurUnitFor, strCurUnitLen);
			AddToText(strText, strSplitLine);
			AddToText(strText,_T("     Soil Profile Type(S)                     : %s\n")    ,strSite);
			AddToText(strText,_T("     Seismic Zone Factor(Z)                   : %s\n")    ,strSiteZ);
			AddToText(strText,_T("     Seismic coefficient (Ca)                 : %-6.2f\n"),Data.dCa);
			AddToText(strText,_T("     Seismic coefficient (Cv)                 : %-6.2f\n"),Data.dCv);
			MakeText(genElements, strText, _T("D_SPFC_CODE_UBC97"));
		}
		else if(SpfcD.nSpecCodeType == D_SPFC_CODE_EURO2004)
		{
			const T_SPFC_EURO2004 &Data = SpfcD.CodeParam.EURO2004;
			CString aSite[] = {_T("A"), _T("B"), _T("C"), _T("D"), _T("E"), _T("S1"), _T("S2")};
			CString strSite = _T("D");
			if(Data.nGroundType>=0 && Data.nGroundType <7)
				strSite = aSite[Data.nGroundType];
			AddToText(strText,_T("      DESIGN SPECTRUM BASED ON Eurocode-8£¨2004£©  [UNIT: %s, %s]\n"),strCurUnitFor, strCurUnitLen);
			AddToText(strText, strSplitLine);
			AddToText(strText,_T("      Ground Type                              : %s\n")    ,strSite);
			AddToText(strText,_T("      Soil Factor(S)                           : %-6.2f\n"),Data.dSoilFactor);
			AddToText(strText,_T("      Tb                                       : %-6.2f\n"),Data.dTb );
			AddToText(strText,_T("      Tc                                       : %-6.2f\n"),Data.dTc );
			AddToText(strText,_T("      Td                                       : %-6.2f\n"),Data.dTd );
			AddToText(strText,_T("      Reference Peak Ground Acceleration (AgR) : %-6.2f\n"),Data.dAgr);
			AddToText(strText,_T("      Importance Factor                        : %-6.2f\n"),Data.dI  );
			AddToText(strText,_T("      Damping Ration(%)                        : %-6.2f\n"),Data.dXi*100);
			MakeText(genElements, strText, _T("D_SPFC_CODE_EURO2004"));
		}
		else if(SpfcD.nSpecCodeType == D_SPFC_CODE_EURO1996)
		{
			const T_SPFC_EURO1996 &Data = SpfcD.CodeParam.EURO1996;
			CString aSite[] = {_T("A(1.0)"), _T("B(1.0)"), _T("C(0.9)")};
			CString strSite = _T("B(1.0)");
			if(Data.nSc>=0 && Data.nSc <3)
				strSite = aSite[Data.nSc];
			AddToText(strText,_T("      DESIGN SPECTRUM BASED ON Eurocode-8£¨1996£©Design  [UNIT: %s, %s]\n"),strCurUnitFor, strCurUnitLen);
			AddToText(strText, strSplitLine);
			AddToText(strText,_T("      Soil Class                            : %s\n")    ,strSite);
			AddToText(strText,_T("      Basic Behavior Factor(qo)             : %-6.2f\n"),Data.dQ0);
			AddToText(strText,_T("      Ductility Class (Kd)                  : %-6d\n")  ,Data.nKd );
			AddToText(strText,_T("      Structural Regularity in Elevation(Kr): %-6d\n")  ,Data.nKr );
			AddToText(strText,_T("      Failure Mode Factor(Kw)               : %-6.2f\n"),Data.dKw );
			AddToText(strText,_T("      Alpha                                 : %-6.2f\n"),Data.dAlpha);
			MakeText(genElements, strText, _T("D_SPFC_CODE_EURO1996"));
		}
		else if(SpfcD.nSpecCodeType == D_SPFC_CODE_EURO1996_ELA)
		{
			const T_SPFC_EURO1996_ELA &Data = SpfcD.CodeParam.EURO1996_ELA;
			CString aSite[] = {_T("A(1.0)"), _T("B(1.0)"), _T("C(0.9)")};
			CString strSite = _T("B(1.0)");
			if(Data.nSc>=0 && Data.nSc <3)
				strSite = aSite[Data.nSc];
			AddToText(strText,_T("      DESIGN SPECTRUM BASED ON Eurocode-8£¨1996£©Elastic  [UNIT: %s, %s]\n"),strCurUnitFor, strCurUnitLen);
			AddToText(strText, strSplitLine);
			AddToText(strText,_T("      Soil Class                            : %s\n")    ,strSite);
			AddToText(strText,_T("      Design Ground Acceleration(g)         : %-6.2f\n"),Data);
			AddToText(strText,_T("      Damping Ration(%)                     : %-6.2f\n"),Data.dDamping*100);
			MakeText(genElements, strText, _T("D_SPFC_CODE_EURO1996_ELA"));
		}
	}
}
void AutoReport_Gen_CD::WriteCharpt1_6(GenSegmentElements &genElements)
{
	MakeTitle(genElements, _T("Software used"), 1, 6);   
	if(Para()->SegmentIsCheck(1,6) == FALSE)
		return;
	CString strText  = _T("This concrete frame was analyzed by MIDAS/Gen. MIDAS/Gen is a program for structural analysis and optimal");
	strText += _T(" design in the civil engineering and architecture domains. The program has been developed so that structural");
	strText += _T(" analysis and design can be accurately completed within the shortest possible time.");
	MakeText(genElements, strText, _T("CHARPT_1_6_TXT1"));

}
void AutoReport_Gen_CD::WriteCharpt2()
{
	GenSegmentElements Elems;
	MakeTitle(Elems, _T("OVERVIEW OF CALCULATION RESULT"), 2, 0);
	WriteCharpt2_1(Elems);
	WriteCharpt2_2(Elems);
	WriteCharpt2_3(Elems);
	WriteCharpt2_4(Elems);
	WriteCharpt2_5(Elems);
	m_Segments.push_back(Elems);
}
void AutoReport_Gen_CD::WriteCharpt2_1(GenSegmentElements &Elems)
{    
	MakeTitle(Elems, _T("Eigenvalue Mode"), 2, 1);  
	if(Para()->SegmentIsCheck(2,1) == FALSE)
		return;
	T_TABLE_D TableD;
	TableD.Initialize();
	TableD.HeaderD.nHeaderRowCount = 2;
	TableD.HeaderD.nHeaderColCount = 4;
	TableD.HeaderD.aTitleName.Add(_T("Mode Num"));
	TableD.HeaderD.aTitleName.Add(_T("Frequency"));
	TableD.HeaderD.aTitleName.Add(_T("Period"));
	//
	TableD.HeaderD.aTitleName.Add(_T("(rad/sec)"));
	TableD.HeaderD.aTitleName.Add(_T("(cycle/sec)"));
	TableD.HeaderD.aTitleName.Add(_T("(sec)"));
	T_MERGE_UNIT_D merge;merge.Initialize();
	merge.nMergeCount = 2; merge.nStartColNumber=1; merge.nStartRowNumber=1;
	TableD.HeaderD.aMergeRowD.Add(merge);
	merge.Initialize();
	merge.nMergeCount = 2; merge.nStartColNumber=2; merge.nStartRowNumber=1;
	TableD.HeaderD.aMergeColD.Add(merge);
	//   
	int nModeNum = m_pDoc->m_pPostCtrl->GetNumEigvPost();
	TableD.RecordD.aRecordData.SetSize(nModeNum);
	T_FREQ_D data;
	for (int i = 0; i < nModeNum; i++)
	{
		m_pDoc->m_pPostCtrl->GetEiFreq(i+1, data);
		TableD.RecordD.aRecordData[i].Add(ToString(i+1, 0));
		for (int j = 0; j < 3; j++)
		{
			TableD.RecordD.aRecordData[i].Add(ToString(data.dblFreq[j], 4));
		}
	}    
	//
	TableD.RecordD.aRecordData.FreeExtra();
	TableD.RecordD.nRecordRowCount = nModeNum;
	TableD.RecordD.nRecordColCount = 4;

	MakeTable(Elems, TableD     , _T("Eigenvalue Mode Table"), _T("CHARPT_2_1_TBL1"));  
}
void AutoReport_Gen_CD::WriteCharpt2_2(GenSegmentElements &Elems)
{
	MakeTitle(Elems, _T("Drift"), 2, 2);  
	if(Para()->SegmentIsCheck(2,2) == FALSE)
		return;
	T_TABLE_D TableD;
	TableD.Initialize();
	TableD.HeaderD.nHeaderRowCount = 1;
	TableD.HeaderD.nHeaderColCount = 2;
	TableD.HeaderD.aTitleName.Add(_T("Direction"));
	TableD.HeaderD.aTitleName.Add(_T("Column Top Max Drift"));
	//   
	CString aDes[] = {_T("Drift X"), _T("Drift Y"), _T("Max Drift X"), _T("Max Drift Y"),};
	CString aVal[] = {_T("-"),_T("-"),_T("-"),_T("-")};
	GetStoryDrift(aVal);

	TableD.RecordD.aRecordData.SetSize(4);
	for (int i=0; i<4; i++)
	{
		TableD.RecordD.aRecordData[i].Add(aDes[i]);
		TableD.RecordD.aRecordData[i].Add(aVal[i]);
	}
	//
	TableD.RecordD.aRecordData.FreeExtra();
	TableD.RecordD.nRecordRowCount = 4;
	TableD.RecordD.nRecordColCount = 2;

	MakeTable(Elems, TableD , _T("Drift Table"), _T("CHARPT_2_2_TBL1"));  
}
void AutoReport_Gen_CD::WriteCharpt2_3(GenSegmentElements &Elems)
{
	MakeTitle(Elems, _T("Stiffness ratio"), 2, 3);  
	if(Para()->SegmentIsCheck(2,3) == FALSE)
		return;
}
void AutoReport_Gen_CD::WriteCharpt2_4(GenSegmentElements &Elems)
{
	MakeTitle(Elems, _T("Shear weight ratio"), 2, 4); 
	if(Para()->SegmentIsCheck(2,4) == FALSE)
		return;
}
void AutoReport_Gen_CD::WriteCharpt2_5(GenSegmentElements &Elems)
{
	MakeTitle(Elems, _T("Shear capacity"), 2, 5); 
	if(Para()->SegmentIsCheck(2,5) == FALSE)
		return;
}
void AutoReport_Gen_CD::WriteCharpt3  ()
{
	GenSegmentElements Elems;
	MakeTitle(Elems, _T("MODEL INFORMATION"), 3, 0);
	WriteCharpt3_1(Elems);
	WriteCharpt3_2(Elems);
	WriteCharpt3_3(Elems);
	WriteCharpt3_4(Elems);
	WriteCharpt3_5(Elems);
	WriteCharpt3_6(Elems);
	WriteCharpt3_7(Elems);
	m_Segments.push_back(Elems);
}
void AutoReport_Gen_CD::WriteCharpt3_1(GenSegmentElements &genElements)
{
	MakeTitle(genElements, _T("Column and beam plan"), 3, 1);   
	if(Para()->SegmentIsCheck(3,1) == FALSE)
		return;
	SetViewPoint(3);
	DrawNodeAll(FALSE);
	T_KEY_LIST lstStorKs;
	m_pDoc->m_pAttrCtrl->GetStorKeyList(lstStorKs);
	//
	CStoryData* pStoryData = m_pDoc->GetStoryData();
	T_STOR_D StorD;
	T_KEY_LIST lstElems;
	CString strName;
	for (int i=0; i<lstStorKs.GetCount(); i++)
	{      
		if(!m_pDoc->m_pAttrCtrl->GetStor(lstStorKs[i], StorD))
			continue;
		lstElems.RemoveAll();
		pStoryData->GetStoryElemKey(lstStorKs[i], lstElems);
		if(FilteElemByType(lstElems, BEAM_EL) < 1)
			continue;        
		ActiveElemAll(FALSE);
		ActiveObj(lstElems, TRUE);
		strName.Format(_T("EL %s Column & Beam Plan"), Length(StorD.dStoryLevel, TRUE));
		MakeImage_Current(strName, FALSE);
		MakePicture(genElements, strName, BookMark(_T("CHARPT_3_1_FIG"), i+1));
	}
}
void AutoReport_Gen_CD::WriteCharpt3_2(GenSegmentElements &genElements)
{
	MakeTitle(genElements, _T("Vertical Plan(By Group) "), 3, 2, _T("VERTICAL_PLAN_BY_GROUP"));
	if(Para()->SegmentIsCheck(3,2) == FALSE)
		return;
	if(Para()->IsUseGroup() == FALSE)
		return;
	DrawNodeAll(FALSE);
	SetViewPoint(1);
	Report_Key_LIST lstKeys;
	Para()->GroupKeys(lstKeys);
	//
	CStoryData* pStoryData = m_pDoc->GetStoryData();
	T_GRUP_D GrupD;
	T_KEY_LIST lstElems;
	T_KEY_LIST lstNodes;
	T_KEY_LIST curGroup;
	CString strName;
	for (int i=0; i<lstKeys.GetCount(); i++)
	{      
		if(!m_pDoc->m_pAttrCtrl->GetGrup(lstKeys[i].m_nKey, GrupD))
			continue;
		curGroup.RemoveAll();
		curGroup.Add(lstKeys[i].m_nKey);
		lstElems.RemoveAll();
		m_pDoc->m_pAttrCtrl->GetKeyListGrupUser(&curGroup, &lstNodes, &lstElems);    
		ActiveElemAll(FALSE);
		ActiveObj(lstElems, TRUE);
		strName.Format(_T("Vertical Plan(%s)"), GrupD.GroupName);
		MakeImage_Current(strName, FALSE);
		MakePicture(genElements, strName, BookMark(_T("CHARPT_3_2_FIG"), i+1));
	}
}
void AutoReport_Gen_CD::WriteCharpt3_3(GenSegmentElements &genElements)
{
	MakeTitle(genElements, _T("Element Weight"), 3, 3);   
	if(Para()->SegmentIsCheck(3,3) == FALSE)
		return;
	T_TABLE_D TableD;
	TableD.Initialize();
	TableD.HeaderD.nHeaderRowCount = 2;
	TableD.HeaderD.nHeaderColCount = 12;
	TableD.HeaderD.aTitleName.Add(_T("No"));
	TableD.HeaderD.aTitleName.Add(_T("Type"));
	TableD.HeaderD.aTitleName.Add(_T("Material"));
	TableD.HeaderD.aTitleName.Add(_T("Section"));
	TableD.HeaderD.aTitleName.Add(_T("Thickness"));
	TableD.HeaderD.aTitleName.Add(_T("L/A/V"));
	TableD.HeaderD.aTitleName.Add(_T("Unit  Weight")  + UnitDes(D_UNITSYS_BASE_DENSITY, TRUE));
	TableD.HeaderD.aTitleName.Add(_T("Total Weight")  + UnitDes(D_UNITSYS_BASE_FORCE, TRUE));
	//
	TableD.HeaderD.aTitleName.Add(_T("No"));
	TableD.HeaderD.aTitleName.Add(_T("Name"));
	TableD.HeaderD.aTitleName.Add(_T("No"));
	TableD.HeaderD.aTitleName.Add(_T("Name"));
	TableD.HeaderD.aTitleName.Add(_T("No"));
	TableD.HeaderD.aTitleName.Add(_T("Name"));
	TableD.HeaderD.aTitleName.Add(_T("Type"));
	TableD.HeaderD.aTitleName.Add(_T("Value"));
	T_MERGE_UNIT_D merge;merge.Initialize();
	merge.nMergeCount = 2; merge.nStartColNumber=1; merge.nStartRowNumber=1;
	TableD.HeaderD.aMergeRowD.Add(merge);
	merge.Initialize();
	merge.nMergeCount = 2; merge.nStartColNumber=2; merge.nStartRowNumber=1;
	TableD.HeaderD.aMergeRowD.Add(merge);
	merge.Initialize();
	merge.nMergeCount = 2; merge.nStartColNumber=9; merge.nStartRowNumber=1;
	TableD.HeaderD.aMergeColD.Add(merge);
	merge.Initialize();
	merge.nMergeCount = 2; merge.nStartColNumber=3; merge.nStartRowNumber=1;
	TableD.HeaderD.aMergeColD.Add(merge);
	merge.Initialize();
	merge.nMergeCount = 2; merge.nStartColNumber=5; merge.nStartRowNumber=1;
	TableD.HeaderD.aMergeColD.Add(merge);
	merge.Initialize();
	merge.nMergeCount = 2; merge.nStartColNumber=7; merge.nStartRowNumber=1;
	TableD.HeaderD.aMergeColD.Add(merge);


	T_KEY_LIST aKey;
	T_ELEM_D ElemData;
	m_pDoc->m_pAttrCtrl->GetElemKeyList(aKey);
	TableD.RecordD.aRecordData.SetSize(aKey.GetCount()); 
	int nRow = 0;
	for (int i=0; i<aKey.GetCount(); i++)
	{
		T_KEY nKey = aKey[i];
		if (!m_pDoc->m_pAttrCtrl->GetElem(nKey, ElemData)) 
			continue;
		TableD.RecordD.aRecordData[nRow].Add(ToString(nKey));
		TableD.RecordD.aRecordData[nRow].Add(m_pDoc->m_pAttrCtrl->GetEltypName(ElemData.eltyp));
		T_MATL_D MatlData;
		if (m_pDoc->m_pAttrCtrl->IsNeedMatl(ElemData.eltyp) && 
			m_pDoc->m_pAttrCtrl->GetMatl(ElemData.elmat, MatlData))
		{
			TableD.RecordD.aRecordData[nRow].Add(ToString(ElemData.elmat)); 
			TableD.RecordD.aRecordData[nRow].Add(MatlData.Name);
		}
		else
		{
			TableD.RecordD.aRecordData[nRow].Add(_T("-"));
			TableD.RecordD.aRecordData[nRow].Add(_T("-"));
		}
		T_SECT_D SectData;
		if (m_pDoc->m_pAttrCtrl->IsNeedSect(ElemData.eltyp) &&
			m_pDoc->m_pAttrCtrl->GetSect(ElemData.elpro, SectData))
		{
			TableD.RecordD.aRecordData[nRow].Add(ToString(ElemData.elpro)); 
			TableD.RecordD.aRecordData[nRow].Add(SectData.SName);
		}
		else
		{
			TableD.RecordD.aRecordData[nRow].Add(_T("-"));
			TableD.RecordD.aRecordData[nRow].Add(_T("-"));
		}

		T_THIK_D ThikData;
		if (m_pDoc->m_pAttrCtrl->IsNeedThik(ElemData.eltyp) &&
			m_pDoc->m_pAttrCtrl->GetThik(ElemData.elpro, ThikData))
		{
			double dThickness = 0;
			if (!m_pDoc->m_pAttrCtrl->GetThikForThikName(ThikData, dThickness)) ASSERT(0);
			TableD.RecordD.aRecordData[nRow].Add(ToString(ElemData.elpro)); 
			TableD.RecordD.aRecordData[nRow].Add(Length(dThickness));
		}
		else
		{
			TableD.RecordD.aRecordData[nRow].Add(_T("-"));
			TableD.RecordD.aRecordData[nRow].Add(_T("-"));
		}
		if (m_pDoc->m_pAttrCtrl->IsFrameType(ElemData.eltyp))
			TableD.RecordD.aRecordData[nRow].Add(_T("L"));
		else if (m_pDoc->m_pAttrCtrl->IsPlaneType(ElemData.eltyp))
			TableD.RecordD.aRecordData[nRow].Add(_T("A"));
		else if (m_pDoc->m_pAttrCtrl->IsSolidType(ElemData.eltyp))
			TableD.RecordD.aRecordData[nRow].Add(_T("V"));
		else ASSERT(0);

		CSelfWgt DataSrc(m_pDoc);
		T_ELEM_WEIGHT WgData;
		DataSrc.GetElemWeight(nKey, WgData);
		TableD.RecordD.aRecordData[nRow].Add(Length(WgData.dLAV));
		TableD.RecordD.aRecordData[nRow].Add(Density(WgData.dUnitWeight));
		TableD.RecordD.aRecordData[nRow].Add(Force(WgData.dTotalWeight));
		nRow += 1; 
	}
	TableD.RecordD.aRecordData.FreeExtra();
	TableD.RecordD.nRecordRowCount = nRow;
	TableD.RecordD.nRecordColCount = 12;    

	MakeTable(genElements, TableD     , _T("Element Weight Table"), _T("CHARPT_3_3_TBL1"));   
}
void AutoReport_Gen_CD::WriteCharpt3_4(GenSegmentElements &genElements)
{
	MakeTitle(genElements, _T("Storey  Weight"), 3, 4);   
	if(Para()->SegmentIsCheck(3,4) == FALSE)
		return;
	T_TABLE_D TableD;
	TableD.Initialize();
	TableD.HeaderD.nHeaderRowCount = 2;
	TableD.HeaderD.nHeaderColCount = 9;
	TableD.HeaderD.aTitleName.Add(_T("Story"));
	TableD.HeaderD.aTitleName.Add(_T("Level ") + UnitDes(D_UNITSYS_BASE_LENGTH, TRUE));
	TableD.HeaderD.aTitleName.Add(_T("---- Element Weight ----"));
	//
	TableD.HeaderD.aTitleName.Add(_T("Truss ")+ UnitDes(D_UNITSYS_BASE_FORCE, TRUE));
	TableD.HeaderD.aTitleName.Add(_T("Beam ") + UnitDes(D_UNITSYS_BASE_FORCE, TRUE));
	TableD.HeaderD.aTitleName.Add(_T("Membrane ")+ UnitDes(D_UNITSYS_BASE_FORCE, TRUE));
	TableD.HeaderD.aTitleName.Add(_T("Plate ")+ UnitDes(D_UNITSYS_BASE_FORCE, TRUE));
	TableD.HeaderD.aTitleName.Add(_T("Wall ")+ UnitDes(D_UNITSYS_BASE_FORCE, TRUE));
	TableD.HeaderD.aTitleName.Add(_T("Solid ")+ UnitDes(D_UNITSYS_BASE_FORCE, TRUE));
	TableD.HeaderD.aTitleName.Add(_T("Sum ")+ UnitDes(D_UNITSYS_BASE_FORCE, TRUE));

	T_MERGE_UNIT_D merge;merge.Initialize();
	merge.nMergeCount = 2; merge.nStartColNumber=1; merge.nStartRowNumber=1;
	TableD.HeaderD.aMergeRowD.Add(merge);
	merge.Initialize();
	merge.nMergeCount = 2; merge.nStartColNumber=2; merge.nStartRowNumber=1;
	TableD.HeaderD.aMergeRowD.Add(merge);
	merge.Initialize();
	merge.nMergeCount = 7; merge.nStartColNumber=3; merge.nStartRowNumber=1;
	TableD.HeaderD.aMergeColD.Add(merge);
	//
	CStoryLoad StWgtCal;
	CMap<unsigned int, unsigned int, T_StoryWeight, T_StoryWeight&> StorWgt;
	StWgtCal.Calc_Story_Weight(StorWgt);
	T_StoryWeight StWgt;
	T_KEY_LIST aKey;
	m_pDoc->m_pAttrCtrl->GetStorKeyList(aKey);
	aKey.Add(0);//ºÏ¼Æ
	TableD.RecordD.aRecordData.SetSize(aKey.GetCount()); 
	int nRow = 0;
	T_STOR_D StorData;
	for (int i=0; i<aKey.GetCount(); i++)
	{
		T_KEY nKey = aKey[i];        
		if(!StorWgt.Lookup(nKey, StWgt))
			continue;
		if(nKey !=0 && m_pDoc->m_pAttrCtrl->GetStor(nKey, StorData) == FALSE)
			continue;
		if(nKey != 0)
		{
			TableD.RecordD.aRecordData[nRow].Add( StorData.StoryName);
			TableD.RecordD.aRecordData[nRow].Add( Length(StorData.dStoryLevel));
		}
		else
		{
			TableD.RecordD.aRecordData[nRow].Add( _T("SUM"));
			TableD.RecordD.aRecordData[nRow].Add( _T("--"));
		}
		TableD.RecordD.aRecordData[nRow].Add( Force(StWgt.TrussWeight));
		TableD.RecordD.aRecordData[nRow].Add( Force(StWgt.BeamWeight));
		TableD.RecordD.aRecordData[nRow].Add( Force(StWgt.PlnStressWeight));
		TableD.RecordD.aRecordData[nRow].Add( Force(StWgt.PlateWeight));
		TableD.RecordD.aRecordData[nRow].Add( Force(StWgt.WallWeight));
		TableD.RecordD.aRecordData[nRow].Add( Force(StWgt.SolidWeight));
		TableD.RecordD.aRecordData[nRow].Add( Force(StWgt.Sum));
		nRow += 1;
	}
	TableD.RecordD.aRecordData.FreeExtra();
	TableD.RecordD.nRecordRowCount = nRow;
	TableD.RecordD.nRecordColCount = 9;    
	MakeTable(genElements, TableD     , _T("Storey   Weight Table"), _T("CHARPT_3_4_TBL1"));   
}
void AutoReport_Gen_CD::WriteCharpt3_5(GenSegmentElements &genElements)
{
	MakeTitle(genElements, _T("Storey  Mass"), 3, 5); 
	if(Para()->SegmentIsCheck(3,5) == FALSE)
		return;
	T_KEY_LIST aKey;
	m_pDoc->m_pAttrCtrl->GetStorKeyList(aKey);
	int nNum = aKey.GetSize();
	if (nNum == 0)
		return;
	T_BLDC_D bldc;
	if (!m_pDoc->m_pAttrCtrl->GetBldc(bldc))
		bldc.Initialize();
	CString csGrLevel, csConsiderMass;
	if (bldc.bUseBaseLevel) 
		csGrLevel.Format(_T("  Use Ground Level : ON, Ground Level , %s\n"), Length(bldc.dBaseLevel));
	else 
		csGrLevel =      _T("  Use Ground Level : OFF\n");
	if (bldc.bBaseMass) 
		csConsiderMass = _T("  Consider Mass under Ground Level : OFF");
	else 
		csConsiderMass = _T("  Consider Mass under Ground Level : ON");
	MakeText(genElements, csGrLevel + csConsiderMass, _T("CHARPT_3_5_TXT1")); 

	double (*DIAPMS)[3] = new double[nNum][3];
	double (*DIAPXY)[2] = new double[nNum][2];
	double (*RLSMSS)[2] = new double[nNum][2];
	CLateralLoad StorMass;
	StorMass.Make_DiaphramMassTableData(DIAPMS, DIAPXY, RLSMSS);
	//
	CString strUnit;
	T_TABLE_D TableD;
	TableD.Initialize();
	TableD.HeaderD.nHeaderRowCount = 2;
	TableD.HeaderD.nHeaderColCount = 7;
	TableD.HeaderD.aTitleName.Add(_T("Story"));
	TableD.HeaderD.aTitleName.Add(_T("Level ") + UnitDes(D_UNITSYS_BASE_LENGTH, TRUE));
	TableD.HeaderD.aTitleName.Add(_T("Translational Mass"));
	strUnit.Format(_T("Rotational Mass\n  (%s/g*%s)"), UnitDes(D_UNITSYS_BASE_FORCE), UnitDes(D_UNITSYS_BASE_AREA));
	TableD.HeaderD.aTitleName.Add(strUnit);
	TableD.HeaderD.aTitleName.Add(_T("Center of Mass"));
	//
	strUnit.Format(_T("(%s/g)"), UnitDes(D_UNITSYS_BASE_FORCE));
	TableD.HeaderD.aTitleName.Add(_T("X-DIR") + strUnit);
	TableD.HeaderD.aTitleName.Add(_T("Y-DIR") + strUnit);
	TableD.HeaderD.aTitleName.Add(_T("X-Coord") + UnitDes(D_UNITSYS_BASE_LENGTH, TRUE));
	TableD.HeaderD.aTitleName.Add(_T("Y-Coord") + UnitDes(D_UNITSYS_BASE_LENGTH, TRUE));

	MergeHeadRow(TableD, 5, 1, 2);
	MergeHeadRow(TableD, 2, 1, 2);
	MergeHeadRow(TableD, 1, 1, 2);
	MergeHeadCol(TableD, 6, 1, 2);
	MergeHeadCol(TableD, 3, 1, 2);
	//
	aKey.Add(0);//for sum
	nNum = aKey.GetCount();  
	TableD.RecordD.aRecordData.SetSize(nNum);
	//
	int nRow = 0;
	T_STOR_D StorData;
	double dSum1=0;
	double dSum2=0;
	for (int i=0; i<aKey.GetCount(); i++)
	{
		T_KEY nKey = aKey[i];        
		if(nKey !=0 && m_pDoc->m_pAttrCtrl->GetStor(nKey, StorData) == FALSE)
			continue;
		if(nKey != 0)
		{
			TableD.RecordD.aRecordData[nRow].Add( StorData.StoryName);
			TableD.RecordD.aRecordData[nRow].Add( Length(StorData.dStoryLevel));
		}
		else
		{
			TableD.RecordD.aRecordData[nRow].Add( _T("SUM"));
			TableD.RecordD.aRecordData[nRow].Add( _T("--"));
		}
		if(nKey != 0)
		{
			TableD.RecordD.aRecordData[nRow].Add(Force (DIAPMS[i][0], FALSE, 4));
			TableD.RecordD.aRecordData[nRow].Add(Force (DIAPMS[i][1], FALSE, 4));
			TableD.RecordD.aRecordData[nRow].Add(Moment(DIAPMS[i][2], FALSE, 4));
			TableD.RecordD.aRecordData[nRow].Add(Length(DIAPXY[i][0]));
			TableD.RecordD.aRecordData[nRow].Add(Length(DIAPXY[i][1]));
			dSum1 += DIAPMS[i][0];
			dSum2 += DIAPMS[i][1];
		}
		else
		{
			TableD.RecordD.aRecordData[nRow].Add(Force (dSum1));
			TableD.RecordD.aRecordData[nRow].Add(Force (dSum1));
		}                                                     
		nRow += 1;
	}
	TableD.RecordD.aRecordData.FreeExtra();
	TableD.RecordD.nRecordRowCount = nRow;// nRow;
	TableD.RecordD.nRecordColCount = 7;    
	MakeTable(genElements, TableD, _T("Storey Mass Table"), _T("CHARPT_3_5_TBL1"));   
	//ADDITIONAL MASSES FOR THE CALCULATION OF EQUIVALENT SEISMIC FORCE
	TableD.Initialize();
	TableD.RecordD.aRecordData.SetSize(nNum);
	TableD.HeaderD.nHeaderRowCount = 2;
	TableD.HeaderD.nHeaderColCount = 4;
	TableD.HeaderD.aTitleName.Add(_T("Story"));
	TableD.HeaderD.aTitleName.Add(_T("Level ") + UnitDes(D_UNITSYS_BASE_LENGTH, TRUE));
	TableD.HeaderD.aTitleName.Add(_T("Translational Mass"));
	//
	strUnit.Format(_T("(%s/g)"), UnitDes(D_UNITSYS_BASE_FORCE));
	TableD.HeaderD.aTitleName.Add(_T("X-DIR") + strUnit);
	TableD.HeaderD.aTitleName.Add(_T("Y-DIR") + strUnit);

	MergeHeadRow(TableD, 2, 1, 2);
	MergeHeadRow(TableD, 1, 1, 2);
	MergeHeadCol(TableD, 3, 1, 2);
	nRow = 0;
	for (int i=0; i<aKey.GetCount(); i++)
	{
		T_KEY nKey = aKey[i];        
		if(nKey ==0 || m_pDoc->m_pAttrCtrl->GetStor(nKey, StorData) == FALSE)
			continue;
		TableD.RecordD.aRecordData[nRow].Add(StorData.StoryName);
		TableD.RecordD.aRecordData[nRow].Add(Length(StorData.dStoryLevel));
		TableD.RecordD.aRecordData[nRow].Add(Force (RLSMSS[i][0], FALSE, 4));
		TableD.RecordD.aRecordData[nRow].Add(Force (RLSMSS[i][1], FALSE, 4));
		nRow += 1;
	}
	delete []DIAPMS;
	delete []DIAPXY;
	delete []RLSMSS;
	//
	TableD.RecordD.aRecordData.FreeExtra();
	TableD.RecordD.nRecordRowCount = nRow;// nRow;
	TableD.RecordD.nRecordColCount = 4;          
	MakeTable(genElements, TableD     , _T("Storey Additional Mass Table"), _T("CHARPT_3_5_TBL2"));   
	//
	CString strText  = _T("Note: The above additional masses represent masses in between two ");
	strText += _T("adjacent stories or on the nodes released from the floor rigid ");
	strText += _T("diaphragm by *Diaphragm Disconnect command. For static seismicanalysis=");
	strText += _T("the masses between two adjacent stories (ie, masses on columns, braces & ");
	strText += _T("walls) are proportionally distributed to upper/lower stories based on ");
	strText += _T("their vertical locations. For dynamic analysis, however, floor masses ");
	strText += _T("and masses on vertical elements remain at their original locations.");
	MakeText(genElements, strText, _T("CHARPT_3_5_TXT2"));
}
void AutoReport_Gen_CD::WriteCharpt3_6(GenSegmentElements &genElements)
{
	MakeTitle(genElements, _T("Load Plan"), 3, 6); 
	if(Para()->SegmentIsCheck(3,6) == FALSE)
		return;
	SetPreOrPostMode(FALSE);
	SetViewPoint(3);
	DrawNodeAll(FALSE);
	T_KEY_LIST lstStorKs;
	m_pDoc->m_pAttrCtrl->GetStorKeyList(lstStorKs);
	//
	CStoryData* pStoryData = m_pDoc->GetStoryData();
	T_STOR_D StorD;
	T_KEY_LIST lstElems;
	CString strName;
	Report_Key_LIST lstCase;
	Para()->LoadCaseKeys(D_LOADCASE_STATIC, lstCase);
	int nFigIdx = 0;
	T_STLD_D StlD;
	for (int k=0; k<lstCase.GetCount(); k++)
	{
		const Report_Key &rptKey = lstCase[k];
		if(!m_pDoc->m_pAttrCtrl->GetStld(rptKey.m_nKey, StlD))
			continue;
		ShowLoad(rptKey, TRUE);
		for (int i=0; i<lstStorKs.GetCount(); i++)
		{      
			if(!m_pDoc->m_pAttrCtrl->GetStor(lstStorKs[i], StorD))
				continue;
			lstElems.RemoveAll();
			pStoryData->GetStoryElemKey(lstStorKs[i], lstElems);     
			ActiveElemAll(FALSE);
			ActiveObj(lstElems, TRUE);
			strName.Format(_T("EL %s Load Plan %s"),  Length(StorD.dStoryLevel, TRUE), StlD.LoadCaseName);
			MakeImage_Current(strName, FALSE);
			MakePicture(genElements, strName, BookMark(_T("CHARPT_3_6_FIG"), ++nFigIdx));
		} 
	}    
	ShowLoad(Report_Key(), FALSE);
}
void AutoReport_Gen_CD::WriteCharpt3_7(GenSegmentElements &genElements)
{
	MakeTitle(genElements, _T("Load Combinations  Table"), 3, 7); 
	if(Para()->SegmentIsCheck(3,7) == FALSE)
		return;
	T_TABLE_D TableD;
	TableD.Initialize();
	TableD.HeaderD.nHeaderRowCount = 1;
	TableD.HeaderD.nHeaderColCount = 5;

	TableD.HeaderD.aTitleName.Add(_T("No"));
	TableD.HeaderD.aTitleName.Add(_T("Name"));
	TableD.HeaderD.aTitleName.Add(_T("Active"));
	TableD.HeaderD.aTitleName.Add(_T("Type"));
	TableD.HeaderD.aTitleName.Add(_T("Description"));
	//
	int nLoadCmbType[] = {D_LCOMTYPE_CONCRETE, D_LCOMTYPE_STEEL};
	CString aName[] = {_T("Concrete Design Load Combinations Table"), _T("Steel Design Load Combinations Table")};
	for (int i=0; i<2; i++)
	{
		TableD.RecordD.Initialize();
		T_LCOM_D LComD;
		T_KEY_LIST aKey;
		m_pDoc->m_pAttrCtrl->GetLcomKeyList(nLoadCmbType[i], aKey);
		TableD.RecordD.aRecordData.SetSize(aKey.GetCount());
		int nRow = 0;
		for (int k=0; k<aKey.GetCount(); k++)
		{
			T_KEY nKey = aKey[k];
			if(!m_pDoc->m_pAttrCtrl->GetLcom(nLoadCmbType[i], nKey, LComD))
				continue;
			TableD.RecordD.aRecordData[nRow].Add(ToString(LComD.LoadCombId));
			TableD.RecordD.aRecordData[nRow].Add(LComD.LoadCombName);
			TableD.RecordD.aRecordData[nRow].Add(LoadCombActiveDes(nLoadCmbType[i], LComD.nActive));
			TableD.RecordD.aRecordData[nRow].Add(LoadCombTypeDes  (nLoadCmbType[i], LComD.LoadCombType));
			TableD.RecordD.aRecordData[nRow].Add(LComD.Description);
			nRow += 1;
		}
		TableD.RecordD.aRecordData.FreeExtra();
		TableD.RecordD.nRecordRowCount = nRow;
		TableD.RecordD.nRecordColCount = 5;          
		MakeTable(genElements, TableD, _T("Load Combinations Table"), BookMark(_T("CHARPT_3_7_TBL"), i+1));   
	}
}

void AutoReport_Gen_CD::WriteCharpt4  ()
{
	GenSegmentElements Elems;
	MakeTitle(Elems, _T("ANALYSIS RESULT"), 4, 0);
	WriteCharpt4_1(Elems);
	WriteCharpt4_2(Elems);
	WriteCharpt4_3(Elems);
	WriteCharpt4_4(Elems);
	WriteCharpt4_5(Elems);
	m_Segments.push_back(Elems);
}
void AutoReport_Gen_CD::WriteCharpt4_1(GenSegmentElements &genElements)
{
	MakeTitle(genElements, _T("Eigenvalue Mode"), 4, 1); 
	if(Para()->SegmentIsCheck(4,1) == FALSE)
		return;
	{
		T_TABLE_D TableD;
		TableD.Initialize();
		TableD.HeaderD.nHeaderRowCount =  2;
		TableD.HeaderD.nHeaderColCount = 13;
		TableD.HeaderD.aTitleName.Add(_T("Mode No"));
		TableD.HeaderD.aTitleName.Add(_T("TRAN-X"));
		TableD.HeaderD.aTitleName.Add(_T("TRAN-Y"));
		TableD.HeaderD.aTitleName.Add(_T("TRAN-Z"));
		TableD.HeaderD.aTitleName.Add(_T("ROTN-X"));
		TableD.HeaderD.aTitleName.Add(_T("ROTN-Y"));
		TableD.HeaderD.aTitleName.Add(_T("ROTN-Z"));
		MergeHeadRow(TableD, 1, 1, 2);
		for (int i=0; i<6; i++)
		{
			TableD.HeaderD.aTitleName.Add(_T("MASS(%)"));
			TableD.HeaderD.aTitleName.Add(_T("SUM(%)"));
			MergeHeadCol(TableD, i*2+2, 1, 2);
		}      
		//   
		int nModeNum = m_pDoc->m_pPostCtrl->GetNumEigvPost();
		TableD.RecordD.aRecordData.SetSize(nModeNum);

		T_MPAM_D data;
		for (int i = 0; i < nModeNum; i++)
		{
			m_pDoc->m_pPostCtrl->GetEiMpam(i+1, data);
			TableD.RecordD.aRecordData[i].Add(ToString(i+1, 0));
			for (int j = 0; j < 6; j++)
			{
				TableD.RecordD.aRecordData[i].Add(ToString(data.dblMpam[j], 4));
				TableD.RecordD.aRecordData[i].Add(ToString(data.dblMpamSum[j], 4));
			}
		}        
		//
		TableD.RecordD.aRecordData.FreeExtra();
		TableD.RecordD.nRecordRowCount = nModeNum;
		TableD.RecordD.nRecordColCount = 13;
		MakeTable(genElements, TableD     , _T("MODAL PARTICIPATION RATIO"), _T("CHARPT_4_1_TBL1"));  
	}
	{
		T_TABLE_D TableD;
		TableD.Initialize();
		TableD.HeaderD.nHeaderRowCount =  2;
		TableD.HeaderD.nHeaderColCount = 13;
		TableD.HeaderD.aTitleName.Add(_T("Mode No"));
		TableD.HeaderD.aTitleName.Add(_T("TRAN-X"));
		TableD.HeaderD.aTitleName.Add(_T("TRAN-Y"));
		TableD.HeaderD.aTitleName.Add(_T("TRAN-Z"));
		TableD.HeaderD.aTitleName.Add(_T("ROTN-X"));
		TableD.HeaderD.aTitleName.Add(_T("ROTN-Y"));
		TableD.HeaderD.aTitleName.Add(_T("ROTN-Z"));
		MergeHeadRow(TableD, 1, 1, 2);
		for (int i=0; i<6; i++)
		{
			TableD.HeaderD.aTitleName.Add(_T("MASS"));
			TableD.HeaderD.aTitleName.Add(_T("SUM"));
			MergeHeadCol(TableD, i*2+2, 1, 2);
		}      
		//   
		int nModeNum = m_pDoc->m_pPostCtrl->GetNumEigvPost();
		TableD.RecordD.aRecordData.SetSize(nModeNum);

		T_EMMR_D data;
		for (int i = 0; i < nModeNum; i++)
		{
			m_pDoc->m_pPostCtrl->GetEiEmmr(i+1, data);
			TableD.RecordD.aRecordData[i].Add(ToString(i+1, 0));
			for (int j = 0; j < 6; j++)
			{
				TableD.RecordD.aRecordData[i].Add(ToString(data.dblEmmrMass[j], 4));
				TableD.RecordD.aRecordData[i].Add(ToString(data.dblEmmrMassSum[j], 4));
			}
		}        
		//
		TableD.RecordD.aRecordData.FreeExtra();
		TableD.RecordD.nRecordRowCount = nModeNum;
		TableD.RecordD.nRecordColCount = 13;
		MakeTable(genElements, TableD     , _T("MODAL PARTICIPATION VALUE"), _T("CHARPT_4_1_TBL2"));  
	}
	{
		T_TABLE_D TableD;
		TableD.Initialize();
		TableD.HeaderD.nHeaderRowCount =  2;
		TableD.HeaderD.nHeaderColCount =  7;
		TableD.HeaderD.aTitleName.Add(_T("Mode No"));
		TableD.HeaderD.aTitleName.Add(_T("TRAN-X"));
		TableD.HeaderD.aTitleName.Add(_T("TRAN-Y"));
		TableD.HeaderD.aTitleName.Add(_T("TRAN-Z"));
		TableD.HeaderD.aTitleName.Add(_T("ROTN-X"));
		TableD.HeaderD.aTitleName.Add(_T("ROTN-Y"));
		TableD.HeaderD.aTitleName.Add(_T("ROTN-Z"));
		MergeHeadRow(TableD, 1, 1, 2);    
		for (int i=0; i<6; i++)
			TableD.HeaderD.aTitleName.Add(_T("Value"));        
		//   
		int nModeNum = m_pDoc->m_pPostCtrl->GetNumEigvPost();
		TableD.RecordD.aRecordData.SetSize(nModeNum);
		T_MPAF_D data;
		for (int i = 0; i < nModeNum; i++)
		{
			m_pDoc->m_pPostCtrl->GetEiMpaf(i+1, data);
			TableD.RecordD.aRecordData[i].Add(ToString(i+1, 0));
			//
			double dFactor=1.0;
			for (int j = 0; j < 6; j++)
				TableD.RecordD.aRecordData[i].Add(ToString(data.dblMpaf[j]*dFactor, 4));            
		}        
		//
		TableD.RecordD.aRecordData.FreeExtra();
		TableD.RecordD.nRecordRowCount = nModeNum;
		TableD.RecordD.nRecordColCount = 7;
		MakeTable(genElements, TableD     , _T("MODAL PARTICIPATION FACTOR"), _T("CHARPT_4_1_TBL3"));  
	}
	{
		T_TABLE_D TableD;
		TableD.Initialize();
		TableD.HeaderD.nHeaderRowCount =  2;
		TableD.HeaderD.nHeaderColCount =  7;
		TableD.HeaderD.aTitleName.Add(_T("Mode No"));
		TableD.HeaderD.aTitleName.Add(_T("TRAN-X"));
		TableD.HeaderD.aTitleName.Add(_T("TRAN-Y"));
		TableD.HeaderD.aTitleName.Add(_T("TRAN-Z"));
		TableD.HeaderD.aTitleName.Add(_T("ROTN-X"));
		TableD.HeaderD.aTitleName.Add(_T("ROTN-Y"));
		TableD.HeaderD.aTitleName.Add(_T("ROTN-Z"));
		MergeHeadRow(TableD, 1, 1, 2);    
		for (int i=0; i<6; i++)
			TableD.HeaderD.aTitleName.Add(_T("Value"));        
		//   
		int nModeNum = m_pDoc->m_pPostCtrl->GetNumEigvPost();
		TableD.RecordD.aRecordData.SetSize(nModeNum);
		T_MDRF_D data;
		for (int i = 0; i < nModeNum; i++)
		{
			m_pDoc->m_pPostCtrl->GetEiMdrf(i+1, data);
			TableD.RecordD.aRecordData[i].Add(ToString(i+1, 0));
			//
			for (int j = 0; j < 6; j++)
				TableD.RecordD.aRecordData[i].Add(ToString(data.dblMdrf[j], 4));            
		}        
		//
		TableD.RecordD.aRecordData.FreeExtra();
		TableD.RecordD.nRecordRowCount = nModeNum;
		TableD.RecordD.nRecordColCount = 7;
		MakeTable(genElements, TableD     , _T("MODAL DIRECTION FACTOR"), _T("CHARPT_4_1_TBL4"));  
	}
}
void AutoReport_Gen_CD::WriteCharpt4_2(GenSegmentElements &genElements)
{
	MakeTitle(genElements, _T("Drift"), 4, 2); 
	if(Para()->SegmentIsCheck(4,2) == FALSE)
		return;
	//
	T_TABLE_D TableD;
	TableD.Initialize();
	TableD.HeaderD.nHeaderRowCount =  2;
	TableD.HeaderD.nHeaderColCount = 15;
	TableD.HeaderD.aTitleName.Add(_T("Load Case"));
	TableD.HeaderD.aTitleName.Add(_T("Story"));
	TableD.HeaderD.aTitleName.Add(_T("Story\nHeight\n")+UnitDes(D_UNITSYS_BASE_LENGTH, TRUE));
	TableD.HeaderD.aTitleName.Add(_T("P-Delta\nIncremental\nFactor(ad)"));
	TableD.HeaderD.aTitleName.Add(_T("Allowable\nStory Drift\nRatio"));
	TableD.HeaderD.aTitleName.Add(_T("Drift at the Center of Mass"));
	//
	TableD.HeaderD.aTitleName.Add(_T("Node"));
	TableD.HeaderD.aTitleName.Add(_T("Story\nDrift\n")    +UnitDes(D_UNITSYS_BASE_LENGTH, TRUE));
	TableD.HeaderD.aTitleName.Add(_T("Modified\nDrift\n") +UnitDes(D_UNITSYS_BASE_LENGTH, TRUE));
	TableD.HeaderD.aTitleName.Add(_T("Story\nDrift\nRatio"));
	TableD.HeaderD.aTitleName.Add(_T("Remark"));
	//
	TableD.HeaderD.aTitleName.Add(_T("Story\nDrift\n")    +UnitDes(D_UNITSYS_BASE_LENGTH, TRUE));
	TableD.HeaderD.aTitleName.Add(_T("Modified\nDrift\n") +UnitDes(D_UNITSYS_BASE_LENGTH, TRUE));
	TableD.HeaderD.aTitleName.Add(_T("Drift Factor\n(Max/Current)"));
	TableD.HeaderD.aTitleName.Add(_T("Story\nDrift\nRatio"));
	TableD.HeaderD.aTitleName.Add(_T("Remark"));
	//
	for (int i=0; i<5; i++)
		MergeHeadRow(TableD, i+1, 1, 2);  
	MergeHeadCol(TableD, 11, 1, 5);
	MergeHeadCol(TableD,  6, 1, 5);
	//
	CString aName[] = {_T("Drift(X)"), _T("Drift(Y)"), _T("Drift(Combin)")};
	for (int i=0; i<3; i++)
	{
		MakeTBLRecord_4_2(i, TableD);
		//
		MakeTable(genElements, TableD,  aName[i], BookMark(_T("CHARPT_4_2_TBL"), i+1));   
	}
}
void AutoReport_Gen_CD::WriteCharpt4_3(GenSegmentElements &genElements)
{
	MakeTitle(genElements, _T("Beam Force Result"), 4, 3); 
	if(Para()->SegmentIsCheck(4,3) == FALSE)
		return;
	CloseCurDlgbar();
	DrawNodeAll(FALSE);
	Report_Key_LIST lstCases;
	Para()->AllCaseKeys(lstCases);
	Report_Key_LIST lstOption;
	Para()->ForceOption(lstOption);
	T_LCOM_D LComD;
	int nRealIdx = 0;
	CString strName;
	CString aOptionName[] = {_T("Fx"), _T("Fy"), _T("Fz"), _T("Mx"), _T("My"), _T("Mz")};
	for (int i=0; i<lstCases.GetCount(); i++)
	{
		if(!MakeLoadComb(lstCases[i], LComD))
			continue;
		for (int k=0; k<lstOption.GetCount(); k++)
		{
			if(lstOption[k].m_nKey > 6)
				continue;
			if(!CallBeamForceFig(lstCases[i], lstOption[k].m_nKey))
				continue;
			strName.Format(_T("%s[%s]"), LComD.LoadCombName, aOptionName[lstOption[k].m_nKey]);
			MakePic_4_3_Flor(genElements, nRealIdx, strName);
			MakePic_4_3_Grup(genElements, nRealIdx, strName);
		}
	}
	CloseCurDlgbar();
}
void AutoReport_Gen_CD::WriteCharpt4_4(GenSegmentElements &genElements)
{
	MakeTitle(genElements, _T("RC Beam Design Result"), 4, 4); 
	if(Para()->SegmentIsCheck(4,4) == FALSE)
		return;
	SetViewPoint(3);
	DrawNodeAll(FALSE);
	T_KEY_LIST lstStorKs;
	m_pDoc->m_pAttrCtrl->GetStorKeyList(lstStorKs);
	//
	CloseCurDlgbar();
	if(!CallRebarRltFig())
		return;
	CStoryData* pStoryData = m_pDoc->GetStoryData();
	T_STOR_D StorD;
	T_KEY_LIST lstElems;
	CString strName;
	for (int i=0; i<lstStorKs.GetCount(); i++)
	{      
		if(!m_pDoc->m_pAttrCtrl->GetStor(lstStorKs[i], StorD))
			continue;
		lstElems.RemoveAll();
		pStoryData->GetStoryElemKey(lstStorKs[i], lstElems);
		if(FilteElemByMembType(lstElems, D_MBTP_BEAM) < 1)
			continue;        
		ActiveElemAll(FALSE);
		ActiveObj(lstElems, TRUE);
		strName.Format(_T("EL %s RC Column Design Result"), Length(StorD.dStoryLevel, TRUE));
		MakeImage_Current(strName, FALSE);
		MakePicture(genElements, strName, BookMark(_T("CHARPT_4_5_FIG"), i+1));
	}
	CloseCurDlgbar();
}
void AutoReport_Gen_CD::WriteCharpt4_5(GenSegmentElements &genElements)
{
	MakeTitle(genElements, _T("RC Column Design Result"), 4, 5); 
	if(Para()->SegmentIsCheck(4,5) == FALSE)
		return;
	SetViewPoint(3);
	DrawNodeAll(FALSE);
	T_KEY_LIST lstStorKs;
	m_pDoc->m_pAttrCtrl->GetStorKeyList(lstStorKs);
	//
	CloseCurDlgbar();
	if(!CallRebarRltFig())
		return;
	CStoryData* pStoryData = m_pDoc->GetStoryData();
	T_STOR_D StorD;
	T_KEY_LIST lstElems;
	CString strName;
	for (int i=0; i<lstStorKs.GetCount(); i++)
	{      
		if(!m_pDoc->m_pAttrCtrl->GetStor(lstStorKs[i], StorD))
			continue;
		lstElems.RemoveAll();
		pStoryData->GetStoryElemKey(lstStorKs[i], lstElems);
		if(FilteElemByMembType(lstElems, D_MBTP_COLUMN) < 1)
			continue;        
		ActiveElemAll(FALSE);
		ActiveObj(lstElems, TRUE);
		strName.Format(_T("EL %s RC Column Design Result"), Length(StorD.dStoryLevel, TRUE));
		MakeImage_Current(strName, FALSE);
		MakePicture(genElements, strName, BookMark(_T("CHARPT_4_5_FIG"), i+1));
	}
	CloseCurDlgbar();
}