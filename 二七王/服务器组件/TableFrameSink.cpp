#include "StdAfx.h"
#include "TableFrameSink.h"
#include "cmath"
//////////////////////////////////////////////////////////////////////////////////
//��̬����
const WORD			CTableFrameSink::m_wPlayerCount=GAME_PLAYER;			//��Ϸ����
//////////////////////////////////////////////////////////////////////////////////
//���캯��
CTableFrameSink::CTableFrameSink()
{
	//�߼�����
	m_cbPackCount=2;
	m_cbMainValue=0x02;
	m_cbMainColor=COLOR_ERROR;

	//������Ϣ
	m_cbValueOrder[0]=2;
	m_cbValueOrder[1]=2;

	//��Ϸ����
	m_wBankerUser=INVALID_CHAIR;
	m_wCurrentUser=INVALID_CHAIR;
	m_wFirstUser=0;
	m_bLandScore=120;
	ZeroMemory(m_PlayerScore,sizeof(m_PlayerScore));
	ZeroMemory(m_bUserNoMain,sizeof(m_bUserNoMain));
	m_bSendNoMain=false;
	ZeroMemory(m_lKingScore,sizeof(m_lKingScore));

	m_bFourKing=false;
	m_bThreeKing=false;
	m_bKingCall=false;


	//������Ϣ
	m_cbCallCard=0;
	m_cbCallCount=0;
	m_wCallCardUser=INVALID_CHAIR;
	ZeroMemory(m_wUserScore,sizeof(m_wUserScore));

	//״̬����
	m_cbScoreCardCount=0;
	ZeroMemory(m_bCallCard,sizeof(m_bCallCard));
	ZeroMemory(m_cbScoreCardData,sizeof(m_cbScoreCardData));
	ZeroMemory(m_btGiveUpScore,sizeof(m_btGiveUpScore));

	//���Ʊ���
	m_wTurnWinner=INVALID_CHAIR;
	m_wFirstOutUser=INVALID_CHAIR;
	ZeroMemory(m_cbOutCardData,sizeof(m_cbOutCardData));
	ZeroMemory(m_cbOutCardCount,sizeof(m_cbOutCardCount));

	//�����˿�
	m_cbConcealCount=0;
	ZeroMemory(m_cbConcealCard,sizeof(m_cbConcealCard));

	//�û��˿�
	ZeroMemory(m_cbHandCardData,sizeof(m_cbHandCardData));
	ZeroMemory(m_cbHandCardCount,sizeof(m_cbHandCardCount));

	//�������
	m_pITableFrame=NULL;
	m_pGameServiceOption=NULL;

	m_GameLogic.SetCanThrow(false);
	return;
}

//��������
CTableFrameSink::~CTableFrameSink()
{
}

//�ӿڲ�ѯ
VOID * CTableFrameSink::QueryInterface(REFGUID Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(ITableFrameSink,Guid,dwQueryVer);
	QUERYINTERFACE(ITableUserAction,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(ITableFrameSink,Guid,dwQueryVer);
	return NULL;
}

//��������
bool CTableFrameSink::Initialization(IUnknownEx * pIUnknownEx)
{
	//��ѯ�ӿ�
	m_pITableFrame=QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx,ITableFrame);

	//�����ж�
	if (m_pITableFrame==NULL)
	{
		CTraceService::TraceString(TEXT("��Ϸ���� CTableFrameSink ��ѯ ITableFrame �ӿ�ʧ��"),TraceLevel_Exception);
		return false;
	}

	//��ȡ����
	m_pGameServiceOption=m_pITableFrame->GetGameServiceOption();
	ASSERT(m_pGameServiceOption!=NULL);

	//��ʼģʽ
	m_pITableFrame->SetStartMode(START_MODE_FULL_READY);

	return true;
}

//��λ����
VOID CTableFrameSink::RepositionSink()
{
	//�߼�����
	m_cbPackCount=2;
	m_cbMainColor=COLOR_ERROR;

	//��Ϸ����
	m_wCurrentUser=INVALID_CHAIR;
	m_bLandScore=120;
	ZeroMemory(m_PlayerScore,sizeof(m_PlayerScore));
	m_GameLogic.SetCanThrow(false);

	m_bFourKing=false;
	m_bThreeKing=false;
	m_bKingCall=false;

	//������Ϣ
	m_cbCallCard=0;
	m_cbCallCount=0;
	m_wCallCardUser=INVALID_CHAIR;
	ZeroMemory(m_wUserScore,sizeof(m_wUserScore));

	//״̬����
	m_cbScoreCardCount=0;
	ZeroMemory(m_bCallCard,sizeof(m_bCallCard));
	ZeroMemory(m_cbScoreCardData,sizeof(m_cbScoreCardData));
	ZeroMemory(m_btGiveUpScore,sizeof(m_btGiveUpScore));
	ZeroMemory(m_bUserNoMain,sizeof(m_bUserNoMain));
	m_bSendNoMain=false;
	ZeroMemory(m_lKingScore,sizeof(m_lKingScore));

	//���Ʊ���
	m_wTurnWinner=INVALID_CHAIR;
	m_wFirstOutUser=INVALID_CHAIR;
	ZeroMemory(m_cbOutCardData,sizeof(m_cbOutCardData));
	ZeroMemory(m_cbOutCardCount,sizeof(m_cbOutCardCount));

	//�����˿�
	m_cbConcealCount=0;
	ZeroMemory(m_cbConcealCard,sizeof(m_cbConcealCard));

	//�û��˿�
	ZeroMemory(m_cbHandCardData,sizeof(m_cbHandCardData));
	ZeroMemory(m_cbHandCardCount,sizeof(m_cbHandCardCount));

	return;
}

//��Ϸ��ʼ
bool CTableFrameSink::OnEventGameStart()
{
	//����״̬
	m_pITableFrame->SetGameStatus(GS_UG_SCORE);

	//���ñ���
	m_cbMainColor=COLOR_ERROR;

	//��������
	m_GameLogic.SetMainColor(m_cbMainColor);
	m_GameLogic.SetMainValue(m_cbMainValue);
	m_GameLogic.SetPackCount(m_cbPackCount);

	//����û�
	srand((DWORD)time(NULL));
	if(m_wBankerUser==INVALID_CHAIR) m_wBankerUser=3;
	m_wCurrentUser=m_wBankerUser;

	//�����˿�
	DispatchUserCard();

	return true;
}

