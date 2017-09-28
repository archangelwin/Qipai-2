#ifndef CMD_UPGRADE_HEAD_FILE
#define CMD_UPGRADE_HEAD_FILE

//////////////////////////////////////////////////////////////////////////
//�����궨��

#define KIND_ID						205									//��Ϸ I D
#define GAME_PLAYER					4									//��Ϸ����
#define GAME_NAME					TEXT("�ĸ�����")					//��Ϸ����

//��Ϸ״̬
#define GS_UG_FREE					GAME_STATUS_FREE					//����״̬
#define GS_UG_CALL					(GAME_STATUS_PLAY+1)				//����״̬
#define GS_UG_BACK					(GAME_STATUS_PLAY+2)				//����״̬
#define GS_UG_PLAY					(GAME_STATUS_PLAY+3)				//��Ϸ״̬

#define VERSION_SERVER			    PROCESS_VERSION(6,0,3)				//����汾
#define VERSION_CLIENT				PROCESS_VERSION(6,0,3)				//����汾

//////////////////////////////////////////////////////////////////////////
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
#define SUB_S_STRUSTEE				109									//�û��й�
#define SUB_S_REQUEST_LEAVE			110									//�����뿪
#define SUB_S_BATCH_CARD			111									//��������

//�Ƹ�����
#define PACK_COUNT					4									//�����
#define CELL_PACK					54									//��Ԫ��Ŀ
#define MAX_COUNT					PACK_COUNT*CELL_PACK/GAME_PLAYER+8	//�����Ŀ


#ifndef _UNICODE
#define myprintf	myprintf
#define mystrcpy	strcpy
#define mystrlen	strlen
#define myscanf		_snscanf
#define	myLPSTR		LPCSTR
#else
#define myprintf	swprintf
#define mystrcpy	wcscpy
#define mystrlen	wcslen
#define myscanf		_snwscanf
#define	myLPSTR		LPWSTR
#endif

//��Ϸ״̬
struct CMD_S_StatusFree
{
	LONGLONG							lBaseScore;							//��������
	//�йܱ���
	bool							bStrustee[GAME_PLAYER];				//�йܱ�־
};

//����״̬
struct CMD_S_StatusCall
{
	//��Ϸ����
	WORD							wBankerUser;						//ׯ���û�
	BYTE							cbPackCount;						//������Ŀ
	BYTE							cbMainValue;						//������ֵ
	BYTE							cbValueOrder[2];					//������ֵ

	//���Ʊ���
	BYTE							cbCardCount;						//�˿���Ŀ
	BYTE							cbCardData[MAX_COUNT];				//�˿��б�

	//������Ϣ
	BYTE							cbComplete;							//��ɱ�־
	BYTE							cbCallCard;							//�����˿�
	BYTE							cbCallCount;						//������Ŀ
	WORD							wCallCardUser;						//�����û�

	//�йܱ���
	bool							bStrustee[GAME_PLAYER];				//�йܱ�־
};

//����״̬
struct CMD_S_StatusBack
{
	//��Ϸ����
	BYTE							cbPackCount;						//������Ŀ
	BYTE							cbMainValue;						//������ֵ
	BYTE							cbMainColor;						//������ֵ
	BYTE							cbValueOrder[2];					//������ֵ

	//������Ϣ
	BYTE							cbCallCard;							//�����˿�
	BYTE							cbCallCount;						//������Ŀ
	WORD							wCallCardUser;						//�����û�

	//�û�����
	WORD							wBankerUser;						//ׯ���û�
	WORD							wCurrentUser;						//��ǰ�û�

	//�˿˱���
	BYTE							cbCardCount;						//�˿���Ŀ
	BYTE							cbCardData[MAX_COUNT];				//�˿��б�

	//������Ϣ
	BYTE							cbConcealCount;						//������Ŀ
	BYTE							cbConcealCard[8];					//�����˿�

	//�йܱ���
	bool							bStrustee[GAME_PLAYER];				//�йܱ�־
};

//��Ϸ״̬
struct CMD_S_StatusPlay
{
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

	//�˿˱���
	BYTE							cbCardCount;						//�˿���Ŀ
	BYTE							cbCardData[MAX_COUNT];				//�˿��б�

	//������Ϣ
	BYTE							cbConcealCount;						//������Ŀ
	BYTE							cbConcealCard[8];					//�����˿�

