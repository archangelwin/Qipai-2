#include "StdAfx.h"
#include "GameLogic.h"
#include "GameClient.h"
#include "GameClientView.h"
#include "Resource.h"


//////////////////////////////////////////////////////////////////////////
//��ť��ʶ

//��Ϸ��ť
#define IDC_START						100								//��ʼ��ť
#define IDC_OUT_CARD					101								//���ư�ť
#define IDC_OUT_PROMPT					102								//��ʾ��ť
#define IDC_SEND_CONCEAL				103								//���װ�ť

#define IDC_ADD_SCORE					104								//�ӷְ�ť
#define IDC_REDUCE_SCORE				105								//���ְ�ť
#define IDC_CONFIRM_SCORE				106								//ȷ���з�
#define IDC_GIVEUP_SCORE				107								//�����з�

#define IDC_GIVE_UP_GAME				108

//���ܰ�ť
#define IDC_CONCEAL_CARD				200								//�鿴����
//#define IDC_REQUEST_LEAVE				201								//�����뿪
#define IDC_LAST_TURN_CARD				202								//�����˿�
#define IDC_TRUSTEE_CONTROL				203								//�йܿ���

//////////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CGameClientView, CGameFrameViewGDI)
	ON_WM_CREATE()
	ON_WM_RBUTTONUP()
	ON_WM_LBUTTONDBLCLK()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////////////

//���캯��
CGameClientView::CGameClientView()
{
	//��ʾ����
	m_bDeasilOrder=true;
	m_bWaitConceal=false;
	m_bLastTurnCard=false;

	//��Ϸ����
	m_wCardScore=0xFFFF;
	m_wBankerUser=INVALID_CHAIR;
	ZeroMemory(m_bScore,sizeof(m_bScore));
	ZeroMemory(m_wPlayerScore,sizeof(m_wPlayerScore));
	ZeroMemory(m_bGiveUpPlayer,sizeof(m_bGiveUpPlayer));
	ZeroMemory(m_bUserNoMain,sizeof(m_bUserNoMain));
	ZeroMemory(m_bMainCount,sizeof(m_bMainCount));
	ZeroMemory(m_bDoubleCount,sizeof(m_bDoubleCount));


	m_wUserCallScore=0;
	m_wCurrentCallScore=120;

	//������Ϣ
	m_cbCallCount=0;
	m_cbCallColor=COLOR_ERROR;
	m_cbMainValue=VALUE_ERROR;
	m_cbValueOrder[0]=VALUE_ERROR;
	m_cbValueOrder[1]=VALUE_ERROR;
	m_wCallCardUser=INVALID_CHAIR;



	return;
}

//��������
CGameClientView::~CGameClientView()
{
}

