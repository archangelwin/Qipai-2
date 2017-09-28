#include "StdAfx.h"
#include "Resource.h"
#include "GameClientView.h"

//////////////////////////////////////////////////////////////////////////
//��ť��ʶ 

#define IDC_START						100								//��ʼ��ť
#define IDC_ONE_SCORE					101								//1 �ְ�ť
#define IDC_TWO_SCORE					102								//2 �ְ�ť
#define IDC_THREE_SCORE					103								//3 �ְ�ť
#define IDC_GIVE_UP_SCORE				104								//������ť
#define IDC_OUT_CARD					105								//���ư�ť
#define IDC_PASS_CARD					106								//PASS��ť
#define IDC_AUTO_OUTCARD                107                             //��ʾ��Ŧ
#define IDC_AUTOPLAY_ON					108								//�йܰ�ť
#define IDC_AUTOPLAY_OFF				109								//ȡ����ť
#define IDC_SORT_CARD			        110                             //�йܰ�Ŧ
#define IDC_SCORE						111								//��ְ�ť
//
#define IDC_MING_CARD_STATE				112								//���ƿ�ʼ
#define IDC_MING_CARD					113								//����
#define IDC_QIANG_LAND					114								//������
#define IDC_NO_QIANG_LAND				115								//��������
#define IDC_MING_OUT					116								//��������
#define IDC_ADD							117								//�ӱ�
#define IDC_NOADD						118								//���ӱ�

//��ը��Ŀ
#define BOMB_EFFECT_COUNT				6								//��ը��Ŀ

//��ը��ʶ
#define IDI_BOMB_EFFECT					101								//��ը��ʶ
//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CGameClientView, CGameFrameViewGDI)
	ON_WM_TIMER()
	ON_WM_CREATE()
	ON_WM_RBUTTONUP()
	ON_WM_LBUTTONDBLCLK()
	ON_BN_CLICKED(IDC_START, OnStart)
	ON_BN_CLICKED(IDC_OUT_CARD, OnOutCard)
	ON_BN_CLICKED(IDC_PASS_CARD, OnPassCard)
	ON_BN_CLICKED(IDC_ONE_SCORE, OnOneScore)
	ON_BN_CLICKED(IDC_TWO_SCORE, OnTwoScore)
	ON_BN_CLICKED(IDC_THREE_SCORE, OnThreeScore)
	ON_BN_CLICKED(IDC_AUTO_OUTCARD, OnAutoOutCard)
	ON_BN_CLICKED(IDC_GIVE_UP_SCORE, OnGiveUpScore)
	ON_BN_CLICKED(IDC_SCORE, OnBnClickedScore)
	ON_BN_CLICKED(IDC_AUTOPLAY_ON, OnAutoPlayerOn)
	ON_BN_CLICKED(IDC_AUTOPLAY_OFF,OnAutoPlayerOff)
	ON_BN_CLICKED(IDC_SORT_CARD, OnBnClickedSortCard)
	//
	ON_BN_CLICKED(IDC_MING_CARD_STATE,OnMingCardState)
	ON_BN_CLICKED(IDC_MING_CARD,OnMingCard)
	ON_BN_CLICKED(IDC_QIANG_LAND,OnQiangLand)
	ON_BN_CLICKED(IDC_NO_QIANG_LAND,OnNoQiangLand)
	ON_BN_CLICKED(IDC_MING_OUT,OnMingOut)
	ON_BN_CLICKED(IDC_ADD,OnAdd)
	ON_BN_CLICKED(IDC_NOADD,OnNoAdd)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//���캯��
CGameClientView::CGameClientView(void)
{
	//��Ϸ����
	m_lBaseScore=0;
	m_wBombTime=1;
	m_cbLandScore=0;
	m_wLandUser=INVALID_CHAIR;
	m_bLaiZiCard=0;
	m_bLaiZiDisplay=false;
	m_bRandCardX=0;
	m_bRandCardY=0;

	//״̬����
	m_bShowScore=false;
	m_bLandTitle=false;
	m_bLaiZi=false;
	memset(m_bPass,0,sizeof(m_bPass));
	memset(m_bScore,0,sizeof(m_bScore));
	memset(m_bCardCount,0,sizeof(m_bCardCount));
	memset(m_bUserTrustee, 0, sizeof( m_bUserTrustee ) );
	memset(m_bAddStates, false, sizeof( m_bAddStates ) );
	memset(m_bAdd,false,sizeof(m_bAdd));

	//��ը����
	m_bBombEffect=false;
	m_cbBombFrameIndex=0;

	//λ����Ϣ
	memset(m_ptScore,0,sizeof(m_ptScore));
	memset(m_ptLand,0,sizeof(m_ptLand));
	m_bDeasilOrder = false;

	ZeroMemory(m_pHistoryScore,sizeof(m_pHistoryScore));

	m_nXTimer=65;
	m_nYTimer=69;

	return;
}

//��������
CGameClientView::~CGameClientView(void)
{
}