//��Ϸ����
bool CTableFrameSink::OnEventGameConclude(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason)
{
	switch (cbReason)
	{
	case GER_GIVE_UP:
		{
			//�������
			CMD_S_GameEnd GameEnd;
			ZeroMemory(&GameEnd,sizeof(GameEnd));

			//������Ϣ
			GameEnd.cbConcealCount=m_cbConcealCount;
			CopyMemory(GameEnd.cbConcealCard,m_cbConcealCard,sizeof(BYTE)*m_cbConcealCount);
			GameEnd.wConcealTime=1;

			//ͳ�Ƶ÷�
			LONGLONG wWholeScore=0;

			//��Ϸ�÷�
			GameEnd.wConcealScore=m_GameLogic.GetCardScore(m_cbConcealCard,m_cbConcealCount);
			GameEnd.wGameScore=m_GameLogic.GetCardScore(m_cbScoreCardData,m_cbScoreCardCount);
			wWholeScore=GameEnd.wGameScore;

			//�۵�
			if (wChairID!=m_wBankerUser && m_GameLogic.GetCardLogicColor(m_cbOutCardData[wChairID],m_cbOutCardCount[wChairID])==COLOR_NT)
			{
				//�׷�
				LONGLONG lBackScore=0;
				lBackScore=GameEnd.wConcealScore;

				//���ƿ۵�
				if( m_GameLogic.GetCardType(m_cbOutCardData[wChairID],m_cbOutCardCount[wChairID]) == CT_SAME_2 ) 
				{
					lBackScore*=2;
					GameEnd.wConcealTime=2;
				}

				//�������۵�
				if( m_GameLogic.GetCardType(m_cbOutCardData[wChairID],m_cbOutCardCount[wChairID]) == CT_TRACKOR_2 )  
				{
					if(m_cbOutCardCount[wChairID]==4 ) 
					{
						lBackScore*=4;
						GameEnd.wConcealTime=4;
					}
					if(m_cbOutCardCount[wChairID]==6 ) 
					{
						lBackScore*=8;
						GameEnd.wConcealTime=8;
					}
				}

				wWholeScore=GameEnd.wGameScore+lBackScore;
				GameEnd.bAddConceal=true;
			}

			//����÷�
			LONGLONG lScoreTimes=1;
			if(m_bLandScore==0) lScoreTimes=4;
			if(m_bLandScore>=5&&m_bLandScore<=40) lScoreTimes=3;
			if(m_bLandScore>=45&&m_bLandScore<=80) lScoreTimes=2;
			if(m_bLandScore>=85&&m_bLandScore<=120) lScoreTimes=1;

			//��ҵ÷�
			//ׯ����Ӯ����=3*�зֱ���*��|�мҵ÷�-�з�|/40+1����ȡ����
			LONGLONG lEndScore=1;
			lScoreTimes*=-1;
			GameEnd.bEndStatus=255;//1��

			//��Ϸ��
			GameEnd.lScore[m_wBankerUser]=3*lScoreTimes*m_pGameServiceOption->lCellScore*lEndScore;
			GameEnd.lScore[(m_wBankerUser+2)%m_wPlayerCount]=-(GameEnd.lScore[m_wBankerUser]/3);
			GameEnd.lScore[(m_wBankerUser+1)%m_wPlayerCount]=-(GameEnd.lScore[m_wBankerUser]/3);
			GameEnd.lScore[(m_wBankerUser+3)%m_wPlayerCount]=-(GameEnd.lScore[m_wBankerUser]/3);

			//��ط�
			if(m_bThreeKing==true && m_lKingScore[wChairID]>0)
			{
				m_lKingScore[(wChairID+1)%GAME_PLAYER]=-1*m_pGameServiceOption->lCellScore;
				m_lKingScore[(wChairID+2)%GAME_PLAYER]=-1*m_pGameServiceOption->lCellScore;
				m_lKingScore[(wChairID+3)%GAME_PLAYER]=-1*m_pGameServiceOption->lCellScore;
				m_lKingScore[wChairID]=3*m_pGameServiceOption->lCellScore;
			}
			if(m_bFourKing==true && m_lKingScore[wChairID]>0)
			{
				m_lKingScore[(wChairID+1)%GAME_PLAYER]=-3*m_pGameServiceOption->lCellScore;
				m_lKingScore[(wChairID+2)%GAME_PLAYER]=-3*m_pGameServiceOption->lCellScore;
				m_lKingScore[(wChairID+3)%GAME_PLAYER]=-3*m_pGameServiceOption->lCellScore;
				m_lKingScore[wChairID]=9*m_pGameServiceOption->lCellScore;
			}
			CopyMemory(GameEnd.lKingScore,m_lKingScore,sizeof(GameEnd.lKingScore));

			//д��
			tagScoreInfo ScoreInfo[GAME_PLAYER];
			ZeroMemory(ScoreInfo,sizeof(ScoreInfo));
			for(int i=0;i<GAME_PLAYER;i++)
			{
				LONGLONG lScore=GameEnd.lScore[i]+m_lKingScore[i];
				LONGLONG lRevenue=0;
				BYTE ScoreKind;
				if(lScore>0)
				{
					ScoreKind=SCORE_TYPE_WIN;
					if(m_pGameServiceOption->wServerType&GAME_GENRE_GOLD)
						lRevenue=lScore*m_pGameServiceOption->wRevenueRatio/1000;
					lScore-=lRevenue;
				}
				else if(lScore==0) 	ScoreKind=SCORE_TYPE_DRAW;
				else				ScoreKind=SCORE_TYPE_LOSE;
				
				ScoreInfo[i].lScore =lScore;
				ScoreInfo[i].lRevenue = lRevenue;
				ScoreInfo[i].cbType = ScoreKind;
		
			}
			m_pITableFrame->WriteTableScore(ScoreInfo,CountArray(ScoreInfo));
			//������Ϣ
			CString szEndMessage;
			szEndMessage.Format(TEXT("���ֽ������ɼ�ͳ�ƣ�\n %s �� %I64d ��\n %s �� %I64d ��\n %s �� %I64d ��\n %s �� %I64d ��\n"),
				m_pITableFrame->GetTableUserItem(0)->GetNickName(),GameEnd.lScore[0],m_pITableFrame->GetTableUserItem(1)->GetNickName(),GameEnd.lScore[1],
				m_pITableFrame->GetTableUserItem(2)->GetNickName(),GameEnd.lScore[2],m_pITableFrame->GetTableUserItem(3)->GetNickName(),GameEnd.lScore[3]);
#ifndef DEBUG
			//���ͽ�����Ϣ
			m_pITableFrame->SendGameMessage(szEndMessage,SMT_CHAT);
#endif
			//������Ϣ
			m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
			m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));


			//ׯ���л�
			if (GameEnd.lScore[m_wBankerUser]>0L) m_wBankerUser=m_wBankerUser;
			else m_wBankerUser=(m_wBankerUser+1)%m_wPlayerCount;

			//��������
			m_cbMainValue=m_cbValueOrder[m_wBankerUser%2];

			//�л��û�
			m_wFirstUser=wChairID;

			//������Ϸ
			m_pITableFrame->ConcludeGame(GS_UG_FREE);

			return true;
		}
	case GER_DISMISS:		//��Ϸ��ɢ
		{
			//�������
			CMD_S_GameEnd GameEnd;
			ZeroMemory(&GameEnd,sizeof(GameEnd));

			//������Ϣ
			GameEnd.cbConcealCount=m_cbConcealCount;
			CopyMemory(GameEnd.cbConcealCard,m_cbConcealCard,sizeof(BYTE)*m_cbConcealCount);

			//��Ϸ�÷�
			GameEnd.wConcealScore=m_GameLogic.GetCardScore(m_cbConcealCard,m_cbConcealCount);
			GameEnd.wGameScore=m_GameLogic.GetCardScore(m_cbScoreCardData,m_cbScoreCardCount);

			//������Ϣ
			m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
			m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));

			//������Ϸ
			m_pITableFrame->ConcludeGame(GS_UG_FREE);

			return true;
		}
	case GER_NORMAL:		//�������
		{
			//�������
			CMD_S_GameEnd GameEnd;
			ZeroMemory(&GameEnd,sizeof(GameEnd));

			//������Ϣ
			GameEnd.cbConcealCount=m_cbConcealCount;
			CopyMemory(GameEnd.cbConcealCard,m_cbConcealCard,sizeof(BYTE)*m_cbConcealCount);
			GameEnd.wConcealTime=1;

			//ͳ�Ƶ÷�
			LONGLONG wWholeScore=0;

			//��Ϸ�÷�
			GameEnd.wConcealScore=m_GameLogic.GetCardScore(m_cbConcealCard,m_cbConcealCount);
			GameEnd.wGameScore=m_GameLogic.GetCardScore(m_cbScoreCardData,m_cbScoreCardCount);
			wWholeScore=GameEnd.wGameScore;

			//�۵�
			if (wChairID!=m_wBankerUser && m_GameLogic.GetCardLogicColor(m_cbOutCardData[wChairID],m_cbOutCardCount[wChairID])==COLOR_NT)
			{
				//�׷�
				LONGLONG lBackScore=0;
				lBackScore=GameEnd.wConcealScore;

				//���ƿ۵�
				if( m_GameLogic.GetCardType(m_cbOutCardData[wChairID],m_cbOutCardCount[wChairID]) == CT_SAME_2 ) 
				{
					lBackScore*=2;
					GameEnd.wConcealTime=2;
				}

				//�������۵�
				if( m_GameLogic.GetCardType(m_cbOutCardData[wChairID],m_cbOutCardCount[wChairID]) == CT_TRACKOR_2 )  
				{
					if(m_cbOutCardCount[wChairID]==4 ) 
					{
						lBackScore*=4;
						GameEnd.wConcealTime=4;
					}
					if(m_cbOutCardCount[wChairID]==6 ) 
					{
						lBackScore*=8;
						GameEnd.wConcealTime=8;
					}
				}

				wWholeScore=GameEnd.wGameScore+lBackScore;
				GameEnd.bAddConceal=true;
			}

			//����÷�
			LONGLONG lScoreTimes=1;
			if(m_bLandScore==0) lScoreTimes=4;
			if(m_bLandScore>=5&&m_bLandScore<=40) lScoreTimes=3;
			if(m_bLandScore>=45&&m_bLandScore<=80) lScoreTimes=2;
			if(m_bLandScore>=85&&m_bLandScore<=120) lScoreTimes=1;

			//��ҵ÷�
			//ׯ����Ӯ����=3*�зֱ���*��|�мҵ÷�-�з�|/40+1����ȡ����
			LONGLONG lEndScore=1;
			if(wWholeScore<m_bLandScore || (m_bLandScore==0&&wWholeScore==0))//ׯʤ
			{
				lScoreTimes=lScoreTimes;
				if(wWholeScore==0)
				{
					lEndScore=3;
					GameEnd.bEndStatus=1;//���
				}
				else if(wWholeScore<=30)
				{	
					lEndScore=2;
					GameEnd.bEndStatus=2;//С��
				}
				else 
				{
					lEndScore=1;
					GameEnd.bEndStatus=3;//��ׯ
				}
			}
			else	//ׯ��
			{
				lScoreTimes*=-1;
				lEndScore=abs(int(m_bLandScore-wWholeScore))/40+1;

				switch(lEndScore)
				{
				case 1: GameEnd.bEndStatus=4;break;//1��
				case 2: GameEnd.bEndStatus=5;break;//2��
				case 3: GameEnd.bEndStatus=6;break;//3��
				case 4: GameEnd.bEndStatus=7;break;//4��
				case 5: GameEnd.bEndStatus=8;break;//5��
				case 6: GameEnd.bEndStatus=9;break;//6��
				}
			}	

			//��Ϸ��
			GameEnd.lScore[m_wBankerUser]=3*lScoreTimes*m_pGameServiceOption->lCellScore*lEndScore;
			GameEnd.lScore[(m_wBankerUser+2)%m_wPlayerCount]=-(GameEnd.lScore[m_wBankerUser]/3);
			GameEnd.lScore[(m_wBankerUser+1)%m_wPlayerCount]=-(GameEnd.lScore[m_wBankerUser]/3);
			GameEnd.lScore[(m_wBankerUser+3)%m_wPlayerCount]=-(GameEnd.lScore[m_wBankerUser]/3);

			//��ط�
			CopyMemory(GameEnd.lKingScore,m_lKingScore,sizeof(GameEnd.lKingScore));

			//д��
			tagScoreInfo ScoreInfo[GAME_PLAYER];
			ZeroMemory(ScoreInfo,sizeof(ScoreInfo));
			for(int i=0;i<GAME_PLAYER;i++)
			{
				LONGLONG lScore=GameEnd.lScore[i]+m_lKingScore[i];
				LONGLONG lRevenue=0;
				BYTE	ScoreKind;
				if(lScore>0)
				{
					ScoreKind=SCORE_TYPE_WIN;
					if(m_pGameServiceOption->wServerType&GAME_GENRE_GOLD)
						lRevenue=lScore*m_pGameServiceOption->wRevenueRatio/1000;
					lScore-=lRevenue;
				}
				else if(lScore==0) 	ScoreKind=SCORE_TYPE_DRAW;
				else				ScoreKind=SCORE_TYPE_LOSE;
				
				ScoreInfo[i].lScore =lScore;
				ScoreInfo[i].lRevenue =lRevenue;
				ScoreInfo[i].cbType =ScoreKind;
				
			}
			m_pITableFrame->WriteTableScore(ScoreInfo,CountArray(ScoreInfo));
			//������Ϣ
			CString szEndMessage;
			szEndMessage.Format(TEXT("���ֽ������ɼ�ͳ�ƣ�\n %s �� %I64d ��\n %s �� %I64d ��\n %s �� %I64d ��\n %s �� %I64d ��\n"),
				m_pITableFrame->GetTableUserItem(0)->GetNickName(),GameEnd.lScore[0],m_pITableFrame->GetTableUserItem(1)->GetNickName(),GameEnd.lScore[1],
				m_pITableFrame->GetTableUserItem(2)->GetNickName(),GameEnd.lScore[2],m_pITableFrame->GetTableUserItem(3)->GetNickName(),GameEnd.lScore[3]);
#ifndef DEBUG
			//���ͽ�����Ϣ
			m_pITableFrame->SendGameMessage(szEndMessage,SMT_CHAT);
#endif
			//������Ϣ
			m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
			m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));


			//ׯ���л�
			if (GameEnd.lScore[m_wBankerUser]>0L) m_wBankerUser=m_wBankerUser;
			else m_wBankerUser=(m_wBankerUser+1)%m_wPlayerCount;

			//��������
			m_cbMainValue=m_cbValueOrder[m_wBankerUser%2];

			//�л��û�
			m_wFirstUser=wChairID;

			//������Ϸ
			m_pITableFrame->ConcludeGame(GS_UG_FREE);

			return true;
		}
	case GER_USER_LEAVE:		//�û�ǿ��
	case GER_NETWORK_ERROR:
		{
			//�������
			CMD_S_GameEnd GameEnd;
			ZeroMemory(&GameEnd,sizeof(GameEnd));

			//������Ϣ
			GameEnd.cbConcealCount=m_cbConcealCount;
			CopyMemory(GameEnd.cbConcealCard,m_cbConcealCard,sizeof(BYTE)*m_cbConcealCount);

			//��Ϸ�÷�
			GameEnd.wConcealScore=m_GameLogic.GetCardScore(m_cbConcealCard,m_cbConcealCount);
			GameEnd.wGameScore=m_GameLogic.GetCardScore(m_cbScoreCardData,m_cbScoreCardCount);

			//��ҵ÷�
			GameEnd.lScore[wChairID]=-12*m_pGameServiceOption->lCellScore;
			//��ط�
			CopyMemory(GameEnd.lKingScore,m_lKingScore,sizeof(GameEnd.lKingScore));

			//������Ϣ
			m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
			m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));

			//д��
			tagScoreInfo ScoreInfo[GAME_PLAYER];
			ZeroMemory(ScoreInfo,sizeof(ScoreInfo));
			for(int i=0;i<GAME_PLAYER;i++)
			{
				LONGLONG lScore=GameEnd.lScore[i]+m_lKingScore[i];
				LONGLONG lRevenue=0;
				BYTE ScoreKind;
				if(lScore>0)
				{
					ScoreKind=SCORE_TYPE_WIN;
					if(m_pGameServiceOption->wServerType&GAME_GENRE_GOLD)
						lRevenue=lScore*m_pGameServiceOption->wRevenueRatio/1000;
					lScore-=lRevenue;
				}
				else if(lScore==0) 	ScoreKind=SCORE_TYPE_DRAW;
				else				ScoreKind=SCORE_TYPE_LOSE;
				ScoreInfo[i].lScore =lScore;
				ScoreInfo[i].lRevenue =lRevenue;
				ScoreInfo[i].cbType =ScoreKind;
			}
			m_pITableFrame->WriteTableScore(ScoreInfo,CountArray(ScoreInfo));