//������Ϣ
int CGameClientView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (__super::OnCreate(lpCreateStruct)==-1) return -1;

	//������Դ
	HINSTANCE hInstance=AfxGetInstanceHandle();
	m_ImageBanker.LoadFromResource(this,hInstance,IDB_BANKER);
	m_ImageBack.LoadFromResource(this,hInstance,IDB_VIEW_BACK);
	m_ImageCenter.LoadFromResource(this,hInstance,IDB_VIEW_CENTER);
	m_ImageColorFlag.LoadFromResource(this,hInstance,IDB_COLOR_FLAG);
	m_ImageLastTurn.LoadFromResource(this,hInstance,IDB_LAST_TURN_TIP);
	m_ImageWaitConceal.LoadFromResource(this,hInstance,IDB_WAIT_CONCEAL);
	m_ImageStationTitle.LoadFromResource(this,hInstance,IDB_STATION_TITLE);
	m_ImageNoMain.LoadFromResource(this,hInstance,IDB_NO_MAIN);
	m_ImageScoreCard.LoadFromResource(this,hInstance,IDB_SCORE_CARD);

	m_ImageNoCall.LoadImage(this,hInstance,TEXT("NO_CALL"));
	m_ImageNumber.LoadImage(this,hInstance,TEXT("NUMBER"));
	m_ImageMain1.LoadImage(this,hInstance,TEXT("MAIN_1"));
	m_ImageMain2.LoadImage(this,hInstance,TEXT("MAIN_2"));

	//��ȡ��С
	m_SizeBanker.cx=m_ImageBanker.GetWidth();
	m_SizeBanker.cy=m_ImageBanker.GetHeight();

	//�����ؼ�
	CRect rcCreate(0,0,0,0);
	m_ScoreView.Create(NULL,NULL,WS_CHILD|WS_CLIPSIBLINGS|WS_CLIPCHILDREN,rcCreate,this,10);
	m_CallCardWnd.Create(NULL,NULL,WS_CHILD|WS_CLIPSIBLINGS|WS_CLIPCHILDREN,rcCreate,this,11);
	m_ConcealCardView.Create(NULL,NULL,WS_CHILD|WS_CLIPSIBLINGS|WS_CLIPCHILDREN,rcCreate,this,12);
	m_CallScoreView.Create(NULL,NULL,WS_CHILD|WS_CLIPSIBLINGS|WS_CLIPCHILDREN,rcCreate,this,13);

	//����GM�˿�
	for (WORD i=0;i<4;i++)
	{
		if(i==1||i==3) m_GmCardControl[i].SetDirection(false);
		m_GmCardControl[i].SetDisplayFlag(true);
		m_GmCardControl[i].SetCardSpace(DEF_CARD_H_SPACE,DEF_CARD_V_SPACE,0);
		m_GmCardControl[i].Create(NULL,NULL,WS_VISIBLE|WS_CHILD|WS_CLIPSIBLINGS|WS_CLIPCHILDREN,rcCreate,this,50+i);
	}
	//�����˿�
	for (WORD i=0;i<4;i++)
	{
		m_UserCardControl[i].SetDirection(true);
		m_UserCardControl[i].SetDisplayFlag(true);
		m_UserCardControl[i].SetCardSpace(DEF_CARD_H_SPACE,DEF_CARD_V_SPACE,0);
		m_UserCardControl[i].Create(NULL,NULL,WS_VISIBLE|WS_CHILD|WS_CLIPSIBLINGS|WS_CLIPCHILDREN,rcCreate,this,20+i);
	}
	m_CardScore.Create(NULL,NULL,WS_VISIBLE|WS_CHILD,rcCreate,this,50);
	m_HandCardControl.Create(NULL,NULL,WS_VISIBLE|WS_CHILD|WS_CLIPSIBLINGS|WS_CLIPCHILDREN,rcCreate,this,60);
	m_BackCardControl.Create(NULL,NULL,WS_VISIBLE|WS_CHILD|WS_CLIPSIBLINGS|WS_CLIPCHILDREN,rcCreate,this,61);

	//�����˿�
	m_CardScore.SetDisplayFlag(true);
	m_CardScore.SetCardSpace(10,0,0);

	m_HandCardControl.SetSinkWindow(this);
	m_ScoreView.SetSinkWindow(this);
	m_CallScoreView.SetSinkWindow(this);
	m_CallCardWnd.SetSinkWindow(this);

	//������ť
	m_btStart.Create(TEXT(""),WS_CHILD|WS_CLIPSIBLINGS|WS_CLIPCHILDREN,rcCreate,this,IDC_START);
	m_btOutCard.Create(TEXT(""),WS_CHILD|WS_CLIPSIBLINGS|WS_CLIPCHILDREN,rcCreate,this,IDC_OUT_CARD);
	m_btOutPrompt.Create(TEXT(""),WS_CHILD|WS_CLIPSIBLINGS|WS_CLIPCHILDREN,rcCreate,this,IDC_OUT_PROMPT);
	m_btSendConceal.Create(TEXT(""),WS_CHILD|WS_CLIPSIBLINGS|WS_CLIPCHILDREN,rcCreate,this,IDC_SEND_CONCEAL);
	m_btGiveUp.Create(TEXT(""),WS_CHILD|WS_CLIPSIBLINGS|WS_CLIPCHILDREN,rcCreate,this,IDC_GIVE_UP_GAME);

	//���ܰ�ť
	m_btConcealCard.Create(TEXT(""),WS_CHILD|WS_DISABLED|WS_VISIBLE,rcCreate,this,IDC_CONCEAL_CARD);
	//m_btRuquestLeave.Create(TEXT(""),WS_CHILD|WS_DISABLED|WS_VISIBLE,rcCreate,this,IDC_REQUEST_LEAVE);
	m_btLastTurnCard.Create(TEXT(""),WS_CHILD|WS_DISABLED|WS_VISIBLE,rcCreate,this,IDC_LAST_TURN_CARD);
	m_btStusteeControl.Create(TEXT(""),WS_CHILD|WS_DISABLED|WS_VISIBLE,rcCreate,this,IDC_TRUSTEE_CONTROL);

	//���ð�ť

	m_btStart.SetButtonImage(IDB_START,hInstance,false,false);
	m_btOutCard.SetButtonImage(IDB_OUT_CARD,hInstance,false,false);
	m_btOutPrompt.SetButtonImage(IDB_OUT_PROMPT,hInstance,false,false);
	m_btSendConceal.SetButtonImage(IDB_SEND_CONCEAL,hInstance,false,false);
	m_btGiveUp.SetButtonImage(IDB_BT_GIVEUP_GAME,hInstance,false,false);

	//���ܰ�ť
	m_btConcealCard.SetButtonImage(IDB_CONCEAL_CARD,hInstance,false,false);
	//m_btRuquestLeave.SetButtonImage(IDB_REQUEST_LEAVE,hInstance,false);
	m_btLastTurnCard.SetButtonImage(IDB_LAST_TURN_CARD,hInstance,false,false);
	m_btStusteeControl.SetButtonImage(IDB_START_TRUSTEE,hInstance,false,false);

	//������ʾ
	m_ToolTipCtrl.Create(this);
	m_ToolTipCtrl.Activate(TRUE);
	m_ToolTipCtrl.AddTool(&m_btConcealCard,TEXT("�鿴����"));
	//m_ToolTipCtrl.AddTool(&m_btRuquestLeave,TEXT("�鿴����"));
	m_ToolTipCtrl.AddTool(&m_btLastTurnCard,TEXT("��ʾ��һ�ֳ���"));
	m_ToolTipCtrl.AddTool(&m_btStusteeControl,TEXT("��ʼ��ֹͣ���й�"));

	//��ȡ����
	//m_bDeasilOrder=AfxGetApp()->GetProfileInt(TEXT("GameOption"),TEXT("DeasilOrder"),FALSE)?true:false;

	return 0;
}

//��Ϣ����
BOOL CGameClientView::PreTranslateMessage(MSG * pMsg)
{
	m_ToolTipCtrl.RelayEvent(pMsg);
	return __super::PreTranslateMessage(pMsg);
}