//������Ϣ
int CGameClientView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (__super::OnCreate(lpCreateStruct)==-1) return -1;

	//������Դ
	HINSTANCE hInstance=AfxGetInstanceHandle();
	m_ImageBomb.LoadFromResource(hInstance,IDB_BOMB);
	m_ImageBack.LoadFromResource(hInstance,IDB_VIEW_BACK);
	m_ImageCenter.LoadFromResource(hInstance,IDB_VIEW_CENTER);
	m_ImageBombEffect.LoadFromResource(hInstance,IDB_BOMB_EFFECT);
	m_ImageHistoryScore.LoadFromResource(hInstance,IDB_HISTORY_SCORE);
	m_ImageLaiZiCard.LoadFromResource(hInstance,IDB_CARD);

	m_ImageScore.LoadImage(hInstance, TEXT( "SCORE" ) );
	m_ImageLand.LoadImage(hInstance, TEXT( "BANKER" ) );
	m_ImageUserTrustee.LoadImage(hInstance,  TEXT("USER_TRUSTEE"));
	m_ImageDouble.LoadImage(hInstance,TEXT("DOUBLE"));
	m_ImageNumber.LoadImage(hInstance,TEXT("NUMBER"));
	m_ImagePlayerRL.LoadImage(hInstance,TEXT("PLAYER_RL"));
	m_ImagePlayerTB.LoadImage(hInstance,TEXT("PLAYER_TB"));
	m_ImageLaiZi.LoadImage(hInstance,TEXT("LAIZI_KUANG"));

	//����λ��
	m_LandSize.cx=m_ImageLand.GetWidth()/2;
	m_LandSize.cy=m_ImageLand.GetHeight();

	//��ȡ��С
	m_sizeHistory.SetSize(m_ImageHistoryScore.GetWidth(),m_ImageHistoryScore.GetHeight());

	//�����ؼ�
	CRect CreateRect(0,0,0,0);

	//�����˿�
	for (WORD i=0;i<3;i++)
	{
		//�û��˿�
		m_UserCardControl[i].SetDirection(true);
		m_UserCardControl[i].SetDisplayFlag(true);	
		m_UserCardControl[i].SetCardSpace( 16, 0, 0 );

		m_UserCardControl[i].Create(NULL,NULL,WS_VISIBLE|WS_CHILD,CreateRect,this,20+i);

		//�û��˿�
		if (i!=1)
		{
			m_HandCardControl[i].SetCardSpace(0,18,0);
			m_HandCardControl[i].SetDirection(false);
			m_HandCardControl[i].SetDisplayFlag(false);
			m_HandCardControl[i].Create(NULL,NULL,WS_VISIBLE|WS_CHILD,CreateRect,this,30+i);
		}
		else
		{
			m_HandCardControl[i].SetDirection(true);
			m_HandCardControl[i].SetDisplayFlag(false);
			m_HandCardControl[i].Create(NULL,NULL,WS_VISIBLE|WS_CHILD,CreateRect,this,30+i);
		}
	}

	//�����˿�
	m_BackCardControl.SetCardSpace(85,0,0);
	m_BackCardControl.SetDisplayFlag(false);
	m_HandCardControl[1].SetSinkWindow(AfxGetMainWnd());
	m_BackCardControl.Create(NULL,NULL,WS_VISIBLE|WS_CHILD,CreateRect,this,41);

	//������ť
	m_btStart.Create(TEXT(""),WS_CHILD,CreateRect,this,IDC_START);
	m_btScore.Create(NULL,WS_CHILD,CreateRect,this,IDC_SCORE);
	m_btOutCard.Create(TEXT(""),WS_CHILD,CreateRect,this,IDC_OUT_CARD);
	m_btPassCard.Create(TEXT(""),WS_CHILD,CreateRect,this,IDC_PASS_CARD);
	m_btOneScore.Create(TEXT(""),WS_CHILD,CreateRect,this,IDC_ONE_SCORE);
	m_btTwoScore.Create(TEXT(""),WS_CHILD,CreateRect,this,IDC_TWO_SCORE);
	m_btGiveUpScore.Create(TEXT(""),WS_CHILD,CreateRect,this,IDC_GIVE_UP_SCORE);
	m_btAutoOutCard.Create(TEXT(""),WS_CHILD,CreateRect,this,IDC_AUTO_OUTCARD);
	m_btThreeScore.Create(TEXT(""),WS_CHILD,CreateRect,this,IDC_THREE_SCORE);
	m_btAutoPlayOn.Create(TEXT(""), WS_CHILD,CreateRect,this,IDC_AUTOPLAY_ON);
	m_btAutoPlayOff.Create(TEXT(""),WS_CHILD,CreateRect,this,IDC_AUTOPLAY_OFF);
	m_btSortCard.Create(NULL,WS_CHILD|WS_DISABLED|WS_VISIBLE,CreateRect,this,IDC_SORT_CARD);
	//
	m_btMingCardState.Create(NULL,WS_CHILD,CreateRect,this,IDC_MING_CARD_STATE);
    m_btMingCard.Create(NULL,WS_CHILD,CreateRect,this,IDC_MING_CARD);
	m_btQiangLand.Create(NULL,WS_CHILD,CreateRect,this,IDC_QIANG_LAND);
	m_btNoQiang.Create(NULL,WS_CHILD,CreateRect,this,IDC_NO_QIANG_LAND);
	m_btMingOut.Create(NULL,WS_CHILD,CreateRect,this,IDC_MING_OUT);
	m_btAdd.Create(NULL,WS_CHILD,CreateRect,this,IDC_ADD);
	m_btNoAdd.Create(NULL,WS_CHILD,CreateRect,this,IDC_NOADD);

	//���ð�ť
	m_btStart.SetButtonImage(IDB_START,hInstance,false,false);
	m_btScore.SetButtonImage(IDB_BT_SHOW_SCORE,hInstance,false,false);
	m_btOutCard.SetButtonImage(IDB_OUT_CARD,hInstance,false,false);
	m_btPassCard.SetButtonImage(IDB_PASS,hInstance,false,false);
	m_btOneScore.SetButtonImage(IDB_ONE_SCORE,hInstance,false,false);
	m_btTwoScore.SetButtonImage(IDB_TWO_SCORE,hInstance,false,false);
	m_btGiveUpScore.SetButtonImage(IDB_GIVE_UP,hInstance,false,false);
	m_btAutoOutCard.SetButtonImage(IDB_AUTO_OUT_CARD,hInstance,false,false);
	m_btThreeScore.SetButtonImage(IDB_THREE_SCORE,hInstance,false,false);
	m_btAutoPlayOn.SetButtonImage  (IDB_AUTOPLAY_ON,hInstance,false,false);
	m_btAutoPlayOff.SetButtonImage (IDB_AUTOPLAY_OFF,hInstance,false,false);
	m_btSortCard.SetButtonImage(IDB_COUNT_SORT,hInstance,false,false);
	//
	m_btMingCardState.SetButtonImage(IDB_MING_CARD_STATE,hInstance,false,false);
	m_btMingCard.SetButtonImage(IDB_MING_CARD,hInstance,false,false);
	m_btQiangLand.SetButtonImage(IDB_QIANG_LAND,hInstance,false,false);
	m_btNoQiang.SetButtonImage(IDB_GIVE_UP,hInstance,false,false);
	m_btMingOut.SetButtonImage(IDB_MING_CARD,hInstance,false,false);
	m_btAdd.SetButtonImage(IDB_ADD,hInstance,false,false);
	m_btNoAdd.SetButtonImage(IDB_NOADD,hInstance,false,false);

    m_btAutoPlayOn.ShowWindow(SW_SHOW);
    m_btAutoPlayOff.ShowWindow(SW_HIDE);

#ifdef VIDEO_GAME

	//������Ƶ
	for (WORD i=0; i<GAME_PLAYER; i++)
	{
		//������Ƶ
		m_DlgVedioService[i].Create(NULL,NULL,WS_CHILD|WS_VISIBLE,CreateRect,this,200+i);
		m_DlgVedioService[i].InitVideoService(i==1,i==1);

		//������Ƶ
		m_VedioServiceManager.SetVideoServiceControl(i,&m_DlgVedioService[i]);
	}

	m_HandCardControl[ 0 ].ShowWindow( SW_HIDE );
	m_HandCardControl[ 2 ].ShowWindow( SW_HIDE );

#endif

	return 0;
}

//�����Ϣ
void CGameClientView::OnRButtonUp(UINT nFlags, CPoint Point)
{
	__super::OnRButtonUp(nFlags, Point);

	//�����˿�
		//�����˿�
	m_btOutCard.EnableWindow(FALSE);
	m_HandCardControl[1].ShootAllCard(false);

//	//text
//#ifdef _DEBUG
//	m_UserCardControl[0].SetLaiZivalue(9);
//	m_UserCardControl[1].SetLaiZivalue(9);
//	m_UserCardControl[2].SetLaiZivalue(9);
//
//	//BYTE bCard[5]={0x03,0x04,0x09,0x06,0x07};
//	//BYTE bCard1[5]={0x03,0x04,0x05,0x06,0x07};
//	BYTE bCard[2]={0x09};
//	BYTE bCard1[2]={0x09};
//	m_UserCardControl[0].SetNoChangeCard(bCard,1);
//	m_UserCardControl[0].SetCardData(bCard1,1);
//	//end
//#endif 

	return;
}

