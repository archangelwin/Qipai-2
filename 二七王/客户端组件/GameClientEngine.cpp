#include "StdAfx.h"
#include "Resource.h"
#include "GameClient.h"
#include "GameClientEngine.h"

//////////////////////////////////////////////////////////////////////////////////
//ʱ���ʶ
#define IDI_OUT_CARD					200								//���ƶ�ʱ��
#define IDI_CALL_CARD					201								//���ƶ�ʱ��
#define IDI_START_GAME					202								//��ʼ��ʱ��
#define IDI_SEND_CONCEAL				203								//���ƶ�ʱ��
#define IDI_LAND_SCORE					204								//�зֶ�ʱ��

//ʱ�䷶Χ
#define TIME_START_GAME					30								//��ʼʱ��
#define TIME_SEND_CONCEAL				50								//����ʱ��


//#ifdef DEBUG
//#define TIME_OUT_CARD					300								//����ʱ��
//#define TIME_CALL_CARD					300								//����ʱ��
//#define TIME_LAND_SCORE					300								//����ʱ��
//#else
#define TIME_OUT_CARD					20								//����ʱ��
#define TIME_CALL_CARD					15								//����ʱ��
#define TIME_LAND_SCORE					20								//����ʱ��
//#endif

//���ܶ�ʱ��
#define IDI_CONCEAL_CARD				300								//���ƶ�ʱ��
#define IDI_DISPATCH_CARD				301								//���ƶ�ʱ��
#define IDI_LAST_TURN_CARD				302								//���ֶ�ʱ��
#define IDI_THROW_CARD_RESULT			303								//˦�ƶ�ʱ��

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CGameClientEngine, CGameFrameEngine)
	ON_WM_TIMER()
	ON_MESSAGE(IDM_START,OnStart)
	ON_MESSAGE(IDM_OUT_CARD,OnOutCard)
	ON_MESSAGE(IDM_CALL_CARD,OnCallCard)
	ON_MESSAGE(IDM_OUT_PROMPT,OnOutPrompt)
	ON_MESSAGE(IDM_SEND_CONCEAL,OnSendConceal)
	ON_MESSAGE(IDM_CONCEAL_CARD,OnConcealCard)
	//ON_MESSAGE(IDM_REQUEST_LEAVE,OnRuquestLeave)
	ON_MESSAGE(IDM_LAST_TURN_CARD,OnLastTurnCard)
	ON_MESSAGE(IDM_TRUSTEE_CONTROL,OnStusteeControl)
	ON_MESSAGE(IDM_LEFT_HIT_CARD,OnLeftHitCard)
	ON_MESSAGE(IDM_RIGHT_HIT_CARD,OnRightHitCard)

	ON_MESSAGE(IDM_LAND_SCORE,OnLandScore)
	ON_MESSAGE(IDM_GIVEUP_GAME,OnGiveUpGame)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////////////
LRESULT CGameClientEngine::OnGiveUpGame(WPARAM wParam, LPARAM lParam)
{
	m_GameClientView.m_btGiveUp.ShowWindow(SW_HIDE);
	SendSocketData(SUB_C_GIVEUP_GAME);
	return 0;
}


//���캯��
CGameClientEngine::CGameClientEngine()
{
	//�߼�����
	m_cbPackCount=0;
	m_cbMainColor=COLOR_ERROR;
	m_cbMainValue=VALUE_ERROR;
	m_lCellScore=1;

	//������Ϣ
	m_cbCallCard=0;
	m_cbCallCount=0;
	m_wCallCardUser=INVALID_CHAIR;

	//״̬����
	m_wBankerUser=INVALID_CHAIR;
	m_wCurrentUser=INVALID_CHAIR;
	ZeroMemory(m_bUserNoMain,sizeof(m_bUserNoMain));

	//�÷���Ϣ
	m_cbScoreCardCount=0;
	ZeroMemory(m_cbScoreCardData,sizeof(m_cbScoreCardData));

	//˦�Ʊ���
	m_bThrowCard=false;
	m_cbResultCardCount=0;
	m_wThrowCardUser=INVALID_CHAIR;
	ZeroMemory(m_cbResultCardData,sizeof(m_cbResultCardData));
	m_GameLogic.SetCanThrow(false);

	//���Ʊ���
	m_bLastTurn=false;
	m_wFirstOutUser=INVALID_CHAIR;
	ZeroMemory(m_cbOutCardData,sizeof(m_cbOutCardData));
	ZeroMemory(m_cbOutCardCount,sizeof(m_cbOutCardCount));

	//�����˿�
	m_cbConcealCount=0;
	ZeroMemory(m_cbConcealCard,sizeof(m_cbConcealCard));

	//�û��˿�
	m_cbHandCardCount=0;
	ZeroMemory(m_cbHandCardData,sizeof(m_cbHandCardData));

	//���ñ���
	m_bStustee=false;
	m_bDeasilOrder=false;
	m_dwCardHSpace=18;//DEFAULT_PELS;
	m_bOverTimes=0;

	//��������
	m_cbRemnantCardCount=0;
	m_cbDispatchCardCount=0;
	ZeroMemory(m_cbDispatchCardData,sizeof(m_cbDispatchCardData));

}

//��������
CGameClientEngine::~CGameClientEngine()
{
}

//��������
bool CGameClientEngine::OnInitGameEngine()
{
	//��������
	HICON hIcon=LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDR_MAINFRAME));
	m_pIClientKernel->SetGameAttribute(KIND_ID,GAME_PLAYER,VERSION_CLIENT,hIcon,GAME_NAME);
	//���ÿؼ�
	m_GameClientView.SetUserOrder(m_bDeasilOrder);
	m_GameClientView.m_HandCardControl.SetCardSpace(m_dwCardHSpace,0,DEF_CARD_SHOOT);

	return true;
}

//���ú���
bool CGameClientEngine::OnResetGameEngine()
{
	//�߼�����
	m_cbPackCount=0;
	m_cbMainColor=COLOR_ERROR;
	m_cbMainValue=VALUE_ERROR;
	m_GameLogic.SetCanThrow(false);

	//������Ϣ
	m_cbCallCard=0;
	m_cbCallCount=0;
	m_wCallCardUser=INVALID_CHAIR;

	//״̬����
	m_wBankerUser=INVALID_CHAIR;
	m_wCurrentUser=INVALID_CHAIR;
	ZeroMemory(m_bUserNoMain,sizeof(m_bUserNoMain));

	//�÷���Ϣ
	m_cbScoreCardCount=0;
	ZeroMemory(m_cbScoreCardData,sizeof(m_cbScoreCardData));
	m_GameClientView.SetCardScore(0);
	m_GameClientView.SetPlayerScore(0);
	m_GameClientView.SetMainCount(0,0);

	//˦�Ʊ���
	m_bThrowCard=false;
	m_cbResultCardCount=0;
	m_wThrowCardUser=INVALID_CHAIR;
	ZeroMemory(m_cbResultCardData,sizeof(m_cbResultCardData));

	//���Ʊ���
	m_bLastTurn=false;
	m_wFirstOutUser=INVALID_CHAIR;
	ZeroMemory(m_cbOutCardData,sizeof(m_cbOutCardData));
	ZeroMemory(m_cbOutCardCount,sizeof(m_cbOutCardCount));

	//�����˿�
	m_cbConcealCount=0;
	ZeroMemory(m_cbConcealCard,sizeof(m_cbConcealCard));

	//�û��˿�
	m_cbHandCardCount=0;
	ZeroMemory(m_cbHandCardData,sizeof(m_cbHandCardData));

	//���ñ���
	m_bStustee=false;
	m_bOverTimes=0;

	//��������
	m_cbRemnantCardCount=0;
	m_cbDispatchCardCount=0;
	ZeroMemory(m_cbDispatchCardData,sizeof(m_cbDispatchCardData));

	//ɾ����ʱ��
	KillTimer(IDI_CONCEAL_CARD);
	KillTimer(IDI_DISPATCH_CARD);
	KillTimer(IDI_LAST_TURN_CARD);
	KillTimer(IDI_THROW_CARD_RESULT);
	return true;
}

//ʱ��ɾ��
bool CGameClientEngine::OnEventGameClockKill(WORD wChairID)
{
	return true;
}

//ʱ����Ϣ
bool CGameClientEngine::OnEventGameClockInfo(WORD wChairID, UINT nElapse, WORD wClockID)
{
	switch (wClockID)
	{
	case IDI_LAND_SCORE:
		{
			//��ʱ�ж�
			if ((nElapse==0)||((m_bStustee==true)&&(nElapse<(TIME_LAND_SCORE))))
			{
				KillGameClock(IDI_LAND_SCORE);
				if ((IsLookonMode()==false)&&(wChairID==GetMeChairID())) OnLandScore(255,255);
				return false;
			}

			//��������
			if ((nElapse<=3)&&(wChairID==GetMeChairID())&&(IsLookonMode()==false)) PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_WARN"));

			return true;		
		}
	case IDI_OUT_CARD:			//�û�����
		{
#ifdef DEBUG
			if (m_bStustee==true)
#else
			//��ʱ�ж�
			if ((nElapse==0)||((m_bStustee==true)&&(nElapse<(TIME_OUT_CARD))))
#endif
			{
				//if ((IsLookonMode()==false)&&(wChairID==GetMeChairID())) m_bOverTimes++;
				//��ʱ5��
				//if(m_bOverTimes>=5) SendSocketData(SUB_C_EJECT);
				if ((IsLookonMode()==false)&&(wChairID==GetMeChairID())) 
				{
					AutomatismOutCard();
				}
				return false;
			}

			//��������
			if ((nElapse<=3)&&(wChairID==GetMeChairID())&&(IsLookonMode()==false)) PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_WARN"));

			return true;
		}
	case IDI_CALL_CARD:			//�û�����
		{
			//��ʱ�ж�
			if ((nElapse==0)||((m_bStustee==true)&&(nElapse<(TIME_CALL_CARD))))
			{
				//���ؿؼ�
				m_GameClientView.m_CallCardWnd.ShowWindow(SW_HIDE);

				//������Ϣ
				if (IsLookonMode()==false) 
				{
					//�������
					BYTE color[]={0x00,0x10,0x20,0x30};
					srand((DWORD)time(NULL));
					int randColor=rand()%4;

					//��������
					CMD_C_CallCard CallCard;
					CallCard.cbCallCard=color[randColor]+0x02;
					CallCard.cbCallCount=1;

					//��������
					SendSocketData(SUB_C_CALL_CARD,&CallCard,sizeof(CallCard));
					KillGameClock(IDI_CALL_CARD);
				}

				return false;
			}

			return true;
		}
	case IDI_START_GAME:		//��ʼ��Ϸ
		{
			if (nElapse==0)
			{
				AfxGetMainWnd()->PostMessage(WM_CLOSE);
				return false;
			}
			if ((nElapse<=3)&&(wChairID==GetMeChairID())&&(IsLookonMode()==false)) PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_WARN"));

			return true;
		}
	case IDI_SEND_CONCEAL:		//���͵���
		{
			if ((nElapse==0)||((m_bStustee==true)&&(nElapse<(TIME_SEND_CONCEAL))))
			{
				//�Զ�����
				if ((IsLookonMode()==false)&&(wChairID==GetMeChairID()))
				{
					//ɾ����ʱ��
					KillGameClock(IDI_SEND_CONCEAL);

					//ɾ���˿�
					m_cbHandCardCount-=m_cbConcealCount;
					m_GameLogic.RemoveCard(m_cbConcealCard,m_cbConcealCount,m_cbHandCardData,m_cbHandCardCount+m_cbConcealCount);

					//���ý���
					m_GameClientView.m_btSendConceal.ShowWindow(SW_HIDE);
					m_GameClientView.m_btSendConceal.EnableWindow(FALSE);
					SetHandCardControl(m_cbHandCardData,m_cbHandCardCount);

					//��������
					CMD_C_ConcealCard ConcealCard;
					ConcealCard.cbConcealCount=m_cbConcealCount;
					CopyMemory(ConcealCard.cbConcealCard,m_cbConcealCard,m_cbConcealCount*sizeof(BYTE));

					//��������
					SendSocketData(SUB_C_CONCEAL_CARD,&ConcealCard,sizeof(ConcealCard));

					//��ʾ��Ϣ
					if (m_bStustee==false && m_pIStringMessage!=NULL)
						m_pIStringMessage->InsertSystemString(TEXT("�޶���ʱ�䵽�ˣ�ϵͳΪ���Զ������"));

					return false;
				}

				return false;
			}
			if ((nElapse<=3)&&(wChairID==GetMeChairID())&&(IsLookonMode()==false)) PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_WARN"));

			return true;
		}
	}

	return false;
}

