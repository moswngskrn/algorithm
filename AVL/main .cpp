#include <iostream>
#include <ctime>
using namespace std;

//Structure of node for AVL tree
struct Node{
    int num;
    Node * left = NULL;
    Node * right = NULL;
    int height = 0;
};

class AVLTree{
    private : Node * root;
    public :
        AVLTree(){
            root = NULL;
        };

    private : int Max(int num1,int num2){
        if(num1>num2){
            return num1;
        }
        return num2;
    }

    private : int height(Node *node){
        if(node==NULL){
            return -1;
        }
        else{
            return node->height;
        }
    }


    //Left-Of-Left
    private : Node * singleRotateRight(Node *node){
        Node * temp = node->left;
        node->left = temp->right;
        temp->right = node;
        node->height=Max(height(node->left),height(node->right))+1;
        temp->height=Max(height(node->left),height(node->right))+1;
        return temp;
    }

    //Right-Of-Right
    private : Node * singleRotateLeft(Node *node){
        Node * temp = node->right;
        node->right = temp->left;
        temp->left = node;
        node->height=Max(height(node->left),height(node->right))+1;
        temp->height=Max(height(node->left),height(node->right))+1;
        return temp;
    }

    //Right-Of-Left
    private : Node * doubleRotateRight(Node *node){
        node->left = singleRotateLeft(node->left);
        return singleRotateRight(node);
    }

    //Left-Of-Right
    private : Node * doubleRotateLeft(Node *node){
        node->right = singleRotateRight(node->right);
        return singleRotateLeft(node);
    }

    //Insert
    public :void Insert(int num){
        root = Insert(num,root);
    }
    private : Node * Insert(int num,Node * node){
        if(node == NULL){
            node = new Node;
            node->num = num;
            node->height=Max(height(node->left),height(node->right))+1;
            return node;
        }
        else if(num<node->num){
            node->left = Insert(num,node->left);
            if(height(node->left)-height(node->right) == 2){
                    if(node->left!=NULL){
                        if(node->left->num>num){
                            node = singleRotateRight(node);
                        }else{
                            node = doubleRotateRight(node);
                        }
                    }
            }
        }
        else if(num>node->num){
            node->right = Insert(num,node->right);
            if(height(node->right)-height(node->left) == 2){
                if(node->right!=NULL){
                    if(node->right->num<num){
                        node = singleRotateLeft(node);
                    }else{
                        node = doubleRotateLeft(node);
                    }
                }
            }
        }
        node->height=Max(height(node->left),height(node->right))+1;
        return node;
    }

    //Delete
    private :Node *lastNode;
    private :Node *deleteNode;
    public : void Delete(int num)   {
        lastNode = NULL;
        deleteNode = NULL;
        root = Delete(num,root);
    }
    private :Node* Delete(int num,Node * node){
        if(node==NULL){
            return NULL;
        }
        lastNode = node;

        if(num<node->num){
            node->left = Delete(num,node->left);
        }else{
            deleteNode = node;
            node->right = Delete(num,node->right);

        }
        if(node==lastNode){
            if(deleteNode != NULL && num==deleteNode->num){
                if(deleteNode==lastNode){
                    node = node->left;
                }else{
                    int temp = deleteNode->num;
                    deleteNode->num = lastNode->num;
                    node = node->right;
                }

            }
        }
        else{
            if((height(node->left)-height(node->right))==2){
                if(num<node->left->num){
                    node = singleRotateRight(node);
                }else{
                    node = doubleRotateRight(node);
                }
            }
            if((height(node->right)-height(node->left))==2){
                if(num>node->right->num){
                    node=singleRotateLeft(node);
                }else{
                    node=doubleRotateLeft(node);
                }
            }
        }
        return node;
    }

    //Search
    private : bool haveNum(int num,Node *node){
        if(node==NULL){
            return false;
        }else{
            if(num==node->num){
                return true;
            }
            else if(num<node->num){
                return haveNum(num,node->left);
            }
            else{
                return haveNum(num,node->right);
            }
        }

    }