//���ý���
VOID CGameClientView::ResetGameView()
{
	//��ʾ����
	m_bWaitConceal=false;
	m_bLastTurnCard=false;

	//��Ϸ����
	m_wCardScore=0xFFFF;
	m_wBankerUser=INVALID_CHAIR;
	m_wUserCallScore=0;
	m_wCurrentCallScore=120;
	ZeroMemory(m_wPlayerScore,sizeof(m_wPlayerScore));
	ZeroMemory(m_bGiveUpPlayer,sizeof(m_bGiveUpPlayer));
	ZeroMemory(m_bUserNoMain,sizeof(m_bUserNoMain));
	ZeroMemory(m_bMainCount,sizeof(m_bMainCount));
	ZeroMemory(m_bDoubleCount,sizeof(m_bDoubleCount));

	//������Ϣ
	m_cbCallCount=0;
	m_cbCallColor=COLOR_ERROR;
	m_cbMainValue=VALUE_ERROR;
	m_cbValueOrder[0]=VALUE_ERROR;
	m_cbValueOrder[1]=VALUE_ERROR;
	m_wCallCardUser=INVALID_CHAIR;

	//���ؿؼ�
	m_btStart.ShowWindow(SW_HIDE);
	m_btOutCard.ShowWindow(SW_HIDE);
	m_btOutPrompt.ShowWindow(SW_HIDE);
	m_btSendConceal.ShowWindow(SW_HIDE);

	//���ð�ť
	m_btConcealCard.EnableWindow(FALSE);
	//m_btRuquestLeave.EnableWindow(FALSE);
	m_btLastTurnCard.EnableWindow(FALSE);
	m_btStusteeControl.EnableWindow(FALSE);
	m_btStusteeControl.SetButtonImage(IDB_START_TRUSTEE,AfxGetInstanceHandle(),false,false);

	//�����˿�
	m_CardScore.SetCardData(NULL,0);
	m_HandCardControl.SetCardData(NULL,0);
	m_HandCardControl.SetPositively(false);
	m_HandCardControl.SetDisplayFlag(false);
	for (WORD i=0;i<CountArray(m_UserCardControl);i++) m_UserCardControl[i].SetCardData(NULL,0);

	//�ؼ����
	m_ScoreView.ShowWindow(SW_HIDE);
	m_CallCardWnd.ShowWindow(SW_HIDE);
	m_ConcealCardView.ShowWindow(SW_HIDE);

	return;
}

