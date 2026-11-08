#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <stdbool.h>

typedef struct BstNode {
    int data;
    struct BstNode* left;
    struct BstNode* right;
} BstNode;

void bst_free(BstNode* root) {
    if (root == NULL) return;

    bst_free(root->left);
    bst_free(root->right);

    free(root);
}

void bst_inorder_print(const BstNode* root) {
    if (root == NULL) return;

    bst_inorder_print(root->left);
    printf("%d ", root->data);
    bst_inorder_print(root->right);
}

void bst_preorder_print(const BstNode* root) {
    if (root == NULL) return;

    printf("%d ", root->data);
    bst_preorder_print(root->left);
    bst_preorder_print(root->right);
}

void bst_postorder_print(const BstNode* root) {
    if (root == NULL) return;

    bst_postorder_print(root->left);
    bst_postorder_print(root->right);
    printf("%d ", root->data);
}

BstNode* bst_search_node(BstNode* root, int value) {
    if (root == NULL || root->data == value) {
        return root;
    }

    if (value < root->data) return bst_search_node(root->left, value);

    return bst_search_node(root->right, value);
}

int bst_tree_height(const BstNode* root) {
    if (root == NULL) {
        return 0;
    }

    int left_count = bst_tree_height(root->left);
    int right_count = bst_tree_height(root->right);  

    return 1 + (left_count > right_count ? left_count : right_count);
}

BstNode* bst_find_min(BstNode* root) {
    if (root == NULL) return NULL;

    if (root->left == NULL) return root;

    return bst_find_min(root->left);
}

BstNode* bst_find_max(BstNode* root) {
    if (root == NULL) return NULL;

    if (root->right == NULL) return root;
    
    return bst_find_max(root->right);
}

BstNode* bst_create_node(int value) {
    BstNode* root = (BstNode*)malloc(sizeof(BstNode));
    if (root == NULL) {
        return NULL;
    }

    root->data = value;
    root->left = NULL;
    root->right = NULL;

    return root;
}

bool bst_insert_node(BstNode** ptr_root, int value) {
    if (ptr_root == NULL) return false;

    if (*ptr_root == NULL) { 
        BstNode* new_node = bst_create_node(value);
        if (new_node == NULL) return false;

        *ptr_root = new_node;
        return true;
    }

    else if (value < (*ptr_root)->data) return bst_insert_node(&(*ptr_root)->left, value);
    else if (value > (*ptr_root)->data) return bst_insert_node(&(*ptr_root)->right, value);
    else return false;
}

bool bst_validate(BstNode* root, long MIN_THRESHOLD, long MAX_THRESHOLD) {
    if (root == NULL) return true;
    
    if (root->data > MIN_THRESHOLD && root->data < MAX_THRESHOLD) {
        if (!bst_validate(root->left, MIN_THRESHOLD, root->data)) return false;
        if (!bst_validate(root->right, root->data, MAX_THRESHOLD)) return false;

        return true;
    }
    return false;
}

bool bst_delete_node(BstNode** ptr_root, int value) {
    if (ptr_root == NULL || *ptr_root == NULL) return false;

    if (value == (*ptr_root)->data) {
        if (!(*ptr_root)->left && !(*ptr_root)->right) {
            free(*ptr_root);
            *ptr_root = NULL;
            return true;
        }

        else if (!(*ptr_root)->left) {
            BstNode* child = (*ptr_root)->right;
            free(*ptr_root);
            *ptr_root = child;
            return true;
        }

        else if (!(*ptr_root)->right) {
            BstNode* child = (*ptr_root)->left;
            free(*ptr_root);
            *ptr_root = child;
            return true;
        }

        else if ((*ptr_root)->left && (*ptr_root)->right) {
            BstNode* min_right_node = bst_find_min((*ptr_root)->right);
            (*ptr_root)->data = min_right_node->data;
            return bst_delete_node(&(*ptr_root)->right, min_right_node->data);
        }
    }

    else if (value < (*ptr_root)->data) return bst_delete_node(&(*ptr_root)->left, value);
    else if (value > (*ptr_root)->data) return bst_delete_node(&(*ptr_root)->right, value);
    return false;
}

int main(void) {
    BstNode* tree = NULL;

    //Создаём узлы, возвращая указатели на них для передачи их в качестве аргумента в функцию по вставке узлов в bst
    bst_insert_node(&tree, 10);
    bst_insert_node(&tree, 5);
    bst_insert_node(&tree, 3);
    bst_insert_node(&tree, 15);
    //Пробуем создать узел со значением-дубликатом, вставки дубликата закономерно не происходит
    bst_insert_node(&tree, 15);
    bst_insert_node(&tree, 12);

    //Вывод числовых значений узлов в формате left-root-right (от наименьшего к наибольшему значению)
    bst_inorder_print(tree);
    printf("\n");

    //Высота дерева (уровень максимального углубления в дереве)
    printf("Высота дерева: %d\n", bst_tree_height(tree));

    //Минимальное и максимальное значение узла в дереве соответственно
    printf("Минимальное и максимальное значение в дереве: %d %d\n", bst_find_min(tree)->data, bst_find_max(tree)->data);

    //Удаляем узлы дерева по числовым значениям
    printf("Удаление узла со значением %d: %d\n", 15, bst_delete_node(&tree, 15));
    bst_inorder_print(tree);
    printf("\n");
    
    printf("Удаление узла со значением %d: %d\n", 10, bst_delete_node(&tree, 10));
    bst_inorder_print(tree);
    printf("\n");

    //Удаляем несуществующий узел
    printf("Удаление несуществующего узла со значением %d: %d\n", 10, bst_delete_node(&tree, 10));
    bst_inorder_print(tree);
    printf("\n");

    //Валидация bst (true)
    printf("Валидация bst (успешная): %d", bst_validate(tree, LONG_MIN, LONG_MAX));
    printf("\n");

    //Валидация bst (false)
    //Вручную подменяем валидное корневого узла на невалидное значение, не соответствующее инварианту (root->data < left->data при инварианте root->data > left->data)
    BstNode* node = bst_search_node(tree, 12);
    node->data = 4;
    printf("Валидация bst (с заведомо ложным исходом): %d\n", bst_validate(tree, LONG_MIN, LONG_MAX));

    bst_free(tree);
    tree = NULL;
}