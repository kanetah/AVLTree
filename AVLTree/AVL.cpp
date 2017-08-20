#include <iostream>
#include "AVL.h"

std::ostream & TREESPACE::operator << (std::ostream & os, Tree & T)
{
	T.OutPut(T.TreePtr);//����OutPut�����
	return os;//����ԭ����
}

void TREESPACE::Tree::OutPut(TNode*p)const
{
	if (p)
		std::cout << p->data;
	else
	{
		std::cout << "null";//null���nullptrָ���ֵ
		return;
	}
	if (p->left || p->right)//�Թ����ʽ���
	{
		std::cout << " < ";
		OutPut(p->left);//���������
		std::cout << " , ";
		OutPut(p->right);//���������
		std::cout << " >";
	}
}

std::istream & TREESPACE::operator >> (std::istream & is, Tree & T)
{
	ElemType i;
	bool taller;
	if (is >> i) T.Insert(&T.TreePtr, i, taller);//��ȡ���벢��Insert������
	return is;//����ԭ����
}

TREESPACE::Tree & TREESPACE::operator-=(Tree &T, ElemType e)
{
	T.Delete(T.Find(e));//��ֵ������ɾ��
	return T;
}

TREESPACE::Tree & TREESPACE::operator+=(Tree & T, ElemType e)
{
	bool taller;
	T.Insert(&T.TreePtr, e, taller);//����
	return T;
}

bool TREESPACE::Tree::Delete(TNode * pe)
{
	TNode*p = TreePtr;//p��ָ��ɾ���ڵ��˫��
	while (pe && (p && !(p->left == pe || p->right == pe)) && pe != TreePtr)
		pe->data > p->data ? p = p->right : p = p->left;
	if (!p || !pe) return false;//�������޸ýڵ�
	if (!pe->left && !pe->right)//ɾ���ڵ��޺��ӣ�ֱ��ɾ�����޸���˫��
	{
		if (p->data > pe->data)
			p->left = nullptr;
		else
			p->right = nullptr;
		if (p->left && !p->right)//����ƽ������
			p->BF = LH;
		else if (!p->left && p->right)
			p->BF = RH;
		else p->BF = EH;
	}
	else if (!pe->left || !pe->right)//ɾ���ڵ���һ�����ӣ���ú��ӳ�Ϊ��˫�׵ĺ���
	{
		TNode *pn;
		pe->left ? pn = pe->left : pn = pe->right;
		if (pe == TreePtr)//ɾ���ڵ�Ϊ���ڵ�
			TreePtr = pn;
		else if (p->data > pe->data)
			p->left = pn;
		else
			p->right = pn;
	}
	else//ɾ���ڵ����������ӣ�����������Ѱ����С������ֵ���滻��ֵ��ɾ���ҵ��Ľڵ�
	{
		TNode *pf = pe;
		pe = pe->right;
		while (pe->left) pe = pe->left;
		pf->data = pe->data;
		return Delete(pe);
	}
	delete pe;//ɾ��
	return true;
}

long TREESPACE::Tree::Depyh(TNode *p) const
{
	if (!p) return 0;//�ݹ���Ҷ�Ӻ�ֹͣ
	long T_r_c = Depyh(p->right);
	long T_l_c = Depyh(p->left);
	return T_r_c > T_l_c ? (T_r_c + 1) : (T_l_c + 1);//ʹ�ýϳ���·������1���ڵ㱾�������1·��������
}

bool TREESPACE::Tree::Insert(TNode **T, ElemType e, bool &taller)
{
	if (!*T)//����һ���½ڵ�
	{
		*T = new TNode;
		(*T)->data = e;
		(*T)->left = (*T)->right = nullptr;
		(*T)->BF = EH;
		taller = true;//������
	}
	else
	{
		if (e == (*T)->data)//������
		{
			taller = false;
			return false;
		}
		if (e<(*T)->data)//��T������������Ѱ
		{
			if (!Insert(&(*T)->left, e, taller))//δ���뷵�أ��������ж��Ƿ񳤸�
				return false;
			if (taller)//�Ѳ����������������������
				switch ((*T)->BF)
				{
				case LH://ԭ�����������������ߣ���Ҫ����ƽ�⴦��
					LeftBalance(T);
					taller = false;
					break;
				case EH://ԭ�����������ȸߣ��������������߶�������
					(*T)->BF = LH;
					taller = true;
					break;
				case RH://ԭ�����������������ߣ��������������ȸ�
					(*T)->BF = EH;
					taller = false;
					break;
				}
		}
		else//��T������������Ѱ
		{
			if (!Insert(&(*T)->right, e, taller))//δ���뷵�أ��������ж��Ƿ񳤸�
				return false;
			if (taller)//������������������������
				switch ((*T)->BF)
				{
				case LH://ԭ�����������������ߣ��������������ȸ�
					(*T)->BF = EH;
					taller = false;
					break;
				case EH://ԭ�����������ȸߣ��������������
					(*T)->BF = RH;
					taller = true;
					break;
				case RH://ԭ�����������������ߣ�����������ƽ�⴦��
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
	else if (T->data < key)//�������������ʲ���
		return Find(T->right, key);
	else
		return Find(T->left, key);
}

void TREESPACE::Tree::R_Rotate(TNode ** p)//����
{
	TNode*L;

	L = (*p)->left;
	(*p)->left = L->right;
	L->right = (*p);
	*p = L;//pָ���µĸ��ڵ�
}

void TREESPACE::Tree::L_Rotate(TNode ** p)//����
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
	switch (L->BF)//���T��������ƽ��ȣ�������Ӧ��ƽ�⴦��
	{
	case LH://�½ڵ������T�����ӵ��������ϣ�������������
		(*T)->BF = L->BF = EH;
		R_Rotate(T);
		break;
	case RH://�²���ڵ���T�����ӵ��������ϣ���˫������
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
	case RH://�½ڵ����T���Һ��ӵ��������ϣ�������������
		(*T)->BF = R->BF = EH;
		L_Rotate(T);
		break;
	case LH://�½ڵ����T���Һ��ӵ��������ϣ���˫������
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

void TREESPACE::Tree::Clear(TNode * p)//������������Ҷ�ӿ�ʼɾ��������
{
	if (!p) return;
	Clear(p->left);
	Clear(p->right);
	delete p;
}
