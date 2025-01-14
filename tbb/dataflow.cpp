#include <iostream>
#include <tbb/tbb.h>

class src_body {
  const int my_limit;
  int my_next_value;
public:
  src_body(int l) : my_limit(l), my_next_value(1) {}
  bool operator()( int &v ) {
    if ( my_next_value <= my_limit ) {
      v = my_next_value++;
      return true;
    } else {
      return false;
    }
  }
};

int main() {
  int sum = 0;
  tbb::flow::graph g;

  /*
  function_node< int, int > squarer( g, unlimited, [](const int &v) { 
      return v*v; 
    } );
  function_node< int, int > cuber( g, unlimited, [](const int &v) { 
      return v*v*v; 
    } );
  function_node< int, int > summer( g, 1, [&](const int &v ) -> int { 
      return sum += v; 
    } );
  make_edge( squarer, summer );
  make_edge( cuber, summer );
  source_node< int > src( g, src_body(10), false );
  make_edge( src, squarer );
  make_edge( src, cuber );
  src.activate();
  g.wait_for_all();
  cout << "Sum is " << sum << "\n";
  */
}