//�����ؼ�
VOID CGameClientView::RectifyControl(INT nWidth, INT nHeight)
{
	int m_nYBorder=0,m_nXBorder=0;
	int m_nYFace=50,m_nXFace=50;
	int m_nYTimer=60,m_nXTimer=60;
	//��������
	int nViewHeight=nHeight-m_nYBorder/*-m_SizeStation.cy*/;

	//��������
	m_ptAvatar[3].x		=nWidth-m_nXBorder-m_nXFace-5;
	m_ptAvatar[3].y		=nViewHeight/2-m_nYFace-70;
	m_ptNickName[3].x		=m_ptAvatar[3].x;
	m_ptNickName[3].y		=m_ptAvatar[3].y+5;
	m_ptClock[3].x		=m_ptAvatar[3].x+10;
	m_ptClock[3].y		=m_ptAvatar[3].y+m_nYFace+m_nYTimer/2+5;
	m_ptReady[3].x		=m_ptAvatar[3].x-10;
	m_ptReady[3].y		=m_ptAvatar[3].y-30;
	m_PointBanker[3].x	=m_ptAvatar[3].x-10;
	m_PointBanker[3].y	=m_ptAvatar[3].y-50;
	m_ptScore[3].x		=m_ptAvatar[3].x-210;
	m_ptScore[3].y		=m_ptAvatar[3].y;
	m_ptAuto[3].x		=m_ptAvatar[3].x;
	m_ptAuto[3].y		=m_ptAvatar[3].y;
	m_ptNoMain[3].x		=m_ptAvatar[3].x-30;
	m_ptNoMain[3].y		=m_ptAvatar[3].y-95;

	m_ptAvatar[1].x		=m_nXBorder+5;
	m_ptAvatar[1].y		=nViewHeight/2-m_nYFace-70;
	m_ptNickName[1].x		=m_ptAvatar[1].x+m_nXFace+5;
	m_ptNickName[1].y		=m_ptAvatar[1].y+5;
	m_ptClock[1].x		=m_ptAvatar[1].x+25;
	m_ptClock[1].y		=m_ptAvatar[1].y+m_nYFace+m_nYTimer/2+5;
	m_ptReady[1].x		=m_ptAvatar[1].x+40;
	m_ptReady[1].y		=m_ptAvatar[1].y-30;
	m_PointBanker[1].x	=m_ptAvatar[1].x;
	m_PointBanker[1].y	=m_ptAvatar[1].y-50;
	m_ptScore[1].x		=m_ptAvatar[1].x+180;
	m_ptScore[1].y		=m_ptAvatar[1].y;
	m_ptAuto[1].x		=m_ptAvatar[1].x;
	m_ptAuto[1].y		=m_ptAvatar[1].y;
	m_ptNoMain[1].x		=m_ptAvatar[1].x;
	m_ptNoMain[1].y		=m_ptAvatar[1].y-95;

	m_ptAvatar[0].x		=nWidth/2-m_nXFace/2+20;
	m_ptAvatar[0].y		=m_nYBorder+5;
	m_ptNickName[0].x		=m_ptAvatar[0].x+m_nXFace+5;
	m_ptNickName[0].y		=m_ptAvatar[0].y;
	m_ptClock[0].x		=m_ptAvatar[0].x-m_nXTimer/2;
	m_ptClock[0].y		=m_ptAvatar[0].y+m_nYTimer/2-5;
	m_ptReady[0].x		=m_ptAvatar[0].x-100;
	m_ptReady[0].y		=m_ptAvatar[0].y+25;
	m_PointBanker[0].x	=m_ptAvatar[0].x-100;
	m_PointBanker[0].y	=m_ptAvatar[0].y-5;
	m_ptScore[0].x		=m_ptAvatar[0].x-120;
	m_ptScore[0].y		=m_ptAvatar[0].y+10;
	m_ptAuto[0].x		=m_ptAvatar[0].x;
	m_ptAuto[0].y		=m_ptAvatar[0].y;
	m_ptNoMain[0].x		=m_ptAvatar[0].x+120;
	m_ptNoMain[0].y		=m_ptAvatar[0].y-20;

	m_ptAvatar[2].x		=nWidth/2-m_nXFace/2+20;
	m_ptAvatar[2].y		=nViewHeight-m_nYFace-5;
	m_ptNickName[2].x		=m_ptAvatar[2].x+m_nXFace+5;
	m_ptNickName[2].y		=m_ptAvatar[2].y;
	m_ptClock[2].x		=m_ptAvatar[2].x-m_nXTimer/2;
	m_ptClock[2].y		=m_ptAvatar[2].y+m_nYTimer/2-15;
	m_ptReady[2].x		=m_ptAvatar[2].x-100;
	m_ptReady[2].y		=m_ptAvatar[2].y-15;
	m_PointBanker[2].x	=m_ptAvatar[2].x-110;
	m_PointBanker[2].y	=m_ptAvatar[2].y-5;
	m_ptScore[2].x		=m_ptAvatar[2].x-180;
	m_ptScore[2].y		=m_ptAvatar[2].y;
	m_ptAuto[2].x		=m_ptAvatar[2].x;
	m_ptAuto[2].y		=m_ptAvatar[2].y;
	m_ptNoMain[2].x		=m_ptAvatar[2].x+120;
	m_ptNoMain[2].y		=m_ptAvatar[2].y-20;

	//�ƶ���ť
	CRect rcButton;
	HDWP hDwp=BeginDeferWindowPos(32);
	const UINT uFlags=SWP_NOACTIVATE|SWP_NOZORDER|SWP_NOCOPYBITS|SWP_NOSIZE;

	//��ʼ��ť
	m_btStart.GetWindowRect(&rcButton);
	DeferWindowPos(hDwp,m_btStart,NULL,(nWidth-rcButton.Width())/2,nViewHeight-225,0,0,uFlags);

	//���װ�ť
	m_btSendConceal.GetWindowRect(&rcButton);
	DeferWindowPos(hDwp,m_btSendConceal,NULL,(nWidth-rcButton.Width())/2,nViewHeight-225,0,0,uFlags);

	//���ư�ť
	m_btOutCard.GetWindowRect(&rcButton);
	DeferWindowPos(hDwp,m_btOutCard,NULL,		nWidth/2-rcButton.Width()/2-50	,nViewHeight-225,0,0,uFlags);
	DeferWindowPos(hDwp,m_btOutPrompt,NULL,		nWidth/2-rcButton.Width()/2+50	,nViewHeight-225,0,0,uFlags);
	DeferWindowPos(hDwp,m_btGiveUp,NULL,		nWidth/2-rcButton.Width()/2+150	,nViewHeight-225,0,0,uFlags);

	//���ܰ�ť
	m_btConcealCard.GetWindowRect(&rcButton);
	DeferWindowPos(hDwp,m_btStusteeControl,NULL,nWidth-m_nXBorder-(rcButton.Width()+5)*1,nViewHeight-5-rcButton.Height(),0,0,uFlags);
	//DeferWindowPos(hDwp,m_btRuquestLeave,NULL,nWidth-m_nXBorder-(rcButton.Width()+5)*2,nViewHeight-5-rcButton.Height(),0,0,uFlags);
	DeferWindowPos(hDwp,m_btConcealCard,NULL,nWidth-m_nXBorder-(rcButton.Width()+5)*2,nViewHeight-5-rcButton.Height(),0,0,uFlags);
	DeferWindowPos(hDwp,m_btLastTurnCard,NULL,nWidth-m_nXBorder-(rcButton.Width()+5)*3,nViewHeight-5-rcButton.Height(),0,0,uFlags);

	//�ƶ��ؼ�
	EndDeferWindowPos(hDwp);

	//�ƶ��˿�
	m_CardScore.SetBenchmarkPos(m_nXBorder+5,nViewHeight+200,enXLeft,enYTop);
	m_UserCardControl[0].SetBenchmarkPos(nWidth/2,m_nYBorder+m_nYFace+45,enXCenter,enYTop);
	m_UserCardControl[2].SetBenchmarkPos(nWidth/2,nViewHeight-228-m_nYBorder,enXCenter,enYBottom);
	m_HandCardControl.SetBenchmarkPos(nWidth/2,nViewHeight-m_nYFace-m_nYBorder-25,enXCenter,enYBottom);
	m_BackCardControl.SetBenchmarkPos(nWidth/2,nHeight/2+90,enXCenter,enYCenter);

	m_GmCardControl[0].SetBenchmarkPos(nWidth/2,m_nYBorder+m_nYFace+25,enXCenter,enYTop);
	m_GmCardControl[1].SetBenchmarkPos(m_nXBorder+m_nXFace+30,nViewHeight/2-71,enXLeft,enYCenter);
	m_GmCardControl[2].SetBenchmarkPos(nWidth/2,nViewHeight-m_nYFace-m_nYBorder-20,enXCenter,enYBottom);
	m_GmCardControl[3].SetBenchmarkPos(nWidth-m_nXFace-m_nXBorder-30,nViewHeight/2-71,enXRight,enYCenter);

	//�û��˿�
	m_UserCardControl[3].SetBenchmarkPos(nWidth-m_nXFace-m_nXBorder-80,nViewHeight/2-71,enXRight,enYCenter);
	m_UserCardControl[1].SetBenchmarkPos(m_nXBorder+m_nXFace+80,nViewHeight/2-71,enXLeft,enYCenter);

	//���ƴ���
	CRect rcCallCard;
	m_CallCardWnd.GetWindowRect(&rcCallCard);
	m_CallCardWnd.SetWindowPos(NULL,(nWidth-rcCallCard.Width())/2,nViewHeight-m_nYBorder-rcCallCard.Height()-182,0,0,SWP_NOZORDER|SWP_NOSIZE);

	//������ͼ
	CRect rcScore;
	m_ScoreView.GetWindowRect(&rcScore);
	m_ScoreView.SetWindowPos(NULL,(nWidth-rcScore.Width())/2,nViewHeight/2-320,0,0,SWP_NOZORDER|SWP_NOSIZE);

	//�з���ͼ
	CRect rcCallScore;
	m_CallScoreView.GetWindowRect(&rcCallScore);
	m_CallScoreView.SetWindowPos(NULL,(nWidth-rcCallScore.Width())/2,nViewHeight-rcCallScore.Height()-390,0,0,SWP_NOZORDER|SWP_NOSIZE);

	//���ƴ���
	CRect rcConceal;
	m_ConcealCardView.GetWindowRect(&rcConceal);
	m_ConcealCardView.SetWindowPos(NULL,(nWidth-rcConceal.Width())/2,nViewHeight/2-160,0,0,SWP_NOZORDER|SWP_NOSIZE);

	m_lWidth=nWidth,m_lHeight=nHeight;

	return;
}

