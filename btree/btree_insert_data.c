// 2插入
/*
B树的插入操作
插入操作是指插入一条记录，即（key, value）的键值对。如果B树中已存在需要插入的键值对，
则用需要插入的value替换旧的value。若B树不存在这个key,则一定是在叶子结点中进行插入操作。
1）根据要插入的key的值，找到叶子结点并插入。
2）判断当前结点key的个数是否小于等于m-1，若满足则结束，否则进行第3步。
3）以结点中间的key为中心分裂成左右两部分，然后将这个中间的key插入到父结点中，这个key的左子树指向分裂后的左半部分，
   这个key的右子支指向分裂后的右半部分，然后将当前结点指向父结点，继续进行第3步。
*/
#include "pch.h"


extern struct config_information config_inf;	//config信息列表 b+tree.c
extern struct disk_file_information disk_file[FILE_AMOUNT];	//每个磁盘文件的信息 b+tree.c
extern struct save_content s_c[content_num];					//目录块

extern int btree_block_apply();


void btree_insert_data(int table_num, int key, int index_num, int data_num) {
	//插入关键字

	bnode node;
	int unit;
	int block_index0, block_index1;
	long position;

	int i, middle_count;			//关键字排序
	printf("==============\n");

	position = index_num * config_inf.block_size * 1024;
	fseek(disk_file[0].file_point, position + sizeof(int), 0);				//定位索引文件起始
	fread(&node, sizeof(bnode), 1, disk_file[0].file_point);

	//插入关键字
	//printf("关键字排序！\n");
	for (i = 0; i < node.keynum; i++)
	{
		if (key <= node.key[i])
		{
			middle_count = node.key[i];
			node.key[i] = key;
			key = middle_count;
		}
		printf(" node.key[%d] = %d\n", i, node.key[i]);
	}
	node.key[node.keynum] = key;     //第node.keynum个key存最大的数据	//key最小则key值变，否则不变
	printf(" node.key[%d] = %d\n", node.keynum, node.key[node.keynum]);
	node.keynum++;
	printf("==============\n");

	//写入以被插入新节点时读取正确
	fseek(disk_file[0].file_point, position + sizeof(int), 0);				//定位索引文件起始
	fwrite(&node, sizeof(bnode), 1, disk_file[0].file_point);

	if (node.keynum == m)
	{
		printf(" node.keynum = %d,", m);

		//int middle_num = m / 2 + 1;				//中间关键字序号   节省空间

		if (index_num != s_c[table_num].index_block)
		{
			//disk_file[0].file_block_amount首块索引块号
			//非首块索引块，分裂申请"一块"空闲快
			printf(" 非根节点分裂\n");
																										
			//申请空块,写入另一半关键字，从m / 2 + 1开始为中间值向上一节点插入
			//block_index1 = btree_insert_node(node.key[m / 2 + 1], node.parent_block_num, node.key_block[m / 2 + 1]);
			block_index1 = btree_insert_node(0, 1, node.parent_block_num, node.key, node.key_block, node.child_block_num);
			//0左复制，1向上传关键字，孩子指针加一写入
			/*
			for (i = m / 2 + 1 + 1; i <= m; i++)
			{
				btree_insert_data(table_num, node.key[i], block_index1, node.key_block[i]);
			}*/
			
			////[m / 2]为第m / 2 + 1个数据，作为中间值向上一节点插入，（从0编号）
			btree_insert_data(table_num, node.key[m / 2], node.parent_block_num, node.key_block[m / 2]);	//索引上传			
			


			//清0
			for (i = m / 2; i < m; i++)
			{
				node.key[i] = 0;
				node.key_block[i] = 0;
				node.child_block_num[i] = 0;
			}
			node.child_block_num[m] = 0;
			node.keynum = m / 2;
		}
		else
		{
			//首块索引块号，分裂申请"两块"空闲快，自身仍做首块，将自身数据分到两子块，使得树长一层
			printf(" 根节点分裂\n");
			//node.leaf = 0;	//设为分支节点

			//申请空块
			//	左孩子
			//block_index0 = btree_insert_node(node.key[0], index_num, node.key_block[0]);
			block_index0 = btree_insert_node(1, 0, index_num, node.key, node.key_block, node.child_block_num);
			/*for (i = 1; i < m / 2; i++)
			{
				btree_insert_data(table_num, node.key[i], block_index0, node.key_block[i]);
			}*/
			//	右孩子
			//printf("   node.key[m / 2 + 1] = %d\n", node.key[m / 2 + 1]);
			//block_index1 = btree_insert_node(node.key[m / 2 + 1], index_num, node.key_block[m / 2 + 1]);
			block_index1 = btree_insert_node(0, 0, index_num, node.key, node.key_block, node.child_block_num);
			/*for (i = m / 2 + 2 + 1; i <= m; i++)
			{
				btree_insert_data(table_num, node.key[i], block_index1, node.key_block[i]);
			}*/

			//关键字调整
			node.key[0] = node.key[m / 2];
			node.key_block[0] = node.key_block[m / 2];
			node.keynum = 1;
			//清0
			for ( i = 1; i < m; i++)
			{
				node.key[i] = 0;
				node.key_block[i] = 0;
			}

			//指针调整
			node.child_block_num[0] = block_index0;
			node.child_block_num[1] = block_index1;
			//清0
			for (i = 2; i <= m; i++)
			{
				node.child_block_num[i] = 0;
			}
		}
	}
	/*
	else
	{
		//建立指针
		node.child_block_num[0] = block_index0;
		node.child_block_num[1] = block_index1;
		node.child_block_num[2] = 0;
	}*/

	//重新写入
	fseek(disk_file[0].file_point, position + sizeof(int), 0);				//定位索引文件起始
	fwrite(&node, sizeof(bnode), 1, disk_file[0].file_point);
}