    //Modify
    public : void Modify(int num,int toNum){
        if(haveNum(num,root)){
            Delete(num,root);
            Insert(toNum,root);
        }
    }

    //Destroy
    public : void Destroy(){
        Destroy(root);
    }
    private : void Destroy(Node *node){
        if(node->left!=NULL){
            Destroy(node->left);
        }
        cout<<"Destroy number : "<<node->num<<endl;
        cout<<"Destroy node : "<<node<<endl;


        delete node;
        if(node->right!=NULL){
            Destroy(node->right);
        }
    }

    int p;
    public : void preorder(int *arr){
        p=0;
        preorderCout(root,arr);
        cout<<endl;
    }
    public : void inorder(int *arr){
        p=0;
        inorderCout(root,arr);
        cout<<endl;
    }
    public :void postorder(int *arr){
        p=0;
        postorderCout(root,arr);
        cout<<endl;
    }

    //data of node in Pre-order fashion
    private : void preorderCout(Node * node,int *arr){
        cout<<node->num<<" ";
        arr[p] = node->num;
        p++;
        if(node->left!=NULL){
            preorderCout(node->left,arr);
        }
        if(node->right!=NULL){
            preorderCout(node->right,arr);
        }
    }
    //data of node in In-order fashion
    private : void inorderCout(Node * node,int *arr){
        if(node==NULL){
            return;
        }
        if(node->left!=NULL){
            inorderCout(node->left,arr);
        }
        cout<<node->num<<" ";
        arr[p] = node->num;
        p++;
        if(node->right!=NULL){
            inorderCout(node->right,arr);
        }
    }
    //data of node in Post-order fashion
    private : void postorderCout(Node * node,int *arr){
        if(node->left!=NULL){
            postorderCout(node->left,arr);
        }
        if(node->right!=NULL){
            postorderCout(node->right,arr);
        }
        cout<<node->num<<" ";
        arr[p] = node->num;
        p++;
    }


};

bool isEqual(int *arr1,int *arr2,int s){
    for(int i=0;i<s;i++){
        if(arr1[i]!=arr2[i]){
            return false;
        }
    }
    return true;
}

int main()
{
    cout<<"-------------test-------------"<<endl;
    int result[3] = {1,2,3};

    //test 1
    AVLTree *test1 = new AVLTree();
    int a[3];
    test1->Insert(1);
    test1->Insert(2);
    test1->Insert(3);
    test1->inorder(a);
    cout<<"inorder({1,2,3}) == {1,2,3} : "<<isEqual(a,result,3)<<endl;

    //test 2
    AVLTree *test2 = new AVLTree();
    int b[3];
    test2->Insert(1);
    test2->Insert(3);
    test2->Insert(2);
    test1->inorder(b);
    cout<<"inorder({1,3,2}) == {1,2,3} : "<<isEqual(a,result,3)<<endl;

    //test 3
    AVLTree *test3 = new AVLTree();
    int c[3];
    test3->Insert(2);
    test3->Insert(1);
    test3->Insert(3);
    test1->inorder(c);
    cout<<"inorder({2,1,3}) == {1,2,3} : "<<isEqual(a,result,3)<<endl;

    //test 4
    AVLTree *test4 = new AVLTree();
    int d[3];
    test4->Insert(2);
    test4->Insert(3);
    test4->Insert(1);
    test1->inorder(d);
    cout<<"inorder({2,3,1}) == {1,2,3} : "<<isEqual(a,result,3)<<endl;
    //test 5
    AVLTree *test5 = new AVLTree();
    int e[3];
    test5->Insert(3);
    test5->Insert(1);
    test5->Insert(2);
    test1->inorder(e);
    cout<<"inorder({3,1,2}) == {1,2,3} : "<<isEqual(a,result,3)<<endl;
    //test 6
    AVLTree *test6 = new AVLTree();
    int f[3];
    test6->Insert(3);
    test6->Insert(2);
    test6->Insert(1);
    test1->inorder(f);
    cout<<"inorder({3,2,1}) == {1,2,3} : "<<isEqual(a,result,3)<<endl;
    return 0;
}
