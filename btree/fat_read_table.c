//6 �������ݱ�
#include "pch.h"

extern struct disk_file_information disk_file[FILE_AMOUNT];	//ÿ�������ļ�����Ϣ//b+tree.c
extern struct fat_block_structure fat_s;	//b+tree.c
extern struct config_information config_inf;	//config��Ϣ�б�
extern struct save_content s_c[content_num];					//Ŀ¼��
extern int fat_search_data(int table_num, int key, int *target_unit);			//fat_search_data.c
struct data_structure data_str;				//table���ݱ������ݸ�ʽ


void fat_read_table(int table_num) {
	int unit, next_unit;	//����ֵ=1 ��ʾ�ñ���ֵ�����ݣ���һ����ֵ��
	//int block_num;	//�µ�block��
	long position, fat_position, block_position;
	//int first_unit = 0;	//��ʼ���ݱ���
	int read_over_flag = 0;	//�洢�ļ��ı���ֵ�Ƿ������һ������ֵ, ��������

	//printf("=============== 6 �������ݱ� ===============\n");

	//��λ��ʼ�飬������
	position = fat_s.fat_block_amount * config_inf.block_size * 1024;
	position = position + sizeof(int) + table_num * sizeof(s_c[0]);		//Խ����ţ���λblock�����ݱ�λ��
	fseek(disk_file[0].file_point, position , 0);	//��λ�ļ���ʼ
	fread(&s_c[table_num], sizeof(s_c[0]), 1, disk_file[0].file_point);


	/*
	fseek(disk_file[0].file_point, 0, 0);	//��λ�ļ���ʼ

	//printf("1\n");

	//fat��ѭ����
	for (int i = 0; i < fat_s.fat_block_amount && !read_over_flag; i++)
	{
		//printf("2\n");
		
		fseek(disk_file[0].file_point, sizeof(int), 1);	//����ÿ��Ŀ��

		//��λÿ��block�е���ʼ���ݱ���
		//0�����Ļ�����fat��Ӧ�ĵ�Ԫ��
		if (!i)
		{
			fseek(disk_file[0].file_point, fat_s.fat_block_amount * sizeof(int), 1);
			first_unit = fat_s.fat_block_amount;	//fatռ��block��������ֵ = ��һ�������ݵı���ֵ��ǩ��Ӧ�Ŀ�ŵ���ֵ��
		}

		//����ֵѭ����
		unit = first_unit;
		for (int j = first_unit; j < fat_s.fat_value_amount && !read_over_flag; j++)
		{
			//printf("3\n");
			
			fread(&next_unit, sizeof(int), 1, disk_file[0].file_point);

			while (next_unit != 0 && !read_over_flag)	//�ǿ���block
			{

				//printf("4\n");

				//���㵱������block��λ��
				position = unit * config_inf.block_size * 1024;
				fseek(disk_file[0].file_point, position + sizeof(int), 0);	//Խ�����
				printf(" block_num = %d ; block_position = %d \n", unit, position);

				fread(&data_str, sizeof(data_str), 1, disk_file[0].file_point);
				printf(" data_str.key = %3d, data_str.name = %s\n", data_str.key, data_str.name);

				if (next_unit != 1)
				{
					unit = next_unit;	//�����ţ��Զ���һ����ֵ

					//����next_unit��fat�е�λ��
					position = ((next_unit / fat_s.fat_value_amount + 1) + next_unit) * sizeof(int);
					//��λ
					fseek(disk_file[0].file_point, position, 0);
					//��ȡnext_unit��fat�еı���ֵ
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

	//��ʼλ�ú�
	if (s_c[table_num].data_block == 0)
	{
		printf("���ݱ����ڣ�\n");
		return 0;
	}
	else
	{
		fat_position = ((s_c[table_num].data_block / fat_s.fat_value_amount + 1) + s_c[table_num].data_block) * sizeof(int);
		fseek(disk_file[0].file_point, fat_position, 0);	//��λ�ļ���ʼ
		fread(&next_unit, sizeof(int), 1, disk_file[0].file_point);

		unit = s_c[table_num].data_block;	//�ӵ�һ��block��ʼ��
		while (next_unit != 0 && !read_over_flag)	//�ǿ���block
		{

			//printf("4\n");

			//���㵱������block��λ��
			position = unit * config_inf.block_size * 1024;
			fseek(disk_file[0].file_point, position + sizeof(int), 0);	//Խ�����
			printf(" block_num = %d ; block_position = %d \n", unit, position);

			fread(&data_str, sizeof(data_str), 1, disk_file[0].file_point);
			printf(" data_str.key = %3d, data_str.name = %s\n", data_str.key, data_str.name);

			if (next_unit != 1)
			{
				unit = next_unit;	//�����ţ��Զ���һ����ֵ

				//����next_unit��fat�е�λ��
				position = ((next_unit / fat_s.fat_value_amount + 1) + next_unit) * sizeof(int);
				//��λ
				fseek(disk_file[0].file_point, position, 0);
				//��ȡnext_unit��fat�еı���ֵ
				fread(&next_unit, sizeof(int), 1, disk_file[0].file_point);
			}
			else
			{
				read_over_flag = 1;
			}
		}

	}


	//δ�ҵ�����
	if (!read_over_flag)
	{
		printf("�����ݴ洢��\n");
	}
}