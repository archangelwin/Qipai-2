#include "Stdafx.h"
#include "GameOption.h"
#include "GameClient.h"
#include "GameClientDlg.h"

//////////////////////////////////////////////////////////////////////////
//�궨��

//��Ϸ��ʱ��
#define IDI_OUT_CARD					200								//���ƶ�ʱ��
#define IDI_MOST_CARD					201								//���ʱ��
#define IDI_START_GAME					202								//��ʼ��ʱ��
#define IDI_LAND_SCORE					203								//�зֶ�ʱ��
#define IDI_DISPATCH_CARD				204								//���ƶ�ʱ��
#define IDI_ADD							205								//�ӱ���ʱ��
#define IDI_QIANG_LAND					206								//��������ʱ��
#define IDI_LAIZI						207								//���Ӷ�ʱ��

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CGameClientEngine, CGameFrameEngine)
	ON_WM_TIMER()
	ON_MESSAGE(IDM_START,OnStart)
	ON_MESSAGE(IDM_OUT_CARD,OnOutCard)
	ON_MESSAGE(IDM_PASS_CARD,OnPassCard)
	ON_MESSAGE(IDM_LAND_SCORE,OnLandScore)
	ON_MESSAGE(IDM_AUTO_OUTCARD,OnAutoOutCard)
	ON_MESSAGE(IDM_LEFT_HIT_CARD,OnLeftHitCard)
	ON_MESSAGE(IDM_RIGHT_HIT_CARD,OnRightHitCard)
	ON_MESSAGE(IDM_LAND_AUTOPLAY,OnAutoPlay)
	ON_MESSAGE(IDM_SORT_HAND_CARD,OnMessageSortCard)
	//
	ON_MESSAGE(IDM_MING_CARD_STATE,OnMingCardState)
	ON_MESSAGE(IDM_MING_CARD,OnMingCard)
	ON_MESSAGE(IDM_QIANG_LAND,OnQiangLand)
	ON_MESSAGE(IDM_MING_OUT,OnMingOut)
	ON_MESSAGE(IDM_ADD,OnAdd)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//���캯��
CGameClientEngine::CGameClientEngine()
{
	//��Ϸ����
	m_wBombTime=1;
	m_bHandCardCount=0;
	m_wLandUser=INVALID_CHAIR;
	memset(m_bCardCount,0,sizeof(m_bCardCount));
	memset(m_bHandCardData,0,sizeof(m_bHandCardData));
	m_bAutoPlay = false ;
	m_cbSortType=ST_COUNT;
	m_bLaiZiData=0;
	memset(m_bMingCard,0,sizeof(m_bMingCard));
	memset(m_bHandCardUser,0,sizeof(m_bHandCardUser));

	//���ñ���
	m_dwCardHSpace=DEFAULT_PELS;

	//���Ʊ���
	m_bTurnCardCount=0;
	m_bTurnOutType=CT_ERROR;
	memset(m_bTurnCardData,0,sizeof(m_bTurnCardData));
	memset(m_bChangeCard,0,sizeof(m_bChangeCard));
	memset(m_bChangeTurnOut,0,sizeof(m_bChangeTurnOut));

	//��������
	m_wTimeOutCount=0;
	m_wMostUser=INVALID_CHAIR;
	m_wCurrentUser=INVALID_CHAIR;
	m_wTimeOutCount=0;
	m_bLaiZiFrame=0;
    
	//��������
	m_cbRemnantCardCount=0;
	m_cbDispatchCardCount=0;
	ZeroMemory(m_cbDispatchCardData,sizeof(m_cbDispatchCardData));

	return;
}

//��������
CGameClientEngine::~CGameClientEngine()
{
}

//��ʼ����
bool CGameClientEngine::OnInitGameEngine()
{
	//���ñ���
	SetWindowText(TEXT("��Ӷ�������Ϸ  --  Ver��6.0.1.0"));

	//����ͼ��
	HICON hIcon=LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDR_MAINFRAME));
	m_pIClientKernel->SetGameAttribute(KIND_ID,GAME_PLAYER,VERSION_CLIENT,hIcon,GAME_NAME);
	SetIcon(hIcon,TRUE);
	SetIcon(hIcon,FALSE);

	//��ȡ����
	m_dwCardHSpace=AfxGetApp()->GetProfileInt(TEXT("GameOption"),TEXT("CardSpace"),DEFAULT_PELS);
	m_bDeasilOrder=AfxGetApp()->GetProfileInt(TEXT("GameOption"),TEXT("DeasilOrder"),FALSE)?true:false;
	m_bAllowLookon = AfxGetApp()->GetProfileInt(TEXT("GameOption"),TEXT("AllowLookon"),FALSE)?true:false;

	//��������
	if ((m_dwCardHSpace>MAX_PELS)||(m_dwCardHSpace<LESS_PELS)) m_dwCardHSpace=DEFAULT_PELS;

	//���ÿؼ�
	m_GameClientView.m_HandCardControl[1].SetCardSpace(m_dwCardHSpace,0,20);

	////�����ɼ�
	//if (m_GameClientView.m_ScoreView.m_hWnd==NULL)
	//{
	//	m_GameClientView.m_ScoreView.Create(IDD_GAME_SCORE,&m_GameClientView);
	//}
	//m_GameClientView.m_ScoreView.ShowWindow(SW_SHOW);

	return true;
}

//���ÿ��
bool CGameClientEngine::OnResetGameEngine()
{
	//��Ϸ����
	m_wBombTime=1;
	m_bHandCardCount=0;
	m_wLandUser=INVALID_CHAIR;
	memset(m_bCardCount,0,sizeof(m_bCardCount));
	memset(m_bHandCardData,0,sizeof(m_bHandCardData));
	m_bAutoPlay = false ;
	m_cbSortType=ST_COUNT;
	m_bLaiZiData=0;
	memset(m_bMingCard,0,sizeof(m_bMingCard));
	memset(m_bHandCardUser,0,sizeof(m_bHandCardUser));

	//���Ʊ���
	m_bTurnCardCount=0;
	m_bTurnOutType=CT_ERROR;
	memset(m_bTurnCardData,0,sizeof(m_bTurnCardData));
	memset(m_bChangeCard,0,sizeof(m_bChangeCard));
	memset(m_bChangeTurnOut,0,sizeof(m_bChangeTurnOut));

	//��������
	m_wTimeOutCount=0;
	m_wMostUser=INVALID_CHAIR;
	m_wCurrentUser=INVALID_CHAIR;
	m_wTimeOutCount=0;
	m_bLaiZiFrame=0;
    
	//��������
	m_cbRemnantCardCount=0;
	m_cbDispatchCardCount=0;
	ZeroMemory(m_cbDispatchCardData,sizeof(m_cbDispatchCardData));

	//ɾ����ʱ
	KillGameClock(0);
	KillTimer(IDI_MOST_CARD);
	KillTimer(IDI_DISPATCH_CARD);
	KillTimer(IDI_LAIZI);

	//��ʷ����
	m_HistoryScore.ResetData();

	return true;
}

//��Ϸ����
void CGameClientEngine::OnGameOptionSet()
{
	////��������
	//CGameOption GameOption;
	//GameOption.m_dwCardHSpace=m_dwCardHSpace;
	//GameOption.m_bEnableSound=IsEnableSound();
	//GameOption.m_bDeasilOrder=m_GameClientView.IsDeasilOrder();
	//GameOption.m_bAllowLookOn=IsAllowUserLookon();

	////��������
	//if (GameOption.DoModal()==IDOK)
	//{
	//	//��ȡ����
	//	m_bDeasilOrder=GameOption.m_bDeasilOrder;
	//	m_dwCardHSpace=GameOption.m_dwCardHSpace;

	//	//���ÿؼ�
	//	EnableSound(GameOption.m_bEnableSound);
	//	m_GameClientView.SetUserOrder(GameOption.m_bDeasilOrder);
	//	m_GameClientView.m_HandCardControl[1].SetCardSpace(m_dwCardHSpace,0,20);
	//	AllowUserLookon(0,GameOption.m_bAllowLookOn);

	//	//��������
	//	AfxGetApp()->WriteProfileInt(TEXT("GameOption"),TEXT("CardSpace"),m_dwCardHSpace);
	//	AfxGetApp()->WriteProfileInt(TEXT("GameOption"),TEXT("DeasilOrder"),m_bDeasilOrder?TRUE:FALSE);
	//	AfxGetApp()->WriteProfileInt(TEXT("GameOption"),TEXT("AllowLookon"),m_bDeasilOrder?TRUE:FALSE);
	//	AfxGetApp()->WriteProfileInt(TEXT("GameOption"),TEXT("CardSpace"),m_dwCardHSpace);
	//}

	return;
}
//ʱ��ɾ��
bool CGameClientEngine::OnEventGameClockKill(WORD wChairID)
{
	return true;
}
//�û�����
VOID  CGameClientEngine::OnEventUserEnter(IClientUserItem * pIClientUserItem, bool bLookonUser)
{
	//���óɼ�
	if (bLookonUser==false)
	{
		//���ñ���
		m_HistoryScore.OnEventUserEnter(pIClientUserItem->GetChairID());

		//���½���
		m_GameClientView.RefreshGameView();
	}

	return;
}

//�û��뿪
VOID  CGameClientEngine::OnEventUserLeave(IClientUserItem * pIClientUserItem, bool bLookonUser)
{
	//���óɼ�
	if (bLookonUser==false)
	{
		//���ñ���
		m_HistoryScore.OnEventUserLeave(pIClientUserItem->GetChairID());

		//���½���
		m_GameClientView.RefreshGameView();
	}

	return;
}
//�û�״̬
VOID  CGameClientEngine::OnEventUserScore(IClientUserItem * pIClientUserItem, bool bLookonUser)
{
	return;
}
//�û�״̬
VOID  CGameClientEngine::OnEventUserStatus(IClientUserItem * pIClientUserItem, bool bLookonUser)
{
	////��������
	//BYTE cbUserStatus=pClientUserItem->GetUserStatus();

	////�Զ���ʼ
	//if ((pIClientUserItem!=pIMySelfUserItem)&&(cbUserStatus==US_READY)) PerformAutoStart();

	////ֹͣ��ʼ
	//if ((pIClientUserItem==pIMySelfUserItem)&&(cbUserStatus==US_READY)) KillGameClock(IDI_START_GAME);

	return;
}
//ʱ����Ϣ
bool CGameClientEngine::OnEventGameClockInfo(WORD wChairID, UINT nElapse, WORD wClockID)
{
	switch (wClockID)
	{
	case IDI_OUT_CARD:			//�û�����
		{
			//�Զ�����
			if ((((nElapse==0)||((m_bAutoPlay==true)&&(nElapse<=29))))&&(wChairID==GetMeChairID()))
			{
				//��ʱ
				if(nElapse==0)
					m_wTimeOutCount++;

				//��ʱ�ж�
				if ((IsLookonMode()==false)&&(m_bAutoPlay==false)&&((m_wTimeOutCount)>=3)) 
				{
					//״̬�л�
					m_wTimeOutCount=0;
					
					//�йܹ���
					m_GameClientView.m_btAutoPlayOn.ShowWindow(SW_HIDE);
					m_GameClientView.m_btAutoPlayOff.ShowWindow(SW_SHOW);
					if(m_pIStringMessage != NULL)
						m_pIStringMessage->InsertSystemString( TEXT( "��������γ�ʱ���л�Ϊ��ϵͳ�йܡ�ģʽ" ) );
					m_bAutoPlay = true;

					CMD_C_UserTrustee UserTrustee;
					ZeroMemory( &UserTrustee, sizeof( UserTrustee ) );
					UserTrustee.bTrustee = true;
					UserTrustee.wUserChairID = GetMeChairID();

					SendSocketData( SUB_C_TRUSTEE, &UserTrustee, sizeof( UserTrustee ) );
				}

				//�Զ���ʼ
				if ( m_GameClientView.m_btStart.IsWindowVisible() ) {
					OnStart( 0, 0 );
				}
				else {
					//�Զ�����
					OnAutoOutCard( 0, 0 ) ;
					OnOutCard( 0, 0 ) ;
				}
				return true;
			}

			//��������
			if (m_bHandCardCount<m_bTurnCardCount) return true;
			if ((nElapse<=10)&&(wChairID==GetMeChairID())&&(IsLookonMode()==false)) PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_WARN"));

			return true;
		}
	case IDI_START_GAME:		//��ʼ��Ϸ
		{
			if (nElapse==0)
			{
				if ((IsLookonMode()==false)&&(wChairID==GetMeChairID())) 
				{
					AfxGetMainWnd()->PostMessage(WM_CLOSE,0,0);
					//OnStart(0,0);
					//OnAutoPlay(1,1);

					CMD_C_UserTrustee UserTrustee;
					ZeroMemory( &UserTrustee, sizeof( UserTrustee ) );
					UserTrustee.bTrustee = true;
					UserTrustee.wUserChairID = GetMeChairID();

					SendSocketData( SUB_C_TRUSTEE, &UserTrustee, sizeof( UserTrustee ) );
				}
				return false;
			}
			if ((nElapse<=10)&&(wChairID==GetMeChairID())&&(IsLookonMode()==false)) PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_WARN"));

			return true;
		}
	case IDI_LAND_SCORE:		//�ڿӽз�
		{
			if ( nElapse==0 || ( nElapse <= 27 && m_bAutoPlay ) )
			{
				if ((IsLookonMode()==false)&&(wChairID==GetMeChairID())) 
					OnLandScore(255,255);
					//OnLandScore(3,3);
				return false;
			}
			if ((nElapse<=10)&&(wChairID==GetMeChairID())&&(IsLookonMode()==false)) PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_WARN"));

			return true;
		}
	case IDI_ADD:				//�ӱ�
		{
			if(nElapse == 0 || ( nElapse <= 27 && m_bAutoPlay ) )
			{
				if ((IsLookonMode()==false)&&(wChairID==GetMeChairID())) OnAdd(0,0);
				return false;
			}

			if ((nElapse<=10)&&(wChairID==GetMeChairID())&&(IsLookonMode()==false)) PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_WARN"));

			return true;
		}
	case IDI_QIANG_LAND:		//������
		{
			if(nElapse == 0 || ( nElapse <= 27 && m_bAutoPlay ) )
			{
				if ((IsLookonMode()==false)&&(wChairID==GetMeChairID())) 
					OnQiangLand(0,0);
				return false;
			}

			if ((nElapse<=10)&&(wChairID==GetMeChairID())&&(IsLookonMode()==false)) PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_WARN"));

			return true;
		}
	}

	return false;
}