//���˫��
void CGameClientView::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	__super::OnLButtonDblClk(nFlags, point);

	//�����˿�
	m_btOutCard.EnableWindow(FALSE);
	m_HandCardControl[1].ShootAllCard(false);

	return;
}
//���ְ�ť
VOID CGameClientView::OnBnClickedScore()
{
	//���ñ���
	m_bShowScore=!m_bShowScore;

	//���ð�ť
	HINSTANCE hResInstance=AfxGetInstanceHandle();
	m_btScore.SetButtonImage((m_bShowScore==true)?IDB_BT_CLOSE_SCORE:IDB_BT_SHOW_SCORE,hResInstance,false,false);

	//���½���
	RefreshGameView();

	return;
}
//���ý���
void CGameClientView::ResetGameView()
{
	//��Ϸ����
	m_lBaseScore=0;
	m_wBombTime=1;
	m_cbLandScore=0;
	m_wLandUser=INVALID_CHAIR;
	m_bLaiZiCard=0;
	m_bLaiZiDisplay=false;
	m_bRandCardX=0;
	m_bRandCardY=0;

	//��ը����
	m_bBombEffect=false;
	m_cbBombFrameIndex=0;

	//״̬����
	m_bShowScore=false;
	m_bLandTitle=false;
	m_bLaiZi=false;
	memset(m_bPass,0,sizeof(m_bPass));
	memset(m_bScore,0,sizeof(m_bScore));
	memset(m_bCardCount,0,sizeof(m_bCardCount));
	memset(m_bUserTrustee, 0, sizeof( m_bUserTrustee ) );
    ZeroMemory(m_pHistoryScore,sizeof(m_pHistoryScore));
	memset(m_bAddStates, false, sizeof( m_bAddStates ) );
	memset(m_bAdd,false,sizeof(m_bAdd));

	//���ؿؼ�
	if(m_ScoreView.m_hWnd) m_ScoreView.ShowWindow(SW_HIDE);

	//���ذ�ť
	m_btStart.ShowWindow(SW_HIDE);
	m_btScore.ShowWindow(SW_HIDE);
	m_btOutCard.ShowWindow(SW_HIDE);
	m_btPassCard.ShowWindow(SW_HIDE);
	m_btGiveUpScore.ShowWindow(SW_HIDE);
	m_btAutoOutCard.ShowWindow(SW_HIDE);
	m_btThreeScore.ShowWindow(SW_HIDE);
	m_btAutoPlayOn.ShowWindow( SW_SHOW );
	m_btAutoPlayOff.ShowWindow( SW_HIDE );

	HINSTANCE hResInstance=AfxGetInstanceHandle();
	m_btScore.SetButtonImage(IDB_BT_SHOW_SCORE,hResInstance,false,false);

	//���ÿؼ�
	m_btOutCard.EnableWindow(FALSE);
	m_btPassCard.EnableWindow(FALSE);
	m_btAutoOutCard.EnableWindow(FALSE);
	m_btSortCard.EnableWindow(FALSE);
	m_btSortCard.SetButtonImage(IDB_COUNT_SORT,AfxGetInstanceHandle(),false,false);

	//�˿˿ؼ�
	m_BackCardControl.SetCardData(NULL,0);
	m_HandCardControl[1].SetPositively(false);
	m_HandCardControl[1].SetDisplayFlag(false);

	m_nXTimer=65;
	m_nYTimer=69;

	//�����˿�
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		m_UserCardControl[i].SetCardData(NULL,0);
		m_HandCardControl[i].SetCardData(NULL,0);
	}

#ifdef VIDEO_GAME
	//�����˿�
	m_HandCardControl[ 0 ].ShowWindow( SW_HIDE );
	m_HandCardControl[ 2 ].ShowWindow( SW_HIDE );
#endif

	return;
}

