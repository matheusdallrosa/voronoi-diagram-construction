#ifndef index
#define index
/*
  Esta classe serve como índice para outros nodos 
  nas árvores do status e da fila de interseccão.
*/
template<class T> 
class Index{
  T idx;
public:
  int a,b; 
  Index(int _a,int _b){    
    a = _a, b = _b;  
  }
  Index(int _a,int _b,T _idx){
    idx = _idx;
    a = _a, b = _b;  
  }
  bool operator<(const Index<T> & o)const{
    return (a != o.a) ? (a < o.a) : (b < o.b);
  }
  T get_idx()const{
    return idx;
  }
};
#endif