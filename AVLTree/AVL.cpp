#include <iostream>
#include "AVL.h"

std::ostream & TREESPACE::operator << (std::ostream & os, Tree & T)
{
	T.OutPut(T.TreePtr);//调用OutPut以输出
	return os;//返回原调用
}

void TREESPACE::Tree::OutPut(TNode*p)const
{
	if (p)
		std::cout << p->data;
	else
	{
		std::cout << "null";//null替代nullptr指针的值
		return;
	}
	if (p->left || p->right)//以广义表方式输出
	{
		std::cout << " < ";
		OutPut(p->left);//输出左子树
		std::cout << " , ";
		OutPut(p->right);//输出右子树
		std::cout << " >";
	}
}

std::istream & TREESPACE::operator >> (std::istream & is, Tree & T)
{
	ElemType i;
	bool taller;
	if (is >> i) T.Insert(&T.TreePtr, i, taller);//读取输入并用Insert插入树
	return is;//返回原调用
}

TREESPACE::Tree & TREESPACE::operator-=(Tree &T, ElemType e)
{
	T.Delete(T.Find(e));//按值搜索并删除
	return T;
}

TREESPACE::Tree & TREESPACE::operator+=(Tree & T, ElemType e)
{
	bool taller;
	T.Insert(&T.TreePtr, e, taller);//插入
	return T;
}

bool TREESPACE::Tree::Delete(TNode * pe)
{
	TNode*p = TreePtr;//p将指向删除节点的双亲
	while (pe && (p && !(p->left == pe || p->right == pe)) && pe != TreePtr)
		pe->data > p->data ? p = p->right : p = p->left;
	if (!p || !pe) return false;//空树或无该节点
	if (!pe->left && !pe->right)//删除节点无孩子，直接删除并修改其双亲
	{
		if (p->data > pe->data)
			p->left = nullptr;
		else
			p->right = nullptr;
		if (p->left && !p->right)//重置平衡因子
			p->BF = LH;
		else if (!p->left && p->right)
			p->BF = RH;
		else p->BF = EH;
	}
	else if (!pe->left || !pe->right)//删除节点有一个孩子，令该孩子成为其双亲的孩子
	{
		TNode *pn;
		pe->left ? pn = pe->left : pn = pe->right;
		if (pe == TreePtr)//删除节点为根节点
			TreePtr = pn;
		else if (p->data > pe->data)
			p->left = pn;
		else
			p->right = pn;
	}
	else//删除节点有两个孩子，在右子树中寻找最小（最左）值，替换该值并删除找到的节点
	{
		TNode *pf = pe;
		pe = pe->right;
		while (pe->left) pe = pe->left;
		pf->data = pe->data;
		return Delete(pe);
	}
	delete pe;//删除
	return true;
}

long TREESPACE::Tree::Depyh(TNode *p) const
{
	if (!p) return 0;//递归在叶子后停止
	long T_r_c = Depyh(p->right);
	long T_l_c = Depyh(p->left);
	return T_r_c > T_l_c ? (T_r_c + 1) : (T_l_c + 1);//使用较长的路径，加1（节点本身代表了1路径）返回
}

bool TREESPACE::Tree::Insert(TNode **T, ElemType e, bool &taller)
{
	if (!*T)//创建一个新节点
	{
		*T = new TNode;
		(*T)->data = e;
		(*T)->left = (*T)->right = nullptr;
		(*T)->BF = EH;
		taller = true;//树长高
	}
	else
	{
		if (e == (*T)->data)//不插入
		{
			taller = false;
			return false;
		}
		if (e<(*T)->data)//在T的左子树中搜寻
		{
			if (!Insert(&(*T)->left, e, taller))//未插入返回，插入则判断是否长高
				return false;
			if (taller)//已插入左子树，且左子树变高
				switch ((*T)->BF)
				{
				case LH://原本左子树比右子树高，需要做左平衡处理
					LeftBalance(T);
					taller = false;
					break;
				case EH://原本左右子树等高，现因左子树增高而树增高
					(*T)->BF = LH;
					taller = true;
					break;
				case RH://原本右子树比左子树高，现在左右子树等高
					(*T)->BF = EH;
					taller = false;
					break;
				}
		}
		else//在T的右子树中搜寻
		{
			if (!Insert(&(*T)->right, e, taller))//未插入返回，插入则判断是否长高
				return false;
			if (taller)//插入右子树，且右子树长高
				switch ((*T)->BF)
				{
				case LH://原本左子树比右子树高，现在左右子树等高
					(*T)->BF = EH;
					taller = false;
					break;
				case EH://原本左右子树等高，现在右子树变高
					(*T)->BF = RH;
					taller = true;
					break;
				case RH://原本右子树比左子树高，现在需做右平衡处理
					RightBalance(T);
					taller = false;
					break;
				}
		}
	}
	return true;
}

TREESPACE::Tree::TNode * TREESPACE::Tree::Find(TNode * T, const ElemType key)
{
	if (!T)
	{
		std::cout << "Not Found!\n";
		return nullptr;
	}
	else if (T->data == key)
		return T;
	else if (T->data < key)//依查找树的性质查找
		return Find(T->right, key);
	else
		return Find(T->left, key);
}

void TREESPACE::Tree::R_Rotate(TNode ** p)//右旋
{
	TNode*L;

	L = (*p)->left;
	(*p)->left = L->right;
	L->right = (*p);
	*p = L;//p指向新的根节点
}

void TREESPACE::Tree::L_Rotate(TNode ** p)//左旋
{
	TNode*R;

	R = (*p)->right;
	(*p)->right = R->left;
	R->left = (*p);
	*p = R;
}

void TREESPACE::Tree::LeftBalance(TNode ** T)
{
	TNode*L, *Lr;
	L = (*T)->left;
	switch (L->BF)//检查T的左子树平衡度，并作相应的平衡处理
	{
	case LH://新节点插入在T的左孩子的左子树上，做单右旋处理
		(*T)->BF = L->BF = EH;
		R_Rotate(T);
		break;
	case RH://新插入节点在T的左孩子的右子树上，做双旋处理
		Lr = L->right;
		switch (Lr->BF)
		{
		case LH:
			(*T)->BF = RH;
			L->BF = EH;
			break;
		case EH:
			(*T)->BF = L->BF = EH;
			break;
		case RH:
			(*T)->BF = EH;
			L->BF = LH;
			break;
		}
		Lr->BF = EH;
		L_Rotate(&(*T)->left);
		R_Rotate(T);
	}
}

void TREESPACE::Tree::RightBalance(TNode ** T)
{
	TNode*R, *Rl;
	R = (*T)->right;
	switch (R->BF)
	{
	case RH://新节点插在T的右孩子的右子树上，做单左旋处理
		(*T)->BF = R->BF = EH;
		L_Rotate(T);
		break;
	case LH://新节点插在T的右孩子的左子树上，做双旋处理
		Rl = R->left;
		switch (Rl->BF)
		{
		case LH:
			(*T)->BF = EH;
			R->BF = RH;
			break;
		case EH:
			(*T)->BF = R->BF = EH;
			break;
		case RH:
			(*T)->BF = LH;
			R->BF = EH;
			break;
		}
		Rl->BF = EH;
		R_Rotate(&(*T)->right);
		L_Rotate(T);
	}
}

void TREESPACE::Tree::Clear(TNode * p)//遍历树，并从叶子开始删除整棵树
{
	if (!p) return;
	Clear(p->left);
	Clear(p->right);
	delete p;
}
