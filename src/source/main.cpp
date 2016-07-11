#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstring>
#include <ctime>
#include <cwchar>
#include <cstdio>
#include <time.h>
#include <algorithm>
#include "function.h"
#include "class.h"
using namespace std;
int main()
{

cout << "Program Running --- " << endl;
ifstream infile("../venmo_input/venmo-trans.txt");

string preLine;
getline(infile, preLine);

//-----get pretime-------------------------------------------
std::size_t pos01 = preLine.find("\"created_time\": \"");
string str01 = preLine.substr(pos01+17,20);
time_t preTime =timeConvert(str01);
time_t timeLimit = preTime - 60;

//-------get pre name 1----------------------------------
std::size_t pos02 = preLine.find("\"target\": \"");
std::size_t pos03 = preLine.find("\", \"actor\":");
string str02 = preLine.substr(pos02+11,pos03-pos02-11);

//--------------get pre name 2----------------------------
std::size_t pos04 = preLine.find("\"actor\":");
std::size_t pos05 = preLine.find("\"}");
string str03 = preLine.substr(pos04+10,pos05-pos04-10);

//-----------create 2 flags --------------------------------
bool passTimecheck = false;
bool passNamecheck = false;

//-----------create edge-time tree----------------------------------------

RBTree<time_t, string>* edge_time = new RBTree<time_t, string>();
string tempName1;
string tempName2;
if ( str02 < str03 )
{ tempName1 = str02;
  tempName2 = str03; }
else
{ tempName1 = str03;
  tempName2 = str02; }


edge_time->insert(preTime, tempName1, tempName2);
//------------create degree tree -----------------------------------------

RBTree<string, int>* degree = new RBTree<string, int>();
degree->insert(str02,1,0);
degree->insert(str03,1,0);

//------------create name-tag ree-------------------------------------
string tag0;
if ( str02 < str03 )
{ tag0 = str02+str03; }
else
{ tag0 = str03+str02; }

RBTree<string,int>* edge_tag = new RBTree<string, int>();
edge_tag->insert(tag0,0,0);

  ofstream myfile;
  myfile.open ("../venmo_output/output.txt");
  myfile <<  fixed << setprecision(2) << 1.0 << endl;

//-------------loop start------------------------------------------------

string line;
while (getline(infile, line))
{
	
std::size_t pos1 = line.find("\"created_time\": \"");
string str1 = line.substr(pos1+17,20);
time_t newtime = timeConvert(str1);

//------------------time check------------------------------
 if (newtime < timeLimit)
  { passTimecheck = false;}
 else
  { passTimecheck = true ;}
//------------------passed first check  --------------------------------

	if (passTimecheck == true)
      {
	//----get first name-----	
	std::size_t pos2 = line.find("\"target\": \"");
	std::size_t pos3 = line.find("\", \"actor\":");
	string str2 = line.substr(pos2+11,pos3-pos2-11);

	//----get second name-----
	std::size_t pos4 = line.find("\"actor\":");
	std::size_t pos5 = line.find("\"}");
	string str3 = line.substr(pos4+10,pos5-pos4-10);
	//----sort name pair------

	if ( str2 < str3 )
	{ tempName1 = str2;
	  tempName2 = str3; }
	else
	{ tempName1 = str3;
  	  tempName2 = str2; }

//------------insert time tree-----------------------
          edge_time->insert(newtime, tempName1, tempName2);



	//-----create tag ---------
	string tag;
	if ( str2 < str3 )
	{ tag = str2+str3; }
	else
	{ tag = str3+str2; }

//------------------second check-----------------------------------
	if (edge_tag->search(tag)==NULL)
	{ passNamecheck = true; } 
	else
	{ passNamecheck = false;}


	if (passNamecheck = true)
	{
//------------insert tag tree------------------------
	  edge_tag->insert(tag,0,0);
//------------insert degree tree---------------------
	  if (degree->search(tempName1) == NULL)
		{
		  degree->insert(tempName1,1,0);
		}
	  else
		{
		  (degree->search(tempName1))->non_key = (degree->search(tempName1))->non_key +1;
		}
	  if (degree->search(tempName2) == NULL)
                {
                  degree->insert(tempName2,1,0);
                }
          else
                {
                  (degree->search(tempName2))->non_key = (degree->search(tempName2))->non_key +1;
                }
	}//seconde check pass space

//-------------------------update time limit----------------------

       timeLimit = (edge_time->maximum() - 60) ;


      }//first check pass space

//------------------------Eviction Start!!!--------------------------------------------------------------------------------------------------

string eviName1;
string eviName2;
string eviTag;
//-----------------------time evict loop--------------------------
   	 while (edge_time->minimumNode(edge_time->mRoot) -> key < timeLimit)
	{
  	  eviName1 = edge_time->minimumNode(edge_time->mRoot) -> non_key;
   	  eviName2 = edge_time->minimumNode(edge_time->mRoot) -> non_key2;
	  eviTag   = eviName1+eviName2;
	//-------------name check again-----------------------------------
          	if (edge_tag->search(eviTag) != NULL) 
	     {
		   edge_tag->remove(eviTag);
		
	//------------degree tree eviction--------------------------------
		if (degree->search(eviName1) != NULL)
		{
		   degree->search(eviName1)->non_key = degree->search(eviName1)->non_key - 1;
			         if (degree->search(eviName1)->non_key <= 0)
			             {
                   			degree->remove(eviName1);
                	             }

		}	
                
		if (degree->search(eviName2) != NULL)
                {
                   degree->search(eviName2)->non_key = degree->search(eviName2)->non_key - 1;
		                 if (degree->search(eviName2)->non_key <= 0)
			            {
                   			degree->remove(eviName2);
                 		    }

                }

	     }//TAG check loop
	//--------- time tree eviction-----------------------------------
	edge_time->removeNode( edge_time->mRoot,edge_time->minimumNode(edge_time->mRoot));

	}// eviction loop	

//------------------------------------------data collectiong from degree-------------------------------------------
 
if ( passTimecheck == true )
{
	int size = degree->size(degree->mRoot);
	int *array = new int[size];

 	array[0] = degree->minimumNode(degree->mRoot) -> non_key;

	RBTNode<string, int> *tempNode ;
	tempNode = degree->minimumNode(degree->mRoot);

		for (int i = 1; i< size ; i++)
	   {   
        	array[i] = degree->successor(tempNode) -> non_key;
 	 	tempNode = degree->successor(tempNode);        
           }

//ood
	if ( size % 2 == 1 )
    {  
	nth_element(array, array + size / 2, array + size);
	float median = array[size/2];
	myfile << fixed << setprecision(2) << median <<endl;
    }  
//even
	if ( size % 2 == 0 )
    {

	nth_element(array, array + size / 2, array + size);
	float median1= array[size/2];
	nth_element(array, array -1 + size/2, array + size);
	float median2 = array[(size-1)/2];

	float median  = (median1+median2)/2 ;  
	myfile <<  fixed << setprecision(2) << median <<endl;
    }

    delete[] array;
  }//passtime
}//read line by line loop

edge_tag->destroy();
edge_time->destroy();
degree->destroy();
myfile.close();
infile.close();

	return 0;
}
