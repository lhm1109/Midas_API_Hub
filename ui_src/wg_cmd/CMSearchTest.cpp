#include "stdafx.h"
#include "wg_cmd.h"
#include "CMSearchTest.h"
#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_base\wg_base_I_GENModelBase.h"
#include "..\wg_base\TestEnvMgr.h"
#include "..\wg_main\wg_mainres2.h"
#include "..\wg_cmd\wg_cmdRes.h"
#include "..\wg_gps\wg_gpsRes.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_InitCtrl.h"
#include "..\wg_db\wg_db_ViewBuff.h"
#include "..\wg_db\wg_db_MatlDB.h"
#include "..\wg_db\DBCodeDef.h"
#include "..\wg_db\DB_ST_DT.h"

#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <algorithm>
#include <sstream>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CCMSeachTestDlg::CCMSeachTestDlg(CWnd* pParent /*=NULL*/)
{
	//{{AFX_DATA_INIT(CCMFiberMatPropItemDlg)	
	//}}AFX_DATA_INIT
	//m_pDoc = CDBDoc::GetDocPoint();
	//m_nResultNum = 10;
}


vector<std::string> CCMSeachTestDlg::split(const std::string& str, char delimiter)
{
	vector<std::string> tokens;
	string token;
	istringstream token_stream(str);

	while (std::getline(token_stream, token, delimiter))
	{
		tokens.push_back(token);
	}
	return tokens;
}

std::string CCMSeachTestDlg::strip(const std::string& word)
{
	const std::string whitespace = " \t\n\r\f\v";
	int start = word.find_first_not_of(whitespace);
	if (start == std::string::npos)
	{
		return "";
	}
	int last = word.find_last_not_of(whitespace);
	if (word[start] == '&')
	{
		return word.substr(start+1, last - start + 1);
	}
	else
	{
		return word.substr(start, last - start + 1);
	}
}

vector<string> CCMSeachTestDlg::search_al(CString input_w, int limit)
{
	string inStr;
	inStr = string(CT2CA(input_w));
	std::transform(inStr.begin(), inStr.end(), inStr.begin(), ::tolower);
	vector<string> input_word_list = split(inStr, ' ');

	map<string, int> mResult;

	if (input_word_list.size() >= 2)
	{
		for (int i = 0; i < str_vector.size(); i++)
		{
			// 1. 검색 키워드 전체가 들어가 있을 경우...
			string tStr = str_vector[i];
			std::transform(tStr.begin(), tStr.end(), tStr.begin(), ::tolower);
			if (tStr.find(inStr) != string::npos)
			{
				mResult[str_vector[i]] = (input_word_list.size())*2;
				continue;
			}

			// 2. 검색 키워드를 공백 단위로 쪼개고 카테고리에 일치하는게 있는지 체크
			vector<string> cri_list = split(tStr, ' ');
			for (int j = 0; j < cri_list.size(); j++)
			{
				int nCnt = 0;
				for (int k = 0; k < input_word_list.size(); k++)
				{
					if (input_word_list[k].length() >= 3)
					{
						int cri_max = min(input_word_list[k].length(), 21);
						for (int l = 3; l < cri_max; l++)
						{
							string sub_tStr = input_word_list[k].substr(0, l);
							if (cri_list[j].length() >= sub_tStr.length() && cri_list[j].substr(0, sub_tStr.length()) == sub_tStr)
							{
								nCnt += 1;
							}
						}
					}
					
					if (cri_list[j].length() >= input_word_list[k].length() && cri_list[j].substr(0, input_word_list[k].length()) == input_word_list[k])
					{
						nCnt += 2;
					}
				}
				if (nCnt >= 1)
				{
					mResult[str_vector[i]] = nCnt;
				}
			}

		}
	}
	else
	{
		for (int i = 0; i < str_vector.size(); i++)
		{
			string tStr = str_vector[i];
			std::transform(tStr.begin(), tStr.end(), tStr.begin(), ::tolower);
			// 2. 검색 키워드를 공백 단위로 쪼개고 카테고리에 일치하는게 있는지 체크
			vector<string> cri_list = split(tStr, ' ');
			for (int j = 0; j < cri_list.size(); j++)
			{
				int nCnt = 0;
				for (int k = 0; k < input_word_list.size(); k++)
				{
					if (input_word_list[k].length() >= 3)
					{
						int cri_max = min(input_word_list[k].length(), 21);
						for (int l = 3; l < cri_max; l++)
						{
							string sub_tStr = input_word_list[k].substr(0, l);
							if (cri_list[j].length() >= sub_tStr.length() && cri_list[j].substr(0, sub_tStr.length()) == sub_tStr)
							{
								nCnt += 1;
							}
						}
					}

					if (cri_list[j].length() >= input_word_list[k].length() && cri_list[j].substr(0, input_word_list[k].length()) == input_word_list[k])
					{
						nCnt += 2;
					}
				}
				if (nCnt >= 1)
				{
					mResult[str_vector[i]] = nCnt;
				}
			}
		}
	}
	vector<pair<string, int>> sorted_mResult(mResult.begin(), mResult.end());
	std::sort(sorted_mResult.begin(), sorted_mResult.end(), [](const auto& a, const auto&b) {return a.second > b.second; });
	int cnt = 0;

	vector<string> v_final;
	for (const auto& entry : sorted_mResult)
	{
		
		if (CheckEnable(entry.first))
		{
			v_final.push_back(entry.first);
			cnt += 1;
		}
		if ((cnt == limit) && (limit != 0))
		{
			break;
		}
	}
	return v_final;
}

std::string CCMSeachTestDlg::img_path(std::string input_w)
{
	return img_path_vector[m_strToint[input_w]];
}

BOOL CCMSeachTestDlg::CheckEnable(string inWords)
{
	T_MENU_STATE state;
	CDBDoc* m_pDoc = CDBDoc::GetDocPoint();
	if (m_pDoc == nullptr) return FALSE;

	m_pDoc->GetMenuState(id_vector[m_strToint[inWords]], state);
	if ((state.bIncludeEnable == TRUE && state.bEnable == FALSE) == FALSE)
	{	
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

void CCMSeachTestDlg::ExecuteDlg(std::string inWords)
{
	T_MENU_STATE state;
	CDBDoc* m_pDoc = CDBDoc::GetDocPoint();
	if (m_pDoc == nullptr) return;

	m_pDoc->GetMenuState(id_vector[m_strToint[inWords]], state);
	if ((state.bIncludeEnable == TRUE && state.bEnable == FALSE) == FALSE)
	{
		AfxGetMainWnd()->SendMessage(WM_COMMAND, MAKEWPARAM(id_vector[m_strToint[inWords]], 0));
	}
}


void CCMSeachTestDlg::ChangeMemb(std::vector<std::string>* p_Str, std::vector<UINT>* p_Id, std::vector<std::string>* p_ImgPath)
{
	str_vector.clear();
	id_vector.clear();
	img_path_vector.clear();

	if (p_Str->size() != p_Id->size() || p_Str->size() != p_ImgPath->size())
	{
		ASSERT(0);
	}

	for (int i = 0; i < p_Str->size(); i++)
	{	
		std::string temp = strip((*p_Str)[i]);
		str_vector.push_back(temp);
		id_vector.push_back((*p_Id)[i]);
		img_path_vector.push_back((*p_ImgPath)[i]);
	}

	// 검색 결과 개수
	m_strToint.clear();
	for (int i = 0; i < str_vector.size(); i++)
	{
		if (m_strToint.find(str_vector[i]) == m_strToint.end())
		{
 			m_strToint[str_vector[i]] = i;
		}
	}
}