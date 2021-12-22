
/* ----------------- */
#include <stdio.h>
#include <assert.h>
/* ----------------- */
#include "rbtree.h"
#include <stdlib.h>
void left_rotate(rbtree *t, node_t *x)
{
  node_t *y = x->right;
  //x를 left 로테이션 하는 것이므로 x의 right 자리에 있는 노드가 현 x자리로 올것이다.
  x->right = y->left;

  if (y->left != t->nil)
  {
    y->left->parent = x;
  }
  y->parent = x->parent;
  if (x->parent == t->nil)
    t->root = y;
  else if (x == x->parent->left)
    x->parent->left = y;
  else
    x->parent->right = y;
  y->left = x;
  x->parent = y;
}

void right_rotate(rbtree *t, node_t *x)
{
  node_t *y = x->left;
  //x를 left 로테이션 하는 것이므로 x의 right 자리에 있는 노드가 현 x자리로 올것이다.
  x->left = y->right;

  if (y->right != t->nil)
  {
    y->right->parent = x;
  }
  y->parent = x->parent;
  if (x->parent == t->nil)
  {
    t->root = y;
  }
  else if (x == x->parent->right)
  {
    x->parent->right = y;
  }
  else
  {
    x->parent->left = y;
  }
  y->right = x;
  x->parent = y;
}


node_t *rb_insert_fixup(rbtree *t, node_t *z)
{
  node_t *y;
  // node_t *g = grandparent(t, z);
 
    while (z->parent->color == RBTREE_RED)
    {
      if (z->parent == z->parent->parent->left)
      {
        y = z->parent->parent->right;
        if (y->color == RBTREE_RED)
        {
          z->parent->color = RBTREE_BLACK;
          y->color = RBTREE_BLACK;
          z->parent->parent->color = RBTREE_RED;
          z = z->parent->parent;
        }
        else
        {
          if (z == z->parent->right)
          {
            z = z->parent;
            left_rotate(t, z);
          }
          z->parent->color = RBTREE_BLACK;
          z->parent->parent->color = RBTREE_RED;
          right_rotate(t, z->parent->parent);
        }
      }
      else
      {
        y = z->parent->parent->left;
        if (y->color == RBTREE_RED)
        {
          z->parent->color = RBTREE_BLACK;
          y->color = RBTREE_BLACK;
          z->parent->parent->color = RBTREE_RED;
          z = z->parent->parent;
        }
        else
        {
          if (z == z->parent->left)
          {
            z = z->parent;
            right_rotate(t, z);
          }
          z->parent->color = RBTREE_BLACK;
          z->parent->parent->color = RBTREE_RED;
          left_rotate(t, z->parent->parent);
        }
      }
    }
  t->root->color = RBTREE_BLACK;
  return t->root;
}

rbtree *new_rbtree(void)
{
  rbtree *p = (rbtree *)calloc(1, sizeof(rbtree));
  node_t *for_nil = (node_t *)calloc(1, sizeof(node_t));
  p->nil = for_nil;
  p->root = for_nil;
  for_nil->color = RBTREE_BLACK;
  return p;
}

void freenode(node_t *x, rbtree *t)
{
  if (x != t->nil)
  {
    freenode(x->left, t);
    freenode(x->right, t);
    free(x);
  }
}
void delete_rbtree(rbtree *t)
{
  // TODO: reclaim the tree nodes's memory
  if (t->root)
    freenode(t->root, t);
  free(t);
}

node_t *rbtree_insert(rbtree *t, const key_t key)
{

  // TODO: implement insert
  node_t *x = t->root;
  node_t *y = t->nil;
  node_t *z = (node_t *)calloc(1, sizeof(node_t));
  z->key = key;
  //1. 현재 RBtree의 Root 값이 존재 하지 않는다면?삽입값이 root가 된다

  while (x != t->nil)
  {
    y = x;
    if (z->key < x->key)
      x = x->left;
    else
      x = x->right;
  }
  //이곳을 빠져나왔다는 것은 마지막 노드의 주소를 y에게 넘기고 왔다는뜻
  z->parent = y;
  //새로 들어가는 노드인 z는 맨 마지막노드의 자식일테니 y를 부모로
  if (y == t->nil)
  {
    t->root = z;
  }
  //y가 비었다면?RB트리의 root 값은 새로 들어온 z가 된다.
  else if (z->key < y->key)
  {
    y->left = z;
  }
  else
    y->right = z;

  z->left = t->nil;
  z->right = t->nil;
  z->color = RBTREE_RED;
  //새로 삽입되는 노드의 칼라는 초기 RED다 .

  //삽입이후 부모의 색상 비교
  rb_insert_fixup(t, z);

  return t->root;
}
node_t *rbtree_find(const rbtree *t, const key_t key)
{
  // TODO: implement find
  node_t *x = t->root;
  while (x->key != key)
  {
    //찾고자 하는 key가 현재 key보다 작을때 왼쪽으로 이동
    if (x == t->nil)
      return NULL;
    if (key < x->key)
    {
      x = x->left;
    }
    else if (key > x->key)
    {
      x = x->right;
    }
  }
  return x;
}

node_t *rbtree_min(const rbtree *t)
{
  // TODO: implement find
  node_t *x = t->root;

  while (x->left != t->nil)
  {
    x = x->left;
  }
  return x;
}
node_t *node_min(rbtree *t, node_t *p)
{
  // TODO: implement find
  while (p->left != t->nil)
  {
    p = p->left;
  }
  return p;
}

node_t *rbtree_max(const rbtree *t)
{
  // TODO: implement find
  node_t *x = t->root;

  while (x->right != t->nil)
  {
    x = x->right;
  }

  return x;
}