//�����ؼ�
VOID CGameClientView::RectifyControl(INT nWidth, INT nHeight)
{

	//˳ʱ���
	if ( m_bDeasilOrder ) {
		//��������
		m_ptAvatar[2].x=5;
		m_ptAvatar[2].y=nHeight/2-FACE_CY;
		m_ptNickName[2].x=5;
		m_ptClock[0].x=2+m_nXTimer/2;
		m_ptClock[0].y=nHeight/2-FACE_CY-5-m_nYTimer/2;
		m_ptClock[2].y=nHeight/2-FACE_CY-5-m_nYTimer;
		m_ptReady[2].x=5+m_nXTimer;
		m_ptReady[2].y=nHeight/2-FACE_CY-m_nYTimer/2-5;
		m_ptScore[2].x=FACE_CX+147-10;
		m_ptScore[2].y=nHeight/2-FACE_CY-30;
		m_ptLand[2].x=5;
		m_ptLand[2].y=nHeight/2+40;

		//��������
		m_ptAvatar[0].x=nWidth-FACE_CX-5;
		m_ptAvatar[0].y=nHeight/2-FACE_CY;
		m_ptNickName[0].x=nWidth-5;
		m_ptNickName[0].y=nHeight/2+5;
		m_ptClock[2].x=nWidth-m_nXTimer/2-2;
		m_ptClock[2].y=nHeight/2-FACE_CY-m_nYTimer/2-5;
		m_ptReady[0].x=nWidth-m_nXTimer-5;
		m_ptReady[0].y=nHeight/2-FACE_CY-m_nYTimer/2-5;
		m_ptScore[0].x=nWidth-FACE_CX-190-7;
		m_ptScore[0].y=nHeight/2-FACE_CY-30;
		m_ptLand[0].x=nWidth-m_LandSize.cx-5;
		m_ptLand[0].y=nHeight/2+40;

		//�û��˿�
		m_UserCardControl[0].SetBenchmarkPos(nWidth-FACE_CX-137,nHeight/2-40,enXRight,enYCenter);
		m_UserCardControl[2].SetBenchmarkPos(FACE_CX+137,nHeight/2-40,enXLeft,enYCenter);
		m_HandCardControl[0].SetBenchmarkPos(nWidth-FACE_CX-50,nHeight/2-50,enXRight,enYCenter);
		m_HandCardControl[2].SetBenchmarkPos(FACE_CX+50,nHeight/2-55,enXLeft,enYCenter);
	}
	else {
		//��������
		m_ptAvatar[0].x=23;
		m_ptAvatar[0].y=nHeight/2-FACE_CY-5;
		m_ptNickName[0].x=14;
		m_ptNickName[0].y=nHeight/2+15;
		m_ptClock[0].x=2+m_nXTimer/2;
		m_ptClock[0].y=nHeight/2-FACE_CY-5-m_nYTimer/2;
		m_ptReady[0].x=5+m_nXTimer;
		m_ptReady[0].y=nHeight/2-FACE_CY-m_nYTimer/2-5;
		m_ptScore[0].x=FACE_CX+147+35;
		m_ptScore[0].y=nHeight/2-FACE_CY-30;
		m_ptLand[0].x=5;
		m_ptLand[0].y=nHeight/2+40;

		//��������
		m_ptAvatar[2].x=nWidth-FACE_CX-20;
		m_ptAvatar[2].y=nHeight/2-FACE_CY-5;
		m_ptNickName[2].x=nWidth-73;
		m_ptNickName[2].y=nHeight/2+15;
		m_ptClock[2].x=nWidth-m_nXTimer/2-2;
		m_ptClock[2].y=nHeight/2-FACE_CY-m_nYTimer/2-5;
		m_ptReady[2].x=nWidth-m_nXTimer-5;
		m_ptReady[2].y=nHeight/2-FACE_CY-m_nYTimer/2-5;
		m_ptScore[2].x=nWidth-FACE_CX-190-7;
		m_ptScore[2].y=nHeight/2-FACE_CY-30;
		m_ptLand[2].x=nWidth-m_LandSize.cx-5;
		m_ptLand[2].y=nHeight/2+40;

		//�û��˿�
		m_UserCardControl[2].SetBenchmarkPos(nWidth-FACE_CX-137,nHeight/2-40,enXRight,enYCenter);
		m_UserCardControl[0].SetBenchmarkPos(FACE_CX+137,nHeight/2-40,enXLeft,enYCenter);
		m_HandCardControl[2].SetBenchmarkPos(nWidth-FACE_CX-50,nHeight/2-50,enXRight,enYCenter);
		m_HandCardControl[0].SetBenchmarkPos(FACE_CX+50,nHeight/2-55,enXLeft,enYCenter);
	}

	////////////////////////////////////////////////////////////////////////////
	//BYTE cbCardData[ 20 ] = { 0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01 };
	////
	//m_HandCardControl[ 1 ].SetCardData( cbCardData, sizeof( cbCardData ) ); 
	//m_HandCardControl[ 0 ].SetCardData( cbCardData, sizeof( cbCardData ) );
	//m_HandCardControl[ 2 ].SetCardData( cbCardData, sizeof( cbCardData ) );
	//m_UserCardControl[ 1 ].SetCardData( cbCardData, 8 ); 
	//m_UserCardControl[ 0 ].SetCardData( cbCardData, 8 );
	//m_UserCardControl[ 2 ].SetCardData( cbCardData, 8 );
	//for ( WORD wChairID = 0; wChairID < GAME_PLAYER; ++wChairID ) {
	//	m_bPass[ wChairID ] = true; 
	//}
	//if ( m_ScoreView.m_hWnd == NULL ) m_ScoreView.Create(IDD_GAME_SCORE,this);
	//m_ScoreView.ShowWindow( SW_SHOW );
	//for ( WORD wChairID = 0; wChairID < GAME_PLAYER; ++wChairID ) {
	//	m_ScoreView.SetGameScore( wChairID, "dkfjdkfjdf", 00 );
	//}
	//////////////////////////////////////////////////////////////////////////////

#ifdef VIDEO_GAME
	CRect VDRect;
	m_DlgVedioService[ 0 ].GetWindowRect( &VDRect );
	if ( m_bDeasilOrder ) {
		m_DlgVedioService[ 2 ].MoveWindow(  5, m_ptClock[ 0 ].y - VDRect.Height() - m_nYTimer, VDRect.Width(), VDRect.Height() );
		m_DlgVedioService[ 0 ].MoveWindow( nWidth - VDRect.Width()  - 5, m_ptClock[ 2 ].y - VDRect.Height() - m_nYTimer, VDRect.Width(), VDRect.Height() );

		m_HandCardControl[0].SetBenchmarkPos(nWidth-FACE_CX-140,nHeight/2-50,enXRight,enYCenter);
		m_HandCardControl[2].SetBenchmarkPos(FACE_CX+140,nHeight/2-55,enXLeft,enYCenter);
	}
	else {
		m_DlgVedioService[ 0 ].MoveWindow( 5, m_ptClock[ 0 ].y - VDRect.Height() - m_nYTimer, VDRect.Width(), VDRect.Height() );
		m_DlgVedioService[ 2 ].MoveWindow( nWidth - VDRect.Width()  - 5, m_ptClock[ 2 ].y - VDRect.Height() - m_nYTimer, VDRect.Width(), VDRect.Height() );

		m_HandCardControl[2].SetBenchmarkPos(nWidth-FACE_CX-140,nHeight/2-50,enXRight,enYCenter);
		m_HandCardControl[0].SetBenchmarkPos(FACE_CX+140,nHeight/2-55,enXLeft,enYCenter);
	}

	m_DlgVedioService[ 1 ].GetWindowRect( &VDRect );
	m_DlgVedioService[ 1 ].MoveWindow(  5, nHeight - VDRect.Height()  - 50, VDRect.Width(), VDRect.Height() );
#endif
    

	//��������
	m_ptAvatar[1].x=nWidth/2-FACE_CX/2;
	m_ptAvatar[1].y=nHeight-FACE_CY-7;
	m_ptNickName[1].x=nWidth/2+10+FACE_CX/2;
	m_ptNickName[1].y=nHeight-FACE_CY+5;
	m_ptClock[1].x=nWidth/2-FACE_CX/2-m_nXTimer/2-5;
	m_ptClock[1].y=nHeight-m_nYTimer/2-2;
	m_ptReady[1].x=nWidth/2 + 10;
	m_ptReady[1].y=nHeight-217;
	m_ptScore[1].x=nWidth/2-21;
	m_ptScore[1].y=nHeight-FACE_CY-210;
	m_ptLand[1].x=nWidth/2-FACE_CX/2-m_LandSize.cx-m_nXTimer*2;
	m_ptLand[1].y=nHeight-m_LandSize.cy+5;

	//�ƶ���ť
	CRect rcButton;
	HDWP hDwp=BeginDeferWindowPos(32);
	m_btStart.GetWindowRect(&rcButton);
	const UINT uFlags=SWP_NOACTIVATE|SWP_NOZORDER|SWP_NOCOPYBITS|SWP_NOSIZE;
	DeferWindowPos(hDwp,m_btStart,NULL,(nWidth-rcButton.Width())/2,nHeight-232,0,0,uFlags);
	DeferWindowPos(hDwp,m_btOutCard,NULL,nWidth/2-rcButton.Width()*3/2-10,nHeight-232,0,0,uFlags);
	DeferWindowPos(hDwp,m_btAutoOutCard,NULL,nWidth/2-rcButton.Width()/2,nHeight-232,0,0,uFlags);
	DeferWindowPos(hDwp,m_btPassCard,NULL,nWidth/2+rcButton.Width()/2+10,nHeight-232,0,0,uFlags);
	DeferWindowPos(hDwp,m_btOneScore,NULL,nWidth/2-rcButton.Width()*2-12,nHeight-232,0,0,uFlags);
	DeferWindowPos(hDwp,m_btTwoScore,NULL,nWidth/2-rcButton.Width()-2,nHeight-232,0,0,uFlags);
	DeferWindowPos(hDwp,m_btThreeScore,NULL,nWidth/2-rcButton.Width()*2-12,nHeight-232,0,0,uFlags);
	DeferWindowPos(hDwp,m_btGiveUpScore,NULL,nWidth/2+30+rcButton.Width()-10,nHeight-232,0,0,uFlags);
	DeferWindowPos(hDwp,m_btSortCard,NULL,nWidth-rcButton.Width()-15,nHeight-rcButton.Height()*2-40,0,0,uFlags);
	//
	DeferWindowPos(hDwp,m_btMingCardState,NULL,(nWidth-rcButton.Width())/2-rcButton.Width(),nHeight-232,0,0,uFlags);
	DeferWindowPos(hDwp,m_btMingCard,NULL,(nWidth-rcButton.Width())/2-rcButton.Width(),nHeight-232,0,0,uFlags);
	DeferWindowPos(hDwp,m_btQiangLand,NULL,(nWidth-rcButton.Width())/2-rcButton.Width(),nHeight-232,0,0,uFlags);
	DeferWindowPos(hDwp,m_btNoQiang,NULL,(nWidth-rcButton.Width())/2+rcButton.Width(),nHeight-232,0,0,uFlags);
	DeferWindowPos(hDwp,m_btMingOut,NULL,(nWidth-rcButton.Width())/2+rcButton.Width()*3,nHeight-232,0,0,uFlags);
	DeferWindowPos(hDwp,m_btAdd,NULL,(nWidth-rcButton.Width())/2-rcButton.Width(),nHeight-232,0,0,uFlags);
	DeferWindowPos(hDwp,m_btNoAdd,NULL,(nWidth-rcButton.Width())/2+rcButton.Width(),nHeight-232,0,0,uFlags);

	//��ְ�ť
	CRect rcScore;
	m_btScore.GetWindowRect(&rcScore);
	
#ifdef VIDEO_GAME
	DeferWindowPos(hDwp,m_btScore,NULL,nWidth-rcScore.Width()-m_sizeHistory.cx-2,0,0,0,uFlags);
#else
	DeferWindowPos(hDwp,m_btScore,NULL,nWidth-(rcScore.Width()+m_sizeHistory.cx)/2-40,0,0,0,uFlags);
#endif
	EndDeferWindowPos(hDwp);

	CRect rcBtAutoPlay;
	m_btAutoPlayOn.GetWindowRect( rcBtAutoPlay );
	m_btAutoPlayOn .MoveWindow( nWidth  - 5 - rcBtAutoPlay.Width() ,     nHeight-50, rcBtAutoPlay.Width(), rcBtAutoPlay.Height(), FALSE);
    m_btAutoPlayOff.MoveWindow( nWidth  - 5 - rcBtAutoPlay.Width(),     nHeight-50, rcBtAutoPlay.Width(), rcBtAutoPlay.Height(), FALSE);
	m_btSortCard.MoveWindow( nWidth  - 5 - rcBtAutoPlay.Width(), nHeight-55-rcBtAutoPlay.Height(), rcBtAutoPlay.Width(), rcBtAutoPlay.Height(), FALSE);

	//�ƶ��˿�
	m_BackCardControl.SetBenchmarkPos(nWidth/2,10,enXCenter,enYTop);
	m_HandCardControl[1].SetBenchmarkPos(nWidth/2,nHeight-FACE_CY-25,enXCenter,enYBottom);
	m_UserCardControl[1].SetBenchmarkPos(nWidth/2,nHeight-243,enXCenter,enYBottom);

	////��������
	//SetFlowerControlInfo((m_bDeasilOrder?2:0),5,nHeight/2-BIG_FACE_HEIGHT/2);
	//SetFlowerControlInfo((m_bDeasilOrder?0:2),nWidth-5-BIG_FACE_WIDTH,nHeight/2-BIG_FACE_HEIGHT/2);
	//SetFlowerControlInfo(1,nWidth/2-BIG_FACE_WIDTH,nHeight-BIG_FACE_HEIGHT-5
	//	,BIG_FACE_WIDTH, BIG_FACE_HEIGHT, 30);

	return;
}
//���û���
bool CGameClientView::SetHistoryScore(WORD wChairID,tagHistoryScore * pHistoryScore)
{
	//Ч�����
	ASSERT(wChairID<GAME_PLAYER);
	if (wChairID>=GAME_PLAYER) return false;

	//���ñ���
	m_pHistoryScore[wChairID]=pHistoryScore;

	//���½���
	RefreshGameView();

	return true;
}

