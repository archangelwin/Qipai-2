#include "StdAfx.h"
#include "ScoreView.h"

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CScoreView, CWnd)
	ON_WM_PAINT()
	ON_WM_CREATE()
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//���캯��
CScoreView::CScoreView()
{
	//������Ϣ
	m_wGameScore=0;
	m_wConcealTime=0;
	m_wConcealScore=0;
	m_cbConcealCount=0;
	m_wLandUser=INVALID_CHAIR;
	m_wLandScore=0;
	ZeroMemory(m_cbConcealCard,sizeof(m_cbConcealCard));

	//��������
	ZeroMemory(m_lScore,sizeof(m_lScore));
	ZeroMemory(m_lKingScore,sizeof(m_lKingScore));
	ZeroMemory(m_szUserName,sizeof(m_szUserName));

	//������Դ
	m_ImageCard.LoadFromResource(AfxGetInstanceHandle(),IDB_CARD);
	m_ImageBack.LoadFromResource(AfxGetInstanceHandle(),IDB_SCORE_BACK);
	m_ImageWinLose.LoadFromResource(AfxGetInstanceHandle(),IDB_WIN_LOSE);

	//��ȡ��С
	m_CardSize.cx=m_ImageCard.GetWidth()/13;
	m_CardSize.cy=m_ImageCard.GetHeight()/5;

	return;
}

//��������
CScoreView::~CScoreView()
{
}

//������Ϣ
int CScoreView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (__super::OnCreate(lpCreateStruct)==-1) return -1;

	//���ñ���
	SetClassLong(m_hWnd,GCL_HBRBACKGROUND,NULL);

	//�ƶ�����
	SetWindowPos(NULL,0,0,m_ImageBack.GetWidth(),m_ImageBack.GetHeight(),SWP_NOMOVE|SWP_NOZORDER);
	
	//�滭����
	CRgn RgnControl;
	m_ImageBack.CreateImageRegion(RgnControl,RGB(255,0,255));
	SetWindowRgn(RgnControl,TRUE);

	m_btOkStart.Create(TEXT(""),WS_CHILD|WS_CLIPSIBLINGS|WS_CLIPCHILDREN,CRect(0,0,0,0),this,2654);
	m_btOkStart.SetButtonImage(IDB_BT_OKSTART,AfxGetInstanceHandle(),false,false);
	m_btOkStart.ShowWindow(SW_SHOW);

	CRect rcButton;
	CRect rcClient;
	GetClientRect(&rcClient);
	HDWP hDwp=BeginDeferWindowPos(32);
	const UINT uFlags=SWP_NOACTIVATE|SWP_NOZORDER|SWP_NOCOPYBITS|SWP_NOSIZE;
	m_btOkStart.GetWindowRect(&rcButton);
	DeferWindowPos(hDwp,m_btOkStart,NULL,		rcClient.Width()-rcButton.Width()-22	,rcClient.Height()-rcButton.Height()-10,0,0,uFlags);
	EndDeferWindowPos(hDwp);

	return 0;
}

//�����Ϣ
void CScoreView::OnLButtonDown(UINT nFlags, CPoint Point)
{
	__super::OnLButtonDown(nFlags,Point);

	//��Ϣģ��
	PostMessage(WM_NCLBUTTONDOWN,HTCAPTION,MAKELPARAM(Point.x,Point.y));

	return;
}

