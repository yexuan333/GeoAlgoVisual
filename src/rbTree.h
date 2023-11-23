#ifndef RBTree_H__
#define RBTree_H__

#include <iostream>
#include <memory>
#include <list>
#include <string>

using namespace std;
// red-black tree

const auto RED = 0;
const auto BLACK = 1;
const auto NULL_NODE = -100;
const auto LEFT_CHILD = 0;
const auto RIGHT_CHILD = 1;

template<typename T>
class RBTreeNode
{
public:
	RBTreeNode(const T& data, int color)
		: Data(data), Color(color), Parent(nullptr), LChild(nullptr), RChild(nullptr){}

public:
	T		Data;
	int		Color;
	RBTreeNode* Parent;
	RBTreeNode* LChild;
	RBTreeNode* RChild;
};

template<typename T>
class RBTree
{
public:
	RBTree(): m_root(nullptr){}

	int insert(const T& data)
	{
		if (!m_root)
		{
			this->m_root = new RBTreeNode<T>(data, BLACK);
			return 0;
		}

		return insert_node(nullptr, LEFT_CHILD, this->m_root, data);
	}

	int erase(const T& data)
	{
		if (nullptr == this->m_root)
			return -1;
		return erase_node(nullptr, LEFT_CHILD, this->m_root, data);
	}

	void dump()
	{
		mid_visit_node(this->m_root);
	}

	int height()
	{
		return this->height_i(this->m_root);
	}

	void check_rb_feature_5()
	{
		std::list<int> list;
		char trace[100] = { 0 };
		check_rb_feature_5_impl(nullptr, this->m_root, trace, 0, list);
		int len = -1;
		for (auto it = list.begin(); it != list.end(); ++it)
		{
			if (-1 == len)
				len = *it;
			else if (len != *it)
			{
				std::cout << "check_rb_feature_5 failed: " << std::endl;
				return;
			}
		}

		std::cout << "check_rb_feature_5 success, black node count:  " << len << std::endl;
	}

private:
	int insert_node(RBTreeNode<T>* pp, int pp_child_tag, RBTreeNode<T>* p, const T& data)
	{
		if (nullptr == p)
		{
			p = new RBTreeNode<T>(data, RED);
			LEFT_CHILD == pp_child_tag ? pp->LChild = p : pp->RChild = p;
			p->Parent = pp;
			if (RED == pp->Color)
				this->adjust_balance(p);
			return 0;
		}
		else if (p->Data == data)
			return -1;
		else if (data < p->Data)
			return this->insert_node(p, LEFT_CHILD, p->LChild, data);
		else
			return this->insert_node(p, RIGHT_CHILD, p->RChild, data);
	}

	int erase_node(RBTreeNode<T>* pp, int pp_child_tag, RBTreeNode<T>* p, const T& data)
	{
		if (nullptr == p)
			return -1;
		else if (p->Data == data)
		{
			RBTreeNode<T>* fake_null = nullptr;
			int delete_node_color = p->Color;
			RBTreeNode<T>*& ref_p = (pp ? (LEFT_CHILD == pp_child_tag ? pp->LChild : pp->RChild) : this->m_root);

			if (p->LChild && p->RChild)
			{
				auto prev = p->LChild;
				auto curr = prev;
				for (; curr && curr->RChild; prev = curr, curr = curr->RChild);
				delete_node_color = curr->Color;
				if (curr == prev)
				{
					p->Data = curr->Data;
					p->LChild = curr->LChild;
					if (curr->LChild)
						curr->LChild->Parent = p;
					pp = p;
					p = p->LChild;
					pp_child_tag = LEFT_CHILD;
					delete curr;
				}
				else
				{
					p->Data = curr->Data;
					pp = prev;
					pp_child_tag = RIGHT_CHILD;
					p = prev->RChild = curr->LChild;
					if (curr->LChild)
						curr->LChild->Parent = prev;
					delete curr;
				}
			}
			else if (p->LChild)
			{
				ref_p = p->LChild;
				p->LChild->Parent = pp;
				delete p;
				p = ref_p;
			}
			else if (p->RChild)
			{
				ref_p = p->RChild;
				p->RChild->Parent = pp;
				delete p;
				p = ref_p;
			}
			else
			{
				p = ref_p = nullptr;
			}

			if (BLACK == delete_node_color && pp)
			{
				if (nullptr == p)
				{
					fake_null = p = new RBTreeNode<T>(NULL_NODE, BLACK);
					p->Parent = pp;
					LEFT_CHILD == pp_child_tag ? pp->LChild = p : pp->RChild = p;
				}

				this->adjust_delete_balance(pp, p);
				if (fake_null)
				{
					if (fake_null->Parent->LChild == fake_null)
						fake_null->Parent->LChild = nullptr;
					else
						fake_null->Parent->RChild = nullptr;
					delete fake_null;
				}
			}
			return 0;
		}
		else if (data < p->Data)
			return this->erase_node(p, LEFT_CHILD, p->LChild, data);
		else
			return this->erase_node(p, RIGHT_CHILD, p->RChild, data);
	}

private:
	void adjust_balance(RBTreeNode<T>* p)
	{
		while (p->Parent && p->Parent->Parent && RED == p->Parent->Color)
		{
			auto pp = p->Parent;
			auto gp = pp->Parent;
			auto up = (gp->LChild == pp ? gp->RChild : gp->LChild);
			if (gp->LChild == pp)
			{
				if (up && RED == up->Color)
				{
					pp->Color = up->Color = BLACK;
					gp->Color = RED;
					p = gp;
				}
				else
				{
					if (pp->RChild == p)
					{
						this->l_rotate(pp);
						swap(p, pp);
					}

					pp->Color = BLACK;
					gp->Color = RED;
					this->r_rotate(gp);
				}
			}
			else
			{
				if (up && RED == up->Color)
				{
					pp->Color = up->Color = BLACK;
					gp->Color = RED;
					p = gp;
				}
				else
				{
					if (pp->LChild == p)
					{
						this->r_rotate(pp);
						swap(p, pp);
					}

					pp->Color = BLACK;
					gp->Color = RED;
					this->l_rotate(gp);
				}
			}
		}

		this->m_root->Color = BLACK;
	}

