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
	int height() const;  //��ȡ��ǰ�ڵ�ĸ߶�
	bool isLeaf() const;   //�ж��Ƿ���Ҷ�ӽڵ�
	type retrieve() const;  //��ȡ��ǰ�ڵ��ֵ
	Node* left() const;  //��ȡ��ǰ�ڵ������
	Node* right() const;  //��ȡ��ǰ�ڵ���Һ���
	Node* getParent() const;   //��ȡ��ǰ�ڵ�ĸ��ڵ�
	void setElement(type element);   //�޸ĵ�ǰ�ڵ��ֵ
	void setLeft(Node* node);  //�޸�����
	void setRight(Node* node); //�޸��Һ���
	void setParent(Node* node);  //�޸ĸ��ڵ�
	void setCount(int number);    //�޸Ľڵ���ֵĴ���
	int getCount() const;

private:
	type element;  //�ڵ��ֵ
	int count = 0;  //�ڵ�ĳ��ֵĴ���
	Node* leftChild;  //����
	Node* rightChild;  //�Һ���
	Node* parent;  //���ڵ�
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
int Node<type>::height() const {   //��ȡ��ǰ�ڵ�ĸ߶�
	if (left() == nullptr && right() == nullptr) return 1;
	if (left() != nullptr && right() == nullptr) return 1 + left()->height();
	if (left() == nullptr && right() != nullptr) return 1 + right()->height();
	return 1 + std::max(left()->height(), right()->height());
}

template<class type>
bool Node<type>::isLeaf() const {    //�Ƿ���Ҷ�ӽڵ�
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
	void insert(type element);   // ����һ��AVL�������ڵ��Ԫ��Ϊelement
	void deleteNode(type element);  //ɾ��ĳ���ڵ�
	//void deleteTree();   //ɾ��������
	bool search(type element);  //��Ԫ�ش������У�����true,���򣬷���false
	Node<type>* search(Node<type>* node, type element);  //��node�ڵ㿪ʼ����element,���ҵ��򷵻ؽڵ㣬���򷵻�һ��Ҷ�ӽڵ�
	Node<type>* findNoBalance(Node<type>* node);  //��node�ڵ㿪ʼ���ҵ�ƽ�����Ӵ���1�Ľڵ�
	void leftRotate(Node<type>* node);  //����ת
	void rightRotate(Node<type>* node);  //����ת
	void balanceTree(Node<type>* rotateNode);   //��������ʹ��ƽ��
	//void inOrderTraversal();  //���������
	Node<type>* getPrior(Node<type>* node);  //�����������ȡnode��ǰ��
	Node<type>* getNext(Node<type>* node);   //�����������ȡnode�ĺ��
	void printTree();   //��ӡAVL��
	void print();  //��ӡ��
	void writeArray(Node<type>* node, int row, int col, vector<vector<string>>& res, int treeDepth);
	int getNodeHeight(Node<type>* node);  //��ȡ�ڵ�ĸ߶ȣ������˽ڵ�Ϊnullptr�����
	string ToString(type value);
	Node<type>* getRoot();
private:
	Node<type>* root;  //���ڵ�
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
	// ��������ƽ��
	Node<type>* rotateNode = findNoBalance(node);    //�ҵ���ƽ��Ľڵ�
	balanceTree(rotateNode);
}