//�Թ�״̬
bool CGameClientEngine::OnEventLookonMode(VOID * pData, WORD wDataSize)
{
	//�Թ�����
	m_GameClientView.m_HandCardControl[1].SetDisplayFlag( ( IsLookonMode() == true ) && ( IsAllowLookon() == true ) );

	return true;
}

//������Ϣ
bool CGameClientEngine::OnEventGameMessage(WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
	switch (wSubCmdID)
	{
	case SUB_S_SEND_CARD:		//�����˿�
		{
			return OnSubSendCard(pData,wDataSize);
		}
	case SUB_S_LAND_SCORE:	//�û��з�
		{
			return OnSubLandScore(pData,wDataSize);
		}
	case SUB_S_GAME_START:		//��Ϸ��ʼ
		{
			return OnSubGameStart(pData,wDataSize);
		}
	case SUB_S_OUT_CARD:		//�û�����
		{
			return OnSubOutCard(pData,wDataSize);
		}
	case SUB_S_PASS_CARD:		//��������
		{
			return OnSubPassCard(pData,wDataSize);
		}
	case SUB_S_GAME_END:		//��Ϸ����
		{
			return OnSubGameEnd(pData,wDataSize);
		}
	case SUB_C_TRUSTEE:			//����й�
		{
			//Ч������
			ASSERT(wDataSize==sizeof(CMD_C_UserTrustee));
			if (wDataSize != sizeof(CMD_C_UserTrustee)) return false;
			
			CMD_C_UserTrustee *pUserTrustee = ( CMD_C_UserTrustee * )pData;

			m_GameClientView.SetUserTrustee( SwitchViewChairID( pUserTrustee->wUserChairID ), pUserTrustee->bTrustee );

			//��ʾ�й�
			if ( pUserTrustee->wUserChairID != GetMeChairID() || IsLookonMode() ) {
				
				CString strMsg;
				if ( pUserTrustee->bTrustee )
					strMsg.Format( TEXT( "[ %s ] ���ѡ�����й�" ), GetTableUserItem( pUserTrustee->wUserChairID )->GetNickName() );
				else
					strMsg.Format( TEXT( "[ %s ] ���ȡ�����й�" ), GetTableUserItem( pUserTrustee->wUserChairID )->GetNickName() );
				if(m_pIStringMessage != NULL)
					m_pIStringMessage->InsertSystemString( strMsg );

			}
			
			return true;
		}
	case SUB_S_MING_CARD:		//����
		{
			//Ч������
			ASSERT(wDataSize==sizeof(CMD_S_MingCard));
			if (wDataSize != sizeof(CMD_S_MingCard)) return false;

			CMD_S_MingCard *pMingCard = ( CMD_S_MingCard * )pData;

			m_bMingCard[pMingCard->wMingUser]=pMingCard->bMingCardStatus;

			//���ñ���
			m_wBombTime=pMingCard->wBombTime;
			m_GameClientView.SetBombTime(m_wBombTime);

			return true;
		}
	case SUB_S_QIANG_LAND:		//������
		{
			//Ч������
			ASSERT(wDataSize==sizeof(CMD_S_QiangLand));
			if (wDataSize != sizeof(CMD_S_QiangLand)) return false;

			CMD_S_QiangLand *pQiangLand = ( CMD_S_QiangLand * )pData;

			//���ý���
			//m_GameClientView.m_btOneScore.ShowWindow(SW_HIDE);
			//m_GameClientView.m_btTwoScore.ShowWindow(SW_HIDE);
			m_GameClientView.m_btThreeScore.ShowWindow(SW_HIDE);
			m_GameClientView.m_btGiveUpScore.ShowWindow(SW_HIDE);

			if(pQiangLand->bQiangLand != 0)
				m_GameClientView.SetLandScore(INVALID_CHAIR,0);

			//���ñ���
			m_wBombTime=pQiangLand->wBombTime;
			m_GameClientView.SetBombTime(m_wBombTime);

			//����ʱ��
			KillGameClock(IDI_LAND_SCORE);
			KillGameClock(IDI_QIANG_LAND);

			if(pQiangLand->wQiangUser == GetMeChairID() && (IsLookonMode() == false))
			{
				m_GameClientView.m_btQiangLand.ShowWindow(SW_SHOW);
				m_GameClientView.m_btNoQiang.ShowWindow(SW_SHOW);
			}

			//����ʱ��
			SetGameClock(pQiangLand->wQiangUser,IDI_QIANG_LAND,30);

			return true;
		}
	case SUB_S_MING_OUT:		//��������
		{
			//Ч������
			ASSERT(wDataSize==sizeof(CMD_S_MingCard));
			if (wDataSize != sizeof(CMD_S_MingCard)) return false;

			CMD_S_MingCard *pMingCard = ( CMD_S_MingCard * )pData;

			m_bMingCard[pMingCard->wMingUser]=pMingCard->bMingCardStatus;

			m_GameClientView.m_HandCardControl[SwitchViewChairID(pMingCard->wMingUser)].SetDisplayFlag(true) ;

			//���ñ���
			m_wBombTime=pMingCard->wBombTime;
			m_GameClientView.SetBombTime(m_wBombTime);

			return true;
		}
	case SUB_S_ADD:				//�ӱ�
		{
			return OnSubAdd(pData,wDataSize);
		}
	}

	return false;
}

