/***************************************************************
Michael Schott
BinaryTreeSymbolTable.hpp
Program 3
Implements a Binary Tree Symbol Table and its essential functions:
put, remove, size, empty, etc. All nodes contain an integer Key
and Value and keep track of their size.
***************************************************************/

#ifndef TREE_SYMBOL_TABLE_H
#define TREE_SYMBOL_TABLE_H

#include <algorithm>

#include "SymbolTable.hpp"

template <typename Key, typename Value>
class BinaryTreeSymbolTable : public SymbolTable<Key, Value>
{
protected:

	struct BinaryTreeNode
	{
		Key _key;
		Value _value;
		BinaryTreeNode* _left;
		BinaryTreeNode* _right;
		unsigned _size;

		BinaryTreeNode(const Key& key = Key{},
			const Value& value = Value{},
			unsigned size = 0,
			BinaryTreeNode* ell = nullptr,
			BinaryTreeNode* r = nullptr)
			: _key{ key }, _value{ value }, _size{ size }, _left{ ell }, _right{ r } {}

		BinaryTreeNode(const BinaryTreeNode& that)
			: _key{ that._key }, _value{ that._value }, _size{ that._size }, _left{ that._left }, _right{ that._right } {}

		~BinaryTreeNode()
		{
			if (_left != nullptr)
			{
				delete _left;
				_left = nullptr;
			}
			if (_right != nullptr)
			{
				delete _right;
				_right = nullptr;
			}
			_size = 0;
		}
	};

	// Key value comparison (less than)
	bool keyLessThan(const Key& lhs, const Key& rhs) const { return lhs < rhs; }

	// Equality of key values
	bool keyEquals(const Key& lhs, const Key& rhs) const { return lhs == rhs; }

	// Equality of key values
	bool keyLessThanOrEquals(const Key& lhs, const Key& rhs) const
	{
		return keyEquals(lhs, rhs) || keyLessThan(lhs, rhs);
	}

	// The container of the <key, value> pairs
	BinaryTreeNode* _root;

public:

	BinaryTreeSymbolTable() : _root{ nullptr } {}

	virtual ~BinaryTreeSymbolTable() { delete _root; }

	virtual ~BinaryTreeSymbolTable() { clear(); }

	//
	// TODO
	//
private:

	//Puts node in appropriate place in the table.
	BinaryTreeNode* putHelper(BinaryTreeNode* node, const Key& key, const Value& value)
	{
		//Create node if at leaf.
		if (node == nullptr) return new BinaryTreeNode(key, value, 1);

		//Go down left subtree if key is less than the node's key.
		else if (keyLessThan(key, node->_key)) node->_left = putHelper(node->_left, key, value);

		//Go down right subtree if key is greater than the node's key.
		else if (keyLessThan(node->_key, key)) node->_right = putHelper(node->_right, key, value);

		//Update value
		else node->_value = value; // Duplicate: overwrite the value

		//Update size
		node->_size = 1 + size(node->_left) + size(node->_right);

		return node;
	}

	//Returns node size
	int size(const BinaryTreeNode* const node) const
	{
		//If nullptr return 0 else return size
		return node == nullptr ? 0 : node->_size;
	}

	//Returns value of given node
	Value getHelper(BinaryTreeNode* node, Key key) const
	{
		//If key is at this node return its value
		if (node->_key == key)
			return node->_value;

		//Else recursive call left 
		else if (key < node->_key && node->_left != nullptr)
			getHelper(node->_left, key);

		//Else recursive call right
		else if (key > node->_key && node->_right != nullptr)
			getHelper(node->_right, key);

		else
		return -1;
	}

	//Gets parent of some node with Key child
	BinaryTreeNode* getParent(BinaryTreeNode* node, Key child)
	{
		if (empty())
		return nullptr;

		//Returns left/right child if key is match Key child
		if ((node->_right != nullptr && node->_right->_key == child) || (node->_left != nullptr && node->_left->_key == child))
			return node;
	
		//else recursive call left
	else if (child < node->_key && node->_left != nullptr)
		getParent(node->_left, child);

		//else recursive call right
	else if (child > node->_key && node->_right != nullptr)
		getParent(node->_right, child);

	else
		return nullptr;
	}

	//Updates size of all nodes from bottom up
	void updateSize(BinaryTreeNode* node)
	{	
		if (node != nullptr)
		{
			//If leaf, size = 1
			if(node->_right == nullptr && node->_left == nullptr)
				node->_size = 1;

			//two children: recursive call left then right then calculate size
			else if (node->_right != nullptr && node->_left != nullptr)
			{
				updateSize(node->_left);
				updateSize(node->_right);
				node->_size = node->_left->_size + node->_right->_size + 1;
			}
			//one child: right recursive call right then update size
			else if (node->_right != nullptr && node->_left == nullptr)
			{
				updateSize(node->_right);
				node->_size = node->_right->_size + 1;
			}
			//one child: left recursive call then update size
			else if (node->_left != nullptr && node->_right == nullptr)
			{
				updateSize(node->_left);
				node->_size = node->_left->_size + 1;
			}
		}
	}

