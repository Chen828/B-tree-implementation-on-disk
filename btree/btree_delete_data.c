// 3 ɾ��
/*
B����ɾ������
ɾ��������ָ������keyɾ����¼�����B���еļ�¼�в����Ӧkey�ļ�¼����ɾ��ʧ�ܡ�
1�������ǰ��Ҫɾ����keyλ�ڷ�Ҷ�ӽ���ϣ����ú��key������ĺ��key��ָ��̼�¼����˼������Ҫɾ����key��
   Ȼ���ں��key���ڵ���֧��ɾ���ú��key����ʱ���keyһ��λ��Ҷ�ӽ���ϣ�������̺Ͷ���������ɾ�����ķ�ʽ���ơ�
   ɾ�������¼��ִ�е�2��
2���ý��key�������ڵ���Math.ceil(m/2)-1������ɾ������������ִ�е�3����
3������ֵܽ��key��������Math.ceil(m/2)-1���򸸽���е�key���Ƶ��ý�㣬�ֵܽ���е�һ��key���ƣ�ɾ������������
   ����
   ��������е�key�����뵱ǰ��㼰�����ֵܽ���е�key�ϲ����γ�һ���µĽ�㡣
   ԭ������е�key����������ָ��ͱ����һ������ָ�룬ָ������½�㡣Ȼ��ǰ����ָ��ָ�򸸽�㣬�ظ��ϵ�2����
*/
#include "pch.h"

extern struct disk_file_information disk_file[FILE_AMOUNT];	//ÿ�������ļ�����Ϣ//b+tree.c
extern struct config_information config_inf;				//config��Ϣ�б�
extern struct fat_block_structure fat_s;	//b+tree.c


