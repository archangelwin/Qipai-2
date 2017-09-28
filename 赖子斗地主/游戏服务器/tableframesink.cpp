#include "StdAfx.h"
#include "TableFrameSink.h"

//////////////////////////////////////////////////////////////////////////

//��̬����
const WORD			CTableFrameSink::m_wPlayerCount=GAME_PLAYER;			//��Ϸ����
const BYTE			CTableFrameSink::m_GameStartMode=START_MODE_FULL_READY;	//��ʼģʽ

//�궨��
#define	GOOD_CARD_PERCENT				4									//���Ƹ���

//////////////////////////////////////////////////////////////////////////

//���캯��
CTableFrameSink::CTableFrameSink()
{
	//��Ϸ����
	m_wBombTime=1;
	m_bLandScore=0;
	m_wFirstUser=INVALID_CHAIR;
	m_wBankerUser=INVALID_CHAIR;
	m_wCurrentUser=INVALID_CHAIR;
	m_lAddScore=8000;
	m_wBomb=1;
	m_wQiangLandDouble=1;
	memset(m_bScoreInfo,0,sizeof(m_bScoreInfo));
	memset(m_bMingCardStatus,0,sizeof(m_bMingCardStatus));
	memset(m_bQiangLand,false,sizeof(m_bQiangLand));
	memset(m_bUserDouble,1,sizeof(m_bUserDouble));
	memset(m_bAddScore,false,sizeof(m_bAddScore));
	memset(m_wPlayTime,0,sizeof(m_wPlayTime));


	//������Ϣ
	m_bTurnCardCount=0;
	m_wTurnWiner=INVALID_CHAIR;
	memset(m_bTurnCardData,0,sizeof(m_bTurnCardData));
	memset(m_bUserTrustee, 0, sizeof( m_bUserTrustee ) );

	//�˿���Ϣ
	m_bLaiZiCard=0;
	memset(m_bBackCard,0,sizeof(m_bBackCard));
	memset(m_bCardCount,0,sizeof(m_bCardCount));
	memset(m_bOutCardCount,0,sizeof(m_bOutCardCount));
	memset(m_bHandCardData,0,sizeof(m_bHandCardData));
	
	//�������
	m_pITableFrame=NULL;
	m_pGameServiceOption=NULL;

	return;
}

//��������
CTableFrameSink::~CTableFrameSink(void)
{
}

//�ӿڲ�ѯ
void *  CTableFrameSink::QueryInterface(const IID & Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(ITableFrameSink,Guid,dwQueryVer);
		QUERYINTERFACE(ITableUserAction,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(ITableFrameSink,Guid,dwQueryVer);
	return NULL;
}

//��ʼ��
bool  CTableFrameSink::Initialization(IUnknownEx * pIUnknownEx)
{
	//��ѯ�ӿ�
	ASSERT(pIUnknownEx!=NULL);
	m_pITableFrame=QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx,ITableFrame);
	if (m_pITableFrame==NULL) return false;

	//��ʼģʽ
	m_pITableFrame->SetStartMode(START_MODE_FULL_READY);

	//��ȡ����
	m_pGameServiceOption=m_pITableFrame->GetGameServiceOption();
	ASSERT(m_pGameServiceOption!=NULL);

	return true;
}
//�û�����
bool  CTableFrameSink::OnActionUserSitDown(WORD wChairID,IServerUserItem * pIServerUserItem, bool bLookonUser)
{
	//��ʷ����
	if (bLookonUser==false) m_HistoryScore.OnEventUserEnter(wChairID);

	return true;
}

//�û�����
bool  CTableFrameSink::OnActionUserStandUp(WORD wChairID,IServerUserItem * pIServerUserItem, bool bLookonUser)
{
	//��ʷ����
	if (bLookonUser==false) m_HistoryScore.OnEventUserLeave(wChairID);
	if (bLookonUser==false) m_wPlayTime[wChairID]=0;

	return true;
}
//��λ����
void  CTableFrameSink::RepositionSink()
{
	//��Ϸ����
	m_wBombTime=1;
	m_bLandScore=0;
	m_wBankerUser=INVALID_CHAIR;
	m_wCurrentUser=INVALID_CHAIR;
	m_lAddScore=8000;
	m_wBomb=1;
	m_wQiangLandDouble=1;
	memset(m_bScoreInfo,0,sizeof(m_bScoreInfo));
	//
	memset(m_bMingCardStatus,0,sizeof(m_bMingCardStatus));
	memset(m_bQiangLand,false,sizeof(m_bQiangLand));
	memset(m_bUserDouble,1,sizeof(m_bUserDouble));
	memset(m_bAddScore,false,sizeof(m_bAddScore));

	//������Ϣ
	m_bTurnCardCount=0;
	m_wTurnWiner=INVALID_CHAIR;
	memset(m_bTurnCardData,0,sizeof(m_bTurnCardData));
	memset(m_bUserTrustee, 0, sizeof( m_bUserTrustee ) );

	//�˿���Ϣ
	m_bLaiZiCard=0;
	memset(m_bBackCard,0,sizeof(m_bBackCard));
	memset(m_bCardCount,0,sizeof(m_bCardCount));
	memset(m_bOutCardCount,0,sizeof(m_bOutCardCount));
	memset(m_bHandCardData,0,sizeof(m_bHandCardData));

	return;
}

//��ʼģʽ
BYTE  CTableFrameSink::GetGameStartMode()
{
	return m_GameStartMode;
}

//��Ϸ״̬
bool  CTableFrameSink::IsUserPlaying(WORD wChairID)
{
	return true;
}

