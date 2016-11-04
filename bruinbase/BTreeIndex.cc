/*
 * Copyright (C) 2008 by The Regents of the University of California
 * Redistribution of this file is permitted under the terms of the GNU
 * Public License (GPL).
 *
 * @author Junghoo "John" Cho <cho AT cs.ucla.edu>
 * @date 3/24/2008
 */
 
#include "BTreeIndex.h"
#include "BTreeNode.h"
#include <cstring>
#include <iostream>
#include <math.h>
using namespace std;


/*
 * BTreeIndex constructor
 */
BTreeIndex::BTreeIndex()
{
    rootPid = -1;
    treeHeight = 0;
    prev = -1;
    memNode = BTLeafNode();

}

/*
 * Open the index file in read or write mode.
 * Under 'w' mode, the index file should be created if it does not exist.
 * @param indexname[IN] the name of the index file
 * @param mode[IN] 'r' for read, 'w' for write
 * @return error code. 0 if no error
 */
RC BTreeIndex::open(const string& indexname, char mode)
{
	if(pf.open(indexname, mode) < 0){
		cout << "There is an error in BTreeIndex::open: Cannot open file" << endl;
		return -1;
	}
	if(pf.endPid() == 0) {
		rootPid = -1; //empty B+ tree
		treeHeight = 0;
		close();
		return open(indexname,mode);
	}
	else{ //there is something in the tree
		if(pf.read(0,buffer) < 0){
			cout << "Error in BTreeIndex::open: There is nothing to read." << endl;
			return -1;
		}
		int page_id_size = sizeof(PageId);
		int key_size = sizeof(int);
		memcpy(&rootPid,buffer,page_id_size);
		memcpy(&treeHeight, page_id_size+buffer, key_size);
	}
    return 0;
}

/*
 * Close the index file.
 * @return error code. 0 if no error
 */
RC BTreeIndex::close()
{
	int page_id_size = sizeof(PageId);
	int key_size = sizeof(int);
	memcpy(buffer,&rootPid,page_id_size);
	memcpy(page_id_size+buffer,&treeHeight,key_size);
	if (pf.write(0,buffer)<0){
		cout << "Error in BTreeIndex::close: Cannot write" << endl;
		return -1;
	}
    return pf.close();
}

/*
 * Insert (key, RecordId) pair to the index.
 * @param key[IN] the key for the value inserted into the index
 * @param rid[IN] the RecordId for the record being inserted into the index
 * @return error code. 0 if no error
 */
RC BTreeIndex::insert(int key, const RecordId& rid)
{
	//if not even at root
	if(treeHeight == 0){
		rootPid = pf.endPid(); //if not root then create a root.

		BTLeafNode leaf = BTLeafNode();
		leaf.insert(key,rid);

		leaf.write(rootPid,pf);

		treeHeight = treeHeight+1;
		return 0;

	}
	//Case: The tree has levels to traverse
    else{
    	PageId new_pid;
    	int new_key;
    	if(helper(key,rid,new_key, new_pid,rootPid,1)==0){ //not an overflow case
    		return 0;
    	}
    	else{
    		BTNonLeafNode parent = BTNonLeafNode();
    		parent.initializeRoot(rootPid,new_key,new_pid);
    		rootPid = pf.endPid();
    		parent.write(rootPid,pf);
    		treeHeight = treeHeight+1;
    		return 0;
    	}

     }
}

RC BTreeIndex::helper(int& key, const RecordId& rid, int& new_key, PageId& new_pid, PageId curr_pid, int height)
{
	//This should deal with all possible insert scenarios we come up against. Overflow or room to insert.
	//if leaf then finished
	if(height == treeHeight){
		BTLeafNode leaf = BTLeafNode();
		int page_id_size = sizeof(PageId);
		int key_size = sizeof(int);
		int record_id_size = sizeof(RecordId);
		int maxNumKeys = floor((PageFile::PAGE_SIZE +key_size)/(record_id_size+key_size))-1; 
		leaf.read(curr_pid,pf); 
		int kk;
		RecordId rr;
		leaf.readEntry(0,kk,rr);

		if(maxNumKeys>leaf.getKeyCount()){ //if there is room in the node
			leaf.insert(key,rid);
			leaf.write(curr_pid,pf); 
			return 0; //not over flow
		}
		else{ //if there is no room then we have to split the leaf
			BTLeafNode new_leaf = BTLeafNode();
			leaf.insertAndSplit(key,rid,new_leaf,new_key);
			new_pid = pf.endPid(); //setting new pointer for new node
			new_leaf.setNextNodePtr(leaf.getNextNodePtr());
			leaf.setNextNodePtr(new_pid); //set pointer of leaf to new sibling leaf
			new_leaf.write(new_pid,pf);
			leaf.write(curr_pid,pf);
			return 1; //overflow
		}

	}
	//If non leaf
	else{
		BTNonLeafNode nonleaf = BTNonLeafNode();
		PageId child_pid;
		int page_id_size = sizeof(PageId);
		int key_size = sizeof(int);
		int maxNumKeys = floor((PageFile::PAGE_SIZE +key_size)/(key_size+page_id_size))-1;
		nonleaf.read(curr_pid, pf);
		nonleaf.locateChildPtr(key,child_pid); //finds which child pid to go down
		if(helper(key,rid,new_key,new_pid,child_pid,height+1)>=1){
			//we have overflow people, let's deal with it
			//Case 1: We have room
			if(maxNumKeys>nonleaf.getKeyCount()){
				nonleaf.insert(new_key,new_pid);
				nonleaf.write(curr_pid,pf);
				new_key='\0'; 
				return 0; //not overflow
			}
			//Case 2: we do not have room but have another overflow problem
			else{
				BTNonLeafNode new_nonleaf = BTNonLeafNode();
				int midKey;
				nonleaf.insertAndSplit(new_key,new_pid,new_nonleaf,midKey);
				new_pid=pf.endPid();
				new_key = midKey;
				new_nonleaf.write(new_pid,pf);
				nonleaf.write(curr_pid,pf);
				return 1; //overflow

			}
		}
		else{
			return 0;
		}
	}
}