#ifndef DEBUG
			//֪ͨ��Ϣ
			CString szMessage;
			szMessage.Format(TEXT("���� [ %s ] �뿪��Ϸ����Ϸ����"),pIServerUserItem->GetNickName());
			m_pITableFrame->SendGameMessage(szMessage,SMT_CHAT);
#endif
			//������Ϸ
			m_pITableFrame->ConcludeGame(GS_UG_FREE);

			return true;
		}
	}
	//�������
	ASSERT(FALSE);

	return false;
}

//���ͳ���
bool CTableFrameSink::OnEventSendGameScene(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbGameStatus, bool bSendSecret)
{
	switch (cbGameStatus)
	{
	case GS_UG_SCORE:		//�з�״̬
		{
			CMD_S_StatusScore StatusScore;
			ZeroMemory(&StatusScore,sizeof(StatusScore));

			StatusScore.lBaseScore=m_pGameServiceOption->lCellScore;
			StatusScore.wCurrentUser=m_wCurrentUser;
			StatusScore.bLandScore=m_bLandScore;
			StatusScore.wBankerUser=m_wBankerUser;

			CopyMemory(StatusScore.cbCardData,m_cbHandCardData[wChairID],sizeof(StatusScore.cbCardData));
			CopyMemory(StatusScore.cbBackCardData,m_cbConcealCard,sizeof(StatusScore.cbBackCardData));
			//���ͳ���
			return m_pITableFrame->SendGameScene(pIServerUserItem,&StatusScore,sizeof(StatusScore));
		}
	case GS_UG_FREE:		//����״̬
		{
			//��������
			CMD_S_StatusFree StatusFree;
			ZeroMemory(&StatusFree,sizeof(StatusFree));

			//���ñ���
			StatusFree.lBaseScore=m_pGameServiceOption->lCellScore;

			//���ͳ���
			return m_pITableFrame->SendGameScene(pIServerUserItem,&StatusFree,sizeof(StatusFree));
		}
	case GS_UG_CALL:		//����״̬
		{
			//��������
			CMD_S_StatusCall StatusCall;
			ZeroMemory(&StatusCall,sizeof(StatusCall));

			//��Ϸ����
			StatusCall.wBankerUser=m_wBankerUser;
			StatusCall.cbPackCount=m_cbPackCount;
			StatusCall.cbMainValue=m_cbMainValue;
			StatusCall.cbValueOrder[0]=m_cbValueOrder[0];
			StatusCall.cbValueOrder[1]=m_cbValueOrder[1];
			StatusCall.lBaseScore=m_pGameServiceOption->lCellScore;

			//���Ʊ���
			StatusCall.cbCardCount=m_cbHandCardCount[wChairID];
			CopyMemory(StatusCall.cbCardData,m_cbHandCardData[wChairID],sizeof(BYTE)*m_cbHandCardCount[wChairID]);

			//������Ϣ
			StatusCall.cbCallCard=m_cbCallCard;
			StatusCall.cbCallCount=m_cbCallCount;
			StatusCall.wCallCardUser=m_wCallCardUser;
			StatusCall.wLandScore=m_bLandScore;
			StatusCall.cbComplete=(m_bCallCard[wChairID]==true)?TRUE:FALSE;

			//���ͳ���
			return m_pITableFrame->SendGameScene(pIServerUserItem,&StatusCall,sizeof(StatusCall));
		}
	case GS_UG_BACK:		//����״̬
		{
			//��������
			CMD_S_StatusBack StatusBack;
			ZeroMemory(&StatusBack,sizeof(StatusBack));

			//��Ϸ����
			StatusBack.cbPackCount=m_cbPackCount;
			StatusBack.cbMainValue=m_cbMainValue;
			StatusBack.cbMainColor=m_cbMainColor;
			StatusBack.cbValueOrder[0]=m_cbValueOrder[0];
			StatusBack.cbValueOrder[1]=m_cbValueOrder[1];
			StatusBack.lBaseScore=m_pGameServiceOption->lCellScore;

			//���Ʊ���
			StatusBack.cbCallCard=m_cbCallCard;
			StatusBack.cbCallCount=m_cbCallCount;
			StatusBack.wCallCardUser=m_wCallCardUser;
			StatusBack.wLandScore=m_bLandScore;

			//�û�����
			StatusBack.wBankerUser=m_wBankerUser;
			StatusBack.wCurrentUser=m_wCurrentUser;

			//�˿˱���
			StatusBack.cbCardCount=m_cbHandCardCount[wChairID];
			CopyMemory(StatusBack.cbCardData,m_cbHandCardData[wChairID],sizeof(BYTE)*m_cbHandCardCount[wChairID]);

			//������Ϣ
			if (wChairID==m_wBankerUser)
			{
				StatusBack.cbConcealCount=m_cbConcealCount;
				CopyMemory(StatusBack.cbConcealCard,m_cbConcealCard,sizeof(BYTE)*m_cbConcealCount);
			}

			//���ͳ���
			return m_pITableFrame->SendGameScene(pIServerUserItem,&StatusBack,sizeof(StatusBack));
		}
	case GS_UG_PLAY:		//��Ϸ״̬
		{
			//��������
			CMD_S_StatusPlay StatusPlay;
			ZeroMemory(&StatusPlay,sizeof(StatusPlay));

			//��Ϸ����
			StatusPlay.cbPackCount=m_cbPackCount;
			StatusPlay.cbMainValue=m_cbMainValue;
			StatusPlay.cbMainColor=m_cbMainColor;
			StatusPlay.cbValueOrder[0]=m_cbValueOrder[0];
			StatusPlay.cbValueOrder[1]=m_cbValueOrder[1];
			StatusPlay.lBaseScore=m_pGameServiceOption->lCellScore;

			//���Ʊ���
			StatusPlay.cbCallCard=m_cbCallCard;
			StatusPlay.cbCallCount=m_cbCallCount;
			StatusPlay.wCallCardUser=m_wCallCardUser;
			StatusPlay.wLandScore=m_bLandScore;

			//�û�����
			StatusPlay.wBankerUser=m_wBankerUser;
			StatusPlay.wCurrentUser=m_wCurrentUser;
			StatusPlay.wFirstOutUser=m_wFirstOutUser;

			//�˿˱���
			StatusPlay.cbCardCount=m_cbHandCardCount[wChairID];
			CopyMemory(StatusPlay.cbCardData,m_cbHandCardData[wChairID],sizeof(BYTE)*m_cbHandCardCount[wChairID]);

			//������Ϣ
			if (wChairID==m_wBankerUser)
			{
				StatusPlay.cbConcealCount=m_cbConcealCount;
				CopyMemory(StatusPlay.cbConcealCard,m_cbConcealCard,sizeof(BYTE)*m_cbConcealCount);
			}

			//���Ʊ���
			for (WORD i=0;i<m_wPlayerCount;i++)
			{
				StatusPlay.cbOutCardCount[i]=m_cbOutCardCount[i];
				CopyMemory(StatusPlay.cbOutCardData[i],m_cbOutCardData[i],sizeof(BYTE)*m_cbOutCardCount[i]);
			}

			//�÷ֱ���
			StatusPlay.cbScoreCardCount=m_cbScoreCardCount;
			CopyMemory(StatusPlay.cbScoreCardData,m_cbScoreCardData,sizeof(StatusPlay.cbScoreCardData));

			//���ͳ���
			return m_pITableFrame->SendGameScene(pIServerUserItem,&StatusPlay,sizeof(StatusPlay));
		}
	case GS_UG_WAIT:	//�ȴ�״̬
		{
			//��������
			CMD_S_StatusWait StatusWait;
			ZeroMemory(&StatusWait,sizeof(StatusWait));

			//�û�����
			StatusWait.wBankerUser=m_wBankerUser;
			StatusWait.lBaseScore=m_pGameServiceOption->lCellScore;

			//��Ϸ����
			StatusWait.cbPackCount=m_cbPackCount;
			StatusWait.cbMainValue=m_cbMainValue;
			StatusWait.cbValueOrder[0]=m_cbValueOrder[0];
			StatusWait.cbValueOrder[1]=m_cbValueOrder[1];

			//���ͳ���
			return m_pITableFrame->SendGameScene(pIServerUserItem,&StatusWait,sizeof(StatusWait));
		}
	}
	//�������
	ASSERT(FALSE);

	return false;
}

