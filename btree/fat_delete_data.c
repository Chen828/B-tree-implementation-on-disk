//3 ɾ��
#include "pch.h"

extern struct disk_file_information disk_file[FILE_AMOUNT];	//ÿ�������ļ�����Ϣ//b+tree.c
extern struct fat_block_structure fat_s;					//b+tree.c
struct data_structure data_str;								//table���ݱ������ݸ�ʽ
extern int fat_search_data(int table_num, int key, int *target_unit);			//fat_search_data.c
extern void fat_read_table(int table_num);									//fat_read_table.c
extern void content();									//b+tree.c


void fat_delete_data() {
	int unit, next_unit;	//����ֵ=1 ��ʾ�ñ���ֵ�����ݣ���һ����ֵ��
	int new_num;	//�µ�block��
	long fat1_position, fat2_position;

	int query_result;		//��ѯ�ؼ��ַ���ֵ��0�����ݣ�1δ�ҵ��ؼ��֣�2���ڹؼ���
	int target_unit = 0;

	int table_num;
	int key;	//���ݹؼ���
	char name[MAN_NAME_LENGTH];	//��������

	//printf("=============== 3 ɾ������ ===============\n");

	//����
	content();
	printf(" ���������ݱ��ţ�");
	scanf("%d", &table_num);

	//����ؼ���
	printf(" ������ؼ��֣�");
	scanf("%d", &key);
	query_result = fat_search_data(table_num, key, &target_unit);
	//*target_unit��¼С�ڹؼ����е����ؼ���λ�ڵĿ�ţ���Ŀ��ؼ���block��ǰһblock��
	//printf(" small_unit = %d\n", target_unit);

	if (query_result == 2)			//ɾ��
	{
		//Ŀ��ؼ��ֵ�block��
		fat1_position = ((target_unit / fat_s.fat_value_amount + 1) + target_unit) * sizeof(int);
		fseek(disk_file[0].file_point, fat1_position, 0);
		fread(&unit, sizeof(int), 1, disk_file[0].file_point);	//unit��Ŀ��ؼ��ֵ�block��

		//Ŀ��ؼ��ֵ���һblock��
		fat2_position = ((unit / fat_s.fat_value_amount + 1) + unit) * sizeof(int);
		fseek(disk_file[0].file_point, fat2_position, 0);
		fread(&next_unit, sizeof(int), 1, disk_file[0].file_point);
		unit = 0;
		fseek(disk_file[0].file_point, fat2_position, 0);
		fwrite(&unit, sizeof(int), 1, disk_file[0].file_point);	//д���

		//��һblock��д����һblock�ŵı���ֵ
		fseek(disk_file[0].file_point, fat1_position, 0);
		fwrite(&next_unit, sizeof(int), 1, disk_file[0].file_point);	//next_unit��Ŀ��ؼ��ֵ�block��

		printf(" ɾ���ɹ���\n==============\n");

		//����
		fat_read_table(table_num);
	}
}