// CRCExportFile.h: interface for the CCRCExportFile class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CRCEXPORTFILE_H__689AD82E_4F2A_443C_9055_A73F0C21B516__INCLUDED_)
#define AFX_CRCEXPORTFILE_H__689AD82E_4F2A_443C_9055_A73F0C21B516__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CRCFileBase.h"
#include "CRCDataCtrl.h"
#include "..\wg_db\wg_db_StagCtrl.h"

struct _CRC_DEFNUM
{
	int iElemNum;
	int iNodeNum;
	int iStagNum;

	int iNodeGrNum; //절점그룹의 갯수
	int iElemGrNum; //요소그룹의 갯수
	int iTsecGrNum;
	//Boundary Group
	int iConsGrNum;
	int iGsprGrNum;
	int iNsprGrNum;
	int iRigdGrNum;
	int iGlnkGrNum;
	int iElnkGrNum;
	//Load Group
	int iCndlGrNum;
	int iBmldGrNum;
	int iPrstGrNum;
	int iEtmpGrNum;
	int iGtmpGrNum;
	int iBtmpGrNum;
	int iSdspGrNum;

	int iMatNum;
	int iTndnMatNum;
	int iStlMatNum;
	int iSkewNum;

	int iTdnaNum; //텐던갯수
	int iTPntNum; //사용자 입력 최대 텐던변화점 수
	int iSectNum; //단면갯수
	int iBarLayNum; //축력방향철근 최대단수
	int iCellNum; //최대 셀갯수(외측포함)
	int iOutPontNum; //바깥쪽최대 꺽인점 수
	int iInPontNum; //안쪽최대 꺽인점 수

	int iMdgrNum;// Main Girder
	int iLdlnNum;// Load Line
	int iCbemNum;// Cross Beam
	int iCpotNum;// Cross Point


	int iLcaseNum; //Non Concurrent Load case Num
	int iCLcaseNum; //Concurrent Load Case Num
	int iSLcaseNum;
	int iLcomNumDgn;	// Load Combination Num for Design.
	int iLcomNum;

	int iDeadNum;
	int iODeadNum;
	int iVhleNum;
	int iCrwdNum;
	int iLiveNum;
	int iP1ltNum;

	int iStagElemNum;
	int iStagBndrNum;
	int iStagLoadNum;

	int iPbldGrNum; //Prestress Beam Load
	int iPtnsGrNum; //Pretension Load
	int iStmpGrNum; //System Temperature
	int iNtmpGrNum; //Nodal Temperature
	int iSmptGrNum; //Settlement Analysis
	int iEBtmpNum;  //1st Force Num By Beam Section Temperature
	int iSecfGrNum;
	int iCglpGrNum; //Change General Link Property Group
	void Initialize()
	{
		iElemNum=0;
		iNodeNum=0;
		iStagNum=0;

		iNodeGrNum=0;
		iElemGrNum=0;
		iTsecGrNum=0;
		iConsGrNum=0;
		iGsprGrNum=0;
		iNsprGrNum=0;
		iRigdGrNum=0;
		iGlnkGrNum=0;
		iElnkGrNum=0;
		iCndlGrNum=0;
		iBmldGrNum=0;
		iPrstGrNum=0;
		iEtmpGrNum=0;
		iGtmpGrNum=0;
		iBtmpGrNum=0;
		iSdspGrNum=0;

		iMatNum=0;
		iTndnMatNum=0;
		iStlMatNum=0;
		iSkewNum=0;

		iTdnaNum=0;
		iTPntNum=0;
		iSectNum=0;
		iBarLayNum=0;
		iCellNum=0;
		iOutPontNum=0;
		iInPontNum=0;

		iMdgrNum=0;
		iLdlnNum=0;
		iCbemNum=0;
		iCpotNum=0;

		iLcaseNum=0;
		iCLcaseNum=0;
		iSLcaseNum=0;
		iLcomNumDgn=0;
		iLcomNum=0;

		iDeadNum=0;
		iODeadNum=0;
		iVhleNum=0;
		iCrwdNum=0;
		iLiveNum=0;
		iP1ltNum=0;

		iStagElemNum=0;
		iStagBndrNum=0;
		iStagLoadNum=0;

		iPbldGrNum = 0;
		iPtnsGrNum = 0;
		iStmpGrNum = 0;
		iNtmpGrNum = 0;
		iSmptGrNum = 0;

		iEBtmpNum = 0;
		iSecfGrNum = 0;

		iCglpGrNum = 0;
	}
};

struct _CRC_EXPD
{
	CArray<T_SECT_K, T_SECT_K> PSectKList;
	CArray<T_MATL_K, T_MATL_K> MatlKList;
	CArray<T_ELEM_K, T_ELEM_K> PElemKList;
	CArray<T_SGLD_K, T_SGLD_K> SgldKList;
	CArray<T_SGLD_D, T_SGLD_D&> SgldDList;
	CMap<T_ELEM_K, T_ELEM_K, T_SECT_K, T_SECT_K> PSectKMap;
	CMap<T_ELEM_K, T_ELEM_K, T_MATL_K, T_MATL_K> MatlKMap;

	T_STAG_D StagD;
	
	void Initialize()
	{
		PSectKList.RemoveAll();
		MatlKList.RemoveAll();
		PElemKList.RemoveAll();
		SgldKList.RemoveAll();
		SgldDList.RemoveAll();
		StagD.Initialize();
		PSectKMap.RemoveAll();
		MatlKMap.RemoveAll();
	}