	//14th time is the charm.
	//Overwrites nodes in the tree recursively until the node
	//associated with the given key is overwritten completely.
	BinaryTreeNode* removeHelper(BinaryTreeNode* node, Key key)
	{

		if (node == nullptr)
			return nullptr;

		//Go down left subtree if not at the to be deleted node
		if (node->_key > key) 
			node->_left = removeHelper(node->_left, key);

		//Go down right subtree if not at the to be deleted node
		else if (node->_key < key) 
			node->_right = removeHelper(node->_right, key);

		else
		{
			//One Child: Right /No Children
			if (node->_left == nullptr)
			{
				BinaryTreeNode* ReplacementNode = node->_right;
				return ReplacementNode;
			}

			//One Child: Left
			else if (node->_right == nullptr)
			{
				BinaryTreeNode* ReplacementNode = node->_left;
				return ReplacementNode;
			}

			//Two Children
			//Gets infix successor
			BinaryTreeNode* successor = minHelper(node->_right);
			//Overwrite to be deleted node's key
			node->_key = successor->_key;
			//Overwrite to be deleted node's value
			node->_value = successor->_value;
			//Handles possible right child of successor
			node->_right = removeHelper(node->_right, successor->_key);

		}

		//Updates size
		node->_size = 1 + size(node->_left) + size(node->_right);

		return node;
	}

	//Gets node with the lowest key
	BinaryTreeNode* minHelper(BinaryTreeNode* node) const
	{

		if (empty())
			return nullptr;

		if (node == nullptr)
			return nullptr;

		//Creates node to be assigned to minimum node.
		BinaryTreeNode* minNode = node;

		//Gets farthest node to the left
		while (minNode->_left != nullptr)
		{
			minNode = minNode->_left;
		}

		
		//if (node->_left == nullptr)
			return minNode;

		//Else recursive call left
		//else minHelper(node->_left);

	}

	//Gets node with the highest key
	BinaryTreeNode* maxHelper(BinaryTreeNode* node) const
	{
		if (empty())
			return nullptr;

		if (node == nullptr)
			return nullptr;

		//Creates node to be assigned to maximum node
		BinaryTreeNode* maxNode = node;

		while (maxNode->_right != nullptr)
		{
			maxNode = maxNode->_right;
		}
		//Checks if node is as far right as possible
	//	if (node->_right == nullptr)
			return maxNode;

		//Else recursive call right
		//else maxHelper(node->_right);
	}

	//Gets highest key less than or equal to given key
	Key floorHelper(BinaryTreeNode* node, Key key) const
	{
		if (node->_key == key)
			return node->_key;

		if (node == nullptr)
			return 0;
		
		//Returns key if node key is less than desired key and right key is greater than desired key
		if(node->_right != nullptr)
			if ((node->_key < key && node->_right->_key > key) && (node->_left == nullptr))
				return node->_key;

		//Else recursive call right
			if (node->_key < key && node->_right != nullptr)
				floorHelper(node->_right, key);
	}

	//Gets lowest key greater than or equal to given key
	Key ceilingHelper(BinaryTreeNode* node, Key key) const
	{
		if (node->_key == key)
			return node->_key;

		if (node == nullptr)
			return 0;

		//Returns key if node key is less than desired key and right key is greater than desired key
		if (node->_key < key && node->_right->_key > key)
				return node->_right->_key;

		//Else recursive call right
		if (node->_key < key && node->_right != nullptr)
			ceilingHelper(node->_right, key);
	}

	//REturns node with the given key
	BinaryTreeNode* getNode(BinaryTreeNode* node, Key key) const
	{
		if (empty())
			return nullptr;

		//If found return node
		else if (node->_key == key)
				return node;

		//else recursive call left
		else if (key < node->_key && node->_left != nullptr)
				getNode(node->_left, key);

		//else recursive call right
		else if (key > node->_key && node->_right != nullptr)
				getNode(node->_right, key);

		else
		return nullptr;
		
	}

	
	//Gets node of rank k
	Key selectHelper(BinaryTreeNode* node, int k) const
	{
		//Find rank of key
		int nodeRank = rank(node->_key);

		//If rank = k return key
		if (nodeRank == k)
			return node->_key;

		//Else recursive call left
		else if (nodeRank > k && node->_left != nullptr)
			selectHelper(node->_left, k);

		//Else recursive call right
		else if (nodeRank < k && node->_right != nullptr)
			selectHelper(node->_right, k);

		else if (node->_right == nullptr && node->_left == nullptr)
			return node->_key;
	}