//�Թ���Ϣ
bool CGameClientEngine::OnEventLookonMode(VOID * pData, WORD wDataSize)
{
	//�˿˿���
	m_GameClientView.m_HandCardControl.SetDisplayFlag((IsAllowLookon()==true));

	//���ܰ�ť
	m_GameClientView.m_btConcealCard.EnableWindow(((m_cbConcealCount>0)&&((IsLookonMode()==false)||(IsAllowLookon()==true)))?TRUE:FALSE);

	return true;
}

//��Ϸ��Ϣ
bool CGameClientEngine::OnEventGameMessage(WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
	switch (wSubCmdID)
	{
	case SUB_S_LAND_SCORE:		//�û��з�
		{
			return OnSubLandScore(pData,wDataSize);
		}
	case SUB_S_GAME_START:		//��Ϸ��ʼ
		{
			return OnSubGameStart(pData,wDataSize);
		}
	case SUB_S_SEND_CARD:		//�����˿�
		{
			return OnSubSendCard(pData,wDataSize);
		}
	case SUB_S_CALL_CARD:		//�û�����
		{
			return OnSubCallCard(pData,wDataSize);
		}
	case SUB_S_SEND_CONCEAL:	//���͵���
		{
			return OnSubSendConceal(pData,wDataSize);
		}
	case SUB_S_GAME_PLAY:		//��Ϸ��ʼ
		{
			return OnSubGamePlay(pData,wDataSize);
		}
	case SUB_S_OUT_CARD:		//�û�����
		{
			return OnSubOutCard(pData,wDataSize);
		}
	case SUB_S_THROW_RESULT:	//˦�ƽ��
		{
			return OnSubThrowResult(pData,wDataSize);
		}
	case SUB_S_TURN_BALANCE:	//һ�ֽ���
		{
			return OnSubTurnBalance(pData,wDataSize);
		}
	case SUB_S_GAME_END:		//��Ϸ����
		{
			return OnSubGameEnd(pData,wDataSize);
		}
	case SUB_S_GM_CARD:
		{
			return OnSubGMCard(pData,wDataSize);
		}
	}
	return true;
}

//������Ϣ
bool CGameClientEngine::OnEventSceneMessage(BYTE cbGameStatus, bool bLookonUser, VOID * pData, WORD wDataSize)
{
	switch (cbGameStatus)
	{
	case GS_UG_SCORE:
		{
			//Ч������
			if (wDataSize!=sizeof(CMD_S_StatusScore)) return false;
			CMD_S_StatusScore * pStatusScore=(CMD_S_StatusScore *)pData;

			m_wCurrentUser=pStatusScore->wCurrentUser;
			m_wBankerUser=pStatusScore->wBankerUser;

			CopyMemory(m_cbConcealCard,pStatusScore->cbBackCardData,sizeof(pStatusScore->cbBackCardData));
			CopyMemory(m_cbHandCardData,pStatusScore->cbCardData,sizeof(pStatusScore->cbCardData));

			m_cbHandCardCount=25;
			m_GameLogic.SortCardList(m_cbHandCardData,m_cbHandCardCount);
			m_GameClientView.m_BackCardControl.SetCardData(m_cbConcealCard,10);
			m_GameClientView.m_HandCardControl.SetCardData(m_cbHandCardData,m_cbHandCardCount);

			//�������
			if (IsLookonMode()==false) 
			{
				m_GameClientView.m_btStusteeControl.EnableWindow(TRUE);
				m_GameClientView.m_HandCardControl.SetDisplayFlag(true);
				m_GameClientView.m_BackCardControl.SetDisplayFlag(false);
			}

			//���ÿؼ�
			if (IsLookonMode()==false&&pStatusScore->wCurrentUser==GetMeChairID())
			{
				m_GameClientView.m_CallScoreView.ShowWindow(SW_SHOW);
				m_GameClientView.SetCurrentCallScore(pStatusScore->bLandScore);
			}

			//ʱ������
			SetGameClock(m_wCurrentUser,IDI_LAND_SCORE,TIME_LAND_SCORE);
			//GM�ʺ�
			if((IsLookonMode()==true)/*&&GetUserData(GetMeChairID())->dwMasterRight>0*/)
			{
				SendSocketData(SUB_C_GM_REQUEST);
			}
			return true;

		}
	case GS_UG_FREE:	//����״̬
		{
			//Ч������
			if (wDataSize!=sizeof(CMD_S_StatusFree)) return false;
			CMD_S_StatusFree * pStatusFree=(CMD_S_StatusFree *)pData;



			m_lCellScore=pStatusFree->lBaseScore;

			//�������
			if (IsLookonMode()==false)
			{
				m_GameClientView.m_btStusteeControl.EnableWindow(TRUE);
				m_GameClientView.m_HandCardControl.SetDisplayFlag(true);

			}
			//���ÿؼ�
			if (IsLookonMode()==false && GetMeUserItem()->GetUserStatus()!=US_READY)
			{
				m_GameClientView.m_btStart.ShowWindow(SW_SHOW);
				m_GameClientView.m_btStart.SetFocus();
				SetGameClock(GetMeChairID(),IDI_START_GAME,TIME_START_GAME);
			}
			return true;
		}
	case GS_UG_CALL:	//����״̬
		{
			//Ч������
			if (wDataSize!=sizeof(CMD_S_StatusCall)) return false;
			CMD_S_StatusCall * pStatusCall=(CMD_S_StatusCall *)pData;

			m_lCellScore=pStatusCall->lBaseScore;

			//�߼�����
			m_cbPackCount=pStatusCall->cbPackCount;
			m_cbMainValue=pStatusCall->cbMainValue;
			m_wBankerUser=pStatusCall->wBankerUser;

			//������Ϣ
			m_cbCallCard=pStatusCall->cbCallCard;
			m_cbCallCount=pStatusCall->cbCallCount;
			m_wCallCardUser=pStatusCall->wCallCardUser;

			//�����˿�
			m_cbHandCardCount=pStatusCall->cbCardCount;
			CopyMemory(m_cbHandCardData,pStatusCall->cbCardData,sizeof(BYTE)*m_cbHandCardCount);

			//�߼����
			m_GameLogic.SetPackCount(m_cbPackCount);
			m_GameLogic.SetMainColor(m_cbMainColor);
			m_GameLogic.SetMainValue(m_cbMainValue);

			//������Ϣ
			WORD wMeChairID=GetMeChairID();
			BYTE cbValueMySelf=pStatusCall->cbValueOrder[wMeChairID%2];
			BYTE cbValueOther=pStatusCall->cbValueOrder[(wMeChairID+1)%2];
			m_GameClientView.SetValueOrder(m_cbMainValue,cbValueMySelf,cbValueOther);

			//������Ϣ
			if (m_wCallCardUser!=INVALID_CHAIR)
			{
				WORD wViewChairID=SwitchViewChairID(m_wCallCardUser);
				BYTE cbCallColor=m_GameLogic.GetCardColor(m_cbCallCard);
				m_GameClientView.SetCallCardInfo(wViewChairID,cbCallColor,m_cbCallCount);
			}
			m_GameClientView.SetCurrentCallScore(pStatusCall->wLandScore);

			//���ƽ���
			BYTE cbCallCard[MAX_PACK];
			WORD wViewChairID=SwitchViewChairID(m_wCallCardUser);
			for (BYTE i=0;i<m_cbCallCount;i++) cbCallCard[i]=m_cbCallCard;
			m_GameClientView.m_UserCardControl[wViewChairID].SetCardData(cbCallCard,m_cbCallCount);

			//����ׯ��
			if (m_wBankerUser!=INVALID_CHAIR) m_GameClientView.SetBankerUser(SwitchViewChairID(m_wBankerUser));

			//�������
			if (IsLookonMode()==false) 
			{
				m_GameClientView.m_btStusteeControl.EnableWindow(TRUE);
				m_GameClientView.m_HandCardControl.SetDisplayFlag(true);
			}

			//�˿�����
			if (pStatusCall->cbComplete==FALSE)
			{
				//��������
				m_GameClientView.m_CallCardWnd.ShowWindow(SW_SHOW);

				//�ɷ��˿�
				DispatchUserCard(m_cbHandCardData,m_cbHandCardCount);
			}
			else
			{
				//�����˿�
				m_GameLogic.SortCardList(m_cbHandCardData,m_cbHandCardCount);

				//�����˿�
				SetHandCardControl(m_cbHandCardData,m_cbHandCardCount);
			}

			//GM�ʺ�
			if((IsLookonMode()==true)/*&&GetUserData(GetMeChairID())->dwMasterRight>0*/)
			{
				SendSocketData(SUB_C_GM_REQUEST);
			}

			return true;
		}
	case GS_UG_BACK:	//����״̬
		{
			//Ч������
			if (wDataSize!=sizeof(CMD_S_StatusBack)) return false;
			CMD_S_StatusBack * pStatusBack=(CMD_S_StatusBack *)pData;

			//�߼�����
			m_cbPackCount=pStatusBack->cbPackCount;
			m_cbMainColor=pStatusBack->cbMainColor;
			m_cbMainValue=pStatusBack->cbMainValue;
			m_lCellScore=pStatusBack->lBaseScore;

			//������Ϣ
			m_cbCallCard=pStatusBack->cbCallCard;
			m_cbCallCount=pStatusBack->cbCallCount;
			m_wCallCardUser=pStatusBack->wCallCardUser;

			//�û�����
			m_wBankerUser=pStatusBack->wBankerUser;
			m_wCurrentUser=pStatusBack->wCurrentUser;
			m_wCallCardUser=pStatusBack->wCallCardUser;

			//�߼����
			m_GameLogic.SetPackCount(m_cbPackCount);
			m_GameLogic.SetMainColor(m_cbMainColor);
			m_GameLogic.SetMainValue(m_cbMainValue);

			//���õ���
			m_cbConcealCount=pStatusBack->cbConcealCount;
			CopyMemory(m_cbConcealCard,pStatusBack->cbConcealCard,m_cbConcealCount*sizeof(BYTE));

			//�����˿�
			m_cbHandCardCount=pStatusBack->cbCardCount;
			CopyMemory(m_cbHandCardData,pStatusBack->cbCardData,sizeof(BYTE)*m_cbHandCardCount);

			//������Ϣ
			WORD wMeChairID=GetMeChairID();
			BYTE cbValueMySelf=pStatusBack->cbValueOrder[wMeChairID%2];
			BYTE cbValueOther=pStatusBack->cbValueOrder[(wMeChairID+1)%2];
			m_GameClientView.SetValueOrder(m_cbMainValue,cbValueMySelf,cbValueOther);

			//������Ϣ
			if (m_wCallCardUser!=INVALID_CHAIR)
			{
				WORD wViewChairID=SwitchViewChairID(m_wCallCardUser);
				BYTE cbCallColor=m_GameLogic.GetCardColor(m_cbCallCard);
				m_GameClientView.SetCallCardInfo(wViewChairID,cbCallColor,m_cbCallCount);
			}
			m_GameClientView.SetCurrentCallScore(pStatusBack->wLandScore);

			//�����˿�
			m_GameLogic.SortCardList(m_cbHandCardData,m_cbHandCardCount);

			//��������
			SetHandCardControl(m_cbHandCardData,m_cbHandCardCount);
			m_GameClientView.SetBankerUser(SwitchViewChairID(m_wBankerUser));

			//�������
			if (IsLookonMode()==false) 
			{
				//�˿�����
				m_GameClientView.m_btStusteeControl.EnableWindow(TRUE);
				m_GameClientView.m_HandCardControl.SetDisplayFlag(true);

				//��������
				if (m_wCurrentUser==GetMeChairID())
				{
					m_GameClientView.m_btSendConceal.EnableWindow(FALSE);
					m_GameClientView.m_btSendConceal.ShowWindow(SW_SHOW);
					m_GameClientView.m_HandCardControl.SetPositively(true);
					m_GameClientView.m_HandCardControl.SetShootCard(m_cbConcealCard,m_cbConcealCount);
				}
				else m_GameClientView.SetWaitConceal(true);
			}
			else m_GameClientView.SetWaitConceal(true);

			//ʱ������
			SetGameClock(m_wCurrentUser,IDI_SEND_CONCEAL,TIME_SEND_CONCEAL);

			//GM�ʺ�
			if((IsLookonMode()==true)/*&&GetUserData(GetMeChairID())->dwMasterRight>0*/)
			{
				SendSocketData(SUB_C_GM_REQUEST);
			}

			return true;
		}
	case GS_UG_PLAY:	//��Ϸ״̬
		{
			//Ч������
			if (wDataSize!=sizeof(CMD_S_StatusPlay)) return false;
			CMD_S_StatusPlay * pStatusPlay=(CMD_S_StatusPlay *)pData;

			//�߼�����
			m_cbPackCount=pStatusPlay->cbPackCount;
			m_cbMainColor=pStatusPlay->cbMainColor;
			m_cbMainValue=pStatusPlay->cbMainValue;
			m_lCellScore=pStatusPlay->lBaseScore;

			//������Ϣ
			m_cbCallCard=pStatusPlay->cbCallCard;
			m_cbCallCount=pStatusPlay->cbCallCount;
			m_wCallCardUser=pStatusPlay->wCallCardUser;

			//�û�����
			m_wBankerUser=pStatusPlay->wBankerUser;
			m_wCurrentUser=pStatusPlay->wCurrentUser;
			m_wFirstOutUser=pStatusPlay->wFirstOutUser;

			//�߼����
			m_GameLogic.SetPackCount(m_cbPackCount);
			m_GameLogic.SetMainColor(m_cbMainColor);
			m_GameLogic.SetMainValue(m_cbMainValue);

			//���õ���
			m_cbConcealCount=pStatusPlay->cbConcealCount;
			CopyMemory(m_cbConcealCard,pStatusPlay->cbConcealCard,m_cbConcealCount*sizeof(BYTE));

			//�����˿�
			m_cbHandCardCount=pStatusPlay->cbCardCount;
			CopyMemory(m_cbHandCardData,pStatusPlay->cbCardData,sizeof(BYTE)*m_cbHandCardCount);

			//���Ʊ���
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				m_cbOutCardCount[i][0]=pStatusPlay->cbOutCardCount[i];
				CopyMemory(m_cbOutCardData[i][0],pStatusPlay->cbOutCardData[i],sizeof(BYTE)*m_cbOutCardCount[i][0]);
			}

			//�÷���Ϣ
			m_cbScoreCardCount=pStatusPlay->cbScoreCardCount;
			CopyMemory(m_cbScoreCardData,pStatusPlay->cbScoreCardData,sizeof(BYTE)*m_cbScoreCardCount);

			//������Ϣ
			WORD wMeChairID=GetMeChairID();
			BYTE cbValueMySelf=pStatusPlay->cbValueOrder[wMeChairID%2];
			BYTE cbValueOther=pStatusPlay->cbValueOrder[(wMeChairID+1)%2];
			m_GameClientView.SetValueOrder(m_cbMainValue,cbValueMySelf,cbValueOther);

			//������Ϣ
			if (m_wCallCardUser!=INVALID_CHAIR)
			{
				WORD wViewChairID=SwitchViewChairID(m_wCallCardUser);
				BYTE cbCallColor=m_GameLogic.GetCardColor(m_cbCallCard);
				m_GameClientView.SetCallCardInfo(wViewChairID,cbCallColor,m_cbCallCount);
			}
			m_GameClientView.SetCurrentCallScore(pStatusPlay->wLandScore);

			//�÷���ͼ
			m_GameClientView.m_CardScore.SetCardData(m_cbScoreCardData,m_cbScoreCardCount);
			m_GameClientView.SetCardScore(m_GameLogic.GetCardScore(m_cbScoreCardData,m_cbScoreCardCount));

			//���ƽ���
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				WORD wViewChairID=SwitchViewChairID(i);
				m_GameClientView.m_UserCardControl[wViewChairID].SetCardData(m_cbOutCardData[i][0],m_cbOutCardCount[i][0]);
			}

			//�����˿�
			m_GameLogic.SortCardList(m_cbHandCardData,m_cbHandCardCount);

			//��������
			SetHandCardControl(m_cbHandCardData,m_cbHandCardCount);
			m_GameClientView.SetBankerUser(SwitchViewChairID(m_wBankerUser));

			//�������
			if (IsLookonMode()==false) 
			{
				m_GameClientView.m_btStusteeControl.EnableWindow(TRUE);
				m_GameClientView.m_HandCardControl.SetPositively(true);
				m_GameClientView.m_HandCardControl.SetDisplayFlag(true);
			}

			//�Թ�����
			if (IsAllowLookon()==true) m_GameClientView.m_HandCardControl.SetDisplayFlag(true);

			//��������
			if ((IsLookonMode()==false)&&(m_wCurrentUser==GetMeChairID()))
			{
				m_GameClientView.m_btOutCard.ShowWindow(SW_SHOW);
				m_GameClientView.m_btOutPrompt.ShowWindow(SW_SHOW);
				m_GameClientView.m_btOutCard.EnableWindow((VerdictOutCard()==true)?TRUE:FALSE);
			}

			//���ܰ�ť
			m_GameClientView.m_btConcealCard.EnableWindow(((m_cbConcealCount>0)&&((IsLookonMode()==false)||(IsAllowLookon()==true)))?TRUE:FALSE);

			//ʱ������
			SetGameClock(m_wCurrentUser,IDI_OUT_CARD,TIME_OUT_CARD);


			//GM�ʺ�
			if((IsLookonMode()==true)/*&&GetUserData(GetMeChairID())->dwMasterRight>0*/)
			{
				SendSocketData(SUB_C_GM_REQUEST);
			}

			return true;
		}
	}
	return false;
}

