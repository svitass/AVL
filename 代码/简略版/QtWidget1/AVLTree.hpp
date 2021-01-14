#ifndef AVLTREE_HPP
#define AVLTREE_HPP

#include <iostream>
#include <queue>
#include <algorithm>
#include <sstream>
#include <vector>
using namespace std;

template <class type>
class Node
{
public:
	Node();
	Node(type element);
	~Node();
	int height() const;  //获取当前节点的高度
	bool isLeaf() const;   //判断是否是叶子节点
	type retrieve() const;  //获取当前节点的值
	Node* left() const;  //获取当前节点的左孩子
	Node* right() const;  //获取当前节点的右孩子
	Node* getParent() const;   //获取当前节点的父节点
	void setElement(type element);   //修改当前节点的值
	void setLeft(Node* node);  //修改左孩子
	void setRight(Node* node); //修改右孩子
	void setParent(Node* node);  //修改父节点
	void setCount(int number);    //修改节点出现的次数
	int getCount() const;

private:
	type element;  //节点的值
	int count = 0;  //节点的出现的次数
	Node* leftChild;  //左孩子
	Node* rightChild;  //右孩子
	Node* parent;  //父节点
};

template <class type>
Node<type>::Node()
{
}

template <class type>
Node<type>::Node(type element)
{
	this->element = element;
	this->count = 1;
	this->leftChild = nullptr;
	this->rightChild = nullptr;
	this->parent = nullptr;
}

template <class type>
Node<type>::~Node()
{
}

template<class type>
int Node<type>::height() const {   //获取当前节点的高度
	if (left() == nullptr && right() == nullptr) return 1;
	if (left() != nullptr && right() == nullptr) return 1 + left()->height();
	if (left() == nullptr && right() != nullptr) return 1 + right()->height();
	return 1 + std::max(left()->height(), right()->height());
}

template<class type>
bool Node<type>::isLeaf() const {    //是否是叶子节点
	return left() == nullptr && right() == nullptr;
}


template<class type>
type Node<type>::retrieve() const {
	return element;
}

template<class type>
Node<type>* Node<type>::left() const {
	return leftChild;
}

template<class type>
Node<type>* Node<type>::right() const {
	return rightChild;
}

template<class type>
Node<type>* Node<type>::getParent() const {
	return parent;
}

template<class type>
void Node<type>::setElement(type element) {
	this->element = element;
}


template<class type>
void Node<type>::setLeft(Node<type>* node) {
	leftChild = node;
	if (node != nullptr) node->parent = this;
}

template<class type>
void Node<type>::setRight(Node<type>* node) {
	rightChild = node;
	if (node != nullptr)  node->parent = this;
}

template<class type>
void Node<type>::setParent(Node<type>* node) {
	parent = node;
	if (node != nullptr) {
		if (element < node->retrieve()) {
			node->setLeft(this);
		}
		else {
			node->setRight(this);
		}
	}
}

template<class type>
void Node<type>::setCount(int number) {
	count = number;
}

template<class type>
int Node<type>::getCount() const {
	return count;
}


template<class type>
class AVL {
public:
	AVL();
	AVL(type element);
	void insert(type element);   // 构造一颗AVL树，根节点的元素为element
	void deleteNode(type element);  //删除某个节点
	//void deleteTree();   //删除整棵树
	bool search(type element);  //若元素存在树中，返回true,否则，返回false
	Node<type>* search(Node<type>* node, type element);  //从node节点开始查找element,若找到则返回节点，否则返回一个叶子节点
	Node<type>* findNoBalance(Node<type>* node);  //从node节点开始，找到平衡因子大于1的节点
	void leftRotate(Node<type>* node);  //左旋转
	void rightRotate(Node<type>* node);  //右旋转
	void balanceTree(Node<type>* rotateNode);   //调整树，使树平衡
	//void inOrderTraversal();  //中序遍历树
	Node<type>* getPrior(Node<type>* node);  //中序遍历：获取node的前驱
	Node<type>* getNext(Node<type>* node);   //中序遍历：获取node的后继
	void printTree();   //打印AVL树
	void print();  //打印树
	void writeArray(Node<type>* node, int row, int col, vector<vector<string>>& res, int treeDepth);
	int getNodeHeight(Node<type>* node);  //获取节点的高度，考虑了节点为nullptr的情况
	string ToString(type value);
	Node<type>* getRoot();
private:
	Node<type>* root;  //根节点
};

