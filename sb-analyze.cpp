#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctype.h>
#include <iostream>
#include <vector>
#include "disjoint.h"
using namespace std;

#define talloc(type, num) (type *) malloc(sizeof(type)*(num))

class Superball {
  public:
    Superball(int argc, char **argv);
    int r;
    int c;
    int mss;
    int empty;
    vector <int> board;
    vector <int> goals;
    vector <int> colors;
};

void usage(const char *s) 
{
  fprintf(stderr, "usage: sb-play rows cols min-score-size colors\n");
  if (s != NULL) fprintf(stderr, "%s\n", s);
  exit(1);
}

Superball::Superball(int argc, char **argv)
{
  int i, j;
  string s;

  if (argc != 5) usage(NULL);

  if (sscanf(argv[1], "%d", &r) == 0 || r <= 0) usage("Bad rows");
  if (sscanf(argv[2], "%d", &c) == 0 || c <= 0) usage("Bad cols");
  if (sscanf(argv[3], "%d", &mss) == 0 || mss <= 0) usage("Bad min-score-size");

  colors.resize(256, 0);

  for (i = 0; i < strlen(argv[4]); i++) {
    if (!isalpha(argv[4][i])) usage("Colors must be distinct letters");
    if (!islower(argv[4][i])) usage("Colors must be lowercase letters");
    if (colors[argv[4][i]] != 0) usage("Duplicate color");
    colors[argv[4][i]] = 2+i;
    colors[toupper(argv[4][i])] = 2+i;
  }

  board.resize(r*c);
  goals.resize(r*c, 0);

  empty = 0;

  for (i = 0; i < r; i++) {
    if (!(cin >> s)) {
      fprintf(stderr, "Bad board: not enough rows on standard input\n");
      exit(1);
    }
    if (s.size() != c) {
      fprintf(stderr, "Bad board on row %d - wrong number of characters.\n", i);
      exit(1);
    }
    for (j = 0; j < c; j++) {
      if (s[j] != '*' && s[j] != '.' && colors[s[j]] == 0) {
        fprintf(stderr, "Bad board row %d - bad character %c.\n", i, s[j]);
        exit(1);
      }
      board[i*c+j] = s[j];
      if (board[i*c+j] == '.') empty++;
      if (board[i*c+j] == '*') empty++;
      if (isupper(board[i*c+j]) || board[i*c+j] == '*') {
        goals[i*c+j] = 1;
        board[i*c+j] = tolower(board[i*c+j]);
      }
    }
  }
}

void analyze(Superball *s,DisjointSetByRankWPC *djs,int y,int x);

int main(int argc, char **argv)
{
  Superball *s;
  s = new Superball(argc, argv);
  DisjointSetByRankWPC *djs;
  djs = new DisjointSetByRankWPC(s->board.size());
  for(int i = 0; i< s->r; i++) {
    for(int j = 0; j< s->c; j++) {
      if (s->goals[i*s->c+j] == 1 && djs->Find(i*s->c+j) == i*s->c+j)
        analyze(s,djs,i,j);
    }
  }
  djs->Print();

}

void analyze(Superball *s,DisjointSetByRankWPC *djs,int y,int x) {
  
  if(y!=0 && s->board[y*(s->c-1)+x] == s->board[y*s->c+x]) {
    djs->Union(s->board[y*s->c+x],s->board[y*(s->c-1)+x]);
    analyze(s,djs,y-1,x);
  }
  if(x!=0 && s->board[y*(s->c)+x-1] == s->board[y*s->c+x]) {
    djs->Union(s->board[y*s->c+x],s->board[y*(s->c)+x-1]);
    analyze(s,djs,y,x-1);
  }
  if(y!=s->r-1 && s->board[y*(s->c+1)+x] == s->board[y*s->c+x]) {
    djs->Union(s->board[y*s->c+x],s->board[y*(s->c+1)+x]);
    analyze(s,djs,y+1,x);
  }
  if(x!=s->c-1 && s->board[y*(s->c)+x+1] == s->board[y*s->c+x]) {
    djs->Union(s->board[y*s->c+x],s->board[y*(s->c)+x+1]);
    analyze(s,djs,y,x+1);
  }
  return;
}
