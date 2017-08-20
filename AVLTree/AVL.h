#ifndef _AVL_H_
#define _AVL_H_
#include <iostream>

namespace TREESPACE
{
	typedef int ElemType;//����Ԫ������
	class Tree
	{
	public:
		enum BalanceFactor//ƽ������ö��
		{
			LH,//����
			EH,//ƽ��
			RH //����
		};
		struct TNode//���ڵ�
		{
			ElemType data;//������
			struct TNode *left, *right;//ָ����
			BalanceFactor BF;//ƽ������
		};
		Tree() { TreePtr = nullptr; }//�������
		virtual ~Tree() { Clear(TreePtr); }//������
		bool IsEmpty()const { return !TreePtr; }//�п���
		long Depyh()const { return Depyh(TreePtr); }//�������
		TNode * Find(const ElemType key) { return Find(TreePtr, key); }//����һ���ڵ�
		friend std::ostream & operator << (std::ostream & os, Tree & T);//����<<�Թ����
		friend std::istream & operator >> (std::istream & os, Tree & T);//����>>�Թ�����
		friend Tree & operator -= (Tree &T,ElemType e);//����-=�Թ�ɾ��
		friend Tree & operator += (Tree &T, ElemType e);//����+=�Թ�����
		virtual bool Delete(TNode * E);//ɾ��

	private:
		TNode * TreePtr;//��ָ��
		long Depyh(TNode * p)const;//�������
		bool Insert(TNode ** T, ElemType e, bool & taller);//����
		TNode * Find(TNode * T, const ElemType key);//����
		void OutPut(TNode * p)const;//���
		void R_Rotate(TNode ** p);//����
		void L_Rotate(TNode ** p);//����
		void LeftBalance(TNode ** T);//��ƽ��
		void RightBalance(TNode ** T);//��ƽ��
		void Clear(TNode * p);//�ͷ����нڵ�
	};
}

#endif // !_AVL_H_