template<class type>
AVL<type>::AVL() {
	root = nullptr;
}


template<class type>
AVL<type>::AVL(type element) {
	Node<type> node = new Node<type>(element);
	this->root = node;
}

template<class type>
void AVL<type>::insert(type element) {
	if (root == nullptr) {
		root = new Node<type>(element);
		return;
	}
	Node<type>* node = search(root, element);
	if (element == node->retrieve()) {
		node->setCount(node->getCount() + 1);
		return;
	}
	Node<type>* newNode = new Node<type>(element);
	if (element < node->retrieve()) {
		node->setLeft(newNode);
	}
	else {
		node->setRight(newNode);
	}
	// 调节树的平衡
	Node<type>* rotateNode = findNoBalance(node);    //找到不平衡的节点
	balanceTree(rotateNode);
}

template<class type>
void AVL<type>::deleteNode(type element) {   //删除某个节点
	if (!search(element)) return;      //不存在该节点
	Node<type>* node = search(root, element);
	if (node->getCount() > 1) {    //该节点在树中出现了不止一次，直接减少节点的频数
		node->setCount(node->getCount() - 1);
		return;
	}
	Node<type>* parent;
	if (node->isLeaf()) { //叶子节点，直接删除
		parent = node->getParent();
		// 判断node节点是parent的左孩子还是右孩子  若当前节点为根节点，则忽略
		if (parent != nullptr) {
			if (node->retrieve() < parent->retrieve()) {    //左孩子
				parent->setLeft(nullptr);
			}
			else {    //右孩子
				parent->setRight(nullptr);
			}
		}
		else {
			root = nullptr;
		}
		delete node;
	}
	else {  //不是叶子节点，若节点有前驱，则用节点的前驱值代替节点，删除前驱所在的节点，否则用节点的后继代替节点
		Node<type>* prior = getPrior(node);
		if (prior == nullptr) {  //没有前驱，获取后继
			Node<type>* next = getNext(node);
			type nextElem = next->retrieve();
			node->setElement(nextElem);
			node->setCount(next->getCount());
			parent = next->getParent();
			if (nextElem < parent->retrieve()) {    //左孩子
				parent->setLeft(nullptr);
			}
			else {    //右孩子
				parent->setRight(nullptr);
			}
			delete next;
		}
		else {   //有前驱
			type priorElem = prior->retrieve();
			node->setElement(priorElem);
			node->setCount(prior->getCount());
			parent = prior->getParent();
			if (priorElem <= parent->retrieve()) {    //左孩子
				parent->setLeft(nullptr);
			}
			else {    //右孩子
				parent->setRight(nullptr);
			}
			delete prior;
		}
	}
	//待调节树的平衡
	Node<type>* rotateNode = findNoBalance(parent);
	balanceTree(rotateNode);
}

//void deleteTree();   //删除整棵树

template<class type>
bool AVL<type>::search(type element) {
	if (root == nullptr) return false;
	Node<type>* node = search(root, element);
	if (element == node->retrieve()) return true;
	return false;
}

template<class type>
Node<type>* AVL<type>::search(Node<type>* node, type element) {  //返回指定元素所在的节点，若存在，返回节点，否则，返回叶子节点
	if (node->isLeaf() || (node->retrieve() == element)) return node;
	if ((element < node->retrieve()) && (node->left() != nullptr)) return search(node->left(), element);
	if ((element > node->retrieve()) && (node->right() != nullptr)) return search(node->right(), element);
	return node;  //没有满足条件的节点
}