	//Returns vector of keys in sorted order
	std::vector<Key> keysHelper(BinaryTreeNode* node, Key low, Key high) const
	{
		std::vector<Key> keys;

		if (empty())
			return keys;

		//Checks if key is in table, if it is push it onto the vecotor
		for (int i = low; i <= high; i++)
		{
			BinaryTreeNode* lowNode = getNode(_root, i);
			if (lowNode != nullptr)
				keys.push_back(i);
		}
		return keys;		
	}

public:

	// Puts key-value pair into the table
	virtual void put(const Key& key, const Value& val = Value{})
	{
		_root = putHelper(_root, key, val);
	}

	// acquire the value paired with key
	virtual bool get(const Key& key, Value& val = Value{}) const
	{
		if (empty())
			return false;

		BinaryTreeNode* newnode = getNode(_root, key);

		if (newnode == nullptr)
			return false;

		//Assigns val to value of the node
		val = newnode->_value;

		return true;
	}

	// remove key (and its value) from table
	virtual void remove(const Key& key)
	{
		if (empty())
			return;

		_root = removeHelper(_root, key);
	}

	// Is there a value paired with key?
	virtual bool contains(const Key& key) const
	{
		if (empty())
			return false;

		//Sets val to value of given key
		Value val = getHelper(_root, key);

		//Checks if that key wasn't in the table
		if (val == -1)
			return false;

		return true;
	}

	// Is the table empty?
	virtual bool empty() const
	{
		if (_root == nullptr)
			return true;

		return false;
	}

	// Number of key-value pairs.
	virtual int size() const
	{
		if (!empty())
			return size(_root);

		else
		return 0;
	}

	// Smallest key
	virtual bool min(Key& key = Key{}) const
	{
		if (empty())
			return false;

		//Gets node with smallest key
		key = minHelper(_root)->_key;
		return true;
	}

	// Largest key
	virtual bool max(Key& key = Key{}) const
	{
		if (empty())
			return false;

		//Gets node with largest key
		key = maxHelper(_root)->_key;
		return true;
	}

	// Largest key less than or equal to key
	virtual bool floor(const Key& key, Key& floorKey) const
	{
		if (empty())
			return false;

		//Checks if given key is less than the smallest key
		if (minHelper(_root)->_key > key)
			return false;

		//If key is greater than largest key return largest key
		if (key > maxHelper(_root)->_key)
		{
			floorKey = maxHelper(_root)->_key;
		}

		//Else find the key
		else
		floorKey = floorHelper(_root, key);
		return true;
	}

	// Smallest key greater than or equal to key
	virtual bool ceiling(const Key& key, Key& ceilingKey) const
	{
		if (empty())
			return false;

		//Checks if key is greater than highest key
		if (maxHelper(_root)->_key < key)
			return false;

		//If key is less than smallest key return smallest key
		if (minHelper(_root)->_key > key)
			ceilingKey = minHelper(_root)->_key;

		//Else find key
		else
		ceilingKey = ceilingHelper(_root, key);

		return true;
	}

	// Number of keys less than key
	virtual int rank(const Key& key) const
	{
		int count = 0;

		if (empty())
			return false;

		//Gets smallest node
		BinaryTreeNode* minNode = minHelper(_root);

		//Counts how many nodes from the minNode to the given key there are
		for (int i = minNode->_key; i < key; i++)
		{
			BinaryTreeNode* lowNode = getNode(_root, i);
			if (lowNode != nullptr)
				count++;
		}
		return count;
	}

	// key of rank k
	virtual bool select(int k = 0, Key& key = Key{}) const
	{
		if (empty() || k < 0)
			return false;

		//Returns key of rank k
		key = selectHelper(_root, k);
		return true;
	}

	// Delete the smallest key
	virtual bool deleteMin()
	{
		if (empty())
			return false;

		//Gets smallest node
		BinaryTreeNode* minNode = minHelper(_root);

		//Parent of smallest node
		BinaryTreeNode* parent = getParent(_root, minNode->_key);

		if (minNode == nullptr)
			return false;

		//Special root case
		if (minNode == _root && _root->_right != nullptr)
		{
			BinaryTreeNode* tempNode = _root->_right;
			Delete(_root);
			_root = tempNode;
			updateSize(_root);
			return true;
		}
		//Special root case
		else if (minNode == _root)
		{
			Delete(_root);
			_root = nullptr;
			return true;
		}
		//Case if minNode has no children
		else if (minNode->_right == nullptr)
		{
			Delete(minNode);
			minNode = nullptr;
			parent->_left = minNode;
		}

		//Case if minNode has a child
		else
		{
			parent->_left = nullptr;
			parent->_left = minNode->_right;
			Delete(minNode);
			minNode = nullptr;
		}

		//Update size after deletion
		updateSize(_root);

		return true;
	}

