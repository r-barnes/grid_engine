#!/usr/bin/python

# hexmap.py
# Draws one group of hexes.

from Tkinter import *

def frame(root, side):
  w = Frame(root)
  w.pack(side=side, expand=YES, fill=BOTH)
  return w

class HexDemo(Frame):
  def __init__(self):
	Frame.__init__(self)
	self.pack(expand=YES, fill=BOTH)
	self.master.title('Hex Drawing Demo')
	self.master.iconname('hexdraw')

	canvas = Canvas()
	canvas.pack(expand=YES, fill=BOTH)
	self.canvas = canvas;

	# These are the three in the middle
	self.DrawHex('one', (50, 10), 40)
	self.DrawHex('two', (50, 50), 40)
	self.DrawHex('thr', (50, 90), 40)

	# These are the two on the left
	self.DrawHex('fou', (20, 30), 40)
	self.DrawHex('fiv', (80, 30), 40)

	# And, these are the two on the right
	self.DrawHex('six', (20, 70), 40)
	self.DrawHex('sev', (80, 70), 40)


  def DrawHex(self, name, origin, size):
    oneseg = size/4
    twoseg = oneseg*2	# same as size/2

    # Get the x and y from the tuple
    orgx = origin[0]
    orgy = origin[1]

    # Delete an old one
    try:
        self.canvas.delete(name)
    except:
        pass

    # Create a new one
    self.hex = self.canvas.create_polygon( 
    orgx, orgy + twoseg,
    orgx + oneseg, orgy,
    orgx + oneseg + twoseg, orgy,
    orgx + size , orgy + twoseg,
    orgx + oneseg + twoseg, orgy + size,
    orgx + oneseg, orgy + size,
    orgx, orgy + twoseg,
    tags=name, width=1, fill="white", outline="black")

    pass

if __name__ == '__main__':

  HexDemo().mainloop()

  