//��Ϸ��ʼ
bool  CTableFrameSink::OnEventGameStart()
{
	//����״̬
	m_pITableFrame->SetGameStatus(GS_WK_SCORE);

	BYTE cbSendGoodCard = rand () % GOOD_CARD_PERCENT ;
	BYTE cbAndroidUserCount = 0 ;
	WORD wAndroidUser[ GAME_PLAYER ] = {0};

	//�����˸���
	for ( WORD wChairID = 0; wChairID < GAME_PLAYER; ++wChairID )
	{
		IServerUserItem *pServerUserItem = m_pITableFrame->GetTableUserItem( wChairID ) ;
		if ( pServerUserItem->IsAndroidUser() )
		{
			wAndroidUser[ cbAndroidUserCount ] = ( wChairID ) ;
			++cbAndroidUserCount ;
		}
	}

	WORD wHaveGoodCardAndroidUser = INVALID_CHAIR ;
	if ( 0 < cbAndroidUserCount ) wHaveGoodCardAndroidUser = wAndroidUser[ rand() % cbAndroidUserCount ] ;

	//���ͺ���
	if ( 1 == cbSendGoodCard && wHaveGoodCardAndroidUser != INVALID_CHAIR )
	{
		//�����˿�
		BYTE bRandCard[54];
		m_GameLogic.RandCardList( bRandCard, sizeof( bRandCard ) / sizeof( bRandCard[ 0 ] ) );

		BYTE cbGoodCard[ NORMAL_COUNT ] ;
		m_GameLogic.GetGoodCardData( cbGoodCard ) ;

		//��ȡ����
		m_GameLogic.RemoveGoodCardData( cbGoodCard, NORMAL_COUNT, bRandCard, FULL_COUNT ) ;

		//�ַ��˿�
		CopyMemory( m_bHandCardData[ wHaveGoodCardAndroidUser ], cbGoodCard, NORMAL_COUNT ) ;
		m_bCardCount[ wHaveGoodCardAndroidUser ] = 17;
		for ( WORD i = 0, j = 0; i < m_wPlayerCount; i++ )
		{
			if ( i != wHaveGoodCardAndroidUser )
			{
				m_bCardCount[ i ] = 17;
				CopyMemory( &m_bHandCardData[ i ], &bRandCard[ j * 17 ], sizeof( BYTE ) * 17 );
				++j ;
			}
		}
		CopyMemory( m_bBackCard, &bRandCard[ 34 ], sizeof( m_bBackCard ) );
		m_GameLogic.SortCardList( m_bBackCard, 3, ST_ORDER );
	}
	else
	{
		//�����˿�
		BYTE bRandCard[ 54 ];
		m_GameLogic.RandCardList( bRandCard, sizeof( bRandCard ) / sizeof( bRandCard[ 0 ] ) );

		//�ַ��˿�
		for ( WORD i = 0; i < m_wPlayerCount; i++ )
		{
			m_bCardCount[ i ] = 17;
			CopyMemory( &m_bHandCardData[ i ], &bRandCard[ i * m_bCardCount[ i ] ], sizeof( BYTE ) * m_bCardCount[ i ] );
		}
		CopyMemory( m_bBackCard, &bRandCard[ 51 ], sizeof( m_bBackCard ) );
		m_GameLogic.SortCardList( m_bBackCard, 3, ST_ORDER );
	}

	//�����û�
	m_wCurrentUser = rand() % GAME_PLAYER;
	m_wFirstUser=m_wCurrentUser;

	//�����˿�
	CMD_S_SendAllCard SendAllCard ;
	SendAllCard.wCurrentUser = m_wCurrentUser ;

	//text
	//BYTE bCardData[12]={0x0d,0x2d,0x3c,0x0c,0x1c,0x26,0x06,0x18,0x28,0x03,0x04,0x06};
	//CopyMemory(m_bHandCardData[0],bCardData,sizeof(BYTE)*12);
	//
	//�����˿�
	CopyMemory( SendAllCard.bCardData, m_bHandCardData, sizeof( m_bHandCardData ) ) ;
	//��������
	CopyMemory( SendAllCard.bMingCard, m_bMingCardStatus, sizeof( m_bMingCardStatus ) ) ;

	//������
	m_bLaiZiCard=m_GameLogic.GetLaiZiCard();
	SendAllCard.bLaiZiCard=m_bLaiZiCard;
	SendAllCard.wBombTime=m_wBombTime;

	//�����˿�
	CopyMemory( SendAllCard.bBackCardData, m_bBackCard, 3 ) ;
	for ( WORD i = 0; i < m_wPlayerCount; i++ )
	{
		m_pITableFrame->SendTableData( i, SUB_S_SEND_CARD, &SendAllCard, sizeof( SendAllCard ) );
		m_pITableFrame->SendLookonData( i, SUB_S_SEND_CARD, &SendAllCard, sizeof( SendAllCard ) );
	}

	//�����˿�
	for ( WORD i = 0; i < m_wPlayerCount; i++ )
	{
		m_GameLogic.SortCardList( m_bHandCardData[ i ], m_bCardCount[ i ], ST_ORDER );
	}

	return true;
}