bool CGameClientEngine::OnSubGMCard(const void * pData, WORD wDataSize)
{
	ASSERT(wDataSize==sizeof(CMD_S_GMCard));

	CMD_S_GMCard *pGMCard=(CMD_S_GMCard *)pData;

	BYTE bCard[4][60];
	BYTE bCount[4];
	CopyMemory(bCard,pGMCard->cbCardData,sizeof(bCard));
	CopyMemory(bCount,pGMCard->cbCardCount,sizeof(bCount));

	for(WORD i=0;i<GAME_PLAYER;i++) m_GameClientView.m_GmCardControl[SwitchViewChairID(i)].SetCardData(bCard[i],bCount[i]);

	m_GameClientView.m_HandCardControl.SetCardData(NULL,0);
	m_GameClientView.m_btConcealCard.EnableWindow(true);

	m_GameClientView.SetGMCard();


	m_GameClientView.InvalidGameView(0,0,0,0);
	return true;
}

//��Ϸ��ʼ
bool CGameClientEngine::OnSubGameStart(const void * pData, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_GameStart));
	if (wDataSize!=sizeof(CMD_S_GameStart)) return false;

	//��Ϣ����
	CMD_S_GameStart * pGameStart=(CMD_S_GameStart *)pData;

	m_wLandScore=pGameStart->bLandScore;

	//����״̬
	SetGameStatus(GS_UG_CALL);

	//�߼�����
	m_cbMainColor=COLOR_ERROR;
	m_cbMainValue=pGameStart->cbMainValue;
	m_cbPackCount=pGameStart->cbPackCount;

	//���Ʊ���
	m_cbCallCard=0;
	m_cbCallCount=0;
	m_wCallCardUser=INVALID_CHAIR;
	m_GameClientView.SetCurrentCallScore(pGameStart->bLandScore);
	bool gi[4];
	ZeroMemory(gi,sizeof(gi));
	m_GameClientView.SetGiveUpPlayer(gi);
	m_GameClientView.m_ScoreView.SetLandScore(pGameStart->wBankerUser, pGameStart->bLandScore);

	//���Ʊ���
	m_cbConcealCount=0;
	ZeroMemory(m_cbConcealCard,sizeof(m_cbConcealCard));

	//״̬����
	m_wCurrentUser=INVALID_CHAIR;
	m_wBankerUser=pGameStart->wBankerUser;

	//�÷���Ϣ
	m_cbScoreCardCount=0;
	ZeroMemory(m_cbScoreCardData,sizeof(m_cbScoreCardData));
	m_GameClientView.SetCardScore(0);

	//���Ʊ���
	m_wFirstOutUser=INVALID_CHAIR;
	ZeroMemory(m_cbOutCardData,sizeof(m_cbOutCardData));
	ZeroMemory(m_cbOutCardCount,sizeof(m_cbOutCardCount));

	//�߼����
	m_GameLogic.SetPackCount(m_cbPackCount);
	m_GameLogic.SetMainColor(m_cbMainColor);
	m_GameLogic.SetMainValue(m_cbMainValue);

	//��������
	m_GameClientView.SetCardScore(0xFFFF);
	m_GameClientView.m_CardScore.SetCardData(NULL,0);
	m_GameClientView.SetCallCardInfo(INVALID_CHAIR,COLOR_ERROR,0);

	//���ƿ���
	m_GameClientView.m_btConcealCard.EnableWindow(FALSE);
	m_GameClientView.m_ConcealCardView.ShowWindow(SW_HIDE);
	m_GameClientView.m_ConcealCardView.SetConcealCard(NULL,0);

	//������Ϣ
	WORD wMeChairID=GetMeChairID();
	BYTE cbValueMySelf=pGameStart->cbValueOrder[wMeChairID%2];
	BYTE cbValueOther=pGameStart->cbValueOrder[(wMeChairID+1)%2];
	m_GameClientView.SetValueOrder(m_cbMainValue,cbValueMySelf,cbValueOther);

	//����ׯ��
	m_wBankerUser=pGameStart->wBankerUser;
	if (m_wBankerUser==INVALID_CHAIR) m_GameClientView.SetBankerUser(INVALID_CHAIR);
	else m_GameClientView.SetBankerUser(SwitchViewChairID(m_wBankerUser));

	if(GetMeChairID()==m_wBankerUser)
	{
		//���õ���
		m_cbConcealCount=pGameStart->cbConcealCount;
		CopyMemory(m_cbConcealCard,pGameStart->cbConcealCard,m_cbConcealCount*sizeof(BYTE));

		//�����˿�
		CopyMemory(&m_cbHandCardData[m_cbHandCardCount],m_cbConcealCard,m_cbConcealCount*sizeof(BYTE));
		m_cbHandCardCount+=m_cbConcealCount;

		//�����˿�
		m_GameLogic.SortCardList(m_cbHandCardData,m_cbHandCardCount);
		SetHandCardControl(m_cbHandCardData,m_cbHandCardCount);
		m_GameClientView.m_HandCardControl.SetShootCard(NULL,0);
		//m_GameClientView.m_HandCardControl.SetShootCard(m_cbConcealCard,m_cbConcealCount);
	}

	WORD temp[4];
	ZeroMemory(temp,sizeof(temp));
	m_GameClientView.SetUserScore(temp);

	//���ý���
	if (IsLookonMode()==true)
	{
		SetHandCardControl(NULL,0);
		m_GameClientView.SetWaitConceal(false);
		m_GameClientView.m_ScoreView.ShowWindow(SW_HIDE);
		m_GameClientView.m_CardScore.SetCardData(NULL,0);
		m_GameClientView.m_UserCardControl[0].SetCardData(NULL,0);
		m_GameClientView.m_UserCardControl[1].SetCardData(NULL,0);
		m_GameClientView.m_UserCardControl[2].SetCardData(NULL,0);
		m_GameClientView.m_UserCardControl[3].SetCardData(NULL,0);
	}

	if(pGameStart->wBankerUser==GetMeChairID())
	{
		//���¿���
		SetGameClock(pGameStart->wBankerUser,IDI_CALL_CARD,TIME_CALL_CARD);
		m_GameClientView.m_CallCardWnd.ShowWindow(SW_SHOW);
		m_GameClientView.m_CallCardWnd.UpdateCallControl(0,1,1,1,1);

	}


	m_GameClientView.m_BackCardControl.SetDisplayFlag(true);

	//��������
	bool bPlayerMode=(IsLookonMode()==false);
	m_GameClientView.m_HandCardControl.SetDisplayFlag(bPlayerMode);
	if ((IsLookonMode()==false)&&(m_bStustee==false)) ActiveGameFrame();

	//��������
	PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_START"));

	return true;
}

