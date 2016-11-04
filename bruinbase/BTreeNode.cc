#include "BTreeNode.h"
#include <iostream>
#include <math.h>
#include <cstring>

using namespace std;

struct BTLeafNode::Leaf_Element{
	int key; //holds the key value in leaf
	RecordId rid; //Points to the tuple in RecordFile
};

//construct
BTLeafNode::BTLeafNode(){
	int i = 0;
	while (i < PageFile::PAGE_SIZE){
		buffer[i]='\0'; //Null
		i++;
	}
	//need to make enough space for node in memory
	int node_size = 0;
	int int_size = sizeof(int);//all keys will be integers
	memcpy(buffer,&node_size,int_size);
};

/*
 * Read the content of the node from the page pid in the PageFile pf.
 * @param pid[IN] the PageId to read
 * @param pf[IN] PageFile to read from
 * @return 0 if successful. Return an error code if there is an error.
 */
RC BTLeafNode::read(PageId pid, const PageFile& pf)
{ 
	return pf.read(pid,buffer); //read content of the node from page pid
}
    
/*
 * Write the content of the node to the page pid in the PageFile pf.
 * @param pid[IN] the PageId to write to
 * @param pf[IN] PageFile to write to
 * @return 0 if successful. Return an error code if there is an error.
 */
RC BTLeafNode::write(PageId pid, PageFile& pf)
{ 
	return pf.write(pid,buffer); 
}

/*
 * Return the number of keys stored in the node.
 * @return the number of keys in the node
 */
int BTLeafNode::getKeyCount()
{
	int node_size; //size of node
	int key_size = sizeof(int); //size of key
	memcpy(&node_size,buffer,key_size); //can't change directly in memory, have to get info and then change
	return node_size; //how many keys
}

/*
 * Insert a (key, rid) pair to the node.
 * @param key[IN] the key to insert
 * @param rid[IN] the RecordId to insert
 * @return 0 if successful. Return an error code if the node is full.
 */
RC BTLeafNode::insert(int key, const RecordId& rid)
{ 
	//error count
	//"if L has less than n-1 key values" from pseduo code
	int page_id_size = sizeof(PageId);
	int key_size = sizeof(int);
	int leaf_entry_size = sizeof(Leaf_Element);
	int maxNumKeys = floor((PageFile::PAGE_SIZE + key_size)/leaf_entry_size)-1; //from class table
	int numKeys = getKeyCount();
	if(numKeys>=maxNumKeys){
		cout << "BTLeafNode::insert: Too many keys. You currently have " << numKeys << " but the max key number allowed is " << maxNumKeys << endl;
		return -1; 
	}
	int eid; //declare the end id
	//find the leaf node L that should contain the key value
	locate(key,eid);
	int move_over_space = leaf_entry_size*(numKeys-eid); //making room in node
	char copy[move_over_space]; //data after eid (place we are trying to insert)
	memcpy(&copy, offset + buffer + leaf_entry_size*eid, move_over_space); //copying the data into copy
	memcpy(offset + buffer +leaf_entry_size*(eid+1),&copy,move_over_space); //copy this data over one space in memory
	
	//create the new object for the new entry
	Leaf_Element l;
	l.key = key;
	l.rid = rid;

	numKeys = numKeys+1;
	memcpy(offset + buffer + leaf_entry_size*eid, &l, leaf_entry_size); //move in the new leaf to the space just opened up
	memcpy(buffer,&numKeys,key_size); //increment the number of keys in tree
	return 0;
	
}

/*
 * Insert the (key, rid) pair to the node
 * and split the node half and half with sibling.
 * The first key of the sibling node is returned in siblingKey.
 * @param key[IN] the key to insert.
 * @param rid[IN] the RecordId to insert.
 * @param sibling[IN] the sibling node to split with. This node MUST be EMPTY when this function is called.
 * @param siblingKey[OUT] the first key in the sibling node after split.
 * @return 0 if successful. Return an error code if there is an error.
 */