//��Ϸ����
bool  CTableFrameSink::OnEventGameConclude(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason)
{
	switch ( cbReason )
	{
	case GER_DISMISS:		//��Ϸ��ɢ
		{
			//Ч�����
			ASSERT( pIServerUserItem != NULL );
			ASSERT( wChairID < m_wPlayerCount );

			//��������
			CMD_S_GameEnd GameEnd;
			memset( &GameEnd, 0, sizeof( GameEnd ) );

			//ʣ���˿�
			BYTE bCardPos = 0;
			for ( WORD i = 0; i < m_wPlayerCount; i++ )
			{
				GameEnd.bCardCount[ i ] = m_bCardCount[ i ];
				CopyMemory( &GameEnd.bCardData[ bCardPos ], m_bHandCardData[ i ], m_bCardCount[ i ] * sizeof( BYTE ) );
				bCardPos += m_bCardCount[ i ];
			}

			//������Ϣ
			m_pITableFrame->SendTableData( INVALID_CHAIR, SUB_S_GAME_END, &GameEnd, sizeof( GameEnd ) );
			m_pITableFrame->SendLookonData( INVALID_CHAIR, SUB_S_GAME_END, &GameEnd, sizeof( GameEnd ) );

			//������Ϸ
			m_pITableFrame->ConcludeGame(GS_WK_FREE);

			return true;
		}
	case GER_NORMAL:		//�������
		{
			//�������
			CMD_S_GameEnd GameEnd;
			ZeroMemory( &GameEnd, sizeof( GameEnd ) );

			//ʣ���˿�
			BYTE bCardPos = 0;
			for ( WORD i = 0; i < m_wPlayerCount; i++ )
			{
				GameEnd.bCardCount[ i ] = m_bCardCount[ i ];
				CopyMemory( &GameEnd.bCardData[ bCardPos ], m_bHandCardData[ i ], m_bCardCount[ i ] * sizeof( BYTE ) );
				bCardPos += m_bCardCount[ i ];
			}

			//��������
			LONGLONG lCellScore = m_pGameServiceOption->lCellScore;
			bool bLandWin = ( m_bCardCount[ m_wBankerUser ] == 0 ) ? true : false;

			//�����ж�
			if ( wChairID == m_wBankerUser )
			{
				WORD wUser1 = ( m_wBankerUser + 1 ) % GAME_PLAYER;
				WORD wUser2 = ( m_wBankerUser + 2 ) % GAME_PLAYER;
				if ( ( m_bOutCardCount[ wUser1 ] == 0 ) && ( m_bOutCardCount[ wUser2 ] == 0 ) )
				{
					m_wBombTime *= 2;
					GameEnd.iOtherAdd[4]=1;
				}
				else
					GameEnd.iOtherAdd[4]=2;
			}
			else
			{
				if ( m_bOutCardCount[ m_wBankerUser ] == 1 ) m_wBombTime *= 2;
			}

			//�ӱ�����
			BYTE bUserDouble=1;
			for(BYTE i=0;i<GAME_PLAYER;i++)
			{
				bUserDouble*=m_bUserDouble[i];
			}

			//��Ϸ����
			LONGLONG lScore=0;
			LONGLONG lRevenue=0;
			tagScoreInfo ScoreInfoArray[GAME_PLAYER];
			ZeroMemory(&ScoreInfoArray,sizeof(ScoreInfoArray));

			m_bLandScore=__max(m_bLandScore,1);

			//ͳ�ƻ���
			for ( WORD i = 0; i < m_wPlayerCount; i++ )
			{

				lScore=0;
				lRevenue=0;

				//ͳ�ƻ���
				if ( i == m_wBankerUser )
				{
					lScore = m_wBombTime * m_bLandScore * lCellScore * bUserDouble*( ( bLandWin == true ) ? 2 : -2 );
					GameEnd.lGameScore[ i ] = lScore;
					ScoreInfoArray[i].lScore=GameEnd.lGameScore[i];
				}
				else 
				{
					lScore = m_wBombTime * m_bLandScore * lCellScore *  bUserDouble *( ( bLandWin == true ) ? -1 : 1 );

					GameEnd.lGameScore[ i ] = lScore;
					ScoreInfoArray[i].lScore=GameEnd.lGameScore[i];
				}

				//ʤ������
				ScoreInfoArray[i].cbType=(GameEnd.lGameScore[i]>0L)?SCORE_TYPE_WIN:SCORE_TYPE_LOSE;

				//����˰��
				if (m_pGameServiceOption->wServerType==GAME_GENRE_GOLD)
				{
					if (GameEnd.lGameScore[i]>=100L)
					{
						//����˰��
						GameEnd.lGameTax+= m_pITableFrame->CalculateRevenue(i,GameEnd.lGameScore[i]);
						lRevenue = m_pITableFrame->CalculateRevenue(i,GameEnd.lGameScore[i]);
						ScoreInfoArray[i].lRevenue=lRevenue;

						//���ֵ���
						lScore=lScore-lRevenue;
						GameEnd.lGameScore[i]=GameEnd.lGameScore[i]-lRevenue;
						ScoreInfoArray[i].lScore=GameEnd.lGameScore[i];
					}
				}
				//��ʷ����
				//m_HistoryScore.OnEventUserScore(i,GameEnd.lGameScore[i]);
			}

			//���ִ���
			if (m_pGameServiceOption->wServerType==GAME_GENRE_GOLD)
			{
				SCORE lScore=0l;
				for(BYTE i=0;i<GAME_PLAYER;i++)
				{
					IServerUserItem * pIServerUser=m_pITableFrame->GetTableUserItem(i);
					if(GameEnd.lGameScore[i] < 0 && pIServerUser->GetUserScore() < -GameEnd.lGameScore[i])
					{
						if(lScore < pIServerUser->GetUserScore() || lScore == 0)
							lScore = pIServerUser->GetUserScore();
					}
				}

				if(lScore != 0)
				{
					if(GameEnd.lGameScore[m_wBankerUser] < 0)
					{
						GameEnd.lGameScore[m_wBankerUser]=-(lScore);
						GameEnd.lGameScore[(m_wBankerUser+1)%GAME_PLAYER]=(lScore/2);
						GameEnd.lGameScore[(m_wBankerUser+2)%GAME_PLAYER]=(lScore/2);

						ScoreInfoArray[m_wBankerUser].lScore=GameEnd.lGameScore[m_wBankerUser];
						ScoreInfoArray[(m_wBankerUser+1)%GAME_PLAYER].lScore=GameEnd.lGameScore[(m_wBankerUser+1)%GAME_PLAYER];
						ScoreInfoArray[(m_wBankerUser+2)%GAME_PLAYER].lScore=GameEnd.lGameScore[(m_wBankerUser+2)%GAME_PLAYER];
					}
					else
					{
						GameEnd.lGameScore[m_wBankerUser]=(lScore*2);
						GameEnd.lGameScore[(m_wBankerUser+1)%GAME_PLAYER]=-(lScore);
						GameEnd.lGameScore[(m_wBankerUser+2)%GAME_PLAYER]=-(lScore);

						ScoreInfoArray[m_wBankerUser].lScore=GameEnd.lGameScore[m_wBankerUser];
						ScoreInfoArray[(m_wBankerUser+1)%GAME_PLAYER].lScore=GameEnd.lGameScore[(m_wBankerUser+1)%GAME_PLAYER];
						ScoreInfoArray[(m_wBankerUser+2)%GAME_PLAYER].lScore=GameEnd.lGameScore[(m_wBankerUser+2)%GAME_PLAYER];
					}

					for(BYTE i=0;i<GAME_PLAYER;i++)
					{
						if (GameEnd.lGameScore[i]>=100L)
						{
							//����˰��
							GameEnd.lGameTax+= m_pITableFrame->CalculateRevenue(i,GameEnd.lGameScore[i]);
							lRevenue = m_pITableFrame->CalculateRevenue(i,GameEnd.lGameScore[i]);
							ScoreInfoArray[i].lRevenue=lRevenue;

							//���ֵ���
							lScore=lScore-lRevenue;
							GameEnd.lGameScore[i]=GameEnd.lGameScore[i]-lRevenue;
							ScoreInfoArray[i].lScore=GameEnd.lGameScore[i];
						}
					}
				}
			}

			for(BYTE i=0;i<GAME_PLAYER;i++)
			{
				//��ʷ����
				m_HistoryScore.OnEventUserScore(i,GameEnd.lGameScore[i]);
			}

			//�޸ķ���
			m_pITableFrame->WriteTableScore(ScoreInfoArray,CountArray(ScoreInfoArray));


			GameEnd.iOtherAdd[0]=m_wBombTime;
			GameEnd.iOtherAdd[2]=m_wQiangLandDouble;
			GameEnd.iOtherAdd[3]=m_wBomb;
			CopyMemory(GameEnd.bUserAdd,m_bUserDouble,sizeof(m_bUserDouble));

			if(GameEnd.iOtherAdd[4] == 0)
				GameEnd.iOtherAdd[4]=1;

			//���Ʊ���
			BYTE bMingCard=0;
			for(BYTE i=0;i<CountArray(m_bMingCardStatus);i++)
			{
				if(m_bMingCardStatus[i] < bMingCard)
					bMingCard=m_bMingCardStatus[i];
			}
			if(bMingCard == 3)
				GameEnd.iOtherAdd[1]=2;
			else if(bMingCard == 2)
				GameEnd.iOtherAdd[1]=4;
			else if(bMingCard == 1)
				GameEnd.iOtherAdd[1]=5;
			else
				GameEnd.iOtherAdd[1]=1;

			//����
			for(BYTE i=0;i<GAME_PLAYER;i++)
			{
				if(m_pITableFrame->GetTableUserItem(i) != NULL)
				{
					m_wPlayTime[i]++;
					GameEnd.wPlayTime[i]=m_wPlayTime[i];
				}
				else
				{
					m_wPlayTime[i]=0;
					GameEnd.wPlayTime[i]=m_wPlayTime[i];
				}
			}

			//��ʷ����
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				tagHistoryScore * pHistoryScore=m_HistoryScore.GetHistoryScore(i);
				GameEnd.lGameResult[i]=pHistoryScore->lCollectScore;
			}

			//������Ϣ
			m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
			m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));

			//�л��û�
			m_wFirstUser=wChairID;

			//������Ϸ
			m_pITableFrame->ConcludeGame(GS_WK_FREE);

			return true;
		}
	case GER_USER_LEAVE:	//�û�ǿ��
	case GER_NETWORK_ERROR:	//�����ж�
		{
			//Ч�����
			ASSERT(pIServerUserItem!=NULL);
			ASSERT(wChairID<m_wPlayerCount);

			//��������
			CMD_S_GameEnd GameEnd;
			ZeroMemory(&GameEnd,sizeof(GameEnd));

			//ʣ���˿�
			BYTE bCardPos=0;
			for (WORD i=0;i<m_wPlayerCount;i++)
			{
				GameEnd.bCardCount[i]=m_bCardCount[i];
				CopyMemory(&GameEnd.bCardData[bCardPos],m_bHandCardData[i],m_bCardCount[i]*sizeof(BYTE));
				bCardPos+=m_bCardCount[i];
			}

			//ը������
			m_wBombTime=__min(m_wBombTime,16);
			m_bLandScore=__max(m_bLandScore,1);

			//�޸Ļ���
			tagScoreInfo ScoreInfoArray[GAME_PLAYER];
			ZeroMemory(&ScoreInfoArray,sizeof(ScoreInfoArray));

			for (WORD i=0;i<m_wPlayerCount;i++)
			{
				//��Ϸ����
				LONGLONG lScore=0;
				LONGLONG lRevenue=0;
				ScoreInfoArray[i].cbType=SCORE_TYPE_DRAW;


				//�������
				if (i==wChairID)
				{
					ScoreInfoArray[i].cbType=SCORE_TYPE_FLEE;
					lScore	 =-m_wBombTime*m_bLandScore*m_pGameServiceOption->lCellScore*2;
					GameEnd.lGameScore[i]=-m_wBombTime*m_bLandScore*m_pGameServiceOption->lCellScore*2;
					ScoreInfoArray[i].lScore=GameEnd.lGameScore[i];
				}
				else if (m_pGameServiceOption->wServerType==GAME_GENRE_GOLD)
				{
					//ͳ�ƻ���
					//ScoreKind=SCORE_TYPE_WIN;
					//lScore=(m_wBombTime*m_bLandScore*m_pGameServiceOption->lCellScore*4)/2;
					//GameEnd.lGameScore[i]=(m_wBombTime*m_bLandScore*m_pGameServiceOption->lCellScore*4)/2;

					////����˰��
					//if (lScore>=100L)
					//{
					//	//����˰��
					//	GameEnd.lGameTax+=GameEnd.lGameScore[i]*m_pGameServiceOption->cbRevenue/100L;
					//	lRevenue = GameEnd.lGameScore[i]*m_pGameServiceOption->cbRevenue/100L;

					//	//���ֵ���
					//	lScore=lScore-lRevenue ;
					//	GameEnd.lGameScore[i]=GameEnd.lGameScore[i]-lRevenue ;
					//}
				}

				//��ʷ����
				m_HistoryScore.OnEventUserScore(i,GameEnd.lGameScore[i]);
			}
			//���ִ���
			if (m_pGameServiceOption->wServerType==GAME_GENRE_GOLD)
			{
				SCORE lScore=0l;
				for(BYTE i=0;i<GAME_PLAYER;i++)
				{
					IServerUserItem * pIServerUser=m_pITableFrame->GetTableUserItem(i);
					if(GameEnd.lGameScore[i] < 0 && pIServerUser->GetUserScore() < -GameEnd.lGameScore[i])
					{
						if(lScore < pIServerUser->GetUserScore() || lScore == 0)
							lScore = pIServerUser->GetUserScore();
					}
				}

				if(lScore != 0)
				{
					if(GameEnd.lGameScore[m_wBankerUser] < 0)
					{
						GameEnd.lGameScore[m_wBankerUser]=-(lScore);
						GameEnd.lGameScore[(m_wBankerUser+1)%GAME_PLAYER]=(lScore/2);
						GameEnd.lGameScore[(m_wBankerUser+2)%GAME_PLAYER]=(lScore/2);

						ScoreInfoArray[m_wBankerUser].lScore=GameEnd.lGameScore[m_wBankerUser];
						ScoreInfoArray[(m_wBankerUser+1)%GAME_PLAYER].lScore=GameEnd.lGameScore[(m_wBankerUser+1)%GAME_PLAYER];
						ScoreInfoArray[(m_wBankerUser+2)%GAME_PLAYER].lScore=GameEnd.lGameScore[(m_wBankerUser+2)%GAME_PLAYER];
					}
					else
					{
						GameEnd.lGameScore[m_wBankerUser]=(lScore*2);
						GameEnd.lGameScore[(m_wBankerUser+1)%GAME_PLAYER]=-(lScore);
						GameEnd.lGameScore[(m_wBankerUser+2)%GAME_PLAYER]=-(lScore);

						ScoreInfoArray[m_wBankerUser].lScore=GameEnd.lGameScore[m_wBankerUser];
						ScoreInfoArray[(m_wBankerUser+1)%GAME_PLAYER].lScore=GameEnd.lGameScore[(m_wBankerUser+1)%GAME_PLAYER];
						ScoreInfoArray[(m_wBankerUser+2)%GAME_PLAYER].lScore=GameEnd.lGameScore[(m_wBankerUser+2)%GAME_PLAYER];
					}

					for(BYTE i=0;i<GAME_PLAYER;i++)
					{
						if (GameEnd.lGameScore[i]>=100L)
						{
							//����˰��
							GameEnd.lGameTax+= m_pITableFrame->CalculateRevenue(i,GameEnd.lGameScore[i]);
							ScoreInfoArray[i].lRevenue=GameEnd.lGameTax;

							//���ֵ���
							GameEnd.lGameScore[i]=GameEnd.lGameScore[i]-GameEnd.lGameTax;
							ScoreInfoArray[i].lScore=GameEnd.lGameScore[i];
						}
					}
				}
			}

			//�޸Ļ���
			m_pITableFrame->WriteTableScore(ScoreInfoArray,CountArray(ScoreInfoArray));

			GameEnd.iOtherAdd[0]=m_wBombTime;
			GameEnd.iOtherAdd[2]=m_wQiangLandDouble;
			GameEnd.iOtherAdd[3]=m_wBomb;
			CopyMemory(GameEnd.bUserAdd,m_bUserDouble,sizeof(m_bUserDouble));

			if(GameEnd.iOtherAdd[4] == 0)
				GameEnd.iOtherAdd[4]=1;

			//���Ʊ���
			BYTE bMingCard=0;
			for(BYTE i=0;i<CountArray(m_bMingCardStatus);i++)
			{
				if(m_bMingCardStatus[i] < bMingCard)
					bMingCard=m_bMingCardStatus[i];
			}
			if(bMingCard == 3)
				GameEnd.iOtherAdd[1]=2;
			else if(bMingCard == 2)
				GameEnd.iOtherAdd[1]=4;
			else if(bMingCard == 1)
				GameEnd.iOtherAdd[1]=5;
			else
				GameEnd.iOtherAdd[1]=1;

			//������Ϣ
			m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
			m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));

			//������Ϸ
			m_pITableFrame->ConcludeGame(GS_WK_FREE);

			return true;
		}
	}

	ASSERT(FALSE);

	return false;
}

