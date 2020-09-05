//6 查阅数据表
#include "pch.h"


extern struct disk_file_information disk_file[FILE_AMOUNT];	//每个磁盘文件的信息//b+tree.c
extern struct config_information config_inf;	//config信息列表//b+tree.c
struct data_structure data_str;								//table数据表中数据格式


void btree_read_table(int index_block) {
	long position, data_position;
	bnode node;


	position = index_block * config_inf.block_size * 1024 + sizeof(int);	//越过块号
	fseek(disk_file[0].file_point, position, 0);	//定位起始
	fread(&node, sizeof(bnode), 1, disk_file[0].file_point);

	printf(" index_block：%d\n", index_block);
	if (node.keynum > 0)
	{
		if (node.child_block_num[0] == 0)	//叶子节点
		{
			for (int i = 0; i < node.keynum; i++)
			{
				/*
				data_position = node.key_block[i] * config_inf.block_size * 1024 + sizeof(int);	//越过块号
				fseek(disk_file[0].file_point, position, 0);	//定位起始
				fread(&data_str, sizeof(data_str), 1, disk_file[0].file_point);*/
				//printf(" key[%d] = %d, name = %s\n", i, node.key[i], data_str.name);
				if (node.key[i] != 0)
				{
					printf(" key[%d] = %d\n", i, node.key[i]);
				}
			}
		}
		else    //非叶子节点
		{
			for (int j = 0; j < node.keynum + 1; j++)
			{
				btree_read_table(node.child_block_num[j]);
				/*
				data_position = node.key_block[j] * config_inf.block_size * 1024 + sizeof(int);	//越过块号
				fseek(disk_file[0].file_point, position, 0);	//定位起始
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
		printf(" 无数据存储！\n");
		return 0;
	}
}


/*
void b_index(Ope ope, int key) {
	int prior_num[1] = { 0 };
	switch (ope)
	{
	case query:
		//查询


		break;


	case insert:
		//插入
		if (btree_search_data(key, prior_num) == ok)
		{
			btree_insert_data(key, prior_num);
			printf("InsertBTree successfully.\n");
			break;
		}
		else
		{
			printf("关键字不存在.\n");
		}


	case delete:
		//删除
		if (btree_search_data(key, prior_num) == ok)
		{
			delete_key(key, prior_num);
			printf("DeleteBTree successfully.\n");
			break;
		}
		else
		{
			printf("关键字不存在.\n");
		}
		break;


	case read:
		break;
		printf("DestroyBTree successfully.\n");
	}
}
*/
