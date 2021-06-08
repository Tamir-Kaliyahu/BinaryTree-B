#pragma once
#include <iostream>
#include <stdbool.h>
#include <stack>
#include <sstream>
using namespace std;
namespace ariel
{

    template <typename T>
    class BinaryTree
    {
    private:
        struct Bnode
        {
            T node_val;
            Bnode *r_node;
            Bnode *l_node;
            Bnode(const T &n_val=NULL, Bnode *left_node = nullptr, Bnode *right_node = nullptr) //nullptr by default
                : node_val(n_val), r_node(right_node), l_node(left_node)
            {
            }
        };

        Bnode *main_root;

    public:
        BinaryTree() : main_root(nullptr){};
        ~BinaryTree() 
        {
            if(main_root!=nullptr)
            {
                DelNodes(main_root);
            }
        }

        void DelNodes(Bnode* n_del)
        {
            if(n_del->r_node!=nullptr && n_del->l_node!=nullptr)
            {
                DelNodes(n_del->r_node);
                DelNodes(n_del->l_node);
            }
            else if(n_del->r_node!=nullptr)
            {
                DelNodes(n_del->r_node);
            }
            else if(n_del->l_node!=nullptr)
            {
                DelNodes(n_del->l_node);
            }
            delete n_del;
        }
        // copy constructor
        BinaryTree(const BinaryTree &Tree1)
        {
            if(Tree1.main_root!=nullptr)
            {
            main_root = new Bnode(Tree1.main_root->node_val);
            TreeCopy(*Tree1.main_root,*main_root);
            }
        }
        // func for deep copy nodes.
        void TreeCopy(Bnode &N_src, Bnode &N_dest)
        {
            if (N_src.l_node!=nullptr)
            {
                N_dest.l_node = new Bnode(N_src.l_node->node_val);
                TreeCopy(*N_src.l_node, *N_dest.l_node);
            }
            if (N_src.r_node!=nullptr)
            {
                N_dest.r_node = new Bnode(N_src.r_node->node_val);
                TreeCopy(*N_src.r_node, *N_dest.r_node);
            }
        }
        // assingment operator
        BinaryTree & operator=(BinaryTree Tree)
        {
            if (this == &Tree) // check if Tree is the same as current.
            {
                return *this;
            }
            delete (this->main_root);
            main_root = new Bnode(Tree.main_root->node_val);
            TreeCopy(*Tree.main_root,*main_root);
            return *this;
        }
        //move assingment constructor
        BinaryTree(BinaryTree && Tree) noexcept {
            this->main_root  = Tree.main_root;
            Tree.main_root = nullptr;
        }
        // move assingment operator
        BinaryTree& operator=(BinaryTree && Tree) noexcept {
            if (main_root!=nullptr)
            { 
                delete main_root;
            }
            this->main_root  = Tree.main_root;
            Tree.main_root = nullptr;
            return *this;
        }
        // enum to determine order
        enum Order
        {
            preorder,
            inorder,
            postorder
        };
        // create a stack for iterator and fill the stack in the relevant order.
        static std::stack<Bnode *> init_stack(enum Order o, Bnode *RootNode)
        {
            stack<Bnode *> ans;
            ans.push(nullptr);
            if (RootNode == nullptr)
            {
                return ans;
            }
                
            switch (o)
            {
            case BinaryTree::Order::inorder:
                init_stackIn(ans, RootNode);
                break;
            case Order::postorder:
                init_stackPost(ans, RootNode);
                break;
            case Order::preorder:
                init_stackPre(ans, RootNode);
                break;
            }
            return ans;
        }
        // 3 recursive funs in order to fill entire stack with Bnode pointers.
        static void init_stackIn(stack<Bnode *> &InStack, Bnode *Current)
        {
            if (Current == nullptr)
            {
                return;
            }

            init_stackIn(InStack, Current->r_node);
            InStack.push(Current);
            init_stackIn(InStack, Current->l_node);
        }

        static void init_stackPost(stack<Bnode *> &InStack, Bnode *Current)
        {
            if (Current == nullptr)
            {
                return;
            }

            InStack.push(Current);
            init_stackPost(InStack, Current->r_node);
            init_stackPost(InStack, Current->l_node);
        }

        static void init_stackPre(stack<Bnode *> &InStack, Bnode *Current)
        {
            if (Current == nullptr)
            {
                return;
            }

            init_stackPre(InStack, Current->r_node);
            init_stackPre(InStack, Current->l_node);
            InStack.push(Current);
        }
        // find node func in order to check for the exsistance of a node in tree for building it.
        Bnode *findNode(Bnode *current, T N_val)
        {
            if (current == nullptr)
            {
                return nullptr;
            }
            stack<Bnode *> nodeStack;
            nodeStack.push(current);
            
            while (!nodeStack.empty())
            {
                Bnode *tempNode = nodeStack.top();
                nodeStack.pop();
                if (tempNode->node_val == N_val)
                {
                    return tempNode;
                }
                if (tempNode->r_node != nullptr)
                {
                    nodeStack.push(tempNode->r_node);
                }
                if (tempNode->l_node != nullptr)
                {
                    nodeStack.push(tempNode->l_node);
                }
            }
            return nullptr;
        }