//���ͳ���
bool  CTableFrameSink::OnEventSendGameScene(WORD wChiarID, IServerUserItem * pIServerUserItem, BYTE cbGameStatus, bool bSendSecret)
{
	switch (cbGameStatus)
	{
	case GS_WK_FREE:		//����״̬
		{
			//��������
			CMD_S_StatusFree StatusFree;
			memset(&StatusFree,0,sizeof(StatusFree));

			//���ñ���
			StatusFree.lBaseScore=m_pGameServiceOption->lCellScore;
			//��ʷ����
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				tagHistoryScore * pHistoryScore=m_HistoryScore.GetHistoryScore(i);
				StatusFree.lTurnScore[i]=pHistoryScore->lTurnScore;
				StatusFree.lCollectScore[i]=pHistoryScore->lCollectScore;
			}

			//���ͳ���
			return m_pITableFrame->SendGameScene(pIServerUserItem,&StatusFree,sizeof(StatusFree));
		}
	case GS_WK_SCORE:		//�з�״̬
		{
			//��������
			CMD_S_StatusScore StatusScore;
			memset(&StatusScore,0,sizeof(StatusScore));

			//���ñ���
			StatusScore.bLandScore=m_bLandScore;
			StatusScore.wCurrentUser=m_wCurrentUser;
			StatusScore.lBaseScore=m_pGameServiceOption->lCellScore;
			StatusScore.wBombTime=m_wBombTime;
			CopyMemory(StatusScore.bMingCardStatus,m_bMingCardStatus,sizeof(StatusScore.bMingCardStatus));
			CopyMemory(StatusScore.bScoreInfo,m_bScoreInfo,sizeof(m_bScoreInfo));
	//		CopyMemory(StatusScore.bCardData,m_bHandCardData[wChiarID],m_bCardCount[wChiarID]);
			CopyMemory(StatusScore.bCardData,m_bHandCardData, sizeof(m_bHandCardData)*sizeof(BYTE));
			CopyMemory( StatusScore.bUserTrustee, m_bUserTrustee, sizeof( m_bUserTrustee ) );

			//��ʷ����
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				tagHistoryScore * pHistoryScore=m_HistoryScore.GetHistoryScore(i);
				StatusScore.lTurnScore[i]=pHistoryScore->lTurnScore;
				StatusScore.lCollectScore[i]=pHistoryScore->lCollectScore;
			}

			//���ͳ���
			return m_pITableFrame->SendGameScene(pIServerUserItem,&StatusScore,sizeof(StatusScore));
		}
	case GS_WK_PLAYING:		//��Ϸ״̬
		{
			//��������
			CMD_S_StatusPlay StatusPlay;
			memset(&StatusPlay,0,sizeof(StatusPlay));

			//���ñ���
			StatusPlay.wBombTime=m_wBombTime;
			StatusPlay.wLandUser=m_wBankerUser;
			StatusPlay.wCurrentUser=m_wCurrentUser;
			StatusPlay.wLastOutUser=m_wTurnWiner;
			StatusPlay.bLandScore=m_bLandScore;
			StatusPlay.bTurnCardCount=m_bTurnCardCount;
			StatusPlay.lBaseScore=m_pGameServiceOption->lCellScore;
			StatusPlay.bLaiZiCard=m_bLaiZiCard;
			CopyMemory(StatusPlay.bMingCardStatus,m_bMingCardStatus,sizeof(StatusPlay.bMingCardStatus));
			CopyMemory(StatusPlay.bBackCard,m_bBackCard,sizeof(m_bBackCard));
			CopyMemory(StatusPlay.bCardCount,m_bCardCount,sizeof(m_bCardCount));
			CopyMemory(StatusPlay.bTurnCardData,m_bTurnCardData,sizeof(BYTE)*m_bTurnCardCount);
	//		CopyMemory(StatusPlay.bCardData,m_bHandCardData[wChiarID],sizeof(BYTE)*m_bCardCount[wChiarID]);
			CopyMemory(StatusPlay.bCardData,m_bHandCardData,sizeof(BYTE)*sizeof(m_bHandCardData));
			CopyMemory( StatusPlay.bUserTrustee, m_bUserTrustee, sizeof( m_bUserTrustee ) );
			
			//��ʷ����
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				tagHistoryScore * pHistoryScore=m_HistoryScore.GetHistoryScore(i);
				StatusPlay.lTurnScore[i]=pHistoryScore->lTurnScore;
				StatusPlay.lCollectScore[i]=pHistoryScore->lCollectScore;
			}

			//���ͳ���
			return m_pITableFrame->SendGameScene(pIServerUserItem,&StatusPlay,sizeof(StatusPlay));
		}
	case GS_WK_QIANG_LAND:	//������
		{
			//��������
			CMD_S_StatusQiangLand QiangLand;
			memset(&QiangLand,0,sizeof(QiangLand));

			//���ñ���
			QiangLand.wCurrentUser=m_wCurrentUser;
			QiangLand.lBaseScore=m_pGameServiceOption->lCellScore;
			QiangLand.wBombTime=m_wBombTime;
			CopyMemory(QiangLand.bMingCardStatus,m_bMingCardStatus,sizeof(QiangLand.bMingCardStatus));
			CopyMemory(QiangLand.bScoreInfo,m_bScoreInfo,sizeof(m_bScoreInfo));
			CopyMemory(QiangLand.bCardData,m_bHandCardData, sizeof(m_bHandCardData)*sizeof(BYTE));
			CopyMemory( QiangLand.bUserTrustee, m_bUserTrustee, sizeof( m_bUserTrustee ) );
			CopyMemory(QiangLand.bMingCard,m_bMingCardStatus,sizeof(m_bMingCardStatus));

			//��ʷ����
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				tagHistoryScore * pHistoryScore=m_HistoryScore.GetHistoryScore(i);
				QiangLand.lTurnScore[i]=pHistoryScore->lTurnScore;
				QiangLand.lCollectScore[i]=pHistoryScore->lCollectScore;
			}

			//���ͳ���
			return m_pITableFrame->SendGameScene(pIServerUserItem,&QiangLand,sizeof(QiangLand));
		}
	case GS_WK_ADD:			//�ӱ�
		{
			//��������
			CMD_S_StatusAdd Add;
			memset(&Add,0,sizeof(Add));

			//���ñ���
			Add.wLandUser=m_wBankerUser;
			Add.wCurrentUser=m_wCurrentUser;
			Add.lBaseScore=m_pGameServiceOption->lCellScore;
			Add.wBombTime=m_wBombTime;
			Add.bLaiZiCard=m_bLaiZiCard;
			CopyMemory(Add.bMingCardStatus,m_bMingCardStatus,sizeof(Add.bMingCardStatus));
			CopyMemory(Add.bScoreInfo,m_bScoreInfo,sizeof(m_bScoreInfo));
			CopyMemory(Add.bCardData,m_bHandCardData, sizeof(m_bHandCardData)*sizeof(BYTE));
			CopyMemory( Add.bUserTrustee, m_bUserTrustee, sizeof( m_bUserTrustee ) );
			CopyMemory(Add.bMingCard,m_bMingCardStatus,sizeof(m_bMingCardStatus));
			CopyMemory(Add.bAddScore,m_bAddScore,sizeof(m_bAddScore));

			//��ʷ����
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				tagHistoryScore * pHistoryScore=m_HistoryScore.GetHistoryScore(i);
				Add.lTurnScore[i]=pHistoryScore->lTurnScore;
				Add.lCollectScore[i]=pHistoryScore->lCollectScore;
			}

			//���ͳ���
			return m_pITableFrame->SendGameScene(pIServerUserItem,&Add,sizeof(Add));
		}
	}

	return false;
}

