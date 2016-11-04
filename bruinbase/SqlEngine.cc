/**
 * Copyright (C) 2008 by The Regents of the University of California
 * Redistribution of this file is permitted under the terms of the GNU
 * Public License (GPL).
 *
 * @author Junghoo "John" Cho <cho AT cs.ucla.edu>
 * @date 3/24/2008
 */

#include <cstring>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include "Bruinbase.h"
#include "SqlEngine.h"
#include "BTreeIndex.h"

using namespace std;

// external functions and variables for load file and sql command parsing 
extern FILE* sqlin;
int sqlparse(void);


RC SqlEngine::run(FILE* commandline)
{
  fprintf(stdout, "Bruinbase> ");

  // set the command line input and start parsing user input
  sqlin = commandline;
  sqlparse();  // sqlparse() is defined in SqlParser.tab.c generated from
               // SqlParser.y by bison (bison is GNU equivalent of yacc)

  return 0;
}

RC SqlEngine::select(int attr, const string& table, const vector<SelCond>& cond)
{
  RecordFile rf;   // RecordFile containing the table
  RecordId   rid;  // record cursor for table scanning

  BTreeIndex tree; //Tree containing the table
  IndexCursor cursor; //record cursor for tree scanning

  RC     rc;
  int    key;     
  string value;
  int    count;
  int    diff;

  // open the table file
  if ((rc = rf.open(table + ".tbl", 'r')) < 0) {
    fprintf(stderr, "Error: table %s does not exist\n", table.c_str());
    return rc;
  }

  // scan the tree from the root
  rid.pid = rid.sid = 0;
  count = 0;

  int where_value = 0; //no operator set as of yet

  if(tree.open(table+".idx",'r')==0){ //if an index exists
    for(int i = 0; i<cond.size();i++){ //changed to for loop because while loop didn't iterate properly
      if(cond[i].attr == 1 && cond[i].comp == SelCond::EQ){
        where_value = atoi(cond[i].value);
        break;
      }

      else if(cond[i].attr == 1 && cond[i].comp == SelCond::GE){
        if (where_value == 0){
          where_value = atoi(cond[i].value);
        } //safety checks if there are two less than or equals to
        else if(where_value<atoi(cond[i].value)){
          where_value=atoi(cond[i].value);
        }
         
      }

      else if(cond[i].attr == 1 && cond[i].comp == SelCond::GT){
        if (where_value == 0){
          where_value = atoi(cond[i].value)+1;
        } //safety check just in case there are two greater thans
        else if(where_value<(atoi(cond[i].value)+1)){
          where_value=(atoi(cond[i].value)+1);
        }
      }
    }

    tree.locate(where_value,cursor);
   while (tree.readForward(cursor, key, rid)==0) {
      // read the tuple if not Count(*)
      if (attr !=4 && (rc = rf.read(rid, key, value)) < 0) {
        fprintf(stderr, "Error: while reading a tuple from table %s\n", table.c_str());
        goto exit_select;
      }

      // check the conditions on the tuple
      for (unsigned i = 0; i < cond.size(); i++) {
        // compute the difference between the tuple value and the condition value
        switch (cond[i].attr) {
        case 1:
           diff = key - atoi(cond[i].value);
           break;
        case 2:
           diff = strcmp(value.c_str(), cond[i].value);
           break;
        }

        // skip the tuple if any condition is not met
        switch (cond[i].comp) {
        case SelCond::EQ:
          //if multiple equal tos in statement, then stick with the first one
           if (diff != 0) {
              if(cond[i].attr == 1) {
                goto exit_select;
              }
            }
            
            if (cond[i].attr==2) {
              goto fakeout_select;
            }
          
           break;
            
        case SelCond::NE:
            goto fakeout_select;
          break;
          //already dealt with GT above
        case SelCond::GT:
           break;
        case SelCond::LT:
           if (diff >= 0) {
              if(cond[i].attr == 1) {
                goto exit_select;
              }
            }
           break;
        //already dealt with GE above
        case SelCond::GE:
           break;
        case SelCond::LE:
           if (diff > 0) {
              if(cond[i].attr == 1) {
                goto exit_select;
              }
            }
           break;
        }
      }

      // the condition is met for the tuple. 
      // increase matching tuple counter
      count++;

      // print the tuple 
      switch (attr) {
      case 1:  // SELECT key
        fprintf(stdout, "%d\n", key);
        break;
      case 2:  // SELECT value
        fprintf(stdout, "%s\n", value.c_str());
        break;
      case 3:  // SELECT *
        fprintf(stdout, "%d '%s'\n", key, value.c_str());
        break;
      }
    }
  }

  else{ //if no B+ Tree indexing
    fakeout_select:
    while (rid < rf.endRid()) {
      // read the tuple
      if ((rc = rf.read(rid, key, value)) < 0) {
        fprintf(stderr, "Error: while reading a tuple from table %s\n", table.c_str());
        goto exit_select;
      }

      // check the conditions on the tuple
      for (unsigned i = 0; i < cond.size(); i++) {
        // compute the difference between the tuple value and the condition value
        switch (cond[i].attr) {
        case 1:
           diff = key - atoi(cond[i].value);
           break;
        case 2:
           diff = strcmp(value.c_str(), cond[i].value);
           break;
        }

        // skip the tuple if any condition is not met
        switch (cond[i].comp) {
        case SelCond::EQ:
           if (diff != 0) goto next_tuple;
           break;
        case SelCond::NE:
          if (diff == 0) goto next_tuple;
          break;
        case SelCond::GT:
           if (diff <= 0) goto next_tuple;
           break;
        case SelCond::LT:
           if (diff >= 0) goto next_tuple;
           break;
        case SelCond::GE:
           if (diff < 0) goto next_tuple;
           break;
        case SelCond::LE:
           if (diff > 0) goto next_tuple;
           break;
        }
      }

      // the condition is met for the tuple. 
      // increase matching tuple counter
      count++;

      // print the tuple 
      switch (attr) {
      case 1:  // SELECT key
        fprintf(stdout, "%d\n", key);
        break;
      case 2:  // SELECT value
        fprintf(stdout, "%s\n", value.c_str());
        break;
      case 3:  // SELECT *
        fprintf(stdout, "%d '%s'\n", key, value.c_str());
        break;
      }

      // move to the next tuple
      next_tuple:
      ++rid;
    }
  }

  // print matching tuple count if "select count(*)"
  if (attr == 4) {
    fprintf(stdout, "%d\n", count);
  }
  rc = 0;

  // close the table file and return
  exit_select:
  rf.close();
  return rc;
}