//�ػ�����
void CScoreView::OnPaint() 
{
	CPaintDC dc(this); 
	m_ImageBack.BitBlt(dc,0,0);

	//������Դ
	CFont InfoFont;
	InfoFont.CreateFont(-16,0,0,0,800,0,0,0,GB2312_CHARSET,3,2,ANTIALIASED_QUALITY,2,TEXT("����"));

	//���� DC
	dc.SetTextColor(RGB(0,250,250));
	dc.SetTextAlign(TA_LEFT|TA_TOP);
	dc.SetBkMode(TRANSPARENT);
	CFont * pOldFont=dc.SelectObject(&InfoFont);

	//��ʾ��Ϣ
	TCHAR szBuffer[128]=TEXT("");

	if(m_wLandUser!=INVALID_CHAIR)
	{
		//ׯ������
		dc.TextOut(100,62,m_szUserName[m_wLandUser]);
		//�з�
		CString strLand;
		strLand.Format(TEXT("%d ��"),m_wLandScore);
		if(m_wLandScore==0) strLand.Append(TEXT("��4����"));
		else if(m_wLandScore>0 && m_wLandScore<=40) strLand.Append(TEXT("��3����"));
		else if(m_wLandScore>40 && m_wLandScore<85) strLand.Append(TEXT("��2����"));
		else strLand.Append(TEXT("��1����"));
		dc.TextOut(100,90,strLand);
		//��Ϸ�÷�
		LONGLONG lGameScore=m_wGameScore;
		if(m_bAddConceal)
		{
			lGameScore=m_wGameScore+m_wConcealScore*m_wConcealTime;
		}
		if(m_bAddConceal==true) _sntprintf(szBuffer,sizeof(szBuffer),TEXT("%d + ���Ʒ� %d * %d �� = %I64d ��"),m_wGameScore,m_wConcealScore,m_wConcealTime,lGameScore);
		else _sntprintf(szBuffer,sizeof(szBuffer),TEXT("%I64d ��"),lGameScore);
		dc.TextOut(140,120,szBuffer);
		//ʤ����־
		INT nWinLoseWidth = m_ImageWinLose.GetWidth()/3;
		INT nWinLostHeight = m_ImageWinLose.GetHeight();
		BYTE index=0;
		LONGLONG lScoreBanker=m_lKingScore[m_wLandUser]+m_lScore[m_wLandUser];
		if(lScoreBanker>0) index=0;
		if(lScoreBanker==0) index=2;
		if(lScoreBanker<0) index=1;
		m_ImageWinLose.TransDrawImage(&dc,210,58,nWinLoseWidth,nWinLostHeight,index*nWinLoseWidth,0,RGB(255,0,255));

		//����
		CString strRank(TEXT(""));
		switch(m_bEndStatus)
		{
		case 1:strRank.Format(TEXT("���"));break;
		case 2:strRank.Format(TEXT("С��"));break;
		case 3:strRank.Format(TEXT("��ׯ"));break;
		case 4:strRank.Format(TEXT("һ��"));break;
		case 5:strRank.Format(TEXT("����"));break;
		case 6:strRank.Format(TEXT("����"));break;
		case 7:strRank.Format(TEXT("�ļ�"));break;
		case 8:strRank.Format(TEXT("�弶"));break;
		case 9:strRank.Format(TEXT("����"));break;
		case 255:strRank.Format(TEXT("Ͷ��"));break;
		}
		dc.TextOut(270,62,strRank);
	}

	//��ʾ����
	CRect rcDraw;
	for (WORD i=0;i<CountArray(m_szUserName);i++)
	{
		//�û�����
		rcDraw.left=15;
		rcDraw.right=rcDraw.left+100;
		rcDraw.top=i*33+195;
		rcDraw.bottom=rcDraw.top+16;
		dc.DrawText(m_szUserName[i],lstrlen(m_szUserName[i]),&rcDraw,DT_VCENTER|DT_CENTER|DT_END_ELLIPSIS);

		//ʤ����־
		INT nWinLoseWidth = m_ImageWinLose.GetWidth()/3;
		INT nWinLostHeight = m_ImageWinLose.GetHeight();
		BYTE index=0;
		if(m_lScore[i]>0) index=0;
		if(m_lScore[i]==0) index=2;
		if(m_lScore[i]<0) index=1;
		m_ImageWinLose.TransDrawImage(&dc,rcDraw.right+13,rcDraw.top-5,nWinLoseWidth,nWinLostHeight,index*nWinLoseWidth,0,RGB(255,0,255));

		//�û�����
		rcDraw.left=180;
		rcDraw.right=rcDraw.left+60;
		_sntprintf(szBuffer,sizeof(szBuffer),TEXT("%I64d"),m_lScore[i]);
		dc.DrawText(szBuffer,lstrlen(szBuffer),&rcDraw,DT_VCENTER|DT_CENTER|DT_END_ELLIPSIS);

		//��ط�
		rcDraw.left=250;
		rcDraw.right=rcDraw.left+80;
		_sntprintf(szBuffer,sizeof(szBuffer),TEXT("%I64d"),m_lKingScore[i]);
		dc.DrawText(szBuffer,lstrlen(szBuffer),&rcDraw,DT_VCENTER|DT_CENTER|DT_END_ELLIPSIS);
			
		//�ܵ÷�
		rcDraw.left=330;
		rcDraw.right=rcDraw.left+80;
		_sntprintf(szBuffer,sizeof(szBuffer),TEXT("%I64d"),m_lKingScore[i]+m_lScore[i]);
		dc.DrawText(szBuffer,lstrlen(szBuffer),&rcDraw,DT_VCENTER|DT_CENTER|DT_END_ELLIPSIS);
	}

	//������Դ
	dc.SelectObject(pOldFont);
	InfoFont.DeleteObject();

	return;
}

//������Ϣ
void CScoreView::SetScoreViewInfo(tagScoreViewInfo & ScoreViewInfo)
{
	//���ñ���
	m_wGameScore=ScoreViewInfo.wGameScore;
	m_wConcealTime=ScoreViewInfo.wConcealTime;
	m_wConcealScore=ScoreViewInfo.wConcealScore;
	m_cbConcealCount=ScoreViewInfo.cbConcealCount;
	m_bAddConceal=ScoreViewInfo.bAddConceal;
	m_bEndStatus=ScoreViewInfo.bEndStatus;
	CopyMemory(m_cbConcealCard,ScoreViewInfo.cbConcealCard,m_cbConcealCount*sizeof(BYTE));

	//ˢ�½���
	if (m_hWnd!=NULL) Invalidate(FALSE);

	return;
}

//���û���
void CScoreView::SetGameScore(WORD wChairID, LPCTSTR pszUserName, LONGLONG lScore,LONGLONG lKingScore)
{
	if (wChairID<CountArray(m_lScore))
	{
		m_lScore[wChairID]=lScore;
		m_lKingScore[wChairID]=lKingScore;
		lstrcpyn(m_szUserName[wChairID],pszUserName,CountArray(m_szUserName[wChairID]));
		Invalidate(FALSE);
	}
	return;
}
void CScoreView::SetLandScore(WORD wLandUser,WORD wLandScore)
{
	m_wLandUser=wLandUser;
	m_wLandScore=wLandScore;
}

BOOL CScoreView::OnCommand(WPARAM wParam,LPARAM lParam)
{
	if(wParam==2654&&m_pSinkWindow!=NULL)
	{
		m_pSinkWindow->SendEngineMessage((WM_USER+100),0,0);
	}
	return __super::OnCommand(wParam,lParam);
}
//////////////////////////////////////////////////////////////////////////
