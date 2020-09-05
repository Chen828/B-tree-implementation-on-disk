//6 查阅数据表
#include "pch.h"

extern struct disk_file_information disk_file[FILE_AMOUNT];	//每个磁盘文件的信息//b+tree.c
extern struct fat_block_structure fat_s;	//b+tree.c
extern struct config_information config_inf;	//config信息列表
extern struct save_content s_c[content_num];					//目录块
extern int fat_search_data(int table_num, int key, int *target_unit);			//fat_search_data.c
struct data_structure data_str;				//table数据表中数据格式


void fat_read_table(int table_num) {
	int unit, next_unit;	//表项值=1 表示该表项值有数据，下一表项值，
	//int block_num;	//新的block号
	long position, fat_position, block_position;
	//int first_unit = 0;	//起始数据表项
	int read_over_flag = 0;	//存储文件的表项值是否是最后一个表项值, 结束读表

	//printf("=============== 6 查阅数据表 ===============\n");

	//定位起始块，读数据
	position = fat_s.fat_block_amount * config_inf.block_size * 1024;
	position = position + sizeof(int) + table_num * sizeof(s_c[0]);		//越过块号，定位block中数据表位置
	fseek(disk_file[0].file_point, position , 0);	//定位文件起始
	fread(&s_c[table_num], sizeof(s_c[0]), 1, disk_file[0].file_point);


	/*
	fseek(disk_file[0].file_point, 0, 0);	//定位文件起始

	//printf("1\n");

	//fat块循环读
	for (int i = 0; i < fat_s.fat_block_amount && !read_over_flag; i++)
	{
		//printf("2\n");
		
		fseek(disk_file[0].file_point, sizeof(int), 1);	//跳过每块的块号

		//定位每个block中的起始数据表项
		//0块里存的还有与fat对应的单元格
		if (!i)
		{
			fseek(disk_file[0].file_point, fat_s.fat_block_amount * sizeof(int), 1);
			first_unit = fat_s.fat_block_amount;	//fat占用block个数的数值 = 第一个存数据的表项值标签对应的块号的数值；
		}

		//表项值循环找
		unit = first_unit;
		for (int j = first_unit; j < fat_s.fat_value_amount && !read_over_flag; j++)
		{
			//printf("3\n");
			
			fread(&next_unit, sizeof(int), 1, disk_file[0].file_point);

			while (next_unit != 0 && !read_over_flag)	//非空闲block
			{

				//printf("4\n");

				//计算当下数据block的位置
				position = unit * config_inf.block_size * 1024;
				fseek(disk_file[0].file_point, position + sizeof(int), 0);	//越过块号
				printf(" block_num = %d ; block_position = %d \n", unit, position);

				fread(&data_str, sizeof(data_str), 1, disk_file[0].file_point);
				printf(" data_str.key = %3d, data_str.name = %s\n", data_str.key, data_str.name);

				if (next_unit != 1)
				{
					unit = next_unit;	//保存块号，以读下一表项值

					//计算next_unit在fat中的位置
					position = ((next_unit / fat_s.fat_value_amount + 1) + next_unit) * sizeof(int);
					//定位
					fseek(disk_file[0].file_point, position, 0);
					//读取next_unit在fat中的表项值
					fread(&next_unit, sizeof(int), 1, disk_file[0].file_point);
				}
				else
				{
					read_over_flag = 1;
				}
			}
		}
	}
	*/

	//起始位置号
	if (s_c[table_num].data_block == 0)
	{
		printf("数据表不存在！\n");
		return 0;
	}
	else
	{
		fat_position = ((s_c[table_num].data_block / fat_s.fat_value_amount + 1) + s_c[table_num].data_block) * sizeof(int);
		fseek(disk_file[0].file_point, fat_position, 0);	//定位文件起始
		fread(&next_unit, sizeof(int), 1, disk_file[0].file_point);

		unit = s_c[table_num].data_block;	//从第一个block开始读
		while (next_unit != 0 && !read_over_flag)	//非空闲block
		{

			//printf("4\n");

			//计算当下数据block的位置
			position = unit * config_inf.block_size * 1024;
			fseek(disk_file[0].file_point, position + sizeof(int), 0);	//越过块号
			printf(" block_num = %d ; block_position = %d \n", unit, position);

			fread(&data_str, sizeof(data_str), 1, disk_file[0].file_point);
			printf(" data_str.key = %3d, data_str.name = %s\n", data_str.key, data_str.name);

			if (next_unit != 1)
			{
				unit = next_unit;	//保存块号，以读下一表项值

				//计算next_unit在fat中的位置
				position = ((next_unit / fat_s.fat_value_amount + 1) + next_unit) * sizeof(int);
				//定位
				fseek(disk_file[0].file_point, position, 0);
				//读取next_unit在fat中的表项值
				fread(&next_unit, sizeof(int), 1, disk_file[0].file_point);
			}
			else
			{
				read_over_flag = 1;
			}
		}

	}


	//未找到空闲
	if (!read_over_flag)
	{
		printf("无数据存储！\n");
	}
}