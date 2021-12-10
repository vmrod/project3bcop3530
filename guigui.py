import time
from tkinter import *
from tkinter import ttk
from ttkthemes import ThemedTk
from lexpy.trie import Trie
import matplotlib
matplotlib.use('TkAgg')
from numpy import arange, sin, pi
from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg, NavigationToolbar2Tk
from matplotlib.figure import Figure
import networkx as nx
import subprocess
import matplotlib.pyplot as plt
from itertools import tee, islice, chain
import threading

terms = []
names=[]

def previous_and_next(some_iterable):
    prevs, items, nexts = tee(some_iterable, 3)
    prevs = chain([None], prevs)
    nexts = chain(islice(nexts, 1, None), [None])
    return zip(prevs, items, nexts)

#loads from local csv
def loadVals():
    start = time.time()

    with open("names.txt") as file:
        temp = file.read().splitlines()
        for i in temp:
            terms.append((i.split(' ', 1)[1].lower() + ' ' + i.split(' ', 1)[0]))
            names.append(i.split(' ', 1)[1])
    terms.sort()
    trie.add_all(terms)
    end = time.time()
    print(end - start)



#kepress 1
def checkkey(event): 
    value = event.widget.get().lower()
    
    if value == '':
        temp = trie.search_with_prefix('a')[:100]
        data=[]
        for i in temp:
            splitted = i.rsplit(" ", 1)
            data.append(splitted[0])
    else:
        data=[]
        temp = trie.search_with_prefix(value)[:100]
        for i in temp:
            splitted = i.rsplit(" ", 1)
            data.append(splitted[0])
    update(data)

#keypress 2
def checkkey2(event): 
    value = event.widget.get().lower()
    
    if value == '':
        temp = trie.search_with_prefix('a')[:100]
        data=[]
        for i in temp:
            splitted = i.rsplit(" ", 1)
            data.append(splitted[0])
    else:
        data=[]
        temp = trie.search_with_prefix(value)[:100]
        for i in temp:
            splitted = i.rsplit(" ", 1)
            data.append(splitted[0])
    update2(data)

#update list 1
def update(data):
    lb.delete(0, 'end')
    for item in data:
        lb.insert('end', item)

#update list 2
def update2(data):
    lb2.delete(0, 'end')
    for item in data:
        lb2.insert('end', item)

#update entrybox 1
def change(event):
    e.delete(0,END)
    try:
        sel = lb.get(lb.curselection())
        global origin
        origin = trie.search_with_prefix(lb.get(lb.curselection()))[0]
    except:
        sel = ''
    e.insert(0,sel)

#update entrybox 2
def change2(event):
    e2.delete(0,END)
    try:
        sel = lb2.get(lb2.curselection())
        global destination
        destination = trie.search_with_prefix(lb2.get(lb2.curselection()))[0]
    except:
        sel = ''
    e2.insert(0,sel)

def intermediate():
    b1["state"] = "disabled"
    bar.config(mode='indeterminate', maximum=100, value=0)
    bar.start(5)
    t1=threading.Thread(target=searchwiki)
    t1.start()