	// Delete the largest key
	virtual bool deleteMax()
	{
		if (empty())
			return false;

		//Gets highest key
		BinaryTreeNode* maxNode = maxHelper(_root);

		//Gets highest key's parent
		BinaryTreeNode* parent = getParent(_root, maxNode->_key);

		if (maxNode == nullptr)
			return false;

		//Special root case
		if (maxNode == _root && _root->_left != nullptr)
		{
			BinaryTreeNode* tempNode = _root->_left;
			Delete(_root);
			_root = tempNode;
			updateSize(_root);
			return true;
		}

		//Special root case
		else if (maxNode == _root)
		{
			Delete(_root);
			_root = nullptr;
			return true;
		}

		//Case if maxNode has no children
		else if (maxNode->_left == nullptr)
		{
			Delete(maxNode);
			maxNode = nullptr;
			parent->_right = maxNode;
		}

		//Case if maxNode has children
		else
		{
			parent->_right = nullptr;
			parent->_right = maxNode->_left;
			Delete(maxNode);
			maxNode = nullptr;
		}

		//Update size after deletion
		updateSize(_root);

		return true;
	}

	// number of keys in [low, high] (including low, high)
	virtual int size(const Key& low, const Key& high) const
	{
		int count = 0;

		if (empty())
			return 0;

		//Counts number of keys in table less than or equal to high starting at low
		for (int i = low; i <= high; i++)
		{
			BinaryTreeNode* lowNode = getNode(_root, i);
			if (lowNode != nullptr)
				count++;
		}

		return count;
	}

	// keys in [low, high] (including low, high), in sorted order
	virtual std::vector<Key> keys(const Key& low, const Key& high) const
	{
		//Returns vector of keys in sorted order
	return	keysHelper(_root, low, high);
	}

	// all keys in the table, in sorted order
	virtual std::vector<Key> keys() const
	{
		Key minimum, maximum;
		min(minimum);
		max(maximum);
		return keys(minimum, maximum);
	}

public:
	// Removes all elements from the table
	virtual void clear()
	{
		if (_root != nullptr) delete _root;
		_root = nullptr;
	}

private:
	// Returns the height of the BST (for debugging).
	// @return the height of the BST (a 1-node tree has height 0)
	int height() const { return height(root); }
	int height(BinaryTreeNode* node) const
	{
		if (node == nullptr) return -1;

		return 1 + std::max(height(node->_left), height(node->_right));
	}

	// Non-recursive node deletion.
	BinaryTreeNode* Delete(BinaryTreeNode* node) const
	{
		node->_left = nullptr;
		node->_right = nullptr;
		delete node;
		node = nullptr;
		return node;
	}

	//
	///////////////////////////////////////////////////////////////////////////////
	// Check integrity of BST data structure.
	///////////////////////////////////////////////////////////////////////////////
	//
	bool check() const
	{
		if (!isBST())            std::cout << "Not in symmetric order" << std::endl;
		if (!isSizeConsistent()) std::cout << "Subtree counts not consistent" << std::endl;
		if (!isRankConsistent()) std::cout << "Ranks not consistent" << std::endl;

		return isBST() && isSizeConsistent() && isRankConsistent();
	}

	//
	// does this binary tree satisfy symmetric order?
	// Note: this test also ensures that data structure is a binary tree since order is strict
	//
	bool isBST() const { return isBST(_root, nullptr, nullptr); }
	// is the tree rooted at x a BST with all keys strictly between min and max
	// (if min or max is null, treat as empty constraint)
	bool isBST(BinaryTreeNode* node, Key* min, Key* max) const
	{
		if (node == nullptr) return true;

		if (min != nullptr && keyLessThanOrEquals(node->_key, *min)) return false;

		if (max != nullptr && keyLessThanOrEquals(*max, node->_key)) return false;

		return isBST(node->_left, min, &(node->_key)) && isBST(node->_right, &(node->_key), max);
	}

	// are the size fields correct?
	bool isSizeConsistent() const { return isSizeConsistent(_root); }
	bool isSizeConsistent(BinaryTreeNode* node) const
	{
		if (node == nullptr) return true;

		// The size of this need must equate to the sum of its children (plus itself)
		if (node->_size != size(node->_left) + size(node->_right) + 1) return false;

		return isSizeConsistent(node->_left) && isSizeConsistent(node->_right);
	}

	// check that ranks are consistent
	bool isRankConsistent() const
	{
		// The i th node should be rank i
		for (int i = 0; i < size(); i++)
		{
			Key key;
			select(i, key);
			if (i != rank(key)) return false;
		}

		// All keys must equate to the key acquired at its rank 
		for (Key key : keys())
		{
			Key acquired;
			select(rank(key), acquired);

			if (!keyEquals(key, acquired)) return false;
		}

		return true;
	}
};

#endif