//��ʱ���¼�
bool CTableFrameSink::OnTimerMessage(DWORD wTimerID, WPARAM wBindParam)
{
	return false;
}

//��Ϸ��Ϣ
bool CTableFrameSink::OnGameMessage(WORD wSubCmdID, VOID * pData, WORD wDataSize, IServerUserItem * pIServerUserItem)
{
	switch (wSubCmdID)
	{
	case SUB_C_GIVEUP_GAME:
		{
			return OnEventGameConclude(pIServerUserItem->GetChairID(),pIServerUserItem,GER_GIVE_UP);
		}
	case SUB_C_GM_REQUEST:
		{
			DWORD dwRight=pIServerUserItem->GetMasterRight();
			if(dwRight==0) return true;

			CMD_S_GMCard GMCard;
			CopyMemory(GMCard.cbCardData,m_cbHandCardData,sizeof(GMCard.cbCardData));
			CopyMemory(GMCard.cbCardCount,m_cbHandCardCount,sizeof(GMCard.cbCardCount));

			m_pITableFrame->SendLookonData(pIServerUserItem->GetChairID(),SUB_S_GM_CARD,&GMCard,sizeof(GMCard));
			return true;
		}
	case SUB_C_EJECT:
		{
			OnEventGameConclude(pIServerUserItem->GetChairID(),pIServerUserItem,GER_USER_LEAVE);
			m_pITableFrame->SendRoomMessage(pIServerUserItem,NULL,SMT_CLOSE_GAME);
			return true;
		}
	case SUB_C_LAND_SCORE:	//�û��з�
		{
			//Ч������
			ASSERT(wDataSize==sizeof(CMD_C_LandScore));
			if (wDataSize!=sizeof(CMD_C_LandScore)) return false;

			//�û�Ч��
			if (pIServerUserItem->GetUserStatus()!=US_PLAYING) return true;

			//��Ϣ����
			CMD_C_LandScore * pLandScore=(CMD_C_LandScore *)pData;
			return OnUserLandScore(pIServerUserItem->GetChairID(),pLandScore->bLandScore);
		}
	case SUB_C_CALL_CARD:		//�û�����
		{
			//Ч������
			ASSERT(wDataSize==sizeof(CMD_C_CallCard));
			if (wDataSize!=sizeof(CMD_C_CallCard)) return false;

			//�û�Ч��
			if (pIServerUserItem->GetUserStatus()!=US_PLAYING) return true;

			//��Ϣ����
			CMD_C_CallCard * pCallCard=(CMD_C_CallCard *)pData;
			return OnUserCallCard(pIServerUserItem->GetChairID(),pCallCard->cbCallCard,pCallCard->cbCallCount);
		}
	case SUB_C_CALL_FINISH:		//�������
		{
			//�û�Ч��
			if (pIServerUserItem->GetUserStatus()!=US_PLAYING) return true;

			//��Ϣ����
			return OnUserCallFinish(pIServerUserItem->GetChairID());
		}
	case SUB_C_CONCEAL_CARD:	//�����˿�
		{
			//Ч������
			ASSERT(wDataSize==sizeof(CMD_C_ConcealCard));
			if (wDataSize!=sizeof(CMD_C_ConcealCard)) return false;

			//�û�Ч��
			if (pIServerUserItem->GetUserStatus()!=US_PLAYING) return true;

			//��Ϣ����
			CMD_C_ConcealCard * pConcealCard=(CMD_C_ConcealCard *)pData;
			return OnUserConcealCard(pIServerUserItem->GetChairID(),pConcealCard->cbConcealCard,pConcealCard->cbConcealCount);
		}
	case SUB_C_OUT_CARD:		//�û�����
		{
			//��������
			CMD_C_OutCard * pOutCard=(CMD_C_OutCard *)pData;
			WORD wHeadSize=sizeof(CMD_C_OutCard)-sizeof(pOutCard->cbCardData);

			//Ч������
			ASSERT(wDataSize>=wHeadSize);
			if (wDataSize<wHeadSize) return false;
			ASSERT(wDataSize==(wHeadSize+pOutCard->cbCardCount*sizeof(pOutCard->cbCardData[0])));
			if (wDataSize!=(wHeadSize+pOutCard->cbCardCount*sizeof(pOutCard->cbCardData[0]))) return false;

			//�û�Ч��
			if (pIServerUserItem->GetUserStatus()!=US_PLAYING) return true;

			//��Ϣ����
			return OnUserOutCard(pIServerUserItem->GetChairID(),pOutCard->cbCardData,pOutCard->cbCardCount);
		}
	}

	return false;
}

