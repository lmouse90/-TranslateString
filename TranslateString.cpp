/*
翻译数字串，类似于电话号码翻译：给一个数字串，
比如12259，映射到字母数组，比如，1 -< a， 2-< b，... ， 12 -< l ，... 26-< z。
那么，12259 -< lyi 或 abbei 或 lbei 或 abyi。
输入一个数字串，判断是否能转换成字符串，如果能，则打印所以有可能的转换成的字符串
*/

#include <iostream>
#include <conio.h>
#include <vector>
#include <fstream>
using namespace std;

//定义二叉树节点
struct Node
{
	int i;
	Node *pLeft;
	Node *pRight;
	Node(int _i):i(_i),pLeft(NULL),pRight(NULL){}
};

//传入路径记录信息，这里只需要传值(***)
//之前错误是因为传入的是引用，这样导致栈保存数据错误
//传值时递归调用时返回的状态才回是当时节点之前的路径，否则会出现状态混乱的现象
bool translate(char* str,vector<int> vec, int& count)
{
	int flag = true;
	if(*(str+1) == '\0')
	{
		if(*str != '0') //对最后 一个节点进行判断，如果是120，可能是1(20),12(0),则第二种就会出错
		{
			for(int i=0; i<vec.size(); i++)
			{
				cout << char('a' + vec[i] - 1);
			}
			cout << char('a' + *(str) - 1 - '0');
			cout << endl;
			count++;
		}
		return true;
	}
	else if(*(str+2) == '\0')
	{
		int num1 = *str - '0';
		int num2 = *(str+1) - '0';
		int num12 = num1*10+num2;

		//the last two, you need to care 20, 30....
		if(num2 == 0)
		{	
			if(num12 <= 26)
			{
				for(int i=0; i<vec.size(); i++)
				{
					cout << char('a' + vec[i] - 1);
				}		
				cout << char('a' + num12 - 1) << endl;	
				count++;
			}
			else
			{
				cout << "input wrong" << endl;
				return false;
			}
		}
		else if(num2 != 0)
		{
			for(int i=0; i<vec.size(); i++)
			{
				cout << char('a' + vec[i] - 1);
			}	
			cout << char('a' + num1 - 1) <<  char('a' + num2- 1) << endl;
			count++;
		}
		return true;
	}

	if(*str < '0' || *str > '9')
	{
		cout << "input wrong" << endl;
		return false;
	}
	int num1 = *str - '0';
	int num2 = *(str+1) - '0';// + (*str-'0') * 10;

	if(num1 == 0)//if 0 exist at the first index, it's wrong
	{
		cout << "input wrong" << endl;
		return false;
	}
	else
	{
		//if exist 10, 20, 30....
		if(num2 == 0)
		{
			if(num1 > 2) // if 30,40..., wrong
			{
				cout << "input wrong" << endl;
				return false;
			}
			else //if 10, 20, it's ok
			{
				vec.push_back(num1*10+num2);
				if(flag) flag = translate(str+2,vec,count);
			}

		}
		else //others, then exist two status
		{
			vec.push_back(num1);
			if(flag) flag = translate(str+1,vec,count);

			if(num2 <= 26)
			{
				vec.pop_back();
				vec.push_back(num2+num1*10);
				if(flag) flag = translate(str+2,vec,count);
			}
		}
	}
	return flag;
}


//构造二叉树，左子树都是选取一个字符，右子树都是选择两个字符
//如果节点为0，或者01类型或者大于26，则都存放为0，然后该节点到此结束
//输出时判断是否为0即可，如果有0节点存在，证明不是正确解
//否则在建树的过程中，如果中途失败了，则前面的端点不确定是否应该存在

bool translateByTree(char* str,Node* &root)
{
	int flag = true;
	if(root == NULL)
	{
		root = new Node(-1);
	}
	if(*str != '\0')
	{
		int num1 = *str - '0';


		if(*str < '0' || *str > '9')
		{
			cout << "input wrong" << endl;
			return false;
		}

		Node* leftNode = new Node(num1);
		root->pLeft = leftNode;
		if(num1 > 0 && num1 < 27)
			if(flag) flag = translateByTree(str+1,root->pLeft);
		else
			if(flag) return true;

		int num2 = -1;
		if(*(str+1) != '\0')
		{
			num2 = *(str+1) - '0';
			int num12;
			if(num1 == 0 || num1*10 + num2 > 26)
			{
				num12 = 0;
			}
			else num12 = num1*10 + num2;

			Node* rightNode = new Node(num12);
			root->pRight = rightNode;
			if(num12 > 0 && num12 < 27)
				if(flag) flag = translateByTree(str+2,root->pRight);
			else 
				if(flag)  return true;
		}
	}
	return flag;
}

//count用来统计输出结果，如果结果为0，说明输入正确字符的情况下如法翻译
void printPath(Node* root,vector<int> vec,int& count)
{
	if(root != NULL)
	{
		if(root->i == 0) return;
		vec.push_back(root->i);
		if(root->pLeft == NULL && root->pRight == NULL)
		{
			for(int i=1; i<vec.size(); i++)
			{
				cout << char('a' + vec[i] - 1);
			}
			cout << endl;
			count++;
			return;
		}
		else
		{
			printPath(root->pLeft,vec,count);
			printPath(root->pRight,vec,count);
		}
	}
}

void test1(char *str)
{
	cout << str << "---result by recursion: " << endl;
	vector<int> vec2;
	int count1 = 0;
	bool flag1 = translate(str,vec2,count1);
}

void test2(char *str)
{
	cout <<str << "---result by create binary tree:" << endl;
	Node* root = NULL;
	bool flag1 = translateByTree(str,root);
	vector<int> vec1;
	int count = 0;
	if(flag1)
	{
		printPath(root,vec1,count);
		if(count == 0)
			cout << "input wrong" << endl;
	}
}
int main()
{
	char str[][10] = {"102210","1-222","12259","30209"};
	
	for(int i=0; i<4; i++)
	{
		test1(str[i]);
		test2(str[i]);
	}

	system("pause");
	return 0;
}
