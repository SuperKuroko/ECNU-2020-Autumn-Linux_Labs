#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "defrag.h"
FILE *istream,*ostream;         //输入流与输出流
SuperBlock *superblock,*boot;   //文件的超级块,起始块
int inode_index;                //inode的起始地址(bytes)
int inode_count;                //inode的数量
int data_index;                 //data_block的起始地址(bytes)
int block_size;                 //block的大小
int block_count;                //遍历一个inode时，用于记录其对应文件占用块的数量
int inode_size = sizeof(iNode); //inode结构所占字节数
char data[5000];                //读取文件时存储数据的临时变量
iNode *inode;                   //指向当前枚举到的inode
int data_pointer = 0;           //指向数据块当前的块索引
int inode_pointer;              //指向目录块当前的块索引

//以下为程序相关函数
void init();                       //初始化部分参数
int address(int number);           //输入一个块的索引，返回其地址(偏移量,以bytes为单位)
int calculate_summary_of_blocks(); //计算出文件一共占用的块的个数
//从上至下分别为0-3级目录的inode读写函数
void write_from_data_blocks(int read_address,int write_address);
void write_from_indirect_blocks(int read_address,int write_address);
void write_from_doubly_indirect_blocks(int read_address,int write_address);
void write_from_triply_indirect_blocks(int read_address,int write_address);


//以下为debug和display相关函数,您可以不关心该部分的函数
FILE*file;                       //需要打印的文件流
void write_usage();              //命令行提示信息
void display(char* f1,char* f2); //display主函数
void print(char *f);             //打印文件f
void order(iNode* node);         //遍历一个inode
void dfs(int val,int level);     //递归打印一个inode


int main(int argc,char *argv[])
{
	//命令行参数数量不正确
	if(argc != 2 && argc != 3) 
	{
		write_usage(); //告诉用户用法
		exit(1);
	}

	//使用3个命令行参数但未使用display关键词
	if(argc == 3 && strcmp(argv[2],"display") != 0) 
	{
		write_usage(); //同上
		exit(1);
	}
	
	//构造输出文件的文件名
	char outFileName[100],extra[8] = "-defrag";
	strcpy(outFileName,argv[1]);
	int j = strlen(outFileName);
	for(int i = 0;i < 7;i++)
		outFileName[j++] = extra[i];
	outFileName[j] = '\0';

	//打开输入与输出流
	istream = fopen(argv[1],"r");
	ostream = fopen(outFileName,"w+");
	if(!istream || !ostream) //打开失败
	{
		write(1,"open file faid!",strlen("open file faid!"));
		exit(1);
	}

	init(); //调用初始化函数，读取超级块来设置部分参数
	inode_pointer = calculate_summary_of_blocks(); //计算出块的总数

	int inode_address = inode_index; //inode的起始地址

	//枚举全部的inode
	for(int i = 0;i < inode_count;i++,inode_address += inode_size)
	{
		//读取出第i个inode的信息
		fseek(istream,inode_address,SEEK_SET);
		fread(inode,inode_size,1,istream);
		
		//如果空闲，下一个
		if(inode->nlink <= 0) continue;
		
		//计算出其对应的文件占用的块数量
		block_count = (inode->size)/block_size;
		if(inode->size % block_size != 0) block_count++;

		//0级目录处理
		for(int v = 0;v < N_DBLOCKS && block_count > 0;v++)
		{
			//构造读入地址与写出地址
			int read_address = address(inode->dblocks[v]);
			int write_address = address(data_pointer);
		
			inode->dblocks[v] = data_pointer;//重排数据块位置
			//写入新的位置
			write_from_data_blocks(read_address,write_address);
		}

		//1级目录处理，结构同上
		for(int v = 0;v < N_IBLOCKS && block_count > 0;v++)
		{
			int read_address = address(inode->iblocks[v]);
			int write_address = address(inode_pointer);
			inode->iblocks[v] = inode_pointer;
			write_from_indirect_blocks(read_address,write_address);
		}
		
		//2级目录处理，结构同上
		if(block_count > 0)
		{
			int read_address = address(inode->i2block);
			int write_address = address(inode_pointer);
			inode->i2block = inode_pointer;
			write_from_doubly_indirect_blocks(read_address,write_address);
		}
		
		//3级目录处理，结构同上
		if(block_count > 0)
		{
			int read_address = address(inode->i3block);
			int write_address = address(inode_pointer);
			inode->i3block = inode_pointer;
			write_from_triply_indirect_blocks(read_address,write_address);
		}  
		
		//将更新之后的inode写进输出流
		fseek(ostream,inode_address,SEEK_SET);
		fwrite(inode,sizeof(iNode),1,ostream);
	}
	
	//重设free_iblock的值
	superblock->free_iblock = inode_pointer;

	//将超级块写入输出流
	fseek(ostream,512,SEEK_SET);
	fwrite(superblock,512,1,ostream);
	
	
	//关闭输入输出流
	fclose(istream);
	fclose(ostream);
	
	//如果是display模式，调用display函数打印文件信息
	if(argc == 3) display(argv[1],outFileName);

	//释放内存
	free(inode);
	free(superblock);
	free(boot);
	return 0;
}