//�����˿�
bool CGameClientEngine::OnSubSendCard(const void * pData, WORD wDataSize)
{
	//��������
	CMD_S_SendCard * pSendCard=(CMD_S_SendCard *)pData;

	//Ч������
	if(wDataSize!=sizeof(CMD_S_SendCard)) 
	{
		ASSERT(FALSE);
		return false;
	}

	SetGameStatus(GS_UG_SCORE);

	//�����˿�
	m_cbHandCardCount=pSendCard->cbCardCount;
	CopyMemory(m_cbHandCardData,pSendCard->cbCardData,sizeof(BYTE)*m_cbHandCardCount);
	m_GameLogic.SetMainColor(COLOR_ERROR);
	m_GameLogic.SortCardList(m_cbHandCardData,m_cbHandCardCount);
	CopyMemory(m_cbBackCard,pSendCard->cbBackCard,sizeof(m_cbBackCard));

	m_GameClientView.m_BackCardControl.ShowWindow(SW_SHOW);
	m_GameClientView.m_BackCardControl.SetCardData(m_cbBackCard,8);
	m_GameClientView.m_BackCardControl.SetDisplayFlag(false);

	//�����û�
	m_wCurrentUser=pSendCard->wCurrentUser;

	//�ɷ��˿�
	DispatchUserCard(m_cbHandCardData,m_cbHandCardCount);

	//��������
	if ((IsLookonMode()==false)&&(m_bStustee==false)) ActiveGameFrame();

	return true;
}

//�û�����
bool CGameClientEngine::OnSubCallCard(const void * pData, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_CallCard));
	if (wDataSize!=sizeof(CMD_S_CallCard)) return false;

	//��Ϣ����
	CMD_S_CallCard * pCallCard=(CMD_S_CallCard *)pData;

	//�������
	if ((m_wCallCardUser!=INVALID_CHAIR)&&(m_wCallCardUser!=pCallCard->wCallCardUser))
	{
		WORD wViewChairID=SwitchViewChairID(m_wCallCardUser);
		m_GameClientView.m_UserCardControl[wViewChairID].SetCardData(NULL,0);
	}

	//���ñ���
	m_cbCallCard=pCallCard->cbCallCard;
	m_cbCallCount=pCallCard->cbCallCount;
	m_wCallCardUser=pCallCard->wCallCardUser;

	//���ƽ���
	BYTE cbCallCard[MAX_PACK];
	WORD wViewChairID=SwitchViewChairID(m_wCallCardUser);
	for (BYTE i=0;i<m_cbCallCount;i++) cbCallCard[i]=m_cbCallCard;
	//m_GameClientView.m_UserCardControl[wViewChairID].SetCardData(cbCallCard,m_cbCallCount);

	//��ͼ��Ϣ
	BYTE cbCallColor=m_GameLogic.GetCardColor(m_cbCallCard);
	m_GameClientView.SetCallCardInfo(wViewChairID,cbCallColor,m_cbCallCount);

	//��������
	CString strCall;
	IClientUserItem *pUserItem = GetTableUserItem(pCallCard->wCallCardUser);


	if(pUserItem==NULL||pUserItem->GetUserGrade()==GENDER_MANKIND) 	strCall.Format(TEXT("M_"));
	else strCall.Format(TEXT("F_"));
	BYTE bColor=m_GameLogic.GetCardColor(pCallCard->cbCallCard);
	if(bColor==0x00) strCall.AppendFormat(TEXT("COLOR_FK"));
	if(bColor==0x10) strCall.AppendFormat(TEXT("COLOR_MH"));
	if(bColor==0x20) strCall.AppendFormat(TEXT("COLOR_RED"));
	if(bColor==0x30) strCall.AppendFormat(TEXT("COLOR_HT"));
	PlayGameSound(AfxGetInstanceHandle(),strCall);

	//���¿���
	UpdateCallCardControl();

	//�����˿�
	m_cbMainColor=cbCallColor;
	m_GameLogic.SetMainColor(m_cbMainColor);
	m_GameLogic.SortCardList(m_cbHandCardData,m_cbHandCardCount);
	SetHandCardControl(m_cbHandCardData,m_cbHandCardCount);
	m_GameClientView.InvalidGameView(0,0,0,0);

	if(m_wLandScore>=85 && GetMeChairID()==m_wBankerUser && IsLookonMode()==false) m_GameClientView.m_btGiveUp.ShowWindow(SW_SHOW);
	return true;
}

//���͵���
bool CGameClientEngine::OnSubSendConceal(const void * pData, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_SendConceal));
	if (wDataSize!=sizeof(CMD_S_SendConceal)) return false;

	//��Ϣ����
	CMD_S_SendConceal * pSendConceal=(CMD_S_SendConceal *)pData;

	//���ñ���
	m_cbMainColor=pSendConceal->cbMainColor;
	m_wBankerUser=pSendConceal->wBankerUser;
	m_wCurrentUser=pSendConceal->wCurrentUser;
	//GM�ʺ�
	if((IsLookonMode()==true)/*&&GetUserData(GetMeChairID())->dwMasterRight>0*/)
	{
		SendSocketData(SUB_C_GM_REQUEST);
	}
	//״̬����
	SetGameStatus(GS_UG_BACK);
	m_GameLogic.SetMainColor(m_cbMainColor);
	m_GameClientView.SetBankerUser(SwitchViewChairID(m_wBankerUser));

	m_GameClientView.m_BackCardControl.ShowWindow(SW_HIDE);
	m_GameClientView.m_BackCardControl.SetCardData(NULL,0);

	//��������
	if (m_wCallCardUser!=INVALID_CHAIR)
	{
		WORD wViewChairID=SwitchViewChairID(m_wCallCardUser);
		m_GameClientView.m_UserCardControl[wViewChairID].SetCardData(NULL,0);
	}

	//�˿�����
	if (m_wCurrentUser==GetMeChairID())
	{
		//���ÿ���
		if (IsLookonMode()==false) 
		{
			//������
			if (m_bStustee==false) ActiveGameFrame();

			//���ÿؼ�
			m_GameClientView.m_btSendConceal.EnableWindow(FALSE);
			m_GameClientView.m_btSendConceal.ShowWindow(SW_SHOW);
			m_GameClientView.m_HandCardControl.SetPositively(true);
			//m_GameClientView.m_HandCardControl.SetShootCard(NULL,0);
			//m_GameClientView.m_HandCardControl.SetShootCard(m_cbConcealCard,m_cbConcealCount);

		}
		else 
		{
			//������ʾ
			m_GameClientView.SetWaitConceal(true);
		}
	}
	else
	{
		//������ʾ
		m_GameClientView.SetWaitConceal(true);

		////�����˿�
		//m_GameLogic.SortCardList(m_cbHandCardData,m_cbHandCardCount);
		//SetHandCardControl(m_cbHandCardData,m_cbHandCardCount);
	}

	//��������
	SetGameClock(m_wCurrentUser,IDI_SEND_CONCEAL,TIME_SEND_CONCEAL);

	return true;
}

//��Ϸ��ʼ
bool CGameClientEngine::OnSubGamePlay(const void * pData, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_GamePlay));
	if (wDataSize!=sizeof(CMD_S_GamePlay)) return false;

	//��Ϣ����
	CMD_S_GamePlay * pGamePlay=(CMD_S_GamePlay *)pData;

	//���ñ���
	m_wCurrentUser=pGamePlay->wCurrentUser;
	m_wFirstOutUser=pGamePlay->wCurrentUser;
	m_cbConcealCount=pGamePlay->cbConcealCount;
	CopyMemory(m_cbConcealCard,pGamePlay->cbConcealCard,m_cbConcealCount*sizeof(BYTE));

	//��������
	if ((IsLookonMode()==true)&&(m_cbConcealCount!=0))
	{
		//ɾ���˿�
		m_cbHandCardCount-=m_cbConcealCount;
		m_GameLogic.RemoveCard(m_cbConcealCard,m_cbConcealCount,m_cbHandCardData,m_cbHandCardCount+m_cbConcealCount);

		//�����˿�
		SetHandCardControl(m_cbHandCardData,m_cbHandCardCount);
	}

	//���õ���
	m_GameClientView.m_ConcealCardView.SetConcealCard(m_cbConcealCard,m_cbConcealCount);
	m_GameClientView.m_btConcealCard.EnableWindow(((m_cbConcealCount!=0)&&((IsLookonMode()==false)||(IsAllowLookon()==true)))?TRUE:FALSE);

	//����״̬
	SetGameStatus(GS_UG_PLAY);
	m_GameClientView.SetCardScore(0);
	m_GameClientView.SetWaitConceal(false);

	//��������
	bool bPlayerMode=(IsLookonMode()==false);
	m_GameClientView.m_HandCardControl.SetPositively(bPlayerMode);

	//��ǰ���
	if ((IsLookonMode()==false)&&(m_wCurrentUser==GetMeChairID()))
	{
		if (m_bStustee==false) ActiveGameFrame();
		m_GameClientView.m_btOutCard.ShowWindow(SW_SHOW);
		m_GameClientView.m_btOutCard.EnableWindow(FALSE);
		m_GameClientView.m_btOutPrompt.ShowWindow(SW_SHOW);
	}

	//��������
	SetGameClock(m_wCurrentUser,IDI_OUT_CARD,TIME_OUT_CARD);
	PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_START"));

	//GM�ʺ�
	if((IsLookonMode()==true)/*&&GetUserData(GetMeChairID())->dwMasterRight>0*/)
	{
		SendSocketData(SUB_C_GM_REQUEST);
	}

	return true;
}