//��ʱ���¼�
bool  CTableFrameSink::OnTimerMessage(DWORD wTimerID, WPARAM wBindParam)
{
	return false;
}

//�����¼�
bool CTableFrameSink::OnGameDataBase(WORD wRequestID, VOID * pData, WORD wDataSize)
{
	return false;
}
//��Ϸ��Ϣ����
bool  CTableFrameSink::OnGameMessage(WORD wSubCmdID, VOID * pData, WORD wDataSize, IServerUserItem * pIServerUserItem)
{
	switch (wSubCmdID)
	{
	case SUB_C_LAND_SCORE:	//�û��з�
		{
			//Ч������
			ASSERT(wDataSize==sizeof(CMD_C_LandScore));
			if (wDataSize!=sizeof(CMD_C_LandScore)) return false;

			//�û�Ч��
			tagUserInfo * pUserData=pIServerUserItem->GetUserInfo();
			if (pUserData->cbUserStatus!=US_PLAYING) return true;

			//��Ϣ����
			CMD_C_LandScore * pLandScore=(CMD_C_LandScore *)pData;
			return OnUserLandScore(pUserData->wChairID,pLandScore->bLandScore);
		}
	case SUB_C_OUT_CART:		//�û�����
		{
			//��������
			CMD_C_OutCard * pOutCard=(CMD_C_OutCard *)pData;
			WORD wHeadSize=sizeof(CMD_C_OutCard)-sizeof(pOutCard->bCardData);

			//Ч������
			ASSERT(wDataSize>=wHeadSize);
			if (wDataSize<wHeadSize) return false;
			ASSERT(wDataSize==(wHeadSize+pOutCard->bCardCount*sizeof(pOutCard->bCardData[0])));
			if (wDataSize!=(wHeadSize+pOutCard->bCardCount*sizeof(pOutCard->bCardData[0]))) return false;

			//�û�Ч��
			tagUserInfo * pUserData=pIServerUserItem->GetUserInfo();
			if (pUserData->cbUserStatus!=US_PLAYING) return true;

			//��Ϣ����
			return OnUserOutCard(pUserData->wChairID,pOutCard->bCardData,pOutCard->bCardCount,pOutCard->bChangeCard);
		}
	case SUB_C_PASS_CARD:
		{
			//�û�Ч��
			tagUserInfo * pUserData=pIServerUserItem->GetUserInfo();
			if (pUserData->cbUserStatus!=US_PLAYING) return true;

			//��Ϣ����
			return OnUserPassCard(pUserData->wChairID);
		}
	case SUB_C_TRUSTEE:			//����й�
		{
			//Ч������
			ASSERT(wDataSize==sizeof(CMD_C_UserTrustee));
			if (wDataSize != sizeof(CMD_C_UserTrustee)) return false;

			CMD_C_UserTrustee *pUserTrustee = ( CMD_C_UserTrustee * )pData;

			m_bUserTrustee[ pUserTrustee->wUserChairID ] = pUserTrustee->bTrustee;

			m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_C_TRUSTEE, (void*)pData,wDataSize);
			m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_C_TRUSTEE, (void*)pData,wDataSize);

			return true;
		}
	case SUB_C_MINGSTATE:		//��������
		{
			//Ч������
			ASSERT(wDataSize==sizeof(CMD_C_MingState));
			if (wDataSize!=sizeof(CMD_C_MingState)) return false;

			//��Ϣ����
			CMD_C_MingState * pMingCard=(CMD_C_MingState *)pData;
			return OnMingState(pMingCard->bMingState,pMingCard->wUserChairID,pIServerUserItem);
		}
	case SUB_C_QIANGLAND:		//������
		{
			//Ч������
			ASSERT(wDataSize==sizeof(CMD_C_QiangLand));
			if (wDataSize!=sizeof(CMD_C_QiangLand)) return false;

			//�û�Ч��
			tagUserInfo * pUserData=pIServerUserItem->GetUserInfo();
			if (pUserData->cbUserStatus!=US_PLAYING) return true;

			//��Ϣ����
			CMD_C_QiangLand * pQiangLand=(CMD_C_QiangLand *)pData;
			return OnQiangLand(pUserData->wChairID,pQiangLand->bQiangLand);
		}
	case SUB_C_MINGOUT:			//��������
		{
			//�û�Ч��
			tagUserInfo * pUserData=pIServerUserItem->GetUserInfo();
			if (pUserData->cbUserStatus!=US_PLAYING) return true;

			return OnMingOut(pUserData->wChairID);
		}
	case SUB_C_ADD:				//�ӱ�
		{
			//Ч������
			ASSERT(wDataSize==sizeof(CMD_C_ADD));
			if (wDataSize!=sizeof(CMD_C_ADD)) return false;

			//�û�Ч��
			tagUserInfo * pUserData=pIServerUserItem->GetUserInfo();
			if (pUserData->cbUserStatus!=US_PLAYING) return true;

			//��Ϣ����
			CMD_C_ADD * pAdd=(CMD_C_ADD *)pData;

			return OnAdd(pUserData->wChairID,pAdd->bAdd);
		}
	}

	return true;
}