template<class type>
void AVL<type>::deleteNode(type element) {   //ɾ��ĳ���ڵ�
	if (!search(element)) return;      //�����ڸýڵ�
	Node<type>* node = search(root, element);
	if (node->getCount() > 1) {    //�ýڵ������г����˲�ֹһ�Σ�ֱ�Ӽ��ٽڵ��Ƶ��
		node->setCount(node->getCount() - 1);
		return;
	}
	Node<type>* parent;
	if (node->isLeaf()) { //Ҷ�ӽڵ㣬ֱ��ɾ��
		parent = node->getParent();
		// �ж�node�ڵ���parent�����ӻ����Һ���  ����ǰ�ڵ�Ϊ���ڵ㣬�����
		if (parent != nullptr) {
			if (node->retrieve() < parent->retrieve()) {    //����
				parent->setLeft(nullptr);
			}
			else {    //�Һ���
				parent->setRight(nullptr);
			}
		}
		else {
			root = nullptr;
		}
		delete node;
	}
	else {  //����Ҷ�ӽڵ㣬���ڵ���ǰ�������ýڵ��ǰ��ֵ����ڵ㣬ɾ��ǰ�����ڵĽڵ㣬�����ýڵ�ĺ�̴���ڵ�
		Node<type>* prior = getPrior(node);
		if (prior == nullptr) {  //û��ǰ������ȡ���
			Node<type>* next = getNext(node);
			type nextElem = next->retrieve();
			node->setElement(nextElem);
			node->setCount(next->getCount());
			parent = next->getParent();
			if (nextElem < parent->retrieve()) {    //����
				parent->setLeft(nullptr);
			}
			else {    //�Һ���
				parent->setRight(nullptr);
			}
			delete next;
		}
		else {   //��ǰ��
			type priorElem = prior->retrieve();
			node->setElement(priorElem);
			node->setCount(prior->getCount());
			parent = prior->getParent();
			if (priorElem <= parent->retrieve()) {    //����
				parent->setLeft(nullptr);
			}
			else {    //�Һ���
				parent->setRight(nullptr);
			}
			delete prior;
		}
	}
	//����������ƽ��
	Node<type>* rotateNode = findNoBalance(parent);
	balanceTree(rotateNode);
}

//void deleteTree();   //ɾ��������

template<class type>
bool AVL<type>::search(type element) {
	if (root == nullptr) return false;
	Node<type>* node = search(root, element);
	if (element == node->retrieve()) return true;
	return false;
}

template<class type>
Node<type>* AVL<type>::search(Node<type>* node, type element) {  //����ָ��Ԫ�����ڵĽڵ㣬�����ڣ����ؽڵ㣬���򣬷���Ҷ�ӽڵ�
	if (node->isLeaf() || (node->retrieve() == element)) return node;
	if ((element < node->retrieve()) && (node->left() != nullptr)) return search(node->left(), element);
	if ((element > node->retrieve()) && (node->right() != nullptr)) return search(node->right(), element);
	return node;  //û�����������Ľڵ�
}

template<class type>
Node<type>* AVL<type>::findNoBalance(Node<type>* node) {
	//if (node == nullptr) return node;     //��nodeΪ��ָ�룬˵���Ѿ��������������ˣ�ֱ�ӷ��ؿ�ָ��
	if (node == nullptr) {
		return node;
	}
	Node<type>* leftTree = node->left();
	Node<type>* rightTree = node->right();
	int leftHeight = getNodeHeight(leftTree);
	int rightHeight = getNodeHeight(rightTree);
	int bf = leftHeight - rightHeight;
	if (-1 <= bf && bf <= 1) {   //ƽ�⣺�������Ҹ��ڵ�
		return findNoBalance(node->getParent());
	}
	//��ƽ�⣺ֱ�ӷ��ص�ǰ�ڵ�
	return node;
}

template<class type>
void AVL<type>::leftRotate(Node<type>* node) {  //����ת  RR��
	Node<type>* parent = node->getParent();
	Node<type>* right = node->right();  //�ڵ���Һ���
	Node<type>* rleft = right->left();  //�ڵ�������������
	node->setRight(rleft);  //�޸ĵ�ǰ�ڵ���Һ���
	right->setLeft(node);  //�޸��ҽڵ������
	right->setParent(parent);  //�޸��ҽڵ�ĸ��ڵ�
	if (parent == nullptr) {
		root = right;   //�޸��ڵ㣬˵����ǰ��ת�Ľڵ�Ϊ���ڵ�
	}
}

template<class type>
void AVL<type>::rightRotate(Node<type>* node) {    //����ת   LL��
	Node<type>* parent = node->getParent();   //���ڵ�
	Node<type>* left = node->left();    //�ڵ������
	Node<type>* lright = left->right();    //���ӵ�������
	node->setLeft(lright);    //�޸ĵ�ǰ�ڵ������
	left->setRight(node);   //�޸���ڵ���Һ���
	left->setParent(parent);  //�޸���ڵ�ĸ��ڵ�
	if (parent == nullptr) {
		root = left;
	}
}