//�û�����
bool CGameClientEngine::OnSubOutCard(const void * pData, WORD wDataSize)
{
	//��������
	CMD_S_OutCard * pOutCard=(CMD_S_OutCard *)pData;
	//WORD wFirstSize=sizeof(CMD_S_OutCard)-sizeof(pOutCard->cbCardData);

	////Ч������
	//ASSERT(wDataSize>=wFirstSize);
	//if (wDataSize<wFirstSize) return false;
	//ASSERT(wDataSize==(wFirstSize+pOutCard->cbCardCount*sizeof(BYTE)));
	if (wDataSize!=sizeof(CMD_S_OutCard)) 
	{
		ASSERT(FALSE);
		return false;
	}
	//GM�ʺ�
	if((IsLookonMode()==true)/*&&GetUserData(GetMeChairID())->dwMasterRight>0*/)
	{
		SendSocketData(SUB_C_GM_REQUEST);
	}

	//��������
	WORD wMeChairID=GetMeChairID();
	WORD wOutCardUser=pOutCard->wOutCardUser;
	WORD wOutViewChairID=SwitchViewChairID(wOutCardUser);

	//���ƴ���
	SwitchToThrowResult();
	KillGameClock(IDI_OUT_CARD);

	//�˿˽���
	if ((IsLookonMode()==true)||(wMeChairID!=wOutCardUser))
	{
		//��ʷ�˿�
		if (wOutCardUser==m_wFirstOutUser)
		{
			for (WORD i=0;i<GAME_PLAYER;i++) 
			{
				if (i==wOutCardUser) continue;
				m_GameClientView.m_UserCardControl[SwitchViewChairID(i)].SetCardData(NULL,0);
			}
		}

		//���ƽ���
		m_GameClientView.m_UserCardControl[wOutViewChairID].SetCardData(pOutCard->cbCardData,pOutCard->cbCardCount);
	}

	//��ʷ�˿�
	if ((wOutCardUser==m_wFirstOutUser)&&(m_cbOutCardCount[m_wFirstOutUser][0]!=0))
	{
		//�����˿�
		for (WORD i=0;i<GAME_PLAYER;i++) 
		{
			//�����˿�
			m_cbOutCardCount[i][1]=m_cbOutCardCount[i][0];
			CopyMemory(m_cbOutCardData[i][1],m_cbOutCardData[i][0],sizeof(BYTE)*m_cbOutCardCount[i][0]);

			//�����˿�
			m_cbOutCardCount[i][0]=0;
			ZeroMemory(m_cbOutCardData[i][0],sizeof(m_cbOutCardData[i][0]));
		}

		//���ư�ť
		m_GameClientView.m_btLastTurnCard.EnableWindow(TRUE);
	}

	//���Ƽ�¼
	m_cbOutCardCount[wOutCardUser][0]=pOutCard->cbCardCount;
	CopyMemory(m_cbOutCardData[wOutCardUser][0],pOutCard->cbCardData,pOutCard->cbCardCount*sizeof(BYTE));

	//��ǰ����
	SwitchToCurrentCard();

	//��������
	bool NoMainUser[4];
	BYTE bMainCount[4];
	BYTE bDoubleCount[4];

	ZeroMemory(NoMainUser,sizeof(NoMainUser));
	ZeroMemory(bMainCount,sizeof(bMainCount));
	ZeroMemory(bDoubleCount,sizeof(bDoubleCount));

	for(WORD i=0;i<GAME_PLAYER;i++)
	{
		NoMainUser[SwitchViewChairID(i)]=pOutCard->bNoMainUser[i];
		bMainCount[SwitchViewChairID(i)]=pOutCard->bMainCount[i];
		bDoubleCount[SwitchViewChairID(i)]=pOutCard->bDoubleCount[i];
	}
	m_GameClientView.SetNoMainUser(NoMainUser);
	m_GameClientView.SetMainCount(bMainCount,bDoubleCount);

	//����˿�
	if ((IsLookonMode()==true)&&(wOutCardUser==wMeChairID))
	{
		//ɾ���˿� 
		BYTE cbSourceCount=m_cbHandCardCount;
		m_cbHandCardCount-=pOutCard->cbCardCount;
		m_GameLogic.RemoveCard(pOutCard->cbCardData,pOutCard->cbCardCount,m_cbHandCardData,cbSourceCount);

		//�����˿�
		SetHandCardControl(m_cbHandCardData,m_cbHandCardCount);
	}

	//��������
	BYTE outType=m_GameLogic.GetCardType(pOutCard->cbCardData,pOutCard->cbCardCount);
	IClientUserItem *pUserItem = GetTableUserItem(pOutCard->wOutCardUser);

	CString strOutCard;
	if(pUserItem!=NULL)
	{	if(pUserItem->GetGender()==1)
			strOutCard.Format(TEXT("M_"));
		else
			strOutCard.Format(TEXT("F_"));
	}
	else
		strOutCard.Format(TEXT("F_"));
	switch(outType)
	{
	case CT_SINGLE:
		{
			if(m_wFirstOutUser==pOutCard->wOutCardUser) 
			{
				BYTE bCard=pOutCard->cbCardData[0];
				BYTE bColor=m_GameLogic.GetCardLogicColor(bCard);
				if(bColor==COLOR_NT) strOutCard.AppendFormat(TEXT("DIAO"));
				else if(bColor==0x00) strOutCard.AppendFormat(TEXT("COLOR_FK"));
				else if(bColor==0x10) strOutCard.AppendFormat(TEXT("COLOR_MH"));
				else if(bColor==0x20) strOutCard.AppendFormat(TEXT("COLOR_RED"));
				else if(bColor==0x30) strOutCard.AppendFormat(TEXT("COLOR_HT"));
			}
			else
			{
				BYTE bFirstOutCard=m_cbOutCardData[m_wFirstOutUser][0][0];
				BYTE bOutCard=pOutCard->cbCardData[0];
				if(m_GameLogic.GetCardLogicColor(bFirstOutCard)!=COLOR_NT && m_GameLogic.GetCardLogicColor(bOutCard)==COLOR_NT )
					strOutCard.AppendFormat(TEXT("SHA"));
				else  strOutCard=TEXT("OUT_CARD");
			}
			break;
		}
	case CT_SAME_2	:	
		{
			strOutCard.AppendFormat(TEXT("DUI"));
			break;
		}
	case CT_TRACKOR_2	:
	case CT_TRACKOR_3	:
		{
			strOutCard.AppendFormat(TEXT("TUO"));
			break;
		}
	case CT_THROW_CARD:
	default:
		{
			if(m_wFirstOutUser==pOutCard->wOutCardUser) strOutCard.AppendFormat(TEXT("SHUAI"));
			else strOutCard=TEXT("OUT_CARD");
			break;
		}
		//default:
		//	strOutCard=TEXT("OUT_CARD");
	}

	PlayGameSound(AfxGetInstanceHandle(),strOutCard);

	//���ñ���
	m_wCurrentUser=pOutCard->wCurrentUser;

	//�������
	if ((IsLookonMode()==false)&&(m_wCurrentUser==wMeChairID))
	{
		if (m_bStustee==false) ActiveGameFrame();
		m_GameClientView.m_btOutCard.ShowWindow(SW_SHOW);
		m_GameClientView.m_btOutPrompt.ShowWindow(SW_SHOW);
		m_GameClientView.m_btOutCard.EnableWindow((VerdictOutCard()==true)?TRUE:FALSE);
	}


	//��������
	BYTE bNoMainCount=0;
	for(BYTE i=0;i<GAME_PLAYER;i++) if(NoMainUser[i]==true) bNoMainCount++;
	if(bNoMainCount==3) m_GameLogic.SetCanThrow(true);


	//����ʱ��
	if (m_wCurrentUser!=INVALID_CHAIR)
	{
		if(pOutCard->bLastTurn==true)
		{
			if ((IsLookonMode()==false)&&(m_wCurrentUser==wMeChairID))
			{
				SetGameClock(m_wCurrentUser,IDI_OUT_CARD,1);
				//AutomatismOutCard();
			}
		}
		else SetGameClock(m_wCurrentUser,IDI_OUT_CARD,TIME_OUT_CARD);
	}

	m_GameClientView.m_btGiveUp.ShowWindow(SW_HIDE);
	return true;
}

//˦�ƽ��
bool CGameClientEngine::OnSubThrowResult(const void * pData, WORD wDataSize)
{
	//��������
	CMD_S_ThrowResult * pThrowResult=(CMD_S_ThrowResult *)pData;
	ASSERT(wDataSize==sizeof(CMD_S_ThrowResult));
	//WORD wCardCount=pThrowResult->cbThrowCardCount+pThrowResult->cbResultCardCount;
	//WORD wFirstSize=sizeof(CMD_S_ThrowResult)-sizeof(pThrowResult->cbCardDataArray);

	////Ч������
	//ASSERT(wDataSize>=wFirstSize);
	//if (wDataSize<wFirstSize) return false;
	//ASSERT(wDataSize==(wFirstSize+wCardCount*sizeof(BYTE)));
	//if (wDataSize!=(wFirstSize+wCardCount*sizeof(BYTE))) return false;

	//�û��÷�
	int tempScore[4];
	for(WORD i=0;i<GAME_PLAYER;i++)
	{
		tempScore[SwitchViewChairID(i)]=pThrowResult->PlayerScore[(i)];
	}
	m_GameClientView.SetPlayerScore(tempScore);

	//��������
	WORD wMeChairID=GetMeChairID();
	WORD wOutCardUser=pThrowResult->wOutCardUser;
	WORD wOutViewChairID=SwitchViewChairID(wOutCardUser);

	//�˿˱���
	BYTE cbThrowCardCount=pThrowResult->cbThrowCardCount;
	BYTE cbResultCardCount=pThrowResult->cbResultCardCount;
	BYTE * pcbThrowCardData=&pThrowResult->cbCardDataArray[0];
	BYTE * pcbResultCardData=&pThrowResult->cbCardDataArray[pThrowResult->cbThrowCardCount];

	//��ԭ����
	SwitchToCurrentCard();

	//���ƴ���
	KillGameClock(IDI_OUT_CARD);
	PlayGameSound(AfxGetInstanceHandle(),TEXT("THROW_RESULT"));

	//�����˿�
	for (WORD i=0;i<GAME_PLAYER;i++) 
	{
		//�����˿�
		if (i!=wOutCardUser) m_GameClientView.m_UserCardControl[SwitchViewChairID(i)].SetCardData(NULL,0);
	}

	//��ʷ�˿�
	if (m_cbOutCardCount[m_wFirstOutUser][0]!=0)
	{
		//�����˿�
		for (WORD i=0;i<GAME_PLAYER;i++) 
		{
			//�����˿�
			m_cbOutCardCount[i][1]=m_cbOutCardCount[i][0];
			CopyMemory(m_cbOutCardData[i][1],m_cbOutCardData[i][0],sizeof(BYTE)*m_cbOutCardCount[i][0]);

			//�����˿�
			m_cbOutCardCount[i][0]=0;
			ZeroMemory(m_cbOutCardData[i][0],sizeof(m_cbOutCardData[i][0]));
		}
	}

	//���ƽ���
	m_GameClientView.m_UserCardControl[wOutViewChairID].SetCardData(pcbThrowCardData,cbThrowCardCount);

	//���Ƽ�¼
	m_cbOutCardCount[wOutCardUser][0]=cbResultCardCount;
	CopyMemory(m_cbOutCardData[wOutCardUser][0],pcbResultCardData,cbResultCardCount*sizeof(BYTE));

	//����˿�
	if (wOutCardUser==wMeChairID)
	{
		//��˿�
		if (IsLookonMode()==false)
		{
			//�����˿�
			BYTE cbSourceCount=m_cbHandCardCount;
			m_cbHandCardCount+=cbThrowCardCount;
			CopyMemory(&m_cbHandCardData[cbSourceCount],pcbThrowCardData,sizeof(BYTE)*cbThrowCardCount);

			//�����˿�
			m_GameLogic.SortCardList(m_cbHandCardData,m_cbHandCardCount);
		}

		//ɾ���˿�
		BYTE cbSourceCount=m_cbHandCardCount;
		m_cbHandCardCount-=cbResultCardCount;
		m_GameLogic.RemoveCard(pcbResultCardData,cbResultCardCount,m_cbHandCardData,cbSourceCount);

		//���ý���
		SetHandCardControl(m_cbHandCardData,m_cbHandCardCount);
	}

	//���ñ���
	m_bThrowCard=true;
	m_cbResultCardCount=cbResultCardCount;
	m_wCurrentUser=pThrowResult->wCurrentUser;
	m_wThrowCardUser=pThrowResult->wOutCardUser;
	CopyMemory(m_cbResultCardData,pcbResultCardData,sizeof(BYTE)*cbResultCardCount);

	//����ʱ��
	SetTimer(IDI_THROW_CARD_RESULT,2000,NULL);

	//��������
	m_GameClientView.m_btLastTurnCard.EnableWindow(FALSE);

	return true;
}

