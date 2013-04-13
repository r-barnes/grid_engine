#!/usr/bin/python
import pygame
import sys
import Queue
import time

def NF(y,x):
  if x%2==0:
    return y
  else:
    return -y

class GridDrawer():
  def __init__(self, width, height, enabled):
    self.enabled=enabled
    if not self.enabled:
      return
    pygame.init()
    self.window = pygame.display.set_mode((640, 480))
    self.size=12
    self.xoffset=200
    self.yoffset=200
    self.width=width
    self.height=height
    self.drawer=None
    self.colours={"white":(255,255,255),"red":(255,0,0),"blue":(0,0,255)}
    self.x0=0
    self.y0=0

  def Flip(self):
    if not self.enabled:
      return
    pygame.display.flip()

  def DrawCell(self, x, y, colour):
    if not self.enabled:
      return
    self.drawer(x,y, colour, 0)
    self.drawer(x,y, 'white', 1)

  def SetGrid(self, grid):
    if grid=='Hex':
      self.drawer=self.DrawHex
    elif grid=='D8':
      self.drawer=self.DrawSquare
    elif grid=='D4':
      self.drawer=self.DrawSquare
    else:
      print 'Unrecognised grid type in GridDrawer'
      sys.exit(-1)

  def DrawGrid(self):
    if not self.enabled:
      return
    self.DrawCell(self.x0,self.y0,'blue')
    for x in range(0,self.width):
      for y in range(0,self.height):
        self.drawer(x,y,"white",1)

  def Erase(self):
    if not self.enabled:
      return
    self.window.fill((0,0,0))
    self.DrawGrid()

  def DrawHex(self, x, y, colour, width):
    if not self.enabled:
      return
    oneseg = self.size/4
    twoseg = oneseg*2  # same as size/2
    orgy=y*self.size+(x%2)*self.size/2 + self.yoffset
    orgx=x*self.size*3/4.              + self.xoffset

    # Create a new one
    pygame.draw.polygon(self.window, self.colours[colour], [ 
    (orgx, orgy + twoseg),
    (orgx + oneseg, orgy),
    (orgx + oneseg + twoseg, orgy),
    (orgx + self.size , orgy + twoseg),
    (orgx + oneseg + twoseg, orgy + self.size),
    (orgx + oneseg, orgy + self.size),
    (orgx, orgy + twoseg)], width)

  def DrawSquare(self, x, y, colour, width):
    orgx=x*self.size+self.xoffset
    orgy=y*self.size+self.yoffset
    pygame.draw.rect(self.window, self.colours[colour], (orgx,orgy,self.size,self.size), width)

  def Step(self):
    if not self.enabled:
      return
    self.Flip()
    time.sleep(0.2)
    if self.ParseEvents()=='quit':
      sys.exit(-1)
    self.Erase()

  def Loop(self):
    if not self.enabled:
      return
    while True: 
     for event in pygame.event.get(): 
      if event.type == pygame.QUIT: 
        sys.exit(0)
      else: 
        print event

  def ParseEvents(self):
    if not self.enabled:
      return
    for event in pygame.event.get():
      if event.type == pygame.KEYUP:
        if event.key==27:
          return 'quit'

def NF(x,y):
  if x%2==0:
    return y
  else:
    return -y

def main():
  if len(sys.argv)!=4:
    print "%s <GRID TYPE> <NUMBER OF RINGS> <draw/nodraw>" % (sys.argv[0])
    sys.exit(-1)

  if not sys.argv[1] in ['HexEven','HexOdd', 'D8','D4']:
    print "Unrecognised type. Valid choices are: HexEven, HexOdd, D8, D4"
    sys.exit(-1)

  maxrings=int(sys.argv[2])

  if sys.argv[3]=='draw':
    h=GridDrawer(10,10,True)
  elif sys.argv[3]=='nodraw':
    h=GridDrawer(10,10,False)
  else:
    print "Please specify draw/nodraw"
    sys.exit(-1)

  x0=0
  y0=0

  if sys.argv[1]=='HexEven':
    h.SetGrid('Hex')
    nx=[ 0,-1,1,0,-1, 1]
    ny=[-1, 0,0,1,-1,-1]
    prefix="int Hex::"
  elif sys.argv[1]=='HexOdd':
    h.SetGrid('Hex')
    nx=[ 0,-1,1,0,-1, 1]
    ny=[-1, 0,0,1,-1,-1]
    prefix="int HexOdd::"
    x0=1
    h.x0=1
  elif sys.argv[1]=='D8':
    h.SetGrid('D8')
    nx=[-1, 0, 1,-1,1,-1,0,1]
    ny=[-1,-1,-1, 0,0, 1,1,1]
    prefix="int D8::"
  elif sys.argv[1]=='D4':
    h.SetGrid('D8')
    nx=[ 0,-1,1,0]
    ny=[-1, 0,0,1]
    prefix="int D4::"
  else:
    print 'Unrecognised grid type'
    sys.exit(-1)

  #Draw initial grid
  h.DrawGrid()
  h.Flip()

  #Create queue used to define edges of explored areas
  q=Queue.Queue()

  #Seed the search
  used=set([(x0,y0)])
  q.put((x0,y0))
  q.put(False) #Marker used to denote the end of a ring
  ring=0

  #Seed results
  begins=[0]
  xs=[]
  ys=[]

  while not q.empty():
    c=q.get()

    #This is the marker that denoted the end of the ring
    if not c:
      h.Step()
      q.put(False)
      ring+=1
      if ring==maxrings:
        break
      begins+=[len(xs)]

      continue

    h.DrawCell(c[0],c[1],"red")

    #Add cell's location to output
    xs+=[c[0]-x0]
    ys+=[c[1]-y0]

    for i in range(len(nx)):
      ox=c[0]+nx[i]
      if sys.argv[1]=='HexEven' or sys.argv[1]=='HexOdd':
        oy=c[1]+NF(c[0],ny[i])
      else:
        oy=c[1]+ny[i]
      if not (ox,oy) in used:
        used.add((ox,oy))
        q.put((ox,oy))

  print prefix+"rlen0="+str(len(begins))+";"
  print prefix+"nlen0="+str(len(xs))+";"
  print prefix+"begins0[]={"+','.join([str(i) for i in begins])+"};"
  print prefix+"dx0[]={"+','.join([str(i) for i in xs])+"};"
  print prefix+"dy0[]={"+','.join([str(i) for i in ys])+"};"

main()