#collect origin and destination and print to screen
def searchwiki():
    print(origin)
    print(destination)
    p1 = origin.rsplit(' ', 1)[1]
    p2 =destination.rsplit(' ', 1)[1]
    print(p1 + ' ' + p2)

    subprocess.run("g++ graph.cpp -o graph", shell = True)
    print("CPP COMPILED!")
    subprocess.call("graph " + p1 + " " + p2, shell=True)
    print("FILE UPDATED!")

    #parse interface text file
    with open("interface.txt") as file:
        myline = file.readline()
    
    full = myline.split("|")
    times = full[0].split(' ')
    G.clear()
    BFS = times[0]
    DFS = times[1]
    
    #node coloring
    color_map = []

    path = full[1].split(' ')
    path.pop()


        
    for previous, item, nxt in previous_and_next(path):
        if nxt == None:
            break
        G.add_edge(names[int(item)], names[int(nxt)],color='r',weight=4)
        
    count = 0
    full.pop()
    for i in full[2:]:
        adjs = i.split(' ')
        for j in range(3):
            G.add_edge(names[int(path[count])], names[int(adjs[j])],color='b',weight=1)
        count += 1

    f.clear()
    a = f.add_subplot(111)
    pos=nx.spring_layout(G, k=0.6, iterations=20)

    colors = nx.get_edge_attributes(G,'color').values()
    weights = nx.get_edge_attributes(G,'weight').values()

    nx.draw(G,pos,ax=a,with_labels=True,  node_shape="s", edge_color=colors, width=list(weights), node_color="none", connectionstyle='arc3, rad = 0.1', bbox=dict(facecolor="orange", boxstyle='round,pad=0.2'))
    canvas.draw()
    bfstime.config(text = "BFS Search Time: " + BFS + 's')
    iddfstime.config(text = "IDDFS Search Time: " + DFS + 's')
    bar.stop()
    bar.config(value=0, maximum=0)
    b1["state"] = "enable"
    root.update_idletasks()
    print("Vis Updated")

# Set up
origin = ''
destination = ''
l = []
trie = Trie()
#loads trie with vals from csv
loadVals()

#geometry for window. loop starts here
root = ThemedTk(theme="radiance")
root.title('WikiSolver3000')
root.geometry('830x880+550+60')

#label1
label = ttk.Label(root, text="Select Origin:")
label.grid(row = 0, column = 0, columnspan = 1)

#label2
label2 = ttk.Label(root, text="Select Destination:")
label2.grid(row = 0, column = 1, columnspan = 1)

#label BFS
bfstime = ttk.Label(root, text="BFS Search Time: 0")
bfstime.grid(row = 4, column = 0, columnspan = 1, pady = (5,5))

#label IDDFS
iddfstime = ttk.Label(root, text="IDDFS Search Time: 0")
iddfstime.grid(row = 4, column = 1, columnspan = 1, pady = (5,5))

#TBox1 
e = ttk.Entry(root, width = 50)
e.grid(row=1, column=0, padx = 5)
e.bind('<KeyRelease>', checkkey)

#TBox2 
e2 = ttk.Entry(root, width = 50)
e2.grid(row=1, column=1, padx = 5)
e2.bind('<KeyRelease>', checkkey2)

#ListBox1
lb = Listbox(root, width = 50,exportselection=False)
lb.grid(row=2, column=0, padx = 5)
update(l)
lb.bind('<<ListboxSelect>>', change)

#ListBox2
lb2 = Listbox(root, width = 50,exportselection=False)
lb2.grid(row=2, column=1, padx = 5)
update2(l)
lb2.bind('<<ListboxSelect>>', change2)

#progbar
bar = ttk.Progressbar(root, orient='horizontal', mode = 'indeterminate', length = 600)
bar.config(value=0, maximum=0)
bar.grid(row = 5, column = 0, columnspan = 2, pady = (5,10))

#SearchButton
b1 = ttk.Button(root, width = 20, text='Search Connection', command=intermediate)
b1.grid(row = 3, column = 0, columnspan =2, pady = (10,5))

#frame for graphplot
frame1 = Frame(root, width=600,height=400, bg = 'black')
frame1.grid(row = 6, column = 0, columnspan = 2)

#plot
f = Figure(figsize=(8, 5), dpi=100)
a = f.add_subplot(111)

#networkx
G=nx.DiGraph()
G.add_edge('Origin', 'Destination')
pos=nx.spring_layout(G)
nx.draw(G,pos,ax=a,with_labels=True,  node_shape="s",  node_color="none", connectionstyle='arc3, rad = 0.1', bbox=dict(facecolor="orange", edgecolor='black', boxstyle='round,pad=0.2'))

#canvas that packs inside frame
canvas = FigureCanvasTkAgg(f, master=frame1)
canvas.draw()
canvas.get_tk_widget().pack(side="top",fill='both',expand=True, padx = 0, pady = 0)

#toolbar
toolbar = NavigationToolbar2Tk(canvas, frame1)
toolbar.update()
canvas._tkcanvas.pack()

#mainloop
root.mainloop()