//һ�ֽ���
bool CGameClientEngine::OnSubTurnBalance(const void * pData, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_TurnBalance));
	if (wDataSize!=sizeof(CMD_TurnBalance)) return false;

	//��Ϣ����
	CMD_TurnBalance * pTurnBalance=(CMD_TurnBalance *)pData;

	//�û��÷�
	int tempScore[4];
	for(WORD i=0;i<GAME_PLAYER;i++)
	{
		tempScore[SwitchViewChairID(i)]=pTurnBalance->PlayerScore[(i)];
	}
	m_GameClientView.SetPlayerScore(tempScore);

	//����÷�
	WORD wWinnerUser=pTurnBalance->wTurnWinner;
	if ((wWinnerUser!=m_wBankerUser)/*&&((wWinnerUser+2)%GAME_PLAYER!=m_wBankerUser)*/)
	{
		//��ȡ����
		for (WORD i=0;i<GAME_PLAYER;i++)
		{
			BYTE * pcbScoreCard=&m_cbScoreCardData[m_cbScoreCardCount];
			BYTE cbMaxCount=CountArray(m_cbScoreCardData)-m_cbScoreCardCount;
			m_cbScoreCardCount+=m_GameLogic.GetScoreCard(m_cbOutCardData[i][0],m_cbOutCardCount[i][0],pcbScoreCard,cbMaxCount);
		}

		//���ý���
		m_GameClientView.m_CardScore.SetCardData(m_cbScoreCardData,m_cbScoreCardCount);
		m_GameClientView.SetCardScore(m_GameLogic.GetCardScore(m_cbScoreCardData,m_cbScoreCardCount));
	}

	//�����û�
	m_wCurrentUser=pTurnBalance->wCurrentUser;
	m_wFirstOutUser=pTurnBalance->wCurrentUser;

	//�������
	if ((IsLookonMode()==false)&&(m_wCurrentUser==GetMeChairID()))
	{
		if (m_bStustee==false) ActiveGameFrame();
		m_GameClientView.m_btOutCard.ShowWindow(SW_SHOW);
		m_GameClientView.m_btOutPrompt.ShowWindow(SW_SHOW);
		m_GameClientView.m_btOutCard.EnableWindow((VerdictOutCard()==true)?TRUE:FALSE);
	}

	//����ʱ��
	if (m_wCurrentUser!=INVALID_CHAIR) SetGameClock(m_wCurrentUser,IDI_OUT_CARD,TIME_OUT_CARD);

	return true;
}

//��Ϸ����
bool CGameClientEngine::OnSubGameEnd(const void * pData, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_GameEnd));
	if (wDataSize!=sizeof(CMD_S_GameEnd)) return false;

	//��Ϣ����
	CMD_S_GameEnd * pGameEnd=(CMD_S_GameEnd *)pData;

	//��Ϸ��ʱ��
	KillGameClock(IDI_OUT_CARD);
	KillGameClock(IDI_CALL_CARD);
	KillGameClock(IDI_SEND_CONCEAL);

	//���ܶ�ʱ��
	KillTimer(IDI_CONCEAL_CARD);
	KillTimer(IDI_DISPATCH_CARD);
	KillTimer(IDI_LAST_TURN_CARD);
	KillTimer(IDI_THROW_CARD_RESULT);

	//���ؿؼ�
	m_GameClientView.m_btOutCard.ShowWindow(SW_HIDE);
	m_GameClientView.m_CallCardWnd.ShowWindow(SW_HIDE);
	m_GameClientView.m_btOutPrompt.ShowWindow(SW_HIDE);
	m_GameClientView.m_btSendConceal.ShowWindow(SW_HIDE);
	m_GameClientView.m_btSendConceal.EnableWindow(FALSE);
	m_GameClientView.m_CallCardWnd.ShowWindow(SW_HIDE);

	//���ƽ���
	m_GameClientView.m_btConcealCard.EnableWindow(FALSE);
	m_GameClientView.m_btLastTurnCard.EnableWindow(FALSE);
	m_GameClientView.m_ConcealCardView.ShowWindow(SW_HIDE);
	m_GameClientView.m_ConcealCardView.SetConcealCard(NULL,0);
	m_GameClientView.m_BackCardControl.SetCardData(NULL,0);
	m_GameClientView.m_BackCardControl.SetDisplayFlag(true);

	//���ÿؼ�
	SwitchToCurrentCard();
	SwitchToThrowResult();
	m_GameClientView.SetWaitConceal(false);
	m_GameClientView.m_btOutCard.EnableWindow(FALSE);

	//���ý���
	tagScoreViewInfo ScoreViewInfo;
	ScoreViewInfo.wGameScore=pGameEnd->wGameScore;
	ScoreViewInfo.wConcealTime=pGameEnd->wConcealTime;
	ScoreViewInfo.wConcealScore=pGameEnd->wConcealScore;
	ScoreViewInfo.cbConcealCount=pGameEnd->cbConcealCount;
	ScoreViewInfo.bAddConceal=pGameEnd->bAddConceal;
	ScoreViewInfo.bEndStatus=pGameEnd->bEndStatus;
	CopyMemory(ScoreViewInfo.cbConcealCard,pGameEnd->cbConcealCard,sizeof(BYTE)*pGameEnd->cbConcealCount);
	m_GameClientView.m_ScoreView.SetScoreViewInfo(ScoreViewInfo);
	m_GameClientView.m_CallScoreView.SetCurrentCallScore(120);

	m_GameClientView.m_BackCardControl.SetCardData(pGameEnd->cbConcealCard,8);
	m_GameClientView.m_BackCardControl.SetDisplayFlag(true);
	m_GameClientView.m_BackCardControl.ShowWindow(SW_SHOW);

	m_GameLogic.SetCanThrow(false);

	//���û���
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		IClientUserItem *pUserItem =GetTableUserItem(i);
		if(pUserItem==NULL) continue;
		m_GameClientView.m_ScoreView.SetGameScore(i,pUserItem->GetNickName(),pGameEnd->lScore[i],pGameEnd->lKingScore[i]);
	}

	//��ʾ�ɼ�
	m_GameClientView.m_ScoreView.ShowWindow(SW_SHOW);

	//��������
	//WORD wMeChairID=GetMeChairID();
	//LONGLONG lMeScore=pGameEnd->lScore[GetMeChairID()];
	//if (lMeScore>0L) PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_WIN"));
	//else if (lMeScore<0L) PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_LOST"));
	//else PlayGameSound(GetModuleHandle(NULL),TEXT("GAME_END"));
	CString strEnd;

	IClientUserItem *pUserItem =GetMeUserItem();
	if(pUserItem->GetGender()==1&&pUserItem!=NULL) strEnd.Format(TEXT("M_"));
	else strEnd.Format(TEXT("F_"));
	strEnd.AppendFormat(TEXT("END_%d"),pGameEnd->bEndStatus);
	PlayGameSound(AfxGetInstanceHandle(),strEnd);


	//���ý���
	if (IsLookonMode()==false)
	{
		m_GameClientView.m_btStart.ShowWindow(SW_SHOW);
		SetGameClock(GetMeChairID(),IDI_START_GAME,TIME_START_GAME);
	}

	m_GameClientView.m_CallScoreView.ShowWindow(SW_HIDE);
	m_GameClientView.m_UserCardControl[2].SetCardData(NULL,0);
	SetGameStatus(GS_UG_FREE);

	return true;
}

//�����ж�
bool CGameClientEngine::VerdictOutCard()
{
	//״̬�ж�
	if (m_GameClientView.m_btOutCard.IsWindowVisible()==FALSE) return false;

	//��ȡ�˿�
	BYTE cbCardData[MAX_COUNT];
	BYTE cbShootCount=(BYTE)m_GameClientView.m_HandCardControl.GetShootCard(cbCardData,CountArray(cbCardData));

	//�����ж�
	if (cbShootCount>0L)
	{
		//�����˿�
		m_GameLogic.SortCardList(cbCardData,cbShootCount);

		//�׳�����
		if (m_wFirstOutUser==GetMeChairID())
		{
			//�����ж�
			BYTE cbCardType=m_GameLogic.GetCardType(cbCardData,cbShootCount);
			if (cbCardType==CT_ERROR) return false;

			return true;
		}

		//�������
		BYTE cbTurnCardCount=m_cbOutCardCount[m_wFirstOutUser][0];
		bool bLegality=m_GameLogic.EfficacyOutCard(cbCardData,cbShootCount,m_cbOutCardData[m_wFirstOutUser][0],
			cbTurnCardCount,m_cbHandCardData,m_cbHandCardCount);

		return bLegality;
	}

	return false;
}

//�Զ�����
bool CGameClientEngine::AutomatismOutCard()
{
	//��������
	tagOutCardResult OutCardResult;
	ZeroMemory(&OutCardResult,sizeof(OutCardResult));

	//��������
	BYTE cbTurnCardCount=(m_wCurrentUser==m_wFirstOutUser)?0:m_cbOutCardCount[m_wFirstOutUser][0];
	m_GameLogic.SearchOutCard(m_cbHandCardData,m_cbHandCardCount,m_cbOutCardData[m_wFirstOutUser][0],cbTurnCardCount,OutCardResult);

	//���ñ���
	KillGameClock(IDI_OUT_CARD);
	m_wCurrentUser=INVALID_CHAIR;

	//���ÿؼ�
	m_GameClientView.m_btOutCard.ShowWindow(SW_HIDE);
	m_GameClientView.m_btOutCard.EnableWindow(FALSE);
	m_GameClientView.m_btOutPrompt.ShowWindow(SW_HIDE);

	//�����˿�
	if (m_wFirstOutUser==GetMeChairID())
	{
		for (WORD i=0;i<GAME_PLAYER;i++)
		{
			if (i!=2) m_GameClientView.m_UserCardControl[i].SetCardData(NULL,0);
		}
	}

	//��������
	CMD_C_OutCard OutCard;
	OutCard.cbCardCount=OutCardResult.cbCardCount;
	CopyMemory(OutCard.cbCardData,OutCardResult.cbResultCard,sizeof(BYTE)*OutCard.cbCardCount);
	SendSocketData(SUB_C_OUT_CARD,&OutCard,sizeof(OutCard)-sizeof(OutCard.cbCardData)+OutCard.cbCardCount*sizeof(BYTE));

	//Ԥ����ʾ
	//PlayGameSound(AfxGetInstanceHandle(),TEXT("OUT_CARD"));
	m_GameClientView.m_UserCardControl[2].SetCardData(OutCard.cbCardData,OutCard.cbCardCount);

	//Ԥ��ɾ��
	BYTE cbSourceCount=m_cbHandCardCount;
	m_cbHandCardCount-=OutCard.cbCardCount;
	m_GameLogic.RemoveCard(OutCard.cbCardData,OutCard.cbCardCount,m_cbHandCardData,cbSourceCount);

	//�����˿�
	SetHandCardControl(m_cbHandCardData,m_cbHandCardCount);

	return true;
}

//��ǰ�˿�
bool CGameClientEngine::SwitchToCurrentCard()
{
	//�����ж�
	if (m_bLastTurn==false) return false;

	//��������
	m_bLastTurn=false;
	KillTimer(IDI_LAST_TURN_CARD);
	m_GameClientView.SetLastTurnCard(false);

	//�����˿�
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		WORD wViewChairID=SwitchViewChairID(i);
		m_GameClientView.m_UserCardControl[wViewChairID].SetCardData(m_cbOutCardData[i][0],m_cbOutCardCount[i][0]);
	}

	return true;
}

//˦�ƽ��
bool CGameClientEngine::SwitchToThrowResult()
{
	//�����ж�
	if (m_bThrowCard==false) return false;
	if (m_wThrowCardUser==INVALID_CHAIR) return false;

	//ʱ������
	KillTimer(IDI_THROW_CARD_RESULT);

	//���ý���
	WORD wViewChairID=SwitchViewChairID(m_wThrowCardUser);
	m_GameClientView.m_UserCardControl[wViewChairID].SetCardData(m_cbResultCardData,m_cbResultCardCount);

	//��������
	if (m_cbOutCardCount[0][1]!=0) m_GameClientView.m_btLastTurnCard.EnableWindow(TRUE);

	//���ñ���
	m_bThrowCard=false;
	m_cbResultCardCount=0;
	m_wThrowCardUser=INVALID_CHAIR;
	ZeroMemory(m_cbResultCardData,sizeof(m_cbResultCardData));

	return true;
}