//�����Ϣ����
bool  CTableFrameSink::OnFrameMessage(WORD wSubCmdID, VOID * pData, WORD wDataSize, IServerUserItem * pIServerUserItem)
{
	return false;
}

//�з��¼�
bool CTableFrameSink::OnUserLandScore(WORD wChairID, BYTE bLandScore)
{
	//Ч��״̬
	if (m_pITableFrame->GetGameStatus()!=GS_WK_SCORE) return true;
	if (wChairID!=m_wCurrentUser) return false;

	//Ч�����
	if (((bLandScore>3)&&(bLandScore!=255))||(bLandScore<=m_bLandScore)) return false;

	//���ñ���
	if (bLandScore!=255)
	{
		m_bLandScore=bLandScore;
		m_wBankerUser=m_wCurrentUser;
		m_bQiangLand[wChairID]=false;

	}
	else		//������Ȩ��
		m_bQiangLand[wChairID]=true;

	m_bScoreInfo[wChairID]=bLandScore;

	//��ʼ�ж�
	if ((m_bLandScore==3)||(m_wFirstUser==(wChairID+1)%m_wPlayerCount))
	{
		//���·���
		if (m_bLandScore==0 && m_wBankerUser == INVALID_CHAIR) 
		{
			BYTE cbSendGoodCard = rand() % GOOD_CARD_PERCENT ;
			BYTE cbAndroidUserCount=0 ;
			WORD wAndroidUser[GAME_PLAYER]={0};

			//���ñ���
			ZeroMemory(m_bScoreInfo, sizeof(m_bScoreInfo)) ;

			//�����˸���
			for(WORD wChairID=0; wChairID<GAME_PLAYER; ++wChairID)
			{
				IServerUserItem *pServerUserItem = m_pITableFrame->GetTableUserItem(wChairID) ;
				if(pServerUserItem->IsAndroidUser())
				{
					wAndroidUser[cbAndroidUserCount] = (wChairID) ;
					++cbAndroidUserCount ;

				}
			}

			WORD wHaveGoodCardAndroidUser = INVALID_CHAIR ;
			if(cbAndroidUserCount>0) wHaveGoodCardAndroidUser = wAndroidUser[rand()%cbAndroidUserCount] ;

			//���ͺ���
			if( 0 == cbSendGoodCard && wHaveGoodCardAndroidUser!=INVALID_CHAIR)
			{
				//�����˿�
				BYTE bRandCard[54];
				m_GameLogic.RandCardList(bRandCard,sizeof(bRandCard)/sizeof(bRandCard[0]));

				BYTE cbGoodCard[NORMAL_COUNT] ;
				m_GameLogic.GetGoodCardData(cbGoodCard) ;

				//��ȡ����
				m_GameLogic.RemoveGoodCardData(cbGoodCard, NORMAL_COUNT, bRandCard, FULL_COUNT ) ;

				//�ַ��˿�
				CopyMemory(m_bHandCardData[wHaveGoodCardAndroidUser], cbGoodCard, NORMAL_COUNT) ;
				m_bCardCount[wHaveGoodCardAndroidUser]=17;
				for (WORD i=0, j=0;i<m_wPlayerCount;i++)
				{
					if(i!=wHaveGoodCardAndroidUser)
					{
						m_bCardCount[i]=17;
						CopyMemory(&m_bHandCardData[i],&bRandCard[j*17],sizeof(BYTE)*17);
						++j ;
					}
				}
				CopyMemory(m_bBackCard,&bRandCard[34],sizeof(m_bBackCard));
				m_GameLogic.SortCardList(m_bBackCard,3,ST_ORDER);
			}
			else
			{
				//�����˿�
				BYTE bRandCard[54];
				m_GameLogic.RandCardList(bRandCard,sizeof(bRandCard)/sizeof(bRandCard[0]));

				//�ַ��˿�
				for (WORD i=0;i<m_wPlayerCount;i++)
				{
					m_bCardCount[i]=17;
					CopyMemory(&m_bHandCardData[i],&bRandCard[i*m_bCardCount[i]],sizeof(BYTE)*m_bCardCount[i]);
				}
				CopyMemory(m_bBackCard,&bRandCard[51],sizeof(m_bBackCard));
				m_GameLogic.SortCardList(m_bBackCard,3,ST_ORDER);
			}

			//�����û�
			m_wCurrentUser = rand() % GAME_PLAYER;
			m_wFirstUser=m_wCurrentUser;

			//�����˿�
			CMD_S_SendAllCard SendAllCard ;
			SendAllCard.wCurrentUser = m_wCurrentUser ;

			//�����˿�
			CopyMemory(SendAllCard.bCardData, m_bHandCardData, sizeof(m_bHandCardData)) ;
			//�����˿�
			CopyMemory(SendAllCard.bBackCardData, m_bBackCard, 3) ;
			//������
			m_bLaiZiCard=m_GameLogic.GetLaiZiCard();
			SendAllCard.bLaiZiCard=m_bLaiZiCard;
			//��������
			for(BYTE i=0;i<GAME_PLAYER;i++)
			{
				if(m_bMingCardStatus[i] > 1)
					m_bMingCardStatus[i]=0;
			}
			CopyMemory( SendAllCard.bMingCard, m_bMingCardStatus, sizeof( m_bMingCardStatus ) ) ;

			for (WORD i=0;i<m_wPlayerCount;i++)
			{
				m_pITableFrame->SendTableData(i,SUB_S_SEND_CARD,&SendAllCard,sizeof(SendAllCard));
				m_pITableFrame->SendLookonData(i,SUB_S_SEND_CARD,&SendAllCard,sizeof(SendAllCard));
			}

			//�����˿�
			for (WORD i=0;i<m_wPlayerCount;i++)
			{
				m_GameLogic.SortCardList(m_bHandCardData[i],m_bCardCount[i],ST_ORDER);
			}

			return true;
		}

		//������Ϣ
		CMD_S_LandScore LandScore;
		LandScore.bLandUser=wChairID;
		LandScore.bLandScore=bLandScore;
		LandScore.wCurrentUser=m_wCurrentUser;
		LandScore.bCurrentScore=m_bLandScore;
		m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_LAND_SCORE,&LandScore,sizeof(LandScore));
		m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_LAND_SCORE,&LandScore,sizeof(LandScore));

		m_pITableFrame->SetGameStatus(GS_WK_QIANG_LAND);

		//������
		CMD_S_QiangLand QiangLand;
		WORD wBankerUser=m_wBankerUser;
		do
		{
			wBankerUser=(wBankerUser+1)%m_wPlayerCount;
			if(wBankerUser == m_wBankerUser)break;
			if(m_bQiangLand[wBankerUser] == true)continue;
			if(m_bScoreInfo[wBankerUser] != 255)
			{
				QiangLand.wQiangUser=wBankerUser;
				QiangLand.bQiangLand=0;
				QiangLand.wBombTime=m_wBombTime;

				m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_QIANG_LAND,&QiangLand,sizeof(QiangLand));
				m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_QIANG_LAND,&QiangLand,sizeof(QiangLand));
				return true;
			}
		}while(true);

		//�ɷ�ӱ�
		for(BYTE i=0;i<GAME_PLAYER;i++)
		{
			LONGLONG lScore=m_pITableFrame->GetTableUserItem(i)->GetUserScore();
			if(lScore > m_lAddScore)
			{
				m_bAddScore[i]=true;
			}
		}
		//�����ӱ�
		for(BYTE i=0;i<GAME_PLAYER;i++)
		{
			if(m_bAddScore[m_wBankerUser] == true)
			{
				if(m_bAddScore[i] == true)
					m_bAddScore[m_wBankerUser]=true;
				else
				{
					m_bAddScore[m_wBankerUser]=false;
					break;
				}
			}
		}
		//ũ��ӱ�
		if(m_bAddScore[m_wBankerUser] == false)
		{
			for(BYTE i=0;i<GAME_PLAYER;i++)
			{
				m_bAddScore[i]=false;
			}
		}

		//��ʼ�ж�
		//����״̬
		m_pITableFrame->SetGameStatus(GS_WK_PLAYING);

		//���͵���
		m_bCardCount[m_wBankerUser]=20;
		CopyMemory(&m_bHandCardData[m_wBankerUser][17],m_bBackCard,sizeof(m_bBackCard));
		m_GameLogic.SortCardList(m_bHandCardData[m_wBankerUser],m_bCardCount[m_wBankerUser],ST_ORDER);

		//������Ϣ
		m_bTurnCardCount=0;
		m_wTurnWiner=m_wBankerUser;
		m_wCurrentUser=m_wBankerUser;

		//������Ϣ
		CMD_S_GameStart GameStart;
		GameStart.wLandUser=m_wBankerUser;
		GameStart.bLandScore=m_bLandScore;
		GameStart.wCurrentUser=m_wCurrentUser;
		GameStart.wBombTime=m_wBombTime;
		CopyMemory(GameStart.bAddScore,m_bAddScore,sizeof(m_bAddScore));
		CopyMemory(GameStart.bBackCard,m_bBackCard,sizeof(m_bBackCard));

		GameStart.bIsAdd=false;
		for(BYTE i=0;i<GAME_PLAYER;i++)
		{
			if(m_bAddScore[i] == true)
			{
				GameStart.bIsAdd=true;
				break;
			}
		}
		m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_GAME_START,&GameStart,sizeof(GameStart));
		m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_START,&GameStart,sizeof(GameStart));

		return true;
		
	}


	//�����û�
	m_wCurrentUser=(wChairID+1)%m_wPlayerCount;

	//������Ϣ
	CMD_S_LandScore LandScore;
	LandScore.bLandUser=wChairID;
	LandScore.bLandScore=bLandScore;
	LandScore.wCurrentUser=m_wCurrentUser;
	LandScore.bCurrentScore=m_bLandScore;
	m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_LAND_SCORE,&LandScore,sizeof(LandScore));
	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_LAND_SCORE,&LandScore,sizeof(LandScore));

	return true;
}

