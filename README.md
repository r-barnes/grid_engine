grid_engine
===========
**Grid Engine is not (as of 02013-04-10) yet ready for public use.**

**grid_engine** is a class for flexibily working with different kinds of two-dimensional grids.

It can handle hexagonal, 4-connected, and 8-connected grids.

Any of these connectivities can be used in a toroidal fashion, such that the edges of the grid wrap around.

Road Map to Public Use
======================
 1. Fix toroidal wrappings (Done: 02013-04-10)
 2. Verify correctness of every function (Done: 02013-04-11)
 3. Develop a test suite (Done: 02013-04-11)
 4. Improved examples (Done: 02013-04-11)
 5. Improved documentation

Examples
========

    //Define a an integer grid type
    typedef grid_engine::grid_engine<int> gtype;

    //Define two 40x40 grids of integers
    gtype grid(40,40), grid2(40,40);

    //Set each cell in the grid to a random number
    for(gtype::parser i=grid.begin();i.good();++i)
      *i=rand();

    //Set each cell in the second grid to the sum of the D8 neighbours
    //at distance 1 from its corresponding cell in the first grid
    grid2.fill(0);
    for(gtype::parser i=grid2.begin();i.good();++i)
      for(gtype::nparser n=i.d8ring(1);n.good();++n)
        *i+=grid(n);

    //Set each cell in the second grid to the sum of the D8 neighbours
    //at distance 2 from its corresponding cell in the first grid
    grid2.fill(0);
    for(gtype::parser i=grid2.begin();i.good();++i)
      for(gtype::nparser n=i.d8ring(2);n.good();++n)
        *i+=grid(n);

    //Set each cell in the second grid to the sum of the D8 neighbours
    //at distances 2-3 (inclusive) from its corresponding cell in the first
    //grid
    grid2.fill(0);
    for(gtype::parser i=grid2.begin();i.good();++i)
      for(gtype::nparser n=i.d8ring(2,3);n.good();++n)
        *i+=grid(n);

    //Set each cell in the second grid to the sum of the D8 neighbours
    //at distances 1 from its corresponding cell in the first
    //grid, assuming that the grid is a toroid
    grid2.fill(0);
    for(gtype::parser i=grid2.begin();i.good();++i)
      for(gtype::nparser n=i.d8tring(1);n.good();++n)
        *i+=grid(n);

    //Set each cell in the second grid to the sum of the D4 neighbours
    //at distance 1 from its corresponding cell in the first grid
    grid2.fill(0);
    for(gtype::parser i=grid2.begin();i.good();++i)
      for(gtype::nparser n=i.d4ring(1);n.good();++n)
        *i+=grid(n);

    //Set each cell in the second grid to the sum of the hexagonal neighbours
    //at distance 1 from its corresponding cell in the first grid
    grid2.fill(0);
    for(gtype::parser i=grid2.begin();i.good();++i)
      for(gtype::nparser n=i.hexring(1);n.good();++n)
        *i+=grid(n);

Notes
=====
The hexagonal data is stored in a 2D rectangular grid which is internally mapped to a hexagonal space. This mapping shifts columns with odd x-coordinates down a half-cell with respect to columns with even x-coordinates. The user should not normally need to worry about this.