        BinaryTree<T> &add_root(T t1)
        {
            if (main_root == nullptr)
            {
                main_root = new Bnode(t1);
            }
            else
            {
                (this->main_root->node_val) = t1;
            }
            return *this;
        }

        BinaryTree<T> &add_left(T t1, T t2)
        {
            Bnode *CurNode = findNode(main_root, t1);
            if (CurNode != nullptr)
            {
                if (CurNode->l_node != nullptr)
                {
                    CurNode->l_node->node_val = t2;
                }
                else
                {
                    CurNode->l_node = new Bnode(t2);
                }
            }
            else
            {
                throw invalid_argument("Root not found!");
            }
            return *this;
        }

        BinaryTree<T> &add_right(const T &t1, const T &t2)
        {
            Bnode *CurNode = findNode(main_root, t1);
            if (CurNode != nullptr)
            {
                if (CurNode->r_node != nullptr)
                {
                    CurNode->r_node->node_val = t2;
                }
                else
                {
                    CurNode->r_node = new Bnode(t2);
                }
            }
            else
            {
                throw invalid_argument("Root not found!");
            }
            return *this;
        }

        friend std::ostream &operator<<(std::ostream &out, const BinaryTree<T> &Tree)
        {
            out << endl;
            out << "---My Binary Tree---:" << endl;
            if (Tree.main_root == NULL)
            {
                return out;
            }
            out << Tree.main_root->node_val << endl;
            printSubtree(out, Tree.main_root,"");
            out << endl;
            return out;
        }
        // 90 degree rotated Tree printer func inspired from a code found in StackOverflow. https://stackoverflow.com/questions/13484943/print-a-binary-tree-in-a-pretty-way
        static ostream &printSubtree(std::ostream &out, Bnode *root, const string &prefix)
        {
            if (root == NULL)
            {
                return out;
            }

            bool hasLeft = (root->l_node != NULL);
            bool hasRight = (root->r_node != NULL);

            if (!hasLeft && !hasRight)
            {
                return out;
            }

            out << prefix;
            out << ((hasLeft && hasRight) ? "├─── " : "");
            out << ((!hasLeft && hasRight) ? "└─── " : "");
            if (hasRight)
            {

                bool printStrand = (hasLeft && hasRight && (root->r_node->r_node != NULL || root->r_node->l_node != NULL));

                string newPrefix = prefix + (printStrand ? "│   " : "    ");
                out << root->r_node->node_val << endl;
                printSubtree(out, root->r_node, newPrefix);
            }

            if (hasLeft)
            {
                out << (hasRight ? prefix : "") << "└─── " << root->l_node->node_val << endl;
                printSubtree(out, root->l_node, prefix + "    ");
            }
            return out;
        }

        class iterator
        {
        public:
            Bnode *currentNode;
            std::stack<Bnode *> iterStack;

            iterator() : currentNode(nullptr){};

            iterator(enum Order o, Bnode *RootNode)
            {

                iterStack = init_stack(o, RootNode);
                if (!iterStack.empty())
                {
                    currentNode = iterStack.top();
                    iterStack.pop();
                }
            }

            T &operator*() const
            {
                return currentNode->node_val;
            }

            T *operator->() const
            {
                return &(currentNode->node_val);
            }

            iterator &operator++()
            {
                if (iterStack.empty())
                {
                    currentNode = nullptr;
                }
                else
                {
                    currentNode = iterStack.top();
                    iterStack.pop();
                }

                return *this;
            }

            iterator operator++(int)
            {
                iterator temp = *this;
                if (iterStack.empty())
                {
                    currentNode = nullptr;
                }
                else
                {
                    currentNode = iterStack.top();
                    iterStack.pop();
                }
                return temp;
            }

            bool operator==(const iterator &rhs) const
            {
                return currentNode == rhs.currentNode;
            }

            bool operator!=(const iterator &rhs) const
            {
                return currentNode != rhs.currentNode;
            }
        };

        iterator begin()
        {
            return iterator(Order::inorder, main_root);
        }

        iterator end()
        {
            return iterator();
        }

        iterator begin_preorder()
        {
            return iterator(Order::preorder, main_root);
        }

        iterator end_preorder()
        {
            return iterator();
        }

        iterator begin_inorder()
        {
            return iterator(Order::inorder, main_root);
        }

        iterator end_inorder()
        {
            return iterator();
        }

        iterator begin_postorder()
        {
            return iterator(Order::postorder, main_root);
        }

        iterator end_postorder()
        {
            return iterator();
        }
    };

}