//�û�����
bool CTableFrameSink::OnUserOutCard(WORD wChairID, BYTE bCardData[], BYTE bCardCount,BYTE bChangeCard[])
{
	//Ч��״̬
	if (m_pITableFrame->GetGameStatus()!=GS_WK_PLAYING) return true;
	if (wChairID!=m_wCurrentUser) return false;

	//�����˿�
	m_GameLogic.SortCardList(bCardData, bCardCount, ST_ORDER);

	//�����ж�
	int bCardType=m_GameLogic.GetCardType(bCardData,bCardCount);
	if (bCardType==CT_ERROR) return false;

	//�������
	if (m_bTurnCardCount==0) m_bTurnCardCount=bCardCount;
	else if (m_GameLogic.CompareCard(m_bTurnCardData,bCardData,m_bTurnCardCount,bCardCount)==false) return false;

	//ɾ���˿�
	if (m_GameLogic.RemoveCard(bCardData,bCardCount,m_bHandCardData[wChairID],m_bCardCount[wChairID])==false) return false;
	m_bCardCount[wChairID]-=bCardCount;

	//���Ƽ�¼
	m_bTurnCardCount=bCardCount;
	m_bOutCardCount[wChairID]++;
	CopyMemory(m_bTurnCardData,bCardData,sizeof(BYTE)*bCardCount);

	//ը���ж�
	if ((bCardType&(1<<CT_BOMB_CARD)) != 0 ||(bCardType&(1<<CT_MISSILE_CARD)) != 0||(bCardType&(1<<CT_LAIZI_BOMB))!= 0) 
	{
		m_wBombTime*=4;
		m_wBomb*=4;
	}
	else if((bCardType&(1<<CT_RUAN_BOMB))!= 0)
	{
		m_wBombTime*=2;
		m_wBomb*=2;
	}
	
	//�л��û�
	m_wTurnWiner=wChairID;
	if (m_bCardCount[wChairID]!=0)
	{
		if ((bCardType&(1<<CT_MISSILE_CARD)) == 0)m_wCurrentUser=(m_wCurrentUser+1)%m_wPlayerCount;
	}
	else m_wCurrentUser=INVALID_CHAIR;

	for(BYTE i=0;i<m_bTurnCardCount;i++)
	{
		if(bChangeCard[i] == NULL)
			ASSERT(FALSE);
		if(m_bTurnCardData[i] == NULL)
			ASSERT(FALSE);
	}

	//��������
	CMD_S_OutCard OutCard;
	ZeroMemory(&OutCard,sizeof(OutCard));
	OutCard.bCardCount=bCardCount;
	OutCard.wOutCardUser=wChairID;
	OutCard.wCurrentUser=m_wCurrentUser;
	CopyMemory(OutCard.bCardData,m_bTurnCardData,m_bTurnCardCount*sizeof(BYTE));
	CopyMemory(OutCard.bChangeCard,bChangeCard,m_bTurnCardCount*sizeof(BYTE));

	//��������
	WORD wSendSize=sizeof(OutCard)-sizeof(OutCard.bCardData)+OutCard.bCardCount*sizeof(BYTE);
	m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_OUT_CARD,&OutCard,wSendSize);
	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_OUT_CARD,&OutCard,wSendSize);

	//�������
	if ((bCardType&(1<<CT_MISSILE_CARD)) != 0) m_bTurnCardCount=0;

	//�����ж�
	if (m_wCurrentUser==INVALID_CHAIR) OnEventGameConclude(wChairID,NULL,GER_NORMAL);

	return true;
}

