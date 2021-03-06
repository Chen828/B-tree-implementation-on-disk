// b+tree.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。

#include "pch.h"
/*
extern void creat_define_config();	//writer_config.c
extern int read_config();			//read_config.c
extern void
 block_count();			//block_set.c
extern void fat_inf();				//fat_manage.c
extern void fat_initialize();		//fat_manage.c

extern void insert_table();			//insert_table.c
extern void fat_insert_data();			//fat_insert_data.c
extern void fat_delete_data();			//fat_delete_data().c
extern void fat_alter_data();			//fat_alter_data().c
extern int fat_search_data(int key, int *target_unit);		//fat_search_data.c
extern int fat_read_table();		    //fat_read_table.c
*/

struct config_information config_inf;	//config信息列表
struct disk_file_information disk_file[FILE_AMOUNT];	//每个磁盘文件的信息
struct fat_block_structure fat_s;
struct save_content s_c[content_num];					//目录块
struct data_structure data_str;				//table数据表中数据格式

//系统初始选项列表
void system_list()
{
	printf("===========================\n");
	printf("     B+tree System\n");
	printf("===========================\n");
	printf(" 操作选项：\n");
	printf("   1 使用配置文件\n");
	printf("   2 创建新配置文件\n");
	printf("   0 退出程序\n");
	printf("===========================\n");
	printf(" 请输入编号：");
}


//fat主操作选项
void manage_ope_list()
{
	printf("\n===========================\n");
	printf("    Operation Opetions\n");
	printf("===========================\n");
	printf("   1 FAT管理\n");
	printf("   2 Btree管理\n");
	printf("  -1 退出程序\n");
	printf("===============\n");
	//printf("===========================\n");
	printf(" 请输入编号：");
}


//fat主操作选项
void fat_operation_list()
{
	printf("\n===========================\n");
	printf("      Fat  Management\n");
	printf("===========================\n");
}


//fat主操作选项
void btree_operation_list()
{
	printf("\n===========================\n");
	printf("     Btree  Management\n");
	printf("===========================\n");
}


//fat-btree操作选项
void fat_btree_operation_list() {
	printf("   1 插入初始表\n");
	printf("   2 插入一条数据\n");
	printf("   3 删除数据\n");
	printf("   4 修改数据\n");
	printf("   5 查询关键字\n");
	printf("   6 查阅数据表\n");
	printf("   0 返回\n");
	printf("  -1 退出程序\n");
	printf("===============\n");
	//printf("===========================\n");
	printf(" 请输入编号：");
}


void content() {
	int table_num = 0;
	long position;

	printf("=============\n");
	printf(" 数据表信息：\n");
	printf(" 编号    表名                       数据地址	  索引地址\n");
	
	position = fat_s.fat_block_amount * config_inf.block_size * 1024;
	position = position + sizeof(int) + table_num * sizeof(s_c[0]);		//越过块号，定位block中数据表位置
	fseek(disk_file[0].file_point, position, 0);	//定位文件起始
	fread(&s_c[table_num], sizeof(s_c[0]), 1, disk_file[0].file_point);
	while (s_c[table_num].data_block != 0) 
	{
		printf(" %3d     %-20s        %-6d%9d\n", table_num, s_c[table_num].table_name, s_c[table_num].data_block, s_c[table_num].index_block);
		table_num++;
		//定位起始块，读数据
		//position = fat_s.fat_block_amount * config_inf.block_size * 1024;
		//position = position + sizeof(int) + table_num * sizeof(s_c[0]);		//越过块号，定位block中数据表位置
		//fseek(disk_file[0].file_point, position, 0);	//定位文件起始
		//fseek(disk_file[0].file_point, sizeof(s_c[0]), 1);	//定位文件起始
		fread(&s_c[table_num], sizeof(s_c[0]), 1, disk_file[0].file_point);
	}
	printf("=============\n");
}


void find() {
	int unit;
	/*
	for (int i = 0; i < 30; i++)
	{
		fseek(disk_file[0].file_point, (fat_s.fat_block_amount + 1 + 1) * sizeof(int), 0);
		fread(&unit, sizeof(int), 1, disk_file[0].file_point);
		printf("%d  %d\n", i, unit);
	}*/
	fseek(disk_file[0].file_point, 24 + 1 * sizeof(int), 0);
	fread(&unit, sizeof(int), 1, disk_file[0].file_point);
	printf("\n24 %d\n", unit);
	fread(&unit, sizeof(int), 1, disk_file[0].file_point);
	printf("25 %d\n", unit);
	fread(&unit, sizeof(int), 1, disk_file[0].file_point);
	printf("26 %d\n", unit);
	fread(&unit, sizeof(int), 1, disk_file[0].file_point);
	printf("27 %d\n\n", unit);
}


