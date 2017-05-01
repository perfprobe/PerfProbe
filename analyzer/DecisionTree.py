mport numpy as np
from sklearn import tree
import subprocess, os, sys
from sklearn.tree import DecisionTreeClassifier, export_graphviz

pfile = sys.argv[1]

A = np.loadtxt(pfile, delimiter=",")
Y = A[:, 0]
X = A[:, 1:]

clf = tree.DecisionTreeClassifier(max_depth=4) #criterion='entropy')
clf = clf.fit(X, Y)

with open("feature.meta", 'r') as f:
     lines = f.readlines()
     for line in lines:
         feature_names = line.split(',')

with open("dt2.dot", 'w') as f:
    export_graphviz(clf, out_file=f, feature_names=feature_names[1:], class_names=['Bad','Good'])
    command = ["dot", "-Tpng", "dt2.dot", "-o", "dt2.png"]
    try:
        subprocess.check_call(command)
    except:
        exit("Could not run dot, ie graphviz, to "
             "produce visualization")

import pygraphviz as pgv
from pygraphviz import *

G = pgv.AGraph("dt2.dot")
G.layout(prog='dot')
G.draw('dt2.png') #df')
