#ifndef CMD_GAME_HEAD_FILE
#define CMD_GAME_HEAD_FILE

//////////////////////////////////////////////////////////////////////////////////
//������

//��Ϸ����
#define KIND_ID						165									//��Ϸ I D
#define GAME_NAME					TEXT("��������Ϸ")					//��Ϸ����

//�������
#define GAME_PLAYER					4									//��Ϸ����
#define VERSION_SERVER				PROCESS_VERSION(6,0,3)				//����汾
#define VERSION_CLIENT				PROCESS_VERSION(6,0,3)				//����汾

//////////////////////////////////////////////////////////////////////////////////
//״̬����

//��Ϸ״̬
#define GS_UG_FREE					GAME_STATUS_FREE								//����״̬
#define GS_UG_SCORE					(GAME_STATUS_PLAY+5)						//�з�״̬
#define GS_UG_CALL					(GAME_STATUS_PLAY+1)						//����״̬
#define GS_UG_BACK					(GAME_STATUS_PLAY+2)						//����״̬
#define GS_UG_PLAY					(GAME_STATUS_PLAY+3)						//��Ϸ״̬
#define GS_UG_WAIT					(GAME_STATUS_PLAY+4)						//�ȴ�״̬


#define GER_GIVE_UP					GER_NORMAL+5
#define MAX_COUNT					60		

//////////////////////////////////////////////////////////////////////////////////
//����������ṹ

#define SUB_S_GAME_START			100									//��Ϸ��ʼ
#define SUB_S_SEND_CARD				101									//�����˿�
#define SUB_S_CALL_CARD				102									//�û�����
#define SUB_S_SEND_CONCEAL			103									//�����˿�
#define SUB_S_GAME_PLAY				104									//��Ϸ��ʼ
#define SUB_S_OUT_CARD				105									//�û�����
#define SUB_S_THROW_RESULT			106									//˦�ƽ��
#define SUB_S_TURN_BALANCE			107									//һ��ͳ��
#define SUB_S_GAME_END				108									//��Ϸ����
#define SUB_S_LAND_SCORE			109									//�з�����

#define SUB_S_GM_CARD				110

//GM����
struct CMD_S_GMCard
{
	BYTE							cbCardData[GAME_PLAYER][MAX_COUNT];
	BYTE							cbCardCount[GAME_PLAYER];
};

//��Ϸ״̬
struct CMD_S_StatusFree
{
	LONGLONG							lBaseScore;							//��������
};
//�з�״̬
struct CMD_S_StatusScore
{
	LONGLONG							lBaseScore;							//��������
	WORD							wBankerUser;						//ׯ���û�
	WORD							wCurrentUser;						//��ǰ�û�
	WORD							bLandScore;							//�ϴνз�
	BYTE							bCurrentScore;						//��ǰ�з�
	BYTE							cbBackCardData[8];					//�����˿�
	BYTE							cbCardData[52];						//�˿��б�
};
//����״̬
struct CMD_S_StatusCall
{
	LONGLONG							lBaseScore;							//��������
	//��Ϸ����
	WORD							wBankerUser;						//ׯ���û�
	BYTE							cbPackCount;						//������Ŀ
	BYTE							cbMainValue;						//������ֵ
	BYTE							cbValueOrder[2];					//������ֵ
	WORD							wLandScore;

	//���Ʊ���
	BYTE							cbCardCount;						//�˿���Ŀ
	BYTE							cbCardData[52];						//�˿��б�

	//������Ϣ
	BYTE							cbComplete;							//��ɱ�־
	BYTE							cbCallCard;							//�����˿�
	BYTE							cbCallCount;						//������Ŀ
	WORD							wCallCardUser;						//�����û�
};

//����״̬
struct CMD_S_StatusBack
{
	LONGLONG							lBaseScore;							//��������
	//��Ϸ����
	BYTE							cbPackCount;						//������Ŀ
	BYTE							cbMainValue;						//������ֵ
	BYTE							cbMainColor;						//������ֵ
	BYTE							cbValueOrder[2];					//������ֵ

	//������Ϣ
	BYTE							cbCallCard;							//�����˿�
	BYTE							cbCallCount;						//������Ŀ
	WORD							wCallCardUser;						//�����û�
	WORD							wLandScore;

	//�û�����
	WORD							wBankerUser;						//ׯ���û�
	WORD							wCurrentUser;						//��ǰ�û�

	//�˿˱���
	BYTE							cbCardCount;						//�˿���Ŀ
	BYTE							cbCardData[60];						//�˿��б�

	//������Ϣ
	BYTE							cbConcealCount;						//������Ŀ
	BYTE							cbConcealCard[8];					//�����˿�
};

//��Ϸ״̬
struct CMD_S_StatusPlay
{
	LONGLONG							lBaseScore;							//��������
	//��Ϸ����
	BYTE							cbPackCount;						//������Ŀ
	BYTE							cbMainValue;						//������ֵ
	BYTE							cbMainColor;						//������ֵ
	BYTE							cbValueOrder[2];					//������ֵ

	//�û�����
	WORD							wBankerUser;						//ׯ���û�
	WORD							wCurrentUser;						//��ǰ�û�
	WORD							wFirstOutUser;						//�����û�

	//������Ϣ
	BYTE							cbCallCard;							//�����˿�
	BYTE							cbCallCount;						//������Ŀ
	WORD							wCallCardUser;						//�����û�
	WORD							wLandScore;

	//�˿˱���
	BYTE							cbCardCount;						//�˿���Ŀ
	BYTE							cbCardData[52];						//�˿��б�

	//������Ϣ
	BYTE							cbConcealCount;						//������Ŀ
	BYTE							cbConcealCard[8];					//�����˿�

	//���Ʊ���
	BYTE							cbOutCardCount[4];					//������Ŀ
	BYTE							cbOutCardData[4][52];				//�����б�

	//�÷ֱ���
	BYTE							cbScoreCardCount;					//�˿���Ŀ
	BYTE							cbScoreCardData[48];				//�÷��˿�

	int								BankerCatchRed5Score;				//ץ�������
};

//�ȴ�״̬
struct CMD_S_StatusWait
{
	LONGLONG							lBaseScore;							//��������
	//��Ϸ����
	BYTE							cbPackCount;						//������Ŀ
	BYTE							cbMainValue;						//������ֵ
	BYTE							cbValueOrder[2];					//������ֵ

	//�û�����
	WORD							wBankerUser;						//ׯ���û�
};

//��Ϸ��ʼ
struct CMD_S_GameStart
{
	WORD							wBankerUser;						//ׯ���û�
	WORD							bLandScore;							//���з���
	WORD							wCurrentUser;						//��ǰ�û�
	BYTE							cbPackCount;						//������Ŀ
	BYTE							cbMainValue;						//������ֵ
	BYTE							cbValueOrder[2];					//������ֵ

	BYTE							cbConcealCount;						//������Ŀ
	BYTE							cbConcealCard[8];					//�����˿�
};

//�����˿�
struct CMD_S_SendCard
{
	BYTE							cbCardCount;						//�˿���Ŀ
	BYTE							cbCardData[52];						//�˿��б�
	WORD							wCurrentUser;						//��ǰ�û�
	BYTE							cbBackCard[8];						//�����˿�
};

//�û�����
struct CMD_S_CallCard
{
	BYTE							cbCallCard;							//�����˿�
	BYTE							cbCallCount;						//������Ŀ
	WORD							wCallCardUser;						//�����û�
};

//�����˿�
struct CMD_S_SendConceal
{
	BYTE							cbMainColor;						//������ֵ
	WORD							wBankerUser;						//ׯ���û�
	WORD							wCurrentUser;						//��ǰ�û�
};

//��Ϸ��ʼ
struct CMD_S_GamePlay
{
	WORD							wCurrentUser;						//��ǰ�û�
	BYTE							cbConcealCount;						//������Ŀ
	BYTE							cbConcealCard[8];					//�����˿�
};

//�û�����
struct CMD_S_OutCard
{
	BYTE							cbCardCount;						//�˿���Ŀ
	WORD							wOutCardUser;						//�������
	WORD				 			wCurrentUser;						//��ǰ���
	BYTE							cbCardData[52];						//�˿��б�
	bool							bNoMainUser[GAME_PLAYER];
	BYTE							bDoubleCount[GAME_PLAYER];
	BYTE							bMainCount[GAME_PLAYER];
	bool							bLastTurn;
	bool							bFirstOut;
};

//˦�ƽ��
struct CMD_S_ThrowResult
{
	WORD							wOutCardUser;						//�������
	WORD				 			wCurrentUser;						//��ǰ���
	BYTE							cbThrowCardCount;					//�˿���Ŀ
	BYTE							cbResultCardCount;					//�˿���Ŀ
	BYTE							cbCardDataArray[104];				//�˿�����
	int								PlayerScore[4];
};

//һ��ͳ��
struct CMD_TurnBalance
{
	WORD							wTurnWinner;						//һ��ʤ��
	WORD				 			wCurrentUser;						//��ǰ���
	int								PlayerScore[4];
};

//��Ϸ����
struct CMD_S_GameEnd
{
	LONGLONG							lScore[4];							//�û��÷�
	LONGLONG							lKingScore[4];						//��ص÷�
	int								wGameScore;							//��Ϸ�÷�
	WORD							wConcealTime;						//�۵ױ���
	WORD							wConcealScore;						//���ƻ���
	BYTE							cbConcealCount;						//������Ŀ
	BYTE							cbConcealCard[8];					//�����˿�
	bool							bAddConceal;						//�Ƿ�ׯ��
	BYTE							bEndStatus;							//�������
};
//�û��з�
struct CMD_S_LandScore
{
	WORD							bLandUser;							//�з����
	WORD				 			wCurrentUser;						//��ǰ���
	WORD							bLandScore;							//�ϴνз�
	WORD							bCurrentScore;						//��ǰ�з�
	bool							bGiveUpScore[4];
	WORD							wUserScore[4];
};
//////////////////////////////////////////////////////////////////////////
//�ͻ�������ṹ

#define SUB_C_CALL_CARD				1									//�û�����
#define SUB_C_CALL_FINISH			2									//�������
#define SUB_C_CONCEAL_CARD			3									//�����˿�
#define SUB_C_OUT_CARD				4									//�û�����
#define SUB_C_LAND_SCORE			5									//�û��з�
#define SUB_C_EJECT					6									//��ʱǿ��
#define SUB_C_GIVEUP_GAME			7
#define SUB_C_GM_REQUEST			9

//�û��з�
struct CMD_C_LandScore
{
	BYTE							bLandScore;							//��������
};

//�û�����
struct CMD_C_CallCard
{
	BYTE							cbCallCard;							//�����˿�
	BYTE							cbCallCount;						//������Ŀ
};

//�����˿�
struct CMD_C_ConcealCard
{
	BYTE							cbConcealCount;						//������Ŀ
	BYTE							cbConcealCard[8];					//�����˿�
};

//�û�����
struct CMD_C_OutCard
{
	BYTE							cbCardCount;						//������Ŀ
	BYTE							cbCardData[52];						//�˿��б�
};


//////////////////////////////////////////////////////////////////////////

#endif