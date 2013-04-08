Examples
========

//Define a 40x40 grid of integers with each cell connected to 8 others
conn8_engine<int> grid(40,40);

//Define a 40x40 integer grid with 8 connectivity with all cells set to 4
conn8_engine<int> grid2(40,40,4);

//Set each cell in the grid to a random number
for(conn8_engine<int>::iterator i=grid.begin();i!=grid.end();++i)
  *i=rand();

//Set each cell in the grid to the average value of itself and its neighbours
for(conn8_engine<int>::iterator i=grid.begin();i!=grid.end();++i)
  for(conn8_engine<int>::niterator n=i.nbegin(1);n!=i.nend();++n)
    *i+=*n;

//Add to every cell the values of all neighbors within 5
for(conn8_engine<int>::iterator i=grid.begin();i!=grid.end();++i)
  for(conn8_engine<int>::niterator n=i.nbegin(1,5);n!=i.nend();++n)
    *i+=*n;

//Add to every cell the values of all neighbors at distance 5
for(conn8_engine<int>::iterator i=grid.begin();i!=grid.end();++i)
  for(conn8_engine<int>::niterator n=i.nbegin(5);n!=i.nend();++n)
    *i+=*n;

//Add to every cell the values of all neighbors at distances 3-5 (inclusive)
for(conn8_engine<int>::iterator i=grid.begin();i!=grid.end();++i)
  for(conn8_engine<int>::niterator n=i.nbegin(3,5);n!=i.nend();++n)
    *i+=*n;

//Add to every cell (3,6) the values of all of its neighbors at distances 3-5
for(conn8_engine<int>::niterator n=grid.nbegin(3,6,3,5);n!=grid.nend();++n)
    grid(3,6)+=*n;
