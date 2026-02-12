#pragma once

/////////////////////////////////////////////////////////////////////////////
// CCMIehcDlg 

#include "..\wg_cmd\MyChildDialog.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include <vector>
#include <string>
#include "..\wg_main\wg_mainres2.h"
#include "..\wg_cmd\wg_cmdRes.h"
#include "..\wg_gps\wg_gpsRes.h"
#include "HeaderPre.h"

class __MY_EXT_CLASS__ CCMSeachTestDlg
{
public:
	CCMSeachTestDlg(CWnd* pParent = NULL);   // standard constructor

public:
	//CDBDoc* m_pDoc;
	CEdit 	m_wndEdit;
	//int m_nResultNum;
	std::map<std::string, UINT> m_strToint;

	//T_IEHC_D m_IehcD;

	virtual std::vector<std::string> split(const std::string& str, char delimiter);
	virtual std::vector<std::string> search_al(CString input_w, int limit = 10);
	virtual BOOL CheckEnable(std::string inWords);
	virtual std::string CCMSeachTestDlg::img_path(std::string input_w);
	void ExecuteDlg(std::string inWords);
	void ChangeMemb(std::vector<std::string>* p_Str, std::vector<UINT>* p_Id, std::vector<std::string>* p_ImgPath);
	std::string strip(const std::string& word);
	std::vector<std::string> str_vector ={};

	std::vector<UINT> id_vector ={};

#if defined(_MEC)
	std::string oriPath = "C:/MIDAS/genw_Websearch2/bin/MecCivilDebug/x64/SVG/RibbonMenu";
#else
	std::string oriPath = "C:/MIDAS/genw_Websearch2/bin/CivilDebug/x64/SVG/RibbonMenu";
#endif


	std::vector<std::string> img_path_vector = {};

	// Generated message map functions
	//{{AFX_MSG(CCMIehcDlg)

	//}}AFX_MSG


};
#include "HeaderPost.h"

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

