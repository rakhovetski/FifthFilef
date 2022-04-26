#include <iostream>
using namespace std;

#include <utility>
#include <ostream>
#include <istream>
#include <fstream>
#include <vector>

using namespace std;

struct Node
{
	int data;
	Node* left;
	Node* right;
	Node(int data)
	{
		this->data = data;
		left = right = NULL;
	}
	Node() :data(0), left(nullptr), right(nullptr) {}
	Node(int _data, Node* leftPtr, Node* rightPtr) : data(_data), left(leftPtr), right(rightPtr) {}
};

class BinaryTree {
public:
	using PNode = Node*;
	BinaryTree();
	explicit BinaryTree(int x);
	void clear();
	~BinaryTree();
	BinaryTree(const BinaryTree& obj);
	BinaryTree(BinaryTree&& obj) noexcept;
	bool operator==(const BinaryTree& obj);
	bool operator!=(const BinaryTree& obj);
	BinaryTree& operator=(const BinaryTree& obj);
	BinaryTree& operator=(BinaryTree&& obj) noexcept;
	void insert(int x, const vector<bool>& boolArr);
	friend ostream& operator<<(ostream& out, BinaryTree& obj);
	unsigned countOfEven();
	bool allPositive();
	void deleteLeaves();
	double average();
	vector<bool> searchElem(int x);
	void levelPrint(ostream& out);
	void levelPrint(PNode elem, unsigned level, ostream& out);
	bool isBSTUtil(Node* s, Node*& left);
	bool isBST(Node* root);
	PNode root;


private:
	void clear(PNode elem);
	bool isEqual(PNode elem, PNode elem1);
	PNode copyTree(PNode obj);
	unsigned countOfEven(PNode elem);
	bool allPositive(PNode elem);
	bool deleteLeaves(PNode elem);
	void average(PNode elem, pair<long, int>& values);
	bool searchElem(PNode elem, vector<bool>& arr, int x);
};

BinaryTree::BinaryTree() : root(nullptr) {}

BinaryTree::BinaryTree(int x) : root(new Node) {
	root->data = x;
	root->left = nullptr;
	root->right = nullptr;
}

void BinaryTree::clear() {
	if (root != nullptr) {
		clear(root);
		root = nullptr;
	}
}

void BinaryTree::clear(PNode node) {
	if (node != nullptr) {
		clear(node->left);
		clear(node->right);
		delete node;
	}
}

BinaryTree::~BinaryTree() {
	clear();
}

BinaryTree::BinaryTree(const BinaryTree& obj) {
	root = copyTree(obj.root);
}

BinaryTree::PNode BinaryTree::copyTree(BinaryTree::PNode obj) {
	if (obj != nullptr) {
		return new Node(obj->data, copyTree(obj->left), copyTree(obj->right));
	}
	return nullptr;
}

BinaryTree::BinaryTree(BinaryTree&& obj) noexcept : root(obj.root) {
	obj.root = nullptr;
}

bool BinaryTree::operator==(const BinaryTree& obj) {
	return isEqual(root, obj.root);
}

bool BinaryTree::isEqual(BinaryTree::PNode const elem, BinaryTree::PNode const elem1) {
	if (elem == nullptr) {
		return elem1 == nullptr;
	}
	else {
		return elem->data == elem1->data && elem->right == elem1->right && elem->left == elem1->left
			&& isEqual(elem->left, elem1->left) && isEqual(elem->right, elem1->right);
	}
}

bool BinaryTree::operator!=(const BinaryTree& obj) {
	return !(*this == obj);
}

BinaryTree& BinaryTree::operator=(const BinaryTree& obj) {
	if (this != &obj) {
		delete root;
		root = copyTree(obj.root);
	}
	return *this;
}

BinaryTree& BinaryTree::operator=(BinaryTree&& obj) noexcept {
	if (this != &obj) {
		delete root;
		root = obj.root;
		obj.root = nullptr;
	}
	return (*this);
}

void BinaryTree::insert(int x, const vector<bool>& boolArr) {
	if (root == nullptr) {
		if (!boolArr.empty()) {
			throw exception();
		}
		root = new Node;
		root->data = x;
	}
	else {
		PNode current = root;
		unsigned arrSize = boolArr.size();
		for (unsigned i = 0; i < arrSize; i++) {
			if (i == arrSize - 1) {
				PNode old = current;
				current = new Node;
				boolArr[i] ? old->right = current : old->left = current;
				current->left = nullptr;
				current->right = nullptr;
			}
			else if (boolArr[i] && current->right != nullptr) {
				current = current->right;
			}
			else if (!boolArr[i] && current->left != nullptr) {
				current = current->left;
			}
			else {
				throw exception();
			}
		}
		current->data = x;
	}

}

ostream& operator<<(ostream& out, BinaryTree& obj) {
	obj.levelPrint(out);
	return out;
}
void BinaryTree::levelPrint(ostream& out) {
	levelPrint(root, 0, out);
}

void BinaryTree::levelPrint(BinaryTree::PNode elem, unsigned int level, ostream& out)
{
	if (elem != nullptr) {
		for (unsigned i = 0; i != level; i++) {
			out << "\t";
		}
		out << elem->data << "\n";
		levelPrint(elem->left, level + 1, out);
		levelPrint(elem->right, level + 1, out);
	}
}

unsigned BinaryTree::countOfEven() {
	return countOfEven(root);
}

unsigned BinaryTree::countOfEven(BinaryTree::PNode elem) {
	return (elem == nullptr ? 0 :
		(elem->data % 2 == 0) + countOfEven(elem->left) + countOfEven(elem->right));
}

bool BinaryTree::allPositive() {
	return allPositive(root);
}

