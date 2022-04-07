#include<bits/stdc++.h>
#include<map>
using namespace std;
struct node {
	char data;
	node *lc,*rc;
};
map<char, int> mp; //用于存放运算符优先级
stack<node*> ex; //存放生成的二叉树的栈
stack<char> op;  //存放运算符的栈

node* createTree(char data, node* node_left, node* node_right) { //建树
	node *root = new node;
	root->data = data;
	root->lc = node_left;
	root->rc = node_right;
	return root;
}
double change(char c) {
	return c - '0';
}
double getAdd(char data,double lvalue,double rvalue) {//计算左右子树相加
	switch (data) {
		case '+':
			return lvalue + rvalue;
			break;
		case '-':
			return lvalue - rvalue;
			break;
		case '*':
			return lvalue * rvalue;
			break;
		case '/':
			return lvalue / rvalue;
			break;
		default:
			break;
	}
}
double getValueOfTree(node*root){ //后序遍历二叉树，计算整个表达式的结果
	double lvalue =0, rvalue = 0;
	if(root->lc==NULL&&root->rc==NULL) return change(root->data);
	else{
		lvalue = getValueOfTree(root->lc);
		rvalue = getValueOfTree(root->rc);
		return getAdd(root->data,lvalue,rvalue);
	}
}
void postTree(node *root){//后序遍历打印二叉树
	if(root==NULL) return;
	postTree(root->lc);
	postTree(root->rc);
	cout<<root->data;
}
int main() {
	mp['('] = 3,mp[')'] = 4,mp['+'] = 1,mp['-'] = 1,mp['*'] = 2,mp['/'] = 2;//设置运算符的优先级
	string s;//以字符串形式接收表达式
	cin>>s;
	for(int i = 0; i < s.length(); i++) {//从左至右依次遍历字符串中的每个字符
		if(isdigit(s[i])) { //如果该字符是数字，生成子树，直接放入ex栈
			node*root = createTree(s[i],NULL,NULL);
			ex.push(root);
		} else {//如果是运算符
			if(s[i] == ')') {//当前操作符为右括号时，让栈顶到左括号为止的操作符出栈，括号不出现在后缀表达式中
				char temp;
				node * rchild,*lchild;
				temp = op.top();
				while(temp != '(') {//遇到左括号，结束循环
					rchild = ex.top();
					ex.pop();
					lchild = ex.top();
					ex.pop();
					//op栈每弹出一个运算符，ex栈弹出两个子树
					ex.push(createTree(temp,lchild,rchild));
					op.pop();
					temp = op.top();
				}
				op.pop();//弹出左括号
			} 
			//如果当前操作符优先级大于操作符栈的顶部元素，或者当前操作符为左括号，或者栈顶元素为左括号，或者op栈为空，则直接入栈
			else if(s[i] == '(' ||op.empty() == true || mp[s[i]] > mp[op.top()] || op.top() == '(' ) 	op.push(s[i]);
			else {
				//如果当前操作符优先级小于或等于操作符栈的顶部元素，先将顶部元素出栈,直到当前操作符遇到左括号或者大于栈顶元素优先级，再将当前操作符入栈
				char temp;
				node * rchild,*lchild;
				temp = op.top();
				while(mp[s[i]]<=mp[temp]) {//和栈顶元素比较优先级
					if(temp == '(') break;
					rchild = ex.top();
					ex.pop();
					lchild = ex.top();
					ex.pop();
					ex.push(createTree(temp,lchild,rchild));
					op.pop();
					if(op.empty()) break; //如果栈已空，退出循环
					temp = op.top();
				}
				op.push(s[i]);
			}
		}
	}
	while(!op.empty()) {//遍历结束后，如果op栈未空，将op栈中的所有运算符依次弹出计算
		char temp = op.top();
		node*rc = ex.top();
		ex.pop();
		node*lc = ex.top();
		ex.pop();
		ex.push(createTree(temp,lc,rc));
		op.pop();
	}
	cout<<"The sequence traversal is as follows"<<endl;
	postTree(ex.top());
	cout<<endl;
	cout<<getValueOfTree(ex.top());
	return 0;
}

