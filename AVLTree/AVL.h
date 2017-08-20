#ifndef _AVL_H_
#define _AVL_H_
#include <iostream>

namespace TREESPACE
{
	typedef int ElemType;//基本元素类型
	class Tree
	{
	public:
		enum BalanceFactor//平衡因子枚举
		{
			LH,//左倾
			EH,//平衡
			RH //右倾
		};
		struct TNode//树节点
		{
			ElemType data;//数据域
			struct TNode *left, *right;//指针域
			BalanceFactor BF;//平衡因子
		};
		Tree() { TreePtr = nullptr; }//构造空树
		virtual ~Tree() { Clear(TreePtr); }//析构树
		bool IsEmpty()const { return !TreePtr; }//判空树
		long Depyh()const { return Depyh(TreePtr); }//返回深度
		TNode * Find(const ElemType key) { return Find(TreePtr, key); }//查找一个节点
		friend std::ostream & operator << (std::ostream & os, Tree & T);//重载<<以供输出
		friend std::istream & operator >> (std::istream & os, Tree & T);//重载>>以供输入
		friend Tree & operator -= (Tree &T,ElemType e);//重载-=以供删除
		friend Tree & operator += (Tree &T, ElemType e);//重载+=以供插入
		virtual bool Delete(TNode * E);//删除

	private:
		TNode * TreePtr;//根指针
		long Depyh(TNode * p)const;//返回深度
		bool Insert(TNode ** T, ElemType e, bool & taller);//插入
		TNode * Find(TNode * T, const ElemType key);//查找
		void OutPut(TNode * p)const;//输出
		void R_Rotate(TNode ** p);//右旋
		void L_Rotate(TNode ** p);//左旋
		void LeftBalance(TNode ** T);//左平衡
		void RightBalance(TNode ** T);//右平衡
		void Clear(TNode * p);//释放所有节点
	};
}

#endif // !_AVL_H_