int btree_delete_data(int key, int index_block, Status *flag) {
	bnode node, parent_node, bro_node, child_node;
	int unit = 0;
	int data_block;
	long position, fat_position, position2;
	int i, bro_i, j, middle_count, key_list[m];

	//printf("=============== 5 ɾ������ ===============\n");


	//��1�����ݿ��ÿ�
	position = index_block * config_inf.block_size * 1024 + sizeof(int);	//Խ�����
	fseek(disk_file[0].file_point, position, 0);
	fread(&node, sizeof(node), 1, disk_file[0].file_point);

	for (i = 0; i < node.keynum; i++)
	{
		if (node.key[i] == key)
		{
			data_block = node.key_block[i];
			break;
		}
	}

	//Ŀ��ؼ��ֵ�fat_block��
	fat_position = ((data_block / fat_s.fat_value_amount + 1) + index_block) * sizeof(int);
	fseek(disk_file[0].file_point, fat_position, 0);
	fwrite(&unit, sizeof(int), 1, disk_file[0].file_point);	//unit��Ŀ��ؼ��ֵ�block��

	if (node.child_block_num[i] != 0)
	{
		//��Ҷ�ӽڵ�
		//			���ݿ��ÿ�
		printf("��Ҷ�ӽڵ㣬���ؼ������ӽ��\n");
		position = node.child_block_num[i] * config_inf.block_size * 1024 + sizeof(int);	//Խ�����
		fseek(disk_file[0].file_point, position, 0);
		fread(&child_node, sizeof(node), 1, disk_file[0].file_point);

		node.key[i] = child_node.key[child_node.keynum - 1];
		node.key_block[i] = child_node.key_block[child_node.keynum - 1];

		btree_delete_data(child_node.key[child_node.keynum - 1], node.child_block_num[i], flag);
	}
	else
	{
		//Ҷ�ӽڵ�
		printf("Ҷ�ӽڵ�\n");
		//node.keynum -= 1;
		node.key[i] = 0;
		node.key_block[i] = 0;
	}

	//��2���ý��key�������ڵ���Math.ceil(m/2)-1������ɾ������������ִ�е�3����
	if (node.keynum >= m / 2 - 1)
	{
		position = index_block * config_inf.block_size * 1024 + sizeof(int);	//Խ�����
		fseek(disk_file[0].file_point, position, 0);
		fwrite(&node, sizeof(node), 1, disk_file[0].file_point);

		printf("ɾ���ɹ���\n");

		return ok;
	}
	else   //��3��
	{
		position = node.parent_block_num * config_inf.block_size * 1024 + sizeof(int);	//Խ�����
		fseek(disk_file[0].file_point, position, 0);
		fread(&parent_node, sizeof(node), 1, disk_file[0].file_point);

		//��λ�ùؼ��֣�Ѱ��ǰһ�ֵ�
		for (i = 0; i < parent_node.keynum; i++)
		{
			if (parent_node.child_block_num[i] == index_block)
			{
				break;
			}
		}

		if (i - 1 >= 0)	//��ǰ�ֵ�
		{
			//i -= 1ǰһ�ֵ�
			position2 = parent_node.child_block_num[i - 1] * config_inf.block_size * 1024 + sizeof(int);	//Խ�����
			fseek(disk_file[0].file_point, position2, 0);
			fread(&bro_node, sizeof(node), 1, disk_file[0].file_point);

			if (bro_node.keynum > m / 2 - 1)	//����, �ܽ�
			{
				//ȡ��˫����i-1����ֵ�Ǹýڵ�����Сֵ��������
				key = parent_node.key[i - 1];
				for (j = 0; j < node.keynum; j++)
				{
					if (key <= node.key[j])
					{
						middle_count = node.key[j];
						node.key[j] = key;
						key = middle_count;
					}
					printf(" node.key[%d] = %d\n", j, node.key[j]);
				}
				node.key[node.keynum] = key;     //��node.keynum��key����������	//key��С��keyֵ�䣬���򲻱�
				printf(" node.key[%d] = %d\n", node.keynum, node.key[node.keynum]);
				//node.keynum++;
				printf(" ˫����i-1ֵ����\n");
				printf("==============\n");

				parent_node.key[i - 1] = bro_node.key[bro_node.keynum - 1];	//�ֵ�����ϵ�˫��i - 1

				btree_delete_data(bro_node.key[bro_node.keynum - 1], parent_node.child_block_num[i - 1], flag);

				return ok;
			}
			else
			{
				/*
				��������е�key�����뵱ǰ��㼰�����ֵܽ���е�key�ϲ����γ�һ���µĽ�㡣
				ԭ������е�key����������ָ��ͱ����һ������ָ�룬ָ������½�㡣
				Ȼ��ǰ����ָ��ָ�򸸽�㣬�ظ��ϵ�2����
				*/

				//�ڵ���
				//�ݴ�ýڵ�ֵ
				for (j = 0; j < node.keynum; j++)
				{
					key_list[j] = node.key[j];
				}
				//���¸�ֵ���ֵܼ���
				for (j = 0; j < bro_node.keynum; j++)
				{
					node.key[j] = bro_node.key[j];
				}
				//���¸�ֵ��ȡ��˫����i-1����
				node.key[j] = parent_node.key[i - 1];
				//�ݴ�ĸýڵ�ֵ����д��
				for (j = 0; j < node.keynum; j++)
				{
					node.key[j] = key_list[j];
				}

				//ɾ���ֵ�ָ�뼰��ڵ�
				//Ŀ��ؼ��ֵ�fat_block��
				fat_position = ((parent_node.child_block_num[i - 1] / fat_s.fat_value_amount + 1) + index_block) * sizeof(int);
				fseek(disk_file[0].file_point, fat_position, 0);
				fwrite(&unit, sizeof(int), 1, disk_file[0].file_point);	//д��

				//˫�ױ仯��λ�ú�Ĺؼ��֣�����ָ��ǰ�ƣ�ԭ���һ������д��
				for (j = i; j < node.keynum; j++)
				{
					//���һλһ�����еģ�����0
					parent_node.key[j - 1] = parent_node.key[j];
					parent_node.key_block[j - 1] = parent_node.key_block[j];
					parent_node.child_block_num[j - 1] = parent_node.child_block_num[j];
				}
			}

		}
		//��ǰ�ڵ�
		else
		{

		}
	}	
}