//�û�����
bool CTableFrameSink::OnUserPassCard(WORD wChairID)
{
	//Ч��״̬
	if (m_pITableFrame->GetGameStatus()!=GS_WK_PLAYING) return true;
	if ((wChairID!=m_wCurrentUser)||(m_bTurnCardCount==0)) return false;

	//���ñ���
	m_wCurrentUser=(m_wCurrentUser+1)%m_wPlayerCount;
	if (m_wCurrentUser==m_wTurnWiner) m_bTurnCardCount=0;

	//��������
	CMD_S_PassCard PassCard;
	PassCard.wPassUser=wChairID;
	PassCard.wCurrentUser=m_wCurrentUser;
	PassCard.bNewTurn=(m_bTurnCardCount==0)?TRUE:FALSE;
	m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_PASS_CARD,&PassCard,sizeof(PassCard));
	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_PASS_CARD,&PassCard,sizeof(PassCard));

	return true;
}
//���ƿ�ʼ
bool CTableFrameSink::OnMingState(BYTE bMingState,WORD wUserChairID,IServerUserItem * pIServerUserItem)
{
	//���ƿ�ʼ
	if (pIServerUserItem->GetUserStatus()!=GAME_STATUS_PLAY)
	{
		m_bMingCardStatus[wUserChairID]=bMingState;
		if(m_wBankerUser == INVALID_CHAIR)
			m_wBankerUser=pIServerUserItem->GetChairID();

		//���ñ���
		m_wBombTime=5;

		return true;
	}
	else		
	{
		//���ñ���
		if(m_wBombTime == 1)
			m_wBombTime*=4;

		//��������
		CMD_S_MingCard MingCard;
		MingCard.bMingCardStatus=bMingState;
		MingCard.wMingUser=wUserChairID;
		MingCard.wBombTime=m_wBombTime;
		if(m_wBankerUser == INVALID_CHAIR)
			m_wBankerUser=pIServerUserItem->GetChairID();

		m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_MING_CARD,&MingCard,sizeof(MingCard));
		m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_MING_CARD,&MingCard,sizeof(MingCard));
	}

	return true;
}
//��������
bool CTableFrameSink::OnMingOut(WORD wCurrentUser)
{
	//����״̬
	m_bMingCardStatus[wCurrentUser]=3;

	//���ñ���
	if(m_wBombTime == 1)
		m_wBombTime*=2;

	CMD_S_MingCard MingCard;
	MingCard.bMingCardStatus=3;
	MingCard.wMingUser=wCurrentUser;
	MingCard.wBombTime=m_wBombTime;

	m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_MING_OUT,&MingCard,sizeof(MingCard));
	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_MING_OUT,&MingCard,sizeof(MingCard));


	return true;
}
//������
bool CTableFrameSink::OnQiangLand(WORD wChairID,bool bQiangLand)
{
	//״̬�ж�
	if(m_pITableFrame->GetGameStatus()!=GS_WK_QIANG_LAND)return false;

	//������
	if(bQiangLand)
	{
		m_wBankerUser=wChairID;

		//���ñ���
		m_wBombTime*=2;
		m_wQiangLandDouble*=2;
	}
	m_bQiangLand[wChairID]=true;

	//�������ֻ�
	for(BYTE i=0;i<GAME_PLAYER;i++)
	{
		if(m_bQiangLand[i] == false)
		{
			CMD_S_QiangLand QiangLand;
			QiangLand.wQiangUser=i;
			QiangLand.bQiangLand=(BYTE)bQiangLand+1;
			QiangLand.wBombTime=m_wBombTime;

			m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_QIANG_LAND,&QiangLand,sizeof(QiangLand));
			m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_QIANG_LAND,&QiangLand,sizeof(QiangLand));

			return true;
		}
	}

	//�ɷ�ӱ�
	for(BYTE i=0;i<GAME_PLAYER;i++)
	{
		LONGLONG lScore=m_pITableFrame->GetTableUserItem(i)->GetUserScore();
		if(lScore > m_lAddScore)
		{
			m_bAddScore[i]=true;
		}
	}
	//�����ӱ�
	for(BYTE i=0;i<GAME_PLAYER;i++)
	{
		if(m_bAddScore[m_wBankerUser] == true)
		{
			if(m_bAddScore[i] == true)
				m_bAddScore[m_wBankerUser]=true;
			else
			{
				m_bAddScore[m_wBankerUser]=false;
				break;
			}
		}
	}
	//ũ��ӱ�
	if(m_bAddScore[m_wBankerUser] == false)
	{
		for(BYTE i=0;i<GAME_PLAYER;i++)
		{
			m_bAddScore[i]=false;
		}
	}

	//��ʼ�ж�
	//����״̬
	m_pITableFrame->SetGameStatus(GS_WK_PLAYING);

	//���͵���
	m_bCardCount[m_wBankerUser]=20;
	CopyMemory(&m_bHandCardData[m_wBankerUser][17],m_bBackCard,sizeof(m_bBackCard));
	m_GameLogic.SortCardList(m_bHandCardData[m_wBankerUser],m_bCardCount[m_wBankerUser],ST_ORDER);

	//������Ϣ
	m_bTurnCardCount=0;
	m_wTurnWiner=m_wBankerUser;
	m_wCurrentUser=m_wBankerUser;

	//������Ϣ
	CMD_S_GameStart GameStart;
	GameStart.wLandUser=m_wBankerUser;
	GameStart.bLandScore=m_bLandScore;
	GameStart.wCurrentUser=m_wCurrentUser;
	GameStart.wBombTime=m_wBombTime;
	CopyMemory(GameStart.bAddScore,m_bAddScore,sizeof(m_bAddScore));
	CopyMemory(GameStart.bBackCard,m_bBackCard,sizeof(m_bBackCard));

	GameStart.bIsAdd=false;
	for(BYTE i=0;i<GAME_PLAYER;i++)
	{
		if(m_bAddScore[i] == true)
		{
			GameStart.bIsAdd=true;
			break;
		}
	}
	m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_GAME_START,&GameStart,sizeof(GameStart));
	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_START,&GameStart,sizeof(GameStart));

	return true;

}

//�û��ӱ�
bool CTableFrameSink::OnAdd(WORD wCurrentUser,bool bAdd)
{
	//״̬�ж�
	if(m_pITableFrame->GetGameStatus() != GS_WK_PLAYING)return false;

	//�Ƿ�ӱ�
	if(bAdd == true)
		m_bUserDouble[wCurrentUser]=2;

	//�û��л�
	CMD_S_Add Add;
	WORD wNextUser=(wCurrentUser+1)%GAME_PLAYER;
	if(m_wBankerUser != wNextUser)
	{
		Add.bAdd=bAdd;
		Add.wAddUser=wCurrentUser;
		Add.wCurrentUser=wNextUser;
		Add.bIsAdd=m_bAddScore[wNextUser];
	}
	else
	{
		Add.bAdd=bAdd;
		Add.wAddUser=wCurrentUser;
		Add.wCurrentUser=INVALID_CHAIR;
		Add.bIsAdd=false;

		m_pITableFrame->SetGameStatus(GS_WK_PLAYING);
	}

	m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_ADD,&Add,sizeof(Add));
	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_ADD,&Add,sizeof(Add));

	return true;
}
//////////////////////////////////////////////////////////////////////////
