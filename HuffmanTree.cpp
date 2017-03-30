//
//  HuffmanTree.cpp
//  huffman
//
//  Created by Tribelhorn, Ben on 10/22/13.
//  Copyright (c) 2013 Tribelhorn, Ben. All rights reserved.
//

#include "HuffmanTree.h"

bool HuffmanTree::createCodeTree(string filename)
{
  std::priority_queue<BST<CharFreq>::BSTNode*,
	std::vector<BST<CharFreq>::BSTNode*>,
	BST<CharFreq>::FrequencyCompare > myPriorityQ;
  
  string str,
	allText = "",
	charCode = "",
	temp = "";
  
  ifstream infile;
  queue<BST<CharFreq>::BSTNode*> encodeQ;
  std::vector<BST<CharFreq>::BSTNode*> arr;
  BST<CharFreq>::BSTNode* ptr;
  
  int count = 0;

  infile.open(filename.c_str());
  if( !infile.is_open() ){
	cout << "ERROR Loading file!\n";
	return false;
  }

  while(!infile.eof()){ // To get you all the lines.
	getline(infile,str); // Saves the line in STRING.
	allText += str + "\n";
  }
  infile.close();
  // **************************************************************
  // TODO: Fill in here
  // 1. Count occurences of every letter or sybmol that you care about
  //    in allText (consider spaces ' ', newlines '\n', periods, commas,
  //    question marks, quotation marks, etc.
  
  for(unsigned int i = 0; i < allText.length(); i++){
	if(count == 0){
	  BST<CharFreq>::BSTNode* temptr = new BST<CharFreq>::BSTNode();
	  temptr->data.letter = allText[i];
	  temptr->data.frequency = 1;
	  arr.push_back(temptr);
	  count++;
	  
	}else{
	  int index = 0;
	  while(index < count && allText[i] != arr[index]->data.letter){
		index++;
	  }
	  
	  if(index < count){
		arr[index]->data.frequency++;
		
	  }else{
		BST<CharFreq>::BSTNode* temptr = new BST<CharFreq>::BSTNode();
		temptr->data.letter = allText[i];
		temptr->data.frequency = 1;
		arr.push_back(temptr);
		count++;
	  }
	}
  }
    
  // 2. Use myPriorityQ to build a tree, as described in the assignment  
  for(int i = 0; i < count; i++){
	myPriorityQ.push(arr[i]);
  }
  
  // 3. Set root (inherted from BST) to the final item in myPriorityQ
  root = nullptr;
  while(!myPriorityQ.empty()){
	if(myPriorityQ.size() == 1){
	  root = myPriorityQ.top();
	  myPriorityQ.pop();
	
	}else{
	  BST<CharFreq>::BSTNode* temptr = new BST<CharFreq>::BSTNode();
	  temptr->left = myPriorityQ.top();
	  myPriorityQ.pop();
	  temptr->right = myPriorityQ.top();
	  myPriorityQ.pop();
	  
	  temptr->data.frequency
	  = temptr->left->data.frequency + temptr->right->data.frequency;
	  temptr->data.encoding = "";
	  myPriorityQ.push(temptr);
	}
  }
  
  //Traverse the tree to make the table of frequency
  arr.resize(256);
  encodeQ.push(root);
  while(!encodeQ.empty()){
	ptr = encodeQ.front();
	encodeQ.pop();
	if(ptr->left != nullptr){
	  ptr->left->data.encoding = ptr->data.encoding + "0";
	  ptr->right->data.encoding = ptr->data.encoding + "1";
	  encodeQ.push(ptr->left);
	  encodeQ.push(ptr->right);
	}else
	  arr[(int)ptr->data.letter] = ptr;
  }
  freqArr = arr;
  // **************************************************************  
  
  return true;
}

