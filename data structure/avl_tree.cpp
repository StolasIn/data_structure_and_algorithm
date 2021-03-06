#include <iostream>
using namespace std;
template <typename T>
class node{
    public:
    T key;
    int height;
    node* left;
    node* right;
    node(T k){
        height = 1;
        key = k;
        left = nullptr;
        right = nullptr;
    }
};

template <typename T>
class avl_tree{
    public:
        node<T>* root;

        //constructor
        avl_tree(){
            root=nullptr;
        }

        //destructor
        ~avl_tree(){
            delete_all(root);
        }

        void delete_tree(){
            delete_all(root);
        }

        //delete all node use recursive
        void delete_all(node<T>* root){
            if(root==nullptr) return;
            delete_all(root->left);
            delete_all(root->right);
            delete root;
        }

        T operator[](T x){
            auto tmp = search(x);
            if(tmp==nullptr) {
                printf("not found , return base type");
                T a;
                return a;
            }
            else return tmp->key;
        }

        //user insert data
        void insert(T x){
            root=insert_node(root, x);
        }

        //user delete data
        void remove(T x){
            root=remove_node(root, x);
        }

        //return iterator address
        node<T> * search(T x){
            return search_node(root,x);
        }

        void inorder(){
            inorder_traversal(root);
            cout<<endl;
        }

        void preorder(){
            preorder_traversal(root);
            cout<<endl;
        }

        void postorder(){
            postorder_traversal(root);
            cout<<endl;
        }

        int get_height(){
            return tree_height(root);
        }

    private:
        int tree_height(node<T>* root){
            if(root==nullptr) return 0;
            int x=tree_height(root->left);
            int y=tree_height(root->right);
            return max(x,y)+1;
        }
        int height(node<T> * head){
            if(head==nullptr) return 0;
            return head->height;
        }

       /*       y                              x
               / \      Right Rotation        / \
              x   C     -------------->      A   y
             / \                                / \
            A   B                              B   C*/

        /*
            1.y???head , x???head????????????
            2.x???????????????
            3.x??????????????????y????????????
            4.y??????x????????????
        */

        node<T> * rightRotation(node<T> * head){
            //???????????????????????????????????????????????????????????????
            node<T> * newhead = head->left;

            //????????????????????????????????????????????????
            head->left = newhead->right;

            //?????????????????????????????????
            newhead->right = head;

            //??????????????????
            head->height = 1+max(height(head->left), height(head->right));
            newhead->height = 1+max(height(newhead->left), height(newhead->right));

            //?????????????????????head???parent
            return newhead;
        }



        /*      x                              y
               / \       Left Rotation        / \
              A   y      ------------->      x   C
                 / \                        / \
                B   C                      A   B*/

        /*
            1.x???head , y???x???????????????
            2.y???????????????
            3.y??????????????????x????????????
            4.x??????y????????????
        */

        node<T> * leftRotation(node<T> * head){

            //????????????head??????????????????????????????head
            node<T> * newhead = head->right;

            //???????????????????????????head????????????
            head->right = newhead->left;

            //head????????????????????????
            newhead->left = head;

            //?????????????????????
            head->height = 1+max(height(head->left), height(head->right));
            newhead->height = 1+max(height(newhead->left), height(newhead->right));

            //??????????????????head???parent
            return newhead;
        }

        //inorder
        void inorder_traversal(node<T> * head){
            if(head==NULL) return ;
            inorder_traversal(head->left);
            cout<<head->key<<endl;
            inorder_traversal(head->right);
        }


        //preorder
        void preorder_traversal(node<T> * head){
            if(head==NULL) return ;
            cout<<head->key<<endl;
            preorder_traversal(head->left);
            preorder_traversal(head->right);
        }


        //postorder
        void postorder_traversal(node<T> * head){
            if(head==NULL) return ;
            postorder_traversal(head->left);
            postorder_traversal(head->right);
            cout<<head->key<<endl;
        }


        //insert ???
        node<T> * insert_node(node<T> * head, T x){
            if(head==NULL){
                //?????????????????????????????????
                node<T> * temp = new node<T>(x);
                return temp;
            }
            //??????insert??????????????????????????????????????????????????????????????????head???????????????
            if(x < head->key) head->left = insert_node(head->left, x);

            //?????????????????????????????????????????????head????????????
            else if(x > head->key) head->right = insert_node(head->right, x);
            //?????????????????????

            //????????????????????????????????????????????????+1 ?????????????????????
            head->height = 1 + max(height(head->left), height(head->right));

            //??????balence factor, ??????????????????-??????????????????
            int bal = height(head->left) - height(head->right);

            //???????????? rr,lr,ll,rl
            //????????????????????????????????????abs(balence factor)>1
            if(bal>1){
                //????????????????????????x???????????????
                if(x < head->left->key){
                    //??????x???????????????????????????ll??????????????????
                    return rightRotation(head);
                }else{
                    //??????x???????????????????????????lr???
                    //???????????????????????????????????????????????????????????????
                    head->left = leftRotation(head->left);
                    return rightRotation(head);
                }
            }else if(bal<-1){
                //????????????????????????x???????????????
                if(x > head->right->key){
                    //??????x???????????????????????????rr??????????????????
                    return leftRotation(head);
                }else{
                    //??????x???????????????????????????rl???
                    //???????????????????????????????????????????????????????????????
                    head->right = rightRotation(head->right);
                    return leftRotation(head);
                }
            }
            //???????????????
            return head;
        }

        //?????????
        node<T> * remove_node(node<T> * head, T x){
            //??????????????????
            if(head==nullptr) return nullptr;

            //?????????
            if(x < head->key){
                //????????????????????????????????????????????????
                head->left = remove_node(head->left, x);
            }else if(x > head->key){
                //????????????????????????????????????????????????
                head->right = remove_node(head->right, x);
            }else{
                //???????????????????????????
                node<T> * r = head->right;

                //???????????????????????????
                if(head->right==nullptr){
                    //???????????????????????????
                    node<T> * l = head->left;
                    delete(head);

                    //????????????
                    head = l;
                }
                //?????????????????????????????????
                else if(head->left==nullptr){
                    //???????????????????????????????????????????????? r
                    delete(head);
                    head = r;
                }
                else{
                    //???????????????????????????
                    //???????????????????????????????????????????????????????????????
                    while(r->left!=nullptr) r = r->left;

                    //????????????
                    head->key = r->key;

                    //?????????????????????
                    head->right = remove_node(head->right, r->key);
                }
            }

            //???????????????????????????????????????????????????????????????
            if(head==nullptr) return head;

            //?????????????????????
            head->height = 1 + max(height(head->left), height(head->right));

            //??????balence factor
            int bal = height(head->left) - height(head->right);

            //??????????????????????????????????????????
            if(bal>1){
                if(x > head->left->key){
                    return rightRotation(head);
                }else{
                    head->left = leftRotation(head->left);
                    return rightRotation(head);
                }
            }else if(bal<-1){
                if(x < head->right->key){
                    return leftRotation(head);
                }else{
                    head->right = rightRotation(head->right);
                    return leftRotation(head);
                }
            }
            return head;
        }

        //?????????
        node<T> * search_node(node<T> * head, T x){
            //????????????????????????????????????
            if(head == nullptr) return nullptr;
            T k = head->key;
            if(k == x) return head;
            if(k > x) return search_node(head->left, x);
            if(k < x) return search_node(head->right, x);
        }
};
int main(){
    avl_tree<int> tr;
    int a;
    cin>>a;
    for(int i=a-1;i>=0;i--){
        tr.insert(i);
    }
    tr.inorder();
}