template<class type>
Node<type>* AVL<type>::findNoBalance(Node<type>* node) {
	//if (node == nullptr) return node;     //若node为空指针，说明已经遍历完整颗树了，直接返回空指针
	if (node == nullptr) {
		return node;
	}
	Node<type>* leftTree = node->left();
	Node<type>* rightTree = node->right();
	int leftHeight = getNodeHeight(leftTree);
	int rightHeight = getNodeHeight(rightTree);
	int bf = leftHeight - rightHeight;
	if (-1 <= bf && bf <= 1) {   //平衡：继续查找父节点
		return findNoBalance(node->getParent());
	}
	//不平衡：直接返回当前节点
	return node;
}

template<class type>
void AVL<type>::leftRotate(Node<type>* node) {  //左旋转  RR型
	Node<type>* parent = node->getParent();
	Node<type>* right = node->right();  //节点的右孩子
	Node<type>* rleft = right->left();  //节点右子树的左孩子
	node->setRight(rleft);  //修改当前节点的右孩子
	right->setLeft(node);  //修改右节点的左孩子
	right->setParent(parent);  //修改右节点的父节点
	if (parent == nullptr) {
		root = right;   //无父节点，说明当前旋转的节点为根节点
	}
}

template<class type>
void AVL<type>::rightRotate(Node<type>* node) {    //右旋转   LL型
	Node<type>* parent = node->getParent();   //父节点
	Node<type>* left = node->left();    //节点的左孩子
	Node<type>* lright = left->right();    //左孩子的右子树
	node->setLeft(lright);    //修改当前节点的左孩子
	left->setRight(node);   //修改左节点的右孩子
	left->setParent(parent);  //修改左节点的父节点
	if (parent == nullptr) {
		root = left;
	}
}

template<class type>
void AVL<type>::balanceTree(Node<type>* rotateNode) {  //调整指定节点，使树平衡
	//判定树的不平衡类型
	if (rotateNode == nullptr) return;   //说明树仍然是平衡的，无需调整
	Node<type>* left = rotateNode->left();
	Node<type>* right = rotateNode->right();
	int bf = getNodeHeight(left) - getNodeHeight(right);
	if (bf > 1) {  //左子树高  LL型，LR型
		int bbf = getNodeHeight(left->left()) - getNodeHeight(left->right());
		if (bbf > 0) {   // LL型   右旋转
			rightRotate(rotateNode);
		}
		else {    // LR型  先左旋转左子节点，再右旋转节点
			leftRotate(left);
			rightRotate(rotateNode);
		}
	}
	if (bf < -1) {   //右子树高，RR型， RL型
		int bbf = getNodeHeight(right->left()) - getNodeHeight(right->right());
		if (bbf > 0) {   // RL型  先右旋转右子节点，再左旋转节点
			rightRotate(right);
			leftRotate(rotateNode);
		}
		else {   // RR型    左旋转
			leftRotate(rotateNode);
		}
	}
}

template<class type>
Node<type>* AVL<type>::getPrior(Node<type>* node) {    //中序遍历：获取node的前驱
	Node<type>* left = node->left();   //中序遍历的根节点
	if (left == nullptr) return left;
	while (left->right() != nullptr) {
		left = left->right();
	}
	return left;
}

template<class type>
Node<type>* AVL<type>::getNext(Node<type>* node) {  //中序遍历：获取node的后继
	Node<type>* right = node->right();
	if (right == nullptr) return right;
	while (right->left() != nullptr) {
		right = right->left();
	}
	return right;  //right为nullptr,返回节点的父节点
}