//��Ϸ����
bool CGameClientEngine::OnEventSceneMessage(BYTE cbGameStatus, bool bLookonUser, VOID * pData, WORD wDataSize)
{
	switch (cbGameStatus)
	{
	case GS_WK_FREE:	//����״̬
		{
			//Ч������
			if (wDataSize!=sizeof(CMD_S_StatusFree)) return false;
			CMD_S_StatusFree * pStatusFree=(CMD_S_StatusFree *)pData;

			//���ý���
			m_GameClientView.SetBaseScore(pStatusFree->lBaseScore);
			//���ƽ���
			m_GameClientView.m_btScore.ShowWindow(SW_SHOW);
			//���ÿؼ�
			if (IsLookonMode()==false)
			{
				m_GameClientView.m_btStart.ShowWindow(SW_SHOW);
				m_GameClientView.m_btStart.SetFocus();
				//
				m_GameClientView.m_btMingCardState.ShowWindow(SW_SHOW);
			}
			else
			{
				m_GameClientView.m_btAutoPlayOff.ShowWindow( SW_HIDE ) ;
				m_GameClientView.m_btAutoPlayOn.ShowWindow( SW_HIDE ) ;
				m_GameClientView.m_btAutoPlayOff.EnableWindow( FALSE ) ;
				m_GameClientView.m_btAutoPlayOn.EnableWindow( FALSE ) ;
				m_GameClientView.m_btSortCard.ShowWindow( SW_HIDE );
			}

			//�����˿�
			m_GameClientView.m_HandCardControl[1].SetDisplayFlag((IsLookonMode()==false)||(IsAllowLookon()==true));

			//����ʱ��
			SetGameClock(GetMeChairID(),IDI_START_GAME,30);

			//��ʷ�ɼ�
			for (BYTE i=0;i<GAME_PLAYER;i++)
			{
				//��ȡ����
				tagHistoryScore * pHistoryScore=m_HistoryScore.GetHistoryScore(i);

				//���û���
				pHistoryScore->lTurnScore=pStatusFree->lTurnScore[i];
				pHistoryScore->lCollectScore=pStatusFree->lCollectScore[i];

				//������
				WORD wViewChairID=SwitchViewChairID(i);
				m_GameClientView.SetHistoryScore(wViewChairID,pHistoryScore);
			}

			return true;
		}
	case GS_WK_SCORE:	//�з�״̬
		{
			//Ч������
			if (wDataSize!=sizeof(CMD_S_StatusScore)) return false;
			CMD_S_StatusScore * pStatusScore=(CMD_S_StatusScore *)pData;

			//���ñ���
			m_bHandCardCount=17;
			for (WORD i=0;i<GAME_PLAYER;i++) m_bCardCount[i]=17;
			CopyMemory(m_bHandCardData,pStatusScore->bCardData[GetMeChairID()],m_bHandCardCount);

			//���ý���
			for (WORD i=0;i<GAME_PLAYER;i++)	
			{
				WORD wViewChairID=SwitchViewChairID(i);
				m_GameClientView.SetCardCount(wViewChairID,m_bCardCount[i]);
				m_GameClientView.SetLandScore(wViewChairID,pStatusScore->bScoreInfo[i]);
			}
			m_GameClientView.ShowLandTitle(true);
			m_GameClientView.SetBaseScore(pStatusScore->lBaseScore);

			//���ƽ���
			m_GameClientView.m_btScore.ShowWindow(SW_SHOW);

			//���ñ���
			m_wBombTime=pStatusScore->wBombTime;
			m_GameClientView.SetBombTime(m_wBombTime);

			//��ť����
			if ((IsLookonMode()==false)&&(pStatusScore->wCurrentUser==GetMeChairID()))
			{
				m_GameClientView.m_btGiveUpScore.ShowWindow(SW_SHOW);
				m_GameClientView.m_btThreeScore.ShowWindow( SW_SHOW );
				m_GameClientView.m_btThreeScore.EnableWindow(pStatusScore->bLandScore<=2?TRUE:FALSE);
			}

			if ( IsLookonMode() )
			{
				m_GameClientView.m_btAutoPlayOff.ShowWindow( SW_HIDE ) ;
				m_GameClientView.m_btAutoPlayOn.ShowWindow( SW_HIDE ) ;
				m_GameClientView.m_btAutoPlayOff.EnableWindow( FALSE ) ;
				m_GameClientView.m_btAutoPlayOn.EnableWindow( FALSE ) ;
				m_GameClientView.m_btSortCard.ShowWindow( SW_HIDE );
			}
			else 
			{
				m_GameClientView.m_btSortCard.EnableWindow(TRUE);
			}

			//�����˿�
			BYTE bCardData[3]={0,0,0};
			m_GameClientView.m_BackCardControl.SetCardData(bCardData,3);
			m_GameClientView.m_HandCardControl[1].SetCardData(m_bHandCardData,m_bHandCardCount);
			m_GameClientView.m_HandCardControl[1].SetDisplayFlag((IsLookonMode()==false)||(IsAllowLookon()==true));

			for ( WORD wChairID = 0; wChairID < GAME_PLAYER; ++wChairID )
			{
				m_GameClientView.SetUserTrustee( SwitchViewChairID( wChairID ), pStatusScore->bUserTrustee[ wChairID ] );
				
				//�Լ��ж�
				if ( ! IsLookonMode() && wChairID == GetMeChairID() && pStatusScore->bUserTrustee[ wChairID ] )
				{
					//�йܹ���
					m_GameClientView.m_btAutoPlayOn.ShowWindow(SW_HIDE);
					m_GameClientView.m_btAutoPlayOff.ShowWindow(SW_SHOW);
					OnAutoPlay( true, 0 );
				}
			}

			//�����˿�
			BYTE cbCardData[20];
			ZeroMemory(cbCardData,sizeof(cbCardData));
			for(BYTE i=0;i<GAME_PLAYER;i++)
			{
				if(i == GetMeChairID())continue;
				if(pStatusScore->bMingCardStatus[i] != 0)
				{
					m_GameClientView.m_HandCardControl[SwitchViewChairID(i)].SetCardData(pStatusScore->bCardData[i],17);
					m_GameClientView.m_HandCardControl[SwitchViewChairID(i)].SetDisplayFlag((IsLookonMode()==false)||(IsAllowLookon()==true));
				}
				else
				{
					m_GameClientView.m_HandCardControl[SwitchViewChairID(i)].SetCardData(cbCardData,17);
				}
			}

			//����ʱ��
			SetGameClock(pStatusScore->wCurrentUser,IDI_LAND_SCORE,30);

			//��ʷ�ɼ�
			for (BYTE i=0;i<GAME_PLAYER;i++)
			{
				//��ȡ����
				tagHistoryScore * pHistoryScore=m_HistoryScore.GetHistoryScore(i);

				//���û���
				pHistoryScore->lTurnScore=pStatusScore->lTurnScore[i];
				pHistoryScore->lCollectScore=pStatusScore->lCollectScore[i];

				//������
				WORD wViewChairID=SwitchViewChairID(i);
				m_GameClientView.SetHistoryScore(wViewChairID,pHistoryScore);
			}

			return true;
		}
	case GS_WK_PLAYING:		//��Ϸ״̬
		{
			//Ч������
			if (wDataSize!=sizeof(CMD_S_StatusPlay)) return false;
			CMD_S_StatusPlay * pStatusPlay=(CMD_S_StatusPlay *)pData;

			//���ñ���
			m_bTurnCardCount=pStatusPlay->bTurnCardCount;
			m_bHandCardCount=pStatusPlay->bCardCount[GetMeChairID()];
			m_bTurnOutType=m_GameLogic.TransListToInt(m_bTurnOutType,m_GameLogic.GetCardType(pStatusPlay->bTurnCardData,m_bTurnCardCount));
			CopyMemory(m_bHandCardData,pStatusPlay->bCardData[GetMeChairID()],m_bHandCardCount);
			CopyMemory(m_bTurnCardData,pStatusPlay->bTurnCardData,pStatusPlay->bTurnCardCount);

			//���ý���
			for (BYTE i=0;i<GAME_PLAYER;i++)
			{
				WORD wViewChairID=SwitchViewChairID(i);
				m_bCardCount[i]=pStatusPlay->bCardCount[i];
				m_GameClientView.SetCardCount(wViewChairID,pStatusPlay->bCardCount[i]);
			}

			//������
			m_GameLogic.SetLaiZiCard(pStatusPlay->bLaiZiCard);
			m_GameClientView.SetLaiZiCard(m_GameLogic.GetCardLogicValue(pStatusPlay->bLaiZiCard));
			m_GameClientView.SetLaiZiDisplay(true);

			//���ƽ���
			m_GameClientView.m_btScore.ShowWindow(SW_SHOW);
			m_GameClientView.SetBombTime(pStatusPlay->wBombTime);
			m_GameClientView.SetBaseScore(pStatusPlay->lBaseScore);
			m_GameClientView.m_BackCardControl.SetCardData(pStatusPlay->bBackCard,3);
			m_GameClientView.m_BackCardControl.SetDisplayFlag( true );
			m_GameClientView.m_HandCardControl[1].SetCardData(m_bHandCardData,m_bHandCardCount);
			m_GameClientView.SetLandUser(SwitchViewChairID(pStatusPlay->wLandUser),pStatusPlay->bLandScore);

			//�������
			if ((IsLookonMode()==false)&&(pStatusPlay->wCurrentUser==GetMeChairID()))
			{
				m_GameClientView.m_btOutCard.EnableWindow(FALSE);
				m_GameClientView.m_btOutCard.ShowWindow(SW_SHOW);
				m_GameClientView.m_btPassCard.ShowWindow(SW_SHOW);
				m_GameClientView.m_btAutoOutCard.ShowWindow(SW_SHOW);
				m_GameClientView.m_btPassCard.EnableWindow((m_bTurnCardCount!=0)?TRUE:FALSE);
				m_GameClientView.m_btAutoOutCard.EnableWindow(TRUE);
			}

			//��������
			if (m_bTurnCardCount!=0)
			{
				WORD wViewChairID=SwitchViewChairID(pStatusPlay->wLastOutUser);
				m_GameClientView.m_UserCardControl[wViewChairID].SetCardData(m_bTurnCardData,m_bTurnCardCount);
			}

			if ( IsLookonMode() )
			{
				m_GameClientView.m_btAutoPlayOff.ShowWindow( SW_HIDE ) ;
				m_GameClientView.m_btAutoPlayOn.ShowWindow( SW_HIDE ) ;
				m_GameClientView.m_btAutoPlayOff.EnableWindow( FALSE ) ;
				m_GameClientView.m_btAutoPlayOn.EnableWindow( FALSE ) ;
				m_GameClientView.m_btSortCard.ShowWindow( SW_HIDE );
			}
			else
			{
				m_GameClientView.m_btSortCard.EnableWindow(TRUE);
			}

			//�����˿�
			BYTE cbCardData[20];
			ZeroMemory(cbCardData,sizeof(cbCardData));
			for(BYTE i=0;i<GAME_PLAYER;i++)
			{
				if(i == GetMeChairID())continue;
				if(pStatusPlay->bMingCardStatus[i] != 0)
				{
					m_GameClientView.m_HandCardControl[SwitchViewChairID(i)].SetCardData(pStatusPlay->bCardData[i],pStatusPlay->bCardCount[i]);
					m_GameClientView.m_HandCardControl[SwitchViewChairID(i)].SetDisplayFlag((IsLookonMode()==false)||(IsAllowLookon()==true));
				}
				else
				{
					m_GameClientView.m_HandCardControl[SwitchViewChairID(i)].SetCardData(cbCardData,17);
				}
			}

			for ( WORD wChairID = 0; wChairID < GAME_PLAYER; ++wChairID )
			{
				m_GameClientView.SetUserTrustee( SwitchViewChairID( wChairID ), pStatusPlay->bUserTrustee[ wChairID ] );

				//�Լ��ж�
				if ( ! IsLookonMode() && wChairID == GetMeChairID() &&pStatusPlay->bUserTrustee[ wChairID ] )
				{
					//�йܹ���
					m_GameClientView.m_btAutoPlayOn.ShowWindow(SW_HIDE);
					m_GameClientView.m_btAutoPlayOff.ShowWindow(SW_SHOW);
					OnAutoPlay( true, 0 );
				}
			}

			//���ö�ʱ��
			SetGameClock(pStatusPlay->wCurrentUser,IDI_OUT_CARD,30);

			//�����˿�
			m_GameClientView.m_HandCardControl[1].SetPositively(IsLookonMode()==false);
			//m_GameClientView.m_HandCardControl[1].SetDisplayFlag((IsLookonMode()==false)||(IsAllowLookon()==true));
			m_GameClientView.m_HandCardControl[1].SetDisplayFlag(true);

			//��ʷ�ɼ�
			for (BYTE i=0;i<GAME_PLAYER;i++)
			{
				//��ȡ����
				tagHistoryScore * pHistoryScore=m_HistoryScore.GetHistoryScore(i);

				//���û���
				pHistoryScore->lTurnScore=pStatusPlay->lTurnScore[i];
				pHistoryScore->lCollectScore=pStatusPlay->lCollectScore[i];

				//������
				WORD wViewChairID=SwitchViewChairID(i);
				m_GameClientView.SetHistoryScore(wViewChairID,pHistoryScore);
			}

			return true;
		}
	case GS_WK_QIANG_LAND: //������
		{
			//Ч������
			if (wDataSize!=sizeof(CMD_S_StatusQiangLand)) return false;
			CMD_S_StatusQiangLand * pQiangLand=(CMD_S_StatusQiangLand *)pData;

			//���ñ���
			m_bHandCardCount=17;
			for (WORD i=0;i<GAME_PLAYER;i++) m_bCardCount[i]=17;
			CopyMemory(m_bHandCardData,pQiangLand->bCardData[GetMeChairID()],m_bHandCardCount);

			//���ý���
			for (WORD i=0;i<GAME_PLAYER;i++)	
			{
				WORD wViewChairID=SwitchViewChairID(i);
				m_GameClientView.SetCardCount(wViewChairID,m_bCardCount[i]);
				m_GameClientView.SetLandScore(wViewChairID,pQiangLand->bScoreInfo[i]);
			}
			m_GameClientView.ShowLandTitle(true);
			m_GameClientView.SetBaseScore(pQiangLand->lBaseScore);

			//���ƽ���
			m_GameClientView.m_btScore.ShowWindow(SW_SHOW);

			//���ñ���
			m_wBombTime=pQiangLand->wBombTime;
			m_GameClientView.SetBombTime(m_wBombTime);

			//��ť����
			if ((IsLookonMode()==false)&&(pQiangLand->wCurrentUser==GetMeChairID()))
			{
				m_GameClientView.m_btQiangLand.ShowWindow(SW_SHOW);
				m_GameClientView.m_btNoQiang.ShowWindow(SW_SHOW);
			}

			if ( IsLookonMode() )
			{
				m_GameClientView.m_btAutoPlayOff.ShowWindow( SW_HIDE ) ;
				m_GameClientView.m_btAutoPlayOn.ShowWindow( SW_HIDE ) ;
				m_GameClientView.m_btAutoPlayOff.EnableWindow( FALSE ) ;
				m_GameClientView.m_btAutoPlayOn.EnableWindow( FALSE ) ;
				m_GameClientView.m_btSortCard.ShowWindow( SW_HIDE );
			}
			else 
			{
				m_GameClientView.m_btSortCard.EnableWindow(TRUE);
			}

			//�����˿�
			BYTE bCardData[3]={0,0,0};
			m_GameClientView.m_BackCardControl.SetCardData(bCardData,3);
			m_GameClientView.m_HandCardControl[1].SetCardData(m_bHandCardData,m_bHandCardCount);
			m_GameClientView.m_HandCardControl[1].SetDisplayFlag((IsLookonMode()==false)||(IsAllowLookon()==true));

			for ( WORD wChairID = 0; wChairID < GAME_PLAYER; ++wChairID )
			{
				m_GameClientView.SetUserTrustee( SwitchViewChairID( wChairID ), pQiangLand->bUserTrustee[ wChairID ] );

				//�Լ��ж�
				if ( ! IsLookonMode() && wChairID == GetMeChairID() && pQiangLand->bUserTrustee[ wChairID ] )
				{
					//�йܹ���
					m_GameClientView.m_btAutoPlayOn.ShowWindow(SW_HIDE);
					m_GameClientView.m_btAutoPlayOff.ShowWindow(SW_SHOW);
					OnAutoPlay( true, 0 );
				}
			}

			//�����˿�
			BYTE cbCardData[20];
			ZeroMemory(cbCardData,sizeof(cbCardData));
			for(BYTE i=0;i<GAME_PLAYER;i++)
			{
				if(i == GetMeChairID())continue;
				if(pQiangLand->bMingCardStatus[i] != 0)
				{
					m_GameClientView.m_HandCardControl[SwitchViewChairID(i)].SetCardData(pQiangLand->bCardData[i],17);
					m_GameClientView.m_HandCardControl[SwitchViewChairID(i)].SetDisplayFlag((IsLookonMode()==false)||(IsAllowLookon()==true));
				}
				else
				{
					m_GameClientView.m_HandCardControl[SwitchViewChairID(i)].SetCardData(cbCardData,17);
				}
			}

			//����ʱ��
			SetGameClock(pQiangLand->wCurrentUser,IDI_QIANG_LAND,30);

			//��ʷ�ɼ�
			for (BYTE i=0;i<GAME_PLAYER;i++)
			{
				//��ȡ����
				tagHistoryScore * pHistoryScore=m_HistoryScore.GetHistoryScore(i);

				//���û���
				pHistoryScore->lTurnScore=pQiangLand->lTurnScore[i];
				pHistoryScore->lCollectScore=pQiangLand->lCollectScore[i];

				//������
				WORD wViewChairID=SwitchViewChairID(i);
				m_GameClientView.SetHistoryScore(wViewChairID,pHistoryScore);
			}

			return true;
		}
	case GS_WK_ADD:			//�ӱ�
		{
			//Ч������
			if (wDataSize!=sizeof(CMD_S_StatusAdd)) return false;
			CMD_S_StatusAdd * pAdd=(CMD_S_StatusAdd *)pData;

			//���ñ���
			m_bHandCardCount=17;
			for (WORD i=0;i<GAME_PLAYER;i++) m_bCardCount[i]=17;
			CopyMemory(m_bHandCardData,pAdd->bCardData[GetMeChairID()],m_bHandCardCount);

			//���ý���
			for (WORD i=0;i<GAME_PLAYER;i++)	
			{
				WORD wViewChairID=SwitchViewChairID(i);
				m_GameClientView.SetCardCount(wViewChairID,m_bCardCount[i]);
				m_GameClientView.SetLandScore(wViewChairID,pAdd->bScoreInfo[i]);
			}
			m_GameClientView.ShowLandTitle(true);
			m_GameClientView.SetBaseScore(pAdd->lBaseScore);
			
			//����
			m_GameLogic.SetLaiZiCard(pAdd->bLaiZiCard);
			m_GameClientView.SetLaiZiCard(m_GameLogic.GetCardLogicValue(pAdd->bLaiZiCard));
			m_GameClientView.SetLaiZiDisplay(true);

			//���ƽ���
			m_GameClientView.m_btScore.ShowWindow(SW_SHOW);

			//���ñ���
			m_wBombTime=pAdd->wBombTime;
			m_GameClientView.SetBombTime(m_wBombTime);

			//��ť����
			if ((IsLookonMode()==false)&&(pAdd->wCurrentUser==GetMeChairID()))
			{
				//�ӱ�
				m_GameClientView.m_btAdd.ShowWindow(SW_SHOW);
				m_GameClientView.m_btNoAdd.ShowWindow(SW_SHOW);
				if(pAdd->bAddScore[GetMeChairID()] == false)
				{
					m_GameClientView.m_btAdd.EnableWindow(false);
					m_GameClientView.m_btNoAdd.EnableWindow(false);
				}
			}

			if ( IsLookonMode() )
			{
				m_GameClientView.m_btAutoPlayOff.ShowWindow( SW_HIDE ) ;
				m_GameClientView.m_btAutoPlayOn.ShowWindow( SW_HIDE ) ;
				m_GameClientView.m_btAutoPlayOff.EnableWindow( FALSE ) ;
				m_GameClientView.m_btAutoPlayOn.EnableWindow( FALSE ) ;
				m_GameClientView.m_btSortCard.ShowWindow( SW_HIDE );
			}
			else 
			{
				m_GameClientView.m_btSortCard.EnableWindow(TRUE);
			}

			//�����˿�
			BYTE bCardData[3]={0,0,0};
			m_GameClientView.m_BackCardControl.SetCardData(bCardData,3);
			m_GameClientView.m_HandCardControl[1].SetCardData(m_bHandCardData,m_bHandCardCount);
			m_GameClientView.m_HandCardControl[1].SetDisplayFlag((IsLookonMode()==false)||(IsAllowLookon()==true));

			for ( WORD wChairID = 0; wChairID < GAME_PLAYER; ++wChairID )
			{
				m_GameClientView.SetUserTrustee( SwitchViewChairID( wChairID ), pAdd->bUserTrustee[ wChairID ] );

				//�Լ��ж�
				if ( ! IsLookonMode() && wChairID == GetMeChairID() && pAdd->bUserTrustee[ wChairID ] )
				{
					//�йܹ���
					m_GameClientView.m_btAutoPlayOn.ShowWindow(SW_HIDE);
					m_GameClientView.m_btAutoPlayOff.ShowWindow(SW_SHOW);
					OnAutoPlay( true, 0 );
				}
			}

			//�����˿�
			BYTE cbCardData[20];
			ZeroMemory(cbCardData,sizeof(cbCardData));
			for(BYTE i=0;i<GAME_PLAYER;i++)
			{
				if(i == GetMeChairID())continue;
				if(pAdd->bMingCardStatus[i] != 0)
				{
					m_GameClientView.m_HandCardControl[SwitchViewChairID(i)].SetCardData(pAdd->bCardData[i],17);
					m_GameClientView.m_HandCardControl[SwitchViewChairID(i)].SetDisplayFlag((IsLookonMode()==false)||(IsAllowLookon()==true));
				}
				else
				{
					m_GameClientView.m_HandCardControl[SwitchViewChairID(i)].SetCardData(cbCardData,17);
				}
			}

			//����ʱ��
			SetGameClock(pAdd->wCurrentUser,IDI_ADD,30);

			//��ʷ�ɼ�
			for (BYTE i=0;i<GAME_PLAYER;i++)
			{
				//��ȡ����
				tagHistoryScore * pHistoryScore=m_HistoryScore.GetHistoryScore(i);

				//���û���
				pHistoryScore->lTurnScore=pAdd->lTurnScore[i];
				pHistoryScore->lCollectScore=pAdd->lCollectScore[i];

				//������
				WORD wViewChairID=SwitchViewChairID(i);
				m_GameClientView.SetHistoryScore(wViewChairID,pHistoryScore);
			}

			return true;
		}
	}

	return false;
}

