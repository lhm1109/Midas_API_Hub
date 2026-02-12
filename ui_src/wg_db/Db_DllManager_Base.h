#pragma once

#include "wg_db_DBDoc.h"
#include "DB_ST_DN.h"
#include "..\dgnengine\src\DgnBase\DgnBaseShape_Struct.h"

enum DGNENGINE_NATION
{
	DGNENGINE_KR_DLL=1,
	DGNENGINE_US_DLL,
	DGNENGINE_CH_DLL,
	DGNENGINE_JP_DLL,
	DGNENGINE_RS_DLL
};

enum DGN_CODETYPE
{
	DGNE_CODETYPE_RC=1,
	DGNE_CODETYPE_STEEL,
	DGNE_CODETYPE_SRC,
	DGNE_CODETYPE_PSC,
	DGNE_CODETYPE_CSG,
	DGNE_CODETYPE_SOD,
};

#include "HeaderPre.h"

class __MY_EXT_CLASS__ CDgnDLLPathSaver
{
public:
	CDgnDLLPathSaver();
	virtual ~CDgnDLLPathSaver();

private:
	CString m_csDgnDLLPath;
};

class __MY_EXT_CLASS__ CDb_DllManager_Base
{
public:
	CDb_DllManager_Base();
	virtual ~CDb_DllManager_Base();

protected:

	CDBDoc* m_pDoc;

protected:
	HINSTANCE m_hBaseDll;
	HINSTANCE m_hCalcDll;
	HINSTANCE m_hReportDll;
	HINSTANCE m_hDrawDll;
	HINSTANCE m_hUmdDataDll;
	HINSTANCE m_hGsdDataDll;
	HINSTANCE m_hPBeamLMeshDll;
	int       m_iNationType;

public:
	void FreeDllLibrary();

	CString Get_CalcDllFileName(int iNationType);
	CString Get_ReportDllFileName(int iNationType);
	CString Get_BaseManagerDllFileName();
	CString Get_DrawDllFileName();
	CString Get_UmdDataDllFileName();
	CString Get_GSDDataDllFileName();
	CString Get_PBeamLMeshDllFileName();
protected:
	HINSTANCE GetOrLoad_CalcDllHandle(int iNationType);
	HINSTANCE GetOrLoad_ReportDllHandle(int iNationType);
	HINSTANCE GetOrLoad_BaseManagerDllHandle();
	HINSTANCE GetOrLoad_DrawDllHandle();
	HINSTANCE GetOrLoad_UmdDataDllHandle();
	HINSTANCE GetOrLoad_GsdDataDllHandle();
	//HINSTANCE GetOrLoad_PBeamLMeshDllHandle();

public:
	T_SECT_SECTBASE_D GetSectBaseD(T_SECT_D& SectD, int nIMJ);
	T_SECT_SECTBASE_D GetSectBaseD(T_SECT_D& SectD, int& nSType, int nIMJ, BOOL bAfter);
	T_SECT_SECTBASE_D GetSectBaseD_Pos(T_SECT_D& SectD, int& nSType, int nPos, BOOL bAfter);
	T_SECT_SECTBASE_D GetSectBaseD_Pos(T_SECT_D& SectD, int& nSType, double dPos, BOOL bAfter);
	T_SECT_SECTBASE_D GetSectBaseD(T_SECT_SECTBASE_D& SectI, T_SECT_SECTBASE_D& SectJ, int nIMJ);        // nIMJ(I:0,1,J:2)
	T_SECT_SECTBASE_D GetSectBaseD_Pos(T_SECT_SECTBASE_D& SectI, T_SECT_SECTBASE_D& SectJ, int nPos);    // nPos(I:0,1,2,3,J:4)
	T_SECT_SECTBASE_D GetSectBaseD_Pos(T_SECT_SECTBASE_D& SectI, T_SECT_SECTBASE_D& SectJ, double dPos); // dPos(I:0.0~J:1.0)
	T_SECT_SECTBASE_D GetSectBaseD_For_SectK(T_SECT_K SectK, int nIMJ, BOOL bDesign=TRUE);
	T_SECT_SECTBASE_D GetSectBaseD_For_ElemK(T_ELEM_K ElemK, int nIMJ, BOOL bDesign=TRUE, int MatlType=1);// MatlType(0:Auto(Conc or Steel), 1:Conc, 2:Steel)
	BOOL GetSectD_For_ElemK(T_ELEM_K ElemK, int nIMJ, T_SECT_D& SectD, BOOL bDesign=TRUE, int MatlType=1);// MatlType(0:Auto(Conc or Steel), 1:Conc, 2:Steel)
	HINSTANCE GetOrLoad_PBeamLMeshDllHandle();