int main()
{
	config_inf.disk_file_amount = 0;

	Function fun;
	int reoperate1 = 1, reoperate2 = 1, fat_reoperate = 1, btree_reoperate = 1;		//重新选择编号：1重选：0不选
	static int choose;		//选择默认/自定义config

	int key, unit = 0;	//查询关键字,获取前一block号
	int table_num = 0;;	//表个数

	int index_num, data_num;		//btree_search
	Status flag = false, *find_flag = &flag;

	//config set
	while (reoperate1)
	{
		//系统初始选项列表
		system_list();
		scanf("%d", &choose);
		
		switch (choose)
		{
		case 1:
			//读config
			read_config();
			//block设置
			block_count();
			fat_inf();
			fat_initialize();

			reoperate1 = 0;
			break;

		case 2:
			//自定义config
			creat_define_config();

			//block设置
			block_count();
			fat_inf();
			fat_initialize();

			reoperate1 = 0;
			break;

		case 0:
			reoperate1 = 0;
			reoperate2 = 0;
			printf(" 0 退出程序\n");
			break;

		default:
			printf(" 输入编号无效，请重新输入！\n");
			break;
		}
	}
	
	printf("=============== Boot up！ ===============\n\n");


	//主操作
	while (reoperate2)
	{
		//主操作选项
		manage_ope_list();
		scanf("%d", &choose);

		switch (choose)
		{
		case 1:						//////////  fat管理

			fun = fat;
			fat_reoperate = 1;

			while (fat_reoperate)
			{
				//操作选项
				fat_operation_list();
				fat_btree_operation_list();
				scanf("%d", &choose);

				switch (choose)
				{
				case 1:
					//插入初始表
					printf("=============== 1 插入初始表 ===============\n");
					//find();
					insert_table(table_num, fun);
					//printf("%d\n", table_num);
					//find();
					table_num++;
					break;

				case 2:
					//插入一条数据
					printf("=============== 2 插入一条数据 ===============\n");
					fat_insert_data();
					break;

				case 3:
					//删除
					printf("=============== 3 删除数据 ===============\n");
					fat_delete_data();
					break;

				case 4:
					//修改
					printf("=============== 4 修改数据 ===============\n");
					fat_alter_data();
					break;

				case 5:
					//查询
					printf("=============== 5 查询关键字 ===============\n");
					//输入数据表编号
					content();
					printf(" 请输入数据表编号：");
					scanf("%d", &table_num);

					//输入关键字
					printf(" 请输入关键字：");
					scanf("%d", &key);
					unit = s_c[table_num].data_block;
					fat_search_data(table_num, key, &unit);
					break;

				case 6:
					//浏览browse
					printf("=============== 6 查阅数据表 ===============\n");
					content();
					printf(" 请输入数据表编号：");
					scanf("%d", &table_num); 
					fat_read_table(table_num);
					break;

				case 0:
					//退出程序
					fat_reoperate = 0;
					break;

				case -1:
					//退出程序
					fat_reoperate = 0;
					reoperate2 = 0;
					break;

				default:
					printf(" 输入编号无效，请重新输入！\n");
					break;
				}
			}
			break;

		case 2:						//////////  btree管理
			
			fun = btree;
			btree_reoperate = 1;

			while (btree_reoperate)
			{
				//操作选项
				btree_operation_list();
				fat_btree_operation_list();
				scanf("%d", &choose);

				switch (choose)
				{
				case 1:
					//插入初始表
					printf("=============== 1 插入初始表 ===============\n");
					insert_table(table_num, fun);
					//find();
					table_num++;
					break;

				case 2:
					//插入一条数据
					printf("=============== 2 插入一条数据 ===============\n");
					content();
					printf(" 请输入数据表编号：");
					scanf("%d", &table_num);

					//输入关键字
					printf(" 请输入关键字：");
					scanf("%d", &key);
					index_num = btree_search_data(key, s_c[table_num].index_block, find_flag);

					if (find_flag == true)
					{
						printf(" 关键字已存在！\n");
					}
					else
					{
						data_num = btree_block_apply();
						btree_insert_data(key, index_num, data_num);
					}
					break;

				case 3:
					//删除
					printf("=============== 3 删除数据 ===============\n");
					//查找
					content();
					printf(" 请输入数据表编号：");
					scanf("%d", &table_num);
					//输入关键字
					printf(" 请输入关键字：");
					scanf("%d", &key);
					index_num = btree_search_data(key, s_c[table_num].index_block, find_flag);
					
					btree_delete_data(key, index_num, flag);
					break;

				case 4:
					//修改
					printf("=============== 4 修改数据 ===============\n");
					//查找
					content();
					printf(" 请输入数据表编号：");
					scanf("%d", &table_num);
					//输入关键字
					printf(" 请输入关键字：");
					scanf("%d", &key);
					btree_alter_data(table_num, key);
					break;

				case 5:
					//查询
					printf("=============== 5 查询关键字 ===============\n");
					content();
					printf(" 请输入数据表编号：");
					scanf("%d", &table_num);
					//输入关键字
					printf(" 请输入关键字：");
					scanf("%d", &key);
					btree_search_data(key, s_c[table_num].index_block, find_flag);
					break;

				case 6:
					//浏览browse
					printf("=============== 6 查阅数据表 ===============\n");
					content();
					printf(" 请输入数据表编号：");
					scanf("%d", &table_num);
					btree_read_table(s_c[table_num].index_block);
					break;

				case 0:
					//退出程序
					btree_reoperate = 0;
					break;

				case -1:
					//退出程序
					btree_reoperate = 0;
					reoperate2 = 0;
					break;

				default:
					printf(" 输入编号无效，请重新输入！\n");
					break;
				}
				printf("==============\n");
			}
			break;
		case -1:
			reoperate2 = 0;
			break;
		default:
			printf(" 输入编号无效，请重新输入！\n");
			break;
		}
	}



	//关闭文件指针
	for (int i = 0; i < config_inf.disk_file_amount; i++)
	{
		fclose(disk_file[i].file_point);
	}
}