void init()
{
	//复制启动块到输出文件
	boot = (SuperBlock*)malloc(512);
	fseek(istream,0,SEEK_SET);
	fread(boot,512,1,istream);
	fseek(ostream,0,SEEK_SET);
	fwrite(boot,512,1,ostream);
	
	//读入超级块
	fseek(istream,512,SEEK_SET);
	superblock = (SuperBlock*)malloc(512);
	fread(superblock,512,1,istream);
	
	//初始化参数
	inode = (iNode*)malloc(sizeof(iNode));
	block_size = superblock->size;
	data_index = (superblock->data_offset)*block_size+1024;
	inode_index = (superblock->inode_offset)*block_size+1024;
	inode_count = (data_index-inode_index)/inode_size;
}

//输入一个块的索引，返回起始地址+索引*单位块大小，即该块的地址
int address(int number){return data_index+number*block_size;}

//计算出文件块的数量
int calculate_summary_of_blocks()
{
	int sum = 0,inode_address = inode_index; //sum为总数量
	for(int i = 0;i < inode_count;i++,inode_address += inode_size)
	{
		//读入第i的inode
		fseek(istream,inode_address,SEEK_SET);
		fread(inode,inode_size,1,istream);
		if(inode->nlink <= 0) continue;
		
		//计算其文件占用块数量
		block_count = (inode->size)/block_size;
		if(inode->size % block_size != 0) block_count++;
		
		//添加到总和
		sum += block_count;
	}
	return sum;
}

//0级目录处理
void write_from_data_blocks(int read_address,int write_address)
{
	//读入文件到data
	fseek(istream,read_address,SEEK_SET);
	fread(data,block_size,1,istream);

	//写出data到输出流
	fseek(ostream,write_address,SEEK_SET);
	fwrite(data,block_size,1,ostream);

	//指针向后挪动，剩余块数量减1
	data_pointer++;
	block_count--;
}

//1级目录处理
void write_from_indirect_blocks(int read_address,int write_address)
{
	int val;
	inode_pointer++; //将inode指针向后挪动
	for(int i = 0;i < block_size/sizeof(int) && block_count > 0;i++)
	{
		//读取数据块的的位置为val
		fseek(istream,read_address+i*sizeof(int),SEEK_SET);
		fread(&val,sizeof(int),1,istream);

		//重设val为data_pointer
		fseek(ostream,write_address+i*sizeof(int),SEEK_SET);
		fwrite(&data_pointer,sizeof(int),1,ostream);	

		//调用0级函数，将val块的内容复制到data_pointer块中
		write_from_data_blocks(address(val),address(data_pointer));
	}
}

//2级目录处理
void write_from_doubly_indirect_blocks(int read_address,int write_address)
{
	int val;
	inode_pointer++; //将inode指针向后挪动，防止递归时1级目录函数访问到该块
	for(int i = 0;i < block_size/sizeof(int) && block_count > 0;i++)
	{
		//读取1级目录块的的位置为val
		fseek(istream,read_address+i*sizeof(int),SEEK_SET);
		fread(&val,sizeof(int),1,istream);

		//重设val为inode_pointer
		fseek(ostream,write_address+i*sizeof(int),SEEK_SET);
		fwrite(&inode_pointer,sizeof(int),1,ostream);	

		//调用1级函数，将val块的内容复制到data_pointer块中
		write_from_indirect_blocks(address(val),address(inode_pointer));
	}
}