//�����˿�
bool CGameClientEngine::OnSubSendCard(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_SendAllCard));
	if (wDataSize!=sizeof(CMD_S_SendAllCard)) return false;

	//��������
	CMD_S_SendAllCard * pSendCard=(CMD_S_SendAllCard *)pBuffer;

	//�رճɼ�
	if (m_GameClientView.m_ScoreView.m_hWnd!=NULL)
	{
		m_GameClientView.m_ScoreView.ShowWindow(SW_HIDE) ;
		m_GameClientView.m_ScoreView.DestroyWindow();
	}

	//��������
	m_wCurrentUser=pSendCard->wCurrentUser;
	m_bHandCardCount=17;
	for(BYTE i=0;i<GAME_PLAYER;i++)
	{
		if(i == GetMeChairID())
			CopyMemory(m_bHandCardData,pSendCard->bCardData[GetMeChairID()],sizeof(pSendCard->bCardData[GetMeChairID()]));
		CopyMemory(m_bHandCardUser[i],pSendCard->bCardData[i],sizeof(pSendCard->bCardData[i]));
	}

	for (WORD i=0;i<GAME_PLAYER;i++) 
		m_bCardCount[i]=CountArray(pSendCard->bCardData);

	for(WORD wChairID=0; wChairID<GAME_PLAYER; ++wChairID)
	{
		m_bCardCount[wChairID] = 17 ;
	}

	//��������
	CopyMemory(m_bMingCard,pSendCard->bMingCard,sizeof(pSendCard->bMingCard));

	//����
	m_wBombTime=pSendCard->wBombTime;
	m_GameClientView.SetBombTime(m_wBombTime);

	//������
	m_bLaiZiData=pSendCard->bLaiZiCard;
	
	
	//���ý���
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		m_GameClientView.SetLandScore(i,0);
		m_GameClientView.SetPassFlag(i,false);
		m_GameClientView.m_UserCardControl[i].SetCardData(NULL,0);
		m_GameClientView.m_HandCardControl[i].SetCardData(NULL,0);
	}

	//���ý���
	m_GameClientView.ShowLandTitle(true);
	m_GameClientView.SetBombTime(m_wBombTime);
	m_GameClientView.SetLandUser(INVALID_CHAIR,0);

	//�����˿�
	BYTE bBackCard[3]={0,0,0};
	m_GameClientView.m_BackCardControl.SetCardData(bBackCard,CountArray(bBackCard));
	m_GameClientView.m_HandCardControl[1].SetDisplayFlag(((IsLookonMode()==false)||(IsAllowLookon()==true)));

	//�ŷ��˿�
	DispatchUserCard(pSendCard->bCardData,m_bHandCardCount);
	m_GameLogic.SortCardListEx(m_bHandCardData,m_bHandCardCount,ST_ORDER);
	for(BYTE i=0;i<GAME_PLAYER;i++)
	{
		m_GameLogic.SortCardListEx(m_bHandCardUser[i],m_bCardCount[i],ST_ORDER);
	}

#ifdef VIDEO_GAME
	//�����˿�
	m_GameClientView.m_HandCardControl[ 0 ].ShowWindow( SW_HIDE );
	m_GameClientView.m_HandCardControl[ 2 ].ShowWindow( SW_HIDE );
#endif

	return true;
}

//�û��з�
bool CGameClientEngine::OnSubLandScore(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_LandScore));
	if (wDataSize!=sizeof(CMD_S_LandScore)) return false;

	//��Ϣ����
	CMD_S_LandScore * pLandScore=(CMD_S_LandScore *)pBuffer;

	//ֹͣ����
	DoncludeDispatchCard();

	//���ý���
	WORD wViewChairID=SwitchViewChairID(pLandScore->bLandUser);
	m_GameClientView.SetLandScore(wViewChairID,pLandScore->bLandScore);

	//�������
	if ((IsLookonMode()==false)&&(pLandScore->wCurrentUser==GetMeChairID()))
	{
		ActiveGameFrame();
		m_GameClientView.m_btGiveUpScore.ShowWindow(SW_SHOW);
		
		////m_GameClientView.m_btOneScore.ShowWindow( SW_SHOW );
		//m_GameClientView.m_btTwoScore.ShowWindow( SW_SHOW );
		m_GameClientView.m_btThreeScore.ShowWindow( SW_SHOW );

		//m_GameClientView.m_btOneScore.EnableWindow(pLandScore->bCurrentScore==0?TRUE:FALSE);
		//m_GameClientView.m_btTwoScore.EnableWindow(pLandScore->bCurrentScore<=1?TRUE:FALSE);
		m_GameClientView.m_btThreeScore.EnableWindow(pLandScore->bCurrentScore<=2?TRUE:FALSE);
	}

	//��������
	PlayGameSound(AfxGetInstanceHandle(),TEXT("CALL_CARD"));

	//����ʱ��
	SetGameClock(pLandScore->wCurrentUser,IDI_LAND_SCORE,30);

	return true;
}

//��Ϸ��ʼ
bool CGameClientEngine::OnSubGameStart(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_GameStart));
	if (wDataSize!=sizeof(CMD_S_GameStart)) return false;

	//ɾ��ʱ��
	KillGameClock(IDI_QIANG_LAND);

	//��Ϣ����
	CMD_S_GameStart * pGameStart=(CMD_S_GameStart *)pBuffer;

	//���ñ���
	//m_wBombTime=1;
	m_bTurnCardCount=0;
	m_bTurnOutType=CT_ERROR;
	m_wLandUser=pGameStart->wLandUser;
	m_bCardCount[pGameStart->wLandUser]=20;
	m_wCurrentUser=pGameStart->wCurrentUser;
	ZeroMemory(m_bTurnCardData,sizeof(m_bTurnCardData));

	//�رճɼ�
	if (m_GameClientView.m_ScoreView.m_hWnd!=NULL)
	{
		m_GameClientView.m_ScoreView.ShowWindow(SW_HIDE) ;
		m_GameClientView.m_ScoreView.DestroyWindow();
	}

	//���ÿؼ�
	m_GameClientView.ShowLandTitle(false);
	m_GameClientView.m_BackCardControl.SetCardData(pGameStart->bBackCard,CountArray(pGameStart->bBackCard));
	m_GameClientView.m_BackCardControl.SetDisplayFlag( true );

	//���ý���
	m_GameClientView.m_btThreeScore.ShowWindow(SW_HIDE);
	m_GameClientView.m_btGiveUpScore.ShowWindow(SW_HIDE);
	m_GameClientView.SetLandScore(INVALID_CHAIR,0);
	m_GameClientView.SetCardCount(SwitchViewChairID(pGameStart->wLandUser),m_bCardCount[pGameStart->wLandUser]);

	//���ñ���
	m_wBombTime=pGameStart->wBombTime;
	m_GameClientView.SetBombTime(m_wBombTime);

	//��������
	if (pGameStart->wLandUser==GetMeChairID())
	{
		BYTE bCardCound=m_bHandCardCount;
		m_bHandCardCount+=CountArray(pGameStart->bBackCard);
		CopyMemory(&m_bHandCardData[bCardCound],pGameStart->bBackCard,sizeof(pGameStart->bBackCard));
		m_GameLogic.SortCardListEx(m_bHandCardData,m_bHandCardCount,ST_ORDER);
		m_GameClientView.m_HandCardControl[1].SetCardData(m_bHandCardData,m_bHandCardCount);
		m_GameClientView.m_HandCardControl[1].SetNoChangeCard(m_bHandCardData,m_bHandCardCount);
	}
	else
	{
		CopyMemory(&m_bHandCardUser[pGameStart->wLandUser][17],pGameStart->bBackCard,sizeof(pGameStart->bBackCard));
		m_GameLogic.SortCardListEx(m_bHandCardUser[pGameStart->wLandUser],m_bCardCount[pGameStart->wLandUser],ST_ORDER);
		m_GameClientView.m_HandCardControl[SwitchViewChairID(pGameStart->wLandUser)].SetCardData(m_bHandCardUser[pGameStart->wLandUser],m_bCardCount[pGameStart->wLandUser]);
	}

	//���ý���
	m_GameClientView.SetLandUser(SwitchViewChairID(pGameStart->wLandUser),pGameStart->bLandScore);

	//��������
	CopyMemory(m_bAddScore,pGameStart->bAddScore,sizeof(pGameStart->bAddScore));
	m_bIsAdd=pGameStart->bIsAdd;

	//����
	SetTimer(IDI_LAIZI,100,NULL);

	return true;
}