template<class type>
void AVL<type>::balanceTree(Node<type>* rotateNode) {  //����ָ���ڵ㣬ʹ��ƽ��
	//�ж����Ĳ�ƽ������
	if (rotateNode == nullptr) return;   //˵������Ȼ��ƽ��ģ��������
	Node<type>* left = rotateNode->left();
	Node<type>* right = rotateNode->right();
	int bf = getNodeHeight(left) - getNodeHeight(right);
	if (bf > 1) {  //��������  LL�ͣ�LR��
		int bbf = getNodeHeight(left->left()) - getNodeHeight(left->right());
		if (bbf > 0) {   // LL��   ����ת
			rightRotate(rotateNode);
		}
		else {    // LR��  ������ת���ӽڵ㣬������ת�ڵ�
			leftRotate(left);
			rightRotate(rotateNode);
		}
	}
	if (bf < -1) {   //�������ߣ�RR�ͣ� RL��
		int bbf = getNodeHeight(right->left()) - getNodeHeight(right->right());
		if (bbf > 0) {   // RL��  ������ת���ӽڵ㣬������ת�ڵ�
			rightRotate(right);
			leftRotate(rotateNode);
		}
		else {   // RR��    ����ת
			leftRotate(rotateNode);
		}
	}
}

template<class type>
Node<type>* AVL<type>::getPrior(Node<type>* node) {    //�����������ȡnode��ǰ��
	Node<type>* left = node->left();   //��������ĸ��ڵ�
	if (left == nullptr) return left;
	while (left->right() != nullptr) {
		left = left->right();
	}
	return left;
}

template<class type>
Node<type>* AVL<type>::getNext(Node<type>* node) {  //�����������ȡnode�ĺ��
	Node<type>* right = node->right();
	if (right == nullptr) return right;
	while (right->left() != nullptr) {
		right = right->left();
	}
	return right;  //rightΪnullptr,���ؽڵ�ĸ��ڵ�
}

template<class type>
void AVL<type>::printTree() {   // ��ӡAVL�� ʹ��queue,�Ƚ��ȳ�
	if (root == nullptr) {
		cout << "AVL��Ϊ��" << endl;
		return;
	}
	queue<Node<type>*> q;
	q.push(root);
	while (!q.empty()) {
		Node<type>* node = q.front();  //���ض��еĵ�һ��Ԫ��
		q.pop();  //ɾ�����еĵ�һ��Ԫ��
		int count = node->getCount();
		while (count > 0) {
			cout << node->retrieve() << " ";
			count--;
		}
		//������м����µĽڵ�
		if (node->left() != nullptr) {
			q.push(node->left());
		}
		if (node->right() != nullptr) {
			q.push(node->right());
		}
	}
	cout << endl;
}

template<class type>    //��ȡ�ڵ�ĸ߶ȣ������˽ڵ�Ϊnullptr�����
int AVL<type>::getNodeHeight(Node<type>* node) {
	if (node == nullptr) return 0;
	return node->height();
}

template<class type>
void AVL<type>::print() {  // ��ӡ��������״
	int height = getNodeHeight(root); // ��ȡ���Ĳ���
	if (height == 0) {
		cout << "AVL��Ϊ�գ�" << endl;
		return;
	}
	int h = height * 2 - 1;
	int w = pow(2, height - 1) * 3 + 1;  //???
	vector<vector<string>> res(h, vector<string>(w));
	//string** res = new string[h][w];
	for (int i = 0;i < h;i++) {   //��ʼ������
		for (int j = 0;j < w;j++) {
			res[i][j] = " ";
		}
	}
	writeArray(root, 0, w / 2, res, height);
	//res�Ѿ�����������Ҫ��ӡ�����ݣ���ӡres
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
	int level = (row + 1) / 2;   //��ȡ���ĵ�ǰ����
	if (level == treeDepth) return;   //���һ��
	int gap = treeDepth - level - 1;  //������Ԫ��֮��ļ��
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
//	cout << "���ڵ㣺" << root->retrieve() << "    ���ӣ�" << root->left()->retrieve() << "     �Һ��ӣ�" << root->right()->retrieve() << endl;
//	cout << "���ڵ�ĸ߶ȣ�" << root->height() << "   ���ӵĸ߶ȣ�" << root->left()->height() << "    �Һ��ӵĸ߶ȣ�" << root->right()->height() << endl;
//	*/
//	string funcName;
//	int element;
//	AVL<int> avl;
//	/*cout << "����findNoBalance" << endl;
//	avl.findNoBalance(nullptr);
//	cout << "���Խ���" << endl;*/
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