//����
void CGameClientView::DrawNumber(CDC *pDC,int x,int y,int iNum)
{
	int iWidth=m_ImageNumber.GetWidth()/10;
	int iNumber=1;
	BYTE NumberSize=0;
	while(iNum >= iNumber)
	{
		if(iNum > 10000)
			m_ImageNumber.DrawImage(pDC,x+iWidth*5-NumberSize*iWidth,y,iWidth,m_ImageNumber.GetHeight(),iWidth*((iNum/iNumber)%10),0);
		else if(iNum > 1000)
			m_ImageNumber.DrawImage(pDC,x+iWidth*4-NumberSize*iWidth,y,iWidth,m_ImageNumber.GetHeight(),iWidth*((iNum/iNumber)%10),0);
		else if(iNum > 100)
			m_ImageNumber.DrawImage(pDC,x+iWidth*3-NumberSize*iWidth,y,iWidth,m_ImageNumber.GetHeight(),iWidth*((iNum/iNumber)%10),0);
		else if(iNum > 10)
			m_ImageNumber.DrawImage(pDC,x+iWidth*2-NumberSize*iWidth,y,iWidth,m_ImageNumber.GetHeight(),iWidth*((iNum/iNumber)%10),0);
		else if(iNum > 0)
			m_ImageNumber.DrawImage(pDC,x-NumberSize*iWidth,y,iWidth,m_ImageNumber.GetHeight(),iWidth*((iNum/iNumber)%10),0);

		iNumber*=10;
		NumberSize++;
	}
}
//������ͼ
void CGameClientView::RefreshGameView()
{
	CRect rect;
	GetClientRect(&rect);
	InvalidGameView(rect.left,rect.top,rect.Width(),rect.Height());

	return;
}
//�滭����
void CGameClientView::DrawGameView(CDC * pDC, int nWidth, int nHeight)
{
	//�滭����
	for ( int iW = 0 ; iW < nWidth; iW += m_ImageBack.GetWidth() )
	{
		for ( int iH = 0;  iH < nHeight; iH += m_ImageBack.GetHeight() )
		{
			m_ImageBack.BitBlt(pDC->GetSafeHdc(), iW, iH);
		}
	}
	m_ImageCenter.BitBlt( pDC->GetSafeHdc(), nWidth/2 - m_ImageCenter.GetWidth()/2, nHeight/2 - m_ImageCenter.GetHeight()/2 );


	//��ҿ�
	for(BYTE i=0;i<GAME_PLAYER;i++)
	{
		if(i == 0)
			m_ImagePlayerRL.DrawImage(pDC,m_ptAvatar[i].x-25,m_ptAvatar[i].y-5);
		else if(i == 1)
			m_ImagePlayerTB.DrawImage(pDC,m_ptAvatar[i].x-5,m_ptAvatar[i].y-18);
		else if(i == 2)
			m_ImagePlayerRL.DrawImage(pDC,m_ptAvatar[i].x-25,m_ptAvatar[i].y-5);
	}

	//�滭�û�
	TCHAR szBuffer[32];
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		//��������
		WORD wUserTimer=GetUserClock(i);
		IClientUserItem * pClientUserItem=GetClientUserItem(i);

		/////////////////////////////////////////////////////

		//tagUserData *pClientUserItem = new tagUserData;
		//strcpy( (char*)pClientUserItem->GetNickName(), "dkjfdkfjdkfj" );
		//pClientUserItem->cbUserStatus = US_READY;
		//wUserTimer = 10;
		//m_ImageLand.DrawImage(pDC,m_ptLand[i].x,m_ptLand[i].y);
		//m_wBombTime = 2;
		///////////////////////////////////////////////////////

		//�滭�û�
		if (pClientUserItem!=NULL)
		{
			//�û�����
			//pDC->SetTextAlign(TA_CENTER);
			pDC->SetTextColor((wUserTimer>0)?RGB(250,250,250):RGB(220,220,220));
			DrawTextString( pDC, pClientUserItem->GetNickName(), RGB( 255, 255, 255 ), RGB( 0, 0, 0 ), m_ptNickName[i].x,m_ptNickName[i].y-5 );

			//ʣ���˿�
		//	if (i!=1)
			{
				_sntprintf(szBuffer,sizeof(szBuffer),TEXT("ʣ�ࣺ%d ��"),m_bCardCount[i]);
				DrawTextString( pDC, szBuffer, RGB( 255, 255, 255 ), RGB( 0, 0, 0 ), m_ptNickName[i].x,m_ptNickName[i].y+18 );
			}

			//������Ϣ
			if (wUserTimer!=0) DrawUserClock(pDC,m_ptClock[i].x,m_ptClock[i].y,wUserTimer);
			if (pClientUserItem->GetUserStatus()==US_READY) DrawUserReady(pDC,m_ptReady[i].x,m_ptReady[i].y);
			DrawUserAvatar(pDC,m_ptAvatar[i].x,m_ptAvatar[i].y,pClientUserItem);
		}
	}

	//�зֱ�־
	int nXImagePos,nImageWidth=m_ImageScore.GetWidth(),nImageHeight=m_ImageScore.GetHeight();
	for (WORD i=0;i<3;i++)
	{
		if ((m_bScore[i]!=0)||(m_bPass[i]==true))
		{
			if ( m_bPass[i]==true ) nXImagePos=m_ImageScore.GetWidth()*4/5;
			else if ( m_bScore[i]==255 ) nXImagePos=m_ImageScore.GetWidth()*3/5;
			else nXImagePos=(m_bScore[i]-1)*m_ImageScore.GetWidth()/5;
			m_ImageScore.DrawImage(pDC,m_ptScore[i].x,m_ptScore[i].y,nImageWidth/5,nImageHeight,nXImagePos,0);
		}
	}

	//ׯ��λ��
	if (m_wLandUser!=INVALID_CHAIR)
	{
		//��ȡ�û�
		IClientUserItem * pClientUserItem=GetClientUserItem(m_wLandUser);
		if (pClientUserItem!=NULL)
		{
			//��������
			TCHAR szBuffer[64];

			//���� DC
			pDC->SetTextAlign(TA_LEFT|TA_TOP);
			pDC->SetTextColor(RGB(250,250,250));

			//�滭��Ϣ
			for(BYTE i=0;i<GAME_PLAYER;i++)
			{
				if(i == m_wLandUser)
					m_ImageLand.DrawImage(pDC,m_ptLand[m_wLandUser].x,m_ptLand[m_wLandUser].y,m_LandSize.cx,m_LandSize.cy,m_LandSize.cx*0,0);
				else
					m_ImageLand.DrawImage(pDC,m_ptLand[i].x,m_ptLand[i].y,m_LandSize.cx,m_LandSize.cy,m_LandSize.cx*1,0);
			}
			_sntprintf(szBuffer,sizeof(szBuffer),TEXT("������%s"),pClientUserItem->GetNickName());
			pDC->TextOut(FACE_CX+10,8,szBuffer,lstrlen(szBuffer));

			//�滭ͷ��
			DrawUserAvatar(pDC,5,5,pClientUserItem);

			//���Ʒ���
			//LONGLONG lScore=m_cbLandScore*m_lBaseScore;
			//_snprintf(szBuffer,sizeof(szBuffer),TEXT("������%I64d ��"),lScore);
			//DrawTextString( pDC, szBuffer, RGB( 255, 255, 255 ), RGB( 0, 0, 0 ), m_nXBorder+FACE_CX+10,m_nYBorder+25 );
		}
	}

	//���Ӷ���
	if(m_bLaiZi == true)
	{
		int cx=nWidth/2-m_ImageLaiZi.GetWidth()/2;
		int cy=nHeight/2-m_ImageLaiZi.GetHeight()/2;
		m_ImageLaiZi.DrawImage(pDC,cx,cy);

		int iImageWidth=m_ImageLaiZiCard.GetWidth()/13;
		int iImageHeight=m_ImageLaiZiCard.GetHeight()/5;
		m_ImageLaiZiCard.TransDrawImage(pDC,cx+30,cy+33,iImageWidth,iImageHeight,iImageWidth*m_bRandCardX,iImageHeight*m_bRandCardY,RGB(255,0,255));
	}

	//�滭��ը
	if (m_bBombEffect==true)
	{
		//�滭Ч��
		INT nImageHeight=m_ImageBombEffect.GetHeight();
		INT nImageWidth=m_ImageBombEffect.GetWidth()/BOMB_EFFECT_COUNT;
		m_ImageBombEffect.TransDrawImage(pDC,(nWidth-nImageWidth)/2,0,nImageWidth,nImageHeight,
			nImageWidth*(m_cbBombFrameIndex%BOMB_EFFECT_COUNT),0,RGB(255,0,255));
	}

	//����
	//if (m_wBombTime>1)
	{
		//ը����־
		//CImageHandle ImageHandle(&m_ImageBomb);
		//m_ImageBomb.TransDrawImage(pDC,m_nXBorder+5,nHeight-m_nYBorder-m_ImageBomb.GetHeight()-5,RGB(255,0,255));

		//���� DC
		pDC->SetTextColor(RGB(250,250,250));
		pDC->SetTextAlign(TA_LEFT|TA_TOP);

		//ը������
		//TCHAR szBuffer[64];
		//_snprintf(szBuffer,sizeof(szBuffer),TEXT("������%d"),m_wBombTime);
		//DrawTextString( pDC, szBuffer, RGB( 255, 255, 255 ), RGB( 0, 0, 0 ), m_nXBorder+FACE_CX+10,m_nYBorder+25 );

		m_ImageDouble.DrawImage( pDC,FACE_CX,25 );
		DrawNumber(pDC,FACE_CX+m_ImageDouble.GetWidth(),30,m_wBombTime);
	}

	//�����˿�
	if(m_bLaiZiDisplay && m_bLaiZiCard > 0)
	{
		//���� DC
		pDC->SetTextColor(RGB(250,250,250));
		pDC->SetTextAlign(TA_CENTER|TA_TOP);

		//ը������
		TCHAR szBuffer[64];
		CString szWord[15]={TEXT("1"),TEXT("2"),TEXT("3"),TEXT("4"),TEXT("5"),TEXT("6"),TEXT("7"),TEXT("8"),TEXT("9"),TEXT("10"),TEXT("J"),TEXT("Q"),TEXT("K"),TEXT("A"),TEXT("2")};
		_sntprintf(szBuffer,sizeof(szBuffer),TEXT("�����˿ˣ�%s"),szWord[m_bLaiZiCard-1]);
		DrawTextString( pDC, szBuffer, RGB( 255, 255, 255 ), RGB( 0, 0, 0 ), nWidth/2,120 );
	}

	//������Ϣ
	if (m_bLandTitle==true)
	{
		//������Դ
		CFont InfoFont;
		InfoFont.CreateFont(-36,0,0,0,400,0,0,0,134,3,2,1,2,TEXT("����"));

		//���� DC
		pDC->SetTextColor(RGB(201,229,133));
		pDC->SetTextAlign(TA_CENTER|TA_TOP);
		CFont * pOldFont=pDC->SelectObject(&InfoFont);

		//�滭��Ϣ
		TextOut(pDC,nWidth/2,200,TEXT("���ȴ��û��е���"));

		//������Դ
		pDC->SelectObject(pOldFont);
		InfoFont.DeleteObject();
	}

	//�ӱ���Ϣ
	for(BYTE i=0;i<GAME_PLAYER;i++)
	{
		if (m_bAdd[i]==true && m_bAddStates[i])
		{
			//������Դ
			CFont InfoFont;
			InfoFont.CreateFont(-36,0,0,0,400,0,0,0,134,3,2,1,2,TEXT("����"));

			//���� DC
			pDC->SetTextColor(RGB(201,229,133));
			pDC->SetTextAlign(TA_CENTER|TA_TOP);
			CFont * pOldFont=pDC->SelectObject(&InfoFont);

			//�滭��Ϣ
			TextOut(pDC,m_ptScore[i].x,m_ptScore[i].y,TEXT(" �ӱ�"));

			//������Դ
			pDC->SelectObject(pOldFont);
			InfoFont.DeleteObject();
		}
		else if(m_bAdd[i]==false && m_bAddStates[i])
		{
			//������Դ
			CFont InfoFont;
			InfoFont.CreateFont(-36,0,0,0,400,0,0,0,134,3,2,1,2,TEXT("����"));

			//���� DC
			pDC->SetTextColor(RGB(201,229,133));
			pDC->SetTextAlign(TA_CENTER|TA_TOP);
			CFont * pOldFont=pDC->SelectObject(&InfoFont);

			//�滭��Ϣ
			TextOut(pDC,m_ptScore[i].x,m_ptScore[i].y,TEXT("���ӱ�"));

			//������Դ
			pDC->SelectObject(pOldFont);
			InfoFont.DeleteObject();
		}
	}

	//�йܱ�ʶ
	for ( WORD wChairID = 0; wChairID < GAME_PLAYER; ++wChairID ) {
		
		IClientUserItem * pClientUserItem=GetClientUserItem( wChairID );
		if ( pClientUserItem == NULL ) m_bUserTrustee[ wChairID ] = false;

		if ( m_bUserTrustee[ wChairID ] ) {
			if ( wChairID == 1 )
				m_ImageUserTrustee.DrawImage( pDC, m_ptLand[ wChairID ].x + 40, m_ptLand[ wChairID ].y+5 );
			else if ( wChairID == 0 && !m_bDeasilOrder || wChairID == 2 && m_bDeasilOrder )
				m_ImageUserTrustee.DrawImage( pDC, m_ptAvatar[ wChairID ].x + FACE_CX + 8, m_ptAvatar[ wChairID ].y + FACE_CY -
				m_ImageUserTrustee.GetHeight() + 3 );
			else 
				m_ImageUserTrustee.DrawImage( pDC, m_ptAvatar[ wChairID ].x - FACE_CX - 8, m_ptAvatar[ wChairID ].y + FACE_CY -
				m_ImageUserTrustee.GetHeight() + 3 );
		}
	}

	//��ʷ����
	m_bShowScore=true;
	m_btScore.ShowWindow(SW_SHOW);
	if (m_bShowScore==true)
	{
		//����λ��
		INT nYBenchmark=27;
		INT nXBenchmark=nWidth-m_ImageHistoryScore.GetWidth()-40;
#ifdef VIDEO_GAME
nYBenchmark=1;
nXBenchmark=nWidth-m_ImageHistoryScore.GetWidth()-1;
m_ImageHistoryScore.BitBlt(pDC->m_hDC,nWidth-m_ImageHistoryScore.GetWidth()-1-m_nXBorder,1);
#else
m_ImageHistoryScore.BitBlt(pDC->m_hDC,nWidth-m_ImageHistoryScore.GetWidth()-40,27);
#endif
		UINT nFormat=DT_SINGLELINE|DT_END_ELLIPSIS|DT_VCENTER|DT_CENTER;

		//�滭����
		pDC->SetTextAlign(TA_LEFT|TA_TOP);
		pDC->SetTextColor(RGB(254,247,137));

		//�滭��Ϣ
		for (WORD i=0;i<GAME_PLAYER;i++)
		{
			//��ȡ�û�
			IClientUserItem * pClientUserItem=GetClientUserItem(i);
			if ((pClientUserItem==NULL)||(m_pHistoryScore[i]==NULL)) continue;

			//λ�ü���
			CRect rcAccounts(nXBenchmark+2,nYBenchmark+24+i*19,nXBenchmark+73,nYBenchmark+42+i*19);
			CRect rcTurnScore(nXBenchmark+74,nYBenchmark+24+i*19,nXBenchmark+127,nYBenchmark+42+i*19);
			CRect rcCollectScore(nXBenchmark+128,nYBenchmark+24+i*19,nXBenchmark+197,nYBenchmark+42+i*19);

			//������Ϣ
			TCHAR szTurnScore[16]=TEXT(""),szCollectScore[16]=TEXT("");
			_sntprintf(szTurnScore,CountArray(szTurnScore),TEXT("%I64d"),m_pHistoryScore[i]->lTurnScore);
			_sntprintf(szCollectScore,CountArray(szCollectScore),TEXT("%I64d"),m_pHistoryScore[i]->lCollectScore);

			//�滭��Ϣ
			TCHAR strAccounts[64];
			_sntprintf(strAccounts,CountArray(strAccounts),TEXT("%s"),pClientUserItem->GetNickName());
			pDC->DrawText(strAccounts,lstrlen(strAccounts),&rcAccounts,nFormat);
			pDC->DrawText(szTurnScore,lstrlen(szTurnScore),&rcTurnScore,nFormat);
			pDC->DrawText(szCollectScore,lstrlen(szCollectScore),&rcCollectScore,nFormat);
		}
	}
	return;
}