//�û�����
bool CGameClientEngine::OnSubOutCard(const void * pBuffer, WORD wDataSize)
{
	//��������
	CMD_S_OutCard * pOutCard=(CMD_S_OutCard *)pBuffer;
	WORD wHeadSize=sizeof(CMD_S_OutCard)-sizeof(pOutCard->bCardData);

	//Ч������
	ASSERT(wDataSize>=wHeadSize);
	if (wDataSize<wHeadSize) return false;
	ASSERT(wDataSize==(wHeadSize+pOutCard->bCardCount*sizeof(pOutCard->bCardData[0])));
	if (wDataSize!=(wHeadSize+pOutCard->bCardCount*sizeof(pOutCard->bCardData[0]))) return false;

	//ɾ����ʱ��
	KillTimer(IDI_MOST_CARD);
	KillGameClock(IDI_OUT_CARD);
	WORD wOutViewChairID=SwitchViewChairID(pOutCard->wOutCardUser);

	//����
	m_GameClientView.SetLaiZiKuang(false);

	//�ӱ���Ϣ
	for(BYTE i=0;i<GAME_PLAYER;i++)
		m_GameClientView.SetAddStates(i,false);

	//�����˿�
	for(BYTE i=0;i<GAME_PLAYER;i++)
	{
		m_GameLogic.SortCardListEx(m_bHandCardUser[i],m_bCardCount[i],ST_ORDER);
	}


	for(BYTE i=0;i<pOutCard->bCardCount;i++)
	{
		if(pOutCard->bCardData[i] == NULL)
			ASSERT(FALSE);
		if(pOutCard->bChangeCard[i] == NULL)
			ASSERT(FALSE);
	}
	//��������
	if ((IsLookonMode()==true)||(GetMeChairID()!=pOutCard->wOutCardUser))
	{
		m_GameClientView.m_UserCardControl[wOutViewChairID].SetCardData(pOutCard->bChangeCard,pOutCard->bCardCount);
		m_GameClientView.m_UserCardControl[wOutViewChairID].SetNoChangeCard(pOutCard->bCardData,pOutCard->bCardCount);
	}

	//��������
	if (m_bTurnCardCount==0)
	{
		for (WORD i=0;i<GAME_PLAYER;i++) 
		{
			if (i==pOutCard->wOutCardUser) continue;
			m_GameClientView.m_UserCardControl[SwitchViewChairID(i)].SetCardData(NULL,0);
		}
		m_GameClientView.SetPassFlag(INVALID_CHAIR,false);
	}

	//��¼����
	memset(m_bChangeTurnOut,0,sizeof(m_bChangeTurnOut));
	m_bTurnCardCount=pOutCard->bCardCount;
	m_bTurnOutType=m_GameLogic.GetCardType(pOutCard->bCardData,pOutCard->bCardCount);
	CopyMemory(m_bTurnCardData,pOutCard->bCardData,sizeof(BYTE)*pOutCard->bCardCount);
	CopyMemory(m_bChangeTurnOut,pOutCard->bChangeCard,sizeof(BYTE)*pOutCard->bCardCount);

	//ը���ж�
	if ((m_bTurnOutType&(1<<CT_BOMB_CARD)) != 0||(m_bTurnOutType&(1<<CT_MISSILE_CARD))!= 0||(m_bTurnOutType&(1<<CT_RUAN_BOMB))!= 0||(m_bTurnOutType&(1<<CT_LAIZI_BOMB))!= 0)
	{
		if((m_bTurnOutType&(1<<CT_RUAN_BOMB))!= 0)
			m_wBombTime*=2;
		else
			m_wBombTime*=4;

		m_GameClientView.SetBombTime(m_wBombTime);

		//���Ŷ���
		m_GameClientView.SetBombEffect(true);
	}

	//�Լ��˿�
	if ((IsLookonMode()==true)&&(pOutCard->wOutCardUser==GetMeChairID()))
	{
		//ɾ���˿� 
		BYTE bSourceCount=m_bHandCardCount;
		m_bHandCardCount-=pOutCard->bCardCount;
		m_GameLogic.RemoveCard(pOutCard->bCardData,pOutCard->bCardCount,m_bHandCardData,bSourceCount);

		//���ý���
		m_GameClientView.SetCardCount(wOutViewChairID,m_bHandCardCount);
		m_GameClientView.m_HandCardControl[1].SetCardData(m_bHandCardData,m_bHandCardCount);
	}

	//��������
	if(pOutCard->wOutCardUser!=GetMeChairID())
	{
		BYTE bSourceCount=m_bCardCount[pOutCard->wOutCardUser];
		m_bCardCount[pOutCard->wOutCardUser]-=pOutCard->bCardCount;
		m_GameClientView.SetCardCount(wOutViewChairID,m_bCardCount[pOutCard->wOutCardUser]);

		m_GameLogic.RemoveCard(pOutCard->bCardData,pOutCard->bCardCount,m_bHandCardUser[pOutCard->wOutCardUser],bSourceCount);
		m_GameClientView.m_HandCardControl[wOutViewChairID].SetCardData(m_bHandCardUser[pOutCard->wOutCardUser],m_bCardCount[pOutCard->wOutCardUser]);
	}

	//����ж�
	if (pOutCard->wCurrentUser==pOutCard->wOutCardUser)
	{
		//���ñ���
		m_bTurnCardCount=0;
		m_bTurnOutType=CT_ERROR;
		m_wMostUser=pOutCard->wCurrentUser;
		memset(m_bTurnCardData,0,sizeof(m_bTurnCardData));

		//���ý���
		for (WORD i=0;i<GAME_PLAYER;i++)
		{
			if (i!=pOutCard->wOutCardUser)
			{
				WORD wViewChairID=SwitchViewChairID(i);
				m_GameClientView.SetPassFlag(wViewChairID,true);
				m_GameClientView.m_UserCardControl[wViewChairID].SetCardData(NULL,0);
			}
		}

		//��������
		PlayGameSound(AfxGetInstanceHandle(),TEXT("MOST_CARD"));

		//���ö�ʱ��
		SetTimer(IDI_MOST_CARD,3000,NULL);

		return true;
	}

	if ((IsLookonMode()==true)||(GetMeChairID()!=pOutCard->wOutCardUser))
	{
		//ը���ж�
		if ((m_bTurnOutType==CT_BOMB_CARD)||(m_bTurnOutType==CT_MISSILE_CARD))
			PlayGameSound(AfxGetInstanceHandle(),TEXT("BOMB_CARD"));
		else
			PlayGameSound(AfxGetInstanceHandle(),TEXT("OUT_CARD"));
	}

	//�������
	if (pOutCard->wCurrentUser!=INVALID_CHAIR)
	{
		WORD wViewChairID=SwitchViewChairID(pOutCard->wCurrentUser);
		m_GameClientView.SetPassFlag(wViewChairID,false);
		m_GameClientView.m_UserCardControl[wViewChairID].SetCardData(NULL,0);
	}

	//�������
	if ((IsLookonMode()==false)&&(pOutCard->wCurrentUser==GetMeChairID()))
	{
		ActiveGameFrame();
		m_GameClientView.m_btPassCard.EnableWindow(TRUE);
		m_GameClientView.m_btOutCard.ShowWindow(SW_SHOW);
		m_GameClientView.m_btPassCard.ShowWindow(SW_SHOW);
		m_GameClientView.m_btAutoOutCard.EnableWindow(TRUE);
		m_GameClientView.m_btAutoOutCard.ShowWindow(SW_SHOW);
		m_GameClientView.m_btOutCard.EnableWindow((VerdictOutCard()==true)?TRUE:FALSE);
	}

	//����ʱ��
	if (pOutCard->wCurrentUser!=INVALID_CHAIR)
	{
		BYTE bCardCount=m_bCardCount[pOutCard->wCurrentUser];
		SetGameClock(pOutCard->wCurrentUser,IDI_OUT_CARD,(bCardCount<m_bTurnCardCount)?3:30);
	}

	return true;
}

//��������
bool CGameClientEngine::OnSubPassCard(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	if (wDataSize!=sizeof(CMD_S_PassCard)) return false;
	CMD_S_PassCard * pPassCard=(CMD_S_PassCard *)pBuffer;

	//ɾ����ʱ��
	KillGameClock(IDI_OUT_CARD);

	//�������
	if ((IsLookonMode()==true)||(pPassCard->wPassUser!=GetMeChairID()))
	{
		WORD wViewChairID=SwitchViewChairID(pPassCard->wPassUser);
		m_GameClientView.SetPassFlag(wViewChairID,true);
		m_GameClientView.m_UserCardControl[wViewChairID].SetCardData(NULL,0);
	}

	//һ���ж�
	if (pPassCard->bNewTurn==TRUE)
	{
		m_bTurnCardCount=0;
		m_bTurnOutType=CT_ERROR;
		memset(m_bTurnCardData,0,sizeof(m_bTurnCardData));
	}

	//���ý���
	WORD wViewChairID=SwitchViewChairID(pPassCard->wCurrentUser);
	m_GameClientView.SetPassFlag(wViewChairID,false);
	m_GameClientView.m_UserCardControl[wViewChairID].SetCardData(NULL,0);

	//�������
	if ((IsLookonMode()==false)&&(pPassCard->wCurrentUser==GetMeChairID()))
	{
		ActiveGameFrame();
		m_GameClientView.m_btOutCard.ShowWindow(SW_SHOW);
		m_GameClientView.m_btPassCard.ShowWindow(SW_SHOW);
		m_GameClientView.m_btAutoOutCard.ShowWindow(SW_SHOW);
		m_GameClientView.m_btPassCard.EnableWindow((m_bTurnCardCount>0)?TRUE:FALSE);
		m_GameClientView.m_btOutCard.EnableWindow((VerdictOutCard()==true)?TRUE:FALSE);
		m_GameClientView.m_btAutoOutCard.EnableWindow( TRUE );
	}

	//��������
	if ((IsLookonMode()==true)||(pPassCard->wPassUser!=GetMeChairID()))	PlayGameSound(AfxGetInstanceHandle(),TEXT("PASS_CARD"));

	//����ʱ��
	if (m_bTurnCardCount!=0)
	{
		BYTE bCardCount=m_bCardCount[pPassCard->wCurrentUser];
		SetGameClock(pPassCard->wCurrentUser,IDI_OUT_CARD,(bCardCount<m_bTurnCardCount)?3:30);
	}
	else SetGameClock(pPassCard->wCurrentUser,IDI_OUT_CARD,30);

	return true;
}

