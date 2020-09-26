#include <vector>
#include <iostream>

//template<typename T, int ORDER>
int ORDER;
class btree{
private:

    enum state_t {OVERFLOWW, UNDERFLOWW, B_OK};


    struct node {
        std::vector<int> data;
        std::vector<node*> children;
        size_t count{0};

        node()
                : data(ORDER+1), children(ORDER+2, nullptr)
        {

        }

        void insert_into(size_t index, const int& value) {
            size_t j = this->count;
            while (j > index) {
                children[j+1] = children[j];
                data[j] = data[j-1];
                j--;
            }
            children[j+1] = children[j];
            data[j] = value;
            this->count++;
        }

        void push_back(const int& value) {
            insert_into(this->count, value);
        }

        state_t insert(const int& value) {
            // binary_search
            size_t index = 0;
            while (this->data[index] < value  && index < this->count) {
                index += 1;
            }
            if (this->children[index] == nullptr) {
                // this is a leaf node
                this->insert_into(index, value);
            } else {
                auto state = this->children[index]->insert(value);
                if (state == state_t::OVERFLOWW) {
                    // split
                    this->split(index);
                }
            }
            return this->count > ORDER ? OVERFLOWW : B_OK;
        }

        void split(size_t position) {
            //todo , hacer que se cree copia abajo y arriba
            // leaf nodes / index nodes
            node* parent = this;
            node* ptr = this->children[position];

            // TODO: reuse ptr buffer
            node* child1 = new node();
            node* child2 = new node();
            size_t i = 0;
            for (; i < ptr->count / 2; i++) {
                child1->children[i] = ptr->children[i];
                child1->data[i] = ptr->data[i];
                child1->count++;
            }
            child1->children[i] = ptr->children[i];
            auto mid = i;
            i += 1;
            size_t j = 0;

            child2->children[j] = ptr->children[mid];
            child2->data[j] = ptr->data[mid];
            child2->count++;
            j++;

            for (; i < ptr->count; i++) {
                child2->children[j] = ptr->children[i];
                child2->data[j] = ptr->data[i];
                child2->count++;
                j++;
            }
            child2->children[j] = ptr->children[i];

            parent->insert_into(position, ptr->data[mid]);
            parent->children[position] = child1;
            parent->children[position + 1] = child2;
        }

        // Inorder traverse
        void traverse(){
            int idx;
            for(idx = 0; idx < count; ++idx){
                if(children[idx]){    // if It is internal node
                    children[idx]->traverse();      // recursive call
                }
                std::cout << data[idx] << " ";
            }
            // If it's internal node, print the rightmost position child
            if(children[idx]){
                children[idx]->traverse();
            }
        }

    };

public:
    btree(){

    }

    btree(int order) {
        ORDER = order;
    }

    void insert(const int& value) {
        auto state = root.insert(value);
        if (state == state_t::OVERFLOWW) {
            // split root node
            split_root();
        }
    }

    void print() {
        print(&root, 0);
        //std::cout << "________________________\n";
    }

    void print(node *ptr, int level) {
        //todo agregar linkeador al otro nodo
        if (ptr) {
            int i;
            for (i = ptr->count - 1; i >= 0; i--) {
                print(ptr->children[i + 1], level + 1);

                for (int k = 0; k < level; k++) {
                    std::cout << "    ";
                }
                std::cout << ptr->data[i] << "\n";
            }
            print(ptr->children[i + 1], level + 1);
        }
    }

    void inOrderPrint(){
        //cout << "** Traverse print BTree **\n";
        root.traverse();
    }

private:
    //
    void split_root() {
        //tdo  tmb que se cree  una copia al linked list

        node* ptr = &root;
        node* child1 = new node();
        node* child2 = new node();
        size_t i = 0;
        for (; i < ptr->count / 2; i++) {
            child1->children[i] = ptr->children[i];
            child1->data[i] = ptr->data[i];
            child1->count++;
        }
        child1->children[i] = ptr->children[i];
        auto mid = i;
        i += 1;
        size_t j = 0;

        child2->children[j] = ptr->children[mid];
        child2->data[j] = ptr->data[mid];
        child2->count++;
        j++;

        for (; i < ptr->count; i++) {
            child2->children[j] = ptr->children[i];
            child2->data[j] = ptr->data[i];
            child2->count++;
            j++;
        }
        child2->children[j] = ptr->children[i];

        ptr->data[0] = ptr->data[mid];
        ptr->children[0] = child1;
        ptr->children[1] = child2;
        ptr->count = 1;
    }

private:
    node root;
};

int main( )
{
    int numElements, order;
    std::cin >> numElements;
    std::cin >> ORDER;
    btree bt;

    int keyValue = 0;
    for (size_t i = 0; i < numElements; i++) {
        std::cin >> keyValue;
        bt.insert(keyValue);
    }

//    bt.insert(2);
//    bt.insert(4);
//    bt.insert(7);
//    bt.insert(10);
    //bt.print();
    bt.inOrderPrint();
    return 0;
}