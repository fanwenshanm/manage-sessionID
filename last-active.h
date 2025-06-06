#include <iostream>
#include <string>

namespace last_active {
enum Color { RED, BLACK };

struct node {
    long long Time;
    int user;
    node *left, *right, *parent;
    Color color;
    node(long long Time = 0, int user = 0)
        : Time(Time),
          user(user),
          left(nullptr),
          right(nullptr),
          parent(nullptr) {
        color = RED;
    }
}* root;

void rotateLeft(node*& temp) {
    node* child = temp->right;
    temp->right = child->left;
    if (temp->right != nullptr) temp->right->parent = temp;
    child->parent = temp->parent;
    if (temp->parent == nullptr)
        root = child;
    else if (temp == temp->parent->left)
        temp->parent->left = child;
    else
        temp->parent->right = child;
    child->left = temp;
    temp->parent = child;
}

void rotateRight(node*& temp) {
    node* child = temp->left;
    temp->left = child->right;
    if (temp->left != nullptr) temp->left->parent = temp;
    child->parent = temp->parent;
    if (temp->parent == nullptr)
        root = child;
    else if (temp == temp->parent->left)
        temp->parent->left = child;
    else
        temp->parent->right = child;
    child->right = temp;
    temp->parent = child;
}

void fixInsert(node*& temp) {
    node* parent = nullptr;
    node* grandparent = nullptr;
    while (temp != root && temp->color == RED && temp->parent != nullptr &&
           temp->parent->color == RED) {
        parent = temp->parent;
        grandparent = parent->parent;
        if (grandparent == nullptr) break;  // Ensure grandparent is not null
        if (parent == grandparent->left) {
            node* uncle = grandparent->right;
            if (uncle != nullptr && uncle->color == RED) {
                grandparent->color = RED;
                parent->color = BLACK;
                uncle->color = BLACK;
                temp = grandparent;
            } else {
                if (temp == parent->right) {
                    rotateLeft(parent);
                    temp = parent;
                    parent = temp->parent;
                }
                rotateRight(grandparent);
                std::swap(parent->color, grandparent->color);
                temp = parent;
            }
        } else {
            node* uncle = grandparent->left;
            if (uncle != nullptr && uncle->color == RED) {
                grandparent->color = RED;
                parent->color = BLACK;
                uncle->color = BLACK;
                temp = grandparent;
            } else {
                if (temp == parent->left) {
                    rotateRight(parent);
                    temp = parent;
                    parent = temp->parent;
                }
                rotateLeft(grandparent);
                std::swap(parent->color, grandparent->color);
                temp = parent;
            }
        }
    }
    if (root != nullptr) root->color = BLACK;  // Ensure root is not null
}

// void fixDelete(node* &temp) {
//     while (temp != root && temp != nullptr && temp->color == BLACK) {
//         if (temp->parent != nullptr && temp == temp->parent->left) {
//             node* sibling = temp->parent->right;
//             if (sibling != nullptr && sibling->color == RED) {
//                 sibling->color = BLACK;
//                 temp->parent->color = RED;
//                 rotateLeft(temp->parent);
//                 sibling = temp->parent->right;
//             }
//             if ((sibling == nullptr) ||
//                 ((sibling->left == nullptr || sibling->left->color == BLACK)
//                 &&
//                  (sibling->right == nullptr || sibling->right->color ==
//                  BLACK))) {
//                 sibling->color = RED;
//                 temp = temp->parent;
//             } else {
//                 if (sibling->right == nullptr ||
//                     sibling->right->color == BLACK) {
//                     if (sibling->left != nullptr)
//                         sibling->left->color = BLACK;
//                     sibling->color = RED;
//                     rotateRight(sibling);
//                     sibling = temp->parent->right;
//                 }
//                 sibling->color = temp->parent->color;
//                 if (sibling != nullptr) {
//                     sibling->color = temp->parent->color;
//                     temp->parent->color = BLACK;
//                     if (sibling->right != nullptr)
//                         sibling->right->color = BLACK;
//                 }
//                 temp = root;
//             }
//         } else {
//             node* sibling = temp->parent->left;
//             if (sibling->color == RED) {
//                 sibling->color = BLACK;
//                 temp->parent->color = RED;
//                 rotateRight(temp->parent);
//                 sibling = temp->parent->left;
//             }
//             if ((sibling->left == nullptr ||
//                     sibling->left->color == BLACK) &&
//                 (sibling->right == nullptr ||
//                     sibling->right->color == BLACK)) {
//                 sibling->color = RED;
//                 temp = temp->parent;
//             } else {
//                 if (sibling->left == nullptr ||
//                     sibling->left->color == BLACK) {
//                     if (sibling->right != nullptr)
//                         sibling->right->color = BLACK;
//                     sibling->color = RED;
//                     rotateLeft(sibling);
//                     sibling = temp->parent->left;
//                 }
//                 sibling->color = temp->parent->color;
//                 temp->parent->color = BLACK;
//                 if (sibling->left != nullptr) sibling->left->color = BLACK;
//                 rotateRight(temp->parent);
//                 temp = root;
//             }
//         }
//     }
//     temp->color = BLACK;
// }
void fixDelete(node*& x) {
    while (x != root && (!x || x->color == BLACK)) {
        if (x == x->parent->left) {
            node* w = x->parent->right;
            if (w && w->color == RED) {
                w->color = BLACK;
                x->parent->color = RED;
                rotateLeft(x->parent);
                w = x->parent->right;
            }
            if ((!w->left || w->left->color == BLACK) &&
                (!w->right || w->right->color == BLACK)) {
                w->color = RED;
                x = x->parent;
            } else {
                if (!w->right || w->right->color == BLACK) {
                    if (w->left) w->left->color = BLACK;
                    w->color = RED;
                    rotateRight(w);
                    w = x->parent->right;
                }
                w->color = x->parent->color;
                x->parent->color = BLACK;
                if (w->right) w->right->color = BLACK;
                rotateLeft(x->parent);
                x = root;
            }
        } else {
            node* w = x->parent->left;
            if (w && w->color == RED) {
                w->color = BLACK;
                x->parent->color = RED;
                rotateRight(x->parent);
                w = x->parent->left;
            }
            if ((!w->right || w->right->color == BLACK) &&
                (!w->left || w->left->color == BLACK)) {
                w->color = RED;
                x = x->parent;
            } else {
                if (!w->left || w->left->color == BLACK) {
                    if (w->right) w->right->color = BLACK;
                    w->color = RED;
                    rotateLeft(w);
                    w = x->parent->left;
                }
                w->color = x->parent->color;
                x->parent->color = BLACK;
                if (w->left) w->left->color = BLACK;
                rotateRight(x->parent);
                x = root;
            }
        }
    }
    if (x) x->color = BLACK;
}

void insert(node* temp) {
    node* parent = nullptr;
    node* current = root;
    while (current != nullptr) {
        parent = current;
        if (temp->Time < current->Time)
            current = current->left;
        else
            current = current->right;
    }
    temp->parent = parent;
    if (parent == nullptr)
        root = temp;
    else if (temp->Time < parent->Time)
        parent->left = temp;
    else
        parent->right = temp;
    fixInsert(temp);
}

node* minValueNode(node*& temp) {
    node* current = temp;
    while (current->left != nullptr) current = current->left;
    return current;
}

// void transplant(node*& root, node*& u, node*& v) {
//     if (u->parent == nullptr)
//         root = v;
//     else if (u == u->parent->left)
//         u->parent->left = v;
//     else
//         u->parent->right = v;
//     if (v != nullptr) v->parent = u->parent;
// }

void transplant(node* u, node* v) {
    if (!u->parent)
        root = v;
    else if (u == u->parent->left)
        u->parent->left = v;
    else
        u->parent->right = v;
    if (v) v->parent = u->parent;
}

// void remove(long long key) {
//     node* temp = root;
//     node* z = nullptr;
//     node* x = nullptr;
//     node* y = nullptr;
//     while (temp != nullptr) {
//         if (temp->Time == key) {
//             z = temp;
//         }

//         if (temp->Time <= key) {
//             temp = temp->right;
//         } else {
//             temp = temp->left;
//         }
//     }

//     if (z == nullptr) {
//         // std::cout << "User not found !" << std::endl;
//         return;
//     }

//     y = z;
//     Color yOriginalColor = y->color;
//     if (z->left == nullptr) {
//         x = z->right;
//         transplant(root, z, z->right);
//     } else if (z->right == nullptr) {
//         x = z->left;
//         transplant(root, z, z->left);
//     } else {
//         y = minValueNode(z->right);
//         yOriginalColor = y->color;
//         x = y->right;
//         if (y->parent == z) {
//             if (x != nullptr) x->parent = y;
//         } else {
//             transplant(root, y, y->right);
//             y->right = z->right;
//             y->right->parent = y;
//         }
//         transplant(root, z, y);
//         y->left = z->left;
//         y->left->parent = y;
//         y->color = z->color;
//     }
//     delete z;
//     if (yOriginalColor == BLACK) {
//         fixDelete(x);
//     }
// }

void remove(int key) {
    node* z = root;
    while (z && z->user != key) {
        if (key < z->user)
            z = z->left;
        else
            z = z->right;
    }

    if (!z) return;  // key not found

    node* y = z;
    node* x = nullptr;
    bool yOriginalColor = y->color;

    if (!z->left) {
        x = z->right;
        transplant(z, z->right);
    } else if (!z->right) {
        x = z->left;
        transplant(z, z->left);
    } else {
        y = minValueNode(z->right);
        yOriginalColor = y->color;
        x = y->right;
        if (y->parent == z) {
            if (x) x->parent = y;
        } else {
            transplant(y, y->right);
            y->right = z->right;
            if (y->right) y->right->parent = y;
        }
        transplant(z, y);
        y->left = z->left;
        if (y->left) y->left->parent = y;
        y->color = z->color;
    }

    delete z;
    if (yOriginalColor == BLACK) fixDelete(x);
}

bool find(node*& temp, long long key) {
    node* current = root;
    while (current != nullptr) {
        if (current->Time == key) {
            return temp = current, true;
        } else if (current->Time < key) {
            current = current->right;
        } else {
            current = current->left;
        }
    }
    return false;
}

void inorderHelper(node* root) {
    if (root == nullptr) return;

    inorderHelper(root->left);
    std::cout << root->Time << " ";
    inorderHelper(root->right);
}
void inorder() { inorderHelper(root); }
};  // namespace last_active