//��Ϸ����
bool CGameClientEngine::OnSubGameEnd(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_GameEnd));
	if (wDataSize!=sizeof(CMD_S_GameEnd)) return false;

	//��Ϣ����
	CMD_S_GameEnd * pGameEnd=(CMD_S_GameEnd *)pBuffer;

	//ɾ����ʱ��
	KillTimer(IDI_MOST_CARD);
	KillGameClock(IDI_OUT_CARD);
	KillGameClock(IDI_LAND_SCORE);
	KillTimer(IDI_DISPATCH_CARD);
	KillTimer(IDI_LAIZI);
	KillGameClock(IDI_QIANG_LAND);
	KillGameClock(IDI_ADD);

	//ֹͣ����
	DoncludeDispatchCard();

	//���ؿؼ�
	m_GameClientView.m_btOutCard.ShowWindow(SW_HIDE);
	m_GameClientView.m_btPassCard.ShowWindow(SW_HIDE);
	//m_GameClientView.m_btOneScore.ShowWindow(SW_HIDE);
	//m_GameClientView.m_btTwoScore.ShowWindow(SW_HIDE);
	m_GameClientView.m_btThreeScore.ShowWindow(SW_HIDE);
	m_GameClientView.m_btGiveUpScore.ShowWindow(SW_HIDE);
	m_GameClientView.m_btAutoOutCard.ShowWindow(SW_HIDE);
	m_GameClientView.m_btSortCard.EnableWindow(FALSE);
	m_GameClientView.m_btMingCard.ShowWindow(SW_HIDE);
	m_GameClientView.m_btQiangLand.ShowWindow(SW_HIDE);
	m_GameClientView.m_btNoQiang.ShowWindow(SW_HIDE);
	m_GameClientView.m_btMingOut.ShowWindow(SW_HIDE);
	m_GameClientView.m_btAdd.ShowWindow(SW_HIDE);
	m_GameClientView.m_btNoAdd.ShowWindow(SW_HIDE);

	//���ÿؼ�
	m_GameClientView.m_btOutCard.EnableWindow(FALSE);
	m_GameClientView.m_btPassCard.EnableWindow(FALSE);

	//�����ɼ�
	if (m_GameClientView.m_ScoreView.m_hWnd==NULL)
	{
		m_GameClientView.m_ScoreView.Create(IDD_GAME_SCORE,&m_GameClientView);
	}

	if(m_pIStringMessage != NULL)
		m_pIStringMessage->InsertSystemString( TEXT( " \n���ֽ������ɼ�ͳ�ƣ�" ));

	//���û���
	CString strScore;
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		IClientUserItem * pClientUserItem=GetTableUserItem(i);
		m_GameClientView.m_ScoreView.SetGameScore(i,pClientUserItem->GetNickName(),pGameEnd->lGameScore[i],pGameEnd->bUserAdd,pGameEnd->iOtherAdd,
			pGameEnd->wPlayTime[GetMeChairID()],pGameEnd->lGameResult[GetMeChairID()]);
		strScore.Format( TEXT( " \n%s ��%I64d" ), pClientUserItem->GetNickName(),pGameEnd->lGameScore[i]);
		if(m_pIStringMessage != NULL)
			m_pIStringMessage->InsertNormalString( strScore);
	}

	////��Ϸ����
	//IClientKernel *pIClientKernel = ( IClientKernel * )GetClientKernel( IID_IClientKernel, VER_IClientKernel );
	//const tagServerAttribute *pServerAttribute  = pIClientKernel->GetServerAttribute();

	//if ( pServerAttribute->wGameGenre == GAME_GENRE_GOLD ) {
	//	strScore.Format( TEXT( "��˰��%I64d" ), pGameEnd->lGameTax);
	//	InsertShtickString( strScore);
	//
	//}

	//m_GameClientView.m_ScoreView.SetGameTax(pGameEnd->lGameTax);

	//�����ɼ�
	CRect scoreRect, gameViewRect ;
	m_GameClientView.GetWindowRect( gameViewRect ) ;
	m_GameClientView.m_ScoreView.GetWindowRect( scoreRect ) ;
	m_GameClientView.m_ScoreView.MoveWindow( gameViewRect.left + ( gameViewRect.Width() - scoreRect.Width() ) / 2, 
		gameViewRect.top + ( gameViewRect.Height() - scoreRect.Height() ) / 2-20, scoreRect.Width(), scoreRect.Height()) ;
	m_GameClientView.m_ScoreView.ShowWindow(SW_SHOW);

	//��ʾ�˿�
	BYTE cbCardPos=0 ;
	for(BYTE i=0; i<GAME_PLAYER; ++i)
	{
		WORD wViewID = SwitchViewChairID(i) ;
		m_GameClientView.m_HandCardControl[wViewID].SetCardData(pGameEnd->bCardData+cbCardPos, pGameEnd->bCardCount[i]) ;
		m_GameClientView.m_HandCardControl[wViewID].SetDisplayFlag(true) ;
		cbCardPos += pGameEnd->bCardCount[i] ;
	}

	//��������
	WORD wMeChairID=GetMeChairID();
	LONGLONG lMeScore=pGameEnd->lGameScore[GetMeChairID()];
	if (lMeScore>0L) PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_WIN"));
	else if (lMeScore<0L) PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_LOST"));
	else PlayGameSound(GetModuleHandle(NULL),TEXT("GAME_END"));

	//���ý���
	if (IsLookonMode()==false)
	{
		m_GameClientView.m_btStart.ShowWindow(SW_SHOW);
		m_GameClientView.m_btMingCardState.ShowWindow(SW_SHOW);
		SetGameClock(GetMeChairID(),IDI_START_GAME,30);

		//�йܹ���
		 m_GameClientView.m_btAutoPlayOn.ShowWindow(SW_SHOW);
		m_GameClientView.m_btAutoPlayOff.ShowWindow(SW_HIDE);
		OnAutoPlay( false, 0 );
	}
	m_GameClientView.ShowLandTitle(false);

#ifdef VIDEO_GAME
	//��ʾ�˿�
	m_GameClientView.m_HandCardControl[ 0 ].ShowWindow( SW_SHOW );
	m_GameClientView.m_HandCardControl[ 2 ].ShowWindow( SW_SHOW );
#endif

	//�û��ɼ�
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		//��ȡ�û�
		IClientUserItem * pClientUserItem=GetTableUserItem(i);
		if (pClientUserItem==NULL) continue;

		//���óɼ�
		m_HistoryScore.OnEventUserScore(i,pGameEnd->lGameScore[i]);
	}
	return true;
}

//�ӱ�
bool CGameClientEngine::OnSubAdd(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_Add));
	if (wDataSize != sizeof(CMD_S_Add)) return false;

	CMD_S_Add *pAdd = ( CMD_S_Add * )pBuffer;

	KillGameClock(IDI_ADD);

	//���ý���
	m_GameClientView.SetLaiZiKuang(false);
	if(pAdd->wCurrentUser == GetMeChairID() && IsLookonMode() == false)
	{
		m_GameClientView.m_btAdd.ShowWindow(SW_SHOW);
		m_GameClientView.m_btNoAdd.ShowWindow(SW_SHOW);
	}
	if(pAdd->bIsAdd == false)
	{
		m_GameClientView.m_btAdd.EnableWindow(false);
		//m_GameClientView.m_btNoAdd.EnableWindow(false);
	}
	else
	{
		m_GameClientView.m_btAdd.EnableWindow(true);
		m_GameClientView.m_btNoAdd.EnableWindow(true);
	}

	//�����ӱ�
	if(pAdd->wCurrentUser == INVALID_CHAIR && m_wLandUser == GetMeChairID() && IsLookonMode() == false)
	{
		ActiveGameFrame();
		m_GameClientView.m_btOutCard.EnableWindow(FALSE);
		m_GameClientView.m_btOutCard.ShowWindow(SW_SHOW);
		m_GameClientView.m_btPassCard.EnableWindow(FALSE);
		m_GameClientView.m_btPassCard.ShowWindow(SW_SHOW);
		m_GameClientView.m_btAutoOutCard.ShowWindow(SW_SHOW);
		m_GameClientView.m_btAutoOutCard.EnableWindow(TRUE);

		//δ����
		if(m_bMingCard[GetMeChairID()] == 0)
			m_GameClientView.m_btMingOut.ShowWindow(SW_SHOW);
	}
	//�ӱ���Ϣ
	m_GameClientView.SetAddStates(SwitchViewChairID(pAdd->wAddUser),true);
	m_GameClientView.SetAdd(SwitchViewChairID(pAdd->wAddUser),pAdd->bAdd);

	//����ʱ��
	if(pAdd->wCurrentUser == INVALID_CHAIR)
		SetGameClock(m_wLandUser,IDI_OUT_CARD,30);
	else if(pAdd->bIsAdd == false)
		SetGameClock(pAdd->wCurrentUser,IDI_ADD,3);
	else
		SetGameClock(pAdd->wCurrentUser,IDI_ADD,30);

	return true;
}
//�����ж�
bool CGameClientEngine::VerdictOutCard()
{
	//״̬�ж�
	if (m_GameClientView.m_btOutCard.IsWindowVisible()==FALSE) return false;

	//��ȡ�˿�
	BYTE bCardData[20];
	BYTE bShootCount=(BYTE)m_GameClientView.m_HandCardControl[1].GetShootCard(bCardData,CountArray(bCardData));

	//�����ж�
	if (bShootCount>0L)
	{
		//�����˿�
		m_GameLogic.SortCardListEx(bCardData, bShootCount, ST_ORDER);

		//��������
		int bCardType=m_GameLogic.GetCardType(bCardData,bShootCount);
		m_GameLogic.ChangeCard(bCardType,bCardData,bShootCount,m_bChangeCard);

		//�����ж�
		if (bCardType==CT_ERROR) return false;

		//�����ж�
		if (m_bTurnCardCount==0) return true;
		if(m_GameLogic.CompareCard(m_bTurnCardData,bCardData,m_bTurnCardCount,bShootCount))
		{
			m_GameLogic.ChangeCard(bCardType,bCardData,bShootCount,m_bChangeCard);
			return true;
		}
		else
			return false;
	}

	return false;
}

//�Զ�����
bool CGameClientEngine::AutomatismOutCard()
{
	//�ȳ�����
	if (m_bTurnCardCount==0)
	{
		//���ƽ���
		KillGameClock(IDI_OUT_CARD);
		m_GameClientView.m_btOutCard.ShowWindow(SW_HIDE);
		m_GameClientView.m_btOutCard.EnableWindow(FALSE);
		m_GameClientView.m_btPassCard.ShowWindow(SW_HIDE);
		m_GameClientView.m_btPassCard.EnableWindow(FALSE);
		m_GameClientView.m_btAutoOutCard.ShowWindow(SW_HIDE);
		m_GameClientView.m_btAutoOutCard.EnableWindow(FALSE);

		//��������
		CMD_C_OutCard OutCard;
		OutCard.bCardCount=1;
		OutCard.bCardData[0]=m_bHandCardData[m_bHandCardCount-1];
		SendSocketData(SUB_C_OUT_CART,&OutCard,sizeof(OutCard)-sizeof(OutCard.bCardData)+OutCard.bCardCount*sizeof(BYTE));

		//Ԥ�ȴ���
		PlayGameSound(AfxGetInstanceHandle(),TEXT("OUT_CARD"));
		m_GameClientView.m_UserCardControl[1].SetCardData(OutCard.bCardData,OutCard.bCardCount);

		//Ԥ��ɾ��
		BYTE bSourceCount=m_bHandCardCount;
		m_bHandCardCount-=OutCard.bCardCount;
		m_GameLogic.RemoveCard(OutCard.bCardData,OutCard.bCardCount,m_bHandCardData,bSourceCount);
		m_GameClientView.m_HandCardControl[1].SetCardData(m_bHandCardData,m_bHandCardCount);
	}
	else OnPassCard(0,0);

	return true;
}

//ֹͣ����
bool CGameClientEngine::DoncludeDispatchCard()
{
	//ֹͣ����
	if (m_cbRemnantCardCount>0)
	{
		//ɾ��ʱ��
		KillTimer(IDI_DISPATCH_CARD);

		//���ý���
		for (WORD i=0;i<GAME_PLAYER;i++)
		{
			WORD wViewChairID = SwitchViewChairID(i) ;
			if (wViewChairID!=1)
			{
				BYTE cbCardData[20] ;
				ZeroMemory(cbCardData, sizeof(cbCardData)) ;
				//m_GameClientView.m_HandCardControl[wViewChairID].SetCardData(cbCardData,17);
			}
			else m_GameClientView.m_HandCardControl[1].SetCardData(m_bHandCardData,m_bHandCardCount); 
		}

		//���ñ���
		m_cbRemnantCardCount=0;
		m_cbDispatchCardCount=0;
		ZeroMemory(m_cbDispatchCardData,sizeof(m_cbDispatchCardData));
	}

	return true;
}

