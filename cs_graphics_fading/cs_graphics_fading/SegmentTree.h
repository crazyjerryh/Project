#pragma once
#include "color.h"
#include <QPainter>
#include <QPen>
#include <fstream>
//for debug 

const int ZMINVALUE=-100;

using namespace RenderAlgorithm;
typedef struct segmentTree_tag{  /*segment tree*/
	int l,r;
	float zValue;
	struct segmentTree_tag *leftchild,*rightchild;
	color clr;
	bool flag;
	struct segmentTree_tag(int _l,int _r,float _zValue,color _clr):
		l(_l),r(_r),zValue(_zValue),clr(_clr),leftchild(0),rightchild(0),flag(false){}
}segmentTree;

void create(segmentTree *&root,int l,int r)
{
	//create a segment tree
	if(l==r){
		  root=new segmentTree(l,l,-100,color(255,255,255));
		  return;
	}
	if(root==0){
	     root=new segmentTree(l,r,-0x100,color(255,255,255));
		 int mid=(l+r)/2;
		 create(root->leftchild,l,mid);
		 create(root->rightchild,mid+1,r);
		 return;
	}
}

void insert(segmentTree *root,int l,int r,float zValue,float zDelta,color clr)
{
	//insert the section into segmentTree
	if(root==0)
	    return;
	
	if(root->l==l&&root->r==r){
		if(zValue>root->zValue){  //current z-value is higher than original z-value
			root->zValue=zValue;
			root->clr=clr;
		}
		root->flag=true;
		return;
	}
	
	//insert the section into segmentTree 
	root->flag=true;
	int mid=(root->l+root->r)/2;
	if(r<=mid)
		insert(root->leftchild,l,r,zValue,zDelta,clr);
	else if(l>mid)
		insert(root->rightchild,l,r,zValue,zDelta,clr);
	else
	{
		insert(root->leftchild,l,mid,zValue,zDelta,clr);
		float val=zValue+(mid-l+1)*zDelta;
		insert(root->rightchild,mid+1,r,val,zDelta,clr);  //only know the 
	}
}

//erase all the tree
void clear(segmentTree *root)
{
	if(!root)
		return;
	root->flag=false;
	root->clr=color(255,255,255);
	root->zValue=-100;

	clear(root->leftchild);
	clear(root->rightchild);
}

void drawTree(segmentTree *root,QPainter &painter,float oldVal,int scanLine)
{
	//draw segment tree
	//默认没有两个三角形贯穿的情况,故而可以使用这种方法
	if(!root)
		return;
	if(!root->flag)
		return;    //未插入数据
	if(root->zValue>oldVal)
	{
		color clr=root->clr;
		QPen pen(QColor(clr.r,clr.g,clr.b));
		painter.setPen(pen);
		painter.drawLine(root->l,scanLine,root->r+1,scanLine);  //-1  +1 的原因为消除破损点,主要原因是浮点数的近似
		oldVal=root->zValue;
	}
	drawTree(root->leftchild,painter,oldVal,scanLine);
	drawTree(root->rightchild,painter,oldVal,scanLine);
}