template<class type>
void AVL<type>::printTree() {   // 打印AVL树 使用queue,先进先出
	if (root == nullptr) {
		cout << "AVL树为空" << endl;
		return;
	}
	queue<Node<type>*> q;
	q.push(root);
	while (!q.empty()) {
		Node<type>* node = q.front();  //返回队列的第一个元素
		q.pop();  //删除队列的第一个元素
		int count = node->getCount();
		while (count > 0) {
			cout << node->retrieve() << " ";
			count--;
		}
		//向队列中加入新的节点
		if (node->left() != nullptr) {
			q.push(node->left());
		}
		if (node->right() != nullptr) {
			q.push(node->right());
		}
	}
	cout << endl;
}

template<class type>    //获取节点的高度，考虑了节点为nullptr的情况
int AVL<type>::getNodeHeight(Node<type>* node) {
	if (node == nullptr) return 0;
	return node->height();
}

template<class type>
void AVL<type>::print() {  // 打印出树的形状
	int height = getNodeHeight(root); // 获取树的层数
	if (height == 0) {
		cout << "AVL树为空！" << endl;
		return;
	}
	int h = height * 2 - 1;
	int w = pow(2, height - 1) * 3 + 1;  //???
	vector<vector<string>> res(h, vector<string>(w));
	//string** res = new string[h][w];
	for (int i = 0;i < h;i++) {   //初始化数组
		for (int j = 0;j < w;j++) {
			res[i][j] = " ";
		}
	}
	writeArray(root, 0, w / 2, res, height);
	//res已经保存了所有要打印的数据，打印res
	for (int i = 0;i < h;i++) {
		string str = "";
		for (int j = 0;j < w;j++) {
			string con = res[i][j];
			str = str + con;
			/*if (con.length() > 1 && j <= w - 1) {
				i += con.length() > 4 ? 2 : con.length() - 1;
			}*/
		}
		cout << str << endl;
	}
}

template<class type>
void AVL<type>::writeArray(Node<type>* node, int row, int col, vector<vector<string>>& res, int treeDepth) {
	if (node == nullptr) return;
	res[row][col] = ToString(node->retrieve());
	int level = (row + 1) / 2;   //获取树的当前层数
	if (level == treeDepth) return;   //最后一层
	int gap = treeDepth - level - 1;  //相邻行元素之间的间隔
	if (node->left() != nullptr) {
		res[row + 1][col - gap] = "/";
		writeArray(node->left(), row + 2, col - gap * 2, res, treeDepth);
	}
	if (node->right() != nullptr) {
		res[row + 1][col + gap] = "\\";
		writeArray(node->right(), row + 2, col + gap * 2, res, treeDepth);
	}
}

template<class type>
string AVL<type>::ToString(type value) {
	stringstream str;
	str.str("");
	str << value;
	return str.str();
}

template<class type>
Node<type>* AVL<type>::getRoot() {
	return root;
}
////test
//int main()
//{
//	/*Node<int>* root = new Node<int>(2);
//	Node<int>* left = new Node<int>(1);
//	Node<int>* right = new Node<int>(3);
//	root->setLeft(left);
//	root->setRight(right);
//	cout << "根节点：" << root->retrieve() << "    左孩子：" << root->left()->retrieve() << "     右孩子：" << root->right()->retrieve() << endl;
//	cout << "根节点的高度：" << root->height() << "   左孩子的高度：" << root->left()->height() << "    右孩子的高度：" << root->right()->height() << endl;
//	*/
//	string funcName;
//	int element;
//	AVL<int> avl;
//	/*cout << "测试findNoBalance" << endl;
//	avl.findNoBalance(nullptr);
//	cout << "测试结束" << endl;*/
//	while (true) {
//		cin >> funcName;
//		if (funcName == "insert") {
//			cin >> element;
//			avl.insert(element);
//		}
//		else if (funcName == "deleteNode") {
//			cin >> element;
//			avl.deleteNode(element);
//		}
//		else if (funcName == "printTree") {
//			avl.printTree();
//		}
//		else if (funcName == "print") {
//			avl.print();
//		}
//		else {
//			//break;
//			continue;
//		}
//	}
//	return 0;
//}
#endif // !AVLTREE_HPP