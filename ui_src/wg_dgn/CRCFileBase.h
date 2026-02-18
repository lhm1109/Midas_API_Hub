#pragma once

class CDBDoc;

class CCRCFileBase
{
public:
	CCRCFileBase();
	virtual ~CCRCFileBase();

// Member Variables.
protected:
	CString m_strFileNa;
	CDBDoc* m_pDoc;
	CStdioFile m_cfile;
	int m_iRecordCount;
	BOOL m_bFirstCommand;
	BOOL m_bLineLimit;

public:
	void Set_FileName(CString strFileName);
	CString Get_FileName();
	int GetInt(double dValue);
	BOOL Write_Double(double dValue, BOOL bEmpty=FALSE);
	BOOL Write_NullDouble();
	BOOL Write_Int(int iValue, BOOL bEmpty=FALSE);
	BOOL Write_NullInt();
	BOOL Write_String(CString strValue, BOOL bEmpty=FALSE);
	BOOL Write_String8(CString strValue, BOOL bEmpty=FALSE);
	BOOL Write_String8(unsigned char cValue, BOOL bEmpty=FALSE);
	BOOL Write_NullString();
	void Write_Command(CString strValue, BOOL bContinue=FALSE);
	void Write_Project(CString strValue);
	void Write_Company(CString strValue);
	BOOL Write_Enable(int iNum);
	void Write_NullLine();
	
};