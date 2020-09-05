// 2����
/*
B���Ĳ������
���������ָ����һ����¼������key, value���ļ�ֵ�ԡ����B�����Ѵ�����Ҫ����ļ�ֵ�ԣ�
������Ҫ�����value�滻�ɵ�value����B�����������key,��һ������Ҷ�ӽ���н��в��������
1������Ҫ�����key��ֵ���ҵ�Ҷ�ӽ�㲢���롣
2���жϵ�ǰ���key�ĸ����Ƿ�С�ڵ���m-1���������������������е�3����
3���Խ���м��keyΪ���ķ��ѳ����������֣�Ȼ������м��key���뵽������У����key��������ָ����Ѻ����벿�֣�
   ���key������ָ֧����Ѻ���Ұ벿�֣�Ȼ�󽫵�ǰ���ָ�򸸽�㣬�������е�3����
*/
#include "pch.h"


extern struct config_information config_inf;	//config��Ϣ�б� b+tree.c
extern struct disk_file_information disk_file[FILE_AMOUNT];	//ÿ�������ļ�����Ϣ b+tree.c
extern struct save_content s_c[content_num];					//Ŀ¼��

extern int btree_block_apply();


void btree_insert_data(int table_num, int key, int index_num, int data_num) {
	//����ؼ���

	bnode node;
	int unit;
	int block_index0, block_index1;
	long position;

	int i, middle_count;			//�ؼ�������
	printf("==============\n");

	position = index_num * config_inf.block_size * 1024;
	fseek(disk_file[0].file_point, position + sizeof(int), 0);				//��λ�����ļ���ʼ
	fread(&node, sizeof(bnode), 1, disk_file[0].file_point);

	//����ؼ���
	//printf("�ؼ�������\n");
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
	node.key[node.keynum] = key;     //��node.keynum��key����������	//key��С��keyֵ�䣬���򲻱�
	printf(" node.key[%d] = %d\n", node.keynum, node.key[node.keynum]);
	node.keynum++;
	printf("==============\n");

	//д���Ա������½ڵ�ʱ��ȡ��ȷ
	fseek(disk_file[0].file_point, position + sizeof(int), 0);				//��λ�����ļ���ʼ
	fwrite(&node, sizeof(bnode), 1, disk_file[0].file_point);

	if (node.keynum == m)
	{
		printf(" node.keynum = %d,", m);

		//int middle_num = m / 2 + 1;				//�м�ؼ������   ��ʡ�ռ�

		if (index_num != s_c[table_num].index_block)
		{
			//disk_file[0].file_block_amount�׿��������
			//���׿������飬��������"һ��"���п�
			printf(" �Ǹ��ڵ����\n");
																										
			//����տ�,д����һ��ؼ��֣���m / 2 + 1��ʼΪ�м�ֵ����һ�ڵ����
			//block_index1 = btree_insert_node(node.key[m / 2 + 1], node.parent_block_num, node.key_block[m / 2 + 1]);
			block_index1 = btree_insert_node(0, 1, node.parent_block_num, node.key, node.key_block, node.child_block_num);
			//0���ƣ�1���ϴ��ؼ��֣�����ָ���һд��
			/*
			for (i = m / 2 + 1 + 1; i <= m; i++)
			{
				btree_insert_data(table_num, node.key[i], block_index1, node.key_block[i]);
			}*/
			
			////[m / 2]Ϊ��m / 2 + 1�����ݣ���Ϊ�м�ֵ����һ�ڵ���룬����0��ţ�
			btree_insert_data(table_num, node.key[m / 2], node.parent_block_num, node.key_block[m / 2]);	//�����ϴ�			
			


			//��0
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
			//�׿�������ţ���������"����"���п죬���������׿飬���������ݷֵ����ӿ飬ʹ������һ��
			printf(" ���ڵ����\n");
			//node.leaf = 0;	//��Ϊ��֧�ڵ�

			//����տ�
			//	����
			//block_index0 = btree_insert_node(node.key[0], index_num, node.key_block[0]);
			block_index0 = btree_insert_node(1, 0, index_num, node.key, node.key_block, node.child_block_num);
			/*for (i = 1; i < m / 2; i++)
			{
				btree_insert_data(table_num, node.key[i], block_index0, node.key_block[i]);
			}*/
			//	�Һ���
			//printf("   node.key[m / 2 + 1] = %d\n", node.key[m / 2 + 1]);
			//block_index1 = btree_insert_node(node.key[m / 2 + 1], index_num, node.key_block[m / 2 + 1]);
			block_index1 = btree_insert_node(0, 0, index_num, node.key, node.key_block, node.child_block_num);
			/*for (i = m / 2 + 2 + 1; i <= m; i++)
			{
				btree_insert_data(table_num, node.key[i], block_index1, node.key_block[i]);
			}*/

			//�ؼ��ֵ���
			node.key[0] = node.key[m / 2];
			node.key_block[0] = node.key_block[m / 2];
			node.keynum = 1;
			//��0
			for ( i = 1; i < m; i++)
			{
				node.key[i] = 0;
				node.key_block[i] = 0;
			}

			//ָ�����
			node.child_block_num[0] = block_index0;
			node.child_block_num[1] = block_index1;
			//��0
			for (i = 2; i <= m; i++)
			{
				node.child_block_num[i] = 0;
			}
		}
	}
	/*
	else
	{
		//����ָ��
		node.child_block_num[0] = block_index0;
		node.child_block_num[1] = block_index1;
		node.child_block_num[2] = 0;
	}*/

	//����д��
	fseek(disk_file[0].file_point, position + sizeof(int), 0);				//��λ�����ļ���ʼ
	fwrite(&node, sizeof(bnode), 1, disk_file[0].file_point);
}