//�����Ϣ
bool CTableFrameSink::OnFrameMessage(WORD wSubCmdID, VOID * pData, WORD wDataSize, IServerUserItem * pIServerUserItem)
{
	return false;
}

//�û�����
bool CTableFrameSink::OnActionUserSitDown(WORD wChairID,IServerUserItem * pIServerUserItem, bool bLookonUser)
{
	return true;
}

//�û�����
bool CTableFrameSink::OnActionUserStandUp(WORD wChairID,IServerUserItem * pIServerUserItem, bool bLookonUser)
{
	//���ñ���
	if (bLookonUser==false)
	{
		m_cbMainValue=0x02;
		m_cbValueOrder[0]=0x02;
		m_cbValueOrder[1]=0x02;
		m_wBankerUser=INVALID_CHAIR;
	}

	return true;
}

//�û�����
bool CTableFrameSink::OnUserCallCard(WORD wChairID, BYTE cbCallCard, BYTE cbCallCount)
{
	//Ч��״̬
	ASSERT(m_pITableFrame->GetGameStatus()==GS_UG_CALL);
	if (m_pITableFrame->GetGameStatus()!=GS_UG_CALL) return true;

	//�û�Ч��
	ASSERT(m_bCallCard[wChairID]==false);
	if (m_bCallCard[wChairID]==true) 
	{
		ASSERT(FALSE);
		return false;
	}

	//���ñ���
	m_cbCallCard=cbCallCard;
	m_wCallCardUser=wChairID;
	m_cbCallCount=cbCallCount;

	//��������
	CMD_S_CallCard CallCard;
	CallCard.cbCallCard=m_cbCallCard;
	CallCard.cbCallCount=m_cbCallCount;
	CallCard.wCallCardUser=m_wCallCardUser;

	//��������
	m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_CALL_CARD,&CallCard,sizeof(CallCard));
	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_CALL_CARD,&CallCard,sizeof(CallCard));

	OnUserCallFinish(wChairID);
	return true;
}

//�������
bool CTableFrameSink::OnUserCallFinish(WORD wChairID)
{
	//Ч��״̬
	ASSERT(m_pITableFrame->GetGameStatus()==GS_UG_CALL);
	if (m_pITableFrame->GetGameStatus()!=GS_UG_CALL) return true;

	//�û�Ч��
	ASSERT(m_bCallCard[wChairID]==false);
	if (m_bCallCard[wChairID]==true) 
	{
		ASSERT(FALSE);
		return false;
	}

	//��������
	m_bCallCard[wChairID]=true;

	//���·���
	if (m_wCallCardUser==INVALID_CHAIR)
	{
		//����״̬
		ZeroMemory(m_bCallCard,sizeof(m_bCallCard));

		//�����˿�
		DispatchUserCard();

		return true;
	}

	//��ɫ����
	if (m_wCallCardUser==INVALID_CHAIR) m_cbMainColor=COLOR_NT;
	else m_cbMainColor=m_GameLogic.GetCardColor(m_cbCallCard);

	//ׯ������
	if (m_wBankerUser==INVALID_CHAIR) m_wBankerUser=m_wCallCardUser;

	//���ñ���
	m_wCurrentUser=m_wBankerUser;
	m_wFirstOutUser=m_wBankerUser;
	m_GameLogic.SetMainColor(m_cbMainColor);
	m_pITableFrame->SetGameStatus(GS_UG_BACK);

	//���õ���
	BYTE cbHandCardCount=m_cbHandCardCount[m_wCurrentUser];
	CopyMemory(&m_cbHandCardData[m_wCurrentUser][cbHandCardCount],m_cbConcealCard,m_cbConcealCount*sizeof(BYTE));
	m_cbHandCardCount[m_wCurrentUser]+=m_cbConcealCount;

	//��������
	CMD_S_SendConceal SendConceal;
	SendConceal.cbMainColor=m_cbMainColor;
	SendConceal.wBankerUser=m_wBankerUser;
	SendConceal.wCurrentUser=m_wCurrentUser;

	//��������
	for (WORD i=0;i<m_wPlayerCount;i++)
	{
		//������Ϣ
		m_pITableFrame->SendTableData(i,SUB_S_SEND_CONCEAL,&SendConceal,sizeof(SendConceal));
		m_pITableFrame->SendLookonData(i,SUB_S_SEND_CONCEAL,&SendConceal,sizeof(SendConceal));
	}

	//����ط�
	for (WORD i=0;i<m_wPlayerCount;i++)
	{
		//��3���Ʋ�������������
		m_GameLogic.SortCardList(m_cbHandCardData[i],m_cbHandCardCount[i]);
		if(m_GameLogic.GetCardColor(m_cbHandCardData[i][2])!=0x40) continue;
		BYTE bTempCardCount=m_cbHandCardCount[i];
		BYTE bTempCard[MAX_COUNT];
		ZeroMemory(bTempCard,sizeof(bTempCard));
		CopyMemory(bTempCard,m_cbHandCardData[i],sizeof(bTempCard));
		m_GameLogic.SortCardList(bTempCard,bTempCardCount);

		//���72
		BYTE bMain7Count=0;
		BYTE bFu7Count=0;
		BYTE bMain2Count=0;
		BYTE bFu2Count=0;
		for(BYTE j=0;j<bTempCardCount;j++)
		{
			BYTE bCard=bTempCard[j];
			if(m_GameLogic.GetCardColor(bCard)==m_cbMainColor && m_GameLogic.GetCardValue(bCard)==7) bMain7Count++;
			if(m_GameLogic.GetCardColor(bCard)!=m_cbMainColor && m_GameLogic.GetCardValue(bCard)==7)
			{
				if(bTempCard[j+1]==bTempCard[j]) bFu7Count++;
			}
			if(m_GameLogic.GetCardColor(bCard)==m_cbMainColor && m_GameLogic.GetCardValue(bCard)==2) bMain2Count++;
			if(m_GameLogic.GetCardColor(bCard)!=m_cbMainColor && m_GameLogic.GetCardValue(bCard)==2) 
			{
				if(bTempCard[j+1]==bTempCard[j]) bFu2Count++;
			}
		}

		//4����
		LONGLONG lKingScore=0;
		if(bTempCard[3]==0x4E&&m_btGiveUpScore[i]==false)
		{
			m_bFourKing=true;
			if		(bMain7Count>=2 && bFu7Count>=2 && bMain2Count>=2 && bFu2Count>=2)	lKingScore=7;
			else if	(bMain7Count>=2 && bFu7Count>=2 && bMain2Count>=2 )					lKingScore=6;
			else if	(bMain7Count>=2 && bFu7Count>=2 )									lKingScore=5;
			else if	(bMain7Count>=2 )													lKingScore=4;
			else																		lKingScore=3;
		}
		else if(m_btGiveUpScore[i]==false||m_bLandScore<85)
		{		
			m_bThreeKing=true;
			if		(bMain7Count>=2 && bFu7Count>=2 && bMain2Count>=2 && bFu2Count>=2)	lKingScore=5;
			else if	(bMain7Count>=2 && bFu7Count>=2 && bMain2Count>=2 )					lKingScore=4;
			else if	(bMain7Count>=2 && bFu7Count>=2 )									lKingScore=3;
			else if	(bMain7Count>=2 )													lKingScore=2;
			else																		lKingScore=1;
		}

		m_lKingScore[(i+1)%GAME_PLAYER]=-1*lKingScore*m_pGameServiceOption->lCellScore;
		m_lKingScore[(i+2)%GAME_PLAYER]=-1*lKingScore*m_pGameServiceOption->lCellScore;
		m_lKingScore[(i+3)%GAME_PLAYER]=-1*lKingScore*m_pGameServiceOption->lCellScore;
		m_lKingScore[i]=3*lKingScore*m_pGameServiceOption->lCellScore;
	}

	return true;
}