RC BTLeafNode::insertAndSplit(int key, const RecordId& rid, 
                              BTLeafNode& sibling, int& siblingKey)
{ 
	int key_size = sizeof(int);
	int numKeys = getKeyCount();
	int i = floor(numKeys/2); //max amount of pointers in half of a key
	while(i<numKeys){
		//Create a node L'
		int new_key;
		RecordId new_rid;
		readEntry(i,new_key,new_rid); //read one entry in node at a time
		//Copy L.P1 ... L.Kn-1 to a block of memory T that can hold n (pointer, key-value) pairs
		sibling.insert(new_key, new_rid);
		i++;
	}
	int half = floor(numKeys/2);
	memcpy(buffer,&half,key_size); //update buffer of new size of node
	int eid; //use this to find if the key should be inserted into the current node or the new node
	int s_key;
	RecordId s_rid;
	sibling.readEntry(0,s_key, s_rid);
	if(key<s_key){ 
		insert(key,rid); 
	}
	else{
		sibling.insert(key,rid);
	}

	siblingKey = s_key;
	//make sure to check that the parent is properly pointing to the new split new and old split node
	return 0;
}

/**
 * If searchKey exists in the node, set eid to the index entry
 * with searchKey and return 0. If not, set eid to the index entry
 * immediately after the largest index key that is smaller than searchKey,
 * and return the error code RC_NO_SUCH_RECORD.
 * Remember that keys inside a B+tree node are always kept sorted.
 * @param searchKey[IN] the key to search for.
 * @param eid[OUT] the index entry number with searchKey or immediately
                   behind the largest key smaller than searchKey.
 * @return 0 if searchKey is found. Otherwise return an error code.
 */
RC BTLeafNode::locate(int searchKey, int& eid)
{ 
	int key;
	RecordId rid;
	int i = 0;
	while(i<getKeyCount()){ //go through all leaf keys in leaf node
		readEntry(i,key,rid); //read what keys are in
		if(key>=searchKey){ //have to search from the left->right to search on leafs
			//succesffuly found key!
			eid=i;
			return 0;
		}
		i++;
	}
	eid=getKeyCount(); //reset eid
	return -1; //not successful
}

/*
 * Read the (key, rid) pair from the eid entry.
 * @param eid[IN] the entry number to read the (key, rid) pair from
 * @param key[OUT] the key from the entry
 * @param rid[OUT] the RecordId from the entry
 * @return 0 if successful. Return an error code if there is an error.
 */
RC BTLeafNode::readEntry(int eid, int& key, RecordId& rid)
{
	//check for errors
	int numKeys = getKeyCount();
	if(eid < 0 || eid>=numKeys){
		cout << "BTLeafNode::readEntry: eid is out of range." << endl;
		return -1;
	}

	Leaf_Element l;

	int key_size = sizeof(int);
	int leaf_size = sizeof(l);
	memcpy(&l, offset + buffer + eid*leaf_size, leaf_size); //reading in one entry at eid position
	key = l.key;
	rid=l.rid;
	return 0;

}

/*
 * Return the pid of the next slibling node.
 * @return the PageId of the next sibling node 
 */
PageId BTLeafNode::getNextNodePtr()
{ 
	PageId pid; //return the pid of the next sibling
	int page_id_size = sizeof(PageId); //determines how big the page id is
	//getting the next node in memory so change the pid pointer of parent
	memcpy(&pid, PageFile::PAGE_SIZE-page_id_size+buffer, page_id_size);
	return pid;
}

/*
 * Set the pid of the next slibling node.
 * @param pid[IN] the PageId of the next sibling node 
 * @return 0 if successful. Return an error code if there is an error.
 */
RC BTLeafNode::setNextNodePtr(PageId pid)
{ 
	int page_id_size = sizeof(PageId); //determines how big the page id is
	//set pointer to next node
	memcpy(PageFile::PAGE_SIZE-page_id_size+buffer,&pid,page_id_size);
	return 0;
}



