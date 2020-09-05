//2 ����һ������
#include "pch.h"

extern struct disk_file_information disk_file[FILE_AMOUNT];	//ÿ�������ļ�����Ϣ//b+tree.c
extern struct fat_block_structure fat_s;					//b+tree.c
extern struct config_information config_inf;				//config��Ϣ�б�
struct data_structure data_str;								//table���ݱ������ݸ�ʽ
extern struct save_content s_c[content_num];					//Ŀ¼��
extern int fat_search_key(int table_num, int key, int *target_unit);			//fat_search_key.c
extern void fat_read_table(int table_num);									//fat_read_table.c
extern void content();									//b+tree.c


void fat_insert_data() {
	int unit, next_unit;	//����ֵ=1 ��ʾ�ñ���ֵ�����ݣ���һ����ֵ��
	int new_num;	//�µ�block��
	long fat_position, block_position;

	int query_result = 2;		//��ѯ�ؼ��ַ���ֵ��0�����ݣ�1δ�ҵ��ؼ��֣�2���ڹؼ���
	int small_unit =  0 ;

	int table_num;	//���ݱ��
	int key;	//���ݹؼ���
	char name[MAN_NAME_LENGTH];	//��������

	//printf("=============== 2 ����һ������ ===============\n");

	//����
	content();

	printf(" ���������ݱ��ţ�");
	scanf("%d", &table_num);
	//����ؼ���
	printf(" ������ؼ��֣�");
	scanf("%d", &key); 
	query_result = fat_search_key(table_num, key, &small_unit);
	
	//ѭ����Ҫ����Ĺؼ��ֲ�����
	/*while (query_result == 2)
	{
		//����ؼ���
		scanf("%d", &key);

		query_result = fat_search_key(key);
		printf(" �ؼ��ִ��ڣ�����������ؼ��֣�");
	}*/

	
	if (query_result == 2)
	{
		printf(" �ؼ����Ѵ���!\n");
	}
	else	//����
	{
		printf(" ������������");
		scanf("%s", &name);

		data_str.key = key;
		strcpy(data_str.name, name);

		//�޴洢����
		if (query_result == 0)
		{
			//�����׿����
			new_num = fat_block_apply(1, 1, 0);
			s_c[table_num].data_block = new_num;

			block_position = new_num * config_inf.block_size * 1024 + sizeof(int);	//Խ�����

			//д������λ��
			printf(" new_block_position: %d ;\n", block_position);

			fseek(disk_file[0].file_point, block_position, 0);
			fwrite(&data_str, sizeof(data_str), 1, disk_file[0].file_point);

			printf(" ����ɹ�:\n");

			//����
			fat_read_table(table_num);
		}
		else     //�д洢����
		{
			//�����ڹؼ��֣�fat_search_key()���ؼ�¼С�ڹؼ����е����ؼ���λ�ڵĿ��

			fat_position = ((small_unit / fat_s.fat_value_amount + 1) + small_unit) * sizeof(int);
			fseek(disk_file[0].file_point, fat_position, 0);
			fread(&next_unit, sizeof(int), 1, disk_file[0].file_point);
			
			//printf("%d\n", small_unit);
			//printf("%d\n", next_unit);

			//�������ݵ�block��
			new_num = fat_block_apply(1, 0, small_unit);
			
			//printf("%d\n", new_num);

			//�������������block��Ӧ��fat����ֵд����һfat����ֵ
			fat_position = ((new_num / fat_s.fat_value_amount + 1) + new_num) * sizeof(int);
			fseek(disk_file[0].file_point, fat_position, 0);
			fwrite(&next_unit, sizeof(int), 1, disk_file[0].file_point);

			//��������λ�ã���д��
			block_position = new_num * config_inf.block_size * 1024 + sizeof(int);	//Խ�����
			printf("             new_block_position: %d ;\n", block_position);
			fseek(disk_file[0].file_point, block_position, 0);
			fwrite(&data_str, sizeof(data_str), 1, disk_file[0].file_point);

			printf(" ����ɹ���\n==============\n");

			//����
			fat_read_table(table_num);
		}
	}
}