bool HuffmanTree::encodeFile(string originalFilename, string outputFilename)
{
	string str;
	string allText = "";
	ifstream infile;
	infile.open(originalFilename.c_str());
	if( !infile.is_open() )
	{
		cout << "ERROR Loading file!\n";
		return false;
	}
	while(!infile.eof()) // To get you all the lines.
	{
		getline(infile,str); // Saves the line in STRING.
		allText += str + "\n";
	}
	infile.close();
	
	std::ofstream outStream(outputFilename.c_str(), std::ios::binary);
	
	if( !outStream.is_open() )
	{
		cout << "ERROR creating output file!\n";
		return false;
	}
	
	string currentStream="";
	
	const int WRITE_SIZE = 8;
	cout << freqArr.size() << endl;
	for(int i=0;i<(int)allText.length();i++)
	{
	  // **************************************************************
	  // TODO: add the appropriate encoding for allText[i] to currentStream
	  //cout << (int)allText[i] << endl;
	  if((int)allText[i] >= 0 && (int)allText[i] <= 255)
		currentStream += freqArr[(int)allText[i]]->data.encoding;
	  // **************************************************************
	  
	  while((int)currentStream.length() > WRITE_SIZE)
		{
		    string byte = currentStream.substr(0, WRITE_SIZE);
			bitset<8> set(byte);
			const char toWrite = (unsigned char)((unsigned int)set.to_ulong());
			outStream.write(&toWrite,sizeof(char));
			currentStream = currentStream.substr(WRITE_SIZE, currentStream.length()-WRITE_SIZE);
		}
	}
	
	outStream.close();
	return false;
}

bool HuffmanTree::decodeFile(string filename)
{
	ifstream encodedfile;
	encodedfile.open(filename.c_str(), ios::binary);
	if( !encodedfile.is_open() )
	{
		cout << "ERROR opening encoded file!\n";
		return false;
	}
	
	encodedfile >> std::noskipws;	// OMFG This is ridiculous
	
	string bitStream = "";
	while(!encodedfile.eof())
	{
		unsigned char readMe;
		encodedfile >> readMe;
		bitset<8> set((unsigned long)readMe);
		bitStream += set.to_string();
	}
	
	encodedfile.close();
	
	int i=0;
	while(i != -1)
	{
	  i = traverseAndPrint(bitStream, i, root);
	}
	return false;
}


int HuffmanTree::traverseAndPrint(string &bits, int i, BSTNode *cur)
{
  if(i >= (int)bits.length())
	return -1;
  // **************************************************************
  //TODO: Write this function using recursion.
  //   This is essentially your decode function.
  //You need to step through the tree based on reading 0 or 1
  //   and when you reach a leaf, print (using cout) the appropriate character.
  //   i represents your current location in the string
  //cur represents the cur node in your tree
  //Don't forget that you need to keep going after printing
  //   out a character (which means restarting at the top of the tree)
  if(cur->left == nullptr){
	cout << cur->data.letter;
	return i;
  }else{
	if(bits[i] == '0')
	  return traverseAndPrint(bits, i+1, cur->left);
	else
	  return traverseAndPrint(bits, i+1, cur->right);
  }
  // **************************************************************
}

void HuffmanTree::displayCode(ostream &out)  
{
  // **************************************************************
  //TODO: print out each letter and its code,
  //   you might want to check for special cases (space, newline, etc.)		  
  BST<CharFreq>::BSTNode* ptr;
  queue<BST<CharFreq>::BSTNode*> printQ;
  printQ.push(root);
  while(!printQ.empty()){
	ptr = printQ.front();
	printQ.pop();
	if(ptr->left != nullptr){
	  printQ.push(ptr->left);
	  printQ.push(ptr->right);
	}else{
	  if(ptr->data.letter == '\n')
		out << "\\n: " << ptr->data.encoding;
	  else if(ptr->data.letter == ' ')
		out << "SPACE: " << ptr->data.encoding;
	  else
		out << ptr->data.letter << ": " << ptr->data.encoding;
	}
  }
  // **************************************************************
}

ostream& operator<<(ostream &out, HuffmanTree &code)
{
	code.displayCode(out);
	return out;
}