//���Ӷ���ֹͣ
bool CGameClientEngine::DoncludeFrame()
{
	//���ý���
	m_GameClientView.SetLaiZiDisplay(true);
	m_GameClientView.m_HandCardControl[1].SetLaiZivalue(m_GameLogic.GetCardValue(m_bLaiZiData));
	m_GameLogic.SetLaiZiCard(m_bLaiZiData);
	m_GameClientView.SetLaiZiCard(m_GameLogic.GetCardLogicValue(m_bLaiZiData));
	m_GameLogic.SortCardListEx(m_bHandCardData,m_bHandCardCount,ST_ORDER);
	m_GameClientView.m_HandCardControl[1].SetCardData(m_bHandCardData,m_bHandCardCount);
	m_GameClientView.m_HandCardControl[1].SetNoChangeCard(m_bHandCardData,m_bHandCardCount);

	for(BYTE i=0;i<GAME_PLAYER;i++)
	{
		m_GameClientView.m_UserCardControl[i].SetLaiZivalue(m_GameLogic.GetCardValue(m_bLaiZiData));
	}

	//�������
	m_GameClientView.m_HandCardControl[1].SetPositively( IsLookonMode()==false || IsAllowLookon() );

	//��ǰ���
	if ((IsLookonMode()==false)&&(m_wCurrentUser==GetMeChairID()) && m_bIsAdd)
	{
		//�ӱ�
		m_GameClientView.m_btAdd.ShowWindow(SW_SHOW);
		m_GameClientView.m_btNoAdd.ShowWindow(SW_SHOW);

		if(m_bAddScore[GetMeChairID()] == false)
		{
			m_GameClientView.m_btAdd.EnableWindow(false);
			//m_GameClientView.m_btNoAdd.EnableWindow(false);
		}
		else
		{
			m_GameClientView.m_btAdd.EnableWindow(true);
			m_GameClientView.m_btNoAdd.EnableWindow(true);
		}
	}

	if ((IsLookonMode()==false)&&(m_wCurrentUser==GetMeChairID()) && !m_bIsAdd)
	{
		ActiveGameFrame();
		m_GameClientView.m_btOutCard.EnableWindow(FALSE);
		m_GameClientView.m_btOutCard.ShowWindow(SW_SHOW);
		m_GameClientView.m_btPassCard.EnableWindow(FALSE);
		m_GameClientView.m_btPassCard.ShowWindow(SW_SHOW);
		m_GameClientView.m_btAutoOutCard.ShowWindow(SW_SHOW);
		m_GameClientView.m_btAutoOutCard.EnableWindow(TRUE);
		m_GameClientView.m_btMingOut.ShowWindow(SW_SHOW);
	}

	//��������
	PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_START"));

	//����ʱ��
	if(m_bAddScore[GetMeChairID()] == false && m_bIsAdd == true)
		SetGameClock(m_wCurrentUser,IDI_ADD,3);
	else if(m_bAddScore[GetMeChairID()] == true && m_bIsAdd == true)
		SetGameClock(m_wCurrentUser,IDI_ADD,30);
	else if(m_bIsAdd == false)
		SetGameClock(m_wCurrentUser,IDI_OUT_CARD,30);
	return true;
}
//�ɷ��˿�
bool CGameClientEngine::DispatchUserCard(BYTE cbCardData[GAME_PLAYER][20], BYTE cbCardCount)
{
	//���ñ���
	m_cbDispatchCardCount=0;
	m_cbRemnantCardCount=cbCardCount;
	for(BYTE i=0;i<GAME_PLAYER;i++)
	{ 
		CopyMemory(m_cbDispatchCardData[i],cbCardData[i],sizeof(BYTE)*cbCardCount);
	}
	//if(m_bMingCard[GetMeChairID()] == 0)
		//m_GameClientView.m_btMingCard.ShowWindow(SW_SHOW);

	//���ö�ʱ��
	SetTimer(IDI_DISPATCH_CARD,220,NULL);

	return true;
}

//��ʱ����Ϣ
void CGameClientEngine::OnTimer(UINT nIDEvent)
{
	if ((nIDEvent==IDI_MOST_CARD)&&(m_wMostUser!=INVALID_CHAIR))
	{
		//��������
		WORD wCurrentUser=m_wMostUser;
		m_wMostUser=INVALID_CHAIR;

		//ɾ����ʱ��
		KillTimer(IDI_MOST_CARD);

		//���ý���
		m_GameClientView.SetPassFlag(INVALID_CHAIR,false);
		for (WORD i=0;i<GAME_PLAYER;i++) 
			m_GameClientView.m_UserCardControl[i].SetCardData(NULL,0);

		//�������
		if ((IsLookonMode()==false)&&(wCurrentUser==GetMeChairID()))
		{
			ActiveGameFrame();
			m_GameClientView.m_btOutCard.ShowWindow(SW_SHOW);
			m_GameClientView.m_btPassCard.ShowWindow(SW_SHOW);
			m_GameClientView.m_btPassCard.EnableWindow(FALSE);
			m_GameClientView.m_btAutoOutCard.ShowWindow(SW_SHOW);
			m_GameClientView.m_btAutoOutCard.EnableWindow(FALSE);
			m_GameClientView.m_btOutCard.EnableWindow((VerdictOutCard()==true)?TRUE:FALSE);
		}
		//
		m_GameClientView.m_btMingCardState.ShowWindow(SW_HIDE);
		m_GameClientView.m_btMingCard.ShowWindow(SW_HIDE);

		//����ʱ��
		SetGameClock(wCurrentUser,IDI_OUT_CARD,30);

		return;
	}

	//�ɷ��˿�
	if (nIDEvent==IDI_DISPATCH_CARD)
	{
		//�ɷ�����
		if (m_cbRemnantCardCount>0)
		{
			//�˿�����
			BYTE cbUserCard[20];
			ZeroMemory(cbUserCard,sizeof(cbUserCard));

			//�ɷ��˿�
			m_cbRemnantCardCount--;
			m_cbDispatchCardCount++;
			//
			for(BYTE i=0;i<GAME_PLAYER;i++)
			{
				m_GameLogic.SortCardListEx(m_cbDispatchCardData[i],m_cbDispatchCardCount,ST_ORDER);
				m_GameLogic.SortCardListEx(m_bHandCardUser[i],m_bCardCount[i],ST_ORDER);
			}

			//���ý���
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				WORD wViewID = SwitchViewChairID(i) ;
				if (wViewID!=1)
				{
					m_GameClientView.m_HandCardControl[wViewID].SetCardData(m_cbDispatchCardData[i],m_cbDispatchCardCount);
					if(m_bMingCard[i] != 0)
						m_GameClientView.m_HandCardControl[wViewID].SetDisplayFlag(true) ;
					else
						m_GameClientView.m_HandCardControl[wViewID].SetDisplayFlag(false) ;
				}
				else m_GameClientView.m_HandCardControl[1].SetCardData(m_cbDispatchCardData[1],m_cbDispatchCardCount); 
			}

			//��������
			PlayGameSound(AfxGetInstanceHandle(),TEXT("DISPATCH_CARD"));
		}

		//��ֹ�ж�
		if (m_cbRemnantCardCount==0)
		{
			//ɾ����ʱ��
			KillTimer(IDI_DISPATCH_CARD);

			//�˿���Ŀ
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				m_GameClientView.SetCardCount(SwitchViewChairID(i),m_bCardCount[i]);
			}

			//��ǰ���
			if ((IsLookonMode()==false)&&(m_wCurrentUser==GetMeChairID()))
			{
				ActiveGameFrame();
				//m_GameClientView.m_btOneScore.ShowWindow(SW_SHOW);
				//m_GameClientView.m_btOneScore.EnableWindow(TRUE);
				//m_GameClientView.m_btTwoScore.ShowWindow(SW_SHOW);
				//m_GameClientView.m_btTwoScore.EnableWindow(TRUE);
				m_GameClientView.m_btThreeScore.ShowWindow(SW_SHOW);
				m_GameClientView.m_btThreeScore.EnableWindow(SW_SHOW);
				m_GameClientView.m_btGiveUpScore.ShowWindow(SW_SHOW);
				m_GameClientView.m_btGiveUpScore.EnableWindow(TRUE);
			}
			//
			m_GameClientView.m_btMingCardState.ShowWindow(SW_HIDE);
			m_GameClientView.m_btMingCard.ShowWindow(SW_HIDE);

			//���ý���
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				WORD wViewID = SwitchViewChairID(i) ;
				if (wViewID!=1)
				{
					BYTE cbCardData[20] ;
					ZeroMemory(cbCardData, sizeof(cbCardData)) ;
					m_GameClientView.m_HandCardControl[wViewID].SetCardData(m_cbDispatchCardData[i],17);
					if(m_bMingCard[i] != 0)
						m_GameClientView.m_HandCardControl[wViewID].SetDisplayFlag(true) ;
					else
						m_GameClientView.m_HandCardControl[wViewID].SetDisplayFlag(false) ;
				}
				else 
				{
					m_GameClientView.m_HandCardControl[wViewID].SetCardData(m_bHandCardData,m_bHandCardCount); 
					if(false==IsLookonMode())
					{
						m_GameClientView.m_HandCardControl[wViewID].SetDisplayFlag(true) ;
						m_GameClientView.m_HandCardControl[wViewID].SetPositively(true) ;
					}
				}
			}

			if(IsLookonMode()==false) m_GameClientView.m_btSortCard.EnableWindow(TRUE);

			//��������
			PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_START"));

			//����ʱ��
			if (m_wCurrentUser!=INVALID_CHAIR) SetGameClock(m_wCurrentUser,IDI_LAND_SCORE,30);
		}

		return;
	}


	//���Ӷ��� 
	if(nIDEvent == IDI_LAIZI)
	{
		m_GameClientView.SetLaiZiKuang(true);

		//����
		BYTE bRand=rand()%52;
		m_GameClientView.SetFrame(bRand%13,bRand%4);

		m_bLaiZiFrame++;

		m_GameClientView.RefreshGameView();

		//��������
		if(m_bLaiZiFrame >= 20)
		{
			//ɾ����ʱ��
			KillTimer(IDI_LAIZI);

			m_GameClientView.SetFrame(m_GameLogic.GetCardValue(m_bLaiZiData)-1,bRand%4);

			DoncludeFrame();
		}
	}
	__super::OnTimer(nIDEvent);
}

//��ʼ��ť
LRESULT CGameClientEngine::OnStart(WPARAM wParam, LPARAM lParam)
{
	//���ñ���
	m_wBombTime=1;
	m_wTimeOutCount=0;
	m_bHandCardCount=0;
	m_bTurnCardCount=0;
	m_bLaiZiFrame=0;
	m_bTurnOutType=CT_ERROR;
	m_wMostUser=INVALID_CHAIR;
	m_bLaiZiData=0;
	memset(m_bHandCardData,0,sizeof(m_bHandCardData));
	memset(m_bTurnCardData,0,sizeof(m_bTurnCardData));

	//���ý���
	KillGameClock(IDI_START_GAME);
	m_GameClientView.ShowLandTitle(false);
	m_GameClientView.SetBombTime(m_wBombTime);
	m_GameClientView.SetCardCount(INVALID_CHAIR,0);
	m_GameClientView.SetLandUser(INVALID_CHAIR,0);
	m_GameClientView.SetLandScore(INVALID_CHAIR,0);
	m_GameClientView.SetPassFlag(INVALID_CHAIR,false);
	m_GameClientView.SetLaiZiCard(0);
	m_GameClientView.SetLaiZiDisplay(false);

	//����
	m_GameClientView.SetLaiZiKuang(false);

	//���ƿ�ʼ
	m_GameClientView.m_btMingCardState.ShowWindow(SW_HIDE);

	//�ӱ���Ϣ
	for(BYTE i=0;i<GAME_PLAYER;i++)
		m_GameClientView.SetAddStates(i,false);

	//���ؿؼ�
	m_GameClientView.m_btStart.ShowWindow(FALSE);
	if (m_GameClientView.m_ScoreView.m_hWnd!=NULL) m_GameClientView.m_ScoreView.DestroyWindow();

	//�����˿�
	m_GameClientView.m_BackCardControl.SetCardData(NULL,0);
	m_GameClientView.m_HandCardControl[1].SetPositively(false);
	for (WORD i=0;i<GAME_PLAYER;i++) 
	{
		m_GameClientView.m_UserCardControl[i].SetCardData(NULL,0);
		m_GameClientView.m_HandCardControl[i].SetCardData(NULL,0);
		WORD wViewID = SwitchViewChairID(i) ;
		if(wViewID==1)	m_GameClientView.m_HandCardControl[wViewID].SetDisplayFlag(true);
		else m_GameClientView.m_HandCardControl[wViewID].SetDisplayFlag(false);
	}

	m_GameClientView.m_HandCardControl[1].SetLaiZivalue(m_GameLogic.GetCardValue(NULL));
	for(BYTE i=0;i<GAME_PLAYER;i++)
	{
		m_GameClientView.m_UserCardControl[i].SetLaiZivalue(m_GameLogic.GetCardValue(NULL));
	}

	//������Ϣ
	SendUserReady(NULL,0);

	////�����ɼ�
	//if (m_GameClientView.m_ScoreView.m_hWnd==NULL)
	//{
	//	m_GameClientView.m_ScoreView.Create(IDD_GAME_SCORE,&m_GameClientView);
	//	m_GameClientView.m_ScoreView.ShowWindow(SW_SHOW);
	//}

	return 0;
}

