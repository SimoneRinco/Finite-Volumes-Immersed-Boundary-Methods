from mpl_toolkits.mplot3d import Axes3D
import matplotlib.pyplot as plt
import numpy as np
import math
import os.path
import argparse
import csv

def main(data_dir, frame):

  # read unknowns
  h, qx, qy = [], [], []
  with open(os.path.join(data_dir, 'unknowns', 'frame_' + str(frame))) as f:
    r = csv.reader(f, delimiter=' ')
    for line in r:
      h.append(float(line[0]))
      qx.append(float(line[1]))
      qy.append(float(line[2]))

  # read wet_cells (x and y)
  x, y = [], []
  with open(os.path.join(data_dir, 'domain', 'wet_cells')) as f:
    r = csv.reader(f, delimiter=' ')
    for line in r:
      x.append(float(line[0]))
      y.append(float(line[1]))

  fig = plt.figure(figsize=plt.figaspect(0.5))

  # plot the height
  ax1 = fig.add_subplot(1, 2, 1, projection='3d')
  ax1.set_zlim((0, 2))
  surf = ax1.plot_trisurf(x, y, h)

  # plot qx, qy
  ax2 = fig.add_subplot(1, 2, 2)

  print(len(x))
  print(len(y))
  print(len(qx))
  print(len(qy))
  ax2.quiver(x, y, qx, qy)

  plt.show()

if __name__ == '__main__':
  parser = argparse.ArgumentParser(description="Plot simulation results")
  parser.add_argument(
    'data_dir',
    help='The directory containing the data',
    nargs=1
    )

  parser.add_argument(
    '-f',
    help='The frame',
    type=int,
    default=0
    )


  args = parser.parse_args()
  main(args.data_dir[0], args.f)
