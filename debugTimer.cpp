//=====================================
//
//�e���v���[�g����[template.cpp]
//Author:GP11A341 21 ���ԗY��
//
//=====================================
#include "DebugTimer.h"
#include "debugWindow.h"
#include <string.h>
#include <assert.h>
#include <vector>

/**************************************
�}�N����`
***************************************/
#define DEBUGTIMER_TAG_MAX			(36)
#define DEBUGTIMER_LABEL_MAX		(36)
#define DEBUGTIMER_COUNT_INTERBAL	(20)
#define DEBUGTIMER_BAR_SIZE			(D3DXVECTOR2(75.0f, 0.0f))
#define USE_DEBUGTIMER

/**************************************
�\���̒�`
***************************************/
#ifdef USE_DEBUGTIMER
typedef struct _TIMERNODE {
	char tag[DEBUGTIMER_TAG_MAX];
	LARGE_INTEGER start, end;
	_TIMERNODE *next;
	bool isCountEnd;
}TIMERNODE;

typedef struct _DEBUGTIMER {
	char label[DEBUGTIMER_LABEL_MAX];
	TIMERNODE *latest;
	TIMERNODE *head;
	_DEBUGTIMER *next;
}DEBUGTIMER;
#endif
/**************************************
�O���[�o���ϐ�
***************************************/
#ifdef USE_DEBUGTIMER
static DEBUGTIMER *head = NULL;
static DWORD cntFrame = 0;
#endif
/**************************************
�v���g�^�C�v�錾
***************************************/

/**************************************
�I������
***************************************/
void UninitDebugTimer(void)
{
#ifdef USE_DEBUGTIMER
	DEBUGTIMER *ptr = head;
	while (ptr != NULL)
	{
		DEBUGTIMER *tmp = ptr->next;

		TIMERNODE *node = ptr->latest;
		while (node != NULL)
		{
			TIMERNODE *tmpNode = node->next;
			free(node);
			node = tmpNode;
		}

		free(ptr);
		ptr = tmp;
	}
#endif
}

/**************************************
�V�K�^�C�}�[�o�^����
***************************************/
bool RegisterDebugTimer(const char* label)
{
#ifdef USE_DEBUGTIMER
	for (DEBUGTIMER *ptr = head; ptr != NULL; ptr = ptr->next)
	{
		if (strcmp(label, ptr->label) == 0)
			return true;
	}

	DEBUGTIMER *p = (DEBUGTIMER*)malloc(sizeof(DEBUGTIMER));
	if (!p)
	{
		assert(p);
		return false;
	}

	ZeroMemory(p, sizeof(DEBUGTIMER));

	strcpy_s(p->label, DEBUGTIMER_LABEL_MAX, label);
	p->next = head;
	head = p;
	return true;
#else
	return false;
#endif
}

/**************************************
�^�C�}�[�v������
***************************************/
bool CountDebugTimer(const char* label, const char* tag)
{
#ifdef USE_DEBUGTIMER
	DEBUGTIMER *ptr = head;
	while (ptr != NULL)
	{
		if (strcmp(ptr->label, label) == 0)
			break;

		ptr = ptr->next;
	}

	if (ptr == NULL)
		return false;

	//�^�O����o�^
	if (ptr->head == NULL)
	{
		TIMERNODE *p = (TIMERNODE*)malloc(sizeof(TIMERNODE));
		ZeroMemory(p, sizeof(TIMERNODE));
		strcpy_s(p->tag, DEBUGTIMER_TAG_MAX, tag);
		ptr->latest = ptr->head = p;
		return true;
	}

	//�^�O�ǉ��o�^
	TIMERNODE *node = ptr->latest;
	while (node != NULL)
	{
		if (strcmp(node->tag, tag) == 0)
			break;

		node = node->next;
	}

	if (node == NULL)
	{
		node = (TIMERNODE*)malloc(sizeof(TIMERNODE));
		ZeroMemory(node, sizeof(TIMERNODE));
		strcpy_s(node->tag, DEBUGTIMER_TAG_MAX, tag);
		node->next = ptr->latest;
		ptr->latest = node;
	}

	//�^�C�}�[�J�E���g�擾
	if (node->isCountEnd)
		GetTimerCount(&node->end);
	else
	{
		GetTimerCount(&node->start);
		node->isCountEnd = true;
	}

	return true;
#else
	return false;
#endif
}

/**************************************
�f�o�b�O�^�C�}�[�`�揈��
***************************************/
void DrawDebugTimer(const char *label)
{
#ifdef USE_DEBUGTIMER
	DEBUGTIMER *ptr = head;
	while (ptr != NULL)
	{
		if (strcmp(ptr->label, label) == 0)
			break;

		ptr = ptr->next;
	}

	if (ptr == NULL || ptr->latest == NULL)
		return;

	BeginDebugWindow(label);
	DebugText("FPS : %d", GetCurrentFPS());

	double progress = 0.0f;
	for (TIMERNODE *node = ptr->latest; node != NULL; node = node->next)
	{
		progress += CalcProgressTime(node->start, node->end);
	}
	DebugText("%3f[msec]", progress);

	for (TIMERNODE *node = ptr->latest; node != NULL; node = node->next)
	{
		double diff = CalcProgressTime(node->start, node->end);
		DebugProgressBar((float)(diff / progress), NULL, DEBUGTIMER_BAR_SIZE);
		DebugSameLine();
		DebugText("%s : %3f[msec]", node->tag, diff);
		node->isCountEnd = false;
	}

	EndDebugWindow(label);
#endif
}