RC SqlEngine::load(const string& table, const string& loadfile, bool index)
{
  //create a new RecordFile object
  RecordFile* rf = new RecordFile(table + ".tbl", 'w');

  //open and read in loadfile
  ifstream ifs;
  ifs.open(loadfile.c_str(), ifstream::in);

  int key;
  string value;
  RecordId rid;
  string line;

  BTreeIndex tree;
  //creating table idx
  if(index){
    tree.open(table + ".idx", 'w');
  }

  while(getline(ifs,line)){  
    //error check parsing
    if(parseLoadLine(line,key,value) != 0){ 
      cout << "Failed parsing at " << line << endl; 
      return -1;
    }
    if(rf->append(key,value,rid) != 0){
      //error check appending
      cout << "Failed appending at " << value << endl; 
      return -1;
    }
    else if(index){
      //inserting tuples from table into indexed B+tree
      tree.insert(key,rid);
    }
  }

  if(index){
    //closing tree
    tree.close();
  }

  ifs.close();
  return 0;
}

RC SqlEngine::parseLoadLine(const string& line, int& key, string& value)
{
    const char *s;
    char        c;
    string::size_type loc;
    
    // ignore beginning white spaces
    c = *(s = line.c_str());
    while (c == ' ' || c == '\t') { c = *++s; }

    // get the integer key value
    key = atoi(s);

    // look for comma
    s = strchr(s, ',');
    if (s == NULL) { return RC_INVALID_FILE_FORMAT; }

    // ignore white spaces
    do { c = *++s; } while (c == ' ' || c == '\t');
    
    // if there is nothing left, set the value to empty string
    if (c == 0) { 
        value.erase();
        return 0;
    }

    // is the value field delimited by ' or "?
    if (c == '\'' || c == '"') {
        s++;
    } else {
        c = '\n';
    }

    // get the value string
    value.assign(s);
    loc = value.find(c, 0);
    if (loc != string::npos) { value.erase(loc); }

    return 0;
}