bool BinaryTree::allPositive(const BinaryTree::PNode elem) {
	return (elem == nullptr ? true : elem->data > 0 && allPositive(elem->left) && allPositive(elem->right));
}

void BinaryTree::deleteLeaves() {
	deleteLeaves(root);
}

bool BinaryTree::deleteLeaves(BinaryTree::PNode elem) {
	if (elem == nullptr) {
		return false;
	}
	else {
		if (elem->right == nullptr && elem->left == nullptr) {
			delete elem;
			return true;
		}
		if (deleteLeaves(elem->left)) {
			elem->left = nullptr;
		}
		if (deleteLeaves(elem->right)) {
			elem->right = nullptr;
		}
		return false;
	}
}

double BinaryTree::average() {
	pair<long, int> res(0, 0);
	average(root, res);
	return ((double)res.first) / res.second;
}

void BinaryTree::average(const BinaryTree::PNode elem, pair<long, int>& values) {
	if (elem != nullptr) {
		values.first += elem->data;
		values.second += 1;
		average(elem->left, values);
		average(elem->right, values);
	}
}

vector<bool> BinaryTree::searchElem(int x) {
	vector<bool> res{};
	if (!searchElem(root, res, x)) {
		throw exception();
	}
	return res;
}

bool BinaryTree::searchElem(BinaryTree::PNode const elem, vector<bool>& arr, int x) {
	if (elem != nullptr) {
		if (elem->data == x) {
			return true;
		}
		arr.push_back(false);
		if (searchElem(elem->left, arr, x)) {
			return true;
		}
		arr.pop_back();
		arr.push_back(true);
		if (searchElem(elem->right, arr, x)) {
			return true;
		}
		arr.pop_back();
	}
	return false;
}

bool BinaryTree::isBSTUtil(struct Node* root, Node*& prev)
{
	if (root)
	{
		if (!isBSTUtil(root->left, prev)) return false;
		if (prev != NULL && root->data <= prev->data) return false;
		prev = root;
		return isBSTUtil(root->right, prev);
	}
	return true;
}
bool BinaryTree::isBST(Node* root)
{
	Node* prev = NULL;
	return isBSTUtil(root, prev);
}


int main() {
	//insert and print
	vector<bool> arr1{};
	BinaryTree binaryTree1(2);

	arr1.push_back(true);
	binaryTree1.insert(3, arr1);
	arr1.pop_back();
	arr1.push_back(false);
	binaryTree1.insert(11, arr1);
	arr1.push_back(false);
	binaryTree1.insert(7, arr1);

	std::cout << binaryTree1;

	//CountOfEven, allPositives, average
	vector<bool> arr2{};
	BinaryTree binaryTree2(22);

	arr2.push_back(true);
	binaryTree2.insert(4, arr2);
	arr2.pop_back();
	arr2.push_back(false);
	binaryTree2.insert(18, arr2);
	arr2.push_back(false);
	binaryTree2.insert(5, arr2);

	cout << binaryTree2.countOfEven() << endl; //3
	cout << binaryTree2.average() << endl; //12.25
	cout << binaryTree2.allPositive() << endl; //true

	binaryTree2.insert(-10, arr2);

	cout << binaryTree2.allPositive() << endl; //false;

	vector<bool> arr3;

	BinaryTree binaryTree3;

	/*      8
	 *   3    10
	 * -1  6    12
	 *      8  7    */
	binaryTree3.insert(8, arr3);
	arr3.push_back(false);
	binaryTree3.insert(3, arr3);
	arr3.push_back(false);
	binaryTree3.insert(-1, arr3);
	arr3.pop_back();
	arr3.push_back(true);
	binaryTree3.insert(6, arr3);
	arr3.push_back(true);
	binaryTree3.insert(8, arr3);

	arr3.clear();
	arr3.push_back(true);
	binaryTree3.insert(10, arr3);
	arr3.push_back(true);
	binaryTree3.insert(12, arr3);
	arr3.push_back(false);
	binaryTree3.insert(7, arr3);
	BinaryTree binaryTree4 = binaryTree3;

	/*      8
	 *   3    10
	 * -1  6    12
	 *         14   */

	binaryTree4.deleteLeaves();
	binaryTree4.insert(14, arr3);


	vector<bool> arr4{};
	BinaryTree binaryTree5;

	/*      8
	 *   3    10
	 * -1  6    12
	 *      8  7    */
	binaryTree5.insert(8, arr4);
	arr4.push_back(false);
	binaryTree5.insert(3, arr4);
	arr4.push_back(false);
	binaryTree5.insert(-1, arr4);
	arr4.pop_back();
	arr4.push_back(true);
	binaryTree5.insert(6, arr4);
	arr4.push_back(true);
	binaryTree5.insert(8, arr4);

	arr4.clear();
	arr4.push_back(true);
	binaryTree5.insert(10, arr4);
	arr4.push_back(true);
	binaryTree5.insert(12, arr4);
	arr4.push_back(false);
	binaryTree5.insert(7, arr4);

	arr4.clear();
	arr4.push_back(false);
	arr4.push_back(true);
	vector<bool> bArr = binaryTree5.searchElem(6);
	if (arr4 == bArr) {
		cout << true;
	}
	//binaryTree5.searchElem(99);

	BinaryTree binaryTree10(7);
	binaryTree10.root->left = new Node(0);
	binaryTree10.root->right = new Node(14);
	binaryTree10.root->left->left = new Node(-1);
	binaryTree10.root->left->right = new Node(5);

	ofstream fileOf("test.txt");

	binaryTree10.levelPrint(binaryTree10.root, 0, fileOf);

	if (binaryTree10.isBST(binaryTree10.root))
		cout << " Is BST";
	else
		cout << " Not a BST";
	return 0;

}