//�����
BOOL CGameClientView::OnCommand(WPARAM wParam, LPARAM lParam)
{
	switch(LOWORD(wParam))
	{
	case  IDC_START:
		{
			SendEngineMessage(IDM_START,0,0);
			return TRUE;
		}
	case  IDC_OUT_CARD:
		{
			SendEngineMessage(IDM_OUT_CARD,1,1);
			return TRUE;
		}
	case  IDC_OUT_PROMPT:
		{
			SendEngineMessage(IDM_OUT_PROMPT,0,0);
			return TRUE;
		}
	case  IDC_SEND_CONCEAL:
		{
			SendEngineMessage(IDM_SEND_CONCEAL,0,0);
			return TRUE;
		}
	case  IDC_CONCEAL_CARD:
		{
			SendEngineMessage(IDM_CONCEAL_CARD,0,0);
			return TRUE;
		}
	//case  IDC_REQUEST_LEAVE:
	//	{
	//		SendEngineMessage(IDM_REQUEST_LEAVE,0,0);
	//		return TRUE;
	//	}
	case  IDC_LAST_TURN_CARD:
		{
			SendEngineMessage(IDM_LAST_TURN_CARD,0,0);
			return TRUE;
		}
	case  IDC_TRUSTEE_CONTROL:
		{
			SendEngineMessage(IDM_TRUSTEE_CONTROL,0,0);
			return TRUE;
		}
	case  IDC_ADD_SCORE:
		{
			//���ӽз�
			m_wUserCallScore+=5;

			if(m_wUserCallScore>200) m_wUserCallScore=200;
			//���½���
			InvalidGameView(0,0,0,0);

			return TRUE;
		}
	case  IDC_REDUCE_SCORE:
		{
			//���ٽз�
			m_wUserCallScore-=5;
			if(m_wUserCallScore<(m_wCurrentCallScore+5)) m_wUserCallScore=m_wCurrentCallScore+5;
			//���½���
			InvalidGameView(0,0,0,0);
			return TRUE;
		}
	case  IDC_CONFIRM_SCORE:
		{
			SendEngineMessage(IDM_LAND_SCORE,m_wUserCallScore,m_wUserCallScore);
			return TRUE;
		}
	case  IDC_GIVEUP_SCORE:
		{
			SendEngineMessage(IDM_LAND_SCORE,0,0);
			return TRUE;
		}
	case  IDC_GIVE_UP_GAME:
		{
			SendEngineMessage(IDM_GIVEUP_GAME,0,0);
			return TRUE;
		}
	}
	return __super::OnCommand(wParam, lParam);
}