//�����˿�
bool CTableFrameSink::OnUserConcealCard(WORD wChairID, BYTE cbConcealCard[], BYTE cbConcealCount)
{
	//Ч��״̬
	ASSERT(m_pITableFrame->GetGameStatus()==GS_UG_BACK);
	if (m_pITableFrame->GetGameStatus()!=GS_UG_BACK) return true;

	//Ч������
	ASSERT((wChairID==m_wCurrentUser)&&(m_cbConcealCount==cbConcealCount));
	if ((wChairID!=m_wCurrentUser)||(m_cbConcealCount!=cbConcealCount))
	{
		ASSERT(FALSE);
		return false;
	}

	//ɾ������
	if (m_GameLogic.RemoveCard(cbConcealCard,cbConcealCount,m_cbHandCardData[wChairID],m_cbHandCardCount[wChairID])==false)
	{
		ASSERT(FALSE);
		return false;
	}

	//�����˿�
	m_cbHandCardCount[wChairID]-=cbConcealCount;
	CopyMemory(m_cbConcealCard,cbConcealCard,cbConcealCount*sizeof(BYTE));

	//�˿�����
	for (WORD i=0;i<m_wPlayerCount;i++) 
	{
		ASSERT(m_cbHandCardCount[i]==m_GameLogic.GetDispatchCount());
		m_GameLogic.SortCardList(m_cbHandCardData[i],m_cbHandCardCount[i]);
	}

	//����״̬
	m_pITableFrame->SetGameStatus(GS_UG_PLAY);

	//��������
	CMD_S_GamePlay GamePlay;
	GamePlay.wCurrentUser=m_wCurrentUser;

	//��������
	for (WORD i=0;i<m_wPlayerCount;i++)
	{
		//�������
		if (i==m_wCurrentUser)
		{
			GamePlay.cbConcealCount=m_cbConcealCount;
			CopyMemory(GamePlay.cbConcealCard,m_cbConcealCard,sizeof(BYTE)*m_cbConcealCount);
		}
		else 
		{
			GamePlay.cbConcealCount=0;
			ZeroMemory(GamePlay.cbConcealCard,sizeof(GamePlay.cbConcealCard));
		}

		//������Ϣ
		m_pITableFrame->SendTableData(i,SUB_S_GAME_PLAY,&GamePlay,sizeof(GamePlay));
		m_pITableFrame->SendLookonData(i,SUB_S_GAME_PLAY,&GamePlay,sizeof(GamePlay));
	}

	return true;
}