	T_SECT_STIFFNESS  GetSectStiffness_Pos(T_SECT_STIFFNESS& StiffI, T_SECT_STIFFNESS& StiffJ, double dPos);
	T_SECT_DESIGN     GetSectDesign_Pos(T_SECT_DESIGN& DgnI, T_SECT_DESIGN& DgnJ, double dPos);

	BOOL Find_nStype(CString strShape, int &nStype);

	// Data Convert
	BOOL ConvertToDLL_Stiffness(T_SECT_D& SectD, int nIMJ, DGN_SECT_STIFFNESS& OutData, BOOL bDesign=TRUE, BOOL bAfter=TRUE);
	BOOL ConvertToDLL_Stiffness(T_SECT_STIFFNESS& StiffnessD, T_SECT_DESIGN& DesignD, DGN_SECT_STIFFNESS& OutData);
	BOOL ConvertToDLL_SectData(T_SECT_D& SectD, int nIMJ, DGN_GSEC_SHAPE_D& OutData, BOOL bDesign=TRUE, BOOL bAfter=TRUE);
	BOOL ConvertToDLL_SectData_CenterPos(T_SECT_D& SectD, double dPos, double& dYBar, double& dZBar);
	BOOL ConvertToDLL_SectData_Pos(T_SECT_D& SectD, int nPos, DGN_GSEC_SHAPE_D& OutData, BOOL bDesign=TRUE, BOOL bAfter=TRUE);
	BOOL ConvertToDLL_SectData_Pos(T_SECT_D& SectD, double dPos, DGN_GSEC_SHAPE_D& OutData, BOOL bDesign=TRUE, BOOL bAfter=TRUE);
	BOOL ConvertToDLL_RebarData(T_SECT_D& SectD, int nIMJ, T_RCHK_COLM& RebarD, DGN_GSEC_REBAR_COLUMN& OutData);
	BOOL ConvertToDLL_RebarData(T_SECT_D& SectD, int nIMJ, T_REBT_D& RebarD, DGN_GSEC_REBAR_COLUMN& OutData);
	BOOL ConvertToDLL_RebarData_Rebb(int nIMJ, const T_REBB_D &RebbD, DGN_GSEC_REBAR_BEAM& OutData);
	BOOL ConvertToDLL_RebarData_Rebc(const T_SECT_D& SectD, const T_REBC_D& RebcD, OUT DGN_GSEC_REBAR_COLUMN& rRbar);
	BOOL ConvertToDLL_RebarData_Rebw(double dLw, double dTw, const T_REBW_D& RebwD, OUT DGN_GSEC_REBAR_COLUMN& rRbar);
	BOOL ConvertToDLL_SectData_SODStiffener(T_SECT_STIFFNESS& Stiffness, T_SECT_SOD_STIFF_D& SODStiffenerIn, ST_SECT_SOD_STIFF_D& SODStiffenerOut);
	BOOL ConvertToDLL_SectData_Rivet(T_SECT_STIFFNESS& Stiffness, T_SECT_RIVET_STIFF_D& RivetIn, ST_SECT_RIVET_STIFF_D& RivetOut);

	BOOL ConvertToDLL_Line(T_GSEC_LINE& InData, DGN_GSEC_LINE& OutData);
	BOOL ConvertToDLL_Polygon(T_GSEC_POLYGON& InData, DGN_GSEC_POLYGON& OutData);
	BOOL ConvertToDLL_Part(T_GSEC_PART& InData, DGN_GSEC_PART& OutData);
	CString ConvertToDLL_ShapeName(CString Shape);
	int  ConvertToDLL_LengthUnitID(int UnitID);
	int  ConvertToDLL_Stype(int nStype);

	BOOL Get_DgnStlSect(int key, T_SECT_D& rData);

	BOOL IsAfterSection(T_SECT_D& SectD);

	static int GetMovingTypeToDLL();

};

#include "HeaderPost.h"
