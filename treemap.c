#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "treemap.h"

typedef struct TreeNode TreeNode;


struct TreeNode {
    Pair* pair;
    TreeNode * left;
    TreeNode * right;
    TreeNode * parent;
};

struct TreeMap {
    TreeNode * root;
    TreeNode * current;
    int (*lower_than) (void* key1, void* key2);
};

int is_equal(TreeMap* tree, void* key1, void* key2){
    if(tree->lower_than(key1,key2)==0 &&  
        tree->lower_than(key2,key1)==0) return 1;
    else return 0;
}


TreeNode * createTreeNode(void* key, void * value) {
    TreeNode * new = (TreeNode *)malloc(sizeof(TreeNode));
    if (new == NULL) return NULL;
    new->pair = (Pair *)malloc(sizeof(Pair));
    new->pair->key = key;
    new->pair->value = value;
    new->parent = new->left = new->right = NULL;
    return new;
}

TreeMap * createTreeMap(int (*lower_than) (void* key1, void* key2)) {
    TreeMap * new = (TreeMap *)malloc(sizeof(TreeMap));
    new->root = NULL;
    new->current = NULL;
    new->lower_than = lower_than;
    return new;
}


void insertTreeMap(TreeMap * tree, void* key, void * value) {
    TreeNode * new = createTreeNode(key, value);
    if (tree->root == NULL)
    {
        tree->root = new;
        tree->current = new;
        return;
    }
    TreeNode * aux = tree->root;
    while (aux != NULL)
        {
            if (is_equal(tree, key, aux->pair->key))
            {
                return;
            }
            if (tree->lower_than(key, aux->pair->key))
            {
                if (aux->left == NULL)
                {
                    aux->left = new;
                    new->parent = aux;
                    tree->current = new;
                    return;
                }
                else
                {
                    aux = aux->left;
                }
            }
            else
            {
                if (aux->right == NULL)
                {
                    aux->right = new;
                    new->parent = aux;
                    tree->current = new;
                    return;
                }
                else
                {
                    aux = aux->right;
                }
            }
        }
}

TreeNode * minimum(TreeNode * x){
    while (x->left != NULL)
    {
        x = x->left;
    }
    return x;
}

/*5.- Implemente la función void removeNode(TreeMap * tree, TreeNode* node). Esta función elimina el nodo node del árbol tree. Recuerde que para eliminar un node existen 3 casos: Nodo sin hijos: Se anula el puntero del padre que apuntaba al nodo Nodo con un hijo: El padre del nodo pasa a ser padre de su hijo Nodo con dos hijos: Descienda al hijo derecho y obtenga el menor nodo del subárbol (con la función minimum). Reemplace los datos (key,value) de node con los del nodo "minimum". Elimine el nodo minimum (para hacerlo puede usar la misma función removeNode)*/
void removeNode(TreeMap* tree, TreeNode* node) {
    // Caso 1: Nodo sin hijos
    if (node->left == NULL && node->right == NULL) {
        if (node->parent->left == node) {
            node->parent->left = NULL;
        } else {
            node->parent->right = NULL;
        }
        free(node);
        return;
    }

    // Caso 2: Nodo con un hijo
    if (node->left == NULL || node->right == NULL) {
        TreeNode* child = (node->left != NULL) ? node->left : node->right;
        if (node->parent->left == node) {
            node->parent->left = child;
        } else {
            node->parent->right = child;
        }
        child->parent = node->parent;
        free(node);
        return;
    }

    // Caso 3: Nodo con dos hijos
    TreeNode* minNode = minimum(node->right);
    node->pair->key = minNode->pair->key;
    node->pair->value = minNode->pair->value;
    removeNode(tree, minNode);
}

void eraseTreeMap(TreeMap * tree, void* key){
    if (tree == NULL || tree->root == NULL) return;

    if (searchTreeMap(tree, key) == NULL) return;
    TreeNode* node = tree->current;
    removeNode(tree, node);

}

Pair * searchTreeMap(TreeMap * tree, void* key) {
    TreeNode * aux = tree->root;
    while (aux != NULL)
        {
            if (is_equal(tree, key, aux->pair->key))
            {
                tree->current = aux;
                return aux->pair;
            }
            if (tree->lower_than(key, aux->pair->key))
            {
                aux = aux->left;
            }
            else
            {
                aux = aux->right;
            }
        }
    return NULL;
}


Pair * upperBound(TreeMap * tree, void* key) {
    TreeNode * aux = tree->root;
    TreeNode * ub_node = NULL;
    while (aux != NULL)
        {
            if (is_equal(tree, key, aux->pair->key))
            {
                tree->current = aux;
                return aux->pair;
            }
            if (tree->lower_than(key, aux->pair->key))
            {
                ub_node = aux;
                aux = aux->left;
            }
            else
            {
                aux = aux->right;
            }
            
        }
    if (ub_node != NULL)
    {
        return ub_node->pair;
    }
    return NULL;
}


Pair * firstTreeMap(TreeMap * tree) {
    TreeNode* node = tree->root;
    while (node->left != NULL) {
        node = node->left;
    }
    tree->current = node;
    return node->pair;
    }


Pair* nextTreeMap(TreeMap* tree) {
    if (tree->current == NULL) {
        return NULL;
    }

    TreeNode* node = tree->current;

    // Si hay un nodo derecho, el siguiente nodo será el más a la izquierda de ese subárbol derecho
    if (node->right != NULL) {
        node = node->right;
        while (node->left != NULL) {
            node = node->left;
        }
        tree->current = node;
        return node->pair;
    }

    // Si no hay un nodo derecho, el siguiente nodo será el primer ancestro que sea un hijo izquierdo
    TreeNode* parent = node->parent;
    while (parent != NULL && parent->right == node) {
        node = parent;
        parent = parent->parent;
    }

    // Si no hay tal ancestro, hemos llegado al final del árbol
    if (parent == NULL) {
        tree->current = NULL;
        return NULL;
    }

    tree->current = parent;
    return parent->pair;
}