//ը������
void CGameClientView::SetBombTime(WORD wBombTime)
{
	//���ñ���
	m_wBombTime=wBombTime;

	//ˢ�½���
	RefreshGameView();

	return;
}

//��������
void CGameClientView::SetBaseScore(LONGLONG lBaseScore)
{
	//���ñ���
	m_lBaseScore=lBaseScore;

	//ˢ�½���
	RefreshGameView();

	return;
}

//��ʾ��ʾ
void CGameClientView::ShowLandTitle(bool bLandTitle)
{
	//���ñ���
	m_bLandTitle=bLandTitle;

	//ˢ�½���
	RefreshGameView();

	return;
}

//���÷���
void CGameClientView::SetPassFlag(WORD wChairID, bool bPass)
{
	//���ñ���
	if (wChairID==INVALID_CHAIR)
	{
		for (WORD i=0;i<GAME_PLAYER;i++) m_bPass[i]=bPass;
	}
	else m_bPass[wChairID]=bPass;

	//���½���
	RefreshGameView();

	return;
}

//�˿���Ŀ
void CGameClientView::SetCardCount(WORD wChairID, BYTE bCardCount)
{
	//���ñ���
	if (wChairID==INVALID_CHAIR)
	{
		for (WORD i=0;i<GAME_PLAYER;i++) m_bCardCount[i]=bCardCount;
	}
	else m_bCardCount[wChairID]=bCardCount;

	//���½���
	RefreshGameView();

	return;
}