//�û�����
bool CTableFrameSink::OnUserOutCard(WORD wChairID, BYTE cbCardData[], BYTE cbCardCount)
{
	//Ч��״̬
	ASSERT(m_pITableFrame->GetGameStatus()==GS_UG_PLAY);
	if (m_pITableFrame->GetGameStatus()!=GS_UG_PLAY) return true;

	//Ч���û�
	ASSERT((wChairID==m_wCurrentUser)&&(cbCardCount<=MAX_COUNT));
	if ((wChairID!=m_wCurrentUser)||(cbCardCount>MAX_COUNT))
	{
		ASSERT(FALSE);
		return false;
	}

	//�׳�����
	if (m_wFirstOutUser==wChairID)
	{
		//�����ж�
		BYTE cbCardType=m_GameLogic.GetCardType(cbCardData,cbCardCount);
		if (cbCardType==CT_ERROR)
		{
			ASSERT(FALSE);
			return false;
		}

		//˦�ƴ���
		if (cbCardType==CT_THROW_CARD)
		{
			//˦���ж�
			tagOutCardResult OutCardResult;
			ZeroMemory(&OutCardResult,sizeof(OutCardResult));
			bool bThrowSuccess=m_GameLogic.EfficacyThrowCard(cbCardData,cbCardCount,wChairID,m_cbHandCardData,m_cbHandCardCount[wChairID],OutCardResult);

			//�������
			if (bThrowSuccess==false)
			{
#ifndef DEBUG
				//֪ͨ��Ϣ
				TCHAR szMessage[128]=TEXT("");
				if(wChairID==m_wBankerUser)
					_sntprintf(szMessage,CountArray(szMessage),TEXT("ׯ��˦��ʧ�ܣ��� 10 ��"));
				else
					_sntprintf(szMessage,CountArray(szMessage),TEXT("˦��ʧ�ܣ�ׯ�Ҽ� 10 ��"));
				for (WORD i=0;i<m_wPlayerCount;i++)
				{
					IServerUserItem * pISendUserItem=m_pITableFrame->GetTableUserItem(i);
					if (pISendUserItem!=NULL) m_pITableFrame->SendGameMessage(pISendUserItem,szMessage,SMT_CHAT);
				}
#endif
				//ɾ���˿�
				if (m_GameLogic.RemoveCard(OutCardResult.cbResultCard,OutCardResult.cbCardCount,m_cbHandCardData[wChairID],m_cbHandCardCount[wChairID])==false) return false;
				m_cbHandCardCount[wChairID]-=OutCardResult.cbCardCount;

				//���Ƽ�¼
				m_cbOutCardCount[wChairID]=OutCardResult.cbCardCount;
				CopyMemory(m_cbOutCardData[wChairID],OutCardResult.cbResultCard,OutCardResult.cbCardCount);

				//�л��û�
				m_wCurrentUser=(wChairID+1)%m_wPlayerCount;

				//����۷�
				if(wChairID==m_wBankerUser)
					m_PlayerScore[m_wBankerUser]-=10;
				else
					m_PlayerScore[m_wBankerUser]+=10;

				//��������
				CMD_S_ThrowResult ThrowResult;
				ThrowResult.wOutCardUser=wChairID;
				ThrowResult.wCurrentUser=m_wCurrentUser;
				ThrowResult.cbThrowCardCount=cbCardCount;
				ThrowResult.cbResultCardCount=OutCardResult.cbCardCount;
				CopyMemory(&ThrowResult.cbCardDataArray[0],cbCardData,sizeof(BYTE)*cbCardCount);
				CopyMemory(&ThrowResult.cbCardDataArray[cbCardCount],OutCardResult.cbResultCard,sizeof(BYTE)*OutCardResult.cbCardCount);

				CopyMemory(ThrowResult.PlayerScore,m_PlayerScore,sizeof(m_PlayerScore));

				//���ͼ���
				BYTE cbCardDataCount=ThrowResult.cbThrowCardCount+ThrowResult.cbResultCardCount;
				//WORD wSendSize=sizeof(ThrowResult)-sizeof(ThrowResult.cbCardDataArray)+cbCardDataCount*sizeof(BYTE);
				WORD wSendSize=sizeof(CMD_S_ThrowResult);

				//��������
				m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_THROW_RESULT,&ThrowResult,wSendSize);
				m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_THROW_RESULT,&ThrowResult,wSendSize);

				return true;
			}
		}
	}
	else
	{
		//��Ŀ�ж�
		ASSERT(cbCardCount==m_cbOutCardCount[m_wFirstOutUser]);
		if (cbCardCount!=m_cbOutCardCount[m_wFirstOutUser]) 
		{
			ASSERT(FALSE);
			return false;
		}

		//����Ч��
		if (m_GameLogic.EfficacyOutCard(cbCardData,cbCardCount,m_cbOutCardData[m_wFirstOutUser],
			m_cbOutCardCount[m_wFirstOutUser],m_cbHandCardData[wChairID],m_cbHandCardCount[wChairID])==false)
		{
			ASSERT(FALSE);
			return false;
		}
	}

	//ɾ���˿�
	if (m_GameLogic.RemoveCard(cbCardData,cbCardCount,m_cbHandCardData[wChairID],m_cbHandCardCount[wChairID])==false)
	{
		ASSERT(FALSE);
		return false;
	}
	m_cbHandCardCount[wChairID]-=cbCardCount;

	//���Ƽ�¼
	m_cbOutCardCount[wChairID]=cbCardCount;
	CopyMemory(m_cbOutCardData[wChairID],cbCardData,cbCardCount);

	//�л��û�
	m_wCurrentUser=(wChairID+1)%m_wPlayerCount;
	if (m_wCurrentUser==m_wFirstOutUser) m_wCurrentUser=INVALID_CHAIR;

	//�������
	if(m_bUserNoMain[wChairID]==false && m_GameLogic.GetCardLogicColor(m_cbHandCardData[wChairID][0])!=COLOR_NT)
	{
		m_bUserNoMain[wChairID]=true;

		IServerUserItem *pUserItem=m_pITableFrame->GetTableUserItem(wChairID);
		//������Ϣ
		CString szEndMessage;
		szEndMessage.Format(TEXT("[ %s ]�� �����ơ�"),pUserItem!=NULL?pUserItem->GetNickName():TEXT("��ȡ�û���ʧ��"));
#ifndef DEBUG
		//������Ϣ
		m_pITableFrame->SendGameMessage(szEndMessage,SMT_CHAT);
#endif
	}	
	//��������
	BYTE bNoMainCount=0;
	for(BYTE i=0;i<GAME_PLAYER;i++) if(m_bUserNoMain[i]==true) bNoMainCount++;
	BYTE bUserMainCount[GAME_PLAYER];
	BYTE bDoubleCount[GAME_PLAYER];
	ZeroMemory(bUserMainCount,sizeof(bUserMainCount));
	ZeroMemory(bDoubleCount,sizeof(bDoubleCount));
	//if(m_bSendNoMain==false)
	{
		if(bNoMainCount>=2)
		{
			m_bSendNoMain=true;
			//��ϸ��Ϣ
			TCHAR szMessage[2048]=TEXT("");
			for(WORD i=0;i<GAME_PLAYER;i++)
			{
				//��������
				BYTE bMainCount=0;
				for(BYTE j=0;j<m_cbHandCardCount[i];j++) if(m_GameLogic.GetCardLogicColor(m_cbHandCardData[i][j])==COLOR_NT) bMainCount++;
				//���Ӹ���
				BYTE bDuiZiCount=0;
				for(BYTE k=0;k<m_cbHandCardCount[i]-1;k++) if(m_cbHandCardData[i][k]==m_cbHandCardData[i][k+1]&&m_GameLogic.GetCardLogicColor(m_cbHandCardData[i][k])==COLOR_NT) bDuiZiCount++;

				bUserMainCount[i]=bMainCount;
				bDoubleCount[i]=bDuiZiCount;
				TCHAR szBuffer[512]=TEXT("");
				_sntprintf(szBuffer,CountArray(szBuffer),TEXT("\n[ %s ]�� ���� %d �����ƣ�%d �����ӡ�"),m_pITableFrame->GetTableUserItem(i)->GetNickName(),bMainCount,bDuiZiCount);

				lstrcat(szMessage,szBuffer);
			}	
#ifndef DEBUG
			//������Ϣ
			for (WORD n=0;n<GAME_PLAYER;n++) m_pITableFrame->SendGameMessage(m_pITableFrame->GetTableUserItem(n),szMessage,SMT_CHAT);	
#endif
		}
	}
	if(bNoMainCount==3) m_GameLogic.SetCanThrow(true);

	//��������
	CMD_S_OutCard OutCard;
	if(m_cbHandCardCount[wChairID]==0) OutCard.bLastTurn=true;
	else OutCard.bLastTurn=false;
	if(m_wFirstOutUser==wChairID)	OutCard.bFirstOut=true;
	else OutCard.bFirstOut=false;
	OutCard.wOutCardUser=wChairID;
	OutCard.cbCardCount=cbCardCount;
	OutCard.wCurrentUser=m_wCurrentUser;
	CopyMemory(OutCard.cbCardData,cbCardData,cbCardCount*sizeof(BYTE));
	CopyMemory(OutCard.bNoMainUser,m_bUserNoMain,sizeof(m_bUserNoMain));
	CopyMemory(OutCard.bMainCount,bUserMainCount,sizeof(bUserMainCount));
	CopyMemory(OutCard.bDoubleCount,bDoubleCount,sizeof(bDoubleCount));

	//��������
	WORD wSendSize=sizeof(CMD_S_OutCard);
	m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_OUT_CARD,&OutCard,wSendSize);
	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_OUT_CARD,&OutCard,wSendSize);

	//һ�ֽ���
	if (m_wCurrentUser==INVALID_CHAIR)
	{
		//�ж�ʤ��
		m_wTurnWinner=m_GameLogic.CompareCardArray(m_cbOutCardData,m_cbOutCardCount[0],m_wFirstOutUser);

		//����÷�
		if ((m_wTurnWinner!=m_wBankerUser)/*&&((m_wTurnWinner+2)%m_wPlayerCount!=m_wBankerUser)*/)
		{
			for (WORD i=0;i<m_wPlayerCount;i++)
			{
				BYTE * pcbScoreCard=&m_cbScoreCardData[m_cbScoreCardCount];
				BYTE cbMaxCount=CountArray(m_cbScoreCardData)-m_cbScoreCardCount;
				m_cbScoreCardCount+=m_GameLogic.GetScoreCard(m_cbOutCardData[i],m_cbOutCardCount[i],pcbScoreCard,cbMaxCount);
			}
		}

		//����ʤ���ߵ÷�
		for (WORD i=0;i<m_wPlayerCount;i++)
		{
			m_PlayerScore[m_wTurnWinner]+=m_GameLogic.GetCardScore(m_cbOutCardData[i],m_cbOutCardCount[i]);
		}

		//���ñ���
		if (m_cbHandCardCount[m_wTurnWinner]!=0)
		{
			m_wFirstOutUser=m_wTurnWinner;
			ZeroMemory(m_cbOutCardData,sizeof(m_cbOutCardData));
			ZeroMemory(m_cbOutCardCount,sizeof(m_cbOutCardCount));
		}

		//�û��л�
		m_wCurrentUser=(m_cbHandCardCount[m_wTurnWinner]==0)?INVALID_CHAIR:m_wTurnWinner;

		//��������
		CMD_TurnBalance TurnBalance;
		TurnBalance.wTurnWinner=m_wTurnWinner;
		TurnBalance.wCurrentUser=m_wCurrentUser;

		CopyMemory(TurnBalance.PlayerScore,m_PlayerScore,sizeof(m_PlayerScore));

		//��������
		m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_TURN_BALANCE,&TurnBalance,sizeof(TurnBalance));
		m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_TURN_BALANCE,&TurnBalance,sizeof(TurnBalance));
	}

	//�����ж�
	if (m_wCurrentUser==INVALID_CHAIR)
	{
		try{
			OnEventGameConclude(m_wTurnWinner,NULL,GER_NORMAL);
		}catch(...)
		{
			ASSERT(0);
		}
	}

	return true;
}