//3级目录处理
void write_from_triply_indirect_blocks(int read_address,int write_address)
{
	int val;
	inode_pointer++;//将inode指针向后挪动，防止递归时2级目录函数访问到该块
	for(int i = 0;i < block_size/sizeof(int) && block_count > 0;i++)
	{
		//读取2级目录块的的位置为val
		fseek(istream,read_address+i*sizeof(int),SEEK_SET);
		fread(&val,sizeof(int),1,istream);

		//重设val为inode_pointer
		fseek(ostream,write_address+i*sizeof(int),SEEK_SET);
		fwrite(&inode_pointer,sizeof(int),1,ostream);	
		
		//调用2级函数，将val块的内容复制到data_pointer块中
		write_from_doubly_indirect_blocks(address(val),address(inode_pointer));
	}
}

//当用户在命令行使用错误命令时，会调用该函数打印提示信息
void write_usage()
{
	char tip[] = 
"\nSorry, you have invoked the program mistakenly.\n\
Usage:\n\
	./defrag <fragmented disk file>          //common  mode\n\
	./defrag <fragmented disk file> display  //display mode\n\n\
For common mode, the program will output a file called <disk file -defrag>, which is generated after defragmentation.\n\n\
For display mode, the program will additional print the information about <disk file> and <disk file -defrag>.\n\n";
	write(1,tip,strlen(tip));
}

//打印主函数，调用print打印指定两个文件
void display(char* f1,char *f2)
{
	printf("-----The information of file before file defragmentation-----\n\n");
	print(f1);
	printf("-----The information of file after file defragmentation-----\n\n");
	print(f2);
}

//打印文件函数
void print(char *f)
{
	file = fopen(f,"r"); //打开文件流

	//读取并输出超级块信息
	fseek(file,512,SEEK_SET);
	fread(superblock,512,1,file);
	printf("Super Block Information:\n");
	printf("block_size   = %5d\n", superblock->size);
	printf("inode_offset = %5d\n", superblock->inode_offset);
	printf("data_offset  = %5d\n", superblock->data_offset);
	printf("swap_offset  = %5d\n", superblock->swap_offset);
	printf("free_inode   = %5d\n", superblock->free_inode);
	printf("free_iblock  = %5d\n", superblock->free_iblock);
	printf("\n");
	
	//枚举每一个块
	int inode_address = inode_index;
	for(int i = 0;i < inode_count;i++,inode_address += inode_size)
	{
		//读取该块的内容
		fseek(file,inode_address,SEEK_SET);
		fread(inode,inode_size,1,file);

		//调用order函数打印该块的全部信息
		printf("inode %d:\n",i);
		order(inode);
	}
	fclose(file);
}
 
//bd记录block_data的序号,  cd为count_data, 记录数量
//bi记录block_inode的序号, ci为count_inode,记录数量
int cd,bd[3000],ci,bi[3000];

void order(iNode* node)
{
	if(node->nlink <= 0)
	{
		printf("This inode is free.\n\n");
		return ;
	}

	//初始化数量,计算占用文件块的总数
	cd = 0;ci = 0;
	block_count = (inode->size)/block_size;
	if(inode->size % block_size != 0) block_count++;
	
	//从0到3级，依次调用dfs函数进行递归
	for(int v = 0;v < N_DBLOCKS && block_count > 0;v++) 
		dfs(inode->dblocks[v],1);
	for(int v = 0;v < N_IBLOCKS && block_count > 0;v++)
		dfs(inode->iblocks[v],2);
	if(block_count > 0)
		dfs(inode->i2block,3);
	if(block_count > 0)
		dfs(inode->i3block,4);  

	//打印序列信息
	printf("Block numbers which store data:\n");
	for(int i = 0;i < cd;i++) printf("%5d%c",bd[i],(i%10==9 || i==cd-1)?'\n':' ');
	printf("\nBlock numbers which are indirect blocks:\n");
	for(int i = 0;i < ci;i++) printf("%5d%c",bi[i],(i%10==9 || i==ci-1)?'\n':' ');
	printf("\n");
}

void dfs(int val,int level)
{   
	//对于level 1 (0级目录)，直接记录其序号
	if(level == 1) 
	{
		bd[cd++] = val;
		block_count--;
		return ;
	}

	//对于更高级的，记录索引块的序号，然后递归到下一级
	bi[ci++] = val;
	for(int i = 0;i < block_size/sizeof(int) && block_count > 0;i++)
	{
		int pos;
		fseek(file,address(val)+i*sizeof(int),SEEK_SET);
		fread(&pos,sizeof(int),1,file);
		dfs(pos,level-1);
	}
}