//int btree_insert_node(int key, int prior_block_index, int block_data) {
int btree_insert_node(int left, int up_data, int parent_block_num, int key[], int key_block[], int child_block_num[]) {
	bnode node, parent_node, child_node;
	int child_node_key[m + 1] = { 0 }, middle_count, middle_index;			//�ؼ�������
	int block_index, position;
	int i;		//ѭ������

	printf(" �����������ڵ�\n");

	//�ڵ��ʼ��
	if (left)	//�������
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
					node.child_block_num[i] = child_block_num[i];	//βָ�븴��
				}
				else
				{
					node.child_block_num[i] = 0;
				}
			}
		}
	}
	else        //�������
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
					node.child_block_num[i] = child_block_num[m / 2 + 1 + i];	//βָ�븴��
				}
				else
				{
					node.child_block_num[i] = 0;
				}
			}
		}
	}
	
	node.child_block_num[m] = 0;	//�����0��ʼ�ơ�0��mΪm+1����
	node.keynum = m - m / 2 - 1;
	//node.leaf = 1;	//��Ҷ�ӽڵ㣿
	node.parent_block_num = parent_block_num;

	//�����½ڵ�
	block_index = btree_block_apply();
	//��������
	position = block_index * config_inf.block_size * 1024;
	fseek(disk_file[0].file_point, position + sizeof(int), 0);				//��λ�����ļ���ʼ
	fwrite(&node, sizeof(bnode), 1, disk_file[0].file_point);

	//����ڵ�
	for ( i = 0; i < node.keynum; i++)
	{
		printf("   node.key[%d] = %d  ", i, node.key[i]);
		printf("   node.key_block[%d] = %d  ", i, node.key_block[i]);
		printf("   node.child_block_num[%d] = %d \n", i, node.child_block_num[i]);
	}
	printf("   node.child_block_num[%d] = %d \n", node.keynum, node.child_block_num[node.keynum]);
	printf(" keynum = %d, node.parent_block_num = %d\n", node.keynum, node.parent_block_num);


	
	//˫����д���ӽڵ㣬������
	//��˫�׽ڵ�
	position = parent_block_num * config_inf.block_size * 1024;
	fseek(disk_file[0].file_point, position + sizeof(int), 0);				//��λ�����ļ���ʼ
	fread(&parent_node, sizeof(bnode), 1, disk_file[0].file_point);
	//��ÿ������ָ���е�һ���ؼ���
	for (i = 0; i < parent_node.keynum; i++)
	{
		if (parent_node.child_block_num[i] != 0)
		{
			position = parent_node.child_block_num[i] * config_inf.block_size * 1024;
			fseek(disk_file[0].file_point, position + sizeof(int), 0);				//��λ�����ļ���ʼ
			fread(&child_node, sizeof(bnode), 1, disk_file[0].file_point);

			child_node_key[i] = child_node.key[0];
		}
		else
		{
			child_node_key[i] = 0;
		}
	}
	//���ݹؼ��ֽ�����ָ������
	middle_index = block_index;
	for (int i = 0; i < parent_node.keynum; i++)
	{
		if (key <= child_node_key[i])
		{
			//�����ؼ���
			middle_count = child_node_key[i];
			child_node_key[i] = key;
			key = middle_count;

			//����˫���еĺ���ָ��
			middle_count = parent_node.child_block_num[i];
			parent_node.child_block_num[i] = middle_index;
			middle_index = middle_count;
		}
		else if (child_node_key[i] == 0)
		{
			//�����ؼ���
			middle_count = child_node_key[i];
			child_node_key[i] = key;
			key = middle_count;

			//����˫���еĺ���ָ��
			middle_count = parent_node.child_block_num[i];
			parent_node.child_block_num[i] = middle_index;
			middle_index = middle_count;
			break;
		}
	}
	//��node.keynum��num����������	//middle_index��С��middle_indexֵ�䣬���򲻱�
	if (up_data) {
		parent_node.child_block_num[parent_node.keynum + 1] = middle_index;
	}
	else
	{
		parent_node.child_block_num[parent_node.keynum] = middle_index;
	}

	//����ڵ�
	printf(" ˫�׽ڵ㣡\n");
	for (i = 0; i < parent_node.keynum; i++)
	{
		printf("   node.key[%d] = %d  ", i, parent_node.key[i]);
		printf("   node.key_block[%d] = %d  ", i, parent_node.key_block[i]);
		printf("   node.child_block_num[%d] = %d \n", i, parent_node.child_block_num[i]);
	}
	printf("   node.child_block_num[%d] = %d \n", parent_node.keynum, parent_node.child_block_num[parent_node.keynum]);
	printf(" keynum = %d, node.parent_block_num = %d\n", parent_node.keynum, parent_node.parent_block_num);

	//д˫�׽ڵ�
	position = parent_block_num * config_inf.block_size * 1024;
	fseek(disk_file[0].file_point, position + sizeof(int), 0);				//��λ�����ļ���ʼ
	fwrite(&parent_node, sizeof(bnode), 1, disk_file[0].file_point);
	
	printf(" ���������\n");
	printf("==============\n");
	return block_index;
}