//�ɷ��˿�
bool CTableFrameSink::DispatchUserCard()
{
	//�����˿�
	BYTE cbRandCard[CELL_PACK*MAX_PACK];
	m_GameLogic.RandCardList(cbRandCard,CELL_PACK*m_cbPackCount);

	//�û��˿�
	for (WORD i=0;i<m_wPlayerCount;i++)
	{
		m_cbHandCardCount[i]=m_GameLogic.GetDispatchCount();
		CopyMemory(&m_cbHandCardData[i],&cbRandCard[i*m_cbHandCardCount[i]],sizeof(BYTE)*m_cbHandCardCount[i]);
	}

	//ָ������
	//#ifdef _DEBUG
	//BYTE test1[25]={0x4F,0x4F,0x4E,0x4E,0x17,0x17,0x02,0x02,0x12,0x12,0x22,0x22,0x32,0x32,0x27,0x27,0x27,0x27,0x27,0x27,0x26,0x26,0x25,0x25,0x27};
	//BYTE test2[25]={0x16,0x16,0x17,0x17,0x18,0x18,0x06,0x06,0x07,0x07,0x08,0x08,0x26,0x26,0x27,0x27,0x28,0x28,0x16,0x16,0x17,0x17,0x18,0x18,0x18};
	//BYTE test3[25]={0x16,0x16,0x17,0x17,0x18,0x18,0x16,0x16,0x17,0x17,0x08,0x08,0x26,0x26,0x27,0x27,0x28,0x28,0x16,0x16,0x17,0x17,0x18,0x18,0x18};
	//BYTE test4[25]={0x16,0x16,0x17,0x17,0x18,0x18,0x16,0x16,0x17,0x17,0x08,0x08,0x26,0x26,0x27,0x27,0x18,0x18,0x16,0x16,0x17,0x17,0x18,0x18,0x18};
	//CopyMemory(m_cbHandCardData[0],test1,sizeof(m_cbHandCardData[1]));
	//CopyMemory(m_cbHandCardData[1],test2,sizeof(m_cbHandCardData[2]));
	//CopyMemory(m_cbHandCardData[2],test3,sizeof(m_cbHandCardData[3]));
	//CopyMemory(m_cbHandCardData[3],test4,sizeof(m_cbHandCardData[4]));
	//#endif

	//�����˿�
	m_cbConcealCount=CELL_PACK*m_cbPackCount-m_wPlayerCount*m_GameLogic.GetDispatchCount();
	CopyMemory(m_cbConcealCard,&cbRandCard[CELL_PACK*m_cbPackCount-m_cbConcealCount],sizeof(BYTE)*m_cbConcealCount);
	m_GameLogic.SortCardList(m_cbConcealCard,m_cbConcealCount);

	//���Ϳ�ʼ
	CMD_S_SendCard SendCard;
	SendCard.wCurrentUser=m_wCurrentUser;
	CopyMemory(SendCard.cbBackCard,m_cbConcealCard,sizeof(m_cbConcealCard));
	for (WORD i=0;i<m_wPlayerCount;i++)
	{
		//Ч��״̬
		ASSERT(m_cbHandCardCount[i]<=CountArray(SendCard.cbCardData));
		if (m_cbHandCardCount[i]>CountArray(SendCard.cbCardData)) 
		{
			ASSERT(FALSE);
			return false;
		}

		//�����˿�
		SendCard.cbCardCount=m_cbHandCardCount[i];
		m_GameLogic.SortCardList(m_cbHandCardData[i],m_cbHandCardCount[i]);
		CopyMemory(SendCard.cbCardData,m_cbHandCardData[i],sizeof(BYTE)*m_cbHandCardCount[i]);

		//����ͳ��
		//WORD wHeadSize=sizeof(SendCard)-sizeof(SendCard.cbCardData);
		//WORD wSendSize=wHeadSize+sizeof(SendCard.cbCardData[0])*SendCard.cbCardCount;
		WORD wSendSize=sizeof(SendCard);

		//��������
		m_pITableFrame->SendTableData(i,SUB_S_SEND_CARD,&SendCard,wSendSize);
		m_pITableFrame->SendLookonData(i,SUB_S_SEND_CARD,&SendCard,wSendSize);
	}

	return true;
}

//�з��¼�
bool CTableFrameSink::OnUserLandScore(WORD wChairID, WORD bLandScore)
{
	//Ч��״̬
	if (m_pITableFrame->GetGameStatus()!=GS_UG_SCORE) return true;
	if (wChairID!=m_wCurrentUser)
	{
		ASSERT(FALSE);
		return false;
	}

	//Ч�����
	if (bLandScore>=m_bLandScore&&bLandScore!=255)
	{
		ASSERT(FALSE);
		return false;
	}

	//���ñ���
	if (bLandScore!=255&&bLandScore<m_bLandScore)
	{
		m_bLandScore=bLandScore;
		m_wCurrentUser=wChairID;
		m_wBankerUser=m_wCurrentUser;
	}


	//�����з�
	if(bLandScore==255) m_btGiveUpScore[wChairID]=true;
	m_wUserScore[wChairID]=bLandScore;

	//����߽з�
	if(bLandScore!=255 && m_lKingScore[wChairID]>0) m_bKingCall=true;
	if(bLandScore==255 && m_bLandScore<85 &&  m_lKingScore[wChairID]>0 && m_bFourKing==false) m_bKingCall=true;

	//��ʼ�ж�
	BYTE GiveUpPlayer=0;
	for(BYTE i=0;i<GAME_PLAYER;i++)
	{
		if(m_btGiveUpScore[i]==true) GiveUpPlayer++;
	}
	if ((m_bLandScore==0)||GiveUpPlayer==4||(GiveUpPlayer==3&&m_bLandScore<120))
	{
		//�����δ�з�
		if(m_bKingCall==false) ZeroMemory(m_lKingScore,sizeof(m_lKingScore));

		//���ñ���
		if (m_bLandScore==255) m_bLandScore=120;
		if (m_wBankerUser==INVALID_CHAIR) m_wBankerUser=m_wFirstUser;

		//����״̬
		m_pITableFrame->SetGameStatus(GS_UG_CALL);

		//������Ϣ
		m_wCurrentUser=m_wBankerUser;

		for(WORD i=0;i<GAME_PLAYER;i++)
		{
			//��������
			CMD_S_GameStart GameStart;
			GameStart.bLandScore=m_bLandScore;
			GameStart.wBankerUser=m_wBankerUser;
			GameStart.cbPackCount=m_cbPackCount;
			GameStart.cbMainValue=m_cbMainValue;
			GameStart.cbValueOrder[0]=m_cbValueOrder[0];
			GameStart.cbValueOrder[1]=m_cbValueOrder[1];

			//�������
			if (i==m_wCurrentUser)
			{
				GameStart.cbConcealCount=m_cbConcealCount;
				CopyMemory(GameStart.cbConcealCard,m_cbConcealCard,sizeof(BYTE)*m_cbConcealCount);
			}
			else 
			{
				GameStart.cbConcealCount=0;
				ZeroMemory(GameStart.cbConcealCard,sizeof(GameStart.cbConcealCard));
			}

			//��������
			m_pITableFrame->SendTableData(i,SUB_S_GAME_START,&GameStart,sizeof(GameStart));
			m_pITableFrame->SendLookonData(i,SUB_S_GAME_START,&GameStart,sizeof(GameStart));
		}
		return true;
	}

	//�����û�
	m_wCurrentUser=(wChairID+1)%m_wPlayerCount;

	//��������������
	if(m_btGiveUpScore[m_wCurrentUser]==true)	m_wCurrentUser=(m_wCurrentUser+1)%m_wPlayerCount;
	if(m_btGiveUpScore[m_wCurrentUser]==true)	m_wCurrentUser=(m_wCurrentUser+1)%m_wPlayerCount;

	//������Ϣ
	CMD_S_LandScore LandScore;
	LandScore.bLandUser=wChairID;
	LandScore.bLandScore=m_bLandScore;
	LandScore.wCurrentUser=m_wCurrentUser;
	LandScore.bCurrentScore=m_bLandScore;
	CopyMemory(LandScore.wUserScore,m_wUserScore,sizeof(m_wUserScore));
	CopyMemory(LandScore.bGiveUpScore,m_btGiveUpScore,sizeof(m_btGiveUpScore));
	m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_LAND_SCORE,&LandScore,sizeof(LandScore));
	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_LAND_SCORE,&LandScore,sizeof(LandScore));

	return true;
}

//////////////////////////////////////////////////////////////////////////////////