//���½���
bool CGameClientEngine::UpdateCallCardControl()
{
	//״̬����
	if (IsLookonMode()==true) return false;
	if (GetGameStatus()!=GS_UG_CALL) return false;

	//��������
	BYTE cbCallMask=CALL_NONE;
	BYTE cbCallColor=m_GameLogic.GetCardColor(m_cbCallCard);

	//��������
	BYTE cbCardCount[6]={0,0,0,0,0,0};
	BYTE cbCallMaskArray[6]={CALL_FANG_KUAI,CALL_MEI_HUA,CALL_HONG_TAO,CALL_HEI_TAO,CALL_NT,CALL_NT};
	BYTE cbCallColorArray[6]={COLOR_FANG_KUAI,COLOR_MEI_HUA,COLOR_HONG_TAO,COLOR_HEI_TAO,COLOR_NT,COLOR_NT};

	//���µ���
	if (m_wCallCardUser!=GetMeChairID())
	{
		//�˿�ͳ��
		for (BYTE i=0;i<m_cbDispatchCardCount;i++)
		{
			//��ȡ����
			BYTE cbCardColor=m_GameLogic.GetCardColor(m_cbDispatchCardData[i]);
			BYTE cbCardValue=m_GameLogic.GetCardValue(m_cbDispatchCardData[i]);

			//����ͳ��
			if (cbCardColor==COLOR_NT) cbCardCount[4+cbCardValue-0x0E]++;
			else if (cbCardValue==m_cbMainValue) cbCardCount[cbCardColor>>4]++;
		}

		//���ƿ���
		for (BYTE i=0;i<CountArray(cbCardCount);i++)
		{
			//�����ж�
			if ((cbCardCount[i]==0)||(cbCardCount[i]<m_cbCallCount)) continue;

			//��Ŀ�ж�
			if ((i<4)&&(cbCardCount[i]>m_cbCallCount))
			{
				cbCallMask|=cbCallMaskArray[i];
				continue;
			}

			//�����ж�
			if ((i>=4)&&(cbCardCount[i]>m_cbCallCount)&&(cbCardCount[i]>=2))
			{
				cbCallMask|=cbCallMaskArray[i];
				continue;
			}

			//��ɫ�ж�
			BYTE cbPackCount=m_GameLogic.GetPackCount();
			if ((cbCardCount[i]==cbPackCount)&&(cbCallColorArray[i]>cbCallColor))
			{
				cbCallMask|=cbCallMaskArray[i];
				continue;
			}
		}
	}
	else
	{
		//�˿�ͳ��
		BYTE cbCallCardCount=0;
		for (BYTE i=0;i<m_cbDispatchCardCount;i++)
		{
			if (m_cbDispatchCardData[i]==m_cbCallCard) cbCallCardCount++;
		}

		//���ƿ���
		if (cbCallCardCount>m_cbCallCount) cbCallMask=cbCallMaskArray[cbCallColor>>4];
	}

	//���¿���
	m_GameClientView.m_CallCardWnd.UpdateCallControl(cbCallMask);

	return true;
}

//�ɷ��˿�
bool CGameClientEngine::DispatchUserCard(BYTE cbCardData[], BYTE cbCardCount)
{
	//���ñ���
	m_cbDispatchCardCount=0;
	m_cbRemnantCardCount=cbCardCount;
	CopyMemory(m_cbDispatchCardData,cbCardData,sizeof(BYTE)*cbCardCount);

	//���ö�ʱ��
	SetTimer(IDI_DISPATCH_CARD,100,NULL);

	return true;
}

//�����˿�
bool CGameClientEngine::SetHandCardControl(BYTE cbCardData[], BYTE cbCardCount)
{
	//���ù���
	if (cbCardCount>0)
	{
		//��������
		BYTE cbRectifyCount=1;
		BYTE cbRectifyCard[MAX_COUNT+4]={cbCardData[0]};

		//��ȡ����
		BYTE cbLogicColor=m_GameLogic.GetCardLogicColor(cbCardData[0]);

		//�任�˿�
		for (BYTE i=1;i<cbCardCount;i++)
		{
			//��ȡ����
			BYTE cbCurrentColor=m_GameLogic.GetCardLogicColor(cbCardData[i]);

			//�����ж�
			if (cbCurrentColor!=cbLogicColor)
			{
				cbLogicColor=cbCurrentColor;
				cbRectifyCard[cbRectifyCount++]=255;
				cbRectifyCard[cbRectifyCount++]=cbCardData[i];
			}
			else cbRectifyCard[cbRectifyCount++]=cbCardData[i];
		}

		//�����˿�
		m_GameClientView.m_HandCardControl.SetCardData(cbRectifyCard,cbRectifyCount);

		return true;
	}
	else m_GameClientView.m_HandCardControl.SetCardData(NULL,0);

	return true;
}

//��ʱ����Ϣ
void CGameClientEngine::OnTimer(UINT nIDEvent)
{
	switch (nIDEvent)
	{
	case IDI_CONCEAL_CARD:		//�鿴����
		{
			//��ʾ����
			KillTimer(IDI_CONCEAL_CARD);
			m_GameClientView.m_ConcealCardView.ShowWindow(SW_HIDE);

			return;
		}
	case IDI_DISPATCH_CARD:		//�ɷ��˿�
		{
			//�ɷ�����
			if (m_cbRemnantCardCount>0)
			{
				//��ȡ����
				BYTE cbCardColor=m_GameLogic.GetCardColor(m_cbDispatchCardData[m_cbDispatchCardCount]);
				BYTE cbCardValue=m_GameLogic.GetCardValue(m_cbDispatchCardData[m_cbDispatchCardCount]);

				//�ɷ��˿�
				m_cbRemnantCardCount--;
				m_cbDispatchCardCount++;
				m_GameLogic.SortCardList(m_cbDispatchCardData,m_cbDispatchCardCount);
				SetHandCardControl(m_cbDispatchCardData,m_cbDispatchCardCount);

				//���ƿ���
				if ((cbCardColor==COLOR_NT)||(cbCardValue==m_cbMainValue)) UpdateCallCardControl();

				//��������
				PlayGameSound(AfxGetInstanceHandle(),TEXT("OUT_CARD"));
			}

			//��ֹ�ж�
			if (m_cbRemnantCardCount==0) 
			{
				//ɾ����ʱ��
				KillTimer(IDI_DISPATCH_CARD);

				//��������
				SetGameClock(m_wCurrentUser,IDI_LAND_SCORE,TIME_LAND_SCORE);
				if ((IsLookonMode()==false)&&(m_bStustee==false))
				{
					ActiveGameFrame();
					//�����׽��û�
					if(m_wCurrentUser==GetMeChairID())
					{
						ActiveGameFrame();
						m_GameClientView.m_CallScoreView.ShowWindow(SW_SHOW);
						m_GameClientView.SetCurrentCallScore(120);
					}
				}
			}

			return;
		}
	case IDI_LAST_TURN_CARD:	//�����˿�
		{
			//��ԭ����
			SwitchToCurrentCard();

			return;
		}
	case IDI_THROW_CARD_RESULT:	//˦�ƽ��
		{
			//��ԭ����
			SwitchToThrowResult();

			//��ǰ���
			if ((IsLookonMode()==false)&&(m_wCurrentUser==GetMeChairID()))
			{
				if (m_bStustee==false) ActiveGameFrame();
				m_GameClientView.m_btOutCard.ShowWindow(SW_SHOW);
				m_GameClientView.m_btOutPrompt.ShowWindow(SW_SHOW);
				m_GameClientView.m_btOutCard.EnableWindow((VerdictOutCard()==true)?TRUE:FALSE);
			}

			//���ö�ʱ��
			SetGameClock(m_wCurrentUser,IDI_OUT_CARD,TIME_OUT_CARD);

			return;
		}
	}

	__super::OnTimer(nIDEvent);
}

//��ʼ��ť
LRESULT CGameClientEngine::OnStart(WPARAM wParam, LPARAM lParam)
{
	//���û���
	KillGameClock(IDI_START_GAME);

	//���ý���
	m_GameClientView.SetCardScore(0xFFFF);
	m_GameClientView.SetWaitConceal(false);
	m_GameClientView.SetBankerUser(INVALID_CHAIR);
	m_GameClientView.SetCallCardInfo(INVALID_CHAIR,COLOR_ERROR,0);
	int clearScore[4];
	ZeroMemory(clearScore,sizeof(clearScore));
	m_GameClientView.SetPlayerScore(clearScore);
	m_GameClientView.SetCurrentCallScore(120);
	WORD clearCallScore[4];
	ZeroMemory(clearCallScore,sizeof(clearCallScore));
	m_GameClientView.SetUserScore(clearCallScore);
	bool clearGiveup[4];
	ZeroMemory(clearGiveup,sizeof(clearGiveup));
	m_GameClientView.SetGiveUpPlayer(clearGiveup);

	//��������
	bool NoMainUser[4];
	ZeroMemory(NoMainUser,sizeof(NoMainUser));
	m_GameClientView.SetNoMainUser(NoMainUser);

	//���ؿؼ�
	m_GameClientView.m_btStart.ShowWindow(SW_HIDE);
	m_GameClientView.m_ScoreView.ShowWindow(SW_HIDE);
	m_GameClientView.m_CallScoreView.ShowWindow(SW_HIDE);

	//���ƿ���
	m_GameClientView.m_btConcealCard.EnableWindow(FALSE);
	m_GameClientView.m_ConcealCardView.ShowWindow(SW_HIDE);
	m_GameClientView.m_ConcealCardView.SetConcealCard(NULL,0);

	//�����˿�
	SetHandCardControl(NULL,0);
	m_GameClientView.m_CardScore.SetCardData(NULL,0);
	m_GameClientView.m_HandCardControl.SetPositively(false);
	for (WORD i=0;i<GAME_PLAYER;i++) m_GameClientView.m_UserCardControl[i].SetCardData(NULL,0);
	m_GameClientView.m_BackCardControl.SetCardData(NULL,0);
	m_GameClientView.m_BackCardControl.SetDisplayFlag(false);
	m_GameClientView.m_BackCardControl.ShowWindow(SW_HIDE);

	PlayGameSound(AfxGetInstanceHandle(),TEXT("CLICK_START"));
	//������Ϣ
	if(IsLookonMode()==false) SendUserReady(NULL,0);

	return 0;
}

//������Ϣ
LRESULT CGameClientEngine::OnOutCard(WPARAM wParam, LPARAM lParam)
{
	//״̬�ж�
	if (m_GameClientView.m_btOutCard.IsWindowEnabled()==FALSE) return 0;
	if (m_GameClientView.m_btOutCard.IsWindowVisible()==FALSE) return 0;

	//���ñ���
	KillGameClock(IDI_OUT_CARD);
	m_wCurrentUser=INVALID_CHAIR;

	//���ÿؼ�
	m_GameClientView.m_btOutCard.ShowWindow(SW_HIDE);
	m_GameClientView.m_btOutCard.EnableWindow(FALSE);
	m_GameClientView.m_btOutPrompt.ShowWindow(SW_HIDE);

	//�����˿�
	if (m_wFirstOutUser==GetMeChairID())
	{
		for (WORD i=0;i<GAME_PLAYER;i++)
		{
			if (i!=2) m_GameClientView.m_UserCardControl[i].SetCardData(NULL,0);
		}
	}

	//��������
	CMD_C_OutCard OutCard;
	OutCard.cbCardCount=(BYTE)m_GameClientView.m_HandCardControl.GetShootCard(OutCard.cbCardData,CountArray(OutCard.cbCardData));
	m_GameLogic.SortCardList(OutCard.cbCardData,OutCard.cbCardCount);
	SendSocketData(SUB_C_OUT_CARD,&OutCard,sizeof(OutCard)-sizeof(OutCard.cbCardData)+OutCard.cbCardCount*sizeof(BYTE));

	//Ԥ����ʾ
	//PlayGameSound(AfxGetInstanceHandle(),TEXT("OUT_CARD"));
	m_GameClientView.m_UserCardControl[2].SetCardData(OutCard.cbCardData,OutCard.cbCardCount);

	//Ԥ��ɾ��
	BYTE cbSourceCount=m_cbHandCardCount;
	m_cbHandCardCount-=OutCard.cbCardCount;
	m_GameLogic.RemoveCard(OutCard.cbCardData,OutCard.cbCardCount,m_cbHandCardData,cbSourceCount);

	//�����˿�
	SetHandCardControl(m_cbHandCardData,m_cbHandCardCount);

	return 0;
}