//int btree_insert_node(int key, int prior_block_index, int block_data) {
int btree_insert_node(int left, int up_data, int parent_block_num, int key[], int key_block[], int child_block_num[]) {
	bnode node, parent_node, child_node;
	int child_node_key[m + 1] = { 0 }, middle_count, middle_index;			//关键字排序
	int block_index, position;
	int i;		//循环变量

	printf(" 插入新索引节点\n");

	//节点初始化
	if (left)	//向左添加
	{
		for (i = 0; i < m; i++)
		{
			if (i < m / 2)
			{
				node.key[i] = key[i];
				node.key_block[i] = key_block[i];
				node.child_block_num[i] = child_block_num[i];
			}
			else {
				node.key[i] = 0;
				node.key_block[i] = 0;
				if (i == m / 2)
				{
					node.child_block_num[i] = child_block_num[i];	//尾指针复制
				}
				else
				{
					node.child_block_num[i] = 0;
				}
			}
		}
	}
	else        //向右添加
	{
		for (i = 0; i < m; i++)
		{
			if (i + m / 2 + 1 < m)
			{
				node.key[i] = key[m / 2 + 1 + i];
				node.key_block[i] = key_block[m / 2 + 1 + i];
				node.child_block_num[i] = child_block_num[m / 2 + 1 + i];
			}
			else {
				node.key[i] = 0;
				node.key_block[i] = 0;
				if (i + m / 2 + 1 == m)
				{
					node.child_block_num[i] = child_block_num[m / 2 + 1 + i];	//尾指针复制
				}
				else
				{
					node.child_block_num[i] = 0;
				}
			}
		}
	}
	
	node.child_block_num[m] = 0;	//数组从0开始计。0到m为m+1个数
	node.keynum = m - m / 2 - 1;
	//node.leaf = 1;	//是叶子节点？
	node.parent_block_num = parent_block_num;

	//申请新节点
	block_index = btree_block_apply();
	//插入数据
	position = block_index * config_inf.block_size * 1024;
	fseek(disk_file[0].file_point, position + sizeof(int), 0);				//定位索引文件起始
	fwrite(&node, sizeof(bnode), 1, disk_file[0].file_point);

	//输出节点
	for ( i = 0; i < node.keynum; i++)
	{
		printf("   node.key[%d] = %d  ", i, node.key[i]);
		printf("   node.key_block[%d] = %d  ", i, node.key_block[i]);
		printf("   node.child_block_num[%d] = %d \n", i, node.child_block_num[i]);
	}
	printf("   node.child_block_num[%d] = %d \n", node.keynum, node.child_block_num[node.keynum]);
	printf(" keynum = %d, node.parent_block_num = %d\n", node.keynum, node.parent_block_num);


	
	//双亲中写孩子节点，并排序
	//读双亲节点
	position = parent_block_num * config_inf.block_size * 1024;
	fseek(disk_file[0].file_point, position + sizeof(int), 0);				//定位索引文件起始
	fread(&parent_node, sizeof(bnode), 1, disk_file[0].file_point);
	//读每个孩子指针中的一个关键字
	for (i = 0; i < parent_node.keynum; i++)
	{
		if (parent_node.child_block_num[i] != 0)
		{
			position = parent_node.child_block_num[i] * config_inf.block_size * 1024;
			fseek(disk_file[0].file_point, position + sizeof(int), 0);				//定位索引文件起始
			fread(&child_node, sizeof(bnode), 1, disk_file[0].file_point);

			child_node_key[i] = child_node.key[0];
		}
		else
		{
			child_node_key[i] = 0;
		}
	}
	//根据关键字将孩子指针排序
	middle_index = block_index;
	for (int i = 0; i < parent_node.keynum; i++)
	{
		if (key <= child_node_key[i])
		{
			//交换关键字
			middle_count = child_node_key[i];
			child_node_key[i] = key;
			key = middle_count;

			//交换双亲中的孩子指针
			middle_count = parent_node.child_block_num[i];
			parent_node.child_block_num[i] = middle_index;
			middle_index = middle_count;
		}
		else if (child_node_key[i] == 0)
		{
			//交换关键字
			middle_count = child_node_key[i];
			child_node_key[i] = key;
			key = middle_count;

			//交换双亲中的孩子指针
			middle_count = parent_node.child_block_num[i];
			parent_node.child_block_num[i] = middle_index;
			middle_index = middle_count;
			break;
		}
	}
	//第node.keynum个num存最大的数据	//middle_index最小则middle_index值变，否则不变
	if (up_data) {
		parent_node.child_block_num[parent_node.keynum + 1] = middle_index;
	}
	else
	{
		parent_node.child_block_num[parent_node.keynum] = middle_index;
	}

	//输出节点
	printf(" 双亲节点！\n");
	for (i = 0; i < parent_node.keynum; i++)
	{
		printf("   node.key[%d] = %d  ", i, parent_node.key[i]);
		printf("   node.key_block[%d] = %d  ", i, parent_node.key_block[i]);
		printf("   node.child_block_num[%d] = %d \n", i, parent_node.child_block_num[i]);
	}
	printf("   node.child_block_num[%d] = %d \n", parent_node.keynum, parent_node.child_block_num[parent_node.keynum]);
	printf(" keynum = %d, node.parent_block_num = %d\n", parent_node.keynum, parent_node.parent_block_num);

	//写双亲节点
	position = parent_block_num * config_inf.block_size * 1024;
	fseek(disk_file[0].file_point, position + sizeof(int), 0);				//定位索引文件起始
	fwrite(&parent_node, sizeof(bnode), 1, disk_file[0].file_point);
	
	printf(" 插入结束！\n");
	printf("==============\n");
	return block_index;
}