//�滭����
VOID CGameClientView::DrawGameView(CDC * pDC, INT nWidth, INT nHeight)
{
	int m_nXBorder=0,m_nYBorder=0;
	//�滭����
	DrawViewImage(pDC,m_ImageBack,DRAW_MODE_SPREAD);
	DrawViewImage(pDC,m_ImageCenter,DRAW_MODE_CENTENT);

	//��Ϸ��Ϣ
	m_ImageStationTitle.BitBlt(pDC->m_hDC,m_nXBorder,m_nYBorder);


	//ׯ����Ϣ
	if (m_wBankerUser!=INVALID_CHAIR)
	{
		IClientUserItem *pUserItem=GetClientUserItem(m_wBankerUser);
		if(pUserItem!=NULL)
		{
			DrawTextString(pDC,pUserItem->GetNickName(),RGB(238,255,255),RGB(50,50,50),m_nXBorder+85,m_nYBorder+16,14,400);
		}
		//ׯ�ұ�־
		m_ImageBanker.TransDrawImage(pDC,m_PointBanker[m_wBankerUser].x,m_PointBanker[m_wBankerUser].y,m_SizeBanker.cx,m_SizeBanker.cy,0,0,RGB(255,0,255));
	}

	//���ƻ�ɫ
	if (m_cbCallColor!=COLOR_ERROR)
	{
		//������Դ
		int nColorImageWidth=m_ImageColorFlag.GetWidth()/6;
		int nColorImageHeight=m_ImageColorFlag.GetHeight();

		//�滭��־
		for (BYTE i=0;i<m_cbCallCount;i++) 
		{
			int nXPos=m_nXBorder+90+i*(nColorImageWidth+4);
			int nYPos=m_nYBorder+46;
			m_ImageColorFlag.TransDrawImage(pDC,nXPos,nYPos,nColorImageWidth,nColorImageHeight,
				nColorImageWidth*(m_cbCallColor>>4),0,RGB(255,0,255));
		}
	}

	//�з���Ϣ
	CString catchInfo;
	if(m_wBankerUser!=INVALID_CHAIR) 
	{
		catchInfo.Format(TEXT("%d��"),m_wCurrentCallScore);
		if(m_wCurrentCallScore==0) catchInfo.Append(TEXT("(4��)"));
		else if(m_wCurrentCallScore>0 && m_wCurrentCallScore<=40) catchInfo.Append(TEXT("(3��)"));
		else if(m_wCurrentCallScore>40 && m_wCurrentCallScore<85) catchInfo.Append(TEXT("(2��)"));
		else catchInfo.Append(TEXT("(1��)"));
		DrawTextString(pDC,catchInfo,RGB(238,255,255),RGB(50,50,50),m_nXBorder+85,m_nYBorder+74,14,400);
	}

	//�÷���Ϣ
	if (m_wCardScore!=0xFFFF)
	{
		TCHAR szCardScore[32]=TEXT("");

		_sntprintf(szCardScore,CountArray(szCardScore),TEXT("%d��"),m_wCardScore);
		DrawTextString(pDC,szCardScore,RGB(238,255,255),RGB(50,50,50),m_nXBorder+85,m_nYBorder+105,14,400);
	}

	//�滭�û�
	for (WORD i=0;i<GAME_PLAYER;i++)
	{

		//IClientUserItem *pUserItem=GetClientUserItem(i);
		//const tagUserInfo * pUserData=(pUserItem==NULL?NULL:pUserItem->GetUserInfo());

		IClientUserItem *pUserItem=GetClientUserItem(i);

		//�滭�û�
		if (pUserItem!=NULL)
		{
			DrawTextString(pDC,pUserItem->GetNickName(),RGB(240,240,240),RGB(50,50,50),m_ptNickName[i].x,m_ptNickName[i].y,14,400,i==3?DT_RIGHT:DT_LEFT);

			//�÷���Ϣ
			TCHAR szCardScore[32]=TEXT("");
			_sntprintf(szCardScore,CountArray(szCardScore),TEXT("�÷֣�%ld ��"),m_wPlayerScore[i]);
			if(m_wPlayerScore[i]!=0) DrawTextString(pDC,szCardScore,RGB(240,240,240),RGB(50,50,50),m_ptNickName[i].x,m_ptNickName[i].y+14,14,400);

			//������Ϣ
			//��������
			WORD wUserTimer=GetUserClock(i);

			if (wUserTimer!=0) DrawUserClock(pDC,m_ptClock[i].x,m_ptClock[i].y,wUserTimer);
			if (pUserItem->GetUserStatus()==US_READY) DrawUserReady(pDC,m_ptReady[i].x,m_ptReady[i].y);
			DrawUserAvatar(pDC,m_ptAvatar[i].x,m_ptAvatar[i].y,pUserItem);

			//�����з�
			if (m_bGiveUpPlayer[i]==true) 
			{
				m_ImageNoCall.DrawImage(pDC,m_ptScore[i].x,m_ptScore[i].y);
			}

			//�зַ���
			if ((m_bScore[i]!=0)&&(m_bGiveUpPlayer[i]!=true))
			{
	
				DrawNumberString(pDC,m_bScore[i],m_ptScore[i].x+10,m_ptScore[i].y+5);
				TCHAR szCallScore[32]=TEXT("");
				_sntprintf(szCallScore,CountArray(szCallScore),TEXT("�з֣�            ��"),m_bScore[i]);
				CDFontEx::DrawText(this, pDC, 14, 400,szCallScore,m_ptScore[i].x,m_ptScore[i].y, RGB(238,255,255), DT_CENTER);
			}

			//������־
			if (m_bUserNoMain[i]==true)
			{
				//������־
				m_ImageNoMain.TransDrawImage(pDC,m_ptNoMain[i].x,m_ptNoMain[i].y,RGB(255,0,255));			
			}

			//�����Ը���
			if(m_bMainCount[i]!=0 || m_bDoubleCount[i]!=0)
			{
				int x=0;int y=0;
				switch(i)
				{
				case 0:x=m_ptAvatar[0].x-50;y=m_ptAvatar[0].y+40;break;
				case 1:x=m_ptAvatar[1].x+120;y=m_ptAvatar[1].y-50;break;
				case 2:x=m_ptAvatar[2].x-50;y=m_ptAvatar[2].y-150;break;
				case 3:x=m_ptAvatar[3].x-40-m_ImageMain1.GetWidth(),y=m_ptAvatar[3].y-50;break;
				}
				m_ImageMain1.DrawImage(pDC,x,y);
				DrawNumberString(pDC,m_bMainCount[i],x-15,y+15);
				DrawNumberString(pDC,m_bDoubleCount[i],x+110,y+15);
			}
		}

	}

	//���Ʊ�־
	if (m_bWaitConceal==true)
	{
		//�滭��־
		int nImageWidth=m_ImageWaitConceal.GetWidth();
		int nImageHegith=m_ImageWaitConceal.GetHeight();
		m_ImageWaitConceal.TransDrawImage(pDC,(nWidth-nImageWidth)/2,(nHeight-m_SizeStation.cy-nImageHegith)/2-50,RGB(255,0,255));
	}

	//���ֱ�־
	if (m_bLastTurnCard==true)
	{

		//�滭��־
		int nImageWidth=m_ImageLastTurn.GetWidth();
		int nImageHegith=m_ImageLastTurn.GetHeight();
		m_ImageLastTurn.TransDrawImage(pDC,(nWidth-nImageWidth)/2,(nHeight-m_SizeStation.cy-nImageHegith)/2-65,RGB(255,0,255));
	}
	//�÷��˿�
	if(m_CardScore.GetCardCount()>0)
	{
		int nXPos = 10;
		int nYPos = 130;

		CSize SizeScoreCard(m_ImageScoreCard.GetWidth()/3,m_ImageScoreCard.GetHeight()/5);

		DWORD bCardCount=m_CardScore.GetCardCount();
		tagCardItem pCardItem[48]={};
		m_CardScore.GetCardData(pCardItem,bCardCount);

		//�滭�˿�
		for (WORD i=0;i<bCardCount;i++)
		{
			BYTE bCard=pCardItem[i].cbCardData;
			if(bCard==0) continue;

			//��ȡλ��
			INT nXImagePos=0;
			INT nYImagePos=0;

			//��ȡλ��
			switch (bCard&0x0F)
			{
			case 5:
				{
					nXImagePos=0;
					nYImagePos=((bCard&0xF0)>>4)*SizeScoreCard.cy;
					break;
				}
			case 10:
				{
					nXImagePos=SizeScoreCard.cx;
					nYImagePos=((bCard&0xF0)>>4)*SizeScoreCard.cy;
					break;
				}
			case 13:
				{
					nXImagePos=SizeScoreCard.cx*2;
					nYImagePos=((bCard&0xF0)>>4)*SizeScoreCard.cy;
					break;
				}
			}

			//�滭�˿�
			INT nXPos1=0;
			INT nYPos1=0;

			nXPos1=nXPos+14*(i%10)+3;
			nYPos1=nYPos+25*(i/10)+3;				

			m_ImageScoreCard.TransDrawImage(pDC,nXPos1,nYPos1,SizeScoreCard.cx,SizeScoreCard.cy,nXImagePos,nYImagePos,RGB(17,66,87));
		}
	}

	return;
}


