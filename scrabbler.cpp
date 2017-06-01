#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <set>
#include <exception>


#define N 36
#define IGNORE -1
#define ERROR -47

using namespace std;


class Node
{
  Node* nodes[N];
  bool _isWord;
  string word;
  int translate(char ch);
public:
  string removeChars(string from, string what);
  Node();
  void add(string s);
  Node* find(string s);
  Node* getSubNode(char ch);
  Node* setSubNode(char ch);
  bool isWord(string s);
  vector<string> scrabble(string s);
  set<string> multiScrabble(string s);
};

class WordNode
{
  vector<WordNode*> nodes;
  bool _isComplete;
public:
  WordNode();
};

class NotSupsequenceException : public exception
{
  virtual const char* what() const throw()
  {
    return "Char not found!";
  }
};

int Node::translate(char ch)
{
  if(ch==' ' || ch == '.') return IGNORE;
  if(ch>='a' && ch<='z') return ch-'a';
  if(ch>='A' && ch<='Z') return ch-'A';
  if(ch>='0' && ch<='9') return 26+ch-'0';
  return ERROR;
}

Node::Node(): _isWord(false), word("")
{
  for(int i = 0;i<N;i++)
  {
    nodes[i] = NULL;
  }
}

Node* Node::getSubNode(char ch)
{
  int i = translate(ch);
  if(i==IGNORE) return this;
  if(i==ERROR) return NULL;
  return nodes[i];
}

Node* Node::setSubNode(char ch)
{
  int i = translate(ch);
  if(i==IGNORE) return this;
  if(i==ERROR) return NULL;
  if(nodes[i]==NULL)
    nodes[i] = new Node();
  return nodes[i];
}


Node* Node::find(string s)
{
  Node * cn = this;
  for(unsigned i = 0;i<s.size();i++)
  {
    cn = cn->getSubNode(s[i]);
    if(cn == NULL) return NULL;
  }
  return cn;
}

bool Node::isWord(string s)
{
  Node * n = find(s);
  if(n==NULL) return false;
  return n->_isWord;
}

void Node::add(string s)
{
  Node * cn = this;
  for(unsigned i = 0;i<s.size();i++)
  {
    cn = cn->setSubNode(s[i]);
    if(cn == NULL)
    {
      cerr << "Invalid char: '" << s[i] << "'" << endl;
      return;
    }    
  }
  cn->_isWord = true;
  cn->word = s;
}

vector<string> Node::scrabble(string s)
{
//   cerr << "D:Scrabble: " << s << endl;
  set<char> used;
  vector<string> v;
  for(unsigned i = 0;i<s.size();i++)
  {
    if(used.count(s[i])==0)
    {
      used.insert(s[i]);
      string ss = s;
      ss[i] = ss[ss.size()-1];
      ss.resize(ss.size()-1);
      Node * successor = getSubNode(s[i]);
      if(successor==NULL)
      {
//         cerr << "D:failed" << endl;
        continue;        
      }
//       cerr << "D:continue" << endl;
      vector<string> v2 = successor->scrabble(ss);
      v.insert(v.end(),v2.begin(),v2.end());
    }
  }

  if(s=="" && _isWord)
  //if(_isWord)
  {
    v.push_back(word);
  }
  return v;
}

string Node::removeChars(string from, string what)
{
  if(what.size()>from.size()) throw new NotSupsequenceException();
  for(int i = what.size()-1;i>=0;i--)
  {
    bool nasiel = false;
    cerr << "hladam: " << what[i] << endl;
    for(int j = from.size()-1;j>=0;j--)
    {
      if(from[j]==what[i])
      {
        from[j] = from[from.size()-1];
        if(from.size()>0) from.resize(from.size()-1);
        else cerr << "zla velkost" << endl;
        nasiel = true;
        break;
      }
    }
    if(!nasiel)
      throw new NotSupsequenceException();
  }
  return from;
}

set<string> Node::multiScrabble(string s)
{
//   cerr << "D:Scrabble: " << s << endl;
//   string givenString = s;
  set<char> used;
  set<string> v;
  for(unsigned i = 0;i<s.size();i++)
  {
    if(used.count(s[i])==0)
    {
      used.insert(s[i]);
      string ss = s;
      ss[i] = ss[ss.size()-1];
      ss.resize(ss.size()-1);
      Node * successor = getSubNode(s[i]);
      if(successor==NULL)
      {
//         cerr << "D:failed" << endl;
        continue;
      }
//       cerr << "D:continue" << endl;
      set<string> v2 = successor->multiScrabble(ss);
      v.insert(v2.begin(),v2.end());
    }
  }

  //if(s=="" && _isWord)
  if(_isWord)
  {    
//     set<string> sentences = multiScrabble(s);
    v.insert(word);    
  }
  return v;
}

WordNode::WordNode()
{
  _isComplete = false;
}


int main(int argc, char* argv[])
{
  if(argc<=1)
  {
    cerr << "Usage: scrabler <dictionary_file>" << endl;
    return -1;
  }
  
  ifstream ifs(argv[1]);
  Node * lexTree = new Node();
  string s = "";
  while(getline(ifs,s))
  {
    lexTree->add(s);
  }
  ifs.close();

  while(getline(cin,s))
  {
    //cout << lexTree->isWord(s) << endl;
    vector<string> v = lexTree->scrabble(s);
//     set<string> v = lexTree->multiScrabble(s);
    cout << "Found: " << v.size() << endl;
    for(typeof(v.begin()) it = v.begin();it!=v.end();it++)    
    {
      cout << *it << endl;
    }    
  }
}



