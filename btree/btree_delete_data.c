// 3 删除
/*
B树的删除操作
删除操作是指，根据key删除记录，如果B树中的记录中不存对应key的记录，则删除失败。
1）如果当前需要删除的key位于非叶子结点上，则用后继key（这里的后继key均指后继记录的意思）覆盖要删除的key，
   然后在后继key所在的子支中删除该后继key。此时后继key一定位于叶子结点上，这个过程和二叉搜索树删除结点的方式类似。
   删除这个记录后执行第2步
2）该结点key个数大于等于Math.ceil(m/2)-1，结束删除操作，否则执行第3步。
3）如果兄弟结点key个数大于Math.ceil(m/2)-1，则父结点中的key下移到该结点，兄弟结点中的一个key上移，删除操作结束。
   否则，
   将父结点中的key下移与当前结点及它的兄弟结点中的key合并，形成一个新的结点。
   原父结点中的key的两个孩子指针就变成了一个孩子指针，指向这个新结点。然后当前结点的指针指向父结点，重复上第2步。
*/
#include "pch.h"

extern struct disk_file_information disk_file[FILE_AMOUNT];	//每个磁盘文件的信息//b+tree.c
extern struct config_information config_inf;				//config信息列表
extern struct fat_block_structure fat_s;	//b+tree.c


int btree_delete_data(int key, int index_block, Status *flag) {
	bnode node, parent_node, bro_node, child_node;
	int unit = 0;
	int data_block;
	long position, fat_position, position2;
	int i, bro_i, j, middle_count, key_list[m];

	//printf("=============== 5 删除数据 ===============\n");


	//（1）数据块置空
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

	//目标关键字的fat_block号
	fat_position = ((data_block / fat_s.fat_value_amount + 1) + index_block) * sizeof(int);
	fseek(disk_file[0].file_point, fat_position, 0);
	fwrite(&unit, sizeof(int), 1, disk_file[0].file_point);	//unit：目标关键字的block号

	if (node.child_block_num[i] != 0)
	{
		//非叶子节点
		//			数据块置空
		printf("非叶子节点，读关键字左孩子结点\n");
		position = node.child_block_num[i] * config_inf.block_size * 1024 + sizeof(int);	//越过块号
		fseek(disk_file[0].file_point, position, 0);
		fread(&child_node, sizeof(node), 1, disk_file[0].file_point);

		node.key[i] = child_node.key[child_node.keynum - 1];
		node.key_block[i] = child_node.key_block[child_node.keynum - 1];

		btree_delete_data(child_node.key[child_node.keynum - 1], node.child_block_num[i], flag);
	}
	else
	{
		//叶子节点
		printf("叶子节点\n");
		//node.keynum -= 1;
		node.key[i] = 0;
		node.key_block[i] = 0;
	}

	//（2）该结点key个数大于等于Math.ceil(m/2)-1，结束删除操作，否则执行第3步。
	if (node.keynum >= m / 2 - 1)
	{
		position = index_block * config_inf.block_size * 1024 + sizeof(int);	//越过块号
		fseek(disk_file[0].file_point, position, 0);
		fwrite(&node, sizeof(node), 1, disk_file[0].file_point);

		printf("删除成功！\n");

		return ok;
	}
	else   //（3）
	{
		position = node.parent_block_num * config_inf.block_size * 1024 + sizeof(int);	//越过块号
		fseek(disk_file[0].file_point, position, 0);
		fread(&parent_node, sizeof(node), 1, disk_file[0].file_point);

		//定位该关键字，寻找前一兄弟
		for (i = 0; i < parent_node.keynum; i++)
		{
			if (parent_node.child_block_num[i] == index_block)
			{
				break;
			}
		}

		if (i - 1 >= 0)	//有前兄弟
		{
			//i -= 1前一兄弟
			position2 = parent_node.child_block_num[i - 1] * config_inf.block_size * 1024 + sizeof(int);	//越过块号
			fseek(disk_file[0].file_point, position2, 0);
			fread(&bro_node, sizeof(node), 1, disk_file[0].file_point);

			if (bro_node.keynum > m / 2 - 1)	//大于, 能借
			{
				//取得双亲中i-1，此值是该节点中最小值，重排序
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
				node.key[node.keynum] = key;     //第node.keynum个key存最大的数据	//key最小则key值变，否则不变
				printf(" node.key[%d] = %d\n", node.keynum, node.key[node.keynum]);
				//node.keynum++;
				printf(" 双亲中i-1值下移\n");
				printf("==============\n");

				parent_node.key[i - 1] = bro_node.key[bro_node.keynum - 1];	//兄弟最大上到双亲i - 1

				btree_delete_data(bro_node.key[bro_node.keynum - 1], parent_node.child_block_num[i - 1], flag);

				return ok;
			}
			else
			{
				/*
				将父结点中的key下移与当前结点及它的兄弟结点中的key合并，形成一个新的结点。
				原父结点中的key的两个孩子指针就变成了一个孩子指针，指向这个新结点。
				然后当前结点的指针指向父结点，重复上第2步。
				*/

				//节点结合
				//暂存该节点值
				for (j = 0; j < node.keynum; j++)
				{
					key_list[j] = node.key[j];
				}
				//重新赋值，兄弟加入
				for (j = 0; j < bro_node.keynum; j++)
				{
					node.key[j] = bro_node.key[j];
				}
				//重新赋值，取得双亲中i-1加入
				node.key[j] = parent_node.key[i - 1];
				//暂存的该节点值重新写入
				for (j = 0; j < node.keynum; j++)
				{
					node.key[j] = key_list[j];
				}

				//删除兄弟指针及其节点
				//目标关键字的fat_block号
				fat_position = ((parent_node.child_block_num[i - 1] / fat_s.fat_value_amount + 1) + index_block) * sizeof(int);
				fseek(disk_file[0].file_point, fat_position, 0);
				fwrite(&unit, sizeof(int), 1, disk_file[0].file_point);	//写空

				//双亲变化的位置后的关键字，孩子指针前移，原最后一个数据写空
				for (j = i; j < node.keynum; j++)
				{
					//最后一位一定是有的，还是0
					parent_node.key[j - 1] = parent_node.key[j];
					parent_node.key_block[j - 1] = parent_node.key_block[j];
					parent_node.child_block_num[j - 1] = parent_node.child_block_num[j];
				}
			}

		}
		//无前节点
		else
		{

		}
	}	
}