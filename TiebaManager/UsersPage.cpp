// UsersPage.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "UsersPage.h"
#include "LoginDlg.h"
#include "Setting.h"
#include "SettingDlg.h"
#include "TiebaManagerDlg.h"


// CUsersPage �Ի���

IMPLEMENT_DYNAMIC(CUsersPage, CNormalDlg)

CUsersPage::CUsersPage(CWnd* pParent /*=NULL*/)
	: CNormalDlg(CUsersPage::IDD, pParent)
{

}

#pragma region MFC
CUsersPage::~CUsersPage()
{
}

void CUsersPage::DoDataExchange(CDataExchange* pDX)
{
	CNormalDlg::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC1, m_currentUserStatic);
	DDX_Control(pDX, IDC_LIST1, m_list);
	DDX_Control(pDX, IDC_BUTTON1, m_loginButton);
	DDX_Control(pDX, IDC_BUTTON2, m_deleteButton);
	DDX_Control(pDX, IDC_BUTTON3, m_switchButton);
}


BEGIN_MESSAGE_MAP(CUsersPage, CNormalDlg)
	ON_BN_CLICKED(IDC_BUTTON1, &CUsersPage::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CUsersPage::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CUsersPage::OnBnClickedButton3)
END_MESSAGE_MAP()
#pragma endregion

// CUsersPage ��Ϣ��������

// ��ʼ��
BOOL CUsersPage::OnInitDialog()
{
	CNormalDlg::OnInitDialog();

	m_resize.AddControl(&m_currentUserStatic, RT_NULL, NULL, RT_NULL, NULL, RT_KEEP_DIST_TO_RIGHT, this);
	m_resize.AddControl(&m_list, RT_NULL, NULL, RT_NULL, NULL, RT_KEEP_DIST_TO_RIGHT, this, RT_KEEP_DIST_TO_BOTTOM, this);
	m_resize.AddControl(&m_loginButton, RT_NULL, NULL, RT_KEEP_DIST_TO_BOTTOM, &m_list);
	m_resize.AddControl(&m_deleteButton, RT_NULL, NULL, RT_KEEP_DIST_TO_BOTTOM, &m_list);
	m_resize.AddControl(&m_switchButton, RT_NULL, NULL, RT_KEEP_DIST_TO_BOTTOM, &m_list);

	// ��ȷ������
	if (!((CTiebaManagerDlg*)AfxGetApp()->m_pMainWnd)->m_confirmButton.IsWindowEnabled())
		m_switchButton.EnableWindow(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣:  OCX ����ҳӦ���� FALSE
}

// ��¼
void CUsersPage::OnBnClickedButton1()
{
	CLoginDlg loginDlg(this);
	if (loginDlg.DoModal() != IDOK)
		return;
	// ����Ŀ¼
	if (!PathFileExists(USERS_PATH))
		CreateDirectory(USERS_PATH, NULL);
	if (!PathFileExists(USERS_PATH + loginDlg.m_userName))
		CreateDirectory(USERS_PATH + loginDlg.m_userName, NULL);
	// ����Cookie
	gzFile f = gzopen_w(USERS_PATH + loginDlg.m_userName + _T("\\ck.tb"), "wb");
	if (f != NULL)
	{
		WriteText(f, loginDlg.m_cookie);
		gzclose(f);
	}

	int index = m_list.FindStringExact(-1, loginDlg.m_userName);
	if (index == LB_ERR)
		index = m_list.AddString(loginDlg.m_userName);
	m_list.SetCurSel(index);
	if (g_currentUser == _T("[NULL]"))
		OnBnClickedButton3();
}

// ɾ��
void CUsersPage::OnBnClickedButton2()
{
	int index = m_list.GetCurSel();
	if (index == LB_ERR)
		return;
	CString name;
	m_list.GetText(index, name);
	if (name == g_currentUser)
	{
		AfxMessageBox(_T("����ɾ����ǰ�˺ţ�"), MB_ICONERROR);
		return;
	}
	CString path = USERS_PATH + name + _T("\\");
	if (!DeleteFile(path + _T("ck.tb")) || !DeleteFile(path + _T("cache.tb")))
	{
		AfxMessageBox(_T("ɾ���˺�ʧ�ܣ�"), MB_ICONERROR);
		return;
	}
	m_list.DeleteString(index);
	m_list.SetCurSel(index == 0 ? 0 : index - 1);
}

// �л�
void CUsersPage::OnBnClickedButton3()
{
	int index = m_list.GetCurSel();
	if (index == LB_ERR)
		return;
	// ��ȷ������
	if (!((CTiebaManagerDlg*)AfxGetApp()->m_pMainWnd)->m_confirmButton.IsWindowEnabled())
		return;
	CString name;
	m_list.GetText(index, name);
	SetCurrentUser(name);
	((CSettingDlg*)GetParent()->GetParent())->ShowCurrentOptions();
	m_currentUserStatic.SetWindowText(_T("��ǰ�˺ţ�") + g_currentUser);
}