//������Ϣ
LRESULT CGameClientEngine::OnCallCard(WPARAM wParam, LPARAM lParam)
{
	//��������
	BYTE cbCallCount=0;
	BYTE cbCallCard=0x00;
	BYTE cbCallColor=(BYTE)lParam;

	////����ͳ��
	//if (cbCallColor==COLOR_NT)
	//{
	//	//��Ŀͳ��
	//	BYTE cbNTCount[2]={0,0};
	//	for (BYTE i=0;i<m_cbDispatchCardCount;i++)
	//	{
	//		BYTE cbCardData=m_cbDispatchCardData[i];
	//		if ((cbCardData==0x4E)||(cbCardData==0x4F)) cbNTCount[cbCardData-0x4E]++;
	//	}

	//	//��������
	//	if (cbNTCount[0]>=cbNTCount[1])
	//	{
	//		cbCallCard=0x4E;
	//		cbCallCount=cbNTCount[0];
	//	}
	//	else
	//	{
	//		cbCallCard=0x4F;
	//		cbCallCount=cbNTCount[1];
	//	}
	//}
	//else
	//{
	//	//��Ŀͳ��
	//	for (BYTE i=0;i<m_cbDispatchCardCount;i++)
	//	{
	//		//��ȡ����
	//		BYTE cbCardData=m_cbDispatchCardData[i];
	//		BYTE cbCardColor=m_GameLogic.GetCardColor(cbCardData);
	//		BYTE cbCardValue=m_GameLogic.GetCardValue(cbCardData);

	//		//����ͳ��
	//		if ((cbCardValue==m_cbMainValue)&&(cbCardColor==cbCallColor)) 
	//		{
	//			cbCallCount++;
	//			cbCallCard=cbCardData;
	//		}
	//	}
	//}

	//���½���

	cbCallCard=(cbCallColor&0xF0)+0x02;
	cbCallCount=1;

	//��������
	CMD_C_CallCard CallCard;
	CallCard.cbCallCard=cbCallCard;
	CallCard.cbCallCount=cbCallCount;

	//��������
	SendSocketData(SUB_C_CALL_CARD,&CallCard,sizeof(CallCard));
	m_GameClientView.m_CallCardWnd.ShowWindow(SW_HIDE);
	return 0;
}

//������ʾ
LRESULT CGameClientEngine::OnOutPrompt(WPARAM wParam, LPARAM lParam)
{
	//��������
	tagOutCardResult OutCardResult;
	ZeroMemory(&OutCardResult,sizeof(OutCardResult));

	//��������
	BYTE cbTurnCardCount=(m_wCurrentUser==m_wFirstOutUser)?0:m_cbOutCardCount[m_wFirstOutUser][0];
	bool bSuccess=m_GameLogic.SearchOutCard(m_cbHandCardData,m_cbHandCardCount,m_cbOutCardData[m_wFirstOutUser][0],cbTurnCardCount,OutCardResult);

	//ʧ���ж�
	if (bSuccess==false)
	{
		if(m_pIStringMessage!=NULL)
			m_pIStringMessage->InsertSystemString(TEXT("�Զ���ʾ���ƴ�������Ѵ�״̬��Ϣ��ͼ���������ṩ��лл��"));
		return false;
	}

	//���ý���
	m_GameClientView.m_btOutCard.EnableWindow(TRUE);
	m_GameClientView.m_HandCardControl.SetShootCard(OutCardResult.cbResultCard,OutCardResult.cbCardCount);

	return 0;
}

//������Ϣ
LRESULT CGameClientEngine::OnSendConceal(WPARAM wParam, LPARAM lParam)
{
	//��ȡ�˿�
	BYTE cbCardData[MAX_COUNT];
	BYTE cbCardCount=(BYTE)m_GameClientView.m_HandCardControl.GetShootCard(cbCardData,CountArray(cbCardData));
	if(cbCardCount!=m_cbConcealCount) return 0;
	//ɾ���˿�
	m_cbHandCardCount-=cbCardCount;
	m_GameLogic.RemoveCard(cbCardData,cbCardCount,m_cbHandCardData,m_cbHandCardCount+cbCardCount);

	//���ý���
	KillGameClock(IDI_SEND_CONCEAL);
	m_GameClientView.m_btSendConceal.ShowWindow(SW_HIDE);
	m_GameClientView.m_btSendConceal.EnableWindow(FALSE);
	SetHandCardControl(m_cbHandCardData,m_cbHandCardCount);

	//��������
	CMD_C_ConcealCard ConcealCard;
	ConcealCard.cbConcealCount=cbCardCount;
	CopyMemory(ConcealCard.cbConcealCard,cbCardData,cbCardCount*sizeof(BYTE));

	//��������
	SendSocketData(SUB_C_CONCEAL_CARD,&ConcealCard,sizeof(ConcealCard));

	return 0;
}

//�鿴����
LRESULT CGameClientEngine::OnConcealCard(WPARAM wParam, LPARAM lParam)
{
	//�鿴����
	if (m_GameClientView.m_ConcealCardView.IsWindowVisible())
	{
		KillTimer(IDI_CONCEAL_CARD);
		m_GameClientView.m_ConcealCardView.ShowWindow(SW_HIDE);
	}
	else
	{
		SetTimer(IDI_CONCEAL_CARD,5000,NULL);
		m_GameClientView.m_ConcealCardView.ShowWindow(SW_SHOW);
	}

	return 0;
}

////�����뿪
//LRESULT CGameClientEngine::OnRuquestLeave(WPARAM wParam, LPARAM lParam)
//{
//	return 0;
//}

//�����˿�
LRESULT CGameClientEngine::OnLastTurnCard(WPARAM wParam, LPARAM lParam)
{
	if (m_bLastTurn==false)
	{
		//�����˿�
		for (WORD i=0;i<GAME_PLAYER;i++)
		{
			WORD wViewChairID=SwitchViewChairID(i);
			m_GameClientView.m_UserCardControl[wViewChairID].SetCardData(m_cbOutCardData[i][1],m_cbOutCardCount[i][1]);
		}

		//��������
		m_bLastTurn=true;
		m_GameClientView.SetLastTurnCard(true);
		SetTimer(IDI_LAST_TURN_CARD,3000,NULL);
	}
	else SwitchToCurrentCard();

	return 0;
}

//�Ϲܿ���
LRESULT CGameClientEngine::OnStusteeControl(WPARAM wParam, LPARAM lParam)
{
	//����״̬
	if (m_bStustee==true)
	{
		m_bStustee=false;
		m_GameClientView.m_btStusteeControl.SetButtonImage(IDB_START_TRUSTEE,AfxGetInstanceHandle(),false,false);
	}
	else
	{
		m_bStustee=true;
		m_GameClientView.m_btStusteeControl.SetButtonImage(IDB_STOP_TRUSTEE,AfxGetInstanceHandle(),false,false);
	}

	return 0;
}

//����˿�
LRESULT CGameClientEngine::OnLeftHitCard(WPARAM wParam, LPARAM lParam)
{
	//��ȡ״̬
	BYTE cbGameStatue=GetGameStatus();

	//����״̬
	switch (cbGameStatue)
	{
	case GS_UG_BACK:	//����״̬
		{
			//״̬����
			if ((IsLookonMode()==false)&&(m_wCurrentUser==GetMeChairID()))
			{
				//��ȡ�˿�
				BYTE cbCardData[MAX_COUNT];
				BYTE cbCardCount=(BYTE)m_GameClientView.m_HandCardControl.GetShootCard(cbCardData,CountArray(cbCardData));

				//���ư�ť
				m_GameClientView.m_btSendConceal.EnableWindow((cbCardCount==m_cbConcealCount)?TRUE:FALSE);
			}

			return 0;
		}
	case GS_UG_PLAY:	//��Ϸ״̬
		{
			//���ÿؼ�
			bool bOutCard=VerdictOutCard();
			m_GameClientView.m_btOutCard.EnableWindow(bOutCard?TRUE:FALSE);

			return 0;
		}
	}

	return 0;
}

//�Ҽ��˿�
LRESULT CGameClientEngine::OnRightHitCard(WPARAM wParam, LPARAM lParam)
{
	//�û�����
	OnOutCard(0,0);

	return 0;
}


//�з���Ϣ
LRESULT CGameClientEngine::OnLandScore(WPARAM wParam, LPARAM lParam)
{
	//���ý���
	KillGameClock(IDI_LAND_SCORE);
	m_GameClientView.m_CallScoreView.ShowWindow(SW_HIDE);

	//��������
	CMD_C_LandScore LandScore;
	LandScore.bLandScore=(BYTE)wParam;
	SendSocketData(SUB_C_LAND_SCORE,&LandScore,sizeof(LandScore));

	return 0;
}

//�û��з�
bool CGameClientEngine::OnSubLandScore(const void * pData, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_LandScore));
	if (wDataSize!=sizeof(CMD_S_LandScore)) return false;

	//��Ϣ����
	CMD_S_LandScore * pLandScore=(CMD_S_LandScore *)pData;

	//���ý���
	WORD wViewChairID=SwitchViewChairID(pLandScore->bLandUser);
	m_GameClientView.SetLandScore(wViewChairID,pLandScore->bLandScore);
	m_GameClientView.SetCurrentCallScore(pLandScore->bLandScore);
	m_GameClientView.m_CallScoreView.SetCurrentCallScore(pLandScore->bLandScore);

	//�û�����
	bool GiveUp[4];
	for(WORD i=0;i<GAME_PLAYER;i++)
	{
		GiveUp[SwitchViewChairID(i)]=pLandScore->bGiveUpScore[i];
	}
	m_GameClientView.SetGiveUpPlayer(GiveUp);

	//�û��з�
	WORD UserScore[4];
	for(WORD i=0;i<GAME_PLAYER;i++)
	{
		UserScore[SwitchViewChairID(i)]=pLandScore->wUserScore[i];
	}
	m_GameClientView.SetUserScore(UserScore);

	//�������
	if ((IsLookonMode()==false)&&(pLandScore->wCurrentUser==GetMeChairID()))
	{
		ActiveGameFrame();
		m_GameClientView.m_CallScoreView.ShowWindow(SW_SHOW);
	}

	//��������
	IClientUserItem *pUserItem=GetTableUserItem(pLandScore->bLandUser);
	PlayLandScoreSound(pUserItem,pLandScore->wUserScore[pLandScore->bLandUser]);

	//����ʱ��
	SetGameClock(pLandScore->wCurrentUser,IDI_LAND_SCORE,TIME_LAND_SCORE);

	return true;
}

//���Žз�����
void CGameClientEngine::PlayLandScoreSound(IClientUserItem *pUserItem,WORD wLandScore)
{
	if(pUserItem == NULL) return;
	CString strScore;
	if(pUserItem->GetGender()==1)	strScore.Format(TEXT("M_"));
	else 	strScore.Format(TEXT("F_"));

	if(wLandScore==255) strScore.AppendFormat(TEXT("NOCALL"));
	else strScore.AppendFormat(TEXT("%d"),wLandScore);
	PlayGameSound(AfxGetInstanceHandle(),strScore);
	return;
}

//////////////////////////////////////////////////////////////////////////
//ȡ����Ϣ
//VOID CGameClientEngine::OnCancel()
//{
//	//�˳���ʾ
//	if (GetGameStatus()!=GS_UG_FREE && IsLookonMode()==false)
//	{
//
//		const IClientUserItem * pUserItem=GetMeUserItem();
//		if (pUserItem!=NULL)
//		{
//			CString strMessage;
//			strMessage.Format(TEXT("��������Ϸ�У�ǿ���˳������� %ld �֣�ȷʵҪǿ����"),12*m_lCellScore);
//			int iRet=AfxMessageBox(strMessage,MB_YESNO|MB_ICONWARNING);
//			if (iRet!=IDYES) 
//			{
//				AfxGetMainWnd()->PostMessage(WM_CLOSE);
//				return;
//			}
//		}
//	}
//
////	return __super::OnCancel();
//}
//////////////////////////////////////////////////////////////////////////////////