//�ڿӷ���
void CGameClientView::SetLandUser(WORD wChairID, BYTE bLandScore)
{
	//���ñ���
	m_wLandUser=wChairID;
	m_cbLandScore=bLandScore;

	//���½���
	RefreshGameView();

	return;
}

//�ڿӷ���
void CGameClientView::SetLandScore(WORD wChairID, BYTE bLandScore)
{
	//���ñ���
	if (wChairID!=INVALID_CHAIR) m_bScore[wChairID]=bLandScore;
	else memset(m_bScore,0,sizeof(m_bScore));

	//���½���
	RefreshGameView();

	return;
}

//��ʼ��ť
void CGameClientView::OnStart()
{

#ifdef VIDEO_GAME
	//�����˿�
	m_HandCardControl[ 0 ].ShowWindow( SW_HIDE );
	m_HandCardControl[ 2 ].ShowWindow( SW_HIDE );
#endif

	SendEngineMessage(IDM_START,0,0);
	return;
}

//���ư�ť
void CGameClientView::OnOutCard()
{
	SendEngineMessage(IDM_OUT_CARD,1,1);
	return;
}

//������ť
void CGameClientView::OnPassCard()
{
	SendEngineMessage(IDM_PASS_CARD,1,1);
	return;
}

//�зְ�ť
void CGameClientView::OnOneScore()
{
	SendEngineMessage(IDM_LAND_SCORE,1,1);
	return;
}

