#include <bits/stdc++.h>

using namespace std;

#define max_children 2
#define dimension 20
#define number_of_points 2000
#define INPUT_FILE "inputs.txt"

int splitcount = 0;

struct Point {
  double p[dimension];
};

struct Rectangle {
  Point rect[2]; // Coordinates of the rectangle corners
};

struct Node {
  bool is_leaf;
  vector<Point> points;
  vector<Node *> children;
  Rectangle rectangle;
};

void update_rectangle(Node* node, Point& p)
{
  for (int i = 0; i < dimension; i++) 
  {
    if (p.p[i] < node->rectangle.rect[0].p[i]) 
    {
      node->rectangle.rect[0].p[i] = p.p[i];
    } 
    else if (p.p[i] > node->rectangle.rect[1].p[i]) 
    {
      node->rectangle.rect[1].p[i] = p.p[i];
    }
  }
}


double calculateEnlargement(Node* node, Point& p) 
{
  // Calculate enlargement of the bounding box if rect is added to node

  double enlarged_area = 1, original_area = 1;

  for (int i = 0; i < dimension; i++) 
  {
      double max = node->rectangle.rect[1].p[i];
      double min = node->rectangle.rect[0].p[i];

      original_area *= (max - min);

      if (p.p[i] > max) 
      {
          max = p.p[i];
      } 
      else if (p.p[i] < min) 
      {
          min = p.p[i];
      }

      enlarged_area *= (max - min);
  }

  return enlarged_area - original_area;

}


void split_node (Node* node, Point& p)
{ 
  Point p1 = node->points[0];
  Point p2 = node->points[1];

  Node* node1 = new Node{true, {p1}, {}, {p1, p1}};
  Node* node2 = new Node{true, {p2}, {}, {p2, p2}};

  node->children = {node1, node2};
  node->is_leaf = false;

  splitcount++;

}


void insert_recursive(Point &p, Node *node) 
{
  if (node->is_leaf) 
  {
    if (node->points.size() < max_children) 
    {
      node->points.push_back(p);
      update_rectangle(node, p);
    } 
    else 
    {
      split_node(node, p);
      insert_recursive(p, node);    
    }
  } 
  else 
  {
    // Choose child node with minimum enlargement and continue insertion
    double min_enlargement = INFINITY;
    Node *best_child = nullptr;

    for (Node *child : node->children) 
    {
      double enlargement = calculateEnlargement(child, p);

      if (enlargement < min_enlargement) 
      {
        min_enlargement = enlargement;
        best_child = child;
      }
    }

    update_rectangle(node, p);
    
      insert_recursive(p, best_child);
  }
}


void insert(Point &p, Node*& root) 
{
  if (root == nullptr) 
  {
    root = new Node{true, {p}, {}, {p, p} };
  } 
  else 
  {
    insert_recursive(p, root);
  }
}


double eud_dist (Point &p1, Point &p2)
{
  double dist = 0;

  for (int i = 0; i < dimension; i++)
  {
    dist += pow (p1.p[i] - p2.p[i], 2);
  }

  return sqrt(dist);
}


double dist_from_rect (Point &p, Rectangle &r)
{
  double dist = 0;

  for (int i = 0; i < dimension; i++)
  {
    double max = r.rect[1].p[i];
    double min = r.rect[0].p[i];

    if (p.p[i] < min)
    {
      dist += pow (min - p.p[i], 2);
    }
    else if (p.p[i] > max)
    {
      dist += pow (p.p[i] - max, 2);
    }
  }

  return sqrt(dist);
}


Point nearest_neighbour (Node* node, Point& p)
{
  if (node->is_leaf)
  {
    Point p1 = node->points[0];
    Point p2 = node->points[1];

    if (eud_dist(p1, p) <= eud_dist(p2, p))
    {
      return p1;
    }
    else 
    {
      return p2;
    }
  }
  else
  {
    double min_dist = INT_MAX;
    Node* best_child = nullptr;

    for (Node* child : node->children)
    {
      double dist = dist_from_rect(p, child->rectangle);

      if (dist < min_dist)
      {
        min_dist = dist;
        best_child = child;
      }
    }

    return nearest_neighbour(best_child, p);
  }
}

// function to read points from input file
void readfile(Point data[number_of_points]) {
  ifstream input(INPUT_FILE);
  for (int i = 0; i < number_of_points; i++) {
    for (int j = 0; j < dimension; j++) {
      input >> data[i].p[j];
    }
  }
}

void printNumbers(Point data[number_of_points]) {
  for (int i = 0; i < number_of_points; i++) {
    for (int j = 0; j < dimension; j++) {
      cout << data[i].p[j] << " ";
    }
    cout << endl;
  }
};


Point bf (Point &p, Point points[])
{
  Point nearest_point;
  double min_dist = INT_MAX;

  for (int i = 0; i < number_of_points; i++)
  {
    double dist = eud_dist(p, points[i]);

    if (dist <= min_dist)
    {
      nearest_point = points[i];
      min_dist = dist;
    }
  }
  cout << min_dist << endl;
  return nearest_point;

}


int main() {
  Point data[number_of_points];
  readfile(data);
  // printNumbers(data);
  Node *root = nullptr; 
  Point query = {{-4, -20, -15, -20, 10, 18, 3, 12, -13, -1, -11, -9, -4, -9, 11, 20, 7, 19, -1, -19}};
//   Point query = {{10, 4, 15, 16, 17, 6, -6, -5, 7, -4, 17, -7, 11, -3, 8, -14, 11, -17, -10, 15 }};
//   Point query = {{16, 3, 16, 20, 17, -3, -6, 9, 10, 17, -15, 1, 6, 19, 12, 7, -4, -7, 1, 4}};
//   Point query = {{-16, 10, -15, -12, 1, -12, 6, 4, 4, 4, 1, -14, -4, -13, 14, -20, 7, -1, 6, -14 }};
//   Point query = {{17, -10, -1, -16, 0, -7, -9, -4, -3, -18, -19, 9, 12, -6, 16, 15, -16, -12, -4, -5 }};

  for (int i=0; i < 200; i++){
    insert(data[i], root);
  }

  Point nearest = nearest_neighbour(root, query);

  for (int i = 0; i < dimension; i++)
  {
    cout << nearest.p[i] << " ";
  }
  cout << endl;
  cout << eud_dist(nearest, query) << endl;

  Point nearestbf = bf(query, data);

  cout << "{";

  for (int i = 0; i < dimension; i++)
  {
    cout << nearestbf.p[i] << " ";
  }

  cout << "}\n";


  cout << endl;
}