BTNonLeafNode::BTNonLeafNode(){
	int i = 0;
	//create a buffer stream
	while(i<PageFile::PAGE_SIZE){
		buffer[i] = '\0'; //this is filling the buffer with null bytes
		i++;
	}

	//need to make enough space for node in memory
	int node_size = 0;
	int int_size = sizeof(int);//all keys will be integers
	memcpy(buffer,&node_size,int_size);

}
/*
 * Read the content of the node from the page pid in the PageFile pf.
 * @param pid[IN] the PageId to read
 * @param pf[IN] PageFile to read from
 * @return 0 if successful. Return an error code if there is an error.
 */
RC BTNonLeafNode::read(PageId pid, const PageFile& pf)
{ 
	return pf.read(pid,buffer); 
}
    
/*
 * Write the content of the node to the page pid in the PageFile pf.
 * @param pid[IN] the PageId to write to
 * @param pf[IN] PageFile to write to
 * @return 0 if successful. Return an error code if there is an error.
 */
RC BTNonLeafNode::write(PageId pid, PageFile& pf)
{ 
	return pf.write(pid,buffer); 
}

/*
 * Return the number of keys stored in the node.
 * @return the number of keys in the node
 */
int BTNonLeafNode::getKeyCount()
{ 
	int node_size; //size of node
	int key_size = sizeof(int); //size of key
	memcpy(&node_size,buffer,key_size); //this info is stored in the beginning of buffer stream
	return node_size; //how many keys in  node
}


/*
 * Insert a (key, pid) pair to the node.
 * @param key[IN] the key to insert
 * @param pid[IN] the PageId to insert
 * @return 0 if successful. Return an error code if the node is full.
 */
RC BTNonLeafNode::insert(int key, PageId pid)
{ 
	//error count
	//"if L has less than n-1 key values" from pseduo code
	int page_id_size = sizeof(PageId);
	int key_size = sizeof(int);
	int maxNumKeys = floor((PageFile::PAGE_SIZE +key_size)/(key_size+page_id_size))-1; //from class table
	int numKeys = getKeyCount();
	if(numKeys>=maxNumKeys){
		cout << "BTNonLeafNode::insert: Too many keys. You currently have " << getKeyCount() << " but the max key number allowed is " << maxNumKeys << endl;
		return -1; 
	}

	int eid; //declare the end id
	//find the nonleaf node L that should contain the key value
	locate(key,eid); 
	int move_over_space = (key_size+page_id_size)*(numKeys-eid); //making room in node
	char copy[move_over_space]; //data after eid (place we are trying to insert)
	memcpy(&copy, offset + buffer + page_id_size+eid*(page_id_size+key_size), move_over_space); //copying the data into copy
	memcpy(offset + buffer + page_id_size+(eid+1)*(page_id_size+key_size),&copy,move_over_space); //copy this data over one space in memory
	
	memcpy(offset + buffer + page_id_size+eid*(page_id_size+key_size), &key, key_size); //move in the new leaf to the space just opened up
	memcpy(offset + buffer + key_size+page_id_size+eid*(page_id_size+key_size),&pid, page_id_size);

	numKeys = numKeys+1;
	memcpy(buffer,&numKeys,key_size); //increment the number of keys in tree

	return 0;
}

/*
 * Insert the (key, pid) pair to the node
 * and split the node half and half with sibling.
 * The middle key after the split is returned in midKey.
 * @param key[IN] the key to insert
 * @param pid[IN] the PageId to insert
 * @param sibling[IN] the sibling node to split with. This node MUST be empty when this function is called.
 * @param midKey[OUT] the key in the middle after the split. This key should be inserted to the parent node.
 * @return 0 if successful. Return an error code if there is an error.
 */
