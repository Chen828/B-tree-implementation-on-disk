//3 删除
#include "pch.h"

extern struct disk_file_information disk_file[FILE_AMOUNT];	//每个磁盘文件的信息//b+tree.c
extern struct fat_block_structure fat_s;					//b+tree.c
struct data_structure data_str;								//table数据表中数据格式
extern int fat_search_data(int table_num, int key, int *target_unit);			//fat_search_data.c
extern void fat_read_table(int table_num);									//fat_read_table.c
extern void content();									//b+tree.c


void fat_delete_data() {
	int unit, next_unit;	//表项值=1 表示该表项值有数据，下一表项值，
	int new_num;	//新的block号
	long fat1_position, fat2_position;

	int query_result;		//查询关键字返回值：0无数据；1未找到关键字；2存在关键字
	int target_unit = 0;

	int table_num;
	int key;	//数据关键字
	char name[MAN_NAME_LENGTH];	//数据人名

	//printf("=============== 3 删除数据 ===============\n");

	//查找
	content();
	printf(" 请输入数据表编号：");
	scanf("%d", &table_num);

	//输入关键字
	printf(" 请输入关键字：");
	scanf("%d", &key);
	query_result = fat_search_data(table_num, key, &target_unit);
	//*target_unit记录小于关键字中的最大关键字位于的块号，即目标关键字block的前一block号
	//printf(" small_unit = %d\n", target_unit);

	if (query_result == 2)			//删除
	{
		//目标关键字的block号
		fat1_position = ((target_unit / fat_s.fat_value_amount + 1) + target_unit) * sizeof(int);
		fseek(disk_file[0].file_point, fat1_position, 0);
		fread(&unit, sizeof(int), 1, disk_file[0].file_point);	//unit：目标关键字的block号

		//目标关键字的下一block号
		fat2_position = ((unit / fat_s.fat_value_amount + 1) + unit) * sizeof(int);
		fseek(disk_file[0].file_point, fat2_position, 0);
		fread(&next_unit, sizeof(int), 1, disk_file[0].file_point);
		unit = 0;
		fseek(disk_file[0].file_point, fat2_position, 0);
		fwrite(&unit, sizeof(int), 1, disk_file[0].file_point);	//写入空

		//下一block号写入上一block号的表项值
		fseek(disk_file[0].file_point, fat1_position, 0);
		fwrite(&next_unit, sizeof(int), 1, disk_file[0].file_point);	//next_unit：目标关键字的block号

		printf(" 删除成功：\n==============\n");

		//读表
		fat_read_table(table_num);
	}
}