/**
 * Run the standard B+Tree key search algorithm and identify the
 * leaf node where searchKey may exist. If an index entry with
 * searchKey exists in the leaf node, set IndexCursor to its location
 * (i.e., IndexCursor.pid = PageId of the leaf node, and
 * IndexCursor.eid = the searchKey index entry number.) and return 0.
 * If not, set IndexCursor.pid = PageId of the leaf node and
 * IndexCursor.eid = the index entry immediately after the largest
 * index key that is smaller than searchKey, and return the error
 * code RC_NO_SUCH_RECORD.
 * Using the returned "IndexCursor", you will have to call readForward()
 * to retrieve the actual (key, rid) pair from the index.
 * @param key[IN] the key to find
 * @param cursor[OUT] the cursor pointing to the index entry with
 *                    searchKey or immediately behind the largest key
 *                    smaller than searchKey.
 * @return 0 if searchKey is found. Othewise an error code
 */
RC BTreeIndex::locate(int searchKey, IndexCursor& cursor)
{	
	PageId pid = rootPid; //starting the traversing at the root
	int eid = -1; //have not found the location of the search key yet

	BTLeafNode leaf = BTLeafNode(); //create object leaf
	BTNonLeafNode nonleaf = BTNonLeafNode(); //create object non leaf

	if(treeHeight >= 2){
		int i = 0;
		while (i<treeHeight-1){
			if(nonleaf.read(pid,pf)<0){
				cout << "Error in BTreeIndex::locate: Cannot read pid in non leaf node." << endl;
				return -1;
			}
			if(nonleaf.locateChildPtr(searchKey,pid)<0){
				cout << "Error in BTreeIndex:: locate: Cannot locate child pointer for non leaf node." << endl;
				return -1;
			}
			i++;
		}
	}
	if(leaf.read(pid,pf)<0){
		cout << "Error in BTreeIndex::locate: Cannot read leaf node." << endl;
		return -1;
	}
	if(leaf.locate(searchKey,eid)<0){
		cout << "Error in BTreeIndex::locate: Cannot find key in leaf." << endl;
		return -1;
	}
	else{
		//move the curser down
		cursor.eid = eid;
		cursor.pid = pid;

	}
    return 0;
}

/*
 * Read the (key, rid) pair at the location specified by the index cursor,
 * and move foward the cursor to the next entry.
 * @param cursor[IN/OUT] the cursor pointing to an leaf-node index entry in the b+tree
 * @param key[OUT] the key stored at the index cursor location.
 * @param rid[OUT] the RecordId stored at the index cursor location.
 * @return error code. 0 if no error
 */
RC BTreeIndex::readForward(IndexCursor& cursor, int& key, RecordId& rid)
{
	if(prev !=cursor.pid){
		if(memNode.read(cursor.pid,pf)<0){
			cout << "Error in BTreeIndex::readForward: Cannot read forward cursor" << endl;
			return -1;
		}
		prev = cursor.pid;
	}

	if(cursor.eid == memNode.getKeyCount()){
		int page_id_size = sizeof(PageId);
		char testBuffer[page_id_size];
		int i =0;
		while (i<page_id_size){
			testBuffer[i] = '\0';
			i++;
		}

		if(memcpy(testBuffer,&cursor.pid, page_id_size) == 0){
			//then the page has not been set
			cout << "Error in BTreeIndex::readForward: Page not set." << endl;
			return -1;
		}
		cursor.eid = 0;
		cursor.pid = memNode.getNextNodePtr(); //end of node in leaf so moving onto next sibling leaf
		if(cursor.pid ==0){
			return -1;
		}
		if(memNode.read(cursor.pid,pf) < 0){
			cout << "Error in BTreeIndex::readForward: Cannot read cursor pid." << endl;
			return -1;
		}
		prev = cursor.pid; //remember where I just left off
	}
	if (memNode.readEntry(cursor.eid,key,rid)<0){
		cout << "Error in BTreeIndex::readForward: Cannot read cursor eid." << endl;
		return -1;
	}
	else{ //increment eid to be the next pid
		cursor.eid++; 
	}

    return 0;
}