RC BTNonLeafNode::insertAndSplit(int key, PageId pid, BTNonLeafNode& sibling, int& midKey)
{
	int key_size = sizeof(int);
	int page_id_size = sizeof(PageId);
	int numKeys = getKeyCount();
	int i = floor(numKeys/2); //max amount of pointers in half of a key
	int half = floor(numKeys/2);

	while(i<numKeys){
		//Create a node L'
		int new_key;
		PageId new_pid;
		memcpy(&new_key, offset + buffer+page_id_size+i*(key_size+page_id_size),key_size);
		memcpy(&new_pid, offset + key_size+buffer+page_id_size+i*(key_size+page_id_size),page_id_size);
		if (i==half){ //create a new root with the middle key
			midKey = new_key;
			memcpy(offset + sibling.buffer, &new_pid, page_id_size);
		}
		else{ //moving all other keys and pids over to new nonleaf
			sibling.insert(new_key, new_pid); 
		}
		i++;
	}
	int eid;
	memcpy(buffer,&half,key_size); //updating the number of nodes in current non leaf

	if(key<midKey){
		insert(key,pid);
	}
	else{
		sibling.insert(key,pid);
	}

	return 0; 
}


/* Locate key in nonleaf node*/
RC BTNonLeafNode::locate(int searchKey, int& eid)
{
	int key;
	int key_size = sizeof(int);
	int page_id_size = sizeof(PageId);
	int numKeys = getKeyCount();
	for(int i= 0; i<numKeys; i++){ //go through all elements in nonleaf
		memcpy(&key, offset + buffer + page_id_size+i*(key_size+page_id_size), key_size);
		if(key>=searchKey){ //have to search from the left->right to search on leafs
			//succesffuly found key!
			eid=i;
			return 0;
		}
	}
	eid=numKeys; //reset eid
	return -1; //not successful	
}

/*
 * Given the searchKey, find the child-node pointer to follow and
 * output it in pid.
 * @param searchKey[IN] the searchKey that is being looked up.
 * @param pid[OUT] the pointer to the child node to follow.
 * @return 0 if successful. Return an error code if there is an error.
 */
RC BTNonLeafNode::locateChildPtr(int searchKey, PageId& pid)
{ 
	PageId l_page_id; //left pointer
	PageId r_page_id; //right pointer
	int temp_key; 
	int key_size = sizeof(int);
	int page_id_size = sizeof(PageId);

	int i = 0;
	int numKeys = getKeyCount();
	while (i<numKeys){
		memcpy(&l_page_id, offset + buffer + (page_id_size+key_size)*i,page_id_size); //locating the left pointer
		memcpy(&temp_key, offset + buffer + page_id_size+(page_id_size+key_size)*i,key_size); //locating the key
		memcpy(&r_page_id, offset + buffer + page_id_size+key_size+(page_id_size+key_size)*i,page_id_size); //locating the right pointer
		if(searchKey>=temp_key && i==numKeys-1){
			pid = r_page_id;
			return 0;
		}
		else if(searchKey<temp_key){ 
			pid = l_page_id;
			return 0;
		}
		i++;
	}
	cout << "There was an error in finding which child pointer to traverse" << endl;
	return -1; //fail!
}

/*
 * Initialize the root node with (pid1, key, pid2).
 * @param pid1[IN] the first PageId to insert
 * @param key[IN] the key that should be inserted between the two PageIds
 * @param pid2[IN] the PageId to insert behind the key
 * @return 0 if successful. Return an error code if there is an error.
 */
RC BTNonLeafNode::initializeRoot(PageId pid1, int key, PageId pid2)
{
	int key_size = sizeof(int);
	int page_id_size = sizeof(PageId);
	//start at offset+buffer and add offset+buffer to every memcpy above to start in the correct place
	memcpy(offset + buffer,&pid1, page_id_size); //point to left of key
	memcpy(offset + buffer + page_id_size,&key,key_size); //insert in key
	memcpy(offset + buffer + key_size+page_id_size, &pid2, page_id_size); //pointer to right of key

	int root = 1;
	memcpy(buffer, &root, key_size); //make room for root in memory
	return 0;
}