//������Ϣ
LRESULT CGameClientEngine::OnOutCard(WPARAM wParam, LPARAM lParam)
{
	//״̬�ж�
	if ((m_GameClientView.m_btOutCard.IsWindowEnabled()==FALSE)||
		(m_GameClientView.m_btOutCard.IsWindowVisible()==FALSE)) return 0;

	//���ý���
	KillGameClock(IDI_OUT_CARD);
	m_GameClientView.m_btOutCard.ShowWindow(SW_HIDE);
	m_GameClientView.m_btOutCard.EnableWindow(FALSE);
	m_GameClientView.m_btPassCard.ShowWindow(SW_HIDE);
	m_GameClientView.m_btPassCard.EnableWindow(FALSE);
	m_GameClientView.m_btAutoOutCard.ShowWindow(SW_HIDE);
	m_GameClientView.m_btAutoOutCard.EnableWindow(FALSE);
	m_GameClientView.m_btMingOut.ShowWindow(SW_HIDE);

	//��������
	CMD_C_OutCard OutCard;
	OutCard.bCardCount=(BYTE)m_GameClientView.m_HandCardControl[1].GetShootCard(OutCard.bCardData,CountArray(OutCard.bCardData));
	CopyMemory(OutCard.bChangeCard,m_bChangeCard,sizeof(m_bChangeCard));
	SendSocketData(SUB_C_OUT_CART,&OutCard,sizeof(OutCard)-sizeof(OutCard.bCardData)+OutCard.bCardCount*sizeof(BYTE));

	//Ԥ����ʾ
	int cbOutCardType = m_GameLogic.GetCardType(OutCard.bCardData,OutCard.bCardCount);
	if ( (cbOutCardType&(1<<CT_BOMB_CARD)) != 0 || (cbOutCardType&(1<<CT_MISSILE_CARD)) != 0 ||(m_bTurnOutType&(1<<CT_RUAN_BOMB))!= 0||(m_bTurnOutType&(1<<CT_LAIZI_BOMB))!= 0) 
		PlayGameSound(AfxGetInstanceHandle(),TEXT("BOMB_CARD"));
	else PlayGameSound(AfxGetInstanceHandle(),TEXT("OUT_CARD"));
	m_GameClientView.m_UserCardControl[1].SetCardData(OutCard.bCardData,OutCard.bCardCount);
	m_GameClientView.m_UserCardControl[1].SetNoChangeCard(OutCard.bCardData,OutCard.bCardCount);

	//Ԥ��ɾ��
	BYTE bSourceCount=m_bHandCardCount;
	m_bHandCardCount-=OutCard.bCardCount;
	m_GameLogic.RemoveCard(OutCard.bCardData,OutCard.bCardCount,m_bHandCardData,bSourceCount);
	m_GameClientView.m_HandCardControl[1].SetCardData(m_bHandCardData,m_bHandCardCount);
	m_GameClientView.m_HandCardControl[1].SetNoChangeCard(m_bHandCardData,m_bHandCardCount);

	//�����˿�
	BYTE cbSortType=(m_cbSortType==ST_ORDER)?ST_COUNT:ST_ORDER;
	m_GameLogic.SortCardListEx(m_bHandCardData,m_bHandCardCount,cbSortType);
	m_GameClientView.m_HandCardControl[1].SetCardData(m_bHandCardData,m_bHandCardCount);

	return 0;
}

//��������
LRESULT CGameClientEngine::OnPassCard(WPARAM wParam, LPARAM lParam)
{
	//״̬�ж�
	if (m_GameClientView.m_btPassCard.IsWindowEnabled()==FALSE) return 0;

	//���ý���
	KillGameClock(IDI_OUT_CARD);
	m_GameClientView.m_btOutCard.ShowWindow(SW_HIDE);
	m_GameClientView.m_btOutCard.EnableWindow(FALSE);
	m_GameClientView.m_btPassCard.ShowWindow(SW_HIDE);
	m_GameClientView.m_btPassCard.EnableWindow(FALSE);
	m_GameClientView.m_btAutoOutCard.ShowWindow(SW_HIDE);
	m_GameClientView.m_btAutoOutCard.EnableWindow(FALSE);

	//��������
	SendSocketData(SUB_C_PASS_CARD);

	//Ԥ����ʾ
	m_GameClientView.SetPassFlag(1,true);
	PlayGameSound(AfxGetInstanceHandle(),TEXT("PASS_CARD"));
	m_GameClientView.m_HandCardControl[1].SetCardData(m_bHandCardData,m_bHandCardCount);

	return 0;
}

//�з���Ϣ
LRESULT CGameClientEngine::OnLandScore(WPARAM wParam, LPARAM lParam)
{
	//���ý���
	KillGameClock(IDI_LAND_SCORE);
	//m_GameClientView.m_btOneScore.ShowWindow(SW_HIDE);
	//m_GameClientView.m_btTwoScore.ShowWindow(SW_HIDE);
	m_GameClientView.m_btGiveUpScore.ShowWindow(SW_HIDE);
	m_GameClientView.m_btThreeScore.ShowWindow(SW_HIDE);

	//��������
	CMD_C_LandScore LandScore;
	LandScore.bLandScore=(BYTE)wParam;
	SendSocketData(SUB_C_LAND_SCORE,&LandScore,sizeof(LandScore));

	return 0;
}

//������ʾ
LRESULT CGameClientEngine::OnAutoOutCard(WPARAM wParam, LPARAM lParam)
{
	//�������
	tagOutCardResult OutCardResult;
	m_GameLogic.SearchOutCard(m_bHandCardData,m_bHandCardCount,m_bTurnCardData,m_bTurnCardCount,OutCardResult,m_bChangeTurnOut);

	//���ý���
	if (OutCardResult.cbCardCount>0)
	{
		//���ý���
		m_GameClientView.m_HandCardControl[1].SetShootCard(OutCardResult.cbResultCard,OutCardResult.cbCardCount);

		//���ÿؼ�
		bool bOutCard=VerdictOutCard();
		m_GameClientView.m_btOutCard.EnableWindow((bOutCard==true)?TRUE:FALSE);
	}
	else 
	{
		//��������
		OnPassCard(0,0);
	}

	return 0;
}

//�Ҽ��˿�
LRESULT CGameClientEngine::OnLeftHitCard(WPARAM wParam, LPARAM lParam)
{
	//���ÿؼ�
	bool bOutCard=VerdictOutCard();
	m_GameClientView.m_btOutCard.EnableWindow(bOutCard?TRUE:FALSE);

	return 0;
}

//����˿�
LRESULT CGameClientEngine::OnRightHitCard(WPARAM wParam, LPARAM lParam)
{
	//�û�����
	OnOutCard(0,0);

	return 0;
}

//////////////////////////////////////////////////////////////////////////

LRESULT CGameClientEngine::OnAutoPlay(WPARAM wParam, LPARAM lParam)
{
    bool bAutoPlay = (wParam == 1);
    if (bAutoPlay == m_bAutoPlay)
        return 0;

    m_bAutoPlay = bAutoPlay;

    if (m_bAutoPlay)
		if(m_pIStringMessage != NULL)
			m_pIStringMessage->InsertSystemString(TEXT("ʹ���йܹ���, ��������Զ�����״̬!"));
    else
		if(m_pIStringMessage != NULL)
			m_pIStringMessage->InsertSystemString(TEXT("ȡ���йܹ���. "));

	//������Ϣ
	CMD_C_UserTrustee UserTrustee;
	ZeroMemory( &UserTrustee, sizeof( UserTrustee ) );
	UserTrustee.bTrustee = m_bAutoPlay;
	UserTrustee.wUserChairID = GetMeChairID();

	SendSocketData( SUB_C_TRUSTEE, &UserTrustee, sizeof( UserTrustee ) );

    return 0;
}


LRESULT CGameClientEngine::OnMessageSortCard(WPARAM wParam, LPARAM lParam)
{

	//�����˿�
	WORD wMeChairID=GetMeChairID();
	m_GameLogic.SortCardListEx(m_bHandCardData,m_bHandCardCount,m_cbSortType);

	//��ȡ�˿�
	m_GameClientView.m_HandCardControl[1].SetCardData(m_bHandCardData,m_bHandCardCount);

	//���ñ���
	m_cbSortType=(m_cbSortType==ST_ORDER)?ST_COUNT:ST_ORDER;

	//���ð�ť
	UINT uImageID=(m_cbSortType==ST_ORDER)?IDB_ORDER_SORT:IDB_COUNT_SORT;
	m_GameClientView.m_btSortCard.SetButtonImage(uImageID,AfxGetInstanceHandle(),false,false);

	//���½���
	m_GameClientView.RefreshGameView();

	return 0;
}
//
LRESULT CGameClientEngine::OnMingCardState(WPARAM wParam,LPARAM lParam)
{
		//���ñ���
	m_wBombTime=1;
	m_wTimeOutCount=0;
	m_bHandCardCount=0;
	m_bTurnCardCount=0;
	m_bTurnOutType=CT_ERROR;
	m_wMostUser=INVALID_CHAIR;
	memset(m_bHandCardData,0,sizeof(m_bHandCardData));
	memset(m_bTurnCardData,0,sizeof(m_bTurnCardData));

	//���ý���
	KillGameClock(IDI_START_GAME);
	m_GameClientView.ShowLandTitle(false);
	m_GameClientView.SetBombTime(m_wBombTime);
	m_GameClientView.SetCardCount(INVALID_CHAIR,0);
	m_GameClientView.SetLandUser(INVALID_CHAIR,0);
	m_GameClientView.SetLandScore(INVALID_CHAIR,0);
	m_GameClientView.SetPassFlag(INVALID_CHAIR,false);
	m_GameClientView.SetLaiZiCard(0);
	m_GameClientView.SetLaiZiDisplay(false);


	//���ؿؼ�
	m_GameClientView.m_btStart.ShowWindow(FALSE);
	if (m_GameClientView.m_ScoreView.m_hWnd!=NULL) m_GameClientView.m_ScoreView.DestroyWindow();

	//�����˿�
	m_GameClientView.m_BackCardControl.SetCardData(NULL,0);
	m_GameClientView.m_HandCardControl[1].SetPositively(false);
	for (WORD i=0;i<GAME_PLAYER;i++) 
	{
		m_GameClientView.m_UserCardControl[i].SetCardData(NULL,0);
		m_GameClientView.m_HandCardControl[i].SetCardData(NULL,0);
		WORD wViewID = SwitchViewChairID(i) ;
		if(wViewID==1)	m_GameClientView.m_HandCardControl[wViewID].SetDisplayFlag(true);
		else m_GameClientView.m_HandCardControl[wViewID].SetDisplayFlag(false);
	}

	//������Ϣ
	CMD_C_MingState MingState;
	MingState.bMingState=1;
	MingState.wUserChairID=GetMeChairID();

	SendSocketData(SUB_C_MINGSTATE,&MingState,sizeof(MingState));
	SendUserReady(NULL,0);

	return 0;
}

//����
LRESULT CGameClientEngine::OnMingCard(WPARAM wParam,LPARAM lParam)
{
	CMD_C_MingState MingState;
	MingState.bMingState=2;
	MingState.wUserChairID=GetMeChairID();

	SendSocketData(SUB_C_MINGSTATE,&MingState,sizeof(MingState));

	return 0;
}

//������
LRESULT CGameClientEngine::OnQiangLand(WPARAM wParam,LPARAM lParam)
{
	CMD_C_QiangLand QiangLand;
	QiangLand.bQiangLand=(bool)wParam;

	SendSocketData(SUB_C_QIANGLAND,&QiangLand,sizeof(QiangLand));

	//���ý���
	m_GameClientView.m_btQiangLand.ShowWindow(SW_HIDE);
	m_GameClientView.m_btNoQiang.ShowWindow(SW_HIDE);

	return 0;
}
//��������
LRESULT CGameClientEngine::OnMingOut(WPARAM wParam,LPARAM lParam)
{
	SendSocketData(SUB_C_MINGOUT);
	return 0;
}
//�ӱ�
LRESULT CGameClientEngine::OnAdd(WPARAM wParam,LPARAM lParam)
{
	//����ʱ��
	KillGameClock(IDI_ADD);

	//���ý���
	m_GameClientView.m_btAdd.ShowWindow(SW_HIDE);
	m_GameClientView.m_btNoAdd.ShowWindow(SW_HIDE);

	CMD_C_ADD Add;
	Add.bAdd=(bool)wParam;

	SendSocketData(SUB_C_ADD,&Add,sizeof(Add));
	return 0;
}