//v는 옮겨질 노드 , u는 삭제될 노드
void transplant(rbtree *t, node_t *u, node_t *v)
{
  if (u->parent == t->nil)
  {
    t->root = v;
  }
  else if (u == u->parent->right)
  {
    u->parent->right = v;
  }
  else
  {
    u->parent->left = v;
  }
  v->parent = u->parent;
}
void delete_fixup(rbtree *t, node_t *x)
{
  while ((x != t->root) && (x->color == RBTREE_BLACK))
  {
    if (x == x->parent->left)
    {
      node_t *w = x->parent->right;
      if (w->color == RBTREE_RED)
      {
        w->color = RBTREE_BLACK;
        x->parent->color = RBTREE_RED;
        left_rotate(t, x->parent);
        w = x->parent->right;
      }
      if (w->left->color == RBTREE_BLACK && w->right->color == RBTREE_BLACK)
      {
        w->color = RBTREE_RED;
        x = x->parent;
      }
      else
      {
        if (w->right->color == RBTREE_BLACK)
        {
          w->left->color = RBTREE_BLACK;
          w->color = RBTREE_RED;
          right_rotate(t, w);
          w = x->parent->right;
        }
        w->color = x->parent->color;
        x->parent->color = RBTREE_BLACK;
        w->right->color = RBTREE_BLACK;
        left_rotate(t, x->parent);
        x = t->root;
      }
    }
    else
    {
      node_t *w = x->parent->left;
      if (w->color == RBTREE_RED)
      {
        w->color = RBTREE_BLACK;
        x->parent->color = RBTREE_RED;
        right_rotate(t, x->parent);
        w = x->parent->left;
      }
      if (w->right->color == RBTREE_BLACK && w->left->color == RBTREE_BLACK)
      {
        w->color = RBTREE_RED;
        x = x->parent;
      }
      else
      {
        if (w->left->color == RBTREE_BLACK)
        {
          w->right->color = RBTREE_BLACK;
          w->color = RBTREE_RED;
          left_rotate(t, w);
          w = x->parent->left;
        }
        w->color = x->parent->color;
        x->parent->color = RBTREE_BLACK;
        w->left->color = RBTREE_BLACK;
        right_rotate(t, x->parent);
        x = t->root;
      }
    }
  }
  x->color = RBTREE_BLACK;
}
//p 는 삭제될 노드
int rbtree_erase(rbtree *t, node_t *p)
{
  // TODO: implement erase
  node_t *x;
  node_t *y = p;
  int y_origin_color = y->color;
  if (p->left == t->nil)
  {
    x = p->right;
    transplant(t, p, p->right);
  }
  else if (p->right == t->nil)
  {
    x = p->left;
    transplant(t, p, p->left);
  }
  else
  {
    y = node_min(t, p->right);
    y_origin_color = y->color;
    x = y->right;
    if (y->parent == p)
    {
      x->parent = y;
    }
    else
    {
      transplant(t, y, y->right);
      y->right = p->right;
      y->right->parent = y;
    }
    transplant(t, p, y);
    y->left = p->left;
    y->left->parent = y;
    y->color = p->color;
  }
  if (y_origin_color == RBTREE_BLACK)
  {
    delete_fixup(t, x);
  }

  return 0;
}
void append(char *dst, char c)
{
  char *p = dst;
  while (*p != '\0')
    p++; // 문자열 끝 탐색
  *p = c;
  *(p + 1) = '\0';
}
key_t *inorder(const rbtree *t, node_t *root, key_t *res)
{

  if (root == t->nil)
    return res;
  res = inorder(t, root->left, res);
  // printf("key = %d color=%d \n", root->key,root->color);
  *res = root->key;
  res = res + 1;
  res = inorder(t, root->right, res);
  return res;
}

int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n)
{
  inorder(t, t->root, arr);
  return 0;
}

/* ------------------------------------------------------------- */
void test_find_erase(rbtree *t, const key_t *arr, const size_t n) {
  for (int i = 0; i < n; i++) {
    node_t *p = rbtree_insert(t, arr[i]);
    assert(p != NULL);
  }

  for (int i = 0; i < n; i++) {
    node_t *p = rbtree_find(t, arr[i]);
    // printf("arr[%d] = %d\n", i, arr[i]);
    assert(p != NULL);
    assert(p->key == arr[i]);
    rbtree_erase(t, p);
  }

  for (int i = 0; i < n; i++) {
    node_t *p = rbtree_find(t, arr[i]);
    assert(p == NULL);
  }

  for (int i = 0; i < n; i++) {
    node_t *p = rbtree_insert(t, arr[i]);
    assert(p != NULL);
    node_t *q = rbtree_find(t, arr[i]);
    assert(q != NULL);
    assert(q->key == arr[i]);
    assert(p == q);
    rbtree_erase(t, p);
    q = rbtree_find(t, arr[i]);
    assert(q == NULL);
  }
}

void test_find_erase_fixed() {
  const key_t arr[] = {10, 5, 8, 34, 67, 23, 156, 24, 2, 12, 24, 36, 990, 25};
  const size_t n = sizeof(arr) / sizeof(arr[0]);
  rbtree *t = new_rbtree();
  assert(t != NULL);

  test_find_erase(t, arr, n);

  delete_rbtree(t);
}

void test_find_erase_rand(const size_t n, const unsigned int seed) {
  srand(seed);
  rbtree *t = new_rbtree();
  key_t *arr = calloc(n, sizeof(key_t));
  for (int i = 0; i < n; i++) {
    arr[i] = rand();
  }

  test_find_erase(t, arr, n);

  free(arr);
  delete_rbtree(t);
}

int main(void){
  test_find_erase_rand(1000000, 17);
}