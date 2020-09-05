//6 �������ݱ�
#include "pch.h"


extern struct disk_file_information disk_file[FILE_AMOUNT];	//ÿ�������ļ�����Ϣ//b+tree.c
extern struct config_information config_inf;	//config��Ϣ�б�//b+tree.c
struct data_structure data_str;								//table���ݱ������ݸ�ʽ


void btree_read_table(int index_block) {
	long position, data_position;
	bnode node;


	position = index_block * config_inf.block_size * 1024 + sizeof(int);	//Խ�����
	fseek(disk_file[0].file_point, position, 0);	//��λ��ʼ
	fread(&node, sizeof(bnode), 1, disk_file[0].file_point);

	printf(" index_block��%d\n", index_block);
	if (node.keynum > 0)
	{
		if (node.child_block_num[0] == 0)	//Ҷ�ӽڵ�
		{
			for (int i = 0; i < node.keynum; i++)
			{
				/*
				data_position = node.key_block[i] * config_inf.block_size * 1024 + sizeof(int);	//Խ�����
				fseek(disk_file[0].file_point, position, 0);	//��λ��ʼ
				fread(&data_str, sizeof(data_str), 1, disk_file[0].file_point);*/
				//printf(" key[%d] = %d, name = %s\n", i, node.key[i], data_str.name);
				if (node.key[i] != 0)
				{
					printf(" key[%d] = %d\n", i, node.key[i]);
				}
			}
		}
		else    //��Ҷ�ӽڵ�
		{
			for (int j = 0; j < node.keynum + 1; j++)
			{
				btree_read_table(node.child_block_num[j]);
				/*
				data_position = node.key_block[j] * config_inf.block_size * 1024 + sizeof(int);	//Խ�����
				fseek(disk_file[0].file_point, position, 0);	//��λ��ʼ
				fread(&data_str, sizeof(data_str), 1, disk_file[0].file_point);*/
				if (j < node.keynum && node.key[j] != 0)
				{
					//printf(" key[%d] = %d, name = %s\n", j, data_str.key, data_str.name);
					printf(" key[%d] = %d\n", j, data_str.key);
				}
			}
		}
	}
	else
	{
		printf(" �����ݴ洢��\n");
		return 0;
	}
}


/*
void b_index(Ope ope, int key) {
	int prior_num[1] = { 0 };
	switch (ope)
	{
	case query:
		//��ѯ


		break;


	case insert:
		//����
		if (btree_search_data(key, prior_num) == ok)
		{
			btree_insert_data(key, prior_num);
			printf("InsertBTree successfully.\n");
			break;
		}
		else
		{
			printf("�ؼ��ֲ�����.\n");
		}


	case delete:
		//ɾ��
		if (btree_search_data(key, prior_num) == ok)
		{
			delete_key(key, prior_num);
			printf("DeleteBTree successfully.\n");
			break;
		}
		else
		{
			printf("�ؼ��ֲ�����.\n");
		}
		break;


	case read:
		break;
		printf("DestroyBTree successfully.\n");
	}
}
*/