//�зְ�ť
void CGameClientView::OnTwoScore()
{
	SendEngineMessage(IDM_LAND_SCORE,2,2);
	return;
}

//������ť
void CGameClientView::OnGiveUpScore()
{
	SendEngineMessage(IDM_LAND_SCORE,255,255);
	return;
}

//������ʾ
void CGameClientView::OnAutoOutCard()
{
	SendEngineMessage(IDM_AUTO_OUTCARD,0,0);
	return;
}

//�зְ�ť
void CGameClientView::OnThreeScore()
{
	SendEngineMessage(IDM_LAND_SCORE,3,3);
	return;
}
//////////////////////////////////////////////////////////////////////////

// �Զ��й�
void CGameClientView::OnAutoPlayerOn()
{
    m_btAutoPlayOn.ShowWindow(SW_HIDE);
    m_btAutoPlayOff.ShowWindow(SW_SHOW);
	SendEngineMessage(IDM_LAND_AUTOPLAY,1,1);
    return;
}

// ȡ���й�
void CGameClientView::OnAutoPlayerOff()
{
    m_btAutoPlayOn.ShowWindow(SW_SHOW);
    m_btAutoPlayOff.ShowWindow(SW_HIDE);
	SendEngineMessage(IDM_LAND_AUTOPLAY,0,0);
    return;
}

//
void CGameClientView::OnMingCardState()
{
	m_btMingCardState.ShowWindow(SW_HIDE);
	SendEngineMessage(IDM_MING_CARD_STATE,0,0);
}
//����
void CGameClientView::OnMingCard()
{
	m_btMingCard.ShowWindow(SW_HIDE);
	SendEngineMessage(IDM_MING_CARD,0,0);
}
//������
void CGameClientView::OnQiangLand()
{
	m_btQiangLand.ShowWindow(SW_HIDE);
	m_btNoQiang.ShowWindow(SW_HIDE);
	SendEngineMessage(IDM_QIANG_LAND,1,1);
}
//��������
void CGameClientView::OnNoQiangLand()
{
	m_btQiangLand.ShowWindow(SW_HIDE);
	m_btNoQiang.ShowWindow(SW_HIDE);
	SendEngineMessage(IDM_QIANG_LAND,0,0);
}
//��������
void CGameClientView::OnMingOut()
{
	m_btMingOut.ShowWindow(SW_HIDE);
	SendEngineMessage(IDM_MING_OUT,0,0);
}
//�ӱ�
void CGameClientView::OnAdd()
{
	m_btAdd.ShowWindow(SW_HIDE);
	m_btNoAdd.ShowWindow(SW_HIDE);
	SendEngineMessage(IDM_ADD,1,1);
}
//���ӱ�
void CGameClientView::OnNoAdd()
{
	m_btAdd.ShowWindow(SW_HIDE);
	m_btNoAdd.ShowWindow(SW_HIDE);
	SendEngineMessage(IDM_ADD,0,0);
}
//����˳��
void CGameClientView::SetUserOrder(bool bDeasilOrder) {

	//״̬�ж�
	if (m_bDeasilOrder==bDeasilOrder) return;

	//���ñ���
	m_bDeasilOrder=bDeasilOrder;
	AfxGetApp()->WriteProfileInt(TEXT("GameOption"),TEXT("DeasilOrder"),m_bDeasilOrder?TRUE:FALSE);

	//���ý���
	CRect rcClient;
	GetClientRect(&rcClient);

	RectifyControl(rcClient.Width(),rcClient.Height());

	//ˢ�½���
	RefreshGameView();
}

//���ñ�ը
bool CGameClientView::SetBombEffect(bool bBombEffect)
{
	if (bBombEffect==true)
	{
		//���ñ���
		m_bBombEffect=true;
		m_cbBombFrameIndex=0;

		//����ʱ��
		SetTimer(IDI_BOMB_EFFECT,100,NULL);
	}
	else
	{
		//ֹͣ����
		if (m_bBombEffect==true)
		{
			//ɾ��ʱ��
			KillTimer(IDI_BOMB_EFFECT);

			//���ñ���
			m_bBombEffect=false;
			m_cbBombFrameIndex=0;

			//���½���
			RefreshGameView();
		}
	}

	return true;
}

//ʱ����Ϣ
VOID CGameClientView::OnTimer(UINT nIDEvent)
{
	//��ը����
	if (nIDEvent==IDI_BOMB_EFFECT)
	{
		//ֹͣ�ж�
		if (m_bBombEffect==false)
		{
			KillTimer(IDI_BOMB_EFFECT);
			return;
		}

		//���ñ���
		if ((m_cbBombFrameIndex+1)>=BOMB_EFFECT_COUNT)
		{
			//ɾ��ʱ��
			KillTimer(IDI_BOMB_EFFECT);

			//���ñ���
			m_bBombEffect=false;
			m_cbBombFrameIndex=0;
		}
		else m_cbBombFrameIndex++;

		//���½���
		RefreshGameView();

		return;
	}

	__super::OnTimer(nIDEvent);
}


//��������
void CGameClientView::DrawTextString(CDC * pDC, LPCTSTR pszString, COLORREF crText, COLORREF crFrame, int nXPos, int nYPos)
{
	//��������
	int nStringLength=lstrlen(pszString);
	int nXExcursion[8]={1,1,1,0,-1,-1,-1,0};
	int nYExcursion[8]={-1,0,1,1,1,0,-1,-1};

	//�滭�߿�
	pDC->SetTextColor(crFrame);
	for (int i=0;i<CountArray(nXExcursion);i++)
	{
		pDC->TextOut(nXPos+nXExcursion[i],nYPos+nYExcursion[i],pszString,nStringLength);
	}

	//�滭����
	pDC->SetTextColor(crText);
	pDC->TextOut(nXPos,nYPos,pszString,nStringLength);

	return;
}

//����й�
void CGameClientView::SetUserTrustee( WORD wChairID, bool bTrustee ) {

	//���ñ���
	if ( INVALID_CHAIR == wChairID ) {
		for ( WORD wChairIdx = 0; wChairIdx < GAME_PLAYER; wChairIdx++ ) {
			m_bUserTrustee[ wChairIdx ] = bTrustee;
		}
	}
	else {
		m_bUserTrustee[ wChairID ] = bTrustee;
	}

	//���½���
	RefreshGameView();
}

//����ť
VOID CGameClientView::OnBnClickedSortCard()
{
	//������Ϣ
	SendEngineMessage(IDM_SORT_HAND_CARD,0,0);

	return;
}