	_CRC_EXPD& operator=(_CRC_EXPD& src)
	{
		PSectKList.Copy(src.PSectKList);
		MatlKList.Copy(src.MatlKList);
		PElemKList.Copy(src.PElemKList);
		SgldKList.Copy(src.SgldKList);
		SgldDList.Copy(src.SgldDList);
		StagD = src.StagD;

		T_ELEM_K ElemK;
		T_SECT_K SectK;
	  POSITION Pos = src.PSectKMap.GetStartPosition();
	  while(Pos)
	  {
		  SectK = 0;
		  src.PSectKMap.GetNextAssoc(Pos, ElemK, SectK);
			PSectKMap.SetAt(ElemK, SectK);
		} 

		T_MATL_K MatlK;
	  Pos = src.MatlKMap.GetStartPosition();
	  while(Pos)
	  {
		  MatlK = 0;
		  src.MatlKMap.GetNextAssoc(Pos, MatlK, MatlK);
			MatlKMap.SetAt(ElemK, MatlK);
		} 

		return *this;
	}

	_CRC_EXPD(_CRC_EXPD& src)
	{
		*this = src;
	}

	_CRC_EXPD()
	{
	}
};

class CCRCExportFile : public CCRCFileBase
{
public:
	CCRCExportFile();
	virtual ~CCRCExportFile();

public:
	BOOL m_bDeg; 

protected:
	_CRC_DEFNUM m_DataNum;

	BOOL m_bStag;
	_CRC_EXPD m_PSCBeamBaseD;
	_CRC_EXPD m_PSCBeamPostD;

	_CRC_EXPD m_ExtElemBaseD;
	_CRC_EXPD m_ExtElemPostD;

	CMap<T_STAG_K, T_STAG_K, _CRC_EXPD, _CRC_EXPD&> m_PSCBeamStagDMap;
	CMap<T_STAG_K, T_STAG_K, _CRC_EXPD, _CRC_EXPD&> m_ExtElemStagDMap;
	CArray<T_STAG_K, T_STAG_K> m_StagKList;
	CCRCDataCtrl m_DataCtrl;
	int m_iBrgDir;	// 0=종방향, 1=횡방향.
	BOOL m_bTestMode; // Coded by Seungjun for Test ('20060719).

public:

	BOOL Export_File();
	BOOL MakeStageData();
	void MakeDataSize();

	//전체 Export
	BOOL Write_Data();
	BOOL Write_Title();
	BOOL Write_Version();
	BOOL Write_Unit();
	BOOL Write_DataDefNum();
	BOOL Write_MatlData();
	BOOL Write_CpShData();
	BOOL Write_TdmeData();
	BOOL Write_SectData();
	BOOL Write_StageData();
	BOOL Write_MvldData(BOOL bWrite);
	BOOL Write_SseiData();
	BOOL Write_TndnData();
	BOOL Write_FinalStagData();
	BOOL Write_FinalData();
	BOOL Write_AvgTndnData();
	BOOL Write_SecPropData();
	BOOL Write_ShearData();
	BOOL Write_StgForceData();
	BOOL Write_StgCpShData();
	BOOL Write_ForceData();
	BOOL Write_StressData();
	BOOL Write_BtssForceData();
	BOOL Write_LcomForceData();
	BOOL Write_SectType(T_SECT_K SectK, T_SECT_D& SectD);
	BOOL Write_SectSize(T_SECT_K SectK, T_SECT_D& SectD, BOOL bSecI);
	BOOL Write_SectCoord(T_SECT_K SectK, T_SECT_D& SectD, BOOL bPosI);

	BOOL Write_FinalLocalAxis();
	BOOL Write_FinalNodeData();
	BOOL Write_FinalElemData();
	BOOL Write_FinalInfluenceLine();
	BOOL Write_FinalLoadLine();
	BOOL Write_SpanData();

	BOOL Write_GrupData(BOOL bWrite);
	BOOL Write_StagInfo(T_STAG_K StagK, T_STAG_D& StagD);
	BOOL Write_ActiveElemGrup(T_STAG_K StagK, T_STAG_D& StagD);
	BOOL Write_InactiveElemGrup(T_STAG_K StagK, T_STAG_D& StagD);
	BOOL Write_ActiveSprtGrup(T_STAG_K StagK, T_STAG_D& StagD);
	BOOL Write_InactiveSprtGrup(T_STAG_K StagK, T_STAG_D& StagD);
	BOOL Write_ActiveLoadGrup(T_STAG_K StagK, T_STAG_D& StagD);
	BOOL Write_InactiveLoadGrup(T_STAG_K StagK, T_STAG_D& StagD);

	BOOL Write_NodeElemGrup(BOOL bWrite);
	BOOL Write_BngrGrup(BOOL bWrite);
	BOOL Write_LoadGrup(BOOL bWrite);


	BOOL Write_TndnGrupData();
	BOOL Write_TndnProfileData();
	BOOL Write_TndnForceData();

	BOOL Get_LastStepInCurStage(T_STAG_K StagK, int& iFinalStepNo);

	//Design Export
public:
	BOOL Write_Dgn_Data();
	BOOL Write_Dgn_AnalData();
	BOOL Write_Dgn_CtrlData();
	BOOL Write_Dgn_ExtTndnData();
	BOOL Write_Dgn_ChkPosData();
	BOOL Write_Dgn_MomReduData();
	BOOL Write_Dgn_LComData();
	BOOL Write_Dgn_RBarData();
	BOOL Write_Dgn_SBarData();
	BOOL Write_Dgn_SheetData();

	//void Get_CrcDataCtrl(CCRCDataCtrl& DataCtrl) { DataCtrl = m_DataCtrl; }

};

#endif // !defined(AFX_CRCEXPORTFILE_H__689AD82E_4F2A_443C_9055_A73F0C21B516__INCLUDED_)
