#!/usr/bin/python
# The shebang line makes is possible to run the program by pressing F5 (Vim)
# This requires that the file is set to executable (chmod +x <filename>),
# and that F5 has been mapped to the run command

import numpy as np
from matplotlib import pyplot as plt
from matplotlib import animation
from pylab import cm
# Can get numbers out of string
import re
import numpy as np

# Options
save_file = 'off'

filename = 'game_of_life.dat'
# Get the header values
with open(filename, 'r') as the_file:
    header = the_file.readline()
header = re.findall(r'\d+', header)

# The header is written from C++ as rows*t, here we only want the row
# for one time
rows = int(header[0])/int(header[2])
cols = int(header[1])
t = int(header[2])

# Read in the matrix
matrix = np.genfromtxt(filename)

# Reshape the matrix
matrix = matrix.reshape(t,rows,cols)


# Animation inspired by 
# http://stackoverflow.com/questions/10429556/animate-quadratic-grid-changes-matshow
# and Jake Vanderplas
# http://jakevdp.github.io/blog/2012/08/18/matplotlib-animation-tutorial/

def generate_data(i):
    current_matrix = matrix[i]
    return current_matrix 

def update(data):
    mat.set_data(data)
    return mat 

def data_gen():
    i=0
    while i < t:
        yield generate_data(i)
        i = i+1

fig, ax = plt.subplots()
mat = ax.matshow(generate_data(0), cmap=cm.gray_r)
ani = animation.FuncAnimation(fig, update, data_gen, interval=100,
                              save_count=t)

if save_file == 'off':
    plt.show()

if save_file != 'off':
    print("Now saving...")
    # Save the animation as an mp4.  This requires ffmpeg or mencoder to be
    # installed.  The extra_args ensure that the x264 codec is used, so that
    # the video can be embedded in html5.  You may need to adjust this for
    # your system: for more information, see
    # http://matplotlib.sourceforge.net/api/animation_api.html
    ani.save('game_of_life.mp4', extra_args=['-vcodec', 'libx264'])
    print("...saved as 'game_of_life.mp4'")