//�û�˳��
void CGameClientView::SetUserOrder(bool bDeasilOrder)
{
	//״̬�ж�
	if (m_bDeasilOrder==bDeasilOrder) return;

	//���ñ���
	m_bDeasilOrder=bDeasilOrder;
	//AfxGetApp()->WriteProfileInt(TEXT("GameOption"),TEXT("DeasilOrder"),m_bDeasilOrder?TRUE:FALSE);

	//���ý���
	CRect rcClient;
	GetClientRect(&rcClient);
	RectifyControl(rcClient.Width(),rcClient.Height());

	//ˢ�½���
	InvalidGameView(0,0,0,0);
	return;
}

//���õ÷�
void CGameClientView::SetCardScore(WORD wCardScore)
{
	//�����û�
	if (wCardScore!=m_wCardScore)
	{
		//���ñ���
		m_wCardScore=wCardScore;

		//���½���
		InvalidGameView(0,0,0,0);
	}

	return;
}

//ׯ���û�
void CGameClientView::SetBankerUser(WORD wBankerUser)
{
	//�����û�
	if (wBankerUser!=m_wBankerUser)
	{
		//���ñ���
		m_wBankerUser=wBankerUser;

		//���½���
		InvalidGameView(0,0,0,0);
	}

	return;
}

//������ʾ
void CGameClientView::SetWaitConceal(bool bWaitConceal)
{
	//���ñ���
	if (bWaitConceal!=m_bWaitConceal)
	{
		//���ñ���
		m_bWaitConceal=bWaitConceal;

		//���½���
		InvalidGameView(0,0,0,0);
	}

	return;
}

//��������
void CGameClientView::SetLastTurnCard(bool bLastTurnCard)
{
	//���ñ���
	if (bLastTurnCard!=m_bLastTurnCard)
	{
		//���ñ���
		m_bLastTurnCard=bLastTurnCard;

		//���½���
		InvalidGameView(0,0,0,0);
	}

	return;
}

//��������
void CGameClientView::SetValueOrder(BYTE cbMainValue, BYTE cbValueMySelf, BYTE cbValueOther)
{
	//���ñ���
	m_cbMainValue=cbMainValue;
	m_cbValueOrder[0]=cbValueMySelf;
	m_cbValueOrder[1]=cbValueOther;

	//���½���
	InvalidGameView(0,0,0,0);

	return;
}

//��������
void CGameClientView::SetCallCardInfo(WORD wCallCardUser, BYTE cbCallColor, BYTE cbCallCount)
{
	//���ñ���
	m_cbCallColor=cbCallColor;
	m_cbCallCount=cbCallCount;
	m_wCallCardUser=wCallCardUser;

	//���½���
	InvalidGameView(0,0,0,0);

	return;
}