	void adjust_delete_balance(RBTreeNode<T>* pp, RBTreeNode<T>* p)
	{
		while ((nullptr == p || BLACK == p->Color) && p != this->m_root)
		{
			if (pp->LChild == p)
			{
				auto s = pp->RChild;
				if (s && RED == s->Color)
				{
					s->Color = BLACK;
					pp->Color = RED;
					this->l_rotate(pp);
					s = pp->RChild;
				}

				if ((nullptr == s->LChild || BLACK == s->LChild->Color)
					&& (nullptr == s->RChild || BLACK == s->RChild->Color))
				{
					s->Color = RED;
					p = pp;
					pp = p->Parent;
				}
				else
				{
					if (nullptr == s->RChild || BLACK == s->RChild->Color)
					{
						s->LChild ? s->LChild->Color = BLACK : 0;
						s->Color = RED;
						this->r_rotate(s);
						s = pp->RChild;
					}

					s->Color = pp->Color;
					pp->Color = BLACK;
					s->RChild->Color = BLACK;
					this->l_rotate(pp);
					p = this->m_root;
				}
			}
			else
			{
				auto s = pp->LChild;
				if (s && RED == s->Color)
				{
					s->Color = BLACK;
					pp->Color = RED;
					this->r_rotate(pp);
					s = pp->LChild;
				}

				if ((nullptr == s->LChild || BLACK == s->LChild->Color)
					&& (nullptr == s->RChild || BLACK == s->RChild->Color))
				{
					s->Color = RED;
					p = pp;
					pp = p->Parent;
				}
				else
				{
					if (nullptr == s->LChild || BLACK == s->LChild->Color)
					{
						s->RChild ? s->RChild->Color = BLACK : 0;
						s->Color = RED;
						this->l_rotate(s);
						s = pp->LChild;
					}

					s->Color = pp->Color;
					pp->Color = BLACK;
					s->LChild->Color = BLACK;
					this->r_rotate(pp);
					p = this->m_root;
				}
			}
		}

		p->Color = BLACK;
	}

private:
	RBTreeNode<T>* l_rotate(RBTreeNode<T>* p)
	{
		auto lr = p->RChild;
		p->RChild = lr->LChild;
		if (lr->LChild)
			lr->LChild->Parent = p;

		lr->Parent = p->Parent;
		if (nullptr == p->Parent)
			this->m_root = lr;
		else if (p->Parent->LChild == p)
			p->Parent->LChild = lr;
		else
			p->Parent->RChild = lr;

		lr->LChild = p;
		p->Parent = lr;

		return lr;
	}

	RBTreeNode<T>* r_rotate(RBTreeNode<T>* p)
	{
		auto lc = p->LChild;
		p->LChild = lc->RChild;
		if (lc->RChild)
			lc->RChild->Parent = p;

		lc->Parent = p->Parent;
		if (nullptr == p->Parent)
			this->m_root = lc;
		else if (p->Parent->LChild == p)
			p->Parent->LChild = lc;
		else
			p->Parent->RChild = lc;

		lc->RChild = p;
		p->Parent = lc;

		return lc;
	}

	int height_i(RBTreeNode<T>* p)
	{
		if (nullptr == p)
			return 0;

		int left_height = 0;
		if (p->LChild)
			left_height = height_i(p->LChild) + 1;

		int right_height = 0;
		if (p->RChild)
			right_height = height_i(p->RChild) + 1;

		return left_height >= right_height ? left_height : right_height;
	}

	void mid_visit_node(const RBTreeNode<T>* p)
	{
		if (nullptr != p)
		{
			mid_visit_node(p->LChild);
			std::cout << p->Data << std::endl;
			mid_visit_node(p->RChild);
		}
	}

	void check_rb_feature_5_impl(RBTreeNode<T>* pp, RBTreeNode<T>* p, char trace[], int idx, std::list<int>& list)
	{
		if (nullptr == p)
		{
			trace[idx] = BLACK;
			int result = 0;
			for (auto k = 0; k <= idx; ++k)
			{
				if (BLACK == trace[k])
					++result;
			}

			list.push_back(result);
		}
		else
		{
			trace[idx] = p->Color;
			check_rb_feature_5_impl(p, p->LChild, trace, idx + 1, list);
			check_rb_feature_5_impl(p, p->RChild, trace, idx + 1, list);
		}
	}

private:
	RBTreeNode<T>*	m_root;
};

#endif

