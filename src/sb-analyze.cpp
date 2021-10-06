#include "disjoint_set.hpp"
#include <iostream>
using namespace std;
using plank::Disjoint_Set;

void analyze(Superball *s,Disjoint_Set *djs,int y,int x);

int main()
{
  Superball *s;
  s = new Superball(argc, argv);
  Disjoint_Set *djs;
  djs = new Disjoint_Set(s->board.size());
  for(int i = 0; i< s->r; i++) {
    for(int j = 0; j< s->c; j++) {
      if (s->goals[i*s->c+j] == 1 && djs->Find(i*s->c+j) == i*s->c+j)
        analyze(s,djs,i,j);
    }
  }
  djs->Print();
}

void analyze(Superball *s,Disjoint_Set *djs,int y,int x) {
  
  if(y!=0 && djs->Find(y*(s->c-1)+x) != djs->Find(y*s->c+x) && s->board[y*(s->c-1)+x] == s->board[y*s->c+x]) {
    djs->Union(djs->Find(y*s->c+x),djs->Find(y*(s->c-1)+x));
    analyze(s,djs,y-1,x);
  }
  if(x!=0 && djs->Find(y*(s->c)+x-1) != djs->Find(y*s->c+x) && s->board[y*(s->c)+x-1] == s->board[y*s->c+x]) {
    djs->Union(djs->Find(y*s->c+x),djs->Find(y*(s->c)+x-1));
    analyze(s,djs,y,x-1);
  }
  if(y!=s->r-1 && djs->Find(y*(s->c+1)+x) != djs->Find(y*s->c+x) && s->board[y*(s->c+1)+x] == s->board[y*s->c+x]) {
    djs->Union(djs->Find(y*s->c+x),djs->Find(y*(s->c+1)+x));
    analyze(s,djs,y+1,x);
  }
  if(x!=s->c-1 && djs->Find(y*(s->c)+x+1) != djs->Find(y*s->c+x) && s->board[y*(s->c)+x+1] == s->board[y*s->c+x]) {
    djs->Union(djs->Find(y*s->c+x),djs->Find(y*(s->c)+x+1));
    analyze(s,djs,y,x+1);
  }
  return;
}