	//���Ʊ���
	BYTE							cbOutCardCount[GAME_PLAYER];				//������Ŀ
	BYTE							cbOutCardData[GAME_PLAYER][MAX_COUNT];		//�����б�
	BYTE							cbLastTurnCard[GAME_PLAYER][MAX_COUNT];		//�ϴγ���
	BYTE							cbLastTurnCount[GAME_PLAYER];				//�ϴγ�����

	//�÷ֱ���
	BYTE							cbScoreCardCount;					//�˿���Ŀ
	BYTE							cbScoreCardData[PACK_COUNT*12];		//�÷��˿�

	//�йܱ���
	bool							bStrustee[GAME_PLAYER];				//�йܱ�־
};

//��Ϸ��ʼ
struct CMD_S_GameStart
{
	WORD							wBankerUser;						//ׯ���û�
	BYTE							cbPackCount;						//������Ŀ
	BYTE							cbMainValue;						//������ֵ
	BYTE							cbValueOrder[2];					//������ֵ
};

//�����˿�
struct CMD_S_SendCard
{
	BYTE							cbCardCount;						//�˿���Ŀ
	BYTE							cbCardData[MAX_COUNT];				//�˿��б�
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
	BYTE							cbConcealCount;						//������Ŀ
	BYTE							cbConcealCard[8];					//�����˿�
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
	BYTE							cbCardData[MAX_COUNT];				//�˿��б�
};

//˦�ƽ��
struct CMD_S_ThrowResult
{
	WORD							wOutCardUser;						//�������
	WORD				 			wCurrentUser;						//��ǰ���
	BYTE							cbThrowCardCount;					//�˿���Ŀ
	BYTE							cbResultCardCount;					//�˿���Ŀ
	BYTE							cbCardDataArray[2*MAX_COUNT];		//�˿�����
};

//һ��ͳ��
struct CMD_TurnBalance
{
	WORD							wTurnWinner;						//һ��ʤ��
	WORD				 			wCurrentUser;						//��ǰ���
};

//��Ϸ����
struct CMD_S_GameEnd
{
	LONGLONG							lScore[GAME_PLAYER];				//�û��÷�
	WORD							wGameScore;							//��Ϸ�÷�
	WORD							wConcealTime;						//�۵ױ���
	WORD							wConcealScore;						//���ƻ���
	BYTE							cbConcealCount;						//������Ŀ
	BYTE							cbConcealCard[8];					//�����˿�
};

//�û��й�
struct CMD_S_Strustee
{
	WORD							wChairId;							//�й��û�
	bool							bStrustee;							//�Ƿ��й�
};

//�����뿪
struct CMD_S_RequestLeave
{
	WORD							wChairID;							//�뿪�û�
	TCHAR							szLeaveReason[128];					//�뿪ԭ��
};

//��������
struct CMD_S_BatchCard
{
	BYTE							cbCardCount;						//�˿���Ŀ
	WORD							wOutCardUser;						//�������
	WORD							wTurnWinner;						//һ��ʤ��
	BYTE							cbCardData[PACK_COUNT*CELL_PACK];	//�˿��б�
};


//////////////////////////////////////////////////////////////////////////
//�ͻ�������ṹ

#define SUB_C_CALL_CARD				1									//�û�����
#define SUB_C_CALL_FINISH			2									//�������
#define SUB_C_CONCEAL_CARD			3									//�����˿�
#define SUB_C_OUT_CARD				4									//�û�����
#define SUB_C_STRUSTEE				5									//�û��й�
#define SUB_C_REQUEST_LEAVE			6									//�����뿪
#define SUB_C_RESPONSES_LEAVE		7									//��Ӧ�뿪

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
	BYTE							cbCardData[MAX_COUNT];				//�˿��б�
};

//�û��й�
struct CMD_C_Strustee
{
	bool bStrustee;														//�Ƿ��й�
};

//�����뿪
struct CMD_C_RequestLeave
{
	TCHAR							szLeaveReason[128];					//�뿪ԭ��
};

//�����Ӧ
struct CMD_C_ResponsesLeave
{
	WORD							wChairID;							//�뿪�û�
	BYTE							cbAgreeLeave;						//ͬ���뿪
};

//////////////////////////////////////////////////////////////////////////

#endif
