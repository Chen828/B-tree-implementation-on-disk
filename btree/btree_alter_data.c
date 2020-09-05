//4 修改
#include "pch.h"

extern struct disk_file_information disk_file[FILE_AMOUNT];	//每个磁盘文件的信息//b+tree.c
extern struct config_information config_inf;				//config信息列表
extern struct save_content s_c[content_num];					//目录块
struct data_structure data_str;								//table数据表中数据格式

extern int btree_search_data(int key, int block_index, Status *find_flag);			//btree_search_data.c
extern void content();

void btree_alter_data(int table_num, int key) {
	int index_block, data_block;
	bnode node;
	long position;

	Status flag = false, *find_flag = &flag;

	char name[MAN_NAME_LENGTH];	//数据人名
	
	int i;
	//printf("=============== 4 修改数据 ===============\n");
	//查找
	index_block = btree_search_data(key, s_c[table_num].index_block, find_flag);

	if (flag == true)
	{
		printf(" 请输入姓名：");
		scanf("%s", &name);

		data_str.key = key;
		strcpy(data_str.name, name);

		//获取数据位置
		position = index_block * config_inf.block_size * 1024 + sizeof(int);	//越过块号
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
		
		//数据的位置，并写入
		position = data_block * config_inf.block_size * 1024 + sizeof(int);	//越过块号
		fseek(disk_file[0].file_point, position, 0);
		fwrite(&data_str, sizeof(data_str), 1, disk_file[0].file_point);
		printf("写入\n");
		fseek(disk_file[0].file_point, position, 0);
		fread(&data_str, sizeof(data_str), 1, disk_file[0].file_point);
		printf("重读\n");
		printf("data_str.key = %d, data_str.name = %s\n", data_str.key, data_str.name);

		printf(" 修改成功：\n==============\n");
	}
	else
	{
		printf(" 关键字不存在！");
	}
}