//��������
void CGameClientView::DrawTextString(CDC * pDC, LPCTSTR pszString, COLORREF crText, COLORREF crFrame, int nXPos, int nYPos,int nWidth, int nWeight,UINT nFormat)
{
	//��������
	int nStringLength=lstrlen(pszString);
	int nXExcursion[8]={1,1,1,0,-1,-1,-1,0};
	int nYExcursion[8]={-1,0,1,1,1,0,-1,-1};

	//�滭�߿�

	for (int i=0;i<CountArray(nXExcursion);i++)
	{
		CDFontEx::DrawText(this, pDC, nWidth, nWeight,pszString,nXPos+nXExcursion[i],nYPos+nYExcursion[i], crFrame, nFormat);
	}

	//�滭����
	CDFontEx::DrawText(this, pDC, nWidth, nWeight,pszString,nXPos,nYPos, crText, nFormat);

	return;
}


//�����Ϣ
void CGameClientView::OnRButtonUp(UINT nFlags, CPoint Point)
{
	__super::OnRButtonUp(nFlags, Point);

	//������Ϣ
	SendEngineMessage(IDM_OUT_CARD,1,1);

	return;
}

//���˫��
void CGameClientView::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	__super::OnLButtonDblClk(nFlags, point);

	//�����˿�
	m_btOutCard.EnableWindow(FALSE);
	m_btSendConceal.EnableWindow(FALSE);
	m_HandCardControl.ShootAllCard(false);

	return;
}
//�ڿӷ���
void CGameClientView::SetLandScore(WORD wChairID, WORD bLandScore)
{
	//���ñ���
	if (wChairID!=INVALID_CHAIR) m_bScore[wChairID]=bLandScore;
	else ZeroMemory(m_bScore,sizeof(m_bScore));

	//���½���
	InvalidGameView(0,0,0,0);

	return;
}
//���ýз�
void CGameClientView::SetCurrentCallScore(WORD wCurrentCallScore)
{
	m_wCurrentCallScore=wCurrentCallScore;
	m_wUserCallScore=m_wCurrentCallScore+5;
}

//��ҷ���
void CGameClientView::SetPlayerScore(int wPlayerScore[])
{
	CopyMemory(m_wPlayerScore,wPlayerScore,sizeof(m_wPlayerScore));

	//���½���
	InvalidGameView(0,0,0,0);
	return;
}
//�����з�
void CGameClientView::SetGiveUpPlayer(bool bGiveUpPlayer[])
{
	CopyMemory(m_bGiveUpPlayer,bGiveUpPlayer,sizeof(bGiveUpPlayer));

	//���½���
	InvalidGameView(0,0,0,0);
	return;
}
//�����з�
void CGameClientView::SetUserScore(WORD wUserScore[])
{
	CopyMemory(m_bScore,wUserScore,sizeof(m_bScore));

	//���½���
	InvalidGameView(0,0,0,0);
	return;
}
//��������
void CGameClientView::SetMainCount(BYTE bMainCount[4],BYTE bDoubleCount[4])
{
	if(bMainCount==NULL && bDoubleCount==NULL)
	{
		ZeroMemory(m_bMainCount,sizeof(m_bMainCount));
		ZeroMemory(m_bDoubleCount,sizeof(m_bDoubleCount));

		//���½���
		InvalidGameView(0,0,0,0);
		return;
	}
	CopyMemory(m_bMainCount,bMainCount,sizeof(bMainCount));
	CopyMemory(m_bDoubleCount,bDoubleCount,sizeof(bDoubleCount));

	//���½���
	InvalidGameView(0,0,0,0);
	return;
}
//��������
void CGameClientView::SetNoMainUser(bool bNoMainUser[4])
{
	CopyMemory(m_bUserNoMain,bNoMainUser,sizeof(m_bUserNoMain));
	//���½���
	InvalidGameView(0,0,0,0);
	return;
}
//////////////////////////////////////////////////////////////////////////
VOID CGameClientView::DrawNumberString(CDC * pDC, LONGLONG lNumber, INT nXPos, INT nYPos)
{
	INT nNumberHeight=m_ImageNumber.GetHeight();
	INT nNumberWidth=m_ImageNumber.GetWidth()/10;

	//������Ŀ
	INT lNumberCount=0;
	LONGLONG lNumberTemp=lNumber;
	do
	{
		lNumberCount++;
		lNumberTemp/=10;
	} while (lNumberTemp>0);

	//λ�ö���
	INT nYDrawPos=nYPos-nNumberHeight/2;
	INT nXDrawPos=nXPos+lNumberCount*nNumberWidth/2-nNumberWidth;

	//�滭����
	for (LONGLONG i=0;i<lNumberCount;i++)
	{
		//�滭����
		INT lCellNumber=lNumber%10;
		m_ImageNumber.DrawImage(pDC,nXDrawPos,nYDrawPos,nNumberWidth,nNumberHeight,lCellNumber*nNumberWidth,0);

		//���ñ���
		lNumber/=10;
		nXDrawPos-=nNumberWidth;
	};

	return;
}
void CGameClientView::SetGMCard()
{	
	int m_nYBorder=0,m_nXBorder=0,m_nYFace=50,m_nXFace=50;
	LONGLONG nWidth=m_lWidth;
	LONGLONG nHeight=m_lHeight;
	m_UserCardControl[0].SetBenchmarkPos(nWidth/2,m_nYBorder+m_nYFace+185,enXCenter,enYTop);
	m_UserCardControl[1].SetBenchmarkPos(m_nXBorder+m_nXFace+180,nHeight/2-71,enXLeft,enYCenter);
	m_UserCardControl[2].SetBenchmarkPos(nWidth/2,nHeight-258-m_nYBorder,enXCenter,enYBottom);
	m_UserCardControl[3].SetBenchmarkPos(nWidth-m_nXFace-m_nXBorder-180,nHeight/2-71,enXRight,enYCenter);
	return;
}
//////////////////////////////////////////////////////////////////////////////////
