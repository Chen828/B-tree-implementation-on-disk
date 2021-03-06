// 入门提示: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件

#ifndef PCH_H
#define PCH_H


//config
#define FILE_AMOUNT	10			//文件名个数
#define FILE_NAME_LENGTH  20	//文件名长度
#define Max_Line_LENGTH  1024	//每行最多读取字节数


//table数据表
#define DATA_AMOUNT	10			//table数据表中数据个数
#define MAN_NAME_LENGTH  16		//人名长度
//#define Max_FREE_AMOUNT  20	//最大申请空闲个数


//head
#include<stdio.h>
#include<stdlib.h>
#include <string.h>
#include <math.h>
#include<time.h>

//config的信息
struct config_information
{
	int block_size;		//系统block大小
	int disk_file_amount;	//系统文件个数
	int system_block_amount;	//block个数
};


//每个磁盘文件的信息
struct disk_file_information
{
	char file_name[FILE_NAME_LENGTH];
	int file_size;
	//int block_size;
	//int file_amount;
	int file_block_amount;	//block个数
	FILE *file_point;	//指向本文件的文件指针
};


//data_block structure
struct data_block_structure
{
	int block_num;
	int file_amount;
};


//fat_block structure
struct fat_block_structure
{
	//表项值：下一块地址；FF文件结束；BB坏；KB保留;0空闲。
	int block_num;//编号
	int fat_block_amount;//fat占用block个数
	int fat_value_amount;//fat的一个block放多少表项

};


//table数据表
struct data_structure
{
	int key;
	char name[MAN_NAME_LENGTH];
};


///////////////////////////   btree

#define MAXM 10                     //定义B树的最大的阶数

#define m  3                      //设定B树的阶数 
#define Max   m - 1                  //结点的最大关键字数量 
#define Min  (m - 1) / 2              //结点的最小关键字数量 
typedef int KeyType;                //KeyType为关键字类型


//管理block
#define content_num		2*1024/(4+4+20)-4	//-4:块号
struct save_content {
	char table_name[FILE_NAME_LENGTH];
	int data_block;
	int index_block;
};


typedef struct node {                //B树和B树结点类型 
	int keynum;                     //结点关键字个数

	KeyType key[m];              //关键字数组 
	int key_block[m];				//存储关键字的block

	int parent_block_num;            //双亲结点指针
	int child_block_num[m + 1];         //孩子结点指针数组 

	//int leaf;
	//1是叶子节点，0不是叶子节点.若是叶子节点，孩子指针则为关键字数据存储的block
}bnode;


typedef enum function {
	fat,		//fat管理
	btree		//btree管理
}Function;


typedef enum status {               //枚举类型（依次递增） 
	true,
	false,
	ok,
	error,
	overflow,
	empty
}Status;


////////////////////////

typedef struct node0 {                //B树和B树结点类型 
	int keynum;                     //结点关键字个数
	KeyType key[MAXM];              //关键字数组，key[0]不使用 
	struct node *parent;            //双亲结点指针
	struct node *ptr[MAXM];         //孩子结点指针数组 
	int leaf;						//1是叶子节点，0不是叶子节点
}BTNode, *BTree;


typedef struct {                     //B树查找结果类型 
	BTNode *pt;                     //指向找到的结点
	int i;                          //在结点中的关键字位置; 
	int tag;                        //查找成功与否标志
}Result;

typedef struct LNode {               //链表和链表结点类型 
	BTree data;                     //数据域
	struct LNode *next;             //指针域
}LNode, *LinkList;

typedef enum ope {               //枚举类型（依次递增） 
	query,
	insert,
	delete,
	read
}Ope;
/*
Status InitBTree(BTree *t);                                     //初始化B树 
int SearchBTNode(BTNode *p, KeyType k);                          //在结点p中查找关键字k的插入位置i 
Result SearchBTree(BTree t, KeyType k);                          //在树t上查找关键字k,返回结果(pt,i,tag)。若查找成功,则特征值
																//tag=1,关键字k是指针pt所指结点中第i个关键字；否则特征值tag=0,
																//关键字k的插入位置为pt结点的第i个
void InsertBTNode(BTNode *p, int i, KeyType k, BTNode *q);        //将关键字k和结点q分别插入到p->key[i+1]和p->ptr[i+1]中
void SplitBTNode(BTNode *p, BTNode *q);                        //将结点p分裂成两个结点,前一半保留,后一半移入结点q
void NewRoot(BTNode *t, KeyType k, BTNode *p, BTNode *q);         //生成新的根结点t,原结点p和结点q为子树指针
void InsertBTree(BTree *t, int i, KeyType k, BTNode *p);           //在树t上结点q的key[i]与key[i+1]之间插入关键字k。若引起
																 //结点过大,则沿双亲链进行必要的结点分裂调整,使t仍是B树
void Remove(BTNode *p, int i);                                   //从p结点删除key[i]和它的孩子指针ptr[i]
void Substitution(BTNode *p, int i);                             //查找被删关键字p->key[i](在非叶子结点中)的替代叶子结点(右子树中值最小的关键字) 
void MoveRight(BTNode *p, int i);                                //将双亲结点p中的最后一个关键字移入右结点q中
																//将左结点aq中的最后一个关键字移入双亲结点p中
void MoveLeft(BTNode *p, int i);                                 //将双亲结点p中的第一个关键字移入结点aq中，
																 //将结点q中的第一个关键字移入双亲结点p中
void Combine(BTNode *p, int i);                                  //将双亲结点p、右结点q合并入左结点aq，
																//并调整双亲结点p中的剩余关键字的位置
void AdjustBTree(BTNode *p, int i);                              //删除结点p中的第i个关键字后,调整B树                                                                
int FindBTNode(BTNode *p, KeyType k, int &i);                     //反映是否在结点p中是否查找到关键字k 
int BTNodeDelete(BTNode *p, KeyType k);                          //在结点p中查找并删除关键字k
void BTreeDelete(BTree &t, KeyType k);                           //构建删除框架，执行删除操作 
void DestroyBTree(BTree &t);                                    //递归释放B树
Status InitQueue(LinkList &L);                                  //初始化队列 
LNode* CreateNode(BTree t);                                     //新建一个结点 
Status Enqueue(LNode *p, BTree t);                               //元素q入队列
Status Dequeue(LNode *p, BTNode *&q);                          //出队列，并以q返回值
Status IfEmpty(LinkList L);                                     //队列判空 
void DestroyQueue(LinkList L);                                  //销毁队列 
Status Traverse(BTree t, LinkList L, int newline, int sum);        //用队列遍历输出B树 
Status PrintBTree(BTree t);                                     //输出B树 
void Test();                                                    //测试B树功能函数 
 */

#endif //PCH_H
